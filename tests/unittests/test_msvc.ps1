# Build and Test Script for MSVC (Windows)
# Based on .github/workflows/msbuild.yml

param(
  [string]$Configuration = "Debug",
  [string]$Platform = "x64"
)

# Find vswhere.exe
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
  Write-Error "vswhere.exe not found at $vswhere. Make sure Visual Studio Installer is installed."
  return
}

Write-Host "Searching for MSBuild.exe and vstest.console.exe..." -ForegroundColor Cyan

# Find MSBuild
$msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe | Select-Object -First 1
if (-not $msbuild) {
  Write-Error "MSBuild.exe not found. Make sure MSBuild is installed."
  return
}
Write-Host "Found MSBuild: $msbuild" -ForegroundColor Green

# Find VSTest.console
$vstest = & $vswhere -latest -products * -find **\vstest.console.exe | Select-Object -First 1
if (-not $vstest) {
  Write-Error "vstest.console.exe not found. Make sure you have the 'Desktop development with C++' workload."
  return
}
Write-Host "Found VSTest: $vstest" -ForegroundColor Green

# Find NuGet
$nuget = Get-Command nuget -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source
if (-not $nuget) {
  if (Test-Path ".\nuget.exe") {
    $nuget = (Get-Item ".\nuget.exe").FullName
  }
  else {
    Write-Host "nuget.exe not found. Downloading to current folder..." -ForegroundColor Yellow
    Invoke-WebRequest -Uri "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" -OutFile ".\nuget.exe"
    $nuget = (Get-Item ".\nuget.exe").FullName
  }
}
Write-Host "Using NuGet: $nuget" -ForegroundColor Green

# 1. Restore
Write-Host "`nStep 1: Restoring NuGet packages..." -ForegroundColor Cyan
& $nuget restore ./tests/unittests/unittests.sln

# 2. Build
Write-Host "`nStep 2: Building project ($Configuration|$Platform)..." -ForegroundColor Cyan
& $msbuild /m /p:Configuration=$Configuration ./tests/unittests/unittests.sln /p:Platform=$Platform /p:PreferredToolArchitecture=x64

if ($LASTEXITCODE -ne 0) {
  Write-Error "Build failed!"
  return
}

# 3. Test
Write-Host "`nStep 3: Running tests..." -ForegroundColor Cyan
# Path from msbuild.yml: ./tests/unittests/!bin/${matrix.targetplatform}/${env.BUILD_CONFIGURATION}/unittests.dll
# Fix for Win32 vs x86 (msbuild uses Win32 for x86 platform in this solution)
$platformPath = if ($Platform -eq "x86") { "Win32" } else { $Platform }
$testDll = "./tests/unittests/!bin/$Platform/$Configuration/unittests.dll"

if (-not (Test-Path $testDll)) {
  # Check alternative path if msbuild output differs
  $testDll = "./tests/unittests/$platformPath/$Configuration/unittests.dll"
}

if (Test-Path $testDll) {
  Write-Host "Running tests in: $testDll" -ForegroundColor Yellow
  & $vstest /Platform:$Platform /logger:"console;verbosity=detailed" $testDll
}
else {
  Write-Error "Test DLL not found at expected locations! Checked: ./tests/unittests/!bin/$Platform/$Configuration/unittests.dll"
  return
}

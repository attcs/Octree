# Build and Test Script for GCC (MinGW/MSYS2)
# Based on .github/workflows/msbuild.yml

param(
    [string]$BuildDir = "build_gcc"
)

$cmakeFilePath = "./tests/gcc"

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

$cmake = Get-Command cmake -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source
if (-not $cmake) {
    Write-Error "cmake not found in PATH."
    return
}

# Check for make or mingw32-make
$make = Get-Command mingw32-make, make -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty Name
if (-not $make) {
    Write-Error "make or mingw32-make not found in PATH."
    return
}

Write-Host "Configuring with CMake (Generator: MinGW Makefiles)..." -ForegroundColor Cyan
& $cmake $cmakeFilePath -B $BuildDir -G "MinGW Makefiles"

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    return
}

Write-Host "Building with $make..." -ForegroundColor Cyan
& $make -C $BuildDir

if ($LASTEXITCODE -eq 0) {
    Write-Host "Running tests..." -ForegroundColor Green
    $executable = Join-Path $BuildDir "octree.exe"
    if (Test-Path $executable) {
        & $executable
    } else {
        $executable = Join-Path $BuildDir "octree"
        if (Test-Path $executable) {
            & $executable
        } else {
            Write-Error "Octree executable not found in $BuildDir"
        }
    }
} else {
    Write-Error "Build failed."
}

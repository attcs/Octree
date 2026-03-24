$VCPKG_PATH = "G:\!Development\vcpkg"
$TOOLCHAIN = "$VCPKG_PATH\scripts\buildsystems\vcpkg.cmake"

if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path "build"
}

Write-Host "Configuring Serialization External Test..." -ForegroundColor Green
cmake -S . -B build "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN"

Write-Host "Building Project..." -ForegroundColor Green
cmake --build build --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "Running Serialization External Test..." -ForegroundColor Green
    ./build/Release/serialization_test.exe
} else {
    Write-Host "Build Failed!" -ForegroundColor Red
}

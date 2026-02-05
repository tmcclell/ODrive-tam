# ODrive Development Environment Setup
# Run this script at the start of each session: . .\setup-env.ps1

$TupPath = "C:\Users\gabhardwaj\Downloads\tup-latest"
$ArmGccPath = "C:\Program Files\Arm\GNU Toolchain mingw-w64-x86_64-arm-none-eabi\bin"

# Add tools to PATH if not already present
if ($env:PATH -notlike "*$TupPath*") {
    $env:PATH = "$TupPath;$env:PATH"
    Write-Host "✓ Added tup to PATH" -ForegroundColor Green
}

if ($env:PATH -notlike "*$ArmGccPath*") {
    $env:PATH = "$ArmGccPath;$env:PATH"
    Write-Host "✓ Added ARM GCC to PATH" -ForegroundColor Green
}

# Set TUP_PATH for the build script
$env:TUP_PATH = "$TupPath\tup.exe"

Write-Host ""
Write-Host "ODrive build environment ready!" -ForegroundColor Cyan
Write-Host "  tup:     $(Get-Command tup -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source)"
Write-Host "  arm-gcc: $(Get-Command arm-none-eabi-gcc -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source)"
Write-Host ""
Write-Host "Build firmware with:" -ForegroundColor Yellow
Write-Host "  python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V"

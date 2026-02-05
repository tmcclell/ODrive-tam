# C++ Testing Setup Script for ODrive
# This script sets up and runs tests using the tup build system

param(
    [switch]$EnableTests,
    [switch]$BuildTests,
    [switch]$RunTests,
    [string]$Filter = "",
    [switch]$Verbose,
    [switch]$ShowConfig
)

$ErrorActionPreference = "Stop"
$FirmwarePath = "Firmware"
$TestsPath = "$FirmwarePath/Tests"
$ConfigFile = "$FirmwarePath/tup.config"

function Enable-Tests {
    Write-Host "📦 Enabling doctest in tup.config..." -ForegroundColor Cyan
    
    # Check if tup.config exists
    if (Test-Path $ConfigFile) {
        $content = Get-Content $ConfigFile -Raw
        if ($content -match "CONFIG_DOCTEST") {
            Write-Host "✅ CONFIG_DOCTEST already in tup.config" -ForegroundColor Green
            return
        }
        # Append to existing config
        Add-Content -Path $ConfigFile -Value "`nCONFIG_DOCTEST=true"
    }
    else {
        # Create new config with doctest enabled
        @"
# ODrive Test Configuration
# Enable unit tests with doctest framework
CONFIG_DOCTEST=true

# Set board version (required for build)
# CONFIG_BOARD_VERSION=v3.6-56V
"@ | Set-Content -Path $ConfigFile
    }
    
    Write-Host "✅ Enabled CONFIG_DOCTEST=true in $ConfigFile" -ForegroundColor Green
    Write-Host "⚠️  Note: You may need to set CONFIG_BOARD_VERSION as well" -ForegroundColor Yellow
}

function Show-Config {
    Write-Host "📋 Current tup.config:" -ForegroundColor Cyan
    
    if (Test-Path $ConfigFile) {
        Get-Content $ConfigFile
    }
    else {
        Write-Host "  (no tup.config file exists)" -ForegroundColor Yellow
    }
}

function Build-Tests {
    Write-Host "🔨 Building tests with tup..." -ForegroundColor Cyan
    
    # Ensure we're in Firmware directory
    Push-Location $FirmwarePath
    
    try {
        # Check if tup is available
        $tupPath = Get-Command tup -ErrorAction SilentlyContinue
        if (-not $tupPath) {
            Write-Host "❌ tup not found in PATH" -ForegroundColor Red
            Write-Host "   Install tup from: http://gittup.org/tup/" -ForegroundColor Yellow
            return $false
        }
        
        Write-Host "Running: tup"
        & tup
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✅ Build successful" -ForegroundColor Green
            return $true
        }
        else {
            Write-Host "❌ Build failed (exit code: $LASTEXITCODE)" -ForegroundColor Red
            return $false
        }
    }
    finally {
        Pop-Location
    }
}

function Invoke-Tests {
    Write-Host "🧪 Running tests..." -ForegroundColor Cyan
    
    $testExe = "$FirmwarePath/Tests/test_runner.exe"
    
    if (-not (Test-Path $testExe)) {
        Write-Host "❌ $testExe not found" -ForegroundColor Red
        Write-Host "   Run with -EnableTests -BuildTests first" -ForegroundColor Yellow
        return
    }
    
    $args = @()
    
    if ($Filter) {
        $args += "--test-case=`"$Filter`""
        Write-Host "Filter: $Filter"
    }
    
    if ($Verbose) {
        $args += "-s"
    }
    
    Write-Host ""
    & $testExe @args
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n✅ All tests passed!" -ForegroundColor Green
    }
    else {
        Write-Host "`n❌ Some tests failed (exit code: $LASTEXITCODE)" -ForegroundColor Red
    }
}

function Get-TestFiles {
    Write-Host "📁 Test files in $TestsPath :" -ForegroundColor Cyan
    
    $testFiles = Get-ChildItem -Path $TestsPath -Filter "test_*.cpp" -ErrorAction SilentlyContinue
    
    if ($testFiles) {
        $testFiles | ForEach-Object { Write-Host "  - $($_.Name)" }
        Write-Host "`n  Total: $($testFiles.Count) test files" -ForegroundColor Gray
    }
    else {
        Write-Host "  No test files found" -ForegroundColor Yellow
    }
}

# Main execution
Write-Host ""
Write-Host "🧪 ODrive C++ Test Setup (tup-based)" -ForegroundColor Magenta
Write-Host "=====================================" -ForegroundColor Magenta
Write-Host ""

if ($ShowConfig) { Show-Config; Write-Host "" }
if ($EnableTests) { Enable-Tests; Write-Host "" }
if ($BuildTests) { Build-Tests; Write-Host "" }
if ($RunTests) { Invoke-Tests; Write-Host "" }

# If no flags specified, show usage
if (-not ($EnableTests -or $BuildTests -or $RunTests -or $ShowConfig)) {
    Get-TestFiles
    Write-Host ""
    Write-Host "Usage:" -ForegroundColor Yellow
    Write-Host "  .\setup-tests.ps1 -ShowConfig        # Show current tup.config"
    Write-Host "  .\setup-tests.ps1 -EnableTests       # Add CONFIG_DOCTEST=true to tup.config"
    Write-Host "  .\setup-tests.ps1 -BuildTests        # Run tup to build firmware + tests"
    Write-Host "  .\setup-tests.ps1 -RunTests          # Run test executable"
    Write-Host "  .\setup-tests.ps1 -RunTests -Filter 'Motor*'  # Run filtered tests"
    Write-Host "  .\setup-tests.ps1 -RunTests -Verbose # Verbose output"
    Write-Host ""
    Write-Host "Quick start:" -ForegroundColor Cyan
    Write-Host "  .\setup-tests.ps1 -EnableTests -BuildTests -RunTests"
    Write-Host ""
    Write-Host "Prerequisites:" -ForegroundColor Yellow
    Write-Host "  - tup (http://gittup.org/tup/)"
    Write-Host "  - ARM GCC toolchain (for firmware build)"
    Write-Host "  - g++ (for host-side tests)"
}

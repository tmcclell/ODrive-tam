#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Compile and run C++ unit tests with formatted report output.

.DESCRIPTION
    This script compiles the test runner using g++ and executes it,
    providing a formatted pass/fail report.

.PARAMETER TestPattern
    Filter tests by name pattern (e.g., "*Motor*")

.PARAMETER Verbose
    Show all assertions, not just failures

.PARAMETER ListOnly
    Only list available test cases without running

.PARAMETER Compile
    Force recompilation even if executable exists

.EXAMPLE
    .\run-tests.ps1
    # Run all tests

.EXAMPLE
    .\run-tests.ps1 -TestPattern "*Encoder*"
    # Run only Encoder tests

.EXAMPLE
    .\run-tests.ps1 -Verbose
    # Run with verbose output showing all assertions

.EXAMPLE
    .\run-tests.ps1 -ListOnly
    # List available test cases
#>

param(
    [string]$TestPattern = "",
    [switch]$Verbose,
    [switch]$ListOnly,
    [switch]$Compile
)

$ErrorActionPreference = "Stop"

# Configuration
$FirmwarePath = "C:\src\GHCPC++\src-ODrive\Firmware"
$TestRunnerPath = "$FirmwarePath\Tests\test_runner.exe"
$MinGWPath = "C:\mingw64\bin"

# Ensure g++ is in PATH
if (-not ($env:PATH -like "*$MinGWPath*")) {
    $env:PATH = "$MinGWPath;$env:PATH"
}

# Check for g++
if (-not (Get-Command "g++" -ErrorAction SilentlyContinue)) {
    Write-Host "❌ ERROR: g++ not found in PATH" -ForegroundColor Red
    Write-Host "   Install MinGW-W64 from https://winlibs.com/" -ForegroundColor Yellow
    Write-Host "   Expected at: $MinGWPath" -ForegroundColor Yellow
    exit 1
}

Push-Location $FirmwarePath
try {
    # ═══════════════════════════════════════════════════════════════════════════
    # COMPILE
    # ═══════════════════════════════════════════════════════════════════════════
    
    $needsCompile = $Compile -or (-not (Test-Path $TestRunnerPath))
    
    # Check if source files are newer than executable
    if (-not $needsCompile -and (Test-Path $TestRunnerPath)) {
        $exeTime = (Get-Item $TestRunnerPath).LastWriteTime
        $testFiles = Get-ChildItem "Tests\*.cpp" -ErrorAction SilentlyContinue
        foreach ($file in $testFiles) {
            if ($file.LastWriteTime -gt $exeTime) {
                $needsCompile = $true
                break
            }
        }
    }
    
    if ($needsCompile) {
        Write-Host ""
        Write-Host "╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
        Write-Host "║  🔨 COMPILING TESTS                                           ║" -ForegroundColor Cyan
        Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
        Write-Host ""
        
        $compileCmd = "g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp"
        Write-Host "  > $compileCmd" -ForegroundColor Gray
        
        $compileOutput = & g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp 2>&1
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "  ❌ COMPILATION FAILED!" -ForegroundColor Red
            Write-Host ""
            $compileOutput | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
            exit 1
        }
        
        Write-Host "  ✅ Compilation successful" -ForegroundColor Green
    }
    
    # ═══════════════════════════════════════════════════════════════════════════
    # RUN TESTS
    # ═══════════════════════════════════════════════════════════════════════════
    
    Write-Host ""
    Write-Host "╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║  🧪 RUNNING TESTS                                             ║" -ForegroundColor Cyan
    Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
    
    # Build arguments
    $testArgs = @()
    if ($ListOnly) { 
        $testArgs += "--list-test-cases"
    }
    if ($Verbose) { 
        $testArgs += "-s"
    }
    if ($TestPattern -ne "") { 
        $testArgs += "--test-case=`"$TestPattern`""
    }
    
    $argString = if ($testArgs.Count -gt 0) { " " + ($testArgs -join " ") } else { "" }
    Write-Host "  > .\Tests\test_runner.exe$argString" -ForegroundColor Gray
    Write-Host ""
    
    # Run tests and capture output
    $output = & .\Tests\test_runner.exe @testArgs 2>&1
    $exitCode = $LASTEXITCODE
    
    # ═══════════════════════════════════════════════════════════════════════════
    # PARSE RESULTS
    # ═══════════════════════════════════════════════════════════════════════════
    
    $testCases = @{ total = 0; passed = 0; failed = 0; skipped = 0 }
    $assertions = @{ total = 0; passed = 0; failed = 0 }
    $status = "UNKNOWN"
    $failedTests = @()
    
    foreach ($line in $output) {
        # Parse test cases line
        if ($line -match '\[doctest\] test cases:\s*(\d+)\s*\|\s*(\d+) passed\s*\|\s*(\d+) failed\s*\|\s*(\d+) skipped') {
            $testCases.total = [int]$Matches[1]
            $testCases.passed = [int]$Matches[2]
            $testCases.failed = [int]$Matches[3]
            $testCases.skipped = [int]$Matches[4]
        }
        
        # Parse assertions line
        if ($line -match '\[doctest\] assertions:\s*(\d+)\s*\|\s*(\d+) passed\s*\|\s*(\d+) failed') {
            $assertions.total = [int]$Matches[1]
            $assertions.passed = [int]$Matches[2]
            $assertions.failed = [int]$Matches[3]
        }
        
        # Parse status
        if ($line -match '\[doctest\] Status: (SUCCESS|FAILURE)') {
            $status = $Matches[1]
        }
        
        # Capture failure details (only actual failures, not summary lines)
        if ($line -match 'FAILED' -and $line -notmatch '\[doctest\]') {
            $failedTests += $line
        }
    }
    
    # ═══════════════════════════════════════════════════════════════════════════
    # DISPLAY REPORT
    # ═══════════════════════════════════════════════════════════════════════════
    
    Write-Host ""
    Write-Host "╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║                    TEST EXECUTION REPORT                      ║" -ForegroundColor Cyan
    Write-Host "╠══════════════════════════════════════════════════════════════╣" -ForegroundColor Cyan
    
    if ($status -eq "SUCCESS") {
        Write-Host "║  Status:      ✅ SUCCESS                                      ║" -ForegroundColor Green
    } elseif ($status -eq "FAILURE") {
        Write-Host "║  Status:      ❌ FAILURE                                      ║" -ForegroundColor Red
    } else {
        Write-Host "║  Status:      ⚠️  UNKNOWN                                      ║" -ForegroundColor Yellow
    }
    
    Write-Host ("║  Test Cases:  {0} passed | {1} failed | {2} skipped               ║" -f $testCases.passed, $testCases.failed, $testCases.skipped)
    Write-Host ("║  Assertions:  {0} passed | {1} failed                              ║" -f $assertions.passed, $assertions.failed)
    Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    
    # Show failures if any
    if ($failedTests.Count -gt 0) {
        Write-Host ""
        Write-Host "FAILED TEST DETAILS:" -ForegroundColor Red
        Write-Host "────────────────────" -ForegroundColor Red
        foreach ($failure in $failedTests) {
            Write-Host "  $failure" -ForegroundColor Red
        }
    }
    
    # Show raw output if verbose or on failure
    if ($Verbose -or $exitCode -ne 0) {
        Write-Host ""
        Write-Host "RAW OUTPUT:" -ForegroundColor Gray
        Write-Host "───────────" -ForegroundColor Gray
        foreach ($line in $output) {
            Write-Host "  $line"
        }
    }
    
    Write-Host ""
    
    exit $exitCode
}
finally {
    Pop-Location
}

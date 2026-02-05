Write-Host "Running repository tests..."

if (-not (Get-Command python -ErrorAction SilentlyContinue) -and -not (Get-Command python3 -ErrorAction SilentlyContinue)) {
    Write-Error "Python not found. Please install Python 3 to run tests."
    exit 1
}

$py = if (Get-Command python3 -ErrorAction SilentlyContinue) { 'python3' } else { 'python' }

if (Test-Path -Path tools/run_tests.py) {
    & $py tools/run_tests.py
} else {
    Write-Error "No test runner found at tools/run_tests.py. You may need to run tests manually or adjust this script."
    exit 1
}

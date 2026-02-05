# C++ Testing Skill

> Expert in test-driven development for embedded C++ using doctest framework

## Description

This skill provides comprehensive support for C++ unit testing in embedded systems. It helps set up testing infrastructure, generate tests following TDD principles, compile and run tests, and report pass/fail results with detailed diagnostics.

## Capabilities

1. **Setup Testing Infrastructure**
   - Configure doctest framework
   - Create test runner
   - Set up build integration

2. **Generate Unit Tests**
   - Create test files from source files
   - Generate test cases for all public methods
   - Cover happy path, edge cases, and error conditions

3. **Compile and Run Tests**
   - Build tests with g++ (MinGW-W64)
   - Execute test runner
   - Capture and parse output

4. **Report Test Results**
   - Show pass/fail summary
   - List failing tests with details
   - Provide fix suggestions

5. **TDD Workflow Support**
   - Write failing tests first (Red)
   - Implement minimal code to pass (Green)
   - Refactor with confidence (Refactor)

6. **Mock Hardware Dependencies**
   - Create mock classes for HAL
   - Simulate ISR contexts
   - Fake timers and peripherals

## Quick Start Commands

### Compile Tests
```powershell
cd C:\src\GHCPC++\src-ODrive\Firmware
$env:PATH = "C:\mingw64\bin;$env:PATH"
g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp
```

### Run All Tests
```powershell
.\Tests\test_runner.exe
```

### Run Specific Tests
```powershell
# By test case name pattern
.\Tests\test_runner.exe --test-case="*Motor*"

# By test file
.\Tests\test_runner.exe --source-file="*test_encoder*"
```

### Verbose Output
```powershell
.\Tests\test_runner.exe -s  # Show all assertions
.\Tests\test_runner.exe -d  # Show duration
```

### List Available Tests
```powershell
.\Tests\test_runner.exe --list-test-cases
```

## When This Skill Applies

Automatically activated when prompts mention:
- "test", "unit test", "TEST_CASE"
- "doctest", "gtest", "googletest"
- "TDD", "test-driven"
- "coverage", "mock", "fixture"
- "assert", "CHECK", "REQUIRE"

## Framework: doctest

### Why doctest for Embedded?
- **Header-only** - Single file include
- **Fast compilation** - Minimal compile-time overhead
- **No exceptions required** - Works with `-fno-exceptions`
- **Minimal footprint** - Small binary size
- **Subcases** - Efficient test organization

### Basic Patterns

```cpp
#include <doctest.h>

// Simple test case
TEST_CASE("Motor::set_current - clamps to limit") {
    Motor motor;
    motor.config_.current_lim = 10.0f;
    
    motor.set_current(15.0f);  // Exceeds limit
    
    CHECK_EQ(motor.current_setpoint_, 10.0f);  // Clamped
}

// Subcases share setup code
TEST_CASE("Encoder::get_position") {
    Encoder enc;
    enc.config_.cpr = 4096;
    
    SUBCASE("zero position") {
        enc.raw_count_ = 0;
        CHECK_EQ(enc.get_position(), 0.0f);
    }
    
    SUBCASE("one revolution") {
        enc.raw_count_ = 4096;
        CHECK_EQ(enc.get_position(), doctest::Approx(1.0f));
    }
    
    SUBCASE("negative position") {
        enc.raw_count_ = -2048;
        CHECK_EQ(enc.get_position(), doctest::Approx(-0.5f));
    }
}

// Test with fixture
TEST_CASE_FIXTURE(MotorFixture, "calibration sequence") {
    run_calibration();
    CHECK(motor_.is_calibrated_);
    CHECK_EQ(motor_.error_, ERROR_NONE);
}
```

### Assertion Macros

| Macro | Purpose |
|-------|---------|
| `CHECK(expr)` | Continue on failure |
| `REQUIRE(expr)` | Stop test on failure |
| `CHECK_EQ(a, b)` | Equality check |
| `CHECK_NE(a, b)` | Inequality check |
| `CHECK_LT(a, b)` | Less than |
| `CHECK_GT(a, b)` | Greater than |
| `CHECK_THROWS(expr)` | Exception expected (if enabled) |
| `CHECK_NOTHROW(expr)` | No exception |
| `CHECK_FALSE(expr)` | Boolean false |
| `WARN(expr)` | Non-fatal warning |

### Floating Point Comparisons

```cpp
// Use doctest::Approx for floating point
CHECK_EQ(result, doctest::Approx(3.14159f).epsilon(0.0001f));

// Or with custom tolerance
CHECK(std::abs(result - expected) < 1e-6f);
```

## Embedded Testing Patterns

### 1. Mocking Hardware

```cpp
// Mock GPIO for testing
class MockGpio : public GpioInterface {
public:
    bool state_ = false;
    int read_count_ = 0;
    
    bool read() override { 
        read_count_++;
        return state_; 
    }
    
    void write(bool val) override { 
        state_ = val; 
    }
};

TEST_CASE("LedController uses GPIO correctly") {
    MockGpio gpio;
    LedController led(gpio);
    
    led.turn_on();
    
    CHECK(gpio.state_);
    CHECK_EQ(gpio.read_count_, 0);  // Didn't read unnecessarily
}
```

### 2. Testing State Machines

```cpp
TEST_CASE("Axis state machine transitions") {
    Axis axis;
    
    SUBCASE("IDLE -> MOTOR_CALIBRATION") {
        axis.current_state_ = AXIS_STATE_IDLE;
        axis.requested_state_ = AXIS_STATE_MOTOR_CALIBRATION;
        
        axis.run_state_machine_loop_iteration();
        
        CHECK_EQ(axis.current_state_, AXIS_STATE_MOTOR_CALIBRATION);
    }
    
    SUBCASE("error during calibration returns to IDLE") {
        axis.current_state_ = AXIS_STATE_MOTOR_CALIBRATION;
        axis.motor_.error_ = ERROR_PHASE_RESISTANCE_OUT_OF_RANGE;
        
        axis.run_state_machine_loop_iteration();
        
        CHECK_EQ(axis.current_state_, AXIS_STATE_IDLE);
        CHECK_NE(axis.error_, ERROR_NONE);
    }
}
```

### 3. Testing ISR-Safe Code

```cpp
// Simulate ISR context with critical section tracking
static volatile bool in_critical_section = false;

void mock_enter_critical() { in_critical_section = true; }
void mock_exit_critical() { in_critical_section = false; }

TEST_CASE("RingBuffer::write is ISR-safe") {
    RingBuffer<uint8_t, 16> buf;
    
    // Simulate ISR writing
    mock_enter_critical();
    bool result = buf.write(0x42);
    mock_exit_critical();
    
    CHECK(result);
    CHECK_FALSE(in_critical_section);  // Exited properly
}
```

### 4. Testing Error Codes

```cpp
TEST_CASE("Encoder::run_offset_calibration error handling") {
    Encoder enc;
    
    SUBCASE("returns TIMEOUT on no response") {
        enc.config_.timeout_ms = 0;  // Immediate timeout
        
        auto result = enc.run_offset_calibration();
        
        CHECK_EQ(result, ErrorCode::TIMEOUT);
    }
    
    SUBCASE("returns HARDWARE_ERROR on invalid reading") {
        enc.simulate_invalid_reading_ = true;
        
        auto result = enc.run_offset_calibration();
        
        CHECK_EQ(result, ErrorCode::HARDWARE_ERROR);
    }
}
```

## Test File Template

```cpp
/**
 * @file test_<module>.cpp
 * @brief Unit tests for <Module> class
 * 
 * Tests cover:
 * - Normal operation
 * - Boundary conditions
 * - Error handling
 * - State transitions
 * 
 * @author <Your Name>
 * @date <Date>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "MotorControl/<module>.hpp"

// ============================================================================
// Test Fixtures
// ============================================================================

class ModuleFixture {
protected:
    Module module_;
    MockDependency mock_dep_;
    
    ModuleFixture() {
        // Common setup
        module_.init(&mock_dep_);
    }
};

// ============================================================================
// Constructor Tests
// ============================================================================

TEST_CASE("Module constructor initializes defaults") {
    Module m;
    CHECK_EQ(m.state_, State::IDLE);
    CHECK_EQ(m.error_, ERROR_NONE);
}

// ============================================================================
// Normal Operation Tests
// ============================================================================

TEST_CASE("Module::process - normal operation") {
    // Tests here
}

// ============================================================================
// Boundary Tests
// ============================================================================

TEST_CASE("Module::process - boundary values") {
    SUBCASE("minimum input") { /* ... */ }
    SUBCASE("maximum input") { /* ... */ }
    SUBCASE("zero input") { /* ... */ }
}

// ============================================================================
// Error Handling Tests
// ============================================================================

TEST_CASE("Module::process - error conditions") {
    SUBCASE("invalid input returns error") { /* ... */ }
    SUBCASE("timeout handling") { /* ... */ }
}

// ============================================================================
// Integration Tests
// ============================================================================

TEST_CASE("Module integrates with dependencies") {
    // Tests here
}
```

## Setup Scripts

### Install doctest (if not present)

```powershell
# Download doctest header
$doctestUrl = "https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h"
$doctestPath = "Firmware/doctest/doctest.h"

if (-not (Test-Path $doctestPath)) {
    New-Item -ItemType Directory -Force -Path (Split-Path $doctestPath)
    Invoke-WebRequest -Uri $doctestUrl -OutFile $doctestPath
    Write-Host "Downloaded doctest.h to $doctestPath"
}
```

### Create Test Runner

```cpp
// test_runner.cpp - Compile and run all tests
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

// Link all test files together
// Build: g++ -std=c++17 -I. test_runner.cpp test_*.cpp -o run_tests
```

### Build Command

```powershell
# Compile tests (Windows with MinGW)
g++ -std=c++17 -I Firmware -I Firmware/doctest `
    Firmware/Tests/test_runner.cpp `
    Firmware/Tests/test_*.cpp `
    -o run_tests.exe

# Run tests
./run_tests.exe

# Run with filters
./run_tests.exe --test-case="Motor*"

# Run with verbose output
./run_tests.exe -s
```

## TDD Workflow

### Red-Green-Refactor Cycle

```
┌─────────────────────────────────────────────────────────────┐
│                         RED                                  │
│  1. Write a failing test for the new feature                │
│  2. Run tests - confirm it fails                            │
│  3. Commit the test (optional)                              │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                        GREEN                                 │
│  1. Write minimal code to make the test pass                │
│  2. Run tests - confirm all pass                            │
│  3. Commit the implementation                               │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│                       REFACTOR                               │
│  1. Improve code quality (naming, structure, patterns)      │
│  2. Run tests - confirm still passing                       │
│  3. Commit the refactoring                                  │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
                    [Next Feature]
```

### Example TDD Session

```
User: I need to add a velocity limit to the motor controller

Step 1 - RED (Write failing test):
```cpp
TEST_CASE("Motor::set_velocity - clamps to configured limit") {
    Motor motor;
    motor.config_.vel_limit = 100.0f;
    
    motor.set_velocity(150.0f);  // Exceeds limit
    
    CHECK_EQ(motor.vel_setpoint_, 100.0f);  // Should clamp
}
```

Step 2 - GREEN (Minimal implementation):
```cpp
void Motor::set_velocity(float vel) {
    vel_setpoint_ = std::clamp(vel, -config_.vel_limit, config_.vel_limit);
}
```

Step 3 - REFACTOR (Add edge cases):
```cpp
TEST_CASE("Motor::set_velocity - edge cases") {
    Motor motor;
    motor.config_.vel_limit = 100.0f;
    
    SUBCASE("negative velocity clamped") {
        motor.set_velocity(-150.0f);
        CHECK_EQ(motor.vel_setpoint_, -100.0f);
    }
    
    SUBCASE("within limit unchanged") {
        motor.set_velocity(50.0f);
        CHECK_EQ(motor.vel_setpoint_, 50.0f);
    }
}
```
```

## Related Resources

- [doctest GitHub](https://github.com/doctest/doctest)
- [doctest Tutorial](https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md)
- [Embedded Testing Best Practices](https://embedded.fm/blog/2017/7/26/unit-testing-for-embedded-systems)

## Test Execution & Reporting

### Complete Test Workflow

When generating tests for selected code, follow this workflow:

```
┌─────────────────────────────────────────────────────────────┐
│  1. ANALYZE - Understand the code to test                   │
│     - Read selected code/file                               │
│     - Identify public interface                             │
│     - List edge cases and error conditions                  │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  2. GENERATE - Create test file                             │
│     - Create Tests/test_<module>.cpp                        │
│     - Include doctest.h and target header                   │
│     - Write TEST_CASE and SUBCASE blocks                    │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  3. COMPILE - Build test executable                         │
│     cd Firmware                                             │
│     $env:PATH = "C:\mingw64\bin;$env:PATH"                  │
│     g++ -O3 -std=c++17 -I. -Idoctest \                      │
│         -o Tests/test_runner.exe Tests/test_runner.cpp      │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  4. RUN - Execute tests                                     │
│     .\Tests\test_runner.exe                                 │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  5. REPORT - Show results                                   │
│     - Parse doctest output                                  │
│     - Show pass/fail summary                                │
│     - List failing tests with details                       │
│     - Suggest fixes for failures                            │
└─────────────────────────────────────────────────────────────┘
```

### Report Format

After running tests, provide a formatted report:

```
╔══════════════════════════════════════════════════════════════╗
║                    TEST EXECUTION REPORT                      ║
╠══════════════════════════════════════════════════════════════╣
║ Status:      ✅ SUCCESS  /  ❌ FAILURE                        ║
║ Test Cases:  8 passed | 0 failed | 0 skipped                 ║
║ Assertions:  44 passed | 0 failed                             ║
║ Duration:    0.023s                                           ║
╠══════════════════════════════════════════════════════════════╣
║ PASSED TESTS:                                                 ║
║ ├─ ✅ Motor::set_current - normal operation                  ║
║ ├─ ✅ Motor::set_current - clamps to limit                   ║
║ ├─ ✅ Encoder::get_position - zero position                  ║
║ └─ ✅ Encoder::get_position - one revolution                 ║
╚══════════════════════════════════════════════════════════════╝
```

### Failure Report Format

```
╔══════════════════════════════════════════════════════════════╗
║                    TEST EXECUTION REPORT                      ║
╠══════════════════════════════════════════════════════════════╣
║ Status:      ❌ FAILURE                                       ║
║ Test Cases:  7 passed | 1 failed | 0 skipped                 ║
║ Assertions:  43 passed | 1 failed                             ║
╠══════════════════════════════════════════════════════════════╣
║ FAILED TESTS:                                                 ║
║ ├─ ❌ Motor::set_velocity - clamps to limit                  ║
║ │   File: test_motor.cpp:45                                   ║
║ │   CHECK_EQ( motor.vel_setpoint_, 100.0f ) FAILED           ║
║ │   Expected: 100.0                                           ║
║ │   Actual:   150.0                                           ║
║ │                                                             ║
║ │   💡 SUGGESTION: The set_velocity() method is not          ║
║ │      clamping the value. Add std::clamp() call:            ║
║ │      vel_setpoint_ = std::clamp(vel, -vel_limit, vel_limit)║
║ └───────────────────────────────────────────────────────────  ║
╚══════════════════════════════════════════════════════════════╝
```

### Parsing doctest Output

doctest outputs in this format:
```
[doctest] doctest version is "2.3.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:      8 |      8 passed |      0 failed |      0 skipped
[doctest] assertions:     44 |     44 passed |      0 failed |
[doctest] Status: SUCCESS!
```

Parse the key values:
- `test cases: X | Y passed | Z failed | W skipped`
- `assertions: A | B passed | C failed`
- `Status: SUCCESS!` or `Status: FAILURE`

### PowerShell Test Runner Script

```powershell
# run-tests.ps1 - Complete test workflow
param(
    [string]$TestPattern = "*",
    [switch]$Verbose,
    [switch]$ListOnly
)

$ErrorActionPreference = "Stop"
$FirmwarePath = "C:\src\GHCPC++\src-ODrive\Firmware"

# Ensure g++ is in PATH
$env:PATH = "C:\mingw64\bin;$env:PATH"

Push-Location $FirmwarePath
try {
    # Compile
    Write-Host "🔨 Compiling tests..." -ForegroundColor Cyan
    g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ Compilation failed!" -ForegroundColor Red
        exit 1
    }
    
    # Run tests
    Write-Host "🧪 Running tests..." -ForegroundColor Cyan
    
    $args = @()
    if ($ListOnly) { $args += "--list-test-cases" }
    if ($Verbose) { $args += "-s" }
    if ($TestPattern -ne "*") { $args += "--test-case=`"$TestPattern`"" }
    
    $output = & .\Tests\test_runner.exe @args 2>&1
    $exitCode = $LASTEXITCODE
    
    # Display output
    $output | ForEach-Object { Write-Host $_ }
    
    # Parse and report
    if ($output -match "Status: SUCCESS") {
        Write-Host "`n✅ ALL TESTS PASSED!" -ForegroundColor Green
    } else {
        Write-Host "`n❌ SOME TESTS FAILED!" -ForegroundColor Red
    }
    
    exit $exitCode
}
finally {
    Pop-Location
}
```

---
description: 'Generate, compile, run and report unit tests for C++ code using doctest framework'
agent: 'agent'
model: Claude Opus 4.5 (copilot)
tools: ['execute/getTerminalOutput', 'execute/runInTerminal', 'read/terminalSelection', 'read/terminalLastCommand', 'edit/editFiles', 'search']
---

# Generate C++ Unit Tests

Generate comprehensive unit tests for the specified C++ code using the **doctest** framework, then compile and run them to verify pass/fail status.

## Context

{{input}}

## Workflow

Follow this complete TDD workflow:

### Step 1: Analyze Selected Code
- Identify the class/function to test
- Determine public interface and expected behavior
- List edge cases and error conditions

### Step 2: Generate Test File
- Create test file in `Firmware/Tests/` directory
- Name: `test_<module>.cpp` matching source file
- Include comprehensive test cases (see structure below)

### Step 3: Compile and Run Tests

**Option A: Use the test runner script (recommended)**
```powershell
cd C:\src\GHCPC++\src-ODrive
.\.github\skills\cpp-testing\run-tests.ps1
```

**Option B: Manual compilation**
```powershell
cd Firmware
$env:PATH = "C:\mingw64\bin;$env:PATH"
g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp
.\Tests\test_runner.exe
```

### Step 4: Report Results
Provide a summary:
- Total test cases and assertions
- Passed/Failed counts
- Any failing test details with line numbers
- Suggestions for fixing failures

## Test Requirements

### Framework: doctest
- Use `#include <doctest.h>` 
- Use `TEST_CASE("Description")` for test cases
- Use `SUBCASE("Description")` for sub-tests
- Use `CHECK()`, `REQUIRE()`, `CHECK_EQ()`, `CHECK_THROWS()` for assertions

### Test Structure
```cpp
#include <doctest.h>
#include "path/to/header.hpp"

TEST_CASE("ClassName::method - normal operation") {
    // Arrange
    ClassName obj;
    
    // Act
    auto result = obj.method(input);
    
    // Assert
    CHECK_EQ(result, expected);
}

TEST_CASE("ClassName::method - edge cases") {
    SUBCASE("zero input") {
        CHECK_EQ(obj.method(0), expected_zero);
    }
    
    SUBCASE("negative input") {
        CHECK_EQ(obj.method(-1), expected_negative);
    }
    
    SUBCASE("boundary values") {
        CHECK_EQ(obj.method(INT_MAX), expected_max);
    }
}

TEST_CASE("ClassName::method - error conditions") {
    SUBCASE("returns error code on invalid input") {
        CHECK_EQ(obj.method(invalid), ErrorCode::INVALID_INPUT);
    }
}
```

### Embedded C++ Constraints
- No dynamic memory allocation in tests (use static arrays)
- No exceptions (use error code checks)
- Mock hardware dependencies
- Test ISR-safe functions with simulated interrupts
- Use fixed-point math where applicable

### Test Categories to Generate

1. **Happy Path Tests**
   - Normal operation with valid inputs
   - Expected outputs verified

2. **Boundary Tests**
   - Minimum/maximum values
   - Zero, one, many patterns
   - Buffer boundaries

3. **Error Handling Tests**
   - Invalid inputs
   - Error code verification
   - Recovery behavior

4. **Edge Cases**
   - Empty inputs
   - Overflow/underflow conditions
   - Race condition scenarios (if applicable)

5. **Integration Points**
   - Interaction with dependencies
   - State machine transitions
   - Callback invocations

### File Naming Convention
- Source: `motor.cpp` → Test: `test_motor.cpp`
- Place in: `Firmware/Tests/` directory
- Include test runner if needed

## Execution Commands

### Compile Tests
```powershell
cd C:\src\GHCPC++\src-ODrive\Firmware
$env:PATH = "C:\mingw64\bin;$env:PATH"
g++ -O3 -std=c++17 -I. -Idoctest -o Tests/test_runner.exe Tests/test_runner.cpp
```

### Run Tests with Report
```powershell
.\Tests\test_runner.exe
```

### Run Specific Test Case
```powershell
.\Tests\test_runner.exe --test-case="*ClassName*"
```

### Verbose Output
```powershell
.\Tests\test_runner.exe -s
```

## Expected Output Format

After running tests, provide a report like:

```
╔══════════════════════════════════════════════════════════════╗
║                    TEST EXECUTION REPORT                      ║
╠══════════════════════════════════════════════════════════════╣
║ Status: SUCCESS / FAILURE                                     ║
║ Test Cases: X passed | Y failed | Z skipped                  ║
║ Assertions: A passed | B failed                               ║
╠══════════════════════════════════════════════════════════════╣
║ FAILED TESTS (if any):                                        ║
║ ├─ test_motor.cpp:45 - Motor::set_velocity clamp test        ║
║ │   Expected: 100.0f, Actual: 150.0f                          ║
║ └─ Suggestion: Check std::clamp implementation                ║
╚══════════════════════════════════════════════════════════════╝
```

Generate:
1. Complete test file with all includes
2. Docstring header with file description
3. Organized test cases by category
4. Comments explaining test intent

## Reference Files

Look at existing tests for patterns:
- #file:Firmware/Tests/test_trap_traj.cpp
- #file:Firmware/fibre-cpp/Tests/test_bufptr.cpp

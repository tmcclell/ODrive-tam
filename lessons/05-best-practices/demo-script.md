# Section 5 Demo Script: C++ Best Practices with Copilot

**Duration:** 50 minutes total (demos ~34 min, hands-on ~16 min)  
**Presenter Notes:** This script provides exact steps and dialogue for live demos

---

## Demo Overview

| Demo | Topic | Duration |
|------|-------|----------|
| 1 | Modern C++ Patterns (RAII, Templates, Const) | 12 min |
| 2 | Embedded C++ Specifics (Static Alloc, Error Codes, Volatile) | 12 min |
| 3 | RTOS & Hardware Patterns (State Machines, HAL) | 10 min |
| 4 | Hands-On: Generate Components | 16 min |

---

## Demo 1: Modern C++ Patterns (12 min)

### Setup
- Open VS Code with ODrive workspace
- Have these files ready to open:
  - `src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp`
  - `src-ODrive/Firmware/fibre-cpp/include/fibre/cpp_utils.hpp`
- Open Copilot Chat panel (Ctrl+Alt+I)

### Demo Flow

---

### Part A: RAII & Resource Management (4 min)

**Presenter Says:**
> "RAII - Resource Acquisition Is Initialization - is fundamental to modern C++. Let's see how Copilot can analyze existing code and suggest RAII improvements."

**Open File:** `src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp`

**Step 1: Use Chat Mode to Analyze**

**In Copilot Chat, type:**
```
Review this GPIO class and suggest RAII improvements:
#file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp

Analyze:
- Current resource lifecycle management
- Missing RAII patterns
- Copy/move semantics issues
- Potential double-free or leak scenarios

Provide specific recommendations with code examples.
```

**Presenter Says:**
> "Notice I'm using Chat Mode (Ask) for analysis first. Copilot will examine the class structure and identify what's missing from proper RAII implementation."

*[Wait for response]*

**Highlight Key Points from Response:**
> "Copilot identified several issues:
> - Missing destructor that would call unsubscribe()
> - Copy semantics need to be deleted
> - Move semantics should be implemented
> - Need ownership tracking flag
> 
> Now let's have Agent mode implement these fixes."

---

**Step 2: Use Agent Mode to Implement**

**Switch to Agent Mode in the dropdown**

**In Copilot Chat, type:**
```
@workspace Implement RAII improvements for GPIO class

Context: #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp
         #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.cpp

Requirements:
- Add destructor that calls unsubscribe() if owns_subscription_
- Delete copy constructor and copy assignment
- Implement move constructor and move assignment
- Add owns_subscription_ member to track cleanup responsibility

Acceptance Criteria:
- Compiles without warnings
- No manual unsubscribe() needed in typical usage
- Safe to store in containers with move semantics
```

**Presenter Says:**
> "Agent mode will:
> 1. Plan the changes across both header and source files
> 2. Show us a diff preview
> 3. Apply coordinated changes
> 
> This is the pattern: Chat to analyze, Agent to implement."

*[Review the generated changes]*

---

### Part B: Templates & Const Correctness (4 min)

**Presenter Says:**
> "Templates and const correctness are essential for embedded C++ - they give us compile-time safety with zero runtime overhead."

**Open File:** `src-ODrive/Firmware/fibre-cpp/include/fibre/cpp_utils.hpp`

**Step 3: Explain Template Patterns**

**In Copilot Chat (Ask Mode), type:**
```
Explain the template metaprogramming patterns in:
#file:src-ODrive/Firmware/fibre-cpp/include/fibre/cpp_utils.hpp

Focus on:
- integer_sequence implementation and use cases
- Type trait techniques for compile-time checks
- SFINAE patterns used
- Benefits for embedded systems (zero runtime cost)
```

**Presenter Says:**
> "Understanding existing patterns is crucial before writing new ones. Let's see what Copilot finds."

*[Wait for response, scroll through explanation]*

---

**Step 4: Audit Const Correctness**

**In Copilot Chat, type:**
```
Audit const correctness in this class:
#file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp

Check:
1. Which methods should be marked const but aren't?
2. Which parameters should be const references?
3. Suggest const overloads where appropriate
```

**Presenter Says:**
> "Const correctness catches bugs at compile time. Let's see what Copilot identifies."

*[Review suggestions]*

> "Notice Copilot found that `read()` and `get_pin_number()` should be const - they don't modify the object's state."

---

### Part C: Cross-File Refactoring (4 min)

**Presenter Says:**
> "One of Copilot's strengths is coordinated refactoring across multiple files. Let me show you a rename operation."

**Step 5: Rename Function Across Files**

**In Copilot Chat (Edit Mode), type:**
```
Rename the function delay_us to delay_ust in #file:src-ODrive/Firmware/MotorControl/utils.hpp and update all files that call this function to use the new name.
```

**Presenter Says:**
> "Watch how Copilot finds all usages in drv8301.cpp and board.cpp and updates them together. This is much safer than find-and-replace because it understands the code structure!"

*[Show the diff preview across multiple files]*

**Key Points:**
- ✅ Header declaration updated
- ✅ Implementation in utils.cpp updated
- ✅ All call sites in drv8301.cpp updated
- ✅ All call sites in board.cpp updated

---

## Demo 2: Embedded C++ Specifics (12 min)

### Setup
- Have these files ready:
  - `src-ODrive/Firmware/fibre-cpp/fibre.cpp` (TheInstance pattern)
  - `src-ODrive/Firmware/MotorControl/encoder.cpp` (error handling)
  - `src-ODrive/Firmware/Board/v3/board.cpp` (volatile/ISR)

### Demo Flow

---

### Part A: Static Allocation - No Heap (4 min)

**Presenter Says:**
> "In embedded systems, we avoid dynamic memory allocation. The heap is unpredictable - fragmentation, allocation failures at runtime, non-deterministic timing. Let's see how Copilot helps enforce static allocation."

**Open File:** `src-ODrive/Firmware/fibre-cpp/fibre.cpp` (line ~56-75)

**Step 1: Show TheInstance Pattern**

**In Copilot Chat, type:**
```
Analyze dynamic memory usage in this driver:
#file:src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.cpp

Identify:
1. All uses of new, delete, malloc, free
2. std::vector or other heap-based containers
3. Unbounded queues or buffers

For each issue, suggest static allocation alternatives:
- Singleton pattern vs new
- std::array with compile-time size
- Fixed-size ring buffers
```

**Presenter Says:**
> "Copilot scans for heap usage patterns and suggests embedded-safe alternatives like TheInstance<T> singleton pattern shown in fibre.cpp."

*[Wait for response]*

> "Key takeaway: Replace std::vector with std::array, use TheInstance<> for singletons, and add compile-time size constants."

---

**Step 2: Refactor to Static (Agent Mode)**

**Switch to Agent Mode**

**In Copilot Chat, type:**
```
@workspace Refactor SPI arbiter to use static allocation only

Context: #file:src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.cpp

Refactoring steps:
1. Replace std::vector with std::array<SpiTask, 8>
2. Add compile-time size constants (MAX_TASKS, QUEUE_SIZE)
3. Add overflow handling (return error on full)
4. Remove all new/delete/malloc/free

Acceptance Criteria:
- Zero calls to heap allocator
- Fixed RAM footprint (no growth at runtime)
```

**Presenter Says:**
> "Agent mode plans and executes the refactoring. Notice it adds the size constants and proper overflow handling."

---

### Part B: No Exceptions - Error Codes (4 min)

**Presenter Says:**
> "Exceptions are forbidden in most embedded systems - they add code bloat and non-deterministic timing. We use error codes instead."

**Open File:** `src-ODrive/Firmware/MotorControl/encoder.cpp` (around line 329-450)

**Step 3: Analyze Error Patterns**

**In Copilot Chat, type:**
```
Analyze error handling patterns across ODrive firmware:
#file:src-ODrive/Firmware/MotorControl/encoder.cpp
#file:src-ODrive/Firmware/MotorControl/axis.cpp

Design unified error handling system:
1. Catalog current error conditions by category
2. Propose enum class ErrorCode with severity levels:
   - Info, Warning, Error, Critical
3. Define error propagation strategy

Output: Complete error code enum with documentation
```

**Presenter Says:**
> "Copilot analyzes the existing patterns - set_error(), bool returns - and designs a comprehensive error system."

*[Review the generated enum class]*

---

**Step 4: Convert Function to Use Error Codes**

**In Copilot Chat (Agent Mode), type:**
```
@workspace Convert calibration function to use error codes

Context: #file:src-ODrive/Firmware/MotorControl/encoder.cpp

Conversion steps:
1. Change return type from void to enum class ErrorCode
2. Replace set_error() calls with error code returns
3. Add error checking after each operation
4. Update all callers to check return value
```

**Presenter Says:**
> "Watch how Agent mode updates both the function and its callers in a coordinated way."

---

### Part C: Volatile and ISR Safety (4 min)

**Presenter Says:**
> "The volatile keyword is critical for embedded systems - it tells the compiler not to optimize away reads/writes to hardware registers or ISR-shared variables."

**Open File:** `src-ODrive/Firmware/Board/v3/board.cpp`

**Step 5: Review Volatile Usage**

**In Copilot Chat, type:**
```
Review volatile keyword usage in:
#file:src-ODrive/Firmware/Board/v3/board.cpp

Check:
- All shared ISR variables are volatile
- Hardware register pointers use volatile
- No missing volatile qualifiers
```

**Presenter Says:**
> "Copilot audits the code for missing volatile qualifiers - a common source of hard-to-debug issues in embedded systems."

---

**Step 6: Generate ISR Handler (Inline Chat)**

**Open a new line in a C++ file and type:**
```cpp
// Generate TIM13 overflow ISR with critical section
extern "C" void TIM13_IRQHandler(void) {
```

**Press Ctrl+I for Inline Chat, type:**
```
Complete this ISR handler with proper pattern:
- Clear interrupt flag first
- Use cpu_enter_critical/cpu_exit_critical
- No blocking calls
- Update volatile timestamp
```

**Presenter Says:**
> "Inline completions for ISR handlers are incredibly helpful. Copilot knows the pattern: clear flags, minimal critical section, no blocking."

---

## Demo 3: RTOS & Hardware Patterns (10 min)

### Setup
- Have these files ready:
  - `src-ODrive/Firmware/MotorControl/axis.cpp` (state machine)
  - `src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp` (HAL)
  - `src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.hpp` (HAL)

### Demo Flow

---

### Part A: State Machines (5 min)

**Presenter Says:**
> "The Axis state machine in ODrive is the crown jewel - it shows how to implement complex control flow in embedded systems. Let's understand and replicate this pattern."

**Open File:** `src-ODrive/Firmware/MotorControl/axis.cpp` (around line 454-600)

**Step 1: Understand the Pattern**

**In Copilot Chat, type:**
```
Explain the state machine architecture in Axis class:
#file:src-ODrive/Firmware/MotorControl/axis.cpp:run_state_machine_loop

Analyze:
1. How does the task chain pattern work?
2. How are states sequenced automatically?
3. How does error handling interrupt state flow?
4. How are resources acquired/released per state?

Then create a state transition diagram in Mermaid format.
```

**Presenter Says:**
> "This is a great use of Ask mode - understanding complex existing patterns before implementing your own."

*[Review the Mermaid diagram if generated]*

---

**Step 2: Generate New State Machine**

**Switch to Agent Mode**

**In Copilot Chat, type:**
```
@workspace Create LED controller with state machine pattern

Reference: #file:src-ODrive/Firmware/MotorControl/axis.cpp
           #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp

State Machine Design:
States:
- LED_STATE_OFF: GPIO low
- LED_STATE_ON: GPIO high  
- LED_STATE_BLINKING: Toggle at rate_hz
- LED_STATE_ERROR: Fast blink (5 Hz)

Features:
- Task chain for automatic sequences
- State entry/exit actions
- Error recovery
- RAII guard for GPIO lifecycle

Files to create:
- led_controller.hpp
- led_controller.cpp

Acceptance Criteria:
- Follows Axis pattern (switch/case with task chain)
- FreeRTOS task with osDelay
- Static allocation only
- No exceptions - error codes
```

**Presenter Says:**
> "Agent mode creates a complete implementation following the established patterns. Notice it:
> - Reuses the task chain pattern from Axis
> - Uses Stm32Gpio with RAII
> - Creates both header and source files"

---

### Part B: HAL Abstractions (5 min)

**Presenter Says:**
> "Hardware Abstraction Layers hide vendor-specific details behind clean interfaces. Let's see how to create new HALs following ODrive patterns."

**Step 3: Learn HAL Patterns**

**In Copilot Chat, type:**
```
Analyze HAL abstraction patterns in ODrive:
#file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp
#file:src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.hpp

Extract common design principles:
1. How do they hide STM32 HAL implementation details?
2. How is resource ownership managed (RAII)?
3. How are async operations handled?
4. How is thread-safety achieved?

Create a HAL Design Checklist for new abstractions.
```

**Presenter Says:**
> "Before implementing a new HAL, understand the patterns. This checklist becomes your template."

*[Review the generated checklist]*

---

**Step 4: Create New HAL**

**In Copilot Chat (Agent Mode), type:**
```
@workspace Create UART HAL abstraction following ODrive patterns

References: #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp
            #file:src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.hpp

UART HAL Requirements:
- Stm32UartHal class wrapping UART_HandleTypeDef
- RAII: constructor acquires, destructor releases
- Delete copy, implement move semantics
- Static RX/TX ring buffers (std::array<uint8_t, 256>)
- Error recovery for overrun/framing errors

Public API:
- ErrorCode init(uint32_t baudrate)
- ErrorCode write(const uint8_t* data, size_t len)
- std::pair<size_t, ErrorCode> read(uint8_t* buf, size_t max_len)

Files to create:
- stm32_uart_hal.hpp
- stm32_uart_hal.cpp
```

**Presenter Says:**
> "Agent mode creates a complete HAL implementation following the established patterns. The key is providing good references - Copilot uses them as templates."

---

## Demo 4: Hands-On Exercises (16 min)

### Setup
- Participants work on their own machines
- Have the readme.md open for reference prompts

### Exercise Overview

**Presenter Says:**
> "Now it's your turn! We have three exercises of increasing complexity. Pick one based on your comfort level."

---

### Exercise 1: LED Driver with State Machine (Beginner)

**Display this prompt for participants:**

```
@workspace Create LED driver with state machine

Context: #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp
         #file:src-ODrive/Firmware/MotorControl/axis.cpp

Class: LedController

States (enum class LedState):
- OFF, ON, BLINKING, FADING, ERROR

Public API:
- LedController(Stm32Gpio&& pin, osPriority priority)
- ErrorCode set_state(LedState state)
- ErrorCode configure_blink(uint32_t rate_hz)
- LedState get_state() const

Implementation details:
- Use osDelay for timing
- State entry/exit actions
- Static allocation only
- Thread-safe state transitions

Files to create:
- Firmware/Drivers/led_controller.hpp
- Firmware/Drivers/led_controller.cpp
```

**Success Criteria:**
- ✅ Compiles without warnings
- ✅ No dynamic allocation
- ✅ State machine follows Axis pattern

---

### Exercise 2: SPI Sensor Driver (Intermediate)

**Display this prompt for participants:**

```
@workspace Create ADXL345 accelerometer driver using SPI arbiter

Context: #file:src-ODrive/Firmware/Drivers/STM32/stm32_spi_arbiter.hpp
         #file:src-ODrive/Firmware/MotorControl/encoder.cpp

Class: Adxl345Driver

Public API:
- ErrorCode init(Stm32SpiArbiter& spi, Stm32Gpio&& cs_pin)
- ErrorCode self_test()
- ErrorCode read_acceleration(float& x, float& y, float& z)
- ErrorCode configure_range(uint8_t g_range)

Features:
- Async SPI transactions
- Error recovery (retry on timeout)
- Volatile for register bitfields
- Static allocation only

Files to create:
- Firmware/Drivers/Sensors/adxl345.hpp
- Firmware/Drivers/Sensors/adxl345.cpp
```

**Success Criteria:**
- ✅ Device ID verification passes
- ✅ Reads X/Y/Z acceleration
- ✅ SPI errors handled gracefully

---

### Exercise 3: Lock-Free Ring Buffer (Advanced)

**Display this prompt for participants:**

```
@workspace Create lock-free ring buffer for ISR-to-task communication

Context: #file:src-ODrive/Firmware/fibre-cpp/include/fibre/bufptr.hpp

Template Class: RingBuffer<T, N>

Requirements:
- static_assert for POD types only
- Use volatile for head/tail (not std::atomic)
- Waste one slot to detect full (head+1)%N == tail
- write() called from ISR (single producer)
- read() called from task (single consumer)
- Overflow: drop newest (return false)

Files to create:
- Firmware/Utils/ring_buffer.hpp
- Tests/ring_buffer_test.cpp

Unit Tests:
- Basic write/read
- Overflow handling
- Rapid ISR simulation
```

**Success Criteria:**
- ✅ No data races
- ✅ Correct full/empty detection
- ✅ Unit tests pass

---

## Demo 5: Test-Driven Development (10 min)

### Setup
- Have terminal ready to run setup script
- Open `Firmware/Tests/` folder

### Demo Flow

---

### Part A: Introduction to TDD (2 min)

**Presenter Says:**
> "Test-Driven Development means writing tests FIRST, then implementing code to make them pass. This sounds backwards, but it leads to better designed, more reliable code."

**Show the Red-Green-Refactor diagram:**

```
🔴 RED      →  Write failing test
    ↓
🟢 GREEN    →  Write minimal code to pass
    ↓
🔵 REFACTOR →  Improve code quality
    ↓
   [Repeat]
```

> "In embedded systems, TDD is especially valuable because:
> - Hardware isn't always available
> - Bugs are expensive (recall, hardware damage)
> - Tests document expected behavior"

---

### Part B: Setup Testing Environment (2 min)

**Presenter Says:**
> "We've created a skill that includes setup scripts. Let me show you."

**In Terminal:**
```powershell
cd src-ODrive

# Show the setup script help
.\.github\skills\cpp-testing\setup-tests.ps1
```

**Show the output:**
```
🧪 ODrive C++ Test Setup
========================

Usage:
  .\setup-tests.ps1 -InstallDoctest      # Download doctest framework
  .\setup-tests.ps1 -CreateTestRunner    # Create test_runner.cpp
  .\setup-tests.ps1 -BuildTests          # Compile all tests
  .\setup-tests.ps1 -RunTests            # Run test executable
```

**Presenter Says:**
> "The skill bundles everything needed: doctest patterns, setup scripts, mock examples. Copilot auto-discovers this when you mention testing."

---

### Part C: TDD Live Demo - Red Phase (3 min)

**Presenter Says:**
> "Let's add a velocity limit feature using TDD. First, the FAILING test."

**Create new file:** `Firmware/Tests/test_velocity_limit.cpp`

**In Copilot Chat (Agent Mode), type:**
```
Create a failing test for Motor::set_velocity that verifies:
- Velocities above config_.vel_limit are clamped to the limit
- Negative velocities beyond -vel_limit are clamped to -vel_limit
- Velocities within limit are unchanged

Use doctest framework. The test should FAIL because set_velocity doesn't exist yet.

Reference: #file:Firmware/Tests/test_trap_traj.cpp for patterns
```

**Show the generated test:**

```cpp
TEST_CASE("Motor::set_velocity - clamps to configured limit") {
    Motor motor;
    motor.config_.vel_limit = 100.0f;
    
    SUBCASE("above limit - clamped") {
        motor.set_velocity(150.0f);
        CHECK_EQ(motor.vel_setpoint_, 100.0f);
    }
    
    SUBCASE("below negative limit - clamped") {
        motor.set_velocity(-150.0f);
        CHECK_EQ(motor.vel_setpoint_, -100.0f);
    }
    
    SUBCASE("within limit - unchanged") {
        motor.set_velocity(50.0f);
        CHECK_EQ(motor.vel_setpoint_, 50.0f);
    }
}
```

**Presenter Says:**
> "This is the RED phase. The test documents EXACTLY what we want. It will fail because `set_velocity()` doesn't exist yet - and that's correct!"

---

### Part D: Green Phase (2 min)

**Presenter Says:**
> "Now we write the MINIMAL code to make it pass."

**In Copilot Chat, type:**
```
Implement Motor::set_velocity() to make this test pass:

TEST_CASE("Motor::set_velocity - clamps to configured limit") {
    // ... test from above
}

Write the minimal implementation in motor.cpp. Use std::clamp.
```

**Show the implementation:**

```cpp
void Motor::set_velocity(float vel) {
    vel_setpoint_ = std::clamp(vel, -config_.vel_limit, config_.vel_limit);
}
```

**Presenter Says:**
> "That's it! One line. The test passes. GREEN phase complete."

---

### Part E: Using the /generate-tests Prompt (1 min)

**Presenter Says:**
> "For existing code, use our prompt file to generate comprehensive tests."

**In Copilot Chat, type:**
```
/generate-tests for #file:Firmware/MotorControl/encoder.cpp

Focus on:
- Encoder::get_pos_estimate() accuracy
- Error handling for sensor failures
- Edge cases for position rollover
```

**Presenter Says:**
> "The prompt automatically includes doctest patterns, embedded constraints, and test categories. It generates a complete test file following our conventions."

---

## Demo Wrap-Up (2 min)

**Presenter Says:**
> "Let's recap what we covered:
>
> **Modern C++ Patterns:**
> - RAII for automatic resource cleanup
> - Templates for zero-cost abstractions
> - Const correctness for compile-time safety
> - Cross-file refactoring with Copilot
>
> **Embedded Constraints:**
> - Static allocation (no heap)
> - Error codes (no exceptions)
> - Volatile for hardware/ISR
>
> **RTOS Patterns:**
> - State machines with task chains
> - HAL abstractions
> - Task synchronization
>
> **Test-Driven Development:**
> - Write tests FIRST (Red)
> - Implement minimal code (Green)
> - Refactor with confidence (Blue)
> - Use /generate-tests prompt and cpp-testing skill
>
> **The Copilot Workflow:**
> 1. **Ask Mode** to analyze and understand patterns
> 2. **Edit Mode** for focused changes and cross-file refactoring
> 3. **Agent Mode** for complex multi-file implementations
>
> Remember: Always provide good context with `#file:` references and clear acceptance criteria!"

---

## Troubleshooting Tips

| Issue | Solution |
|-------|----------|
| Copilot generates exceptions | Add "No exceptions - use error codes" to prompt |
| Generated code uses new/malloc | Add "Static allocation only, no heap" constraint |
| Missing volatile qualifiers | Explicitly mention "ISR-shared" or "hardware register" |
| State machine too complex | Break into Chat (design) then Agent (implement) |
| Cross-file changes incomplete | Use `@workspace` and reference both files |

---

## Quick Reference: Mode Selection

| Task | Mode | Why |
|------|------|-----|
| Understand existing code | Ask | Read-only analysis |
| Audit const/RAII patterns | Ask | Get recommendations first |
| Apply specific fixes | Edit | Focused single-file changes |
| Rename across files | Edit | Coordinated refactoring |
| Generate new components | Agent | Multi-file, planning required |
| Convert patterns codebase-wide | Agent | Needs search + modify |

---

## Files Referenced in This Demo

| File | Pattern Demonstrated |
|------|---------------------|
| `stm32_gpio.hpp/cpp` | RAII, const correctness, HAL |
| `cpp_utils.hpp` | Templates, type traits |
| `axis.cpp` | State machine, task chain |
| `encoder.cpp` | Error handling patterns |
| `board.cpp` | Volatile, ISR handlers |
| `stm32_spi_arbiter.hpp` | HAL, async patterns |
| `bufptr.hpp` | Buffer abstractions |
| `utils.hpp` | Utility functions, cross-file refactoring |

# Section 5: Hands-On Exercises

**Duration:** 15 minutes  
**Format:** Individual or pair programming  
**Goal:** Apply C++ best practices using GitHub Copilot on real ODrive code

---

## Exercise Overview

Complete **ONE** of three exercises based on your comfort level:

| Exercise | Focus Area | Difficulty | Time |
|----------|------------|------------|------|
| 1. RAII GPIO Guard | Resource management, destructors | ⭐ Beginner | 15 min |
| 2. Error Code Refactoring | Exception-free patterns | ⭐⭐ Medium | 15 min |
| 3. Lock-Free Ring Buffer | ISR safety, volatile, atomics | ⭐⭐⭐ Advanced | 15 min |

> 💡 **Tip:** If you finish early, try the TDD bonus at the end of your exercise!

---

## Exercise 1: RAII GPIO Guard (Beginner)

### Scenario
Create a scoped GPIO lock that automatically releases the pin when it goes out of scope - preventing resource leaks.

### Learning Goals
- Understand RAII (Resource Acquisition Is Initialization)
- Implement proper destructor cleanup
- Delete copy semantics, implement move semantics
- Use `[[nodiscard]]` for safety

### Files to Work With
- `src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp`
- Create new: `src-ODrive/Firmware/Drivers/STM32/gpio_guard.hpp`

---

### Step 1: Analyze Existing Pattern (2 min)

**💬 Chat Mode - Ask Copilot to explain:**
```
Explain the resource management pattern in this GPIO class:
#file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp

Questions:
1. How is the GPIO pin acquired and released?
2. What happens if someone forgets to call unsubscribe()?
3. Is it safe to copy this class?
4. How would RAII improve this?
```

**✅ Expected:** Copilot explains that manual cleanup is required and suggests RAII improvements.

---

### Step 2: Create the RAII Guard (5 min)

**🤖 Agent Mode - Generate the guard class:**
```
@workspace Create a scoped GPIO guard class using RAII

Context: #file:src-ODrive/Firmware/Drivers/STM32/stm32_gpio.hpp

Create: Firmware/Drivers/STM32/gpio_guard.hpp

Class: ScopedGpioGuard

Requirements:
- Constructor takes Stm32Gpio& and acquires the pin (set high or low)
- Destructor automatically releases (restores original state)
- Delete copy constructor and copy assignment
- Implement move constructor and move assignment
- Add [[nodiscard]] to factory methods
- Store original pin state to restore on destruction

Example usage:
```cpp
void critical_section() {
    ScopedGpioGuard guard(enable_pin, true);  // Pin goes HIGH
    do_work();
}  // Pin automatically restored on exit
```

Acceptance Criteria:
- Compiles without warnings
- Works with const Stm32Gpio&
- Exception-safe (no exceptions, but still safe on early return)
- Doxygen documentation
```

**✅ Verify:** File created with proper RAII semantics.

---

### Step 3: Add Const Correctness (3 min)

**✏️ Edit Mode - Apply const improvements:**
```
Review and apply const correctness to ScopedGpioGuard:

1. Mark all read-only methods as const
2. Mark parameters that aren't modified as const
3. Use const references where appropriate
4. Add constexpr to compile-time constants
```

---

### Step 4: Test Your Implementation (5 min)

**💬 Chat Mode - Generate test cases:**
```
/generate-tests for the ScopedGpioGuard class

Test scenarios:
1. Guard sets pin HIGH on construction, restores on destruction
2. Guard restores state even if exception occurs (simulate with early return)
3. Move constructor transfers ownership
4. Deleted copy constructor causes compile error
```

**Run the tests:**
```powershell
cd src-ODrive/Firmware
g++ -std=c++17 -I. -o Tests/test_gpio_guard.exe Tests/test_gpio_guard.cpp
.\Tests\test_gpio_guard.exe
```

### ✅ Success Criteria
- [ ] Guard compiles without warnings
- [ ] Destructor always runs (check with print statement)
- [ ] Copy operations deleted
- [ ] Move operations work correctly
- [ ] Const correctness applied

---

## Exercise 2: Error Code Refactoring (Medium)

### Scenario
Convert a function that uses `set_error()` to return explicit error codes instead. This is the embedded C++ pattern for exception-free error handling.

### Learning Goals
- Design `enum class ErrorCode` with meaningful values
- Replace side-effect error setting with return values
- Use `[[nodiscard]]` to ensure errors are checked
- Update callers to handle errors

### Files to Modify
- `src-ODrive/Firmware/MotorControl/encoder.cpp`
- `src-ODrive/Firmware/MotorControl/encoder.hpp`

---

### Step 1: Analyze Current Error Pattern (2 min)

**💬 Chat Mode - Understand the pattern:**
```
Analyze the error handling in run_offset_calibration():
#file:src-ODrive/Firmware/MotorControl/encoder.cpp

Questions:
1. How does set_error() work?
2. What are the different error conditions?
3. What does the bool return value mean?
4. Why is this pattern problematic for embedded systems?
```

---

### Step 2: Design Error Code Enum (3 min)

**💬 Chat Mode - Design the enum:**
```
Design an ErrorCode enum for Encoder calibration:
#file:src-ODrive/Firmware/MotorControl/encoder.cpp

Requirements:
- enum class (not plain enum)
- uint8_t underlying type (minimal size)
- Include: Success, IndexNotFound, HallNotCalibrated, NoResponse, 
  CprMismatch, Disarmed, Aborted, Timeout
- Use [[nodiscard]] attribute recommendation

Output: Complete enum definition with Doxygen comments
```

---

### Step 3: Refactor the Function (5 min)

**🤖 Agent Mode - Apply the refactoring:**
```
@workspace Refactor run_offset_calibration to use ErrorCode returns

Context: 
- #file:src-ODrive/Firmware/MotorControl/encoder.cpp
- #file:src-ODrive/Firmware/MotorControl/encoder.hpp

Changes:
1. Add the ErrorCode enum to encoder.hpp
2. Change return type from bool to ErrorCode
3. Replace set_error() + return false with: return ErrorCode::XXX
4. Replace return true with: return ErrorCode::Success
5. Keep the set_error() call for backward compatibility (both set_error AND return code)
6. Add [[nodiscard]] to the function declaration

Important: 
- Don't break existing callers (they check bool)
- Provide helper: static bool succeeded(ErrorCode e) { return e == ErrorCode::Success; }
```

---

### Step 4: Update One Caller (3 min)

**🤖 Agent Mode - Update axis.cpp:**
```
@workspace Update the caller of run_offset_calibration in axis.cpp

Context:
- #file:src-ODrive/Firmware/MotorControl/axis.cpp
- Search for: encoder_.run_offset_calibration()

Change from:
```cpp
status = encoder_.run_offset_calibration();
```

To:
```cpp
auto calibResult = encoder_.run_offset_calibration();
status = (calibResult == Encoder::ErrorCode::Success);
if (!status) {
    // Error already logged via set_error(), but now we also have the code
}
```
```

---

### Step 5: Quick Test (2 min)

**Verify compilation:**
```powershell
cd src-ODrive/Firmware
# Check for syntax errors
g++ -std=c++17 -fsyntax-only -I. MotorControl/encoder.cpp
```

### ✅ Success Criteria
- [ ] ErrorCode enum defined with all error types
- [ ] Function returns ErrorCode instead of bool
- [ ] [[nodiscard]] attribute applied
- [ ] At least one caller updated
- [ ] Code compiles without errors

---

## Exercise 3: Lock-Free Ring Buffer (Advanced)

### Scenario
Create an ISR-safe ring buffer for passing ADC samples from an interrupt to a processing task - without locks.

### Learning Goals
- Use `volatile` correctly for ISR-shared data
- Understand single-producer/single-consumer patterns
- Implement correct full/empty detection
- Write ISR-safe code

### Files to Create
- `src-ODrive/Firmware/Utils/ring_buffer.hpp`
- `src-ODrive/Firmware/Tests/test_ring_buffer.cpp`

---

### Step 1: Design Review (2 min)

**💬 Chat Mode - Validate design:**
```
Review this lock-free ring buffer design for ISR safety:

Requirements:
- Single producer (ISR writes samples)
- Single consumer (task reads samples)  
- No mutex/semaphore (ISR can't block)
- Detect full vs empty correctly

Design choices to validate:
1. Use volatile size_t for head/tail indices
2. Waste one slot to distinguish full from empty
3. Producer only writes head, consumer only writes tail
4. No memory barriers needed (ARM Cortex-M strong ordering)

Is this design correct? Any issues?
```

---

### Step 2: Implement the Buffer (6 min)

**🤖 Agent Mode - Generate implementation:**
```
@workspace Create a lock-free ring buffer for ISR-to-task communication

Create: Firmware/Utils/ring_buffer.hpp

Template: RingBuffer<T, N>

```cpp
template<typename T, size_t N>
class RingBuffer {
    static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
    static_assert(N > 1 && N <= 256, "N must be 2-256");
    
public:
    // Producer (ISR) - returns false if full
    [[nodiscard]] bool write(const T& item);
    
    // Consumer (task) - returns false if empty
    [[nodiscard]] bool read(T& item);
    
    // Status queries
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool is_full() const;
    
private:
    std::array<T, N> buffer_;
    volatile size_t head_{0};  // Write position (ISR modifies)
    volatile size_t tail_{0};  // Read position (task modifies)
};
```

Implementation requirements:
- write(): ISR-safe, O(1), no blocking
- read(): task-safe, O(1), no blocking  
- Full when: (head + 1) % N == tail
- Empty when: head == tail
- Use volatile for indices
- Overflow policy: drop newest (return false)

Acceptance Criteria:
- No locks or atomic operations
- Works with uint8_t, uint16_t, float
- Doxygen with ISR safety notes
```

---

### Step 3: Write Unit Tests (4 min)

**💬 Chat Mode - Generate tests:**
```
/generate-tests for the RingBuffer<T, N> class

Test categories:
1. Basic operations - write one, read one
2. Fill to capacity - verify size tracking
3. Overflow handling - write when full returns false
4. Empty handling - read when empty returns false
5. Wrap-around - write/read past end of array
6. Stress test - rapid alternating writes/reads
```

---

### Step 4: Compile and Run (3 min)

```powershell
cd src-ODrive/Firmware
$env:PATH = "C:\mingw64\bin;$env:PATH"

# Compile tests
g++ -std=c++17 -O2 -I. -Idoctest -o Tests/test_ring_buffer.exe Tests/test_ring_buffer.cpp

# Run tests
.\Tests\test_ring_buffer.exe
```

**Expected output:**
```
[doctest] test cases:      6 |      6 passed |      0 failed |
[doctest] assertions:     20 |     20 passed |      0 failed |
[doctest] Status: SUCCESS!
```

### ✅ Success Criteria
- [ ] Template compiles for multiple types
- [ ] static_assert rejects non-trivial types
- [ ] Volatile used for shared indices
- [ ] Full/empty detection correct
- [ ] All unit tests pass
- [ ] ISR safety documented

---

## 🎁 Bonus: TDD Challenge (If Time Permits)

If you finish your exercise early, try this TDD (Test-Driven Development) challenge:

### The Challenge: Add a `peek()` Method

**Step 1: 🔴 Write the failing test FIRST**
```cpp
TEST_CASE("RingBuffer::peek - returns next item without removing") {
    RingBuffer<int, 4> rb;
    rb.write(42);
    
    int value = 0;
    bool ok = rb.peek(value);
    
    CHECK(ok);
    CHECK_EQ(value, 42);
    CHECK_EQ(rb.size(), 1);  // Item NOT removed
}
```

**Step 2: 🟢 Implement the minimal code**
```
@workspace Add peek() method to RingBuffer that reads without removing

Implementation: Just like read() but don't advance tail_
```

**Step 3: 🔵 Refactor if needed**
- Add edge case tests (peek empty buffer)
- Add const correctness

---

## 📋 Quick Reference: Common Patterns

### RAII Template
```cpp
class ScopedResource {
public:
    explicit ScopedResource(Resource& r) : r_(r) { r_.acquire(); }
    ~ScopedResource() { r_.release(); }
    
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;
private:
    Resource& r_;
};
```

### Error Code Pattern
```cpp
enum class ErrorCode : uint8_t {
    Success = 0,
    InvalidInput,
    Timeout,
    HardwareFault
};

[[nodiscard]] ErrorCode doOperation();

// Caller MUST check:
if (auto err = doOperation(); err != ErrorCode::Success) {
    handleError(err);
}
```

### Volatile for ISR
```cpp
// Shared between ISR and task
volatile bool data_ready = false;
volatile uint16_t adc_value;

void ADC_IRQHandler() {
    adc_value = ADC1->DR;  // Read hardware
    data_ready = true;      // Signal task
}

void task() {
    if (data_ready) {
        data_ready = false;
        process(adc_value);
    }
}
```

---

## 🚨 Troubleshooting

| Issue | Solution |
|-------|----------|
| Copilot doesn't see file changes | Reload VS Code window (Ctrl+Shift+P → "Reload Window") |
| Compilation errors | Check include paths: `-I. -Idoctest` |
| Tests don't run | Ensure `#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` is in test file |
| Agent mode not working | Use `@workspace` prefix and provide file context with `#file:` |
| Error codes not checked | Add `[[nodiscard]]` and enable `-Werror=unused-result` |

---

## 📊 Self-Assessment

After completing your exercise, rate yourself:

| Skill | Beginner | Intermediate | Advanced |
|-------|----------|--------------|----------|
| RAII/Destructors | Created guard | Added move semantics | Tested edge cases |
| Error Codes | Defined enum | Refactored function | Updated callers |
| ISR Safety | Used volatile | No race conditions | Full test coverage |
| TDD | Wrote tests | Red-Green-Refactor | Mocked dependencies |

---

**Congratulations!** You've applied embedded C++ best practices using GitHub Copilot! 🎉

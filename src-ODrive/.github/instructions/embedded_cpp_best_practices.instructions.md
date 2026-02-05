---
name: 'Embedded C++ Best Practices'
description: 'Best practices for embedded C++ programming including memory management, real-time considerations, hardware interaction, and safety-critical patterns for microcontroller firmware development.'
applyTo: '**'
---

# Embedded C++ Best Practices

This document defines best practices for embedded C++ programming on microcontrollers (STM32, ARM Cortex-M).

---

## Memory Management

### Stack vs Heap

| Use | Preference |
|-----|------------|
| **Local variables** | Stack (automatic) — fast, predictable |
| **Fixed-size buffers** | Stack or static — no fragmentation |
| **Dynamic allocation** | Avoid in real-time code — use pools or static allocation |

```cpp
// ✅ GOOD - Stack allocation (predictable)
void processData() {
    uint8_t buffer[256];  // Fixed size on stack
    // ...
}

// ✅ GOOD - Static allocation for persistent data
static MotorController motors[2];

// ⚠️ CAUTION - Heap allocation (use sparingly)
// Only acceptable during initialization, not in real-time loops
auto config = std::make_unique<Config>();

// ❌ BAD - Dynamic allocation in ISR or control loop
void controlLoop() {
    auto* data = new SensorData();  // NEVER in real-time code
}
```

### Avoid Memory Fragmentation

```cpp
// ✅ GOOD - Pre-allocated pools
template<typename T, size_t N>
class ObjectPool {
    std::array<T, N> pool_;
    std::array<bool, N> inUse_{};
public:
    T* acquire();
    void release(T* obj);
};

// ✅ GOOD - Fixed-size containers
std::array<float, 100> samples;  // Not std::vector

// ❌ BAD - Growing containers in embedded
std::vector<float> samples;  // Can fragment heap
samples.push_back(value);    // May reallocate
```

### Static Initialization Order

```cpp
// ⚠️ CAUTION - Static initialization order fiasco
// File A
static Logger logger;

// File B - May initialize before logger!
static Motor motor(logger);  // Undefined behavior possible

// ✅ GOOD - Use function-local statics (Meyers Singleton)
Logger& getLogger() {
    static Logger instance;
    return instance;
}
```

---

## Interrupt Safety

### Volatile for Hardware Registers and Shared Data

```cpp
// ✅ GOOD - Volatile for hardware-modified variables
volatile bool dataReady = false;

void ISR_Handler() {
    dataReady = true;
}

void mainLoop() {
    while (!dataReady) {
        // Wait - volatile ensures re-read
    }
}

// ❌ BAD - Missing volatile
bool dataReady = false;  // Compiler may optimize out re-reads
```

### Atomic Operations

```cpp
// ✅ GOOD - Atomic for shared counters/flags
#include <atomic>
std::atomic<uint32_t> encoderCount{0};

void encoderISR() {
    encoderCount.fetch_add(1, std::memory_order_relaxed);
}

// ✅ GOOD - GCC atomics for lock-free exchange
bool acquired = !__atomic_exchange_n(&inUse, true, __ATOMIC_SEQ_CST);

// ❌ BAD - Non-atomic read-modify-write in ISR context
void badISR() {
    sharedCounter++;  // Race condition!
}
```

### Critical Sections

```cpp
// ✅ GOOD - RAII critical section
class CriticalSection {
public:
    CriticalSection() { __disable_irq(); }
    ~CriticalSection() { __enable_irq(); }
    
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;
};

void safeUpdate() {
    CriticalSection lock;
    // Protected code - interrupts disabled
    sharedData.update();
}  // Interrupts automatically re-enabled

// ✅ GOOD - Scoped critical section macro
#define CRITICAL_SECTION() \
    for (CriticalSection _cs; !_cs.done(); _cs.setDone())
```

### ISR Best Practices

```cpp
// ✅ GOOD - Short ISR, defer work
volatile bool newDataFlag = false;
volatile uint16_t rawAdcValue;

void ADC_IRQHandler() {
    rawAdcValue = ADC1->DR;     // Quick read
    newDataFlag = true;          // Signal main loop
    // NO heavy processing here
}

void mainLoop() {
    if (newDataFlag) {
        newDataFlag = false;
        processAdcValue(rawAdcValue);  // Heavy work here
    }
}

// ❌ BAD - Heavy processing in ISR
void ADC_IRQHandler() {
    float voltage = ADC1->DR * 3.3f / 4096.0f;
    applyFilter(voltage);        // Too slow!
    updatePID(voltage);          // Blocks other interrupts!
}
```

---

## Real-Time Considerations

### Deterministic Timing

```cpp
// ✅ GOOD - Bounded execution time
void controlLoop() {
    // All paths have similar execution time
    float error = target_ - current_;
    float output = kP * error;
    output = std::clamp(output, -1.0f, 1.0f);
    applyOutput(output);
}

// ❌ BAD - Unbounded execution time
void controlLoop() {
    for (auto& item : dynamicList) {  // Unknown size!
        process(item);
    }
}
```

### Avoid Blocking Operations

```cpp
// ✅ GOOD - Non-blocking with timeout
bool waitForReady(uint32_t timeoutMs) {
    uint32_t start = HAL_GetTick();
    while (!isReady()) {
        if (HAL_GetTick() - start > timeoutMs) {
            return false;  // Timeout
        }
    }
    return true;
}

// ❌ BAD - Infinite blocking
void waitForReady() {
    while (!isReady()) {
        // May hang forever!
    }
}
```

### Fixed-Point vs Floating-Point

```cpp
// ✅ GOOD - Use fixed-point when no FPU or for determinism
using Q16_16 = int32_t;  // 16.16 fixed-point

constexpr Q16_16 toFixed(float f) {
    return static_cast<Q16_16>(f * 65536.0f);
}

constexpr float toFloat(Q16_16 q) {
    return static_cast<float>(q) / 65536.0f;
}

// ✅ OK - Floating-point on Cortex-M4F (has FPU)
float velocity = position * dt;  // Fast with hardware FPU
```

---

## Hardware Abstraction

### Encapsulate Hardware Access

```cpp
// ✅ GOOD - Hardware abstraction class
class Stm32Gpio {
public:
    Stm32Gpio(GPIO_TypeDef* port, uint16_t pin);
    
    void write(bool state);
    bool read() const;
    bool config(uint32_t mode, uint32_t pull);
    
private:
    GPIO_TypeDef* port_;
    uint16_t pinMask_;
};

// Usage - hardware details hidden
Stm32Gpio ledPin(GPIOA, GPIO_PIN_5);
ledPin.write(true);

// ❌ BAD - Direct register access scattered everywhere
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
GPIOA->ODR |= (1 << 5);  // Same thing, different style
```

### DMA and Peripheral Management

```cpp
// ✅ GOOD - RAII for peripheral state
class SpiTransaction {
public:
    SpiTransaction(Stm32Gpio& csPin) : csPin_(csPin) {
        csPin_.write(false);  // Assert CS
    }
    
    ~SpiTransaction() {
        csPin_.write(true);   // Deassert CS
    }
    
    SpiTransaction(const SpiTransaction&) = delete;
    
private:
    Stm32Gpio& csPin_;
};

void readSensor() {
    SpiTransaction txn(sensorCs);  // CS asserted
    spi.transfer(cmd);
    spi.transfer(data);
}  // CS automatically deasserted
```

---

## Error Handling for Embedded

### Error Codes Over Exceptions

```cpp
// ✅ GOOD - Error codes (no exception overhead)
enum class MotorError : uint8_t {
    None = 0,
    OverVoltage,
    OverCurrent,
    EncoderFault,
    Timeout
};

MotorError Motor::initialize() {
    if (!encoder_.detect()) {
        return MotorError::EncoderFault;
    }
    if (voltage_ > kMaxVoltage) {
        return MotorError::OverVoltage;
    }
    return MotorError::None;
}

// ❌ BAD - Exceptions in embedded (large overhead)
void Motor::initialize() {
    if (!encoder_.detect()) {
        throw std::runtime_error("Encoder fault");  // Avoid!
    }
}
```

### Fault Handlers

```cpp
// ✅ GOOD - Graceful degradation
void Motor::handleFault(MotorError error) {
    disablePwm();           // Safe state first
    logError(error);        // Record for diagnostics
    notifyHost(error);      // Inform external systems
    enterFaultState();      // Prevent further operation
}

// ✅ GOOD - Watchdog for system health
void initWatchdog() {
    // Configure independent watchdog
    IWDG->KR = 0x5555;      // Enable register access
    IWDG->PR = 4;           // Prescaler
    IWDG->RLR = 1000;       // Reload value
    IWDG->KR = 0xCCCC;      // Start watchdog
}

void kickWatchdog() {
    IWDG->KR = 0xAAAA;      // Reload counter
}
```

---

## Power and Performance

### Compiler Optimization Hints

```cpp
// ✅ GOOD - Branch prediction hints
if (__builtin_expect(errorCondition, 0)) {  // Unlikely
    handleError();
}

// ✅ GOOD - Force inline for critical paths
__attribute__((always_inline))
inline void fastPath() {
    // Time-critical code
}

// ✅ GOOD - Prevent optimization of timing-sensitive code
__attribute__((optimize("O0")))
void delayMicroseconds(uint32_t us) {
    // Precise timing loop
}
```

### Memory Barriers

```cpp
// ✅ GOOD - Ensure memory ordering
void setupDmaTransfer() {
    buffer[0] = command;
    buffer[1] = data;
    
    __DMB();  // Data Memory Barrier - ensure writes complete
    
    DMA1_Stream0->CR |= DMA_SxCR_EN;  // Start DMA
}

// ✅ GOOD - Instruction barrier after system config
void configureInterrupts() {
    NVIC_SetPriority(TIM1_IRQn, 2);
    __ISB();  // Instruction Sync Barrier
}
```

---

## Code Organization

### Minimal Headers in Embedded

```cpp
// ✅ GOOD - Forward declarations reduce compile time
class Motor;
class Encoder;

class Controller {
    Motor* motor_;      // Pointer - forward decl OK
    Encoder* encoder_;
};

// ❌ BAD - Heavy includes in headers
#include "motor.hpp"      // Full definition not needed
#include "encoder.hpp"
#include <algorithm>      // May pull in large headers
```

### Constexpr for Compile-Time Computation

```cpp
// ✅ GOOD - Computed at compile time
constexpr float kPwmFrequency = 20000.0f;
constexpr uint32_t kTimerPeriod = 
    static_cast<uint32_t>(SystemCoreClock / kPwmFrequency);

constexpr uint16_t computeCrc(const uint8_t* data, size_t len);

// ❌ BAD - Runtime computation of constants
const uint32_t timerPeriod = SystemCoreClock / 20000;  // Runs at startup
```

---

## Safety-Critical Patterns

### Defensive Programming

```cpp
// ✅ GOOD - Validate all inputs
bool Motor::setSpeed(float speed) {
    if (std::isnan(speed) || std::isinf(speed)) {
        return false;
    }
    if (speed < kMinSpeed || speed > kMaxSpeed) {
        return false;
    }
    speed_ = speed;
    return true;
}

// ✅ GOOD - Assert invariants in debug builds
void updateControl() {
    assert(isInitialized_);
    assert(encoderValid_);
    // ...
}
```

### State Machine Safety

```cpp
// ✅ GOOD - Explicit state transitions with validation
enum class State { Idle, Armed, Running, Fault };

bool Motor::transitionTo(State newState) {
    // Validate transition is allowed
    switch (state_) {
        case State::Idle:
            if (newState != State::Armed) return false;
            break;
        case State::Armed:
            if (newState != State::Running && 
                newState != State::Idle) return false;
            break;
        case State::Running:
            if (newState != State::Idle && 
                newState != State::Fault) return false;
            break;
        case State::Fault:
            if (newState != State::Idle) return false;
            break;
    }
    state_ = newState;
    return true;
}
```

---

## Summary Checklist

### Memory
- [ ] No dynamic allocation in real-time code paths
- [ ] Fixed-size containers preferred over dynamic
- [ ] Stack usage analyzed and bounded

### Interrupts
- [ ] Volatile used for hardware/ISR-shared variables
- [ ] Atomics or critical sections for shared data
- [ ] ISRs are short, defer heavy work to main loop

### Real-Time
- [ ] All code paths have bounded execution time
- [ ] No blocking waits without timeouts
- [ ] Watchdog enabled for fault recovery

### Hardware
- [ ] Hardware access abstracted into classes
- [ ] RAII used for peripheral state management
- [ ] DMA/peripheral configurations verified

### Safety
- [ ] All inputs validated
- [ ] Error codes used (not exceptions)
- [ ] Fault states defined and handled
- [ ] State transitions validated

---

*This guide defines best practices for embedded C++ development on microcontrollers.*

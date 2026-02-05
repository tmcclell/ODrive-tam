---
applyTo: "**/*.h,**/*.hpp"
description: "C++ header file standards for ODrive firmware. Apply when writing or reviewing header files (.h, .hpp)."
---

# C++ Header File Standards

This document defines coding standards for C++ header files (`.hpp`, `.h`).

---

## Include Guards

- Use `#pragma once` at the very top of every header file.
- For compatibility with older compilers, you may also add traditional guards as a fallback.

```cpp
// ✅ GOOD - Pragma once (preferred)
#pragma once

// ✅ ALSO ACCEPTABLE - Pragma once with fallback guards
#pragma once
#ifndef ODRIVE_MOTOR_CONTROLLER_HPP_
#define ODRIVE_MOTOR_CONTROLLER_HPP_

// ... content ...

#endif // ODRIVE_MOTOR_CONTROLLER_HPP_
```

### Guard Naming Convention
If using traditional guards, follow this format:
- `PROJECTNAME_PATH_FILENAME_HPP_`
- All uppercase with underscores
- Include full path to ensure uniqueness

```cpp
// File: MotorControl/encoder.hpp
#ifndef ODRIVE_MOTORCONTROL_ENCODER_HPP_
#define ODRIVE_MOTORCONTROL_ENCODER_HPP_
```

---

## Header File Structure

Follow this organization for all header files:

```cpp
#pragma once

// ============================================================================
// 1. File header comment
// ============================================================================
/**
 * @file motor_controller.hpp
 * @brief Motor control interface with FOC support
 *
 * @copyright Copyright (c) ODrive Robotics
 */

// ============================================================================
// 2. System includes (alphabetical)
// ============================================================================
#include <cstdint>
#include <memory>
#include <optional>

// ============================================================================
// 3. Third-party includes (alphabetical)
// ============================================================================
#include <arm_math.h>

// ============================================================================
// 4. Project includes (alphabetical)
// ============================================================================
#include "component.hpp"
#include "utils.hpp"

// ============================================================================
// 5. Forward declarations
// ============================================================================
class Axis;
class GateDriver;

// ============================================================================
// 6. Namespace
// ============================================================================
namespace odrive {

// ============================================================================
// 7. Constants and type aliases
// ============================================================================
constexpr float kDefaultGain = 1.0f;

using MotorCallback = std::function<void(ErrorCode)>;

// ============================================================================
// 8. Enumerations
// ============================================================================
enum class ControlMode : uint8_t {
    Voltage,
    Current,
    Velocity,
    Position
};

// ============================================================================
// 9. Class declaration
// ============================================================================
/**
 * @class MotorController
 * @brief Controls motor with multiple control modes
 */
class MotorController {
public:
    // -- Public types --
    struct Config_t {
        float gain = 1.0f;
        ControlMode mode = ControlMode::Voltage;
    };

    // -- Constructors/Destructor --
    MotorController() = default;
    explicit MotorController(const Config_t& config);
    ~MotorController() = default;

    // -- Copy/Move semantics --
    MotorController(const MotorController&) = delete;
    MotorController& operator=(const MotorController&) = delete;
    MotorController(MotorController&&) = default;
    MotorController& operator=(MotorController&&) = default;

    // -- Getters --
    [[nodiscard]] float getSpeed() const;
    [[nodiscard]] bool isEnabled() const { return isEnabled_; }

    // -- Setters --
    void setSpeed(float speed);
    void setMode(ControlMode mode);

    // -- Actions --
    bool initialize();
    void update();

private:
    // -- Private methods --
    void updateInternal();

    // -- Member variables --
    float speed_ = 0.0f;
    bool isEnabled_ = false;
    Config_t config_;
};

}  // namespace odrive
```

---

## Includes

### Include What You Use (IWYU)
- Only include headers necessary for the declarations in the file.
- Use forward declarations where possible to minimize dependencies.

```cpp
// ✅ GOOD - Forward declaration for pointer/reference
#pragma once

class Motor;  // Forward declaration sufficient

class Controller {
    Motor* motor_;           // Pointer - forward decl OK
    const Motor& motorRef_;  // Reference - forward decl OK
};
```

```cpp
// ❌ BAD - Full include when not needed
#pragma once

#include "motor.hpp"  // Heavy include

class Controller {
    Motor* motor_;  // Only using pointer, forward decl sufficient
};
```

### When Full Include Is Required
You MUST include the full header when:
- Declaring a member by value (not pointer/reference)
- Inheriting from a class
- Using methods or sizeof() on the type
- Using the type in inline/template code

```cpp
#pragma once

#include "motor.hpp"    // Required: member by value
#include "base.hpp"     // Required: inheritance

class Controller : public Base {  // Needs Base definition
    Motor motor_;                  // Needs Motor definition
};
```

### Include Order
1. Related header (for .cpp files)
2. C system headers (`<cstdint>`, `<cmath>`)
3. C++ standard library (`<vector>`, `<memory>`)
4. Third-party libraries
5. Project headers

Separate each group with a blank line. Alphabetize within groups.

```cpp
// In motor_controller.cpp
#include "motor_controller.hpp"

#include <cmath>
#include <cstdint>

#include <algorithm>
#include <memory>
#include <vector>

#include <arm_math.h>

#include "encoder.hpp"
#include "utils.hpp"
```

---

## Namespace Pollution

- **NEVER** use `using namespace` in a header file at global scope.
- It forces that namespace on every file that includes the header.

```cpp
// ❌ NEVER DO THIS IN A HEADER
#pragma once
using namespace std;  // Pollutes all includers!

class Motor {
    string name_;  // Forces std:: on everyone
};
```

```cpp
// ✅ GOOD - Explicit namespace
#pragma once
#include <string>

class Motor {
    std::string name_;  // Explicit and safe
};
```

### Acceptable Uses
- `using` declarations inside function bodies
- `using` declarations inside class scope (for type aliases)
- `using` in .cpp files (not headers)

```cpp
// ✅ OK - Type alias in class scope
class Motor {
public:
    using Callback = std::function<void(ErrorCode)>;
};

// ✅ OK - Inside function body
void process() {
    using std::vector;
    vector<int> data;
}
```

---

## Inline Implementation

Keep implementation **out of headers** unless:
- It is a **template** (must be in header)
- It is a **constexpr** function
- It is a **trivial getter/setter** (1-2 lines)

```cpp
class Encoder {
public:
    // ✅ OK - Trivial getter in header
    [[nodiscard]] int getCpr() const { return cpr_; }
    
    // ✅ OK - Trivial setter in header
    void setCpr(int cpr) { cpr_ = cpr; }

    // ✅ OK - Constexpr in header
    static constexpr float getCoggingRatio() { return 1.0f / 3600.0f; }

    // ❌ Move to .cpp - Complex logic
    void calibrate();
    bool runOffsetCalibration();

private:
    int cpr_ = 4096;
};
```

### Template Functions
Templates must be in headers (or explicitly instantiated in .cpp):

```cpp
// ✅ OK - Template in header
template<typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
```

---

## Forward Declarations

Use forward declarations to reduce compile-time dependencies.

### When to Forward Declare

| Use Case | Forward Declare? |
|----------|------------------|
| Pointer member (`Motor*`) | ✅ Yes |
| Reference member (`Motor&`) | ✅ Yes |
| Function parameter (`void foo(Motor*)`) | ✅ Yes |
| Function return type (`Motor* get()`) | ✅ Yes |
| Value member (`Motor motor_`) | ❌ No - need full include |
| Inheritance (`class Foo : Motor`) | ❌ No - need full include |
| Calling methods (`motor.update()`) | ❌ No - need full include |

```cpp
// ✅ GOOD - Forward declarations reduce dependencies
#pragma once

class Axis;
class Encoder;
class GateDriver;

class Motor {
public:
    void setAxis(Axis* axis);
    Encoder* getEncoder();
    
private:
    Axis* axis_ = nullptr;
    Encoder* encoder_ = nullptr;
    GateDriver* driver_ = nullptr;
};
```

---

## Self-Contained Headers

Every header must compile on its own. Include all dependencies.

```cpp
// ✅ GOOD - Self-contained
#pragma once

#include <cstdint>
#include <string>

class Motor {
    std::string name_;
    uint32_t id_;
};
```

```cpp
// ❌ BAD - Relies on includer to provide dependencies
#pragma once

class Motor {
    std::string name_;  // Error if <string> not included first
    uint32_t id_;       // Error if <cstdint> not included first
};
```

### Testing Self-Containment
Each header should be the first include in its corresponding .cpp file:

```cpp
// motor.cpp
#include "motor.hpp"  // FIRST - tests self-containment

#include <algorithm>
// ... other includes
```

---

## Summary Checklist

Before committing a header file:
- [ ] Uses `#pragma once`
- [ ] Follows standard structure (file header, includes, forward decls, namespace, class)
- [ ] Includes only what is needed (IWYU)
- [ ] Uses forward declarations where possible
- [ ] No `using namespace` at global scope
- [ ] Implementation in .cpp unless template/constexpr/trivial
- [ ] Self-contained (compiles on its own)
- [ ] Getters use camelCase (`getSpeed()`, not `get_speed()`)
- [ ] Uses `[[nodiscard]]` for getters returning values

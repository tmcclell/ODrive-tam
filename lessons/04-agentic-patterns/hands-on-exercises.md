# Section 4: Hands-On Exercises

**Duration:** 12 minutes  
**Format:** Individual or pair programming  
**Goal:** Practice agentic development with real ODrive code

---

## Exercise Overview

You'll complete **ONE** of three exercises based on your interest:

| Exercise | Focus Area | Difficulty |
|----------|------------|------------|
| 1. Motor Diagnostics | Data structures, embedded patterns | ⭐⭐ Medium |
| 2. Safety Watchdog | Real-time systems, fault handling | ⭐⭐⭐ Advanced |
| 3. Configuration Manager | Software architecture, persistence | ⭐ Beginner-Medium |

---

## Exercise 1: Motor Diagnostics Module

### Scenario
Your customer wants telemetry data for predictive maintenance. Add a diagnostics system to track motor health metrics.

### Requirements

**Data to Track:**
- Total runtime (hours)
- Total revolutions
- Average power consumption (watts)
- Peak temperature reached
- Fault history (last 10 faults with timestamps)
- Number of emergency stops
- Total distance traveled (if encoder present)

**Constraints:**
- Static memory allocation only (no malloc/free)
- Must work in 8kHz interrupt context
- Accessible via USB and CAN protocols
- Persistent across reboots (store in EEPROM)
- MISRA C++ compliant

### Files to Modify
- `src-ODrive/Firmware/MotorControl/motor.hpp` - Add diagnostics struct
- `src-ODrive/Firmware/MotorControl/motor.cpp` - Implement tracking logic
- `src-ODrive/Firmware/MotorControl/axis.cpp` - Wire up fault logging

### Step-by-Step Guide

**Step 1: Design the Data Structure (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Design a diagnostics data structure for the Motor class.

Context:
- File: src-ODrive/Firmware/MotorControl/motor.hpp
- Must track: runtime hours, revolutions, avg power, peak temp, fault history
- Use static memory (no dynamic allocation)
- Fault history: circular buffer, last 10 entries
- Each fault entry needs: error code, timestamp, motor state

Show me:
1. The struct definition
2. Storage size calculation
3. How to initialize it safely
```

**Step 2: Implement Runtime Tracking (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement runtime tracking in the motor control loop.

Context:
- File: src-ODrive/Firmware/MotorControl/motor.cpp
- Control loop runs at 8kHz (every 125 microseconds)
- Motor is "running" when current > 0.1A
- Update diagnostics.runtime_hours incrementally
- Also count total revolutions using encoder position

Where should this code go in motor.cpp? Show me the implementation.
```

**Step 3: Add Fault Logging (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement fault history logging.

Context:
- File: src-ODrive/Firmware/MotorControl/axis.cpp
- When axis.error_ is set (non-zero), log it to diagnostics
- Use circular buffer pattern (overwrite oldest when full)
- Capture: error code, timestamp (microseconds), motor current, motor temp
- Must be interrupt-safe

Show me:
1. The circular buffer implementation
2. How to log a fault from axis.cpp
3. How to retrieve fault history
```

**Step 4: Verify and Document (3 min)**

> Select **ODrive QA** from agent dropdown, then paste:

```
Review the motor diagnostics implementation for:
1. Race conditions in interrupt context
2. Potential integer overflows
3. Memory safety
4. Suggest unit tests
```

Then select **ODrive Engineer** and paste:

```
Add Doxygen comments to all public methods.
```

> **Note:** ODrive QA will invoke the `cpp-testing` skill to analyze code and suggest tests.

### Success Criteria
- ✅ Diagnostics struct compiles without warnings
- ✅ No dynamic memory allocation
- ✅ Interrupt-safe implementation
- ✅ Circular buffer works correctly
- ✅ Documentation complete

---

## Exercise 2: Safety Watchdog System

### Scenario
Implement a safety watchdog that monitors motor operation and triggers emergency stop if conditions are unsafe.

### Requirements

**Monitor These Conditions:**
- Motor overcurrent (>120% of rated)
- Motor overtemperature (>85°C)
- Encoder position mismatch (hall vs. incremental)
- Communication timeout (no commands for >1 second)
- Supply voltage out of range
- Control loop timing violations

**Watchdog Behavior:**
- Check conditions every 1ms
- If any condition triggers: immediate safe stop
- Log the fault cause
- Require explicit reset to clear
- Configurable threshold per condition

### Files to Create/Modify
- `src-ODrive/Firmware/MotorControl/safety_watchdog.hpp` (new)
- `src-ODrive/Firmware/MotorControl/safety_watchdog.cpp` (new)
- `src-ODrive/Firmware/MotorControl/axis.cpp` - integrate watchdog

### Step-by-Step Guide

**Step 1: Design the Watchdog Interface (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Design a SafetyWatchdog class for motor protection.

Context:
- File: src-ODrive/Firmware/MotorControl/safety_watchdog.hpp
- Monitors 6 different fault conditions (listed above)
- Runs in 1ms timer task (FreeRTOS)
- Needs access to motor state (current, temp, encoder)
- Can trigger emergency stop via axis->set_error()

Show me:
1. Class interface (hpp file)
2. Configuration struct for thresholds
3. Public API: update(), reset(), get_status()
```

**Step 2: Implement Fault Detection (4 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement the fault detection logic in SafetyWatchdog::update()

Context:
- File: src-ODrive/Firmware/MotorControl/safety_watchdog.cpp
- Access motor data via motor_ reference
- Check all 6 conditions
- Use configurable thresholds (don't hardcode)
- Add hysteresis to prevent chattering
- Log which condition triggered

Requirements:
- Must complete in <100 microseconds
- No floating point division in critical path
- Set axis error if fault detected

Show implementation of update() method.
```

**Step 3: Integration and Testing (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Show me how to integrate SafetyWatchdog into axis.cpp

Context:
- File: src-ODrive/Firmware/MotorControl/axis.cpp
```

Then select **ODrive QA** from dropdown and paste:

```
Suggest:
1. Unit tests for each fault condition
2. Test rig configuration
3. How to verify watchdog timing (doesn't exceed 100us)
```

> **Note:** ODrive QA invokes `cpp-testing` skill to generate comprehensive test suggestions.

**Step 4: Configuration Interface (2 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Add watchdog configuration to axis.config_

Context:
- File: src-ODrive/Firmware/odrive-interface.yaml

Show:
1. How to add to ODrive protocol (odrive-interface.yaml)
2. Default values for thresholds
3. How to enable/disable watchdog at runtime
```

### Success Criteria
- ✅ Watchdog detects all 6 fault types
- ✅ Timing requirement met (<100us)
- ✅ Configurable via USB/CAN
- ✅ False positive rate is minimal (hysteresis working)
- ✅ Integration tests pass

---

## Exercise 3: Configuration Manager

### Scenario
ODrive configuration is scattered across multiple files. Create a unified configuration manager with validation and persistence.

### Requirements

**Features:**
- Centralized config validation
- Load/save from NVM (non-volatile memory)
- JSON import/export
- Configuration versioning (migration support)
- Validate inter-dependent settings
- Rollback to last-known-good on validation failure

**Configuration to Manage:**
- Motor parameters (resistance, inductance, poles)
- Control gains (PID tuning)
- Safety limits (current, voltage, temp)
- Encoder settings
- CAN/UART settings

### Files to Create/Modify
- `src-ODrive/Firmware/MotorControl/config_manager.hpp` (new)
- `src-ODrive/Firmware/MotorControl/config_manager.cpp` (new)
- `src-ODrive/tools/odrive/configuration.py` - add JSON import/export

### Step-by-Step Guide

**Step 1: Design Configuration Schema (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Design a ConfigManager class for ODrive.

Context:
- File: src-ODrive/Firmware/MotorControl/config_manager.hpp
- Needs to manage all axis/motor/encoder configuration
- Must validate interdependencies (e.g., current limit vs motor rating)
- Support save/load to NVM
- Support JSON export for backup/sharing
- Version the config format for migration

Show me:
1. Class interface
2. Validation framework
3. How to handle config versioning
```

**Step 2: Implement Validation Logic (4 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement configuration validation.

Context:
- File: src-ODrive/Firmware/MotorControl/config_manager.cpp

Validation Rules:
1. motor.current_lim <= motor.current_lim_max (hardware limit)
2. controller.vel_limit > 0
3. encoder.cpr must be power of 2 (for some encoder types)
4. controller gains: kp > 0, ki >= 0, kd >= 0
5. CAN ID must be unique and in range 0-63

Show:
1. validate_config() method
2. Error reporting (which validation failed?)
3. How to atomically apply config (all-or-nothing)
```

**Step 3: Persistence Layer (3 min)**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement save/load to NVM.

Context:
- File: src-ODrive/Firmware/MotorControl/config_manager.cpp
- ODrive uses STM32 internal flash for config storage
- Config size: ~4KB
- Must handle flash wear leveling
- Add CRC for corruption detection
- Implement factory reset

Show:
1. save_to_nvm() implementation
2. load_from_nvm() with CRC check
3. factory_reset() method
```

**Step 4: Python Tooling (2 min)**

> With regular Copilot (not custom agent) since this is Python tooling, paste:

```
Update src-ODrive/tools/odrive/configuration.py to support JSON import/export.

Requirements:
- Export current config to JSON file
- Import JSON and validate before applying
- Human-readable format
- Include config version number

Show the Python implementation.
```

### Success Criteria
- ✅ All validation rules enforce correctly
- ✅ Bad config rejected before apply
- ✅ NVM save/load works with CRC
- ✅ JSON export/import functional
- ✅ Factory reset clears to defaults

---

## General Tips for All Exercises

### Context Engineering Checklist
- [ ] Reference specific files and line numbers
- [ ] Specify memory constraints (static allocation, size limits)
- [ ] Mention timing requirements (interrupt context, max execution time)
- [ ] Include coding standards (MISRA C++, naming conventions)
- [ ] Reference existing patterns to follow

### Decomposition Strategy
1. Start with interface/API design
2. Implement core logic
3. Add error handling and edge cases
4. Integrate with existing code
5. Add tests and documentation

### Agent Selection Guide
- **ODrive Engineer** - Firmware, hardware, control algorithms (routes to appropriate skills)
- **ODrive QA** - Testing strategy, bug verification, build validation (invokes cpp-testing)
- **Regular Copilot** - Python tools, documentation, non-embedded code

> **Key Insight:** Same agent (**ODrive Engineer**) handles multiple domains based on your prompt context. Add focus (e.g., "hardware constraints", "control theory", "firmware implementation") to guide the response.

### Iterative Refinement
- Don't expect perfect code on first generation
- Review, identify issues, provide specific feedback
- Use `/fix` for compile errors
- Use `/explain` to understand generated code

---

## Presenter Notes

### Time Management
- Announce "2 minutes remaining" to help participants wrap up
- It's OK if participants don't finish - goal is to practice the workflow

### Circulate and Help
- Look for participants struggling with context engineering
- Point them to the step-by-step prompts
- Remind them to use the agents (`@firmware-engineer`)

### Common Issues
**"My code doesn't compile"**
→ Use `/fix` or provide more context about the error. Or ask **ODrive QA** to verify the build.

**"I don't know which agent to use"**
→ Use **ODrive Engineer** for development tasks and **ODrive QA** for testing/validation. Add context to specify the domain focus (firmware, hardware, control).

**"This is taking too long"**
→ Focus on Steps 1-2, skip the rest. The goal is learning the process.

### Debrief Questions (After Exercise)
- "Who got through all steps?"
- "What was the hardest part?"
- "How did agents help vs. regular Copilot?"
- "Would you use this approach on real work?"

---

## Solution Examples

Full solution code is available in `solutions/` folder:
- `solutions/exercise1-motor-diagnostics/`
- `solutions/exercise2-safety-watchdog/`
- `solutions/exercise3-config-manager/`

Use these for reference or if participants want to see completed examples.

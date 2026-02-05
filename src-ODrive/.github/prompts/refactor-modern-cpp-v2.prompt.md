# Refactor to Modern C++

Refactor firmware code to follow modern C++17 best practices and coding standards.

## Instructions

Use the **ODrive-Engineer** agent with **odrive-toolchain** skill to refactor code while maintaining compatibility.

### Input Required
- **File or Directory**: ${{TARGET_PATH}} (e.g., "Firmware/MotorControl/motor.cpp")
- **Refactoring Focus**: ${{FOCUS}} (e.g., "smart pointers", "range-based for", "const correctness")

### What This Prompt Does

1. **Analyzes existing code** for C++17 compliance
2. **Identifies modernization opportunities**:
   - Replace raw pointers with smart pointers
   - Use auto appropriately
   - Apply range-based for loops
   - Add const correctness
   - Use uniform initialization
   - Apply structured bindings
3. **Refactors code** following [cpp_coding_standards.instructions.md](../instructions/cpp_coding_standards.instructions.md)
4. **Validates changes** with build and tests
5. **Ensures backward compatibility**

### Commands

```bash
# Build firmware before refactoring (baseline)
cd Firmware && make CONFIG=board-v3.6-56V

# Run tests to verify functionality
python tools/run_tests.py

# Search for specific patterns to refactor
grep -r "new " Firmware/MotorControl/  # Find raw pointers
grep -r "for (int i" Firmware/         # Find index-based loops

# Use odrive-toolchain to build after changes
cd Firmware && make CONFIG=board-v3.6-56V
```

### Prerequisites

- Read [cpp_coding_standards.instructions.md](../instructions/cpp_coding_standards.instructions.md)
- Firmware builds successfully
- Unit tests pass

### Example Usage

Refactor motor control code:
```
@odrive-engineer Refactor Firmware/MotorControl/motor.cpp to use smart pointers instead of raw pointers
```

Modernize entire module:
```
@odrive-engineer Refactor all of Firmware/MotorControl/ to modern C++17 - focus on const correctness and auto usage
```

Apply specific pattern:
```
@odrive-engineer Convert index-based for loops in axis.cpp to range-based for loops
```

### Refactoring Checklist

Before refactoring:
- [ ] Code builds successfully
- [ ] All tests pass
- [ ] Understand code functionality

Refactoring targets:
- [ ] Replace raw pointers with `std::unique_ptr` or `std::shared_ptr`
- [ ] Use `auto` for verbose type declarations
- [ ] Convert to range-based for loops where appropriate
- [ ] Add `const` to methods that don't modify state
- [ ] Use uniform initialization `{}`
- [ ] Apply `override` keyword to virtual methods
- [ ] Replace magic numbers with named constants
- [ ] Add `[[nodiscard]]` to important return values

After refactoring:
- [ ] Code builds without warnings
- [ ] All tests still pass
- [ ] Code follows naming conventions
- [ ] Documentation updated
- [ ] Performance not degraded

### Output

- Refactored code following modern C++17 standards
- Build verification results
- Test results confirming functionality preserved
- List of changes made
- Performance impact assessment (if applicable)

### Skills Invoked

| Skill | Purpose |
|-------|---------|
| **odrive-toolchain** | Build firmware, run tests, verify changes |
| **ODrive-Engineer** | Refactor code, apply modern patterns |

### Related Prompts

| Need | Use |
|------|-----|
| Add documentation | `add-doxygen.prompt.md` |
| Generate tests | `generate-doctest.prompt.md` |
| Optimize performance | `optimize-critical.prompt.md` |
| Check safety | `check-safety.prompt.md` |

### Coding Standards Reference

**Must Read Before Refactoring:**
- [cpp_coding_standards.instructions.md](../instructions/cpp_coding_standards.instructions.md) - C++ style guide
- [header_file_rules.instructions.md](../instructions/header_file_rules.instructions.md) - Header best practices
- [embedded_cpp_best_practices.instructions.md](../instructions/embedded_cpp_best_practices.instructions.md) - Embedded C++ best practices

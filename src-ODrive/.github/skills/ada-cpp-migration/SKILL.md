# Ada to C++ Migration Skill

**Skill Name:** ada-cpp-migration  
**Version:** 1.0.0  
**Purpose:** Migrate Ada embedded systems to Modern C++ (C++20/23) with safety preservation

---

## Overview

This skill provides a complete methodology for migrating Ada projects to Modern C++ while preserving:
- **Type safety** via strong types and range constraints
- **Concurrency guarantees** via mutex wrappers and task abstractions
- **Real-time behavior** via accurate timing primitives
- **Embedded constraints** via static allocation patterns

---

## Skill Contents

```
ada-cpp-migration/
├── SKILL.md                          # This file - skill definition
├── references/
│   ├── migration-plan.md             # Complete 9-phase migration plan
│   ├── ada-cpp-patterns.md           # Quick pattern mapping reference
│   ├── type-mapping.md               # Detailed type conversion guide
│   ├── concurrency-patterns.md       # Task/protected object mappings
│   └── error-handling.md             # Exception to error code patterns
├── templates/
│   ├── strong_type.hpp               # Reusable strong type library
│   ├── range_type.hpp                # Range-constrained type wrapper
│   ├── protected_object.hpp          # Protected object base class
│   ├── cyclic_task.hpp               # Cyclic task wrapper
│   ├── serial_port.hpp               # Stream I/O replacement
│   ├── fault_handler.hpp             # Last chance handler equivalent
│   └── CMakeLists.txt                # Build system template
└── prompts/
    └── (see .github/prompts/ for migration-specific prompts)
```

---

## How to Use This Skill

### 1. Project Analysis

Before any migration, analyze the Ada project:

```
@ada-to-cpp-migrator Analyze Ada project at ${input:project_path}

Expected output:
- Dependency graph
- Module migration order
- Complexity assessment per module
- Concurrency inventory (tasks, protected objects)
- Type catalog (strong types, ranges, derived types)
```

### 2. Foundation Setup

Set up the C++ project structure:

```
@ada-to-cpp-migrator Initialize C++ project structure for Ada migration

Creates:
- CMakeLists.txt with C++23, cross-compilation
- include/ and src/ directories
- Foundation libraries (strong_type, realtime, concurrency)
- Test framework setup
```

### 3. Module-by-Module Migration

Migrate each module following the dependency order:

```
@ada-to-cpp-migrator Migrate Ada module: ${input:module_name}

For each module, generates:
- Header file with types and declarations
- Source file with implementations
- Test file with unit tests
- Migration notes documenting mappings
```

### 4. Verification

Verify the migrated code:

```
@ada-to-cpp-migrator Verify migration of ${input:module_name}

Checks:
- Compiles without warnings
- Static analysis passes
- Unit tests pass
- Behavioral equivalence documented
```

---

## Migration Order

Always migrate **bottom-up** following the dependency graph:

```
┌─────────────────────────────────────────────────────────────────┐
│ Level 0 (No Dependencies) - Migrate First                       │
├─────────────────────────────────────────────────────────────────┤
│  hk_data.hpp     │  ttc_data.hpp    │  strong_types.hpp         │
└─────────────────────────────────────────────────────────────────┘
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Level 1 (HAL Only)                                              │
├─────────────────────────────────────────────────────────────────┤
│  sensors.hpp     │  serial_port.hpp │  realtime/clock.hpp       │
└─────────────────────────────────────────────────────────────────┘
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Level 2 (Uses Level 0-1)                                        │
├─────────────────────────────────────────────────────────────────┤
│  storage.hpp     │  ttc.hpp         │  cyclic_task.hpp          │
└─────────────────────────────────────────────────────────────────┘
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Level 3 (Task Packages)                                         │
├─────────────────────────────────────────────────────────────────┤
│  housekeeping.hpp │ basic_tm.hpp   │ hk_tm.hpp │ tc_receiver.hpp│
└─────────────────────────────────────────────────────────────────┘
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│ Level 4 (Main Entry Point)                                      │
├─────────────────────────────────────────────────────────────────┤
│  main.cpp (replaces main Ada procedure)                         │
└─────────────────────────────────────────────────────────────────┘
```

---

## Key Ada to C++ Mappings

| Ada Concept | C++ Equivalent | Template |
|-------------|----------------|----------|
| `type X is new Y` | `StrongType<Y, XTag>` | `strong_type.hpp` |
| `type X is range A..B` | `RangeType<T, A, B>` | `range_type.hpp` |
| `task T with Priority` | `CyclicTask` | `cyclic_task.hpp` |
| `protected P` | Mutex-wrapped class | `protected_object.hpp` |
| `delay until T` | `std::this_thread::sleep_until` | — |
| `X'Output(S, V)` | `port.write_object(v)` | `serial_port.hpp` |
| `Ada.Real_Time` | `std::chrono` | — |
| `Last_Chance_Handler` | `fault_handler` | `fault_handler.hpp` |

---

## Constraints Enforced

All generated C++ code MUST follow these constraints:

### Memory Safety
- ❌ No `new`/`delete` or `malloc`/`free`
- ✅ Use `std::array`, stack allocation, static storage

### Exception Handling (Embedded)
- ❌ No `throw`/`try`/`catch`
- ✅ Use `std::expected<T,E>` or error codes

### Type Safety
- ❌ No raw `int`/`float` for domain values
- ✅ Use strong type wrappers

### Concurrency
- ❌ No unprotected shared state
- ✅ All shared data wrapped in mutex

### Modern C++
- ✅ C++20/23 features (`concepts`, `ranges`, `std::expected`)
- ✅ `constexpr` for compile-time constants
- ✅ `[[nodiscard]]` for important return values
- ✅ `const` correctness everywhere

---

## Verification Checklist

For each migrated module:

- [ ] Compiles with `-Wall -Wextra -Werror -pedantic`
- [ ] No clang-tidy warnings
- [ ] No cppcheck warnings
- [ ] Unit tests pass (>80% coverage)
- [ ] Thread safety validated
- [ ] Timing behavior matches Ada original
- [ ] Documentation complete (Doxygen)

---

## Related Prompts

| Task | Prompt File |
|------|-------------|
| Analyze Ada project | `analyze-ada-project.prompt.md` |
| Migrate type definitions | `migrate-ada-types.prompt.md` |
| Migrate protected object | `migrate-ada-protected.prompt.md` |
| Migrate cyclic task | `migrate-ada-task.prompt.md` |
| Generate unit tests | `generate-migration-tests.prompt.md` |

---

## Example Session

```bash
# 1. Analyze the project
@ada-to-cpp-migrator Analyze Ada project at ~/projects/Embedded_Ada_Examples

# 2. Set up C++ project structure
@ada-to-cpp-migrator Initialize C++ project for migration output at ~/projects/Embedded_Cpp_Migrated

# 3. Migrate foundation types (Level 0)
@ada-to-cpp-migrator Migrate hk_data.ads to C++ strong types

# 4. Migrate protected objects (Level 2)
@ada-to-cpp-migrator Migrate Storage protected object from storage.ads

# 5. Migrate tasks (Level 3)
@ada-to-cpp-migrator Migrate Housekeeping task with timing parameters

# 6. Migrate main entry point (Level 4)
@ada-to-cpp-migrator Create main.cpp that starts all tasks

# 7. Verify
@ada-to-cpp-migrator Verify complete migration and run tests
```

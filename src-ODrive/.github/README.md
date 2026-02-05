# ODrive Development System

**Agentic Workflow & Spec-Driven Development Framework**

This directory contains the intelligent agent system for ODrive firmware and tools development. It provides specialized AI assistants for firmware engineering, motor control, hardware design, and operations.

---

## 🏗️ System Architecture

```
📋 instructions/*.instructions.md (Coding Standards)
        ↓
  🤖 [Primary Orchestrator - GitHub Copilot]
        ↓
  ┌─────────────┬──────────────────┬─────────────────┐
  ↓             ↓                  ↓                 ↓
ODrive-       ODrive-            ODrive-          [Future]
Engineer      Toolchain          Ops
  ↓             ↓                  ↓
Skills:      Skill:             Skill:
• control-   odrive-toolchain   odrive-ops
  algorithms (🚧)
• foc-tuning (🚧)
• sensorless (🚧)
• pcb-review (🚧)
• signal-int (🚧)
```

---

## 📂 Directory Structure

```
.github/
├── agents/                           # 🤖 Orchestrator agents (3)
│   ├── ODrive-Engineer.agent.md        # Development orchestrator
│   ├── ODrive-Toolchain.agent.md       # Build/test/search agent
│   └── ODrive-Ops.agent.md             # CI/CD/releases agent
│
├── skills/                           # 🔧 Reusable operational skills
│   ├── odrive-toolchain/               # Build, test, symbol search
│   │   ├── SKILL.md
│   │   ├── build_firmware.py
│   │   ├── search_symbol.py
│   │   ├── list_errors.py
│   │   ├── setup-env.ps1
│   │   └── run_tests.ps1
│   ├── odrive-ops/                     # CI/CD, releases
│   │   └── SKILL.md
│   └── [stub skills for future...]
│
├── instructions/                     # 📖 Coding standards & guidelines
│   ├── cpp_coding_standards.instructions.md
│   ├── header_file_rules.instructions.md
│   ├── embedded_cpp_best_practices.instructions.md
│   └── python_coding_standards.instructions.md
│
├── prompts/                          # 💡 Common development workflows
│   ├── build-firmware.prompt.md
│   ├── toolchain.prompt.md
│   ├── ops.prompt.md
│   └── ... (more prompts)
│
└── workflows/                        # ⚙️ GitHub Actions CI/CD
    └── firmware.yaml
```

---

## 🤖 Agent Responsibilities

### 1. ODrive-Engineer Agent (Development)
**File:** `agents/ODrive-Engineer.agent.md`  
**Domains:** Firmware, Motor Control, Hardware

**Responsibilities:**
- Implement firmware features and drivers (STM32, FreeRTOS)
- Design and tune motor control algorithms (FOC, PID, observers)
- Analyze hardware interfaces (PCB, signal integrity, power electronics)
- Protocol implementation (USB, CAN, UART, SPI, I2C)
- Code optimization and refactoring

**Skills Invoked:**
- 🚧 `control-algorithms` - Control patterns (planned)
- 🚧 `foc-tuning` - Auto-tuning procedures (planned)
- 🚧 `sensorless-control` - Observer implementations (planned)
- 🚧 `pcb-review` - PCB design review (planned)
- 🚧 `signal-integrity` - Signal analysis (planned)

---

### 2. ODrive-Toolchain Agent (Build/Test)
**File:** `agents/ODrive-Toolchain.agent.md`  
**Domains:** Build, Test, Search, Error Inspection

**Responsibilities:**
- Build firmware for board variants
- Run unit tests
- Search for symbols and definitions
- List and explain error codes

**Skill:** `odrive-toolchain`

---

### 3. ODrive-Ops Agent (CI/CD)
**File:** `agents/ODrive-Ops.agent.md`  
**Domains:** CI/CD, Releases, Deployments

**Responsibilities:**
- Check CI pipeline status
- Create and publish releases
- Deploy documentation
- Manage GitHub Actions workflows

**Skill:** `odrive-ops`

---

## 🔧 Skills Library

Skills are reusable modules that agents invoke for specific operational tasks.

### ✅ Production Skills

#### odrive-toolchain
**File:** `skills/odrive-toolchain/SKILL.md`  
**Purpose:** Build automation, testing, workspace inspection

**Capabilities:**
- Build firmware for specific board variants
- Run test suite (unit, integration, hardware-in-loop)
- Search for C++ symbols (functions, classes, variables)
- List error flags from interface YAML
- Workspace structure inspection

**Scripts:**
```
.github/skills/odrive-toolchain/
├── build_firmware.py    # Build firmware
├── search_symbol.py     # Search symbols
├── list_errors.py       # List error codes
├── setup-env.ps1        # Environment setup (Windows)
└── run_tests.ps1        # Run tests
```

**Example Usage:**
```bash
# Build for specific board
python .github/skills/odrive-toolchain/build_firmware.py board-v3.6-56V

# Search for symbol
python .github/skills/odrive-toolchain/search_symbol.py Motor::update
```

---

#### odrive-ops
**File:** `skills/odrive-ops/SKILL.md`  
**Purpose:** CI/CD workflows, release automation, deployment

**Capabilities:**
- GitHub Actions workflow management
- Firmware build matrix configuration
- Release channel management
- Documentation deployment

**Used By:** ODrive-Ops agent

---

### 🚧 Planned Skills (Stubs)

#### control-algorithms
**Status:** Stub - In Development  
**Purpose:** Common control algorithm implementations  
**Used By:** ODrive-Engineer

#### foc-tuning
**Status:** Stub - In Development  
**Purpose:** Automated FOC parameter tuning procedures  
**Used By:** ODrive-Engineer

#### sensorless-control
**Status:** Stub - In Development  
**Purpose:** Observer and estimation implementations  
**Used By:** ODrive-Engineer

#### pcb-review
**Status:** Stub - In Development  
**Purpose:** Automated PCB schematic and layout review  
**Used By:** ODrive-Engineer

#### signal-integrity
**Status:** Stub - In Development  
**Purpose:** Signal integrity and EMI analysis  
**Used By:** ODrive-Engineer

---

## 📖 Instructions Library

Foundational coding standards and guidelines that all agents must follow.

| Instruction File | Applies To | Purpose |
|-----------------|------------|---------|
| `cpp_coding_standards.instructions.md` | `**/*.{cpp,c,hpp,h,cc}` | C++ style, naming, modern practices |
| `header_file_rules.instructions.md` | `**/*.{h,hpp}` | Header structure, includes, pragmas |
| `embedded_cpp_best_practices.instructions.md` | `**` | Embedded C++ memory, real-time, hardware, safety patterns |
| `python_coding_standards.instructions.md` | `**/*.py` | Python style, PEP 8, testing |

**Mandatory:** Agents MUST read applicable instruction files before generating or modifying code.

---

## 💡 Prompts Library

Reusable workflow prompts for common development tasks.

| Prompt | Purpose |
|--------|---------|
| `build-firmware.prompt.md` | Build firmware for board variants |
| `toolchain.prompt.md` | Unified toolchain operations |
| `ops.prompt.md` | CI/CD and release operations |
| `refactor-modern-cpp-v2.prompt.md` | Modernize legacy C++ code to C++17 |
| `modernize-cpp.prompt.md` | Apply modern C++ patterns |
| `add-doxygen.prompt.md` | Add Doxygen documentation |
| `optimize-critical.prompt.md` | Optimize performance-critical code |
| `check-safety.prompt.md` | Safety validation for motor control |
| `explain-foc.prompt.md` | Explain FOC algorithm concepts |
| `tune-motor-controller.prompt.md` | Tune motor control parameters |
| `audit-todos.prompt.md` | Audit and prioritize TODO comments |

---

## 🚀 Quick Start

### For Firmware Development
```bash
# Activate ODrive Engineer agent
@odrive-engineer Implement CAN bus message handler for velocity commands
```

### For Build/Test Operations
```bash
# Build firmware
@odrive-toolchain Build firmware for board v3.6

# Search for symbols
@odrive-toolchain Find definition of Motor::update
```

### For CI/CD Operations
```bash
# Check CI/CD status
@odrive-ops Check firmware CI pipeline status

# Create release
@odrive-ops Create release v0.5.7
```

---

## 📋 Usage Rules

### Agent Selection
1. **Development tasks:** ODrive-Engineer
2. **Build/Test/Search tasks:** ODrive-Toolchain
3. **CI/CD/Release tasks:** ODrive-Ops

### Safety First
- ⚠️ **Hardware operations require explicit confirmation**
- Never auto-execute: `make flash`, `odrivetool`, device configuration
- Always provide warnings for voltage/current/speed changes

### Code Quality
- All code changes must follow instruction files
- Review checklist before any commit
- Tests required for new functionality

---

## 🔗 Related Documentation

- **Main README:** `../README.md` - Project overview
- **Developer Guide:** `../docs/developer-guide.rst` - Setup and architecture
- **API Documentation:** `../docs/` - Public API reference
- **Change Log:** `../CHANGELOG.md` - Version history

---

## 🛠️ Contributing to the Agent System

### Adding a New Agent

1. Create agent file: `.github/agents/{name}.agent.md`
2. Follow existing agent template structure
3. Update this README's hierarchy diagram
4. Update agent routing in relevant agent files
5. Document responsibilities and skills used

### Adding a New Skill

1. Create skill directory: `.github/skills/{name}/`
2. Create skill file: `.github/skills/{name}/SKILL.md`
3. Update this README's skills table
4. Update relevant agent files to reference new skill
5. Tag with status: ✅ (production) or 🚧 (development)

### Adding a New Instruction

1. Create instruction file: `.github/instructions/{name}.instructions.md`
2. Add frontmatter with `applyTo` glob pattern
3. Update this README's instructions table
4. Update agent files to reference new instruction

---

## 📊 Status Dashboard

| Component | Status | Last Updated |
|-----------|--------|------------|
| Constitution | ✅ Complete | 2026-01-22 |
| **Agents (3 Orchestrators)** | | |
| ODrive-Engineer.agent | ✅ Complete | 2026-01-22 |
| ODrive-Toolchain.agent | ✅ Complete | 2026-01-22 |
| ODrive-Ops.agent | ✅ Complete | 2026-01-22 |
| **Production Skills (2)** | | |
| odrive-toolchain | ✅ Production | 2026-01-22 |
| odrive-ops | ✅ Production | 2026-01-22 |
| **Planned Skills (5)** | | |
| control-algorithms | 🚧 Planned | - |
| foc-tuning | 🚧 Planned | - |
| sensorless-control | 🚧 Planned | - |
| pcb-review | 🚧 Planned | - |
| signal-integrity | 🚧 Planned | - |

---

**Version:** 1.0.0  
**Last Updated:** January 22, 2026  
**Maintained By:** ODrive Robotics Team

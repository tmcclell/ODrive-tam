# Lesson 3: Planning & Steering Documents - GitHub Copilot Customization

**Session Duration:** 60 minutes  
**Audience:** Embedded/C++ Developers/Project Managers/QA Engineers/Firmware Engineers  
**Environment:** Windows, VS Code  
**Extensions:** GitHub Copilot  
**Source Control:** GitHub/Bitbucket

---

## Overview

This lesson teaches you how to customize GitHub Copilot's behavior through planning and steering documents. You'll learn to create persistent coding standards, reusable prompts, specialized agents, skill folders, and use **Plan Mode** to turn project ideas into structured issues. These tools ensure consistent, high-quality AI output across your team.

**What You'll Learn:**
- Creating `copilot-instructions.md` for repo-wide coding standards
- Building reusable prompt files (`.prompt.md`) for common workflows
- Designing custom agents (`.agent.md`) for specialized personas
- Packaging skills with bundled resources for complex tasks
- Using **Plan Mode** to turn ideas into structured epics and issues
- **Spec-Driven Development** for production-quality, scalable software

**Key Concepts:**

| Concept | Description |
|---------|-------------|
| **Custom Instructions** | Always-on coding standards applied to every request |
| **Prompt Files** | Reusable task templates invoked with `/prompt-name` |
| **Custom Agents** | Specialized personas with specific tools and behaviors |
| **Agent Skills** | Instruction folders with bundled scripts and resources |
| **Plan Mode** 🆕 | Turn project ideas into structured epics, features, and tasks |
| **Spec-Driven Development** 🆕 | Formalized guardrails for AI: constitution → spec → plan → tasks |
| **Context Layering** | Building prompts with constitution → agent → files → constraints |

---

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Why Planning Matters](#why-planning-matters)
- [Agenda](#agenda-planning--steering-documents-60-min)
- [Learning Path](#learning-path)
- [Copilot Customization Hierarchy](#1-copilot-customization-hierarchy)
- [Custom Instructions](#2-custom-instructions-copilot-instructionsmd)
- [Prompt Files](#3-prompt-files-promptmd)
- [Custom Agents](#4-custom-agents-agentmd)
- [Agent Skills](#5-agent-skills-skillmd-folders)
- [**Plan Mode (Preview)**](#6-plan-mode-preview) 🆕
- [**Spec-Driven Development**](#7-spec-driven-development) 🆕
- [Speaker Instructions](#speaker-instructions)
- [**Hands-On Exercises**](#hands-on-exercises) 🆕
- [Quick Reference](#quick-reference-customization-options)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)
- [Additional Resources](#additional-resources)
- [Frequently Asked Questions](#frequently-asked-questions)
- [Summary: Key Takeaways](#summary-key-takeaways)
- [Next Steps](#next-steps)

---

## Prerequisites

Before starting this session, ensure you have:

- ✅ **Completed Basic Feature Overview** - Understanding of Chat modes, participants, and slash commands
- ✅ **Visual Studio Code** with GitHub Copilot extensions installed and enabled
- ✅ **Active Copilot subscription** with access to all features
- ✅ **Workspace access** - Ability to create folders and files in `.github/` directory
- ✅ **Basic Markdown knowledge** - For authoring customization files

### Verify Your Setup

1. **Check workspace permissions:**
   - Ensure you can create folders in your workspace
   - Verify `.github/` directory exists or can be created

2. **Enable customization features:**
   - Open VS Code Settings (Ctrl+,)
   - Search for `github.copilot.chat.codeGeneration.useInstructionFiles` and enable it
   - Search for `chat.useAgentSkills` and enable it for agent skills (preview)

3. **Test basic functionality:**
   - Open Chat view (Ctrl+Alt+I)
   - Verify agents dropdown is accessible
   - Confirm you can create new files in workspace

---

## Why Planning Matters

Front-loading context improves output quality dramatically. Instead of repeatedly providing the same context in every chat prompt, planning documents enable you to:

### Benefits of Planning Documents

1. **Consistency Across Sessions**
   - Define coding standards once, apply everywhere
   - Ensure all team members get the same AI behavior
   - Maintain consistent code quality

2. **Reduced Repetition**
   - Stop copy-pasting the same instructions
   - Eliminate redundant context in prompts
   - Save time and effort on every interaction

3. **Improved AI Output Quality**
   - More accurate code generation
   - Better adherence to project standards
   - Context-aware suggestions

4. **Team Collaboration**
   - Share best practices across the team
   - Version control for AI instructions
   - Onboard new developers faster

5. **Specialized Capabilities**
   - Create domain-specific agents
   - Build reusable workflows
   - Extend AI capabilities with custom tools

---

## Agenda: Planning & Steering Documents (60 min)

| Sub-Topic | Focus | Time |
|-----------|-------|------|
| Why Planning Matters | Front-loading context improves output quality | 3 min |
| copilot-instructions.md | Repo-level coding standards, constraints, patterns | 5 min |
| Prompt Files (.prompt.md) | Reusable task templates for common workflows | 5 min |
| Custom Agents (.agent.md) | Specialized agent profiles for domain-specific tasks | 5 min |
| Agent Skills (SKILL.md folders) | Self-contained instruction folders with bundled resources | 5 min |
| **Plan Mode (Preview)** 🆕 | Turn project ideas into structured issues/epics | 5 min |
| **Spec-Driven Development** 🆕 | Formalized specs for scalable, production-quality AI workflows | 5 min |
| **Instructor Demo** | Live demo of all features | 7 min |
| **[Hands-On Exercises](hands-on-exercises.md)** 🆕 | Participants practice creating customizations | 20 min |

---

## Learning Path

This lesson covers seven key areas. Work through them sequentially for the best learning experience.

| Topic | What You'll Learn | Estimated Time |
|-------|-------------------|----------------|
| Customization Hierarchy | Where files go and when they're loaded | 2 min |
| Custom Instructions | Repo-level coding standards | 5 min |
| Prompt Files | Reusable task templates | 5 min |
| Custom Agents | Specialized personas | 5 min |
| Agent Skills | Bundled instructions with resources | 5 min |
| **Plan Mode (Preview)** 🆕 | Turn ideas into structured issues | 5 min |
| **Spec-Driven Development** 🆕 | Production-quality specs with SpecKit | 5 min |
| **Instructor Demo** | Live demonstration of features | 8 min |
| **[Hands-On Exercises](hands-on-exercises.md)** 🆕 | Practice creating customizations | 20 min |

---

## 1. Copilot Customization Hierarchy

### File Locations and Loading Behavior

| Customization | Location | When Loaded | Purpose |
|---------------|----------|-------------|---------|
| **Instructions** | `.github/copilot-instructions.md` or `*.instructions.md` | Always (every request) or conditionally via glob patterns | Global coding standards, style guides |
| **Prompt Files** | `.github/prompts/*.prompt.md` | When user invokes with `/prompt-name` | Reusable task templates |
| **Custom Agents** | `.github/agents/*.agent.md` | When selected from agents dropdown | Specialized assistant personas with tool configs |
| **Agent Skills** | `.github/skills/*/SKILL.md` or `~/.copilot/skills/*/SKILL.md` | Auto-discovered from prompt | Task workflows with bundled scripts/references/templates |

### Visual Hierarchy

```
.github/
├── copilot-instructions.md          # Always loaded
├── instructions/
│   ├── cpp.instructions.md          # Loaded for *.cpp, *.hpp files
│   └── python.instructions.md       # Loaded for *.py files
├── prompts/
│   ├── generate-test.prompt.md      # Invoked with /generate-test
│   └── review-code.prompt.md        # Invoked with /review-code
├── agents/
│   ├── ODrive-Engineer.agent.md     # Selected from dropdown
│   └── ODrive-QA.agent.md           # Selected from dropdown
└── skills/
    └── misra-compliance/
        ├── SKILL.md                 # Skill definition
        ├── rules.md                 # Reference material
        └── examples/                # Example code
```

---

## 2. Custom Instructions (copilot-instructions.md)

Custom instructions enable you to define common guidelines and rules that automatically influence how AI generates code and handles other development tasks.

### Types of Instructions Files

#### 1. Global Instructions File
**Location:** `.github/copilot-instructions.md`

- Automatically applies to **all chat requests** in the workspace
- Stored within the workspace for team sharing
- Works in VS Code, Visual Studio, and GitHub.com

**Example for Embedded C++ Project:**

```markdown
# Project Coding Standards

## C++ Standards
- Use C++17 standard
- Follow Google C++ Style Guide
- Use smart pointers (std::unique_ptr, std::shared_ptr)
- Avoid raw pointers except for non-owning references

## Embedded Constraints
- No dynamic memory allocation (no malloc/new)
- No exceptions (use error codes)
- Static allocation only
- Use volatile for hardware registers

## Documentation
- Use Doxygen-style comments
- Document all public APIs
- Include @brief, @param, @return tags

## Testing
- Use doctest framework
- Write unit tests for all functions
- Aim for 80% code coverage
```

#### 2. Conditional Instructions Files
**Location:** `.github/instructions/*.instructions.md`

- Apply to specific file types using glob patterns
- Multiple files for different contexts
- Stored in workspace or user profile

### Creating Path-Specific Custom Instructions

1. **Create the directory** if it doesn't exist:
   ```
   .github/instructions/
   ```

2. **Create instruction files** with the naming pattern `NAME.instructions.md`

3. **Add frontmatter** with `applyTo` glob pattern:

```markdown
---
applyTo: 'app/models/**/*.rb'
---
```

4. **Specify multiple patterns** by separating with commas:

```markdown
---
applyTo: '**/*.ts,**/*.tsx'
---
```

### Glob Pattern Reference

| Pattern | Matches |
|---------|---------|
| `*` | All files in current directory |
| `**` or `**/*` | All files in all directories (recursive) |
| `*.py` | All `.py` files in current directory |
| `**/*.py` | All `.py` files recursively in all directories |
| `src/*.py` | `.py` files in `src/` only (e.g., `src/foo.py`, not `src/foo/bar.py`) |
| `src/**/*.py` | `.py` files recursively under `src/` (e.g., `src/foo.py`, `src/foo/bar.py`) |
| `**/subdir/**/*.py` | `.py` files in any `subdir/` at any depth |
| `Firmware/**/*.{cpp,c,h,hpp}` | C/C++ files recursively under `Firmware/` |

### Exclude from Specific Agents (Optional)

Use `excludeAgent` to prevent instructions from being used by specific agents:

```markdown
---
applyTo: '**'
excludeAgent: 'code-review'
---
# These instructions are only used by Copilot coding agent
```

| Value | Effect |
|-------|--------|
| `code-review` | Excluded from Copilot code review |
| `coding-agent` | Excluded from Copilot coding agent |
| *(not specified)* | Used by both agents |

---

**Example: Python Instructions**

```markdown
---
name: Python Coding Standards
description: Python coding standards for project scripts
applyTo: '**/*.py'
---

# Python Coding Standards
- Follow PEP 8 style guide
- Use type hints for all functions
- Write docstrings for all public functions
- Use pytest for testing
```

**Example: Embedded C/C++ Instructions**

```markdown
---
name: Embedded C/C++ Standards
description: Embedded C/C++ coding standards for firmware
applyTo: 'Firmware/**/*.{c,h,cpp,hpp}'
---

# Embedded C/C++ Standards
- No dynamic memory allocation
- Use static allocation only
- Document interrupt handlers
- Use volatile for hardware registers
- Follow MISRA C guidelines where applicable
```

---

### Try It: File-Type-Specific Instructions Demo

Use these prompts to see how Copilot automatically applies different coding standards based on file type:

#### Header File Prompts (.hpp/.h) → `header_file_rules.instructions.md`

| Prompt | Expected Patterns |
|--------|-------------------|
| `Create a header file for a motor controller class` | `#pragma once`, forward declarations |
| `Create a header file for an SPI driver` | No `using namespace`, trivial getters inline |
| `Add a new header file for CAN message parsing` | Complex methods declared only (not defined) |

**What to look for:**
- ✅ Uses `#pragma once` (not old-style `#ifndef` guards)
- ✅ Forward declarations instead of heavy includes
- ✅ No `using namespace` in header scope
- ✅ Trivial getters inline, complex methods declared only

---

#### C++ Source Prompts (.cpp) → `cpp_coding_standards.instructions.md`

| Prompt | Expected Patterns |
|--------|-------------------|
| `Create a C++ class to manage encoder readings` | PascalCase class, camelCase methods |
| `Implement a velocity controller in C++` | `is`/`has` prefixes, `kConstants` |
| `Create a .cpp file with a function to calculate motor torque` | Trailing `_` for private members |

**What to look for:**
- ✅ `PascalCase` for class names (`EncoderManager`, `VelocityController`)
- ✅ `camelCase` for methods (`getPosition()`, `calculateTorque()`)
- ✅ `is`/`has` prefixes for booleans (`isCalibrated()`, `hasError()`)
- ✅ Trailing underscore for private members (`position_`, `velocity_`)
- ✅ `kPascalCase` for constants (`kMaxSpeed`, `kDefaultTimeout`)
- ✅ `enum class` with PascalCase values

---

#### Python Prompts (.py) → `python_coding_standards.instructions.md`

| Prompt | Expected Patterns |
|--------|-------------------|
| `Create a Python script to log motor temperature` | snake_case, type hints, docstrings |
| `Write a Python class to configure ODrive parameters` | `UPPER_CASE` constants, `_private` |
| `Create a Python function to validate motor configuration` | Google-style docstrings |

**What to look for:**
- ✅ `snake_case` for functions (`read_temperature()`, `configure_axis()`)
- ✅ `PascalCase` for classes (`TemperatureLogger`, `MotorConfig`)
- ✅ `UPPER_CASE` for constants (`MAX_TEMPERATURE`, `DEFAULT_INTERVAL`)
- ✅ Type hints on all parameters and returns (`def read_temp() -> float:`)
- ✅ Google-style docstrings with `Args:`, `Returns:`, `Raises:`
- ✅ `_leading_underscore` for private members

---

#### How It Works: The `applyTo` Pattern

Each instruction file contains a glob pattern that controls when it's loaded:

```yaml
# Header rules - loaded for .h and .hpp files
applyTo: '**/*.{h,hpp}'

# C++ rules - loaded for .cpp, .c, .cc files  
applyTo: '**/*.{cpp,c,cc}'

# Python rules - loaded for .py files
applyTo: '**/*.py'
```

**This is automatic!** You don't need to specify which rules to use - Copilot detects the file type and loads the appropriate instructions.

---

## 3. Prompt Files (.prompt.md)

Prompt files are Markdown files that define reusable prompts for common development tasks. They are standalone prompts that you can run directly in chat.

### Prompt File Structure

```markdown
---
name: prompt-name
description: Brief description of what this prompt does
argument-hint: Optional hint text for users
agent: ask|edit|agent|custom-agent-name
model: Claude Sonnet 4
tools: ['search', 'fetch', 'githubRepo']
---

# Prompt Instructions

Your detailed instructions go here...

Use variables:
- ${workspaceFolder}
- ${file}
- ${selection}
- ${input:variableName}
- ${input:variableName:placeholder text}
```

### Example: Build Firmware Prompt

This prompt builds ODrive firmware for specific board variants using the toolchain skill.

**File:** `.github/prompts/build-firmware.prompt.md`

```markdown
# Build Firmware

Build ODrive firmware for specific board variants and configurations.

## Instructions

Use the **odrive-toolchain** skill to build firmware with proper board configuration.

### Workflow Hierarchy

```
copilot-instructions.md (Constitution)
        ↓
  ODrive-Toolchain.agent (Orchestrator)
        ↓
  odrive-toolchain skill
        ↓
  build_firmware.py + setup-env.ps1
```

### Commands

```powershell
# Step 1: Source environment (Windows PowerShell)
. .github\skills\odrive-toolchain\setup-env.ps1

# Step 2: Build using the skill script
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V

# List available configurations
python .github\skills\odrive-toolchain\build_firmware.py --list-configs
```

### Available Board Variants

| Board | Config String | Voltage | Notes |
|-------|--------------|---------|-------|
| **v3.6** | `board-v3.6-56V` | 56V | Current production |
| **v3.5** | `board-v3.5-24V` | 24V | Legacy |

### Output

**On Success:**
- ✅ Build completed successfully
- 📦 Binary location: `Firmware/build/ODriveFirmware.elf`
- 📊 Build statistics (size, warnings)

### Example: Optimize Critical Path Prompt

This prompt optimizes performance-critical code in real-time execution paths.

**File:** `.github/prompts/optimize-critical.prompt.md`

```markdown
---
name: optimize-critical
description: 'Optimize critical path code for performance'
agent: edit
---

Analyze the selected code block, which is part of a critical real-time execution path.

1. Identify any potential performance bottlenecks:
   - Division operations
   - Heavy trig functions
   - Memory allocation

2. Suggest 2-3 specific optimizations to reduce cycle count or memory usage.

3. Apply the changes if they are safe and don't reduce readability significantly.

4. **Strict rule**: No dynamic memory allocation (`new`, `malloc`, `std::vector` resizing) allowed.

5. Avoid unnecessary object copies; pass heavy objects by `const` reference where possible.
```

**Usage:**
1. Select a code block in the editor
2. Type `/optimize-critical` in Chat view
3. Copilot analyzes and suggests optimizations

### Example: Embedded Safety Check Prompt

This prompt performs safety analysis on embedded firmware modules.

**File:** `.github/prompts/check-safety.prompt.md`

```markdown
---
name: check-safety
description: 'Perform a safety check on a specific module'
agent: agent
---

Perform a safety analysis on the specified module file.

Check for:
1. **Race Conditions**: Shared variables accessed without locks/interrupt protection.
2. **Null Dereferences**: Pointers used without checking.
3. **Floating Point Safety**: Division by zero or likely NaN propagation.
4. **Deadlocks**: Potential lock ordering issues.

Output a Markdown report with a "Risk Level" (Low/Medium/High) for each finding.
```

**Usage:**
1. Open a firmware source file (e.g., `motor.cpp`)
2. Type `/check-safety` in Chat view
3. Copilot analyzes the code and produces a safety report

### Using Prompt Files

**Example with Build Firmware:**
1. Type `/build-firmware` in Chat view
2. Copilot sources environment and builds for specified board
3. Reports build status and output files

**Example with Optimize Critical:**
1. Select performance-critical code in the editor
2. Type `/optimize-critical` in Chat view
3. Copilot analyzes and applies safe optimizations

### Create a Prompt File

1. **In Chat view:**
   - Click **Configure Chat (⚙️)** > **Prompt Files** > **New prompt file**
   - Or use Command Palette: `Chat: New Prompt File`

2. **Choose location:**
   - **Workspace:** `.github/prompts/` (team sharing)
   - **User profile:** Available across all workspaces

3. **Author the prompt:**
   - Fill in YAML frontmatter
   - Write clear instructions
   - Use variables for flexibility

4. **Use the prompt:**
   - Type `/prompt-name` in Chat view
   - Provide any required arguments

---

## 4. Custom Agents (.agent.md)

Custom agents enable you to configure the AI to adopt different personas tailored to specific development roles and tasks.

### Custom Agent Structure

```markdown
---
name: agent-name
description: Brief description shown in chat input
argument-hint: Optional hint text
tools: ['search', 'fetch', 'usages', 'terminal']
model: Claude Sonnet 4
infer: true
handoffs:
  - label: Next Step Button Text
    agent: target-agent-name
    prompt: Prompt to send to next agent
    send: false
---

# Agent Instructions

Your specialized instructions go here...
```

### Example: ODrive Engineer Agent

The primary development orchestrator for ODrive firmware.

**File:** `.github/agents/ODrive-Engineer.agent.md`

```markdown
---
name: ODrive Engineer
description: Primary orchestrator agent for ODrive development. Invokes specialized skills for firmware, motor control, and hardware tasks.
tools: ['search', 'fetch', 'usages', 'githubRepo', 'terminal']
---

# ODrive Engineer (Primary Development Orchestrator)

You are the **primary development orchestrator** for the ODrive Development System.

## Quick Reference

| Attribute | Value |
|-----------|-------|
| **Role** | Primary Development Orchestrator |
| **Domains** | Firmware, Motor Control, Hardware |
| **Skills** | odrive-toolchain, control-algorithms, foc-tuning |
| **Invocation** | `@odrive-engineer [request]` |

## Skills Hierarchy

```
copilot-instructions.md (Constitution)
        ↓
  ODrive-Engineer.agent (You - Primary Orchestrator)
        ↓
  ┌───────┴────────┬──────────┬───────────┐
  ↓               ↓          ↓          ↓
odrive-       odrive-    control-   foc-tuning
toolchain       ops      algorithms    (🚧)
  (✅)          (✅)        (🚧)
```

## Safety

**CRITICAL:** Before ANY hardware operation (flashing, calibration, motor operation),
provide clear warnings and wait for explicit user confirmation.
```

**Usage:**
- Invoke with `@odrive-engineer` in Chat view
- Automatically routes requests to appropriate skills

### Example: ODrive Toolchain Agent

Handles build, test, and code navigation operations.

**File:** `.github/agents/ODrive-Toolchain.agent.md`

```markdown
---
name: ODrive Toolchain
description: Build, compile, test, and code navigation for ODrive firmware development
tools: ['search', 'fetch', 'terminal']
---

# ODrive Toolchain Agent

You handle **build, test, and code navigation** for ODrive firmware.

## Quick Reference

| Attribute | Value |
|-----------|-------|
| **Role** | Build & Test Operations |
| **Invocation** | `@odrive-toolchain [request]` |
| **Skill** | `odrive-toolchain` |

## What You Do

| Operation | Command |
|-----------|--------|
| **Build firmware** | `python .github/skills/odrive-toolchain/build_firmware.py board-v3.6-56V` |
| **Find symbols** | `python .github/skills/odrive-toolchain/search_symbol.py Controller` |
| **List errors** | `python .github/skills/odrive-toolchain/list_errors.py --filter encoder` |

## Safety

✅ **Safe**: Build, search, list errors, run software tests  
⚠️ **Confirmation Required**: Flash firmware, HIL tests
```

**Usage:**
- Invoke with `@odrive-toolchain build firmware for v3.6`
- Automatically sources environment and executes build scripts

### Create a Custom Agent

1. **In Chat view:**
   - In agents dropdown, select **Configure Custom Agents** > **Create new custom agent**
   - Or use Command Palette: `Chat: New Custom Agent`

2. **Choose location:**
   - **Workspace:** `.github/agents/` (team sharing)
   - **User profile:** Available across all workspaces

3. **Author the agent:**
   - Fill in YAML frontmatter
   - Define tools and model
   - Write specialized instructions
   - Add handoffs if needed

4. **Use the agent:**
   - Select from agents dropdown in Chat view
   - All prompts use this agent's configuration

---

## 5. Agent Skills (SKILL.md folders)

Agent Skills are folders of instructions, scripts, and resources that Copilot can load when relevant to perform specialized tasks.

### Skill vs Instructions Comparison

| Feature | Agent Skills | Custom Instructions |
|---------|-------------|---------------------|
| **Purpose** | Specialized capabilities & workflows | Coding standards & guidelines |
| **Portability** | Works across VS Code, CLI, GitHub.com | VS Code and GitHub.com only |
| **Content** | Instructions + scripts + examples + resources | Instructions only |
| **Loading** | On-demand when relevant | Always applied or via glob patterns |
| **Standard** | Open standard (agentskills.io) | VS Code-specific |

### Skill Structure

Example from the ODrive toolchain skill:

```
.github/skills/odrive-toolchain/
├── SKILL.md                 # Skill definition
├── setup-env.ps1            # Environment setup script
├── build_firmware.py        # Build automation
├── search_symbol.py         # Symbol search tool
├── list_errors.py           # Error code listing
└── find_symbol.ps1          # Quick symbol search
```

### Example: ODrive Toolchain Skill

**File:** `.github/skills/odrive-toolchain/SKILL.md`

```markdown
---
name: odrive-toolchain
description: Build, test, search, and error inspection for ODrive firmware
status: production
version: 1.0.0
---

# ODrive Toolchain Skill

All build, test, search, and error inspection scripts in one place.

## Scripts

| Script | Purpose | Usage |
|--------|---------|-------|
| `setup-env.ps1` | Add tup + ARM GCC to PATH | `. .\setup-env.ps1` |
| `build_firmware.py` | Build firmware for board | `python build_firmware.py board-v3.6-56V` |
| `search_symbol.py` | Search for symbols | `python search_symbol.py Encoder --type class` |
| `list_errors.py` | List error codes | `python list_errors.py --filter encoder` |

## Quick Usage

### Build Firmware
```powershell
. .github\skills\odrive-toolchain\setup-env.ps1
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V
```

## Safety

✅ **Safe**: All operations are read-only or local builds  
⚠️ **Never auto-execute**: Flash firmware, HIL tests
```

### Example: FOC Tuning Skill (Stub)

This shows how to create a skill that's in development.

**File:** `.github/skills/foc-tuning/SKILL.md`

```markdown
---
name: foc-tuning
description: 🚧 STUB - Automated FOC parameter tuning procedures and optimization
status: in-development
---

# FOC Tuning Skill (🚧 In Development)

**Status:** Stub - Planned for future implementation  
**Owner:** motor-control-engineer.agent

## Purpose

This skill will provide automated tuning procedures for Field-Oriented Control (FOC) motor controllers.

## Planned Capabilities

- Automated current loop bandwidth measurement
- Velocity loop step response tuning
- Position loop gain optimization
- Anti-cogging calibration procedures
- Stability margin analysis

## Dependencies

- Motor parameter identification
- Hardware-in-the-loop testing infrastructure

## Implementation Status

- [ ] Current loop auto-tuning
- [ ] Velocity loop auto-tuning
- [ ] Position loop auto-tuning
- [ ] Anti-cogging calibration

## Related Skills

- `control-algorithms` - Controller implementations
- `sensorless-control` - Observer tuning
```

**Note:** Stub skills document planned functionality before implementation.

### Create an Agent Skill

1. **Enable skills:**
   - Open Settings (Ctrl+,)
   - Search for `chat.useAgentSkills`
   - Enable the setting

2. **Create skill directory:**
   ```
   .github/skills/my-skill/
   └── SKILL.md
   ```

3. **Author SKILL.md:**
   - Fill in YAML frontmatter:
     - `name`: lowercase with hyphens (e.g., `my-skill`)
     - `description`: Clear description of what the skill does and when to use it
   - Write detailed instructions
   - Reference any bundled resources

4. **Add resources (optional):**
   - Scripts in `scripts/`
   - Examples in `examples/`
   - Documentation in `references/`

5. **Test the skill:**
   - Create a prompt that matches skill description
   - Copilot auto-loads skill when relevant

---

## 6. Plan Mode (Preview) 🆕

> **Note:** This feature is in public preview and subject to change.

Plan Mode is GitHub Copilot's agentic issue creation feature that transforms your project ideas into structured epics, features, and tasks. It's the bridge between vision and execution.

### What is Plan Mode?

Plan Mode helps you manage project planning using GitHub Issues with Copilot. You can turn product ideas into structured backlogs with epics, features, and tasks - all through natural language conversation.

### Plan Mode Workflow

![Migrating Apps with GitHub Copilot](.\images\planning-process.png)

*Plan Mode workflow: Legacy application → GHCP documents codebase → Verify & refine → Generate PRD → Plan Mode creates structured plan → Agent mode implements.*

### When to Use Plan Mode

| Scenario | Plan Mode Benefit |
|----------|-------------------|
| **New project kickoff** | Turn ideas into structured issues |
| **Legacy migration** | Break down complex migrations into phases |
| **Feature planning** | Decompose features into actionable tasks |
| **Sprint planning** | Generate detailed task breakdowns |
| **Technical debt** | Organize refactoring into manageable chunks |

### Plan Mode Best Practices

1. **Be Specific in Your Prompt**
   - Include technology stack
   - Specify requirements and constraints
   - Mention team structure if relevant

2. **Iterate on Generated Issues**
   - Ask Copilot to break down high-level issues
   - Request improved descriptions with code snippets
   - Add acceptance criteria and technical details

3. **Use Versioning**
   - Compare different versions of generated issues
   - Keep the best aspects from each version

4. **Organize Hierarchically**
   - Epic → Features → Tasks
   - Use clear naming conventions
   - Link related issues appropriately

### Example: Ada to C++ Migration Plan

We've created a complete migration plan using Plan Mode. See the example:

📄 **[Ada to C++ Migration Plan](ada-to-cpp-migration-plan.md)**

This plan demonstrates:
- **Epic structure** for the overall migration project
- **Feature breakdown** for each migration phase
- **Task decomposition** with specific prompts
- **Pattern mappings** between Ada and C++ constructs
- **Risk assessment** and timeline estimation

### How to Use Plan Mode

#### Step 1: Start the Conversation

```
I'm planning to migrate an Ada embedded system to Modern C++. 
The system has cyclic tasks, protected objects, and strong types.
Please help me create a structured project plan with epics, features, and tasks.
```

#### Step 2: Review and Refine

After Copilot generates the initial plan:
- Click on epics to view details
- Navigate the issue tree
- Request breakdowns of high-level items

```
Can you break down the "Feature: Concurrency Migration" into smaller tasks?
```

#### Step 3: Improve Descriptions

```
Can you improve the description for "Task: Migrate Storage protected object"?
Please include:
- Technical summary
- Ada to C++ pattern mapping
- Implementation steps
- Code snippets
- Acceptance criteria
```

#### Step 4: Create Issues

Once satisfied with the plan:
1. Review all generated issues
2. Click "Create all" to create issues in your repository
3. Assign to team members or Copilot

### Plan Mode + Agents + Skills

Plan Mode works seamlessly with custom agents and skills:

```
┌───────────────────────────────────────────────────────────────┐
│                         Plan Mode                              │
│  Creates structured epics, features, and tasks                 │
└───────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌───────────────────────────────────────────────────────────────┐
│                      Custom Agents                             │
│  @ada-to-cpp-migrator executes specific migration tasks        │
└───────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌───────────────────────────────────────────────────────────────┐
│                         Skills                                 │
│  ada-cpp-migration provides patterns, templates, references    │
└───────────────────────────────────────────────────────────────┘
```

### Plan Mode Output Example

```markdown
## 📋 Epic: Migrate Ada System to C++

### 🎯 Feature 1: Project Analysis
- Task 1.1: Scan Ada source files
- Task 1.2: Build dependency graph
- Task 1.3: Inventory concurrency patterns

### 🎯 Feature 2: Foundation Setup
- Task 2.1: Create C++ project structure
- Task 2.2: Implement strong type library
- Task 2.3: Create task wrapper framework

### 🎯 Feature 3: Type Migration
- Task 3.1: Migrate HK_Data package
- Task 3.2: Migrate TTC_Data package
...
```

### Further Reading

- [Using GitHub Copilot to create or update issues](https://docs.github.com/en/copilot/using-github-copilot/using-github-copilot-to-create-issues)
- [Best practices for using GitHub Copilot to work on tasks](https://docs.github.com/en/copilot/using-github-copilot/best-practices-for-using-github-copilot-to-work-on-tasks)
- [Asking GitHub Copilot to create a pull request](https://docs.github.com/en/copilot/using-github-copilot/asking-github-copilot-to-create-a-pull-request)

---

## 7. Spec-Driven Development 🆕

> **What it is:** A formalized process for providing AI models with guardrails through specifications, plans, and tasks - enabling production-quality, maintainable software.

### Why Spec-Driven Development?

When developers first adopt AI coding assistants, many start with **"vibe coding"** - having an idea, typing a few sentences, and letting the AI figure it out. This approach has benefits (fast prototyping) but significant downsides for production software:

| Vibe Coding Approach | Spec-Driven Approach |
|---------------------|----------------------|
| AI makes architectural decisions for you | You define architectural constraints upfront |
| Framework/library choices are arbitrary | Stack choices are explicit and intentional |
| Inconsistent patterns across the codebase | Consistent conventions enforced by specs |
| Harder to maintain long-term | Documentation and context preserved |
| Difficult to onboard new team members | Clear specifications for everyone |

> **Key Insight:** "If you start building production software, software that needs to be maintained by somebody, software that needs to evolve down the line beyond your initial prototype, that vibe coding approach doesn't really scale because the AI doesn't have the sense of taste or conventions that you need to be applying."

### Plan Mode vs. Spec-Driven Development

Both approaches help you plan before coding, but they serve different purposes:

| Aspect | Plan Mode | Spec-Driven Development |
|--------|-----------|-------------------------|
| **Scope** | Single feature or idea → issues | Entire project or major feature → full implementation |
| **Output** | GitHub Issues (epics, features, tasks) | Markdown specs, plans, tasks + code |
| **Primary Use** | Project management, sprint planning | Production codebases, team collaboration |
| **Integration** | GitHub Issues | Works with any agent (Copilot, Claude, Gemini) |
| **Artifacts** | Issues in your repository | `spec.md`, `plan.md`, `tasks.md` files |
| **Best For** | Breaking down work for a team | Ensuring consistent, high-quality AI output |

**When to use Plan Mode:**
- You want to create GitHub Issues for tracking
- You're planning sprints or breaking down features
- You need to assign tasks to team members
- You want Copilot to create PRs from issues

**When to use Spec-Driven Development:**
- You're building production software that needs to scale
- You want the AI to follow specific conventions consistently
- You're working on a greenfield project with complex requirements
- You need to explore multiple implementation variations (e.g., Hugo vs Next.js)
- You want a documented decision trail for your project

### The Spec-Driven Workflow

```
┌─────────────────────────────────────────────────────────────────┐
│                    1. CONSTITUTION                               │
│    Non-negotiable principles: stack, deployment, coding style   │
│    File: constitution.md or agents.md                           │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    2. SPECIFICATION                              │
│    Functional requirements: user stories, scenarios, edge cases │
│    File: specs/001-feature-name/spec.md                         │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    3. CLARIFICATION                              │
│    AI asks questions YOU didn't think to ask                    │
│    "How will episode data be managed?" "Expected catalog size?" │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    4. TECHNICAL PLAN                             │
│    Architecture, dependencies, data models, contracts           │
│    File: specs/001-feature-name/plan.md                         │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    5. TASKS                                      │
│    Broken-down implementation steps with MVP identification     │
│    File: specs/001-feature-name/tasks.md                        │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    6. IMPLEMENTATION                             │
│    Agent executes tasks one by one, following the plan          │
│    Each task can be parallelized or done in background          │
└─────────────────────────────────────────────────────────────────┘
```

### Key Artifacts

#### 1. Constitution (`constitution.md` or `agents.md`)

Non-negotiable principles for your entire project:

```markdown
# Project Constitution

## Non-Negotiable Principles

### Deployment
- Always deploy to Azure
- Use Azure-specific packages, not other cloud providers

### Technology Stack
- Always use the latest LTS version of Node.js
- Use TypeScript with strict mode enabled
- Use this specific logging package: [package-name]

### Performance
- Lighthouse score must be 90+ for all pages
- Static-first architecture

### Development Workflow
- Test against Chrome, Firefox (Safari optional)
- All code must have unit tests
```

#### 2. Specification (`spec.md`)

Functional requirements with user stories and scenarios:

```markdown
# Feature: Podcast Website

## User Stories

### Browse Featured Episodes
As a visitor, I want to see featured episodes on the landing page
so that I can quickly discover popular content.

**Scenarios:**
- Display 3-5 featured episodes with thumbnails
- Show episode title, duration, and publish date
- Link to episode detail page

### View Episode Details
As a visitor, I want to view episode details including:
- Full description
- Audio player
- Show notes
- Guest information

## Edge Cases
- Episode with no guest
- Episode longer than 2 hours
- Missing thumbnail image
```

#### 3. Clarification Session

The AI asks questions you didn't think to ask:

| Question | Options | Answer |
|----------|---------|--------|
| How will episode data be managed? | A) Admin CMS B) Static JSON C) RSS import | B |
| Expected episode catalog size? | A) 10-50 B) 50-100 C) 100-200 D) 200+ | D |
| Audio hosting provider? | A) Self-hosted B) Libsyn C) Transistor | A |

#### 4. Technical Plan (`plan.md`)

Architecture and implementation details:

```markdown
# Technical Plan: Podcast Website

## Technical Context
- Framework: Hugo (static site generator)
- Templating: Go templates with HTML5
- Styling: Tailwind CSS
- Build: npm for asset pipeline

## Dependencies
- hugo-extended v0.120+
- tailwindcss v3.4+
- alpinejs v3.x (minimal interactivity)

## Content Structure
content/
├── episodes/
│   ├── ep001-introduction.md
│   └── ep002-guest-interview.md
├── guests/
│   └── john-doe.md
└── pages/
    ├── about.md
    └── contact.md

## Data Model
### Episode
| Field | Type | Required |
|-------|------|----------|
| title | string | ✓ |
| date | datetime | ✓ |
| duration | string | ✓ |
| audio_url | url | ✓ |
| description | markdown | ✓ |
| guest | reference | |
| featured | boolean | |
```

#### 5. Tasks (`tasks.md`)

Broken-down implementation steps:

```markdown
# Implementation Tasks

## MVP (Phase 1)
- [ ] Task 1.1: Initialize Hugo project structure
- [ ] Task 1.2: Create base layout with header/footer
- [ ] Task 1.3: Implement episode list template
- [ ] Task 1.4: Implement episode detail template
- [ ] Task 1.5: Add Tailwind CSS configuration

## Phase 2: Enhanced Features
- [ ] Task 2.1: Add audio player component
- [ ] Task 2.2: Create guest profile pages
- [ ] Task 2.3: Implement search functionality
```

### Getting Started with SpecKit

[GitHub SpecKit](https://github.com/github/spec-kit) is a collection of prompts and scripts that implement spec-driven development. It works with any agent (Copilot, Claude, Gemini, etc.).

#### Installation

```bash
# Using UV (recommended for Python)
uv tool install spec-kit

# Initialize in a new project
specify init my-project

# Initialize in an existing project
cd my-existing-project
specify init .
```

#### Workflow Commands

| Command | Purpose |
|---------|--------|
| `specify init` | Initialize SpecKit in a project |
| Constitution | Establish non-negotiable project principles |
| Build Specification | Create functional requirements |
| Clarify | AI asks questions to fill gaps |
| Build Technical Plan | Generate architecture and data models |
| Create Tasks | Break down into implementable steps |
| Implement | Execute tasks one by one |

#### Example Session

```
# Start with constitution
> Create a constitution for a static podcast website with minimal dependencies

# Build the specification
> Build a specification for a podcast website with featured episodes

# Clarify requirements (AI asks you questions)
> Clarify spec requirements
  - Q1: How will episode data be managed? → B (Static JSON)
  - Q2: Expected catalog size? → D (200+ episodes)
  - Q3: Audio hosting? → A (Self-hosted)

# Generate technical plan
> Build technical plan using Hugo and Tailwind CSS

# Create tasks
> Create implementation tasks

# Implement
> Implement tasks
```

### Benefits for Existing Projects

Spec-driven development isn't just for greenfield projects:

1. **Retroactive Documentation**
   - Point SpecKit at your existing codebase
   - Generate a constitution based on current patterns
   - Document what's already there

2. **New Feature Development**
   - Create a new spec folder: `specs/002-new-feature/`
   - Go through the spec → plan → tasks workflow
   - Maintain historical context for future changes

3. **Multiple Variations**
   - Create one spec, fork into multiple implementations
   - Compare Hugo vs Next.js for the same requirements
   - Use git worktrees for parallel exploration

### Spec-Driven vs. Custom Instructions

| Custom Instructions | Spec-Driven Development |
|--------------------|-------------------------|
| `copilot-instructions.md` | `constitution.md` + `spec.md` + `plan.md` |
| Applied automatically to all requests | Invoked explicitly per feature |
| General coding standards | Detailed functional + technical specs |
| Good for coding style | Good for architectural decisions |
| Complements spec-driven | Uses custom instructions as foundation |

> **Best Practice:** Use both! Custom instructions for always-on coding standards, spec-driven development for major features and architectural work.

### Further Reading

- [GitHub SpecKit Repository](https://github.com/github/spec-kit) - 53K+ stars
- [SpecKit Documentation](https://github.com/github/spec-kit#readme)
- [Spec-Driven Development with Existing Projects (YouTube)](https://www.youtube.com/watch?v=example)

---

## Speaker Instructions

### 1. Demo: Why Planning Matters (10 min)

**Show the problem:**
1. Open Chat view and generate code without any instructions
2. Show generic, non-project-specific output
3. Highlight inconsistencies across multiple generations

**Show the solution:**
1. Add a `.github/copilot-instructions.md` file with project standards
2. Generate similar code again
3. Highlight improved consistency and adherence to standards

**Key points to emphasize:**
- Context matters for quality
- Repetition wastes time
- Team collaboration benefits
- Version control for AI behavior

### 2. Demo: Custom Instructions (10 min)

**Create copilot-instructions.md:**
1. Create `.github/copilot-instructions.md`
2. Add C++ coding standards
3. Add embedded constraints
4. Add documentation requirements

**Test the instructions:**
1. Ask Copilot to generate a motor control function
2. Show it follows static allocation rules
3. Show it includes Doxygen comments
4. Show it avoids exceptions

**Example for ODrive project:**
```markdown
# ODrive Firmware Coding Standards

## C++ Standards
- Use C++17
- No exceptions
- No RTTI
- Static allocation only

## Motor Control
- All control loops at 8kHz
- Use fixed-point math where possible
- Document timing constraints

## HAL Patterns
- Abstract hardware access
- Use volatile for registers
- Document interrupt context
```

### 3. Demo: Prompt Files (10 min)

**Create reusable prompts:**

1. **Create state machine generator:**
   - File: `.github/prompts/generate-state-machine.prompt.md`
   - Demonstrates code generation workflow
   - Shows variable usage

2. **Create code review prompt:**
   - File: `.github/prompts/embedded-review.prompt.md`
   - Demonstrates analysis workflow
   - Shows tool selection

**Use the prompts:**
1. Type `/generate-state-machine` in chat
2. Provide state machine name
3. Show generated code follows standards

### 4. Demo: Custom Agents (12 min)

**Create specialized agents:**

1. **Planning Agent:**
   - Read-only tools
   - Generates implementation plans
   - Handoff to implementation agent

2. **Embedded Expert Agent:**
   - Editing tools enabled
   - Specialized embedded knowledge
   - MISRA compliance focus

3. **Code Reviewer Agent:**
   - Analysis tools only
   - Security focus
   - Handoff to fix issues

**Demo workflow:**
1. Switch to Planning agent
2. Generate plan for new feature
3. Use handoff to Implementation agent
4. Make code changes
5. Use handoff to Code Reviewer agent
6. Review and suggest improvements

### 5. Demo: Agent Skills (10 min)

**Create example skill:**
1. Create `.github/skills/misra-compliance/`
2. Add SKILL.md with MISRA rules
3. Include example scripts for checking
4. Add reference documentation

**Show progressive loading:**
1. Skill discovered by name/description
2. Instructions loaded when relevant
3. Resources accessed as needed

**Test the skill:**
1. Ask: "Check this code for MISRA compliance"
2. Copilot auto-loads the skill
3. Uses bundled resources for checks
4. Provides detailed compliance report

### 6. Demo: Hands-On Exercise (8 min)

Walk through creating a complete customization setup:
1. Create copilot-instructions.md for project
2. Create one prompt file
3. Create one custom agent
4. Create one skill folder

---

## Hands-On Exercises

**Duration:** 20 minutes

For the hands-on portion of this lesson, participants should follow the dedicated exercises file:

📄 **[Hands-On Exercises](hands-on-exercises.md)**

### Exercise Overview

| Exercise | Focus | Time |
|----------|-------|------|
| 1. Custom Instructions | Create coding standards + test with `#file:` references | 5 min |
| 2. Prompt Files | Create reusable prompts + test with `/command` | 5 min |
| 3. Custom Agents | Create specialized persona + test with real code | 5 min |
| 4. Agent Skills | Create skill folder + test auto-discovery | 5 min |

### Key Learning Outcomes

After completing the exercises, participants will be able to:

- ✅ Create custom instructions that auto-apply based on file type
- ✅ Build reusable prompt files invoked with `/name`
- ✅ Design specialized agents with checklists and output formats
- ✅ Package skills with bundled resources
- ✅ Use `#file:` references to verify instructions are applied
- ✅ Check the References panel to debug customization issues

### Quick Start

1. Open the [hands-on-exercises.md](hands-on-exercises.md) file
2. Start with Exercise 1 (Custom Instructions)
3. Complete as many exercises as time allows
4. Focus on Exercises 1-2 if short on time

---

## Quick Reference: Customization Options

### File Locations

| Type | Workspace Location | User Profile |
|------|-------------------|--------------|
| Global Instructions | `.github/copilot-instructions.md` | N/A |
| Conditional Instructions | `.github/instructions/*.instructions.md` | Profile folder |
| Prompt Files | `.github/prompts/*.prompt.md` | Profile folder |
| Custom Agents | `.github/agents/*.agent.md` | Profile folder |
| Agent Skills | `.github/skills/*/SKILL.md` | `~/.copilot/skills/` |

### When to Use Each

| Use Case | Solution |
|----------|----------|
| Project coding standards | `.github/copilot-instructions.md` |
| Language-specific rules | `*.instructions.md` with `applyTo` glob |
| Common development tasks | `.prompt.md` files |
| Specialized personas | `.agent.md` files |
| Complex workflows with resources | Skills with SKILL.md |
| Planning workflows | Agent with read-only tools + handoffs |

### Variables in Prompt Files

| Variable | Description |
|----------|-------------|
| `${workspaceFolder}` | Workspace root path |
| `${file}` | Current file path |
| `${fileBasename}` | Current file name |
| `${fileDirname}` | Current file directory |
| `${selection}` | Selected text |
| `${selectedText}` | Selected text (alias) |
| `${input:name}` | User input variable |
| `${input:name:placeholder}` | User input with placeholder |

### Tool Selection

| Tool Category | Tools | Use Case |
|---------------|-------|----------|
| **Read-only** | search, fetch, githubRepo, codebase | Planning, research, analysis |
| **File operations** | create_file, edit_files, read_file | Implementation, refactoring |
| **Terminal** | run_in_terminal, get_terminal_output | Build, test, deployment |
| **Testing** | runTests, test_failure | Test generation, debugging |
| **Source control** | changes, get_changed_files | Code review, commit messages |

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Instructions not applying | Verify `github.copilot.chat.codeGeneration.useInstructionFiles` is enabled |
| Prompt file not found | Check file is in `.github/prompts/` with `.prompt.md` extension |
| Custom agent not showing | Verify file is in `.github/agents/` with `.agent.md` extension |
| Skill not loading | Enable `chat.useAgentSkills` setting and verify SKILL.md format |
| Glob pattern not matching | Test pattern syntax, ensure paths are relative to workspace root |
| Tool not available | Check tool name spelling, verify MCP server is running |
| Variables not resolving | Use correct syntax `${variableName}`, check variable is supported |
| Handoff button not appearing | Verify `handoffs` array in agent frontmatter, check target agent exists |
| Workspace customization not shared | Ensure files are committed to source control in `.github/` folder |
| User profile customization not syncing | Enable Settings Sync and include "Prompts and Instructions" |

### Debug Tips

1. **Check which instructions are loaded:**
   - Look at References section in chat response
   - Use Chat Debug view to see language model requests

2. **Verify tool availability:**
   - Use Tools picker in Chat view
   - Check MCP server status

3. **Test glob patterns:**
   - Use VS Code's file search with glob pattern
   - Verify pattern matches intended files

4. **Review agent configuration:**
   - Configure Custom Agents > hover over agent name
   - Tooltip shows source location

---

## Additional Resources

### Official Documentation
- [Custom Instructions Guide](https://code.visualstudio.com/docs/copilot/copilot-customization)
- [Prompt Files Documentation](https://code.visualstudio.com/docs/copilot/prompt-files)
- [Custom Agents Documentation](https://code.visualstudio.com/docs/copilot/custom-agents)
- [Agent Skills Standard](https://agentskills.io)

### Community Resources
- [Awesome Copilot Repository](https://github.com/github/awesome-copilot) - Community examples
- [VS Code Copilot Settings](https://code.visualstudio.com/docs/copilot/copilot-settings)
- [GitHub Copilot Documentation](https://docs.github.com/en/copilot)

### Example Repositories

Your workspace already has examples in:
- `.github/agents/` - Custom agent examples
- `.github/instructions/` - Instructions file examples
- `.github/prompts/` - Prompt file examples
- `.github/skills/` - Agent skill examples

---

## Best Practices

### Custom Instructions

1. **Be specific and concise**
   - Clear, actionable guidelines
   - Avoid ambiguity
   - Use examples

2. **Organize by topic**
   - Language standards
   - Project constraints
   - Documentation
   - Testing

3. **Use multiple files for complex projects**
   - Language-specific instructions
   - Domain-specific instructions
   - Role-specific instructions

4. **Reference, don't duplicate**
   - Link to external style guides
   - Reference existing documentation
   - Avoid repeating project docs

5. **Version control**
   - Commit to source control
   - Review changes as code
   - Document updates

### Prompt Files

1. **Make prompts reusable**
   - Use variables for flexibility
   - Avoid hardcoded values
   - Support arguments

2. **Provide clear examples**
   - Show expected input
   - Show expected output
   - Include edge cases

3. **Select appropriate tools**
   - Match tools to task
   - Don't over-specify
   - Use tool sets when appropriate

4. **Test thoroughly**
   - Use editor play button
   - Test with different inputs
   - Refine based on results

### Custom Agents

1. **Single responsibility**
   - One clear purpose per agent
   - Focused tool set
   - Specialized instructions

2. **Use handoffs for workflows**
   - Plan → Implement → Review
   - Clear transition points
   - Context preservation

3. **Tool selection matters**
   - Planning: read-only tools
   - Implementation: editing tools
   - Review: analysis tools

4. **Reference existing instructions**
   - Link to instructions files
   - Don't duplicate guidelines
   - Keep agents focused

### Agent Skills

1. **Clear, specific descriptions**
   - What the skill does
   - When to use it
   - Key capabilities

2. **Bundle related resources**
   - Templates
   - Scripts
   - Examples
   - Documentation

3. **Progressive disclosure**
   - Keep SKILL.md focused
   - Reference resources as needed
   - Don't overload context

4. **Follow open standard**
   - Use standard structure
   - Compatible across tools
   - Community shareable

---

## Frequently Asked Questions

### Q: How do I share customizations with my team?

**Short Answer:** Commit `.github/` folder files to your repository.

**Detailed Explanation:**
- All files in `.github/` (instructions, prompts, agents, skills) can be version controlled
- Team members get the same AI behavior when they clone the repo
- Changes can be reviewed via pull requests
- Use `~/.copilot/skills/` for personal customizations that shouldn't be shared

---

### Q: What's the difference between agents and skills?

**Short Answer:** Agents are personas you select; skills are auto-loaded capabilities.

**Detailed Explanation:**
- **Agents:** You explicitly select from dropdown. Define specialized behavior and tools.
- **Skills:** Automatically discovered based on prompt content. Bundled with resources.

Example:
- Agent: "Embedded Expert" - always responds with embedded constraints in mind
- Skill: "MISRA Compliance" - auto-loads when you mention MISRA or code review

---

### Q: Can I use conditional instructions for different parts of my project?

**Short Answer:** Yes, use glob patterns in `applyTo` frontmatter.

**Detailed Explanation:**
Create multiple `.instructions.md` files with different patterns:

```markdown
---
name: Firmware C++ Rules
description: C++ rules specific to firmware code
applyTo: 'Firmware/**/*.cpp'
---
# Firmware C++ Rules
```

```markdown
---
name: Python Tool Rules
description: Python rules for tooling scripts
applyTo: 'Tools/**/*.py'
---
# Python Tool Rules
```

The appropriate instructions load based on which file you're working with.

---

### Q: How do I debug why my custom agent isn't working?

**Short Answer:** Check file location, extension, and frontmatter format.

**Detailed Explanation:**
1. **Location:** Must be in `.github/agents/` or user profile
2. **Extension:** Must be `.agent.md`
3. **Frontmatter:** Must have valid YAML with `name` and `description`
4. **Tools:** Tool names must match exactly (`edit_files` not `editFiles`)
5. **Reload:** Sometimes VS Code needs restart to pick up new agents

---

### Q: Can skills reference external URLs or APIs?

**Short Answer:** Skills can include instructions for using `#fetch` tool.

**Detailed Explanation:**
Skills themselves don't make API calls, but they can:
1. Include `fetch` in their tool list
2. Provide instructions for fetching specific URLs
3. Document API endpoints and expected responses
4. The agent then uses the fetch tool during execution

---

### Q: How do handoffs work between agents?

**Short Answer:** Handoffs create buttons that switch to another agent with context.

**Detailed Explanation:**
```yaml
handoffs:
  - label: "Start Implementation"    # Button text
    agent: agent                     # Target agent
    prompt: "Implement the plan..."  # Prompt sent to target
    send: false                      # true = auto-send, false = user reviews
```

When clicked:
1. Conversation context transfers to the target agent
2. The specified prompt is queued (or sent automatically)
3. User can review and modify before sending

---

## Summary: Key Takeaways

### 1. Customization Hierarchy
- **Instructions:** Always-on coding standards
- **Prompts:** Reusable task templates (`/name`)
- **Agents:** Specialized personas (dropdown selection)
- **Skills:** Auto-loaded capabilities with resources

### 2. File Locations Matter
- `.github/copilot-instructions.md` - Global, always applied
- `.github/instructions/*.instructions.md` - Conditional via glob
- `.github/prompts/*.prompt.md` - Invoked with `/name`
- `.github/agents/*.agent.md` - Selected from dropdown
- `.github/skills/*/SKILL.md` - Auto-discovered

### 3. Front-Loading Context Wins
- Define standards once, apply everywhere
- Reduce repetition in prompts
- Ensure consistent team behavior
- Version control your AI configuration

### 4. Layer Your Customizations
- Base: Global instructions (coding standards)
- Task: Prompt files (specific workflows)
- Role: Custom agents (specialized personas)
- Capability: Skills (complex tasks with resources)

### 5. Test and Iterate
- Verify instructions are being applied
- Test prompts produce expected output
- Refine agents based on actual usage
- Share successful patterns with team

---

## Next Steps

After completing this session:

1. **Create your base customization:**
   - Start with `.github/copilot-instructions.md`
   - Add language-specific instructions as needed
   - Test and refine based on results

2. **Build prompt library:**
   - Identify common tasks in your workflow
   - Create 3-5 prompt files for most frequent tasks
   - Share with team and iterate

3. **Develop specialized agents:**
   - Create agents for different development roles
   - Add handoffs for common workflows
   - Experiment with tool combinations

4. **Create domain skills:**
   - Identify specialized knowledge areas
   - Package instructions with resources
   - Share across tools (VS Code, CLI, GitHub.com)

5. **Team adoption:**
   - Commit customizations to source control
   - Document team workflows
   - Gather feedback and improve

---

*Lesson 3: Planning & Steering Documents - GitHub Copilot Customization*  
*Last Updated: January 2026*

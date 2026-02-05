# Lesson 2: Hands-On Exercises

**Duration:** 15 minutes  
**Format:** Individual practice  
**Goal:** Practice using GitHub Copilot's three modes, chat participants, and slash commands

---

## Overview

This hands-on session lets you practice using GitHub Copilot's core features. Complete as many exercises as time allows.

| Exercise | Focus | Time |
|----------|-------|------|
| 1. Explore the Codebase | Ask Mode - Understanding unfamiliar code | 3 min |
| 2. Generate Documentation | Edit Mode - Add Doxygen comments | 3 min |
| 3. Fix a Bug | Slash Commands - Use /fix | 3 min |
| 4. Generate Unit Tests | Slash Commands - Use /tests | 3 min |
| 5. Architecture Deep Dive | Chat Participants - @workspace | 3 min |

> 💡 **Tip:** Focus on Exercises 1-3 if short on time. They demonstrate the core interaction patterns.

---

## Try Each Copilot Mode

Before starting the exercises, familiarize yourself with the three modes:

### Ask Mode
- Open Chat view (`Ctrl+Alt+I`)
- Try: "Explain this project in simple, layman's terms. What is the high-level functionality? Provide a tour of the folder structure and explain the key external dependencies."

### Edit Mode
- Open a source file (e.g., `controller.cpp`)
- Select code and press `Ctrl+I`
- Try: "Add a new configuration variable `float soft_stop_rate` to the `ControllerConfig` struct. Then, update the `update()` method in #controller.cpp to use this rate to decelerate the velocity setpoint to zero when the axis state changes to `IDLE`."

### Agent Mode
- In Chat view, switch to Agent mode
- Try: "Implement a 'Stall Detection' feature. Research how other motor controllers detect stalls. Modifications should likely involve #file:motor.cpp to monitor back-EMF vs current. Create a plan, then add the config flag and the detection logic."

> **💡 Tip:** When referencing files in your prompts, type `#file:` followed by the filename (e.g., `#file:motor.cpp`). This tells GitHub Copilot to include that file's content as context.

---

## Try Chat Participants & Slash Commands

### Chat Participants (@-mentions)

- **@workspace:**
  - Try: "@workspace Trace where `ERROR_PHASE_RESISTANCE_OUT_OF_RANGE` is defined in #file:Firmware/odrive-interface.yaml and find all occurrences where it is raised in the C++ firmware files."

- **@terminal:**
  - Try: "@terminal What is the specific `tup` command to build only the firmware for the v3.6 board variant? Explain the flags."

- **@vscode:**
  - Try: "@vscode How do I configure `launch.json` to attach the Cortex-Debug extension to an ST-Link OpenOCD server for this project?"

### Slash Commands (/-commands)

- **Code documentation:**
  1. Select a function in the editor
  2. Press `Ctrl+I` and type: `/doc`

- **Fix code errors:**
  1. Select code with errors
  2. Press `Ctrl+I` and type: `/fix`

- **Generate tests:**
  1. Select a function
  2. Press `Ctrl+I` and type: `/tests`

### Chat Tools (#-mentions)

- **Reference specific files:**
  - In Chat view: "Explain #file:motor.cpp"

- **Search codebase:**
  - In Chat view: "Find all error handling code #codebase"

---

## Exercise 1: Explore the Codebase (3 min)

**Goal:** Use Ask Mode to understand an unfamiliar codebase

<details>
<summary>📋 Instructions</summary>

1. Open Chat view (`Ctrl+Alt+I`)
2. Ask: "Explain this project in simple, layman's terms. What is the high-level functionality?"
3. Follow up: "What are the key external dependencies?"
4. Follow up: "Show me the main entry point and explain the startup sequence"

**Success Criteria:**
- ✅ Understand the project's purpose
- ✅ Know the folder structure
- ✅ Identify key dependencies
</details>

<details>
<summary>💡 Solution</summary>

**Sample prompts that work well:**
```
Explain this project in simple, layman's terms. What is the high-level 
functionality? Provide a tour of the folder structure.
```

```
What are the external dependencies in this project? List them with 
a brief description of what each provides.
```

```
Where is the main entry point? Walk me through the startup sequence 
from power-on to ready state.
```
</details>

---

## Exercise 2: Generate Documentation (3 min)

**Goal:** Use Edit Mode to document a function

<details>
<summary>📋 Instructions</summary>

1. Open a C++ source file (e.g., `motor.cpp`)
2. Find a function without documentation
3. Select the function signature
4. Press `Ctrl+I` and type: `/doc`
5. Review and accept the generated Doxygen comments

**Success Criteria:**
- ✅ Function has `@brief` description
- ✅ Parameters are documented with `@param`
- ✅ Return value documented with `@return`
</details>

<details>
<summary>💡 Solution</summary>

**Alternative approaches:**
```
Add Doxygen-style comments to this function, including @brief, 
@param for each parameter, and @return.
```

You can also be more specific:
```
Document this function for embedded developers. Include:
- Purpose and when to call it
- Parameter ranges and units
- Return value meaning
- Thread-safety notes
- Example usage
```
</details>

---

## Exercise 3: Fix a Bug with /fix (3 min)

**Goal:** Use slash commands to diagnose and fix issues

<details>
<summary>📋 Instructions</summary>

1. Find code with a known issue or warning
2. Select the problematic code
3. Press `Ctrl+I` and type: `/fix`
4. Review the suggested fix
5. Accept or refine the solution

**Success Criteria:**
- ✅ Issue is correctly identified
- ✅ Fix is appropriate and complete
- ✅ Code compiles without warnings
</details>

<details>
<summary>💡 Solution</summary>

**For more context-aware fixes:**
```
/fix This code has a race condition when accessed from ISR context. 
Suggest a thread-safe alternative.
```

**For embedded-specific fixes:**
```
/fix This uses dynamic allocation. Refactor to use static allocation 
appropriate for embedded systems.
```
</details>

---

## Exercise 4: Generate Unit Tests (3 min)

**Goal:** Use /tests to create test coverage

<details>
<summary>📋 Instructions</summary>

1. Open a file with a utility function
2. Select the function you want to test
3. In Chat, type: `/tests`
4. Review the generated test cases
5. Ask for additional edge cases if needed

**Success Criteria:**
- ✅ Tests cover normal cases
- ✅ Tests cover edge cases
- ✅ Tests follow project testing framework
</details>

<details>
<summary>💡 Solution</summary>

**For comprehensive test generation:**
```
/tests Generate doctest unit tests for this function. Include:
- Normal operation with typical inputs
- Edge cases (zero, max values, boundaries)
- Error conditions and invalid inputs
- Performance-sensitive cases if applicable
```

**Follow-up for more tests:**
```
Add tests for thread-safety: what happens if this function is 
called from multiple threads simultaneously?
```
</details>

---

## Exercise 5: Use @workspace for Architecture (3 min)

**Goal:** Understand project architecture using chat participants

<details>
<summary>📋 Instructions</summary>

1. In Chat, ask: `@workspace How does the motor control loop work?`
2. Follow up: `@workspace Show me the relationship between Axis, Motor, and Encoder classes`
3. Ask: `@workspace Where are errors defined and how are they propagated?`

**Success Criteria:**
- ✅ Understand control loop architecture
- ✅ See class relationships
- ✅ Understand error handling patterns
</details>

<details>
<summary>💡 Solution</summary>

**Effective @workspace queries:**
```
@workspace Explain the high-level architecture of Firmware/MotorControl. 
How do the Axis, Motor, and Encoder classes interact during the control loop?
```

```
@workspace Create a Mermaid diagram showing the state machine in axis.cpp
```

```
@workspace What error codes are defined and where are they raised? 
Show me the error propagation pattern.
```
</details>

---

## 📊 Summary: What You've Practiced

| Exercise | Mode/Feature | Key Skill |
|----------|--------------|-----------|
| 1. Explore Codebase | Ask Mode | Understanding unfamiliar code |
| 2. Generate Docs | Edit Mode + /doc | Inline documentation generation |
| 3. Fix Bugs | Edit Mode + /fix | Diagnostic and repair |
| 4. Generate Tests | /tests | Test coverage creation |
| 5. Architecture | @workspace | Project-wide understanding |

**Key Takeaway:** Match the mode to the task - Ask for exploration, Edit for changes, Agent for complex multi-step work.

---

## Bonus Challenges (If Time Permits)

### Challenge 1: Multi-File Refactoring
Use Agent Mode to rename a function across multiple files:
```
Find all usages of the function `delay_us` and rename it to `delay_microseconds`. 
Update all call sites and add documentation explaining the new name.
```

### Challenge 2: Architecture Diagram
Ask Copilot to generate a visual:
```
@workspace Create a Mermaid sequence diagram showing how a position 
command flows from USB input through the control loop to PWM output.
```

### Challenge 3: Error Analysis
Use @workspace to find patterns:
```
@workspace List all error codes in the project and for each one, 
show where it's raised and how it's handled. Are there any that 
are defined but never used?
```

### Challenge 4: Code Review
Use Agent Mode to review code quality:
```
Review #file:motor.cpp for:
- Thread safety issues
- Missing error handling
- Documentation gaps
- Code that doesn't follow embedded best practices

Provide a prioritized list of improvements.
```

---

## 🔍 Troubleshooting

| Issue | Solution |
|-------|----------|
| Chat view not opening | Verify GitHub Copilot Chat extension is installed |
| Inline chat not appearing | Press `Ctrl+I` with code selected |
| @workspace not finding files | Ensure workspace is opened as folder (not just file) |
| Suggestions not relevant | Add more context with `#file:` references |
| /doc not generating comments | Select the full function signature |

---

## Quick Reference

### Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+Alt+I` | Open Chat view |
| `Ctrl+I` | Inline chat (in editor) |
| `Ctrl+Shift+Alt+L` | Quick Chat |
| `Tab` | Accept inline suggestion |
| `Escape` | Dismiss suggestion |

### Chat Participants

| Participant | Use For |
|-------------|---------|
| `@workspace` | Project-wide questions |
| `@terminal` | Shell/build commands |
| `@vscode` | IDE configuration |
| `@github` | Repos, issues, PRs |

### Slash Commands

| Command | Use For |
|---------|---------|
| `/doc` | Generate documentation |
| `/explain` | Explain code |
| `/fix` | Fix errors |
| `/tests` | Generate tests |

---

*Lesson 2 Hands-On Exercises - GitHub Copilot Basic Features*  
*Duration: 15 minutes*

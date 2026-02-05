# Section 2 Demo Script: Basic Feature Overview

**Duration:** 45 minutes total (demos ~30 min, hands-on ~15 min)  
**Presenter Notes:** This script provides exact steps and dialogue for live demos

---

## Demo 1: Copilot Chat Modes (10 min)

### Setup
- Open VS Code with ODrive workspace
- Have `Firmware/MotorControl/motor.cpp` visible
- Open Copilot Chat panel (Ctrl+Alt+I)
- Ensure Chat mode selector is visible at top of chat

### Demo Flow

---

### Part A: Ask Mode (3 min)

**Presenter Says:**
> "Let's start with Ask Mode - this is your go-to for understanding code, getting explanations, and asking questions without making changes."

**Show the Mode Selector:**
> "Notice at the top of the Chat panel, you can switch between Ask, Edit, and Agent modes. We're in Ask mode by default."

**Type in Chat:**
```
Explain this project in simple, layman's terms. What is the high-level functionality? Provide a tour of the folder structure and explain the key external dependencies.
```

**Presenter Says:**
> "Watch how Copilot analyzes the workspace and gives us a comprehensive overview. This is perfect for onboarding or understanding unfamiliar codebases."

**Wait for response, then continue:**

**Type in Chat:**
```
How is the custom fibre protocol implemented in #file:Firmware/fibre-cpp/fibre.cpp? Explain how an object method on the firmware side is exposed to the Python client.
```

**Presenter Says:**
> "Notice I used `#file:` to reference a specific file. This gives Copilot precise context. Ask Mode is read-only - it won't modify your code, just explain and answer questions."

**Key Points to Highlight:**
- ✅ Great for learning new codebases
- ✅ Understanding complex logic
- ✅ Debugging assistance
- ✅ Architecture questions
- ❌ Does NOT modify files

---

### Part B: Edit Mode (3 min)

**Presenter Says:**
> "Now let's switch to Edit Mode. This is where Copilot actually modifies your code."

**Click the Mode Selector → Select "Edit"**

**Open File:** `Firmware/MotorControl/controller.hpp`

**Type in Chat:**
```
Add a new configuration variable `float soft_stop_rate` to the ControllerConfig struct with a default value of 10.0f and add a Doxygen comment explaining it controls the deceleration rate when stopping.
```

**Presenter Says:**
> "Watch how Copilot shows a diff preview before applying changes. You can review, accept, or reject each change."

**Show the diff preview, then accept:**

> "Edit Mode is perfect for:
> - Adding documentation
> - Refactoring code
> - Implementing specific features
> - Code generation within a file
>
> The key difference from Ask Mode: it actually modifies your files!"

**Alternative Demo - Inline Chat:**

**Presenter Says:**
> "You can also use Edit mode inline. Let me show you."

**Select a function in the editor, then press Ctrl+I**

**Type:**
```
/doc
```

**Presenter Says:**
> "The `/doc` slash command generates documentation for the selected code. Inline chat is faster for quick edits right where you're working."

**Alternative Demo - Cross-File Refactoring:**

**Presenter Says:**
> "Edit mode is also great for refactoring that spans multiple files. Let me show you a rename operation."

**Type in Chat:**
```
Rename the function delay_us to delay_ust in #file:Firmware/MotorControl/utils.hpp and update all files that call this function to use the new name.
```

**Presenter Says:**
> "Watch how Copilot finds all usages in drv8301.cpp and board.cpp and updates them together. This is much safer than find-and-replace!"

---

### Part C: Agent Mode (4 min)

**Presenter Says:**
> "Agent Mode is the most powerful - it can plan, execute multi-step tasks, run terminal commands, and work across multiple files autonomously."

**Click the Mode Selector → Select "Agent"**

**Type in Chat:**
```
Find all instances of raw `printf` debug statements in the Firmware/MotorControl/ directory and list them with file names and line numbers. Then suggest a better logging approach using a macro.
```

**Presenter Says:**
> "Watch how Agent mode:
> 1. Plans the task
> 2. Searches across multiple files
> 3. Executes the search
> 4. Provides actionable recommendations
>
> Agent mode can also run terminal commands, build code, and fix errors automatically!"

**Show Another Agent Example:**

**Type in Chat:**
```
Build the ODrive firmware by running the tup build command. If there are errors, analyze them and suggest fixes.
```

**Presenter Says:**
> "Agent mode will:
> - Run the build command in the terminal
> - Parse the output
> - Identify errors
> - Suggest or apply fixes
>
> This is like having a junior developer who can execute tasks end-to-end!"

**Summary Table to Show:**

| Mode | Modifies Files | Runs Commands | Best For |
|------|----------------|---------------|----------|
| **Ask** | ❌ No | ❌ No | Questions, explanations, learning |
| **Edit** | ✅ Yes | ❌ No | Code changes, refactoring, docs |
| **Agent** | ✅ Yes | ✅ Yes | Multi-step tasks, automation |

---

## Demo 2: Chat Participants & Slash Commands (10 min)

### Setup
- Keep ODrive workspace open
- Have Chat panel visible
- Have a terminal open

### Demo Flow

---

### Part A: @workspace Participant (3 min)

**Presenter Says:**
> "Chat participants let you target specific domains. The most powerful is `@workspace` - it searches your entire project."

**Type in Chat:**
```
@workspace Trace where ERROR_PHASE_RESISTANCE_OUT_OF_RANGE is defined and find all places where it is raised in the C++ firmware files.
```

**Presenter Says:**
> "Notice I prefixed with `@workspace`. This tells Copilot to search the entire codebase, not just open files. Without it, Copilot might not find all references."

**Wait for response, highlight the results:**

> "It found:
> - The definition in the interface YAML
> - All the places it's raised in C++ code
> - The error handling paths
>
> This would take you 10-15 minutes to trace manually!"

---

### Part B: @terminal Participant (2 min)

**Presenter Says:**
> "The `@terminal` participant is great for shell commands and understanding terminal output."

**Type in Chat:**
```
@terminal What is the tup command to build only the firmware for the v3.6 board variant? Explain the flags.
```

**Presenter Says:**
> "It knows your project's build system and can generate the exact command. Let me try another one."

**Type in Chat:**
```
@terminal list the 5 largest files in the Firmware directory, sorted by size
```

**Presenter Says:**
> "You can ask it to generate commands, explain errors, or help with shell scripting. It understands the terminal context."

---

### Part C: @vscode Participant (2 min)

**Presenter Says:**
> "Need help with VS Code itself? Use `@vscode`."

**Type in Chat:**
```
@vscode How do I configure launch.json to attach the Cortex-Debug extension to an ST-Link OpenOCD server for debugging this STM32 project?
```

**Presenter Says:**
> "It generates a complete debug configuration tailored to this project. No more Googling for JSON snippets!"

**Type in Chat:**
```
@vscode Create a tasks.json entry to run the tup build command when I press Ctrl+Shift+B
```

---

### Part D: Slash Commands (3 min)

**Presenter Says:**
> "Slash commands are shortcuts for common operations. Let me show you the most useful ones."

**Open a file with a function, select the function code**

**Demo /explain:**

**Press Ctrl+I (Inline Chat), Type:**
```
/explain
```

**Presenter Says:**
> "The `/explain` command breaks down what the selected code does. Great for complex algorithms."

**Demo /fix:**

**Find or create code with an error, select it**

**Press Ctrl+I, Type:**
```
/fix
```

**Presenter Says:**
> "`/fix` automatically identifies and corrects errors. It reads compiler messages and applies fixes."

**Demo /tests:**

**Select a function**

**Press Ctrl+I, Type:**
```
/tests
```

**Presenter Says:**
> "`/tests` generates unit tests for the selected code. It follows your project's testing patterns."

**Demo /doc:**

**Select a function without documentation**

**Press Ctrl+I, Type:**
```
/doc
```

**Presenter Says:**
> "`/doc` adds documentation comments. It infers parameter meanings from the code."

**Quick Reference to Show:**

| Command | What It Does |
|---------|--------------|
| `/explain` | Explain selected code |
| `/fix` | Fix errors in selection |
| `/tests` | Generate unit tests |
| `/doc` | Add documentation |
| `/new` | Create new file/workspace |

---

## Demo 3: Inline Chat & Chat Tools (10 min)

### Setup
- Open `Firmware/MotorControl/foc.cpp`
- Have editor visible with Chat panel

### Demo Flow

---

### Part A: Inline Chat (4 min)

**Presenter Says:**
> "Inline Chat lets you work directly in the editor without switching to the Chat panel."

**Position cursor inside a function**

**Press Ctrl+I**

**Presenter Says:**
> "See the inline chat box? I can ask questions or request changes right here."

**Type:**
```
Add input validation to check if the voltage is within the DC bus voltage limits
```

**Presenter Says:**
> "The code is inserted right where I'm working. No copy-paste needed!"

**Show ghost text completions:**

**Start typing a new function:**
```cpp
// Calculate the magnitude of a 3-phase current
float calculate_current_magnitude(
```

**Presenter Says:**
> "See the ghost text? Copilot predicts what you're about to write. Press Tab to accept, or keep typing to refine."

**Press Tab to accept the suggestion**

---

### Part B: Chat Tools with # (3 min)

**Presenter Says:**
> "Chat tools let you reference specific context. They start with `#`."

**Type in Chat Panel:**
```
Compare the error handling in #file:Firmware/MotorControl/motor.cpp vs #file:Firmware/MotorControl/axis.cpp. Which approach is more robust?
```

**Presenter Says:**
> "I used `#file:` to reference two specific files for comparison. Other useful tools:"

**Type in Chat:**
```
Search for all TODO comments #codebase
```

**Presenter Says:**
> "`#codebase` searches the entire workspace. It's like `@workspace` but as a tool reference."

**Demo #selection:**

**Select some code in the editor**

**Type in Chat:**
```
Explain this code #selection and suggest optimizations
```

**Presenter Says:**
> "`#selection` references whatever you have selected in the editor. No need to copy-paste code into chat!"

**Quick Reference:**

| Tool | Usage |
|------|-------|
| `#file:path` | Reference specific file |
| `#codebase` | Search entire workspace |
| `#selection` | Current editor selection |
| `#terminalSelection` | Terminal selection |
| `#changes` | Git changes |
| `#fetch` | Fetch web content |

---

### Part C: Multi-File Editing (3 min)

**Presenter Says:**
> "Agent mode can edit multiple files in one operation. Let me show you."

**Switch to Agent Mode**

**Type in Chat:**
```
Add a new error code ERROR_MOTOR_STALL to the system:
1. Add it to Firmware/odrive-interface.yaml in the errors enum
2. Add a check in Firmware/MotorControl/motor.cpp that raises this error when velocity is near zero but current is high
3. Add documentation for this error in docs/troubleshooting.rst

Show me a plan before making changes.
```

**Presenter Says:**
> "Agent mode:
> 1. Creates a plan
> 2. Shows you what it will do
> 3. Waits for approval
> 4. Executes across all files
>
> You stay in control, but the AI handles the tedious multi-file coordination!"

---

## Hands-On Exercises (15 min)

**Presenter Says:**
> "Now it's your turn! Try these exercises. I'll walk around to help."

### Exercise 1: Ask Mode (3 min)
```
Try this prompt in Ask mode:
"Explain the high-level architecture of Firmware/MotorControl. 
How do the Axis, Motor, and Encoder classes interact during the control loop?"
```

### Exercise 2: Edit Mode with Inline Chat (4 min)
```
1. Open any .cpp file
2. Select a function
3. Press Ctrl+I
4. Type: /doc
5. Accept the generated documentation
```

### Exercise 3: @workspace Search (3 min)
```
Try: "@workspace Find all places where DMA transfers are configured 
for ADC readings and explain the flow"
```

### Exercise 4: Agent Mode Task (5 min)
```
Try: "Create a simple Python script in tools/ that connects to an ODrive 
and prints the firmware version. Use the existing odrive library patterns 
from other scripts in the directory."
```

---

## Demo Tips for Presenters

### Before the Demo
- [ ] Verify Copilot is signed in and working
- [ ] Have ODrive repo cloned and indexed
- [ ] Pre-open key files for quick navigation
- [ ] Clear Chat history for clean demo
- [ ] Test keyboard shortcuts work

### During the Demo
- **Pause after each response** - Let audience read
- **Highlight mode differences** - Ask vs Edit vs Agent
- **Show the mode selector** - Many people miss it
- **Demonstrate failures** - Show that iteration is normal
- **Use keyboard shortcuts** - Ctrl+Alt+I, Ctrl+I, etc.

### Common Issues & Recovery
- **Slow response:** "Cloud models can take a moment. While we wait..."
- **Poor output:** "This is why we iterate! Let me refine the prompt..."
- **Wrong mode:** "Oops, I was in Ask mode. Let me switch to Edit..."
- **Network error:** Show pre-captured screenshots

### Time Management
- Demo 1 (Modes): 10 min strict
- Demo 2 (Participants): 10 min strict
- Demo 3 (Inline/Tools): 10 min (can trim)
- Hands-On: 15 min (can extend if ahead)

---

## Audience Engagement

### Questions to Ask

**After Demo 1:**
> "Who's been using Copilot primarily in Ask mode? [hands]  
> Who knew about Agent mode before today? [hands]  
> Agent mode is a game-changer for multi-file tasks!"

**After Demo 2:**
> "How many of you use `@workspace` regularly? [hands]  
> It's one of the most underused features!"

**After Demo 3:**
> "What's your preferred way to interact - Chat panel or Inline? [discuss]"

### Expected Questions

**Q: "What's the difference between @workspace and #codebase?"**
A: "@workspace is a participant that handles project-wide questions. #codebase is a tool that triggers a code search. Similar results, slightly different usage patterns."

**Q: "Can I use multiple modes in one session?"**
A: "Absolutely! Switch modes as needed. Use Ask to understand, Edit to implement, Agent for complex tasks."

**Q: "Is my code sent to the cloud?"**
A: "Yes, GitHub Copilot uses cloud models. For air-gapped needs, see our Foundry Local section later."

**Q: "How do I know which mode to use?"**
A: "Simple rule: Ask = questions, Edit = code changes, Agent = multi-step tasks or running commands."

---

## Keyboard Shortcut Cheat Sheet

| Action | Shortcut |
|--------|----------|
| Open Chat Panel | `Ctrl+Alt+I` |
| Inline Chat | `Ctrl+I` |
| Quick Chat | `Ctrl+Shift+Alt+L` |
| New Chat Session | `Ctrl+N` (in chat) |
| Accept Suggestion | `Tab` |
| Dismiss Suggestion | `Escape` |
| Model Picker | `Ctrl+Alt+.` |

---

**Demo Script Version:** 1.0  
**Last Updated:** January 2026

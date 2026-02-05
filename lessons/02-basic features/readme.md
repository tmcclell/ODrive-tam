# Lesson 2: Basic Feature Overview - GitHub Copilot in VS Code

**Session Duration:** 45 minutes  
**Audience:** Embedded/C++ Developers/Project Managers/QA engineers/Firmware Engineers  
**Environment:** Windows, VS Code  
**Extensions:** GitHub Copilot  
**Source Control:** GitHub/Bitbucket

---

## Overview

This lesson introduces the core features of GitHub Copilot in VS Code. You'll learn the three primary interaction modes, how to leverage chat participants and slash commands, and how to maximize productivity with inline suggestions and multi-file editing.

**What You'll Learn:**
- The three Copilot modes (Ask, Edit, Agent) and when to use each
- Chat participants (`@workspace`, `@terminal`, `@vscode`, `@github`) for domain-specific assistance
- Slash commands (`/fix`, `/explain`, `/tests`, `/doc`) for common tasks
- Inline suggestions and multi-file editing workflows

**Key Concepts:**

| Concept | Description |
|---------|-------------|
| **Ask Mode** | Question-and-answer about code, architecture, debugging |
| **Edit Mode** | Code generation, refactoring, documentation via Inline Chat |
| **Agent Mode** | Autonomous multi-step tasks across files with planning |
| **Chat Participants** | `@`-prefixed helpers for project, terminal, IDE, and GitHub |
| **Slash Commands** | `/`-prefixed shortcuts for common operations |

---

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Why Copilot Features Matter](#why-copilot-features-matter)
- [Learning Path](#learning-path)
- [Agenda](#agenda-basic-feature-overview-45-min)
- [Get Started with Chat in VS Code](#1-get-started-with-chat-in-vs-code)
- [Copilot Chat Modes](#2-copilot-chat-modes)
- [Chat Participants & Slash Commands](#3-chat-participants--slash-commands)
- [Inline Chat & Copilot Edits](#4-inline-chat--copilot-edits)
- [**Hands-On Exercises**](#hands-on-exercises)
- [Quick Reference](#quick-reference-github-copilot-chat)
- [Tips for Effective Prompts](#tips-for-effective-prompts)
- [Explore Different Language Models](#explore-different-language-models)
- [Customize Your Chat Experience](#customize-your-chat-experience)
- [Troubleshooting](#troubleshooting)
- [Additional Resources](#additional-resources)
- [Frequently Asked Questions](#frequently-asked-questions)
- [Summary: Key Takeaways](#summary-key-takeaways)
- [Next Steps](#next-steps)

---

## Prerequisites

Before starting this session, ensure you have:

- ✅ **Visual Studio Code** installed (latest version recommended)
- ✅ **GitHub Copilot subscription** (or Copilot Free plan with monthly limits)
- ✅ Active internet connection for AI model access
- ✅ A GitHub account for authentication

---

## Copilot Feature Availability Across IDEs

GitHub Copilot features vary by IDE. This workshop focuses on **VS Code**, which has the most comprehensive feature set.

![Copilot Features by IDE](copilot-features-by-ide.png)

| Feature | VS Code | Visual Studio | JetBrains | Eclipse | Xcode | Neovim | GitHub.com |
|---------|:-------:|:-------------:|:---------:|:-------:|:-----:|:------:|:----------:|
| **Code completion** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Copilot Chat** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Model picker** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Agent mode** | ✅ | ✅ | ✅ | ✅ | ✅ | | ✅ |
| **Edit mode** | ✅ | ✅ | ✅ | | | | |
| **Extensions** | ✅ | ✅ | ✅ | | | | ✅ |
| **Code review** | ✅ | ✅ | | | | | ✅ |
| **Custom instructions** | ✅ | ✅ | | | | | |
| **MCP Support** | ✅ | ✅ | ✅ | ✅ | ✅ | | ✅ |
| **PR summaries** | | | | | | | ✅ |
| **Text completions** | | | | | | | ✅ |
| **Copilot Spaces** | | | | | | | ✅ |
| **Copilot Coding Agent** | | | | | | | ✅ |

> **Note:** VS Code and Visual Studio offer the richest Copilot experience with Edit mode, Code review, and Custom instructions support.

---

### Verify Your Setup

1. **Check VS Code installation:**
   - Open VS Code
   - Press `Ctrl+Shift+P` and type "About" to verify version

2. **Verify Copilot subscription:**
   - Visit https://github.com/settings/copilot
   - Ensure you have an active subscription or free plan

3. **Install GitHub Copilot extension:**
   - Open VS Code Extensions (Ctrl+Shift+X)
   - Search for "GitHub Copilot"
   - Install both "GitHub Copilot" and "GitHub Copilot Chat" extensions

---

## Why Copilot Features Matter

Understanding the full range of Copilot features enables you to:

### Benefits of Mastering Copilot Features

1. **Right Tool for the Job**
   - Ask Mode for exploration and understanding
   - Edit Mode for targeted code changes
   - Agent Mode for complex multi-file tasks

2. **Faster Code Comprehension**
   - Use `@workspace` to understand project architecture
   - Use `/explain` to decode unfamiliar code
   - Use `#file:` references for context-aware answers

3. **Accelerated Development**
   - Generate tests with `/tests`
   - Fix bugs with `/fix`
   - Document code with `/doc`

4. **Seamless IDE Integration**
   - Terminal assistance with `@terminal`
   - VS Code settings help with `@vscode`
   - GitHub integration with `@github`

---

## Learning Path

This lesson covers five key areas. Work through them sequentially for the best learning experience.

| Topic | What You'll Learn | Estimated Time |
|-------|-------------------|----------------|
| Get Started with Chat | Three ways to access Copilot Chat | 5 min |
| Copilot Chat Modes | Ask, Edit, Agent - when to use each | 10 min |
| Chat Participants & Slash Commands | `@workspace`, `@terminal`, `/fix`, `/explain` | 10 min |
| Inline Chat & Copilot Edits | In-editor completions, multi-file editing | 10 min |
| **[Hands-On Exercises](hands-on-exercises.md)** | Try each mode with walkthrough | 10 min |

---

## Agenda: Basic Feature Overview (45 min)

| Sub-Topic                        | Focus                                               | Time   |
|----------------------------------|-----------------------------------------------------|--------|
| Copilot Chat Modes               | Ask, Edit, Agent mode - when to use each            | 10 min |
| Chat Participants & Slash Cmds   | @workspace, @terminal, /fix, /explain, /tests, /doc | 10 min |
| Inline Chat & Copilot Edits      | In-editor completions, multi-file editing           | 10 min |
| **[Hands-On Exercises](hands-on-exercises.md)** | Try each mode with walkthrough in VS Code  | 15 min |

---

## 1. Get Started with Chat in VS Code

Chat in Visual Studio Code enables you to use natural language for AI-powered coding assistance. Ask questions about your code, get help understanding complex logic, generate new features, fix bugs, and more - all through a conversational interface.

### Access Chat in VS Code
**🎯 Copilot Mode: All Modes**

VS Code provides three ways to start an AI chat conversation, each optimized for different workflows and tasks:

#### 1. Chat View
Press **Ctrl+Alt+I** to open the Chat view in a dedicated side panel. If you prefer a larger workspace for chat, you can open it as an editor tab by selecting **New Chat Editor** from the chat menu or as a separate window by selecting **New Chat Window**.

**Use the Chat view for:**
- Ongoing, multi-turn chat conversations
- Switching between different agents to ask questions, make code edits across files, or start autonomous coding workflows
- Working on features that span multiple files
- Planning and implementing complex changes

#### 2. Inline Chat
Press **Ctrl+I** to start a chat conversation directly in your editor or terminal.

**Use inline chat for:**
- Getting suggestions inline, right where you're working
- Understanding code in your current context
- Getting help with terminal commands and output

#### 3. Quick Chat
Press **Ctrl+Shift+Alt+L** to open a lightweight chat overlay.

**Use quick chat for:**
- Quick questions that don't require extended conversation
- Getting answers without changing your current view
- Looking up information while maintaining focus on your work

### Essential Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+Alt+I` | Open the Chat view |
| `Ctrl+I` | Enter voice chat prompt in Chat view / Start inline chat in editor or terminal |
| `Ctrl+I` (hold) | Start inline voice chat |
| `Ctrl+N` | Start a new chat session in Chat view |
| `Ctrl+Shift+I` | Switch to using agents in Chat view |
| `Ctrl+Shift+Alt+L` | Open Quick Chat |
| `Ctrl+Alt+.` | Show the model picker to select a different AI model |
| `Tab` | Accept inline suggestion or navigate to next edit suggestion |
| `Escape` | Dismiss inline suggestion |
| `F2` | Get AI-powered suggestions when renaming symbols |

---

## 2. Copilot Chat Modes

### Ask Mode
**🎯 Purpose: Questions, exploration, understanding**

Use the Chat panel to ask questions about the codebase, architecture, or debug issues.

**Example Prompts:**
```
Explain this project in simple, layman's terms. What is the high-level 
functionality? Provide a tour of the folder structure and explain the 
key external dependencies.
```

```
Explain the high-level architecture of Firmware/MotorControl. How do 
the Axis, Motor, and Encoder classes interact during the control loop?
```

```
How is the custom fibre protocol implemented in #file:fibre.cpp? 
Explain how an object method on the firmware side is exposed to the 
Python client.
```

```
I am seeing ERROR_OVERSPEED in my logs. Look at #file:axis.cpp and 
#file:motor.cpp. What specific conditions trigger this error and how 
can I disable it safely?
```

```
What is the pin mapping for the SPI interface in #file:board.cpp? 
Can you list the GPIO pins used for the DRV8301?
```

```
How does the build system work in this repo? Detailed check of 
#file:Tupfile.lua and explain how the firmware binary is generated 
from the source files.
```

### Edit Mode
**🎯 Purpose: Code generation, refactoring, documentation**

Use Inline Chat (Ctrl+I) to generate/refactor code, add documentation, or create scripts/tests.

**Example Prompts:**
```
Add a new configuration variable `float soft_stop_rate` to the 
ControllerConfig struct. Then, update the update() method in 
controller.cpp to use this rate to decelerate the velocity setpoint 
to zero when the axis state changes to IDLE.
```

```
Create a new test case using the doctest framework that verifies 
the SVM utility function in #file:utils.cpp handles all six sextants 
correctly with edge-case inputs.
```

```
Add Doxygen-style comments to the on_measurement function in 
#file:foc.cpp, explaining the Clarke transform and the current 
measurement processing.
```

```
Write a Python script using odrive.enums that connects to a generic 
ODrive, configures the plotter, and graphs 
axis0.motor.current_control.Iq_measured and 
axis0.motor.current_control.Iq_setpoint in real-time.
```

```
Optimize the SVM function in #file:utils.cpp using SIMD intrinsics 
or faster math approximations if possible, while maintaining 
readability and accuracy.
```

```
Rename the function delay_us to delay_ust in #file:utils.hpp and 
update all files that call this function to use the new name.
```

### Agent Mode
**🎯 Purpose: Multi-step, cross-file, autonomous tasks**

Use for complex tasks where Copilot can plan, research, and execute changes.

**Example Prompts:**
```
Build the ODrive firmware by running the tup build command in the 
terminal. If there are compilation errors, analyze the error output, 
identify which files are causing issues, examine the problematic code, 
suggest fixes, apply them, and then rebuild to verify the fixes work.
```

```
Implement a 'Stall Detection' feature. Research how other motor 
controllers detect stalls. Modifications should likely involve 
#file:motor.cpp to monitor back-EMF vs current. Create a plan, 
then add the config flag and the detection logic.
```

```
Create a new test plan in tools/integration_tests/ that uses the 
odrivetool library. It should sequence a full calibration, enter 
closed loop control, perform a sinusoidal position move, and verify 
the final position error is within tolerance.
```

```
I want to port the Arduino/ArduinoI2C library to work with an 
STM32-based Arduino core. Analyze #file:ArduinoI2C.ino and identify 
which hardware-specific AVR registers or libraries need to be abstracted.
```

```
Find all instances of raw printf debug statements across the Firmware/ 
directory. Replace them with a new macro ODRIVE_LOG() that respects 
a compile-time log level defined in #file:freertos_vars.h.
```

```
Read #file:docs/error-codes.rst and compare it against the actual 
error enums defined in #file:odrive-interface.yaml. List any 
discrepancies where the documentation is missing new error codes 
added to the code.
```

---

## 3. Chat Participants & Slash Commands

### Chat Participants (@-mentions)
Chat participants handle domain-specific requests. VS Code provides built-in participants:

| Participant | Description | Example |
|-------------|-------------|---------|
| `@workspace` | Project-wide context and code search | `@workspace how is authentication implemented?` |
| `@terminal` | Shell commands and terminal help | `@terminal list the 5 largest files` |
| `@vscode` | VS Code features and settings | `@vscode how to enable word wrapping?` |
| `@github` | GitHub repos, issues, and PRs | `@github What are all open PRs assigned to me?` |

**@workspace Examples:**
```
@workspace Trace where ERROR_PHASE_RESISTANCE_OUT_OF_RANGE is defined 
in #file:Firmware/odrive-interface.yaml and find all occurrences where 
it is raised in the C++ firmware files.
```

```
@workspace Explain the odrive_firmware_pkg definition in 
#file:Firmware/Tupfile.lua. How does it gather the source files from 
MotorControl/ and communication/?
```

```
@workspace how is authentication implemented?
```

**@terminal Examples:**
```
@terminal What is the specific tup command to build only the firmware 
for the v3.6 board variant? Explain the flags.
```

```
@terminal scan tools/setup.py and generate the pip command to install 
all development dependencies for the Python tools.
```

```
@terminal list the 5 largest files in this workspace
```

**@vscode Examples:**
```
@vscode How do I configure launch.json to attach the Cortex-Debug 
extension to an ST-Link OpenOCD server for this project?
```

```
@vscode Create a tasks.json entry to run the tup build command 
when I press Ctrl+Shift+B.
```

```
@vscode how to enable word wrapping?
```

### Slash Commands (/-commands)
Slash commands are shortcuts to specific functionality:

| Slash Command | Description |
|---------------|-------------|
| `/doc` | Generate code documentation comments |
| `/explain` | Explain a code block, file, or concept |
| `/fix` | Fix a code block or resolve errors |
| `/tests` | Generate tests for selected methods |
| `/setupTests` | Get help setting up a testing framework |
| `/clear` | Start a new chat session |
| `/new` | Scaffold a new workspace or file |
| `/newNotebook` | Scaffold a new Jupyter notebook |
| `/search` | Generate a search query for the Search view |
| `/startDebugging` | Generate launch.json and start debugging |

### Chat Tools (#-mentions)
Use tools in chat to accomplish specialized tasks:

| Chat Tool | Description |
|-----------|-------------|
| `#file` | Reference a specific file as context |
| `#codebase` | Perform a code search in the workspace |
| `#selection` | Get the current editor selection |
| `#terminalSelection` | Get the current terminal selection |
| `#changes` | List of source control changes |
| `#fetch` | Fetch content from a web page |
| `#githubRepo` | Perform a code search in a GitHub repo |
| `#extensions` | Search for and ask about VS Code extensions |

---

## 4. Inline Chat & Copilot Edits

### Inline Suggestions
Start typing and Copilot provides inline suggestions. Press `Tab` to accept.

**Tips for better inline suggestions:**
- Write descriptive comments before the code
- Use meaningful variable and function names
- Provide context with comments like `// Generate SPI driver for...`

### Inline Chat (Ctrl+I)
Select code and press Ctrl+I to open inline chat for:
- Adding documentation
- Refactoring code
- Fixing errors
- Generating tests

### Multi-File Editing (Agent Mode)
Agent Mode can make coordinated changes across multiple files:
- Header and source file updates
- Interface changes with all implementations
- Refactoring with test updates

---
## Hands-On Exercises

For detailed hands-on exercises with step-by-step instructions, see the separate exercises document:

📋 **[Hands-On Exercises](hands-on-exercises.md)** (15 min)

The exercises cover:
- **Exercise 1:** Explore the Codebase (Ask Mode)
- **Exercise 2:** Generate Documentation (Edit Mode + /doc)
- **Exercise 3:** Fix a Bug (/fix command)
- **Exercise 4:** Generate Unit Tests (/tests command)
- **Exercise 5:** Architecture Deep Dive (@workspace)

> 💡 **Tip:** Focus on Exercises 1-3 if short on time. They demonstrate the core interaction patterns.

---

## Quick Reference: GitHub Copilot Chat

### Essential Commands

| Command/Prompt | What It Does |
|----------------|-------------|
| `Ctrl+Alt+I` | Open the Chat view in the Secondary Side Bar |
| `Ctrl+I` | Start inline chat in editor or terminal |
| `Ctrl+Shift+Alt+L` | Open Quick Chat |
| `Ctrl+N` | Start a new chat session in Chat view |
| `Ctrl+Alt+.` | Show model picker to select different AI model |

### Modes at a Glance

| Mode | Shortcut | Best For |
|------|----------|----------|
| Ask | Chat view | Questions, exploration, understanding |
| Edit | Ctrl+I | Code changes, refactoring, documentation |
| Agent | Chat with complex task | Multi-file, autonomous, planning |

### Chat Participants

| Participant | Description | Example |
|-------------|-------------|---------|
| `@workspace` | Project-wide context and code search | `@workspace how is authentication implemented?` |
| `@terminal` | Shell commands and terminal help | `@terminal list the 5 largest files` |
| `@vscode` | VS Code features and settings | `@vscode how to enable word wrapping?` |
| `@github` | GitHub repos, issues, and PRs | `@github What are all open PRs assigned to me?` |

### Slash Commands

| Command | Description |
|---------|-------------|
| `/doc` | Generate code documentation |
| `/explain` | Explain code or concepts |
| `/fix` | Fix code errors |
| `/tests` | Generate unit tests |
| `/setupTests` | Setup testing framework |
| `/new` | Scaffold new workspace/file |
| `/newNotebook` | Create Jupyter notebook |
| `/startDebugging` | Generate debug config |

### Chat Tools (# commands)

| Tool | Description |
|------|-------------|
| `#file` | Reference a specific file |
| `#codebase` | Search the workspace |
| `#selection` | Current editor selection |
| `#terminalSelection` | Current terminal selection |
| `#changes` | Source control changes |
| `#fetch` | Fetch web page content |
| `#githubRepo` | Search GitHub repository |
| `#extensions` | Search VS Code extensions |

### Prompt Tips

1. **Add context with #file:** - Reference specific files
2. **Be specific** - Clear requirements get better results
3. **Iterate** - Refine responses with follow-up questions
4. **Use constraints** - Specify patterns, standards, limitations

### Inline Suggestions

| Action | Description |
|--------|-------------|
| Start typing | Get inline suggestions as you code |
| `Tab` | Accept suggestion |
| `Escape` | Dismiss suggestion |
| Code comments | Provide prompt via comments (e.g., `# write a calculator class`) |

### Editor AI Features

| Action | Description |
|--------|-------------|
| `Ctrl+I` | Start inline chat in editor |
| `F2` | AI-powered symbol renaming |
| Right-click → Generate Code | Access common AI actions (explain, fix, test, review) |
| Lightbulb (Code Actions) | Fix linting/compiler errors |

### Source Control & Issues

| Feature | Description |
|---------|-------------|
| Generate commit message | AI-generated commit messages based on changes |
| PR description | Generate pull request titles and descriptions |
| `@github` participant | Query issues, PRs, and commits |

---

## Tips for Effective Prompts

1. **Add context with #-mentions:** Reference specific files (`#file`), your codebase (`#codebase`), or terminal output (`#terminalSelection`)

2. **Use / commands:** Type `/` to access common commands like `/new`, `/explain`, or create your own custom prompts

3. **Reference tools:** Type `#` followed by a tool name to extend chat capabilities

4. **Be specific and iterative:** 
   - Provide clear requirements
   - Ask follow-up questions to refine results
   - Keep prompts simple and focused

5. **Choose the right agent:**
   - **Ask**: Questions and exploration
   - **Edit**: Code modifications across files
   - **Agent**: Complex, multi-step autonomous tasks

---

## Explore Different Language Models

VS Code offers different language models optimized for different tasks:
- Some models are designed for fast coding tasks
- Others excel at complex reasoning and planning

**To change the language model:**
- Use the model picker in the chat input field (`Ctrl+Alt+.`)
- Select the model that best fits your needs

---

## Customize Your Chat Experience

Customize your chat experience to generate responses that match your coding style, tools, and developer workflow:

### Custom Instructions
Define common guidelines for tasks like generating code, performing code reviews, or generating commit messages. Custom instructions describe the conditions in which the AI should operate (how a task should be done).

### Reusable Prompt Files
Define reusable prompts for common tasks. Prompt files are standalone prompts that you can run directly in chat by typing `/prompt-name`.

### Custom Agents
Define how chat operates, which tools it can use, and how it interacts with the codebase. Each chat prompt runs within the boundaries of the agent.

### MCP Servers
Extend chat with custom capabilities by integrating external tools and services through the Model Context Protocol.

**Tips:**
- Define language-specific instructions to get more accurate generated code
- Store your instructions in your workspace to easily share them with your team
- Define reusable prompt files for common tasks to save time

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Copilot not responding | Restart VS Code; verify Copilot extension is installed and enabled |
| Chat view not opening | Verify GitHub Copilot Chat extension is installed and enabled |
| Authentication fails | Verify Copilot subscription at https://github.com/settings/copilot |
| "Not authenticated" error | Sign in to GitHub through VS Code (File → Preferences → Settings → GitHub) |
| Directory not trusted | Approve when prompted, or mark folder as trusted in workspace settings |
| Slow responses | Use `/compact` to reduce context size, or switch to a faster language model |
| Wrong file being modified | Review carefully before accepting; use full path with `#file:` |
| Inline suggestions not appearing | Check that GitHub Copilot extension is enabled and you're signed in |
| PowerShell version issues | Update to PowerShell 7+: `winget install Microsoft.PowerShell` |

### Getting Help

- **Official Documentation:** https://docs.github.com/en/copilot
- **VS Code Copilot Guide:** https://code.visualstudio.com/docs/copilot
- **Check Extension Status:** Extensions view (Ctrl+Shift+X) → "GitHub Copilot"
- **View Output Logs:** Output panel (Ctrl+Shift+U) → "GitHub Copilot"

---

## Additional Resources

### Microsoft Learn
- [Get Started with GitHub Copilot in VS Code](https://learn.microsoft.com/training/modules/get-started-github-copilot-visual-studio-code/)
- [Use GitHub Copilot Chat in VS Code](https://learn.microsoft.com/training/modules/use-github-copilot-chat-vs-code/)
- [Effective Prompt Engineering](https://code.visualstudio.com/docs/copilot/prompt-crafting)

### Official Documentation
- [GitHub Copilot Documentation](https://docs.github.com/en/copilot)
- [VS Code Copilot Guide](https://code.visualstudio.com/docs/copilot/overview)
- [Copilot Free Plan](https://github.com/features/copilot)

---

## Frequently Asked Questions

### Q: When should I use Ask Mode vs Edit Mode vs Agent Mode?

**Short Answer:** Ask for questions, Edit for changes, Agent for complex multi-step tasks.

**Detailed Explanation:**
- **Ask Mode:** Use when you need to understand code, explore architecture, or get explanations. The AI reads but doesn't modify files.
- **Edit Mode:** Use when you know exactly what file/function to change. Select code, press Ctrl+I, describe the change.
- **Agent Mode:** Use when the task requires research, planning, and changes across multiple files. The AI can autonomously explore, plan, and execute.

---

### Q: What's the difference between @workspace and #codebase?

**Short Answer:** `@workspace` is a chat participant; `#codebase` is a tool for search.

**Detailed Explanation:**
- **@workspace:** Invokes a specialized participant that understands your project structure. Good for architectural questions.
- **#codebase:** Performs semantic search across your code. Good for finding specific patterns or implementations.

Use `@workspace` for "how does X work?" questions. Use `#codebase` for "find all instances of X" queries.

---

### Q: How do I get better inline suggestions?

**Short Answer:** Provide more context through comments and meaningful names.

**Detailed Explanation:**
1. Write a descriptive comment before the code you want generated
2. Use meaningful function and variable names
3. Have relevant files open in the editor
4. Start typing the function signature to guide Copilot
5. If suggestions are wrong, add more specific comments

Example:
```cpp
// Calculate motor torque from current using Kt constant
// Input: phase_current_amps (float)
// Output: torque in Nm (float)
// Constraint: Must handle division by zero
float calculate_torque(
```

---

### Q: Can Copilot work with my company's private code?

**Short Answer:** Yes, Copilot can work with any code in your VS Code workspace.

**Detailed Explanation:**
- Copilot analyzes code in your current workspace to provide context-aware suggestions
- For business/enterprise plans, code is not used to train models
- You can configure what data is sent to GitHub via VS Code settings
- Check your organization's Copilot policy for specific guidelines

---

### Q: How do I control which AI model Copilot uses?

**Short Answer:** Use `Ctrl+Alt+.` to open the model picker.

**Detailed Explanation:**
Different models have different strengths:
- **Claude Sonnet 4:** Great for complex reasoning and planning
- **GPT-4o:** Strong general-purpose model
- **Faster models:** Lower latency for quick suggestions

You can change models per-conversation or set a default in VS Code settings.

---

### Q: What if Copilot generates incorrect or insecure code?

**Short Answer:** Always review generated code; use `/fix` or ask for improvements.

**Detailed Explanation:**
1. **Review before accepting:** Never blindly accept suggestions
2. **Request improvements:** "Make this thread-safe" or "Add error handling"
3. **Specify constraints:** Include security requirements in your prompts
4. **Use validation:** Ask Copilot to review its own output for issues
5. **Test thoroughly:** Generated code should go through normal testing

Example prompt for secure code:
```
Generate this function with:
- Input validation for all parameters
- Bounds checking on buffer access
- No dynamic allocation
- Error codes for failure cases
```

---

## Summary: Key Takeaways

### 1. Three Modes for Three Purposes
- **Ask Mode:** Questions and exploration (Chat view)
- **Edit Mode:** Targeted code changes (Ctrl+I)
- **Agent Mode:** Complex autonomous tasks

### 2. Chat Participants Extend Capabilities
- `@workspace` for project-wide context
- `@terminal` for shell and build help
- `@vscode` for IDE configuration
- `@github` for repository integration

### 3. Slash Commands Accelerate Common Tasks
- `/doc` - Generate documentation
- `/explain` - Understand code
- `/fix` - Resolve errors
- `/tests` - Create unit tests

### 4. Context is King
- Use `#file:` to reference specific files
- Provide constraints and requirements
- Iterate with follow-up questions

### 5. Practice Makes Perfect
- Start with Ask Mode to explore
- Progress to Edit Mode for changes
- Graduate to Agent Mode for complex tasks

---

## Next Steps

After completing this session:

1. **Practice with your own projects** - Try using different modes on your actual codebase
2. **Experiment with custom instructions** - Define your coding standards and preferences
3. **Create reusable prompts** - Build a library of common tasks for your team
4. **Explore advanced features** - Try agents, custom tools, and MCP servers
5. **Share knowledge** - Help your team adopt best practices

---

*Lesson 2: Basic Feature Overview - GitHub Copilot in VS Code*  
*Last Updated: January 2026*

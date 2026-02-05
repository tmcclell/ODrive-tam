# GitHub Copilot Hackathon for Developers

## 🎯 Workshop Overview

| Detail | Description |
|--------|-------------|
| **Audience** | Combination of 25 C#/C++ Embedded Developers (Intermediate to Advanced) |
| **Languages** | C++, C# |
| **Editor** | VS Code with GitHub Copilot |
| **Schedule** | 9:00 AM - 3:00 PM (Both Days) |
| **Reference Repos** | [ODrive](https://github.com/odriverobotics/ODrive), [Marlin](https://github.com/MarlinFirmware/Marlin) |

---

A hands-on hackathon teaching embedded C++ and C# developers how to leverage GitHub Copilot for agentic development workflows. Migrate existing applications into agentic patterns for developer acceleration.

---

### Workshop Goals

- Apply agentic development patterns to real-world codebases
- Modernize legacy embedded code using AI-assisted workflows

## 🎓 Key Learning Outcomes

### Copilot Features

- Inline code completion and Chat modes (@workspace, @vscode)
- TODO: Model picking - premium requests.
- Context steering with copilot-instructions.md and prompt files
- Decomposition and iterative refinement for complex tasks
- Custom agents and skills for domain-specific workflows
- Debugging with @terminal and /fix commands
- CLI usage for automation and scripting
- Multi-agent patterns for parallel workstreams

## 🚀 Quick Start

### Prerequisites

1. **VS Code** with the following extensions:
   - GitHub Copilot
   - GitHub Copilot Chat
   - C/C++ Extension Pack or C# Dev Kit

2. **GitHub Copilot license** - Active subscription

3. **Terminal Access for installing CLI components**

4. **Source Control** -  access to hack repo.

---

## 🗂️ Repository Structure

```
ODriveHack/
├── lessons/
│   ├── 01-welcome-agenda/         # Day 1 agenda and objectives
│   ├── 02-basic features/         # Copilot modes and commands
│   ├── 03-planning/               # Spec driven development
│   ├── 04-agentic-patterns/       # Context engineering
│   ├── 05-best-practices/         # Patterns with Copilot, Unit Tests
│   ├── 06-debugging/              # AI-assisted debugging
│   ├── 07-copilot-cli/            # Command-line interface and GHCP SDK.
│   ├── 08-parallel-agents/        # Multi-agent workflows│   
├── src-ODrive/                    # ODrive firmware for exercises
└── README.md                      # This file
```
---

## 📚 Lesson Overview

### Day 1: GitHub Copilot

| # | Lesson | Duration | Topics |
|---|--------|----------|--------|
| 1 | [Welcome & Setup]
| 2 | [Basic Features](lessons/02-basic%20features/readme.md) | 45 min | Chat modes, @workspace, /fix, /explain, inline chat |
| 3 | [Planning & Steering](lessons/03-planning/readme.md) | 60 min | spec driven development, plan mode |
| 4 | [Agentic Patterns](lessons/04-agentic-patterns/readme.md) | 45 min | Context engineering, copilot-instructions.md, prompt files, custom agents, skills |
| 5 | [Best Practices](lessons/05-best-practices/readme.md) | 50 min | RAII, templates, const correctness, embedded patterns, unit tests| 
| 6 | [Debugging](lessons/06-debugging/readme.md) | 45 min | @terminal, /fix, common bugs |
| 7 | [Copilot CLI](lessons/07-copilot-cli/readme.md) | 45 min | Using the cli and a first look at the GHCP SDK |
| 8 | [Parallel Agents](lessons/08-parallel-agents/readme.md) | 20 min | Multi-agent workflows, cloud agents |

### Recording
- ✅ Sessions can be recorded for playback (per customer request)
- Recommend recording Day 1 instructional content only
- Day 2 hack sessions typically not recorded (IP concerns)

## 📚 Additional Resources

### Microsoft Learn

- [Get Started with GitHub Copilot in VS Code](https://learn.microsoft.com/training/modules/get-started-github-copilot-visual-studio-code/)
- [GitHub Copilot Chat in VS Code](https://learn.microsoft.com/training/modules/use-github-copilot-chat-vs-code/)
- [C++ Development in VS Code](https://learn.microsoft.com/cpp/build/vscpp-step-0-installation)
- [GitHub Copilot CLI](https://learn.microsoft.com/training/modules/use-github-copilot-cli/)
- [C# Dev Kit](https://marketplace.visualstudio.com/items?itemName=ms-dotnettools.csdevkit/)

### Reference Repositories

- [ODrive Firmware](https://github.com/odriverobotics/ODrive) - Motor control, embedded C++
- [Marlin Firmware](https://github.com/MarlinFirmware/Marlin) - 3D printer firmware

---

## 👥 Instructors

| Initials | Name |
|----------|------|
| TM | Tammy McClellan |
| TI | Tom Iverson |
| GB | Gaurav Bhardwaj |

---

## 📝 License

Workshop materials are provided for educational purposes. 

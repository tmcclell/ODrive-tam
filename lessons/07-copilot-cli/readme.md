# Lesson 7: GitHub Copilot CLI

**Session Duration:** 45 minutes  
**Audience:** Embedded/C++ Developers (Intermediate to Advanced)  
**Environment:** Windows + PowerShell 7+ (also Linux, macOS, WSL)  
**Focus:** Terminal-based AI agent for embedded development workflows

---

## Overview

GitHub Copilot CLI is an AI agent that operates directly from your terminal. It can answer questions, write and debug code, interact with GitHub.com, and execute shell commands—all through natural language prompts.

**What You'll Learn:**
- Install and authenticate Copilot CLI
- Use interactive and programmatic modes
- Leverage file references, shell integration, and GitHub.com tasks
- Customize with MCP servers, custom agents, and instructions

**Key Concepts:**

| Concept | Description |
|---------|-------------|
| **AI Agent** | Powerful agent that works iteratively on your behalf |
| **Interactive Mode** | Launch `copilot` for multi-turn conversations |
| **Programmatic Mode** | Single prompt with `-p` flag for automation |
| **File References** | Use `@path/file` to focus on specific files |
| **GitHub.com Integration** | Create PRs, manage issues, work across repos |
| **MCP Servers** | Extend capabilities with Model Context Protocol |
| **Custom Agents** | Specialized versions for different tasks |
| **Directory Trust** | Security feature requiring explicit folder approval |

---

## Agenda (45 min)

| Sub-Topic | Focus | Time |
|-----------|-------|------|
| Installation & Auth | Install CLI, authenticate with GitHub | 8 min |
| Live Demo | Modes, GitHub integration, customization | 12 min |
| Hands-On Exercises | Participants explore CLI capabilities | 20 min |
| Wrap-Up | Q&A, troubleshooting | 5 min |

---

## Table of Contents

- [Overview](#overview)
- [Agenda (45 min)](#agenda-45-min)
- [Prerequisites](#prerequisites)
- [1. Installation](#1-installation)
- [2. Authentication](#2-authentication)
- [3. Modes of Use](#3-modes-of-use)
- [4. Model Selection](#4-model-selection)
- [5. Key Features Demo](#5-key-features-demo)
- [6. Customization](#6-customization)
- [7. Hands-On Exercises](#7-hands-on-exercises)
- [Quick Reference](#quick-reference)
- [Troubleshooting](#troubleshooting)
- [GitHub Copilot SDK](#github-copilot-sdk)
- [Additional Resources](#additional-resources)

---

## Prerequisites

Before starting this session, ensure you have:

- ✅ **Windows 10/11** with PowerShell 7+ (or Linux/macOS/WSL)
- ✅ **GitHub account** with Copilot Pro, Pro+, Business, or Enterprise
- ✅ **Git** installed and configured
- ✅ **Workshop repository** cloned locally

### Verify Your Environment

```powershell
# Check PowerShell version (need 7+)
$PSVersionTable.PSVersion

# Check Git is available
git --version
```

---

## 1. Installation

### Windows (winget)

```powershell
winget install GitHub.Copilot
```

> Restart PowerShell after installation

### macOS (Homebrew)

```bash
brew install github/copilot-cli/copilot
```

### Verify Installation

```powershell
copilot --version
```

### Fallback: Direct Download

If package manager fails, download from [GitHub Releases](https://github.com/github/copilot-cli/releases)

---

## 2. Authentication

### Launch & Login

```powershell
cd C:\path\to\workshop-repo
copilot
```

```
/login
```

### Authentication Flow

1. Copy the device code displayed
2. Open https://github.com/login/device
3. Enter code and authorize
4. Return to terminal — "Successfully authenticated!"

### Trust Directory

When prompted, choose **"Yes, always trust this folder"** for your projects.

> **Warning:** Only launch Copilot CLI from directories you trust. Do not launch from your home directory or directories with sensitive data.

---

## 3. Modes of Use

### Interactive Mode (Default)

Start a multi-turn conversation session:

```powershell
copilot
```

### Programmatic Mode

Run a single prompt and exit—ideal for scripts and automation:

```powershell
copilot -p "Show me this week's commits and summarize them" --allow-tool 'shell(git)'
```

### Tool Approval Options

| Option | Description |
|--------|-------------|
| `--allow-tool 'shell(git)'` | Allow specific command without approval |
| `--deny-tool 'shell(rm)'` | Block specific command |
| `--allow-all-tools` | Allow all tools (use with caution) |

**Example: Allow git but block destructive commands:**
```powershell
copilot --allow-all-tools --deny-tool 'shell(rm)' --deny-tool 'shell(git push)'
```

---

## 4. Model Selection

The default model is **Claude Sonnet 4.5**. Change models with:

```
/model
```

Select from available models—each shows a multiplier for premium request usage (e.g., `Claude Sonnet 4.5 (1x)`).

---

## 5. Key Features Demo

### Local Tasks

**Code Analysis:**
```
Explain @Firmware/MotorControl/motor.cpp
What functions are in @Firmware/Drivers/spi_driver.c?
```

**Code Generation:**
```
Add a factorial function to @src\utils.c
Suggest improvements to @Firmware/MotorControl/encoder.cpp
```

**Git Operations:**
```
Commit the changes to this repo
Show me the last 5 changes made to CHANGELOG.md
Revert the last commit, leaving changes unstaged
```

### GitHub.com Integration

**Pull Requests:**
```
List my open PRs
Create a PR that updates the README at https://github.com/owner/repo
Check the changes in PR https://github.com/owner/repo/pull/123
Merge all open PRs that I created in owner/repo
```

**Issues:**
```
List all open issues assigned to me in owner/repo
Raise an improvement issue in owner/repo for the unclosed file handle
I've been assigned this issue: https://github.com/owner/repo/issues/123. Start working on it.
```

**GitHub Actions:**
```
List any Actions workflows in this repo that add comments to PRs
Create a workflow that runs eslint on PRs and fails if errors are found
```

### Shell Integration

```
!git status
!git log --oneline -5
How many lines of code are in src\?
```

---

## 6. Customization

| Feature | Description |
|---------|-------------|
| **Custom Instructions** | Add project context and build/test/validate guidance |
| **MCP Servers** | Connect to external data sources and tools |
| **Custom Agents** | Create specialized versions (e.g., frontend expert) |
| **Hooks** | Execute shell commands at key execution points |
| **Skills** | Enhance with instructions, scripts, and resources |

### Slash Commands

| Command | Purpose |
|---------|---------|
| `/help` | List available commands |
| `/model` | Change AI model |
| `/compact` | Compress conversation context |
| `/mcp` | List configured MCP servers |
| `/feedback` | Submit feedback, bugs, or feature requests |

---

## 7. Hands-On Exercises

### Exercise 1: Get Oriented (3 min)

```powershell
cd C:\path\to\workshop-repo
copilot
```

```
/help
/model
What is this project and what does it do?
```

### Exercise 2: Local Tasks (5 min)

```
Explain @Firmware/MotorControl/motor.cpp
What functions are defined in @Firmware/Drivers/spi_driver.c?
Suggest improvements to @Firmware/Board/v3/board.cpp
```

### Exercise 3: GitHub.com Tasks (5 min)

```
List my open PRs
List all open issues assigned to me in <your-repo>
Check the changes in PR https://github.com/<owner>/<repo>/pull/<number>
```

### Exercise 4: Code Generation (4 min)

```
Add a function to @src\utils.c that calculates factorial
Create @src\logger.c with debug, info, error logging functions
```
> Review and approve/reject proposed changes

### Exercise 4: Code Generation (4 min)

```
Add a function to @src\utils.c that calculates factorial
Create @src\logger.c with debug, info, error logging functions
```
> Review and approve/reject proposed changes

### Exercise 5: Git Integration (3 min)

```
!git log --oneline -5
Show me what files changed in the last commit
Commit the staged changes with a descriptive message
```

---

## Quick Reference

### Essential Commands

| You Type | What Happens |
|----------|--------------|
| `copilot` | Start interactive session |
| `copilot -p "prompt"` | Programmatic mode (single prompt) |
| `/help` | List commands |
| `/model` | Change AI model (default: Claude Sonnet 4.5) |
| `/compact` | Reduce context size |
| `/mcp` | List MCP servers |
| `@file.c` | Reference a file |
| `!git status` | Run shell command |

### Common Prompts

| Prompt | Use Case |
|--------|----------|
| `Explain @filename` | Understand code |
| `Suggest improvements to @filename` | Code review |
| `Add a function to @filename that...` | Code generation |
| `List my open PRs` | GitHub.com integration |
| `Create a PR that...` | Automated PR creation |
| `Commit the changes` | Git operations |

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `copilot` not recognized | Restart PowerShell |
| PowerShell version < 7 | `winget install Microsoft.PowerShell` |
| winget not found | Install "App Installer" from Microsoft Store |
| Authentication fails | Verify subscription at github.com/settings/copilot |
| Slow responses | Use `/compact` to reduce context |
| Tool not approved | Use `--allow-tool` or approve when prompted |

---

## GitHub Copilot SDK

While **Copilot CLI** is an interactive terminal tool for developers, the **[GitHub Copilot SDK](https://github.com/github/copilot-sdk?utm_source=blog-cli-sdk-repo-cta&utm_medium=blog&utm_campaign=cli-sdk-jan-2026)** is a programmatic library that lets you embed Copilot's agentic capabilities directly into your own applications and services.

### CLI vs SDK: When to Use Each

| Copilot CLI | Copilot SDK |
|-------------|-------------|
| Interactive terminal sessions | Programmatic integration in your apps |
| Developer productivity tool | Build AI-powered tools and automation |
| Run `copilot` command directly | Import as a library in your code |
| Human-in-the-loop workflows | Automated pipelines and services |

### SDK Installation

| Language | Command |
|----------|----------|
| **Node.js / TypeScript** | `npm install @github/copilot-sdk` |
| **Python** | `pip install github-copilot-sdk` |
| **Go** | `go get github.com/github/copilot-sdk/go` |
| **.NET** | `dotnet add package GitHub.Copilot.SDK` |

> **Prerequisite:** The Copilot CLI must be installed. The SDK communicates with the CLI running in server mode via JSON-RPC.

### Quick Demo: Python

```python
from github_copilot_sdk import CopilotClient

# SDK automatically starts CLI in server mode
client = CopilotClient()

# Analyze firmware code programmatically
response = client.chat(
    "Review @Firmware/MotorControl/motor.cpp for potential issues"
)
print(response)
```

### Use Case: Automated Firmware Documentation

Build a script that generates documentation for your embedded codebase:

```python
from github_copilot_sdk import CopilotClient
import os

client = CopilotClient()

# Generate docs for each driver file
for file in os.listdir("Firmware/Drivers"):
    if file.endswith(".c"):
        doc = client.chat(f"Generate API documentation for @Firmware/Drivers/{file}")
        with open(f"docs/{file}.md", "w") as f:
            f.write(doc)
```

This enables CI/CD integration, batch processing, and custom tooling that would be impractical with interactive CLI sessions.

### SDK Resources

- [GitHub Copilot SDK Repository](https://github.com/github/copilot-sdk?utm_source=blog-cli-sdk-repo-cta&utm_medium=blog&utm_campaign=cli-sdk-jan-2026)
- [Getting Started Guide](https://github.com/github/copilot-sdk/blob/main/docs/getting-started.md)
- [Cookbook & Recipes](https://github.com/github/copilot-sdk/blob/main/cookbook/README.md)

> **Note:** The SDK is currently in Technical Preview.

---

## Additional Resources

### Microsoft Learn Documentation

| Resource | Description |
|----------|-------------|
| [GitHub Copilot CLI](https://learn.microsoft.com/training/modules/use-github-copilot-cli/) | Official CLI training module |
| [Get started with GitHub Copilot](https://learn.microsoft.com/training/modules/get-started-github-copilot-visual-studio-code/) | Copilot fundamentals |
| [GitHub Copilot Chat in VS Code](https://learn.microsoft.com/training/modules/use-github-copilot-chat-vs-code/) | Chat interface patterns |

### Official Documentation

- [About GitHub Copilot CLI](https://docs.github.com/en/copilot/concepts/agents/about-copilot-cli) - Comprehensive feature overview
- [Installing GitHub Copilot CLI](https://docs.github.com/en/copilot/how-tos/set-up/install-copilot-cli) - Installation guide
- [Using GitHub Copilot CLI](https://docs.github.com/en/copilot/how-tos/use-copilot-agents/use-copilot-cli) - Usage patterns
- [Copilot Subscription](https://github.com/settings/copilot) - Manage your subscription
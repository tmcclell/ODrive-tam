# GitHub Copilot SDK

**Focus:** Programmatic AI integration for custom applications and automation  
**Status:** Technical Preview

---

## Overview

While **Copilot CLI** is an interactive terminal tool for developers, the **[GitHub Copilot SDK](https://github.com/github/copilot-sdk)** is a programmatic library that lets you embed Copilot's agentic capabilities directly into your own applications and services.

**What You'll Learn:**
- Install and configure the SDK for your language
- Understand the CLI-SDK architecture
- Build automation scripts with AI capabilities
- Implement common use cases for embedded development

**Key Concepts:**

| Concept | Description |
|---------|-------------|
| **Programmatic Access** | Call Copilot from code instead of interactive terminal |
| **CLI Server Mode** | SDK communicates with CLI via JSON-RPC |
| **Multi-Language Support** | Python, Node.js, Go, and .NET SDKs available |
| **Batch Processing** | Process multiple files or tasks in automated pipelines |
| **CI/CD Integration** | Embed AI assistance in build and deployment workflows |

---

## CLI vs SDK: When to Use Each

| Copilot CLI | Copilot SDK |
|-------------|-------------|
| Interactive terminal sessions | Programmatic integration in your apps |
| Developer productivity tool | Build AI-powered tools and automation |
| Run `copilot` command directly | Import as a library in your code |
| Human-in-the-loop workflows | Automated pipelines and services |
| Ad-hoc queries and exploration | Repeatable, scriptable operations |

---

## Prerequisites

Before using the SDK, ensure you have:

- ✅ **GitHub Copilot CLI** installed and authenticated (see [readme.cli.md](readme.cli.md))
- ✅ **GitHub account** with Copilot Pro, Pro+, Business, or Enterprise
- ✅ **Language runtime** for your chosen SDK (Python 3.8+, Node.js 18+, Go 1.21+, or .NET 8+)

---

## Installation

| Language | Command |
|----------|---------|
| **Python** | `pip install github-copilot-sdk` |
| **Node.js / TypeScript** | `npm install @github/copilot-sdk` |
| **Go** | `go get github.com/github/copilot-sdk/go` |
| **.NET** | `dotnet add package GitHub.Copilot.SDK` |

> **Note:** The SDK automatically starts the CLI in server mode. Ensure the `copilot` command is available in your PATH.

---

## Quick Start Examples

### Python

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

### Node.js / TypeScript

```typescript
import { CopilotClient } from '@github/copilot-sdk';

const client = new CopilotClient();

// Explain a file
const response = await client.chat(
    "Explain the control loop in @Firmware/MotorControl/foc.cpp"
);
console.log(response);
```

### .NET

```csharp
using GitHub.Copilot.SDK;

var client = new CopilotClient();

// Generate documentation
var response = await client.ChatAsync(
    "Generate API documentation for @Firmware/Drivers/spi_driver.c"
);
Console.WriteLine(response);
```

## Use Cases

### 1. Automated Documentation Generation

Generate documentation for your embedded codebase:

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

### 2. Code Review Automation

Integrate AI code review into your CI pipeline:

```python
from github_copilot_sdk import CopilotClient
import subprocess

client = CopilotClient()

# Get changed files from git
result = subprocess.run(
    ["git", "diff", "--name-only", "HEAD~1"],
    capture_output=True, text=True
)
changed_files = result.stdout.strip().split("\n")

# Review each changed file
for file in changed_files:
    if file.endswith((".c", ".cpp", ".h")):
        review = client.chat(
            f"Review @{file} for embedded best practices: "
            "memory safety, MISRA compliance, and potential bugs"
        )
        print(f"=== Review: {file} ===\n{review}\n")
```

### 3. Test Generation Pipeline

Generate unit tests for new or modified code:

```python
from github_copilot_sdk import CopilotClient

client = CopilotClient()

# Generate tests for a module
tests = client.chat(
    "Generate doctest unit tests for all public functions in "
    "@Firmware/MotorControl/utils.cpp. Follow the existing test patterns."
)

with open("Firmware/doctest/test_utils.cpp", "w") as f:
    f.write(tests)
```

### 4. Migration Assistant

Automate code migration tasks:

```python
from github_copilot_sdk import CopilotClient
import glob

client = CopilotClient()

# Find all files using deprecated API
for file in glob.glob("Firmware/**/*.cpp", recursive=True):
    migration = client.chat(
        f"Analyze @{file} and identify any uses of deprecated HAL v1 APIs. "
        "Suggest migrations to HAL v2 equivalents."
    )
    if "deprecated" in migration.lower():
        print(f"Migration needed: {file}\n{migration}\n")
```

### 5. Commit Message Generation

Generate descriptive commit messages:

```python
from github_copilot_sdk import CopilotClient
import subprocess

client = CopilotClient()

# Get staged diff
diff = subprocess.run(
    ["git", "diff", "--cached"],
    capture_output=True, text=True
).stdout

# Generate commit message
message = client.chat(
    f"Generate a conventional commit message for these changes:\n\n{diff}"
)
print(message)
```

---

## Configuration

### Environment Variables

| Variable | Description |
|----------|-------------|
| `COPILOT_CLI_PATH` | Custom path to copilot executable |
| `COPILOT_TIMEOUT` | Request timeout in seconds (default: 120) |
| `COPILOT_MODEL` | Default model to use |

### Client Options

```python
from github_copilot_sdk import CopilotClient

client = CopilotClient(
    cli_path="/custom/path/to/copilot",  # Optional: custom CLI location
    timeout=180,                          # Optional: request timeout
    model="claude-sonnet-4.5"            # Optional: specify model
)
```

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `CLI not found` | Ensure `copilot` is in PATH or set `COPILOT_CLI_PATH` |
| `Authentication failed` | Run `copilot` interactively and use `/login` |
| `Timeout errors` | Increase timeout for large file operations |
| `Rate limiting` | Check your Copilot subscription tier limits |
| `Empty responses` | Verify file paths exist and are readable |

---

## Resources

### Official Documentation

- [GitHub Copilot SDK Repository](https://github.com/github/copilot-sdk)
- [Getting Started Guide](https://github.com/github/copilot-sdk/blob/main/docs/getting-started.md)
- [Cookbook & Recipes](https://github.com/github/copilot-sdk/blob/main/cookbook/README.md)
- [API Reference](https://github.com/github/copilot-sdk/blob/main/docs/api-reference.md)

### Related Resources

- [GitHub Copilot CLI](readme.cli.md) - Interactive terminal tool
- [Copilot Subscription](https://github.com/settings/copilot) - Manage your subscription

> **Note:** The SDK is currently in Technical Preview. APIs may change before general availability.


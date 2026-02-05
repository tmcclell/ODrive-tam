# Lesson 9: Foundry Local Demo

**Session Duration:** 15 minutes  
**Audience:** Embedded/C++ Developers (Intermediate to Advanced)  
**Environment:** Windows + VS Code (demo); optional WSL/macOS setup  
**Focus:** Show Foundry Local CLI capabilities and local AI inference

---

## Overview

This lesson demonstrates Microsoft's Foundry Local—an on-device AI runtime that enables local language model execution. You'll learn how to install the CLI tools, run models locally, explore execution providers (CPU/GPU), and understand integration patterns for embedded development scenarios.

**What You'll Learn:**
- Install and configure Foundry Local CLI
- Download and run local language models
- Explore SDK integration patterns for local AI
- Connect Foundry Local with VS Code workflows

**Key Concepts:**

| Concept | Description |
|---------|-------------|
| **Foundry Local** | Microsoft's on-device AI runtime for local inference |
| **On-device inference** | Run AI models locally for privacy, reduced latency, and no cloud costs |
| **OpenAI-compatible API** | Use familiar SDKs (Python, JavaScript, C#) to integrate |
| **Hardware-aware execution** | Automatically selects best model variant (CPU/GPU/NPU) for your hardware |
| **Model Cache** | Local storage for downloaded models |
| **No Azure subscription required** | Fully local, no sign-up needed |

---

## Agenda (15 min)

| Sub-Topic | Focus | Time |
|-----------|-------|------|
| What is Foundry Local? | Local runtime, privacy, hardware acceleration | 3 min |
| Live Demo | CLI installation, model operations, interactive chat | 10 min |
| Self-Setup Guide | Install steps, WSL/macOS, SDK integration | 2 min |

---

## Table of Contents

- [Overview](#overview)
- [Agenda (15 min)](#agenda-15-min)
- [Prerequisites](#prerequisites)
- [Why Foundry Local Matters](#why-foundry-local-matters)
- [1. Installation (8 min)](#1-installation-8-min)
- [2. CLI Commands (7 min)](#2-cli-commands-7-min)
- [Additional Resources](#additional-resources)

---

## Prerequisites

Before starting this session, ensure you have:

- ✅ **Windows 10/11** or WSL with Ubuntu
- ✅ **Visual Studio Code** with GitHub Copilot extensions
- ✅ **PowerShell 7+** or Bash terminal
- ✅ **Sufficient disk space** - At least 10GB free for model downloads
- ✅ **Internet connection** - For initial model downloads

### Optional for GPU Acceleration

- ✅ **NVIDIA GPU** with CUDA support (for CUDA provider)
- ✅ **DirectX 12 compatible GPU** (for DirectML provider)
- ✅ **Latest GPU drivers** installed

### Verify Your Environment

```powershell
# Check PowerShell version
$PSVersionTable.PSVersion

# Check available disk space
Get-PSDrive C | Select-Object Used, Free
```
---

## Why Foundry Local Matters

Foundry Local enables AI-powered development workflows without cloud dependencies. This is particularly relevant for embedded development where:

### Key Benefits

| Benefit | Description |
|---------|-------------|
| **Offline Operation** | No internet required after model download |
| **Data Privacy** | Sensitive code never leaves your machine |
| **Low Latency** | No network round-trip for inference |
| **Cost Control** | No per-query API charges |
| **Air-Gapped Dev** | Works in secure/isolated environments |

## Learning Path

| Topic | What You'll Learn | Estimated Time |
|-------|-------------------|----------------|
| Installation | Install Foundry CLI | 8 min |
| CLI Commands | Model management and inference | 7 min |
| Execution Providers | CPU vs GPU options | 5 min |
| SDK Integration | Programmatic access | 10 min |
| WSL Integration | Linux environment setup | 5 min |

---

## 1. Installation (8 min)

### Windows Installation

**Option 1: Using winget (recommended)**
```powershell
winget install Microsoft.FoundryLocal
```

**Option 2: Manual installation**
1. Download the latest `.msix` package from [GitHub releases](https://github.com/microsoft/Foundry-Local/releases)
2. Download the dependency: [Microsoft.VCLibs.x64.14.00.Desktop.appx](https://aka.ms/Microsoft.VCLibs.x64.14.00.Desktop.appx)
3. Install both packages:
```powershell
Add-AppxPackage .\FoundryLocal.msix -DependencyPath .\VcLibs.appx
```

**Verify installation:**
```powershell
foundry --help
foundry service status
```

### macOS Installation

**Using Homebrew:**
```bash
brew install microsoft/foundrylocal/foundrylocal
```

**Manual installation:**
1. Download the latest release from [GitHub releases](https://github.com/microsoft/Foundry-Local/releases)
2. Extract the downloaded file
3. Run the installer:
```bash
./install-foundry.command
```

**Verify installation:**
```bash
foundry --help
foundry service status
```

---

## 2. CLI Commands (7 min)

### Model Discovery

```powershell
# List available models
foundry model list

# Search for specific model types
foundry model list --filter "code"

# Show model details
foundry model info <model-name>
```

### Model Download

```powershell
# Download a model (example: small code model)
foundry model download phi-3.5-mini

# List cached models
foundry cache list
```

### Running Inference

**Interactive Chat:**
```powershell
# Download (if needed) and run a model interactively
foundry model run phi-3.5-mini
```

- **First run**: Downloads model and starts interactive chat
- **Subsequent runs**: Instant launch from cache
- Show the interactive prompt that appears

### Model Management

```powershell
# Remove a model from cache
foundry cache remove phi-3.5-mini

# List cached models
foundry cache list

# Show cache directory path
foundry cache location
```

### Complete CLI Command Reference

#### Model Commands

| Command | Description |
|---------|-------------|
| `foundry model list` | List all available models (downloads execution providers on first run) |
| `foundry model list --filter <key>=<value>` | Filter models by device, task, alias, or provider |
| `foundry model run <model>` | Download (if needed) and run a model interactively |
| `foundry model info <model>` | Display detailed model information |
| `foundry model info <model> --license` | Show model license information |
| `foundry model download <model>` | Download a model without running it |
| `foundry model load <model>` | Load a model into the service |
| `foundry model unload <model>` | Unload a model from the service |

**Filter examples:**
```powershell
foundry model list --filter device=GPU
foundry model list --filter device=!GPU
foundry model list --filter task=chat-completion
foundry model list --filter provider=CUDAExecutionProvider
foundry model list --filter alias=phi*
```

> **Note**: Use model **alias** (e.g., `phi-3.5-mini`) to auto-select the best variant for your hardware, or use the full **model ID** (e.g., `qwen2.5-0.5b-instruct-generic-cpu`) to run a specific variant.

#### Service Commands

| Command | Description |
|---------|-------------|
| `foundry service status` | Display service status and endpoint URL |
| `foundry service start` | Start the Foundry Local service |
| `foundry service stop` | Stop the Foundry Local service |
| `foundry service restart` | Restart the Foundry Local service |
| `foundry service ps` | List currently loaded models |
| `foundry service diag` | Display service logs |
| `foundry service set <options>` | Configure service settings |

#### Cache Commands

| Command | Description |
|---------|-------------|
| `foundry cache location` | Show current cache directory path |
| `foundry cache list` | List all cached models |
| `foundry cache cd <path>` | Change cache directory location |
| `foundry cache remove <model>` | Remove a model from cache |

---

## 3. Execution Providers (5 min)

### Available Providers

| Provider | Hardware | Performance | Memory |
|----------|----------|-------------|--------|
| **CPU** | Any x64/ARM64 | Baseline | System RAM |
| **DirectML** | DirectX 12 GPU | 2-5x faster | GPU VRAM |
| **CUDA** | NVIDIA GPU | 3-10x faster | GPU VRAM |

### Provider Recommendations

| Scenario | Recommended Provider |
|----------|---------------------|
| No dedicated GPU | CPU |
| AMD GPU / Intel GPU | DirectML |
| NVIDIA GPU | CUDA |
| Low VRAM (< 4GB) | CPU |
| Maximum performance | CUDA or DirectML |

---
## Additional Resources

### SDK Reference

- [Foundry Local SDK Reference](https://github.com/lifesawesome/ODrive-Custom/blob/master/README.md) - Complete SDK documentation and integration examples

### Microsoft Learn Documentation

| Resource | Description |
|----------|-------------|

- [Foundry Local GitHub Repository](https://github.com/microsoft/Foundry-Local) - Source code, releases, and issue tracking

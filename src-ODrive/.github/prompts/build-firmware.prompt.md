# Build Firmware

Build ODrive firmware for specific board variants and configurations.

## Instructions

Use the **odrive-toolchain** skill to build firmware with proper board configuration.

### Workflow Hierarchy

```
instructions/*.instructions.md (Coding Standards)
        ↓
  ODrive-Toolchain.agent (Orchestrator)
        ↓
  odrive-toolchain skill
        ↓
  build_firmware.py + setup-env.ps1
```

### Input Required
- **Board Variant**: ${{BOARD_CONFIG}} (e.g., "board-v3.6-56V", "board-v3.5-24V", "board-v4.2")
- **Build Type**: ${{BUILD_TYPE}} (optional: "release", "debug", default: "release")

### What This Prompt Does

1. **Sources environment** via `setup-env.ps1` (adds tup + ARM GCC to PATH)
2. **Validates board configuration** exists
3. **Builds firmware** using `build_firmware.py` for specified board variant
4. **Reports build status** (success/failure)
5. **Lists output files** (firmware binary location)
6. **Checks for warnings** and reports issues

### Script Locations

```
.github/skills/odrive-toolchain/setup-env.ps1      # Environment setup
.github/skills/odrive-toolchain/build_firmware.py  # Build script
```

### Commands

```powershell
# Step 1: Source environment (Windows PowerShell - once per session)
. .github\skills\odrive-toolchain\setup-env.ps1

# Step 2: Build using the skill script (recommended)
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V

# Build with clean
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V --clean

# List available configurations
python .github\skills\odrive-toolchain\build_firmware.py --list-configs

# Check tools are available
python .github\skills\odrive-toolchain\build_firmware.py --check-tools
```

### Legacy Commands (Manual Setup Required)

```bash
# Direct tup/make commands (requires manual tup.config setup)
cd Firmware && tup --no-environ-check

# Check available configurations in tup.config.default
grep "CONFIG_BOARD_VERSION" Firmware/tup.config.default
```

### Available Board Variants

| Board | Config String | Voltage | Notes |
|-------|--------------|---------|-------|
| **v3.6** | `board-v3.6-56V` | 56V | Current production |
| **v3.5** | `board-v3.5-24V` `board-v3.5-48V` | 24V/48V | Legacy |
| **v4.2** | `board-v4.2` | 56V | Future |

### Prerequisites

- ARM GCC toolchain installed (`arm-none-eabi-gcc`)
- Tup build system installed
- Python 3.x
- Windows: Run `setup-env.ps1` to add tools to PATH

### Example Usage

Build for v3.6 board:
```
@odrive-toolchain Build firmware for board v3.6
```

Build multiple variants:
```
@odrive-toolchain Build firmware for both v3.5-24V and v3.6-56V boards
```

Clean and rebuild:
```
@odrive-toolchain Clean build directory and rebuild firmware for board v3.6
```

Check build system:
```
@odrive-toolchain List all available board configurations
```

### Output

**On Success:**
- ✅ Build completed successfully
- 📦 Binary location: `Firmware/build/ODriveFirmware.elf`
- 📊 Build statistics (size, warnings)
- 🎯 Ready to flash or test

**On Failure:**
- ❌ Build errors with file/line numbers
- 💡 Suggested fixes
- 🔍 Common causes of build failures

### Build Workflow

```
1. Source environment (setup-env.ps1)
        ↓
2. Run build_firmware.py with board config
        ↓
3. Script updates tup.config
        ↓
4. Tup compiles source with ARM GCC
        ↓
5. Linker creates binary
        ↓
6. Post-build size report
        ↓
7. Output: ODriveFirmware.elf/.bin/.hex
```

### Skills Invoked

| Skill | Purpose |
|-------|---------|
| **odrive-toolchain** | Execute build commands, report results |
| **ODrive-Engineer** | Fix build errors if needed |

### Common Build Issues

| Error | Cause | Fix |
|-------|-------|-----|
| `arm-none-eabi-gcc: not found` | Toolchain not installed | Install ARM GCC toolchain |
| `fatal error: *.h: No such file` | Missing header/include path | Check include directories |
| `undefined reference to...` | Missing implementation | Check linker settings |
| `region 'FLASH' overflowed` | Binary too large | Optimize code size |

### Post-Build Actions

After successful build:
- [ ] Check binary size fits in flash
- [ ] Verify no new warnings introduced
- [ ] **DO NOT** auto-flash to hardware (requires confirmation)

### Unit Tests (Optional - CI Only)

C++ unit tests are located in `Firmware/Tests/` and use the doctest framework.
These tests run automatically in CI (GitHub Actions) but require additional setup locally.

**Local Requirements (Optional):**
- Native `g++` compiler (MinGW-w64 or MSYS2 on Windows)
- Set `CONFIG_DOCTEST=true` in `Firmware/tup.config`
- Rebuild with tup - tests compile and run automatically

**Note:** A successful firmware build validates the core code compiles correctly.
Full test coverage is verified in CI before merging to main branches.

### Related Prompts

| Need | Use |
|------|-----|
| Run tests | `run-tests.prompt.md` |
| Find symbols | `find-symbol.prompt.md` |
| List errors | `list-errors.prompt.md` |
| Deploy firmware | `devops-ci.prompt.md` |
| Debug build errors | `debug-motor-error-v2.prompt.md` |
| Optimize binary size | `optimize-critical.prompt.md` |

### Safety Note

⚠️ **Building firmware is SAFE** - it compiles locally and does not affect hardware.  
⚠️ **Flashing firmware requires confirmation** - see constitution safety rules.

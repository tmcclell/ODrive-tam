# Toolchain Operations

Build, test, search, and error inspection for ODrive firmware.

## Agent: ODrive-Toolchain

### Build Firmware
```powershell
# Setup (once per session)
. .github\skills\odrive-toolchain\setup-env.ps1

# Build
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V

# Options
python .github\skills\odrive-toolchain\build_firmware.py --list-configs
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V --clean
```

### Search Symbols
```powershell
python .github\skills\odrive-toolchain\search_symbol.py Controller
python .github\skills\odrive-toolchain\search_symbol.py "ERROR_" --regex
python .github\skills\odrive-toolchain\search_symbol.py Encoder --type class
```

### List Errors
```powershell
python .github\skills\odrive-toolchain\list_errors.py
python .github\skills\odrive-toolchain\list_errors.py --filter encoder
python .github\skills\odrive-toolchain\list_errors.py --format table
```

### Run Tests
```powershell
python tools/run_tests.py
python tools/run_tests.py --filter encoder
```

---

## Board Variants

| Board | Config |
|-------|--------|
| v3.6 | `board-v3.6-56V` |
| v3.5 | `board-v3.5-24V`, `board-v3.5-48V` |

---

## Safety

✅ Safe: Build, search, errors, software tests  
⚠️ Confirm: Flash firmware, HIL tests

---
name: 'ODrive Toolchain'
description: 'Build, test, search, and error inspection for ODrive firmware'
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
| `find_symbol.ps1` | Quick symbol search | `.\find_symbol.ps1 -Symbol "Controller"` |
| `list_errors.py` | List error codes | `python list_errors.py --filter encoder` |
| `list_errors_from_yaml.py` | Parse interface YAML | `python list_errors_from_yaml.py` |

---

## Quick Usage

### Build Firmware
```powershell
. .github\skills\odrive-toolchain\setup-env.ps1
python .github\skills\odrive-toolchain\build_firmware.py board-v3.6-56V
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

## Safety

✅ **Safe**: All operations are read-only or local builds  
⚠️ **Never auto-execute**: Flash firmware, HIL tests

---

## Related

- **Agent**: `.github/agents/ODrive-Toolchain.agent.md`
- **Prompt**: `.github/prompts/toolchain.prompt.md`

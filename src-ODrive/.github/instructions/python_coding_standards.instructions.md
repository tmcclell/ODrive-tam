---
applyTo: "**/*.py"
description: "Python coding standards for ODrive tools and scripts. Apply when writing or reviewing Python files."
---

# Python Coding Standards

This document defines coding standards for Python files.

---

## General Guidelines

- **Standard**: Follow [PEP 8](https://peps.python.org/pep-0008/) for code style.
- **Formatter**: Use `black` (recommended) or `autopep8` for consistent formatting.
- **Linter**: Use `flake8`, `pylint`, or `ruff` to catch errors and style issues.
- Prioritize readability and clarity over cleverness.
- Write code that is maintainable and self-documenting.

---

## Naming Conventions

| Element | Convention | Example |
|---------|-----------|---------|
| **Modules/Packages** | `lowercase_with_underscores` | `motor_control.py` |
| **Classes** | `PascalCase` | `VelocityController` |
| **Functions/Methods** | `snake_case` | `calculate_velocity()` |
| **Variables** | `snake_case` | `encoder_position` |
| **Constants** | `UPPER_CASE_WITH_UNDERSCORES` | `MAX_VOLTAGE = 56.0` |
| **Private Members** | `_leading_underscore` | `_internal_state` |

```python
# ✅ GOOD
class MotorController:
    MAX_SPEED = 100.0
    
    def __init__(self):
        self._current_velocity = 0.0
    
    def get_velocity(self) -> float:
        return self._current_velocity

# ❌ BAD
class motorController:
    maxSpeed = 100.0
    
    def GetVelocity(self):
        pass
```

---

## Type Hints

- Use type hints for **all** function arguments and return values.
- Use modern syntax (`list[str]`, `dict[str, int]`) for Python 3.9+.
- Use `typing` module for older Python or complex types (`Optional`, `Union`, `Callable`).

```python
from typing import Optional

def configure_motor(motor_id: int, settings: dict[str, float]) -> bool:
    """Configure motor with given settings."""
    return True

def find_encoder(name: str) -> Optional[Encoder]:
    """Find encoder by name, returns None if not found."""
    pass
```

---

## Docstrings

- Use **Google Style** docstrings for all public classes and functions.
- Include `Args:`, `Returns:`, `Raises:` sections as appropriate.
- Place docstrings immediately after the `def` or `class` statement.

```python
def calculate_torque(current: float, kt: float) -> float:
    """Calculate motor torque from phase current.

    Args:
        current: Phase current in Amps.
        kt: Motor torque constant in Nm/A.

    Returns:
        Motor torque in Newton-meters.

    Raises:
        ValueError: If current or kt is negative.
    """
    if current < 0 or kt < 0:
        raise ValueError("Current and kt must be non-negative")
    return current * kt
```

---

## Code Style and Formatting

- Use **4 spaces** for indentation (never tabs).
- Maximum line length: **88 characters** (Black default) or **120** for flexibility.
- Use blank lines to separate logical sections:
  - 2 blank lines between top-level definitions
  - 1 blank line between methods in a class
- Imports at the top of the file, grouped in order:
  1. Standard library
  2. Third-party packages
  3. Local/project imports

```python
# ✅ GOOD - Import ordering
import math
import os
from typing import Optional

import numpy as np

from odrive.utils import calculate_checksum
```

---

## Error Handling

- Use **specific exceptions** (`ValueError`, `TypeError`, `IOError`) not bare `Exception`.
- Use `try/finally` or context managers (`with`) for resource cleanup.
- Document exceptions in docstrings with `Raises:` section.
- Avoid silent failures—log or re-raise appropriately.

```python
# ✅ GOOD
def read_config(path: str) -> dict:
    """Read configuration from file.
    
    Raises:
        FileNotFoundError: If config file doesn't exist.
        ValueError: If config format is invalid.
    """
    try:
        with open(path, 'r') as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        raise ValueError(f"Invalid config format: {e}") from e

# ❌ BAD
def read_config_bad(path):
    try:
        return json.load(open(path))
    except:
        pass  # Silent failure - NEVER do this
```

---

## Testing

- Write unit tests for all public functions using `pytest` or `unittest`.
- Name test files with `test_` prefix: `test_motor_control.py`.
- Name test functions descriptively: `test_velocity_returns_zero_when_stopped()`.
- Cover edge cases: empty inputs, boundary values, invalid types.
- Include docstrings explaining what each test verifies.

```python
def test_calculate_torque_with_zero_current():
    """Verify torque is zero when current is zero."""
    assert calculate_torque(0.0, 0.5) == 0.0


def test_calculate_torque_raises_on_negative():
    """Verify ValueError raised for negative current."""
    with pytest.raises(ValueError):
        calculate_torque(-1.0, 0.5)
```

---

## Algorithm Documentation

- For complex algorithms, include:
  - Brief explanation of the approach
  - Time/space complexity if relevant
  - References to papers or documentation
- Use comments to explain **why**, not **what** (code shows what).

```python
def estimate_velocity(samples: list[float], dt: float) -> float:
    """Estimate velocity using least-squares differentiation.
    
    Uses a Savitzky-Golay filter approach for noise-robust
    differentiation of position samples.
    
    Reference: https://en.wikipedia.org/wiki/Savitzky–Golay_filter
    
    Time complexity: O(n) where n is number of samples.
    
    Args:
        samples: Position samples in turns.
        dt: Time step between samples in seconds.
    
    Returns:
        Estimated velocity in turns/second.
    """
    # Implementation here
    pass
```

---

## Operations with Hardware

- When communicating with ODrive devices via USB/Serial, handle timeouts gracefully.
- Use the `odrive` library abstraction layers rather than raw serial commands.
- Always implement proper cleanup in `finally` blocks or context managers.
- Log communication errors with sufficient context for debugging.

```python
# ✅ GOOD - Hardware communication with proper handling
def configure_odrive(serial_number: str, config: dict) -> bool:
    """Configure ODrive with given settings.
    
    Args:
        serial_number: ODrive serial number.
        config: Configuration dictionary.
    
    Returns:
        True if configuration successful.
    
    Raises:
        TimeoutError: If device doesn't respond.
        ConnectionError: If device disconnects.
    """
    try:
        odrv = odrive.find_any(serial_number=serial_number, timeout=10)
        if odrv is None:
            raise TimeoutError(f"ODrive {serial_number} not found")
        
        for key, value in config.items():
            setattr(odrv.axis0, key, value)
        
        odrv.save_configuration()
        return True
    except Exception as e:
        logging.error(f"Failed to configure ODrive {serial_number}: {e}")
        raise
```

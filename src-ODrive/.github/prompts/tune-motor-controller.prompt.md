# Tune Motor Controller

Analyze and tune ODrive motor control parameters for optimal performance.

## Instructions

Use the **ODrive-Engineer** agent with **foc-tuning** skill (planned) to analyze and optimize motor control parameters.

### Input Required
- **Axis**: ${{AXIS}} (e.g., "axis0", "axis1")
- **Control Mode**: ${{MODE}} (e.g., "velocity", "position", "current")
- **Issue**: ${{ISSUE}} (e.g., "oscillation", "overshoot", "sluggish response")

### What This Prompt Does

1. **Analyzes current controller settings** (Kp, Ki, Kd, bandwidth)
2. **Identifies performance issues**:
   - Oscillations (controller too aggressive)
   - Overshoot (insufficient damping)
   - Sluggish response (controller too conservative)
   - Instability (incorrect parameters)
3. **Suggests tuning adjustments** based on control theory
4. **Provides measurement commands** to validate changes
5. **Documents tuning process** for future reference

### Commands

```bash
# Check current controller gains (using odrivetool)
# ⚠️ Requires hardware - ask for confirmation
odrivetool
odrv0.axis0.controller.config.vel_gain
odrv0.axis0.controller.config.vel_integrator_gain

# Analyze control loop performance
# Use oscilloscope or logging tools
python tools/plot_oscilloscope.py

# Simulate control loop response (planned script)
python .github/skills/foc-tuning/scripts/simulate_response.py --kp 0.02 --ki 0.001

# Measure system response (planned script)  
python .github/skills/foc-tuning/scripts/measure_step_response.py axis0
```

### Prerequisites

- Motor calibration completed
- Encoder properly configured
- Basic understanding of PID control
- Access to motor control parameters

### Example Usage

Tune velocity controller:
```
@odrive-engineer My velocity controller is oscillating at 10 Hz. Current vel_gain is 0.02. Help me tune it.
```

Analyze control instability:
```
@odrive-engineer Position control is unstable - motor vibrates when holding position. What should I check?
```

Optimize response time:
```
@odrive-engineer Velocity response is too slow. How can I make it more aggressive without oscillations?
```

### Control Loop Tuning Guide

#### Velocity Controller (Inner Loop)

**Parameters:**
- `vel_gain` (Kp): Proportional gain
- `vel_integrator_gain` (Ki): Integral gain

**Symptoms & Solutions:**

| Symptom | Likely Cause | Adjustment |
|---------|--------------|------------|
| Oscillation at specific frequency | Kp too high | Reduce `vel_gain` by 20-50% |
| Sluggish response | Kp too low | Increase `vel_gain` by 20-30% |
| Steady-state error | Ki too low | Increase `vel_integrator_gain` |
| Overshoot/ringing | Ki too high or Kp too high | Reduce both gains |

**Tuning Process:**
1. Start with low gains (conservative)
2. Increase Kp until oscillation begins
3. Back off Kp by 30-40%
4. Add small Ki for steady-state accuracy
5. Test step response and adjust

---

#### Position Controller (Outer Loop)

**Parameters:**
- `pos_gain` (Kp): Proportional gain

**Symptoms & Solutions:**

| Symptom | Likely Cause | Adjustment |
|---------|--------------|------------|
| Slow tracking | Kp too low | Increase `pos_gain` |
| Overshoot on moves | Kp too high | Decrease `pos_gain` |
| Oscillation around target | Velocity loop poorly tuned | Tune velocity loop first |

**Tuning Process:**
1. **First:** Tune velocity controller (inner loop)
2. **Then:** Tune position controller (outer loop)
3. Start with low `pos_gain` (e.g., 10-20)
4. Increase until tracking is responsive
5. Check for overshoot on moves

---

### Measurement & Validation

**Step Response Test:**
```python
# Command a step input
# Measure: rise time, overshoot, settling time

# Good response characteristics:
# - Rise time: < 100ms
# - Overshoot: < 10%
# - Settling time: < 200ms
# - No oscillation
```

**Frequency Response:**
```python
# Sweep input frequency
# Measure: gain margin, phase margin, bandwidth

# Target specifications:
# - Gain margin: > 6 dB
# - Phase margin: > 30 degrees
# - Bandwidth: 10-100 Hz (typical)
```

### Output

- Current controller parameters
- Identified performance issues
- Recommended parameter changes
- Step-by-step tuning instructions
- Validation measurements to perform
- Expected performance improvements

### Skills Invoked

| Skill | Purpose | Status |
|-------|---------|--------|
| **foc-tuning** | Automated tuning procedures | 🚧 Planned |
| **ODrive-Engineer** | Manual tuning guidance | ✅ |
| **odrive-toolchain** | Build firmware with new gains | ✅ |

### Safety Notes

⚠️ **Tuning requires hardware operation:**
- Motor will spin during tuning
- Ensure motor is securely mounted
- Keep safe distance from moving parts
- Have emergency stop ready
- Start with conservative (low) gains

⚠️ **Never auto-execute tuning without confirmation!**

### Related Prompts

| Need | Use |
|------|-----|
| Debug motor errors | `debug-motor-error-v2.prompt.md` |
| Explain FOC theory | `explain-foc.prompt.md` |
| Check safety | `check-safety.prompt.md` |
| Optimize performance | `optimize-critical.prompt.md` |

### Control Theory References

- **PID Tuning**: Ziegler-Nichols method
- **FOC Theory**: Field-Oriented Control fundamentals
- **Stability**: Bode plots, Nyquist criterion
- **Performance**: Step response specifications

### Future Enhancements (Planned)

When `foc-tuning` skill is implemented:
- Automated step response measurement
- Auto-tuning algorithms (relay method, model-based)
- Frequency response analysis
- Stability margin calculation
- Performance optimization tools

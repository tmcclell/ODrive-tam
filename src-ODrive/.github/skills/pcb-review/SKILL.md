---
name: pcb-review
description: 🚧 STUB - Automated PCB schematic and layout review assistance
status: in-development
---

# PCB Review Skill (🚧 In Development)

**Status:** Stub - Planned for future implementation  
**Owner:** hardware-engineer.agent

## Purpose

This skill will provide automated assistance for reviewing PCB schematics and layouts, checking for common design issues, and validating design rules.

## Planned Capabilities

- Schematic review checklists
- Power supply sequencing validation
- Decoupling capacitor placement analysis
- Signal routing review (high-speed, analog)
- Component selection verification
- BOM optimization and availability checking
- Design rule compliance checking

## Dependencies

- PCB design tool export formats (KiCad, Altium)
- Component database access
- Design rule libraries
- Manufacturing constraint knowledge

## Usage Examples (Planned)

```python
# Example: Automated schematic review
from odrive.pcb_tools import SchematicReviewer

reviewer = SchematicReviewer('board_v4.0')
issues = reviewer.check_all()
reviewer.generate_report('review_report.md')
```

## Implementation Status

- [ ] Schematic parsing and analysis
- [ ] Power supply review automation
- [ ] Decoupling verification
- [ ] High-speed signal routing checks
- [ ] Component verification
- [ ] BOM analysis tools
- [ ] Design rule checking

## Related Skills

- `signal-integrity` - Signal quality analysis
- `power-electronics` - Power circuit review (planned)

---

*This skill is currently a placeholder. Implementation pending.*

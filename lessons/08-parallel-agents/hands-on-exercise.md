# Section 8: Hands-On Exercise - Parallel Agents

**Duration:** 5 minutes (fast-paced exercise)  
**Format:** Individual or pair programming  
**Goal:** Experience parallel agent orchestration with real ODrive code

---

## Quick Exercise: Implement CAN Bus Diagnostics

### Scenario
Your team needs comprehensive CAN bus diagnostics for debugging communication issues in the field. This requires:
- **Hardware expertise** - CAN electrical specs and error detection
- **Firmware implementation** - Error counters and logging
- **Testing** - Validation of diagnostic accuracy

### Your Mission
Use **3 parallel agents** to design and implement this feature in **5 minutes**.

---

## Step 1: Launch Parallel Agents (1.5 min)

Open **3 separate Copilot Chat panels** and send these prompts simultaneously:

### Chat Panel 1: Hardware Focus

> Select **ODrive Engineer** from agent dropdown, then paste:

```
What CAN bus hardware diagnostics are available on ODrive?

Task Context: Hardware/electrical analysis (will invoke pcb-review skill)

Context:
- ODrive uses STM32 CAN peripheral (bxCAN or FDCAN)
- Board: v3.6 or v4.x
- CAN transceiver: TJA1051 or similar

Questions:
1. What hardware error counters exist (TX errors, RX errors, bus-off)?
2. How to detect: bit errors, stuff errors, CRC errors, form errors?
3. Can we measure bus voltage/signal quality?
4. What causes "bus-off" state and how to recover?
```

### Chat Panel 2: Firmware Focus

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Design a CAN diagnostics module for ODrive.

Task Context: Firmware implementation

Context:
- File: src-ODrive/Firmware/communication/can/odrive_can.cpp, odrive_can.hpp
- Need to track: message counters, error counters, bus state
- Accessible via USB/CAN for remote debugging

Requirements:
- Track TX/RX message counts
- Track error counts (per type)
- Detect bus-off, error-warning, error-passive states
- Timestamp last error
- Provide reset_diagnostics() method
- Low overhead (don't impact real-time performance)

Show me the diagnostics struct definition.
```

### Chat Panel 3: QA/Testing

> Select **ODrive QA** from agent dropdown, then paste:

```
Create test plan for CAN bus diagnostics.

Task Context: Testing & validation (invokes cpp-testing skill)

Feature: Track CAN errors and statistics
Files: src-ODrive/Firmware/communication/can/odrive_can.cpp

Test scenarios:
1. Normal operation (count messages, no errors)
2. Intentional bit errors (short CAN_H to ground)
3. Bus-off recovery
4. High traffic (message loss detection)
5. Diagnostic counter overflow handling

What test equipment needed? How to verify accuracy?
```

**⏰ Set a timer for 1.5 minutes - review responses when all agents reply**

---

## Step 2: Review Agent Outputs (1.5 min)

Quickly scan each agent's response:

**ODrive Engineer (Hardware Focus) should provide:**
- ✅ Hardware error registers (TEC, REC counters)
- ✅ Error types detectable
- ✅ Bus state definitions
- ✅ Recovery procedures

**ODrive Engineer (Firmware Focus) should provide:**
- ✅ Diagnostics struct layout
- ✅ Where to hook error callbacks
- ✅ Storage considerations
- ✅ Performance impact assessment

**ODrive QA should provide:**
- ✅ Test scenarios
- ✅ Required test equipment (CAN analyzer, resistor for faults)
- ✅ Acceptance criteria
- ✅ Test rig configuration

**If any agent's response is unclear:**
```
"Can you elaborate on [specific point]?"
```

---

## Step 3: Synthesize Implementation (1.5 min)

**Open a new Chat Panel (Panel 4):**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement CAN diagnostics based on expert input.

Hardware constraints (from hardware-focused window):
- Use STM32 CAN error registers (ESR, TEC, REC)
- Monitor LEC (Last Error Code) for error types
- Handle bus-off state with auto-recovery

Design (from firmware-focused window):
- Diagnostics struct in can.hpp
- Hook into CAN interrupt handlers
- Minimal performance overhead

Testing requirements (from ODrive QA window):
- Must handle counter overflow gracefully
- Diagnostic reset doesn't affect active communication
- Timestamps for debugging

Files: src-ODrive/Firmware/communication/can/odrive_can.cpp, odrive_can.hpp

Implement:
1. CANDiagnostics struct
2. Update CAN error interrupt handler to populate diagnostics
3. Add get_diagnostics() and reset_diagnostics() methods
4. Ensure thread-safety (CAN interrupts vs USB reads)

Show the implementation for can.hpp first.
```

**⏰ 1.5 minutes to review implementation**

---

## Step 4: Quick Validation (30 sec)

**Ask yourself:**
- ✅ Does implementation match hardware capabilities?
- ✅ Are all error types covered?
- ✅ Is it testable per QA's criteria?
- ✅ Low performance overhead?

**If issues found:**
```
"The implementation is missing [X]. Please add it."
```

---

## Success Criteria

By the end of 5 minutes, you should have:
- ✅ Hardware constraints understood (from ODrive Engineer with hardware context)
- ✅ Implementation design (from ODrive Engineer with firmware context)
- ✅ Test strategy (from ODrive QA)
- ✅ Working code structure (synthesized implementation)

---

## What You Learned

**Key Takeaways:**
1. **Speed:** 3 parallel windows vs. 3 sequential queries = 3x faster
2. **Coverage:** Different specialized agents produced unique domain insights
3. **Quality:** Better implementation because all angles covered
4. **Orchestration:** You coordinated multiple agents - that's the skill!
5. **Key Insight:** Use the right agent for the domain: **ODrive Engineer** for firmware, **ODrive QA** for testing, **ODrive Ops** for CI/CD!

---

## Bonus Challenge (If You Have Time)

### Extend with Background Agent

**Scenario:** Generate comprehensive test suite automatically

**GitHub Issue (if GitHub integration available):**
```
@copilot ODrive QA

Based on the CAN diagnostics implementation in src-ODrive/Firmware/communication/can/odrive_can.cpp,
generate a complete test suite in src-ODrive/Firmware/Tests/test_can_diagnostics.cpp

Task Context: Testing & automation (invokes cpp-testing skill)

Requirements:
- Use doctest framework (existing ODrive tests)
- Cover all error types
- Test counter overflow
- Test thread safety (concurrent access)
- Mock CAN hardware registers
- 100% code coverage of diagnostics module

Create a PR with the test implementation.
```

Let the background agent work while you continue other tasks!

---

## Alternative Exercise (Choose One)

If CAN diagnostics doesn't interest you, try one of these:

### Option A: USB Enumeration Diagnostics
- Hardware: USB hardware capabilities
- Firmware: Track enumeration state, disconnects, errors
- QA: Test USB plug/unplug, enumeration failures

### Option B: Motor Thermal Model
- Hardware: Thermistor specs, thermal resistance
- Motor Control: Thermal model equations (I²R losses)
- Firmware: Real-time temperature estimation
- QA: Validate against physical temperature measurements

### Option C: Multi-Axis Synchronization
- Firmware: Implement synchronized motion across multiple axes
- Motor Control: Trajectory coordination, timing constraints
- Hardware: CAN bus latency considerations
- QA: Test synchronization accuracy, jitter measurement

---

## Presenter Notes

### Time Management
- **Strict 5-minute limit** - This is a "taste" of parallel agents
- Announce "1 minute remaining" to help participants wrap up
- OK if participants don't finish - goal is experiencing the workflow

### Circulation Tips
Look for:
- Participants launching agents sequentially (not in parallel) → Show them how to open multiple chat panels
- Participants stuck on prompts → Direct them to the example prompts
- Participants getting poor agent responses → Help them add more context

### Common Issues

**"My agents are slow to respond"**
→ Network issue. Have backup: "Let's use the example responses in solutions/"

**"I don't have 3 separate chat panels"**
→ VS Code: View → Chat (multiple times) or use web browser tabs with GitHub.com

**"Which agent do I ask this to?"**
→ Use the guide:
- Hardware/electrical = ODrive Engineer (with hardware context in prompt)
- Implementation = ODrive Engineer (with firmware context in prompt)
- Algorithm/math = ODrive Engineer (with control context in prompt)
- Testing/QA = ODrive QA
- Build/compile = ODrive Toolchain
- CI/CD/releases = ODrive Ops
- Code review = ODrive Reviewer
- Ada migration = Ada to C++ Migrator

> **Key:** 6 specialized agents + 9 skills!

### Debrief Questions (After Exercise)

**Ask the group:**
> "Who launched all 3 agents in parallel vs. sequential? [show of hands]  
> Parallel folks - did you feel the speed advantage?"

> "What was harder - asking good questions or synthesizing the answers?"

> "Would you use this on your real work? What feature would benefit?"

---

## Solution Reference

Full solution available in:
`solutions/section8-can-diagnostics/`

Files:
- `can_diagnostics_struct.hpp` - Complete struct definition
- `can_error_handler.cpp` - Interrupt handler implementation
- `test_plan.md` - Full QA test specification
- `hardware_notes.md` - Hardware engineer's full analysis

Use these for:
- Participants who finish early (they can compare)
- Participants who get stuck (they can reference)
- Post-workshop self-study

---

## Extension Activities (Post-Workshop)

For participants who want more practice:

### Activity 1: Add Advanced Features
Extend the CAN diagnostics with:
- Bus load percentage calculation
- Message latency histogram
- Error rate trending (errors per second)
- Automatic bus-off recovery with exponential backoff

### Activity 2: Create Dashboard
Build a Python tool that:
- Queries CAN diagnostics via USB
- Plots error rates over time
- Alerts when errors exceed threshold
- Exports diagnostics to CSV

### Activity 3: Documentation Sprint
Use parallel agents to:
- ODrive Engineer (firmware context): Generate API docs for CAN module
- ODrive Reviewer: Review documentation for completeness
- ODrive QA: Generate test report template
- ODrive Ops: Set up docs CI/CD workflow

---

## Feedback Collection

**Quick poll (30 seconds):**
> "On a scale of 1-5, how likely are you to use parallel agents in your work?  
> 1 = Not at all, 5 = Definitely"

> "What's the biggest barrier to adopting this approach?"
- Context engineering skills
- Access to custom agents
- Time to learn
- Unclear when to use it

**Note these for final session wrap-up!**

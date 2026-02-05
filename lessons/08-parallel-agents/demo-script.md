# Section 8 Demo Script: Parallel Agents

**Duration:** 5 minutes (live demo portion of Section 8)  
**Presenter Notes:** This is a fast-paced, high-impact demo showing parallel agent orchestration

---

## Demo Setup (Before Session)

### VS Code Preparation

#### 1. Open the Workspace
- [ ] Open ODrive workspace in VS Code
- [ ] Have these files ready to reference:
  - `src-ODrive/Firmware/MotorControl/encoder.cpp`
  - `src-ODrive/Firmware/MotorControl/encoder.hpp`
  - `src-ODrive/Firmware/MotorControl/motor.cpp`

#### 2. Set Up Multiple Chat Threads
VS Code doesn't support multiple labeled chat panels, but you can simulate parallel agents using **multiple chat threads**:

1. **Open the Chat panel:** Press `Ctrl+Alt+I` (Windows) or `Cmd+Alt+I` (Mac)
2. **Create 4 new chat threads:**
   - Click the **"+" button** (New Chat) in the Chat panel header
   - Repeat to create 4 separate threads
3. **"Label" each thread** by making the first line of your prompt descriptive:
   - Thread 1: Start prompt with `## Hardware Analysis`
   - Thread 2: Start prompt with `## Firmware Implementation`
   - Thread 3: Start prompt with `## Control Strategy`
   - Thread 4: Start prompt with `## QA/Testing`
4. **Switch between threads** using the chat history dropdown (clock icon) or by clicking previous conversations

#### 3. Alternative: Side-by-Side Chat Windows
For a more visual demo showing parallel execution:
1. Open Chat panel (`Ctrl+Alt+I`)
2. **Drag the Chat tab** from the sidebar into the main editor area
3. Open Chat again (`Ctrl+Alt+I`) - now you have 2 visible chat areas
4. Use **"+" (New Chat)** in each area for additional threads

This lets you show 2 chats responding simultaneously on screen.

#### 4. Clear Chat History
- [ ] Click the **"..." menu** in the Chat panel → **Clear All Chats** (or start fresh threads)

#### 5. Test Agent Availability (if using custom agents)
- [ ] If you have custom agents installed: Click the agent dropdown in Chat and verify all 6 agents appear:
  - **ODrive Engineer** (primary development)
  - **ODrive QA** (testing & QA)
  - **ODrive Ops** (CI/CD & releases)
  - **ODrive Reviewer** (code review)
  - **ODrive Toolchain** (build & test)
  - **Ada to C++ Migrator** (Ada migration)
- [ ] If custom agents don't appear: Ensure `.github/agents/` folder exists in the workspace

> **Note:** Custom agents are selected from the **dropdown menu** at the top of the Copilot Chat panel, NOT via @mention syntax. Select the agent first, then type your prompt.

### Backup Plan
- [ ] Take screenshots of expected agent responses
- [ ] Have recorded demo video ready (in case of network issues)
- [ ] Print prompts on note cards for reference

---

## Demo Flow: Encoder Calibration Feature

### Introduction (30 seconds)

**Presenter Says:**
> "We're going to implement encoder calibration - a feature that requires expertise in hardware, firmware, control theory, and testing. Instead of doing these sequentially, watch how we use parallel agents to work on all aspects simultaneously."

**Show the 4 Chat Panels:**
> "I've opened 4 separate chat panels. Each will use different specialized agents selected from the dropdown menu. We'll select **ODrive Engineer** for firmware and control tasks, and **ODrive QA** for testing. The key is that we have 6 agents available in the dropdown, each specialized for different domains!"

---

### Step 1: Launch All Agents Simultaneously (1 minute)

**Presenter Says:**
> "I'm going to fire off 4 questions to 4 different agents - all at once. Watch the timing."

**Chat Panel 1 (Hardware Focus):**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
What are the electrical requirements for encoder 
calibration on ODrive v3.6?

Task Context: Hardware/electrical analysis (will invoke pcb-review skill)

Context:
- Encoder: Incremental with index pulse
- Calibration rotates motor one full revolution
- Need to measure electrical angle vs mechanical angle

Questions:
1. Max safe calibration current?
2. Max safe calibration speed?
3. Any risk of damaging encoder?
4. Special considerations for different encoder types?
```

**Chat Panel 2 (Firmware Focus):**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Design the encoder calibration routine structure.

Task Context: Firmware implementation

Context:
- File: src-ODrive/Firmware/MotorControl/encoder.cpp
- Must rotate motor smoothly
- Record encoder counts vs electrical angle
- Detect index pulse
- Store calibration in NVM
- Trigger via axis.requested_state = AXIS_STATE_ENCODER_CALIBRATION

Show me:
1. Function signature for calibrate()
2. High-level algorithm steps
3. Error cases to handle
4. Where to store calibration data
```

**Chat Panel 3 (Control Focus):**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
What's the optimal control strategy during 
encoder calibration?

Task Context: Control algorithms (will invoke control-algorithms skill)

Context:
- Need constant velocity (e.g., 1 revolution per second)
- Minimize torque ripple for accurate measurement
- Motor may have unknown load

Questions:
1. Open-loop or closed-loop control?
2. What current/voltage profile?
3. How to handle mechanical resistance?
4. How to verify calibration quality?
```

**Chat Panel 4 (QA/Testing):**

> Select **ODrive QA** from agent dropdown, then paste:

```
Create test plan for encoder calibration.

Task Context: Testing & validation (invokes cpp-testing skill)

Context:
- New feature: AXIS_STATE_ENCODER_CALIBRATION
- Calibrates encoder offset and direction
- Stores data in NVM

Test Coverage Needed:
1. Happy path (normal calibration)
2. Repeatability (10 runs, compare results)
3. Error cases (stall, disconnected encoder, power loss)
4. Edge cases (different motor types, loaded shaft)
5. Persistence (survives reboot)

Provide test cases and acceptance criteria.
```

**Presenter Says:**
> "Notice I sent all 4 prompts within seconds of each other. The agents are now working in parallel. Let's see their responses..."

*[Pause 10-15 seconds for agents to respond]*

---

### Step 2: Review Agent Outputs (1.5 minutes)

**Presenter Says:**
> "Let's review what each agent came up with. I'll go through them quickly."

**Panel 1 - ODrive Engineer (Hardware Focus) Response:**
> "The agent (with hardware context) tells me:
> - Max calibration current: 10A (safe for continuous operation)
> - Max speed: 2 rev/sec (to avoid back-EMF issues)
> - No damage risk to encoder if within these limits
> - For different encoder types, may need different resolution
> 
> Great! Now I know the electrical constraints."

**Panel 2 - ODrive Engineer (Firmware Focus) Response:**
> "The agent (with firmware context) provided:
> - Function signature: `bool Encoder::calibrate(float current_A, float speed_revs)`
> - Algorithm steps: Initialize → Ramp up → Constant velocity → Record data → Detect index → Store → Stop
> - Error cases: Stall detection, timeout, encoder disconnect
> - Storage: Use `encoder_.config_.calibration_offset` and `encoder_.config_.direction`
> 
> Perfect! I have the implementation structure."

**Panel 3 - ODrive Engineer (Control Focus) Response:**
> "The agent (with control context) recommends:
> - Use open-loop current control (Iq command)
> - Constant current setpoint (e.g., 5A in Q-axis)
> - No velocity feedback during calibration (encoder not yet calibrated!)
> - Quality check: Verify index pulse detected, no missed counts
> 
> This answers the 'how to control' question."

**Panel 4 - ODrive QA Response:**
> "The QA agent created:
> - 8 detailed test cases covering happy path and edge cases
> - Test rig configuration (test-rig-calibration.yaml)
> - Acceptance criteria: Calibration repeatable within 0.5 electrical degrees
> - Performance requirement: Complete in < 5 seconds
> 
> Excellent! We have a complete test strategy."

**Presenter Says:**
> "In about 90 seconds, we gathered expertise from 4 different domains using specialized agents. If I had done this sequentially, it would have taken 5-10 minutes. Now let's integrate their knowledge."

---

### Step 3: Synthesize into Implementation (1.5 minutes)

**Open a New Chat Panel (Panel 5):**

**Presenter Says:**
> "Now I'll take all this expert input and ask for the full implementation. I'll reference what we learned from the parallel agents."

**Chat Panel 5:**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Implement the encoder calibration routine.

Context from Parallel Agent Windows:
✅ Hardware constraints (from hardware-focused window):
   - Max current: 10A
   - Max speed: 2 rev/sec
   - Safe for all encoder types

✅ Control strategy (from control-focused window):
   - Open-loop current control
   - Constant Iq setpoint (5-10A)
   - No velocity feedback (encoder uncalibrated)

✅ Implementation structure (from firmware-focused window):
   - Function: bool Encoder::calibrate(float current_A, float speed_revs)
   - Steps: Init → Ramp → Constant velocity → Record → Detect index → Store
   - Storage: encoder_.config_.calibration_offset, encoder_.config_.direction

✅ Error handling:
   - Stall detection (motor doesn't move)
   - Timeout (calibration takes too long)
   - Encoder disconnect (no pulses)
   - Index not found (encoder doesn't have index)

✅ Testing criteria (from ODrive QA window):
   - Repeatability within 0.5 electrical degrees
   - Complete in < 5 seconds

Files: src-ODrive/Firmware/MotorControl/encoder.cpp, encoder.hpp

Please implement the complete calibration routine with all safety checks.
```

**Presenter Says:**
> "Notice how I'm providing rich context from all the parallel agents. The Firmware Engineer now has all the information needed to produce production-quality code."

*[Wait for response]*

**Presenter Says:**
> "And there it is! A complete, safe implementation that:
> - Respects hardware limits from the hardware-focused window
> - Uses the control strategy from the control-focused window
> - Handles all error cases identified by the firmware-focused window
> - Meets the test criteria from the **ODrive QA** window
> 
> This is the power of parallel agents - using **6 specialized agents** and **9 skills** to gather multi-domain expertise quickly and synthesize it into working code."

---

### Step 4: Wrap-Up (30 seconds)

**Presenter Says:**
> "Let's recap what just happened:
> 
> **Time Comparison:**
> - Sequential approach: 10-15 minutes (query each expert, wait, query next)
> - Parallel agents: 3 minutes (all queries at once, synthesize)
> 
> **Quality Comparison:**
> - Without agents: You might forget hardware constraints or test cases
> - With parallel agents: All domains covered systematically
> 
> **The Key Insight:** You become an orchestrator. Your job is to:
> 1. Identify the domains needed (hardware, firmware, control, QA)
> 2. Use the right agent for each domain: **ODrive Engineer**, **ODrive QA**, **ODrive Ops**, **ODrive Reviewer**, **ODrive Toolchain**, or **Ada to C++ Migrator**
> 3. 6 specialized agents + 9 skills = comprehensive expertise
> 4. Synthesize the answers into a coherent implementation
> 
> This is modern software engineering with AI assistance!"

---

## Presenter Tips

### Before the Demo

**Practice the Demo:**
- Run through it 2-3 times before the session
- Agents may give slightly different responses each time - be flexible
- Have your prompts ready (copy-paste from this script)

**Prepare for Variations:**
- If an agent gives an unexpected response, roll with it: "Interesting! The agent suggests [X]. Let me refine..."
- If an agent is slow, fill time by discussing what you expect
- If network fails, use backup screenshots

### During the Demo

**Pacing:**
- **Fast setup** - Don't spend time explaining, just fire the prompts
- **Slow review** - Take time to explain each agent's contribution
- **Fast synthesis** - Show the final result quickly

**Highlighting:**
- Use mouse/pointer to highlight key parts of agent responses
- Read key bullet points aloud - don't assume audience reads fast enough
- Emphasize the **time saved** and **quality improved**

**Engagement:**
- Ask audience: "Who would have remembered to check [hardware constraint]?"
- Make it relatable: "This is like having 4 senior engineers on your team"

### Handling Issues

**If an agent gives poor output:**
> "Sometimes agents need refinement. Let me be more specific..." [Show refinement]

**If an agent is slow:**
> "While we wait, let me explain what I'm expecting from this agent..." [Fill time]

**If there's a network issue:**
> "Let me show you the expected response I prepared..." [Use backup screenshots]

**If audience has questions mid-demo:**
> "Great question - let me finish the demo, then I'll address that."

---

## Backup Materials

### Screenshots to Prepare

Take screenshots of:
1. All 4 agent prompts typed out
2. Each agent's response
3. The final synthesis prompt
4. The final implementation

Label them: `demo8-step1.png`, `demo8-step2.png`, etc.

### Recorded Video

Record a full run-through (3-4 minutes) as backup:
- Use OBS Studio or Windows Game Bar
- Include audio narration
- Upload to cloud storage accessible at venue

---

## Audience Engagement

### After the Demo - Discussion Questions

**Ask the audience:**
> "Who can think of a feature in your codebase that would benefit from this parallel agent approach?"

*[Take 1-2 examples, discuss how they'd decompose it]*

> "What's the hardest part - formulating the questions, or synthesizing the answers?"

*[Most will say formulating questions - emphasize context engineering skills]*

---

## Follow-Up Materials

Point audience to:
- `hands-on-exercise.md` - They'll try parallel agents themselves
- `coordination-patterns.md` - Advanced patterns for larger projects
- `parallel-agent-prompts.md` - Ready-to-use prompt templates

---

## Timing Breakdown

| Activity | Target Time | Actual Time |
|----------|-------------|-------------|
| Introduction | 30 sec | ___ |
| Launch agents | 1 min | ___ |
| Review outputs | 1.5 min | ___ |
| Synthesize | 1.5 min | ___ |
| Wrap-up | 30 sec | ___ |
| **Total** | **5 min** | ___ |

Use this to stay on track during the demo!

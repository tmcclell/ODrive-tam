# Section 4: Agentic Development & Context Engineering

**Presentation Slides**  
**Duration:** 45 minutes (10:40-11:25)  
**Format:** PowerPoint/Google Slides

---

## Slide 1: Title Slide
**Title:** Agentic Development & Context Engineering  
**Subtitle:** Orchestrating AI for Embedded Systems  
**Time:** 10:40-11:25 (45 minutes)

---

## Part 1: What is an Agentic Developer? (8 min)

### Slide 2: Traditional vs Agentic Developer
**Layout:** Two-column comparison

| Traditional Developer | Agentic Developer |
|----------------------|-------------------|
| 🖊️ Writes every line manually | 🎯 Directs AI agents |
| 🔧 Passive tools | 🤖 Active collaborators |
| 🧠 Does all thinking | 🎼 Orchestrates experts |
| ⏱️ Hours of coding | ⏱️ Minutes of directing |

**Speaker Notes:** "The shift is from 'How do I code this?' to 'How do I describe this?'"

---

### Slide 3: The Mindset Shift
**Visual:** Workflow diagram

```
OLD: Developer → Manual Coding → Solution (4 hours)

NEW: Developer → AI Agents → Review → Solution (30 minutes)
                    ↓↑
              (Iteration)
```

**Key Point:** "You become an architect, not a bricklayer"

---

### Slide 4: ODrive Agents & Skills Architecture
**Layout:** 6 agents + skills diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    SPECIALIZED AGENTS (6)                       │
├──────────────┬──────────────┬──────────────┬────────────────────┤
│ 🔧 ODrive    │ ✅ ODrive    │ 🚀 ODrive    │ 👁️ ODrive         │
│   Engineer   │    QA        │    Ops       │    Reviewer        │
│ Development  │ Testing      │ CI/CD        │ Code Review        │
├──────────────┴──────────────┴──────────────┴────────────────────┤
│ 🔨 ODrive Toolchain        │ 🔄 Ada to C++ Migrator             │
│   Build & Test             │   Legacy Migration                  │
└────────────────────────────┴────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                   SPECIALIZED SKILLS (9)                        │
├──────────────────┬──────────────────┬───────────────────────────┤
│ ✅ odrive-       │ ✅ odrive-ops    │ ✅ cpp-testing            │
│    toolchain     │    CI/CD         │    Test generation        │
├──────────────────┼──────────────────┼───────────────────────────┤
│ ✅ ada-cpp-      │ 🚧 control-      │ 🚧 foc-tuning             │
│    migration     │    algorithms    │                           │
├──────────────────┼──────────────────┼───────────────────────────┤
│ 🚧 pcb-review    │ 🚧 sensorless-   │ 🚧 signal-integrity       │
│                  │    control       │                           │
└──────────────────┴──────────────────┴───────────────────────────┘
```

**Speaker Notes:** "6 agents, 9 skills. Select the right agent from dropdown - it routes to appropriate skills!"

---

## Part 2: Context Engineering (12 min)

### Slide 5: Why Context Matters
**Visual:** Venn diagram

```
   [Rich Context]
         ↓
   [Better Output]
         ↓
   [Less Iteration]
```

**Quote:** "Context is currency in AI collaboration"

---

### Slide 6: Poor vs Rich Context
**Layout:** Side-by-side code blocks

**❌ Poor:**
```
"Add error handling to motor"
```
Result: Generic code, doesn't compile

**✅ Rich:**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Add error handling to motor.cpp

Context:
- File: src-ODrive/Firmware/MotorControl/motor.cpp
- Use axis.error_ enum
- Must work in 8kHz interrupt
- No dynamic allocation
- MISRA C++ compliant

Handle: Overcurrent, overtemp, encoder disconnect
```
Result: Production-ready code! Agent invokes `odrive-toolchain` to verify build.

---

### Slide 7: The 5 W's of Context Engineering
**Layout:** Circular diagram with 5 sections

```
       [WHAT]
      ↗      ↖
  [WHY]      [WHERE]
      ↖      ↗
       [WHEN] [WHO]
```

1. **WHAT** - Goal/objective
2. **WHERE** - Files/modules (with `src-ODrive/` prefix)
3. **WHY** - Business reason
4. **WHEN** - Timing constraints
5. **WHO** - Right agent (ODrive Engineer, ODrive QA, ODrive Ops, etc.)

---

### Slide 8: Context Techniques
**Layout:** Bullet list with examples

1. **File References**
   - `"Review Firmware/MotorControl/motor.cpp"`

2. **@-mentions**
   - `"@workspace Find all thermistor uses"`

3. **Code Selection**
   - Select code + `Ctrl+I`

4. **Multi-file Context**
   - Reference multiple files at once

5. **Error Context**
   - `"@terminal shows: [error]"`

---

### Slide 9: Context Layering
**Visual:** Pyramid diagram

```
      ┌─────────────────┐
      │ Skills (Auto)   │ Layer 5
      ├─────────────────┤
      │  Requirements   │ Layer 4
      ├─────────────────┤
      │ Specific Files  │ Layer 3
      ├─────────────────┤
      │ Agent Selection │ Layer 2
      ├─────────────────┤
      │ Constitution    │ Layer 1
      │ (copilot-       │
      │  instructions)  │
      └─────────────────┘
```

**Bottom-up approach:** Build context in layers; agents auto-invoke skills

---

### Slide 10: Demo - Context Engineering
**Slide Type:** Demo announcement

**Text:**
> 🎬 **LIVE DEMO**
> 
> Adding Overcurrent Protection
> - Poor Context → Generic output
> - Rich Context → Perfect fit!
> 
> See: `demo-script.md` Section 1

**Speaker Notes:** Execute Demo 1 from demo-script.md

---

## Part 3: Decomposition Strategies (8 min)

### Slide 11: Why Decompose?
**Visual:** Large task broken into pieces

```
[Complex Task: Sensorless Motor Control]
            ↓ DECOMPOSE
┌────────┬────────┬────────┬────────┐
│Design  │Implement│Integrate│Test   │
│API     │Algorithm│System   │Validate│
└────────┴────────┴────────┴────────┘
```

**Key Point:** "AI handles pieces better than monoliths"

---

### Slide 12: Decomposition Patterns
**Layout:** 3 columns

**Top-Down**
1. Architecture
2. Core algorithm
3. Integration
4. Testing

**Bottom-Up**
1. Utilities
2. Components
3. Integration
4. Optimization

**Horizontal Slice**
1. Minimal MVP
2. Add feedback
3. Add robustness
4. Optimize

---

### Slide 13: Example - CAN Extension
**Visual:** Tree diagram

```
CAN Timestamp Extension
├── Protocol Definition (ODrive Engineer)
│   ├── Message format (pcb-review skill 🚧)
│   └── DBC file update
├── Firmware (ODrive Engineer)
│   ├── CAN driver update
│   ├── Timestamp generation
│   └── Message handlers
├── Python Tools (Regular Copilot)
│   ├── Parser
│   └── ODrive API
└── Testing (ODrive QA)
    ├── Unit tests (cpp-testing skill)
    └── Integration tests
```

**Note:** Same agent can handle multiple domains via different prompts!

---

### Slide 14: When to Decompose
**Layout:** Decision tree

```
Is task >30 min manual work? ───NO──→ Don't decompose
    │
   YES
    ↓
Multiple files/modules? ───NO──→ Consider decomposing
    │
   YES
    ↓
Multiple domains? ───NO──→ Consider decomposing
    │
   YES
    ↓
✅ DEFINITELY DECOMPOSE
```

---

### Slide 15: Demo - Decomposition
**Slide Type:** Demo announcement

**Text:**
> 🎬 **LIVE DEMO**
> 
> CAN Timestamp Feature
> - Multi-domain (HW + FW + SW + QA)
> - Sequential agent consultation
> - Synthesis into complete feature
> 
> See: `demo-script.md` Section 2

**Speaker Notes:** Execute Demo 2 from demo-script.md

---

## Part 4: Iterative Refinement (5 min)

### Slide 16: The Refinement Loop
**Visual:** Circular flow diagram

```
1. Generate → 2. Review → 3. Refine → 4. Regenerate
     ↑                                      ↓
     └──────────────────────────────────────┘
```

**Key Insight:** "First output is rarely perfect - and that's OK!"

---

### Slide 17: Refinement Strategies
**Layout:** Numbered list with examples

1. **Be Specific About Issues**
   - ❌ "This isn't right"
   - ✅ "Interrupt priority too low, use RTOS limit"

2. **Reference Working Examples**
   - "Follow pattern in encoder.cpp:123-145"

3. **Add Constraints Incrementally**
   - Pass 1: Basic function
   - Pass 2: Error handling
   - Pass 3: Performance
   - Pass 4: Documentation

---

### Slide 18: Review Checklist
**Layout:** Checkbox list

**Before accepting generated code:**

- [ ] Memory allocation (stack only?)
- [ ] Interrupt safety (no blocking?)
- [ ] Thread safety (mutexes?)
- [ ] Error handling (all paths?)
- [ ] Coding style (matches project?)
- [ ] Performance (optimized?)
- [ ] Hardware constraints (timing OK?)
- [ ] Safety (bounds checking?)

---

## Part 5: Hands-On Exercise (12 min)

### Slide 19: Hands-On Exercise
**Layout:** 3 options

**Choose ONE exercise (12 minutes):**

1. **⭐⭐ Motor Diagnostics**
   - Track runtime, power, faults

2. **⭐⭐⭐ Safety Watchdog**
   - Monitor 6 fault conditions

3. **⭐ Configuration Manager**
   - Unified config with validation

**Files:** See `hands-on-exercises.md`

---

### Slide 20: Exercise Tips
**Layout:** Tips box

💡 **Tips:**
- Start with Step 1 prompts
- Use **ODrive Engineer** for development
- Use **ODrive QA** for testing/validation
- Reference `prompt-library.md`
- Don't worry about finishing
- Goal: Practice the workflow!

**Presenter:** "I'll circulate to help. Questions?"

---

## Closing

### Slide 21: Key Takeaways
**Layout:** Summary bullets

**Section 4: Agentic Development**
✅ Context is currency  
✅ Decompose complex tasks  
✅ Iterate to perfection  
✅ You're the architect  
✅ Agents orchestrate skills automatically
✅ 2 agents: **ODrive Engineer** + **ODrive QA**

---

### Slide 22: Q&A
**Layout:** Question prompts

**Common Questions:**
- How much context is too much?
- What if the agent doesn't understand my domain?
- Can I use this for safety-critical code?
- How do I debug when AI-generated code fails?

**Open Floor:** Any questions?

---

## Appendix: Backup Slides

### Backup Slide 1: Troubleshooting
**When agents don't respond well:**

1. Add more context
2. Use `/fix` for errors
3. Reference working examples
4. Try different agent
5. Break into smaller tasks

---

### Backup Slide 2: Agent & Skill Selection Guide
**Quick reference:**

| Need | Agent | Skill Invoked |
|------|-------|---------------|
| Firmware, drivers | ODrive Engineer | (direct) + qa-assistant for builds |
| Control algorithms | ODrive Engineer | control-algorithms (🚧) |
| Hardware specs | ODrive Engineer | pcb-review (🚧) |
| Tests, validation | ODrive QA | cpp-testing |
| CI/CD, releases | ODrive QA | odrive-ops |
| Python, docs | Regular Copilot | - |

**Legend:** 🚧 = Planned skill

---

## Slide Design Notes

### Visual Style
- **Clean, minimal** - Not too much text
- **Code examples** - Monospace font, syntax highlighting
- **Diagrams** - Simple flowcharts, boxes, arrows
- **Icons** - Use emojis or simple icons for visual interest
- **Consistent colors** - Stick to 3-4 colors throughout

### Fonts
- **Titles:** 44pt, Bold
- **Body:** 24-28pt
- **Code:** 20pt, Monospace (Consolas, Monaco)

### Animation
- **Minimal** - Simple fade-ins, no fancy effects
- **Reveals** - Use for step-by-step processes
- **Avoid** - Transitions, spins, bounces (distracting)

---

## Timing Breakdown

| Slide Range | Topic | Time |
|-------------|-------|------|
| 1-4 | Agentic Developer | 8 min |
| 5-10 | Context Engineering | 12 min |
| 11-15 | Decomposition | 8 min |
| 16-18 | Refinement | 5 min |
| 19-20 | Hands-On | 12 min |
| **Total** | | **45 min** |

---

## Presenter Reminders

- ⏰ **Keep to time** - Use a visible timer
- 🎤 **Speak clearly** - Project your voice
- 👀 **Make eye contact** - Engage the audience
- 🖱️ **Use pointer** - Highlight key parts
- ⏸️ **Pause after questions** - Let them think
- 📱 **Phone on silent** - No interruptions

**Good luck! 🚀**

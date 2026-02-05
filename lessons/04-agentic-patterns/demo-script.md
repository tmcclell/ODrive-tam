# Section 4 Demo Script: Agentic Development with ODrive

**Duration:** 12 minutes (for the "Context Engineering" and "Decomposition" demos)  
**Presenter Notes:** This script provides the exact steps and dialogue for live demos

---

## Demo 1: Context Engineering - Poor vs. Rich Context (5 min)

### Setup
- Open VS Code with ODrive workspace
- Have `Firmware/MotorControl/motor.cpp` visible
- Open Copilot Chat panel

### Demo Flow

**Part A: Poor Context (Show what NOT to do)**

**Presenter Says:**
> "Let's say I want to add overcurrent protection. Watch what happens when I give minimal context."

**Type in Chat:**
```
Add overcurrent protection to the motor
```

**Presenter Says:**
> "Look at the result - it's generic code that doesn't understand our architecture. It doesn't know about:
> - Our error handling system
> - Our interrupt-based control loop
> - Our existing current sensing infrastructure
> - MISRA C++ compliance requirements
> 
> This code won't compile or integrate properly."

---

**Part B: Rich Context (Show best practices)**

**Presenter Says:**
> "Now watch the difference when I provide rich context. I'll use our ODrive-Engineer agent and give it everything it needs."

**Type in Chat:**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Add overcurrent protection to the motor driver.

Context:
- File: src-ODrive/Firmware/MotorControl/motor.cpp
- Current sensing happens in foc.cpp via ADC DMA
- We have phase current measurements: Iu, Iv, Iw
- Motor has DC current limit: motor_.config_.current_lim
- Need to trigger ERROR_CURRENT_LIMIT_VIOLATION
- Must work in interrupt context (current control loop at 8kHz)
- Use existing error handling framework from axis.hpp

Requirements:
- Check current magnitude on every PWM cycle
- Use fast sqrt approximation (arm_sqrt_f32)
- Add 10% hysteresis to prevent chattering
- No heap allocation
- MISRA C++ compliant
- Add to the update() method around line 234

Please show me the code to add to motor.cpp
```

**Presenter Says:**
> "Notice how the agent now produces code that:
> - Uses our actual current measurement variables
> - Integrates with our error system
> - Respects interrupt context constraints
> - Uses our coding standards
> - Fits exactly where it needs to go
>
> The agent can also invoke the `cpp-testing` skill to verify the build!
> 
> **Key Lesson:** The quality of AI output is directly proportional to the quality of your context!"

---

## Demo 2: Decomposition Strategy - Adding CAN Timestamp Feature (7 min)

### Setup
- Open VS Code with ODrive workspace
- Have file explorer visible showing structure
- Open Copilot Chat panel

### Demo Flow

**Presenter Says:**
> "Let's tackle a complex feature: adding 64-bit timestamps to CAN messages. This touches multiple domains - hardware, firmware, and Python tools. Perfect for demonstrating decomposition."

**Step 1: Start with Hardware Specs**

**Presenter Says:**
> "First, I'll consult the ODrive-Engineer agent with a hardware focus about the CAN message format."

**Type in Chat:**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
We need to add 64-bit microsecond timestamps to CAN messages.

Context:
- ODrive uses CAN 2.0B (29-bit extended IDs)
- Current messages are 8 bytes maximum
- We need backwards compatibility with existing tools
- Focus on electrical and protocol constraints

Questions:
1. Should we use CAN-FD for larger payload?
2. Or split timestamp across multiple messages?
3. What's the recommended message structure?
4. Any electrical considerations for higher CAN-FD bitrates?
```

**Presenter Says:**
> "The agent gives us the electrical constraints and message format options. Notice I specified 'electrical and protocol constraints' to guide the agent toward hardware considerations. Let's say it recommends CAN-FD. Now we move to firmware."

---

**Step 2: Firmware Implementation**

**Type in Chat:**

> Select **ODrive Engineer** from agent dropdown, then paste:

```
Based on the hardware spec, implement CAN-FD timestamp support.

Context:
- Files: src-ODrive/Firmware/communication/can.cpp, src-ODrive/Firmware/communication/can.hpp
- STM32 CAN peripheral (bxCAN or FDCAN depending on chip)
- Existing CAN protocol uses 8-byte messages
- Need microsecond timestamp from system timer

Tasks:
1. Add CAN-FD initialization to can.cpp
2. Create timestamp_msg_t structure
3. Implement send_timestamped_message() function
4. Ensure backwards compatibility with non-FD devices

Start with the message structure and function signature.
```

**Presenter Says:**
> "Now the agent provides the firmware implementation. Same agent, different task context. I can review it incrementally."

---

**Step 3: Refine with Specifics**

**Type in Chat:**

> With **ODrive Engineer** still selected, paste:

```
Looking good! A few refinements:

1. Use TIM5 for microsecond counter (it's 32-bit, we need to extend to 64-bit)
2. Add overflow handling for the timer
3. The timestamp should be captured at message *transmission* not creation
4. Add a configuration flag: can_.config_.enable_timestamps

Update the implementation with these requirements.
```

**Presenter Says:**
> "This is iterative refinement - I'm guiding the agent to match our specific hardware and requirements. Same agent, just providing more specific context."

---

**Step 4: Cross-Domain with Python Tools**

**Type in Chat:**
```
Now I need to update the Python tools to parse timestamped CAN messages.

Context:
- File: src-ODrive/tools/odrive/can.py
- Messages now have optional 8-byte timestamp appended (CAN-FD)
- Timestamp format: uint64_t little-endian microseconds
- Need backwards compatibility with 8-byte messages

Task:
Update the parse_can_message() function to:
1. Detect message length (8 bytes = legacy, 16 bytes = timestamped)
2. Extract timestamp if present
3. Add timestamp field to returned message dict
4. Maintain compatibility with existing code

Show me the updated parse function.
```

**Presenter Says:**
> "Notice I switched to regular Copilot because this is Python tooling, not embedded firmware. Use the right tool for the job!"

---

**Step 5: Testing Strategy**

**Type in Chat:**

> Select **ODrive QA** from agent dropdown, then paste:

```
Create a test plan for the CAN timestamp feature.

Context:
- Firmware: src-ODrive/Firmware/communication/can.cpp
- Python: src-ODrive/tools/odrive/can.py
- Feature: 64-bit timestamps on CAN-FD messages
- Backwards compatibility required

Please provide:
1. Unit tests for firmware (what to test?)
2. Integration tests with real hardware
3. Python tool tests
4. Test rig configuration (test-rig-*.yaml)
5. Acceptance criteria
```

**Presenter Says:**
> "The QA agent invokes the `cpp-testing` skill to provide a complete test strategy. Now we have:
> - Hardware specification (ODrive Engineer with hardware focus)
> - Firmware implementation (ODrive Engineer with firmware focus)
> - Python tools update (Regular Copilot)
> - Test plan (ODrive QA)
> 
> All from proper task decomposition and using the right agent/context for each domain!"

---

**Step 6: Summary**

**Presenter Says:**
> "This demonstrates the power of decomposition:
> 
> ✅ **Complex task** broken into domain-specific subtasks  
> ✅ **Same agents, different contexts** - ODrive Engineer handles firmware, hardware, and control via different prompts  
> ✅ **ODrive QA** for testing - invokes specialized skills automatically  
> ✅ **Iterative refinement** to get details right  
> ✅ **End-to-end feature** implemented collaboratively  
> 
> You couldn't do this in one prompt - but with decomposition, you can orchestrate AI agents to build complete features.
>
> **Key insight:** Two agents (**ODrive Engineer** + **ODrive QA**) with skills can handle all domains!"

---

## Demo Tips for Presenters

### Before the Demo
- [ ] Test all prompts with actual ODrive repo
- [ ] Have files pre-opened for quick navigation
- [ ] Clear Chat history for clean demo
- [ ] Verify agents are available (**ODrive Engineer**, **ODrive QA**)
- [ ] Have backup screenshots in case of network issues

### During the Demo
- **Pause after each response** - Let audience read the output
- **Highlight key parts** - Point out how context affected the result
- **Explain your thinking** - "I'm using ODrive Engineer with a hardware focus because..."
- **Show the iteration** - Don't pretend it's perfect first time
- **Acknowledge limitations** - "Sometimes it needs refinement..."

### If Something Goes Wrong
- **Network issues:** Use pre-recorded screenshots
- **Bad AI response:** "This is why review is important!" - show refinement
- **Unexpected error:** Use `/fix` command to demonstrate recovery

### Time Management
- Demo 1: 5 minutes (keep it tight)
- Demo 2: 7 minutes (can extend slightly if needed)
- Have a "speed run" version ready if running behind

---

## Audience Engagement

### Questions to Ask Audience

**After Demo 1:**
> "Who has tried using Copilot and felt the output wasn't quite right? [Show of hands]  
> That's usually a context problem, not an AI problem!"

**After Demo 2:**
> "What other ODrive features would benefit from this multi-agent decomposition approach?"

### Expected Questions from Audience

**Q: "Does using agents slow down development?"**
A: "Initially, yes - you're learning. But once proficient, you'll move faster than writing manually. Think of it like learning Git - upfront cost, long-term gain."

**Q: "What if I don't have custom agents?"**
A: "You can still use decomposition with regular Copilot. Our ODrive setup uses two orchestrator agents (**ODrive Engineer** and **ODrive QA**) that invoke skills. I'll show you how to create agents in the workshop materials."

**Q: "Why only 2 agents instead of 4 domain-specific ones?"**
A: "Orchestrator agents are more flexible. **ODrive Engineer** can handle firmware, control, or hardware based on your prompt context. Skills provide the specialization. This simplifies agent management while maintaining expertise."

**Q: "Can agents access proprietary internal documentation?"**
A: "Agents work with what's in your workspace. For proprietary docs, put them in `.github/` folders or reference them in copilot-instructions.md."

**Q: "What about security - is my code sent to the cloud?"**
A: "Yes, GitHub Copilot uses cloud models. For air-gapped environments, see Section 9 on Foundry Local. Otherwise, follow your company's Copilot data governance policies."

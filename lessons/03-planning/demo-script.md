# Section 3 Demo Script: Planning & Steering Documents

**Duration:** 60 minutes total (instructor demos ~40 min, hands-on ~20 min)  
**Presenter Notes:** This script provides exact steps and dialogue for live demos

---

## Demo 1: File-Type-Specific Custom Instructions (10 min)

### Setup
- Open VS Code with ODrive workspace
- Ensure `.github/instructions/` folder contains your instruction files
- Open Copilot Chat panel

### Demo Flow

**Presenter Says:**
> "Let me show you how Copilot automatically applies different coding standards based on file type. We have three instruction files set up:
> - `cpp_coding_standards.instructions.md` → for `.cpp` files
> - `header_file_rules.instructions.md` → for `.hpp`/`.h` files  
> - `python_coding_standards.instructions.md` → for `.py` files
>
> Watch how the same concept produces different code styles depending on the file type requested."

---

### Demo 1.1: Header File (.hpp) → Header Rules

**Type in Chat:**
```
Create a header file for a motor controller class
```

**Expected Output - Copilot follows `header_file_rules.instructions.md`:**

**Presenter Says:**
> "Notice these patterns from our header rules:
> - ✅ Uses `#pragma once` (not old-style `#ifndef` guards)
> - ✅ Forward declarations instead of heavy includes
> - ✅ No `using namespace` in header
> - ✅ Trivial getters are inline
> - ✅ Complex methods declared only (defined in .cpp)"

---

### Demo 1.2: C++ Source File (.cpp) → C++ Coding Standards

**Type in Chat:**
```
Add a method to #file:Firmware/Drivers/STM32/stm32_gpio.cpp that checks if the GPIO pin is currently high.

The method should:
- Return a boolean
- Be named following our conventions
- Include brief documentation
```

✅ **Check References panel** → `cpp_coding_standards.instructions.md` should appear

**Expected Output - Copilot follows `cpp_coding_standards.instructions.md`:**

**Presenter Says:**
> "Watch for our C++ standards:
> - ✅ `PascalCase` for class names (`EncoderManager`)
> - ✅ `camelCase` for methods (`getPosition()`, `updateReadings()`)
> - ✅ `is`/`has` prefixes for booleans (`isCalibrated()`, `hasError()`)
> - ✅ Trailing underscore for private members (`position_`, `velocity_`)
> - ✅ `kPascalCase` for constants (`kMaxPosition`, `kDefaultTimeout`)
> - ✅ `enum class` with PascalCase values
>
> And notice in the References panel - our instruction file is loaded!"

---

### Demo 1.3: Python File (.py) → Python Coding Standards

**Type in Chat:**
```
Create a Python script to log motor temperature
```

**Expected Output - Copilot follows `python_coding_standards.instructions.md`:**

**Presenter Says:**
> "Now Python standards are applied:
> - ✅ `snake_case` for functions (`read_temperature()`, `log_to_file()`)
> - ✅ `PascalCase` for classes (`TemperatureLogger`)
> - ✅ `UPPER_CASE` for constants (`MAX_TEMPERATURE`, `LOG_INTERVAL`)
> - ✅ Type hints on all parameters and returns
> - ✅ Google-style docstrings with `Args:`, `Returns:`, `Raises:`
> - ✅ `_leading_underscore` for private members"

---

### Summary: How File Type Triggers Instructions

**Show this table:**

| File Extension | Instruction File Triggered | Key Rules Applied |
|----------------|---------------------------|------------------|
| `*.hpp`, `*.h` | `header_file_rules.instructions.md` | `#pragma once`, forward decls, no `using namespace` |
| `*.cpp`, `*.c` | `cpp_coding_standards.instructions.md` | PascalCase classes, camelCase methods, `kConstants` |
| `*.py` | `python_coding_standards.instructions.md` | snake_case, type hints, Google docstrings |

**Presenter Says:**
> "The `applyTo` glob pattern in each instruction file controls this:
> ```yaml
> applyTo: '**/*.{h,hpp}'    # Header rules
> applyTo: '**/*.{cpp,c}'    # C++ rules  
> applyTo: '**/*.py'         # Python rules
> ```
> This is automatic - you don't need to specify which rules to use!"

---

### Additional Prompts to Try

> 💡 **Tip:** Using `#file:` syntax lets you verify which instructions are applied by checking the References panel!

**C++ with File Reference:**
```
Add a method to enable the NVIC interrupt for a GPIO pin in #file:Firmware/Drivers/STM32/stm32_gpio.cpp

The method should:
- Take priority and sub-priority as parameters
- Use the get_irq_number() helper function
- Follow the existing code patterns
```
✅ **Check References panel** → `cpp_coding_standards.instructions.md` should appear

**Python with File Reference:**
```
Add a function to #file:docs/conf.py that validates all fibre interface files exist before building documentation.

The function should:
- Check each path in fibre_interface_files list
- Log warnings for missing files
- Return True if all files exist, False otherwise
```
✅ **Check References panel** → `python_coding_standards.instructions.md` should appear

**Header with File Reference:**
```
Add a method declaration to #file:Firmware/Drivers/STM32/stm32_gpio.hpp for configuring alternate function mode.

The method should:
- Take the alternate function number (0-15) as parameter
- Return bool for success/failure
- Be consistent with existing config() method
```
✅ **Check References panel** → `header_file_rules.instructions.md` should appear

**Presenter Says:**
> "Using explicit `#file:` references has two benefits:
> 1. Copilot understands the existing code patterns in that file
> 2. You can verify which instructions are being applied by checking References
>
> This is great for debugging when code doesn't follow your standards - check if the right instructions are being loaded!"

---

## Demo 2: Prompt Files (.prompt.md) (8 min)

### Setup
- Create `.github/prompts/` directory
- Have Chat panel visible

### Demo Flow

### Part A: Create a Reusable Prompt (4 min)

**Presenter Says:**
> "Instructions are always-on. Prompt files are reusable templates you invoke when needed."

**Create file:** `.github/prompts/add-doxygen.prompt.md`

````markdown
---
mode: edit
description: Add Doxygen documentation to selected code
---

# Add Doxygen Documentation

Add comprehensive Doxygen documentation to the selected code following these rules:

## Required Elements
- `@brief` - One-line summary
- `@param` - For each parameter with [in], [out], or [in,out]
- `@return` - Return value description
- `@throws` - Any exceptions (if applicable)
- `@note` - Important usage notes
- `@warning` - Safety-critical information
- `@see` - Related functions

## Style
- Use `/** */` block comments
- Keep `@brief` under 80 characters
- Add blank line between sections
- Include units for physical quantities (e.g., "velocity in rad/s")

## Example Format
```cpp
/**
 * @brief Calculate motor phase currents using Clarke transform.
 * 
 * @param[in] i_a Phase A current in Amperes
 * @param[in] i_b Phase B current in Amperes
 * @param[out] i_alpha Alpha component of current
 * @param[out] i_beta Beta component of current
 * 
 * @return true if calculation succeeded
 * 
 * @note Assumes balanced three-phase system
 * @see inverse_clarke_transform()
 */
```

Apply this documentation style to the selected code.
````

**Presenter Says:**
> "Now I can use this prompt anytime with a slash command."

### Part B: Use the Prompt (2 min)

**Open a file with an undocumented function**

**Select the function**

**In Chat, type:**
```
/add-doxygen
```

**Presenter Says:**
> "See how it appears in the slash command menu? The prompt file becomes a reusable command. Let's see the output."

**Show the generated documentation:**

> "It followed all our rules:
> - Correct format
> - All required elements
> - Project-specific style
>
> One prompt file, consistent documentation across the team!"

### Part C: Prompt with Variables (2 min)

**Create file:** `.github/prompts/generate-test.prompt.md`

````markdown
---
mode: edit
description: Generate unit tests for selected code
---

# Generate Unit Tests

Generate unit tests for the selected code using the **${input:framework}** testing framework.

## Test Requirements
- Test happy path with normal inputs
- Test edge cases (zero, max, min values)
- Test error conditions
- Use descriptive test names: `test_<function>_<scenario>_<expected>`

## Framework-Specific Patterns

### If doctest:
```cpp
TEST_CASE("function_name") {
    SUBCASE("happy path") { ... }
    SUBCASE("edge case") { ... }
}
```

### If gtest:
```cpp
TEST(TestSuite, FunctionName_Scenario_Expected) {
    EXPECT_EQ(expected, actual);
}
```

Generate comprehensive tests for the selected code.
````

**Presenter Says:**
> "See `${input:framework}`? This creates an input variable. When I use the prompt, Copilot asks for the value."

**Demo:**
```
/generate-test
```

**Type in the input box:** `doctest`

**Presenter Says:**
> "The prompt adapts based on my input. This single prompt handles multiple testing frameworks!"

---

## Demo 3: Custom Agents (.agent.md) (10 min)

### Setup
- Create `.github/agents/` directory
- Have Chat panel visible

### Demo Flow

### Part A: Create a Specialized Agent (5 min)

**Presenter Says:**
> "Custom agents are specialized personas with specific expertise and tools. Let me create one."

**Create file:** `.github/agents/firmware-engineer.agent.md`

````markdown
---
name: 'Firmware Engineer'
description: 'Embedded C++ developer for motor control firmware'
tools: ['codebase', 'file', 'terminal']
---

# Firmware Engineer

You are an embedded systems engineer specializing in motor control firmware.

## Your Expertise
- STM32 microcontrollers (F4/H7 series)
- Field-Oriented Control (FOC) algorithms
- Real-time systems and FreeRTOS
- Communication protocols (USB, CAN, UART, SPI)

## Coding Standards

Follow these conventions:
- **Classes**: PascalCase (e.g., `MotorController`)
- **Methods**: camelCase (e.g., `getPosition()`)
- **Constants**: kPascalCase (e.g., `kMaxVoltage`)
- **Private members**: trailing underscore (e.g., `position_`)

## Embedded Constraints
- No dynamic memory allocation
- No exceptions (use error codes)
- Use volatile for hardware registers
- Keep ISRs short

## Output Format

When reviewing code:
1. **Issue**: Description of the problem
2. **Location**: File and line number  
3. **Fix**: Suggested solution
4. **Why**: Explanation of the risk
````

**Presenter Says:**
> "This agent has:
> - A specific **persona** (firmware engineer)
> - Defined **expertise** areas
> - **Coding standards** to follow
> - An **output format** for consistency
>
> Let's try it!"

### Part B: Use the Agent (3 min)

**Open a code file**

**In Chat, select the agent dropdown → Choose "Firmware Engineer"**

**Or type:**
```
@firmware-engineer Review #file:Firmware/Drivers/STM32/stm32_gpio.cpp

Check for:
- Proper volatile usage
- Interrupt safety  
- Naming convention compliance
```

**Presenter Says:**
> "Watch how the agent:
> - Uses its embedded expertise
> - Applies coding standards
> - Formats output consistently
>
> Every team member gets the same quality review!"

**Show the structured output:**

> "Notice the consistent format:
> - Clear issue descriptions
> - Specific file locations
> - Actionable fixes
> - Risk explanations
>
> This is institutional knowledge, available to everyone!"

### Part C: Agent with Tool Access (2 min)

**Presenter Says:**
> "Notice the `tools` in the frontmatter? This controls what the agent can access."

**Show the frontmatter:**
```yaml
---
tools: ['codebase', 'file', 'terminal']
---
```

**Presenter Says:**
> "This agent can:
> - `codebase` - Search the entire workspace
> - `file` - Read specific files
> - `terminal` - Run commands
>
> You can restrict agents to only the tools they need. A documentation agent might only need `file`, while a build agent needs `terminal`."

---

## Demo 4: Agent Skills (SKILL.md) (8 min)

### Setup
- Create `.github/skills/` directory
- Have sample skill folder ready

### Demo Flow

### Part A: Skill Structure (3 min)

**Presenter Says:**
> "Skills are the most powerful customization. They bundle instructions with resources - scripts, templates, references."

**Show folder structure:**
```
.github/skills/
└── build-toolchain/
    ├── SKILL.md              # Main skill definition
    ├── scripts/
    │   ├── build_firmware.py  # Build script
    │   └── search_symbol.py   # Symbol search
    └── configs/
        └── board-configs.md   # Board configuration reference
```

**Presenter Says:**
> "A skill is a folder with:
> - `SKILL.md` - The main instructions
> - `scripts/` - Tools the AI can run
> - `configs/` - Reference documentation
>
> The AI can read these files and use the scripts!"

### Part B: Create a Skill (3 min)

**Create folder:** `.github/skills/build-toolchain/`

**Create file:** `.github/skills/build-toolchain/SKILL.md`

````markdown
---
name: 'Build Toolchain'
description: 'Build firmware and run tests for embedded projects'
status: production
---

# Build Toolchain Skill

Build, test, and inspect firmware projects.

## Capabilities

| Command | Purpose |
|---------|---------|  
| Build firmware | Compile for target board |
| Run tests | Execute unit test suite |
| Search symbols | Find functions and classes |
| List configs | Show available board configurations |

## Usage Examples

### Build Firmware
```powershell
make CONFIG=board-v3.6-56V
make clean
make --list-configs
```

### Run Tests
```powershell
python tools/run_tests.py
python tools/run_tests.py --filter encoder
```

### Search Symbols
```powershell
grep -r "class Encoder" Firmware/
grep -r "ERROR_" Firmware/
```

## Safety

✅ **Safe**: Build and test operations are local  
⚠️ **Confirm first**: Flash firmware to hardware
````

### Part C: Use the Skill (2 min)

**Presenter Says:**
> "Skills are auto-discovered based on your prompt. Watch this."

**Type in Chat:**
```
Build the firmware for board-v3.6-56V.

What command should I run?
```

**Presenter Says:**
> "Notice I didn't explicitly call the skill. Copilot matched my request to the skill description and loaded it automatically. The skill's commands, examples, and safety warnings are all available."

**Show the output:**

> "The response includes:
> - The correct build command
> - Available configuration options
> - Safety warnings about flashing
>
> This is institutional knowledge packaged for AI consumption!"

---

## Demo 5: Plan Mode (8 min)

### Setup
- Open Copilot Chat on GitHub.com (Plan Mode works best there)
- Or use VS Code with latest Copilot

### Demo Flow

### Part A: Create Project Plan (4 min)

**Presenter Says:**
> "Plan Mode turns your ideas into structured GitHub Issues. Perfect for project kickoffs and complex features."

**Type in Chat:**
```
I'm planning to add CAN-FD support to the ODrive firmware. The feature should:
- Support higher data rates (up to 8 Mbps)
- Maintain backwards compatibility with CAN 2.0
- Include timestamp support for messages
- Update Python tools to parse CAN-FD frames

Please create a structured project plan with epics, features, and tasks.
```

**Presenter Says:**
> "Watch how Copilot generates a full project structure."

**Wait for response:**

> "It created:
> - **Epic**: CAN-FD Support
> - **Features**: Hardware config, Protocol handling, Tool updates
> - **Tasks**: Specific implementation items
>
> Each with descriptions and acceptance criteria!"

### Part B: Refine the Plan (2 min)

**Presenter Says:**
> "The first draft is rarely perfect. Let's refine it."

**Type in Chat:**
```
Can you break down the "Feature: Hardware Configuration" into smaller tasks? 
Include STM32 FDCAN peripheral setup, timing configuration, and interrupt handling.
```

**Show refined output:**

> "Now we have actionable tasks:
> - Task: Configure FDCAN clock source
> - Task: Set bit timing parameters
> - Task: Implement TX/RX interrupt handlers
>
> Each task is small enough to assign to a developer."

### Part C: Improve Descriptions (2 min)

**Type in Chat:**
```
Improve the description for "Task: Configure FDCAN clock source" with:
- Technical details for STM32H7
- Code snippets for initialization
- Acceptance criteria
- Links to reference documentation
```

**Presenter Says:**
> "Now the task has everything a developer needs to get started. You can iterate like this for each task, then create the issues in your repo with one click."

**Show the Plan Mode workflow diagram:**
```
Idea → Plan Mode → Refine → Create Issues → Agent Mode → Implementation
```

> "Plan Mode creates the structure, Agent Mode executes the tasks. They work together!"

---

## Demo 6: Spec-Driven Development with SpecKit (10 min)

### Setup
- Have terminal open in VS Code
- Ensure Python/UV is installed (for specify CLI)
- Create a demo folder or use existing project

### Demo Flow

### Part A: Understanding Vibe Coding vs. Spec-Driven (2 min)

**Presenter Says:**
> "Before we dive into SpecKit, let me explain WHY we need it. Many of us start with what's called 'vibe coding' - you have an idea, type a few sentences, and let the AI figure it out."

**Show this comparison:**

| Vibe Coding | Spec-Driven |
|-------------|-------------|
| "Build me a website" | Constitution → Spec → Plan → Tasks |
| AI chooses frameworks | You define the stack |
| Inconsistent patterns | Enforced conventions |
| Hard to maintain | Documented decisions |

**Presenter Says:**
> "Vibe coding is great for prototypes, but production software needs guardrails. SpecKit provides those guardrails - it's essentially formalizing context for AI models so they make the decisions YOU want."

### Part B: Install and Initialize SpecKit (2 min)

**Presenter Says:**
> "SpecKit is just prompts and scripts - no magic. Let me show you how to set it up."

**In Terminal:**
```powershell
# Install the specify CLI (using UV - recommended)
uv tool install spec-kit

# Initialize in a new project folder
mkdir podcast-demo
cd podcast-demo
specify init .
```

**Presenter Says:**
> "The CLI asks which agent to use - Copilot, Claude, Gemini, etc. Select 'copilot' and your shell type."

**Show the folder structure created:**
```
.github/
├── .speckit/
│   ├── agents/
│   │   └── specify.agent.md
│   ├── prompts/
│   │   ├── constitution.prompt.md
│   │   ├── spec.prompt.md
│   │   ├── clarify.prompt.md
│   │   └── plan.prompt.md
│   └── scripts/
└── vscode/
```

**Presenter Says:**
> "It's just markdown files and scripts. You could manually download these from the GitHub releases - no installation required!"

### Part C: Create a Constitution (2 min)

**Presenter Says:**
> "The constitution is your project's non-negotiable principles. These apply to EVERYTHING."

**In Copilot Chat - notice the suggested action "Constitution":**

**Click it or type:**
```
Create a constitution for a static podcast website with minimal dependencies, 
always deploy to Azure, use Hugo for static generation, and Tailwind CSS for styling.
```

**Show the generated constitution.md:**

**Presenter Says:**
> "Look what it created:
> - **Minimal dependencies** - it's tracking this!
> - **Static-first architecture** - from my prompt
> - **Azure deployment** - non-negotiable
> - **Performance standards** - Lighthouse 90+
>
> This is now enforced on everything that follows. It's like `copilot-instructions.md` but specifically for this project's architecture."

### Part D: Build Specification & Clarify (2 min)

**Presenter Says:**
> "Now let's create the functional spec. Notice the handoff suggestion at the bottom of the chat."

**Click "Build Specification" or type:**
```
Build a specification for a podcast website with featured episodes and guest profiles
```

**Wait for spec to generate, then:**

**Presenter Says:**
> "Good start, but here's my favorite part - Clarify mode. The AI asks ME questions I didn't think to ask!"

**Click "Clarify Spec Requirements":**

**Show the AI asking questions:**
```
Q1: How will episode data be managed?
    A) Admin CMS  B) Static JSON  C) RSS Import

Q2: Expected episode catalog size?
    A) 10-50  B) 50-100  C) 100-200  D) 200+

Q3: Audio hosting provider?
    A) Self-hosted  B) Libsyn  C) Transistor
```

**Answer with single letters:**
```
B
D
A
```

**Presenter Says:**
> "Each answer updates the spec! It's now recording these clarifications and adding them to the specification. This is incredibly powerful - the AI is helping me think through things I would have forgotten."

### Part E: Technical Plan & Tasks (2 min)

**Presenter Says:**
> "Now we add the technical details."

**Click "Build Technical Plan" or type:**
```
Build technical plan using Hugo and Tailwind CSS for styling
```

**Wait for plan to generate, show the artifacts:**

**Presenter Says:**
> "Look at what it created:
> - **plan.md** - Architecture, dependencies, folder structure
> - **data-model.md** - Episode entity with fields
> - **contracts.md** - API definitions
> - **research.md** - How to integrate Tailwind with Hugo
>
> All this context is now available for implementation!"

**Show the folder structure:**
```
specs/
└── 001-podcast-website/
    ├── spec.md           # Functional requirements
    ├── plan.md           # Technical architecture
    ├── data-model.md     # Entity definitions
    ├── tasks.md          # Implementation steps
    └── research.md       # Technical research
```

**Click "Create Tasks":**

**Presenter Says:**
> "Finally, we break it into implementable tasks. Notice it identifies MVP tasks separately!"

**Click "Implement" to start execution:**

> "Now it follows the plan to implement each task. The spec, plan, and constitution guide every decision. THIS is spec-driven development!"

### Part F: Existing Projects & Multiple Variations (Optional - if time)

**Presenter Says:**
> "What about existing projects? Point SpecKit at your codebase:"

```
I have an existing React app. Analyze the project and create a constitution 
based on the current patterns and dependencies.
```

> "For new features, create `specs/002-new-feature/` and go through the workflow. 
> 
> Even cooler - you can fork a spec into multiple implementations. Same spec, 
> different technical plans: one with Hugo, one with Next.js. Compare the outcomes!"

---

## Hands-On Exercises (20 min)

**Presenter Says:**
> "Your turn! Open the [hands-on-exercises.md](hands-on-exercises.md) file and work through the exercises."

📄 **Direct participants to:** [hands-on-exercises.md](hands-on-exercises.md)

### Exercise Overview (20 min total)

| Exercise | Focus | Time |
|----------|-------|------|
| 1. Custom Instructions | Create coding standards + test with `#file:` | 5 min |
| 2. Prompt Files | Create reusable prompts + test with `/command` | 5 min |
| 3. Custom Agents | Create specialized persona + test with real code | 5 min |
| 4. Agent Skills | Create skill folder + test auto-discovery | 5 min |

**Presenter Tips:**
- Walk around and help participants who get stuck
- Point out the `applyTo` pattern in instruction files
- Remind them to check the References panel
- Exercises 1-2 are most important if short on time

---

## Demo Tips for Presenters

### Before the Demo
- [ ] Clear/rename existing `.github/` customizations
- [ ] Have template files ready to copy
- [ ] Test that skills auto-discovery works
- [ ] Verify Plan Mode is accessible

### During the Demo
- **Show the file structure** - People need to see where files go
- **Highlight the YAML frontmatter** - Often overlooked
- **Compare before/after** - The difference is dramatic
- **Encourage questions** - "What standards does your team use?"

### Common Issues & Recovery
- **Instructions not loading:** Check file path and YAML syntax
- **Agent not appearing:** Restart VS Code or refresh agents list
- **Skill not discovered:** Ensure SKILL.md exists and description matches

### Time Management
- Demo 1 (File-Type Instructions): 8 min
- Demo 2 (Prompts): 6 min
- Demo 3 (Agents): 8 min
- Demo 4 (Skills): 6 min
- Demo 5 (Plan Mode): 6 min
- Demo 6 (SpecKit): 6 min
- **Hands-On Exercises**: 20 min

**Total: 60 min** - Skip Demo 4 (Skills) or Demo 6 (SpecKit) if running short

---

## Audience Engagement

### Questions to Ask

**After Demo 1:**
> "Notice how the same concept - temperature reading - looks completely different in C++ vs Python?
> The file type automatically triggers the right coding standards!"

**After Demo 2:**
> "What repetitive prompts do you use frequently? [discuss]
> Those are perfect candidates for prompt files!"

**After Demo 4:**
> "What specialized roles exist on your team? [discuss]
> Reviewers, architects, security experts - each could be an agent!"

### Expected Questions

**Q: "Do instruction files slow down Copilot?"**
A: "No noticeable impact. They're loaded with your prompt and add minimal overhead."

**Q: "Can I share instructions across repos?"**
A: "Yes! You can create a shared repo of instructions and reference them. Skills can also be installed globally in `~/.copilot/skills/`."

**Q: "What's the priority when multiple instructions conflict?"**
A: "File-specific instructions (via `applyTo`) take precedence over global. More specific patterns win."

**Q: "Can I disable instructions temporarily?"**
A: "Rename the file or move it out of `.github/`. There's no toggle yet."

**Q: "How do skills differ from agents?"**
A: "Agents are personas you select. Skills are auto-discovered based on your prompt. Agents can invoke skills, but you invoke agents directly."

---

## File Location Quick Reference

| Type | Location | When Loaded |
|------|----------|-------------|
| Global Instructions | `.github/copilot-instructions.md` | Always |
| Conditional Instructions | `.github/instructions/*.instructions.md` | When `applyTo` matches |
| Prompt Files | `.github/prompts/*.prompt.md` | When you type `/name` |
| Agents | `.github/agents/*.agent.md` | When you select from dropdown |
| Skills | `.github/skills/*/SKILL.md` | Auto-discovered by prompt |

---

**Demo Script Version:** 1.0  
**Last Updated:** January 2026

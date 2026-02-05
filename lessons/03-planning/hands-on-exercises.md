# Lesson 3: Hands-On Exercises

**Duration:** 22 minutes  
**Format:** Individual practice  
**Goal:** Practice creating custom instructions, prompt files, agents, and skills

---

## Overview

This hands-on session lets you practice creating and testing GitHub Copilot customizations. Complete as many exercises as time allows.

| Exercise | Focus | Time |
|----------|-------|------|
| 0. Clean Slate | Delete existing customization files | 2 min |
| 1. Custom Instructions | Create coding standards + test with file references | 5 min |
| 2. Prompt Files | Create reusable prompts + test with `/command` | 5 min |
| 3. Custom Agents | Create specialized persona + test with real code | 5 min |
| 4. Agent Skills | Create skill folder + test auto-discovery | 5 min |

> 💡 **Tip:** Focus on Exercises 1-2 if short on time. They provide the most immediate value.

---

## Exercise 0: Clean Slate Setup (2 min)

### ⚠️ Important: Delete Existing Customization Files

Before starting, delete the existing customization files so you can create them fresh from scratch.

### Step 1: Delete Instructions Files

In VS Code Explorer, navigate to `.github/instructions/` and delete these files:
- `cpp_coding_standards.instructions.md`
- `header_file_rules.instructions.md`  
- `python_coding_standards.instructions.md`

**Or run in terminal:**
```powershell
Remove-Item -Path ".github/instructions/*.instructions.md" -Force
```

### Step 2: Delete Prompt Files

Navigate to `.github/prompts/` and delete all `.prompt.md` files:

**Or run in terminal:**
```powershell
Remove-Item -Path ".github/prompts/*.prompt.md" -Force
```

### Step 3: Delete Agent Files

Navigate to `.github/agents/` and delete all `.agent.md` files:

**Or run in terminal:**
```powershell
Remove-Item -Path ".github/agents/*.agent.md" -Force
```

### Step 4: Delete Skills Folders

Navigate to `.github/skills/` and delete all skill folders:

**Or run in terminal:**
```powershell
Remove-Item -Path ".github/skills/*" -Recurse -Force
```

### Step 5: Keep the Global Instructions (Optional)

You can optionally keep or delete `.github/copilot-instructions.md`:
```powershell
# To delete:
Remove-Item -Path ".github/copilot-instructions.md" -Force
```

### ✅ Verification

After cleanup, your `.github/` folder should look like:
```
.github/
├── instructions/     # Empty folder
├── prompts/          # Empty folder  
├── agents/           # Empty folder
├── skills/           # Empty folder
└── (copilot-instructions.md - optional)
```

> 💡 **Tip:** Reload VS Code window (Ctrl+Shift+P → "Reload Window") to ensure Copilot picks up the changes.

---

## Exercise 1: Create Custom Instructions (5 min)

### Task: Create a C++ coding standards file

> 💡 **Quick Creation:** In Chat view, click **Configure Chat (⚙️)** → **Chat Instructions** → **New instruction file**

1. Create `.github/instructions/cpp_coding_standards.instructions.md` with:

```markdown
---
name: C++ Coding Standards
description: C++ coding standards for embedded firmware
applyTo: '**/*.{cpp,c,cc}'
---
```

> 📝 **Glob Pattern Tips:**
> - `**/*.cpp` → All `.cpp` files recursively
> - `Firmware/**/*.cpp` → Only `.cpp` files under `Firmware/`
> - `**/*.{cpp,c,cc}` → Multiple extensions with braces
> - Separate multiple patterns with commas: `'**/*.ts,**/*.tsx'`

**Full file content:**

```markdown
---
name: C++ Coding Standards
description: C++ coding standards for embedded firmware
applyTo: '**/*.{cpp,c,cc}'
---

# C++ Coding Standards

## Naming Conventions
- Classes: PascalCase (e.g., `MotorController`)
- Methods: camelCase (e.g., `getPosition()`)
- Variables: camelCase (e.g., `motorSpeed`)
- Constants: kPascalCase (e.g., `kMaxVoltage`)
- Private members: trailing underscore (e.g., `position_`)
- Booleans: is/has prefix (e.g., `isRunning()`, `hasError()`)

## Embedded Constraints
- No dynamic memory allocation (no malloc/new)
- No exceptions (use error codes)
- Use volatile for hardware registers
- Static allocation only

## Modern C++ (C++17)
- Use auto for complex types
- Use constexpr for compile-time constants
- Use enum class (not plain enum)
- Use [[nodiscard]] for important return values

## Documentation
- Use Doxygen-style comments (@brief, @param, @return)
- Document all public APIs
- Include units for physical quantities
```

### 🧪 Test It: Try This Prompt

**Test 1: Add a Method to Existing File**
```
Add a method to #file:Firmware/Drivers/STM32/stm32_gpio.cpp that checks if the GPIO pin is currently high.

The method should:
- Return a boolean
- Be named following our conventions
- Include Doxygen documentation
```

✅ **Check References panel** → `cpp_coding_standards.instructions.md` should appear

**Test 2: Create a New Class**
```
Create a C++ class to manage motor temperature readings.

The class should:
- Store current and maximum temperature
- Provide methods to update and query temperature
- Include safety threshold checking
```

**What to observe:**
- ✅ Class uses `PascalCase` (e.g., `TemperatureManager`)
- ✅ Methods use `camelCase` (e.g., `getCurrentTemperature()`)
- ✅ Private members have trailing `_` (e.g., `currentTemp_`)
- ✅ Constants use `kPascalCase` (e.g., `kMaxSafeTemperature`)
- ✅ Booleans use `is`/`has` prefix (e.g., `isOverheated()`)

### Success Criteria
- ✅ Instructions file created with `applyTo` frontmatter
- ✅ Generated code follows your naming conventions
- ✅ No dynamic allocation in generated code

---

## Exercise 2: Create a Prompt File (5 min)

### Task: Create a reusable documentation prompt

> 💡 **Quick Creation:** In Chat view, click **Configure Chat (⚙️)** → **Prompt Files** → **New prompt file**

1. Create `.github/prompts/add-doxygen.prompt.md`:

```markdown
---
name: add-doxygen
description: Add Doxygen documentation to selected code
mode: edit
---

# Add Doxygen Documentation

Add comprehensive Doxygen documentation to the selected code:

## Required Elements
- `@brief` - One-line summary
- `@param` - For each parameter with [in], [out], or [in,out]
- `@return` - Return value description
- `@note` - Important usage notes

## Style
- Use `/** */` block comments
- Keep `@brief` under 80 characters
- Include units for physical quantities (e.g., "velocity in rad/s")
```

### 🧪 Test It: Use Your Prompt

**Test 1: Add Documentation**
1. Open `Firmware/Drivers/STM32/stm32_gpio.cpp`
2. Select the `config()` function
3. Type in Chat: `/add-doxygen`
4. Verify documentation follows your rules

**Test 2: With File Context**
```
/add-doxygen #file:Firmware/Drivers/STM32/stm32_gpio.cpp
```

### Success Criteria
- ✅ Prompt appears in slash command menu
- ✅ Documentation follows defined format
- ✅ Works with file context

---

## Exercise 3: Create a Custom Agent (5 min)

### Task: Create a code reviewer agent

> 💡 **Quick Creation:** In agents dropdown, click **Configure Custom Agents** → **Create new custom agent**

1. Create `.github/agents/code-reviewer.agent.md`:

````markdown
---
name: Code Reviewer
description: Reviews code for quality, readability, and best practices
tools: ['search', 'fetch', 'usages']
---

# Code Reviewer

You are a senior software engineer who reviews code for quality and best practices.

## Review Focus

### Code Quality
- Clear, descriptive names
- Functions do one thing
- No magic numbers
- Proper error handling

### Readability
- Consistent formatting
- Helpful comments (why, not what)
- Logical organization

### Best Practices
- DRY (Don't Repeat Yourself)
- SOLID principles
- Appropriate abstractions

## Output Format

For each issue found:
- **Category**: Quality / Readability / Best Practice
- **Location**: File and line
- **Issue**: What's wrong
- **Suggestion**: How to improve

End with: ✅ Approved, ⚠️ Needs Changes, or ❌ Requires Rework
````

### 🧪 Test It: Use the Agent

**Test 1: Select Agent and Review**
1. Select "Code Reviewer" from agents dropdown
2. Type:
```
Review this code for quality and best practices:

#file:Firmware/Drivers/STM32/stm32_gpio.cpp
```

**Test 2: Quick Code Review**
```
@code-reviewer What improvements would you suggest for the config() method in #file:Firmware/Drivers/STM32/stm32_gpio.cpp?
```

### Success Criteria
- ✅ Agent appears in dropdown
- ✅ Reviews follow the defined format
- ✅ Provides actionable suggestions

---

## Exercise 4: Create an Agent Skill (5 min)

### Task: Create a documentation standards skill

> 📝 **Note:** Skills are created manually as folder structures (no gear icon).

1. Create folder: `.github/skills/doc-standards/`

2. Create `.github/skills/doc-standards/SKILL.md`:

````markdown
---
name: doc-standards
description: Enforce consistent documentation across the codebase
---

# Documentation Standards Skill

Ensure all code follows consistent documentation practices.

## Required Documentation

### For Functions
- Brief description (one line)
- Parameters with types and purpose
- Return value description
- Example usage (for complex functions)

### For Classes
- Purpose and responsibility
- Key methods overview
- Usage example

### For Files
- File header with purpose
- Author and date (optional)
- Dependencies and requirements

## Documentation Style

**C++ (Doxygen):**
```cpp
/**
 * @brief One-line description.
 * @param name Description of parameter
 * @return What the function returns
 */
```

**Python (Google-style):**
```python
def function(arg):
    """One-line description.
    
    Args:
        arg: Description of argument
        
    Returns:
        What the function returns
    """
```

## Common Issues
- Missing parameter descriptions
- Outdated documentation
- No examples for complex APIs
````

### 🧪 Test It: Trigger the Skill

**Test: Documentation Review (Skill Auto-Discovery)**
```
Review the documentation in #file:Firmware/Drivers/STM32/stm32_gpio.cpp

Check if it follows documentation standards and suggest improvements.
```

**Test: Generate Documentation**
```
Add proper documentation to the config() method following our documentation standards.

#file:Firmware/Drivers/STM32/stm32_gpio.cpp
```

✅ **Check:** References section shows skill was loaded

### Success Criteria
- ✅ Skill folder created with SKILL.md
- ✅ Skill auto-loads when "documentation" mentioned
- ✅ Provides style-specific suggestions

---

## 📊 Summary: What You've Practiced

| Exercise | What You Created | How You Tested |
|----------|-----------------|----------------|
| 1. Instructions | C++ coding standards file | Created class, checked naming conventions |
| 2. Prompt Files | Doxygen documentation template | `/add-doxygen` with file context |
| 3. Custom Agents | Code Reviewer persona | Agent + `#file:` for code review |
| 4. Agent Skills | Documentation Standards skill | Keyword triggers skill loading |

**Key Takeaway:** Always check the **References panel** to verify your customizations are being applied!

---

## 🔍 Troubleshooting

| Issue | Solution |
|-------|----------|
| Instructions not loading | Enable `github.copilot.chat.codeGeneration.useInstructionFiles` setting |
| Prompt not in menu | Verify file is in `.github/prompts/` with `.prompt.md` extension |
| Agent not in dropdown | Check file is in `.github/agents/` with `.agent.md` extension |
| Skill not auto-loading | Enable `chat.useAgentSkills` setting (preview feature) |
| Wrong instruction file | Check `applyTo` glob pattern matches file type |

---

## Bonus Challenges (If Time Permits)

### Challenge 1: Prompt with Variables
Create a prompt that accepts user input:
```markdown
---
description: Generate tests for ${input:framework} framework
---
Generate tests using **${input:framework}**.
```

### Challenge 2: Agent with Handoffs
Add a handoff button to your agent:
```yaml
handoffs:
  - label: "Fix Issues"
    agent: edit
    prompt: "Fix the issues identified above"
```

### Challenge 3: Multi-File Instructions
Create different instructions for different folders:
```yaml
applyTo: "Firmware/**/*.cpp"  # Firmware-specific
applyTo: "tools/**/*.py"      # Tools-specific
```

---

*Lesson 3 Hands-On Exercises - GitHub Copilot Customization*  
*Duration: 20 minutes*

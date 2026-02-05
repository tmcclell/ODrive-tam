# Hack App Ideas (Choose One!)

## Instructions for Developers

Follow this workflow to take an idea from concept to implementation:

### Step 1: Select Your Idea
- Review the app ideas below and pick **one** that interests you
- Consider your team's strengths and the time available

### Step 2: Build Your Plan
- Use GitHub Copilot to help you create an initial project plan
- Prompt example: *"Create a development plan for [your chosen app] including architecture, tech stack, and key features"*
- Define your MVP scope—what's the smallest working version?
- 📘 **Learn more:** Check out [spec-kit](https://github.com/github/spec-kit) for guidance on building effective specs and PRDs

### Step 3: Iterate on the Plan
- Review the generated plan with your team
- Ask Copilot to refine specific sections: *"Expand on the testing strategy"* or *"Simplify the data model"*
- Identify dependencies and potential blockers
- Break down into smaller, actionable tasks
- **Build your Copilot customizations:**
    - Create a `copilot-instructions.md` file to guide Copilot's behavior for your project
    - Define custom agents tailored to your app's domain (e.g., a "manufacturing expert" or "test generator")
    - Set up reusable skills for common tasks like generating test stubs or validating SOPs

### Step 4: Implement

Choose the implementation approach that works best for your team:

#### Option A: Parallel Cloud Agents (Maximum Speed)
1. **Convert tasks to GitHub Issues** – Take your plan's tasks and create GitHub Issues for each one
2. **Assign Copilot to Issues** – Use "Assign to Copilot" on multiple issues simultaneously
3. **Copilot builds in parallel** – Copilot Coding Agent works on issues concurrently in the cloud, creating PRs for each
4. **Review & merge** – Review the generated PRs, provide feedback, and merge when ready
5. *Best for:* Teams wanting to maximize velocity with well-defined, independent tasks

#### Option B: IDE Agent Mode (Interactive)
1. **Work in VS Code/IDE** – Use Copilot Agent Mode directly in your editor
2. **Iterate in real-time** – Have a conversation with the agent, refine as you go
3. **Hands-on control** – You see every change, approve edits, and guide direction
4. *Best for:* Complex features requiring close collaboration and immediate feedback

#### Option C: Background Agents (Async Productivity)
1. **Kick off background tasks** – Start Copilot working on a task in the background
2. **Continue other work** – While the agent works, you can focus on different tasks
3. **Review when ready** – Come back to completed work and integrate
4. *Best for:* Multitasking developers who want to parallelize their own workflow

#### General Best Practices
- Start with scaffolding (project structure, basic setup)
- Write tests as you go—leverage the testing ideas mentioned for each app
- Commit frequently and validate each milestone
- Mix approaches as needed—use Cloud Agents for boilerplate, IDE Agent for tricky logic

---

## App Ideas

## 1. Mystery Pallet Tracker (Humorous)
A tiny web app where forklift operators log “mysterious pallets,” tag likely causes (gremlins, unauthorized teleportation, GPS drift), and mark them resolved.  
Great for unit testing domain logic + Playwright UI tests.

## 2. Andon Lite – Mini Andon Board
A lightweight issue‑raising dashboard for Safety/Quality/Delivery/Cost issues on a single manufacturing line.  
Simple states (Open → Acknowledged → Resolved).  
Perfect for testing state machines + end‑to‑end flows.

## 3. SOP Buddy – Copilot CLI SDK Powered App
Drop a Markdown SOP into the repo → app converts it into an interactive checklist + autogenerates C# unit test stubs or Playwright test skeletons using the Copilot CLI SDK.  
Super cool demo of AI‑assisted developer workflow.

## 4. Thermostat Whisperer (Humorous)
HVAC “mood interpreter” dashboard showing a cell’s temperature swings with dramatic alerts like “Your test chamber is experiencing mild existential dread.”  
Good for chart rendering + alert rule unit tests.

## 5. 5S Sidekick – Mini 5S Audit Wizard
A fun browser wizard where users grade an area (Sort/Set/Shine/Standardize/Sustain), attach pics, and generate an improvement ticket.  
Great for form validation tests + Playwright multi‑step flows.
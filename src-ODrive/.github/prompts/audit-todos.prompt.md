---
name: audit-todos
description: 'Scan workspace for TODOs and FIXMEs, optionally create Jira issues'
agent: agent
model: Claude Opus 4.5 (copilot)
tools: ['read', 'atlassian/atlassian-mcp-server/*']
---
Scan the entire `Firmware/` directory for comments containing "TODO" or "FIXME".

## Audit Steps

1. List all findings grouped by file (exclude ThirdParty/, doctest/ directories).
2. For each, assess priority based on context:
   - 🔴 CRITICAL: Safety issues, buffer overflows, race conditions
   - 🟠 HIGH: Functional bugs, protocol issues
   - 🟡 MEDIUM: Tech debt, refactoring needed
   - ⚪ LOW: Minor improvements, documentation
3. Present a summary table with file, line, TODO text, and priority.

## Jira Issue Creation (Optional)

If the user requests issue creation:

1. First, use `mcp_atlassian_atl_getAccessibleAtlassianResources` to get the cloudId.
2. Use `mcp_atlassian_atl_getVisibleJiraProjects` to list available projects.
3. Ask user to confirm:
   - Which project to create issues in
   - Which priority level TODOs to create issues for (e.g., only CRITICAL)
4. Use `mcp_atlassian_atl_search` to check if similar issues already exist.
5. Use `mcp_atlassian_atl_createJiraIssue` to create issues with:
   - Summary: `[TODO] <brief description from comment>`
   - Description: Include file path, line number, full TODO text, and surrounding context
   - Issue type: Bug (for CRITICAL/HIGH) or Task (for MEDIUM/LOW)

### Issue Template

```markdown
**Source:** `{file_path}:{line_number}`

**TODO Comment:**
> {todo_text}

**Context:**
{surrounding_code_snippet}

**Priority Assessment:** {priority_reason}

---
*Auto-generated from codebase TODO audit*
```

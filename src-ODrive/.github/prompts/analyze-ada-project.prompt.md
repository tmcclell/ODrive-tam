---
name: analyze-ada-project
description: Analyze an Ada project and create a migration plan to Modern C++
agent: Ada to C++ Migrator
tools: ['read', 'search/codebase', 'web/githubRepo']
---

# Analyze Ada Project for C++ Migration

Analyze the Ada project and produce a comprehensive migration plan.

## Input

- **Project Path or GitHub URL**: ${input:project_path:Path to Ada project or GitHub URL}

## Analysis Tasks

### 1. Package Dependency Graph

Scan all `.ads` and `.adb` files and map:
- Package dependencies (`with` clauses)
- Parent-child package relationships
- Circular dependency detection

Output a dependency graph showing migration order (bottom-up).

### 2. Concurrency Inventory

Identify and catalog:
- **Tasks**: Name, priority, period (if cyclic), entry points
- **Protected Objects**: Name, procedures, functions, entries with barriers
- **Synchronization**: Rendezvous points, select statements

### 3. Type Catalog

List all custom types:
- **Derived Types**: `type X is new Y`
- **Range Types**: `type X is range A .. B`
- **Record Types**: Fields and any representation clauses
- **Array Types**: Constrained and unconstrained
- **Tagged Types**: Inheritance hierarchy
- **Access Types**: Pointer usage patterns

### 4. HAL/Hardware Usage

Identify:
- Hardware register access patterns
- Interrupt handlers
- DMA usage
- Peripheral initialization

### 5. Complexity Assessment

For each package, estimate:
- Migration complexity (Low/Medium/High)
- Lines of code
- Test requirements
- Special considerations

## Output Format

```markdown
# Ada Project Migration Analysis

## Summary
- Total packages: X
- Tasks: X
- Protected objects: X
- Estimated migration time: X weeks

## Dependency Graph
[ASCII art or mermaid diagram]

## Migration Order
1. [Package] - Level 0 (no dependencies)
2. [Package] - Level 0
...

## Concurrency Inventory
| Component | Type | Priority | Period | Notes |
|-----------|------|----------|--------|-------|

## Type Catalog
| Ada Type | Recommended C++ | Complexity |
|----------|-----------------|------------|

## Risk Assessment
| Risk | Impact | Mitigation |
|------|--------|------------|
```

## Reference

Read these before analysis:
- `.github/skills/ada-cpp-migration/SKILL.md`
- `.github/skills/ada-cpp-migration/references/ada-cpp-patterns.md`

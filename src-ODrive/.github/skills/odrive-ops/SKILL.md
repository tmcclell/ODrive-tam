---
name: 'ODrive Ops'
description: 'CI/CD, releases, and deployment operations for ODrive'
status: production
version: 1.0.0
---

# ODrive Ops Skill

All CI/CD, release, and deployment scripts in one place.

## Scripts

| Script | Purpose | Usage |
|--------|---------|-------|
| `check_ci.py` | Check GitHub Actions status | `python check_ci.py` |
| `validate_workflows.py` | Validate workflow YAML | `python validate_workflows.py` |

---

## Quick Usage (GitHub CLI)

### Check CI Status
```powershell
gh run list --limit 5
gh run view <run-id>
gh run view <run-id> --log-failed
```

### Trigger Workflow (⚠️ Confirmation Required)
```powershell
gh workflow run firmware.yaml -f board=v3.6-56V
```

### Create Release (⚠️ Confirmation Required)
```powershell
git tag -a v0.5.7 -m "Release v0.5.7"
git push origin v0.5.7
```

### View Workflows
```powershell
gh workflow list
gh workflow view firmware.yaml
```

---

## Workflow Files

| Workflow | File | Trigger |
|----------|------|---------|
| Firmware | `.github/workflows/firmware.yaml` | Push, PR |
| Tests | `.github/workflows/tests.yaml` | Push, PR |
| Release | `.github/workflows/release.yaml` | Tag push |
| Docs | `.github/workflows/docs.yaml` | Push to master |

---

## Safety

✅ **Safe**: Check status, read logs, validate syntax  
⚠️ **Confirmation Required**: Trigger workflows, create releases, deploy

---

## Related

- **Agent**: `.github/agents/ODrive-Ops.agent.md`
- **Prompt**: `.github/prompts/ops.prompt.md`

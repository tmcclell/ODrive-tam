# Ops Operations

CI/CD, releases, and deployments for ODrive.

## Agent: ODrive-Ops

### Check CI Status
```powershell
gh run list --limit 5
gh run view <run-id>
gh run view <run-id> --log-failed
```

### View Workflows
```powershell
gh workflow list
gh workflow view firmware.yaml
```

### Trigger Workflow (⚠️ Confirmation Required)
```powershell
# Build firmware
gh workflow run firmware.yaml -f board=v3.6-56V

# Run tests
gh workflow run tests.yaml
```

### Create Release (⚠️ Confirmation Required)
```powershell
git tag -a v0.5.7 -m "Release v0.5.7"
git push origin v0.5.7
```

---

## Workflows

| Workflow | File | Purpose |
|----------|------|---------|
| Firmware | `firmware.yaml` | Build all boards |
| Tests | `tests.yaml` | Run tests |
| Release | `release.yaml` | Publish releases |
| Docs | `docs.yaml` | Deploy docs |

---

## Safety

✅ Safe: Check status, read logs  
⚠️ Confirm: Trigger workflows, releases, deploy

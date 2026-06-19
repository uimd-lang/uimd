# Reporting Issues

Use GitHub Issues for UIMD bugs, regressions, render mismatches, compiler
problems, SDK install failures, and MCP tester failures. For security-sensitive
problems, use GitHub's private vulnerability reporting instead of a public
issue.

## Sanitized UIMD Reports

When a problem depends on a `.uimd` layout, generate a Markdown issue report
from the source file:

```bash
uimd issue-report path/to/app.uimd "Short problem summary" --output issue.md
```

Then review the generated file and create the GitHub issue:

```bash
gh issue create \
  -R uimd-lang/uimd \
  --title "Short problem summary" \
  --body-file issue.md
```

For Python/C++ parity problems, include the affected targets:

```bash
uimd issue-report path/to/app.uimd \
  "Python/C++ render parity mismatch" \
  --kind parity \
  --targets python,cpp \
  --output issue.md
```

The command only generates Markdown. It does not submit anything to GitHub, so
the reporter can inspect the output before publishing it.

## Privacy Behavior

The default privacy mode is `safe`.

```bash
uimd issue-report app.uimd "Problem summary"
```

In safe mode, UIMD:

- removes `description:` fields
- anonymizes member and element names
- preserves the ASCII `ui` layout geometry with same-length replacement names
- leaves colors unchanged
- reparses the sanitized source
- verifies that layout geometry is preserved

Colors are intentionally not changed. Render and parity bugs often depend on
exact foreground and background values, so changing colors can destroy the
reproduction.

For public examples or intentionally shareable sources, disable sanitization:

```bash
uimd issue-report app.uimd "Problem summary" --privacy none --output issue.md
```

To keep names while still removing descriptions:

```bash
uimd issue-report app.uimd "Problem summary" --no-anonymize-names --output issue.md
```

To keep descriptions too:

```bash
uimd issue-report app.uimd "Problem summary" --keep-descriptions --output issue.md
```

## What To Add Manually

After generating the report, fill in the placeholders for:

- expected behavior
- actual behavior
- exact reproduction command
- first mismatch or error message
- screenshots, snapshot viewer command, or attached logs when useful

Large logs and snapshots should usually be attached through the GitHub web UI
or linked from a gist instead of pasted into the main issue body.

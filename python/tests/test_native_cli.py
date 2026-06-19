"""Smoke tests for the native UIMD CLI."""

import os
import subprocess
import tempfile
import unittest


PROJECT_ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "..", ".."))
NATIVE_UIMD_BINARY = os.path.join(PROJECT_ROOT, "cpp", "build", "tools", "uimd", "uimd")
NATIVE_CLI_TIMEOUT_SECONDS = 10


class TestNativeCliIssueReport(unittest.TestCase):
    def test_issue_report_anonymizes_member_names_and_removes_descriptions(self):
        if not os.path.exists(NATIVE_UIMD_BINARY):
            self.skipTest("native uimd binary is not built")

        source = """# Repro

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Internal customer dashboard."
```

## Members

```yaml
project_name:
  type: textinput
  placeholder: Project name
  description: "Internal project name."

apply_btn:
  type: button
  title: Apply
  description: "Internal action description."
```

## Style

```yaml
@project_name:
  background: "#252b35"
  color: "#ffffff"

@apply_btn:
  background: "#2f80ed"
  color: "#f5f5f5"
```

## User Interface

```ui
+---------------------------+
|project_name.. apply_btn.. |
+---------------------------+
```
"""

        with tempfile.TemporaryDirectory() as temp_dir:
            source_path = os.path.join(temp_dir, "repro.uimd")
            with open(source_path, "w", encoding="utf-8") as handle:
                handle.write(source)

            result = subprocess.run(
                [
                    NATIVE_UIMD_BINARY,
                    "issue-report",
                    source_path,
                    "Parity mismatch in filters",
                    "--kind",
                    "parity",
                    "--targets",
                    "python,cpp",
                ],
                cwd=PROJECT_ROOT,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=NATIVE_CLI_TIMEOUT_SECONDS,
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("Parity mismatch in filters", result.stdout)
        self.assertIn("input0000001", result.stdout)
        self.assertIn("button002", result.stdout)
        self.assertIn("#2f80ed", result.stdout)
        self.assertIn("- element names anonymized: yes", result.stdout)
        self.assertIn("- descriptions removed: yes", result.stdout)
        self.assertIn("- colors changed: no", result.stdout)
        self.assertIn("- layout geometry preserved: yes", result.stdout)
        self.assertNotIn("project_name", result.stdout)
        self.assertNotIn("apply_btn", result.stdout)
        self.assertNotIn("description:", result.stdout)
        self.assertNotIn("Internal", result.stdout)

    def test_issue_report_geometry_check_ignores_anonymized_content_order(self):
        if not os.path.exists(NATIVE_UIMD_BINARY):
            self.skipTest("native uimd binary is not built")

        source = """# Repro

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
filter_address_label:
  type: label
  text: Address
  description: "Address filter label."

filter_developer_label:
  type: label
  text: Developer
  description: "Developer filter label."
```

## User Interface

```ui
+-------------------------+---------------------------+
|filter_address_label..** |filter_developer_label..** |
+-------------------------+---------------------------+
```
"""

        with tempfile.TemporaryDirectory() as temp_dir:
            source_path = os.path.join(temp_dir, "repro.uimd")
            with open(source_path, "w", encoding="utf-8") as handle:
                handle.write(source)

            result = subprocess.run(
                [
                    NATIVE_UIMD_BINARY,
                    "issue-report",
                    source_path,
                    "Geometry order mismatch",
                ],
                cwd=PROJECT_ROOT,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=NATIVE_CLI_TIMEOUT_SECONDS,
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("Geometry order mismatch", result.stdout)
        self.assertIn("- layout geometry preserved: yes", result.stdout)
        self.assertNotIn("filter_address_label", result.stdout)
        self.assertNotIn("filter_developer_label", result.stdout)


if __name__ == "__main__":
    unittest.main()

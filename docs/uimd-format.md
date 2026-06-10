# UIMD Format

UIMD means User Interface Markdown.

`.uimd` is the canonical source extension. Legacy `.md` files remain accepted
when their metadata contains `format: ui-md`.

## Minimal File

````md
# Hello

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Hello world UI."
```

## Members

```yaml
title:
  type: label
  text: "Hello from UIMD"
  description: "Application title."
```

## User Interface

```ui
+-----------------------------+
| title...................... |
+-----------------------------+
```
````

## Sections

`## Metadata` defines the format, version, kind, and user/agent descriptions.

`## Definition` optionally declares inheritance, dependencies, and related
window metadata.

`## Members` declares UI elements.

`## Style` declares visual styling.

`## User Interface` contains the ASCII UI layout in a fenced `ui` block.

ASCII borders are design guides for layout. They do not become element border
properties.

The star expansion marker (`*`) may appear at most two times consecutively in a
row or vertically in a column. After marking expansion, use ordinary empty
space or `.` filler for additional visual area.

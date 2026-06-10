# UI Rules

This document describes how to write UI `.uimd` files for this project and how the
runtime interprets them.

## Basic Structure

Each UI file uses explicit Markdown sections:

1. `# <Title>`
2. `## Metadata`
3. optional `## Definition`
4. optional `## Members` for files without runtime elements, required when the
   UI declares elements
5. optional `## Style`
6. `## User Interface`

Example:

~~~md
# Example

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Small example window."
```

## Members

```yaml
ok:
  type: button
  title: OK
  description: "Confirm button."
```

## Style

```yaml
include: dark

@ok:
  color: "#ffffff"
```

## User Interface

```ui
+--------+
| ok.... |
+--------+
```
~~~

YAML frontmatter (`--- ... ---`) is not supported by the active format.

## Metadata

`## Metadata` is required.

```yaml
format: uimd
format-version: 1
kind: window
description: "What this UI is for."
status: stable
tags: [example, form]
```

Required fields:

- `format: uimd`
- `format-version: 1`
- `kind`
- `description`

Optional fields:

- `status`
- `tags`

`description` becomes the MCP/window description. Metadata is documentation and
tooling data only; it must not affect layout, style, focus, sizing, or runtime
behavior.

## Definition

`## Definition` is optional. If the section or `extends` is missing, the
compiler uses `uiwindow`.

```yaml
extends: uiwindow
uses:
  - message
  - settings
direction: auto
```

Supported `extends` values:

| Value | Runtime Class | Use When |
|-------|---------------|----------|
| `uiwindow` | `UIWindow` | Top-level application window |
| `uicontrol` | `UIControl` | Reusable embedded panel/component |
| `uiscrollview` | `UIScrollView` | Scrollable container of child components |
| `uielement` | `UIElement` | Passive display widget |
| `uipopup` | `UIPopup` | Overlay popup |

`kind` in metadata does not control inheritance. Use `Definition.extends`.

`uses` declares compile-time dependencies on other `.uimd` components. The
compiler searches for `<name>.uimd` first, then legacy `<name>.md`, recursively, compiles each dependency in its
own directory, and generates import/include paths automatically.

`direction` applies to `uipopup`:

| Value | Behavior |
|-------|----------|
| `auto` | Runtime picks the best direction to keep the popup on screen |
| `up` | Popup grows upward from the anchor row |
| `down` | Popup grows downward from the anchor row |
| `left` | Popup grows left of the anchor column |
| `right` | Popup grows right of the anchor column |

## Members

`## Members` is a top-level mapping of member IDs. There is no `members:`
wrapper. Each member must define `type`.

Supported built-in types:

- `label`
- `spanlabel`
- `infolabel`
- `messagetable`
- `textinput`
- `textarea`
- `numberinput`
- `button`
- `checkbox`
- `combobox`
- `listbox`
- `uielement`

Type-specific required fields:

- `label`: `text`
- `spanlabel`: `text`
- `button`: `title`
- `checkbox`: `title`
- `combobox`: `options`
- `listbox`: `options`

Common member metadata:

- `description`: documentation/tooling/MCP description only.
- `expose: false`: hide from agent-facing MCP element lists and schemas.

Element roles for MCP are derived from `type`; the source format does not have a
manual role override in version 1.

### Member Examples

```yaml
name:
  type: textinput
  value: ""
  maxlength: 50
  description: "User name field."

age:
  type: numberinput
  value: 0
  step_size: 1
  min_value: 0
  max_value: 120
  format_str: "0"

ok:
  type: button
  title: OK

agree:
  type: checkbox
  title: I agree
  value: false

theme:
  type: combobox
  options: [Dark, Light]
  selected_item: Dark

roles:
  type: listbox
  options: [Admin, User, Guest]
  selected_items: [User]
  multiple: false
```

### Reusable Components

A non-built-in `type` embeds another `.uimd` UI component. The type name normally
matches a dependency declared in `uses`.

```yaml
# Definition
uses:
  - chat_panel

# Members
chat:
  type: chat_panel
```

Optional overrides:

```yaml
chat:
  type: chat_panel
  source: chat_panel
  class: ChatPanel
  cpp-class: sample::CustomPanel
  cpp-header: custom_panel.hpp
```

## Shared Namespace

Member IDs, named ASCII cells/regions, and unquoted layout element names share
one namespace. The compiler rejects duplicates and conflicts.

Valid:

```ui
+-form-----------+
| name_input.... |
+----------------+
```

Invalid because `name_input` is both a cell name and an element name:

```ui
+-name_input-----+
| name_input.... |
+----------------+
```

Quoted text in the UI grid is display text, not a namespace name.

## ASCII Grid

The ASCII border `+`, `-`, `|` is only a visual layout aid. It is not
transferred to runtime element styles.

Rules:

- Element position is relative to the cell content area.
- The content area is the inside of the cell without border characters.
- `.` extends the element footprint within a cell.
- The star expansion marker (`*`) must not appear more than two times
  consecutively in one row or more than two times vertically in one column.
  Use at most two star markers to request expansion, then use ordinary empty
  space or `.` layout filler for additional visual rows or columns.
- Every intersection of a horizontal and vertical border must use `+`.
- Horizontal borders must extend at least to the edge of the cell content.

### Cell Sizing

Structural cell size is controlled by special characters on the cell borders:

| Border | Modifier | Effect |
|--------|----------|--------|
| Top horizontal | `**` | Cell width fills remaining horizontal space |
| Top horizontal | digits | Fixed cell width |
| Top horizontal | `##` | Fit-content cell width |
| Left vertical | `*` on one or two consecutive rows | Cell height fills remaining vertical space |
| Left vertical | digits | Fixed cell height |
| Left vertical | `#` on consecutive rows | Fit-content cell height |

No modifier means auto sizing from the element footprint and cell content.

Example:

```ui
+-sidebar-12-+-main-**---------+
| nav....... | content........ |
#            |                 |
#            |                 |
+------------+-----------------+
```

## Style

`## Style` is optional and contains style declarations directly. There is no
`style:` wrapper.

```yaml
include: dark

this:
  background: "#1e1e1e"
label:
  color: "#d4d4d4"
@name:
  color: "#ffffff"
```

Selector forms:

- `this`: root window/control.
- Built-in type selectors: `label`, `button`, `textinput`, etc.
- `@name`: member ID or named layout cell/region.
- Nested selectors are flattened by the compiler.

If no `## Style` section is present for a non-theme UI, `dark` is included
automatically. Local styles override included theme styles per selector.

State prefixes:

- `focus-background`, `focus-color`
- `edit-background`, `edit-color`
- `cursor-background`, `cursor-color`
- `selected-background`, `selected-color`
- `checked-background`, `checked-color`
- `unchecked-background`, `unchecked-color`
- `error-background`, `error-color`

## Scroll Views

`extends: uiscrollview` creates a scrollable container. The viewport is the
layout cell named `panel` or `viewport`.

```yaml
extends: uiscrollview
```

```yaml
@panel:
  gap: 1
```

```ui
+-panel-**--+
|           |
*           |
*           |
+----------+
```

Children are `UIControl` instances added by runtime code. Scroll behavior is
runtime-level and must not be implemented separately in examples.

## Event Hooks

Generated application logic should prefer subclass override hooks over global
callback wiring.

Use generated hooks only for event-capable elements and only for their relevant
events:

| Element Type | Event Hooks |
|--------------|-------------|
| `button` | click |
| `checkbox` | change |
| `textinput` | change, submit |
| `textarea` | change, submit |
| `numberinput` | change, submit |
| `combobox` | change |
| `listbox` | selection change |

Python and C++ hook names must stay aligned. For an element named
`hello_button`, the C++ hook is `onHelloButtonClick()`, and Python generated
code must expose the idiomatic equivalent for the same logical event.

Low-level runtime callback configuration remains available for advanced use,
but examples and scaffolded applications should use generated override hooks by
default.

## Popups

`extends: uipopup` creates an overlay popup. Use `direction` in `## Definition`
to declare the preferred expansion direction.

~~~md
# Commands

## Metadata

```yaml
format: uimd
format-version: 1
kind: popup
description: "Command popup."
```

## Definition

```yaml
extends: uipopup
direction: up
```

## Members

```yaml
items:
  type: listbox
  options: []
```

## User Interface

```ui
+-popup-**+
| items.. |
+---------+
```
~~~

## Themes

Theme files use the same section parser with `kind: theme`. They normally have
`## Metadata` and `## Style`, without `## Members` or `## User Interface`.

~~~md
# Dark

## Metadata

```yaml
format: uimd
format-version: 1
kind: theme
description: "Default dark theme."
```

## Style

```yaml
this:
  background: "#101010"
label:
  color: "#ffffff"
```
~~~

## Testing With MCP Tester

UIMD applications should be tested through the visual MCP tester when behavior,
focus, rendering, dialogs, or Python/C++ parity matters. The tester is installed
with the Python package and can be run from external projects that use UIMD.

Basic commands:

```bash
uimd mcp-test APP_PATH YAML_CONFIG
uimd-mcp-test APP_PATH YAML_CONFIG
```

Recommended automation flags:

```bash
uimd mcp-test --exit-on-finish --log-file tests/mcp/results.log APP_PATH YAML_CONFIG
```

The tested app must be a generated UIMD app or compatible executable that
accepts the runtime MCP flags passed by the tester, including `--mcp-server`,
`--gui`, `--mcp-transport tcp`, `--mcp-port`, `--viewport`, and
`--mcp-wait-render`.

YAML test configs should:

- Store test files under `tests/mcp/` in the consuming project when possible.
- Use app paths relative to the YAML file or the project root.
- Drive the UI through MCP tools such as `activate_element`, `press_key`,
  `set_text`, `get_element`, `get_state`, and `get_render_snapshot`.
- Prefer semantic assertions on element values and state before render
  snapshots.
- Use render snapshot comparisons for visual regressions and backend parity.
- Keep setup and cleanup file operations in tester steps rather than in example
  application code.
- Avoid sleeps in test scripts unless they model real asynchronous behavior;
  prefer tester delay flags when timing needs to be adjusted.

Compare mode runs the same YAML script against two implementations:

```bash
uimd mcp-test --compare python/examples/calculator/calculator.py cpp/build/examples/calculator/calculator tests/mcp/calculator.yaml
```

For repository-wide validation, aggregate configs can include multiple scripts
and can be run with example roots:

```bash
uimd mcp-test --compare python/examples cpp/build/examples tests/mcp/all_examples.yaml
```

Failures should be fixed in shared runtime, generator, compiler, specifications,
or tester infrastructure when the behavior is general. Do not add
example-specific workarounds for focus, rendering, MCP, dialog, modal, layout,
style, or platform parity issues.

## Design Rules

- Put all layout and design decisions in the `.uimd` file.
- Do not add example-specific runtime fixes for shared rendering, focus, MCP,
  dialog, modal, layout, style, or platform parity behavior.
- Use `uses` for cross-file dependencies so the compiler resolves paths.
- Keep ASCII borders as visual guides only.

# Markdown UI Format

Markdown UI files use explicit Markdown sections. YAML frontmatter is not part
of the active format and must be rejected by the compiler.

## Required Shape

~~~md
# Window Title

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "What this UI is for."
```

## Definition

```yaml
extends: uiwindow
uses:
  - reusable_panel
direction: auto
```

## Members

```yaml
name_input:
  type: textinput
  description: "User name field."
save_button:
  type: button
  title: Save
```

## Style

```yaml
include: dark

this:
  background: "#1e1e1e"
@save_button:
  color: "#ffffff"
```

## User Interface

```ui
+-------------+
| name_input. |
+-------------+
| save_button |
+-------------+
```
~~~

`# Window Title` is the runtime window/component title. Generated class names
come from the source filename.

## Metadata

`## Metadata` is required for every UI source.

- `format: uimd` is required.
- `format-version: 1` is required.
- `kind` is required. Current values are documentation-level metadata; it does
  not control inheritance.
- `description` is required and becomes the MCP/window description.
- `status` and `tags` are optional documentation fields.

Metadata must not affect layout, style, focus, sizing, or runtime behavior.

## Definition

`## Definition` is optional. Missing `## Definition`, or missing `extends`
inside it, defaults to `uiwindow`.

Supported fields:

- `extends`: `uiwindow`, `uicontrol`, `uiscrollview`, `uielement`, or `uipopup`.
- `uses`: compile-time dependencies on other `.uimd` components.
- `direction`: popup direction for `uipopup`.

`kind` in metadata does not affect inheritance; only `Definition.extends` does.

## Members

`## Members` is a top-level mapping of member IDs. There is no `members:`
wrapper. Each member must define `type`.

Type-specific required fields:

- `label`: `text`
- `spanlabel`: `text`
- `button`: `title`
- `checkbox`: `title`
- `combobox`: `options`
- `listbox`: `options`

Other built-in element types, including `textinput`, `textarea`,
`numberinput`, `infolabel`, `messagetable`, and `uielement`, do not require a
type-specific field beyond `type`.

`description` is a documentation/tooling field. It is used for generated MCP
element descriptions and must not affect runtime design.

MCP roles are derived from `type`; the format has no manual role override in
version 1. `expose: false` hides the member from agent-facing element lists and
schemas while preserving direct runtime access by element ID.

A non-built-in `type` names a reusable UI dependency declared through `uses`.

## Tools

`## Tools` is optional and declares app-specific MCP tools as a top-level
mapping. Each tool name must be an identifier and each tool must include a
`description`. `input` and `output` are optional short schema mappings compiled
to JSON Schema for MCP metadata. The first version uses one unified shape and
does not require separate query/command tool types.

Example:

```yaml
query_projects:
  description: "Return filtered project records."
  input:
    name: string
    status: string
    required: [name]
  output:
    projects:
      - id: string
        name: string
        tasks_open: int
        active: bool
```

Supported scalar type names include `string`, `int`, `number`, `bool`,
`object`, `array`, and `any`. Arrays can be written with one item schema, for
example `tags: [string]` or a YAML list containing one object schema.

## Style

`## Style` is optional and contains the style mapping directly. There is no
`style:` wrapper.

Supported selector shapes:

- `include: dark` loads a theme from the current source directory or
  `shared/themes`.
- `this` targets the root window/control.
- Built-in type selectors such as `button`, `textinput`, and `checkbox` target
  all elements of that type.
- `@name` targets a member ID or named layout cell/region.
- Nested selectors are flattened by the compiler.

If no `## Style` section is present for a non-theme UI, `dark` is included by
default. Local style properties override included theme properties per selector.

State prefixes map to element style slots:

- `focus-*`
- `edit-*`
- `cursor-*`
- `selected-*`
- `checked-*`
- `unchecked-*`
- `error-*`

## User Interface

`## User Interface` contains one fenced `ui` block with the ASCII layout grid.
ASCII borders (`+`, `-`, `|`) are visual layout guides only and never become
element borders.

The star expansion marker (`*`) is limited to at most two consecutive markers
in one row and at most two vertical markers in one column. Use ordinary empty
space or `.` filler for additional visual area after expansion has been marked.

Member IDs, named cells/regions, and unquoted layout element names share one
namespace. The compiler must reject duplicates or conflicts with a clear error.

Quoted text in the grid is implicit display text, not a namespace name.

## Themes

Theme files use the same section parser with `kind: theme`, normally without a
`## User Interface` block and without `## Members`.

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

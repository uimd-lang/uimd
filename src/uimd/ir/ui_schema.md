# UI IR Schema

The UI IR is the backend-neutral model produced from Markdown UI definitions
before language-specific code generation.

## Top-Level Objects

- `Window`: title, base class, dimensions, root layout tree, elements, styles, dependencies.
- `Control`: reusable embedded UI with the same layout and element model as a window.
- `Element`: named UI component with type, text/value/options, relative layout position, and style state references.
- `LayoutCell`: parsed cell from the ASCII grid with row, column, width, height, name, and cell style selector.
- `ResolvedCell`: runtime layout result with computed size and dynamic position derived from the layout tree.
- `Style`: render-affecting properties such as foreground, background, padding, gap, border settings, and state overrides.
- `Color`: named color, hex color, transparent, or backend-specific terminal color token.
- `Event`: keyboard, mouse, paste, resize, focus, blur, open, close, save, and element-changed events.
- `FocusState`: currently focused element and focus traversal order.
- `EditMode`: whether the focused element consumes editing keys directly.
- `ScrollState`: viewport dimensions, content height, current line offset, and dirty/cache invalidation flags.

## Element State Styles

Each element may resolve these style states:

- base `style`
- `focus_style`
- `edit_style`
- `cursor_style`
- `selected_style`
- `checked_style`
- `unchecked_style`
- `error_style`

Missing values fall back to the base style. Backends must include every
render-affecting value in style cache keys.

## Positioning Rule

Absolute positions should be computed from resolved layout cells plus relative
element coordinates. Do not store duplicate absolute positions unless a backend
needs a transient render result.

# Layout Rules

## ASCII Grid Semantics

The ASCII border characters `+`, `-`, and `|` are only design aids for parsing
cell boundaries. They do not become runtime borders on elements.

## Cells

Cells may be named and styled through selectors. A cell can contain elements,
text labels, or nested layout structure.

Core fields:

- relative row and column
- width and height
- optional name
- optional style selector
- parent/child layout relationship

## Sizing

Backends must support:

- fixed dimensions from the parsed grid
- auto dimensions
- fit-content width and height
- expanded cells
- padding and gap

In `.uimd` ASCII layout source, the star expansion marker (`*`) must never
appear more than two times consecutively in one row or more than two times
vertically in one column. Two adjacent top-border star markers request expanded
width; one or two left-border star markers on consecutive rows request expanded
height. Additional visual rows or columns should use ordinary empty space or
`.` filler instead of additional star markers.

Fit-content height for wrapped labels must account for explicit newlines and
word wrapping. Scroll view content height is derived from child measurements and
gap spacing.

## Positioning

Resolved absolute positions are a render result, not source data. Compute them
from the structural layout tree and relative coordinates. Avoid storing
duplicate absolute position members when the value can be derived.

## Selectors

Selectors can target element types, named cells, nested cells, and descendants.
Cell backgrounds and element styles are separate concepts.

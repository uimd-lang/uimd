# Runtime Behavior

## Focus And Edit Mode

Navigation mode is the default. Tab, Shift+Tab, and arrow keys move focus
between elements. Enter enters edit mode for editable controls. Escape exits
edit mode.

Buttons and check boxes activate directly from navigation mode. Combo boxes
open on Enter and leave edit mode after a selection is confirmed.

## Input

Backends should model:

- keyboard keys and modifiers
- Enter, Escape, Tab, Shift+Tab
- mouse press, drag, release, and wheel
- paste
- terminal resize

Platform-specific input decoding must stay behind a terminal backend interface.

### Keyboard Dispatch Order

Every logical keyboard event follows the same cancellable three-stage route:

1. the active window receives `onPreviewKey`;
2. when unhandled, the focused element receives `onKey` and performs its
   element-specific interpretation;
3. when still unhandled, the active window receives `onKey` as the fallback.

A handled stage stops further routing. Runtime navigation and edit-mode
defaults run only after the public stages that precede them. `onPreviewKey`
receives the logical key, the focused element's full stable runtime ID/path,
and the current edit-mode state. A repeated reusable descendant must therefore
be distinguishable from another instance that uses the same local member name.

Element semantic actions are dispatched by the element rather than by a
window inspecting raw key strings. Enter on an edited ListBox first raises its
cancellable item-activate event with the active index and value. When handled,
the ListBox default Enter behavior and later window fallback do not run. When
unhandled, the ListBox performs its normal selection/commit behavior. Ordinary
notifications such as click, value changed, and selection changed describe an
action that already happened and are not cancellable.

## Rendering

Backends should track dirty state at app, window, layout, and element levels.
C++ terminal rendering should use current and previous cell buffers and emit only
changed cells/runs.

## Scroll View

Scroll views contain reusable controls and support:

- many children
- children taller than the viewport
- line-level offset
- cached child heights
- invalidation when child content changes
- rendering only visible or intersecting rows
- wheel scrolling that marks dirty only when the offset changes

## Text Selection

Labels with `user-select: text` can participate in mouse drag selection,
including across child controls in a scroll view. Any key press clears active
label selection.

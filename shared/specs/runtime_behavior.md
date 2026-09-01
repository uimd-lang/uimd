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

## Rendering

Backends should track dirty state at app, window, layout, and element levels.
C++ terminal rendering should use current and previous cell buffers and emit only
changed cells/runs.

### Raw Image Teardown

Terminal raw-image payloads such as Sixel occupy a graphics plane that is not
guaranteed to be erased by text-cell spaces or `CSI 2 J`. A diff renderer must
therefore compare current raw anchors with its previous cell buffer. Before an
old raw anchor is removed, moved, resized, or replaced, the renderer must erase
the old cell rectangle with the terminal rectangular-erase primitive, repaint
text cells inside that rectangle, and re-emit current raw rectangles only when
they intersect the erased area or changed through the ordinary diff. The text
overlay follows any replacement raw payload. The erase and repaint belong to
one synchronized terminal update. An unchanged raw payload with unchanged
geometry outside the erased area must not be erased or retransmitted.

This lifecycle belongs to the shared terminal buffer. Window stacks, dialogs,
examples, and application callbacks must not perform raw-image cleanup.

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

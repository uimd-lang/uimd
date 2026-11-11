# ScrollView Virtualization Contract

## Scope

This contract adds an optional virtual data-source mode to `UIScrollView` /
`ScrollView`. Existing retained children, rendered-child callbacks, scrolling,
focus, rendering, and public source compatibility remain unchanged when no
virtual data source is configured.

All supported runtimes implement the same state machine. Language-idiomatic
capitalization and ownership types may differ, but callback order and visible
behavior must not.

## Data source

A virtual data source supplies:

- the current logical item count;
- a stable key for a logical item index;
- a materializer that receives the logical index and an optional recycled row,
  updates all row-visible state, and returns the row to use;
- an optional estimated row height for the current content width; when it is
  omitted, the runtime materializes and measures one representative row.

The estimate is a data-source-level value, not a per-item traversal. It may be
computed from content or a representative row, but must not require visiting
every logical item. A non-positive estimate is rejected.

Configuring a virtual data source clears retained children. Adding retained
children while virtual mode is active clears virtual mode. This makes ownership
unambiguous and preserves the old retained API.

## Height index

Virtual mode stores the configured estimate as an implicit baseline and stores
only measured differences in a Fenwick tree. The tree must support:

- item height lookup in `O(log N)`;
- measured-height replacement in `O(log N)`;
- prefix extent in `O(log N)`;
- content-offset to logical-index lookup in `O(log N)` using Fenwick binary
  lifting with the implicit baseline included in every node.

The gap is part of each indexed unit; the final trailing gap is excluded from
the content height. Initializing or resizing the logical collection must not
allocate or visit `N` row components or `N` height entries.

## Materialization

For each layout/render pass, ScrollView derives the first visible logical index
from the height index, then materializes only rows intersecting the viewport and
one viewport of overscan above and below. Materialized rows are ordered by
logical index. Rows leaving the materialized range enter a bounded recycle pool.

Before a recycled row is passed to the data source, runtime-owned transient
state is cleared recursively: focus, edit ownership, hover/pressed state, mouse
capture, and label selection. The materializer must replace all domain-visible
row values. A focused row may remain pinned until focus moves or the source
removes it; it is the only permitted materialized row outside the overscan
range.

Actual measured heights replace estimates after materialization. The runtime
captures the top visible logical item and the row offset within it before
refinement, then restores that anchor after updating the height index. When
auto-scroll is enabled, the bottom remains the anchor instead.

The anchor offset is relative to the item's complete indexed span: its measured
height plus the following gap, except that the final item has no trailing gap.
An anchor inside an inter-item gap must retain that exact gap coordinate across
width changes and height-index rebuilds. If an item's span shrinks, restoration
clamps the offset to the final row of the new complete span.

## Mutations

Virtual mode exposes targeted notifications for insert, remove, replace, and
height invalidation. They update logical count and the height index without
materializing unaffected rows.

The application mutates its data source before sending a notification.
Insertion and replacement ranges therefore use the new data-source coordinates,
while a removal range describes the removed rows in the old coordinates and
must not be clamped to the already-smaller post-removal item count.

- Insertions before the visible anchor shift its logical index so the same item
  remains visible.
- Removals recycle affected materialized rows and repair focus/edit/selection
  before the next render. Removing the anchored row selects the next surviving
  logical row, or the previous row when the removal reaches the end.
- Replacements preserve the stable key when the source represents the same
  logical item; a changed key is treated as removal plus insertion for transient
  state.
- Height invalidation resets only the affected measured differences. A width
  change clears measured differences and re-establishes the same logical anchor
  incrementally from visible rows.

## Rendering and input

Content height, maximum scroll offset, scroll indicators, line/page/Home/End
scrolling, mouse-wheel scrolling, hit testing, selection, focus navigation, and
MCP snapshots use logical indexes and the height index. No steady-state event or
frame may traverse every logical row.

The terminal diff still receives a complete terminal-sized frame; only rows
inside the visible viewport are rendered from child components. Existing
terminal scroll-region acceleration remains available.

## Diagnostics and benchmark gate

Each runtime exposes read-only virtual diagnostics containing at least logical
item count, materialized row count, recycled row count, rows measured in the
last pass, rows considered for the last visible-range calculation, and rows
rendered in the last pass. Diagnostics must not change behavior.

The cross-platform benchmark covers 1K, 10K, 100K, and 1M logical rows with
fixed and variable actual heights. It reports child construction, layout/height
resolution, visible-range calculation, rendering, terminal diff, and one
keyboard/event dispatch separately. The correctness gate requires operation
counts to stay bounded by visible rows plus overscan after warm-up. Timing is
reported for comparison; the target is p95 frame latency below 16.7 ms on the
documented benchmark machine, not a machine-independent unit-test assertion.

Run the canonical reference phase benchmark from the repository root with
`python3 tools/scrollview_virtualization_benchmark.py`. Port-specific runtime
regressions assert the same bounded operation counts, while the Agent Chat MCP
scenario compares the complete rendered and input behavior against C++ at a
fixed 90x35 viewport.

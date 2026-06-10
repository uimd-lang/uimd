# Markdown Viewer Demo

This document is intentionally longer than one screen. It demonstrates how a
Markdown-like source can be split into reusable UI blocks and rendered in a
scrollable panel.

The viewer uses a permanent ListBox focus on the left. When you move with Up
or Down, the preview changes immediately. The right side is a UIScrollView, so
the document can be longer than the terminal height.

## What this example shows

The example is not a full Markdown application. It is a compact practical
viewer that exercises several important runtime features at the same time:

- reusable UI components
- dynamic document content
- scrollable panels
- selectable text
- SpanLabel headings
- MessageTable rendering
- code block rendering
- fullscreen layout resizing

Each content block is represented by a small UI component. A paragraph is not
drawn by manually writing into the terminal. It is a normal label inside a
scroll view. A heading is a SpanLabel. A code block is a SpanLabel with word
wrapping disabled. A table is a MessageTable element.

## Why use blocks

Using blocks keeps the source document separate from the visual layout. The
parser only decides that something is a heading, paragraph, table, or code
block. The visual look lives in the `.uimd` UI files inside the `blocks`
directory.

This is useful because a documentation viewer and a chat message list can share
the same runtime mechanisms while still using different visual treatments. The
data is simple text. The presentation is controlled by the component tree.

## Runtime behavior

When the selected document changes, the application clears the current
UIScrollView children and adds new child controls. The scroll position returns
to the top of the document, and the status line shows the selected file and the
number of parsed blocks.

The document list remains focused. That is deliberate. It allows the user to
move through files quickly and preview each one without pressing Enter. The
behavior is similar to a file browser or an email client preview pane.

## Mouse selection

Paragraph text can be selected with the mouse. This is important because a
terminal UI should not become a dead screenshot of text. Documentation,
snippets, identifiers, and command output should remain selectable when that is
useful.

The same selection mechanism is also used by chat messages. Dragging can cross
multiple labels inside one scroll view. When a key is pressed, the active
selection is cleared.

## Scroll view responsibilities

UIScrollView is responsible for keeping a bounded viewport while the document
itself can be much taller. It only renders the visible portion of the child
sequence. That keeps the example responsive even when the document grows.

The scroll indicator is intentionally minimal. It shows whether more content is
available above or below. The goal is not to imitate a desktop browser; the goal
is to keep terminal navigation readable.

## Layout responsibilities

The application window owns the header, sidebar, document panel, and status
line. The document view owns only the scrollable content area. Individual block
components own their own internal style and sizing.

This separation matters. If all spacing and coloring were written in one large
Python method, the example would be hard to modify and poor as a library demo.
By keeping UI definitions in Markdown files, the visual structure stays close
to the rendered result.

## Practical use cases

The same pattern can be used for:

- local help viewers
- release notes
- structured logs
- generated reports
- configuration explainers
- terminal dashboards with detail panes
- read-only previews in file managers

The important point is that the document can be generated or loaded from disk,
while the rendered UI still benefits from normal runtime controls.

## Document loading

The example loads `.markdown` files from the local `docs` directory. The files
use the `.markdown` extension so the UI compiler does not mistake them for UI
definition files during recursive compilation.

The document order is explicit for the four demo files, and any additional
`.markdown` files in the directory can be appended after them. This keeps the
example predictable while still allowing quick experiments.

## Parser limits

The parser is intentionally small. It recognizes headings, paragraphs, fenced
code blocks, and simple Markdown tables. It does not implement nested lists,
inline emphasis, links, images, HTML, or full CommonMark rules.

That limitation is useful for a GUI example. The focus is on demonstrating the
library controls, not building a Markdown engine. If a real application needed
full Markdown support, it could replace the parser and keep the same rendering
components.

## Test sequence

Use this sequence when testing the example manually:

1. Start the viewer.
2. Move through the document list with Down and Up.
3. Verify that the preview changes immediately.
4. Scroll this document with the mouse wheel.
5. Drag-select text from a paragraph.
6. Open the table document and verify table height.
7. Open the code document and verify monospaced code rendering.

## Long reading section

This section adds enough text to make scrolling unavoidable in a normal
terminal. The content is plain on purpose. The interesting behavior is not the
sentence itself, but the way the runtime preserves the window structure while
the document moves inside the viewport.

When this document is rendered, each paragraph becomes a separate child
control. The runtime is not rendering one giant string. It is laying out a
sequence of controls, each with its own natural height and style.

This also tests resize behavior. If the terminal is made narrower, paragraphs
wrap to more rows. The scroll view should still measure the document correctly.
If the terminal is made taller, more blocks should become visible without
changing the source document.

## Final notes

The end of the overview document should only be visible after scrolling. If the
whole document fits on the first screen, the demo content is too short or the
terminal is unusually large.

The other documents focus on specific block types. `styles.markdown` stresses
text styling and selection, `tables.markdown` stresses MessageTable layout, and
`code.markdown` stresses code block rendering.

## Extra scroll margin

This section gives the overview enough vertical depth for larger terminals.
Manual demos are often run in a tall terminal window, so the document should
still require scrolling even when the window is comfortable for development.

The extra content also checks that the last blocks do not jump when the scroll
view reaches the bottom. The final position should feel stable, with the status
line fixed and the document content moving only inside the content panel.

## End

This is the last block of the overview document. It exists so the bottom of the
scroll view has an obvious final target during manual testing.

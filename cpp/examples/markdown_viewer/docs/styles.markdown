# Styled Text

This document focuses on visual style, selectable text, and how reusable blocks
can keep a consistent document rhythm.

## Selectable paragraphs

Paragraph blocks enable `user-select: text`. Drag across text in the terminal
to test selection and copying. This is the same behavior used by chat messages.

Long paragraphs are useful here because they force wrapping. Wrapped text
should still be selectable in a predictable way. The user should be able to
start dragging on one visual line and continue through following wrapped lines.

Selection is not only a visual feature. In practical terminal applications,
users often need to copy IDs, file paths, commands, or short explanations. A
read-only viewer that prevents selection is less useful than a simple pager.

## Colored headings

Heading blocks use SpanLabel, so the marker and the heading text can use
different colors inside one logical element.

The current heading style is intentionally visible but not decorative. It
should make document sections easy to scan without making the screen look like
a syntax highlighting test.

The same SpanLabel mechanism is used elsewhere in the library when one logical
piece of text needs multiple foreground colors. The widget gallery demonstrates
small inline spans, while this viewer demonstrates document-level blocks.

## Runtime style states

The document list keeps focus. The selected row uses selected styling, while
the list background stays stable in edit mode. This is useful for preview-style
navigation where the user should not need to press Enter for every item.

This pattern is common in GUI applications. A list on the left controls a
preview pane on the right. Focus remains in the list, but the content panel is
updated as soon as the selection changes.

The important part is that the application logic does not need to poll the
selected item. The runtime sends a selection change event, and the viewer
rebuilds the document blocks immediately.

## Reusable block structure

The viewer is built from these reusable blocks:

- heading
- paragraph
- code_block
- table_block

Each block is a normal `.uimd` component with a small Python class that sets its
content. This keeps application logic readable and keeps visual design in the
Markdown UI files.

The block components can be changed independently. For example, the paragraph
background can be adjusted without touching the parser. The code block header
can be changed without modifying the scroll view.

## Background rhythm

The document panel alternates between block backgrounds and gaps. The gaps are
not random decoration. They help the user distinguish one semantic block from
the next while scrolling.

A terminal UI has less visual resolution than a browser or desktop app. Small
background changes and predictable vertical spacing make a long document easier
to read.

The goal is not to make every block loud. Most content should stay quiet so the
selected document, headings, tables, and code blocks can stand out when needed.

## Text wrapping

Paragraph wrapping is handled by the label runtime. A paragraph can contain
explicit newlines, or it can be one long line that wraps based on the current
available width.

When the window width changes, the natural height of wrapped text changes. The
scroll view must account for that. This document includes enough paragraphs to
make such changes visible during manual testing.

If wrapping breaks, the most obvious symptoms are clipped paragraphs, incorrect
scroll indicators, or content that overlaps the status line.

## Visual consistency

The style values used here are intentionally close to the chat example but not
identical. The viewer should feel like a documentation surface, not a chat log.

The sidebar is darker and stable. The selected document row is bright enough to
be obvious. The content area is subdued, with headings and code headers adding
structure.

Consistency is useful only when it clarifies behavior. If every example used
exactly the same colors and spacing, the library would look less flexible.

## Manual checks

Use these checks when changing styles:

- selected list item remains readable
- inactive list rows remain readable
- paragraph text is readable on the content background
- headings are visible without dominating the page
- code blocks keep their darker background
- table cells remain readable
- status line remains distinct from document content

## More text for scroll testing

This paragraph exists to extend the document. It should appear only after the
reader scrolls below the initial sections. The content repeats the same idea in
plain language because the purpose is to exercise the renderer, not to teach a
new topic.

Another paragraph adds additional wrapped rows. It should be selectable, and it
should not merge visually with the previous paragraph in a way that makes block
boundaries unclear.

The final paragraphs are useful when testing the bottom scroll indicator. As
the user approaches the end of the document, the indicator should show that
there is no more content below.

## Selection edge cases

Try selecting text from the middle of this paragraph and dragging downward past
the visible viewport. The runtime should continue to track selection through
wrapped rows and through following labels inside the scroll view.

Selection should not change the document list focus. The left list remains the
main navigation control, while the right document panel acts as the preview and
reading surface.

## Typography density

Terminal examples can become unreadable if every line is packed too tightly.
This document intentionally uses short sections and paragraph gaps so scrolling
still feels like moving through a structured document rather than a wall of
text.

The spacing also tests that block gaps are applied consistently across many
children. If one block forgets its height, the visual rhythm becomes uneven or
later content starts too early.

## End of styled text

This is the last styled text block. It should require scrolling to reach in a
normal terminal window.

# Code Blocks

Fenced code blocks are rendered with a small keyword highlighter. This is the
same SpanLabel technique used by the chat code message component.

## Simple class

```python
class Message:
    def __init__(self, user, text):
        self.user = user
        self.text = text

    def render(self):
        if self.user:
            return f"{self.user}: {self.text}"
        return self.text
```

The highlighter is intentionally small. It demonstrates colored spans rather
than full syntax parsing.

## Parsing table lines

```python
def parse_table(lines):
    rows = []
    for line in lines:
        if line.startswith("|"):
            rows.append(line)
    return rows
```

The code block keeps word wrapping disabled. That makes it useful for source
snippets, logs, or command output where preserving columns matters.

## Loading local documents

```python
def load_documents(directory):
    documents = {}
    for name in sorted(os.listdir(directory)):
        if not name.endswith(".markdown"):
            continue
        path = os.path.join(directory, name)
        with open(path, "r", encoding="utf-8") as handle:
            documents[name] = handle.read()
    return documents
```

This example is intentionally direct. It is not hiding the fact that the
document viewer reads local files from disk and turns them into runtime blocks.

## Building blocks

```python
def make_child(block):
    kind = block["kind"]
    if kind == "heading":
        child = Heading()
        child.set_heading(block["level"], block["text"])
        return child
    if kind == "code":
        child = CodeBlock()
        child.set_code(block["lang"], block["text"])
        return child
    if kind == "table":
        child = TableBlock()
        child.set_table(block["text"])
        return child
    child = Paragraph()
    child.set_text(block["text"])
    return child
```

The viewer can keep the parser simple because each block type owns its own
rendering. The application does not need to know how a table draws borders or
how a code block colors keywords.

## Event-style preview

```python
class MarkdownViewer(MarkdownViewerUI):
    def selectionchanged(self, element, value):
        if element == self.docs:
            self.show_selected()

    def confirmed(self, element, value):
        if element == self.docs:
            self.show_selected()
            self.enter_preview_mode()
```

The important behavior is immediate preview. Moving through the list should
refresh the right panel without a separate confirmation step.

## A longer function

```python
def parse_markdown(text):
    lines = text.splitlines()
    blocks = []
    paragraph = []
    index = 0

    def flush_paragraph():
        if paragraph:
            blocks.append({
                "kind": "paragraph",
                "text": "\n".join(paragraph).strip(),
            })
            paragraph.clear()

    while index < len(lines):
        line = lines[index]
        if not line.strip():
            flush_paragraph()
            index += 1
            continue
        if line.startswith("#"):
            flush_paragraph()
            level = len(line) - len(line.lstrip("#"))
            blocks.append({
                "kind": "heading",
                "level": level,
                "text": line[level:].strip(),
            })
            index += 1
            continue
        paragraph.append(line)
        index += 1

    flush_paragraph()
    return blocks
```

This block is long enough to make vertical scrolling obvious. It also checks
that indentation is preserved and that the code block background extends over
the full block height.

## Command output

Code blocks can also represent command output. The highlighter is simple, so
plain text output still remains readable.

```text
$ ./uimd generate python/examples/markdown_viewer --target python
Compiling UIMD files...

Generated 6 file(s):
  - blocks/paragraph_ui.py
  - blocks/heading_ui.py
  - blocks/code_block_ui.py
  - blocks/table_block_ui.py
  - document_view/document_view_ui.py
  - markdown_viewer_ui.py
```

This is a useful practical case. A terminal GUI often needs to show snippets of
logs, build output, diagnostics, or generated commands.

## Resize expectations

When the terminal becomes wider, code should not wrap. The line may be clipped
by the viewport if it is too long, but the visual columns should remain stable.

When the terminal becomes taller, more code lines should be visible before the
user needs to scroll. The status line and document list should remain fixed.

## End

The final code block should be reachable only after scrolling. If it appears on
the initial screen, the document is too short for this example.

import Foundation
import Uimd

private let kExpectedLayoutEntries = 8
private let kPreferredDocuments = [
    "overview.markdown",
    "styles.markdown",
    "tables.markdown",
    "code.markdown",
]

private let kPythonKeywords: Set<String> = [
    "False", "None", "True", "and", "as", "assert", "async", "await", "break", "class",
    "continue", "def", "del", "elif", "else", "except", "finally", "for", "from",
    "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass",
    "raise", "return", "try", "while", "with", "yield",
]

private struct MarkdownBlock
{
    var kind: String
    var level = 0
    var text: String
    var language = ""
}

private struct EdgeInsets
{
    var top = 0
    var right = 0
    var bottom = 0
    var left = 0
}

private func docsDir() -> String
{
    URL(fileURLWithPath: #filePath)
        .deletingLastPathComponent()
        .appendingPathComponent("docs")
        .standardizedFileURL
        .path
}

private func readTextFile(_ path: String) -> String
{
    (try? String(contentsOfFile: path, encoding: .utf8)) ?? ""
}

private func trim(_ text: String) -> String
{
    text.trimmingCharacters(in: .whitespacesAndNewlines)
}

private func splitLines(_ text: String) -> [String]
{
    text.components(separatedBy: "\n")
}

private func isTableSeparator(_ line: String) -> Bool
{
    let value = trim(line)
    if value.count < 3 || value.first != "|" || value.last != "|"
    {
        return false
    }
    return value.allSatisfy { ch in
        ch == "|" || ch == " " || ch == "-" || ch == ":"
    }
}

private func parseMarkdown(_ text: String) -> [MarkdownBlock]
{
    let lines = splitLines(text)
    var blocks: [MarkdownBlock] = []
    var paragraph: [String] = []
    var index = 0

    func flushParagraph()
    {
        let value = trim(paragraph.joined(separator: "\n"))
        if !value.isEmpty
        {
            blocks.append(MarkdownBlock(kind: "paragraph", text: value))
        }
        paragraph.removeAll()
    }

    while index < lines.count
    {
        let line = lines[index]
        if trim(line).isEmpty
        {
            flushParagraph()
            index += 1
        }
        else if line.hasPrefix("```")
        {
            flushParagraph()
            var language = trim(String(line.dropFirst(3)))
            if language.isEmpty
            {
                language = "text"
            }
            index += 1
            var codeLines: [String] = []
            while index < lines.count && !lines[index].hasPrefix("```")
            {
                codeLines.append(lines[index])
                index += 1
            }
            if index < lines.count
            {
                index += 1
            }
            blocks.append(MarkdownBlock(kind: "code", text: codeLines.joined(separator: "\n"), language: language))
        }
        else if line.first == "#"
        {
            flushParagraph()
            let level = line.prefix { $0 == "#" }.count
            let textStart = line.index(line.startIndex, offsetBy: level)
            blocks.append(MarkdownBlock(kind: "heading", level: level, text: trim(String(line[textStart...]))))
            index += 1
        }
        else if line.hasPrefix("|") && index + 1 < lines.count && isTableSeparator(lines[index + 1])
        {
            flushParagraph()
            var tableLines = [line, lines[index + 1]]
            index += 2
            while index < lines.count && lines[index].hasPrefix("|")
            {
                tableLines.append(lines[index])
                index += 1
            }
            blocks.append(MarkdownBlock(kind: "table", text: tableLines.joined(separator: "\n")))
        }
        else
        {
            paragraph.append(line)
            index += 1
        }
    }
    flushParagraph()
    return blocks.filter { !$0.text.isEmpty }
}

private func loadDocuments() -> [(name: String, text: String)]
{
    let root = docsDir()
    var documents: [(name: String, text: String)] = []
    var loaded = Set<String>()
    for name in kPreferredDocuments
    {
        let path = URL(fileURLWithPath: root).appendingPathComponent(name).path
        if FileManager.default.fileExists(atPath: path)
        {
            documents.append((name: name, text: readTextFile(path)))
            loaded.insert(name)
        }
    }
    let names = (try? FileManager.default.contentsOfDirectory(atPath: root)) ?? []
    for name in names.sorted()
    {
        if name.hasSuffix(".markdown") && !loaded.contains(name)
        {
            let path = URL(fileURLWithPath: root).appendingPathComponent(name).path
            documents.append((name: name, text: readTextFile(path)))
        }
    }
    return documents
}

private func isIdentifierChar(_ ch: Character) -> Bool
{
    ch == "_" || ch.isLetter || ch.isNumber
}

private func highlightedCodeSpans(_ source: String) -> [LabelSpan]
{
    var spans: [LabelSpan] = []
    var cursor = source.startIndex
    var index = source.startIndex
    while index < source.endIndex
    {
        if !isIdentifierChar(source[index])
        {
            index = source.index(after: index)
            continue
        }
        let start = index
        while index < source.endIndex && isIdentifierChar(source[index])
        {
            index = source.index(after: index)
        }
        let word = String(source[start..<index])
        if !kPythonKeywords.contains(word)
        {
            continue
        }
        if start > cursor
        {
            spans.append(LabelSpan(text: String(source[cursor..<start])))
        }
        spans.append(LabelSpan(text: word, foreground: Color("#c084fc")))
        cursor = index
    }
    if cursor < source.endIndex
    {
        spans.append(LabelSpan(text: String(source[cursor...])))
    }
    return spans.isEmpty ? [LabelSpan(text: source)] : spans
}

private func generatedSourceHeight(_ view: GeneratedWindowBase) -> Int
{
    max(1, view.layout.reduce(0) { height, entry in
        max(height, entry.sourceCell.row + entry.sourceCell.height)
    })
}
private func stylePaddingTop(_ style: Style) -> Int
{
    max(0, style.paddingTop ?? style.padding ?? 0)
}

private func stylePaddingRight(_ style: Style) -> Int
{
    max(0, style.paddingRight ?? style.padding ?? 0)
}

private func stylePaddingBottom(_ style: Style) -> Int
{
    max(0, style.paddingBottom ?? style.padding ?? 0)
}

private func stylePaddingLeft(_ style: Style) -> Int
{
    max(0, style.paddingLeft ?? style.padding ?? 0)
}

private func constrainedPadding(size: Size, style: Style) -> EdgeInsets
{
    let width = max(1, size.width)
    let height = max(1, size.height)
    var padding = EdgeInsets()
    padding.left = min(stylePaddingLeft(style), width - 1)
    padding.right = min(stylePaddingRight(style), width - padding.left - 1)
    padding.top = min(stylePaddingTop(style), height - 1)
    padding.bottom = min(stylePaddingBottom(style), height - padding.top - 1)
    return padding
}

private func blankRow(width: Int, style: Style) -> [TerminalCell]
{
    Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: max(1, width))
}

private func fitRow(_ row: [TerminalCell], width: Int, fillCell: TerminalCell) -> [TerminalCell]
{
    let safeWidth = max(1, width)
    if row.count > safeWidth
    {
        return Array(row.prefix(safeWidth))
    }
    if row.count < safeWidth
    {
        return row + Array(repeating: fillCell, count: safeWidth - row.count)
    }
    return row
}

private final class Paragraph: ParagraphUI
{
    func setText(_ value: String)
    {
        text.setText(value)
    }
}

private final class Heading: HeadingUI
{
    func setHeading(_ level: Int, _ value: String)
    {
        let clamped = min(max(1, level), 3)
        let color = clamped == 1 ? "#facc15" : (clamped == 2 ? "#93c5fd" : "#a7f3d0")
        heading_text.setSpans([
            LabelSpan(text: String(repeating: "#", count: clamped), foreground: Color("#64748b")),
            LabelSpan(text: " "),
            LabelSpan(text: value, foreground: Color(color)),
        ])
    }
}

private final class CodeBlock: CodeBlockUI
{
    func setCode(language: String, source: String)
    {
        lang.setText(language.isEmpty ? "text" : language)
        code.setSpans(highlightedCodeSpans(source))
    }
}

private final class TableBlock: TableBlockUI
{
    func setTable(_ value: String)
    {
        table.setText(value)
    }
}

private final class DocumentView: DocumentViewUI
{
    private var blocks: [MarkdownBlock] = []

    override init()
    {
        super.init()
        setAutoScroll(false)
        setDynamicChildrenRenderer
        {
            [weak self] width in
            self?.renderBlocks(width: width) ?? []
        }
    }

    func setBlocks(_ blocks: [MarkdownBlock])
    {
        self.blocks = blocks
        invalidateDynamicChildren()
        setAutoScroll(false)
    }

    private func renderBlocks(width: Int) -> [[[TerminalCell]]]
    {
        let blockWidth = max(1, width)
        return blocks.map { renderBlock($0, width: blockWidth) }
    }

    private func renderBlock(_ block: MarkdownBlock, width: Int) -> [[TerminalCell]]
    {
        let view: GeneratedWindowBase
        switch block.kind
        {
        case "heading":
            let heading = Heading()
            heading.setHeading(block.level, block.text)
            view = heading
        case "code":
            let code = CodeBlock()
            code.setCode(language: block.language, source: block.text)
            view = code
        case "table":
            let table = TableBlock()
            table.setTable(block.text)
            view = table
        default:
            let paragraph = Paragraph()
            paragraph.setText(block.text)
            view = paragraph
        }
        let base = view.generatedContentSize()
        let renderWidth = max(base.width, width)
        let resolved = view.generatedResolvedContentSize(Size(width: renderWidth, height: base.height))
        return view.renderContent(
            size: Size(width: max(1, renderWidth), height: max(base.height, resolved.height)),
            focusedName: nil,
            editMode: false
        )
    }
}

public final class MarkdownViewerApp: MarkdownViewerUI
{
    private let documentView = DocumentView()
    private var documents: [(name: String, text: String)] = []
    private var finished = false

    public override init()
    {
        super.init()
        viewer.setChild(documentView)
        documents = loadDocuments()
        let names = documents.map(\.name)
        docs.options = names
        if !names.isEmpty
        {
            docs.setSelectedIndex(0)
        }
        showSelected()
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "docs"
        options.startInEditMode = true
        options.onFocusChanged = { [weak self] name, focused in
            if focused
            {
                self?.focus(name)
            }
        }
        return options
    }

    public override func shouldClose() -> Bool
    {
        finished
    }

    override public func onDocsSelectionChange(_ value: [String])
    {
        _ = value
        showSelected()
    }

    override public func onUpBtnClick()
    {
        _ = documentView.handleKey("ArrowUp")
    }

    override public func onDownBtnClick()
    {
        _ = documentView.handleKey("ArrowDown")
    }

    override public func onCloseBtnClick()
    {
        finished = true
    }

    func showSelected()
    {
        if docs.options.isEmpty
        {
            documentView.setBlocks([])
            status.setText("no document - 0 blocks - arrows preview")
            return
        }
        if docs.selectedIndex < 0 || docs.selectedIndex >= docs.options.count
        {
            docs.setSelectedIndex(0)
        }
        docs.setSelectedIndex(docs.selectedIndex)
        let name = docs.options[docs.selectedIndex]
        let text = documents.first { $0.name == name }?.text ?? ""
        let blocks = parseMarkdown(text)
        documentView.setBlocks(blocks)
        status.setText(name + " - " + String(blocks.count) + " blocks - arrows preview")
    }

    func scroll(_ direction: Int)
    {
        _ = documentView.handleKey(direction > 0 ? "ArrowUp" : "ArrowDown")
    }

    func focus(_ name: String)
    {
        if name == "viewer"
        {
            status.setText("viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll")
        }
        else if name == "up_btn" || name == "down_btn"
        {
            status.setText("buttons scroll the viewer; Right focuses viewer")
        }
    }

    func documentContentHeight() -> Int
    {
        documentView.contentHeight()
    }
}

private func smoke()
{
    let app = MarkdownViewerApp()
    precondition(app.windowTitle == "Markdown Viewer")
    precondition(app.layout.count == kExpectedLayoutEntries)
    precondition(app.header != nil)
    precondition(app.docs != nil)
    precondition(app.viewer != nil)
    precondition(app.close_btn != nil)
    precondition(app.docs.options.count == kPreferredDocuments.count)
    precondition(app.status.text.contains("blocks"))
}

private func logicTest()
{
    let app = MarkdownViewerApp()
    _ = app.renderContent(size: Size(width: 90, height: 35), focusedName: nil, editMode: false)
    precondition(app.documentContentHeight() > 0)
    precondition(app.status.text.hasPrefix("overview.markdown"))
    app.docs.setSelectedIndex(2)
    app.showSelected()
    _ = app.renderContent(size: Size(width: 90, height: 35), focusedName: nil, editMode: false)
    precondition(app.status.text.hasPrefix("tables.markdown"))
    precondition(app.documentContentHeight() > 0)
    precondition(parseMarkdown("# H\n\ntext\n\n```python\nprint(1)\n```\n\n| A |\n| - |\n| B |").count == 4)
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            if CommandLine.arguments.contains("--smoke")
            {
                smoke()
                return 0
            }
            if CommandLine.arguments.contains("--logic-test")
            {
                logicTest()
                return 0
            }
            let app = MarkdownViewerApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

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
    private var viewOffset = 0
    private var lastViewportHeight = 1
    private var lastContentHeight = 1

    func setBlocks(_ blocks: [MarkdownBlock])
    {
        self.blocks = blocks
        scrollToTop()
    }

    override func scrollToTop()
    {
        viewOffset = 0
    }

    override func contentHeight() -> Int
    {
        lastContentHeight
    }

    @discardableResult
    override func handleKey(_ key: String) -> Bool
    {
        switch key
        {
        case "ArrowUp", "Up":
            scrollLines(-1)
            return true
        case "ArrowDown", "Down":
            scrollLines(1)
            return true
        case "PageUp":
            scrollLines(-max(1, lastViewportHeight - 1))
            return true
        case "PageDown":
            scrollLines(max(1, lastViewportHeight - 1))
            return true
        case "Home":
            viewOffset = 0
            return true
        case "End":
            viewOffset = maxOffset()
            return true
        default:
            return false
        }
    }

    override func scrollBy(_ delta: Int, viewport: Size)
    {
        lastViewportHeight = max(1, viewport.height)
        scrollLines(delta)
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        let panelStyle = scrollView().style
        let padding = constrainedPadding(size: size, style: panelStyle)
        let innerWidth = max(1, width - padding.left - padding.right)
        let innerHeight = max(1, height - padding.top - padding.bottom)
        lastViewportHeight = innerHeight
        let fill = TerminalCell(" ", foreground: panelStyle.color, background: panelStyle.background)
        var buffer = Array(repeating: Array(repeating: fill, count: width), count: height)
        let content = renderedBlocks(width: innerWidth, style: panelStyle)
        lastContentHeight = content.count
        viewOffset = min(max(0, viewOffset), maxOffset())
        var visibleRows: [[TerminalCell]] = []
        for row in 0..<innerHeight
        {
            let sourceRow = viewOffset + row
            if sourceRow < content.count
            {
                visibleRows.append(fitRow(content[sourceRow], width: innerWidth, fillCell: fill))
            }
            else
            {
                visibleRows.append(Array(repeating: fill, count: innerWidth))
            }
        }
        if viewOffset > 0 && !visibleRows.isEmpty
        {
            applyScrollIndicator(row: &visibleRows[0], indicator: "^")
        }
        if viewOffset + innerHeight < content.count && !visibleRows.isEmpty
        {
            applyScrollIndicator(row: &visibleRows[visibleRows.count - 1], indicator: "v")
        }
        for row in 0..<visibleRows.count
        {
            for col in 0..<min(innerWidth, visibleRows[row].count)
            {
                let targetRow = padding.top + row
                let targetCol = padding.left + col
                if targetRow < height && targetCol < width
                {
                    buffer[targetRow][targetCol] = visibleRows[row][col]
                }
            }
        }
        return buffer
    }

    private func scrollLines(_ delta: Int)
    {
        viewOffset = min(max(0, viewOffset + delta), maxOffset())
    }

    private func maxOffset() -> Int
    {
        max(0, lastContentHeight - lastViewportHeight)
    }

    private func renderedBlocks(width: Int, style: Style) -> [[TerminalCell]]
    {
        let gap = max(0, style.gap ?? 0)
        let fill = TerminalCell(" ", foreground: style.color, background: style.background)
        var rows: [[TerminalCell]] = []
        for block in blocks
        {
            if !rows.isEmpty
            {
                for _ in 0..<gap
                {
                    rows.append(Array(repeating: fill, count: max(1, width)))
                }
            }
            rows.append(contentsOf: renderBlock(block, width: width))
        }
        return rows.isEmpty ? [Array(repeating: fill, count: max(1, width))] : rows
    }

    private func firstForeground(in rows: [[TerminalCell]]) -> Color?
    {
        for row in rows
        {
            for cell in row
            {
                if let foreground = cell.foreground, !foreground.isTransparent
                {
                    return foreground
                }
            }
        }
        return nil
    }

    private func inferredForeground(in row: [TerminalCell]) -> Color?
    {
        var foreground: Color?
        for cell in row
        {
            if let color = cell.foreground, !color.isTransparent
            {
                foreground = color
            }
        }
        return foreground
    }

    private func applyScrollIndicator(row: inout [TerminalCell], indicator: String)
    {
        guard !row.isEmpty else
        {
            return
        }
        let col = row.count - 1
        row[col].text = indicator
        if let foreground = row[col].foreground, !foreground.isTransparent
        {
            return
        }
        row[col].foreground = inferredForeground(in: row)
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
        if let first = names.first
        {
            docs.selectedIndex = 0
            docs.selectedValues = [first]
        }
        showSelected()
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "docs"
        options.startInEditMode = true
        options.onKeyBeforeFocusedElement = { [weak self] key, name, editMode in
            guard let self else
            {
                return false
            }
            if name == "viewer" && editMode
            {
                return self.documentView.handleKey(key)
            }
            return false
        }
        options.onFocusChanged = { [weak self] name, focused in
            if focused
            {
                self?.focus(name)
            }
        }
        options.onKey = { [weak self] key in
            if key == "q" || key == "Q"
            {
                self?.finished = true
                return true
            }
            return false
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
            docs.selectedIndex = 0
        }
        let name = docs.options[docs.selectedIndex]
        docs.selectedValues = [name]
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
    app.docs.selectedIndex = 2
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

// Auto-generated UI code for code_block - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CodeBlockUI: GeneratedWindowBase
{
    public var lang: Label!
    public var code: SpanLabel!

    public init()
    {
        super.init("Code Block")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "CodeBlock", source: "# Code Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown code block with a language header and selectable code text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nlang:\n  type: label\n  text: code\n  description: \"Language label for the code block.\"\n\ncode:\n  type: spanlabel\n  text: \"\"\n  description: \"Rendered code content with horizontal preservation and mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@lang:\n  color: \"#93c5fd\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| lang..**...................................... |\n+-body-**----------------------------------------+\n| code..**...................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n", description: "Reusable markdown code block with a language header and selectable code text.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        lang = addElement(Label("lang", "code"))
        lang.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }())
        lang.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        code = addElement(SpanLabel("code", ""))
        code.setStyle({
                    var style = Style()
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "code",
                type: "spanlabel",
                cellName: "body",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870910),
                sourceCell: Rect(row: 2, col: 0, width: 48, height: 4),
                width: .expanded,
                height: .fitContent,
                cellWidth: .expanded,
                cellHeight: .fitContent,
                charsSize: Size(width: 46, height: 4),
                cellCharsSize: Size(width: 48, height: 4),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#0c1a2d")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.color = Color("#d1d5db")
                    style.userSelect = "text"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "lang",
                type: "label",
                cellName: "head",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 48, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 46, height: 1),
                cellCharsSize: Size(width: 48, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1d314c")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#93c5fd")
                    return style
                }()
            ),
        ]
    }


    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        return false
    }
}

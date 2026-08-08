import Darwin
import Foundation
import CUimdImageDecoder

private let kComboBoxClosedRows = 1
private let kComboBoxDropdownRows = 6
private let kCommitModeStandard = "standard"
private let kCommitModeLeave = "leave"
private let kTextTabWidth = 4
private let kModalBackgroundDimFactor = 0.5
private let kRenderTimeOverrideThreadKey = "uimd.renderTimeOverrideMs"
private let kImageCellBackgroundRenderingDepthThreadKey = "uimd.imageCellBackgroundRenderingDepth"
private let kNoActiveDynamicEditName = "__uimd_no_active_dynamic_edit__"
private let kTerminalEscapeSequenceTimeoutMilliseconds: Int32 = 100
private let kTerminalUtf8ReadDelayMicros: useconds_t = 10_000
private let kTerminalInputIdleSleepMicros: useconds_t = 10_000
private let kCopyNotificationDurationSeconds = 3.0
private let kCopyNotificationRow = 0
private let kCopyNotificationRightMargin = 1
private let kTerminalRawInputMinBytes: UInt8 = 0
private let kTerminalRawInputTimeoutDeciseconds: UInt8 = 1
private let kTerminalAnsiBaseRow = 1
private let kTerminalAnsiBaseCol = 1
private let kTerminalDefaultForeground = 39
private let kTerminalDefaultBackground = 49
private let kTerminalPasteMaxBytes = 1_048_576
private let kImageCellPixelWidth = 8
private let kImageCellPixelHeight = 16
private let kFallbackVerticalSamplesPerCell = 2
private let kSixelBitsPerGlyph = 6
private let kSixelColorComponentScale = 100
private let kSixelColorLevels = 6
private let kTestFallbackCheckerTilePixels = 4
private let kTestFallbackCheckerLightAlpha = 160
private let kTestFallbackColorQuantum = 32
private let kImageInfoSampleGridSize = 3
private let kImageInfoColorQuantum = 64
private let kFallbackUpperHalfBlock = "▀"
private let kFallbackFullBlock = "█"
private let kMissingImagePlaceholder = "image"
private let kMinimumRenderableSize = 1
private let kScrollIndicatorAbove = "^"
private let kScrollIndicatorBelow = "v"
private let kDefaultWheelScrollLines = 4
private let kWheelScrollViewportFraction = 3
private let kTextInputWheelScrollRows = 1
private let kMaxCoalescedMouseWheelDelta = 12
private let kTextInputOptionHorizontalSteps = 5
private let kTextInputOptionVerticalSteps = 3
private let kAnimatedRenderIntervalSeconds = 0.070
private let kTerminalTitleSuffix = " [swift]"
private let kCopyNotificationText = "Copied to clipboard"
private let kCopyNotificationForeground = "#ffffff"
private let kCopyNotificationBackground = "#2255bb"
private let kDialogButtonCloseDelayMicros: useconds_t = 180_000
private let kSixelFallbackWarning = "Sixel is not supported. Continue with fallback image blocks?"
private let kTerminalCellPixelQuery = "\u{001B}[16t"
private let kTerminalTextAreaPixelQuery = "\u{001B}[14t"
private let kTerminalPixelQueryDelayMicros: useconds_t = 50_000
private let kTerminalPixelQueryMaxBytes = 128
public let generatedScrollKeyboardStepRows = 1
public let generatedScrollIndicatorOverlapRows = 1
private let kTerminalEnterAlternateScreen = "\u{001B}[?1049h\u{001B}[?1000h\u{001B}[?1002h\u{001B}[?1006h\u{001B}[?2004h\u{001B}[>4;2m\u{001B}[?25l\u{001B}[2J"
private let kTerminalLeaveAlternateScreen = "\u{001B}[0m\u{001B}[2J\u{001B}[H\u{001B}[>4;0m\u{001B}[?25h\u{001B}[?2004l\u{001B}[?1006l\u{001B}[?1002l\u{001B}[?1000l\u{001B}[?1049l\r\n"
private let kTerminalSyncUpdateBegin = "\u{001B}[?2026h"
private let kTerminalSyncUpdateEnd = "\u{001B}[?2026l"

private final class RuntimeClipboard: @unchecked Sendable
{
    private let lock = NSLock()
    private var text = ""

    func store(_ value: String)
    {
        lock.lock()
        text = value
        lock.unlock()
    }

    func load() -> String
    {
        lock.lock()
        let value = text
        lock.unlock()
        return value
    }
}

private let runtimeClipboardStorage = RuntimeClipboard()

@discardableResult
private func copyTextToClipboard(_ text: String) -> Bool
{
    runtimeClipboardStorage.store(text)
    guard !text.isEmpty else
    {
        return false
    }
    for command in clipboardCommands()
    {
        if runClipboardCommand(command.fileName, arguments: command.arguments, text: text)
        {
            return true
        }
    }
    if runAppleScriptClipboardCommand(text)
    {
        return true
    }
    return false
}

private func runtimeClipboardText() -> String
{
    runtimeClipboardStorage.load()
}

private func clipboardCommands() -> [(fileName: String, arguments: [String])]
{
    [
        ("pbcopy", []),
        ("wl-copy", []),
        ("xclip", ["-selection", "clipboard"]),
        ("xsel", ["--clipboard", "--input"]),
    ]
}

private func runClipboardCommand(_ fileName: String, arguments: [String], text: String) -> Bool
{
    let process = Process()
    process.executableURL = URL(fileURLWithPath: "/usr/bin/env")
    process.arguments = [fileName] + arguments
    let input = Pipe()
    process.standardInput = input
    process.standardOutput = Pipe()
    process.standardError = Pipe()
    do
    {
        try process.run()
        input.fileHandleForWriting.write(Data(text.utf8))
        try input.fileHandleForWriting.close()
        process.waitUntilExit()
        return process.terminationStatus == 0
    }
    catch
    {
        return false
    }
}

private func runAppleScriptClipboardCommand(_ text: String) -> Bool
{
    let process = Process()
    process.executableURL = URL(fileURLWithPath: "/usr/bin/osascript")
    process.arguments = [
        "-e",
        "on run argv",
        "-e",
        "set the clipboard to item 1 of argv",
        "-e",
        "end run",
        "--",
        text,
    ]
    process.standardOutput = Pipe()
    process.standardError = Pipe()
    do
    {
        try process.run()
        process.waitUntilExit()
        return process.terminationStatus == 0
    }
    catch
    {
        return false
    }
}

public struct Rect: Equatable
{
    public var row: Int
    public var col: Int
    public var width: Int
    public var height: Int

    public init(row: Int, col: Int, width: Int, height: Int)
    {
        self.row = row
        self.col = col
        self.width = width
        self.height = height
    }
}

public struct Size: Equatable
{
    public var width: Int
    public var height: Int

    public init(width: Int, height: Int)
    {
        self.width = width
        self.height = height
    }
}

public struct Point: Equatable
{
    public var row: Int
    public var col: Int

    public init(row: Int, col: Int)
    {
        self.row = row
        self.col = col
    }
}

public enum AxisDimension: Equatable
{
    case auto(Int)
    case fixed(Int)
    case expanded
    case fitContent
}

public struct Color: Equatable
{
    public var text: String

    public init(_ text: String = "")
    {
        self.text = text
    }

    public var snapshotValue: String?
    {
        if text.isEmpty || text == "transparent"
        {
            return nil
        }
        if text.hasPrefix("#") && text.count == 9
        {
            return String(text.prefix(7)).lowercased()
        }
        return text.lowercased()
    }

    public var isTransparent: Bool
    {
        text.isEmpty || text == "transparent"
    }

    public var rgba: (red: Int, green: Int, blue: Int, alpha: Int)?
    {
        let value = text.lowercased()
        guard value.hasPrefix("#") else
        {
            return nil
        }
        let hex = String(value.dropFirst())
        func byte(_ part: String) -> Int?
        {
            Int(part, radix: 16)
        }
        if hex.count == 3
        {
            let chars = Array(hex)
            guard
                let red = byte(String([chars[0], chars[0]])),
                let green = byte(String([chars[1], chars[1]])),
                let blue = byte(String([chars[2], chars[2]]))
            else
            {
                return nil
            }
            return (red, green, blue, 255)
        }
        if hex.count == 6 || hex.count == 8
        {
            let chars = Array(hex)
            guard
                let red = byte(String(chars[0..<2])),
                let green = byte(String(chars[2..<4])),
                let blue = byte(String(chars[4..<6]))
            else
            {
                return nil
            }
            let alpha = hex.count == 8 ? (byte(String(chars[6..<8])) ?? 255) : 255
            return (red, green, blue, alpha)
        }
        return nil
    }

    public func blended(over background: Color?) -> Color?
    {
        if isOpaqueHexColor(text)
        {
            return self
        }
        guard let foreground = rgba, foreground.alpha < 255 else
        {
            return self
        }
        guard foreground.alpha > 0 else
        {
            return background
        }
        guard let base = background?.rgba else
        {
            return self
        }
        let alpha = Double(foreground.alpha) / 255.0
        func channel(_ fg: Int, _ bg: Int) -> Int
        {
            Int((alpha * Double(fg) + (1.0 - alpha) * Double(bg)).rounded())
        }
        return Color.rgb(
            red: channel(foreground.red, base.red),
            green: channel(foreground.green, base.green),
            blue: channel(foreground.blue, base.blue)
        )
    }

    public static func rgb(red: Int, green: Int, blue: Int) -> Color
    {
        Color(String(format: "#%02x%02x%02x", red, green, blue))
    }
}

private func isOpaqueHexColor(_ text: String) -> Bool
{
    guard text.first == "#" else
    {
        return false
    }
    let hexCount = text.count - 1
    if hexCount == 3 || hexCount == 6
    {
        return true
    }
    if hexCount != 8
    {
        return false
    }
    let alphaStart = text.index(text.endIndex, offsetBy: -2)
    return text[alphaStart...].lowercased() == "ff"
}

public struct TextGradient: Equatable
{
    public static let defaultIntervalMs = 70

    public var intervalMs: Int = TextGradient.defaultIntervalMs
    public var step: Int = 1
    public var segmentSize: Int = 1
    public var colors: [Color] = []

    public init(intervalMs: Int = TextGradient.defaultIntervalMs, step: Int = 1, segmentSize: Int = 1, colors: [Color] = [])
    {
        self.intervalMs = intervalMs
        self.step = step
        self.segmentSize = segmentSize
        self.colors = colors
    }
}

public struct Style: Equatable
{
    public var color: Color?
    public var background: Color?
    public var backgroundTexture: String?
    public var backgroundTextureColor: Color?
    public var scopeDimBackground: Color? = Color("#00000066")
    public var borderColor: Color?
    public var borderWidthHorizontal: Int?
    public var borderWidthVertical: Int?
    public var padding: Int?
    public var paddingTop: Int?
    public var paddingRight: Int?
    public var paddingBottom: Int?
    public var paddingLeft: Int?
    public var margin: Int?
    public var marginTop: Int?
    public var marginRight: Int?
    public var marginBottom: Int?
    public var marginLeft: Int?
    public var gap: Int?
    public var textAlign: String = ""
    public var userSelect: String = ""
    public var scrollX: Bool?
    public var scrollY: Bool?
    public var textColorGradient: TextGradient?
    public var textBackgroundGradient: TextGradient?

    public init()
    {
    }

    public func merged(_ other: Style?) -> Style
    {
        guard let other else
        {
            return self
        }
        var result = self
        result.color = other.color ?? result.color
        result.background = other.background ?? result.background
        result.backgroundTexture = other.backgroundTexture ?? result.backgroundTexture
        result.backgroundTextureColor = other.backgroundTextureColor ?? result.backgroundTextureColor
        result.scopeDimBackground = other.scopeDimBackground ?? result.scopeDimBackground
        result.borderColor = other.borderColor ?? result.borderColor
        result.borderWidthHorizontal = other.borderWidthHorizontal ?? result.borderWidthHorizontal
        result.borderWidthVertical = other.borderWidthVertical ?? result.borderWidthVertical
        result.padding = other.padding ?? result.padding
        result.paddingTop = other.paddingTop ?? result.paddingTop
        result.paddingRight = other.paddingRight ?? result.paddingRight
        result.paddingBottom = other.paddingBottom ?? result.paddingBottom
        result.paddingLeft = other.paddingLeft ?? result.paddingLeft
        result.margin = other.margin ?? result.margin
        result.marginTop = other.marginTop ?? result.marginTop
        result.marginRight = other.marginRight ?? result.marginRight
        result.marginBottom = other.marginBottom ?? result.marginBottom
        result.marginLeft = other.marginLeft ?? result.marginLeft
        result.gap = other.gap ?? result.gap
        result.textAlign = other.textAlign.isEmpty ? result.textAlign : other.textAlign
        result.userSelect = other.userSelect.isEmpty ? result.userSelect : other.userSelect
        result.scrollX = other.scrollX ?? result.scrollX
        result.scrollY = other.scrollY ?? result.scrollY
        result.textColorGradient = other.textColorGradient ?? result.textColorGradient
        result.textBackgroundGradient = other.textBackgroundGradient ?? result.textBackgroundGradient
        return result
    }

    fileprivate func mergedListBoxRowLayer(_ other: Style?) -> Style
    {
        guard let other else
        {
            return self
        }
        let inheritedBackground = background
        var result = merged(other)
        if let layerBackground = other.background,
           let alpha = layerBackground.rgba?.alpha,
           alpha < 255,
           inheritedBackground != nil
        {
            result.background = layerBackground.blended(over: inheritedBackground)
        }
        return result
    }
}

public struct TerminalCell: Equatable
{
    public var text: String
    public var foreground: Color?
    public var background: Color?
    public var raw: String
    public var rawWidth: Int
    public var rawHeight: Int
    public var rawSkip: Bool

    public init(
        _ text: String = " ",
        foreground: Color? = nil,
        background: Color? = nil,
        raw: String = "",
        rawWidth: Int = 0,
        rawHeight: Int = 0,
        rawSkip: Bool = false
    )
    {
        self.text = text.isEmpty ? " " : text
        self.foreground = foreground
        self.background = background
        self.raw = raw
        self.rawWidth = rawWidth
        self.rawHeight = rawHeight
        self.rawSkip = rawSkip
    }
}

public struct ElementRenderState
{
    public var focused: Bool = false
    public var editMode: Bool = false
    public var parentBackground: Color?
    public var clipTop: Int?
    public var clipBottom: Int?

    public init(
        focused: Bool = false,
        editMode: Bool = false,
        parentBackground: Color? = nil,
        clipTop: Int? = nil,
        clipBottom: Int? = nil
    )
    {
        self.focused = focused
        self.editMode = editMode
        self.parentBackground = parentBackground
        self.clipTop = clipTop
        self.clipBottom = clipBottom
    }
}

private func hasPartialAlpha(_ color: Color?) -> Bool
{
    guard let color else
    {
        return false
    }
    if color.isTransparent || isOpaqueHexColor(color.text)
    {
        return false
    }
    guard let rgba = color.rgba else
    {
        return false
    }
    return rgba.alpha < 255
}

private func mergedStateStyle(_ baseStyle: Style, _ stateStyle: Style, parentBackground: Color?) -> Style
{
    var base = baseStyle
    var baseBackground = base.background
    let stateBackground = stateStyle.background
    if hasPartialAlpha(baseBackground),
       let parent = parentBackground,
       parent.rgba != nil
    {
        base.background = baseBackground?.blended(over: parent)
        baseBackground = base.background
    }
    base = base.merged(stateStyle)
    if hasPartialAlpha(stateBackground),
       let inherited = baseBackground,
       inherited.rgba != nil,
       stateBackground?.rgba?.alpha == 0
    {
        base.background = inherited
    }
    else if hasPartialAlpha(stateBackground),
            let inherited = baseBackground,
            inherited.rgba != nil
    {
        base.background = stateBackground?.blended(over: inherited)
    }
    return base
}

public struct GeneratedLayoutEntry: Equatable
{
    public var name: String
    public var type: String
    public var cellName: String
    public var relative: Rect
    public var sourceCell: Rect
    public var width: AxisDimension
    public var height: AxisDimension
    public var cellWidth: AxisDimension
    public var cellHeight: AxisDimension
    public var charsSize: Size
    public var cellCharsSize: Size
    public var marginRight: Int
    public var marginBottom: Int
    public var cellStyle: Style
    public var elementStyle: Style

    public init(
        name: String,
        type: String,
        cellName: String,
        relative: Rect,
        sourceCell: Rect,
        width: AxisDimension,
        height: AxisDimension,
        cellWidth: AxisDimension,
        cellHeight: AxisDimension,
        charsSize: Size,
        cellCharsSize: Size,
        marginRight: Int,
        marginBottom: Int,
        cellStyle: Style = Style(),
        elementStyle: Style = Style()
    )
    {
        self.name = name
        self.type = type
        self.cellName = cellName
        self.relative = relative
        self.sourceCell = sourceCell
        self.width = width
        self.height = height
        self.cellWidth = cellWidth
        self.cellHeight = cellHeight
        self.charsSize = charsSize
        self.cellCharsSize = cellCharsSize
        self.marginRight = marginRight
        self.marginBottom = marginBottom
        self.cellStyle = cellStyle
        self.elementStyle = elementStyle
    }
}

open class UIElement
{
    public let name: String
    public var mcpElementId: String?
    public var parentFocusHostId: String?
    public var childFocusOrder: [String] = []
    public var lastFocusedChildId: String?
    public var childFocusScopeGeneration = 0
    public var enabled = true
    public var focusable = true
    public var commitMode = kCommitModeStandard
    public var frame = Rect(row: 0, col: 0, width: 0, height: 0)
    public var style = Style()
    public var focusStyle: Style?
    public var editStyle: Style?
    public var cursorStyle: Style?
    public var selectedStyle: Style?
    public var activeStyle: Style?
    public var checkedStyle: Style?
    public var uncheckedStyle: Style?
    public var disabledStyle: Style?
    public var children: [UIElement] = []

    public init(_ name: String)
    {
        self.name = name
    }

    @discardableResult
    open func addChild<T: UIElement>(_ child: T) -> T
    {
        children.append(child)
        return child
    }

    public func clearChildElements()
    {
        children.removeAll()
    }

    open var terminalText: String
    {
        name
    }

    open var valueForSnapshot: Any?
    {
        terminalText
    }

    open var elementType: String
    {
        "element"
    }

    open var role: String
    {
        "text"
    }

    public func setStyle(_ value: Style)
    {
        style = value
    }

    public func setFocusStyle(_ value: Style)
    {
        focusStyle = value
    }

    public func setEditStyle(_ value: Style)
    {
        editStyle = value
    }

    public func setCursorStyle(_ value: Style)
    {
        cursorStyle = value
    }

    public func setSelectedStyle(_ value: Style)
    {
        selectedStyle = value
    }

    public func setActiveStyle(_ value: Style)
    {
        activeStyle = value
    }

    public func setCheckedStyle(_ value: Style)
    {
        checkedStyle = value
    }

    public func setUncheckedStyle(_ value: Style)
    {
        uncheckedStyle = value
    }

    public func setDisabledStyle(_ value: Style)
    {
        disabledStyle = value
    }

    public func effectiveStyle(_ state: ElementRenderState = ElementRenderState()) -> Style
    {
        if !enabled
        {
            guard let disabledStyle else
            {
                return style
            }
            return mergedStateStyle(style, disabledStyle, parentBackground: state.parentBackground)
        }
        if state.editMode
        {
            if let editStyle
            {
                return mergedStateStyle(style, editStyle, parentBackground: state.parentBackground)
            }
        }
        if state.focused
        {
            guard let focusStyle else
            {
                return style
            }
            return mergedStateStyle(style, focusStyle, parentBackground: state.parentBackground)
        }
        return style
    }

    open func setText(_ value: String)
    {
        _ = value
    }

    open func handleKey(_ key: String) -> Bool
    {
        _ = key
        return false
    }

    open func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        renderPlainText(terminalText, width: safeWidth(size.width, terminalText), height: safeHeight(size.height), style: effectiveStyle(state))
    }
}

open class Label: UIElement
{
    public var text: String

    public init(_ name: String, _ text: String)
    {
        self.text = text
        super.init(name)
        focusable = false
    }

    open override var terminalText: String
    {
        text
    }

    open override var elementType: String
    {
        "label"
    }

    open override func setText(_ value: String)
    {
        text = value
    }
}

public struct LabelSpan
{
    public var text: String
    public var foreground: Color?
    public var background: Color?

    public init(text: String, foreground: Color? = nil, background: Color? = nil)
    {
        self.text = text
        self.foreground = foreground
        self.background = background
    }
}

open class SpanLabel: Label
{
    public var spans: [LabelSpan] = []

    public func setSpans(_ spans: [LabelSpan])
    {
        self.spans = spans
        text = spans.map(\.text).joined()
    }

    open override var elementType: String
    {
        "spanlabel"
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        guard !spans.isEmpty else
        {
            return super.render(size: size, state: state)
        }
        let width = safeWidth(size.width, text)
        let height = safeHeight(size.height)
        let base = effectiveStyle(state)
        let blank = TerminalCell(" ", foreground: base.color, background: base.background)
        var rows: [[TerminalCell]] = []
        var current: [TerminalCell] = []

        func finishRow()
        {
            while current.count < width
            {
                current.append(blank)
            }
            rows.append(Array(current.prefix(width)))
            current.removeAll()
        }

        for span in spans
        {
            for character in span.text
            {
                if rows.count >= height
                {
                    break
                }
                if character == "\r"
                {
                    continue
                }
                if character == "\n"
                {
                    finishRow()
                    continue
                }
                if current.count >= width
                {
                    finishRow()
                    if rows.count >= height
                    {
                        break
                    }
                }
                current.append(TerminalCell(
                    String(character),
                    foreground: span.foreground ?? base.color,
                    background: span.background ?? base.background
                ))
            }
        }
        if rows.count < height
        {
            finishRow()
        }
        while rows.count < height
        {
            rows.append(Array(repeating: blank, count: width))
        }
        return Array(rows.prefix(height))
    }
}

open class InfoLabel: Label
{
    open override var elementType: String
    {
        "infolabel"
    }
}

open class FrameBufferView: UIElement
{
    open override var valueForSnapshot: Any?
    {
        NSNull()
    }
}

open class MessageTable: Label
{
    open override var elementType: String
    {
        "messagetable"
    }

    public func fitContentHeight() -> Int
    {
        guard let table = parseMarkdownTable(text) else
        {
            return 1
        }
        return max(1, table.rows.count + 4)
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = max(1, size.width)
        let style = effectiveStyle(state)
        guard let table = parseMarkdownTable(text) else
        {
            return [Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: width)]
        }
        let widths = expandColumnWidths(columnWidths(table), targetWidth: width)
        var rows: [[TerminalCell]] = []
        rows.append(separatorRow(widths: widths, style: style, left: "┌", middle: "┬", right: "┐"))
        rows.append(dataRow(cells: table.headers, widths: widths, alignments: table.alignments, style: style))
        rows.append(separatorRow(widths: widths, style: style, left: "├", middle: "┼", right: "┤"))
        for row in table.rows
        {
            rows.append(dataRow(cells: row, widths: widths, alignments: table.alignments, style: style))
        }
        rows.append(separatorRow(widths: widths, style: style, left: "└", middle: "┴", right: "┘"))
        return rows
    }

    private enum TableAlignment
    {
        case left
        case center
        case right
    }

    private struct ParsedTable
    {
        var headers: [String]
        var alignments: [TableAlignment]
        var rows: [[String]]
    }

    private func parseMarkdownTable(_ value: String) -> ParsedTable?
    {
        let lines = value.components(separatedBy: "\n")
        guard lines.count >= 3 else
        {
            return nil
        }
        let headers = splitTableCells(lines[0])
        let separator = splitTableCells(lines[1])
        guard !headers.isEmpty, headers.count == separator.count, isMarkdownSeparatorRow(separator) else
        {
            return nil
        }
        var rows: [[String]] = []
        for line in lines.dropFirst(2)
        {
            let row = splitTableCells(line)
            if !row.isEmpty && row.count == headers.count
            {
                rows.append(row)
            }
        }
        if rows.isEmpty
        {
            return nil
        }
        return ParsedTable(headers: headers, alignments: parseAlignments(separator), rows: rows)
    }

    private func splitTableCells(_ line: String) -> [String]
    {
        var value = line
        if value.first == "|"
        {
            value.removeFirst()
        }
        if value.last == "|"
        {
            value.removeLast()
        }
        return value.split(separator: "|", omittingEmptySubsequences: false)
            .map { String($0).trimmingCharacters(in: .whitespacesAndNewlines) }
    }

    private func isMarkdownSeparatorRow(_ cells: [String]) -> Bool
    {
        if cells.isEmpty
        {
            return false
        }
        for cell in cells
        {
            let compact = cell.trimmingCharacters(in: .whitespacesAndNewlines)
            if compact.isEmpty
            {
                return false
            }
            for ch in compact
            {
                if ch != "-" && ch != ":"
                {
                    return false
                }
            }
        }
        return true
    }

    private func parseAlignments(_ cells: [String]) -> [TableAlignment]
    {
        cells.map { cell in
            let compact = cell.trimmingCharacters(in: .whitespacesAndNewlines)
            let left = compact.first == ":"
            let right = compact.last == ":"
            if left && right
            {
                return .center
            }
            if right
            {
                return .right
            }
            return .left
        }
    }

    private func columnWidths(_ table: ParsedTable) -> [Int]
    {
        var widths = table.headers.map { max(0, $0.count) }
        for row in table.rows
        {
            for column in 0..<min(row.count, widths.count)
            {
                widths[column] = max(widths[column], row[column].count)
            }
        }
        return widths
    }

    private func renderedTableWidth(_ widths: [Int]) -> Int
    {
        if widths.isEmpty
        {
            return 0
        }
        return 1 + widths.reduce(0) { $0 + $1 + 2 } + (widths.count - 1) + 1
    }

    private func expandColumnWidths(_ widths: [Int], targetWidth: Int) -> [Int]
    {
        guard !widths.isEmpty else
        {
            return widths
        }
        let extra = targetWidth - renderedTableWidth(widths)
        guard extra > 0 else
        {
            return widths
        }
        let perColumn = extra / widths.count
        let remainder = extra % widths.count
        return widths.enumerated().map { index, width in
            width + perColumn + (index < remainder ? 1 : 0)
        }
    }

    private func alignedText(_ text: String, width: Int, alignment: TableAlignment) -> String
    {
        if text.count >= width
        {
            return String(text.prefix(width))
        }
        let padding = width - text.count
        switch alignment
        {
        case .right:
            return String(repeating: " ", count: padding) + text
        case .center:
            let left = padding / 2
            return String(repeating: " ", count: left) + text + String(repeating: " ", count: padding - left)
        case .left:
            return text + String(repeating: " ", count: padding)
        }
    }

    private func rowFromText(_ text: String, style: Style) -> [TerminalCell]
    {
        text.map { TerminalCell(String($0), foreground: style.color, background: style.background) }
    }

    private func separatorRow(widths: [Int], style: Style, left: String, middle: String, right: String) -> [TerminalCell]
    {
        var text = left
        for column in 0..<widths.count
        {
            text += String(repeating: "─", count: widths[column] + 2)
            text += column + 1 < widths.count ? middle : right
        }
        return rowFromText(text, style: style)
    }

    private func dataRow(cells: [String], widths: [Int], alignments: [TableAlignment], style: Style) -> [TerminalCell]
    {
        var text = "│"
        for column in 0..<widths.count
        {
            let value = column < cells.count ? alignedText(cells[column], width: widths[column], alignment: alignments[column]) : String(repeating: " ", count: widths[column])
            text += " " + value + " "
            text += "│"
        }
        return rowFromText(text, style: style)
    }
}

open class Image: UIElement
{
    public var source: String
    public var alt: String
    public var fit: String
    public var renderMode: String
    public var align: String
    public var verticalAlign: String

    public init(
        _ name: String,
        source: String = "",
        alt: String = "",
        fit: String = "contain",
        renderMode: String = "auto",
        align: String = "center",
        verticalAlign: String = "middle"
    )
    {
        self.source = source
        self.alt = alt
        self.fit = fit
        self.renderMode = renderMode
        self.align = align
        self.verticalAlign = verticalAlign
        super.init(name)
    }

    open override var terminalText: String
    {
        alt.isEmpty ? source : alt
    }

    open override var valueForSnapshot: Any?
    {
        source
    }

    open override var elementType: String
    {
        "image"
    }

    public func setSource(_ value: String)
    {
        source = value
    }

    public func setAlt(_ value: String)
    {
        alt = value
    }

    public func setFit(_ value: String)
    {
        fit = value
    }

    public func setRenderMode(_ value: String)
    {
        renderMode = value
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = max(1, size.width)
        let height = max(1, size.height)
        let style = effectiveStyle(state)
        let sourcePath = resolvedImagePath(source)
        guard let raster = loadImageRaster(sourcePath) else
        {
            let text = alt.isEmpty ? (URL(fileURLWithPath: source).lastPathComponent.isEmpty ? kMissingImagePlaceholder : URL(fileURLWithPath: source).lastPathComponent) : alt
            return imagePlaceholderContent(width: width, height: height, style: style, text: text.isEmpty ? kMissingImagePlaceholder : text)
        }
        let background = imageFallbackBackground(style: style, state: state)
        if currentImageCellBackgroundRenderingDepth() > 0
        {
            let key = imageRenderedContentCacheKey(
                source: sourcePath,
                mode: "cell_background",
                width: width,
                height: height,
                fit: fit,
                align: align,
                verticalAlign: verticalAlign,
                style: style,
                background: background
            )
            return cachedImageRenderedContent(key: key)
            {
                cellBackgroundImageContent(
                    raster: raster,
                    width: width,
                    height: height,
                    fit: fit,
                    align: align,
                    verticalAlign: verticalAlign,
                    style: style,
                    background: background
                )
            }
        }
        let resolvedMode = resolvedImageRenderMode(self)
        if resolvedMode == "sixel"
        {
            let region = imageCellRegion(
                width: width,
                height: height,
                sourceWidth: raster.width,
                sourceHeight: raster.height,
                fit: fit,
                align: align,
                verticalAlign: verticalAlign
            )
            var visibleTop = region.row
            var visibleBottom = region.row + region.height
            if state.clipTop != nil || state.clipBottom != nil
            {
                visibleTop = max(visibleTop, state.clipTop ?? 0)
                visibleBottom = min(visibleBottom, state.clipBottom ?? height)
            }
            let visibleRows = max(0, visibleBottom - visibleTop)
            let regionFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain" ? "cover" : fit
            let raw = visibleRows > 0
                ? cachedSixelPayload(
                    source: sourcePath,
                    raster: raster,
                    width: region.width,
                    height: visibleRows,
                    fit: regionFit,
                    align: align,
                    verticalAlign: verticalAlign,
                    background: background,
                    sourceHeight: region.height,
                    cropTop: visibleTop - region.row
                )
                : ""
            if !raw.isEmpty
            {
                var content = imageBlankContent(width: width, height: height, style: style)
                for row in visibleTop..<visibleBottom where row >= 0 && row < content.count
                {
                    for col in region.col..<(region.col + region.width) where col >= 0 && col < content[row].count
                    {
                        content[row][col].rawSkip = true
                    }
                }
                if visibleTop >= 0,
                   visibleTop < content.count,
                   region.col >= 0,
                   region.col < content[visibleTop].count
                {
                    content[visibleTop][region.col].raw = raw
                    content[visibleTop][region.col].rawWidth = region.width
                    content[visibleTop][region.col].rawHeight = visibleRows
                    content[visibleTop][region.col].rawSkip = false
                    return content
                }
            }
            return imageBlankContent(width: width, height: height, style: style)
        }
        if resolvedMode != "deterministic"
        {
            let key = imageRenderedContentCacheKey(
                source: sourcePath,
                mode: "fallback",
                width: width,
                height: height,
                fit: fit,
                align: align,
                verticalAlign: verticalAlign,
                style: style,
                background: background
            )
            return cachedImageRenderedContent(key: key)
            {
                fallbackImageContent(
                    raster: raster,
                    width: width,
                    height: height,
                    fit: fit,
                    align: align,
                    verticalAlign: verticalAlign,
                    style: style,
                    background: background
                )
            }
        }
        let checker = renderMode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() != "fallback"
        let quantized = resolvedMode == "deterministic"
        let key = imageRenderedContentCacheKey(
            source: sourcePath,
            mode: "deterministic",
            width: width,
            height: height,
            fit: fit,
            align: align,
            verticalAlign: verticalAlign,
            style: style,
            background: background,
            checker: checker,
            quantized: quantized
        )
        return cachedImageRenderedContent(key: key)
        {
            deterministicImageContent(
                raster: raster,
                width: width,
                height: height,
                fit: fit,
                align: align,
                verticalAlign: verticalAlign,
                style: style,
                background: background,
                checker: checker,
                quantized: quantized
            )
        }
    }
}

open class Button: UIElement
{
    public var title: String

    public init(_ name: String, _ title: String)
    {
        self.title = title
        super.init(name)
    }

    open override var terminalText: String
    {
        title
    }

    open override var elementType: String
    {
        "button"
    }

    open override var role: String
    {
        "action"
    }

    public func setTitle(_ value: String)
    {
        title = value
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = safeWidth(size.width, title)
        var body = title
        let titleWidth = title.count
        if state.focused && width >= titleWidth + 2
        {
            if width >= titleWidth + 6
            {
                body = " [" + centerText(" " + title + " ", width: width - 4) + "] "
            }
            else if width >= titleWidth + 4
            {
                body = " [" + centerText(title, width: width - 4) + "] "
            }
            else
            {
                body = "[" + title + "]"
            }
        }
        return renderPlainText(centerText(body, width: width), width: width, height: 1, style: effectiveStyle(state))
    }
}

open class CheckBox: UIElement
{
    public var title: String
    public var checked: Bool

    public init(_ name: String, _ title: String, _ checked: Bool = false)
    {
        self.title = title
        self.checked = checked
        super.init(name)
    }

    open override var terminalText: String
    {
        "\(checked ? "[x]" : "[ ]") \(title)"
    }

    open override var valueForSnapshot: Any?
    {
        checked
    }

    open override var elementType: String
    {
        "checkbox"
    }

    open override var role: String
    {
        "toggle"
    }

    public func setChecked(_ value: Bool)
    {
        checked = value
    }

    open override func handleKey(_ key: String) -> Bool
    {
        if key == "Enter" || key == " "
        {
            checked.toggle()
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let text = terminalText
        let width = safeWidth(size.width, text)
        let base = effectiveStyle(state)
        var row = renderPlainText(text, width: width, height: 1, style: base)[0]
        var indicatorStyle = base.merged(checked ? checkedStyle : uncheckedStyle)
        if (state.focused || state.editMode),
           indicatorStyle.background?.isTransparent == true,
           let inheritedBackground = base.background
        {
            indicatorStyle.background = inheritedBackground
        }
        for index in 0..<min(3, width)
        {
            row[index].foreground = indicatorStyle.color
            row[index].background = indicatorStyle.background
        }
        return [row]
    }
}

open class TextInput: UIElement
{
    public var value: String
    public var maxLength: Int
    public var cursor = 0
    public var selectionStart: Int?
    public var columnScrollOffset = 0
    public var isMultiline = false

    public init(_ name: String, _ value: String = "", maxLength: Int = 0)
    {
        self.value = value
        self.maxLength = maxLength
        self.cursor = value.count
        super.init(name)
    }

    open override var terminalText: String
    {
        value
    }

    open override var valueForSnapshot: Any?
    {
        value
    }

    open override var elementType: String
    {
        "textinput"
    }

    open override var role: String
    {
        "input"
    }

    public func setValue(_ value: String)
    {
        self.value = value
        cursor = value.count
        selectionStart = nil
    }

    open override func setText(_ value: String)
    {
        setValue(value)
    }

    public func insertText(_ text: String)
    {
        var previousWasCarriageReturn = false
        for character in text
        {
            if character == "\r"
            {
                _ = handleKey(isMultiline ? "Enter" : " ")
                previousWasCarriageReturn = true
                continue
            }
            if character == "\n"
            {
                if previousWasCarriageReturn
                {
                    previousWasCarriageReturn = false
                    continue
                }
                _ = handleKey(isMultiline ? "Enter" : " ")
                continue
            }
            previousWasCarriageReturn = false
            _ = handleKey(String(character))
        }
    }

    fileprivate func insertLiteralText(_ text: String)
    {
        if hasSelection
        {
            replaceSelection(text)
            return
        }
        var result = value
        let index = result.index(result.startIndex, offsetBy: max(0, min(cursor, result.count)))
        result.insert(contentsOf: text, at: index)
        if maxLength > 0 && result.count > maxLength
        {
            result = String(result.prefix(maxLength))
        }
        value = result
        cursor = min(value.count, cursor + text.count)
        selectionStart = nil
    }

    public var hasSelection: Bool
    {
        guard let selectionStart else
        {
            return false
        }
        return selectionStart != cursor
    }

    public func selectionBounds() -> (start: Int, end: Int)?
    {
        guard let selectionStart else
        {
            return nil
        }
        let start = max(0, min(min(selectionStart, cursor), value.count))
        let end = max(0, min(max(selectionStart, cursor), value.count))
        return start == end ? nil : (start, end)
    }

    public func selectedText() -> String
    {
        guard let bounds = selectionBounds() else
        {
            return ""
        }
        let startIndex = value.index(value.startIndex, offsetBy: bounds.start)
        let endIndex = value.index(value.startIndex, offsetBy: bounds.end)
        return String(value[startIndex..<endIndex])
    }

    public func selectRange(start: Int, end: Int)
    {
        let length = value.count
        selectionStart = max(0, min(start, length))
        cursor = max(0, min(end, length))
    }

    public func clearSelection()
    {
        selectionStart = nil
    }

    public func replaceSelection(_ text: String)
    {
        guard let bounds = selectionBounds() else
        {
            insertText(text)
            return
        }
        var result = value
        let startIndex = result.index(result.startIndex, offsetBy: bounds.start)
        let endIndex = result.index(result.startIndex, offsetBy: bounds.end)
        result.replaceSubrange(startIndex..<endIndex, with: text)
        if maxLength > 0 && result.count > maxLength
        {
            result = String(result.prefix(maxLength))
        }
        value = result
        cursor = min(value.count, bounds.start + text.count)
        selectionStart = nil
    }

    open func cursorForPoint(row: Int, col: Int, size: Size) -> Int
    {
        _ = row
        _ = size
        return clamped(columnScrollOffset + col, lower: 0, upper: value.count)
    }

    fileprivate func deleteSelection()
    {
        guard let bounds = selectionBounds() else
        {
            return
        }
        var result = value
        let startIndex = result.index(result.startIndex, offsetBy: bounds.start)
        let endIndex = result.index(result.startIndex, offsetBy: bounds.end)
        result.removeSubrange(startIndex..<endIndex)
        value = result
        cursor = bounds.start
        selectionStart = nil
    }

    fileprivate func lineStartForCursor() -> Int
    {
        guard cursor > 0 else
        {
            return 0
        }
        let prefix = Array(value.prefix(cursor))
        if let newline = prefix.lastIndex(of: "\n")
        {
            return newline + 1
        }
        return 0
    }

    fileprivate func lineEndForCursor() -> Int
    {
        let chars = Array(value)
        guard cursor < chars.count else
        {
            return chars.count
        }
        if let newline = chars[cursor...].firstIndex(of: "\n")
        {
            return newline
        }
        return chars.count
    }

    open override func handleKey(_ key: String) -> Bool
    {
        if key == "cmd_c"
        {
            let selected = selectedText()
            _ = copyTextToClipboard(selected.isEmpty ? value : selected)
            return true
        }
        if key == "cmd_v"
        {
            let text = runtimeClipboardText()
            if !text.isEmpty
            {
                insertText(text)
            }
            return true
        }
        if key == "Shift+Left"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            cursor = max(0, cursor - 1)
            return true
        }
        if key == "Shift+Right"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            cursor = min(value.count, cursor + 1)
            return true
        }
        if key == "Shift+Home"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            cursor = lineStartForCursor()
            return true
        }
        if key == "Shift+End"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            cursor = lineEndForCursor()
            return true
        }
        if key == "Backspace"
        {
            if hasSelection
            {
                deleteSelection()
                return true
            }
            guard cursor > 0 else
            {
                return true
            }
            var result = value
            let removeIndex = result.index(result.startIndex, offsetBy: cursor - 1)
            result.remove(at: removeIndex)
            value = result
            cursor -= 1
            return true
        }
        if key == "Delete"
        {
            if hasSelection
            {
                deleteSelection()
                return true
            }
            guard cursor < value.count else
            {
                return true
            }
            var result = value
            let removeIndex = result.index(result.startIndex, offsetBy: cursor)
            result.remove(at: removeIndex)
            value = result
            return true
        }
        if key == "Left"
        {
            if hasSelection
            {
                cursor = selectionBounds()?.start ?? cursor
                selectionStart = nil
            }
            else
            {
                cursor = max(0, cursor - 1)
            }
            return true
        }
        if key == "Right"
        {
            if hasSelection
            {
                cursor = selectionBounds()?.end ?? cursor
                selectionStart = nil
            }
            else
            {
                cursor = min(value.count, cursor + 1)
            }
            return true
        }
        if key == "Alt+Left"
        {
            selectionStart = nil
            for _ in 0..<kTextInputOptionHorizontalSteps
            {
                cursor = max(0, cursor - 1)
            }
            return true
        }
        if key == "Alt+Right"
        {
            selectionStart = nil
            for _ in 0..<kTextInputOptionHorizontalSteps
            {
                cursor = min(value.count, cursor + 1)
            }
            return true
        }
        if key == "Up" || key == "Down" || key == "Shift+Up" || key == "Shift+Down" || key == "Alt+Up" || key == "Alt+Down"
        {
            return false
        }
        if key == "Home"
        {
            cursor = lineStartForCursor()
            selectionStart = nil
            return true
        }
        if key == "End"
        {
            cursor = lineEndForCursor()
            selectionStart = nil
            return true
        }
        if key.count == 1
        {
            insertLiteralText(key)
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = safeWidth(size.width, value) + (size.width > 0 ? 0 : 1)
        let base = effectiveStyle(state)
        let chars = Array(value)
        let textWidth = chars.count
        cursor = max(0, min(cursor, textWidth))
        columnScrollOffset = max(0, columnScrollOffset)
        if textWidth <= width
        {
            columnScrollOffset = 0
        }
        if state.editMode
        {
            if cursor < columnScrollOffset
            {
                columnScrollOffset = cursor
            }
            else if cursor >= columnScrollOffset + width
            {
                columnScrollOffset = max(0, cursor - width + 1)
            }
        }

        let visibleEnd = min(textWidth, columnScrollOffset + width)
        let visible: String
        if columnScrollOffset < visibleEnd
        {
            visible = String(chars[columnScrollOffset..<visibleEnd])
        }
        else
        {
            visible = ""
        }
        var row = renderPlainText(visible, width: width, height: 1, style: base)[0]
        let selection = selectionBounds()
        let cursorEffectiveStyle = base.merged(cursorStyle)
        if state.editMode, let selection
        {
            for col in 0..<width
            {
                let sourceIndex = columnScrollOffset + col
                if sourceIndex >= selection.start && sourceIndex < selection.end
                {
                    row[col].foreground = cursorEffectiveStyle.color
                    row[col].background = cursorEffectiveStyle.background
                }
            }
        }
        if !state.editMode && width > 0 && textWidth > columnScrollOffset + width
        {
            row[width - 1].text = ">"
        }
        else if state.editMode && selection == nil && width > 0
        {
            let visibleCol = max(0, min(width - 1, cursor - columnScrollOffset))
            row[visibleCol].foreground = cursorEffectiveStyle.color
            row[visibleCol].background = cursorEffectiveStyle.background
        }
        return [row]
    }
}

open class TextArea: TextInput
{
    public var rowScrollOffset = 0
    private var manualRowScroll = false

    public override init(_ name: String, _ value: String = "", maxLength: Int = 0)
    {
        super.init(name, value, maxLength: maxLength)
        isMultiline = true
        commitMode = kCommitModeLeave
    }

    open override var elementType: String
    {
        "textarea"
    }

    open override func handleKey(_ key: String) -> Bool
    {
        manualRowScroll = false
        if key == "Up"
        {
            moveCursorVertical(-1)
            selectionStart = nil
            return true
        }
        if key == "Down"
        {
            moveCursorVertical(1)
            selectionStart = nil
            return true
        }
        if key == "Shift+Up"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            moveCursorVertical(-1)
            return true
        }
        if key == "Shift+Down"
        {
            if selectionStart == nil
            {
                selectionStart = cursor
            }
            moveCursorVertical(1)
            return true
        }
        if key == "Alt+Up"
        {
            selectionStart = nil
            for _ in 0..<kTextInputOptionVerticalSteps
            {
                moveCursorVertical(-1)
            }
            return true
        }
        if key == "Alt+Down"
        {
            selectionStart = nil
            for _ in 0..<kTextInputOptionVerticalSteps
            {
                moveCursorVertical(1)
            }
            return true
        }
        if key == "Enter" || key == "Alt+Enter"
        {
            if maxLength > 0 && value.count >= maxLength && !hasSelection
            {
                return true
            }
            insertLiteralText("\n")
            return true
        }
        return super.handleKey(key)
    }

    public func scrollByRows(_ delta: Int, viewportHeight: Int, manual: Bool = true) -> Bool
    {
        let height = safeHeight(viewportHeight)
        guard height > 1 else
        {
            return false
        }
        let rows = visualRows(width: safeWidth(frame.width, value))
        let maxOffset = max(0, rows.count - height)
        let next = clamped(rowScrollOffset + delta, lower: 0, upper: maxOffset)
        guard next != rowScrollOffset else
        {
            return false
        }
        rowScrollOffset = next
        manualRowScroll = manual
        return true
    }

    private func visualRows(width: Int) -> [(text: String, start: Int, end: Int)]
    {
        let safeWidth = max(1, width)
        let chars = Array(value)
        var rows: [(text: String, start: Int, end: Int)] = []
        var segmentStart = 0
        while segmentStart <= chars.count
        {
            var segmentEnd = segmentStart
            while segmentEnd < chars.count && chars[segmentEnd] != "\n"
            {
                segmentEnd += 1
            }
            let segment = Array(chars[segmentStart..<segmentEnd])
            if segment.isEmpty
            {
                rows.append(("", segmentStart, segmentStart))
            }
            else
            {
                var offset = 0
                while offset < segment.count
                {
                    let remaining = segment.count - offset
                    let chunkSize = min(remaining, safeWidth)
                    if chunkSize < safeWidth
                    {
                        rows.append((
                            String(segment[offset..<(offset + chunkSize)]),
                            segmentStart + offset,
                            segmentStart + offset + chunkSize
                        ))
                        offset += chunkSize
                        continue
                    }
                    var lastSpace: Int?
                    if chunkSize > 1
                    {
                        for index in stride(from: chunkSize - 1, through: 1, by: -1)
                        {
                            if segment[offset + index] == " "
                            {
                                lastSpace = index
                                break
                            }
                        }
                    }
                    if let lastSpace, lastSpace > 0
                    {
                        rows.append((
                            String(segment[offset..<(offset + lastSpace)]),
                            segmentStart + offset,
                            segmentStart + offset + lastSpace
                        ))
                        offset += lastSpace + 1
                    }
                    else
                    {
                        rows.append((
                            String(segment[offset..<(offset + chunkSize)]),
                            segmentStart + offset,
                            segmentStart + offset + chunkSize
                        ))
                        offset += chunkSize
                    }
                }
            }
            if segmentEnd >= chars.count
            {
                break
            }
            segmentStart = segmentEnd + 1
        }
        if rows.isEmpty
        {
            rows.append(("", 0, 0))
        }
        return rows
    }

    private func cursorLocation(in rows: [(text: String, start: Int, end: Int)], width: Int) -> (row: Int, col: Int)
    {
        for index in rows.indices
        {
            let row = rows[index]
            if cursor <= row.end && (row.text.count < width || cursor < row.end)
            {
                return (index, max(0, min(max(0, width - 1), cursor - row.start)))
            }
        }
        let lastIndex = max(0, rows.count - 1)
        let last = rows[lastIndex]
        return (lastIndex, max(0, min(max(0, width - 1), last.end - last.start)))
    }

    private func moveCursorVertical(_ delta: Int)
    {
        let width = safeWidth(frame.width, value)
        let rows = visualRows(width: width)
        let current = cursorLocation(in: rows, width: width)
        let targetRow = current.row + delta
        guard targetRow >= 0 && targetRow < rows.count else
        {
            return
        }
        let target = rows[targetRow]
        cursor = max(target.start, min(target.end, target.start + current.col))
    }

    open override func cursorForPoint(row: Int, col: Int, size: Size) -> Int
    {
        let rows = visualRows(width: safeWidth(size.width, value))
        let targetRow = clamped(row, lower: 0, upper: max(0, safeHeight(size.height) - 1)) + rowScrollOffset
        guard targetRow >= 0 && targetRow < rows.count else
        {
            return value.count
        }
        let target = rows[targetRow]
        return max(target.start, min(target.end, target.start + max(0, col)))
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = safeWidth(size.width, value)
        let height = safeHeight(size.height)
        let base = effectiveStyle(state)
        cursor = max(0, min(cursor, value.count))
        let visualRows = visualRows(width: width)
        let cursorPosition = cursorLocation(in: visualRows, width: width)
        let maxOffset = max(0, visualRows.count - height)
        rowScrollOffset = clamped(rowScrollOffset, lower: 0, upper: maxOffset)
        if state.editMode && !manualRowScroll
        {
            if cursorPosition.row < rowScrollOffset
            {
                rowScrollOffset = cursorPosition.row
            }
            else if cursorPosition.row >= rowScrollOffset + height
            {
                rowScrollOffset = max(0, cursorPosition.row - height + 1)
            }
        }

        let hasAbove = rowScrollOffset > 0
        let hasBelow = rowScrollOffset + height < visualRows.count
        let cursorEffectiveStyle = base.merged(cursorStyle)
        let selection = state.editMode ? selectionBounds() : nil
        var rendered: [[TerminalCell]] = []
        rendered.reserveCapacity(height)
        for row in 0..<height
        {
            let rowIndex = rowScrollOffset + row
            let visual: (text: String, start: Int, end: Int) = rowIndex < visualRows.count
                ? visualRows[rowIndex]
                : (text: "", start: cursor, end: cursor)
            var renderedRow = renderPlainText(visual.text, width: width, height: 1, style: base)[0]
            if width > 0 && visual.text.count < width
            {
                if row == 0 && hasAbove
                {
                    renderedRow[width - 1].text = "^"
                }
                else if row == height - 1 && hasBelow
                {
                    renderedRow[width - 1].text = "v"
                }
            }
            if let selection
            {
                for col in 0..<min(width, visual.text.count)
                {
                    let sourceIndex = visual.start + col
                    if sourceIndex >= selection.start && sourceIndex < selection.end
                    {
                        renderedRow[col].foreground = cursorEffectiveStyle.color
                        renderedRow[col].background = cursorEffectiveStyle.background
                    }
                }
            }
            else if state.editMode && rowIndex == cursorPosition.row && width > 0
            {
                renderedRow[cursorPosition.col].foreground = cursorEffectiveStyle.color
                renderedRow[cursorPosition.col].background = cursorEffectiveStyle.background
            }
            rendered.append(renderedRow)
        }
        return rendered
    }
}

open class NumberInput: TextInput
{
    public var numberValue: Double
    public var step: Double
    private var editText = ""
    private var editCursor = 0
    private var editOriginalValue = 0.0
    private var editing = false
    private var replaceOnFirstTextInput = false

    public init(_ name: String, _ value: Double = 0.0, step: Double = 1.0)
    {
        self.numberValue = value
        self.step = step
        super.init(name, formatNumber(value), maxLength: 0)
    }

    open override var valueForSnapshot: Any?
    {
        numberValue
    }

    open override var elementType: String
    {
        "numberinput"
    }

    public func setValue(_ value: Double)
    {
        let wasEditing = editing
        numberValue = value
        super.setValue(formatNumber(value))
        editOriginalValue = value
        editText = wasEditing ? formatNumber(value) : ""
        editCursor = wasEditing ? editText.count : 0
        editing = wasEditing
        replaceOnFirstTextInput = false
    }

    public func setEditText(_ text: String)
    {
        editText = text
        editCursor = editText.count
        editing = true
        replaceOnFirstTextInput = false
    }

    open override func setText(_ value: String)
    {
        setValue(Double(value) ?? 0.0)
    }

    open override func handleKey(_ key: String) -> Bool
    {
        if key == "cmd_c"
        {
            _ = copyTextToClipboard(formatNumber(numberValue))
            return true
        }
        if key == "cmd_v"
        {
            let text = runtimeClipboardText()
            if !text.isEmpty
            {
                insertText(text)
            }
            return true
        }
        ensureEditText()
        if key == "Up"
        {
            numberValue += step
            editText = formatNumber(numberValue)
            editCursor = editText.count
            replaceOnFirstTextInput = false
            return true
        }
        if key == "Down"
        {
            numberValue -= step
            editText = formatNumber(numberValue)
            editCursor = editText.count
            replaceOnFirstTextInput = false
            return true
        }
        if key == "Left"
        {
            editCursor = max(0, editCursor - 1)
            replaceOnFirstTextInput = false
            return true
        }
        if key == "Right"
        {
            editCursor = min(editText.count, editCursor + 1)
            replaceOnFirstTextInput = false
            return true
        }
        if key == "Home"
        {
            editCursor = 0
            replaceOnFirstTextInput = false
            return true
        }
        if key == "End"
        {
            editCursor = editText.count
            replaceOnFirstTextInput = false
            return true
        }
        if key == "Backspace"
        {
            replaceOnFirstTextInput = false
            if editCursor > 0
            {
                let removeIndex = editText.index(editText.startIndex, offsetBy: editCursor - 1)
                editText.remove(at: removeIndex)
                editCursor -= 1
            }
            return true
        }
        if key == "Enter"
        {
            commitEdit()
            return true
        }
        if key.count == 1, let character = key.first, character.isNumber || key == "." || key == "-"
        {
            if replaceOnFirstTextInput
            {
                editText = ""
                editCursor = 0
                replaceOnFirstTextInput = false
            }
            let insertIndex = editText.index(editText.startIndex, offsetBy: max(0, min(editCursor, editText.count)))
            editText.insert(character, at: insertIndex)
            editCursor += 1
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        if state.editMode
        {
            ensureEditText()
        }
        else
        {
            value = formatNumber(numberValue)
            cursor = value.count
        }
        let text = state.editMode ? editText : formatNumber(numberValue)
        let width = safeWidth(size.width, text) + (size.width > 0 ? 0 : 1)
        let base = effectiveStyle(state)
        var row = renderPlainText(text, width: width, height: 1, style: base)[0]
        if state.editMode && width > 0
        {
            let cursorEffectiveStyle = base.merged(cursorStyle)
            if replaceOnFirstTextInput
            {
                for col in 0..<min(editText.count, row.count)
                {
                    row[col].foreground = cursorEffectiveStyle.color
                    row[col].background = cursorEffectiveStyle.background
                }
            }
            else
            {
                let cursorCol = max(0, min(width - 1, editCursor))
                row[cursorCol].foreground = cursorEffectiveStyle.color
                row[cursorCol].background = cursorEffectiveStyle.background
            }
        }
        return [row]
    }

    public func beginEdit()
    {
        if editing
        {
            return
        }
        editOriginalValue = numberValue
        editText = formatNumber(numberValue)
        editCursor = editText.count
        editing = true
        replaceOnFirstTextInput = numberValue == 0.0
    }

    public func setEditCursor(_ cursor: Int, preserveReplaceOnFirstTextInput: Bool = false)
    {
        ensureEditText()
        editCursor = clamped(cursor, lower: 0, upper: editText.count)
        if !preserveReplaceOnFirstTextInput
        {
            replaceOnFirstTextInput = false
        }
    }

    public func cancelEdit()
    {
        if !editing
        {
            return
        }
        numberValue = editOriginalValue
        value = formatNumber(numberValue)
        cursor = value.count
        editText = ""
        editCursor = 0
        editing = false
        replaceOnFirstTextInput = false
    }

    public func commitEdit()
    {
        ensureEditText()
        if editText.isEmpty
        {
            numberValue = 0.0
        }
        else if let parsed = Double(editText)
        {
            numberValue = parsed
        }
        else
        {
            numberValue = editOriginalValue
        }
        value = formatNumber(numberValue)
        cursor = value.count
        editText = ""
        editCursor = 0
        editing = false
        replaceOnFirstTextInput = false
    }

    private func ensureEditText()
    {
        if !editing
        {
            beginEdit()
        }
    }
}

open class ComboBox: UIElement
{
    public var options: [String]
    public var selectedIndex: Int
    {
        didSet
        {
            selectedIndexDidChange()
        }
    }

    public init(_ name: String, _ options: [String] = [], selectedIndex: Int = -1)
    {
        self.options = options
        self.selectedIndex = selectedIndex >= 0 ? selectedIndex : (options.isEmpty ? -1 : 0)
        super.init(name)
    }

    open override var terminalText: String
    {
        selectedIndex >= 0 && selectedIndex < options.count ? options[selectedIndex] : ""
    }

    open override var valueForSnapshot: Any?
    {
        terminalText
    }

    open override var elementType: String
    {
        "combobox"
    }

    open override var role: String
    {
        "selection"
    }

    open func selectedIndexDidChange()
    {
    }

    public func selectValue(_ value: String)
    {
        if let index = options.firstIndex(of: value)
        {
            selectedIndex = index
        }
    }

    open override func handleKey(_ key: String) -> Bool
    {
        guard !options.isEmpty else
        {
            return false
        }
        if key == "Down"
        {
            selectedIndex = min(options.count - 1, max(0, selectedIndex) + 1)
            return true
        }
        if key == "Up"
        {
            selectedIndex = max(0, selectedIndex - 1)
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let selected = terminalText
        let width = safeWidth(size.width, selected)
        let expanded = state.editMode
        let height = expanded ? options.count + kComboBoxClosedRows : kComboBoxClosedRows
        let base = effectiveStyle(ElementRenderState(focused: state.focused, editMode: expanded, parentBackground: state.parentBackground))
        var rows: [[TerminalCell]] = []
        rows.reserveCapacity(height)

        var closedChars = Array(fitText(selected, width: width))
        if width > 0 && (state.focused || state.editMode)
        {
            closedChars[width - 1] = expanded ? "v" : ">"
        }
        rows.append(renderPlainText(String(closedChars), width: width, height: kComboBoxClosedRows, style: base)[0])

        if expanded
        {
            for optionIndex in 0..<options.count
            {
                let optionStyle = optionIndex == selectedIndex ? base.merged(selectedStyle) : base
                rows.append(renderPlainText("  \(options[optionIndex])", width: width, height: kComboBoxClosedRows, style: optionStyle)[0])
            }
        }
        return rows
    }
}

open class ListBox: ComboBox
{
    public var multiple = false
    public var selectedValues: [String]
    public var disabledValues: [String] = []
    public private(set) var activeItemVisible = false
    public private(set) var activeIndex = 0
    private var scrollOffset = 0
    private var lastViewportHeight = 0

    public init(_ name: String, _ options: [String] = [], selectedValues: [String] = [])
    {
        self.selectedValues = selectedValues
        super.init(name, options, selectedIndex: selectedValues.first.flatMap { options.firstIndex(of: $0) } ?? -1)
        activeIndex = max(0, selectedIndex)
    }

    open override var valueForSnapshot: Any?
    {
        selectedValues
    }

    open override var elementType: String
    {
        "listbox"
    }

    public func setSelectedValues(_ values: [String])
    {
        selectedValues = values.filter { options.contains($0) }
        if let last = selectedValues.last, let index = options.firstIndex(of: last)
        {
            selectedIndex = index
            activeIndex = index
        }
        ensureSelectedVisibleIfRendered()
    }

    public func setDisabledValues(_ values: [String])
    {
        disabledValues = values
    }

    open override func selectedIndexDidChange()
    {
        guard !options.isEmpty else
        {
            activeIndex = 0
            if !multiple
            {
                selectedValues = []
            }
            scrollOffset = 0
            return
        }
        let index = clamped(selectedIndex, lower: 0, upper: options.count - 1)
        activeIndex = index
        if !multiple
        {
            selectedValues = selectedIndex >= 0 && selectedIndex < options.count ? [options[index]] : []
        }
        ensureSelectedVisibleIfRendered()
    }

    public func scrollBy(_ delta: Int, viewportHeight: Int)
    {
        if options.isEmpty
        {
            scrollOffset = 0
            return
        }
        let maxOffset = max(0, options.count - max(1, viewportHeight))
        scrollOffset = clamped(scrollOffset + delta, lower: 0, upper: maxOffset)
    }

    public func scrollOffsetValue() -> Int
    {
        scrollOffset
    }

    public func setSelectedIndex(_ index: Int)
    {
        if options.isEmpty
        {
            selectedIndex = 0
            activeIndex = 0
            selectedValues = []
            scrollOffset = 0
            return
        }
        selectedIndex = clamped(index, lower: 0, upper: options.count - 1)
        activeIndex = selectedIndex
        if !multiple
        {
            selectedValues = [options[selectedIndex]]
        }
        ensureSelectedVisibleIfRendered()
    }

    public func setActiveIndex(_ index: Int)
    {
        if options.isEmpty
        {
            selectedIndex = 0
            activeIndex = 0
            scrollOffset = 0
            return
        }
        activeIndex = clamped(index, lower: 0, upper: options.count - 1)
        ensureActiveVisibleIfRendered()
    }

    public func showActiveItem()
    {
        activeItemVisible = true
    }

    public func hideActiveItem()
    {
        activeItemVisible = false
    }

    private func ensureSelectedVisible(viewportHeight: Int)
    {
        ensureIndexVisible(selectedIndex, viewportHeight: viewportHeight)
    }

    private func ensureSelectedVisibleIfRendered()
    {
        if lastViewportHeight > 0
        {
            ensureSelectedVisible(viewportHeight: lastViewportHeight)
        }
    }

    private func ensureActiveVisibleIfRendered()
    {
        if lastViewportHeight > 0
        {
            ensureIndexVisible(activeIndex, viewportHeight: lastViewportHeight)
        }
    }

    private func ensureIndexVisible(_ index: Int, viewportHeight: Int)
    {
        guard !options.isEmpty else
        {
            scrollOffset = 0
            return
        }
        let height = max(1, viewportHeight)
        if index < scrollOffset
        {
            scrollOffset = index
        }
        else if index >= scrollOffset + height
        {
            scrollOffset = index - height + 1
        }
    }

    open override func handleKey(_ key: String) -> Bool
    {
        guard !options.isEmpty else
        {
            return false
        }
        if key == "Down"
        {
            setActiveIndex(activeIndex + 1)
            showActiveItem()
            return true
        }
        if key == "Up"
        {
            setActiveIndex(activeIndex - 1)
            showActiveItem()
            return true
        }
        if key == "Enter"
        {
            let value = options[max(0, min(activeIndex, options.count - 1))]
            if multiple
            {
                if selectedValues.contains(value)
                {
                    selectedValues = selectedValues.filter { $0 != value }
                }
                else
                {
                    selectedValues.append(value)
                }
            }
            else
            {
                setSelectedIndex(activeIndex)
                hideActiveItem()
            }
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let height = safeHeight(size.height)
        lastViewportHeight = height
        let displayIndex = state.editMode && activeItemVisible ? activeIndex : selectedIndex
        let selected = displayIndex >= 0 && displayIndex < options.count ? options[displayIndex] : ""
        let width = safeWidth(size.width, selected)
        if options.isEmpty
        {
            return renderPlainText("", width: width, height: height, style: effectiveStyle(state))
        }

        let maxOffset = max(0, options.count - height)
        scrollOffset = clamped(scrollOffset, lower: 0, upper: maxOffset)
        if state.editMode && activeItemVisible
        {
            ensureIndexVisible(activeIndex, viewportHeight: height)
        }
        else
        {
            ensureSelectedVisible(viewportHeight: height)
        }

        let base = effectiveStyle(state)
        let hasAbove = scrollOffset > 0
        let hasBelow = scrollOffset + height < options.count
        var rows: [[TerminalCell]] = []
        rows.reserveCapacity(height)
        for row in 0..<height
        {
            let optionIndex = scrollOffset + row
            let selectedRow = optionIndex < options.count && selectedValues.contains(options[optionIndex])
            let activeRow = optionIndex < options.count && state.editMode && activeItemVisible && optionIndex == activeIndex
            let disabledRow = optionIndex < options.count && disabledValues.contains(options[optionIndex])
            var chars = Array(fitText(optionIndex < options.count ? options[optionIndex] : "", width: width))
            if row == 0 && hasAbove && width > 0
            {
                chars[width - 1] = "^"
            }
            else if row == height - 1 && hasBelow && width > 0
            {
                chars[width - 1] = "v"
            }
            var rowStyle = base
            if selectedRow
            {
                rowStyle = rowStyle.mergedListBoxRowLayer(selectedStyle)
            }
            if activeRow
            {
                if activeStyle != nil
                {
                    rowStyle = rowStyle.mergedListBoxRowLayer(activeStyle)
                }
                else if !selectedRow
                {
                    rowStyle = rowStyle.mergedListBoxRowLayer(selectedStyle)
                }
            }
            if disabledRow
            {
                rowStyle = base.merged(disabledStyle)
            }
            rows.append(renderPlainText(String(chars), width: width, height: kComboBoxClosedRows, style: rowStyle)[0])
        }
        return rows
    }

    fileprivate var visibleScrollOffset: Int
    {
        scrollOffset
    }
}

open class ScrollView: UIElement
{
    public var gap: Int
    {
        didSet
        {
            if gap < 0
            {
                gap = 0
            }
            invalidateHeightCache()
        }
    }
    public var descendantFocusStyle: Style?
    private var renderedChildren: [[[TerminalCell]]] = []
    private var dynamicChildrenRenderer: ((Int) -> [[[TerminalCell]]])?
    private var dynamicChildrenWidth: Int?
    private var cachedChildHeightsWidth: Int?
    private var cachedChildHeights: [Int]?
    private var scrollOffset = 0
    private var viewOffset = 0
    private var autoScroll = false
    private var lastNaturalSkip = 0

    public init(_ name: String, _ gap: Int = 0)
    {
        self.gap = max(0, gap)
        super.init(name)
    }

    open override var elementType: String
    {
        "scrollview"
    }

    open override var valueForSnapshot: Any?
    {
        NSNull()
    }

    open func setAutoScroll(_ enabled: Bool)
    {
        autoScroll = enabled
        if enabled
        {
            viewOffset = 0
        }
    }

    public func addChild(_ child: [[TerminalCell]])
    {
        renderedChildren.append(child)
        dynamicChildrenWidth = nil
        invalidateHeightCache()
        if autoScroll
        {
            viewOffset = 0
        }
    }

    @discardableResult
    open override func addChild<T: UIElement>(_ child: T) -> T
    {
        let added = super.addChild(child)
        dynamicChildrenWidth = nil
        invalidateHeightCache()
        if autoScroll
        {
            viewOffset = 0
        }
        return added
    }

    public func clearChildren()
    {
        renderedChildren.removeAll()
        clearChildElements()
        dynamicChildrenWidth = nil
        invalidateHeightCache()
        scrollOffset = 0
        viewOffset = 0
        lastNaturalSkip = 0
    }

    public func setDynamicChildrenRenderer(_ renderer: @escaping (Int) -> [[[TerminalCell]]])
    {
        dynamicChildrenRenderer = renderer
        invalidateDynamicChildren()
    }

    public func setDescendantFocusStyle(_ value: Style)
    {
        descendantFocusStyle = value
    }

    public func invalidateDynamicChildren()
    {
        dynamicChildrenWidth = nil
        invalidateHeightCache()
    }

    public func viewOffsetValue() -> Int
    {
        viewOffset
    }

    public func scrollOffsetValue() -> Int
    {
        scrollOffset
    }

    public func scrollPosition() -> ScrollViewPosition
    {
        ScrollViewPosition(scrollOffset: scrollOffset, viewOffset: viewOffset, autoScroll: autoScroll)
    }

    public func restoreScrollPosition(_ position: ScrollViewPosition)
    {
        _ = currentChildren(width: max(1, frame.width))
        let total = renderedChildren.count + children.count
        scrollOffset = clamped(position.scrollOffset, lower: 0, upper: max(0, total - 1))
        viewOffset = max(0, position.viewOffset)
        autoScroll = position.autoScroll
        invalidateHeightCache()
        let frameViewport = Size(
            width: max(1, frame.width),
            height: max(1, frame.height)
        )
        let viewport = generatedScrollViewport(size: frameViewport, style: style)
        let naturalSkip = max(
            0,
            contentHeight(width: max(1, viewport.width)) - max(1, viewport.height)
        )
        viewOffset = autoScroll ? 0 : min(viewOffset, naturalSkip)
        lastNaturalSkip = naturalSkip
    }

    public func contentHeight(width: Int? = nil) -> Int
    {
        let renderWidth = max(1, width ?? frame.width)
        ensureHeightCache(width: renderWidth)
        let heights = cachedChildHeights ?? []
        let startIndex = min(max(0, scrollOffset), heights.count)
        let visibleHeights = heights[startIndex..<heights.count]
        guard !visibleHeights.isEmpty else
        {
            return 0
        }
        return visibleHeights.reduce(0, +) + max(0, visibleHeights.count - 1) * max(0, gap)
    }

    public func maxViewOffset(viewport: Size) -> Int
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: style)
        let naturalSkip = max(0, contentHeight(width: max(1, scrollViewport.width)) - max(1, scrollViewport.height))
        syncViewOffsetForNaturalSkip(naturalSkip)
        lastNaturalSkip = naturalSkip
        return naturalSkip
    }

    private func scrollViewportSize(for viewport: Size) -> Size
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: style)
        return Size(width: max(1, scrollViewport.width), height: max(1, scrollViewport.height))
    }

    private func childHeights(width: Int) -> [Int]
    {
        let renderWidth = max(1, width)
        ensureHeightCache(width: renderWidth)
        return cachedChildHeights ?? []
    }

    private func maxChildrenInViewport(_ viewport: Size) -> Int
    {
        let total = currentChildren(width: max(1, viewport.width)).count + children.count
        guard total > 0 else
        {
            return 0
        }
        let sampleSize = min(kWheelScrollViewportFraction + 2, total)
        guard sampleSize > 0 else
        {
            return kMinimumRenderableSize
        }
        let heights = childHeights(width: max(1, viewport.width))
        var measured = 0.0
        var measuredCount = 0
        for height in heights.prefix(sampleSize)
        {
            measured += Double(height)
            measuredCount += 1
        }
        let averageHeight = max(1.0, measuredCount == 0 ? 1.0 : measured / Double(measuredCount))
        let denominator = Int(averageHeight) + gap
        guard denominator > 0 else
        {
            return kMinimumRenderableSize
        }
        return max(kMinimumRenderableSize, (max(1, viewport.height) + gap) / denominator)
    }

    @discardableResult
    public func scrollTo(_ index: Int, viewport: Size) -> Bool
    {
        let scrollViewport = scrollViewportSize(for: viewport)
        let total = currentChildren(width: scrollViewport.width).count + children.count
        guard total > 0 else
        {
            let previous = scrollOffset
            scrollOffset = 0
            return scrollOffset != previous
        }
        let previous = scrollOffset
        let maxVisible = maxChildrenInViewport(scrollViewport)
        let maxOffset = max(0, total - maxVisible)
        scrollOffset = clamped(index, lower: 0, upper: maxOffset)
        if scrollOffset == 0
        {
            autoScroll = false
        }
        return scrollOffset != previous
    }

    @discardableResult
    public func scrollBy(_ delta: Int, viewport: Size) -> Bool
    {
        scrollTo(scrollOffset + delta, viewport: viewport)
    }

    @discardableResult
    public func scrollLines(_ delta: Int, viewport: Size) -> Bool
    {
        let maxOffset = maxViewOffset(viewport: viewport)
        let previous = viewOffset
        viewOffset = clamped(viewOffset + delta, lower: 0, upper: maxOffset)
        autoScroll = viewOffset == 0
        return viewOffset != previous
    }

    @discardableResult
    public func scrollToTop(_ viewport: Size? = nil) -> Bool
    {
        let previous = viewOffset
        if let viewport
        {
            viewOffset = maxViewOffset(viewport: viewport)
        }
        else
        {
            viewOffset = max(0, contentHeight(width: max(1, frame.width)) - max(1, frame.height))
        }
        autoScroll = false
        return viewOffset != previous
    }

    @discardableResult
    public func scrollToBottom(_ viewport: Size? = nil) -> Bool
    {
        let changed = scrollOffset != 0 || viewOffset != 0 || !autoScroll
        scrollOffset = 0
        viewOffset = 0
        autoScroll = true
        _ = viewport
        return changed
    }

    public func wheelScrollLines(viewport: Size) -> Int
    {
        let scrollViewport = scrollViewportSize(for: viewport)
        let viewportLimited = max(1, scrollViewport.height / kWheelScrollViewportFraction)
        return max(1, min(kDefaultWheelScrollLines, viewportLimited))
    }

    @discardableResult
    public func handleWheel(_ wheelDelta: Int, viewport: Size) -> Bool
    {
        if wheelDelta == 0
        {
            return false
        }
        let step = wheelScrollLines(viewport: viewport) * max(1, abs(wheelDelta))
        if wheelDelta > 0
        {
            return scrollLines(step, viewport: viewport)
        }
        return scrollLines(-step, viewport: viewport)
    }

    @discardableResult
    public func handleKey(_ key: String, viewport: Size) -> Bool
    {
        if key == "Alt+Up"
        {
            return scrollLines(wheelScrollLines(viewport: viewport), viewport: viewport)
        }
        if key == "Alt+Down"
        {
            return scrollLines(-wheelScrollLines(viewport: viewport), viewport: viewport)
        }
        if key == "Ctrl+Up"
        {
            return scrollLines(max(1, viewport.height), viewport: viewport)
        }
        if key == "Ctrl+Down"
        {
            return scrollLines(-max(1, viewport.height), viewport: viewport)
        }
        guard style.scrollY ?? true else
        {
            return false
        }
        switch key
        {
        case "ArrowUp", "Up":
            return scrollLines(1, viewport: viewport)
        case "ArrowDown", "Down":
            return scrollLines(-1, viewport: viewport)
        case "PageUp":
            return scrollLines(max(1, viewport.height), viewport: viewport)
        case "PageDown":
            return scrollLines(-max(1, viewport.height), viewport: viewport)
        case "Home":
            return scrollToTop(viewport)
        case "End":
            return scrollToBottom(viewport)
        default:
            return false
        }
    }

    @discardableResult
    public func ensureChildVisible(_ child: UIElement, viewport: Size) -> Bool
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: style)
        let renderWidth = max(1, scrollViewport.width)
        let rendered = currentChildren(width: renderWidth)
        ensureHeightCache(width: renderWidth)
        let heights = cachedChildHeights ?? []
        let totalItems = rendered.count + children.count
        var itemIndex = 0
        var globalRow = 0

        func appendGap()
        {
            if itemIndex >= scrollOffset && itemIndex + 1 < totalItems
            {
                globalRow += max(0, gap)
            }
            itemIndex += 1
        }

        for renderedChild in rendered
        {
            if itemIndex >= scrollOffset
            {
                let childHeight = itemIndex < heights.count ? heights[itemIndex] : max(0, renderedChild.count)
                globalRow += childHeight
            }
            appendGap()
        }

        for liveChild in children
        {
            let childHeight = itemIndex < heights.count ? heights[itemIndex] : nativeChildHeight(liveChild, width: renderWidth)
            if liveChild === child
            {
                if itemIndex < scrollOffset
                {
                    return false
                }
                let naturalSkip = maxViewOffset(viewport: viewport)
                let currentActualSkip = naturalSkip - min(viewOffset, naturalSkip)
                var nextActualSkip = currentActualSkip
                if globalRow < currentActualSkip
                {
                    nextActualSkip = globalRow
                }
                else if globalRow + childHeight > currentActualSkip + max(1, scrollViewport.height)
                {
                    nextActualSkip = globalRow + childHeight - max(1, scrollViewport.height)
                }
                nextActualSkip = clamped(nextActualSkip, lower: 0, upper: naturalSkip)
                let previous = viewOffset
                viewOffset = naturalSkip - nextActualSkip
                autoScroll = viewOffset == 0
                return viewOffset != previous
            }
            if itemIndex >= scrollOffset
            {
                globalRow += childHeight
            }
            appendGap()
        }
        return false
    }

    public func childViews(size: Size) -> [ScrollViewChildView]
    {
        let viewport = generatedScrollViewport(size: size, style: style)
        let renderWidth = max(1, viewport.width)
        let rendered = currentChildren(width: renderWidth)
        ensureHeightCache(width: renderWidth)
        let heights = cachedChildHeights ?? []
        let naturalSkip = max(0, contentHeight(width: renderWidth) - max(1, viewport.height))
        syncViewOffsetForNaturalSkip(naturalSkip)
        lastNaturalSkip = naturalSkip
        viewOffset = clamped(viewOffset, lower: 0, upper: naturalSkip)
        let skip = naturalSkip - viewOffset
        let totalItems = rendered.count + children.count
        var itemIndex = 0
        var globalRow = 0
        var views: [ScrollViewChildView] = []

        func appendGap()
        {
            if itemIndex >= scrollOffset && itemIndex + 1 < totalItems
            {
                globalRow += max(0, gap)
            }
            itemIndex += 1
        }

        func appendView(element: UIElement?, childHeight: Int)
        {
            if itemIndex >= scrollOffset
            {
                let localRow = viewport.row + globalRow - skip
                let visibleTop = max(localRow, viewport.row)
                let visibleBottom = min(localRow + childHeight, viewport.row + viewport.height)
                let visible = visibleBottom > visibleTop
                let clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight)
                views.append(ScrollViewChildView(
                    element: element,
                    frame: Rect(row: localRow, col: viewport.col, width: max(1, renderWidth), height: childHeight),
                    visible: visible,
                    index: itemIndex,
                    clipped: clipped
                ))
                globalRow += childHeight
            }
            appendGap()
        }

        for renderedChild in rendered
        {
            let childHeight = itemIndex < heights.count ? heights[itemIndex] : max(0, renderedChild.count)
            appendView(element: nil, childHeight: childHeight)
        }
        for liveChild in children
        {
            let childHeight = itemIndex < heights.count ? heights[itemIndex] : nativeChildHeight(liveChild, width: renderWidth)
            appendView(element: liveChild, childHeight: childHeight)
        }
        return views
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let effective = effectiveStyle(state)
        let width = max(1, size.width)
        let height = max(1, size.height)
        let fillCell = TerminalCell(" ", foreground: effective.color, background: effective.background)
        var output = Array(repeating: Array(repeating: fillCell, count: width), count: height)
        let viewport = generatedScrollViewport(size: Size(width: width, height: height), style: effective)
        let rows = renderedRows(width: max(1, viewport.width), style: effective)
        let maxOffset = max(0, rows.count - max(1, viewport.height))
        syncViewOffsetForNaturalSkip(maxOffset)
        lastNaturalSkip = maxOffset
        viewOffset = clamped(viewOffset, lower: 0, upper: maxOffset)
        let actualSkip = maxOffset - viewOffset
        for row in 0..<max(0, viewport.height)
        {
            let sourceRow = actualSkip + row
            guard sourceRow < rows.count else
            {
                continue
            }
            let targetRow = viewport.row + row
            guard targetRow >= 0 && targetRow < output.count else
            {
                continue
            }
            for col in 0..<min(viewport.width, rows[sourceRow].count)
            {
                let targetCol = viewport.col + col
                if targetCol >= 0 && targetCol < output[targetRow].count
                {
                    output[targetRow][targetCol] = rows[sourceRow][col]
                }
            }
        }
        applyGeneratedScrollIndicators(
            content: &output,
            viewOffset: actualSkip,
            maxViewOffset: maxOffset,
            viewport: Size(width: width, height: height),
            style: effective
        )
        return output
    }

    private func currentChildren(width: Int) -> [[[TerminalCell]]]
    {
        if let dynamicChildrenRenderer, dynamicChildrenWidth != width
        {
            renderedChildren = dynamicChildrenRenderer(width)
            dynamicChildrenWidth = width
            invalidateHeightCache()
            if autoScroll
            {
                viewOffset = 0
            }
        }
        return renderedChildren
    }

    private func renderedRows(width: Int, style: Style) -> [[TerminalCell]]
    {
        let renderWidth = max(1, width)
        let rendered = currentChildren(width: renderWidth)
        ensureHeightCache(width: renderWidth)
        let heights = cachedChildHeights ?? []
        let blank = scrollBlankRow(width: renderWidth, style: style)
        var rows: [[TerminalCell]] = []
        let totalItems = rendered.count + children.count
        var itemIndex = 0

        func appendGap()
        {
            if itemIndex >= scrollOffset && itemIndex < totalItems - 1
            {
                for _ in 0..<max(0, gap)
                {
                    rows.append(blank)
                }
            }
            itemIndex += 1
        }

        for renderedChild in rendered
        {
            if itemIndex >= scrollOffset
            {
                for row in renderedChild
                {
                    rows.append(inheritScrollRowStyle(
                        fitCells(row, width: max(1, width), fillCell: blank[0]),
                        parentStyle: style
                    ))
                }
            }
            appendGap()
        }
        for child in children
        {
            if itemIndex >= scrollOffset
            {
                let childHeight = itemIndex < heights.count ? heights[itemIndex] : nativeChildHeight(child, width: renderWidth)
                for _ in 0..<max(0, childHeight)
                {
                    rows.append(blank)
                }
            }
            appendGap()
        }
        return rows
    }

    private func nativeChildHeight(_ child: UIElement, width: Int) -> Int
    {
        if let reusable = child as? ReusableElement,
           let generated = reusable.child
        {
            return max(1, generated.generatedContentSizeForWidth(max(1, width)).height)
        }
        if child.frame.height > 0
        {
            return max(1, child.frame.height)
        }
        return max(1, child.render(size: Size(width: max(1, width), height: max(1, child.frame.height))).count)
    }

    private func ensureHeightCache(width: Int)
    {
        let renderWidth = max(1, width)
        if let cachedChildHeights, let cachedChildHeightsWidth, cachedChildHeightsWidth == renderWidth
        {
            _ = cachedChildHeights
            return
        }
        let rendered = currentChildren(width: renderWidth)
        if let cachedChildHeights, let cachedChildHeightsWidth, cachedChildHeightsWidth == renderWidth
        {
            _ = cachedChildHeights
            return
        }
        var heights: [Int] = []
        heights.reserveCapacity(rendered.count + children.count)
        for child in rendered
        {
            heights.append(max(0, child.count))
        }
        for child in children
        {
            heights.append(nativeChildHeight(child, width: renderWidth))
        }
        cachedChildHeightsWidth = renderWidth
        cachedChildHeights = heights
    }

    private func invalidateHeightCache()
    {
        cachedChildHeightsWidth = nil
        cachedChildHeights = nil
    }

    private func syncViewOffsetForNaturalSkip(_ naturalSkip: Int)
    {
        let clampedNaturalSkip = max(0, naturalSkip)
        let previousNaturalSkip = max(0, lastNaturalSkip)
        if autoScroll
        {
            viewOffset = 0
        }
        else if viewOffset >= previousNaturalSkip
        {
            viewOffset = clampedNaturalSkip
        }
        else if viewOffset > clampedNaturalSkip
        {
            viewOffset = clampedNaturalSkip
        }
    }
}

private func fitCells(_ row: [TerminalCell], width: Int, fillCell: TerminalCell) -> [TerminalCell]
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

private func scrollBlankRow(width: Int, style: Style) -> [TerminalCell]
{
    Array(repeating: TerminalCell(" ", foreground: nil, background: style.background), count: max(1, width))
}

private func inheritScrollRowStyle(_ row: [TerminalCell], parentStyle: Style) -> [TerminalCell]
{
    row.map
    {
        cell in
        var result = cell
        result.background = blendScrollColorOverParent(result.background, parentStyle.background)
        if result.foreground == nil || result.foreground?.isTransparent == true
        {
            result.foreground = parentStyle.color
        }
        return result
    }
}

private func blendScrollColorOverParent(_ color: Color?, _ parent: Color?) -> Color?
{
    guard let color, !color.isTransparent else
    {
        return parent
    }
    if let rgba = color.rgba, rgba.alpha < 255, let parent
    {
        return color.blended(over: parent)
    }
    return color
}

public protocol GeneratedScrollableContent: AnyObject
{
    func scrollBy(_ delta: Int, viewport: Size)
}

public protocol GeneratedScrollableKeyHandling: GeneratedScrollableContent
{
    func handleScrollKey(_ key: String, viewport: Size) -> Bool
}

public protocol GeneratedScrollableElementVisibility: GeneratedScrollableContent
{
    func ensureElementVisible(_ elementId: String, viewport: Size)
}

public struct ScrollViewChildView
{
    public var element: UIElement?
    public var frame: Rect
    public var visible: Bool
    public var index: Int
    public var clipped: Bool

    public init(element: UIElement?, frame: Rect, visible: Bool, index: Int, clipped: Bool)
    {
        self.element = element
        self.frame = frame
        self.visible = visible
        self.index = index
        self.clipped = clipped
    }
}

public struct ScrollViewPosition
{
    public var scrollOffset: Int
    public var viewOffset: Int
    public var autoScroll: Bool

    public init(scrollOffset: Int = 0, viewOffset: Int = 0, autoScroll: Bool = false)
    {
        self.scrollOffset = scrollOffset
        self.viewOffset = viewOffset
        self.autoScroll = autoScroll
    }
}

open class ViewHost: UIElement
{
    public var child: GeneratedWindowBase?
    public var activeFocusedName: String?
    public var activeEditedName: String?
    public var activeEditMode = false
    public var activeScrollView: ScrollView?
    public var activeScrollViewEditElement: UIElement?

    public override init(_ name: String)
    {
        super.init(name)
        focusable = false
    }

    public func setChild(_ child: GeneratedWindowBase)
    {
        self.child = child
        activeFocusedName = nil
        activeEditedName = nil
        activeEditMode = false
        activeScrollView = nil
        activeScrollViewEditElement = nil
        lastFocusedChildId = nil
        childFocusScopeGeneration += 1
    }

    open override var elementType: String
    {
        "viewhost"
    }

    open override var valueForSnapshot: Any?
    {
        NSNull()
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        guard let child else
        {
            return super.render(size: size, state: state)
        }
        _ = state
        let previousSuppressDim = child.suppressActiveContainerDim
        let previousActiveEditFocusName = child.activeEditFocusName
        child.suppressActiveContainerDim = activeFocusedName != nil && activeEditMode
        child.activeEditFocusName = activeEditMode
            ? (activeEditedName ?? activeFocusedName)
            : kNoActiveDynamicEditName
        let childOwnsActiveScrollView = activeScrollView != nil &&
            ownerWindowForElement(child, activeScrollView) != nil
        let childActiveScrollViewFocusBackground =
            childOwnsActiveScrollView &&
            reusableFocusStyleAppliesToChild(
                self,
                child: child,
                directFocus: state.focused,
                descendantOnlyFocus: !state.focused
            )
                ? focusStyle?.background
                : nil
        let previousGeneratedWindowStyle = child.generatedWindowStyle
        let directFocusBackground: Color?
        if state.focused,
           child.generatedScrollView() == nil,
           reusableFocusStyleAppliesToChild(
               self,
               child: child,
               directFocus: true,
               descendantOnlyFocus: false
           ),
           let focusStyle,
           let background = focusStyle.background
        {
            child.generatedWindowStyle = child.generatedWindowStyle.merged(focusStyle)
            directFocusBackground = background
        }
        else
        {
            directFocusBackground = nil
        }
        var rendered = child.renderContentForHost(
            size: size,
            focusedName: activeFocusedName,
            editMode: activeEditMode,
            activeScrollView: activeScrollView,
            activeScrollViewEditElement: activeScrollViewEditElement,
            activeScrollViewFocusBackground: childActiveScrollViewFocusBackground,
            clipTop: state.clipTop,
            clipBottom: state.clipBottom,
            useHostViewportForRootScrollViewIndicators: child.generatedScrollView() != nil
        )
        child.suppressActiveContainerDim = previousSuppressDim
        child.activeEditFocusName = previousActiveEditFocusName
        child.generatedWindowStyle = previousGeneratedWindowStyle
        if let directFocusBackground
        {
            var descendantBackgrounds: [Color] = []
            collectDescendantBaseStyleBackgrounds(child, into: &descendantBackgrounds)
            applyReusableFocusBackgroundToDescendantBackgrounds(
                content: &rendered,
                focusBackground: directFocusBackground,
                descendantBackgrounds: descendantBackgrounds
            )
            applyReusableFocusBackground(
                content: &rendered,
                focusBackground: directFocusBackground,
                baseBackground: style.background ?? state.parentBackground
            )
        }
        else if state.focused && !childOwnsActiveScrollView && !child.suppressActiveContainerScopeVisuals,
           let focusBackground = generatedChildFocusBackground(
            for: self,
            child: child,
            directFocus: state.focused,
            descendantOnlyFocus: false
           )
        {
            applyReusableHostFocusBackground(
                content: &rendered,
                child: child,
                focusBackground: focusBackground,
                baseBackground: style.background ?? state.parentBackground
            )
        }
        return rendered
    }
}

open class ReusableElement: UIElement
{
    public var child: GeneratedWindowBase?
    public var activeFocusedName: String?
    public var activeEditedName: String?
    public var activeEditMode = false
    public var activeScrollView: ScrollView?
    public var activeScrollViewEditElement: UIElement?

    public func setChild(_ child: GeneratedWindowBase)
    {
        self.child = child
        activeFocusedName = nil
        activeEditedName = nil
        activeEditMode = false
        activeScrollView = nil
        activeScrollViewEditElement = nil
        lastFocusedChildId = nil
        childFocusScopeGeneration += 1
    }

    open override var valueForSnapshot: Any?
    {
        NSNull()
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        guard let child else
        {
            return super.render(size: size, state: state)
        }
        _ = state
        let previousActiveEditFocusName = child.activeEditFocusName
        child.activeEditFocusName = activeEditMode
            ? (activeEditedName ?? activeFocusedName)
            : kNoActiveDynamicEditName
        let childOwnsActiveScrollView = activeScrollView != nil &&
            ownerWindowForElement(child, activeScrollView) != nil
        let childActiveScrollViewFocusBackground =
            childOwnsActiveScrollView &&
            reusableFocusStyleAppliesToChild(
                self,
                child: child,
                directFocus: state.focused,
                descendantOnlyFocus: !state.focused
            )
                ? focusStyle?.background
                : nil
        let previousGeneratedWindowStyle = child.generatedWindowStyle
        let directFocusBackground: Color?
        if state.focused,
           child.generatedScrollView() == nil,
           reusableFocusStyleAppliesToChild(
               self,
               child: child,
               directFocus: true,
               descendantOnlyFocus: false
           ),
           let focusStyle,
           let background = focusStyle.background
        {
            child.generatedWindowStyle = child.generatedWindowStyle.merged(focusStyle)
            directFocusBackground = background
        }
        else
        {
            directFocusBackground = nil
        }
        var rendered = child.renderContentForHost(
            size: size,
            focusedName: activeFocusedName,
            editMode: activeEditMode,
            activeScrollView: activeScrollView,
            activeScrollViewEditElement: activeScrollViewEditElement,
            activeScrollViewFocusBackground: childActiveScrollViewFocusBackground,
            clipTop: state.clipTop,
            clipBottom: state.clipBottom
        )
        child.activeEditFocusName = previousActiveEditFocusName
        child.generatedWindowStyle = previousGeneratedWindowStyle
        if let directFocusBackground
        {
            var descendantBackgrounds: [Color] = []
            collectDescendantBaseStyleBackgrounds(child, into: &descendantBackgrounds)
            applyReusableFocusBackgroundToDescendantBackgrounds(
                content: &rendered,
                focusBackground: directFocusBackground,
                descendantBackgrounds: descendantBackgrounds
            )
            applyReusableFocusBackground(
                content: &rendered,
                focusBackground: directFocusBackground,
                baseBackground: style.background ?? state.parentBackground
            )
        }
        else if state.focused && !childOwnsActiveScrollView && !child.suppressActiveContainerScopeVisuals,
           let focusBackground = generatedChildFocusBackground(
            for: self,
            child: child,
            directFocus: state.focused,
            descendantOnlyFocus: false
           )
        {
            applyReusableHostFocusBackground(
                content: &rendered,
                child: child,
                focusBackground: focusBackground,
                baseBackground: style.background ?? state.parentBackground
            )
        }
        return rendered
    }
}

public struct GeneratedElementMetadata
{
    public var name: String
    public var description: String
    public var expose: Bool

    public init(name: String, description: String = "", expose: Bool = true)
    {
        self.name = name
        self.description = description
        self.expose = expose
    }
}

public struct GeneratedAppToolMetadata
{
    public var name: String
    public var description: String
    public var inputSchemaJson: String
    public var outputSchemaJson: String

    public init(name: String, description: String = "", inputSchemaJson: String = "{}", outputSchemaJson: String = "")
    {
        self.name = name
        self.description = description
        self.inputSchemaJson = inputSchemaJson
        self.outputSchemaJson = outputSchemaJson
    }
}

public struct GeneratedWindowRuntimeOptions
{
    public var initialFocusName: String?
    public var startInEditMode = false
    public var windowStack: GeneratedWindowStack?
    public var onButton: ((String) -> Bool)?
    public var onTextChanged: ((String, String) -> Bool)?
    public var onTextConfirmed: ((String, String) -> Bool)?
    public var onSelectionChanged: ((String, [String]) -> Bool)?
    public var onKeyBeforeFocusedElement: ((String, String, Bool) -> Bool)?
    public var onKeyBeforeFocused: ((String) -> Bool)?
    public var onMousePressBeforeFocused: ((Point) -> Bool)?
    public var onMouseWheelBeforeFocused: ((Point, Int) -> Bool)?
    public var onMouseWheel: ((String, Int) -> Bool)?
    public var onEditStarted: ((String) -> Bool)?
    public var onFocusChanged: ((String, Bool) -> Void)?
    public var onKey: ((String) -> Bool)?
    public var shouldClose: (() -> Bool)?

    public init()
    {
    }
}

public struct GeneratedWindowFrameOptions
{
    public var className = ""
    public var initialFocusName: String?
    public var startInEditMode = false
    public var dimBackground = true
    public var onButton: ((String) -> Bool)?
    public var onTextChanged: ((String, String) -> Bool)?
    public var onTextConfirmed: ((String, String) -> Bool)?
    public var onSelectionChanged: ((String, [String]) -> Bool)?
    public var onKeyBeforeFocusedElement: ((String, String, Bool) -> Bool)?
    public var onKeyBeforeFocused: ((String) -> Bool)?
    public var onMousePressBeforeFocused: ((Point) -> Bool)?
    public var onMouseWheelBeforeFocused: ((Point, Int) -> Bool)?
    public var onMouseWheel: ((String, Int) -> Bool)?
    public var onEditStarted: ((String) -> Bool)?
    public var onFocusChanged: ((String, Bool) -> Void)?
    public var onKey: ((String) -> Bool)?
    public var shouldClose: (() -> Bool)?

    public init()
    {
    }
}

public final class GeneratedWindowStackFrame
{
    public let window: GeneratedWindowBase
    public var options: GeneratedWindowRuntimeOptions
    public var className: String
    public var dimBackground: Bool
    public var focusedName: String?
    public var editMode: Bool
    public var activeDynamicEditName: String?
    public var activeScrollView: ScrollView?
    public var activeScrollViewEditElement: UIElement?
    fileprivate var editSnapshot: EditSnapshot?
    fileprivate var scrollViewLastDescendantByScrollId: [ObjectIdentifier: RememberedScrollViewDescendant] = [:]

    fileprivate init(window: GeneratedWindowBase, frameOptions: GeneratedWindowFrameOptions)
    {
        self.window = window
        var runtime = window.runtimeOptions()
        runtime.initialFocusName = frameOptions.initialFocusName ?? runtime.initialFocusName
        runtime.startInEditMode = frameOptions.startInEditMode || runtime.startInEditMode
        runtime.onButton = frameOptions.onButton ?? runtime.onButton
        runtime.onTextChanged = frameOptions.onTextChanged ?? runtime.onTextChanged
        runtime.onTextConfirmed = frameOptions.onTextConfirmed ?? runtime.onTextConfirmed
        runtime.onSelectionChanged = frameOptions.onSelectionChanged ?? runtime.onSelectionChanged
        runtime.onKeyBeforeFocusedElement = frameOptions.onKeyBeforeFocusedElement ?? runtime.onKeyBeforeFocusedElement
        runtime.onKeyBeforeFocused = frameOptions.onKeyBeforeFocused ?? runtime.onKeyBeforeFocused
        runtime.onMousePressBeforeFocused = frameOptions.onMousePressBeforeFocused ?? runtime.onMousePressBeforeFocused
        runtime.onMouseWheelBeforeFocused = frameOptions.onMouseWheelBeforeFocused ?? runtime.onMouseWheelBeforeFocused
        runtime.onMouseWheel = frameOptions.onMouseWheel ?? runtime.onMouseWheel
        runtime.onEditStarted = frameOptions.onEditStarted ?? runtime.onEditStarted
        runtime.onFocusChanged = frameOptions.onFocusChanged ?? runtime.onFocusChanged
        runtime.onKey = frameOptions.onKey ?? runtime.onKey
        runtime.shouldClose = frameOptions.shouldClose ?? runtime.shouldClose
        self.options = runtime
        self.className = frameOptions.className
        self.dimBackground = frameOptions.dimBackground
        self.focusedName = runtime.initialFocusName
        self.editMode = runtime.startInEditMode
    }
}

public final class GeneratedWindowStack
{
    private var frames: [GeneratedWindowStackFrame] = []

    public init()
    {
    }

    public func push(_ window: GeneratedWindowBase, _ options: GeneratedWindowFrameOptions = GeneratedWindowFrameOptions())
    {
        frames.append(GeneratedWindowStackFrame(window: window, frameOptions: options))
    }

    public func remove(_ window: GeneratedWindowBase)
    {
        frames.removeAll { $0.window === window }
    }

    public func popTop()
    {
        if !frames.isEmpty
        {
            frames.removeLast()
        }
    }

    fileprivate var topFrame: GeneratedWindowStackFrame?
    {
        frames.last
    }

    fileprivate var allFrames: [GeneratedWindowStackFrame]
    {
        frames
    }
}

open class GeneratedWindowBase
{
    public let windowTitle: String
    public private(set) var generatedKind = "window"
    public private(set) var generatedFocusable = false
    public private(set) var layout: [GeneratedLayoutEntry] = []
    public private(set) var elements: [UIElement] = []
    private var dynamicElementRegistry: [String: UIElement] = [:]
    private var dynamicElementOrder: [String] = []
    public private(set) var closeRequested = false
    public private(set) var mcpEnabled = false
    public private(set) var mcpClassName = ""
    public private(set) var mcpSource = ""
    public private(set) var mcpDescription = ""
    public private(set) var mcpElementMetadata: [GeneratedElementMetadata] = []
    public private(set) var mcpAppTools: [GeneratedAppToolMetadata] = []
    public var generatedWindowStyle = Style()
    public var generatedWindowFocusStyle: Style?
    public var mode = ""
    public var activeEditFocusName: String?
    fileprivate var suppressActiveContainerDim = false
    fileprivate var suppressActiveContainerScopeVisuals = false
    private var forcedFullscreenRenderDepth = 0
    private var hostViewportRootScrollIndicatorsDepth = 0
    public var suppressGeneratedFocusVisuals: Bool
    {
        get
        {
            suppressActiveContainerScopeVisuals
        }
        set
        {
            suppressActiveContainerScopeVisuals = newValue
        }
    }

    public init(_ title: String)
    {
        self.windowTitle = title
    }

    @discardableResult
    public func addElement<T: UIElement>(_ element: T) -> T
    {
        elements.append(element)
        return element
    }

    public func setGeneratedLayout(_ layout: [GeneratedLayoutEntry])
    {
        self.layout = layout
    }

    public func setGeneratedLayoutCellBackground(_ background: Color)
    {
        for index in layout.indices
        {
            layout[index].cellStyle.background = background
        }
    }

    public func setGeneratedWindowStyle(_ style: Style)
    {
        generatedWindowStyle = style
    }

    public func setGeneratedWindowFocusStyle(_ style: Style)
    {
        generatedWindowFocusStyle = style
    }

    public func setGeneratedFocusable(_ focusable: Bool)
    {
        generatedFocusable = focusable
    }

    public func setGeneratedKind(_ kind: String)
    {
        generatedKind = kind
    }

    public func setMcpMetadata(
        enabled: Bool,
        className: String,
        source: String,
        description: String,
        elementMetadata: [GeneratedElementMetadata] = []
    )
    {
        mcpEnabled = enabled
        mcpClassName = className
        mcpSource = source
        mcpDescription = description
        mcpElementMetadata = elementMetadata
    }

    public func setMcpAppTools(_ appTools: [GeneratedAppToolMetadata])
    {
        mcpAppTools = appTools
    }

    open func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = GeneratedWindowRuntimeOptions()
        options.onButton = { [weak self] name in self?.handleGeneratedButton(name) ?? false }
        options.onTextChanged = { [weak self] name, value in self?.handleGeneratedTextChanged(name, value: value) ?? false }
        options.onTextConfirmed = { [weak self] name, value in self?.handleGeneratedTextConfirmed(name, value: value) ?? false }
        options.onSelectionChanged = { [weak self] name, value in self?.handleGeneratedSelectionChanged(name, value: value) ?? false }
        options.shouldClose = { [weak self] in self?.shouldClose() ?? false }
        return options
    }

    public func requestClose()
    {
        closeRequested = true
    }

    open func shouldClose() -> Bool
    {
        closeRequested
    }

    open func callAppTool(_ name: String, inputJson: String) throws -> String
    {
        _ = inputJson
        throw RuntimeError("App MCP tool is not implemented: \(name)")
    }

    open func handleGeneratedButton(_ name: String) -> Bool
    {
        _ = name
        return false
    }

    open func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        _ = name
        _ = value
        return false
    }

    open func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        _ = name
        _ = value
        return false
    }

    open func handleGeneratedKey(_ key: String) -> Bool
    {
        _ = key
        return false
    }

    open func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        _ = name
        _ = value
        return false
    }

    open func generatedScrollView() -> ScrollView?
    {
        nil
    }

    public func element(named name: String) -> UIElement?
    {
        if let element = elements.first(where: { $0.name == name })
        {
            return element
        }
        if let element = dynamicElementRegistry[name]
        {
            element.mcpElementId = name
            return element
        }
        return nil
    }

    public func clearDynamicElements()
    {
        for element in dynamicElementRegistry.values
        {
            element.mcpElementId = nil
            element.parentFocusHostId = nil
        }
        dynamicElementRegistry.removeAll()
        dynamicElementOrder.removeAll()
    }

    public func registerDynamicElement(_ id: String, element: UIElement, parentFocusHostId: String? = nil)
    {
        guard !id.isEmpty else
        {
            return
        }
        if dynamicElementRegistry[id] == nil
        {
            dynamicElementOrder.append(id)
        }
        element.mcpElementId = id
        element.parentFocusHostId = parentFocusHostId
        dynamicElementRegistry[id] = element
    }

    public func dynamicFocusableIds() -> [String]
    {
        dynamicElementOrder.filter
        {
            guard let element = dynamicElementRegistry[$0] else
            {
                return false
            }
            return generatedElementIsFocusable(element)
        }
    }

    public func focusableElementIds() -> [String]
    {
        elements.filter { generatedElementIsFocusable($0) }.map(\.name) + dynamicFocusableIds()
    }

    public func runtimeElements() -> [UIElement]
    {
        elements + dynamicElementOrder.compactMap { dynamicElementRegistry[$0] }
    }

    public func generatedElementOwnsScrollableContent(_ element: UIElement) -> Bool
    {
        if let reusable = element as? ReusableElement,
           reusable.child is GeneratedScrollableContent
        {
            return true
        }
        if let viewHost = element as? ViewHost,
           viewHost.child is GeneratedScrollableContent
        {
            return true
        }
        return false
    }

    public func generatedFocusedNameIsInActiveEditScope(_ focusedName: String?, editMode: Bool) -> Bool
    {
        guard editMode else
        {
            return false
        }
        guard let activeEditFocusName else
        {
            return true
        }
        if activeEditFocusName == kNoActiveDynamicEditName
        {
            guard let focusedName
            else
            {
                return false
            }
            guard let currentElement = element(named: focusedName)
            else
            {
                return focusedName.contains(".") || focusedName.contains("[")
            }
            if generatedElementOwnsScrollableContent(currentElement)
            {
                return true
            }
            if let parentId = currentElement.parentFocusHostId,
               let parent = element(named: parentId)
            {
                return generatedElementOwnsScrollableContent(parent)
            }
            return false
        }
        guard let focusedName else
        {
            return false
        }
        return activeEditFocusName == focusedName ||
            activeEditFocusName.hasPrefix(focusedName + ".") ||
            activeEditFocusName.hasPrefix(focusedName + "[") ||
            focusedName.hasPrefix(activeEditFocusName + ".") ||
            focusedName.hasPrefix(activeEditFocusName + "[")
    }

    public func generatedFocusedNameIsActivelyEdited(_ focusedName: String?, editMode: Bool) -> Bool
    {
        guard editMode else
        {
            return false
        }
        guard let activeEditFocusName else
        {
            return true
        }
        if activeEditFocusName == kNoActiveDynamicEditName
        {
            return false
        }
        return activeEditFocusName == focusedName
    }

    public func elementMetadata(_ name: String) -> GeneratedElementMetadata?
    {
        mcpElementMetadata.first { $0.name == name }
    }

    public func renderPlainText() -> String
    {
        renderedText(renderContent(size: generatedContentSize(), focusedName: nil, editMode: false)).joined(separator: "\n")
    }

    public func generatedContentSize() -> Size
    {
        var runtimeCells = collectRuntimeCells(self)
        let natural = runtimeLayoutSize(buildRuntimeTree(
            runtimeCells,
            horizontalSeparator: runtimeBorderWidthHorizontal(generatedWindowStyle),
            verticalSeparator: runtimeBorderWidthVertical(generatedWindowStyle)
        ))
        var resolved: [ResolvedRuntimeCell] = []
        resolveRuntimeCellsWithFitPass(
            self,
            runtimeCells: &runtimeCells,
            content: Rect(row: 0, col: 0, width: natural.width, height: natural.height),
            mode: generatedRuntimeMode(self),
            resolvedCells: &resolved
        )
        return resolvedContentExtent(resolved, fallback: natural)
    }

    public func generatedContentSizeForWidth(_ width: Int) -> Size
    {
        var runtimeCells = collectRuntimeCells(self)
        let natural = runtimeLayoutSize(buildRuntimeTree(
            runtimeCells,
            horizontalSeparator: runtimeBorderWidthHorizontal(generatedWindowStyle),
            verticalSeparator: runtimeBorderWidthVertical(generatedWindowStyle)
        ))
        var resolved: [ResolvedRuntimeCell] = []
        resolveRuntimeCellsWithFitPass(
            self,
            runtimeCells: &runtimeCells,
            content: Rect(row: 0, col: 0, width: max(1, width), height: natural.height),
            mode: generatedRuntimeMode(self),
            resolvedCells: &resolved
        )
        var result = resolvedContentExtent(resolved, fallback: Size(width: max(1, width), height: natural.height))
        for entry in layout
        {
            guard let cell = resolvedCellFor(resolved, entry: entry) else
            {
                continue
            }
            let element = entry.name.isEmpty ? nil : self.element(named: entry.name)
            let entrySize = renderSizeFor(entry, cellRect: cell.rect, element: element)
            result.width = max(result.width, cell.rect.col + paddingLeft(entry.cellStyle) + entry.relative.col + entrySize.width)
            result.height = max(result.height, cell.rect.row + paddingTop(entry.cellStyle) + entry.relative.row + entrySize.height)
        }
        return result
    }

    public func generatedResolvedContentSize(_ size: Size) -> Size
    {
        var runtimeCells = collectRuntimeCells(self)
        var resolved: [ResolvedRuntimeCell] = []
        resolveRuntimeCellsWithFitPass(
            self,
            runtimeCells: &runtimeCells,
            content: Rect(row: 0, col: 0, width: max(1, size.width), height: max(1, size.height)),
            mode: generatedRuntimeMode(self),
            resolvedCells: &resolved
        )
        return resolvedContentExtent(resolved, fallback: Size(width: max(1, size.width), height: max(1, size.height)))
    }

    public func syncElementFrames(to frame: Rect, forceFullscreenLayout: Bool = true)
    {
        syncGeneratedWindowElementFrames(self, to: frame, forceFullscreenLayout: forceFullscreenLayout)
    }

    open func renderContent(
        size: Size,
        focusedName: String?,
        editMode: Bool
    ) -> [[TerminalCell]]
    {
        renderGeneratedContent(
            size: size,
            focusedName: focusedName,
            editMode: editMode,
            activeScrollView: nil,
            activeScrollViewEditElement: nil,
            activeScrollViewFocusBackground: nil
        )
    }

    open func renderContent(
        size: Size,
        focusedName: String?,
        editMode: Bool,
        activeScrollView: ScrollView?,
        activeScrollViewEditElement: UIElement?
    ) -> [[TerminalCell]]
    {
        if activeScrollView == nil && activeScrollViewEditElement == nil
        {
            return renderContent(size: size, focusedName: focusedName, editMode: editMode)
        }
        return renderGeneratedContent(
            size: size,
            focusedName: focusedName,
            editMode: editMode,
            activeScrollView: activeScrollView,
            activeScrollViewEditElement: activeScrollViewEditElement,
            activeScrollViewFocusBackground: nil
        )
    }

    private func renderGeneratedContent(
        size: Size,
        focusedName: String?,
        editMode: Bool,
        activeScrollView: ScrollView?,
        activeScrollViewEditElement: UIElement?,
        activeScrollViewFocusBackground: Color?,
        clipTop renderClipTop: Int? = nil,
        clipBottom renderClipBottom: Int? = nil
    ) -> [[TerminalCell]]
    {
        let focusedElementBeforeRender = focusedName.flatMap { element(named: $0) }
        clearDynamicElements()
        let width = max(1, size.width)
        let height = max(1, size.height)
        var buffer = Array(
            repeating: Array(
                repeating: TerminalCell(" ", foreground: generatedWindowStyle.color, background: generatedWindowStyle.background),
                count: width
            ),
            count: height
        )
        var runtimeCells = collectRuntimeCells(self)
        var resolvedCells: [ResolvedRuntimeCell] = []
        resolveRuntimeCellsWithFitPass(
            self,
            runtimeCells: &runtimeCells,
            content: Rect(row: 0, col: 0, width: width, height: height),
            mode: forcedFullscreenRenderDepth > 0 ? "fullscreen" : generatedRuntimeMode(self),
            resolvedCells: &resolvedCells
        )
        var paintedCells = Set<RuntimeCell>()
        var activeContainerScope: (element: UIElement, rect: Rect, dim: Bool, clip: Rect, focus: Bool)?
        func childFocusName(hostName: String, focusedName: String?) -> String?
        {
            guard let focusedName else
            {
                return nil
            }
            let prefix = hostName + "."
            if focusedName.hasPrefix(prefix)
            {
                let rest = String(focusedName.dropFirst(prefix.count))
                guard !rest.isEmpty else
                {
                    return nil
                }
                return rest
            }
            guard focusedName.hasPrefix(hostName + "[") else
            {
                return nil
            }
            let rest = String(focusedName.dropFirst(hostName.count))
            guard !rest.isEmpty else
            {
                return nil
            }
            return rest
        }
        func ownsGeneratedScrollableContent(_ element: UIElement) -> Bool
        {
            if let reusable = element as? ReusableElement,
               reusable.child is GeneratedScrollableContent
            {
                return true
            }
            if let viewHost = element as? ViewHost,
               viewHost.child is GeneratedScrollableContent
            {
                return true
            }
            return false
        }
        func scrollChildFocusName(_ fullFocusedName: String?, index: Int, scrollEntryName: String) -> String?
        {
            guard let fullFocusedName, !fullFocusedName.isEmpty else
            {
                return nil
            }
            let indexedPrefix = "[\(index)]."
            if fullFocusedName.hasPrefix(indexedPrefix)
            {
                return String(fullFocusedName.dropFirst(indexedPrefix.count))
            }
            let entryPrefix = scrollEntryName + indexedPrefix
            if fullFocusedName.hasPrefix(entryPrefix)
            {
                return String(fullFocusedName.dropFirst(entryPrefix.count))
            }
            guard let range = fullFocusedName.range(of: indexedPrefix) else
            {
                return nil
            }
            return String(fullFocusedName[range.upperBound...])
        }
        func localFocusName(in childWindow: GeneratedWindowBase, target: UIElement?) -> String?
        {
            guard let target else
            {
                return nil
            }
            for element in childWindow.elements
            {
                if element === target
                {
                    return element.name
                }
                if let generated = generatedWindowChild(of: element),
                   let nested = localFocusName(in: generated, target: target)
                {
                    return element.name + "." + nested
                }
                guard let scrollView = element as? ScrollView else
                {
                    continue
                }
                let scrollFrame = scrollView.frame
                for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
                {
                    guard let child = childView.element else
                    {
                        continue
                    }
                    let indexed = "\(element.name)[\(childView.index)]"
                    if child === target
                    {
                        return indexed
                    }
                    if let generated = generatedWindowChild(of: child),
                       let nested = localFocusName(in: generated, target: target)
                    {
                        return indexed + "." + nested
                    }
                }
            }
            return nil
        }
        func intersect(_ first: Rect, _ second: Rect) -> Rect
        {
            let top = max(first.row, second.row)
            let left = max(first.col, second.col)
            let bottom = min(first.row + first.height, second.row + second.height)
            let right = min(first.col + first.width, second.col + second.width)
            return Rect(row: top, col: left, width: max(0, right - left), height: max(0, bottom - top))
        }
        func offsetGeneratedChildFrames(_ child: GeneratedWindowBase, by origin: Rect)
        {
            for element in child.elements
            {
                element.frame = Rect(
                    row: origin.row + element.frame.row,
                    col: origin.col + element.frame.col,
                    width: element.frame.width,
                    height: element.frame.height
                )
                if let reusable = element as? ReusableElement,
                   let nested = reusable.child
                {
                    offsetGeneratedChildFrames(nested, by: element.frame)
                }
                if let viewHost = element as? ViewHost,
                   let nested = viewHost.child
                {
                    offsetGeneratedChildFrames(nested, by: element.frame)
                }
            }
        }
        func focusedNameIsActivelyEdited(_ fullFocusedName: String?) -> Bool
        {
            generatedFocusedNameIsActivelyEdited(fullFocusedName, editMode: editMode)
        }
        func focusedNameIsInActiveEditScope(_ fullFocusedName: String?) -> Bool
        {
            generatedFocusedNameIsInActiveEditScope(fullFocusedName, editMode: editMode)
        }
        func childActiveEditedName(hostName: String) -> String?
        {
            guard editMode,
                  let activeEditFocusName
            else
            {
                return nil
            }
            if activeEditFocusName == kNoActiveDynamicEditName
            {
                return kNoActiveDynamicEditName
            }
            return childFocusName(hostName: hostName, focusedName: activeEditFocusName)
        }
        func immediateChildFocusName(_ focusedName: String?) -> String?
        {
            guard let focusedName, !focusedName.isEmpty else
            {
                return nil
            }
            if let separator = focusedName.firstIndex(where: { $0 == "." || $0 == "[" })
            {
                return String(focusedName[..<separator])
            }
            return focusedName
        }
        for entry in layout
        {
            guard let cell = resolvedCellFor(resolvedCells, entry: entry) else
            {
                continue
            }
            if paintedCells.insert(cell.cell).inserted
            {
                fill(buffer: &buffer, rect: clipped(cell.rect, width: width, height: height), style: entry.cellStyle)
            }
        }
        func drawEntry(_ entry: GeneratedLayoutEntry, element: UIElement, cellRect: Rect, elementEditMode: Bool)
        {
            let size = renderSizeFor(entry, cellRect: cellRect, element: element)
            let rect = Rect(
                row: renderRowFor(layout, entry: entry, cellRect: cellRect, size: size),
                col: renderColFor(layout, entry: entry, cellRect: cellRect, size: size),
                width: size.width,
                height: size.height
            )
            element.frame = rect
            let previous = element.style
            if previous == Style()
            {
                element.style = entry.elementStyle
            }
            let parentBackground: Color?
            if rect.row >= 0 && rect.row < buffer.count && rect.col >= 0 && rect.col < buffer[rect.row].count
            {
                parentBackground = buffer[rect.row][rect.col].background
            }
            else
            {
                parentBackground = nil
            }
            let entryClip = cellContentClip(entry, cellRect: cellRect)
            var elementClipTop: Int?
            var elementClipBottom: Int?
            do
            {
                let baseClipTop = max(0, entryClip.row)
                let baseClipBottom = max(baseClipTop, min(buffer.count, entryClip.row + entryClip.height))
                let absoluteClipTop = max(baseClipTop, renderClipTop ?? baseClipTop)
                let absoluteClipBottom = max(
                    absoluteClipTop,
                    min(baseClipBottom, renderClipBottom ?? baseClipBottom)
                )
                let visibleTop = max(rect.row, absoluteClipTop)
                let visibleBottom = min(rect.row + rect.height, absoluteClipBottom)
                elementClipTop = max(0, visibleTop - rect.row)
                elementClipBottom = max(0, visibleBottom - rect.row)
                if elementClipTop == 0 && elementClipBottom == rect.height
                {
                    elementClipTop = nil
                    elementClipBottom = nil
                }
            }
            let nestedChildWindow = generatedWindowChild(of: element)
            let nestedActiveScrollView = activeScrollView.flatMap
            {
                scrollView in
                nestedChildWindow.flatMap { ownerWindowForElement($0, scrollView) } == nil ? nil : scrollView
            }
            let nestedActiveScrollViewEditElement = activeScrollViewEditElement.flatMap
            {
                editElement in
                nestedChildWindow.flatMap { ownerWindowForElement($0, editElement) } == nil ? nil : editElement
            }
            var nestedFocus = childFocusName(hostName: entry.name, focusedName: focusedName)
            if nestedFocus == nil,
               let focusedName,
               let child = nestedChildWindow,
               child.element(named: focusedName) != nil
            {
                nestedFocus = focusedName
            }
            if nestedFocus == nil,
               let child = nestedChildWindow,
               let scrollView = nestedActiveScrollView
            {
                nestedFocus = localFocusName(in: child, target: scrollView)
            }
            var nestedActiveEditedName = childActiveEditedName(hostName: entry.name)
            if nestedActiveEditedName == nil,
               editMode,
               let activeEditFocusName,
               activeEditFocusName != kNoActiveDynamicEditName,
               let child = nestedChildWindow,
               child.element(named: activeEditFocusName) != nil
            {
                nestedActiveEditedName = activeEditFocusName
            }
            let nestedFocusIsLocalChild = focusedName != nil &&
                nestedChildWindow?.element(named: focusedName!) != nil
            let nestedOwnsActiveScrollView = nestedActiveScrollView != nil
            let nestedOwnsActiveScrollViewEditElement = nestedActiveScrollViewEditElement != nil
            let nestedActiveEditMode = nestedFocus != nil &&
                editMode &&
                (activeScrollView == nil ||
                 nestedOwnsActiveScrollView ||
                 nestedOwnsActiveScrollViewEditElement) &&
                (focusedNameIsInActiveEditScope(focusedName) || nestedFocusIsLocalChild || nestedOwnsActiveScrollView)
            let activeScrollViewFocused =
                !suppressActiveContainerScopeVisuals &&
                ((activeScrollView != nil && activeScrollView === element) ||
                 ((element as? ScrollView).map
                 {
                    scrollView in
                    (focusedElementBeforeRender != nil &&
                     focusedElementBeforeRender !== scrollView &&
                     elementTreeContains(scrollView, focusedElementBeforeRender)) ||
                    (activeScrollViewEditElement != nil &&
                     elementTreeContains(scrollView, activeScrollViewEditElement))
                 } ?? false))
            let scrollViewSelfFocused = element is ScrollView && focusedName == entry.name && !activeScrollViewFocused
            let state = ElementRenderState(
                focused: focusedName == entry.name && !scrollViewSelfFocused && !activeScrollViewFocused,
                editMode: elementEditMode && !activeScrollViewFocused,
                parentBackground: parentBackground,
                clipTop: elementClipTop,
                clipBottom: elementClipBottom
            )
            if let viewHost = element as? ViewHost
            {
                viewHost.activeFocusedName = nestedFocus
                viewHost.activeEditedName = nestedActiveEditedName
                viewHost.activeEditMode = nestedActiveEditMode
                viewHost.activeScrollView = nestedActiveScrollView
                viewHost.activeScrollViewEditElement = nestedActiveScrollViewEditElement
            }
            if let reusable = element as? ReusableElement
            {
                reusable.activeFocusedName = nestedFocus
                reusable.activeEditedName = nestedActiveEditedName
                reusable.activeEditMode = nestedActiveEditMode
                reusable.activeScrollView = nestedActiveScrollView
                reusable.activeScrollViewEditElement = nestedActiveScrollViewEditElement
            }
            let activeViewHostContainerChild = (element as? ViewHost)
                .flatMap { host in immediateChildFocusName(nestedFocus).flatMap { host.child?.element(named: $0) } }
                .flatMap { $0 is ReusableElement || $0 is ViewHost ? $0 : nil }
            let activeReusableContainer =
                element is ReusableElement &&
                (elementEditMode || nestedFocus != nil) &&
                ownsGeneratedScrollableContent(element) &&
                !(editMode && nestedOwnsActiveScrollView)
            var childScopeSuppressionRestore: (() -> Void)?
            if let viewHost = element as? ViewHost, let child = viewHost.child
            {
                let previous = child.suppressActiveContainerScopeVisuals
                child.suppressActiveContainerScopeVisuals = previous || suppressActiveContainerScopeVisuals
                childScopeSuppressionRestore = {
                    child.suppressActiveContainerScopeVisuals = previous
                }
            }
            else if let reusable = element as? ReusableElement, let child = reusable.child
            {
                let previous = child.suppressActiveContainerScopeVisuals
                child.suppressActiveContainerScopeVisuals = previous || suppressActiveContainerScopeVisuals
                childScopeSuppressionRestore = {
                    child.suppressActiveContainerScopeVisuals = previous
                }
            }
            let rendered = element.render(size: Size(width: rect.width, height: rect.height), state: state)
            childScopeSuppressionRestore?()
            var clip = entryClip
            if element is ComboBox, elementEditMode, !rendered.isEmpty
            {
                let clipBottom = max(clip.row + clip.height, rect.row + rendered.count)
                clip.height = max(0, clipBottom - clip.row)
            }
            if generatedWindowChild(of: element) != nil, editMode, nestedActiveEditMode, !rendered.isEmpty
            {
                let renderedWidth = rendered.reduce(0) { max($0, $1.count) }
                let clipBottom = max(clip.row + clip.height, rect.row + rendered.count)
                let clipRight = max(clip.col + clip.width, rect.col + renderedWidth)
                clip.height = max(0, clipBottom - clip.row)
                clip.width = max(0, clipRight - clip.col)
            }
            blitClipped(buffer: &buffer, rendered: rendered, row: rect.row, col: rect.col, clip: clip)
            if generatedWindowChild(of: element) != nil
            {
                registerGeneratedHostChildElements(
                    on: self,
                    host: element,
                    hostId: entry.name,
                    hostFrame: rect
                )
            }
            if let scrollView = element as? ScrollView
            {
                scrollView.childFocusOrder.removeAll()
                let viewport = generatedScrollViewport(size: Size(width: rect.width, height: rect.height), style: scrollView.style)
                let scrollViewportFrame = Rect(
                    row: rect.row + viewport.row,
                    col: rect.col + viewport.col,
                    width: viewport.width,
                    height: viewport.height
                )
                let scrollClip = intersect(
                    scrollViewportFrame,
                    clip
                )
                if scrollClip.width > 0 && scrollClip.height > 0
                {
                    var childContentAboveViewport = false
                    var childContentBelowViewport = false
                    let childViews = scrollView.childViews(size: Size(width: rect.width, height: rect.height))
                    for childView in childViews
                    {
                        let childFrame = Rect(
                            row: rect.row + childView.frame.row,
                            col: rect.col + childView.frame.col,
                            width: childView.frame.width,
                            height: childView.frame.height
                        )
                        if childFrame.row < scrollViewportFrame.row
                        {
                            childContentAboveViewport = true
                        }
                        if childFrame.row + childFrame.height > scrollViewportFrame.row + scrollViewportFrame.height
                        {
                            childContentBelowViewport = true
                        }
                        guard childView.visible,
                              let child = childView.element
                        else
                        {
                            continue
                        }
                        child.frame = childFrame
                        var localFocus = scrollChildFocusName(focusedName, index: childView.index, scrollEntryName: entry.name)
                        let childEditMode = localFocus != nil && editMode
                        let childParentBackground: Color?
                        if childFrame.row >= 0 && childFrame.row < buffer.count &&
                            childFrame.col >= 0 && childFrame.col < buffer[childFrame.row].count
                        {
                            childParentBackground = buffer[childFrame.row][childFrame.col].background
                        }
                        else
                        {
                            childParentBackground = nil
                        }
                        var childClipTop: Int?
                        var childClipBottom: Int?
                        if childView.clipped
                        {
                            childClipTop = max(0, scrollClip.row - childFrame.row)
                            childClipBottom = max(
                                0,
                                min(childFrame.height, scrollClip.row + scrollClip.height - childFrame.row)
                            )
                        }
                        let childRendered: [[TerminalCell]]
                        if let reusable = child as? ReusableElement,
                           let generated = reusable.child
                        {
                            if localFocus == nil
                            {
                                localFocus = localFocusName(in: generated, target: focusedElementBeforeRender)
                            }
                            let reusableGeneratedScrollView = generated.generatedScrollView()
                            let childDirectFocus = (focusedName.map
                            {
                                $0 == child.name || $0 == child.mcpElementId
                            } ?? false) || child === focusedElementBeforeRender
                            let childOwnsActiveScrollView = activeScrollView != nil &&
                                ownerWindowForElement(generated, activeScrollView) != nil
                            let childOwnsActiveScrollViewEditElement = activeScrollViewEditElement != nil &&
                                ownerWindowForElement(generated, activeScrollViewEditElement) != nil
                            let childHasFocusedElement = focusedElementBeforeRender != nil &&
                                ownerWindowForElement(generated, focusedElementBeforeRender) != nil
                            let navigationScopeDescendantFocused =
                                !suppressActiveContainerScopeVisuals &&
                                !editMode &&
                                activeScrollView === scrollView &&
                                childHasFocusedElement
                            let reusableDescendantFocused = childDirectFocus ||
                                navigationScopeDescendantFocused ||
                                (editMode && (
                                    localFocus != nil ||
                                    childHasFocusedElement ||
                                    childOwnsActiveScrollView ||
                                    childOwnsActiveScrollViewEditElement
                                ))
                            let effectiveChildEditMode = editMode &&
                                (localFocus != nil || childOwnsActiveScrollView || childOwnsActiveScrollViewEditElement) &&
                                (activeScrollView == nil || childOwnsActiveScrollView || childOwnsActiveScrollViewEditElement)
                            let reusableWholeChildFocus =
                                reusableDescendantFocused &&
                                !generatedHasMultipleFocusableElements(generated, activeScrollView: activeScrollView)
                            var childActiveScrollViewFocusBackground: Color?
                            var childDescendantFocusBackground: Color?
                            let previousActiveEditFocusName = generated.activeEditFocusName
                            let previousGeneratedWindowStyle = generated.generatedWindowStyle
                            generated.activeEditFocusName = effectiveChildEditMode ? (localFocus ?? activeEditFocusName) : kNoActiveDynamicEditName
                            if reusableWholeChildFocus
                            {
                                let applyReusableFocusStyle = reusableFocusStyleAppliesToChild(
                                    reusable,
                                    child: generated,
                                    directFocus: childDirectFocus,
                                    descendantOnlyFocus: reusableDescendantFocused && !childDirectFocus
                                )
                                if applyReusableFocusStyle
                                {
                                    if reusableGeneratedScrollView == nil
                                    {
                                        generated.generatedWindowStyle = generated.generatedWindowStyle.merged(reusable.focusStyle)
                                    }
                                    if let background = reusable.focusStyle?.background
                                    {
                                        childActiveScrollViewFocusBackground = background
                                    }
                                }
                                if childActiveScrollViewFocusBackground == nil,
                                   let background = scrollView.descendantFocusStyle?.background
                                {
                                    childActiveScrollViewFocusBackground = background
                                }
                                if childActiveScrollViewFocusBackground == nil,
                                   let background = scrollView.focusStyle?.background
                                {
                                    childActiveScrollViewFocusBackground = background
                                }
                                childDescendantFocusBackground = childActiveScrollViewFocusBackground
                            }
                            if childDescendantFocusBackground == nil,
                               editMode,
                               localFocus != nil,
                               reusableWholeChildFocus
                            {
                                childDescendantFocusBackground =
                                    scrollView.descendantFocusStyle?.background ?? scrollView.focusStyle?.background
                            }
                            var renderedChild = generated.renderContentForHost(
                                size: Size(width: childFrame.width, height: childFrame.height),
                                focusedName: localFocus,
                                editMode: effectiveChildEditMode,
                                activeScrollView: childOwnsActiveScrollView ? activeScrollView : nil,
                                activeScrollViewEditElement: childOwnsActiveScrollViewEditElement ? activeScrollViewEditElement : nil,
                                activeScrollViewFocusBackground: childActiveScrollViewFocusBackground,
                                clipTop: childClipTop,
                                clipBottom: childClipBottom,
                                useHostViewportForRootScrollViewIndicators: false
                            )
                            generated.activeEditFocusName = previousActiveEditFocusName
                            generated.generatedWindowStyle = previousGeneratedWindowStyle
                            if let focusBackground = childDescendantFocusBackground,
                               reusableGeneratedScrollView == nil
                            {
                                var descendantBackgrounds: [Color] = []
                                collectDescendantBaseStyleBackgrounds(generated, into: &descendantBackgrounds)
                                applyReusableFocusBackgroundToDescendantBackgrounds(
                                    content: &renderedChild,
                                    focusBackground: focusBackground,
                                    descendantBackgrounds: descendantBackgrounds
                                )
                                if reusableWholeChildFocus,
                                   let selectedBackground = scrollView.selectedStyle?.background
                                {
                                    applySelectedBackgroundToFocusedReusableChild(
                                        content: &renderedChild,
                                        selectedBackground: selectedBackground,
                                        focusBackground: focusBackground,
                                        descendantBackgrounds: descendantBackgrounds
                                    )
                                }
                            }
                            if let focusBackground = childActiveScrollViewFocusBackground,
                               reusableGeneratedScrollView == nil
                            {
                                applyReusableFocusBackground(
                                    content: &renderedChild,
                                    focusBackground: focusBackground,
                                    baseBackground: reusable.style.background
                                )
                            }
                            childRendered = renderedChild
                            offsetGeneratedChildFrames(generated, by: childFrame)
                        }
                        else
                        {
                            childRendered = child.render(
                                size: Size(width: childFrame.width, height: childFrame.height),
                                state: ElementRenderState(
                                    focused: localFocus == child.name || focusedName == child.name || child === focusedElementBeforeRender,
                                    editMode: childEditMode,
                                    parentBackground: childParentBackground,
                                    clipTop: childClipTop,
                                    clipBottom: childClipBottom
                                )
                            )
                        }
                        registerGeneratedScrollChildElements(
                            on: self,
                            scrollEntryName: entry.name,
                            scrollFrame: Rect(row: rect.row, col: rect.col, width: rect.width, height: rect.height),
                            childView: childView,
                            child: child
                        )
                        blitClipped(buffer: &buffer, rendered: childRendered, row: childFrame.row, col: childFrame.col, clip: scrollClip)
                    }
                    let useHostViewportForRootScrollViewIndicators =
                        hostViewportRootScrollIndicatorsDepth > 0 && self.generatedScrollView() === scrollView
                    if !useHostViewportForRootScrollViewIndicators
                    {
                        let viewportSize = Size(width: scrollViewportFrame.width, height: scrollViewportFrame.height)
                        let indicatorCol = scrollViewportFrame.col + scrollViewportFrame.width - kMinimumRenderableSize
                        let maxViewOffset = scrollView.maxViewOffset(viewport: viewportSize)
                        let viewOffset = scrollView.viewOffsetValue()
                        if viewOffset < maxViewOffset || childContentAboveViewport
                        {
                            applyScrollIndicator(
                                content: &buffer,
                                row: scrollViewportFrame.row,
                                col: indicatorCol,
                                indicator: kScrollIndicatorAbove,
                                foregroundStartCol: scrollViewportFrame.col
                            )
                        }
                        if viewOffset > 0 || childContentBelowViewport
                        {
                            applyScrollIndicator(
                                content: &buffer,
                                row: scrollViewportFrame.row + scrollViewportFrame.height - kMinimumRenderableSize,
                                col: indicatorCol,
                                indicator: kScrollIndicatorBelow,
                                foregroundStartCol: scrollViewportFrame.col
                            )
                        }
                    }
                    if activeScrollViewFocused || scrollViewSelfFocused
                    {
                        applyScrollableHostFocusBackground(
                            buffer: &buffer,
                            active: Rect(row: rect.row, col: rect.col, width: rect.width, height: rect.height),
                            style: scrollView.style,
                            focusBackground: activeScrollViewFocusBackground ?? scrollView.focusStyle?.background,
                            baseBackground: parentBackground ?? entry.cellStyle.background
                        )
                    }
                }
            }
            if editMode && activeReusableContainer
            {
                activeContainerScope = (element, rect, true, clip, true)
            }
            else if editMode, let activeViewHostContainerChild
            {
                let childOwnsScrollableContent = ownsGeneratedScrollableContent(activeViewHostContainerChild)
                var activeChildRect = activeViewHostContainerChild.frame
                if childOwnsScrollableContent,
                   let child = generatedWindowChild(of: activeViewHostContainerChild),
                   let scrollView = child.generatedScrollView()
                {
                    activeChildRect.width += paddingRight(scrollView.style)
                    activeChildRect.height += paddingBottom(scrollView.style)
                }
                activeContainerScope = (
                    activeViewHostContainerChild,
                    activeChildRect,
                    childOwnsScrollableContent,
                    clip,
                    !childOwnsScrollableContent
                )
            }
            else if !editMode && focusedName == entry.name && (element is ReusableElement || element is ViewHost)
            {
                activeContainerScope = (element, rect, false, clip, true)
            }
            element.style = previous
        }
        var deferredComboBox: (entry: GeneratedLayoutEntry, element: UIElement, cellRect: Rect)?
        for entry in layout
        {
            guard !entry.name.isEmpty else
            {
                continue
            }
            guard let cell = resolvedCellFor(resolvedCells, entry: entry) else
            {
                continue
            }
            guard let element = element(named: entry.name) else
            {
                var text = entry.name
                if text.count >= 2 && text.first == "\"" && text.last == "\""
                {
                    text = String(text.dropFirst().dropLast())
                }
                let size = renderSizeFor(entry, cellRect: cell.rect, element: nil)
                let rect = Rect(
                    row: renderRowFor(layout, entry: entry, cellRect: cell.rect, size: size),
                    col: renderColFor(layout, entry: entry, cellRect: cell.rect, size: size),
                    width: size.width,
                    height: size.height
                )
                let rendered = Uimd.renderPlainText(text, width: size.width, height: size.height, style: entry.elementStyle)
                blit(buffer: &buffer, rendered: rendered, row: rect.row, col: rect.col)
                continue
            }
            let elementEditMode = focusedName == entry.name &&
                editMode &&
                (activeScrollView == nil || activeScrollViewEditElement === element)
            if elementEditMode && element is ComboBox && !(element is ListBox)
            {
                deferredComboBox = (entry: entry, element: element, cellRect: cell.rect)
                continue
            }
            drawEntry(entry, element: element, cellRect: cell.rect, elementEditMode: elementEditMode)
        }
        if let deferredComboBox
        {
            drawEntry(
                deferredComboBox.entry,
                element: deferredComboBox.element,
                cellRect: deferredComboBox.cellRect,
                elementEditMode: true
            )
        }
        if let activeContainerScope
        {
            if !suppressActiveContainerScopeVisuals
            {
                if activeContainerScope.focus,
                   let child = generatedWindowChild(of: activeContainerScope.element),
                   let scrollView = child.generatedScrollView()
                {
                    if generatedWindowChild(of: activeContainerScope.element)?.suppressActiveContainerScopeVisuals != true
                    {
                        let activeVisualRect = intersect(
                            activeContainerScope.rect,
                            activeContainerScope.clip
                        )
                        applyScrollableHostFocusBackground(
                            buffer: &buffer,
                            active: activeVisualRect,
                            style: scrollView.style,
                            focusBackground: generatedChildFocusBackground(
                                for: activeContainerScope.element,
                                child: child,
                                directFocus: focusedName == activeContainerScope.element.name,
                                descendantOnlyFocus: focusedName != activeContainerScope.element.name
                            ),
                            baseBackground: activeContainerScope.element.style.background
                        )
                    }
                }
                else if activeContainerScope.focus
                {
                    let activeVisualRect = intersect(activeContainerScope.rect, activeContainerScope.clip)
                    applyActiveElementFocusBackground(
                        buffer: &buffer,
                        active: activeVisualRect,
                        style: activeContainerScope.element.style,
                        focusBackground: activeContainerScope.element.focusStyle?.background
                    )
                }
                if activeContainerScope.dim && !suppressActiveContainerDim && activeScrollView == nil
                {
                    let activeVisualRect = activeContainerScope.focus
                        ? intersect(activeContainerScope.rect, activeContainerScope.clip)
                        : activeContainerScope.rect
                    dimOutsideActiveElement(
                        buffer: &buffer,
                        active: activeVisualRect,
                        dim: activeContainerScope.element.style.scopeDimBackground
                    )
                }
            }
        }
        if editMode,
           let activeScrollView,
           activeScrollViewRepresentedInCurrentLayout(self, activeScrollView),
           !suppressActiveContainerScopeVisuals,
           !suppressActiveContainerDim
        {
            let dim = generatedScrollViewScopeDimBackground(self, activeScrollView) ?? activeScrollView.style.scopeDimBackground
            dimOutsideActiveElement(
                buffer: &buffer,
                active: activeScrollView.frame,
                dim: dim
            )
        }
        if hostViewportRootScrollIndicatorsDepth > 0
        {
            applyHostViewportRootScrollIndicators(
                content: &buffer,
                window: self,
                size: Size(width: width, height: height)
            )
        }
        return buffer
    }

    fileprivate func renderContentForHost(
        size: Size,
        focusedName: String?,
        editMode: Bool,
        activeScrollView: ScrollView? = nil,
        activeScrollViewEditElement: UIElement? = nil,
        activeScrollViewFocusBackground: Color? = nil,
        clipTop: Int? = nil,
        clipBottom: Int? = nil,
        useHostViewportForRootScrollViewIndicators: Bool = false
    ) -> [[TerminalCell]]
    {
        forcedFullscreenRenderDepth += 1
        if useHostViewportForRootScrollViewIndicators
        {
            hostViewportRootScrollIndicatorsDepth += 1
        }
        defer
        {
            if useHostViewportForRootScrollViewIndicators
            {
                hostViewportRootScrollIndicatorsDepth -= 1
            }
            forcedFullscreenRenderDepth -= 1
        }
        return renderGeneratedContent(
            size: size,
            focusedName: focusedName,
            editMode: editMode,
            activeScrollView: activeScrollView,
            activeScrollViewEditElement: activeScrollViewEditElement,
            activeScrollViewFocusBackground: activeScrollViewFocusBackground,
            clipTop: clipTop,
            clipBottom: clipBottom
        )
    }
}

public func syncGeneratedReusableChildFrames(_ reusable: ReusableElement, to frame: Rect)
{
    guard let child = reusable.child else
    {
        return
    }
    syncGeneratedWindowElementFrames(child, to: frame, forceFullscreenLayout: true)
}

public func syncGeneratedWindowElementFrames(
    _ window: GeneratedWindowBase,
    to frame: Rect,
    forceFullscreenLayout: Bool = true
)
{
    let width = max(1, frame.width)
    let height = max(1, frame.height)
    var runtimeCells = collectRuntimeCells(window)
    var resolvedCells: [ResolvedRuntimeCell] = []
    resolveRuntimeCellsWithFitPass(
        window,
        runtimeCells: &runtimeCells,
        content: Rect(row: 0, col: 0, width: width, height: height),
        mode: forceFullscreenLayout ? "fullscreen" : generatedRuntimeMode(window),
        resolvedCells: &resolvedCells
    )

    for entry in window.layout
    {
        guard !entry.name.isEmpty,
              let element = window.element(named: entry.name),
              let cell = resolvedCellFor(resolvedCells, entry: entry)
        else
        {
            continue
        }
        let size = renderSizeFor(entry, cellRect: cell.rect, element: element)
        element.frame = Rect(
            row: frame.row + renderRowFor(window.layout, entry: entry, cellRect: cell.rect, size: size),
            col: frame.col + renderColFor(window.layout, entry: entry, cellRect: cell.rect, size: size),
            width: size.width,
            height: size.height
        )

        if let reusable = element as? ReusableElement
        {
            syncGeneratedReusableChildFrames(reusable, to: element.frame)
            continue
        }
        guard let scrollView = element as? ScrollView else
        {
            continue
        }
        let scrollFrame = scrollView.frame
        for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
        {
            guard let child = childView.element else
            {
                continue
            }
            if !childView.visible
            {
                child.frame = Rect(row: 0, col: 0, width: 0, height: 0)
                continue
            }
            child.frame = Rect(
                row: scrollFrame.row + childView.frame.row,
                col: scrollFrame.col + childView.frame.col,
                width: childView.frame.width,
                height: childView.frame.height
            )
            if let reusable = child as? ReusableElement
            {
                syncGeneratedReusableChildFrames(reusable, to: child.frame)
            }
        }
    }
}

@discardableResult
private func registerGeneratedHostChildElements(
    on owner: GeneratedWindowBase,
    host: UIElement,
    hostId: String,
    hostFrame: Rect
) -> [String]
{
    guard let child = generatedWindowChild(of: host) else
    {
        host.childFocusOrder.removeAll()
        return []
    }
    syncGeneratedWindowElementFrames(child, to: hostFrame, forceFullscreenLayout: true)
    let registered: [String]
    if let scrollView = child.generatedScrollView()
    {
        registered = registerGeneratedScrollViewChildren(
            on: owner,
            scrollView: scrollView,
            idPrefix: hostId,
            parentFocusHostId: hostId
        )
    }
    else
    {
        registered = registerGeneratedWindowElements(
            on: owner,
            window: child,
            prefix: hostId,
            parentFocusHostId: hostId
        )
    }
    host.childFocusOrder = registered
    return registered
}

@discardableResult
private func registerGeneratedScrollChildElements(
    on owner: GeneratedWindowBase,
    scrollEntryName: String,
    scrollFrame: Rect,
    childView: ScrollViewChildView,
    child: UIElement
) -> [String]
{
    let childFrame = Rect(
        row: scrollFrame.row + childView.frame.row,
        col: scrollFrame.col + childView.frame.col,
        width: childView.frame.width,
        height: childView.frame.height
    )
    child.frame = childFrame
    if let reusable = child as? ReusableElement
    {
        syncGeneratedReusableChildFrames(reusable, to: childFrame)
    }
    let registered = registerGeneratedIndexedScrollChildTree(
        on: owner,
        child: child,
        indexedPrefix: "\(scrollEntryName)[\(childView.index)]",
        parentFocusHostId: scrollEntryName
    )
    if let scrollView = owner.element(named: scrollEntryName) as? ScrollView
    {
        appendUniqueFocusIds(registered, to: &scrollView.childFocusOrder)
    }
    return registered
}

private func registerGeneratedScrollViewChildren(
    on owner: GeneratedWindowBase,
    scrollView: ScrollView,
    idPrefix: String,
    parentFocusHostId: String
) -> [String]
{
    scrollView.childFocusOrder.removeAll()
    let scrollFrame = scrollView.frame
    var registered: [String] = []
    for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
    {
        guard let child = childView.element else
        {
            continue
        }
        let childFrame = Rect(
            row: scrollFrame.row + childView.frame.row,
            col: scrollFrame.col + childView.frame.col,
            width: childView.frame.width,
            height: childView.frame.height
        )
        child.frame = childFrame
        if let reusable = child as? ReusableElement
        {
            syncGeneratedReusableChildFrames(reusable, to: childFrame)
        }
        let childIds = registerGeneratedIndexedScrollChildTree(
            on: owner,
            child: child,
            indexedPrefix: "\(idPrefix)[\(childView.index)]",
            parentFocusHostId: parentFocusHostId
        )
        appendUniqueFocusIds(childIds, to: &registered)
    }
    scrollView.childFocusOrder = registered
    return registered
}

private func registerGeneratedWindowElements(
    on owner: GeneratedWindowBase,
    window: GeneratedWindowBase,
    prefix: String,
    parentFocusHostId: String
) -> [String]
{
    var registered: [String] = []
    for element in window.elements
    {
        let elementIds = registerGeneratedElementTree(
            on: owner,
            element: element,
            prefix: prefix,
            parentFocusHostId: parentFocusHostId
        )
        appendUniqueFocusIds(elementIds, to: &registered)
    }
    return registered
}

private func registerGeneratedIndexedScrollChildTree(
    on owner: GeneratedWindowBase,
    child: UIElement,
    indexedPrefix: String,
    parentFocusHostId: String
) -> [String]
{
    if let reusable = child as? ReusableElement,
       let generated = reusable.child
    {
        syncGeneratedReusableChildFrames(reusable, to: child.frame)
        let childIds: [String]
        if let scrollView = generated.generatedScrollView()
        {
            childIds = registerGeneratedScrollViewChildren(
                on: owner,
                scrollView: scrollView,
                idPrefix: indexedPrefix,
                parentFocusHostId: parentFocusHostId
            )
        }
        else
        {
            childIds = registerGeneratedWindowElements(
                on: owner,
                window: generated,
                prefix: indexedPrefix,
                parentFocusHostId: parentFocusHostId
            )
        }
        child.childFocusOrder = childIds
        return childIds
    }
    if let viewHost = child as? ViewHost,
       let generated = viewHost.child
    {
        syncGeneratedWindowElementFrames(generated, to: child.frame, forceFullscreenLayout: true)
        let childIds: [String]
        if let scrollView = generated.generatedScrollView()
        {
            childIds = registerGeneratedScrollViewChildren(
                on: owner,
                scrollView: scrollView,
                idPrefix: indexedPrefix,
                parentFocusHostId: parentFocusHostId
            )
        }
        else
        {
            childIds = registerGeneratedWindowElements(
                on: owner,
                window: generated,
                prefix: indexedPrefix,
                parentFocusHostId: parentFocusHostId
            )
        }
        child.childFocusOrder = childIds
        return childIds
    }
    return registerGeneratedElementTree(
        on: owner,
        element: child,
        prefix: indexedPrefix,
        parentFocusHostId: parentFocusHostId
    )
}

private func registerGeneratedElementTree(
    on owner: GeneratedWindowBase,
    element: UIElement,
    prefix: String,
    parentFocusHostId: String
) -> [String]
{
    let id = "\(prefix).\(element.name)"
    if !element.name.isEmpty
    {
        owner.registerDynamicElement(id, element: element, parentFocusHostId: parentFocusHostId)
    }

    if let reusable = element as? ReusableElement,
       let child = reusable.child
    {
        syncGeneratedReusableChildFrames(reusable, to: element.frame)
        let childIds: [String]
        if let scrollView = child.generatedScrollView()
        {
            childIds = registerGeneratedScrollViewChildren(
                on: owner,
                scrollView: scrollView,
                idPrefix: id,
                parentFocusHostId: id
            )
        }
        else
        {
            childIds = registerGeneratedWindowElements(
                on: owner,
                window: child,
                prefix: id,
                parentFocusHostId: id
            )
        }
        if let scrollView = child.generatedScrollView()
        {
            _ = scrollView
            element.childFocusOrder = childIds
            return generatedElementIsFocusable(element) ? [id] : []
        }
        if !childIds.isEmpty
        {
            element.childFocusOrder = childIds
            return childIds
        }
        if generatedElementIsFocusable(element), child.generatedFocusable, !(element is ViewHost)
        {
            element.childFocusOrder.removeAll()
            return [id]
        }
    }
    if let viewHost = element as? ViewHost,
       let child = viewHost.child
    {
        syncGeneratedWindowElementFrames(child, to: element.frame, forceFullscreenLayout: true)
        let childIds = registerGeneratedWindowElements(
            on: owner,
            window: child,
            prefix: id,
            parentFocusHostId: id
        )
        if !childIds.isEmpty
        {
            element.childFocusOrder = childIds
            return childIds
        }
    }
    if let scrollView = element as? ScrollView
    {
        var ids: [String] = []
        if generatedElementIsFocusable(element)
        {
            ids.append(id)
        }
        let childIds = registerGeneratedScrollViewChildren(
            on: owner,
            scrollView: scrollView,
            idPrefix: "\(prefix).\(element.name)",
            parentFocusHostId: parentFocusHostId
        )
        appendUniqueFocusIds(childIds, to: &ids)
        return ids
    }

    var registered: [String] = []
    if generatedElementIsFocusable(element)
    {
        registered.append(id)
    }
    for child in element.children
    {
        let childIds = registerGeneratedElementTree(
            on: owner,
            element: child,
            prefix: "\(prefix).\(element.name)",
            parentFocusHostId: parentFocusHostId
        )
        appendUniqueFocusIds(childIds, to: &registered)
    }
    return registered
}

private func appendUniqueFocusIds(_ ids: [String], to target: inout [String])
{
    for id in ids where !target.contains(id)
    {
        target.append(id)
    }
}

private func generatedElementIsFocusable(_ element: UIElement) -> Bool
{
    guard element.enabled && element.focusable else
    {
        return false
    }
    if generatedElementIsFocusableType(element)
    {
        return true
    }
    if let reusable = element as? ReusableElement,
       let child = reusable.child
    {
        if child.generatedScrollView() != nil
        {
            return true
        }
        return element.childFocusOrder.isEmpty && child.generatedFocusable && !(element is ViewHost)
    }
    if let viewHost = element as? ViewHost,
       let child = viewHost.child
    {
        return child.generatedScrollView() != nil
    }
    return false
}

private func generatedElementIsFocusableType(_ element: UIElement) -> Bool
{
    element is Button ||
        element is CheckBox ||
        element is TextInput ||
        element is NumberInput ||
        element is ComboBox ||
        element is ListBox ||
        element is ScrollView
}

private func appendGeneratedFocusableElements(
    from targetWindow: GeneratedWindowBase,
    activeScrollView: ScrollView?,
    to focusable: inout [UIElement]
)
{
    for element in targetWindow.elements
    {
        if let reusable = element as? ReusableElement,
           let child = reusable.child
        {
            syncGeneratedReusableChildFrames(reusable, to: reusable.frame)
            if let generatedScrollView = child.generatedScrollView()
            {
                if reusable.enabled && reusable.focusable
                {
                    focusable.append(reusable)
                }
                if activeScrollView === generatedScrollView
                {
                    var childFocusable: [UIElement] = []
                    appendGeneratedFocusableElements(
                        from: child,
                        activeScrollView: activeScrollView,
                        to: &childFocusable
                    )
                    focusable.append(contentsOf: childFocusable.filter { $0 !== generatedScrollView })
                }
                continue
            }

            var childFocusable: [UIElement] = []
            appendGeneratedFocusableElements(
                from: child,
                activeScrollView: activeScrollView,
                to: &childFocusable
            )
            if childFocusable.isEmpty &&
                reusable.enabled &&
                reusable.focusable &&
                child.generatedFocusable
            {
                focusable.append(reusable)
            }
            else
            {
                focusable.append(contentsOf: childFocusable)
            }
            continue
        }

        if let viewHost = element as? ViewHost,
           let child = viewHost.child
        {
            syncGeneratedWindowElementFrames(child, to: viewHost.frame, forceFullscreenLayout: true)
            if let generatedScrollView = child.generatedScrollView()
            {
                if viewHost.enabled && viewHost.focusable
                {
                    focusable.append(viewHost)
                }
                if activeScrollView === generatedScrollView
                {
                    var childFocusable: [UIElement] = []
                    appendGeneratedFocusableElements(
                        from: child,
                        activeScrollView: activeScrollView,
                        to: &childFocusable
                    )
                    focusable.append(contentsOf: childFocusable.filter { $0 !== generatedScrollView })
                }
                continue
            }

            appendGeneratedFocusableElements(
                from: child,
                activeScrollView: activeScrollView,
                to: &focusable
            )
            continue
        }

        if let scrollView = element as? ScrollView
        {
            if generatedElementIsFocusable(element)
            {
                focusable.append(element)
            }
            if activeScrollView === scrollView
            {
                let scrollFrame = scrollView.frame
                let size = Size(width: scrollFrame.width, height: scrollFrame.height)
                for childView in scrollView.childViews(size: size)
                {
                    guard let child = childView.element else
                    {
                        continue
                    }
                    child.frame = Rect(
                        row: scrollFrame.row + childView.frame.row,
                        col: scrollFrame.col + childView.frame.col,
                        width: childView.frame.width,
                        height: childView.frame.height
                    )
                    if let reusable = child as? ReusableElement,
                       let generated = reusable.child
                    {
                        syncGeneratedReusableChildFrames(reusable, to: child.frame)
                        appendGeneratedFocusableElements(
                            from: generated,
                            activeScrollView: activeScrollView,
                            to: &focusable
                        )
                        continue
                    }
                    if generatedElementIsFocusable(child)
                    {
                        focusable.append(child)
                    }
                }
            }
            continue
        }

        if generatedElementIsFocusable(element)
        {
            focusable.append(element)
        }
    }
}

private func generatedFocusableElements(
    in window: GeneratedWindowBase,
    activeScrollView: ScrollView? = nil
) -> [UIElement]
{
    var focusable: [UIElement] = []
    appendGeneratedFocusableElements(
        from: window,
        activeScrollView: activeScrollView,
        to: &focusable
    )
    return focusable
}

private func generatedHasMultipleFocusableElements(
    _ window: GeneratedWindowBase,
    activeScrollView: ScrollView? = nil
) -> Bool
{
    generatedFocusableElements(in: window, activeScrollView: activeScrollView).count > 1
}

open class GeneratedScrollViewBase: GeneratedWindowBase, GeneratedScrollableKeyHandling
{
    private var generatedScrollViewElement: ScrollView?

    open override func generatedScrollView() -> ScrollView?
    {
        generatedScrollViewElement
    }

    public func setGeneratedScrollView(_ scrollView: ScrollView)
    {
        generatedScrollViewElement = scrollView
    }

    public func scrollView() -> ScrollView
    {
        guard let generatedScrollViewElement else
        {
            preconditionFailure("generated scroll view has not been initialized")
        }
        return generatedScrollViewElement
    }

    open func setAutoScroll(_ enabled: Bool)
    {
        scrollView().setAutoScroll(enabled)
    }

    public func clearChildren()
    {
        scrollView().clearChildren()
    }

    public func addRenderedChild(_ child: [[TerminalCell]])
    {
        scrollView().addChild(child)
    }

    public func setDynamicChildrenRenderer(_ renderer: @escaping (Int) -> [[[TerminalCell]]])
    {
        scrollView().setDynamicChildrenRenderer(renderer)
    }

    public func invalidateDynamicChildren()
    {
        scrollView().invalidateDynamicChildren()
    }

    public func viewOffset() -> Int
    {
        scrollView().viewOffsetValue()
    }

    public func scrollOffset() -> Int
    {
        scrollView().scrollOffsetValue()
    }

    open func contentHeight() -> Int
    {
        scrollView().contentHeight(width: max(1, scrollView().frame.width))
    }

    open func contentHeight(width: Int) -> Int
    {
        scrollView().contentHeight(width: width)
    }

    open func scrollToTop()
    {
        _ = scrollView().scrollToTop(Size(width: max(1, scrollView().frame.width), height: max(1, scrollView().frame.height)))
    }

    open func scrollToBottom()
    {
        _ = scrollView().scrollToBottom(Size(width: max(1, scrollView().frame.width), height: max(1, scrollView().frame.height)))
    }

    open func scrollBy(_ delta: Int, viewport: Size)
    {
        _ = scrollView().scrollBy(delta, viewport: viewport)
    }

    @discardableResult
    open func handleScrollKey(_ key: String, viewport: Size) -> Bool
    {
        scrollView().handleKey(key, viewport: viewport)
    }

    @discardableResult
    open func handleKey(_ key: String) -> Bool
    {
        scrollView().handleKey(key, viewport: Size(width: max(1, scrollView().frame.width), height: max(1, scrollView().frame.height)))
    }
}

private func simpleGeneratedLayoutEntry(
    name: String,
    type: String,
    row: Int,
    col: Int,
    width: Int,
    height: Int,
    totalWidth: Int,
    totalHeight: Int,
    elementStyle: Style = Style()
) -> GeneratedLayoutEntry
{
    GeneratedLayoutEntry(
        name: name,
        type: type,
        cellName: "",
        relative: Rect(row: row, col: col, width: width, height: height),
        sourceCell: Rect(row: 0, col: 0, width: totalWidth, height: totalHeight),
        width: .auto(width),
        height: .auto(height),
        cellWidth: .auto(totalWidth),
        cellHeight: .auto(totalHeight),
        charsSize: Size(width: width, height: height),
        cellCharsSize: Size(width: totalWidth, height: totalHeight),
        marginRight: max(0, totalWidth - col - width),
        marginBottom: max(0, totalHeight - row - height),
        cellStyle: Style(),
        elementStyle: elementStyle
    )
}

private func standardDialogWindowStyle() -> Style
{
    var style = Style()
    style.background = Color("#162033")
    style.borderColor = Color("transparent")
    style.borderWidthHorizontal = 0
    style.borderWidthVertical = 0
    return style
}

private func standardDialogLabelStyle() -> Style
{
    var style = Style()
    style.background = Color("transparent")
    style.color = Color("#cbd5e1")
    return style
}

private func standardDialogHeaderStyle() -> Style
{
    var style = standardDialogLabelStyle()
    style.background = Color("#1d314c")
    style.textAlign = "center"
    return style
}

private func standardDialogButtonStyle() -> Style
{
    var style = Style()
    style.background = Color("#334155")
    style.color = Color("#e2e8f0")
    return style
}

private func standardDialogButtonFocusStyle() -> Style
{
    var style = Style()
    style.background = Color("#2b5f9f")
    style.color = Color("#ffffff")
    return style
}

private func standardDialogInputDisabledStyle() -> Style
{
    var style = Style()
    style.background = Color("#0b1220")
    style.color = Color("#64748b")
    return style
}

private func standardDialogButtonDisabledStyle() -> Style
{
    var style = Style()
    style.background = Color("#1f2937")
    style.color = Color("#64748b")
    return style
}

public final class MessageBoxYesNo: GeneratedWindowBase
{
    private static let dialogWidth = 44
    private static let dialogHeight = 7
    public let dialog_header: Label
    public let message: Label
    public let yes_btn: Button
    public let no_btn: Button
    public var onClose: ((Bool) -> Void)?
    private var closed = false

    public init(_ header: String = "Message", _ text: String = "")
    {
        dialog_header = Label("dialog_header", header)
        message = Label("message", text)
        yes_btn = Button("yes_btn", "Yes")
        no_btn = Button("no_btn", "No")
        super.init("Message Box")
        mode = "dialog"
        setGeneratedKind("dialog")
        setGeneratedWindowStyle(standardDialogWindowStyle())
        setMcpMetadata(enabled: true, className: "MessageBoxYesNo", source: "", description: "Yes/No message box.")
        setGeneratedLayout([
            simpleGeneratedLayoutEntry(name: "dialog_header", type: "label", row: 0, col: 0, width: Self.dialogWidth, height: 1, totalWidth: Self.dialogWidth, totalHeight: Self.dialogHeight, elementStyle: standardDialogHeaderStyle()),
            simpleGeneratedLayoutEntry(name: "message", type: "label", row: 2, col: 2, width: Self.dialogWidth - 4, height: 2, totalWidth: Self.dialogWidth, totalHeight: Self.dialogHeight, elementStyle: standardDialogLabelStyle()),
            simpleGeneratedLayoutEntry(name: "yes_btn", type: "button", row: 5, col: 3, width: 18, height: 1, totalWidth: Self.dialogWidth, totalHeight: Self.dialogHeight, elementStyle: standardDialogButtonStyle()),
            simpleGeneratedLayoutEntry(name: "no_btn", type: "button", row: 5, col: 23, width: 18, height: 1, totalWidth: Self.dialogWidth, totalHeight: Self.dialogHeight, elementStyle: standardDialogButtonStyle()),
        ])
        addElement(dialog_header)
        addElement(message)
        addElement(yes_btn)
        addElement(no_btn)
        yes_btn.setFocusStyle(standardDialogButtonFocusStyle())
        no_btn.setFocusStyle(standardDialogButtonFocusStyle())
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "no_btn"
        options.onKey = { [weak self] key in
            guard key == "Escape" else
            {
                return false
            }
            self?.close(false)
            return true
        }
        return options
    }

    public override func handleGeneratedKey(_ key: String) -> Bool
    {
        guard key == "Escape" else
        {
            return false
        }
        close(false)
        return true
    }

    public override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "yes_btn"
        {
            close(true)
            return true
        }
        if name == "no_btn"
        {
            close(false)
            return true
        }
        return false
    }

    public override func shouldClose() -> Bool
    {
        closed
    }

    private func close(_ confirmed: Bool)
    {
        closed = true
        onClose?(confirmed)
    }

    public func cancel()
    {
        close(false)
    }
}

public final class FileBrowser: GeneratedWindowBase
{
    private static let dialogWidth = 48
    private static let dialogBodyHeight = 14
    public let dialog_header: Label
    public let path_label: Label
    public let entries: ListBox
    public let filename: TextInput
    public let open_btn: Button
    public let close_btn = Button("close_btn", "Close")
    public var onClose: ((String) -> Void)?
    public var currentDirectory: String
    {
        currentDir.path
    }
    public private(set) var result = ""
    public private(set) var closed = false
    private let rootDir: URL
    private var currentDir: URL
    private let browserMode: String
    private var initialFilename: String
    private let extensionFilter: String

    public init(
        root: String = ".",
        start: String = ".",
        mode: String = "open",
        onClose: ((String) -> Void)? = nil,
        initialFilename: String = "",
        extensionFilter: String = ""
    )
    {
        let rootUrl = FileBrowser.canonicalUrl(root.isEmpty ? "." : root)
        let requested = start.isEmpty ? rootUrl : FileBrowser.canonicalUrl(start)
        var isDirectory: ObjCBool = false
        var current = requested
        var requestedInitialFilename = initialFilename
        if FileManager.default.fileExists(atPath: requested.path, isDirectory: &isDirectory)
        {
            if !isDirectory.boolValue
            {
                current = requested.deletingLastPathComponent()
                if requestedInitialFilename.isEmpty
                {
                    requestedInitialFilename = requested.lastPathComponent
                }
            }
        }
        current = FileBrowser.clampDir(current, root: rootUrl)
        rootDir = rootUrl
        currentDir = current
        browserMode = mode
        self.initialFilename = requestedInitialFilename
        self.extensionFilter = extensionFilter.trimmingCharacters(in: .whitespacesAndNewlines)
        dialog_header = Label("dialog_header", mode == "save" ? "Save As" : "Open File")
        path_label = Label("path_label", current.path)
        entries = ListBox("entries", [], selectedValues: [])
        filename = TextInput("filename", mode == "save" ? requestedInitialFilename : "", maxLength: 120)
        open_btn = Button("open_btn", mode == "save" ? "Save" : "Open")
        self.onClose = onClose
        super.init("File Browser")
        self.mode = "dialog"
        setGeneratedKind("dialog")
        setGeneratedWindowStyle(standardDialogWindowStyle())
        setMcpMetadata(enabled: true, className: "FileBrowser", source: "", description: "File browser dialog.")
        setGeneratedLayout(Self.buildLayout())
        addElement(dialog_header)
        addElement(path_label)
        addElement(entries)
        addElement(filename)
        addElement(open_btn)
        addElement(close_btn)
        entries.commitMode = kCommitModeLeave
        entries.setStyle(Self.entriesStyle())
        entries.setFocusStyle(Self.entriesFocusStyle())
        entries.setEditStyle(Self.entriesEditStyle())
        entries.setSelectedStyle(Self.entriesSelectedStyle())
        entries.setActiveStyle(Self.entriesActiveStyle())
        entries.setDisabledStyle(Self.entriesDisabledStyle())
        filename.setStyle(Self.filenameStyle())
        filename.setFocusStyle(Self.filenameFocusStyle())
        filename.setEditStyle(Self.filenameEditStyle())
        filename.setCursorStyle(Self.filenameCursorStyle())
        filename.setSelectedStyle(Self.entriesSelectedStyle())
        filename.setDisabledStyle(standardDialogInputDisabledStyle())
        open_btn.setFocusStyle(standardDialogButtonFocusStyle())
        open_btn.setDisabledStyle(standardDialogButtonDisabledStyle())
        close_btn.setFocusStyle(standardDialogButtonFocusStyle())
        close_btn.setDisabledStyle(standardDialogButtonDisabledStyle())
        refreshEntries()
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "entries"
        options.startInEditMode = true
        options.onKeyBeforeFocusedElement = { [weak self] key, name, editMode in
            guard let self, key == "Enter", name == "entries", editMode else
            {
                return false
            }
            self.entries.setSelectedIndex(self.entries.activeIndex)
            self.entries.hideActiveItem()
            self.previewSelected()
            if self.selectedEntryIsDirectory()
            {
                _ = self.acceptCurrent()
                return true
            }
            return false
        }
        options.onMousePressBeforeFocused = { [weak self] point in
            self?.handleEntryMousePress(point) ?? false
        }
        options.onKey = { [weak self] key in
            guard let self else
            {
                return false
            }
            if key == "Escape"
            {
                self.close("")
                return true
            }
            return false
        }
        return options
    }

    private func handleEntryMousePress(_ point: Point) -> Bool
    {
        let frame = entries.frame
        guard point.row >= frame.row,
              point.row < frame.row + frame.height,
              point.col >= frame.col,
              point.col < frame.col + frame.width else
        {
            return false
        }
        let index = entries.scrollOffsetValue() + point.row - frame.row
        guard index >= 0, index < entries.options.count else
        {
            return false
        }
        entries.setSelectedIndex(index)
        previewSelected()
        if selectedEntryIsDirectory()
        {
            return acceptCurrent()
        }
        return false
    }

    public override func handleGeneratedKey(_ key: String) -> Bool
    {
        guard key == "Escape" else
        {
            return false
        }
        close("")
        return true
    }

    public override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "open_btn"
        {
            return acceptCurrent()
        }
        if name == "close_btn"
        {
            close("")
            return true
        }
        return false
    }

    public override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "entries"
        {
            previewSelected()
            return true
        }
        return false
    }

    public override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "filename"
        {
            updateOpenEnabled()
            return true
        }
        return false
    }

    public override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "entries"
        {
            previewSelected()
            return true
        }
        if name == "filename", browserMode == "save"
        {
            return acceptFilename()
        }
        return false
    }

    public override func shouldClose() -> Bool
    {
        closed
    }

    public func refreshEntries()
    {
        var directories: [String] = []
        var files: [String] = []
        var disabledRows: [String] = []
        if let urls = try? FileManager.default.contentsOfDirectory(
            at: currentDir,
            includingPropertiesForKeys: [.isDirectoryKey, .isRegularFileKey],
            options: []
        )
        {
            for url in urls
            {
                let values = try? url.resourceValues(forKeys: [.isDirectoryKey, .isRegularFileKey])
                if values?.isDirectory == true
                {
                    directories.append(url.lastPathComponent + "/")
                }
                else if values?.isRegularFile == true
                {
                    files.append(url.lastPathComponent)
                }
            }
        }
        directories.sort()
        files.sort()
        for file in files
        {
            if !pathMatchesFilter(currentDir.appendingPathComponent(file))
            {
                disabledRows.append(file)
            }
        }
        let rows = [".."] + directories + files
        entries.options = rows
        entries.setDisabledValues(disabledRows)
        let selectedIndex = initialFilename.isEmpty ? 0 : (rows.firstIndex(of: initialFilename) ?? 0)
        entries.setSelectedIndex(rows.isEmpty ? 0 : selectedIndex)
        path_label.text = currentDir.path
        previewSelected()
        updateOpenEnabled()
    }

    @discardableResult
    public func acceptCurrent() -> Bool
    {
        let path = selectedPath()
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: path.path, isDirectory: &isDirectory), isDirectory.boolValue
        {
            currentDir = FileBrowser.clampDir(path, root: rootDir)
            refreshEntries()
            return true
        }
        let selectedName = path.lastPathComponent
        let requestedName = filename.value.trimmingCharacters(in: .whitespacesAndNewlines)
        let name = requestedName.isEmpty ? selectedName : requestedName
        guard !name.isEmpty else
        {
            return false
        }
        let namedPath = currentDir.appendingPathComponent(name)
        if browserMode == "open"
        {
            var namedIsDirectory: ObjCBool = false
            if FileManager.default.fileExists(atPath: namedPath.path, isDirectory: &namedIsDirectory),
               !namedIsDirectory.boolValue,
               FileBrowser.clampDir(namedPath.deletingLastPathComponent(), root: rootDir).path == currentDir.path,
               pathMatchesFilter(namedPath)
            {
                close(namedPath.path)
                return true
            }
            return false
        }
        close(namedPath.path)
        return true
    }

    @discardableResult
    private func acceptFilename() -> Bool
    {
        let name = filename.value.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !name.isEmpty else
        {
            return false
        }
        let path = currentDir.appendingPathComponent(name)
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: path.path, isDirectory: &isDirectory), isDirectory.boolValue
        {
            return false
        }
        if FileBrowser.clampDir(path.deletingLastPathComponent(), root: rootDir).path != currentDir.path
        {
            return false
        }
        close(path.path)
        return true
    }

    public func close(_ path: String)
    {
        result = path
        closed = true
        onClose?(path)
    }

    public func cancel()
    {
        close("")
    }

    private func selectedPath() -> URL
    {
        guard entries.selectedIndex >= 0 && entries.selectedIndex < entries.options.count else
        {
            return currentDir
        }
        let selected = entries.options[entries.selectedIndex]
        if selected == ".."
        {
            return FileBrowser.clampDir(currentDir.deletingLastPathComponent(), root: rootDir)
        }
        return currentDir.appendingPathComponent(selected)
    }

    private func selectedPathForOpenState() -> URL?
    {
        guard entries.selectedIndex >= 0 && entries.selectedIndex < entries.options.count else
        {
            return nil
        }
        let selected = entries.options[entries.selectedIndex]
        if selected == ".."
        {
            return FileBrowser.clampDir(currentDir.deletingLastPathComponent(), root: rootDir)
        }
        return currentDir.appendingPathComponent(selected)
    }

    private func selectedEntryIsDirectory() -> Bool
    {
        guard entries.selectedIndex >= 0 && entries.selectedIndex < entries.options.count else
        {
            return false
        }
        let selected = entries.options[entries.selectedIndex]
        if selected == ".." || selected.hasSuffix("/")
        {
            return true
        }
        var isDirectory: ObjCBool = false
        return FileManager.default.fileExists(atPath: currentDir.appendingPathComponent(selected).path, isDirectory: &isDirectory)
            && isDirectory.boolValue
    }

    private func updateOpenEnabled()
    {
        if browserMode == "save"
        {
            open_btn.enabled = !filename.value.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty
            return
        }
        guard let path = selectedPathForOpenState() else
        {
            open_btn.enabled = false
            return
        }
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: path.path, isDirectory: &isDirectory), isDirectory.boolValue
        {
            open_btn.enabled = true
            return
        }
        open_btn.enabled = FileManager.default.fileExists(atPath: path.path)
            && FileBrowser.clampDir(path.deletingLastPathComponent(), root: rootDir).path == currentDir.path
            && pathMatchesFilter(path)
    }

    private func previewSelected()
    {
        guard entries.selectedIndex >= 0 && entries.selectedIndex < entries.options.count else
        {
            return
        }
        let selected = entries.options[entries.selectedIndex]
        if selected != ".."
        {
            filename.setValue(selected.hasSuffix("/") ? String(selected.dropLast()) : selected)
        }
        else if browserMode == "open"
        {
            filename.setValue("")
        }
    }

    private func pathMatchesFilter(_ path: URL) -> Bool
    {
        guard !extensionFilter.isEmpty else
        {
            return true
        }
        var ext = path.pathExtension
        guard !ext.isEmpty else
        {
            return false
        }
        if ext.hasPrefix(".")
        {
            ext.removeFirst()
        }
        do
        {
            let regex = try NSRegularExpression(pattern: "^(?:" + extensionFilter + ")$", options: [.caseInsensitive])
            let range = NSRange(ext.startIndex..<ext.endIndex, in: ext)
            return regex.firstMatch(in: ext, range: range) != nil
        }
        catch
        {
            let lowered = ext.lowercased()
            for rawPart in extensionFilter.split(separator: "|")
            {
                var part = rawPart.trimmingCharacters(in: .whitespacesAndNewlines)
                if part.hasPrefix(".")
                {
                    part.removeFirst()
                }
                if lowered == part.lowercased()
                {
                    return true
                }
            }
            return false
        }
    }

    private static func canonicalUrl(_ path: String) -> URL
    {
        let url: URL
        if path.hasPrefix("/")
        {
            url = URL(fileURLWithPath: path)
        }
        else
        {
            url = URL(fileURLWithPath: FileManager.default.currentDirectoryPath).appendingPathComponent(path)
        }
        return url.standardizedFileURL.resolvingSymlinksInPath()
    }

    private static func clampDir(_ url: URL, root: URL) -> URL
    {
        let candidate = url.standardizedFileURL.resolvingSymlinksInPath()
        let rootPath = root.path
        let candidatePath = candidate.path
        if candidatePath == rootPath || candidatePath.hasPrefix(rootPath + "/")
        {
            return candidate
        }
        return root
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        let bodyCell = Rect(row: 2, col: 0, width: dialogWidth, height: dialogBodyHeight)
        return [
            GeneratedLayoutEntry(name: "dialog_header", type: "label", cellName: "", relative: Rect(row: 0, col: 0, width: 536870911, height: 1), sourceCell: Rect(row: 0, col: 0, width: dialogWidth, height: 1), width: .expanded, height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(1), charsSize: Size(width: dialogWidth, height: 1), cellCharsSize: Size(width: dialogWidth, height: 1), marginRight: 0, marginBottom: 0, cellStyle: Style(), elementStyle: standardDialogHeaderStyle()),
            GeneratedLayoutEntry(name: "path_label", type: "label", cellName: "", relative: Rect(row: 1, col: 3, width: 536870911, height: 1), sourceCell: bodyCell, width: .expanded, height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 42, height: 1), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 3, marginBottom: 12, cellStyle: Style(), elementStyle: standardDialogLabelStyle()),
            GeneratedLayoutEntry(name: "entries", type: "listbox", cellName: "", relative: Rect(row: 3, col: 3, width: 536870911, height: 6), sourceCell: bodyCell, width: .expanded, height: .auto(6), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 42, height: 6), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 3, marginBottom: 5, cellStyle: Style(), elementStyle: entriesStyle()),
            GeneratedLayoutEntry(name: "\"File:\"", type: "label", cellName: "", relative: Rect(row: 10, col: 3, width: 9, height: 1), sourceCell: bodyCell, width: .auto(9), height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 9, height: 1), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 36, marginBottom: 3, cellStyle: Style(), elementStyle: standardDialogLabelStyle()),
            GeneratedLayoutEntry(name: "filename", type: "textinput", cellName: "", relative: Rect(row: 10, col: 14, width: 536870911, height: 1), sourceCell: bodyCell, width: .expanded, height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 31, height: 1), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 3, marginBottom: 3, cellStyle: Style(), elementStyle: filenameStyle()),
            GeneratedLayoutEntry(name: "open_btn", type: "button", cellName: "", relative: Rect(row: 12, col: 3, width: 20, height: 1), sourceCell: bodyCell, width: .auto(20), height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 20, height: 1), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 25, marginBottom: 1, cellStyle: Style(), elementStyle: standardDialogButtonStyle()),
            GeneratedLayoutEntry(name: "close_btn", type: "button", cellName: "", relative: Rect(row: 12, col: 25, width: 536870911, height: 1), sourceCell: bodyCell, width: .expanded, height: .auto(1), cellWidth: .auto(dialogWidth), cellHeight: .auto(dialogBodyHeight), charsSize: Size(width: 20, height: 1), cellCharsSize: Size(width: dialogWidth, height: dialogBodyHeight), marginRight: 3, marginBottom: 1, cellStyle: Style(), elementStyle: standardDialogButtonStyle()),
        ]
    }

    private static func entriesStyle() -> Style
    {
        var style = Style()
        style.background = Color("#0d1524")
        style.color = Color("#cbd5e1")
        return style
    }

    private static func entriesFocusStyle() -> Style
    {
        var style = Style()
        style.background = Color("#1d2f4d")
        style.color = Color("#ffffff")
        return style
    }

    private static func entriesEditStyle() -> Style
    {
        var style = Style()
        style.background = Color("#243a5c")
        style.color = Color("#cbd5e1")
        return style
    }

    private static func entriesSelectedStyle() -> Style
    {
        var style = Style()
        style.background = Color("#2563eb")
        style.color = Color("#ffffff")
        return style
    }

    private static func entriesActiveStyle() -> Style
    {
        var style = Style()
        style.background = Color("#DDDDDD99")
        style.color = Color("#FFFFFF")
        return style
    }

    private static func entriesDisabledStyle() -> Style
    {
        var style = Style()
        style.color = Color("#64748b")
        return style
    }

    private static func filenameStyle() -> Style
    {
        var style = Style()
        style.background = Color("#0d1524")
        style.color = Color("#e5e7eb")
        return style
    }

    private static func filenameFocusStyle() -> Style
    {
        var style = Style()
        style.background = Color("#243a5c")
        style.color = Color("#ffffff")
        return style
    }

    private static func filenameEditStyle() -> Style
    {
        var style = Style()
        style.background = Color("#2d456d")
        style.color = Color("#ffffff")
        return style
    }

    private static func filenameCursorStyle() -> Style
    {
        var style = Style()
        style.background = Color("#ffffff")
        style.color = Color("#0e1117")
        return style
    }
}

public enum GeneratedWindowRuntime
{
    public static func runGeneratedAppMain(_ body: () throws -> Int) -> Int
    {
        do
        {
            return try body()
        }
        catch
        {
            FileHandle.standardError.write(Data("error: \(error)\n".utf8))
            return 1
        }
    }

    public static func runGeneratedWindow(
        _ window: GeneratedWindowBase,
        _ options: GeneratedWindowRuntimeOptions = GeneratedWindowRuntimeOptions(),
        _ args: [String] = CommandLine.arguments
    ) -> Int
    {
        var runtimeOptions = options
        if runtimeOptions.onButton == nil
        {
            runtimeOptions = window.runtimeOptions()
        }
        if args.contains("--mcp-server")
        {
            let runtime = GeneratedRuntimeController(window: window, options: runtimeOptions, args: args)
            return runtime.serve()
        }
        if args.contains("--smoke")
        {
            let output = window.renderPlainText()
            if !output.isEmpty
            {
                print(output)
            }
            return 0
        }
        let runtime = GeneratedRuntimeController(window: window, options: runtimeOptions, args: args)
        return runtime.runTerminal()
    }
}

private enum TerminalInputEvent
{
    case key(String)
    case paste(String)
    case mouse(kind: String, point: Point)
    case scroll(point: Point, delta: Int)
    case none
}

private struct TerminalModeGuard
{
    private var originalTermios = termios()
    private var hasOriginalTermios = false

    init(title: String)
    {
        hasOriginalTermios = tcgetattr(STDIN_FILENO, &originalTermios) == 0
        if hasOriginalTermios
        {
            var raw = originalTermios
            cfmakeraw(&raw)
            withUnsafeMutableBytes(of: &raw.c_cc)
            {
                controlCharacters in
                controlCharacters[Int(VMIN)] = kTerminalRawInputMinBytes
                controlCharacters[Int(VTIME)] = kTerminalRawInputTimeoutDeciseconds
            }
            _ = tcsetattr(STDIN_FILENO, TCSANOW, &raw)
        }
        writeTerminal(terminalTitleSequence(title))
        writeTerminal(kTerminalEnterAlternateScreen)
    }

    func restore()
    {
        writeTerminal(kTerminalLeaveAlternateScreen)
        if hasOriginalTermios
        {
            var restored = originalTermios
            _ = tcsetattr(STDIN_FILENO, TCSANOW, &restored)
        }
    }
}

private func sanitizedTerminalTitle(_ value: String) -> String
{
    String(value.unicodeScalars.filter { $0.value >= 0x20 && $0.value != 0x7f }.map(Character.init))
}

private func terminalTitleSequence(_ title: String) -> String
{
    "\u{001B}]0;\(sanitizedTerminalTitle(title + kTerminalTitleSuffix))\u{0007}"
}

private func styleHasAnimatedTextGradient(_ style: Style?) -> Bool
{
    guard let style else
    {
        return false
    }
    return style.textColorGradient != nil || style.textBackgroundGradient != nil
}

private func elementHasAnimatedTextGradient(_ element: UIElement) -> Bool
{
    if styleHasAnimatedTextGradient(element.style) ||
        styleHasAnimatedTextGradient(element.focusStyle) ||
        styleHasAnimatedTextGradient(element.editStyle) ||
        styleHasAnimatedTextGradient(element.cursorStyle) ||
        styleHasAnimatedTextGradient(element.selectedStyle) ||
        styleHasAnimatedTextGradient(element.activeStyle)
    {
        return true
    }
    return element.children.contains { elementHasAnimatedTextGradient($0) }
}

private func windowHasAnimatedTextGradient(_ window: GeneratedWindowBase) -> Bool
{
    if styleHasAnimatedTextGradient(window.generatedWindowStyle)
    {
        return true
    }
    return window.elements.contains { elementHasAnimatedTextGradient($0) }
}

public struct RuntimeError: Error, CustomStringConvertible
{
    public var description: String

    public init(_ description: String)
    {
        self.description = description
    }
}

private struct RememberedDynamicFocus
{
    let elementId: String
    let parentIdentity: ObjectIdentifier
    let childFocusScopeGeneration: Int
}

private struct RememberedScrollViewDescendant
{
    let element: UIElement
    let scrollViewIdentity: ObjectIdentifier
    let elementIdentity: ObjectIdentifier
}

private struct FocusIdentity
{
    let element: UIElement?
    let elementIdentity: ObjectIdentifier?
}

private struct BackgroundFocusCleanupContext
{
    var hadActiveStackFrame = false
    var focused = FocusIdentity(element: nil, elementIdentity: nil)
    var focusedIndex = -1
    var activeScrollView: ScrollView?
    var activeScrollViewPosition: ScrollViewPosition?
}

fileprivate struct EditSnapshot
{
    var element: UIElement?
    var textValue = ""
    var textCursor = 0
    var textSelectionAnchor: Int?
    var numberValue = 0.0
    var selectedIndex = -1
    var selectedValues: [String] = []
}

private final class GeneratedRuntimeController
{
    private let rootWindow: GeneratedWindowBase
    private let rootOptions: GeneratedWindowRuntimeOptions
    private var viewport: Rect
    private var rootFocusedName: String?
    private var rootEditMode = false
    private var rootActiveDynamicEditName: String?
    private var rootActiveScrollView: ScrollView?
    private var rootActiveScrollViewEditElement: UIElement?
    private var rootEditSnapshot: EditSnapshot?
    private var rootScrollViewLastDescendantByScrollId: [ObjectIdentifier: RememberedScrollViewDescendant] = [:]
    private var rememberedDynamicFocusByParentId: [String: RememberedDynamicFocus] = [:]
    private var pendingMcpMouseDown: Point?
    private var terminalMouseSelectionElement: TextInput?
    private var terminalMouseSelectionFrame: Rect?
    private var terminalMouseSelectionAnchor = 0
    private var mousePressActivatedClickControl = false
    private var copyNotificationRequested = false
    private var beforeStandardButtonAction: (() -> Void)?

    init(window: GeneratedWindowBase, options: GeneratedWindowRuntimeOptions, args: [String])
    {
        self.rootWindow = window
        var runtimeOptions = options
        if runtimeOptions.windowStack == nil
        {
            runtimeOptions.windowStack = GeneratedWindowStack()
        }
        self.rootOptions = runtimeOptions
        let size = window.generatedContentSize()
        self.viewport = parseViewport(args) ?? Rect(row: 0, col: 0, width: size.width, height: size.height)
        self.rootFocusedName = runtimeOptions.initialFocusName
        self.rootEditMode = runtimeOptions.startInEditMode
    }

    func runTerminal() -> Int
    {
        guard isatty(STDIN_FILENO) != 0 && isatty(STDOUT_FILENO) != 0 else
        {
            syncImageTerminalCellPixels()
            let output = rootWindow.renderPlainText()
            if !output.isEmpty
            {
                print(output)
            }
            return 0
        }

        let mode = TerminalModeGuard(title: rootWindow.windowTitle)
        defer
        {
            mode.restore()
        }

        var terminalFrameBuffer: TerminalFrameBuffer?
        let inputParser = TerminalInputParser()
        var pendingMouseDown: Point?
        let hasAnimatedTextGradient = windowHasAnimatedTextGradient(rootWindow)
        var lastAnimatedRenderAt = Date()
        var copyNotification = ""
        var copyNotificationExpiresAt: Date?
        var sixelFallbackWarningRejected = false
        if windowNeedsSixelFallbackWarning(rootWindow)
        {
            let warningDialog = MessageBoxYesNo("Warning", kSixelFallbackWarning)
            warningDialog.onClose = { confirmed in
                if !confirmed
                {
                    sixelFallbackWarningRejected = true
                }
            }
            var frame = GeneratedWindowFrameOptions()
            frame.className = "MessageBoxYesNo"
            frame.initialFocusName = "no_btn"
            rootOptions.windowStack?.push(warningDialog, frame)
        }
        var dirty = true
        beforeStandardButtonAction = { [weak self] in
            guard let self else
            {
                return
            }
            terminalFrameBuffer?.setContent(self.renderContent())
            if !copyNotification.isEmpty
            {
                terminalFrameBuffer?.renderNotification(copyNotification)
            }
            if let output = terminalFrameBuffer?.renderDiff(), !output.isEmpty
            {
                writeTerminal(output)
            }
            usleep(kDialogButtonCloseDelayMicros)
        }
        defer
        {
            beforeStandardButtonAction = nil
        }
        while true
        {
            if options.shouldClose?() == true || sixelFallbackWarningRejected
            {
                return 0
            }
            let size = currentTerminalSize(fallback: rootWindow.generatedContentSize())
            viewport = Rect(row: 0, col: 0, width: size.width, height: size.height)
            if terminalFrameBuffer == nil
            {
                terminalFrameBuffer = TerminalFrameBuffer(width: size.width, height: size.height)
                dirty = true
            }
            if terminalFrameBuffer?.resize(width: size.width, height: size.height) == true
            {
                dirty = true
            }
            if hasAnimatedTextGradient && Date().timeIntervalSince(lastAnimatedRenderAt) >= kAnimatedRenderIntervalSeconds
            {
                dirty = true
                lastAnimatedRenderAt = Date()
            }
            if !copyNotification.isEmpty,
               let expiresAt = copyNotificationExpiresAt,
               Date() >= expiresAt
            {
                copyNotification = ""
                copyNotificationExpiresAt = nil
                dirty = true
            }
            if dirty
            {
                terminalFrameBuffer?.setContent(renderContent())
                if !copyNotification.isEmpty
                {
                    terminalFrameBuffer?.renderNotification(copyNotification)
                }
                if let output = terminalFrameBuffer?.renderDiff(), !output.isEmpty
                {
                    writeTerminal(output)
                }
                dirty = false
            }

            let inputBytes = readTerminalInputBytes()
            guard !inputBytes.isEmpty else
            {
                usleep(kTerminalInputIdleSleepMicros)
                continue
            }
            do
            {
                let events = coalesceTerminalInputEvents(inputParser.feed(inputBytes))
                guard !events.isEmpty else
                {
                    continue
                }
                for event in events
                {
                    let keepRunning = try handleTerminalInput(event, pendingMouseDown: &pendingMouseDown)
                    if !keepRunning || options.shouldClose?() == true
                    {
                        return 0
                    }
                    if takeCopyNotificationRequested()
                    {
                        copyNotification = kCopyNotificationText
                        copyNotificationExpiresAt = Date().addingTimeInterval(kCopyNotificationDurationSeconds)
                    }
                }
                dirty = true
            }
            catch
            {
                writeTerminal("\u{001B}[0m\u{001B}[\(max(1, viewport.height));1H")
                FileHandle.standardError.write(Data("uimd: Swift terminal input error: \(error)\n".utf8))
                return 1
            }
        }
    }

    private func handleTerminalInput(_ event: TerminalInputEvent, pendingMouseDown: inout Point?) throws -> Bool
    {
        let hadModalStack = hasOpenModalStack()
        let backgroundFocusCleanupContext = captureBackgroundFocusCleanupContext()
        cleanupClosedStackFrames()
        do
        {
            let keepRunning = try handleTerminalInputBody(event, pendingMouseDown: &pendingMouseDown)
            cleanupClosedStackFrames()
            cleanupBackgroundFocusAfterModalClose(
                backgroundFocusCleanupContext,
                wasOpen: hadModalStack
            )
            return keepRunning
        }
        catch
        {
            cleanupClosedStackFrames()
            throw error
        }
    }

    private func handleTerminalInputBody(_ event: TerminalInputEvent, pendingMouseDown: inout Point?) throws -> Bool
    {
        switch event
        {
        case .none:
            return true
        case .key(let key):
            if key == "Ctrl+C"
            {
                return false
            }
            try handleTerminalKeyInput(key, refreshLayoutForNavigation: false)
            return true
        case .paste(let text):
            guard let focusedName else
            {
                return true
            }
            _ = try performTypeText(focusedName, text)
            return true
        case .mouse(let kind, let point):
            let arguments: [String: Any] = ["x": point.col, "y": point.row]
            if kind == "press"
            {
                pendingMouseDown = point
                try performMousePointerEvent(arguments, pressed: true, refreshLayout: false)
                if mousePressActivatedClickControl
                {
                    pendingMouseDown = nil
                    mousePressActivatedClickControl = false
                }
            }
            else if kind == "move"
            {
                if let down = pendingMouseDown, down != point
                {
                    pendingMouseDown = nil
                }
                if !performTerminalMouseSelection(row: point.row, col: point.col, release: false)
                {
                    try performMousePointerEvent(arguments, pressed: false, refreshLayout: false)
                }
            }
            else
            {
                let isClick = pendingMouseDown == point
                pendingMouseDown = nil
                if isClick
                {
                    _ = try performMouseClick(arguments, refreshLayout: false)
                }
                else if performTerminalMouseSelection(row: point.row, col: point.col, release: true)
                {
                    return true
                }
                else
                {
                    try performMousePointerEvent(arguments, pressed: false, refreshLayout: false)
                }
            }
            return true
        case .scroll(let point, let delta):
            try performTerminalScrollAt(point: point, delta: delta)
            return true
        }
    }

    private func stateResult() throws -> Any
    {
        return try callTool("get_state", [:])
    }

    private func snapshotResult(_ element: UIElement) -> Any
    {
        return snapshot(element)
    }

    private enum RuntimeEventResult
    {
        case state
        case snapshot(UIElement)
        case payload(Any)
    }

    private func toolPayload(for result: RuntimeEventResult) throws -> Any
    {
        switch result
        {
        case .state:
            return try stateResult()
        case .snapshot(let element):
            return snapshotResult(element)
        case .payload(let payload):
            return payload
        }
    }

    private var activeFrame: GeneratedWindowStackFrame?
    {
        rootOptions.windowStack?.topFrame
    }

    private var window: GeneratedWindowBase
    {
        activeFrame?.window ?? rootWindow
    }

    private var options: GeneratedWindowRuntimeOptions
    {
        activeFrame?.options ?? rootOptions
    }

    @discardableResult
    private func handleActiveFrameButton(_ name: String) -> Bool
    {
        if let frame = activeFrame
        {
            return frame.options.onButton?(name) ?? false
        }
        return rootOptions.onButton?(name) ?? false
    }

    private var focusedName: String?
    {
        get
        {
            if let frame = activeFrame
            {
                return frame.focusedName
            }
            return rootFocusedName
        }
        set
        {
            let previous = focusedName
            if let frame = activeFrame
            {
                frame.focusedName = newValue
            }
            else
            {
                rootFocusedName = newValue
            }
            guard previous != newValue else
            {
                return
            }
            if let previous
            {
                options.onFocusChanged?(previous, false)
            }
            if let newValue
            {
                options.onFocusChanged?(newValue, true)
            }
        }
    }

    private var editMode: Bool
    {
        get
        {
            activeFrame?.editMode ?? rootEditMode
        }
        set
        {
            if !newValue
            {
                activeDynamicEditName = nil
                activeScrollView = nil
                activeScrollViewEditElement = nil
            }
            if let frame = activeFrame
            {
                frame.editMode = newValue
            }
            else
            {
                rootEditMode = newValue
            }
        }
    }

    private var activeDynamicEditName: String?
    {
        get
        {
            if let frame = activeFrame
            {
                return frame.activeDynamicEditName
            }
            return rootActiveDynamicEditName
        }
        set
        {
            if let frame = activeFrame
            {
                frame.activeDynamicEditName = newValue
            }
            else
            {
                rootActiveDynamicEditName = newValue
            }
        }
    }

    private var activeScrollView: ScrollView?
    {
        get
        {
            if let frame = activeFrame
            {
                return frame.activeScrollView
            }
            return rootActiveScrollView
        }
        set
        {
            if let frame = activeFrame
            {
                frame.activeScrollView = newValue
            }
            else
            {
                rootActiveScrollView = newValue
            }
        }
    }

    private var activeScrollViewEditElement: UIElement?
    {
        get
        {
            if let frame = activeFrame
            {
                return frame.activeScrollViewEditElement
            }
            return rootActiveScrollViewEditElement
        }
        set
        {
            if let frame = activeFrame
            {
                frame.activeScrollViewEditElement = newValue
            }
            else
            {
                rootActiveScrollViewEditElement = newValue
            }
        }
    }

    private var editSnapshot: EditSnapshot?
    {
        get
        {
            if let frame = activeFrame
            {
                return frame.editSnapshot
            }
            return rootEditSnapshot
        }
        set
        {
            if let frame = activeFrame
            {
                frame.editSnapshot = newValue
            }
            else
            {
                rootEditSnapshot = newValue
            }
        }
    }

    func serve() -> Int
    {
        syncImageTerminalCellPixels()
        let port = Int(optionValue("--mcp-port") ?? "0") ?? 0
        guard port > 0 else
        {
            FileHandle.standardError.write(Data("error: --mcp-port is required for Swift MCP runtime\n".utf8))
            return 1
        }
        let server = socket(AF_INET, SOCK_STREAM, 0)
        if server < 0
        {
            FileHandle.standardError.write(Data("error: cannot create MCP socket\n".utf8))
            return 1
        }
        var reuse: Int32 = 1
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, socklen_t(MemoryLayout<Int32>.size))
        var address = sockaddr_in()
        address.sin_len = UInt8(MemoryLayout<sockaddr_in>.size)
        address.sin_family = sa_family_t(AF_INET)
        address.sin_port = in_port_t(port).bigEndian
        address.sin_addr = in_addr(s_addr: inet_addr("127.0.0.1"))
        let bindResult = withUnsafePointer(to: &address)
        {
            $0.withMemoryRebound(to: sockaddr.self, capacity: 1)
            {
                Darwin.bind(server, $0, socklen_t(MemoryLayout<sockaddr_in>.size))
            }
        }
        if bindResult != 0 || listen(server, 16) != 0
        {
            close(server)
            FileHandle.standardError.write(Data("error: cannot bind Swift MCP socket on port \(port)\n".utf8))
            return 1
        }
        while true
        {
            let client = accept(server, nil, nil)
            if client < 0
            {
                continue
            }
            handle(client: client)
            close(client)
        }
    }

    private func optionValue(_ name: String) -> String?
    {
        let args = CommandLine.arguments
        guard let index = args.firstIndex(of: name), index + 1 < args.count else
        {
            return nil
        }
        return args[index + 1]
    }

    private func handle(client: Int32)
    {
        guard let line = readLine(from: client), !line.isEmpty else
        {
            return
        }
        let response: String
        do
        {
            response = try handleJsonRpcLine(line)
        }
        catch
        {
            response = jsonString(["jsonrpc": "2.0", "id": NSNull(), "error": ["code": -32603, "message": "\(error)"]])
        }
        _ = response.withCString
        {
            Darwin.send(client, $0, strlen($0), 0)
        }
        _ = "\n".withCString
        {
            Darwin.send(client, $0, strlen($0), 0)
        }
    }

    private func handleJsonRpcLine(_ line: String) throws -> String
    {
        let data = Data(line.utf8)
        let json = try JSONSerialization.jsonObject(with: data)
        guard let request = json as? [String: Any] else
        {
            throw RuntimeError("JSON-RPC request must be an object")
        }
        let id = request["id"] ?? NSNull()
        do
        {
            let result = try handleRequest(request)
            return jsonString(["jsonrpc": "2.0", "id": id, "result": result])
        }
        catch
        {
            return jsonString(["jsonrpc": "2.0", "id": id, "error": ["code": -32603, "message": "\(error)"]])
        }
    }

    private func handleRequest(_ request: [String: Any]) throws -> Any
    {
        let method = request["method"] as? String ?? ""
        let params = request["params"] as? [String: Any] ?? [:]
        if method == "initialize"
        {
            return [
                "protocolVersion": "2024-11-05",
                "serverInfo": ["name": "ui-swift-runtime", "version": "0.1"],
                "capabilities": ["tools": [:]],
            ]
        }
        if method == "tools/list"
        {
            return ["tools": listTools()]
        }
        if method == "tools/call"
        {
            let name = params["name"] as? String ?? ""
            let arguments = params["arguments"] as? [String: Any] ?? [:]
            let toolResult = try callTool(name, arguments)
            return ["content": [["type": "text", "text": jsonString(toolResult)]]]
        }
        if method.hasPrefix("ui.")
        {
            return try callTool(String(method.dropFirst(3)), params)
        }
        return try callTool(method, params)
    }

    private func listTools() -> [[String: Any]]
    {
        var names = [
            "get_window",
            "get_elements",
            "get_element",
            "get_focused_element",
            "get_state",
            "get_render_rect",
            "set_viewport",
            "repaint",
            "get_render_frame",
            "get_render_snapshot",
            "get_render_snapshot_compact",
            "get_render_cell",
            "get_image_render_info",
            "get_text_snapshot",
            "ui.get_schema",
            "focus_element",
            "enter_edit_mode",
            "exit_edit_mode",
            "get_edit_mode",
            "activate_element",
            "click_element",
            "press_key",
            "set_cursor",
            "select_text",
            "copy_selection",
            "replace_selection",
            "get_selection",
            "set_text",
            "type_text",
            "ui.get_value",
            "ui.set_value",
            "ui.get_options",
            "ui.select_option",
            "ui.set_checked",
            "ui.uncheck",
            "ui.check",
            "ui.set_selection",
            "scroll",
            "mouse_click",
            "mouse_press",
            "mouse_move",
            "mouse_release",
        ]
        names.append(contentsOf: window.mcpAppTools.map { $0.name })
        return names.map { ["name": $0, "description": "Swift UIMD runtime tool", "inputSchema": ["type": "object"]] }
    }

    private func callTool(_ name: String, _ arguments: [String: Any]) throws -> Any
    {
        let hadModalStack = hasOpenModalStack()
        let backgroundFocusCleanupContext = captureBackgroundFocusCleanupContext()
        cleanupClosedStackFrames()
        let result = try callToolBody(name, arguments)
        cleanupClosedStackFrames()
        cleanupBackgroundFocusAfterModalClose(
            backgroundFocusCleanupContext,
            wasOpen: hadModalStack
        )
        return result
    }

    private func hasOpenModalStack() -> Bool
    {
        rootOptions.windowStack?.allFrames.isEmpty == false
    }

    private func cleanupClosedStackFrames()
    {
        guard let stack = rootOptions.windowStack else
        {
            return
        }
        while let frame = stack.topFrame, frame.window.shouldClose()
        {
            stack.remove(frame.window)
        }
    }

    private func captureBackgroundFocusCleanupContext() -> BackgroundFocusCleanupContext
    {
        var context = BackgroundFocusCleanupContext()
        context.hadActiveStackFrame = activeFrame != nil
        guard context.hadActiveStackFrame else
        {
            return context
        }
        context.activeScrollView = rootActiveScrollView
        context.activeScrollViewPosition = rootActiveScrollView?.scrollPosition()
        let focusable = focusableElements(in: rootWindow, activeScrollView: context.activeScrollView)
        context.focused = focusIdentityFor(targetWindow: rootWindow, focusable: focusable, focusedName: rootFocusedName)
        context.focusedIndex = focusIndex(in: focusable, matching: rootFocusedName)
        return context
    }

    private func cleanupBackgroundFocusAfterModalClose(
        _ context: BackgroundFocusCleanupContext,
        wasOpen: Bool
    )
    {
        guard context.hadActiveStackFrame && wasOpen && !hasOpenModalStack() else
        {
            return
        }
        if let activeScrollView = rootActiveScrollView,
           !focusIdentityPresent(in: rootWindow, activeScrollView: activeScrollView, previous: context.focused)
        {
            if repairRemovedBackgroundScrollViewScope(activeScrollView, context: context)
            {
                restoreBackgroundScrollPosition(context, activeScrollView: activeScrollView)
                return
            }
            clearRemovedBackgroundScrollViewScope(activeScrollView)
            return
        }
        let activeScrollView = rootActiveScrollView
        if exitBackgroundEditModeAfterModalClose()
        {
            restoreBackgroundScrollPosition(context, activeScrollView: activeScrollView)
            return
        }
        clearFocusIfElementRemoved(context.focused)
        restoreBackgroundScrollPosition(context, activeScrollView: activeScrollView)
    }

    private func restoreBackgroundScrollPosition(
        _ context: BackgroundFocusCleanupContext,
        activeScrollView: ScrollView?
    )
    {
        guard let activeScrollView,
              activeScrollView === context.activeScrollView,
              let position = context.activeScrollViewPosition
        else
        {
            return
        }
        activeScrollView.restoreScrollPosition(position)
    }

    private func repairRemovedBackgroundScrollViewScope(
        _ activeScrollView: ScrollView,
        context: BackgroundFocusCleanupContext
    ) -> Bool
    {
        guard activeScrollViewRepresentedInCurrentLayout(rootWindow, activeScrollView)
        else
        {
            return false
        }
        let focusable = focusableElements(in: rootWindow, activeScrollView: activeScrollView)
        guard !focusable.isEmpty else
        {
            return false
        }
        let index: Int
        if context.focusedIndex >= 0
        {
            index = min(context.focusedIndex, focusable.count - 1)
        }
        else
        {
            index = min(max(0, focusIndex(in: focusable, matching: rootFocusedName)), focusable.count - 1)
        }
        let nextFocus = focusable[index]
        rootEditSnapshot = nil
        rootEditMode = true
        rootActiveScrollView = activeScrollView
        rootActiveScrollViewEditElement = nil
        rootActiveDynamicEditName = nil
        rootWindow.suppressActiveContainerScopeVisuals = false
        setFocusedElement(nextFocus)
        return true
    }

    private func clearRemovedBackgroundScrollViewScope(_ activeScrollView: ScrollView)
    {
        rootScrollViewLastDescendantByScrollId.removeValue(forKey: ObjectIdentifier(activeScrollView))
        rootEditSnapshot = nil
        rootEditMode = false
        rootActiveScrollView = nil
        rootActiveScrollViewEditElement = nil
        rootActiveDynamicEditName = nil
        rootFocusedName = nil
        rootWindow.suppressActiveContainerScopeVisuals = false
    }

    private func exitBackgroundEditModeAfterModalClose() -> Bool
    {
        rootEditSnapshot = nil
        rootActiveScrollViewEditElement = nil
        if let activeScrollView = rootActiveScrollView
        {
            rootEditMode = true
            rootWindow.suppressActiveContainerScopeVisuals = false
            if let focused = rootFocusedName.flatMap({ rootWindow.element(named: $0) })
            {
                rememberScrollViewDescendant(activeScrollView, focused)
            }
            return true
        }
        guard let focused = rootFocusedName,
              let element = rootWindow.element(named: focused)
        else
        {
            rootEditMode = false
            rootActiveDynamicEditName = nil
            return false
        }
        guard let parentId = element.parentFocusHostId,
              let parent = rootWindow.element(named: parentId),
              parent is ReusableElement || parent is ViewHost
        else
        {
            rootEditMode = false
            rootActiveDynamicEditName = nil
            return false
        }
        rememberDynamicFocus(parentId: parentId, parent: parent, elementId: focused)
        rootEditMode = true
        rootActiveDynamicEditName = nil
        return true
    }

    private func clearFocusIfElementRemoved(_ previous: FocusIdentity)
    {
        guard previous.element != nil else
        {
            return
        }
        if focusIdentityPresent(in: rootWindow, activeScrollView: rootActiveScrollView, previous: previous)
        {
            return
        }
        if let activeScrollView = rootActiveScrollView
        {
            rootScrollViewLastDescendantByScrollId.removeValue(forKey: ObjectIdentifier(activeScrollView))
            rootActiveScrollView = nil
            rootActiveScrollViewEditElement = nil
        }
        rootEditSnapshot = nil
        rootFocusedName = nil
        rootEditMode = false
        rootActiveDynamicEditName = nil
        rootWindow.suppressActiveContainerScopeVisuals = false
    }

    private func standardEscapeButtonName(for window: GeneratedWindowBase, className: String?) -> String?
    {
        let effectiveClassName = (className?.isEmpty == false) ? className! : window.mcpClassName
        if effectiveClassName == "FileBrowser"
        {
            return window.element(named: "close_btn") is Button ? "close_btn" : nil
        }
        if effectiveClassName.hasPrefix("MessageBox")
        {
            for buttonName in ["cancel_btn", "no_btn", "ok_btn"]
            {
                if window.element(named: buttonName) is Button
                {
                    return buttonName
                }
            }
        }
        return nil
    }

    private func handleStandardEscapeButton() -> Bool
    {
        if let frame = activeFrame
        {
            guard let buttonName = standardEscapeButtonName(for: frame.window, className: frame.className) else
            {
                return false
            }
            frame.editMode = false
            frame.activeDynamicEditName = nil
            focusedName = buttonName
            beforeStandardButtonAction?()
            _ = frame.options.onButton?(buttonName)
            return true
        }

        guard let buttonName = standardEscapeButtonName(for: rootWindow, className: rootWindow.mcpClassName) else
        {
            return false
        }
        rootEditMode = false
        rootActiveDynamicEditName = nil
        focusedName = buttonName
        beforeStandardButtonAction?()
        _ = rootOptions.onButton?(buttonName)
        return true
    }

    private func callToolBody(_ name: String, _ arguments: [String: Any]) throws -> Any
    {
        switch name
        {
        case "get_window":
            return getWindow()
        case "get_elements":
            return window.elements.map { snapshot($0) }
        case "get_element":
            return snapshotResult(try requireElement(arguments["element_id"] as? String))
        case "get_focused_element":
            guard let focusedName, let element = window.element(named: focusedName) else
            {
                return NSNull()
            }
            return snapshotResult(element)
        case "get_state":
            _ = renderContent()
            let focusedSnapshot: Any
            if let focusedName,
               let focusedElement = window.element(named: focusedName)
            {
                focusedSnapshot = snapshot(focusedElement, refreshLayout: false)
            }
            else
            {
                focusedSnapshot = NSNull()
            }
            return [
                "window": getWindow(),
                "focused_element": focusedSnapshot,
                "edit_mode": editMode,
                "elements": window.runtimeElements().map { snapshot($0, refreshLayout: false) },
            ]
        case "get_render_rect", "get_viewport":
            return ["row": viewport.row, "col": viewport.col, "width": viewport.width, "height": viewport.height]
        case "set_viewport":
            viewport = Rect(
                row: max(0, intArg(arguments, "row", 0)),
                col: max(0, intArg(arguments, "col", 0)),
                width: max(1, intArg(arguments, "width", 1)),
                height: max(1, intArg(arguments, "height", 1))
            )
            return try callTool("get_render_rect", [:])
        case "repaint":
            return ["ok": true]
        case "get_render_frame":
            let content = renderMcpSnapshotContent()
            return ["width": content.first?.count ?? 0, "height": content.count, "cells": compactCells(content)]
        case "get_render_snapshot":
            return withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                let content = renderMcpSnapshotContent()
                let lines = renderedText(content)
                return ["ansi_lines": lines, "text_lines": lines, "cells": fullCells(content)]
            }
        case "get_render_snapshot_compact":
            return withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                ["format": "render-cells-v1", "cells": compactCells(renderMcpSnapshotContent())]
            }
        case "get_render_cell":
            let row = intArg(arguments, "y", intArg(arguments, "row", 0))
            let col = intArg(arguments, "x", intArg(arguments, "col", 0))
            return try withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                let content = renderMcpSnapshotContent()
                guard row >= 0 && row < content.count && col >= 0 && col < content[row].count else
                {
                    throw RuntimeError("render cell out of range")
                }
                var cell = cellJson(content[row][col])
                cell["x"] = col
                cell["y"] = row
                return cell
            }
        case "get_image_render_info":
            return try getImageRenderInfo(arguments["element_id"] as? String)
        case "get_text_snapshot":
            return ["lines": renderedText(renderMcpSnapshotContent())]
        case "focus_element":
            let element = try requireElement(arguments["element_id"] as? String)
            focusActiveWindowElement(element)
            return snapshotResult(element)
        case "enter_edit_mode":
            if let id = arguments["element_id"] as? String
            {
                if editMode,
                   activeScrollView == nil,
                   let currentFocusedName = focusedName,
                   let current = window.element(named: currentFocusedName)
                {
                    commitEdit(current)
                    editSnapshot = nil
                    activeScrollViewEditElement = nil
                    editMode = false
                }
                let element = try requireElement(id)
                focusActiveWindowElement(element)
            }
            if let focusedName,
               let element = window.element(named: focusedName),
               ownsGeneratedScrollScope(element),
               enterActiveScrollViewScope(from: element)
            {
                _ = options.onEditStarted?(focusedName)
            }
            else
            {
                if let focusedName,
                   let element = window.element(named: focusedName)
                {
                    beginElementEdit(element, elementId: focusedName)
                }
            }
            return ["edit_mode": editMode]
        case "exit_edit_mode":
            editMode = false
            activeDynamicEditName = nil
            return ["edit_mode": editMode]
        case "get_edit_mode":
            return ["edit_mode": editMode]
        case "activate_element":
            return try activateElement(arguments["element_id"] as? String)
        case "click_element":
            return try clickElement(arguments["element_id"] as? String)
        case "press_key":
            return try pressKey(arguments["key"] as? String ?? "")
        case "set_cursor":
            return try setCursor(arguments)
        case "select_text":
            return try selectText(arguments)
        case "copy_selection":
            return try copySelection(arguments)
        case "replace_selection":
            return try replaceSelection(arguments)
        case "get_selection":
            return try getSelection(arguments)
        case "set_text":
            return try setText(arguments["element_id"] as? String, stringValue(arguments["text"]))
        case "type_text":
            return try typeText(arguments["element_id"] as? String, stringValue(arguments["text"]))
        case "scroll":
            return try scrollElement(arguments)
        case "mouse_click":
            pendingMcpMouseDown = nil
            return try mouseClick(arguments)
        case "mouse_press":
            pendingMcpMouseDown = Point(row: intArg(arguments, "y", 0), col: intArg(arguments, "x", 0))
            return try mousePointerEvent(arguments, pressed: true)
        case "mouse_move":
            let point = Point(row: intArg(arguments, "y", 0), col: intArg(arguments, "x", 0))
            if let down = pendingMcpMouseDown, down != point
            {
                pendingMcpMouseDown = nil
            }
            return try mousePointerEvent(arguments, pressed: false)
        case "mouse_release":
            let point = Point(row: intArg(arguments, "y", 0), col: intArg(arguments, "x", 0))
            let isClick = pendingMcpMouseDown == point
            pendingMcpMouseDown = nil
            return try isClick ? mouseClick(arguments) : mousePointerEvent(arguments, pressed: false)
        case "ui.get_schema", "get_schema":
            return getSchema()
        case "ui.get_value", "get_value":
            let element = try requireElement(arguments["element_id"] as? String)
            return ["value": element.valueForSnapshot ?? NSNull()]
        case "ui.set_value", "set_value":
            return try setValue(arguments["element_id"] as? String, arguments["value"])
        case "ui.get_options", "get_options":
            let element = try requireElement(arguments["element_id"] as? String)
            if let combo = element as? ComboBox
            {
                return ["options": combo.options]
            }
            throw RuntimeError("Element has no selectable options: \(element.name)")
        case "ui.select_option", "select_option":
            return try selectOption(arguments)
        case "ui.set_checked", "set_checked":
            return try setChecked(arguments["element_id"] as? String, boolValue(arguments["checked"] ?? arguments["value"]))
        case "ui.check", "check":
            return try setChecked(arguments["element_id"] as? String, true)
        case "ui.uncheck", "uncheck":
            return try setChecked(arguments["element_id"] as? String, false)
        case "ui.set_selection", "set_selection":
            return try setSelection(arguments)
        default:
            if window.mcpAppTools.contains(where: { $0.name == name })
            {
                let input = jsonString(arguments)
                let output = try window.callAppTool(name, inputJson: input)
                return parseJsonObject(output) ?? ["result": output]
            }
            throw RuntimeError("unknown tool: \(name)")
        }
    }

    private func getWindow() -> [String: Any]
    {
        let frameSize = windowFrameSize()
        return [
            "title": window.windowTitle,
            "class": activeWindowClassName(),
            "backend": "swift",
            "mode": resolvedWindowMode(),
            "description": window.mcpDescription,
            "width": frameSize.width,
            "height": frameSize.height,
            "mcp_enabled": window.mcpEnabled,
        ]
    }

    private func activeWindowClassName() -> String
    {
        if let frame = activeFrame
        {
            if !frame.className.isEmpty
            {
                return frame.className
            }
            if !frame.window.mcpClassName.isEmpty
            {
                return frame.window.mcpClassName
            }
            return compactWindowClassName(frame.window.windowTitle)
        }
        if !rootWindow.mcpClassName.isEmpty
        {
            return rootWindow.mcpClassName
        }
        return compactWindowClassName(rootWindow.windowTitle)
    }

    private func compactWindowClassName(_ title: String) -> String
    {
        let compact = title.unicodeScalars.filter { CharacterSet.alphanumerics.contains($0) }.map(String.init).joined()
        return compact.isEmpty ? "GeneratedWindowBase" : compact
    }

    private func snapshot(_ element: UIElement, refreshLayout: Bool = true) -> [String: Any]
    {
        if refreshLayout
        {
            _ = renderContent()
        }
        let elementId = runtimeElementId(element)
        let metadata = window.elementMetadata(elementId) ?? window.elementMetadata(element.name)
        let frameSize = windowFrameSize()
        let offset = windowOffset(frameSize: frameSize)
        let content = windowContentBounds(frameSize: frameSize)
        let surfaceFrame = Rect(
            row: element.frame.row + offset.row + content.row,
            col: element.frame.col + offset.col + content.col,
            width: element.frame.width,
            height: element.frame.height
        )
        let dynamicDescendant = element.parentFocusHostId != nil
        let elementEditActive = editMode && (!dynamicDescendant || activeDynamicEditName == elementId)
        var result: [String: Any] = [
            "id": elementId,
            "type": element.elementType,
            "role": element.role,
            "description": metadata?.description ?? "",
            "focused": focusedName == elementId,
            "visible": true,
            "enabled": element.enabled,
            "bounds": rectJson(surfaceFrame),
            "style_state": focusedName == elementId ? (elementEditActive ? "edit" : "focus") : "base",
            "value": element.valueForSnapshot ?? NSNull(),
        ]
        if let combo = element as? ComboBox
        {
            result["options"] = combo.options
        }
        if let input = element as? TextInput
        {
            result["cursor"] = input.cursor
            result["selection"] = selectionSnapshot(input)
        }
        return result
    }

    private func runtimeElementId(_ element: UIElement) -> String
    {
        element.mcpElementId ?? element.name
    }

    private func runtimeWindowsForStateCleanup() -> [GeneratedWindowBase]
    {
        [rootWindow] + (rootOptions.windowStack?.allFrames.map(\.window) ?? [])
    }

    private func runtimeElementIsRepresented(_ elementId: String?, in targetWindow: GeneratedWindowBase) -> Bool
    {
        guard let elementId, !elementId.isEmpty else
        {
            return false
        }
        return targetWindow.element(named: elementId) != nil
    }

    private func rememberedDynamicFocusIsRepresented(parentId: String, remembered: RememberedDynamicFocus) -> Bool
    {
        for targetWindow in runtimeWindowsForStateCleanup()
        {
            guard let parent = targetWindow.element(named: parentId),
                  ObjectIdentifier(parent) == remembered.parentIdentity,
                  parent.childFocusScopeGeneration == remembered.childFocusScopeGeneration,
                  targetWindow.element(named: remembered.elementId) != nil
            else
            {
                continue
            }
            return true
        }
        return false
    }

    private func cleanupRememberedDynamicFocusAfterLayout() -> Bool
    {
        var changed = false
        for (parentId, remembered) in rememberedDynamicFocusByParentId
        {
            if rememberedDynamicFocusIsRepresented(parentId: parentId, remembered: remembered)
            {
                continue
            }
            rememberedDynamicFocusByParentId.removeValue(forKey: parentId)
            for targetWindow in runtimeWindowsForStateCleanup()
            {
                if let parent = targetWindow.element(named: parentId),
                   parent.lastFocusedChildId == remembered.elementId
                {
                    parent.lastFocusedChildId = nil
                }
            }
            changed = true
        }
        return changed
    }

    private func cleanupInvalidWindowFocusAfterLayout(
        _ targetWindow: GeneratedWindowBase,
        focusedName: inout String?,
        editMode: inout Bool,
        activeDynamicEditName: inout String?,
        activeScrollView: inout ScrollView?,
        activeScrollViewEditElement: inout UIElement?
    ) -> Bool
    {
        var changed = false
        if let scrollView = activeScrollView,
           !activeScrollViewRepresentedInCurrentLayout(targetWindow, scrollView)
        {
            activeScrollView = nil
            activeScrollViewEditElement = nil
            activeDynamicEditName = nil
            editMode = false
            focusedName = nil
            changed = true
        }
        if let editElement = activeScrollViewEditElement,
           !elementRepresentedInCurrentLayout(targetWindow, editElement)
        {
            activeScrollViewEditElement = nil
            if let scrollView = activeScrollView
            {
                let scopeRoot = generatedScrollViewProxy(in: targetWindow, for: scrollView) ?? scrollView
                activeDynamicEditName = runtimeElementId(scopeRoot)
            }
            changed = true
        }
        if let activeDynamicName = activeDynamicEditName,
           !runtimeElementIsRepresented(activeDynamicName, in: targetWindow)
        {
            activeDynamicEditName = nil
            editMode = false
            changed = true
        }
        if let focusedElementId = focusedName,
           !runtimeElementIsRepresented(focusedElementId, in: targetWindow)
        {
            focusedName = nil
            editMode = false
            activeDynamicEditName = nil
            changed = true
        }
        if editMode && focusedName == nil
        {
            editMode = false
            activeDynamicEditName = nil
            changed = true
        }
        return changed
    }

    private func cleanupInvalidRuntimeFocusAfterLayout() -> Bool
    {
        var changed = cleanupInvalidWindowFocusAfterLayout(
            rootWindow,
            focusedName: &rootFocusedName,
            editMode: &rootEditMode,
            activeDynamicEditName: &rootActiveDynamicEditName,
            activeScrollView: &rootActiveScrollView,
            activeScrollViewEditElement: &rootActiveScrollViewEditElement
        )
        if let frames = rootOptions.windowStack?.allFrames
        {
            for frame in frames
            {
                changed = cleanupInvalidWindowFocusAfterLayout(
                    frame.window,
                    focusedName: &frame.focusedName,
                    editMode: &frame.editMode,
                    activeDynamicEditName: &frame.activeDynamicEditName,
                    activeScrollView: &frame.activeScrollView,
                    activeScrollViewEditElement: &frame.activeScrollViewEditElement
                ) || changed
            }
        }
        changed = cleanupRememberedDynamicFocusAfterLayout() || changed
        return changed
    }

    private func dispatchGeneratedButton(_ element: UIElement, elementId: String) -> Bool
    {
        if let owner = ownerWindowForElement(window, element),
           owner !== window
        {
            return owner.handleGeneratedButton(element.name)
        }
        return rootOptions.onButton?(elementId) ?? false
    }

    private func dispatchGeneratedTextChanged(_ element: UIElement, elementId: String, value: String) -> Bool
    {
        if let owner = ownerWindowForElement(window, element),
           owner !== window
        {
            return owner.handleGeneratedTextChanged(element.name, value: value)
        }
        return options.onTextChanged?(elementId, value) ?? false
    }

    private func dispatchGeneratedSelectionChanged(_ element: UIElement, elementId: String, value: [String]) -> Bool
    {
        if let owner = ownerWindowForElement(window, element),
           owner !== window
        {
            return owner.handleGeneratedSelectionChanged(element.name, value: value)
        }
        return options.onSelectionChanged?(elementId, value) ?? false
    }

    private func getImageRenderInfo(_ id: String?) throws -> [String: Any]
    {
        syncImageTerminalCellPixels()
        let element = try requireElement(id)
        guard let image = element as? Image else
        {
            throw RuntimeError("Element is not an image: \(element.name)")
        }
        _ = renderContent()
        let bounds = image.frame
        let frameSize = windowFrameSize()
        let offset = windowOffset(frameSize: frameSize)
        let content = windowContentBounds(frameSize: frameSize)
        let surfaceBounds = Rect(
            row: bounds.row + offset.row + content.row,
            col: bounds.col + offset.col + content.col,
            width: bounds.width,
            height: bounds.height
        )
        let width = max(1, bounds.width)
        let height = max(1, bounds.height)
        let sourcePath = resolvedImagePath(image.source)
        let sourceRaster = loadImageRaster(sourcePath)
        let sourceLoaded = sourceRaster != nil
        let sourceWidth = sourceRaster?.width ?? 0
        let sourceHeight = sourceRaster?.height ?? 0
        var resolvedMode = resolvedImageRenderMode(image)
        if !sourceLoaded
        {
            resolvedMode = "placeholder"
        }
        let style = image.effectiveStyle()
        let background = imageFallbackBackground(style: style, state: ElementRenderState())
        let region = imageCellRegion(
            width: width,
            height: height,
            sourceWidth: sourceWidth,
            sourceHeight: sourceHeight,
            fit: image.fit,
            align: image.align,
            verticalAlign: image.verticalAlign
        )
        let visibleRows = sourceLoaded ? region.height : 0
        let rawPresent = resolvedMode == "sixel" && visibleRows > 0
        let visibleWidth = visibleRows > 0 ? region.width : 0
        let visibleHeight = visibleRows
        let imageRight = region.col + region.width
        let imageBottom = region.row + region.height
        let visibleRight = region.col + visibleWidth
        let visibleBottom = region.row + visibleHeight
        let cellPixels = terminalCellPixels()
        let regionFit = image.fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain" ? "cover" : image.fit
        let signatureRows = resolvedMode == "sixel" ? max(1, visibleRows) : region.height
        let sampleSignature = sourceRaster.map
        {
            imageInfoSampleSignature(
                raster: $0,
                targetWidth: region.width,
                targetHeight: max(1, signatureRows),
                fit: regionFit,
                align: image.align,
                verticalAlign: image.verticalAlign,
                background: background
            )
        } ?? []
        return [
            "id": element.name,
            "source": image.source,
            "path": sourcePath,
            "fit": image.fit,
            "configured_render_mode": image.renderMode,
            "resolved_render_mode": resolvedMode,
            "source_loaded": sourceLoaded,
            "source_width": sourceWidth,
            "source_height": sourceHeight,
            "element_width": width,
            "element_height": height,
            "cell_pixel_width": cellPixels.width,
            "cell_pixel_height": cellPixels.height,
            "image_left": region.col,
            "image_top": region.row,
            "image_width": region.width,
            "image_height": region.height,
            "image_right": imageRight,
            "image_bottom": imageBottom,
            "visible_left": region.col,
            "visible_top": region.row,
            "visible_width": visibleWidth,
            "visible_height": visibleHeight,
            "visible_right": visibleRight,
            "visible_bottom": visibleBottom,
            "raw_expected": rawPresent,
            "raw_present": rawPresent,
            "sample_signature": sampleSignature,
            "bounds": rectJson(surfaceBounds),
            "absolute_image_left": surfaceBounds.col + region.col,
            "absolute_image_top": surfaceBounds.row + region.row,
            "absolute_image_right": surfaceBounds.col + imageRight,
            "absolute_image_bottom": surfaceBounds.row + imageBottom,
            "absolute_visible_left": surfaceBounds.col + region.col,
            "absolute_visible_top": surfaceBounds.row + region.row,
            "absolute_visible_right": surfaceBounds.col + visibleRight,
            "absolute_visible_bottom": surfaceBounds.row + visibleBottom,
        ]
    }

    private func activateElement(_ id: String?, click: Bool = false) throws -> Any
    {
        try toolPayload(for: performActivateElement(id, click: click))
    }

    private func clickElement(_ id: String?) throws -> Any
    {
        _ = renderContent()
        let element = try requireElement(id)
        guard element.enabled else
        {
            return snapshot(element, refreshLayout: false)
        }
        let elementId = id ?? runtimeElementId(element)
        let frame = element.frame
        _ = try performMousePressTarget(
            element,
            row: frame.row + frame.height / 2,
            col: frame.col + frame.width / 2
        )
        if let activeElement = window.element(named: elementId)
        {
            return snapshot(activeElement, refreshLayout: false)
        }
        return ["ok": true, "element_id": element.name]
    }

    private func performActivateElement(_ id: String?, click: Bool = false) throws -> RuntimeEventResult
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        return try performActivateElement(element, elementId: elementId, click: click)
    }

    private func performActivateElement(_ element: UIElement, elementId: String, click: Bool = false) throws -> RuntimeEventResult
    {
        guard element.enabled else
        {
            return .snapshot(element)
        }
        let nestedContainerElement = element.parentFocusHostId.flatMap { window.element(named: $0) } is ReusableElement
        let parentFocusHostId = element.parentFocusHostId
        let previousRememberedDynamicFocus = parentFocusHostId.flatMap { rememberedDynamicFocusByParentId[$0] }
        let modalDepthBeforeActivation = rootOptions.windowStack?.allFrames.count ?? 0
        let activatedWindow = window
        let activatedName = element.name
        func restoreRememberedDynamicFocusIfModalOpened()
        {
            guard !click,
                  let parentFocusHostId,
                  (rootOptions.windowStack?.allFrames.count ?? 0) > modalDepthBeforeActivation
            else
            {
                return
            }
            if let previousRememberedDynamicFocus
            {
                rememberedDynamicFocusByParentId[parentFocusHostId] = previousRememberedDynamicFocus
            }
            else
            {
                rememberedDynamicFocusByParentId.removeValue(forKey: parentFocusHostId)
            }
        }
        func restoreActiveElementAfterActivation() -> RuntimeEventResult
        {
            if window === activatedWindow,
               let activeElement = window.element(named: elementId)
            {
                focusActiveWindowElementWithScrollViewScope(activeElement)
                return .snapshot(activeElement)
            }
            return .payload(["ok": true, "element_id": activatedName])
        }
        focusActiveWindowElementWithScrollViewScope(element)
        if element.parentFocusHostId != nil && !click
        {
            ensureDynamicElementVisible(elementId)
        }
        if click && nestedContainerElement
        {
            editMode = true
            ensureDynamicElementVisible(elementId)
        }
        if let button = element as? Button
        {
            _ = button
            if !click
            {
                setEditModePreservingActiveScroll(false)
            }
            if let frame = activeFrame
            {
                let frameWindow = frame.window
                _ = handleActiveFrameButton(elementId)
                let frameRemoved = rootOptions.windowStack?.topFrame !== frame
                if frameWindow.shouldClose()
                {
                    rootOptions.windowStack?.remove(frameWindow)
                }
                if frameWindow.shouldClose() || frameRemoved
                {
                    return .payload(["ok": true, "element_id": activatedName])
                }
            }
            else
            {
                _ = dispatchGeneratedButton(element, elementId: elementId)
            }
            restoreRememberedDynamicFocusIfModalOpened()
            return restoreActiveElementAfterActivation()
        }
        else if let image = element as? Image
        {
            _ = image
            if !click
            {
                setEditModePreservingActiveScroll(false)
            }
            if let frame = activeFrame
            {
                let frameWindow = frame.window
                _ = handleActiveFrameButton(elementId)
                let frameRemoved = rootOptions.windowStack?.topFrame !== frame
                if frameWindow.shouldClose()
                {
                    rootOptions.windowStack?.remove(frameWindow)
                }
                if frameWindow.shouldClose() || frameRemoved
                {
                    return .payload(["ok": true, "element_id": activatedName])
                }
            }
            else
            {
                _ = dispatchGeneratedButton(element, elementId: elementId)
            }
            restoreRememberedDynamicFocusIfModalOpened()
            return restoreActiveElementAfterActivation()
        }
        else if let checkbox = element as? CheckBox
        {
            if !click
            {
                setEditModePreservingActiveScroll(false)
            }
            checkbox.checked.toggle()
            _ = dispatchGeneratedTextChanged(element, elementId: elementId, value: checkbox.checked ? "true" : "false")
            let result = restoreActiveElementAfterActivation()
            if !click
            {
                setEditModePreservingActiveScroll(false)
            }
            activeScrollViewEditElement = nil
            return result
        }
        else if element is ReusableElement
        {
            _ = options.onButton?(elementId)
            return restoreActiveElementAfterActivation()
        }
        else if click && (element is ComboBox || element is ListBox)
        {
            beginElementEdit(element, elementId: elementId)
        }
        return .snapshot(element)
    }

    private func scrollElement(_ arguments: [String: Any]) throws -> Any
    {
        return snapshotResult(try performScrollElement(arguments))
    }

    @discardableResult
    private func performScrollElement(_ arguments: [String: Any]) throws -> UIElement
    {
        let element = try requireElement(arguments["element_id"] as? String)
        _ = renderContent()
        let delta = intArg(arguments, "delta", 0)
        if let scrollView = element as? ScrollView
        {
            _ = scrollView.scrollLines(delta, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height))
            return scrollView
        }
        if let reusable = element as? ReusableElement
        {
            syncGeneratedReusableChildFrames(reusable, to: reusable.frame)
            if let scrollView = reusable.child?.generatedScrollView()
            {
                _ = scrollView.scrollLines(delta, viewport: scrollViewportSize(scrollView, fallback: reusable.frame))
                return reusable
            }
        }
        if let viewHost = element as? ViewHost,
           let child = viewHost.child,
           let scrollView = child.generatedScrollView()
        {
            syncGeneratedWindowElementFrames(child, to: viewHost.frame, forceFullscreenLayout: true)
            _ = scrollView.scrollLines(delta, viewport: scrollViewportSize(scrollView, fallback: viewHost.frame))
            return viewHost
        }
        let steps = max(1, abs(delta))
        let scrollDelta = delta < 0 ? 1 : -1
        if let list = element as? ListBox
        {
            for _ in 0..<steps
            {
                list.scrollBy(scrollDelta, viewportHeight: list.frame.height)
            }
            return list
        }
        if let textArea = element as? TextArea
        {
            for _ in 0..<steps
            {
                _ = textArea.scrollByRows(scrollDelta, viewportHeight: textArea.frame.height)
            }
            return textArea
        }
        if let reusable = element as? ReusableElement,
           let scrollable = reusable.child as? GeneratedScrollableContent
        {
            scrollable.scrollBy(delta, viewport: Size(width: reusable.frame.width, height: reusable.frame.height))
            return reusable
        }
        if let viewHost = element as? ViewHost,
           let scrollable = viewHost.child as? GeneratedScrollableContent
        {
            scrollable.scrollBy(delta, viewport: Size(width: viewHost.frame.width, height: viewHost.frame.height))
            return viewHost
        }
        throw RuntimeError("Element is not scrollable: \(element.name)")
    }

    private func scrollViewportSize(_ scrollView: ScrollView, fallback: Rect) -> Size
    {
        Size(
            width: max(1, scrollView.frame.width > 0 ? scrollView.frame.width : fallback.width),
            height: max(1, scrollView.frame.height > 0 ? scrollView.frame.height : fallback.height)
        )
    }

    private func performTerminalScrollAt(point: Point, delta: Int) throws
    {
        _ = renderContent()
        let content = contentPoint(["x": point.col, "y": point.row])
        let local = Point(row: content.row, col: content.col)
        if options.onMouseWheelBeforeFocused?(local, delta) == true
        {
            return
        }
        if activeScrollView == nil,
           try performTerminalGeneratedScrollHostMouseWheel(point: local, delta: delta)
        {
            return
        }
        let focusable = focusableElements(activeScrollView: activeScrollView)
        if try performTerminalMouseWheel(on: focusable, point: local, delta: delta)
        {
            return
        }
        if try performTerminalGeneratedScrollHostMouseWheel(point: local, delta: delta)
        {
            return
        }
        if try performTerminalScrollViewMouseWheel(point: local, delta: delta)
        {
            return
        }
        if let target = elementAt(window.runtimeElements(), row: local.row, col: local.col),
           options.onMouseWheel?(runtimeElementId(target), delta) == true
        {
            return
        }
    }

    @discardableResult
    private func performTerminalScrollElement(_ element: UIElement, delta: Int) throws -> Bool
    {
        if let scrollView = element as? ScrollView
        {
            return scrollView.handleWheel(delta, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height))
        }
        if let list = element as? ListBox
        {
            let scrollDelta = delta > 0 ? -abs(delta) : abs(delta)
            list.scrollBy(scrollDelta, viewportHeight: list.frame.height)
            return true
        }
        if let textArea = element as? TextArea
        {
            let scrollDelta = (delta > 0 ? -abs(delta) : abs(delta)) * kTextInputWheelScrollRows
            return textArea.scrollByRows(scrollDelta, viewportHeight: textArea.frame.height)
        }
        if let reusable = element as? ReusableElement
        {
            syncGeneratedReusableChildFrames(reusable, to: reusable.frame)
            if let scrollView = reusable.child?.generatedScrollView()
            {
                return scrollView.handleWheel(delta, viewport: scrollViewportSize(scrollView, fallback: reusable.frame))
            }
            if let scrollable = reusable.child as? GeneratedScrollableContent
            {
                let scrollDelta = delta > 0 ? -abs(delta) : abs(delta)
                scrollable.scrollBy(scrollDelta, viewport: Size(width: reusable.frame.width, height: reusable.frame.height))
                return true
            }
        }
        if let viewHost = element as? ViewHost,
           let child = viewHost.child
        {
            syncGeneratedWindowElementFrames(child, to: viewHost.frame, forceFullscreenLayout: true)
            if let scrollView = child.generatedScrollView()
            {
                return scrollView.handleWheel(delta, viewport: scrollViewportSize(scrollView, fallback: viewHost.frame))
            }
            if let scrollable = child as? GeneratedScrollableContent
            {
                let scrollDelta = delta > 0 ? -abs(delta) : abs(delta)
                scrollable.scrollBy(scrollDelta, viewport: Size(width: viewHost.frame.width, height: viewHost.frame.height))
                return true
            }
        }
        if isScrollableElement(element)
        {
            _ = try performScrollElement(["element_id": runtimeElementId(element), "delta": delta])
            return true
        }
        if let host = scrollHostFromElementIdentifier(element)
        {
            _ = try performScrollElement(["element_id": runtimeElementId(host), "delta": delta])
            return true
        }
        if let host = scrollHostContainingElement(element)
        {
            _ = try performScrollElement(["element_id": runtimeElementId(host), "delta": delta])
            return true
        }
        if let parentId = element.parentFocusHostId,
           let parent = window.element(named: parentId),
           isScrollableElement(parent)
        {
            _ = try performScrollElement(["element_id": runtimeElementId(parent), "delta": delta])
            return true
        }
        return false
    }

    private func performTerminalMouseWheel(on elements: [UIElement], point: Point, delta: Int) throws -> Bool
    {
        guard let target = elementAt(elements, row: point.row, col: point.col)
        else
        {
            return false
        }
        if try performTerminalScrollElement(target, delta: delta)
        {
            return true
        }
        if let host = scrollHostContainingElement(target)
        {
            return try performTerminalScrollElement(host, delta: delta)
        }
        return false
    }

    private func performTerminalGeneratedScrollHostMouseWheel(point: Point, delta: Int) throws -> Bool
    {
        for element in window.runtimeElements().reversed()
        {
            guard rectContains(element.frame, row: point.row, col: point.col) else
            {
                continue
            }
            if let reusable = element as? ReusableElement,
               reusable.child?.generatedScrollView() != nil
            {
                return try performTerminalScrollElement(reusable, delta: delta)
            }
            if let viewHost = element as? ViewHost,
               viewHost.child?.generatedScrollView() != nil
            {
                return try performTerminalScrollElement(viewHost, delta: delta)
            }
        }
        return false
    }

    private func performTerminalScrollViewMouseWheel(point: Point, delta: Int) throws -> Bool
    {
        if let scrollView = scrollViewAtPosition(in: window, row: point.row, col: point.col)
        {
            return scrollView.handleWheel(delta, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height))
        }
        if let target = elementAt(window.runtimeElements(), row: point.row, col: point.col),
           let scrollView = target as? ScrollView
        {
            return scrollView.handleWheel(delta, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height))
        }
        return false
    }

    private func scrollHostFromElementIdentifier(_ element: UIElement) -> UIElement?
    {
        guard let elementId = element.mcpElementId,
              let bracket = elementId.firstIndex(of: "[")
        else
        {
            return nil
        }
        let rawHost = String(elementId[..<bracket])
        for candidate in scrollHostNameCandidates(rawHost)
        {
            if let host = window.element(named: candidate),
               isScrollableElement(host)
            {
                return host
            }
        }
        return nil
    }

    private func scrollHostContainingElement(_ target: UIElement) -> UIElement?
    {
        for element in window.runtimeElements().reversed()
        {
            if element === target || !isScrollableElement(element)
            {
                continue
            }
            if let reusable = element as? ReusableElement,
               let child = reusable.child,
               ownerWindowForElement(child, target) != nil
            {
                return element
            }
            if let viewHost = element as? ViewHost,
               let child = viewHost.child,
               ownerWindowForElement(child, target) != nil
            {
                return element
            }
            if let scrollView = element as? ScrollView
            {
                for child in scrollView.children where elementTreeContains(child, target)
                {
                    return scrollView
                }
            }
        }
        return nil
    }

    private func scrollHostNameCandidates(_ rawHost: String) -> [String]
    {
        var candidates = [rawHost]
        if let dot = rawHost.lastIndex(of: ".")
        {
            candidates.append(String(rawHost[rawHost.index(after: dot)...]))
        }
        return candidates
    }

    private func isScrollableElement(_ element: UIElement) -> Bool
    {
        if element is ScrollView || element is ListBox || element is TextArea
        {
            return true
        }
        if let reusable = element as? ReusableElement,
           reusable.child?.generatedScrollView() != nil || reusable.child is GeneratedScrollableContent
        {
            return true
        }
        if let viewHost = element as? ViewHost,
           viewHost.child?.generatedScrollView() != nil || viewHost.child is GeneratedScrollableContent
        {
            return true
        }
        return false
    }

    private func contentPoint(_ arguments: [String: Any]) -> (row: Int, col: Int)
    {
        let frameSize = windowFrameSize()
        let offset = windowOffset(frameSize: frameSize)
        let content = windowContentBounds(frameSize: frameSize)
        return (
            row: intArg(arguments, "y", 0) - viewport.row - offset.row - content.row,
            col: intArg(arguments, "x", 0) - viewport.col - offset.col - content.col
        )
    }

    private func elementAt(row: Int, col: Int) -> UIElement?
    {
        window.runtimeElements().reversed().first { rectContains($0.frame, row: row, col: col) }
    }

    private func elementAt(_ elements: [UIElement], row: Int, col: Int) -> UIElement?
    {
        elements.reversed().first { rectContains(mouseHitFrame($0), row: row, col: col) }
    }

    private func isFocusableType(_ element: UIElement) -> Bool
    {
        element is Button ||
            element is CheckBox ||
            element is TextInput ||
            element is NumberInput ||
            element is ComboBox ||
            element is ListBox ||
            element is ScrollView
    }

    private func mouseHitFrame(_ element: UIElement) -> Rect
    {
        var frame = element.frame
        if element is ComboBox && !(element is ListBox)
        {
            frame.height = kComboBoxClosedRows
        }
        return frame
    }

    private func isMouseTargetType(_ element: UIElement) -> Bool
    {
        if isFocusableType(element)
        {
            return true
        }
        if let reusable = element as? ReusableElement,
           let child = reusable.child
        {
            return reusable.focusable && (child.generatedScrollView() != nil || child.generatedFocusable)
        }
        if let viewHost = element as? ViewHost,
           let child = viewHost.child
        {
            return viewHost.focusable && (child.generatedScrollView() != nil || child.generatedFocusable)
        }
        return false
    }

    private func appendFocusableElements(
        from targetWindow: GeneratedWindowBase,
        activeScrollView: ScrollView?,
        idPrefix: String?,
        parentFocusHostId: String?,
        to focusable: inout [UIElement]
    )
    {
        for element in targetWindow.elements
        {
            let elementId = idPrefix.map { "\($0).\(element.name)" } ?? element.name
            if let child = generatedWindowChild(of: element)
            {
                if let reusable = element as? ReusableElement
                {
                    syncGeneratedReusableChildFrames(reusable, to: element.frame)
                }
                else
                {
                    syncGeneratedWindowElementFrames(child, to: element.frame, forceFullscreenLayout: true)
                }

                if let generatedScrollView = child.generatedScrollView()
                {
                    element.mcpElementId = elementId
                    element.parentFocusHostId = parentFocusHostId
                    if generatedElementIsFocusable(element)
                    {
                        focusable.append(element)
                    }
                    if activeScrollView === generatedScrollView
                    {
                        var childFocusable: [UIElement] = []
                        appendFocusableElements(
                            from: child,
                            activeScrollView: activeScrollView,
                            idPrefix: elementId,
                            parentFocusHostId: elementId,
                            to: &childFocusable
                        )
                        focusable.append(contentsOf: childFocusable.filter { $0 !== generatedScrollView })
                    }
                    continue
                }

                var childFocusable: [UIElement] = []
                appendFocusableElements(
                    from: child,
                    activeScrollView: activeScrollView,
                    idPrefix: elementId,
                    parentFocusHostId: elementId,
                    to: &childFocusable
                )
                if childFocusable.isEmpty &&
                    generatedElementIsFocusable(element) &&
                    child.generatedFocusable &&
                    !(element is ViewHost)
                {
                    element.mcpElementId = elementId
                    element.parentFocusHostId = parentFocusHostId
                    focusable.append(element)
                }
                else
                {
                    focusable.append(contentsOf: childFocusable)
                }
                continue
            }

            if let scrollView = element as? ScrollView
            {
                let rootGeneratedScrollView = targetWindow.generatedScrollView() === scrollView
                let scrollElementId = rootGeneratedScrollView ? (idPrefix ?? elementId) : elementId
                scrollView.mcpElementId = scrollElementId
                scrollView.parentFocusHostId = parentFocusHostId
                if generatedElementIsFocusable(element)
                {
                    focusable.append(element)
                }
                if activeScrollView === scrollView
                {
                    let scrollFrame = scrollView.frame
                    let size = Size(width: scrollFrame.width, height: scrollFrame.height)
                    for childView in scrollView.childViews(size: size)
                    {
                        guard let child = childView.element else
                        {
                            continue
                        }
                        let indexedPrefix = "\(scrollElementId)[\(childView.index)]"
                        child.frame = Rect(
                            row: scrollFrame.row + childView.frame.row,
                            col: scrollFrame.col + childView.frame.col,
                            width: childView.frame.width,
                            height: childView.frame.height
                        )
                        if let generated = generatedWindowChild(of: child)
                        {
                            if let reusable = child as? ReusableElement
                            {
                                syncGeneratedReusableChildFrames(reusable, to: child.frame)
                            }
                            else
                            {
                                syncGeneratedWindowElementFrames(generated, to: child.frame, forceFullscreenLayout: true)
                            }
                            appendFocusableElements(
                                from: generated,
                                activeScrollView: activeScrollView,
                                idPrefix: indexedPrefix,
                                parentFocusHostId: elementId,
                                to: &focusable
                            )
                            continue
                        }
                        if generatedElementIsFocusable(child)
                        {
                            child.mcpElementId = "\(indexedPrefix).\(child.name)"
                            child.parentFocusHostId = elementId
                            focusable.append(child)
                        }
                    }
                }
                continue
            }

            if generatedElementIsFocusable(element)
            {
                element.mcpElementId = elementId
                element.parentFocusHostId = parentFocusHostId
                focusable.append(element)
            }
        }
    }

    private func focusableElements(activeScrollView: ScrollView? = nil) -> [UIElement]
    {
        focusableElements(in: window, activeScrollView: activeScrollView)
    }

    private func focusableElements(in targetWindow: GeneratedWindowBase, activeScrollView: ScrollView? = nil) -> [UIElement]
    {
        var focusable: [UIElement] = []
        appendFocusableElements(
            from: targetWindow,
            activeScrollView: activeScrollView,
            idPrefix: nil,
            parentFocusHostId: nil,
            to: &focusable
        )
        return focusable
    }

    private func focusIndex(in focusable: [UIElement], matching focusedId: String?) -> Int
    {
        guard let focusedId else
        {
            return -1
        }
        return focusable.firstIndex
        {
            runtimeElementId($0) == focusedId || $0.name == focusedId
        } ?? -1
    }

    private func focusedElement(from focusable: [UIElement]) -> UIElement?
    {
        let index = focusIndex(in: focusable, matching: focusedName)
        guard index >= 0 && index < focusable.count else
        {
            return nil
        }
        return focusable[index]
    }

    private func focusIdentityFor(targetWindow: GeneratedWindowBase, focusable: [UIElement], focusedName: String?) -> FocusIdentity
    {
        let index = focusIndex(in: focusable, matching: focusedName)
        guard index >= 0 && index < focusable.count else
        {
            return FocusIdentity(element: nil, elementIdentity: nil)
        }
        let element = focusable[index]
        return FocusIdentity(element: element, elementIdentity: ObjectIdentifier(element))
    }

    private func focusIdentityPresent(
        in targetWindow: GeneratedWindowBase,
        activeScrollView: ScrollView?,
        previous: FocusIdentity
    ) -> Bool
    {
        guard let previousElement = previous.element,
              let previousIdentity = previous.elementIdentity
        else
        {
            return true
        }
        return focusableElements(in: targetWindow, activeScrollView: activeScrollView).contains
        {
            $0 === previousElement && ObjectIdentifier($0) == previousIdentity
        }
    }

    private func setEditModePreservingActiveScroll(_ newValue: Bool)
    {
        if !newValue
        {
            activeDynamicEditName = nil
            activeScrollViewEditElement = nil
        }
        if let frame = activeFrame
        {
            frame.editMode = newValue
        }
        else
        {
            rootEditMode = newValue
        }
    }

    private func setFocusedElement(_ element: UIElement?)
    {
        guard let element else
        {
            focusedName = nil
            return
        }
        let elementId: String
        if let scrollView = activeScrollView,
           let scopedElement = focusableElements(activeScrollView: scrollView).first(where: { $0 === element })
        {
            elementId = runtimeElementId(scopedElement)
        }
        else
        {
            elementId = runtimeElementId(element)
        }
        focusedName = elementId
        rememberParentDynamicFocus(elementId, element)
        if let scrollView = activeScrollView
        {
            let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView)
            if element !== scrollView && (scopeRoot == nil || element !== scopeRoot!)
            {
                rememberScrollViewDescendant(scrollView, element)
                _ = ensureElementFrameVisibleInScrollView(scrollView, element: element)
            }
        }
    }

    private func focusActiveWindowElement(_ element: UIElement)
    {
        let targetWindow = window
        targetWindow.suppressActiveContainerScopeVisuals = false
        let activeScrollViewBelongsToWindow =
            activeScrollViewRepresentedInCurrentLayout(targetWindow, activeScrollView)
        let previousFocusable = focusableElements(
            in: targetWindow,
            activeScrollView: activeScrollViewBelongsToWindow ? activeScrollView : nil
        )
        let previous = focusedElement(from: previousFocusable)
        let containingScrollView = scrollViewContainingElement(in: targetWindow, element)
        if activeScrollViewBelongsToWindow && editMode
        {
            if let activeScrollViewEditElement
            {
                commitEdit(activeScrollViewEditElement)
            }
            editSnapshot = nil
            activeScrollViewEditElement = nil
            setEditModePreservingActiveScroll(false)
        }
        let activeScrollViewProxy = activeScrollViewBelongsToWindow
            ? generatedScrollViewProxy(in: targetWindow, for: activeScrollView)
            : nil
        if activeScrollViewBelongsToWindow &&
            (element === activeScrollView || (activeScrollViewProxy != nil && element === activeScrollViewProxy!))
        {
            activeScrollView = nil
            activeScrollViewEditElement = nil
            setEditModePreservingActiveScroll(false)
        }
        if activeScrollViewBelongsToWindow &&
            element !== activeScrollView &&
            containingScrollView !== activeScrollView
        {
            activeScrollView = nil
            activeScrollViewEditElement = nil
            setEditModePreservingActiveScroll(false)
        }
        if let containingScrollView
        {
            activeScrollView = containingScrollView
            activeScrollViewEditElement = nil
            setEditModePreservingActiveScroll(false)
        }

        let targetScrollViewBelongsToWindow =
            activeScrollViewRepresentedInCurrentLayout(targetWindow, activeScrollView)
        let focusable = focusableElements(
            in: targetWindow,
            activeScrollView: targetScrollViewBelongsToWindow ? activeScrollView : nil
        )
        guard focusable.contains(where: { $0 === element }) else
        {
            if previous != nil
            {
                setFocusedElement(nil)
                setEditModePreservingActiveScroll(false)
            }
            return
        }
        setFocusedElement(element)
        if let scrollView = scrollViewContainingElement(in: targetWindow, element)
        {
            _ = ensureElementFrameVisibleInScrollView(scrollView, element: element)
        }
    }

    private func focusActiveWindowElementWithScrollViewScope(_ element: UIElement)
    {
        window.suppressActiveContainerScopeVisuals = false
        if let scrollView = scrollViewContainingElement(element)
        {
            activeScrollView = scrollView
            activeScrollViewEditElement = nil
            setEditModePreservingActiveScroll(true)
            setFocusedElement(element)
            rememberScrollViewDescendant(scrollView, element)
            _ = ensureElementFrameVisibleInScrollView(scrollView, element: element)
            return
        }

        focusActiveWindowElement(element)
    }

    private func generatedScrollViewProxy(in targetWindow: GeneratedWindowBase, for target: ScrollView?) -> UIElement?
    {
        guard let target else
        {
            return nil
        }
        for element in targetWindow.elements
        {
            if let child = generatedWindowChild(of: element)
            {
                if child.generatedScrollView() === target
                {
                    return element
                }
                if let proxy = generatedScrollViewProxy(in: child, for: target)
                {
                    return proxy
                }
            }
            guard let scrollView = element as? ScrollView else
            {
                continue
            }
            for child in scrollView.children
            {
                guard let generated = generatedWindowChild(of: child) else
                {
                    continue
                }
                if generated.generatedScrollView() === target
                {
                    return child
                }
                if let proxy = generatedScrollViewProxy(in: generated, for: target)
                {
                    return proxy
                }
            }
        }
        return nil
    }

    private func scrollViewContainingElement(_ target: UIElement?) -> ScrollView?
    {
        scrollViewContainingElement(in: window, target)
    }

    private func scrollViewContainingElement(in targetWindow: GeneratedWindowBase, _ target: UIElement?) -> ScrollView?
    {
        guard let target else
        {
            return nil
        }
        for element in targetWindow.elements
        {
            if let child = generatedWindowChild(of: element),
               let scrollView = scrollViewContainingElement(in: child, target)
            {
                return scrollView
            }
            guard let scrollView = element as? ScrollView else
            {
                continue
            }
            for child in scrollView.children
            {
                if child === target
                {
                    return scrollView
                }
                if let generated = generatedWindowChild(of: child),
                   ownerWindowForElement(generated, target) != nil
                {
                    return scrollView
                }
                if let nestedScrollView = child as? ScrollView
                {
                    for nestedChild in nestedScrollView.children where nestedChild === target
                    {
                        return nestedScrollView
                    }
                }
            }
        }
        return nil
    }

    private func scrollViewScopeElements(_ scrollView: ScrollView) -> [UIElement]
    {
        let activeFocusable = focusableElements(activeScrollView: scrollView)
        let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
        guard let scrollIndex = activeFocusable.firstIndex(where: { $0 === scopeRoot }) else
        {
            return []
        }
        let navigationFocusable = focusableElements(activeScrollView: nil)
        let navigationIndex = navigationFocusable.firstIndex(where: { $0 === scopeRoot }) ?? -1
        let nextNavigationElement = navigationIndex >= 0 && navigationIndex + 1 < navigationFocusable.count
            ? navigationFocusable[navigationIndex + 1]
            : nil
        var scoped: [UIElement] = []
        for index in scrollIndex..<activeFocusable.count
        {
            let element = activeFocusable[index]
            if index != scrollIndex,
               let nextNavigationElement,
               element === nextNavigationElement
            {
                break
            }
            scoped.append(element)
        }
        return scoped
    }

    private func rememberScrollViewDescendant(_ scrollView: ScrollView?, _ descendant: UIElement?)
    {
        guard let scrollView,
              let descendant
        else
        {
            return
        }
        let key = ObjectIdentifier(scrollView)
        let remembered = RememberedScrollViewDescendant(
            element: descendant,
            scrollViewIdentity: key,
            elementIdentity: ObjectIdentifier(descendant)
        )
        if let frame = activeFrame
        {
            frame.scrollViewLastDescendantByScrollId[key] = remembered
        }
        else
        {
            rootScrollViewLastDescendantByScrollId[key] = remembered
        }
    }

    private func rememberedScrollViewDescendant(for scrollView: ScrollView, scoped: [UIElement]) -> UIElement?
    {
        let key = ObjectIdentifier(scrollView)
        let remembered = activeFrame?.scrollViewLastDescendantByScrollId[key] ?? rootScrollViewLastDescendantByScrollId[key]
        guard let remembered else
        {
            return nil
        }
        guard remembered.scrollViewIdentity == key,
              ObjectIdentifier(remembered.element) == remembered.elementIdentity,
              scoped.contains(where: { $0 === remembered.element })
        else
        {
            if let frame = activeFrame
            {
                frame.scrollViewLastDescendantByScrollId.removeValue(forKey: key)
            }
            else
            {
                rootScrollViewLastDescendantByScrollId.removeValue(forKey: key)
            }
            return nil
        }
        return remembered.element
    }

    private func firstFocusableDescendant(in scrollView: ScrollView) -> UIElement?
    {
        let scoped = scrollViewScopeElements(scrollView)
        let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
        if let remembered = rememberedScrollViewDescendant(for: scrollView, scoped: scoped)
        {
            return remembered
        }
        if let visible = scoped.first(where: {
            $0 !== scrollView &&
                $0 !== scopeRoot &&
                elementVisibleInScrollView(scrollView, element: $0)
        })
        {
            return visible
        }
        return scoped.first { $0 !== scrollView && $0 !== scopeRoot }
    }

    private func moveFocusSpatialIndex(_ focusedIndex: inout Int, focusable: [UIElement], key: String)
    {
        guard !focusable.isEmpty else
        {
            focusedIndex = -1
            return
        }
        guard focusedIndex >= 0 && focusedIndex < focusable.count else
        {
            focusedIndex = 0
            return
        }

        let current = focusable[focusedIndex].frame
        let currentRow = centerRow(current)
        let currentCol = centerCol(current)
        var best: (index: Int, band: Int, primary: Int, gap: Int, edge: Int)?
        for index in focusable.indices where index != focusedIndex
        {
            let candidate = focusable[index].frame
            let primary: Int
            let gap: Int
            let edge: Int
            let band: Int?
            if key == "Up" && centerRow(candidate) < currentRow
            {
                guard candidate.row + candidate.height <= current.row else
                {
                    continue
                }
                primary = max(0, current.row - (candidate.row + candidate.height))
                band = bandScanRank(
                    currentStart: current.col,
                    currentEnd: current.col + current.width,
                    candidateStart: candidate.col,
                    candidateEnd: candidate.col + candidate.width
                )
                gap = axisGap(
                    startA: current.col,
                    endA: current.col + current.width,
                    startB: candidate.col,
                    endB: candidate.col + candidate.width
                )
                edge = abs(candidate.col - current.col)
            }
            else if key == "Down" && centerRow(candidate) > currentRow
            {
                guard candidate.row >= current.row + current.height else
                {
                    continue
                }
                primary = max(0, candidate.row - (current.row + current.height))
                band = bandScanRank(
                    currentStart: current.col,
                    currentEnd: current.col + current.width,
                    candidateStart: candidate.col,
                    candidateEnd: candidate.col + candidate.width
                )
                gap = axisGap(
                    startA: current.col,
                    endA: current.col + current.width,
                    startB: candidate.col,
                    endB: candidate.col + candidate.width
                )
                edge = abs(candidate.col - current.col)
            }
            else if key == "Right" && centerCol(candidate) > currentCol
            {
                guard candidate.col >= current.col + current.width else
                {
                    continue
                }
                primary = max(0, candidate.col - (current.col + current.width))
                band = bandScanRank(
                    currentStart: current.row,
                    currentEnd: current.row + current.height,
                    candidateStart: candidate.row,
                    candidateEnd: candidate.row + candidate.height
                )
                gap = axisGap(
                    startA: current.row,
                    endA: current.row + current.height,
                    startB: candidate.row,
                    endB: candidate.row + candidate.height
                )
                edge = abs(candidate.row - current.row)
            }
            else if key == "Left" && centerCol(candidate) < currentCol
            {
                guard candidate.col + candidate.width <= current.col else
                {
                    continue
                }
                primary = max(0, current.col - (candidate.col + candidate.width))
                band = bandScanRank(
                    currentStart: current.row,
                    currentEnd: current.row + current.height,
                    candidateStart: candidate.row,
                    candidateEnd: candidate.row + candidate.height
                )
                gap = axisGap(
                    startA: current.row,
                    endA: current.row + current.height,
                    startB: candidate.row,
                    endB: candidate.row + candidate.height
                )
                edge = abs(candidate.row - current.row)
            }
            else
            {
                continue
            }
            guard let band else
            {
                continue
            }
            if best == nil ||
                spatialScoreIsBetter(
                    candidate: (band, primary, gap, edge),
                    best: (best!.band, best!.primary, best!.gap, best!.edge)
                )
            {
                best = (index, band, primary, gap, edge)
            }
        }

        if let best
        {
            focusedIndex = best.index
        }
    }

    private func scrollViewAtPosition(in targetWindow: GeneratedWindowBase, row: Int, col: Int) -> ScrollView?
    {
        for element in targetWindow.elements.reversed()
        {
            if let child = generatedWindowChild(of: element)
            {
                let frame = element.frame
                if frame.width > 0 && frame.height > 0
                {
                    if let reusable = element as? ReusableElement
                    {
                        syncGeneratedReusableChildFrames(reusable, to: frame)
                    }
                    else
                    {
                        syncGeneratedWindowElementFrames(child, to: frame, forceFullscreenLayout: true)
                    }
                }
                if let nested = scrollViewAtPosition(in: child, row: row, col: col)
                {
                    return nested
                }
            }
            if let scrollView = element as? ScrollView,
               rectContains(scrollView.frame, row: row, col: col)
            {
                return scrollView
            }
        }
        return nil
    }

    private func appendMouseTargetElements(
        from targetWindow: GeneratedWindowBase,
        activeScrollView: ScrollView?,
        idPrefix: String?,
        parentFocusHostId: String?,
        to targets: inout [UIElement]
    )
    {
        for element in targetWindow.elements
        {
            let elementId = idPrefix.map { "\($0).\(element.name)" } ?? element.name
            if let child = generatedWindowChild(of: element)
            {
                if let reusable = element as? ReusableElement
                {
                    syncGeneratedReusableChildFrames(reusable, to: element.frame)
                }
                else
                {
                    syncGeneratedWindowElementFrames(child, to: element.frame, forceFullscreenLayout: true)
                }

                if let generatedScrollView = child.generatedScrollView()
                {
                    element.mcpElementId = elementId
                    element.parentFocusHostId = parentFocusHostId
                    if element.enabled && element.focusable
                    {
                        targets.append(element)
                    }
                    if activeScrollView === generatedScrollView
                    {
                        var childTargets: [UIElement] = []
                        appendMouseTargetElements(
                            from: child,
                            activeScrollView: activeScrollView,
                            idPrefix: elementId,
                            parentFocusHostId: elementId,
                            to: &childTargets
                        )
                        targets.append(contentsOf: childTargets.filter { $0 !== generatedScrollView })
                    }
                    continue
                }

                var childTargets: [UIElement] = []
                appendMouseTargetElements(
                    from: child,
                    activeScrollView: activeScrollView,
                    idPrefix: elementId,
                    parentFocusHostId: elementId,
                    to: &childTargets
                )
                if childTargets.isEmpty &&
                    element.focusable &&
                    child.generatedFocusable &&
                    !(element is ViewHost)
                {
                    element.mcpElementId = elementId
                    element.parentFocusHostId = parentFocusHostId
                    targets.append(element)
                }
                else
                {
                    targets.append(contentsOf: childTargets)
                }
                continue
            }

            if let scrollView = element as? ScrollView
            {
                let rootGeneratedScrollView = targetWindow.generatedScrollView() === scrollView
                let scrollElementId = rootGeneratedScrollView ? (idPrefix ?? elementId) : elementId
                scrollView.mcpElementId = scrollElementId
                scrollView.parentFocusHostId = parentFocusHostId
                if isFocusableType(element)
                {
                    targets.append(element)
                }
                if activeScrollView === scrollView
                {
                    let scrollFrame = scrollView.frame
                    let size = Size(width: scrollFrame.width, height: scrollFrame.height)
                    for childView in scrollView.childViews(size: size)
                    {
                        guard let child = childView.element else
                        {
                            continue
                        }
                        let indexedPrefix = "\(scrollElementId)[\(childView.index)]"
                        child.frame = Rect(
                            row: scrollFrame.row + childView.frame.row,
                            col: scrollFrame.col + childView.frame.col,
                            width: childView.frame.width,
                            height: childView.frame.height
                        )
                        if let generated = generatedWindowChild(of: child)
                        {
                            if let reusable = child as? ReusableElement
                            {
                                syncGeneratedReusableChildFrames(reusable, to: child.frame)
                            }
                            else
                            {
                                syncGeneratedWindowElementFrames(generated, to: child.frame, forceFullscreenLayout: true)
                            }
                            appendMouseTargetElements(
                                from: generated,
                                activeScrollView: activeScrollView,
                                idPrefix: indexedPrefix,
                                parentFocusHostId: elementId,
                                to: &targets
                            )
                            continue
                        }
                        if isFocusableType(child)
                        {
                            child.mcpElementId = "\(indexedPrefix).\(child.name)"
                            child.parentFocusHostId = elementId
                            targets.append(child)
                        }
                    }
                }
                continue
            }

            if isFocusableType(element)
            {
                element.mcpElementId = elementId
                element.parentFocusHostId = parentFocusHostId
                targets.append(element)
            }
        }
    }

    private func mouseTargetElements(activeScrollView: ScrollView?) -> [UIElement]
    {
        var targets: [UIElement] = []
        appendMouseTargetElements(
            from: window,
            activeScrollView: activeScrollView,
            idPrefix: nil,
            parentFocusHostId: nil,
            to: &targets
        )
        return targets
    }

    private func mouseTargetElement(
        from targets: [UIElement],
        row: Int,
        col: Int
    ) -> UIElement?
    {
        if editMode,
           let focusedName,
           let focused = window.element(named: focusedName) as? ComboBox,
           !(focused is ListBox)
        {
            let dropdown = comboBoxDropdownFrame(focused)
            if rectContains(dropdown, row: row, col: col)
            {
                return focused
            }
        }
        return targets.reversed().first { rectContains(mouseHitFrame($0), row: row, col: col) }
    }

    private func mouseScrollViewForPointer(row: Int, col: Int) -> ScrollView?
    {
        if let scrollView = activeScrollView,
           rectContains(scrollView.frame, row: row, col: col)
        {
            return scrollView
        }
        return scrollViewAtPosition(in: window, row: row, col: col)
    }

    private func mouseTargetElementAt(row: Int, col: Int) -> UIElement?
    {
        let activeScrollView = mouseScrollViewForPointer(row: row, col: col)
        return mouseTargetElement(
            from: mouseTargetElements(activeScrollView: activeScrollView),
            row: row,
            col: col
        )
    }

    private func mousePointerEvent(_ arguments: [String: Any], pressed: Bool) throws -> Any
    {
        try performMousePointerEvent(arguments, pressed: pressed)
        return try stateResult()
    }

    private func performMousePointerEvent(_ arguments: [String: Any], pressed: Bool, refreshLayout: Bool = true) throws
    {
        if refreshLayout
        {
            _ = renderContent()
        }
        let point = contentPoint(arguments)
        let clickedElement = mouseTargetElementAt(row: point.row, col: point.col)
        if pressed
        {
            mousePressActivatedClickControl = false
            if options.onMousePressBeforeFocused?(Point(row: point.row, col: point.col)) == true
            {
                return
            }
            if try performMousePressTarget(clickedElement, row: point.row, col: point.col)
            {
                mousePressActivatedClickControl = true
                pendingMcpMouseDown = nil
                return
            }
            return
        }
    }

    private func commitActiveEditBeforeMouseTarget(_ target: UIElement?)
    {
        let scrollViewScopeActive = editMode && activeScrollView != nil
        if scrollViewScopeActive,
           let edited = activeScrollViewEditElement
        {
            let sameTarget = target.map { edited === $0 } ?? false
            if !sameTarget
            {
                commitEdit(edited)
                activeScrollViewEditElement = nil
                editSnapshot = nil
            }
        }
        if editMode,
           !scrollViewScopeActive,
           let currentFocusedName = focusedName,
           let current = window.element(named: currentFocusedName)
        {
            let sameTarget = target.map { current === $0 } ?? false
            if !sameTarget
            {
                commitEdit(current)
                editSnapshot = nil
                editMode = false
            }
        }
    }

    private func performTerminalMouseSelection(row: Int, col: Int, release: Bool) -> Bool
    {
        guard let textInput = terminalMouseSelectionElement else
        {
            return false
        }
        let content = contentPoint(["x": col, "y": row])
        let frame = terminalMouseSelectionFrame ?? textInput.frame
        let cursor = textInput.cursorForPoint(
            row: content.row - frame.row,
            col: content.col - frame.col,
            size: Size(width: frame.width, height: frame.height)
        )
        textInput.selectRange(start: terminalMouseSelectionAnchor, end: cursor)
        if release
        {
            let selected = textInput.selectedText()
            if !selected.isEmpty
            {
                _ = copyTextToClipboard(selected)
                requestCopyNotification()
            }
            terminalMouseSelectionElement = nil
            terminalMouseSelectionFrame = nil
            terminalMouseSelectionAnchor = 0
        }
        return true
    }

    private func performMousePressTarget(_ element: UIElement?, row: Int, col: Int) throws -> Bool
    {
        commitActiveEditBeforeMouseTarget(element)
        terminalMouseSelectionElement = nil
        terminalMouseSelectionFrame = nil
        terminalMouseSelectionAnchor = 0
        guard let element else
        {
            focusedName = nil
            editMode = false
            activeScrollViewEditElement = nil
            return true
        }
        guard element.enabled else
        {
            return false
        }
        let hitFrame = element.frame

        let mouseScrollView = mouseScrollViewForPointer(row: row, col: col)
        if let mouseScrollView,
           element !== mouseScrollView
        {
            activeScrollView = mouseScrollView
            setEditModePreservingActiveScroll(true)
            setFocusedElement(element)
        }
        else
        {
            focusActiveWindowElement(element)
        }

        let elementId = runtimeElementId(element)
        let localRow = row - hitFrame.row
        let localCol = col - hitFrame.col

        if element is Button || element is CheckBox || element is Image || element is ReusableElement
        {
            _ = try performActivateElement(element, elementId: elementId, click: true)
            return true
        }
        if let numberInput = element as? NumberInput
        {
            editSnapshot = captureSnapshot(numberInput)
            beginElementEdit(numberInput, elementId: elementId)
            numberInput.setEditCursor(localCol, preserveReplaceOnFirstTextInput: numberInput.numberValue == 0.0)
            return true
        }
        if let textInput = element as? TextInput
        {
            editSnapshot = captureSnapshot(textInput)
            beginElementEdit(textInput, elementId: elementId)
            let cursor = textInput.cursorForPoint(
                row: localRow,
                col: localCol,
                size: Size(width: element.frame.width, height: element.frame.height)
            )
            textInput.selectRange(start: cursor, end: cursor)
            terminalMouseSelectionElement = textInput
            terminalMouseSelectionFrame = element.frame
            terminalMouseSelectionAnchor = cursor
            return true
        }
        if let listBox = element as? ListBox
        {
            guard !listBox.options.isEmpty else
            {
                return false
            }
            let optionIndex = listBox.scrollOffsetValue() + localRow
            guard optionIndex >= 0 && optionIndex < listBox.options.count else
            {
                return false
            }
            let previous = listBox.valueForSnapshot
            if listBox.multiple
            {
                let value = listBox.options[optionIndex]
                if listBox.selectedValues.contains(value)
                {
                    listBox.selectedValues = listBox.selectedValues.filter { $0 != value }
                }
                else
                {
                    listBox.selectedValues.append(value)
                }
                listBox.setActiveIndex(optionIndex)
            }
            else
            {
                listBox.setSelectedIndex(optionIndex)
            }
            listBox.setActiveIndex(optionIndex)
            listBox.hideActiveItem()
            editSnapshot = captureSnapshot(listBox)
            editMode = true
            if activeScrollView != nil
            {
                activeScrollViewEditElement = listBox
            }
            notifyValueChange(listBox, before: previous)
            return true
        }
        if let comboBox = element as? ComboBox
        {
            let editingInScrollScope = activeScrollView != nil && activeScrollViewEditElement === element
            let comboEditMode = activeScrollView != nil ? editingInScrollScope : (editMode && focusedName == elementId)
            if comboEditMode && localRow > 0
            {
                let previous = comboBox.valueForSnapshot
                comboBox.selectedIndex = comboBoxOptionIndex(forLocalRow: localRow, optionCount: comboBox.options.count)
                commitEdit(comboBox)
                editSnapshot = nil
                if activeScrollView != nil
                {
                    activeScrollViewEditElement = nil
                    exitActiveScrollViewScope()
                }
                else
                {
                    editMode = false
                }
                notifyValueChange(comboBox, before: previous)
                return true
            }
            if comboEditMode
            {
                commitEdit(comboBox)
                editSnapshot = nil
                if activeScrollView != nil
                {
                    activeScrollViewEditElement = nil
                }
                else
                {
                    editMode = false
                }
                return true
            }
            beginElementEdit(comboBox, elementId: elementId)
            return true
        }
        return false
    }

    private func mouseClick(_ arguments: [String: Any]) throws -> Any
    {
        try toolPayload(for: performMouseClick(arguments))
    }

    private func performMouseClick(_ arguments: [String: Any], refreshLayout: Bool = true) throws -> RuntimeEventResult
    {
        try performMousePointerEvent(arguments, pressed: true, refreshLayout: refreshLayout)
        _ = performTerminalMouseSelection(
            row: intArg(arguments, "y", 0),
            col: intArg(arguments, "x", 0),
            release: true
        )
        return .state
    }

    private func captureSnapshot(_ element: UIElement?) -> EditSnapshot?
    {
        guard let element else
        {
            return nil
        }
        var snapshot = EditSnapshot(element: element)
        if let numberInput = element as? NumberInput
        {
            snapshot.numberValue = numberInput.numberValue
            numberInput.beginEdit()
        }
        else if let textInput = element as? TextInput
        {
            snapshot.textValue = textInput.value
            snapshot.textCursor = textInput.cursor
            if textInput.hasSelection
            {
                snapshot.textSelectionAnchor = textInput.selectionStart
            }
        }
        else if let comboBox = element as? ComboBox
        {
            snapshot.selectedIndex = comboBox.selectedIndex
        }
        else if let listBox = element as? ListBox
        {
            snapshot.selectedIndex = listBox.selectedIndex
            snapshot.selectedValues = listBox.selectedValues
        }
        return snapshot
    }

    private func beginElementEdit(_ element: UIElement)
    {
        if let numberInput = element as? NumberInput
        {
            numberInput.beginEdit()
        }
        else if let textInput = element as? TextInput
        {
            textInput.cursor = textInput.value.count
            textInput.clearSelection()
        }
        else if let listBox = element as? ListBox
        {
            listBox.hideActiveItem()
        }
    }

    private func restoreSnapshot(_ snapshot: EditSnapshot)
    {
        guard let element = snapshot.element else
        {
            return
        }
        if let numberInput = element as? NumberInput
        {
            numberInput.cancelEdit()
            numberInput.setValue(snapshot.numberValue)
        }
        else if let textInput = element as? TextInput
        {
            textInput.setValue(snapshot.textValue)
            let cursor = max(0, min(snapshot.textCursor, textInput.value.count))
            textInput.selectRange(start: snapshot.textSelectionAnchor ?? cursor, end: cursor)
        }
        else if let comboBox = element as? ComboBox
        {
            comboBox.selectedIndex = max(0, min(snapshot.selectedIndex, comboBox.options.count - 1))
        }
        else if let listBox = element as? ListBox
        {
            listBox.setSelectedValues(snapshot.selectedValues)
            if snapshot.selectedValues.isEmpty
            {
                listBox.setSelectedIndex(snapshot.selectedIndex)
            }
        }
    }

    private func commitEdit(_ element: UIElement?)
    {
        if let numberInput = element as? NumberInput
        {
            numberInput.commitEdit()
        }
    }

    private func escapeElementEdit(_ element: UIElement?)
    {
        if usesLeaveCommit(element)
        {
            commitEdit(element)
            editSnapshot = nil
            editMode = false
            return
        }
        if let snapshot = editSnapshot
        {
            restoreSnapshot(snapshot)
        }
        editSnapshot = nil
        editMode = false
    }

    private func beginElementEdit(_ element: UIElement, elementId: String)
    {
        editSnapshot = captureSnapshot(element)
        beginElementEdit(element)
        editMode = true
        activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        if let scrollView = activeScrollView,
           scrollViewContainingElement(element) === scrollView
        {
            activeScrollViewEditElement = element
        }
        _ = options.onEditStarted?(elementId)
    }

    private func pressKey(_ key: String) throws -> Any
    {
        if key != "Escape"
        {
            window.suppressActiveContainerScopeVisuals = false
        }
        try handleTerminalKeyInput(key)
        return try stateResult()
    }

    private func focusedElementForClipboard() -> UIElement?
    {
        if let activeScrollViewEditElement
        {
            return activeScrollViewEditElement
        }
        guard let currentFocusedName = focusedName else
        {
            return nil
        }
        return window.element(named: currentFocusedName)
    }

    private func copyFocusedText(_ element: UIElement?) -> Bool
    {
        guard let element else
        {
            return false
        }
        if let numberInput = element as? NumberInput
        {
            let text = formatNumber(numberInput.numberValue)
            return !text.isEmpty && copyTextToClipboard(text)
        }
        if let input = element as? TextInput
        {
            let selected = input.selectedText()
            let text = selected.isEmpty ? input.value : selected
            return !text.isEmpty && copyTextToClipboard(text)
        }
        return false
    }

    private func requestCopyNotification()
    {
        copyNotificationRequested = true
    }

    private func takeCopyNotificationRequested() -> Bool
    {
        let requested = copyNotificationRequested
        copyNotificationRequested = false
        return requested
    }

    private func handleClipboardKey(_ key: String) -> Bool
    {
        guard key == "cmd_c" || key == "cmd_v" else
        {
            return false
        }
        guard let element = focusedElementForClipboard() else
        {
            return true
        }
        if key == "cmd_c"
        {
            if copyFocusedText(element)
            {
                requestCopyNotification()
            }
            return true
        }
        let elementId = runtimeElementId(element)
        if key == "cmd_v", !editMode, element is TextInput
        {
            beginElementEdit(element, elementId: elementId)
            _ = options.onEditStarted?(elementId)
        }
        let before = element.valueForSnapshot
        if element.handleKey(key)
        {
            notifyValueChange(element, before: before)
        }
        return true
    }

    private func handleTerminalKeyInput(_ key: String, refreshLayoutForNavigation: Bool = true) throws
    {
        if editMode && key == "Escape"
        {
            if activeScrollView != nil
            {
                _ = try handleActiveScrollViewKey(key)
                return
            }
            guard let currentFocusedName = focusedName,
                  let element = window.element(named: currentFocusedName)
            else
            {
                editMode = false
                return
            }
            if activeDynamicEditName == currentFocusedName
            {
                restoreDynamicScopeAfterElementEdit(element)
                return
            }
            if ownsGeneratedScrollScope(element)
            {
                editMode = false
                activeDynamicEditName = nil
                return
            }
            if let parentId = element.parentFocusHostId
            {
                if let parent = window.element(named: parentId)
                {
                    rememberDynamicFocus(parentId: parentId, parent: parent, elementId: currentFocusedName)
                }
                focusedName = parentId
                editMode = false
                activeDynamicEditName = nil
                return
            }
            escapeElementEdit(element)
            return
        }
        if !editMode && key == "Escape" && activeScrollView != nil
        {
            exitActiveScrollViewScope()
            return
        }
        if key == "Escape", handleStandardEscapeButton()
        {
            return
        }
        if handleClipboardKey(key)
        {
            return
        }
        if let focusedName,
           options.onKeyBeforeFocusedElement?(key, focusedName, editMode) == true
        {
            return
        }
        if options.onKeyBeforeFocused?(key) == true
        {
            return
        }
        if activeScrollView != nil &&
            (editMode || ["Up", "Down", "Left", "Right", "Enter", " "].contains(key))
        {
            if try handleActiveScrollViewKey(key)
            {
                return
            }
        }
        if let currentFocusedName = focusedName,
           let element = window.element(named: currentFocusedName),
           dynamicScopeNavigationActive(for: element)
        {
            if ["Up", "Down", "Left", "Right"].contains(key)
            {
                _ = moveDynamicFocus(from: currentFocusedName, key: key)
                return
            }
            if key == "Enter" || key == " "
            {
                if element is Button || element is ReusableElement
                {
                    _ = try performActivateElement(currentFocusedName)
                    editMode = true
                    activeDynamicEditName = dynamicScrollScopeHostId(for: element)
                    return
                }
                if element is CheckBox
                {
                    let before = element.valueForSnapshot
                    _ = element.handleKey(key)
                    notifyValueChange(element, before: before)
                    activeDynamicEditName = dynamicScrollScopeHostId(for: element)
                    return
                }
                if element is TextInput || element is ComboBox || element is ListBox
                {
                    beginElementEdit(element, elementId: currentFocusedName)
                    return
                }
            }
            return
        }
        if editMode,
           key == "Tab",
           options.onKey?(key) == true
        {
            return
        }
        if let currentFocusedName = focusedName,
           let element = window.element(named: currentFocusedName)
        {
            if editMode && ownsGeneratedScrollScope(element) && ["Up", "Down", "PageUp", "PageDown", "Home", "End"].contains(key)
            {
                if handleGeneratedScrollScopeKey(element, key: key)
                {
                    return
                }
            }
            if editMode && key == "Enter", let listBox = element as? ListBox
            {
                let before = element.valueForSnapshot
                if element.handleKey(key)
                {
                    notifyValueChange(element, before: before)
                }
                if listBox.multiple
                {
                    editSnapshot = nil
                    return
                }
                _ = options.onTextConfirmed?(currentFocusedName, listBox.selectedValues.first ?? "")
                commitEdit(element)
                editSnapshot = nil
                if element.parentFocusHostId != nil
                {
                    restoreDynamicScopeAfterElementEdit(element)
                }
                else
                {
                    editMode = false
                }
                return
            }
            if editMode && key == "Enter" && usesLeaveCommit(element)
            {
                let before = element.valueForSnapshot
                if element.handleKey(key)
                {
                    notifyValueChange(element, before: before)
                }
                editSnapshot = nil
                return
            }
            if editMode && key == "Enter"
            {
                let before = element.valueForSnapshot
                _ = element.handleKey(key)
                notifyValueChange(element, before: before)
                if let input = element as? TextInput
                {
                    _ = options.onTextConfirmed?(currentFocusedName, input.value)
                }
                commitEdit(element)
                editSnapshot = nil
                if element.parentFocusHostId != nil
                {
                    restoreDynamicScopeAfterElementEdit(element)
                }
                else
                {
                    editMode = false
                }
                return
            }
            if editMode
            {
                let before = element.valueForSnapshot
                if editMode && (element is Button || element is CheckBox) && moveDynamicFocus(from: currentFocusedName, key: key)
                {
                    return
                }
                if element.handleKey(key)
                {
                    notifyValueChange(element, before: before)
                    if key == "Enter", element is ComboBox
                    {
                        if element.parentFocusHostId != nil
                        {
                            restoreDynamicScopeAfterElementEdit(element)
                        }
                        else
                        {
                            editMode = false
                        }
                    }
                }
                return
            }
        }
        if options.onKey?(key) == true
        {
            return
        }
        if !editMode && key == "Tab"
        {
            moveFocus(1)
            return
        }
        if !editMode && key == "Shift+Tab"
        {
            moveFocus(-1)
            return
        }
        if !editMode && ["Up", "Down", "Left", "Right"].contains(key)
        {
            if refreshLayoutForNavigation
            {
                _ = renderContent()
            }
            if moveSpatialFocus(key)
            {
                return
            }
        }
        guard let currentFocusedName = focusedName, let element = window.element(named: currentFocusedName) else
        {
            return
        }
        if !editMode && (key == "Enter" || key == " ")
        {
            if ownsGeneratedScrollScope(element)
            {
                if enterActiveScrollViewScope(from: element)
                {
                    _ = options.onEditStarted?(currentFocusedName)
                }
                return
            }
            if enterChildFocusScope(element)
            {
                return
            }
            if element is Button || element is CheckBox || element is ReusableElement
            {
                _ = try performActivateElement(currentFocusedName)
                return
            }
            beginElementEdit(element, elementId: runtimeElementId(element))
            return
        }
    }

    private func setText(_ id: String?, _ text: String) throws -> Any
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        focusActiveWindowElement(element)
        editMode = true
        activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        rememberParentDynamicFocus(elementId, element)
        _ = options.onEditStarted?(elementId)
        if let numberInput = element as? NumberInput
        {
            numberInput.setValue(Double(text) ?? 0.0)
            numberInput.setEditText(text)
        }
        else
        {
            element.setText(text)
        }
        _ = dispatchGeneratedTextChanged(element, elementId: elementId, value: text)
        return snapshotResult(element)
    }

    private func typeText(_ id: String?, _ text: String) throws -> Any
    {
        return snapshotResult(try performTypeText(id, text))
    }

    @discardableResult
    private func performTypeText(_ id: String?, _ text: String) throws -> UIElement
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        focusActiveWindowElement(element)
        editMode = true
        activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        rememberParentDynamicFocus(elementId, element)
        if let numberInput = element as? NumberInput
        {
            numberInput.beginEdit()
            for character in text
            {
                _ = numberInput.handleKey(String(character))
                _ = dispatchGeneratedTextChanged(
                    numberInput,
                    elementId: elementId,
                    value: formatNumber(numberInput.numberValue)
                )
            }
        }
        else if let input = element as? TextInput
        {
            input.insertText(text)
            _ = dispatchGeneratedTextChanged(input, elementId: elementId, value: input.value)
        }
        else
        {
            throw RuntimeError("Element is not editable text: \(element.name)")
        }
        return element
    }

    private func selectionSnapshot(_ input: TextInput) -> [String: Any]
    {
        guard let bounds = input.selectionBounds() else
        {
            return ["start": NSNull(), "end": NSNull(), "text": ""]
        }
        return ["start": bounds.start, "end": bounds.end, "text": input.selectedText()]
    }

    private func requireTextInput(_ id: String?) throws -> TextInput
    {
        let element = try requireElement(id)
        guard let input = element as? TextInput else
        {
            throw RuntimeError("Element is not a text input: \(element.name)")
        }
        let elementId = id ?? runtimeElementId(input)
        focusActiveWindowElement(input)
        editMode = true
        rememberParentDynamicFocus(elementId, input)
        return input
    }

    private func setCursor(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        input.cursor = max(0, min(intArg(arguments, "offset", input.cursor), input.value.count))
        input.clearSelection()
        return ["cursor": input.cursor]
    }

    private func selectText(_ arguments: [String: Any]) throws -> Any
    {
        let element = try requireElement(arguments["element_id"] as? String)
        guard let input = element as? TextInput else
        {
            throw RuntimeError("Element is not a text input: \(element.name)")
        }
        let elementId = arguments["element_id"] as? String ?? runtimeElementId(input)
        let editingTarget = editMode && focusedName == elementId
        focusActiveWindowElement(input)
        if !editingTarget
        {
            beginElementEdit(input, elementId: elementId)
        }
        else
        {
            editMode = true
            rememberParentDynamicFocus(elementId, input)
        }
        input.selectRange(start: intArg(arguments, "start", 0), end: intArg(arguments, "end", 0))
        return selectionSnapshot(input)
    }

    private func copySelection(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        let selected = input.selectedText()
        _ = copyTextToClipboard(selected)
        return ["text": selected]
    }

    private func replaceSelection(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        input.replaceSelection(stringValue(arguments["text"]))
        _ = dispatchGeneratedTextChanged(input, elementId: runtimeElementId(input), value: input.value)
        return snapshotResult(input)
    }

    private func getSelection(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        return selectionSnapshot(input)
    }

    private func setValue(_ id: String?, _ value: Any?) throws -> Any
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        if let checkbox = element as? CheckBox
        {
            let checked = boolValue(value)
            checkbox.checked = checked
            _ = dispatchGeneratedTextChanged(checkbox, elementId: elementId, value: checked ? "true" : "false")
            return snapshotResult(element)
        }
        if let list = element as? ListBox
        {
            let values = (value as? [String]) ?? [stringValue(value)]
            list.setSelectedValues(values)
            _ = dispatchGeneratedSelectionChanged(list, elementId: elementId, value: list.selectedValues)
            return snapshotResult(element)
        }
        if let combo = element as? ComboBox
        {
            combo.selectValue(stringValue(value))
            _ = dispatchGeneratedSelectionChanged(combo, elementId: elementId, value: [combo.terminalText])
            return snapshotResult(element)
        }
        if let number = element as? NumberInput
        {
            number.setText(stringValue(value))
            _ = dispatchGeneratedTextChanged(number, elementId: elementId, value: number.value)
            return snapshotResult(element)
        }
        if let input = element as? TextInput
        {
            input.setValue(stringValue(value))
            _ = dispatchGeneratedTextChanged(input, elementId: elementId, value: input.value)
            return snapshotResult(element)
        }
        throw RuntimeError("Element value cannot be set directly: \(element.name)")
    }

    private func selectOption(_ arguments: [String: Any]) throws -> Any
    {
        let element = try requireElement(arguments["element_id"] as? String)
        let elementId = arguments["element_id"] as? String ?? runtimeElementId(element)
        let value = stringValue(arguments["value"] ?? arguments["option"] ?? "")
        if let list = element as? ListBox
        {
            list.setSelectedValues([value])
            _ = dispatchGeneratedSelectionChanged(list, elementId: elementId, value: list.selectedValues)
            return snapshotResult(element)
        }
        guard let combo = element as? ComboBox else
        {
            throw RuntimeError("Element has no selectable options: \(element.name)")
        }
        combo.selectValue(value)
        _ = dispatchGeneratedSelectionChanged(combo, elementId: elementId, value: [value])
        return snapshotResult(element)
    }

    private func setSelection(_ arguments: [String: Any]) throws -> Any
    {
        let element = try requireElement(arguments["element_id"] as? String)
        guard let list = element as? ListBox else
        {
            throw RuntimeError("Element is not a listbox: \(element.name)")
        }
        let elementId = arguments["element_id"] as? String ?? runtimeElementId(list)
        let values = (arguments["value"] as? [String]) ?? (arguments["values"] as? [String]) ?? []
        list.setSelectedValues(values)
        _ = dispatchGeneratedSelectionChanged(list, elementId: elementId, value: values)
        return snapshotResult(element)
    }

    private func setChecked(_ id: String?, _ value: Bool) throws -> Any
    {
        let element = try requireElement(id)
        guard let checkbox = element as? CheckBox else
        {
            throw RuntimeError("Element is not a checkbox: \(element.name)")
        }
        let elementId = id ?? runtimeElementId(element)
        checkbox.checked = value
        _ = dispatchGeneratedTextChanged(checkbox, elementId: elementId, value: value ? "true" : "false")
        return snapshotResult(element)
    }

    private func rememberParentDynamicFocus(_ elementId: String, _ element: UIElement)
    {
        guard let parentId = element.parentFocusHostId,
              let parent = window.element(named: parentId)
        else
        {
            return
        }
        rememberDynamicFocus(parentId: parentId, parent: parent, elementId: elementId)
    }

    private func rememberDynamicFocus(parentId: String, parent: UIElement, elementId: String)
    {
        parent.lastFocusedChildId = elementId
        rememberedDynamicFocusByParentId[parentId] = RememberedDynamicFocus(
            elementId: elementId,
            parentIdentity: ObjectIdentifier(parent),
            childFocusScopeGeneration: parent.childFocusScopeGeneration
        )
    }

    private func notifyValueChange(_ element: UIElement, before: Any?)
    {
        let after = element.valueForSnapshot
        if String(describing: before) == String(describing: after)
        {
            return
        }
        let elementId = runtimeElementId(element)
        if let combo = element as? ComboBox
        {
            _ = dispatchGeneratedSelectionChanged(combo, elementId: elementId, value: [combo.terminalText])
        }
        else if let list = element as? ListBox
        {
            _ = dispatchGeneratedSelectionChanged(list, elementId: elementId, value: list.selectedValues)
        }
        else if let text = after as? String
        {
            _ = dispatchGeneratedTextChanged(element, elementId: elementId, value: text)
        }
        else if let values = after as? [String]
        {
            _ = dispatchGeneratedSelectionChanged(element, elementId: elementId, value: values)
        }
    }

    private func usesLeaveCommit(_ element: UIElement?) -> Bool
    {
        if let listBox = element as? ListBox, listBox.multiple
        {
            return true
        }
        return element?.commitMode == kCommitModeLeave
    }

    private func ownsGeneratedScrollScope(_ element: UIElement) -> Bool
    {
        if let reusable = element as? ReusableElement,
           reusable.child is GeneratedScrollableContent
        {
            return true
        }
        if let viewHost = element as? ViewHost,
           viewHost.child is GeneratedScrollableContent
        {
            return true
        }
        return false
    }

    private func dynamicScrollScopeHostId(for element: UIElement) -> String?
    {
        guard let parentId = element.parentFocusHostId,
              let parent = window.element(named: parentId),
              ownsGeneratedScrollScope(parent)
        else
        {
            return nil
        }
        return parentId
    }

    private func dynamicScopeNavigationActive(for element: UIElement) -> Bool
    {
        guard editMode,
              let parentId = element.parentFocusHostId
        else
        {
            return false
        }
        return activeDynamicEditName == nil || activeDynamicEditName == parentId
    }

    private func restoreDynamicScopeAfterElementEdit(_ element: UIElement)
    {
        if let scopeHostId = dynamicScrollScopeHostId(for: element)
        {
            activeDynamicEditName = scopeHostId
            editMode = true
            return
        }
        activeDynamicEditName = nil
        editMode = false
    }

    private func handleGeneratedScrollScopeKey(_ element: UIElement, key: String) -> Bool
    {
        let viewport = Size(width: max(1, element.frame.width), height: max(1, element.frame.height))
        if let reusable = element as? ReusableElement,
           let scrollable = reusable.child as? GeneratedScrollableContent
        {
            if let keyHandler = scrollable as? GeneratedScrollableKeyHandling
            {
                return keyHandler.handleScrollKey(key, viewport: viewport)
            }
            if key == "Down"
            {
                scrollable.scrollBy(1, viewport: viewport)
                return true
            }
            if key == "Up"
            {
                scrollable.scrollBy(-1, viewport: viewport)
                return true
            }
        }
        if let viewHost = element as? ViewHost,
           let scrollable = viewHost.child as? GeneratedScrollableContent
        {
            if let keyHandler = scrollable as? GeneratedScrollableKeyHandling
            {
                return keyHandler.handleScrollKey(key, viewport: viewport)
            }
            if key == "Down"
            {
                scrollable.scrollBy(1, viewport: viewport)
                return true
            }
            if key == "Up"
            {
                scrollable.scrollBy(-1, viewport: viewport)
                return true
            }
        }
        return false
    }

    private func enterActiveScrollViewScope(from element: UIElement) -> Bool
    {
        let scrollView: ScrollView?
        if let direct = element as? ScrollView
        {
            scrollView = direct
        }
        else
        {
            scrollView = generatedScrollViewChild(of: element)
        }
        guard let scrollView else
        {
            return false
        }
        activeScrollView = scrollView
        activeScrollViewEditElement = nil
        editMode = true
        let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
        activeDynamicEditName = runtimeElementId(scopeRoot)
        let target = firstFocusableDescendant(in: scrollView)
        if let target
        {
            setFocusedElement(target)
            rememberScrollViewDescendant(scrollView, target)
        }
        else
        {
            setFocusedElement(scopeRoot)
        }
        if let focused = focusedElement(from: focusableElements(activeScrollView: scrollView)),
           focused !== scrollView,
           focused !== scopeRoot
        {
            _ = ensureElementFrameVisibleInScrollView(scrollView, element: focused)
        }
        return true
    }

    private func exitActiveScrollViewScope()
    {
        guard let scrollView = activeScrollView else
        {
            editMode = false
            return
        }
        let scopedFocusable = focusableElements(activeScrollView: scrollView)
        if let focused = focusedElement(from: scopedFocusable),
           focused !== scrollView,
           let parentId = focused.parentFocusHostId,
           let parent = window.element(named: parentId)
        {
            rememberDynamicFocus(parentId: parentId, parent: parent, elementId: runtimeElementId(focused))
            rememberScrollViewDescendant(scrollView, focused)
        }
        let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
        activeScrollView = nil
        activeScrollViewEditElement = nil
        activeDynamicEditName = nil
        editMode = false
        setFocusedElement(scopeRoot)
    }

    private func moveActiveScrollViewScopeFocus(key: String) -> Bool
    {
        guard let scrollView = activeScrollView else
        {
            return false
        }
        var activeFocusable = focusableElements(activeScrollView: scrollView)
        let current = focusedElement(from: activeFocusable) ?? scrollView
        let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
        var scoped = scrollViewScopeElements(scrollView).filter { $0 !== scrollView && $0 !== scopeRoot }
        if scoped.isEmpty
        {
            if key == "Up" || key == "Down"
            {
                return scrollView.handleKey(key, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height))
            }
            return false
        }
        var scopedIndex = scoped.firstIndex { $0 === current } ?? -1
        let beforeIndex = scopedIndex
        moveFocusSpatialIndex(&scopedIndex, focusable: scoped, key: key)
        if scopedIndex != beforeIndex,
           scopedIndex >= 0,
           scopedIndex < scoped.count
        {
            let target = scoped[scopedIndex]
            setFocusedElement(target)
            rememberScrollViewDescendant(scrollView, target)
            _ = ensureElementFrameVisibleInScrollView(scrollView, element: target)
            return true
        }
        guard key == "Up" || key == "Down" else
        {
            return false
        }
        guard scrollView.handleKey(key, viewport: Size(width: scrollView.frame.width, height: scrollView.frame.height)) else
        {
            return false
        }
        activeFocusable = focusableElements(activeScrollView: scrollView)
        scoped = scrollViewScopeElements(scrollView).filter { $0 !== scrollView && $0 !== scopeRoot }
        guard !scoped.isEmpty else
        {
            return true
        }
        var nextIndex = scoped.firstIndex { $0 === current } ?? -1
        if nextIndex >= 0
        {
            let beforeScrollMove = nextIndex
            moveFocusSpatialIndex(&nextIndex, focusable: scoped, key: key)
            if nextIndex == beforeScrollMove
            {
                return true
            }
        }
        else
        {
            nextIndex = key == "Down" ? 0 : scoped.count - 1
        }
        if nextIndex >= 0,
           nextIndex < scoped.count
        {
            let next = scoped[nextIndex]
            setFocusedElement(next)
            rememberScrollViewDescendant(scrollView, next)
            _ = ensureElementFrameVisibleInScrollView(scrollView, element: next)
        }
        return true
    }

    private func beginActiveScrollViewElementEdit(_ element: UIElement, elementId: String)
    {
        editSnapshot = captureSnapshot(element)
        beginElementEdit(element)
        editMode = true
        activeScrollViewEditElement = element
        activeDynamicEditName = elementId
        _ = options.onEditStarted?(elementId)
    }

    private func handleActiveScrollViewKey(_ key: String) throws -> Bool
    {
        guard activeScrollView != nil else
        {
            return false
        }
        if key == "Escape"
        {
            if let edited = activeScrollViewEditElement
            {
                if !usesLeaveCommit(edited)
                {
                    if let snapshot = editSnapshot,
                       snapshot.element === edited
                    {
                        restoreSnapshot(snapshot)
                    }
                }
                else
                {
                    commitEdit(edited)
                }
                editSnapshot = nil
                activeScrollViewEditElement = nil
                if let scrollView = activeScrollView
                {
                    let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
                    activeDynamicEditName = runtimeElementId(scopeRoot)
                }
            }
            else
            {
                exitActiveScrollViewScope()
            }
            return true
        }
        let focusable = focusableElements(activeScrollView: activeScrollView)
        guard let focused = focusedElement(from: focusable) else
        {
            return true
        }
        if let edited = activeScrollViewEditElement
        {
            let editedId = runtimeElementId(edited)
            if key == "Enter"
            {
                let before = edited.valueForSnapshot
                _ = edited.handleKey(key)
                notifyValueChange(edited, before: before)
                if let listBox = edited as? ListBox, listBox.multiple
                {
                    return true
                }
                if let input = edited as? TextInput
                {
                    _ = options.onTextConfirmed?(editedId, input.value)
                }
                else if let listBox = edited as? ListBox
                {
                    _ = options.onTextConfirmed?(editedId, listBox.selectedValues.first ?? "")
                }
                commitEdit(edited)
                editSnapshot = nil
                activeScrollViewEditElement = nil
                if let scrollView = activeScrollView
                {
                    let scopeRoot = generatedScrollViewProxy(in: window, for: scrollView) ?? scrollView
                    activeDynamicEditName = runtimeElementId(scopeRoot)
                }
                return true
            }
            let before = edited.valueForSnapshot
            if edited.handleKey(key)
            {
                notifyValueChange(edited, before: before)
            }
            return true
        }
        if ["Up", "Down", "Left", "Right"].contains(key)
        {
            _ = moveActiveScrollViewScopeFocus(key: key)
            return true
        }
        if key == "Enter" || key == " "
        {
            let focusedId = runtimeElementId(focused)
            if focused is Button || focused is Image
            {
                _ = dispatchGeneratedButton(focused, elementId: focusedId)
                return true
            }
            if let checkbox = focused as? CheckBox
            {
                let before = checkbox.valueForSnapshot
                _ = checkbox.handleKey(key)
                notifyValueChange(checkbox, before: before)
                return true
            }
            if focused is ReusableElement
            {
                _ = options.onButton?(focusedId)
                return true
            }
            if focused is TextInput || focused is NumberInput || focused is ComboBox || focused is ListBox
            {
                beginActiveScrollViewElementEdit(focused, elementId: focusedId)
                return true
            }
        }
        return true
    }

    private func moveFocus(_ delta: Int)
    {
        let focusable = focusableElements(activeScrollView: activeScrollView)
        guard !focusable.isEmpty else
        {
            focusedName = nil
            return
        }
        let index = focusIndex(in: focusable, matching: focusedName)
        let next = (index + delta + focusable.count) % focusable.count
        setFocusedElement(focusable[next])
    }

    private func moveSpatialFocus(_ key: String) -> Bool
    {
        let focusable = focusableElements(activeScrollView: activeScrollView)
        let order = focusable.map { runtimeElementId($0) }
        guard !focusable.isEmpty else
        {
            focusedName = nil
            return false
        }
        guard let current = focusedElement(from: focusable)
        else
        {
            setFocusedElement(focusable[0])
            return true
        }
        let currentId = runtimeElementId(current)
        let direction = (key == "Right" || key == "Down") ? 1 : -1
        let target: String?
        if key == "Left" || key == "Right"
        {
            target = horizontalDynamicFocusTarget(from: currentId, direction: direction, order: order)
        }
        else
        {
            target = verticalFocusTarget(from: current, direction: direction, order: order, currentId: currentId)
        }
        guard let target, target != currentId else
        {
            return false
        }
        if let element = focusable.first(where: { runtimeElementId($0) == target })
        {
            setFocusedElement(element)
            return true
        }
        return false
    }

    private func verticalFocusTarget(from current: UIElement, direction: Int, order: [String], currentId: String) -> String?
    {
        let currentRect = current.frame
        let currentCenterRow = centerRow(currentRect)
        var best: (id: String, band: Int, primary: Int, gap: Int, edge: Int)?
        for candidateId in order where candidateId != currentId
        {
            guard let candidate = window.element(named: candidateId) else
            {
                continue
            }
            let candidateRect = candidate.frame
            let candidateCenterRow = centerRow(candidateRect)
            let primary: Int
            if direction > 0
            {
                guard candidateCenterRow > currentCenterRow,
                      candidateRect.row >= currentRect.row + currentRect.height
                else
                {
                    continue
                }
                primary = max(0, candidateRect.row - (currentRect.row + currentRect.height))
            }
            else
            {
                guard candidateCenterRow < currentCenterRow,
                      candidateRect.row + candidateRect.height <= currentRect.row
                else
                {
                    continue
                }
                primary = max(0, currentRect.row - (candidateRect.row + candidateRect.height))
            }
            guard let band = bandScanRank(
                currentStart: currentRect.col,
                currentEnd: currentRect.col + currentRect.width,
                candidateStart: candidateRect.col,
                candidateEnd: candidateRect.col + candidateRect.width
            )
            else
            {
                continue
            }
            let gap = axisGap(
                startA: currentRect.col,
                endA: currentRect.col + currentRect.width,
                startB: candidateRect.col,
                endB: candidateRect.col + candidateRect.width
            )
            let edge = abs(candidateRect.col - currentRect.col)
            if best == nil ||
                spatialScoreIsBetter(
                    candidate: (band, primary, gap, edge),
                    best: (best!.band, best!.primary, best!.gap, best!.edge)
                )
            {
                best = (candidateId, band, primary, gap, edge)
            }
        }
        return best?.id
    }

    private func enterChildFocusScope(_ element: UIElement) -> Bool
    {
        let order = element.childFocusOrder.filter { window.element(named: $0) != nil }
        guard !order.isEmpty else
        {
            return false
        }
        let parentId = runtimeElementId(element)
        if let last = element.lastFocusedChildId, order.contains(last)
        {
            focusedName = last
        }
        else if let remembered = rememberedDynamicFocusByParentId[parentId],
                remembered.parentIdentity == ObjectIdentifier(element),
                remembered.childFocusScopeGeneration == element.childFocusScopeGeneration,
                order.contains(remembered.elementId)
        {
            focusedName = remembered.elementId
            element.lastFocusedChildId = remembered.elementId
        }
        else
        {
            focusedName = order[0]
        }
        editMode = true
        activeDynamicEditName = ownsGeneratedScrollScope(element) ? parentId : nil
        if let focusedName
        {
            ensureDynamicElementVisible(focusedName)
        }
        return true
    }

    private func ensureDynamicElementVisible(_ elementId: String)
    {
        guard let element = window.element(named: elementId),
              let parentId = element.parentFocusHostId,
              let parent = window.element(named: parentId),
              let reusable = parent as? ReusableElement,
              let scrollable = reusable.child as? GeneratedScrollableContent
        else
        {
            return
        }
        let scrollStyle = reusable.child?.layout.first?.cellStyle ?? reusable.child?.generatedWindowStyle ?? parent.style
        let parentHeight = max(1, parent.frame.height)
        let topPadding = min(paddingTop(scrollStyle), max(0, parentHeight - 1))
        let bottomPadding = min(paddingBottom(scrollStyle), max(0, parentHeight - topPadding - 1))
        let viewportTop = parent.frame.row + topPadding
        let viewportHeight = max(0, parentHeight - topPadding - bottomPadding)
        let viewportBottom = viewportTop + viewportHeight
        let elementTop = element.frame.row
        let elementBottom = element.frame.row + max(1, element.frame.height)
        let viewport = Size(width: parent.frame.width, height: parent.frame.height)
        syncGeneratedReusableChildFrames(reusable, to: parent.frame)
        if let scrollView = reusable.child?.generatedScrollView(),
           ensureElementFrameVisibleInScrollView(scrollView, element: element)
        {
            return
        }
        if let visibility = scrollable as? GeneratedScrollableElementVisibility
        {
            visibility.ensureElementVisible(elementId, viewport: viewport)
            return
        }
        if elementTop < viewportTop
        {
            scrollable.scrollBy(elementTop - viewportTop, viewport: viewport)
        }
        else if elementBottom > viewportBottom
        {
            scrollable.scrollBy(elementBottom - viewportBottom, viewport: viewport)
        }
    }

    private func moveDynamicFocus(from id: String, key: String) -> Bool
    {
        guard ["Up", "Down", "Left", "Right"].contains(key),
              let element = window.element(named: id),
              let parentId = element.parentFocusHostId,
              let parent = window.element(named: parentId)
        else
        {
            return false
        }
        let order = parent.childFocusOrder.filter { window.element(named: $0) != nil }
        guard !order.isEmpty else
        {
            return false
        }
        let target: String?
        if key == "Left" || key == "Right"
        {
            target = horizontalDynamicFocusTarget(from: id, direction: key == "Right" ? 1 : -1, order: order)
        }
        else
        {
            target = verticalFocusTarget(from: element, direction: key == "Down" ? 1 : -1, order: order, currentId: id)
        }
        guard let target, target != id else
        {
            rememberDynamicFocus(parentId: parentId, parent: parent, elementId: id)
            activeDynamicEditName = ownsGeneratedScrollScope(parent) ? parentId : nil
            if (key == "Up" || key == "Down"),
               let reusable = parent as? ReusableElement,
               let scrollable = reusable.child as? GeneratedScrollableContent
            {
                let viewport = Size(width: parent.frame.width, height: parent.frame.height)
                if let keyHandler = scrollable as? GeneratedScrollableKeyHandling
                {
                    _ = keyHandler.handleScrollKey(key, viewport: viewport)
                }
                else
                {
                    scrollable.scrollBy(key == "Down" ? 1 : -1, viewport: viewport)
                }
            }
            return true
        }
        focusedName = target
        activeDynamicEditName = ownsGeneratedScrollScope(parent) ? parentId : nil
        rememberDynamicFocus(parentId: parentId, parent: parent, elementId: target)
        ensureDynamicElementVisible(target)
        return true
    }

    private func horizontalDynamicFocusTarget(from id: String, direction: Int, order: [String]) -> String?
    {
        guard let current = window.element(named: id) else
        {
            return nil
        }
        let currentRect = current.frame
        let currentCenterCol = centerCol(currentRect)
        var best: (id: String, band: Int, primary: Int, gap: Int, edge: Int)?
        for candidateId in order where candidateId != id
        {
            guard let candidate = window.element(named: candidateId) else
            {
                continue
            }
            let candidateRect = candidate.frame
            let candidateCenterCol = centerCol(candidateRect)
            let primary: Int
            if direction > 0
            {
                guard candidateCenterCol > currentCenterCol,
                      candidateRect.col >= currentRect.col + currentRect.width
                else
                {
                    continue
                }
                primary = max(0, candidateRect.col - (currentRect.col + currentRect.width))
            }
            else
            {
                guard candidateCenterCol < currentCenterCol,
                      candidateRect.col + candidateRect.width <= currentRect.col
                else
                {
                    continue
                }
                primary = max(0, currentRect.col - (candidateRect.col + candidateRect.width))
            }
            guard let band = bandScanRank(
                currentStart: currentRect.row,
                currentEnd: currentRect.row + currentRect.height,
                candidateStart: candidateRect.row,
                candidateEnd: candidateRect.row + candidateRect.height
            )
            else
            {
                continue
            }
            let gap = axisGap(
                startA: currentRect.row,
                endA: currentRect.row + currentRect.height,
                startB: candidateRect.row,
                endB: candidateRect.row + candidateRect.height
            )
            let edge = abs(candidateRect.row - currentRect.row)
            if best == nil ||
                spatialScoreIsBetter(
                    candidate: (band, primary, gap, edge),
                    best: (best!.band, best!.primary, best!.gap, best!.edge)
                )
            {
                best = (candidateId, band, primary, gap, edge)
            }
        }
        return best?.id
    }

    private func centerRow(_ rect: Rect) -> Int
    {
        rect.row + rect.height / 2
    }

    private func centerCol(_ rect: Rect) -> Int
    {
        rect.col + rect.width / 2
    }

    private func axisOverlaps(startA: Int, endA: Int, startB: Int, endB: Int) -> Bool
    {
        max(startA, startB) < min(endA, endB)
    }

    private func axisGap(startA: Int, endA: Int, startB: Int, endB: Int) -> Int
    {
        if endA <= startB
        {
            return startB - endA
        }
        if endB <= startA
        {
            return startA - endB
        }
        return 0
    }

    private func bandScanRank(currentStart: Int, currentEnd: Int, candidateStart: Int, candidateEnd: Int) -> Int?
    {
        if axisOverlaps(startA: currentStart, endA: currentEnd, startB: candidateStart, endB: candidateEnd)
        {
            return 0
        }
        let maxDistance = max(
            max(abs(candidateStart - currentStart), abs(candidateEnd - currentEnd)),
            max(abs(candidateStart - currentEnd), abs(candidateEnd - currentStart))
        ) + max(1, currentEnd - currentStart) + max(1, candidateEnd - candidateStart)
        var rank = 1
        for distance in 1...maxDistance
        {
            for offset in [distance, -distance]
            {
                if axisOverlaps(
                    startA: currentStart + offset,
                    endA: currentEnd + offset,
                    startB: candidateStart,
                    endB: candidateEnd
                )
                {
                    return rank
                }
                rank += 1
            }
        }
        return nil
    }

    private func spatialScoreIsBetter(
        candidate: (band: Int, primary: Int, gap: Int, edge: Int),
        best: (band: Int, primary: Int, gap: Int, edge: Int)
    ) -> Bool
    {
        if candidate.band != best.band
        {
            return candidate.band < best.band
        }
        if candidate.primary != best.primary
        {
            return candidate.primary < best.primary
        }
        if candidate.gap != best.gap
        {
            return candidate.gap < best.gap
        }
        return candidate.edge < best.edge
    }

    private func resolvedWindowMode(_ targetWindow: GeneratedWindowBase? = nil) -> String
    {
        let targetWindow = targetWindow ?? window
        let explicit = targetWindow.mode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
        if ["fullscreen", "expand_width", "expand_height", "dialog"].contains(explicit)
        {
            return explicit
        }
        let expandsWidth = targetWindow.layout.contains
        {
            if case .expanded = $0.cellWidth
            {
                return true
            }
            return false
        }
        let expandsHeight = targetWindow.layout.contains
        {
            if case .expanded = $0.cellHeight
            {
                return true
            }
            return false
        }
        if expandsWidth && expandsHeight
        {
            return "fullscreen"
        }
        if expandsWidth
        {
            return "expand_width"
        }
        if expandsHeight
        {
            return "expand_height"
        }
        return "normal"
    }

    private func windowFrameSize(_ targetWindow: GeneratedWindowBase? = nil) -> Size
    {
        let targetWindow = targetWindow ?? window
        let contentNatural = targetWindow.generatedContentSize()
        let horizontal = windowBorderWidthHorizontal(targetWindow)
        let vertical = windowBorderWidthVertical(targetWindow)
        let natural = Size(
            width: contentNatural.width + vertical * 2,
            height: contentNatural.height + horizontal * 2
        )
        let mode = resolvedWindowMode(targetWindow)
        let style = targetWindow.generatedWindowStyle
        let availableWidth = max(1, viewport.width - marginLeft(style) - marginRight(style))
        let availableHeight = max(1, viewport.height - marginTop(style) - marginBottom(style))
        let width = mode == "fullscreen" || mode == "expand_width" ? availableWidth : min(natural.width, availableWidth)
        let height = mode == "fullscreen" || mode == "expand_height" ? availableHeight : min(natural.height, availableHeight)
        return Size(width: max(1, width), height: max(1, height))
    }

    private func windowOffset(_ targetWindow: GeneratedWindowBase? = nil, frameSize: Size? = nil) -> Rect
    {
        let targetWindow = targetWindow ?? window
        let frameSize = frameSize ?? windowFrameSize(targetWindow)
        let mode = resolvedWindowMode(targetWindow)
        let style = targetWindow.generatedWindowStyle
        let topMargin = marginTop(style)
        let rightMargin = marginRight(style)
        let bottomMargin = marginBottom(style)
        let leftMargin = marginLeft(style)
        let availableWidth = max(1, viewport.width - leftMargin - rightMargin)
        let availableHeight = max(1, viewport.height - topMargin - bottomMargin)
        let row: Int
        let col: Int
        if mode == "fullscreen"
        {
            row = topMargin
            col = leftMargin
        }
        else
        {
            row = mode == "expand_height" ? topMargin : topMargin + max(0, (availableHeight - frameSize.height) / 2)
            col = mode == "expand_width" ? leftMargin : leftMargin + max(0, (availableWidth - frameSize.width) / 2)
        }
        return Rect(row: row, col: col, width: frameSize.width, height: frameSize.height)
    }

    private func windowContentBounds(_ targetWindow: GeneratedWindowBase? = nil, frameSize: Size) -> Rect
    {
        let targetWindow = targetWindow ?? window
        let horizontal = windowBorderWidthHorizontal(targetWindow)
        let vertical = windowBorderWidthVertical(targetWindow)
        return Rect(
            row: horizontal,
            col: vertical,
            width: max(1, frameSize.width - vertical * 2),
            height: max(1, frameSize.height - horizontal * 2)
        )
    }

    private func windowBorderWidthHorizontal(_ targetWindow: GeneratedWindowBase? = nil) -> Int
    {
        let targetWindow = targetWindow ?? window
        return max(0, targetWindow.generatedWindowStyle.borderWidthHorizontal ?? 0)
    }

    private func windowBorderWidthVertical(_ targetWindow: GeneratedWindowBase? = nil) -> Int
    {
        let targetWindow = targetWindow ?? window
        return max(0, targetWindow.generatedWindowStyle.borderWidthVertical ?? 0)
    }

    private func renderActiveEditName(
        targetWindow: GeneratedWindowBase,
        focusedName: String?,
        editMode: Bool,
        dynamicEditName: String?
    ) -> String?
    {
        guard editMode else
        {
            return nil
        }
        if let dynamicEditName
        {
            return dynamicEditName
        }
        guard let focusedName,
              let element = targetWindow.element(named: focusedName),
              element.parentFocusHostId != nil
        else
        {
            return nil
        }
        return kNoActiveDynamicEditName
    }

    private func renderWindowFrame(
        _ targetWindow: GeneratedWindowBase,
        focusedName: String?,
        editMode: Bool,
        activeEditName: String? = nil,
        activeScrollView: ScrollView? = nil,
        activeScrollViewEditElement: UIElement? = nil,
        suppressActiveContainerScopeVisuals: Bool = false
    ) -> (frame: [[TerminalCell]], offset: Rect)
    {
        let frameSize = windowFrameSize(targetWindow)
        let offset = windowOffset(targetWindow, frameSize: frameSize)
        var frame = Array(
            repeating: Array(
                repeating: TerminalCell(
                    " ",
                    foreground: targetWindow.generatedWindowStyle.borderColor,
                    background: targetWindow.generatedWindowStyle.background
                ),
                count: frameSize.width
            ),
            count: frameSize.height
        )
        let content = windowContentBounds(targetWindow, frameSize: frameSize)
        fill(buffer: &frame, rect: content, style: targetWindow.generatedWindowStyle)
        let previousSuppressActiveContainerScopeVisuals = targetWindow.suppressActiveContainerScopeVisuals
        let previousActiveEditFocusName = targetWindow.activeEditFocusName
        targetWindow.suppressActiveContainerScopeVisuals =
            previousSuppressActiveContainerScopeVisuals || suppressActiveContainerScopeVisuals
        targetWindow.activeEditFocusName = activeEditName
        let contentEditMode = suppressActiveContainerScopeVisuals ? false : editMode
        let contentFrame = targetWindow.renderContent(
            size: Size(width: content.width, height: content.height),
            focusedName: focusedName,
            editMode: contentEditMode,
            activeScrollView: activeScrollView,
            activeScrollViewEditElement: activeScrollViewEditElement
        )
        targetWindow.suppressActiveContainerScopeVisuals = previousSuppressActiveContainerScopeVisuals
        targetWindow.activeEditFocusName = previousActiveEditFocusName
        blit(buffer: &frame, rendered: contentFrame, row: content.row, col: content.col)
        return (frame, offset)
    }

    private func dimColor(_ color: Color?, factor: Double = kModalBackgroundDimFactor, rounded: Bool = false) -> Color?
    {
        guard let color, let rgba = color.rgba, !color.isTransparent else
        {
            return color
        }
        func channel(_ value: Int) -> Int
        {
            let dimmed = Double(value) * factor
            return rounded ? Int(dimmed.rounded()) : Int(dimmed)
        }
        return Color.rgb(
            red: channel(rgba.red),
            green: channel(rgba.green),
            blue: channel(rgba.blue)
        )
    }

    private func dimBufferForModalOverlay(_ buffer: inout [[TerminalCell]])
    {
        for row in 0..<buffer.count
        {
            for col in 0..<buffer[row].count
            {
                buffer[row][col].foreground = dimColor(buffer[row][col].foreground)
                buffer[row][col].background = dimColor(buffer[row][col].background)
            }
        }
    }

    private func overlayFocusedComboBox(
        buffer: inout [[TerminalCell]],
        targetWindow: GeneratedWindowBase,
        focusedName: String?,
        editMode: Bool,
        activeScrollView: ScrollView?,
        activeScrollViewEditElement: UIElement?,
        windowRow: Int,
        windowCol: Int
    )
    {
        guard editMode, let focusedName, let comboBox = targetWindow.element(named: focusedName) as? ComboBox, !(comboBox is ListBox) else
        {
            return
        }
        if activeScrollView != nil && activeScrollViewEditElement !== comboBox
        {
            return
        }
        let frame = comboBox.frame
        let rendered = comboBox.render(
            size: Size(width: max(1, frame.width), height: kComboBoxClosedRows + comboBox.options.count),
            state: ElementRenderState(focused: true, editMode: true)
        )
        blit(buffer: &buffer, rendered: rendered, row: windowRow + frame.row, col: windowCol + frame.col)
    }

    private func renderContent() -> [[TerminalCell]]
    {
        if runtimeContainsImage()
        {
            syncImageTerminalCellPixels()
        }
        let modalStackOpen = rootOptions.windowStack?.allFrames.isEmpty == false
        let content = renderContentSurface(modalStackOpen: modalStackOpen)
        guard cleanupInvalidRuntimeFocusAfterLayout() else
        {
            return content
        }
        return renderContentSurface(modalStackOpen: modalStackOpen)
    }

    private func renderMcpSnapshotContent() -> [[TerminalCell]]
    {
        return renderContent()
    }

    private func windowNeedsSixelFallbackWarning(_ window: GeneratedWindowBase) -> Bool
    {
        var visitedWindows = Set<ObjectIdentifier>()
        var visitedElements = Set<ObjectIdentifier>()
        return windowNeedsSixelFallbackWarning(
            window,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
        )
    }

    private func runtimeContainsImage() -> Bool
    {
        var visitedWindows = Set<ObjectIdentifier>()
        var visitedElements = Set<ObjectIdentifier>()
        if windowContainsImage(
            rootWindow,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
        )
        {
            return true
        }
        for frame in rootOptions.windowStack?.allFrames ?? []
        {
            if windowContainsImage(
                frame.window,
                visitedWindows: &visitedWindows,
                visitedElements: &visitedElements
            )
            {
                return true
            }
        }
        return false
    }

    private func windowContainsImage(
        _ window: GeneratedWindowBase?,
        visitedWindows: inout Set<ObjectIdentifier>,
        visitedElements: inout Set<ObjectIdentifier>
    ) -> Bool
    {
        guard let window else
        {
            return false
        }
        let identifier = ObjectIdentifier(window)
        guard !visitedWindows.contains(identifier) else
        {
            return false
        }
        visitedWindows.insert(identifier)
        for element in window.elements
        {
            if elementContainsImage(
                element,
                visitedWindows: &visitedWindows,
                visitedElements: &visitedElements
            )
            {
                return true
            }
        }
        return false
    }

    private func elementContainsImage(
        _ element: UIElement?,
        visitedWindows: inout Set<ObjectIdentifier>,
        visitedElements: inout Set<ObjectIdentifier>
    ) -> Bool
    {
        guard let element else
        {
            return false
        }
        let identifier = ObjectIdentifier(element)
        guard !visitedElements.contains(identifier) else
        {
            return false
        }
        visitedElements.insert(identifier)
        if element is Image
        {
            return true
        }
        if let reusable = element as? ReusableElement,
           windowContainsImage(
            reusable.child,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
           )
        {
            return true
        }
        if let viewHost = element as? ViewHost,
           windowContainsImage(
            viewHost.child,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
           )
        {
            return true
        }
        for child in element.children
        {
            if elementContainsImage(
                child,
                visitedWindows: &visitedWindows,
                visitedElements: &visitedElements
            )
            {
                return true
            }
        }
        return false
    }

    private func windowNeedsSixelFallbackWarning(
        _ window: GeneratedWindowBase?,
        visitedWindows: inout Set<ObjectIdentifier>,
        visitedElements: inout Set<ObjectIdentifier>
    ) -> Bool
    {
        guard let window else
        {
            return false
        }
        let identifier = ObjectIdentifier(window)
        guard !visitedWindows.contains(identifier) else
        {
            return false
        }
        visitedWindows.insert(identifier)
        for element in window.elements
        {
            if elementNeedsSixelFallbackWarning(
                element,
                visitedWindows: &visitedWindows,
                visitedElements: &visitedElements
            )
            {
                return true
            }
        }
        return false
    }

    private func elementNeedsSixelFallbackWarning(
        _ element: UIElement?,
        visitedWindows: inout Set<ObjectIdentifier>,
        visitedElements: inout Set<ObjectIdentifier>
    ) -> Bool
    {
        guard let element else
        {
            return false
        }
        let identifier = ObjectIdentifier(element)
        guard !visitedElements.contains(identifier) else
        {
            return false
        }
        visitedElements.insert(identifier)
        if let image = element as? Image,
           imageModeNeedsSixelFallbackWarning(image.renderMode)
        {
            return true
        }
        if let reusable = element as? ReusableElement,
           windowNeedsSixelFallbackWarning(
            reusable.child,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
           )
        {
            return true
        }
        if let viewHost = element as? ViewHost,
           windowNeedsSixelFallbackWarning(
            viewHost.child,
            visitedWindows: &visitedWindows,
            visitedElements: &visitedElements
           )
        {
            return true
        }
        for child in element.children
        {
            if elementNeedsSixelFallbackWarning(
                child,
                visitedWindows: &visitedWindows,
                visitedElements: &visitedElements
            )
            {
                return true
            }
        }
        return false
    }

    private func renderContentSurface(modalStackOpen: Bool) -> [[TerminalCell]]
    {
        var surface = Array(
            repeating: Array(repeating: TerminalCell(" "), count: max(1, viewport.width)),
            count: max(1, viewport.height)
        )
        let rootBackgroundFocusedName = rootFocusedName
        let rootBackgroundEditMode = rootEditMode
        let rootBackgroundActiveEditName = renderActiveEditName(
            targetWindow: rootWindow,
            focusedName: rootBackgroundFocusedName,
            editMode: rootBackgroundEditMode,
            dynamicEditName: rootActiveDynamicEditName
        )
        let root = modalStackOpen
            ? withImageCellBackgroundRendering
            {
                renderWindowFrame(
                    rootWindow,
                    focusedName: rootBackgroundFocusedName,
                    editMode: rootBackgroundEditMode,
                    activeEditName: rootBackgroundActiveEditName,
                    activeScrollView: rootActiveScrollView,
                    activeScrollViewEditElement: rootActiveScrollViewEditElement,
                    suppressActiveContainerScopeVisuals: true
                )
            }
            : renderWindowFrame(
                rootWindow,
                focusedName: rootBackgroundFocusedName,
                editMode: rootBackgroundEditMode,
                activeEditName: rootBackgroundActiveEditName,
                activeScrollView: rootActiveScrollView,
                activeScrollViewEditElement: rootActiveScrollViewEditElement,
                suppressActiveContainerScopeVisuals: false
            )
        blit(buffer: &surface, rendered: root.frame, row: root.offset.row, col: root.offset.col)
        if !modalStackOpen
        {
            overlayFocusedComboBox(
                buffer: &surface,
                targetWindow: rootWindow,
                focusedName: rootBackgroundFocusedName,
                editMode: rootBackgroundEditMode,
                activeScrollView: rootActiveScrollView,
                activeScrollViewEditElement: rootActiveScrollViewEditElement,
                windowRow: root.offset.row,
                windowCol: root.offset.col
            )
        }

        guard let stack = rootOptions.windowStack, !stack.allFrames.isEmpty else
        {
            return surface
        }

        let frames = stack.allFrames
        if frames.contains(where: { $0.dimBackground })
        {
            dimBufferForModalOverlay(&surface)
        }

        for index in 0..<frames.count
        {
            let frame = frames[index]
            let isTopFrame = index == frames.count - 1
            var rendered = !isTopFrame
                ? withImageCellBackgroundRendering
                {
                    renderWindowFrame(
                        frame.window,
                        focusedName: frame.focusedName,
                        editMode: isTopFrame ? frame.editMode : false,
                        activeEditName: isTopFrame
                            ? renderActiveEditName(
                                targetWindow: frame.window,
                                focusedName: frame.focusedName,
                                editMode: frame.editMode,
                                dynamicEditName: frame.activeDynamicEditName
                            )
                            : nil,
                        activeScrollView: isTopFrame ? frame.activeScrollView : nil,
                        activeScrollViewEditElement: isTopFrame ? frame.activeScrollViewEditElement : nil
                    )
                }
                : renderWindowFrame(
                    frame.window,
                    focusedName: frame.focusedName,
                    editMode: frame.editMode,
                    activeEditName: renderActiveEditName(
                        targetWindow: frame.window,
                        focusedName: frame.focusedName,
                        editMode: frame.editMode,
                        dynamicEditName: frame.activeDynamicEditName
                    ),
                    activeScrollView: frame.activeScrollView,
                    activeScrollViewEditElement: frame.activeScrollViewEditElement
                )
            let laterFrameDimsBackground = frames[(index + 1)...].contains(where: { $0.dimBackground })
            if !isTopFrame && laterFrameDimsBackground
            {
                dimBufferForModalOverlay(&rendered.frame)
            }
            blit(buffer: &surface, rendered: rendered.frame, row: rendered.offset.row, col: rendered.offset.col)
            if isTopFrame
            {
                overlayFocusedComboBox(
                    buffer: &surface,
                    targetWindow: frame.window,
                    focusedName: frame.focusedName,
                    editMode: frame.editMode,
                    activeScrollView: frame.activeScrollView,
                    activeScrollViewEditElement: frame.activeScrollViewEditElement,
                    windowRow: rendered.offset.row,
                    windowCol: rendered.offset.col
                )
            }
        }

        return surface
    }

    private func getSchema() -> [String: Any]
    {
        [
            "window": getWindow(),
            "elements": window.elements.map
            {
                element in [
                    "id": element.name,
                    "type": element.elementType,
                    "role": element.role,
                    "value": element.valueForSnapshot ?? NSNull(),
                ] as [String: Any]
            },
        ]
    }

    private func requireElement(_ id: String?) throws -> UIElement
    {
        guard let id else
        {
            throw RuntimeError("Unknown element: ")
        }
        if let element = window.element(named: id)
        {
            return element
        }
        _ = renderContent()
        guard let element = window.element(named: id) else
        {
            throw RuntimeError("Unknown element: \(id)")
        }
        return element
    }
}

private func writeTerminal(_ text: String)
{
    _ = text.withCString
    {
        Darwin.write(STDOUT_FILENO, $0, strlen($0))
    }
}

private final class TerminalFrameBuffer
{
    private var width: Int
    private var height: Int
    private var cells: [TerminalCell]
    private var previous: [TerminalCell]
    private var forceFullRedraw = true

    init(width: Int, height: Int)
    {
        self.width = max(0, width)
        self.height = max(0, height)
        self.cells = Self.blankCells(width: self.width, height: self.height)
        self.previous = Self.blankCells(width: self.width, height: self.height)
    }

    @discardableResult
    func resize(width: Int, height: Int) -> Bool
    {
        let nextWidth = max(0, width)
        let nextHeight = max(0, height)
        guard nextWidth != self.width || nextHeight != self.height else
        {
            return false
        }
        self.width = nextWidth
        self.height = nextHeight
        self.cells = Self.blankCells(width: nextWidth, height: nextHeight)
        self.previous = Self.blankCells(width: nextWidth, height: nextHeight)
        self.forceFullRedraw = true
        return true
    }

    func setContent(_ content: [[TerminalCell]])
    {
        cells = Self.blankCells(width: width, height: height)
        let rows = min(height, content.count)
        for row in 0..<rows
        {
            let cols = min(width, content[row].count)
            for col in 0..<cols
            {
                cells[index(row: row, col: col)] = content[row][col]
            }
        }
    }

    func requestFullRedraw()
    {
        forceFullRedraw = true
    }

    func renderNotification(_ message: String)
    {
        guard !message.isEmpty, width > 0, height > 0 else
        {
            return
        }
        let text = " \(message) "
        let row = max(0, min(kCopyNotificationRow, height - 1))
        let col = max(0, width - text.count - kCopyNotificationRightMargin)
        let foreground = Color(kCopyNotificationForeground)
        let background = Color(kCopyNotificationBackground)
        for (offset, char) in text.enumerated()
        {
            let targetCol = col + offset
            guard targetCol < width else
            {
                break
            }
            cells[index(row: row, col: targetCol)] = TerminalCell(
                String(char),
                foreground: foreground,
                background: background
            )
        }
    }

    func renderDiff(rowOffset: Int = 0, colOffset: Int = 0) -> String
    {
        var output: [String] = []
        let fullRedraw = forceFullRedraw
        var synchronizeUpdate = false
        var rawEmitted = false
        for row in 0..<height
        {
            var col = 0
            while col < width
            {
                let startIndex = index(row: row, col: col)
                if cells[startIndex].rawSkip
                {
                    previous[startIndex] = cells[startIndex]
                    col += 1
                    continue
                }
                if !fullRedraw && cells[startIndex] == previous[startIndex]
                {
                    col += 1
                    continue
                }

                let styleCell = cells[startIndex]
                if !styleCell.raw.isEmpty
                {
                    synchronizeUpdate = true
                    let rawWidth = max(1, styleCell.rawWidth)
                    let rawHeight = max(1, styleCell.rawHeight)
                    let clearWidth = min(rawWidth, width - col)
                    let clearHeight = min(rawHeight, height - row)
                    for clearRow in row..<(row + clearHeight)
                    {
                        output.append("\u{001B}[\(clearRow + rowOffset + kTerminalAnsiBaseRow);\(col + colOffset + kTerminalAnsiBaseCol)H")
                        output.append(sgrForCell(styleCell))
                        output.append(String(repeating: " ", count: clearWidth))
                    }
                    if clearHeight >= rawHeight
                    {
                        output.append("\u{001B}[\(row + rowOffset + kTerminalAnsiBaseRow);\(col + colOffset + kTerminalAnsiBaseCol)H")
                        output.append(styleCell.raw)
                        rawEmitted = true
                    }
                    for coveredRow in row..<(row + clearHeight)
                    {
                        for coveredCol in col..<(col + clearWidth)
                        {
                            let coveredIndex = index(row: coveredRow, col: coveredCol)
                            previous[coveredIndex] = cells[coveredIndex]
                        }
                    }
                    col += clearWidth
                    continue
                }

                let runCol = col
                var run = ""
                while col < width
                {
                    let currentIndex = index(row: row, col: col)
                    let current = cells[currentIndex]
                    if !fullRedraw && current == previous[currentIndex]
                    {
                        break
                    }
                    if current.rawSkip || !current.raw.isEmpty
                    {
                        break
                    }
                    if current.foreground != styleCell.foreground ||
                       current.background != styleCell.background
                    {
                        break
                    }
                    run += safeTerminalCellText(current.text)
                    previous[currentIndex] = current
                    col += 1
                }

                output.append("\u{001B}[\(row + rowOffset + kTerminalAnsiBaseRow);\(runCol + colOffset + kTerminalAnsiBaseCol)H")
                output.append(sgrForCell(styleCell))
                output.append(run)
            }
        }
        guard !output.isEmpty else
        {
            forceFullRedraw = false
            return ""
        }
        if rawEmitted
        {
            for row in 0..<height
            {
                var col = 0
                while col < width
                {
                    let cell = cells[index(row: row, col: col)]
                    if cell.rawSkip || !cell.raw.isEmpty
                    {
                        col += 1
                        continue
                    }
                    let styleCell = cell
                    let runCol = col
                    var run = ""
                    while col < width
                    {
                        let current = cells[index(row: row, col: col)]
                        if current.rawSkip || !current.raw.isEmpty
                        {
                            break
                        }
                        if current.foreground != styleCell.foreground ||
                           current.background != styleCell.background
                        {
                            break
                        }
                        run += safeTerminalCellText(current.text)
                        col += 1
                    }
                    output.append("\u{001B}[\(row + rowOffset + kTerminalAnsiBaseRow);\(runCol + colOffset + kTerminalAnsiBaseCol)H")
                    output.append(sgrForCell(styleCell))
                    output.append(run)
                }
            }
        }
        output.append("\u{001B}[0m")
        var rendered = output.joined()
        if synchronizeUpdate
        {
            rendered = kTerminalSyncUpdateBegin + rendered + kTerminalSyncUpdateEnd
        }
        forceFullRedraw = false
        return rendered
    }

    private func index(row: Int, col: Int) -> Int
    {
        row * width + col
    }

    private static func blankCells(width: Int, height: Int) -> [TerminalCell]
    {
        Array(repeating: TerminalCell(), count: max(0, width * height))
    }
}

private func currentTerminalSize(fallback: Size) -> Size
{
    var windowSize = winsize()
    if ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == 0,
       windowSize.ws_col > 0,
       windowSize.ws_row > 0
    {
        return Size(width: Int(windowSize.ws_col), height: Int(windowSize.ws_row))
    }
    return Size(width: max(1, fallback.width), height: max(1, fallback.height))
}

private final class ImageTerminalCellPixelsStore: @unchecked Sendable
{
    private let lock = NSLock()
    private var value: Size?

    func set(_ size: Size)
    {
        lock.lock()
        value = size
        lock.unlock()
    }

    func get() -> Size?
    {
        lock.lock()
        let current = value
        lock.unlock()
        return current
    }
}

private let imageTerminalCellPixelsOverride = ImageTerminalCellPixelsStore()

private final class TerminalCellPixelQueryStore: @unchecked Sendable
{
    private let lock = NSLock()
    private var queried = false

    func markQueried() -> Bool
    {
        lock.lock()
        defer
        {
            lock.unlock()
        }
        if queried
        {
            return false
        }
        queried = true
        return true
    }
}

private let terminalCellPixelQueryStore = TerminalCellPixelQueryStore()

public func setImageTerminalCellPixels(_ size: Size)
{
    guard size.width > 0, size.height > 0 else
    {
        return
    }
    imageTerminalCellPixelsOverride.set(Size(width: size.width, height: size.height))
}

private func detectedTerminalCellPixels() -> Size?
{
    var windowSize = winsize()
    let hasWindowSize = ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == 0 &&
        windowSize.ws_col > 0 &&
        windowSize.ws_row > 0
    if hasWindowSize,
       windowSize.ws_xpixel > 0,
       windowSize.ws_ypixel > 0
    {
        let cols = Int(windowSize.ws_col)
        let rows = Int(windowSize.ws_row)
        let pixelWidth = max(1, (Int(windowSize.ws_xpixel) + cols / 2) / cols)
        let pixelHeight = max(1, (Int(windowSize.ws_ypixel) + rows / 2) / rows)
        return Size(width: pixelWidth, height: pixelHeight)
    }
    guard terminalCellPixelQueryStore.markQueried() else
    {
        return nil
    }
    if let cellSize = queryTerminalPixelSize(command: kTerminalCellPixelQuery, responseKind: 6)
    {
        return cellSize
    }
    if hasWindowSize,
       let areaSize = queryTerminalPixelSize(command: kTerminalTextAreaPixelQuery, responseKind: 4)
    {
        let cols = Int(windowSize.ws_col)
        let rows = Int(windowSize.ws_row)
        return Size(
            width: max(1, (areaSize.width + cols / 2) / cols),
            height: max(1, (areaSize.height + rows / 2) / rows)
        )
    }
    return nil
}

private func syncImageTerminalCellPixels()
{
    if imageTerminalCellPixelsOverride.get() != nil
    {
        return
    }
    guard let detected = detectedTerminalCellPixels() else
    {
        return
    }
    setImageTerminalCellPixels(detected)
}

private func queryTerminalPixelSize(command: String, responseKind: Int) -> Size?
{
    guard isatty(STDIN_FILENO) != 0 && isatty(STDOUT_FILENO) != 0 else
    {
        return nil
    }
    writeTerminal(command)
    usleep(kTerminalPixelQueryDelayMicros)
    let bytes = readNonblockingTerminalBytes(limit: kTerminalPixelQueryMaxBytes)
    return parseTerminalPixelSizeResponse(bytes, responseKind: responseKind)
}

private func parseTerminalPixelSizeResponse(_ bytes: [UInt8], responseKind: Int) -> Size?
{
    let prefix = Array("\u{001B}[\(responseKind);".utf8)
    guard !prefix.isEmpty, bytes.count >= prefix.count else
    {
        return nil
    }
    var index = 0
    while index + prefix.count <= bytes.count
    {
        if Array(bytes[index..<(index + prefix.count)]) == prefix
        {
            var cursor = index + prefix.count
            let height = parseUnsignedInteger(bytes, cursor: &cursor)
            guard height > 0,
                  cursor < bytes.count,
                  bytes[cursor] == UInt8(ascii: ";")
            else
            {
                index += 1
                continue
            }
            cursor += 1
            let width = parseUnsignedInteger(bytes, cursor: &cursor)
            guard width > 0,
                  cursor < bytes.count,
                  bytes[cursor] == UInt8(ascii: "t")
            else
            {
                index += 1
                continue
            }
            return Size(width: width, height: height)
        }
        index += 1
    }
    return nil
}

private func parseUnsignedInteger(_ bytes: [UInt8], cursor: inout Int) -> Int
{
    var value = 0
    var consumed = false
    while cursor < bytes.count
    {
        let byte = bytes[cursor]
        guard byte >= UInt8(ascii: "0") && byte <= UInt8(ascii: "9") else
        {
            break
        }
        value = value * 10 + Int(byte - UInt8(ascii: "0"))
        cursor += 1
        consumed = true
    }
    return consumed ? value : 0
}

private func terminalCellPixels() -> Size
{
    if let override = imageTerminalCellPixelsOverride.get()
    {
        return override
    }
    return detectedTerminalCellPixels() ?? Size(width: kImageCellPixelWidth, height: kImageCellPixelHeight)
}

private func sgrForCell(_ cell: TerminalCell) -> String
{
    "\u{001B}[\(sgrForColor(cell.foreground, foreground: true));\(sgrForColor(cell.background, foreground: false))m"
}

private func sgrForColor(_ color: Color?, foreground: Bool) -> String
{
    guard let rgba = color?.rgba, color?.isTransparent != true else
    {
        return "\(foreground ? kTerminalDefaultForeground : kTerminalDefaultBackground)"
    }
    return "\(foreground ? 38 : 48);2;\(rgba.red);\(rgba.green);\(rgba.blue)"
}

private func safeTerminalCellText(_ text: String) -> String
{
    var result = ""
    for scalar in text.unicodeScalars
    {
        result += isUnsafeTerminalScalar(scalar) ? "?" : String(scalar)
    }
    return result.isEmpty ? " " : result
}

private func readTerminalByte() -> UInt8?
{
    var byte: UInt8 = 0
    let count = Darwin.read(STDIN_FILENO, &byte, 1)
    return count == 1 ? byte : nil
}

private func terminalInputReady(timeoutMilliseconds: Int32) -> Bool
{
    var descriptor = pollfd(fd: STDIN_FILENO, events: Int16(POLLIN), revents: 0)
    let ready = Darwin.poll(&descriptor, 1, timeoutMilliseconds)
    return ready > 0 && (descriptor.revents & Int16(POLLIN)) != 0
}

private func readNonblockingTerminalBytes(limit: Int) -> [UInt8]
{
    let flags = fcntl(STDIN_FILENO, F_GETFL)
    if flags >= 0
    {
        _ = fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK)
    }
    defer
    {
        if flags >= 0
        {
            _ = fcntl(STDIN_FILENO, F_SETFL, flags)
        }
    }

    var bytes: [UInt8] = []
    while bytes.count < max(0, limit)
    {
        var byte: UInt8 = 0
        let count = Darwin.read(STDIN_FILENO, &byte, 1)
        if count == 1
        {
            bytes.append(byte)
            continue
        }
        break
    }
    return bytes
}

private func readTerminalInputBytes() -> [UInt8]
{
    let flags = fcntl(STDIN_FILENO, F_GETFL)
    if flags >= 0
    {
        _ = fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK)
    }
    defer
    {
        if flags >= 0
        {
            _ = fcntl(STDIN_FILENO, F_SETFL, flags)
        }
    }

    guard let first = readTerminalByte() else
    {
        return []
    }
    var bytes = [first]
    if first == TerminalInputParser.escapeByte
    {
        while TerminalInputParser.escapeSequenceNeedsMoreBytes(bytes)
        {
            guard terminalInputReady(timeoutMilliseconds: kTerminalEscapeSequenceTimeoutMilliseconds) else
            {
                break
            }
            let continuation = drainTerminalInputBytes()
            guard !continuation.isEmpty else
            {
                break
            }
            bytes.append(contentsOf: continuation)
        }
        return bytes
    }
    if first >= TerminalInputParser.firstMultibyteUtf8Byte
    {
        usleep(kTerminalUtf8ReadDelayMicros)
    }
    return bytes + drainTerminalInputBytes()
}

private func drainTerminalInputBytes() -> [UInt8]
{
    let flags = fcntl(STDIN_FILENO, F_GETFL)
    if flags >= 0
    {
        _ = fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK)
    }
    defer
    {
        if flags >= 0
        {
            _ = fcntl(STDIN_FILENO, F_SETFL, flags)
        }
    }

    var bytes: [UInt8] = []
    while true
    {
        var byte: UInt8 = 0
        let count = Darwin.read(STDIN_FILENO, &byte, 1)
        if count == 1
        {
            bytes.append(byte)
            continue
        }
        break
    }
    return bytes
}

private final class TerminalInputParser
{
    static let escapeByte = UInt8(27)
    static let firstMultibyteUtf8Byte = UInt8(0xC2)

    private static let pasteStart = Array("\u{001B}[200~".utf8)
    private static let pasteEnd = Array("\u{001B}[201~".utf8)
    private static let terminalCellPixelResponsePrefix = Array("\u{001B}[6;".utf8)
    private static let terminalTextAreaPixelResponsePrefix = Array("\u{001B}[4;".utf8)
    private static let terminalCellPixelResponseTerminator = UInt8(ascii: "t")
    private static let carriageReturn = UInt8(13)
    private static let lineFeed = UInt8(10)
    private static let tab = UInt8(9)
    private static let backspace = UInt8(8)
    private static let delete = UInt8(127)
    private static let ctrlC = UInt8(3)
    private static let ctrlD = UInt8(4)
    private static let ss3Prefix = UInt8(ascii: "O")
    private static let sgrMousePrefix = Array("\u{001B}[<".utf8)
    private static let sgrMouseWheelUpButton = 64
    private static let sgrMouseWheelDownButton = 65
    private static let sgrMousePressedSuffix = UInt8(ascii: "M")
    private static let sgrMouseReleasedSuffix = UInt8(ascii: "m")
    private static let terminalCoordinateBase = 1
    private static let noModifier = 1
    private static let shiftModifier = 2
    private static let altModifier = 3
    private static let altShiftModifier = 4
    private static let ctrlModifier = 5
    private static let ctrlShiftModifier = 6
    private static let metaModifier = 9
    private static let metaShiftModifier = 10
    private static let enterCodepoint = 13
    private static let ctrlCCodepoint = 3
    private static let uppercaseCCodepoint = 67
    private static let lowercaseCCodepoint = 99
    private static let uppercaseVCodepoint = 86
    private static let lowercaseVCodepoint = 118
    private static let altEnterSequences = [
        Array("\u{001B}[13;3u".utf8),
        Array("\u{001B}[13;5u".utf8),
        Array("\u{001B}[27;3;13~".utf8),
        Array("\u{001B}[27;5;13~".utf8),
    ]

    private var buffer: [UInt8] = []

    static func escapeSequenceNeedsMoreBytes(_ bytes: [UInt8]) -> Bool
    {
        guard bytes.first == Self.escapeByte else
        {
            return false
        }
        guard bytes.count > 1 else
        {
            return true
        }
        if bytes[1] == Self.ss3Prefix
        {
            return bytes.count < 3
        }
        guard bytes[1] == UInt8(ascii: "[") else
        {
            return false
        }
        guard bytes.count > 2 else
        {
            return true
        }
        return !bytes.dropFirst(2).contains(where: Self.isCsiFinal)
    }

    func feed(_ bytes: [UInt8]) -> [TerminalInputEvent]
    {
        buffer.append(contentsOf: bytes)
        var events: [TerminalInputEvent] = []
        while parseNext(events: &events)
        {
        }
        return events
    }

    private func parseNext(events: inout [TerminalInputEvent]) -> Bool
    {
        guard let first = buffer.first else
        {
            return false
        }
        if startsWith(Self.pasteStart)
        {
            return parseBracketedPaste(events: &events)
        }
        if first == Self.escapeByte
        {
            return parseEscapeSequence(events: &events)
        }
        if first == Self.ctrlC || first == Self.ctrlD
        {
            buffer.removeFirst()
            events.append(.key("Ctrl+C"))
            return true
        }
        if first == Self.carriageReturn || first == Self.lineFeed
        {
            buffer.removeFirst()
            events.append(.key("Enter"))
            return true
        }
        if first == Self.tab
        {
            buffer.removeFirst()
            events.append(.key("Tab"))
            return true
        }
        if first == Self.delete || first == Self.backspace
        {
            buffer.removeFirst()
            events.append(.key("Backspace"))
            return true
        }
        if first >= Self.firstMultibyteUtf8Byte
        {
            return parseUtf8Text(events: &events)
        }
        buffer.removeFirst()
        if first >= UInt8(ascii: " ")
        {
            events.append(.key(String(UnicodeScalar(first))))
        }
        else
        {
            events.append(.none)
        }
        return true
    }

    private func parseEscapeSequence(events: inout [TerminalInputEvent]) -> Bool
    {
        if buffer.count == 1
        {
            buffer.removeFirst()
            events.append(.key("Escape"))
            return true
        }
        if buffer.count >= 2 && (buffer[1] == Self.carriageReturn || buffer[1] == Self.lineFeed)
        {
            buffer.removeFirst(2)
            events.append(.key("Alt+Enter"))
            return true
        }
        for sequence in Self.altEnterSequences
        {
            if startsWith(sequence)
            {
                buffer.removeFirst(sequence.count)
                events.append(.key("Alt+Enter"))
                return true
            }
            if isBufferedPrefix(of: sequence)
            {
                return false
            }
        }
        if startsWith(Array("\u{001B}[".utf8)), let end = firstIndex(of: UInt8(ascii: "u"))
        {
            let sequence = Array(buffer.prefix(end + 1))
            if let key = parseModifiedKeyCsi(sequence), !key.isEmpty
            {
                buffer.removeFirst(end + 1)
                events.append(.key(key))
                return true
            }
        }
        if startsWith(Array("\u{001B}[13;".utf8)), firstIndex(of: UInt8(ascii: "u")) == nil
        {
            return false
        }
        if startsWith(Array("\u{001B}[27;".utf8)), let end = firstIndex(of: UInt8(ascii: "~"))
        {
            let sequence = Array(buffer.prefix(end + 1))
            if let key = parseLegacyModifiedKeyCsi(sequence), !key.isEmpty
            {
                buffer.removeFirst(end + 1)
                events.append(.key(key))
                return true
            }
        }
        if startsWith(Self.pasteStart)
        {
            return parseBracketedPaste(events: &events)
        }
        if startsWith(Self.sgrMousePrefix)
        {
            return parseSgrMouse(events: &events)
        }
        if startsWith(Self.terminalCellPixelResponsePrefix) || startsWith(Self.terminalTextAreaPixelResponsePrefix)
        {
            guard let end = firstIndex(of: Self.terminalCellPixelResponseTerminator) else
            {
                return false
            }
            buffer.removeFirst(end + 1)
            return true
        }
        return consumeSimpleEscape(events: &events)
    }

    private func parseBracketedPaste(events: inout [TerminalInputEvent]) -> Bool
    {
        guard let end = firstIndex(of: Self.pasteEnd, startingAt: Self.pasteStart.count) else
        {
            return false
        }
        let payload = Array(buffer[Self.pasteStart.count..<end])
        buffer.removeFirst(end + Self.pasteEnd.count)
        events.append(.paste(String(bytes: payload, encoding: .utf8) ?? ""))
        return true
    }

    private func parseSgrMouse(events: inout [TerminalInputEvent]) -> Bool
    {
        guard let final = firstIndex(where: { $0 == Self.sgrMousePressedSuffix || $0 == Self.sgrMouseReleasedSuffix }, startingAt: Self.sgrMousePrefix.count) else
        {
            return false
        }
        let suffix = buffer[final]
        let payload = String(bytes: buffer[Self.sgrMousePrefix.count..<final], encoding: .utf8) ?? ""
        let fields = payload.split(separator: ";")
        guard fields.count == 3,
              let button = Int(fields[0]),
              let x = Int(fields[1]),
              let y = Int(fields[2])
        else
        {
            buffer.removeFirst(final + 1)
            return true
        }
        buffer.removeFirst(final + 1)
        let point = Point(
            row: max(0, y - Self.terminalCoordinateBase),
            col: max(0, x - Self.terminalCoordinateBase)
        )
        if button == Self.sgrMouseWheelUpButton || button == Self.sgrMouseWheelDownButton
        {
            events.append(.scroll(
                point: point,
                delta: button == Self.sgrMouseWheelUpButton ? 1 : -1
            ))
            return true
        }
        if suffix == Self.sgrMouseReleasedSuffix
        {
            events.append(.mouse(kind: "release", point: point))
            return true
        }
        events.append(.mouse(kind: (button & 32) == 32 ? "move" : "press", point: point))
        return true
    }

    private func consumeSimpleEscape(events: inout [TerminalInputEvent]) -> Bool
    {
        if startsWith(Array("\u{001B}[".utf8))
        {
            guard let finalIndex = firstIndex(where: { Self.isCsiFinal($0) }, startingAt: 2) else
            {
                return false
            }
            let sequence = Array(buffer.prefix(finalIndex + 1))
            if let key = parseCsiKey(sequence), !key.isEmpty
            {
                buffer.removeFirst(finalIndex + 1)
                events.append(.key(key))
                return true
            }
        }
        if buffer.count >= 3 && buffer[0] == Self.escapeByte && buffer[1] == Self.ss3Prefix
        {
            let sequence = Array(buffer.prefix(3))
            if let key = parseSs3Key(sequence), !key.isEmpty
            {
                buffer.removeFirst(3)
                events.append(.key(key))
                return true
            }
        }
        buffer.removeFirst()
        events.append(.key("Escape"))
        return true
    }

    private func parseUtf8Text(events: inout [TerminalInputEvent]) -> Bool
    {
        let length = utf8SequenceLength(buffer[0])
        guard length > 0 else
        {
            buffer.removeFirst()
            events.append(.none)
            return true
        }
        guard buffer.count >= length else
        {
            return false
        }
        let bytes = Array(buffer.prefix(length))
        buffer.removeFirst(length)
        guard let text = String(bytes: bytes, encoding: .utf8), !text.isEmpty else
        {
            events.append(.none)
            return true
        }
        events.append(text.count == 1 ? .key(text) : .paste(text))
        return true
    }

    private func startsWith(_ prefix: [UInt8]) -> Bool
    {
        buffer.count >= prefix.count && Array(buffer.prefix(prefix.count)) == prefix
    }

    private func isBufferedPrefix(of text: [UInt8]) -> Bool
    {
        buffer.count < text.count && Array(text.prefix(buffer.count)) == buffer
    }

    private func firstIndex(of byte: UInt8) -> Int?
    {
        buffer.firstIndex(of: byte)
    }

    private func firstIndex(where predicate: (UInt8) -> Bool, startingAt start: Int) -> Int?
    {
        guard start < buffer.count else
        {
            return nil
        }
        for index in start..<buffer.count where predicate(buffer[index])
        {
            return index
        }
        return nil
    }

    private func firstIndex(of needle: [UInt8], startingAt start: Int = 0) -> Int?
    {
        guard !needle.isEmpty, start < buffer.count, buffer.count - start >= needle.count else
        {
            return nil
        }
        for index in start...(buffer.count - needle.count)
        {
            if Array(buffer[index..<(index + needle.count)]) == needle
            {
                return index
            }
        }
        return nil
    }

    private func utf8SequenceLength(_ first: UInt8) -> Int
    {
        if first & 0b1110_0000 == 0b1100_0000
        {
            return 2
        }
        if first & 0b1111_0000 == 0b1110_0000
        {
            return 3
        }
        if first & 0b1111_1000 == 0b1111_0000
        {
            return 4
        }
        return 0
    }

    private func parseModifiedKeyCsi(_ bytes: [UInt8]) -> String?
    {
        guard let sequence = String(bytes: bytes, encoding: .utf8),
              sequence.hasPrefix("\u{001B}["),
              sequence.hasSuffix("u")
        else
        {
            return nil
        }
        let payload = sequence.dropFirst(2).dropLast()
        guard let separator = payload.firstIndex(of: ";"),
              let codepoint = Int(payload[..<separator]),
              let modifier = Int(payload[payload.index(after: separator)...]),
              modifier != Self.noModifier
        else
        {
            return nil
        }
        if codepoint == Self.enterCodepoint
        {
            return "Alt+Enter"
        }
        if codepoint == Self.ctrlCCodepoint ||
            ((codepoint == Self.lowercaseCCodepoint || codepoint == Self.uppercaseCCodepoint) &&
                (modifier == Self.ctrlModifier || modifier == Self.ctrlShiftModifier))
        {
            return "Ctrl+C"
        }
        if (codepoint == Self.lowercaseCCodepoint || codepoint == Self.uppercaseCCodepoint) &&
            (modifier == Self.metaModifier || modifier == Self.metaShiftModifier)
        {
            return "cmd_c"
        }
        if (codepoint == Self.lowercaseVCodepoint || codepoint == Self.uppercaseVCodepoint) &&
            (modifier == Self.metaModifier || modifier == Self.metaShiftModifier)
        {
            return "cmd_v"
        }
        return nil
    }

    private func parseLegacyModifiedKeyCsi(_ bytes: [UInt8]) -> String?
    {
        guard let sequence = String(bytes: bytes, encoding: .utf8),
              sequence.hasPrefix("\u{001B}[27;"),
              sequence.hasSuffix("~")
        else
        {
            return nil
        }
        let payload = sequence.dropFirst(2).dropLast()
        let fields = payload.split(separator: ";")
        guard fields.count == 3,
              let modifier = Int(fields[1]),
              let codepoint = Int(fields[2])
        else
        {
            return nil
        }
        if codepoint == Self.ctrlCCodepoint ||
            ((codepoint == Self.lowercaseCCodepoint || codepoint == Self.uppercaseCCodepoint) &&
                (modifier == Self.ctrlModifier || modifier == Self.ctrlShiftModifier))
        {
            return "Ctrl+C"
        }
        if (codepoint == Self.lowercaseCCodepoint || codepoint == Self.uppercaseCCodepoint) &&
            (modifier == Self.metaModifier || modifier == Self.metaShiftModifier)
        {
            return "cmd_c"
        }
        if (codepoint == Self.lowercaseVCodepoint || codepoint == Self.uppercaseVCodepoint) &&
            (modifier == Self.metaModifier || modifier == Self.metaShiftModifier)
        {
            return "cmd_v"
        }
        return nil
    }

    private func parseCsiKey(_ bytes: [UInt8]) -> String?
    {
        guard bytes.count >= 3,
              let sequence = String(bytes: bytes, encoding: .utf8),
              sequence.hasPrefix("\u{001B}[")
        else
        {
            return nil
        }
        let compact = String(sequence.dropFirst())
        let keyMap: [String: String] = [
            "[A": "Up",
            "[B": "Down",
            "[C": "Right",
            "[D": "Left",
            "[H": "Home",
            "[F": "End",
            "[Z": "Shift+Tab",
            "[1~": "Home",
            "[4~": "End",
            "[3~": "Delete",
            "[5~": "PageUp",
            "[6~": "PageDown",
        ]
        if let key = keyMap[compact]
        {
            return key
        }
        let payload = sequence.dropFirst(2).dropLast()
        guard let separator = payload.lastIndex(of: ";"),
              let modifier = Int(payload[payload.index(after: separator)...]),
              let final = sequence.last
        else
        {
            return nil
        }
        return modifiedArrowKey(final: final, modifier: modifier)
    }

    private func parseSs3Key(_ bytes: [UInt8]) -> String?
    {
        guard bytes.count == 3,
              let sequence = String(bytes: bytes, encoding: .utf8),
              sequence.hasPrefix("\u{001B}O"),
              let final = sequence.last
        else
        {
            return nil
        }
        return keyForCsiFinal(final)
    }

    private func modifiedArrowKey(final: Character, modifier: Int) -> String?
    {
        guard let key = keyForCsiFinal(final) else
        {
            return nil
        }
        switch modifier
        {
        case Self.shiftModifier:
            return "Shift+\(key)"
        case Self.altModifier:
            return "Alt+\(key)"
        case Self.altShiftModifier:
            return "Alt+Shift+\(key)"
        case Self.ctrlModifier:
            return "Ctrl+\(key)"
        case Self.ctrlShiftModifier:
            return "Ctrl+Shift+\(key)"
        default:
            return key
        }
    }

    private func keyForCsiFinal(_ final: Character) -> String?
    {
        switch final
        {
        case "A":
            return "Up"
        case "B":
            return "Down"
        case "C":
            return "Right"
        case "D":
            return "Left"
        case "H":
            return "Home"
        case "F":
            return "End"
        case "Z":
            return "Shift+Tab"
        default:
            return nil
        }
    }

    private static func isCsiFinal(_ byte: UInt8) -> Bool
    {
        byte >= UInt8(ascii: "@") && byte <= UInt8(ascii: "~")
    }
}

private func coalesceTerminalInputEvents(_ events: [TerminalInputEvent]) -> [TerminalInputEvent]
{
    var coalesced: [TerminalInputEvent] = []
    coalesced.reserveCapacity(events.count)
    var index = 0
    while index < events.count
    {
        if case .scroll = events[index]
        {
            var mergedPoint = Point(row: 0, col: 0)
            var delta = 0
            while index < events.count
            {
                guard case let .scroll(point, wheelDelta) = events[index] else
                {
                    break
                }
                mergedPoint = point
                delta += wheelDelta
                index += 1
            }
            let clamped = max(-kMaxCoalescedMouseWheelDelta, min(kMaxCoalescedMouseWheelDelta, delta))
            if clamped != 0
            {
                coalesced.append(.scroll(point: mergedPoint, delta: clamped))
            }
            continue
        }
        coalesced.append(events[index])
        index += 1
    }
    return coalesced
}

private func resolvedImagePath(_ source: String) -> String
{
    guard !source.isEmpty else
    {
        return ""
    }
    let fileManager = FileManager.default
    if source.hasPrefix("/")
    {
        return source
    }
    var directory = URL(fileURLWithPath: fileManager.currentDirectoryPath)
    while true
    {
        let candidate = directory.appendingPathComponent(source).path
        if fileManager.fileExists(atPath: candidate)
        {
            return candidate
        }
        let parent = directory.deletingLastPathComponent()
        if parent.path == directory.path
        {
            break
        }
        directory = parent
    }
    return URL(fileURLWithPath: fileManager.currentDirectoryPath).appendingPathComponent(source).path
}

private func imageFileSize(_ path: String) -> Size?
{
    guard let data = try? Data(contentsOf: URL(fileURLWithPath: path)), !data.isEmpty else
    {
        return nil
    }
    return pngImageSize(data)
        ?? bmpImageSize(data)
        ?? gifImageSize(data)
        ?? jpegImageSize(data)
        ?? tgaImageSize(data)
        ?? portableAnyMapImageSize(data)
}

private struct RuntimeRgb: Hashable
{
    var red: Int
    var green: Int
    var blue: Int
}

private struct RuntimeImageRaster
{
    var width: Int
    var height: Int
    var pixels: [RuntimeRgb]
    var alpha: [Int]
}

private final class RuntimeImageRasterCache: @unchecked Sendable
{
    private let lock = NSLock()
    private var rasters: [String: RuntimeImageRaster] = [:]
    private var failedPaths = Set<String>()

    func cached(_ path: String) -> RuntimeImageRaster??
    {
        lock.lock()
        defer
        {
            lock.unlock()
        }
        if let raster = rasters[path]
        {
            return .some(raster)
        }
        if failedPaths.contains(path)
        {
            return .some(nil)
        }
        return nil
    }

    func store(_ raster: RuntimeImageRaster?, for path: String)
    {
        lock.lock()
        if let raster
        {
            rasters[path] = raster
            failedPaths.remove(path)
        }
        else
        {
            rasters.removeValue(forKey: path)
            failedPaths.insert(path)
        }
        lock.unlock()
    }
}

private let runtimeImageRasterCache = RuntimeImageRasterCache()

private struct RuntimeImageRenderedContentCacheKey: Hashable
{
    var source: String
    var mode: String
    var width: Int
    var height: Int
    var fit: String
    var align: String
    var verticalAlign: String
    var foreground: String
    var styleBackground: String
    var background: RuntimeRgb
    var checker: Bool
    var quantized: Bool
}

private final class RuntimeImageRenderedContentCache: @unchecked Sendable
{
    private let lock = NSLock()
    private var content: [RuntimeImageRenderedContentCacheKey: [[TerminalCell]]] = [:]

    func cached(_ key: RuntimeImageRenderedContentCacheKey) -> [[TerminalCell]]?
    {
        lock.lock()
        defer
        {
            lock.unlock()
        }
        return content[key]
    }

    func store(_ rendered: [[TerminalCell]], for key: RuntimeImageRenderedContentCacheKey)
    {
        lock.lock()
        content[key] = rendered
        lock.unlock()
    }
}

private let runtimeImageRenderedContentCache = RuntimeImageRenderedContentCache()

private struct RuntimeImageSixelCacheKey: Hashable
{
    var source: String
    var width: Int
    var height: Int
    var fit: String
    var align: String
    var verticalAlign: String
    var background: RuntimeRgb
    var sourceHeight: Int
    var cropTop: Int
    var cellPixelWidth: Int
    var cellPixelHeight: Int
}

private final class RuntimeImageSixelCache: @unchecked Sendable
{
    private let lock = NSLock()
    private var payloads: [RuntimeImageSixelCacheKey: String] = [:]

    func cached(_ key: RuntimeImageSixelCacheKey) -> String?
    {
        lock.lock()
        defer
        {
            lock.unlock()
        }
        return payloads[key]
    }

    func store(_ payload: String, for key: RuntimeImageSixelCacheKey)
    {
        lock.lock()
        payloads[key] = payload
        lock.unlock()
    }
}

private let runtimeImageSixelCache = RuntimeImageSixelCache()

private let kTestFallbackCheckerRgb = RuntimeRgb(red: 255, green: 255, blue: 255)

private func loadImageRaster(_ path: String) -> RuntimeImageRaster?
{
    if let cached = runtimeImageRasterCache.cached(path)
    {
        return cached
    }

    var decoded = UimdDecodedImage()
    guard uimd_decode_image_rgba(path, &decoded) != 0,
          decoded.width > 0,
          decoded.height > 0,
          let rgba = decoded.rgba
    else
    {
        runtimeImageRasterCache.store(nil, for: path)
        return nil
    }
    defer { uimd_free_decoded_image(decoded) }

    let width = Int(decoded.width)
    let height = Int(decoded.height)
    let bytesPerPixel = 4
    var pixels: [RuntimeRgb] = []
    var alpha: [Int] = []
    pixels.reserveCapacity(width * height)
    alpha.reserveCapacity(width * height)
    for index in stride(from: 0, to: width * height * bytesPerPixel, by: bytesPerPixel)
    {
        let red = Int(rgba[index])
        let green = Int(rgba[index + 1])
        let blue = Int(rgba[index + 2])
        let pixelAlpha = Int(rgba[index + 3])
        pixels.append(RuntimeRgb(red: red, green: green, blue: blue))
        alpha.append(pixelAlpha)
    }
    let raster = RuntimeImageRaster(width: width, height: height, pixels: pixels, alpha: alpha)
    runtimeImageRasterCache.store(raster, for: path)
    return raster
}

private func imageBlankContent(width: Int, height: Int, style: Style) -> [[TerminalCell]]
{
    let blank = TerminalCell(" ", foreground: style.color, background: style.background)
    return Array(repeating: Array(repeating: blank, count: max(1, width)), count: max(1, height))
}

private func imagePlaceholderContent(width: Int, height: Int, style: Style, text: String) -> [[TerminalCell]]
{
    var content = imageBlankContent(width: width, height: height, style: style)
    let row = max(0, min(content.count - 1, max(1, height) / 2))
    let characters = Array(text)
    let startCol = max(0, (max(1, width) - characters.count) / 2)
    for index in 0..<characters.count where startCol + index < content[row].count
    {
        content[row][startCol + index] = TerminalCell(String(characters[index]), foreground: style.color, background: style.background)
    }
    return content
}

private func rgbFromColor(_ color: Color?) -> RuntimeRgb?
{
    guard let rgba = color?.rgba else
    {
        return nil
    }
    return RuntimeRgb(red: rgba.red, green: rgba.green, blue: rgba.blue)
}

private func imageFallbackBackground(style: Style, state: ElementRenderState) -> RuntimeRgb
{
    if let rgba = style.background?.rgba, rgba.alpha == 255
    {
        return RuntimeRgb(red: rgba.red, green: rgba.green, blue: rgba.blue)
    }
    return rgbFromColor(state.parentBackground) ?? RuntimeRgb(red: 0, green: 0, blue: 0)
}

private func imageRenderedContentCacheKey(
    source: String,
    mode: String,
    width: Int,
    height: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    style: Style,
    background: RuntimeRgb,
    checker: Bool = false,
    quantized: Bool = false
) -> RuntimeImageRenderedContentCacheKey
{
    RuntimeImageRenderedContentCacheKey(
        source: source,
        mode: mode,
        width: max(1, width),
        height: max(1, height),
        fit: fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        align: align.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        verticalAlign: verticalAlign.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        foreground: style.color?.snapshotValue ?? "",
        styleBackground: style.background?.snapshotValue ?? "",
        background: background,
        checker: checker,
        quantized: quantized
    )
}

private func cachedImageRenderedContent(
    key: RuntimeImageRenderedContentCacheKey,
    render: () -> [[TerminalCell]]
) -> [[TerminalCell]]
{
    if let cached = runtimeImageRenderedContentCache.cached(key)
    {
        return cached
    }
    let rendered = render()
    runtimeImageRenderedContentCache.store(rendered, for: key)
    return rendered
}

private func colorFromRgb(_ color: RuntimeRgb) -> Color
{
    Color.rgb(red: color.red, green: color.green, blue: color.blue)
}

private func blendWithBackground(_ pixel: RuntimeRgb, alpha: Int, background: RuntimeRgb) -> RuntimeRgb
{
    if alpha >= 255
    {
        return pixel
    }
    if alpha <= 0
    {
        return background
    }
    func channel(_ source: Int, _ destination: Int) -> Int
    {
        ((source * alpha) + (destination * (255 - alpha))) / 255
    }
    return RuntimeRgb(
        red: channel(pixel.red, background.red),
        green: channel(pixel.green, background.green),
        blue: channel(pixel.blue, background.blue)
    )
}

private func quantizeTestFallbackColor(_ color: RuntimeRgb) -> RuntimeRgb
{
    func channel(_ value: Int) -> Int
    {
        max(0, min(255, (value / kTestFallbackColorQuantum) * kTestFallbackColorQuantum))
    }
    return RuntimeRgb(red: channel(color.red), green: channel(color.green), blue: channel(color.blue))
}

private func testFallbackCheckerLightSquare(x: Int, y: Int) -> Bool
{
    ((x / kTestFallbackCheckerTilePixels) + (y / kTestFallbackCheckerTilePixels)) % 2 == 0
}

private func testFallbackCheckerTint(_ color: RuntimeRgb, x: Int, y: Int) -> RuntimeRgb
{
    let alpha = testFallbackCheckerLightSquare(x: x, y: y) ? kTestFallbackCheckerLightAlpha : 0
    return blendWithBackground(kTestFallbackCheckerRgb, alpha: alpha, background: color)
}

private func rasterPixel(_ raster: RuntimeImageRaster, x: Int, y: Int) -> RuntimeRgb
{
    let clampedX = max(0, min(raster.width - 1, x))
    let clampedY = max(0, min(raster.height - 1, y))
    return raster.pixels[clampedY * raster.width + clampedX]
}

private func rasterAlpha(_ raster: RuntimeImageRaster, x: Int, y: Int) -> Int
{
    let clampedX = max(0, min(raster.width - 1, x))
    let clampedY = max(0, min(raster.height - 1, y))
    return raster.alpha[clampedY * raster.width + clampedX]
}

private func channelFromImageSample(_ value: Double) -> Int
{
    max(0, min(255, Int(value.rounded())))
}

private func intervalOverlap(firstStart: Double, firstEnd: Double, secondStart: Double, secondEnd: Double) -> Double
{
    max(0.0, min(firstEnd, secondEnd) - max(firstStart, secondStart))
}

private func sampleRasterArea(
    _ source: RuntimeImageRaster,
    left: Double,
    top: Double,
    right: Double,
    bottom: Double,
    background: RuntimeRgb
) -> RuntimeRgb
{
    let fullArea = max(0.000001, (right - left) * (bottom - top))
    if source.width <= 0 ||
        source.height <= 0 ||
        source.pixels.isEmpty ||
        right <= 0.0 ||
        bottom <= 0.0 ||
        left >= Double(source.width) ||
        top >= Double(source.height)
    {
        return background
    }

    var red = Double(background.red) * fullArea
    var green = Double(background.green) * fullArea
    var blue = Double(background.blue) * fullArea
    let startX = max(0, Int(floor(left)))
    let endX = min(source.width, Int(ceil(right)))
    let startY = max(0, Int(floor(top)))
    let endY = min(source.height, Int(ceil(bottom)))

    for y in startY..<endY
    {
        let yWeight = intervalOverlap(firstStart: top, firstEnd: bottom, secondStart: Double(y), secondEnd: Double(y + 1))
        if yWeight <= 0.0
        {
            continue
        }
        for x in startX..<endX
        {
            let xWeight = intervalOverlap(firstStart: left, firstEnd: right, secondStart: Double(x), secondEnd: Double(x + 1))
            if xWeight <= 0.0
            {
                continue
            }
            let weight = xWeight * yWeight
            let alpha = Double(rasterAlpha(source, x: x, y: y)) / 255.0
            let pixel = rasterPixel(source, x: x, y: y)
            red += (Double(pixel.red) - Double(background.red)) * alpha * weight
            green += (Double(pixel.green) - Double(background.green)) * alpha * weight
            blue += (Double(pixel.blue) - Double(background.blue)) * alpha * weight
        }
    }

    return RuntimeRgb(
        red: channelFromImageSample(red / fullArea),
        green: channelFromImageSample(green / fullArea),
        blue: channelFromImageSample(blue / fullArea)
    )
}

private func resizeImageRaster(
    _ source: RuntimeImageRaster,
    targetWidth: Int,
    targetHeight: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    background: RuntimeRgb
) -> RuntimeImageRaster?
{
    let targetWidth = max(1, targetWidth)
    let targetHeight = max(1, targetHeight)
    guard source.width > 0, source.height > 0, !source.pixels.isEmpty else
    {
        return nil
    }

    let pixelCount = targetWidth * targetHeight
    var pixels = Array(repeating: background, count: pixelCount)
    var alpha = Array(repeating: 255, count: pixelCount)
    let normalizedFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    let stretch = normalizedFit == "stretch"
    let scale = stretch ? 0.0 : (
        normalizedFit == "cover"
            ? max(Double(targetWidth) / Double(source.width), Double(targetHeight) / Double(source.height))
            : min(Double(targetWidth) / Double(source.width), Double(targetHeight) / Double(source.height))
    )
    let drawnWidth = stretch ? Double(targetWidth) : max(1.0, Double(source.width) * scale)
    let drawnHeight = stretch ? Double(targetHeight) : max(1.0, Double(source.height) * scale)
    let cover = normalizedFit == "cover"
    let xOffset = stretch ? 0.0 : (
        cover
            ? alignmentOffset(outer: drawnWidth, inner: Double(targetWidth), value: align, startValue: "left", endValue: "right")
            : alignmentOffset(outer: Double(targetWidth), inner: drawnWidth, value: align, startValue: "left", endValue: "right")
    )
    let yOffset = stretch ? 0.0 : (
        cover
            ? alignmentOffset(outer: drawnHeight, inner: Double(targetHeight), value: verticalAlign, startValue: "top", endValue: "bottom")
            : alignmentOffset(outer: Double(targetHeight), inner: drawnHeight, value: verticalAlign, startValue: "top", endValue: "bottom")
    )

    for y in 0..<targetHeight
    {
        for x in 0..<targetWidth
        {
            let sourceLeft: Double
            let sourceRight: Double
            let sourceTop: Double
            let sourceBottom: Double
            if stretch
            {
                sourceLeft = Double(x) * Double(source.width) / Double(targetWidth)
                sourceRight = Double(x + 1) * Double(source.width) / Double(targetWidth)
                sourceTop = Double(y) * Double(source.height) / Double(targetHeight)
                sourceBottom = Double(y + 1) * Double(source.height) / Double(targetHeight)
            }
            else if cover
            {
                sourceLeft = (Double(x) + xOffset) / scale
                sourceRight = (Double(x + 1) + xOffset) / scale
                sourceTop = (Double(y) + yOffset) / scale
                sourceBottom = (Double(y + 1) + yOffset) / scale
            }
            else
            {
                sourceLeft = (Double(x) - xOffset) / scale
                sourceRight = (Double(x + 1) - xOffset) / scale
                sourceTop = (Double(y) - yOffset) / scale
                sourceBottom = (Double(y + 1) - yOffset) / scale
            }
            let index = y * targetWidth + x
            pixels[index] = sampleRasterArea(
                source,
                left: sourceLeft,
                top: sourceTop,
                right: sourceRight,
                bottom: sourceBottom,
                background: background
            )
            alpha[index] = 255
        }
    }
    return RuntimeImageRaster(width: targetWidth, height: targetHeight, pixels: pixels, alpha: alpha)
}

private func cropRasterRows(_ raster: RuntimeImageRaster, top: Int, height: Int) -> RuntimeImageRaster
{
    guard raster.width > 0, raster.height > 0, height > 0 else
    {
        return RuntimeImageRaster(width: raster.width, height: 0, pixels: [], alpha: [])
    }
    let start = max(0, min(raster.height, top))
    let end = max(start, min(raster.height, start + height))
    var pixels: [RuntimeRgb] = []
    var alpha: [Int] = []
    pixels.reserveCapacity((end - start) * raster.width)
    alpha.reserveCapacity((end - start) * raster.width)
    for row in start..<end
    {
        let rowStart = row * raster.width
        let rowEnd = rowStart + raster.width
        pixels.append(contentsOf: raster.pixels[rowStart..<rowEnd])
        if raster.alpha.count >= rowEnd
        {
            alpha.append(contentsOf: raster.alpha[rowStart..<rowEnd])
        }
    }
    return RuntimeImageRaster(width: raster.width, height: end - start, pixels: pixels, alpha: alpha)
}

private func quantizeSixelChannel(_ value: Int) -> Int
{
    let index = Int((Double(value) * Double(kSixelColorLevels - 1) / 255.0).rounded())
    return max(0, min(255, index * 255 / (kSixelColorLevels - 1)))
}

private func quantizeSixelRaster(_ raster: RuntimeImageRaster) -> RuntimeImageRaster
{
    let pixels = raster.pixels.map
    {
        RuntimeRgb(
            red: quantizeSixelChannel($0.red),
            green: quantizeSixelChannel($0.green),
            blue: quantizeSixelChannel($0.blue)
        )
    }
    return RuntimeImageRaster(width: raster.width, height: raster.height, pixels: pixels, alpha: raster.alpha)
}

private func sixelComponent(_ value: Int) -> Int
{
    max(0, min(kSixelColorComponentScale, Int((Double(value) * Double(kSixelColorComponentScale) / 255.0).rounded())))
}

private func appendSixelRun(_ output: inout String, ch: String, count: Int)
{
    guard count > 0 else
    {
        return
    }
    if count >= 4
    {
        output += "!\(count)\(ch)"
    }
    else
    {
        output += String(repeating: ch, count: count)
    }
}

private func sortedSixelColors(_ colors: [RuntimeRgb: Int]) -> [RuntimeRgb]
{
    colors.keys.sorted
    {
        if $0.red != $1.red
        {
            return $0.red < $1.red
        }
        if $0.green != $1.green
        {
            return $0.green < $1.green
        }
        return $0.blue < $1.blue
    }
}

private func sixelPayload(_ raster: RuntimeImageRaster) -> String
{
    guard raster.width > 0, raster.height > 0 else
    {
        return ""
    }
    var colors: [RuntimeRgb: Int] = [:]
    for index in 0..<raster.pixels.count
    {
        if raster.alpha.indices.contains(index), raster.alpha[index] == 0
        {
            continue
        }
        let color = raster.pixels[index]
        if colors[color] == nil
        {
            colors[color] = colors.count
        }
    }

    let sortedColors = sortedSixelColors(colors)
    var output = "\u{001B}Pq"
    for color in sortedColors
    {
        guard let index = colors[color] else
        {
            continue
        }
        output += "#\(index);2;\(sixelComponent(color.red));\(sixelComponent(color.green));\(sixelComponent(color.blue))"
    }

    var y = 0
    while y < raster.height
    {
        for color in sortedColors
        {
            guard let colorIndex = colors[color] else
            {
                continue
            }
            var run = ""
            var hasPixels = false
            var previous = ""
            var count = 0
            for x in 0..<raster.width
            {
                var bits = 0
                for bit in 0..<kSixelBitsPerGlyph
                {
                    let py = y + bit
                    let index = py * raster.width + x
                    if py < raster.height,
                       (!raster.alpha.indices.contains(index) || raster.alpha[index] > 0),
                       rasterPixel(raster, x: x, y: py) == color
                    {
                        bits |= 1 << bit
                    }
                }
                if bits != 0
                {
                    hasPixels = true
                }
                let ch = String(UnicodeScalar(63 + bits)!)
                if ch == previous
                {
                    count += 1
                }
                else
                {
                    appendSixelRun(&run, ch: previous, count: count)
                    previous = ch
                    count = 1
                }
            }
            if !hasPixels
            {
                continue
            }
            appendSixelRun(&run, ch: previous, count: count)
            output += "#\(colorIndex)"
            output += run
            output += "$"
        }
        output += "-"
        y += kSixelBitsPerGlyph
    }
    output += "\u{001B}\\"
    return output
}

private func libsixelPayload(_ raster: RuntimeImageRaster) -> String?
{
    guard raster.width > 0, raster.height > 0, !raster.pixels.isEmpty else
    {
        return nil
    }
    var rgb: [UInt8] = []
    rgb.reserveCapacity(raster.pixels.count * 3)
    for pixel in raster.pixels
    {
        rgb.append(UInt8(max(0, min(255, pixel.red))))
        rgb.append(UInt8(max(0, min(255, pixel.green))))
        rgb.append(UInt8(max(0, min(255, pixel.blue))))
    }

    var payload = UimdSixelPayload(data: nil, size: 0)
    let encoded = rgb.withUnsafeBufferPointer
    {
        pointer in
        uimd_encode_sixel_rgb(pointer.baseAddress, Int32(raster.width), Int32(raster.height), &payload) != 0
    }
    guard encoded,
          let data = payload.data,
          payload.size > 0
    else
    {
        uimd_free_sixel_payload(payload)
        return nil
    }
    let bytes = UnsafeBufferPointer(start: UnsafeRawPointer(data).assumingMemoryBound(to: UInt8.self), count: Int(payload.size))
    let result = String(decoding: bytes, as: UTF8.self)
    uimd_free_sixel_payload(payload)
    return result.isEmpty ? nil : result
}

private func cachedSixelPayload(
    source: String,
    raster: RuntimeImageRaster,
    width: Int,
    height: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    background: RuntimeRgb,
    sourceHeight: Int = 0,
    cropTop: Int = 0
) -> String
{
    let width = max(1, width)
    let height = max(1, height)
    let sourceHeight = sourceHeight > 0 ? sourceHeight : height
    let cropTop = max(0, cropTop)
    let cellPixels = terminalCellPixels()
    let key = RuntimeImageSixelCacheKey(
        source: source,
        width: width,
        height: height,
        fit: fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        align: align.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        verticalAlign: verticalAlign.trimmingCharacters(in: .whitespacesAndNewlines).lowercased(),
        background: background,
        sourceHeight: sourceHeight,
        cropTop: cropTop,
        cellPixelWidth: cellPixels.width,
        cellPixelHeight: cellPixels.height
    )
    if let cached = runtimeImageSixelCache.cached(key)
    {
        return cached
    }
    guard let fitted = resizeImageRaster(
        raster,
        targetWidth: width * max(1, cellPixels.width),
        targetHeight: sourceHeight * max(1, cellPixels.height),
        fit: fit,
        align: align,
        verticalAlign: verticalAlign,
        background: background
    ) else
    {
        runtimeImageSixelCache.store("", for: key)
        return ""
    }
    let payloadRaster = cropRasterRows(
        fitted,
        top: cropTop * max(1, cellPixels.height),
        height: height * max(1, cellPixels.height)
    )
    let raw = libsixelPayload(payloadRaster) ?? sixelPayload(quantizeSixelRaster(payloadRaster))
    runtimeImageSixelCache.store(raw, for: key)
    return raw
}

private func fallbackContent(raster: RuntimeImageRaster, width: Int, height: Int) -> [[TerminalCell]]
{
    guard raster.width > 0, raster.height > 0 else
    {
        return []
    }
    let width = max(1, width)
    let height = max(1, height)
    var content: [[TerminalCell]] = []
    content.reserveCapacity(height)
    for row in 0..<height
    {
        let topY = row * kFallbackVerticalSamplesPerCell
        let bottomY = min(raster.height - 1, topY + 1)
        var renderedRow: [TerminalCell] = []
        renderedRow.reserveCapacity(width)
        for col in 0..<width
        {
            renderedRow.append(TerminalCell(
                kFallbackUpperHalfBlock,
                foreground: colorFromRgb(rasterPixel(raster, x: col, y: topY)),
                background: colorFromRgb(rasterPixel(raster, x: col, y: bottomY))
            ))
        }
        content.append(renderedRow)
    }
    return content
}

private func fallbackImageContent(
    raster: RuntimeImageRaster,
    width: Int,
    height: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    style: Style,
    background: RuntimeRgb
) -> [[TerminalCell]]
{
    let width = max(1, width)
    let height = max(1, height)
    var content = imageBlankContent(width: width, height: height, style: style)
    let region = imageCellRegion(width: width, height: height, sourceWidth: raster.width, sourceHeight: raster.height, fit: fit, align: align, verticalAlign: verticalAlign)
    let regionFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain" ? "cover" : fit
    guard let resized = resizeImageRaster(
        raster,
        targetWidth: region.width,
        targetHeight: region.height * kFallbackVerticalSamplesPerCell,
        fit: regionFit,
        align: align,
        verticalAlign: verticalAlign,
        background: background
    ) else
    {
        return content
    }
    let image = fallbackContent(raster: resized, width: region.width, height: region.height)
    guard !image.isEmpty else
    {
        return content
    }
    for row in 0..<region.height
    {
        let targetRow = region.row + row
        guard targetRow >= 0 && targetRow < content.count else
        {
            continue
        }
        for col in 0..<region.width
        {
            let targetCol = region.col + col
            guard targetCol >= 0 && targetCol < content[targetRow].count else
            {
                continue
            }
            content[targetRow][targetCol] = image[row][col]
        }
    }
    return content
}

private func cellBackgroundImageContent(
    raster: RuntimeImageRaster,
    width: Int,
    height: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    style: Style,
    background: RuntimeRgb
) -> [[TerminalCell]]
{
    let width = max(1, width)
    let height = max(1, height)
    var content = imageBlankContent(width: width, height: height, style: style)
    let region = imageCellRegion(width: width, height: height, sourceWidth: raster.width, sourceHeight: raster.height, fit: fit, align: align, verticalAlign: verticalAlign)
    let regionFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain" ? "cover" : fit
    guard let image = resizeImageRaster(
        raster,
        targetWidth: region.width,
        targetHeight: region.height,
        fit: regionFit,
        align: align,
        verticalAlign: verticalAlign,
        background: background
    ) else
    {
        return content
    }
    for row in 0..<region.height
    {
        let targetRow = region.row + row
        guard targetRow >= 0 && targetRow < content.count else
        {
            continue
        }
        for col in 0..<region.width
        {
            let targetCol = region.col + col
            guard targetCol >= 0 && targetCol < content[targetRow].count else
            {
                continue
            }
            let color = colorFromRgb(rasterPixel(image, x: col, y: row))
            content[targetRow][targetCol] = TerminalCell(kFallbackFullBlock, foreground: color, background: color)
        }
    }
    return content
}

private func sampleImageColor(
    raster: RuntimeImageRaster,
    x: Int,
    y: Int,
    targetWidth: Int,
    targetHeight: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    background: RuntimeRgb,
    checker: Bool,
    quantized: Bool
) -> RuntimeRgb
{
    let targetWidth = max(1, targetWidth)
    let targetHeight = max(1, targetHeight)
    let x = max(0, min(targetWidth - 1, x))
    let y = max(0, min(targetHeight - 1, y))
    let base = checker ? testFallbackCheckerTint(background, x: x, y: y) : background
    guard raster.width > 0, raster.height > 0, !raster.pixels.isEmpty else
    {
        return quantized ? quantizeTestFallbackColor(base) : base
    }

    let normalizedFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    var sourceX = 0
    var sourceY = 0
    if normalizedFit == "stretch"
    {
        sourceX = Int(Double(x) * Double(raster.width) / Double(targetWidth))
        sourceY = Int(Double(y) * Double(raster.height) / Double(targetHeight))
    }
    else
    {
        let scale = normalizedFit == "cover"
            ? max(Double(targetWidth) / Double(raster.width), Double(targetHeight) / Double(raster.height))
            : min(Double(targetWidth) / Double(raster.width), Double(targetHeight) / Double(raster.height))
        let drawnWidth = Double(raster.width) * scale
        let drawnHeight = Double(raster.height) * scale
        let xOffset = alignmentOffset(outer: Double(targetWidth), inner: drawnWidth, value: align, startValue: "left", endValue: "right")
        let yOffset = alignmentOffset(outer: Double(targetHeight), inner: drawnHeight, value: verticalAlign, startValue: "top", endValue: "bottom")
        let sourceXFloat = (Double(x) - xOffset) / scale
        let sourceYFloat = (Double(y) - yOffset) / scale
        if sourceXFloat < 0.0 || sourceYFloat < 0.0 || sourceXFloat >= Double(raster.width) || sourceYFloat >= Double(raster.height)
        {
            return quantized ? quantizeTestFallbackColor(base) : base
        }
        sourceX = Int(sourceXFloat)
        sourceY = Int(sourceYFloat)
    }

    sourceX = max(0, min(raster.width - 1, sourceX))
    sourceY = max(0, min(raster.height - 1, sourceY))
    var pixel = blendWithBackground(rasterPixel(raster, x: sourceX, y: sourceY), alpha: rasterAlpha(raster, x: sourceX, y: sourceY), background: base)
    if checker
    {
        pixel = testFallbackCheckerTint(pixel, x: x, y: y)
    }
    return quantized ? quantizeTestFallbackColor(pixel) : pixel
}

private func deterministicImageContent(
    raster: RuntimeImageRaster,
    width: Int,
    height: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    style: Style,
    background: RuntimeRgb,
    checker: Bool,
    quantized: Bool
) -> [[TerminalCell]]
{
    let width = max(1, width)
    let height = max(1, height)
    var content = imageBlankContent(width: width, height: height, style: style)
    let region = imageCellRegion(width: width, height: height, sourceWidth: raster.width, sourceHeight: raster.height, fit: fit, align: align, verticalAlign: verticalAlign)
    let regionFit = fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain" ? "cover" : fit
    let sampleHeight = max(1, region.height * kFallbackVerticalSamplesPerCell)
    for row in region.row..<(region.row + region.height)
    {
        guard row >= 0 && row < content.count else
        {
            continue
        }
        for col in region.col..<(region.col + region.width)
        {
            guard col >= 0 && col < content[row].count else
            {
                continue
            }
            let sampleCol = col - region.col
            let sampleTop = (row - region.row) * kFallbackVerticalSamplesPerCell
            let top = sampleImageColor(
                raster: raster,
                x: sampleCol,
                y: sampleTop,
                targetWidth: region.width,
                targetHeight: sampleHeight,
                fit: regionFit,
                align: align,
                verticalAlign: verticalAlign,
                background: background,
                checker: checker,
                quantized: quantized
            )
            let bottom = sampleImageColor(
                raster: raster,
                x: sampleCol,
                y: sampleTop + 1,
                targetWidth: region.width,
                targetHeight: sampleHeight,
                fit: regionFit,
                align: align,
                verticalAlign: verticalAlign,
                background: background,
                checker: checker,
                quantized: quantized
            )
            content[row][col] = TerminalCell(kFallbackUpperHalfBlock, foreground: colorFromRgb(top), background: colorFromRgb(bottom))
        }
    }
    return content
}

private func pngImageSize(_ data: Data) -> Size?
{
    guard data.count >= 24 else
    {
        return nil
    }
    let signature = [UInt8](data.prefix(8))
    guard signature == [0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A] else
    {
        return nil
    }
    func byte(_ offset: Int) -> Int
    {
        Int(data[data.index(data.startIndex, offsetBy: offset)])
    }
    let width = (byte(16) << 24) | (byte(17) << 16) | (byte(18) << 8) | byte(19)
    let height = (byte(20) << 24) | (byte(21) << 16) | (byte(22) << 8) | byte(23)
    guard width > 0, height > 0 else
    {
        return nil
    }
    return Size(width: width, height: height)
}

private func bmpImageSize(_ data: Data) -> Size?
{
    guard data.count >= 26,
          data[data.startIndex] == 0x42,
          data[data.index(data.startIndex, offsetBy: 1)] == 0x4D
    else
    {
        return nil
    }
    let width = littleEndianInt32(data, offset: 18)
    let height = abs(littleEndianInt32(data, offset: 22))
    guard width > 0, height > 0 else
    {
        return nil
    }
    return Size(width: width, height: height)
}

private func gifImageSize(_ data: Data) -> Size?
{
    guard data.count >= 10 else
    {
        return nil
    }
    let header = String(decoding: data.prefix(6), as: UTF8.self)
    guard header == "GIF87a" || header == "GIF89a" else
    {
        return nil
    }
    let width = littleEndianUInt16(data, offset: 6)
    let height = littleEndianUInt16(data, offset: 8)
    guard width > 0, height > 0 else
    {
        return nil
    }
    return Size(width: width, height: height)
}

private func jpegImageSize(_ data: Data) -> Size?
{
    guard data.count >= 4,
          byteAt(data, 0) == 0xFF,
          byteAt(data, 1) == 0xD8
    else
    {
        return nil
    }
    var offset = 2
    while offset + 9 < data.count
    {
        while offset < data.count && byteAt(data, offset) != 0xFF
        {
            offset += 1
        }
        while offset < data.count && byteAt(data, offset) == 0xFF
        {
            offset += 1
        }
        guard offset < data.count else
        {
            return nil
        }
        let marker = byteAt(data, offset)
        offset += 1
        if marker == 0xD9 || marker == 0xDA
        {
            return nil
        }
        guard offset + 1 < data.count else
        {
            return nil
        }
        let length = bigEndianUInt16(data, offset: offset)
        guard length >= 2, offset + length <= data.count else
        {
            return nil
        }
        if jpegStartOfFrameMarkers.contains(marker)
        {
            guard offset + 7 < data.count else
            {
                return nil
            }
            let height = bigEndianUInt16(data, offset: offset + 3)
            let width = bigEndianUInt16(data, offset: offset + 5)
            guard width > 0, height > 0 else
            {
                return nil
            }
            return Size(width: width, height: height)
        }
        offset += length
    }
    return nil
}

private let jpegStartOfFrameMarkers: Set<Int> = [
    0xC0, 0xC1, 0xC2, 0xC3,
    0xC5, 0xC6, 0xC7,
    0xC9, 0xCA, 0xCB,
    0xCD, 0xCE, 0xCF,
]

private func tgaImageSize(_ data: Data) -> Size?
{
    guard data.count >= 18 else
    {
        return nil
    }
    let imageType = byteAt(data, 2)
    guard [1, 2, 3, 9, 10, 11].contains(imageType) else
    {
        return nil
    }
    let width = littleEndianUInt16(data, offset: 12)
    let height = littleEndianUInt16(data, offset: 14)
    guard width > 0, height > 0 else
    {
        return nil
    }
    return Size(width: width, height: height)
}

private func portableAnyMapImageSize(_ data: Data) -> Size?
{
    guard let text = String(data: data.prefix(min(data.count, 512)), encoding: .ascii) else
    {
        return nil
    }
    var tokens: [String] = []
    for line in text.split(separator: "\n", omittingEmptySubsequences: false)
    {
        let content = line.split(separator: "#", maxSplits: 1, omittingEmptySubsequences: false).first ?? ""
        tokens.append(contentsOf: content.split { $0 == " " || $0 == "\t" || $0 == "\r" || $0 == "\n" }.map(String.init))
        if tokens.count >= 3
        {
            break
        }
    }
    guard tokens.count >= 3,
          ["P2", "P3", "P5", "P6"].contains(tokens[0]),
          let width = Int(tokens[1]),
          let height = Int(tokens[2]),
          width > 0,
          height > 0
    else
    {
        return nil
    }
    return Size(width: width, height: height)
}

private func byteAt(_ data: Data, _ offset: Int) -> Int
{
    Int(data[data.index(data.startIndex, offsetBy: offset)])
}

private func littleEndianUInt16(_ data: Data, offset: Int) -> Int
{
    byteAt(data, offset) | (byteAt(data, offset + 1) << 8)
}

private func bigEndianUInt16(_ data: Data, offset: Int) -> Int
{
    (byteAt(data, offset) << 8) | byteAt(data, offset + 1)
}

private func littleEndianInt32(_ data: Data, offset: Int) -> Int
{
    let raw = UInt32(byteAt(data, offset))
        | (UInt32(byteAt(data, offset + 1)) << 8)
        | (UInt32(byteAt(data, offset + 2)) << 16)
        | (UInt32(byteAt(data, offset + 3)) << 24)
    return Int(Int32(bitPattern: raw))
}

private func resolvedImageRenderMode(_ image: Image) -> String
{
    if truthyEnvironment("UIMD_DETERMINISTIC_IMAGE_FALLBACK")
    {
        return "deterministic"
    }
    let mode = image.renderMode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if mode == "fallback"
    {
        return "fallback"
    }
    if mode == "sixel"
    {
        return terminalSupportsSixel() ? "sixel" : "fallback"
    }
    return terminalSupportsSixel() ? "sixel" : "fallback"
}

private func terminalSupportsSixel() -> Bool
{
    if truthyEnvironment("UIMD_FORCE_SIXEL")
    {
        return true
    }
    if truthyEnvironment("UIMD_DISABLE_SIXEL")
    {
        return false
    }
    let environment = ProcessInfo.processInfo.environment
    let termProgram = (environment["TERM_PROGRAM"] ?? "").trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    let term = (environment["TERM"] ?? "").trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    let colorTerm = (environment["COLORTERM"] ?? "").trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    let itermSession = (environment["ITERM_SESSION_ID"] ?? "").trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    let lcTerminal = (environment["LC_TERMINAL"] ?? "").trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if termProgram.contains("apple_terminal")
    {
        return false
    }
    if !itermSession.isEmpty || lcTerminal.contains("iterm")
    {
        return true
    }
    if ["iterm", "wezterm", "mlterm", "foot", "contour"].contains(where: { termProgram.contains($0) })
    {
        return true
    }
    if term.contains("sixel") || colorTerm.contains("sixel")
    {
        return true
    }
    return ["mlterm", "foot", "contour"].contains(where: { term.contains($0) })
}

private func truthyEnvironment(_ name: String) -> Bool
{
    let value = ProcessInfo.processInfo.environment[name]?.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() ?? ""
    return ["1", "true", "yes", "on"].contains(value)
}

private func imageModeNeedsSixelFallbackWarning(_ renderMode: String) -> Bool
{
    if truthyEnvironment("UIMD_DETERMINISTIC_IMAGE_FALLBACK") ||
        truthyEnvironment("UIMD_DISABLE_SIXEL") ||
        truthyEnvironment("UIMD_FORCE_SIXEL")
    {
        return false
    }
    let mode = renderMode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if mode == "fallback"
    {
        return false
    }
    return !terminalSupportsSixel()
}

private func imageCellRegion(
    width: Int,
    height: Int,
    sourceWidth: Int,
    sourceHeight: Int,
    fit: String,
    align: String,
    verticalAlign: String
) -> Rect
{
    let width = max(1, width)
    let height = max(1, height)
    guard fit.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() == "contain",
          sourceWidth > 0,
          sourceHeight > 0
    else
    {
        return Rect(row: 0, col: 0, width: width, height: height)
    }
    let cellPixels = terminalCellPixels()
    let scale = min(
        Double(width * cellPixels.width) / Double(sourceWidth),
        Double(height * cellPixels.height) / Double(sourceHeight)
    )
    let cols = max(1, min(width, Int((Double(sourceWidth) * scale / Double(cellPixels.width)).rounded())))
    let rows = max(1, min(height, Int((Double(sourceHeight) * scale / Double(cellPixels.height)).rounded())))
    return Rect(
        row: alignmentOffset(outer: height, inner: rows, value: verticalAlign, startValue: "top", endValue: "bottom"),
        col: alignmentOffset(outer: width, inner: cols, value: align, startValue: "left", endValue: "right"),
        width: cols,
        height: rows
    )
}

private func alignmentOffset(outer: Int, inner: Int, value: String, startValue: String, endValue: String) -> Int
{
    let span = max(0, outer - inner)
    let normalized = value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if normalized == startValue
    {
        return 0
    }
    if normalized == endValue
    {
        return span
    }
    return span / 2
}

private func alignmentOffset(outer: Double, inner: Double, value: String, startValue: String, endValue: String) -> Double
{
    let span = max(0.0, outer - inner)
    let normalized = value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if normalized == startValue
    {
        return 0.0
    }
    if normalized == endValue
    {
        return span
    }
    return span / 2.0
}

private func imageInfoSampleSignature(
    raster: RuntimeImageRaster,
    targetWidth: Int,
    targetHeight: Int,
    fit: String,
    align: String,
    verticalAlign: String,
    background: RuntimeRgb
) -> [String]
{
    let targetWidth = max(1, targetWidth)
    let targetHeight = max(1, targetHeight)
    guard let fitted = resizeImageRaster(
        raster,
        targetWidth: targetWidth,
        targetHeight: targetHeight,
        fit: fit,
        align: align,
        verticalAlign: verticalAlign,
        background: background
    ) else
    {
        return []
    }
    var signature: [String] = []
    signature.reserveCapacity(kImageInfoSampleGridSize * kImageInfoSampleGridSize)
    for row in imageInfoSamplePositions(extent: targetHeight)
    {
        for col in imageInfoSamplePositions(extent: targetWidth)
        {
            signature.append(rgbHex(quantizeImageInfoColor(rasterPixel(fitted, x: col, y: row))))
        }
    }
    return signature
}

private func imageInfoSamplePositions(extent: Int) -> [Int]
{
    let extent = max(1, extent)
    if kImageInfoSampleGridSize == 1
    {
        return [extent / 2]
    }
    return (0..<kImageInfoSampleGridSize).map
    {
        index in
        let position = Int((Double(index) * Double(extent - 1) / Double(kImageInfoSampleGridSize - 1)).rounded())
        return max(0, min(extent - 1, position))
    }
}

private func quantizeImageInfoColor(_ color: RuntimeRgb) -> RuntimeRgb
{
    func channel(_ value: Int) -> Int
    {
        max(0, min(255, (value / kImageInfoColorQuantum) * kImageInfoColorQuantum))
    }
    return RuntimeRgb(red: channel(color.red), green: channel(color.green), blue: channel(color.blue))
}

private func rgbHex(_ color: RuntimeRgb) -> String
{
    String(format: "#%02x%02x%02x", color.red, color.green, color.blue)
}

private struct RuntimeCell: Hashable
{
    var sourceRow: Int
    var sourceCol: Int
    var sourceWidth: Int
    var sourceHeight: Int
    var logicalWidth: Int?
    var logicalHeight: Int?
}

private final class RuntimeNode
{
    var cell: RuntimeCell?
    var orientation = ""
    var children: [RuntimeNode] = []
    var left = 0
    var top = 0
    var right = 0
    var bottom = 0
    var logicalWidth: Int?
    var logicalHeight: Int?
}

private struct ResolvedRuntimeCell
{
    var cell: RuntimeCell
    var rect: Rect
}

private func axisValue(_ dimension: AxisDimension) -> Int
{
    switch dimension
    {
    case .auto(let value), .fixed(let value):
        return value
    case .expanded, .fitContent:
        return 0
    }
}

private func axisIsExpanded(_ dimension: AxisDimension) -> Bool
{
    if case .expanded = dimension
    {
        return true
    }
    return false
}

private func axisIsFixed(_ dimension: AxisDimension) -> Bool
{
    if case .fixed = dimension
    {
        return true
    }
    return false
}

private func axisIsFitContent(_ dimension: AxisDimension) -> Bool
{
    if case .fitContent = dimension
    {
        return true
    }
    return false
}

private func paddingTop(_ style: Style) -> Int
{
    max(0, style.paddingTop ?? style.padding ?? 0)
}

private func paddingRight(_ style: Style) -> Int
{
    max(0, style.paddingRight ?? style.padding ?? 0)
}

private func paddingBottom(_ style: Style) -> Int
{
    max(0, style.paddingBottom ?? style.padding ?? 0)
}

private func paddingLeft(_ style: Style) -> Int
{
    max(0, style.paddingLeft ?? style.padding ?? 0)
}

private func marginTop(_ style: Style) -> Int
{
    max(0, style.marginTop ?? style.margin ?? 0)
}

private func marginRight(_ style: Style) -> Int
{
    max(0, style.marginRight ?? style.margin ?? 0)
}

private func marginBottom(_ style: Style) -> Int
{
    max(0, style.marginBottom ?? style.margin ?? 0)
}

private func marginLeft(_ style: Style) -> Int
{
    max(0, style.marginLeft ?? style.margin ?? 0)
}

private func runtimeBorderWidthHorizontal(_ style: Style) -> Int
{
    max(0, style.borderWidthHorizontal ?? 0)
}

private func runtimeBorderWidthVertical(_ style: Style) -> Int
{
    max(0, style.borderWidthVertical ?? 0)
}

private func sameRuntimeCell(_ cell: RuntimeCell, _ entry: GeneratedLayoutEntry) -> Bool
{
    cell.sourceRow == entry.sourceCell.row
        && cell.sourceCol == entry.sourceCell.col
        && cell.sourceWidth == entry.sourceCell.width
        && cell.sourceHeight == entry.sourceCell.height
}

private func sameSourceCell(_ lhs: GeneratedLayoutEntry, _ rhs: GeneratedLayoutEntry) -> Bool
{
    lhs.sourceCell.row == rhs.sourceCell.row
        && lhs.sourceCell.col == rhs.sourceCell.col
        && lhs.sourceCell.width == rhs.sourceCell.width
        && lhs.sourceCell.height == rhs.sourceCell.height
}

private func logicalCellSize(_ dimension: AxisDimension, sourceSize: Int) -> Int?
{
    if axisIsExpanded(dimension)
    {
        return nil
    }
    if axisIsFixed(dimension)
    {
        return max(1, axisValue(dimension))
    }
    let value = axisValue(dimension)
    return max(1, value > 0 ? value : sourceSize)
}

private func textLineCount(_ text: String) -> Int
{
    if text.isEmpty
    {
        return 0
    }
    return text.filter { $0 == "\n" }.count + 1
}

private func wrappedTextLineCount(_ text: String, width: Int) -> Int
{
    let width = max(1, width)
    if text.isEmpty
    {
        return 1
    }
    var rows = 0
    for segment in text.split(separator: "\n", omittingEmptySubsequences: false)
    {
        let count = visualGlyphs(String(segment)).count
        rows += max(1, Int(ceil(Double(count) / Double(width))))
    }
    return max(1, rows)
}

private func fitContentHeightFor(_ element: UIElement?, type: String, fallback: Int, width: Int?) -> Int
{
    if let table = element as? MessageTable
    {
        return table.fitContentHeight()
    }
    if let list = element as? ListBox, !list.options.isEmpty
    {
        return max(1, list.options.count)
    }
    if let label = element as? Label
    {
        if let width, type != "spanlabel" && type != "infolabel"
        {
            return wrappedTextLineCount(label.text, width: width)
        }
        let lines = textLineCount(label.text)
        return lines > 0 ? max(1, lines) : 1
    }
    return max(1, fallback)
}

private func fitEntryHeight(_ window: GeneratedWindowBase, _ entry: GeneratedLayoutEntry, availableCellWidth: Int? = nil) -> Int
{
    let element = window.element(named: entry.name)
    let fallback = entry.charsSize.height > 0 ? entry.charsSize.height : entry.sourceCell.height
    let contentWidth = availableCellWidth.map { max(1, $0 - entry.relative.col - entry.marginRight) }
    let contentHeight = axisIsFitContent(entry.height)
        ? fitContentHeightFor(element, type: entry.type, fallback: fallback, width: contentWidth)
        : max(1, fallback)
    return max(1, entry.relative.row + contentHeight + entry.marginBottom)
}

private func fitCellHeight(_ window: GeneratedWindowBase, _ cellEntry: GeneratedLayoutEntry, resolvedCellWidth: Int? = nil) -> Int
{
    var contentHeight = cellEntry.sourceCell.height
    let availableCellWidth = resolvedCellWidth.map
    {
        max(1, $0 - paddingLeft(cellEntry.cellStyle) - paddingRight(cellEntry.cellStyle))
    }
    for entry in window.layout
    {
        if sameSourceCell(entry, cellEntry)
        {
            contentHeight = max(contentHeight, fitEntryHeight(window, entry, availableCellWidth: availableCellWidth))
        }
    }
    return max(1, contentHeight + paddingTop(cellEntry.cellStyle) + paddingBottom(cellEntry.cellStyle))
}

private func fitEntryWidth(_ window: GeneratedWindowBase, _ entry: GeneratedLayoutEntry) -> Int
{
    let element = window.element(named: entry.name)
    let fallback = entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width
    var contentWidth = fallback
    if let list = element as? ListBox
    {
        for option in list.options
        {
            contentWidth = max(contentWidth, visualGlyphs(option).count)
        }
    }
    else if let label = element as? Label
    {
        var lineWidth = 0
        for character in label.text
        {
            if character == "\n"
            {
                contentWidth = max(contentWidth, lineWidth)
                lineWidth = 0
            }
            else if character != "\r"
            {
                lineWidth += 1
            }
        }
        contentWidth = max(contentWidth, lineWidth)
    }
    return max(1, entry.relative.col + contentWidth + entry.marginRight)
}

private func fitCellWidth(_ window: GeneratedWindowBase, _ cellEntry: GeneratedLayoutEntry) -> Int
{
    var contentWidth = cellEntry.sourceCell.width
    for entry in window.layout
    {
        if sameSourceCell(entry, cellEntry)
        {
            contentWidth = max(contentWidth, fitEntryWidth(window, entry))
        }
    }
    return max(1, contentWidth + paddingLeft(cellEntry.cellStyle) + paddingRight(cellEntry.cellStyle))
}

private func collectRuntimeCells(_ window: GeneratedWindowBase) -> [RuntimeCell]
{
    var cells: [RuntimeCell] = []
    for entry in window.layout
    {
        if cells.contains(where: { sameRuntimeCell($0, entry) })
        {
            continue
        }
        var logicalWidth = logicalCellSize(entry.cellWidth, sourceSize: entry.sourceCell.width)
        var logicalHeight = logicalCellSize(entry.cellHeight, sourceSize: entry.sourceCell.height)
        if axisIsFitContent(entry.cellWidth)
        {
            logicalWidth = fitCellWidth(window, entry)
        }
        if axisIsFitContent(entry.cellHeight)
        {
            logicalHeight = fitCellHeight(window, entry)
        }
        if let width = logicalWidth, !axisIsExpanded(entry.cellWidth), !axisIsFitContent(entry.cellWidth)
        {
            logicalWidth = max(1, width + paddingLeft(entry.cellStyle) + paddingRight(entry.cellStyle))
        }
        if let height = logicalHeight, !axisIsExpanded(entry.cellHeight), !axisIsFitContent(entry.cellHeight)
        {
            logicalHeight = max(1, height + paddingTop(entry.cellStyle) + paddingBottom(entry.cellStyle))
        }
        cells.append(RuntimeCell(
            sourceRow: entry.sourceCell.row,
            sourceCol: entry.sourceCell.col,
            sourceWidth: max(1, entry.sourceCell.width),
            sourceHeight: max(1, entry.sourceCell.height),
            logicalWidth: logicalWidth,
            logicalHeight: logicalHeight
        ))
    }
    return cells
}

private func makeLeafNode(_ cell: RuntimeCell) -> RuntimeNode
{
    let node = RuntimeNode()
    node.cell = cell
    node.left = cell.sourceCol
    node.top = cell.sourceRow
    node.right = node.left + cell.sourceWidth + 1
    node.bottom = node.top + cell.sourceHeight + 1
    node.logicalWidth = cell.logicalWidth
    node.logicalHeight = cell.logicalHeight
    return node
}

private func intervalsCover(_ spans: [(Int, Int)], start: Int, end: Int) -> Bool
{
    if spans.isEmpty
    {
        return false
    }
    let sorted = spans.sorted { $0.0 == $1.0 ? $0.1 < $1.1 : $0.0 < $1.0 }
    var cursor = start
    for span in sorted
    {
        if span.0 > cursor
        {
            return false
        }
        cursor = max(cursor, span.1)
        if cursor >= end
        {
            return true
        }
    }
    return cursor >= end
}

private func horizontalCuts(_ nodes: [RuntimeNode], left: Int, top: Int, right: Int, bottom: Int) -> [Int]
{
    let candidates = Array(Set(nodes.flatMap { [$0.top, $0.bottom] })).sorted()
    var cuts: [Int] = []
    for candidate in candidates
    {
        if candidate <= top || candidate >= bottom
        {
            continue
        }
        let spans = nodes.compactMap
        {
            node -> (Int, Int)? in
            node.top == candidate || node.bottom == candidate ? (node.left, node.right) : nil
        }
        if intervalsCover(spans, start: left, end: right)
        {
            cuts.append(candidate)
        }
    }
    return cuts
}

private func verticalCuts(_ nodes: [RuntimeNode], left: Int, top: Int, right: Int, bottom: Int) -> [Int]
{
    let candidates = Array(Set(nodes.flatMap { [$0.left, $0.right] })).sorted()
    var cuts: [Int] = []
    for candidate in candidates
    {
        if candidate <= left || candidate >= right
        {
            continue
        }
        let spans = nodes.compactMap
        {
            node -> (Int, Int)? in
            node.left == candidate || node.right == candidate ? (node.top, node.bottom) : nil
        }
        if intervalsCover(spans, start: top, end: bottom)
        {
            cuts.append(candidate)
        }
    }
    return cuts
}

private func sliceByRows(_ nodes: [RuntimeNode], boundaries: [Int]) -> [[RuntimeNode]]
{
    var groups: [[RuntimeNode]] = []
    var used = Array(repeating: false, count: nodes.count)
    for index in 0..<(max(0, boundaries.count - 1))
    {
        let bandTop = boundaries[index]
        let bandBottom = boundaries[index + 1]
        var group: [RuntimeNode] = []
        for nodeIndex in nodes.indices
        {
            if nodes[nodeIndex].top >= bandTop && nodes[nodeIndex].bottom <= bandBottom
            {
                group.append(nodes[nodeIndex])
                used[nodeIndex] = true
            }
        }
        if group.isEmpty
        {
            return []
        }
        group.sort
        {
            ($0.top, $0.left) < ($1.top, $1.left)
        }
        groups.append(group)
    }
    return used.allSatisfy { $0 } ? groups : []
}

private func sliceByCols(_ nodes: [RuntimeNode], boundaries: [Int]) -> [[RuntimeNode]]
{
    var groups: [[RuntimeNode]] = []
    var used = Array(repeating: false, count: nodes.count)
    for index in 0..<(max(0, boundaries.count - 1))
    {
        let bandLeft = boundaries[index]
        let bandRight = boundaries[index + 1]
        var group: [RuntimeNode] = []
        for nodeIndex in nodes.indices
        {
            if nodes[nodeIndex].left >= bandLeft && nodes[nodeIndex].right <= bandRight
            {
                group.append(nodes[nodeIndex])
                used[nodeIndex] = true
            }
        }
        if group.isEmpty
        {
            return []
        }
        group.sort
        {
            ($0.left, $0.top) < ($1.left, $1.top)
        }
        groups.append(group)
    }
    return used.allSatisfy { $0 } ? groups : []
}

private func makeGroupNode(
    _ orientation: String,
    children: [RuntimeNode],
    left: Int,
    top: Int,
    right: Int,
    bottom: Int,
    horizontalSeparator: Int,
    verticalSeparator: Int
) -> RuntimeNode
{
    let node = RuntimeNode()
    node.orientation = orientation
    node.children = children
    node.left = left
    node.top = top
    node.right = right
    node.bottom = bottom

    if orientation == "vertical"
    {
        for child in children
        {
            guard let childWidth = child.logicalWidth else
            {
                node.logicalWidth = nil
                break
            }
            node.logicalWidth = max(node.logicalWidth ?? 1, childWidth)
        }
        var height = 0
        for child in children
        {
            guard let childHeight = child.logicalHeight else
            {
                node.logicalHeight = nil
                return node
            }
            height += childHeight
        }
        height += max(0, children.count - 1) * horizontalSeparator
        node.logicalHeight = height
    }
    else
    {
        for child in children
        {
            guard let childHeight = child.logicalHeight else
            {
                node.logicalHeight = nil
                break
            }
            node.logicalHeight = max(node.logicalHeight ?? 1, childHeight)
        }
        var width = 0
        for child in children
        {
            guard let childWidth = child.logicalWidth else
            {
                node.logicalWidth = nil
                return node
            }
            width += childWidth
        }
        width += max(0, children.count - 1) * verticalSeparator
        node.logicalWidth = width
    }
    return node
}

private func buildRuntimeTree(_ cells: [RuntimeCell], horizontalSeparator: Int = 0, verticalSeparator: Int = 0) -> RuntimeNode?
{
    buildRuntimeTree(cells.map { makeLeafNode($0) }, horizontalSeparator: horizontalSeparator, verticalSeparator: verticalSeparator)
}

private func buildRuntimeTree(_ inputNodes: [RuntimeNode], horizontalSeparator: Int = 0, verticalSeparator: Int = 0) -> RuntimeNode?
{
    var nodes = inputNodes
    if nodes.isEmpty
    {
        return nil
    }
    if nodes.count == 1
    {
        return nodes[0]
    }
    nodes.sort
    {
        ($0.top, $0.left, $0.bottom, $0.right) < ($1.top, $1.left, $1.bottom, $1.right)
    }
    let left = nodes.map(\.left).min() ?? 0
    let top = nodes.map(\.top).min() ?? 0
    let right = nodes.map(\.right).max() ?? 0
    let bottom = nodes.map(\.bottom).max() ?? 0

    var cuts = horizontalCuts(nodes, left: left, top: top, right: right, bottom: bottom)
    if !cuts.isEmpty
    {
        var boundaries = [top]
        boundaries.append(contentsOf: cuts)
        boundaries.append(bottom)
        let groups = sliceByRows(nodes, boundaries: boundaries)
        if groups.count > 1
        {
            let children = groups.compactMap { buildRuntimeTree($0, horizontalSeparator: horizontalSeparator, verticalSeparator: verticalSeparator) }
            return makeGroupNode(
                "vertical",
                children: children,
                left: left,
                top: top,
                right: right,
                bottom: bottom,
                horizontalSeparator: horizontalSeparator,
                verticalSeparator: verticalSeparator
            )
        }
    }

    cuts = verticalCuts(nodes, left: left, top: top, right: right, bottom: bottom)
    if !cuts.isEmpty
    {
        var boundaries = [left]
        boundaries.append(contentsOf: cuts)
        boundaries.append(right)
        let groups = sliceByCols(nodes, boundaries: boundaries)
        if groups.count > 1
        {
            let children = groups.compactMap { buildRuntimeTree($0, horizontalSeparator: horizontalSeparator, verticalSeparator: verticalSeparator) }
            return makeGroupNode(
                "horizontal",
                children: children,
                left: left,
                top: top,
                right: right,
                bottom: bottom,
                horizontalSeparator: horizontalSeparator,
                verticalSeparator: verticalSeparator
            )
        }
    }

    return nodes[0]
}

private func resolveRuntimeNode(_ node: RuntimeNode?, rect inputRect: Rect, resolved: inout [ResolvedRuntimeCell], horizontalSeparator: Int, verticalSeparator: Int, mode: String)
{
    guard let node else
    {
        return
    }
    let rect = Rect(row: inputRect.row, col: inputRect.col, width: max(1, inputRect.width), height: max(1, inputRect.height))
    if node.children.isEmpty
    {
        if let cell = node.cell
        {
            resolved.append(ResolvedRuntimeCell(cell: cell, rect: rect))
        }
        return
    }
    if node.orientation == "vertical"
    {
        var fixedTotal = 0
        var expandedCount = 0
        for child in node.children
        {
            if let height = child.logicalHeight
            {
                fixedTotal += height
            }
            else
            {
                expandedCount += 1
            }
        }
        let separators = max(0, node.children.count - 1) * horizontalSeparator
        let remaining = max(0, rect.height - fixedTotal - separators)
        let distributeFixedExtra = expandedCount == 0 && remaining > 0 && mode == "fullscreen"
        let appendTrailingExtra = expandedCount == 0 && remaining > 0 && mode != "fullscreen" && horizontalSeparator > 0
        let expandTargets = distributeFixedExtra ? node.children.count : expandedCount
        let share = expandTargets > 0 ? remaining / expandTargets : 0
        let remainder = expandTargets > 0 ? remaining % expandTargets : 0
        var expandIndex = 0
        var cursor = rect.row
        for index in node.children.indices
        {
            let child = node.children[index]
            var childHeight = child.logicalHeight ?? 0
            if child.logicalHeight == nil || distributeFixedExtra
            {
                childHeight = (distributeFixedExtra ? childHeight : 0) + share + (expandIndex < remainder ? 1 : 0)
                expandIndex += 1
            }
            else if appendTrailingExtra && index + 1 == node.children.count
            {
                childHeight += remaining
            }
            resolveRuntimeNode(
                child,
                rect: Rect(row: cursor, col: rect.col, width: rect.width, height: max(1, childHeight)),
                resolved: &resolved,
                horizontalSeparator: horizontalSeparator,
                verticalSeparator: verticalSeparator,
                mode: mode
            )
            cursor += max(1, childHeight) + horizontalSeparator
        }
        return
    }

    var fixedTotal = 0
    var expandedCount = 0
    for child in node.children
    {
        if let width = child.logicalWidth
        {
            fixedTotal += width
        }
        else
        {
            expandedCount += 1
        }
    }
    let separators = max(0, node.children.count - 1) * verticalSeparator
    let remaining = max(0, rect.width - fixedTotal - separators)
    let distributeFixedExtra = expandedCount == 0 && remaining > 0 && mode == "fullscreen"
    let appendTrailingExtra = expandedCount == 0 && remaining > 0 && mode != "fullscreen"
    let expandTargets = distributeFixedExtra ? node.children.count : expandedCount
    let share = expandTargets > 0 ? remaining / expandTargets : 0
    let remainder = expandTargets > 0 ? remaining % expandTargets : 0
    var expandIndex = 0
    var cursor = rect.col
    for index in node.children.indices
    {
        let child = node.children[index]
        var childWidth = child.logicalWidth ?? 0
        if child.logicalWidth == nil || distributeFixedExtra
        {
            childWidth = (distributeFixedExtra ? childWidth : 0) + share + (expandIndex < remainder ? 1 : 0)
            expandIndex += 1
        }
        else if appendTrailingExtra && index + 1 == node.children.count
        {
            childWidth += remaining
        }
        resolveRuntimeNode(
            child,
            rect: Rect(row: rect.row, col: cursor, width: max(1, childWidth), height: rect.height),
            resolved: &resolved,
            horizontalSeparator: horizontalSeparator,
            verticalSeparator: verticalSeparator,
            mode: mode
        )
        cursor += max(1, childWidth) + verticalSeparator
    }
}

private func runtimeLayoutSize(_ root: RuntimeNode?) -> Size
{
    guard let root else
    {
        return Size(width: 1, height: 1)
    }
    let sourceWidth = max(1, root.right - root.left - 1)
    let sourceHeight = max(1, root.bottom - root.top - 1)
        return Size(width: max(1, root.logicalWidth ?? sourceWidth), height: max(1, root.logicalHeight ?? sourceHeight))
}

private func layoutContentRect(_ runtimeTree: RuntimeNode?, content: Rect, mode: String) -> Rect
{
    let natural = runtimeLayoutSize(runtimeTree)
    var result = content
    if mode == "fullscreen" || mode == "expand_width"
    {
        result.width = max(result.width, natural.width)
    }
    if mode == "fullscreen" || mode == "expand_height"
    {
        result.height = max(result.height, natural.height)
    }
    return result
}

private func resolveRuntimeCellsWithFitPass(_ window: GeneratedWindowBase, runtimeCells: inout [RuntimeCell], content: Rect, mode: String, resolvedCells: inout [ResolvedRuntimeCell])
{
    let horizontalSeparator = runtimeBorderWidthHorizontal(window.generatedWindowStyle)
    let verticalSeparator = runtimeBorderWidthVertical(window.generatedWindowStyle)
    var runtimeTree = buildRuntimeTree(runtimeCells, horizontalSeparator: horizontalSeparator, verticalSeparator: verticalSeparator)
    resolvedCells.removeAll()
    resolveRuntimeNode(
        runtimeTree,
        rect: layoutContentRect(runtimeTree, content: content, mode: mode),
        resolved: &resolvedCells,
        horizontalSeparator: horizontalSeparator,
        verticalSeparator: verticalSeparator,
        mode: mode
    )
    var changed = false
    for index in runtimeCells.indices
    {
        guard let cellEntry = window.layout.first(where: { sameRuntimeCell(runtimeCells[index], $0) && axisIsFitContent($0.cellHeight) }),
              let resolved = resolvedCellFor(resolvedCells, entry: cellEntry)
        else
        {
            continue
        }
        let newHeight = fitCellHeight(window, cellEntry, resolvedCellWidth: resolved.rect.width)
        if runtimeCells[index].logicalHeight != newHeight
        {
            runtimeCells[index].logicalHeight = newHeight
            changed = true
        }
    }
    if changed
    {
        runtimeTree = buildRuntimeTree(runtimeCells, horizontalSeparator: horizontalSeparator, verticalSeparator: verticalSeparator)
        resolvedCells.removeAll()
        resolveRuntimeNode(
            runtimeTree,
            rect: layoutContentRect(runtimeTree, content: content, mode: mode),
            resolved: &resolvedCells,
            horizontalSeparator: horizontalSeparator,
            verticalSeparator: verticalSeparator,
            mode: mode
        )
    }
}

private func resolvedContentExtent(_ resolvedCells: [ResolvedRuntimeCell], fallback: Size) -> Size
{
    var result = Size(width: max(1, fallback.width), height: max(1, fallback.height))
    for cell in resolvedCells
    {
        result.width = max(result.width, cell.rect.col + cell.rect.width)
        result.height = max(result.height, cell.rect.row + cell.rect.height)
    }
    return result
}

private func resolvedCellFor(_ cells: [ResolvedRuntimeCell], entry: GeneratedLayoutEntry) -> ResolvedRuntimeCell?
{
    cells.first { sameRuntimeCell($0.cell, entry) }
}

private func dimensionWithin(_ dimension: AxisDimension, available: Int, fallback: Int) -> Int
{
    let available = max(1, available)
    if axisIsExpanded(dimension)
    {
        return available
    }
    if axisIsFitContent(dimension)
    {
        return max(1, min(max(1, fallback), available))
    }
    let requested = axisValue(dimension) > 0 ? axisValue(dimension) : fallback
    return max(1, requested)
}

private func renderSizeFor(_ entry: GeneratedLayoutEntry, cellRect: Rect, element: UIElement?) -> Size
{
    let availableWidth = cellRect.width - paddingLeft(entry.cellStyle) - paddingRight(entry.cellStyle) - entry.relative.col - entry.marginRight
    let availableHeight = cellRect.height - paddingTop(entry.cellStyle) - paddingBottom(entry.cellStyle) - entry.relative.row - entry.marginBottom
    let fallbackWidth = entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width
    var fallbackHeight = entry.charsSize.height > 0 ? entry.charsSize.height : entry.sourceCell.height
    if axisIsFitContent(entry.height)
    {
        fallbackHeight = fitContentHeightFor(element, type: entry.type, fallback: fallbackHeight, width: max(1, availableWidth))
    }
    if element == nil && entry.type == "label"
    {
        let availableStaticWidth = cellRect.width - paddingLeft(entry.cellStyle) - entry.relative.col
        return Size(
            width: max(1, min(max(1, fallbackWidth), max(1, availableStaticWidth))),
            height: dimensionWithin(entry.height, available: availableHeight, fallback: fallbackHeight)
        )
    }
    return Size(
        width: dimensionWithin(entry.width, available: availableWidth, fallback: fallbackWidth),
        height: dimensionWithin(entry.height, available: availableHeight, fallback: fallbackHeight)
    )
}

private func entrySourceColSpan(_ entry: GeneratedLayoutEntry) -> (Int, Int)
{
    let width = max(1, entry.charsSize.width > 0 ? entry.charsSize.width : entry.sourceCell.width)
    return (entry.relative.col, entry.relative.col + width)
}

private func sourceSpansOverlap(_ first: (Int, Int), _ second: (Int, Int)) -> Bool
{
    first.0 < second.1 && second.0 < first.1
}

private func hasExpandedEntryBeforeInSourceRow(_ layout: [GeneratedLayoutEntry], entry: GeneratedLayoutEntry) -> Bool
{
    for other in layout
    {
        if !sameSourceCell(other, entry) || other.relative.row != entry.relative.row || other.relative.col >= entry.relative.col
        {
            continue
        }
        if axisIsExpanded(other.width)
        {
            return true
        }
    }
    return false
}

private func hasExpandedEntryBeforeInSourceColumn(_ layout: [GeneratedLayoutEntry], entry: GeneratedLayoutEntry) -> Bool
{
    let span = entrySourceColSpan(entry)
    for other in layout
    {
        if !sameSourceCell(other, entry) || other.relative.row >= entry.relative.row || !sourceSpansOverlap(entrySourceColSpan(other), span)
        {
            continue
        }
        if axisIsExpanded(other.height)
        {
            return true
        }
    }
    return false
}

private func renderColFor(_ layout: [GeneratedLayoutEntry], entry: GeneratedLayoutEntry, cellRect: Rect, size: Size) -> Int
{
    let baseCol = paddingLeft(entry.cellStyle) + entry.relative.col
    let cellWidthDelta = max(0, cellRect.width - entry.cellCharsSize.width)
    let followsExpandedEntry = hasExpandedEntryBeforeInSourceRow(layout, entry: entry)
    if entry.marginRight == 0,
       axisIsExpanded(entry.cellWidth),
       entry.cellCharsSize.width > 0,
       cellRect.width > entry.cellCharsSize.width,
       !axisIsExpanded(entry.width)
    {
        return cellRect.col + max(baseCol, cellRect.width - size.width - entry.marginRight - paddingRight(entry.cellStyle))
    }
    if followsExpandedEntry,
       axisIsExpanded(entry.cellWidth),
       entry.cellCharsSize.width > 0,
       cellWidthDelta > 0,
       !axisIsExpanded(entry.width)
    {
        return cellRect.col + max(baseCol, baseCol + cellWidthDelta)
    }
    return cellRect.col + baseCol
}

private func renderRowFor(_ layout: [GeneratedLayoutEntry], entry: GeneratedLayoutEntry, cellRect: Rect, size: Size) -> Int
{
    _ = size
    let baseRow = paddingTop(entry.cellStyle) + entry.relative.row
    let cellHeightDelta = max(0, cellRect.height - entry.cellCharsSize.height)
    let followsExpandedEntry = hasExpandedEntryBeforeInSourceColumn(layout, entry: entry)
    if followsExpandedEntry,
       axisIsExpanded(entry.cellHeight),
       entry.cellCharsSize.height > 0,
       cellHeightDelta > 0,
       !axisIsExpanded(entry.height)
    {
        return cellRect.row + max(baseRow, baseRow + cellHeightDelta)
    }
    return cellRect.row + baseRow
}

private func generatedRuntimeMode(_ window: GeneratedWindowBase) -> String
{
    let explicit = window.mode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    if ["fullscreen", "expand_width", "expand_height", "dialog"].contains(explicit)
    {
        return explicit
    }
    let expandsWidth = window.layout.contains { axisIsExpanded($0.cellWidth) }
    let expandsHeight = window.layout.contains { axisIsExpanded($0.cellHeight) }
    if expandsWidth && expandsHeight
    {
        return "fullscreen"
    }
    if expandsWidth
    {
        return "expand_width"
    }
    if expandsHeight
    {
        return "expand_height"
    }
    return "normal"
}

private func clamped(_ value: Int, lower: Int, upper: Int) -> Int
{
    min(max(value, lower), upper)
}

private func rectContains(_ rect: Rect, row: Int, col: Int) -> Bool
{
    row >= rect.row && row < rect.row + rect.height && col >= rect.col && col < rect.col + rect.width
}

private func comboBoxDropdownFrame(_ comboBox: ComboBox) -> Rect
{
    let optionRows = min(kComboBoxDropdownRows, kComboBoxClosedRows + comboBox.options.count)
    return Rect(
        row: comboBox.frame.row,
        col: comboBox.frame.col,
        width: comboBox.frame.width,
        height: max(comboBox.frame.height, optionRows)
    )
}

private func comboBoxOptionIndex(forLocalRow localRow: Int, optionCount: Int) -> Int
{
    clamped(localRow - kComboBoxClosedRows, lower: 0, upper: max(0, optionCount - 1))
}

private func parseViewport(_ args: [String]) -> Rect?
{
    guard let index = args.firstIndex(of: "--viewport"), index + 1 < args.count else
    {
        return nil
    }
    let parts = args[index + 1].split(separator: ",").compactMap { Int($0) }
    guard parts.count == 4 else
    {
        return nil
    }
    return Rect(row: parts[0], col: parts[1], width: parts[2], height: parts[3])
}

private func readLine(from fd: Int32) -> String?
{
    var bytes: [UInt8] = []
    var byte: UInt8 = 0
    while true
    {
        let count = Darwin.recv(fd, &byte, 1, 0)
        if count <= 0
        {
            break
        }
        if byte == 10
        {
            break
        }
        bytes.append(byte)
    }
    return String(bytes: bytes, encoding: .utf8)
}

private func safeWidth(_ width: Int, _ text: String) -> Int
{
    max(1, width > 0 ? width : max(1, text.count))
}

private func safeHeight(_ height: Int) -> Int
{
    max(1, height > 0 ? height : 1)
}

private func fitText(_ text: String, width: Int) -> String
{
    let glyphs = visualGlyphs(text)
    let clipped = glyphs.prefix(width).map(\.text).joined()
    if glyphs.count >= width
    {
        return clipped
    }
    return clipped + String(repeating: " ", count: width - glyphs.count)
}

private func centerText(_ text: String, width: Int) -> String
{
    let textWidth = visibleWidth(text)
    if textWidth >= width
    {
        return fitText(text, width: width)
    }
    let padding = width - textWidth
    let left = padding / 2
    return String(repeating: " ", count: left) + visibleText(text) + String(repeating: " ", count: padding - left)
}

private struct VisualGlyph
{
    var text: String
    var sourceStart: Int
    var sourceEnd: Int
}

private struct LabelVisualRow
{
    var start: Int
    var end: Int
    var cells: [VisualGlyph]
}

private func inRange(_ value: UInt32, _ first: UInt32, _ last: UInt32) -> Bool
{
    value >= first && value <= last
}

private func isUnsafeTerminalScalar(_ scalar: UnicodeScalar) -> Bool
{
    let value = scalar.value
    if value < 0x20 || value == 0x7f
    {
        return true
    }
    if inRange(value, 0x0300, 0x036f) ||
        inRange(value, 0x1ab0, 0x1aff) ||
        inRange(value, 0x1dc0, 0x1dff) ||
        inRange(value, 0x20d0, 0x20ff) ||
        inRange(value, 0xfe20, 0xfe2f)
    {
        return true
    }
    if inRange(value, 0x200b, 0x200f) ||
        inRange(value, 0x202a, 0x202e) ||
        inRange(value, 0x2060, 0x206f) ||
        inRange(value, 0xfe00, 0xfe0f) ||
        inRange(value, 0xe0100, 0xe01ef)
    {
        return true
    }
    if inRange(value, 0x1100, 0x115f) ||
        inRange(value, 0x2329, 0x232a) ||
        inRange(value, 0x2e80, 0xa4cf) ||
        inRange(value, 0xac00, 0xd7a3) ||
        inRange(value, 0xf900, 0xfaff) ||
        inRange(value, 0xfe10, 0xfe19) ||
        inRange(value, 0xfe30, 0xfe6f) ||
        inRange(value, 0xff00, 0xff60) ||
        inRange(value, 0xffe0, 0xffe6) ||
        inRange(value, 0x1f000, 0x1faff) ||
        inRange(value, 0x20000, 0x3fffd)
    {
        return true
    }
    return false
}

private func tabSpacesForColumn(_ column: Int) -> Int
{
    kTextTabWidth - (max(0, column) % kTextTabWidth)
}

private func visualGlyphs(_ text: String, startColumn: Int = 0, sourceStart: Int = 0) -> [VisualGlyph]
{
    var glyphs: [VisualGlyph] = []
    var column = max(0, startColumn)
    var byteOffset = 0
    for scalar in text.unicodeScalars
    {
        let scalarText = String(scalar)
        let rawStart = sourceStart + byteOffset
        let rawEnd = rawStart + max(1, scalarText.utf8.count)
        byteOffset += max(1, scalarText.utf8.count)

        if scalar.value == 0x0d
        {
            continue
        }
        if scalar.value == 0x09
        {
            let spaces = tabSpacesForColumn(column)
            for _ in 0..<spaces
            {
                glyphs.append(VisualGlyph(text: " ", sourceStart: rawStart, sourceEnd: rawEnd))
                column += 1
            }
            continue
        }

        let visible = isUnsafeTerminalScalar(scalar) ? "?" : scalarText
        glyphs.append(VisualGlyph(text: visible.isEmpty ? " " : visible, sourceStart: rawStart, sourceEnd: rawEnd))
        column += 1
    }
    return glyphs
}

private func visibleText(_ text: String, startColumn: Int = 0) -> String
{
    visualGlyphs(text, startColumn: startColumn).map(\.text).joined()
}

private func visibleWidth(_ text: String, startColumn: Int = 0) -> Int
{
    visualGlyphs(text, startColumn: startColumn).count
}

private func makeLabelVisualRow(fallbackStart: Int, cells: [VisualGlyph]) -> LabelVisualRow
{
    guard let first = cells.first, let last = cells.last else
    {
        return LabelVisualRow(start: fallbackStart, end: fallbackStart, cells: [])
    }
    return LabelVisualRow(start: first.sourceStart, end: last.sourceEnd, cells: cells)
}

private func buildLabelVisualRows(_ text: String, width: Int) -> [LabelVisualRow]
{
    let safeWidth = max(1, width)
    let allBytes = Array(text.utf8)
    var rows: [LabelVisualRow] = []
    var segmentStart = 0
    let segments = text.components(separatedBy: "\n")

    for segmentIndex in 0..<segments.count
    {
        let segment = segments[segmentIndex]
        if segment.isEmpty
        {
            rows.append(LabelVisualRow(start: segmentStart, end: segmentStart, cells: []))
        }
        else
        {
            let glyphs = visualGlyphs(segment, sourceStart: segmentStart)
            var offset = 0
            while offset < glyphs.count
            {
                let remaining = glyphs.count - offset
                let chunkSize = min(remaining, safeWidth)
                var chunk = Array(glyphs[offset..<(offset + chunkSize)])
                if chunkSize < safeWidth
                {
                    rows.append(makeLabelVisualRow(fallbackStart: segmentStart, cells: chunk))
                    offset += chunkSize
                    continue
                }

                var lastSpace: Int?
                if chunk.count > 1
                {
                    for index in stride(from: chunk.count, through: 2, by: -1)
                    {
                        let glyph = chunk[index - 1]
                        if glyph.text == " " &&
                            glyph.sourceStart >= 0 &&
                            glyph.sourceStart < allBytes.count &&
                            allBytes[glyph.sourceStart] == Character(" ").asciiValue
                        {
                            lastSpace = index - 1
                            break
                        }
                    }
                }
                if let lastSpace, lastSpace > 0
                {
                    chunk = Array(chunk.prefix(lastSpace))
                    rows.append(makeLabelVisualRow(fallbackStart: segmentStart, cells: chunk))
                    offset += lastSpace + 1
                }
                else
                {
                    rows.append(makeLabelVisualRow(fallbackStart: segmentStart, cells: chunk))
                    offset += chunkSize
                }
            }
        }

        if segmentIndex + 1 < segments.count
        {
            segmentStart += segment.utf8.count + 1
        }
    }
    if rows.isEmpty
    {
        rows.append(LabelVisualRow(start: 0, end: 0, cells: []))
    }
    return rows
}

private func gradientFrame(_ gradient: TextGradient) -> Int
{
    let interval = max(1, gradient.intervalMs)
    let timeMs = currentRenderTimeOverrideMs() ?? Int(Date().timeIntervalSince1970 * 1000.0)
    return (timeMs / interval) * gradient.step
}

private func gradientColorAt(_ gradient: TextGradient?, sourceIndex: Int) -> Color?
{
    guard let gradient, !gradient.colors.isEmpty else
    {
        return nil
    }
    let segmentSize = max(1, gradient.segmentSize)
    let frame = gradientFrame(gradient)
    let size = gradient.colors.count
    var index = ((sourceIndex / segmentSize) - frame) % size
    if index < 0
    {
        index += size
    }
    return gradient.colors[index]
}

private func blendGradientColor(_ overlay: Color?, base: Color?) -> Color?
{
    guard let overlay else
    {
        return base
    }
    guard let rgba = overlay.rgba else
    {
        return overlay
    }
    if rgba.alpha == 0
    {
        return base
    }
    if rgba.alpha < 255, base != nil
    {
        return overlay.blended(over: base)
    }
    return overlay
}

private func renderGlyphRow(_ glyphs: [VisualGlyph], width: Int, style: Style) -> [TerminalCell]
{
    let clipped = Array(glyphs.prefix(width))
    let contentWidth = clipped.count
    let offset: Int
    if style.textAlign == "center"
    {
        offset = max(0, (width - contentWidth) / 2)
    }
    else if style.textAlign == "right"
    {
        offset = max(0, width - contentWidth)
    }
    else
    {
        offset = 0
    }

    var row: [TerminalCell] = []
    row.reserveCapacity(width)
    for _ in 0..<min(offset, width)
    {
        row.append(TerminalCell(" ", foreground: style.color, background: style.background))
    }
    for glyph in clipped
    {
        if row.count >= width
        {
            break
        }
        let foreground = blendGradientColor(
            gradientColorAt(style.textColorGradient, sourceIndex: glyph.sourceStart),
            base: style.color
        )
        let background = blendGradientColor(
            gradientColorAt(style.textBackgroundGradient, sourceIndex: glyph.sourceStart),
            base: style.background
        )
        row.append(TerminalCell(glyph.text, foreground: foreground, background: background))
    }
    while row.count < width
    {
        row.append(TerminalCell(" ", foreground: style.color, background: style.background))
    }
    return row
}

private func renderPlainText(_ text: String, width: Int, height: Int, style: Style) -> [[TerminalCell]]
{
    let safeRenderWidth = safeWidth(width, text)
    let safeRenderHeight = safeHeight(height)
    var rows: [[TerminalCell]] = []
    if safeRenderHeight > 1
    {
        for row in buildLabelVisualRows(text, width: safeRenderWidth)
        {
            if rows.count >= safeRenderHeight
            {
                break
            }
            rows.append(renderGlyphRow(row.cells, width: safeRenderWidth, style: style))
        }
    }
    else
    {
        let firstLine = text.components(separatedBy: "\n").first ?? ""
        rows.append(renderGlyphRow(visualGlyphs(firstLine), width: safeRenderWidth, style: style))
    }
    while rows.count < safeRenderHeight
    {
        rows.append(renderGlyphRow([], width: safeRenderWidth, style: style))
    }
    return rows
}

private func formatNumber(_ value: Double) -> String
{
    if value.rounded() == value
    {
        return String(Int(value))
    }
    return String(value)
}

private func resolvedRect(_ entry: GeneratedLayoutEntry, viewport: Size, sourceColumnShift: Int, sourceCellWidth: Int) -> Rect
{
    let width: Int
    switch entry.width
    {
    case .expanded:
        width = max(1, sourceCellWidth - entry.relative.col - entry.marginRight)
    case .fixed(let value), .auto(let value):
        width = max(1, value)
    case .fitContent:
        width = max(1, entry.charsSize.width)
    }
    let height: Int
    switch entry.height
    {
    case .expanded:
        height = max(1, entry.sourceCell.height - entry.relative.row - entry.marginBottom)
    case .fixed(let value), .auto(let value):
        height = max(1, value)
    case .fitContent:
        height = max(1, entry.charsSize.height)
    }
    return Rect(
        row: entry.sourceCell.row + entry.relative.row,
        col: entry.sourceCell.col + sourceColumnShift + entry.relative.col,
        width: width,
        height: height
    )
}

private func clipped(_ rect: Rect, width: Int, height: Int) -> Rect
{
    let row = max(0, rect.row)
    let col = max(0, rect.col)
    return Rect(row: row, col: col, width: max(0, min(rect.width, width - col)), height: max(0, min(rect.height, height - row)))
}

private func cellContentClip(_ entry: GeneratedLayoutEntry, cellRect: Rect) -> Rect
{
    let top = paddingTop(entry.cellStyle)
    let right = paddingRight(entry.cellStyle)
    let bottom = paddingBottom(entry.cellStyle)
    let left = paddingLeft(entry.cellStyle)
    return Rect(
        row: cellRect.row + top,
        col: cellRect.col + left,
        width: max(0, cellRect.width - left - right),
        height: max(0, cellRect.height - top - bottom)
    )
}

private func fill(buffer: inout [[TerminalCell]], rect: Rect, style: Style)
{
    guard rect.width > 0 && rect.height > 0 else
    {
        return
    }
    let textureGlyphs = style.backgroundTexture.map { Array($0).map(String.init) } ?? []
    for row in rect.row..<min(buffer.count, rect.row + rect.height)
    {
        for col in rect.col..<min(buffer[row].count, rect.col + rect.width)
        {
            if !textureGlyphs.isEmpty
            {
                let textureWidth = max(1, rect.width)
                let index = ((row - rect.row) * textureWidth + (col - rect.col)) % textureGlyphs.count
                buffer[row][col].text = textureGlyphs[index]
                if let foreground = style.backgroundTextureColor ?? style.color, !foreground.isTransparent
                {
                    buffer[row][col].foreground = foreground.blended(over: buffer[row][col].background)
                }
            }
            else if let color = style.color, !color.isTransparent
            {
                buffer[row][col].foreground = color.blended(over: buffer[row][col].background)
            }
            if let background = style.background, !background.isTransparent
            {
                buffer[row][col].background = background.blended(over: buffer[row][col].background)
            }
        }
    }
}

private func dimOutsideActiveElement(buffer: inout [[TerminalCell]], active: Rect, dim: Color?)
{
    guard let dim, !dim.isTransparent else
    {
        return
    }
    for row in 0..<buffer.count
    {
        for col in 0..<buffer[row].count
        {
            if rectContains(active, row: row, col: col)
            {
                continue
            }
            if buffer[row][col].text == kFallbackUpperHalfBlock
            {
                if buffer[row][col].foreground != nil
                {
                    buffer[row][col].foreground = dim.blended(over: buffer[row][col].foreground)
                }
                buffer[row][col].background = dim.blended(over: buffer[row][col].background) ?? dim
                continue
            }
            buffer[row][col].background = dim.blended(over: buffer[row][col].background) ?? dim
        }
    }
}

private func cellHasRenderedContent(_ cell: TerminalCell) -> Bool
{
    cell.text != " " && cell.text != "^" && cell.text != "v"
}

private func rowHasRenderedContent(buffer: [[TerminalCell]], row: Int, startCol: Int, endCol: Int) -> Bool
{
    guard row >= 0 && row < buffer.count else
    {
        return false
    }
    let start = max(0, startCol)
    let end = min(buffer[row].count, endCol)
    guard start < end else
    {
        return false
    }
    for col in start..<end
    {
        if cellHasRenderedContent(buffer[row][col])
        {
            return true
        }
    }
    return false
}

private func applyActiveElementFocusBackground(buffer: inout [[TerminalCell]], active: Rect, style: Style, focusBackground: Color?)
{
    guard let focusBackground, !focusBackground.isTransparent else
    {
        return
    }
    let baseBackground = style.background
    let focusedBaseBackground = focusBackground.blended(over: baseBackground)
    let protectedStartCol = active.col + paddingLeft(style)
    let protectedEndCol = active.col + active.width - paddingRight(style)
    let transparentViewportBackground = (style.background?.rgba?.alpha ?? 255) < 255
    for row in max(0, active.row)..<min(buffer.count, active.row + active.height)
    {
        let protectContentColumns = transparentViewportBackground && rowHasRenderedContent(
            buffer: buffer,
            row: row,
            startCol: active.col,
            endCol: active.col + active.width
        )
        for col in max(0, active.col)..<min(buffer[row].count, active.col + active.width)
        {
            if protectContentColumns && col >= protectedStartCol && col < protectedEndCol
            {
                continue
            }
            let background = buffer[row][col].background
            if background == focusedBaseBackground
            {
                continue
            }
            if background == nil || background == baseBackground
            {
                buffer[row][col].background = focusBackground.blended(over: background)
            }
        }
    }
}

private func applyScrollableHostFocusBackground(
    buffer: inout [[TerminalCell]],
    active: Rect,
    style: Style,
    focusBackground: Color?,
    baseBackground: Color?
)
{
    guard let focusBackground, !focusBackground.isTransparent else
    {
        return
    }
    let styleBackground = style.background
    let protectedStartCol = active.col + paddingLeft(style)
    let protectedEndCol = active.col + active.width - paddingRight(style)
    let transparentViewportBackground = (style.background?.rgba?.alpha ?? 255) < 255
    for row in max(0, active.row)..<min(buffer.count, active.row + active.height)
    {
        let protectContentColumns = transparentViewportBackground && rowHasRenderedContent(
            buffer: buffer,
            row: row,
            startCol: active.col,
            endCol: active.col + active.width
        )
        for col in max(0, active.col)..<min(buffer[row].count, active.col + active.width)
        {
            if protectContentColumns && col >= protectedStartCol && col < protectedEndCol
            {
                continue
            }
            let background = buffer[row][col].background
            if background == nil || background == styleBackground || background == baseBackground
            {
                buffer[row][col].background = focusBackground.blended(over: background ?? baseBackground)
                    ?? focusBackground
            }
        }
    }
}

private func appendUniqueOpaqueBackground(_ backgrounds: inout [Color], _ color: Color?)
{
    guard let color, !color.isTransparent else
    {
        return
    }
    if backgrounds.contains(where: { $0.snapshotValue == color.snapshotValue })
    {
        return
    }
    backgrounds.append(color)
}

private func collectDescendantBaseStyleBackgrounds(_ window: GeneratedWindowBase, into backgrounds: inout [Color])
{
    for element in window.elements
    {
        collectDescendantBaseStyleBackgrounds(element, into: &backgrounds)
    }
}

private func collectDescendantBaseStyleBackgrounds(_ element: UIElement, into backgrounds: inout [Color])
{
    appendUniqueOpaqueBackground(&backgrounds, element.style.background)
    appendUniqueOpaqueBackground(&backgrounds, element.focusStyle?.background)
    appendUniqueOpaqueBackground(&backgrounds, element.editStyle?.background)
    if let reusable = element as? ReusableElement,
       let child = reusable.child
    {
        collectDescendantBaseStyleBackgrounds(child, into: &backgrounds)
    }
    if let viewHost = element as? ViewHost,
       let child = viewHost.child
    {
        collectDescendantBaseStyleBackgrounds(child, into: &backgrounds)
    }
    if let scrollView = element as? ScrollView
    {
        for child in scrollView.children
        {
            collectDescendantBaseStyleBackgrounds(child, into: &backgrounds)
        }
    }
}

private func applyReusableFocusBackgroundToDescendantBackgrounds(
    content: inout [[TerminalCell]],
    focusBackground: Color,
    descendantBackgrounds: [Color]
)
{
    guard !descendantBackgrounds.isEmpty else
    {
        return
    }
    for row in content.indices
    {
        for col in content[row].indices
        {
            guard let background = content[row][col].background else
            {
                continue
            }
            for descendantBackground in descendantBackgrounds
            {
                let focusedBackground = focusBackground.blended(over: descendantBackground) ?? focusBackground
                if background.snapshotValue == descendantBackground.snapshotValue ||
                    background.snapshotValue == focusedBackground.snapshotValue
                {
                    content[row][col].background = focusedBackground
                    break
                }
            }
        }
    }
}

private func applySelectedBackgroundToFocusedReusableChild(
    content: inout [[TerminalCell]],
    selectedBackground: Color,
    focusBackground: Color?,
    descendantBackgrounds: [Color]
)
{
    for row in content.indices
    {
        for col in content[row].indices
        {
            guard let background = content[row][col].background else
            {
                continue
            }
            var descendantBackgroundMatched = false
            for descendantBackground in descendantBackgrounds
            {
                if background.snapshotValue == descendantBackground.snapshotValue
                {
                    descendantBackgroundMatched = true
                    break
                }
                if let focusBackground
                {
                    let focusedBackground = focusBackground.blended(over: descendantBackground) ?? focusBackground
                    if background.snapshotValue == focusedBackground.snapshotValue
                    {
                        descendantBackgroundMatched = true
                        break
                    }
                }
            }
            if descendantBackgroundMatched
            {
                continue
            }
            content[row][col].background = selectedBackground.blended(over: background) ?? selectedBackground
        }
    }
}

private func generatedScrollViewChild(of element: UIElement) -> ScrollView?
{
    if let reusable = element as? ReusableElement
    {
        return reusable.child?.generatedScrollView()
    }
    if let viewHost = element as? ViewHost
    {
        return viewHost.child?.generatedScrollView()
    }
    return nil
}

private func generatedWindowChild(of element: UIElement) -> GeneratedWindowBase?
{
    if let reusable = element as? ReusableElement
    {
        return reusable.child
    }
    if let viewHost = element as? ViewHost
    {
        return viewHost.child
    }
    return nil
}

private func windowOwnsElement(_ window: GeneratedWindowBase, _ target: UIElement?) -> Bool
{
    guard let target else
    {
        return false
    }
    return window.elements.contains { $0 === target }
}

private func ownerWindowForElement(_ window: GeneratedWindowBase, _ target: UIElement?) -> GeneratedWindowBase?
{
    guard let target else
    {
        return nil
    }
    if windowOwnsElement(window, target)
    {
        return window
    }
    for element in window.elements
    {
        if let child = generatedWindowChild(of: element),
           let owner = ownerWindowForElement(child, target)
        {
            return owner
        }
        guard let scrollView = element as? ScrollView else
        {
            continue
        }
        for child in scrollView.children
        {
            guard let generated = generatedWindowChild(of: child),
                  let owner = ownerWindowForElement(generated, target)
            else
            {
                continue
            }
            return owner
        }
    }
    return nil
}

private func childElementRepresentedInCurrentLayout(_ element: UIElement, _ target: UIElement?) -> Bool
{
    guard let target else
    {
        return false
    }
    if element === target
    {
        return true
    }
    if let child = generatedWindowChild(of: element)
    {
        let frame = element.frame
        if frame.width > 0 && frame.height > 0
        {
            if let reusable = element as? ReusableElement
            {
                syncGeneratedReusableChildFrames(reusable, to: frame)
            }
            else
            {
                syncGeneratedWindowElementFrames(child, to: frame, forceFullscreenLayout: true)
            }
        }
        return elementRepresentedInCurrentLayout(child, target)
    }
    guard let scrollView = element as? ScrollView else
    {
        return false
    }
    let frame = scrollView.frame
    let viewport = Size(width: max(1, frame.width), height: max(1, frame.height))
    for childView in scrollView.childViews(size: viewport)
    {
        guard let child = childView.element else
        {
            continue
        }
        if childElementRepresentedInCurrentLayout(child, target)
        {
            return true
        }
    }
    return false
}

private func elementRepresentedInCurrentLayout(_ window: GeneratedWindowBase, _ target: UIElement?) -> Bool
{
    guard let target else
    {
        return false
    }
    for entry in window.layout
    {
        guard !entry.name.isEmpty,
              let element = window.elements.first(where: { $0.name == entry.name })
        else
        {
            continue
        }
        if childElementRepresentedInCurrentLayout(element, target)
        {
            return true
        }
    }
    return false
}

private func generatedScrollViewProxyFor(_ window: GeneratedWindowBase, _ target: ScrollView?) -> UIElement?
{
    guard let target else
    {
        return nil
    }
    for element in window.elements
    {
        if let child = generatedWindowChild(of: element)
        {
            if child.generatedScrollView() === target
            {
                return element
            }
            if let proxy = generatedScrollViewProxyFor(child, target)
            {
                return proxy
            }
        }
        guard let scrollView = element as? ScrollView else
        {
            continue
        }
        for child in scrollView.children
        {
            guard let generated = generatedWindowChild(of: child) else
            {
                continue
            }
            if generated.generatedScrollView() === target
            {
                return child
            }
            if let proxy = generatedScrollViewProxyFor(generated, target)
            {
                return proxy
            }
        }
    }
    return nil
}

private func generatedScrollViewScopeDimBackground(_ window: GeneratedWindowBase, _ target: ScrollView?) -> Color?
{
    guard let proxy = generatedScrollViewProxyFor(window, target),
          let background = proxy.style.scopeDimBackground
    else
    {
        return nil
    }
    return background
}

private func activeScrollViewRepresentedInCurrentLayout(_ window: GeneratedWindowBase, _ target: ScrollView?) -> Bool
{
    guard let target else
    {
        return false
    }
    if let proxy = generatedScrollViewProxyFor(window, target),
       elementRepresentedInCurrentLayout(window, proxy)
    {
        return true
    }
    return elementRepresentedInCurrentLayout(window, target)
}

private func elementTreeContains(_ element: UIElement, _ target: UIElement?) -> Bool
{
    guard let target else
    {
        return false
    }
    if element === target
    {
        return true
    }
    if let reusable = element as? ReusableElement,
       let child = reusable.child,
       ownerWindowForElement(child, target) != nil
    {
        return true
    }
    if let scrollView = element as? ScrollView
    {
        for child in scrollView.children
        {
            if elementTreeContains(child, target)
            {
                return true
            }
        }
    }
    for child in element.children
    {
        if elementTreeContains(child, target)
        {
            return true
        }
    }
    return false
}

private func focusRectWithinScrollChild(_ child: UIElement, target: UIElement, width: Int, height: Int) -> Rect?
{
    if child === target
    {
        return Rect(
            row: 0,
            col: 0,
            width: max(1, child.frame.width),
            height: max(1, height)
        )
    }
    if let reusable = child as? ReusableElement,
       let generated = reusable.child,
       ownerWindowForElement(generated, target) != nil
    {
        syncGeneratedWindowElementFrames(
            generated,
            to: Rect(row: 0, col: 0, width: max(1, width), height: max(1, height)),
            forceFullscreenLayout: true
        )
        return Rect(
            row: target.frame.row,
            col: target.frame.col,
            width: max(1, target.frame.width),
            height: max(1, target.frame.height)
        )
    }
    if elementTreeContains(child, target)
    {
        return Rect(
            row: target.frame.row,
            col: target.frame.col,
            width: max(1, target.frame.width),
            height: max(1, target.frame.height)
        )
    }
    return nil
}

private func scrollViewViewportClip(frame: Rect, style: Style) -> Rect
{
    let left = min(paddingLeft(style), max(0, frame.width - 1))
    let right = min(paddingRight(style), max(0, frame.width - left - 1))
    let top = min(paddingTop(style), max(0, frame.height - 1))
    let bottom = min(paddingBottom(style), max(0, frame.height - top - 1))
    return Rect(
        row: frame.row + top,
        col: frame.col + left,
        width: max(0, frame.width - left - right),
        height: max(0, frame.height - top - bottom)
    )
}

private func scrollViewViewportClipClampedToContent(frame: Rect, style: Style, contentSize: Size) -> Rect
{
    let left = paddingLeft(style)
    let right = paddingRight(style)
    let top = paddingTop(style)
    let bottom = paddingBottom(style)
    var viewport = Rect(
        row: frame.row + top,
        col: frame.col + left,
        width: max(0, frame.width - left - right),
        height: max(0, frame.height - top - bottom)
    )
    viewport.width = min(viewport.width, max(0, contentSize.width - viewport.col))
    viewport.height = min(viewport.height, max(0, contentSize.height - viewport.row))
    return viewport
}

private func generatedScrollViewSourceFrame(_ window: GeneratedWindowBase, scrollView: ScrollView, fallback: Size) -> Rect
{
    for entry in window.layout where entry.name == scrollView.name
    {
        return entry.sourceCell
    }
    return Rect(row: 0, col: 0, width: fallback.width, height: fallback.height)
}

@discardableResult
private func scrollViewByLines(_ scrollView: ScrollView, delta: Int) -> Bool
{
    guard delta != 0 else
    {
        return false
    }
    let viewport = Size(width: scrollView.frame.width, height: scrollView.frame.height)
    let key = delta < 0 ? "Up" : "Down"
    var changed = false
    for _ in 0..<abs(delta)
    {
        changed = scrollView.handleKey(key, viewport: viewport) || changed
    }
    return changed
}

@discardableResult
private func ensureElementFrameVisibleInScrollView(_ scrollView: ScrollView, element: UIElement?) -> Bool
{
    guard let element else
    {
        return false
    }
    let viewport = scrollViewViewportClip(frame: scrollView.frame, style: scrollView.style)
    guard viewport.width > 0, viewport.height > 0 else
    {
        return false
    }
    let scrollFrame = scrollView.frame
    for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
    {
        guard let child = childView.element,
              let targetRect = focusRectWithinScrollChild(
                child,
                target: element,
                width: childView.frame.width,
                height: childView.frame.height
              )
        else
        {
            continue
        }
        let targetTop = scrollFrame.row + childView.frame.row + targetRect.row
        let targetBottom = scrollFrame.row + childView.frame.row + targetRect.row + max(1, targetRect.height)
        var delta = 0
        if targetTop < viewport.row
        {
            delta = targetTop - viewport.row
        }
        else if targetBottom > viewport.row + viewport.height
        {
            delta = targetBottom - viewport.row - viewport.height
        }
        return scrollViewByLines(scrollView, delta: delta)
    }
    let target = element.frame
    var delta = 0
    if target.row < viewport.row
    {
        delta = target.row - viewport.row
    }
    else if target.row + target.height > viewport.row + viewport.height
    {
        delta = target.row + target.height - viewport.row - viewport.height
    }
    return scrollViewByLines(scrollView, delta: delta)
}

private func elementVisibleInScrollView(_ scrollView: ScrollView, element: UIElement?) -> Bool
{
    guard let element else
    {
        return false
    }
    let viewport = scrollViewViewportClip(frame: scrollView.frame, style: scrollView.style)
    guard viewport.width > 0, viewport.height > 0 else
    {
        return false
    }
    let scrollFrame = scrollView.frame
    for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
    {
        guard childView.visible,
              let child = childView.element,
              let targetRect = focusRectWithinScrollChild(
                child,
                target: element,
                width: childView.frame.width,
                height: childView.frame.height
              )
        else
        {
            continue
        }
        let targetTop = scrollFrame.row + childView.frame.row + targetRect.row
        let targetBottom = targetTop + max(1, targetRect.height)
        return targetBottom > viewport.row && targetTop < viewport.row + viewport.height
    }
    return false
}

private func reusableFocusStyleAppliesToChild(
    _ element: UIElement,
    child: GeneratedWindowBase,
    directFocus: Bool,
    descendantOnlyFocus: Bool
) -> Bool
{
    guard let background = element.focusStyle?.background
    else
    {
        return false
    }
    if !hasPartialAlpha(background)
    {
        return true
    }
    if child.generatedScrollView() != nil
    {
        return false
    }
    return directFocus || !descendantOnlyFocus
}

private func generatedChildFocusBackground(
    for element: UIElement,
    child: GeneratedWindowBase,
    directFocus: Bool,
    descendantOnlyFocus: Bool
) -> Color?
{
    if reusableFocusStyleAppliesToChild(
        element,
        child: child,
        directFocus: directFocus,
        descendantOnlyFocus: descendantOnlyFocus
    )
    {
        return element.focusStyle?.background
    }
    if let scrollView = child.generatedScrollView()
    {
        return scrollView.focusStyle?.background ?? child.generatedWindowFocusStyle?.background
    }
    return child.generatedWindowFocusStyle?.background
}

private func scrollViewHostRect(scrollView: ScrollView, fallback: Rect) -> Rect
{
    let fallbackViewport = generatedScrollViewport(
        size: Size(width: fallback.width, height: fallback.height),
        style: scrollView.style
    )
    if scrollView.frame.width > 0 && scrollView.frame.height > 0
    {
        let viewport = generatedScrollViewport(
            size: Size(width: scrollView.frame.width, height: scrollView.frame.height),
            style: scrollView.style
        )
        return Rect(
            row: fallback.row + scrollView.frame.row,
            col: fallback.col + scrollView.frame.col,
            width: min(viewport.width, fallbackViewport.width),
            height: min(viewport.height, fallbackViewport.height)
        )
    }
    return Rect(
        row: fallback.row,
        col: fallback.col,
        width: fallbackViewport.width,
        height: fallbackViewport.height
    )
}

public func applyGeneratedWindowFocusBackground(
    content: inout [[TerminalCell]],
    child: GeneratedWindowBase,
    focusBackground: Color,
    protectRenderedContentRows: Bool = false
)
{
    guard !focusBackground.isTransparent
    else
    {
        return
    }
    var backgrounds: [Color] = []
    appendUniqueOpaqueBackground(&backgrounds, child.generatedWindowStyle.background)
    for entry in child.layout
    {
        appendUniqueOpaqueBackground(&backgrounds, entry.cellStyle.background)
    }
    guard !backgrounds.isEmpty else
    {
        return
    }
    for row in content.indices
    {
        if protectRenderedContentRows && rowHasRenderedContent(buffer: content, row: row, startCol: 0, endCol: content[row].count)
        {
            continue
        }
        for col in content[row].indices
        {
            guard let background = content[row][col].background else
            {
                continue
            }
            for base in backgrounds
            {
                let focused = focusBackground.blended(over: base) ?? focusBackground
                if background.snapshotValue == base.snapshotValue || background.snapshotValue == focused.snapshotValue
                {
                    content[row][col].background = focused
                    break
                }
            }
        }
    }
}

private func applyReusableHostFocusBackground(
    content: inout [[TerminalCell]],
    child: GeneratedWindowBase,
    focusBackground: Color,
    baseBackground: Color?
)
{
    guard !focusBackground.isTransparent
    else
    {
        return
    }
    if let scrollView = child.generatedScrollView()
    {
        let fallback = Rect(
            row: 0,
            col: 0,
            width: content.first?.count ?? 0,
            height: content.count
        )
        applyScrollableHostFocusBackground(
            buffer: &content,
            active: scrollViewHostRect(scrollView: scrollView, fallback: fallback),
            style: scrollView.style,
            focusBackground: focusBackground,
            baseBackground: baseBackground
        )
        return
    }
    var backgrounds: [Color] = []
    appendUniqueOpaqueBackground(&backgrounds, baseBackground)
    appendUniqueOpaqueBackground(&backgrounds, child.generatedWindowStyle.background)
    for row in content.indices
    {
        for col in content[row].indices
        {
            guard let background = content[row][col].background
            else
            {
                content[row][col].background = focusBackground
                continue
            }
            for base in backgrounds
            {
                let focused = focusBackground.blended(over: base) ?? focusBackground
                if background.snapshotValue == base.snapshotValue || background.snapshotValue == focused.snapshotValue
                {
                    content[row][col].background = focused
                    break
                }
            }
        }
    }
}

private func applyReusableFocusBackground(
    content: inout [[TerminalCell]],
    focusBackground: Color,
    baseBackground: Color?
)
{
    guard !focusBackground.isTransparent else
    {
        return
    }
    let focusedBaseBackground = baseBackground.map { focusBackground.blended(over: $0) ?? focusBackground }
    for row in content.indices
    {
        for col in content[row].indices
        {
            let background = content[row][col].background
            if let focusedBaseBackground,
               background?.snapshotValue == focusBackground.snapshotValue
            {
                content[row][col].background =
                    focusBackground.blended(over: focusedBaseBackground) ?? focusBackground
                continue
            }
            let matchesBase = background == nil ||
                (baseBackground != nil && background?.snapshotValue == baseBackground?.snapshotValue)
            let matchesFocusedBase = focusedBaseBackground != nil &&
                background?.snapshotValue == focusedBaseBackground?.snapshotValue
            if !matchesBase && !matchesFocusedBase
            {
                continue
            }
            if matchesFocusedBase
            {
                continue
            }
            content[row][col].background = focusBackground.blended(over: background) ?? focusBackground
        }
    }
}

public func applyFocusBackgroundToBlankRow(content: inout [[TerminalCell]], row: Int, focusBackground: Color)
{
    guard !focusBackground.isTransparent,
          row >= 0,
          row < content.count,
          !rowHasRenderedContent(buffer: content, row: row, startCol: 0, endCol: content[row].count)
    else
    {
        return
    }
    for col in content[row].indices
    {
        content[row][col].background = focusBackground.blended(over: content[row][col].background) ?? focusBackground
    }
}

public func applyFocusBackgroundToBaseCells(
    content: inout [[TerminalCell]],
    row: Int,
    focusBackground: Color,
    baseBackgrounds: [Color?]
)
{
    guard !focusBackground.isTransparent,
          row >= 0,
          row < content.count
    else
    {
        return
    }
    var backgrounds: [Color] = []
    for background in baseBackgrounds
    {
        appendUniqueOpaqueBackground(&backgrounds, background)
    }
    guard !backgrounds.isEmpty
    else
    {
        return
    }
    for col in content[row].indices
    {
        guard let background = content[row][col].background
        else
        {
            content[row][col].background = focusBackground
            continue
        }
        for base in backgrounds
        {
            let focused = focusBackground.blended(over: base) ?? focusBackground
            if background.snapshotValue == base.snapshotValue || background.snapshotValue == focused.snapshotValue
            {
                content[row][col].background = focused
                break
            }
        }
    }
}

private func applyHostViewportRootScrollIndicators(
    content: inout [[TerminalCell]],
    window: GeneratedWindowBase,
    size: Size
)
{
    guard let scrollView = window.generatedScrollView() else
    {
        return
    }
    let scrollFrame = generatedScrollViewSourceFrame(window, scrollView: scrollView, fallback: size)
    let scrollViewport = scrollViewViewportClipClampedToContent(
        frame: scrollFrame,
        style: scrollView.style,
        contentSize: size
    )
    guard scrollViewport.width > 0, scrollViewport.height > 0 else
    {
        return
    }

    var childContentAboveViewport = false
    var childContentBelowViewport = false
    for childView in scrollView.childViews(size: Size(width: scrollFrame.width, height: scrollFrame.height))
    {
        let childFrame = Rect(
            row: scrollFrame.row + childView.frame.row,
            col: scrollFrame.col + childView.frame.col,
            width: childView.frame.width,
            height: childView.frame.height
        )
        if childFrame.row < scrollViewport.row
        {
            childContentAboveViewport = true
        }
        if childFrame.row + childFrame.height > scrollViewport.row + scrollViewport.height
        {
            childContentBelowViewport = true
        }
    }

    let viewportSize = Size(width: scrollViewport.width, height: scrollViewport.height)
    let indicatorCol = scrollViewport.col + scrollViewport.width - kMinimumRenderableSize
    let maxViewOffset = scrollView.maxViewOffset(viewport: viewportSize)
    let viewOffset = scrollView.viewOffsetValue()
    if viewOffset < maxViewOffset || childContentAboveViewport
    {
        applyScrollIndicator(
            content: &content,
            row: scrollViewport.row,
            col: indicatorCol,
            indicator: kScrollIndicatorAbove,
            foregroundStartCol: scrollViewport.col
        )
    }
    if viewOffset > 0 || childContentBelowViewport
    {
        applyScrollIndicator(
            content: &content,
            row: scrollViewport.row + scrollViewport.height - kMinimumRenderableSize,
            col: indicatorCol,
            indicator: kScrollIndicatorBelow,
            foregroundStartCol: scrollViewport.col
        )
    }
}

public func applyGeneratedScrollIndicators(
    content: inout [[TerminalCell]],
    viewOffset: Int,
    maxViewOffset: Int,
    viewport: Size,
    style: Style,
    childContentAboveViewport: Bool = false,
    childContentBelowViewport: Bool = false
)
{
    guard !content.isEmpty else
    {
        return
    }
    let top = paddingTop(style)
    let left = paddingLeft(style)
    let right = paddingRight(style)
    let bottom = paddingBottom(style)
    let viewportRow = top
    let viewportCol = left
    let viewportWidth = min(max(0, viewport.width - left - right), max(0, content[0].count - viewportCol))
    let viewportHeight = min(max(0, viewport.height - top - bottom), max(0, content.count - viewportRow))
    guard viewportWidth > 0 && viewportHeight > 0 else
    {
        return
    }
    let indicatorCol = viewportCol + viewportWidth - kMinimumRenderableSize
    let hasScrolledFromTop = viewOffset > 0 || childContentAboveViewport
    let indicatorsActive = hasScrolledFromTop || childContentBelowViewport
    if hasScrolledFromTop
    {
        applyScrollIndicator(
            content: &content,
            row: viewportRow,
            col: indicatorCol,
            indicator: kScrollIndicatorAbove,
            foregroundStartCol: viewportCol
        )
    }
    if childContentBelowViewport || (indicatorsActive && viewOffset < maxViewOffset)
    {
        applyScrollIndicator(
            content: &content,
            row: viewportRow + viewportHeight - kMinimumRenderableSize,
            col: indicatorCol,
            indicator: kScrollIndicatorBelow,
            foregroundStartCol: viewportCol
        )
    }
}

public func generatedScrollViewport(size: Size, style: Style) -> Rect
{
    let top = paddingTop(style)
    let left = paddingLeft(style)
    let right = paddingRight(style)
    let bottom = paddingBottom(style)
    return Rect(
        row: top,
        col: left,
        width: max(0, size.width - left - right),
        height: max(0, size.height - top - bottom)
    )
}

public func generatedScrollGap(style: Style) -> Int
{
    max(0, style.gap ?? 0)
}

public func renderGeneratedContentForHost(_ child: GeneratedWindowBase, size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
{
    child.renderContentForHost(size: size, focusedName: focusedName, editMode: editMode)
}

private func applyScrollIndicator(
    content: inout [[TerminalCell]],
    row: Int,
    col: Int,
    indicator: String,
    foregroundStartCol: Int
)
{
    guard row >= 0,
          row < content.count,
          col >= 0,
          col < content[row].count
    else
    {
        return
    }
    content[row][col].text = indicator
    if content[row][col].foreground == nil || content[row][col].foreground?.isTransparent == true
    {
        content[row][col].foreground = inferredScrollIndicatorForeground(
            row: content[row],
            startCol: foregroundStartCol,
            endCol: col
        )
    }
}

private func inferredScrollIndicatorForeground(row: [TerminalCell], startCol: Int, endCol: Int) -> Color?
{
    guard !row.isEmpty else
    {
        return nil
    }
    let start = max(0, startCol)
    let end = min(row.count - kMinimumRenderableSize, endCol)
    guard start <= end else
    {
        return nil
    }
    var foreground: Color?
    for scanCol in start...end
    {
        if let scanned = row[scanCol].foreground, !scanned.isTransparent
        {
            foreground = scanned
        }
    }
    return foreground
}

private func applyGeneratedWindowFocusBackground(content: inout [[TerminalCell]], child: GeneratedWindowBase)
{
    guard let focusBackground = child.generatedWindowFocusStyle?.background
    else
    {
        return
    }
    applyGeneratedWindowFocusBackground(content: &content, child: child, focusBackground: focusBackground)
}

private func blit(buffer: inout [[TerminalCell]], rendered: [[TerminalCell]], row: Int, col: Int)
{
    for sourceRow in 0..<rendered.count
    {
        let targetRow = row + sourceRow
        guard targetRow >= 0 && targetRow < buffer.count else
        {
            continue
        }
        for sourceCol in 0..<rendered[sourceRow].count
        {
            let targetCol = col + sourceCol
            guard targetCol >= 0 && targetCol < buffer[targetRow].count else
            {
                continue
            }
            var cell = rendered[sourceRow][sourceCol]
            let existing = buffer[targetRow][targetCol]
            if let background = cell.background, !background.isTransparent
            {
                cell.background = background.blended(over: existing.background)
            }
            else
            {
                cell.background = existing.background
            }
            if let foreground = cell.foreground, !foreground.isTransparent
            {
                cell.foreground = foreground.blended(over: cell.background)
            }
            else
            {
                cell.foreground = existing.foreground
            }
            buffer[targetRow][targetCol] = cell
        }
    }
}

private func blitClipped(buffer: inout [[TerminalCell]], rendered: [[TerminalCell]], row: Int, col: Int, clip: Rect)
{
    for sourceRow in 0..<rendered.count
    {
        let targetRow = row + sourceRow
        guard targetRow >= clip.row,
              targetRow < clip.row + clip.height,
              targetRow >= 0,
              targetRow < buffer.count
        else
        {
            continue
        }
        for sourceCol in 0..<rendered[sourceRow].count
        {
            let targetCol = col + sourceCol
            guard targetCol >= clip.col,
                  targetCol < clip.col + clip.width,
                  targetCol >= 0,
                  targetCol < buffer[targetRow].count
            else
            {
                continue
            }
            var cell = rendered[sourceRow][sourceCol]
            let existing = buffer[targetRow][targetCol]
            if let background = cell.background, !background.isTransparent
            {
                cell.background = background.blended(over: existing.background)
            }
            else
            {
                cell.background = existing.background
            }
            if let foreground = cell.foreground, !foreground.isTransparent
            {
                cell.foreground = foreground.blended(over: cell.background)
            }
            else
            {
                cell.foreground = existing.foreground
            }
            buffer[targetRow][targetCol] = cell
        }
    }
}

private func renderedText(_ rendered: [[TerminalCell]]) -> [String]
{
    rendered.map { row in row.map(\.text).joined() }
}

private func rectJson(_ rect: Rect) -> [String: Any]
{
    [
        "top": rect.row,
        "left": rect.col,
        "bottom": rect.row + rect.height,
        "right": rect.col + rect.width,
        "width": rect.width,
        "height": rect.height,
    ]
}

private func fullCells(_ content: [[TerminalCell]]) -> [[[String: Any]]]
{
    content.map { row in row.map(cellJson) }
}

private func compactCells(_ content: [[TerminalCell]]) -> [[[Any]]]
{
    content.map
    {
        row in row.map
        {
            cell in [
                cell.text.isEmpty ? " " : cell.text,
                cell.text == " " ? NSNull() : jsonNullableColor(cell.foreground),
                jsonNullableColor(cell.background),
                [],
            ]
        }
    }
}

private func cellJson(_ cell: TerminalCell) -> [String: Any]
{
    let text = cell.text.isEmpty ? " " : cell.text
    return [
        "char": text,
        "foreground": text == " " ? NSNull() : jsonNullableColor(cell.foreground),
        "background": jsonNullableColor(cell.background),
        "attributes": [],
    ]
}

private func jsonNullableColor(_ color: Color?) -> Any
{
    color?.snapshotValue ?? NSNull()
}

private func withRenderTimeOverride<T>(_ timeMs: Int?, _ body: () throws -> T) rethrows -> T
{
    let previous = currentRenderTimeOverrideMs()
    setCurrentRenderTimeOverrideMs(timeMs)
    defer
    {
        setCurrentRenderTimeOverrideMs(previous)
    }
    return try body()
}

private func currentRenderTimeOverrideMs() -> Int?
{
    Thread.current.threadDictionary[kRenderTimeOverrideThreadKey] as? Int
}

private func setCurrentRenderTimeOverrideMs(_ timeMs: Int?)
{
    if let timeMs
    {
        Thread.current.threadDictionary[kRenderTimeOverrideThreadKey] = timeMs
    }
    else
    {
        Thread.current.threadDictionary.removeObject(forKey: kRenderTimeOverrideThreadKey)
    }
}

private func withImageCellBackgroundRendering<T>(_ body: () throws -> T) rethrows -> T
{
    let previous = currentImageCellBackgroundRenderingDepth()
    setCurrentImageCellBackgroundRenderingDepth(previous + 1)
    defer
    {
        setCurrentImageCellBackgroundRenderingDepth(previous)
    }
    return try body()
}

private func currentImageCellBackgroundRenderingDepth() -> Int
{
    Thread.current.threadDictionary[kImageCellBackgroundRenderingDepthThreadKey] as? Int ?? 0
}

private func setCurrentImageCellBackgroundRenderingDepth(_ depth: Int)
{
    if depth > 0
    {
        Thread.current.threadDictionary[kImageCellBackgroundRenderingDepthThreadKey] = depth
    }
    else
    {
        Thread.current.threadDictionary.removeObject(forKey: kImageCellBackgroundRenderingDepthThreadKey)
    }
}

private func optionalIntArg(_ arguments: [String: Any], _ key: String) -> Int?
{
    if let value = arguments[key] as? Int
    {
        return value
    }
    if let value = arguments[key] as? NSNumber
    {
        return value.intValue
    }
    if let value = arguments[key] as? String
    {
        return Int(value)
    }
    return nil
}

private func intArg(_ arguments: [String: Any], _ key: String, _ fallback: Int) -> Int
{
    if let value = arguments[key] as? Int
    {
        return value
    }
    if let value = arguments[key] as? NSNumber
    {
        return value.intValue
    }
    if let value = arguments[key] as? String, let parsed = Int(value)
    {
        return parsed
    }
    return fallback
}

private func stringValue(_ value: Any?) -> String
{
    if let value = value as? String
    {
        return value
    }
    if let value = value as? NSNumber
    {
        return value.stringValue
    }
    return "\(value ?? "")"
}

private func boolValue(_ value: Any?) -> Bool
{
    if let value = value as? Bool
    {
        return value
    }
    if let value = value as? NSNumber
    {
        return value.boolValue
    }
    if let value = value as? String
    {
        let normalized = value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
        return !(normalized.isEmpty || normalized == "0" || normalized == "false" || normalized == "no" || normalized == "off")
    }
    return false
}

private func jsonString(_ value: Any) -> String
{
    let data = (try? JSONSerialization.data(withJSONObject: value, options: [.sortedKeys])) ?? Data("null".utf8)
    return String(data: data, encoding: .utf8) ?? "null"
}

private func parseJsonObject(_ value: String) -> Any?
{
    guard let data = value.data(using: .utf8) else
    {
        return nil
    }
    return try? JSONSerialization.jsonObject(with: data)
}

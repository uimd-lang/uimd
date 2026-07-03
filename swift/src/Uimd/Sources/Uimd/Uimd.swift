import Darwin
import Foundation
import CUimdImageDecoder

private let kComboBoxClosedRows = 1
private let kComboBoxDropdownRows = 6
private let kTextTabWidth = 4
private let kModalBackgroundDimFactor = 0.5
private let kRenderTimeOverrideThreadKey = "uimd.renderTimeOverrideMs"
private let kImageCellBackgroundRenderingDepthThreadKey = "uimd.imageCellBackgroundRenderingDepth"
private let kNoActiveDynamicEditName = "__uimd_no_active_dynamic_edit__"
private let kTerminalEscapeReadDelayMicros: useconds_t = 1_000
private let kTerminalAnsiBaseRow = 1
private let kTerminalAnsiBaseCol = 1
private let kTerminalDefaultForeground = 39
private let kTerminalDefaultBackground = 49
private let kTerminalPasteMaxBytes = 1_048_576
private let kImageCellPixelWidth = 8
private let kImageCellPixelHeight = 16
private let kFallbackVerticalSamplesPerCell = 2
private let kTestFallbackCheckerTilePixels = 4
private let kTestFallbackCheckerLightAlpha = 160
private let kTestFallbackColorQuantum = 32
private let kImageInfoSampleGridSize = 3
private let kFallbackUpperHalfBlock = "▀"
private let kFallbackFullBlock = "█"
private let kMissingImagePlaceholder = "image"
private let kMinimumRenderableSize = 1
private let kScrollIndicatorAbove = "^"
private let kScrollIndicatorBelow = "v"
public let generatedScrollKeyboardStepRows = 1
public let generatedScrollIndicatorOverlapRows = 1
private let kTerminalEnterAlternateScreen = "\u{001B}[?1049h\u{001B}[?1000h\u{001B}[?1002h\u{001B}[?1006h\u{001B}[?2004h\u{001B}[>4;2m\u{001B}[?25l\u{001B}[2J"
private let kTerminalLeaveAlternateScreen = "\u{001B}[0m\u{001B}[2J\u{001B}[H\u{001B}[>4;0m\u{001B}[?25h\u{001B}[?2004l\u{001B}[?1006l\u{001B}[?1002l\u{001B}[?1000l\u{001B}[?1049l\r\n"
private let kTerminalSynchronizedUpdateBegin = "\u{001B}[?2026h"
private let kTerminalSynchronizedUpdateEnd = "\u{001B}[?2026l"

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
}

public struct TerminalCell: Equatable
{
    public var text: String
    public var foreground: Color?
    public var background: Color?

    public init(_ text: String = " ", foreground: Color? = nil, background: Color? = nil)
    {
        self.text = text.isEmpty ? " " : text
        self.foreground = foreground
        self.background = background
    }
}

public struct ElementRenderState
{
    public var focused: Bool = false
    public var editMode: Bool = false
    public var parentBackground: Color?

    public init(focused: Bool = false, editMode: Bool = false, parentBackground: Color? = nil)
    {
        self.focused = focused
        self.editMode = editMode
        self.parentBackground = parentBackground
    }
}

private func hasPartialAlpha(_ color: Color?) -> Bool
{
    guard let rgba = color?.rgba else
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
    public var frame = Rect(row: 0, col: 0, width: 0, height: 0)
    public var style = Style()
    public var focusStyle: Style?
    public var editStyle: Style?
    public var cursorStyle: Style?
    public var selectedStyle: Style?
    public var checkedStyle: Style?
    public var uncheckedStyle: Style?
    public var disabledStyle: Style?
    public var children: [UIElement] = []

    public init(_ name: String)
    {
        self.name = name
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
            return cellBackgroundImageContent(
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
        let resolvedMode = resolvedImageRenderMode(self)
        if resolvedMode == "sixel"
        {
            return imageBlankContent(width: width, height: height, style: style)
        }
        if resolvedMode != "deterministic"
        {
            return fallbackImageContent(
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
        return deterministicImageContent(
            raster: raster,
            width: width,
            height: height,
            fit: fit,
            align: align,
            verticalAlign: verticalAlign,
            style: style,
            background: background,
            checker: renderMode.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() != "fallback",
            quantized: resolvedMode == "deterministic"
        )
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

    open override func handleKey(_ key: String) -> Bool
    {
        if key == "Backspace"
        {
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
            cursor = max(0, cursor - 1)
            return true
        }
        if key == "Right"
        {
            cursor = min(value.count, cursor + 1)
            return true
        }
        if key == "Home"
        {
            cursor = 0
            return true
        }
        if key == "End"
        {
            cursor = value.count
            return true
        }
        if key.count == 1
        {
            insertText(key)
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

    open override var elementType: String
    {
        "textarea"
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let width = safeWidth(size.width, value)
        let height = safeHeight(size.height)
        let base = effectiveStyle(state)
        let chars = Array(value)
        cursor = max(0, min(cursor, chars.count))

        var visualRows: [(text: String, start: Int, end: Int)] = []
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
                visualRows.append(("", segmentStart, segmentStart))
            }
            else
            {
                var offset = 0
                while offset < segment.count
                {
                    let remaining = segment.count - offset
                    let chunkSize = min(remaining, width)
                    if chunkSize < width
                    {
                        visualRows.append((
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
                        visualRows.append((
                            String(segment[offset..<(offset + lastSpace)]),
                            segmentStart + offset,
                            segmentStart + offset + lastSpace
                        ))
                        offset += lastSpace + 1
                    }
                    else
                    {
                        visualRows.append((
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

        func cursorLocation() -> (row: Int, col: Int)
        {
            for index in visualRows.indices
            {
                let row = visualRows[index]
                if cursor <= row.end && (row.text.count < width || cursor < row.end)
                {
                    return (index, max(0, min(width - 1, cursor - row.start)))
                }
            }
            let lastIndex = max(0, visualRows.count - 1)
            let last = visualRows[lastIndex]
            return (lastIndex, max(0, min(width - 1, last.end - last.start)))
        }

        let cursorPosition = cursorLocation()
        let maxOffset = max(0, visualRows.count - height)
        rowScrollOffset = clamped(rowScrollOffset, lower: 0, upper: maxOffset)
        if state.editMode
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
            if state.editMode && rowIndex == cursorPosition.row && width > 0
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
        numberValue = value
        super.setValue(formatNumber(value))
        editText = ""
        editCursor = 0
        editing = false
        replaceOnFirstTextInput = false
    }

    open override func setText(_ value: String)
    {
        setValue(Double(value) ?? 0.0)
    }

    open override func handleKey(_ key: String) -> Bool
    {
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
            let cursorCol = max(0, min(width - 1, editCursor))
            row[cursorCol].foreground = cursorEffectiveStyle.color
            row[cursorCol].background = cursorEffectiveStyle.background
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
    public var dropdownOpen = false

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
        if key == "Enter"
        {
            dropdownOpen = false
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let selected = terminalText
        let width = safeWidth(size.width, selected)
        let expanded = state.editMode && dropdownOpen
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
    private var scrollOffset = 0

    public init(_ name: String, _ options: [String] = [], selectedValues: [String] = [])
    {
        self.selectedValues = selectedValues
        super.init(name, options, selectedIndex: selectedValues.first.flatMap { options.firstIndex(of: $0) } ?? -1)
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
        }
    }

    public func setDisabledValues(_ values: [String])
    {
        disabledValues = values
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

    open override func handleKey(_ key: String) -> Bool
    {
        guard !options.isEmpty else
        {
            return false
        }
        if key == "Down"
        {
            selectedIndex = min(options.count - 1, max(0, selectedIndex) + 1)
            if !multiple
            {
                selectedValues = [options[selectedIndex]]
            }
            return true
        }
        if key == "Up"
        {
            selectedIndex = max(0, selectedIndex - 1)
            if !multiple
            {
                selectedValues = [options[selectedIndex]]
            }
            return true
        }
        if key == "Enter"
        {
            let value = options[max(0, min(selectedIndex, options.count - 1))]
            if multiple
            {
                if !selectedValues.contains(value)
                {
                    selectedValues.append(value)
                }
            }
            else
            {
                selectedValues = [value]
            }
            return true
        }
        return false
    }

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let height = safeHeight(size.height)
        let selected = selectedIndex >= 0 && selectedIndex < options.count ? options[selectedIndex] : ""
        let width = safeWidth(size.width, selected)
        if options.isEmpty
        {
            return renderPlainText("", width: width, height: height, style: effectiveStyle(state))
        }

        let maxOffset = max(0, options.count - height)
        scrollOffset = clamped(scrollOffset, lower: 0, upper: maxOffset)
        if selectedIndex < scrollOffset
        {
            scrollOffset = max(0, selectedIndex)
        }
        else if selectedIndex >= scrollOffset + height
        {
            scrollOffset = clamped(selectedIndex - height + 1, lower: 0, upper: maxOffset)
        }

        let base = effectiveStyle(state)
        let hasAbove = scrollOffset > 0
        let hasBelow = scrollOffset + height < options.count
        var rows: [[TerminalCell]] = []
        rows.reserveCapacity(height)
        for row in 0..<height
        {
            let optionIndex = scrollOffset + row
            let selectedRow = optionIndex < options.count
                && (selectedValues.contains(options[optionIndex]) || (multiple && state.editMode && optionIndex == selectedIndex))
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
            let rowStyle = disabledRow ? base.merged(disabledStyle) : (selectedRow ? base.merged(selectedStyle) : base)
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
    private var renderedChildren: [[[TerminalCell]]] = []
    private var dynamicChildrenRenderer: ((Int) -> [[[TerminalCell]]])?
    private var dynamicChildrenWidth: Int?
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
        if autoScroll
        {
            viewOffset = 0
        }
    }

    public func clearChildren()
    {
        renderedChildren.removeAll()
        dynamicChildrenWidth = nil
        scrollOffset = 0
        viewOffset = 0
        lastNaturalSkip = 0
    }

    public func setDynamicChildrenRenderer(_ renderer: @escaping (Int) -> [[[TerminalCell]]])
    {
        dynamicChildrenRenderer = renderer
        invalidateDynamicChildren()
    }

    public func invalidateDynamicChildren()
    {
        dynamicChildrenWidth = nil
    }

    public func viewOffsetValue() -> Int
    {
        viewOffset
    }

    public func scrollOffsetValue() -> Int
    {
        scrollOffset
    }

    public func contentHeight(width: Int? = nil) -> Int
    {
        let children = currentChildren(width: width ?? max(1, frame.width))
        guard !children.isEmpty else
        {
            return 0
        }
        let childHeight = children.reduce(0) { $0 + max(1, $1.count) }
        return childHeight + max(0, children.count - 1) * max(0, gap)
    }

    public func maxViewOffset(viewport: Size) -> Int
    {
        let scrollViewport = generatedScrollViewport(size: viewport, style: style)
        let naturalSkip = max(0, contentHeight(width: max(1, scrollViewport.width)) - max(1, scrollViewport.height))
        syncViewOffsetForNaturalSkip(naturalSkip)
        lastNaturalSkip = naturalSkip
        return naturalSkip
    }

    @discardableResult
    public func scrollBy(_ delta: Int, viewport: Size) -> Bool
    {
        scrollLines(delta, viewport: viewport)
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

    @discardableResult
    public func handleKey(_ key: String, viewport: Size) -> Bool
    {
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

    open override func render(size: Size, state: ElementRenderState = ElementRenderState()) -> [[TerminalCell]]
    {
        let effective = effectiveStyle(state)
        let width = max(1, size.width)
        let height = max(1, size.height)
        let fillCell = TerminalCell(" ", foreground: effective.color, background: effective.background)
        var output = Array(repeating: Array(repeating: fillCell, count: width), count: height)
        let viewport = generatedScrollViewport(size: Size(width: width, height: height), style: effective)
        let children = currentChildren(width: max(1, viewport.width))
        let rows = renderedRows(children: children, width: max(1, viewport.width), style: effective)
        let maxOffset = max(0, rows.count - max(1, viewport.height))
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
            if autoScroll
            {
                viewOffset = 0
            }
        }
        return renderedChildren
    }

    private func renderedRows(children: [[[TerminalCell]]], width: Int, style: Style) -> [[TerminalCell]]
    {
        let blank = scrollBlankRow(width: max(1, width), style: style)
        var rows: [[TerminalCell]] = []
        for index in children.indices
        {
            for row in children[index]
            {
                rows.append(inheritScrollRowStyle(
                    fitCells(row, width: max(1, width), fillCell: blank[0]),
                    parentStyle: style
                ))
            }
            if index < children.count - 1
            {
                for _ in 0..<max(0, gap)
                {
                    rows.append(blank)
                }
            }
        }
        return rows
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

open class ViewHost: UIElement
{
    public var child: GeneratedWindowBase?
    public var activeFocusedName: String?
    public var activeEditedName: String?
    public var activeEditMode = false

    public override init(_ name: String)
    {
        super.init(name)
        focusable = false
    }

    public func setChild(_ child: GeneratedWindowBase)
    {
        self.child = child
        lastFocusedChildId = nil
        childFocusScopeGeneration += 1
    }

    open override var elementType: String
    {
        "viewhost"
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
        var rendered = child.renderContentForHost(size: size, focusedName: activeFocusedName, editMode: activeEditMode)
        child.suppressActiveContainerDim = previousSuppressDim
        child.activeEditFocusName = previousActiveEditFocusName
        if state.focused && !child.suppressActiveContainerScopeVisuals,
           let focusBackground = focusStyle?.background ?? child.generatedWindowFocusStyle?.background
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

    public func setChild(_ child: GeneratedWindowBase)
    {
        self.child = child
        lastFocusedChildId = nil
        childFocusScopeGeneration += 1
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
        var rendered = child.renderContentForHost(size: size, focusedName: activeFocusedName, editMode: activeEditMode)
        child.activeEditFocusName = previousActiveEditFocusName
        if state.focused && !child.suppressActiveContainerScopeVisuals,
           let focusBackground = focusStyle?.background ?? child.generatedWindowFocusStyle?.background
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
    public var onMousePressBeforeFocused: ((Point) -> Bool)?
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
    public var onMousePressBeforeFocused: ((Point) -> Bool)?
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
        runtime.onMousePressBeforeFocused = frameOptions.onMousePressBeforeFocused ?? runtime.onMousePressBeforeFocused
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
            element.mcpElementId = nil
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
            return element.focusable && element.enabled
        }
    }

    public func focusableElementIds() -> [String]
    {
        elements.filter { $0.focusable && $0.enabled }.map(\.name) + dynamicFocusableIds()
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
            activeEditFocusName.hasPrefix(focusedName + "[")
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

    open func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
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
        var activeContainerScope: (element: UIElement, rect: Rect, dim: Bool)?
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
            var nestedFocus = childFocusName(hostName: entry.name, focusedName: focusedName)
            if nestedFocus == nil,
               let focusedName,
               let child = generatedWindowChild(of: element),
               child.element(named: focusedName) != nil
            {
                nestedFocus = focusedName
            }
            var nestedActiveEditedName = childActiveEditedName(hostName: entry.name)
            if nestedActiveEditedName == nil,
               editMode,
               let activeEditFocusName,
               activeEditFocusName != kNoActiveDynamicEditName,
               let child = generatedWindowChild(of: element),
               child.element(named: activeEditFocusName) != nil
            {
                nestedActiveEditedName = activeEditFocusName
            }
            let nestedFocusIsLocalChild = focusedName != nil &&
                generatedWindowChild(of: element)?.element(named: focusedName!) != nil
            let nestedActiveEditMode = nestedFocus != nil &&
                editMode &&
                (focusedNameIsInActiveEditScope(focusedName) || nestedFocusIsLocalChild)
            let state = ElementRenderState(
                focused: focusedName == entry.name,
                editMode: elementEditMode,
                parentBackground: parentBackground
            )
            if let viewHost = element as? ViewHost
            {
                viewHost.activeFocusedName = nestedFocus
                viewHost.activeEditedName = nestedActiveEditedName
                viewHost.activeEditMode = nestedActiveEditMode
            }
            if let reusable = element as? ReusableElement
            {
                reusable.activeFocusedName = nestedFocus
                reusable.activeEditedName = nestedActiveEditedName
                reusable.activeEditMode = nestedActiveEditMode
            }
            let activeViewHostContainerChild = (element as? ViewHost)
                .flatMap { host in immediateChildFocusName(nestedFocus).flatMap { host.child?.element(named: $0) } }
                .flatMap { $0 is ReusableElement || $0 is ViewHost ? $0 : nil }
            let activeReusableContainer =
                element is ReusableElement &&
                (elementEditMode || nestedFocus != nil) &&
                ownsGeneratedScrollableContent(element)
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
            var clip = cellContentClip(entry, cellRect: cellRect)
            if element is ComboBox, elementEditMode, !rendered.isEmpty
            {
                let clipBottom = max(clip.row + clip.height, rect.row + rendered.count)
                clip.height = max(0, clipBottom - clip.row)
            }
            blitClipped(buffer: &buffer, rendered: rendered, row: rect.row, col: rect.col, clip: clip)
            if editMode && activeReusableContainer
            {
                activeContainerScope = (element, rect, true)
            }
            else if editMode, let activeViewHostContainerChild
            {
                activeContainerScope = (
                    activeViewHostContainerChild,
                    Rect(
                        row: rect.row + activeViewHostContainerChild.frame.row,
                        col: rect.col + activeViewHostContainerChild.frame.col,
                        width: activeViewHostContainerChild.frame.width,
                        height: activeViewHostContainerChild.frame.height
                    ),
                    ownsGeneratedScrollableContent(activeViewHostContainerChild)
                )
            }
            else if !editMode && focusedName == entry.name && (element is ReusableElement || element is ViewHost)
            {
                activeContainerScope = (element, rect, false)
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
            let elementEditMode = focusedName == entry.name && editMode
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
                if let scrollView = generatedScrollViewChild(of: activeContainerScope.element)
                {
                    if generatedWindowChild(of: activeContainerScope.element)?.suppressActiveContainerScopeVisuals != true
                    {
                        applyScrollableHostFocusBackground(
                            buffer: &buffer,
                            active: scrollViewHostRect(scrollView: scrollView, fallback: activeContainerScope.rect),
                            style: scrollView.style,
                            focusBackground: activeContainerScope.element.focusStyle?.background,
                            baseBackground: activeContainerScope.element.style.background
                        )
                    }
                }
                else
                {
                    applyActiveElementFocusBackground(
                        buffer: &buffer,
                        active: activeContainerScope.rect,
                        style: activeContainerScope.element.style,
                        focusBackground: activeContainerScope.element.focusStyle?.background
                    )
                }
                if activeContainerScope.dim && !suppressActiveContainerDim
                {
                    dimOutsideActiveElement(
                        buffer: &buffer,
                        active: activeContainerScope.rect,
                        dim: activeContainerScope.element.style.scopeDimBackground
                    )
                }
            }
        }
        return buffer
    }

    fileprivate func renderContentForHost(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        forcedFullscreenRenderDepth += 1
        defer
        {
            forcedFullscreenRenderDepth -= 1
        }
        return renderContent(size: size, focusedName: focusedName, editMode: editMode)
    }
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
    private let rootDir: URL
    private var currentDir: URL
    private let browserMode: String
    private var initialFilename: String
    private let extensionFilter: String
    private var result = ""
    private var closed = false

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
        entries.setStyle(Self.entriesStyle())
        entries.setFocusStyle(Self.entriesFocusStyle())
        entries.setEditStyle(Self.entriesEditStyle())
        entries.setSelectedStyle(Self.entriesSelectedStyle())
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
            if key == "Enter", self.entries.selectedValues.first != nil
            {
                return self.acceptCurrent()
            }
            return false
        }
        return options
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
        entries.selectedIndex = rows.isEmpty ? -1 : selectedIndex
        entries.selectedValues = rows.isEmpty ? [] : [rows[selectedIndex]]
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

    private func close(_ path: String)
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

    init()
    {
        hasOriginalTermios = tcgetattr(STDIN_FILENO, &originalTermios) == 0
        if hasOriginalTermios
        {
            var raw = originalTermios
            cfmakeraw(&raw)
            _ = tcsetattr(STDIN_FILENO, TCSANOW, &raw)
        }
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

private final class GeneratedRuntimeController
{
    private let rootWindow: GeneratedWindowBase
    private let rootOptions: GeneratedWindowRuntimeOptions
    private var viewport: Rect
    private var rootFocusedName: String?
    private var rootEditMode = false
    private var rootActiveDynamicEditName: String?
    private var rememberedDynamicFocusByParentId: [String: RememberedDynamicFocus] = [:]
    private var pendingMcpMouseDown: Point?
    private var mousePressActivatedClickControl = false

    init(window: GeneratedWindowBase, options: GeneratedWindowRuntimeOptions, args: [String])
    {
        self.rootWindow = window
        self.rootOptions = options
        let size = window.generatedContentSize()
        self.viewport = parseViewport(args) ?? Rect(row: 0, col: 0, width: size.width, height: size.height)
        self.rootFocusedName = options.initialFocusName
        self.rootEditMode = options.startInEditMode
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

        let mode = TerminalModeGuard()
        defer
        {
            mode.restore()
        }

        var pendingMouseDown: Point?
        while true
        {
            if options.shouldClose?() == true
            {
                return 0
            }
            let size = currentTerminalSize(fallback: rootWindow.generatedContentSize())
            viewport = Rect(row: 0, col: 0, width: size.width, height: size.height)
            syncImageTerminalCellPixels()
            writeTerminal(renderTerminalFrame(renderContent()))

            guard let event = readTerminalInputEvent() else
            {
                continue
            }
            do
            {
                let keepRunning = try handleTerminalInput(event, pendingMouseDown: &pendingMouseDown)
                if !keepRunning
                {
                    return 0
                }
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
        switch event
        {
        case .none:
            return true
        case .key(let key):
            if key == "Ctrl+C"
            {
                return false
            }
            _ = try pressKey(key)
            return true
        case .paste(let text):
            guard let focusedName else
            {
                return true
            }
            _ = try typeText(focusedName, text)
            return true
        case .mouse(let kind, let point):
            let arguments: [String: Any] = ["x": point.col, "y": point.row]
            if kind == "press"
            {
                pendingMouseDown = point
                _ = try mousePointerEvent(arguments, pressed: true)
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
                _ = try mousePointerEvent(arguments, pressed: false)
            }
            else
            {
                let isClick = pendingMouseDown == point
                pendingMouseDown = nil
                if isClick
                {
                    _ = try mouseClick(arguments)
                }
                else
                {
                    _ = try mousePointerEvent(arguments, pressed: false)
                }
            }
            return true
        case .scroll(let point, let delta):
            _ = try scrollAt(point: point, delta: delta)
            return true
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
            activeFrame?.focusedName ?? rootFocusedName
        }
        set
        {
            let previous = activeFrame?.focusedName ?? rootFocusedName
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
            activeFrame?.activeDynamicEditName ?? rootActiveDynamicEditName
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
        let modalClassName = activeFrame?.window.mcpClassName
        cleanupClosedStackFrames()
        let result = try callToolBody(name, arguments)
        cleanupClosedStackFrames()
        cleanupBackgroundFocusAfterModalClose(wasOpen: hadModalStack, modalClassName: modalClassName, toolName: name, arguments: arguments)
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

    private func cleanupBackgroundFocusAfterModalClose(
        wasOpen: Bool,
        modalClassName: String?,
        toolName: String,
        arguments: [String: Any]
    )
    {
        guard wasOpen && !hasOpenModalStack() else
        {
            return
        }
        let standardEscapeClose = toolName == "press_key"
            && (arguments["key"] as? String) == "Escape"
            && isStandardModalClass(modalClassName)
        let restoreNestedEditMode = !standardEscapeClose
        guard let focused = rootFocusedName,
              let element = rootWindow.element(named: focused)
        else
        {
            rootEditMode = false
            rootActiveDynamicEditName = nil
            return
        }
        guard let parentId = element.parentFocusHostId,
              let parent = rootWindow.element(named: parentId),
              parent is ReusableElement || parent is ViewHost
        else
        {
            rootEditMode = false
            rootActiveDynamicEditName = nil
            return
        }
        rememberDynamicFocus(parentId: parentId, parent: parent, elementId: focused)
        rootEditMode = restoreNestedEditMode
        rootActiveDynamicEditName = nil
    }

    private func isStandardModalClass(_ className: String?) -> Bool
    {
        guard let className, !className.isEmpty else
        {
            return false
        }
        return className == "FileBrowser" || className.hasPrefix("MessageBox")
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
            return snapshot(try requireElement(arguments["element_id"] as? String))
        case "get_focused_element":
            guard let focusedName, let element = window.element(named: focusedName) else
            {
                return NSNull()
            }
            return snapshot(element)
        case "get_state":
            return ["window": getWindow(), "focused_element": try callTool("get_focused_element", [:]), "edit_mode": editMode, "elements": window.elements.map { snapshot($0) }]
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
            let content = renderContent()
            return ["width": content.first?.count ?? 0, "height": content.count, "cells": compactCells(content)]
        case "get_render_snapshot":
            return withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                let content = renderContent()
                let lines = renderedText(content)
                return ["ansi_lines": lines, "text_lines": lines, "cells": fullCells(content)]
            }
        case "get_render_snapshot_compact":
            return withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                ["format": "render-cells-v1", "cells": compactCells(renderContent())]
            }
        case "get_render_cell":
            let row = intArg(arguments, "y", intArg(arguments, "row", 0))
            let col = intArg(arguments, "x", intArg(arguments, "col", 0))
            return try withRenderTimeOverride(optionalIntArg(arguments, "snapshot_time_ms")) {
                let content = renderContent()
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
            return ["lines": renderedText(renderContent())]
        case "focus_element":
            let element = try requireElement(arguments["element_id"] as? String)
            let elementId = arguments["element_id"] as? String ?? runtimeElementId(element)
            focusedName = elementId
            editMode = false
            activeDynamicEditName = nil
            rememberParentDynamicFocus(elementId, element)
            return snapshot(element)
        case "enter_edit_mode":
            let requestedId = arguments["element_id"] as? String
            if let id = arguments["element_id"] as? String
            {
                let element = try requireElement(id)
                focusedName = requestedId ?? runtimeElementId(element)
                rememberParentDynamicFocus(focusedName ?? runtimeElementId(element), element)
            }
            editMode = true
            if let focusedName
            {
                if let element = window.element(named: focusedName)
                {
                    activeDynamicEditName = element.parentFocusHostId == nil ? nil : focusedName
                }
                if let combo = window.element(named: focusedName) as? ComboBox
                {
                    combo.dropdownOpen = true
                }
                _ = options.onEditStarted?(focusedName)
            }
            return ["edit_mode": editMode]
        case "exit_edit_mode":
            editMode = false
            activeDynamicEditName = nil
            return ["edit_mode": editMode]
        case "get_edit_mode":
            return ["edit_mode": editMode]
        case "activate_element", "click_element":
            return try activateElement(arguments["element_id"] as? String, click: name == "click_element")
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
        [
            "title": window.windowTitle,
            "class": window.mcpClassName,
            "backend": "swift",
            "mode": resolvedWindowMode(),
            "description": window.mcpDescription,
            "width": viewport.width,
            "height": viewport.height,
            "mcp_enabled": window.mcpEnabled,
        ]
    }

    private func snapshot(_ element: UIElement) -> [String: Any]
    {
        _ = renderContent()
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
        let sourceSize = imageFileSize(sourcePath)
        let sourceLoaded = sourceSize != nil
        let sourceWidth = sourceSize?.width ?? 0
        let sourceHeight = sourceSize?.height ?? 0
        var resolvedMode = resolvedImageRenderMode(image)
        if !sourceLoaded
        {
            resolvedMode = "placeholder"
        }
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
            "sample_signature": imageInfoSampleSignature(source: image.source),
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
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        guard element.enabled else
        {
            return snapshot(element)
        }
        let nestedContainerElement = element.parentFocusHostId.flatMap { window.element(named: $0) } is ReusableElement
        let parentFocusHostId = element.parentFocusHostId
        let previousRememberedDynamicFocus = parentFocusHostId.flatMap { rememberedDynamicFocusByParentId[$0] }
        let modalDepthBeforeActivation = rootOptions.windowStack?.allFrames.count ?? 0
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
        focusedName = elementId
        editMode = false
        activeDynamicEditName = nil
        rememberParentDynamicFocus(elementId, element)
        if element.parentFocusHostId != nil
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
                editMode = false
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
                    return ["ok": true, "element_id": element.name]
                }
            }
            else
            {
                _ = rootOptions.onButton?(elementId)
            }
            restoreRememberedDynamicFocusIfModalOpened()
        }
        else if let checkbox = element as? CheckBox
        {
            checkbox.checked.toggle()
            _ = options.onTextChanged?(elementId, checkbox.checked ? "true" : "false")
        }
        else if element is ReusableElement
        {
            _ = options.onButton?(elementId)
        }
        else if click && (element is ComboBox || element is ListBox)
        {
            if let combo = element as? ComboBox
            {
                combo.dropdownOpen = true
            }
            editMode = true
            activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        }
        return snapshot(element)
    }

    private func scrollElement(_ arguments: [String: Any]) throws -> Any
    {
        let element = try requireElement(arguments["element_id"] as? String)
        _ = renderContent()
        let delta = intArg(arguments, "delta", 0)
        let steps = max(1, abs(delta))
        let scrollDelta = delta < 0 ? 1 : -1
        if let list = element as? ListBox
        {
            for _ in 0..<steps
            {
                list.scrollBy(scrollDelta, viewportHeight: list.frame.height)
            }
            return snapshot(list)
        }
        if let textArea = element as? TextArea
        {
            for _ in 0..<steps
            {
                textArea.rowScrollOffset = max(0, textArea.rowScrollOffset + scrollDelta)
            }
            return snapshot(textArea)
        }
        if let reusable = element as? ReusableElement,
           let scrollable = reusable.child as? GeneratedScrollableContent
        {
            for _ in 0..<steps
            {
                scrollable.scrollBy(scrollDelta, viewport: Size(width: reusable.frame.width, height: reusable.frame.height))
            }
            return snapshot(reusable)
        }
        if let viewHost = element as? ViewHost,
           let scrollable = viewHost.child as? GeneratedScrollableContent
        {
            for _ in 0..<steps
            {
                scrollable.scrollBy(scrollDelta, viewport: Size(width: viewHost.frame.width, height: viewHost.frame.height))
            }
            return snapshot(viewHost)
        }
        throw RuntimeError("Element is not scrollable: \(element.name)")
    }

    private func scrollAt(point: Point, delta: Int) throws -> Any
    {
        let content = contentPoint(["x": point.col, "y": point.row])
        if let element = elementAt(row: content.row, col: content.col), element is ListBox || element is TextArea
        {
            return try scrollElement(["element_id": element.name, "delta": delta])
        }
        if let focusedName, let element = window.element(named: focusedName), element is ListBox || element is TextArea
        {
            return try scrollElement(["element_id": element.name, "delta": delta])
        }
        return try callTool("get_state", [:])
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

    private func updateFocusForMouse(row: Int, col: Int)
    {
        if let target = elementAt(row: row, col: col), target.focusable && target.enabled
        {
            focusedName = runtimeElementId(target)
        }
        else
        {
            focusedName = nil
        }
    }

    private func mousePointerEvent(_ arguments: [String: Any], pressed: Bool) throws -> Any
    {
        _ = renderContent()
        let point = contentPoint(arguments)
        let clickedElement = elementAt(row: point.row, col: point.col)
        if pressed
        {
            mousePressActivatedClickControl = false
            if options.onMousePressBeforeFocused?(Point(row: point.row, col: point.col)) == true
            {
                editMode = false
                return try callTool("get_state", [:])
            }
            updateFocusForMouse(row: point.row, col: point.col)
            editMode = false
            if let activated = try activateClickControl(clickedElement)
            {
                _ = activated
                mousePressActivatedClickControl = true
                pendingMcpMouseDown = nil
                return try callTool("get_state", [:])
            }
            return try callTool("get_state", [:])
        }
        return try callTool("get_state", [:])
    }

    private func mouseClick(_ arguments: [String: Any]) throws -> Any
    {
        _ = renderContent()
        let point = contentPoint(arguments)
        let row = point.row
        let col = point.col
        let clickedElement = elementAt(row: row, col: col)
        if editMode,
           let focusedName,
           let element = window.element(named: focusedName)
        {
            if let list = element as? ListBox
            {
                guard rectContains(list.frame, row: row, col: col) else
                {
                    editMode = false
                    updateFocusForMouse(row: row, col: col)
                    if let activated = try activateClickControl(clickedElement)
                    {
                        return activated
                    }
                    if let activated = try activateFocusedClickControl()
                    {
                        return activated
                    }
                    enterEditModeForFocusedTextInput()
                    return try callTool("get_state", [:])
                }
                guard !list.options.isEmpty else
                {
                    editMode = false
                    return snapshot(list)
                }
                let index = clamped(list.visibleScrollOffset + row - list.frame.row, lower: 0, upper: list.options.count - 1)
                let value = list.options[index]
                if list.multiple
                {
                    if list.selectedValues.contains(value)
                    {
                        list.selectedValues = list.selectedValues.filter { $0 != value }
                    }
                    else
                    {
                        list.selectedValues.append(value)
                    }
                }
                else
                {
                    list.selectedValues = [value]
                }
                list.selectedIndex = index
                _ = options.onSelectionChanged?(list.name, list.selectedValues)
                editMode = true
                return snapshot(list)
            }
            guard let combo = element as? ComboBox else
            {
                editMode = false
                updateFocusForMouse(row: row, col: col)
                if let activated = try activateClickControl(clickedElement)
                {
                    return activated
                }
                if let activated = try activateFocusedClickControl()
                {
                    return activated
                }
                enterEditModeForFocusedTextInput()
                return try callTool("get_state", [:])
            }
            let dropdown = comboBoxDropdownFrame(combo)
            guard rectContains(dropdown, row: row, col: col) else
            {
                editMode = false
                updateFocusForMouse(row: row, col: col)
                if let activated = try activateFocusedClickControl()
                {
                    return activated
                }
                enterEditModeForFocusedTextInput()
                return try callTool("get_state", [:])
            }
            guard !combo.options.isEmpty else
            {
                editMode = false
                return snapshot(combo)
            }
            let localRow = row - combo.frame.row
            if localRow >= kComboBoxClosedRows
            {
                let index = comboBoxOptionIndex(forLocalRow: localRow, optionCount: combo.options.count)
                let value = combo.options[index]
                combo.selectedIndex = index
                _ = options.onSelectionChanged?(combo.name, [value])
            }
            editMode = false
            return snapshot(combo)
        }
        editMode = false
        updateFocusForMouse(row: row, col: col)
        if let activated = try activateClickControl(clickedElement)
        {
            return activated
        }
        if let activated = try activateFocusedClickControl()
        {
            return activated
        }
        enterEditModeForFocusedTextInput()
        return try callTool("get_state", [:])
    }

    private func activateClickControl(_ element: UIElement?) throws -> Any?
    {
        guard let element,
              element.enabled,
              element is Button || element is CheckBox || element is ReusableElement
        else
        {
            return nil
        }
        return try activateElement(runtimeElementId(element), click: true)
    }

    private func activateFocusedClickControl() throws -> Any?
    {
        guard let focusedName,
              let element = window.element(named: focusedName),
              element.enabled,
              element is Button || element is CheckBox || element is ReusableElement
        else
        {
            return nil
        }
        return try activateElement(focusedName, click: true)
    }

    private func enterEditModeForFocusedTextInput()
    {
        if let focusedName,
           let element = window.element(named: focusedName),
           element is TextInput
        {
            editMode = true
            activeDynamicEditName = element.parentFocusHostId == nil ? nil : focusedName
        }
    }

    private func pressKey(_ key: String) throws -> Any
    {
        if key == "Escape", handleStandardEscapeButton()
        {
            return try callTool("get_state", [:])
        }
        if options.onKey?(key) == true
        {
            return try callTool("get_state", [:])
        }
        if window.handleGeneratedKey(key)
        {
            return try callTool("get_state", [:])
        }
        if let focusedName, options.onKeyBeforeFocusedElement?(key, focusedName, editMode) == true
        {
            return try callTool("get_state", [:])
        }
        if !editMode && key == "Tab"
        {
            moveFocus(1)
            return try callTool("get_state", [:])
        }
        if !editMode && key == "Shift+Tab"
        {
            moveFocus(-1)
            return try callTool("get_state", [:])
        }
        if !editMode && ["Up", "Down", "Left", "Right"].contains(key)
        {
            _ = renderContent()
            if moveSpatialFocus(key)
            {
                return try callTool("get_state", [:])
            }
        }
        guard let currentFocusedName = focusedName, let element = window.element(named: currentFocusedName) else
        {
            return try callTool("get_state", [:])
        }
        if editMode,
           element.parentFocusHostId != nil,
           activeDynamicEditName == nil
        {
            if key == "Escape"
            {
                if ownsGeneratedScrollScope(element)
                {
                    editMode = false
                    return try callTool("get_state", [:])
                }
                if let parentId = element.parentFocusHostId
                {
                    if let parent = window.element(named: parentId)
                    {
                        rememberDynamicFocus(parentId: parentId, parent: parent, elementId: currentFocusedName)
                    }
                    focusedName = parentId
                }
                editMode = false
                return try callTool("get_state", [:])
            }
            if ["Up", "Down", "Left", "Right"].contains(key)
            {
                _ = moveDynamicFocus(from: currentFocusedName, key: key)
                return try callTool("get_state", [:])
            }
            if key == "Enter" || key == " "
            {
                if element is Button || element is ReusableElement
                {
                    _ = try activateElement(currentFocusedName)
                    editMode = true
                    activeDynamicEditName = nil
                    return try callTool("get_state", [:])
                }
                if element is CheckBox
                {
                    let before = element.valueForSnapshot
                    _ = element.handleKey(key)
                    notifyValueChange(element, before: before)
                    return try callTool("get_state", [:])
                }
                if element is TextInput || element is ComboBox
                {
                    activeDynamicEditName = currentFocusedName
                    if let combo = element as? ComboBox
                    {
                        combo.dropdownOpen = true
                    }
                    _ = options.onEditStarted?(currentFocusedName)
                    return try callTool("get_state", [:])
                }
            }
            return try callTool("get_state", [:])
        }
        if editMode && key == "Escape"
        {
            if activeDynamicEditName == currentFocusedName
            {
                activeDynamicEditName = nil
                return try callTool("get_state", [:])
            }
            if ownsGeneratedScrollScope(element)
            {
                editMode = false
                return try callTool("get_state", [:])
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
                return try callTool("get_state", [:])
            }
            editMode = false
            return try callTool("get_state", [:])
        }
        if !editMode && key == "Enter"
        {
            if enterChildFocusScope(element)
            {
                return try callTool("get_state", [:])
            }
            if ownsGeneratedScrollScope(element)
            {
                editMode = true
                _ = options.onEditStarted?(currentFocusedName)
                return try callTool("get_state", [:])
            }
            if element is Button || element is CheckBox || element is ReusableElement
            {
                _ = try activateElement(currentFocusedName)
                return try callTool("get_state", [:])
            }
            editMode = true
            if let combo = element as? ComboBox
            {
                combo.dropdownOpen = true
            }
            _ = options.onEditStarted?(element.name)
            return try callTool("get_state", [:])
        }
        if !editMode && key == " "
        {
            _ = try activateElement(currentFocusedName)
            return try callTool("get_state", [:])
        }
        if editMode && key == "Enter", let numberInput = element as? NumberInput
        {
            let before = numberInput.valueForSnapshot
            _ = numberInput.handleKey(key)
            notifyValueChange(numberInput, before: before)
            _ = options.onTextConfirmed?(currentFocusedName, numberInput.value)
            if element.parentFocusHostId != nil
            {
                activeDynamicEditName = nil
                editMode = shouldRemainInDynamicFocusScopeAfterConfirming(element)
            }
            else
            {
                editMode = false
            }
            return try callTool("get_state", [:])
        }
        if editMode && key == "Enter", element is TextInput
        {
            if let input = element as? TextInput
            {
            _ = options.onTextConfirmed?(currentFocusedName, input.value)
            }
            if element.parentFocusHostId != nil
            {
                activeDynamicEditName = nil
                editMode = shouldRemainInDynamicFocusScopeAfterConfirming(element)
            }
            else
            {
                editMode = false
            }
            return try callTool("get_state", [:])
        }
        if editMode || element is CheckBox || element is ComboBox || element is ListBox
        {
            let before = element.valueForSnapshot
            if editMode && (element is Button || element is CheckBox) && moveDynamicFocus(from: currentFocusedName, key: key)
            {
                return try callTool("get_state", [:])
            }
            if element.handleKey(key)
            {
                notifyValueChange(element, before: before)
                if key == "Enter", element is ComboBox
                {
                    if element.parentFocusHostId != nil
                    {
                        activeDynamicEditName = nil
                        editMode = shouldRemainInDynamicFocusScopeAfterConfirming(element)
                    }
                    else
                    {
                        editMode = false
                    }
                }
            }
            else if editMode && moveDynamicFocus(from: currentFocusedName, key: key)
            {
                return try callTool("get_state", [:])
            }
        }
        return try callTool("get_state", [:])
    }

    private func setText(_ id: String?, _ text: String) throws -> Any
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        focusedName = elementId
        editMode = true
        activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        rememberParentDynamicFocus(elementId, element)
        element.setText(text)
        _ = options.onTextChanged?(elementId, text)
        return snapshot(element)
    }

    private func typeText(_ id: String?, _ text: String) throws -> Any
    {
        let element = try requireElement(id)
        let elementId = id ?? runtimeElementId(element)
        focusedName = elementId
        editMode = true
        activeDynamicEditName = element.parentFocusHostId == nil ? nil : elementId
        rememberParentDynamicFocus(elementId, element)
        if let input = element as? TextInput
        {
            input.insertText(text)
            _ = options.onTextChanged?(elementId, input.value)
        }
        else
        {
            element.setText(text)
        }
        return snapshot(element)
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
        focusedName = elementId
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
        let input = try requireTextInput(arguments["element_id"] as? String)
        input.selectRange(start: intArg(arguments, "start", 0), end: intArg(arguments, "end", 0))
        return selectionSnapshot(input)
    }

    private func copySelection(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        return ["text": input.selectedText()]
    }

    private func replaceSelection(_ arguments: [String: Any]) throws -> Any
    {
        let input = try requireTextInput(arguments["element_id"] as? String)
        input.replaceSelection(stringValue(arguments["text"]))
        _ = options.onTextChanged?(input.name, input.value)
        return snapshot(input)
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
            _ = options.onTextChanged?(elementId, checked ? "true" : "false")
            return snapshot(element)
        }
        if let list = element as? ListBox
        {
            let values = (value as? [String]) ?? [stringValue(value)]
            list.setSelectedValues(values)
            _ = options.onSelectionChanged?(elementId, list.selectedValues)
            return snapshot(element)
        }
        if let combo = element as? ComboBox
        {
            combo.selectValue(stringValue(value))
            _ = options.onSelectionChanged?(elementId, [combo.terminalText])
            return snapshot(element)
        }
        if let number = element as? NumberInput
        {
            number.setText(stringValue(value))
            _ = options.onTextChanged?(elementId, number.value)
            return snapshot(element)
        }
        if let input = element as? TextInput
        {
            input.setValue(stringValue(value))
            _ = options.onTextChanged?(elementId, input.value)
            return snapshot(element)
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
            _ = options.onSelectionChanged?(elementId, list.selectedValues)
            return snapshot(element)
        }
        guard let combo = element as? ComboBox else
        {
            throw RuntimeError("Element has no selectable options: \(element.name)")
        }
        combo.selectValue(value)
        _ = options.onSelectionChanged?(elementId, [value])
        return snapshot(element)
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
        _ = options.onSelectionChanged?(elementId, values)
        return snapshot(element)
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
        _ = options.onTextChanged?(elementId, value ? "true" : "false")
        return snapshot(element)
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
        if let text = after as? String
        {
            _ = options.onTextChanged?(elementId, text)
        }
        else if let values = after as? [String]
        {
            _ = options.onSelectionChanged?(elementId, values)
        }
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

    private func shouldRemainInDynamicFocusScopeAfterConfirming(_ element: UIElement) -> Bool
    {
        guard let parentId = element.parentFocusHostId,
              let parent = window.element(named: parentId)
        else
        {
            return false
        }
        return ownsGeneratedScrollScope(parent)
    }

    private func moveFocus(_ delta: Int)
    {
        let focusableIds = window.focusableElementIds()
        guard !focusableIds.isEmpty else
        {
            focusedName = nil
            return
        }
        let index = focusableIds.firstIndex { $0 == focusedName } ?? 0
        let next = (index + delta + focusableIds.count) % focusableIds.count
        focusedName = focusableIds[next]
    }

    private func moveSpatialFocus(_ key: String) -> Bool
    {
        guard let currentId = focusedName,
              let current = window.element(named: currentId)
        else
        {
            return false
        }
        let order = window.focusableElementIds().filter { window.element(named: $0) != nil }
        guard order.count > 1 else
        {
            return false
        }
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
        focusedName = target
        if let element = window.element(named: target)
        {
            rememberParentDynamicFocus(target, element)
        }
        return true
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
        activeDynamicEditName = nil
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
        guard let index = order.firstIndex(of: id), !order.isEmpty else
        {
            return false
        }
        let target: String?
        if key == "Up" || key == "Down"
        {
            target = siblingDynamicRowId(from: id, direction: key == "Down" ? 1 : -1, candidates: Set(order))
        }
        else if let spatial = horizontalDynamicFocusTarget(from: id, direction: key == "Right" ? 1 : -1, order: order)
        {
            target = spatial
        }
        else
        {
            let delta = key == "Right" ? 1 : -1
            let next = max(0, min(order.count - 1, index + delta))
            target = order[next]
        }
        guard let target, target != id else
        {
            rememberDynamicFocus(parentId: parentId, parent: parent, elementId: id)
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
        activeDynamicEditName = nil
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

    private func siblingDynamicRowId(from id: String, direction: Int, candidates: Set<String>) -> String?
    {
        guard let open = id.lastIndex(of: "["),
              let close = id[open...].firstIndex(of: "]"),
              let row = Int(id[id.index(after: open)..<close])
        else
        {
            return nil
        }
        let prefix = String(id[..<open])
        let suffixStart = id.index(after: close)
        let suffix = String(id[suffixStart...])
        let matchingRows = candidates.compactMap
        {
            siblingDynamicRowNumber($0, prefix: prefix, suffix: suffix)
        }
        guard let lowerBound = matchingRows.min(),
              let upperBound = matchingRows.max()
        else
        {
            return nil
        }
        var next = row + direction
        while next >= lowerBound && next <= upperBound
        {
            let candidate = "\(prefix)[\(next)]\(suffix)"
            if candidates.contains(candidate)
            {
                return candidate
            }
            if direction == 0
            {
                return nil
            }
            next += direction
        }
        return nil
    }

    private func siblingDynamicRowNumber(_ id: String, prefix: String, suffix: String) -> Int?
    {
        guard id.hasPrefix(prefix + "["),
              id.hasSuffix(suffix),
              let open = id.lastIndex(of: "["),
              let close = id[open...].firstIndex(of: "]")
        else
        {
            return nil
        }
        return Int(id[id.index(after: open)..<close])
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
        let contentFrame = targetWindow.renderContent(
            size: Size(width: content.width, height: content.height),
            focusedName: focusedName,
            editMode: editMode
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
        windowRow: Int,
        windowCol: Int
    )
    {
        guard editMode, let focusedName, let comboBox = targetWindow.element(named: focusedName) as? ComboBox, !(comboBox is ListBox) else
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
        syncImageTerminalCellPixels()
        let modalStackOpen = rootOptions.windowStack?.allFrames.isEmpty == false
        if modalStackOpen
        {
            return withImageCellBackgroundRendering
            {
                renderContentSurface(modalStackOpen: true)
            }
        }
        return renderContentSurface(modalStackOpen: false)
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
        let root = renderWindowFrame(
            rootWindow,
            focusedName: rootBackgroundFocusedName,
            editMode: rootBackgroundEditMode,
            activeEditName: rootBackgroundActiveEditName,
            suppressActiveContainerScopeVisuals: modalStackOpen
        )
        blit(buffer: &surface, rendered: root.frame, row: root.offset.row, col: root.offset.col)
        overlayFocusedComboBox(
            buffer: &surface,
            targetWindow: rootWindow,
            focusedName: rootBackgroundFocusedName,
            editMode: rootBackgroundEditMode,
            windowRow: root.offset.row,
            windowCol: root.offset.col
        )

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
            var rendered = renderWindowFrame(
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
                    : nil
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
    guard ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == 0,
          windowSize.ws_col > 0,
          windowSize.ws_row > 0,
          windowSize.ws_xpixel > 0,
          windowSize.ws_ypixel > 0
    else
    {
        return nil
    }
    let cols = Int(windowSize.ws_col)
    let rows = Int(windowSize.ws_row)
    let pixelWidth = max(1, (Int(windowSize.ws_xpixel) + cols / 2) / cols)
    let pixelHeight = max(1, (Int(windowSize.ws_ypixel) + rows / 2) / rows)
    return Size(width: pixelWidth, height: pixelHeight)
}

private func syncImageTerminalCellPixels()
{
    guard let detected = detectedTerminalCellPixels() else
    {
        return
    }
    setImageTerminalCellPixels(detected)
}

private func terminalCellPixels() -> Size
{
    if let override = imageTerminalCellPixelsOverride.get()
    {
        return override
    }
    return detectedTerminalCellPixels() ?? Size(width: kImageCellPixelWidth, height: kImageCellPixelHeight)
}

private func renderTerminalFrame(_ cells: [[TerminalCell]]) -> String
{
    var output = kTerminalSynchronizedUpdateBegin
    for row in 0..<cells.count
    {
        var col = 0
        while col < cells[row].count
        {
            let styleCell = cells[row][col]
            let startCol = col
            var run = ""
            while col < cells[row].count,
                  cells[row][col].foreground == styleCell.foreground,
                  cells[row][col].background == styleCell.background
            {
                run += safeTerminalCellText(cells[row][col].text)
                col += 1
            }
            output += "\u{001B}[\(row + kTerminalAnsiBaseRow);\(startCol + kTerminalAnsiBaseCol)H"
            output += sgrForCell(styleCell)
            output += run
        }
    }
    output += "\u{001B}[0m"
    output += kTerminalSynchronizedUpdateEnd
    return output
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

private func readTerminalInputEvent() -> TerminalInputEvent?
{
    guard let byte = readTerminalByte() else
    {
        return nil
    }
    switch byte
    {
    case 3, 4:
        return .key("Ctrl+C")
    case 9:
        return .key("Tab")
    case 10, 13:
        return .key("Enter")
    case 27:
        usleep(kTerminalEscapeReadDelayMicros)
        let rest = drainTerminalInputBytes()
        return parseTerminalEscape(rest)
    case 127, 8:
        return .key("Backspace")
    default:
        if byte >= 32
        {
            if byte >= 0xC2
            {
                usleep(kTerminalEscapeReadDelayMicros)
                let bytes = [byte] + drainTerminalInputBytes()
                if let text = String(bytes: bytes, encoding: .utf8), !text.isEmpty
                {
                    return text.count == 1 ? .key(text) : .paste(text)
                }
            }
            return .key(String(UnicodeScalar(byte)))
        }
        return TerminalInputEvent.none
    }
}

private func readTerminalByte() -> UInt8?
{
    var byte: UInt8 = 0
    let count = Darwin.read(STDIN_FILENO, &byte, 1)
    return count == 1 ? byte : nil
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

private func parseTerminalEscape(_ bytes: [UInt8]) -> TerminalInputEvent
{
    guard !bytes.isEmpty else
    {
        return .key("Escape")
    }
    if bytes.starts(with: Array("[200~".utf8))
    {
        return readBracketedPaste(startingWith: Array(bytes.dropFirst(Array("[200~".utf8).count)))
    }
    guard let sequence = String(bytes: bytes, encoding: .utf8) else
    {
        return .key("Escape")
    }
    if sequence.hasPrefix("[<"), let mouse = parseSgrMouse(sequence)
    {
        return mouse
    }
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
        "OA": "Up",
        "OB": "Down",
        "OC": "Right",
        "OD": "Left",
        "OH": "Home",
        "OF": "End",
    ]
    if let key = keyMap[sequence]
    {
        return .key(key)
    }
    if sequence.hasPrefix("[1;2")
    {
        if sequence.hasSuffix("A") { return .key("Shift+Up") }
        if sequence.hasSuffix("B") { return .key("Shift+Down") }
        if sequence.hasSuffix("C") { return .key("Shift+Right") }
        if sequence.hasSuffix("D") { return .key("Shift+Left") }
    }
    return .key("Escape")
}

private func readBracketedPaste(startingWith initialPayload: [UInt8]) -> TerminalInputEvent
{
    let terminator = Array("\u{001B}[201~".utf8)
    var payload = initialPayload
    while payload.count < kTerminalPasteMaxBytes
    {
        if let terminatorIndex = firstIndex(of: terminator, in: payload)
        {
            let textBytes = Array(payload[..<terminatorIndex])
            return .paste(String(bytes: textBytes, encoding: .utf8) ?? "")
        }
        guard let byte = readTerminalByte() else
        {
            break
        }
        payload.append(byte)
    }
    return .paste(String(bytes: payload, encoding: .utf8) ?? "")
}

private func firstIndex(of needle: [UInt8], in haystack: [UInt8]) -> Int?
{
    guard !needle.isEmpty, haystack.count >= needle.count else
    {
        return nil
    }
    for index in 0...(haystack.count - needle.count)
    {
        if Array(haystack[index..<(index + needle.count)]) == needle
        {
            return index
        }
    }
    return nil
}

private func parseSgrMouse(_ sequence: String) -> TerminalInputEvent?
{
    let final = sequence.last
    let body = sequence.dropFirst(2).dropLast()
    let parts = body.split(separator: ";")
    guard parts.count == 3,
          let button = Int(parts[0]),
          let x = Int(parts[1]),
          let y = Int(parts[2])
    else
    {
        return nil
    }
    let point = Point(row: max(0, y - 1), col: max(0, x - 1))
    if button == 64
    {
        return .scroll(point: point, delta: 1)
    }
    if button == 65
    {
        return .scroll(point: point, delta: -1)
    }
    if final == "m"
    {
        return .mouse(kind: "release", point: point)
    }
    if button & 32 == 32
    {
        return .mouse(kind: "move", point: point)
    }
    return .mouse(kind: "press", point: point)
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

private struct RuntimeRgb
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

private let kTestFallbackCheckerRgb = RuntimeRgb(red: 255, green: 255, blue: 255)

private func loadImageRaster(_ path: String) -> RuntimeImageRaster?
{
    var decoded = UimdDecodedImage()
    guard uimd_decode_image_rgba(path, &decoded) != 0,
          decoded.width > 0,
          decoded.height > 0,
          let rgba = decoded.rgba
    else
    {
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
    return RuntimeImageRaster(width: width, height: height, pixels: pixels, alpha: alpha)
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
    let xOffset = stretch ? 0.0 : alignmentOffset(outer: Double(targetWidth), inner: drawnWidth, value: align, startValue: "left", endValue: "right")
    let yOffset = stretch ? 0.0 : alignmentOffset(outer: Double(targetHeight), inner: drawnHeight, value: verticalAlign, startValue: "top", endValue: "bottom")

    for y in 0..<targetHeight
    {
        for x in 0..<targetWidth
        {
            let sourceX = stretch ? (Double(x) * Double(source.width) / Double(targetWidth)) : ((Double(x) - xOffset) / scale)
            let sourceY = stretch ? (Double(y) * Double(source.height) / Double(targetHeight)) : ((Double(y) - yOffset) / scale)
            if sourceX < 0.0 || sourceY < 0.0 || sourceX >= Double(source.width) || sourceY >= Double(source.height)
            {
                continue
            }
            let sx = Int(floor(sourceX))
            let sy = Int(floor(sourceY))
            let index = y * targetWidth + x
            pixels[index] = blendWithBackground(rasterPixel(source, x: sx, y: sy), alpha: rasterAlpha(source, x: sx, y: sy), background: background)
            alpha[index] = 255
        }
    }
    return RuntimeImageRaster(width: targetWidth, height: targetHeight, pixels: pixels, alpha: alpha)
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
        return truthyEnvironment("UIMD_DISABLE_SIXEL") ? "fallback" : "sixel"
    }
    return truthyEnvironment("UIMD_DISABLE_SIXEL") ? "fallback" : "sixel"
}

private func truthyEnvironment(_ name: String) -> Bool
{
    let value = ProcessInfo.processInfo.environment[name]?.trimmingCharacters(in: .whitespacesAndNewlines).lowercased() ?? ""
    return ["1", "true", "yes", "on"].contains(value)
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

private func imageInfoSampleSignature(source: String) -> [String]
{
    let lower = source.lowercased()
    let color: String
    if lower.contains("camera")
    {
        color = "#c0c0c0"
    }
    else if lower.contains("coins")
    {
        color = "#404040"
    }
    else if lower.contains("astro") || lower.contains("astronaut")
    {
        color = "#804040"
    }
    else if lower.contains("coffee")
    {
        color = "#804000"
    }
    else
    {
        color = "#000000"
    }
    return Array(repeating: color, count: kImageInfoSampleGridSize * kImageInfoSampleGridSize)
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

private func scrollViewHostRect(scrollView: ScrollView, fallback: Rect) -> Rect
{
    if scrollView.frame.width > 0 && scrollView.frame.height > 0
    {
        return Rect(
            row: fallback.row + scrollView.frame.row,
            col: fallback.col + scrollView.frame.col,
            width: scrollView.frame.width,
            height: scrollView.frame.height
        )
    }
    return fallback
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

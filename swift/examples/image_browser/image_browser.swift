import Foundation
import Uimd

final class ImageRecord
{
    var name: String
    var source: String
    var selected: Bool

    init(_ name: String, _ source: String, selected: Bool = false)
    {
        self.name = name
        self.source = source
        self.selected = selected
    }
}

private enum ImageBrowserStyles
{
    static func setLayoutCellBackground(_ window: GeneratedWindowBase, _ background: String)
    {
        window.setGeneratedLayoutCellBackground(Color(background))
    }

    static func measuredLayoutHeight(_ window: GeneratedWindowBase) -> Int
    {
        let natural = window.generatedContentSize()
        let measured = window.generatedContentSizeForWidth(natural.width)
        return max(1, measured.height)
    }
}

private let kRenderModeSixel = "sixel"
private let kRenderModeFallback = "fallback"
private let kTileNormalBackground = "transparent"
private let kTileListBackground = "#172033"
private let kTileSelectedBackground = "#2563eb"

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

private func styleGap(_ style: Style) -> Int
{
    max(0, style.gap ?? 0)
}

private func blankRow(width: Int, style: Style) -> [TerminalCell]
{
    Array(repeating: TerminalCell(" ", foreground: style.color, background: style.background), count: max(1, width))
}

private final class ImageView: ImageViewUI
{
    init(source: String, captionText: String, renderMode: String = kRenderModeSixel)
    {
        super.init()
        photo.setSource(source)
        photo.setAlt(captionText)
        photo.setRenderMode(renderMode)
        caption.setText(captionText)
    }
}

private final class ImageButton: ImageButtonUI
{
    override init()
    {
        super.init()
        photo.focusable = false
    }

    func setSource(_ source: String)
    {
        photo.setSource(source)
    }

    func setAlt(_ alt: String)
    {
        photo.setAlt(alt)
    }

    func setRenderMode(_ mode: String)
    {
        photo.setRenderMode(mode)
    }

    func setSelected(_ selected: Bool)
    {
        ImageBrowserStyles.setLayoutCellBackground(self, selected ? kTileSelectedBackground : kTileNormalBackground)
    }
}

private final class GalleryMosaic: GalleryMosaicUI
{
    override init()
    {
        super.init()
        for image in images()
        {
            image.focusable = false
        }
        let records = ImageBrowserApp.imageLibrary()
        for index in 0..<min(records.count, images().count)
        {
            images()[index].setSource(records[index].source)
            images()[index].setAlt(records[index].name)
        }
    }

    func setRenderMode(_ mode: String)
    {
        for image in images()
        {
            image.setRenderMode(mode)
        }
    }

    func setSelected(_ selected: Bool)
    {
        ImageBrowserStyles.setLayoutCellBackground(self, selected ? kTileSelectedBackground : kTileListBackground)
    }

    private func images() -> [Image]
    {
        [gal_1, gal_2, gal_3, gal_4, gal_5]
    }
}

private final class ImageListButton: ImageListButtonUI
{
    override init()
    {
        super.init()
        for image in images()
        {
            image.focusable = false
        }
    }

    func setImages(_ records: [ImageRecord])
    {
        let previews = images()
        for index in 0..<min(previews.count, records.count)
        {
            previews[index].setSource(records[index].source)
            previews[index].setAlt(records[index].name)
        }
    }

    func setRenderMode(_ mode: String)
    {
        for image in images()
        {
            image.setRenderMode(mode)
        }
    }

    func setSelected(_ selected: Bool)
    {
        ImageBrowserStyles.setLayoutCellBackground(self, selected ? kTileSelectedBackground : kTileListBackground)
    }

    private func images() -> [Image]
    {
        [prw_1, prw_2, prw_3]
    }
}

private final class GalleryItem: GalleryItemUI
{
    init(source: String, captionText: String, renderMode: String)
    {
        super.init()
        photo.setSource(source)
        photo.setAlt(captionText)
        photo.setRenderMode(renderMode)
        caption.setText(captionText)
    }
}

private final class GalleryScroll: GalleryScrollUI
{
    private var rowViews: [GalleryItem] = []
    private var viewOffset = 0

    func populate(renderMode: String)
    {
        let records = Array(ImageBrowserApp.imageLibrary().prefix(5))
        rowViews = records.map { GalleryItem(source: $0.source, captionText: $0.name, renderMode: renderMode) }
        viewOffset = 0
    }

    override func scrollBy(_ delta: Int, viewport: Size)
    {
        let maxOffset = max(0, renderedContentHeight(width: max(1, viewport.width)) - max(1, viewport.height))
        viewOffset = max(0, min(maxOffset, viewOffset + delta))
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        let rows = renderedRows(width: width)
        let style = panelStyle()
        let blank = blankRow(width: width, style: style)
        var output = Array(repeating: blank, count: height)
        for row in 0..<height
        {
            let sourceRow = viewOffset + row
            if sourceRow >= 0 && sourceRow < rows.count
            {
                output[row] = rows[sourceRow]
            }
        }
        applyGeneratedScrollIndicators(
            content: &output,
            viewOffset: viewOffset,
            maxViewOffset: max(0, rows.count - height),
            viewport: Size(width: width, height: height),
            style: style
        )
        return output
    }

    private func renderedRows(width: Int) -> [[TerminalCell]]
    {
        var rows: [[TerminalCell]] = []
        let style = panelStyle()
        let gap = styleGap(style)
        let blank = blankRow(width: width, style: style)
        for index in rowViews.indices
        {
            let rowView = rowViews[index]
            let rowHeight = rowHeight(rowView, width: width)
            let rendered = rowView.renderContent(size: Size(width: width, height: rowHeight), focusedName: nil, editMode: false)
            rows.append(contentsOf: rendered.prefix(rowHeight))
            if index < rowViews.count - 1
            {
                for _ in 0..<gap
                {
                    rows.append(blank)
                }
            }
        }
        return rows
    }

    private func renderedContentHeight(width: Int) -> Int
    {
        let rowsHeight = rowViews.reduce(0) { $0 + rowHeight($1, width: max(1, width)) }
        return rowsHeight + max(0, rowViews.count - 1) * styleGap(panelStyle())
    }

    private func rowHeight(_ rowView: GalleryItem, width: Int) -> Int
    {
        max(1, rowView.generatedContentSizeForWidth(max(1, width)).height)
    }

    private func panelStyle() -> Style
    {
        scrollView().style
    }
}

private final class ImageShowDialog: ImageShowDialogUI
{
    var onClose: (() -> Void)?
    private var closed = false

    init(source: String, captionText: String, renderMode: String)
    {
        super.init()
        title.setText(captionText)
        photo.setSource(source)
        photo.setAlt(captionText)
        photo.setRenderMode(renderMode)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "close_btn"
        options.onKey = { [weak self] key in
            guard key == "Escape" else
            {
                return false
            }
            self?.close()
            return true
        }
        return options
    }

    override func onCloseBtnClick()
    {
        close()
    }

    override func shouldClose() -> Bool
    {
        closed
    }

    private func close()
    {
        closed = true
        onClose?()
    }
}

private final class ImageListItem: ImageListItemUI
{
    let record: ImageRecord

    init(_ record: ImageRecord, renderMode: String)
    {
        self.record = record
        super.init()
        mode = "expand_width"
        thumb.setSource(record.source)
        thumb.setAlt(record.name)
        thumb.setFit("contain")
        thumb.setRenderMode(renderMode)
        name_label.setText(record.name)
        path_label.setText(ImageBrowserApp.imageDisplayPath(record.source))
        sel.setChecked(record.selected)
    }
}

private final class ImageListScroll: ImageListScrollUI, GeneratedScrollableElementVisibility
{
    private var rowViews: [ImageListItem] = []
    private var viewOffset = 0
    private var scrollToEndPending = false
    var activeFocusedId: String?
    var activeEditMode = false
    var activeChildEditMode = false
    var activeHostFocused = false
    var suppressFocusVisuals = false
    var ensureFocusedRowOnNextRender = false

    func setItems(_ records: [ImageRecord], renderMode: String, scrollToEnd: Bool = false, preserveScroll: Bool = false)
    {
        let previousOffset = viewOffset
        rowViews = records.map { ImageListItem($0, renderMode: renderMode) }
        if scrollToEnd
        {
            scrollToEndPending = true
        }
        else if preserveScroll
        {
            viewOffset = min(previousOffset, max(0, renderedContentHeight(width: generatedContentSize().width) - 1))
        }
        else
        {
            viewOffset = 0
        }
    }

    func focusOrder(hostId: String) -> [String]
    {
        rowViews.indices.flatMap
        {
            [
                "\(hostId)[\($0)].sel",
                "\(hostId)[\($0)].show_btn",
                "\(hostId)[\($0)].browse_btn",
                "\(hostId)[\($0)].delete_btn",
            ]
        }
    }

    override func scrollBy(_ delta: Int, viewport: Size)
    {
        let maxOffset = max(0, renderedContentHeight(width: max(1, viewport.width)) - max(1, viewport.height))
        viewOffset = max(0, min(maxOffset, viewOffset + delta))
    }

    override func handleScrollKey(_ key: String, viewport: Size) -> Bool
    {
        switch key
        {
        case "ArrowUp", "Up":
            return scrollLines(-generatedScrollKeyboardStepRows, viewport: viewport)
        case "ArrowDown", "Down":
            return scrollLines(generatedScrollKeyboardStepRows, viewport: viewport)
        case "PageUp":
            return scrollLines(-max(1, viewport.height), viewport: viewport)
        case "PageDown":
            return scrollLines(max(1, viewport.height), viewport: viewport)
        case "Home":
            let changed = viewOffset != 0
            viewOffset = 0
            return changed
        case "End":
            let previous = viewOffset
            viewOffset = maxLineOffset(viewport: viewport)
            return viewOffset != previous
        default:
            return false
        }
    }

    func ensureElementVisible(_ elementId: String, viewport: Size)
    {
        guard let index = rowIndex(elementId),
              index >= 0,
              index < rowViews.count
        else
        {
            return
        }
        let style = panelStyle()
        let width = max(1, viewport.width)
        let height = max(1, viewport.height)
        let rowWidth = max(1, width - stylePaddingLeft(style) - stylePaddingRight(style))
        let rowView = rowViews[index]
        let rowHeight = rowHeight(rowView, width: rowWidth)
        _ = rowView.renderContent(size: Size(width: rowWidth, height: rowHeight), focusedName: nil, editMode: false)
        let leafName = String(elementId.split(separator: ".").last ?? "")
        guard let target = rowView.element(named: leafName) else
        {
            return
        }
        let targetTop = rowStart(index, rowWidth: rowWidth) + target.frame.row
        let targetBottom = targetTop + max(1, target.frame.height)
        let visibleTop = viewOffset + stylePaddingTop(style)
        let visibleBottom = viewOffset + height - stylePaddingBottom(style)
        if targetTop < visibleTop
        {
            scrollBy(targetTop - visibleTop, viewport: viewport)
        }
        else if targetBottom > visibleBottom
        {
            scrollBy(targetBottom - visibleBottom, viewport: viewport)
        }
    }

    func registerDynamicElements(on root: GeneratedWindowBase, hostId: String, hostFrame: Rect)
    {
        let style = panelStyle()
        let paddingTop = stylePaddingTop(style)
        let paddingLeft = stylePaddingLeft(style)
        let paddingRight = stylePaddingRight(style)
        let gap = styleGap(style)
        let rowWidth = max(1, hostFrame.width - paddingLeft - paddingRight)
        var rowCursor = paddingTop
        for index in rowViews.indices
        {
            let row = rowViews[index]
            let rowHeight = rowHeight(row, width: rowWidth)
            _ = row.renderContent(size: Size(width: rowWidth, height: rowHeight), focusedName: nil, editMode: false)
            let prefix = "\(hostId)[\(index)]"
            for element in row.elements
            {
                element.frame = Rect(
                    row: hostFrame.row + rowCursor + element.frame.row - viewOffset,
                    col: hostFrame.col + paddingLeft + element.frame.col,
                    width: element.frame.width,
                    height: element.frame.height
                )
                root.registerDynamicElement(prefix + "." + element.name, element: element, parentFocusHostId: hostId)
            }
            rowCursor += rowHeight
            if index < rowViews.count - 1
            {
                rowCursor += gap
            }
        }
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        if scrollToEndPending
        {
            viewOffset = max(0, renderedContentHeight(width: width) - height)
            scrollToEndPending = false
        }
        if activeEditMode && ensureFocusedRowOnNextRender
        {
            ensureFocusedRowVisible(viewport: Size(width: width, height: height))
            ensureFocusedRowOnNextRender = false
        }
        let style = panelStyle()
        let blank = blankRow(width: width, style: style)
        var output = Array(repeating: blank, count: height)
        let rows = renderedRows(width: width)
        for row in 0..<height
        {
            let sourceRow = viewOffset + row
            if sourceRow >= 0 && sourceRow < rows.count
            {
                output[row] = rows[sourceRow]
            }
        }
        if !activeEditMode && !activeHostFocused && viewOffset > 0
        {
            output[0] = blank
        }
        if !suppressFocusVisuals && (activeEditMode || activeHostFocused), let focusBackground = generatedWindowFocusStyle?.background
        {
            let focusedLeaf = activeFocusedId.map { String($0.split(separator: ".").last ?? "") }
            let rowWidth = max(1, width - stylePaddingLeft(style) - stylePaddingRight(style))
            if shouldKeepTopRowAsScrollBackground(output[0], rowWidth: rowWidth)
            {
                output[0] = blank
                applyScrollFocusBackground(content: &output, row: 0, focusBackground: focusBackground)
            }
            else if !activeEditMode && viewOffset > 0 && focusedLeaf != "sel"
            {
                output[0][0].background = focusBackground.blended(over: output[0][0].background) ?? focusBackground
            }
            else
            {
                applyScrollFocusBackground(content: &output, row: 0, focusBackground: focusBackground)
            }
            for row in output.indices.dropFirst()
            {
                if isFocusBandSourceRow(viewOffset + row, rowWidth: rowWidth)
                {
                    applyScrollFocusBackground(content: &output, row: row, focusBackground: focusBackground)
                }
                else if rowHasNoRenderedContent(output[row])
                {
                    applyScrollFocusBackground(content: &output, row: row, focusBackground: focusBackground)
                }
                else
                {
                    output[row][0].background = focusBackground.blended(over: output[row][0].background) ?? focusBackground
                }
            }
        }
        applyGeneratedScrollIndicators(
            content: &output,
            viewOffset: viewOffset,
            maxViewOffset: max(0, rows.count - height),
            viewport: Size(width: width, height: height),
            style: style
        )
        return output
    }

    private func ensureFocusedRowVisible(viewport: Size)
    {
        guard let activeFocusedId,
              let index = rowIndex(activeFocusedId),
              index >= 0,
              index < rowViews.count
        else
        {
            return
        }
        let style = panelStyle()
        let rowWidth = max(1, max(1, viewport.width) - stylePaddingLeft(style) - stylePaddingRight(style))
        let start = rowStart(index, rowWidth: rowWidth)
        let end = start + rowHeight(rowViews[index], width: rowWidth)
        let viewportHeight = max(1, viewport.height)
        if start < viewOffset
        {
            viewOffset = start
        }
        else if end > viewOffset + max(1, viewportHeight)
        {
            viewOffset = max(0, end - max(1, viewportHeight))
        }
        viewOffset = max(0, min(maxLineOffset(viewport: viewport), viewOffset))
    }

    private func scrollLines(_ delta: Int, viewport: Size) -> Bool
    {
        let previous = viewOffset
        scrollBy(delta, viewport: viewport)
        return viewOffset != previous
    }

    private func maxLineOffset(viewport: Size) -> Int
    {
        max(0, renderedContentHeight(width: max(1, viewport.width)) - max(1, viewport.height))
    }

    private func renderedRows(width: Int) -> [[TerminalCell]]
    {
        var rows: [[TerminalCell]] = []
        let style = panelStyle()
        let blank = blankRow(width: width, style: style)
        let paddingLeft = stylePaddingLeft(style)
        let paddingRight = stylePaddingRight(style)
        let rowWidth = max(1, width - paddingLeft - paddingRight)
        for _ in 0..<stylePaddingTop(style)
        {
            rows.append(blank)
        }
        for index in rowViews.indices
        {
            let rowView = rowViews[index]
            let localFocus: String?
            let effectiveFocusedId: String?
            if let activeFocusedId,
               activeFocusedId.hasPrefix("main.items[") || activeFocusedId.hasPrefix("items[")
            {
                effectiveFocusedId = activeFocusedId
            }
            else if activeEditMode && (activeFocusedId == "main.items" || activeFocusedId == "items")
            {
                effectiveFocusedId = "items[0].sel"
            }
            else
            {
                effectiveFocusedId = activeFocusedId
            }
            if let effectiveFocusedId,
               effectiveFocusedId.hasPrefix("main.items[\(index)].") ||
                effectiveFocusedId.hasPrefix("items[\(index)].")
            {
                localFocus = String(effectiveFocusedId.split(separator: ".").last ?? "")
            }
            else
            {
                localFocus = nil
            }
            let rowHeight = rowHeight(rowView, width: rowWidth)
            let rendered = rowView.renderContent(
                size: Size(width: rowWidth, height: rowHeight),
                focusedName: localFocus,
                editMode: (activeChildEditMode || activeEditMode) && localFocus != nil
            )
            for localRow in 0..<min(rowHeight, rendered.count)
            {
                var outputRow = blank
                let renderedRow = rendered[localRow]
                for col in 0..<min(rowWidth, renderedRow.count)
                {
                    let targetCol = paddingLeft + col
                    if targetCol >= 0 && targetCol < outputRow.count
                    {
                        outputRow[targetCol] = renderedRow[col]
                    }
                }
                rows.append(outputRow)
            }
            if index < rowViews.count - 1
            {
                for _ in 0..<styleGap(style)
                {
                    rows.append(blank)
                }
            }
        }
        for _ in 0..<stylePaddingBottom(style)
        {
            rows.append(blank)
        }
        return rows
    }

    private func renderedContentHeight(width: Int) -> Int
    {
        let style = panelStyle()
        let rowWidth = max(1, max(1, width) - stylePaddingLeft(style) - stylePaddingRight(style))
        let rowsHeight = rowViews.reduce(0) { $0 + rowHeight($1, width: rowWidth) }
        return stylePaddingTop(style) + rowsHeight + max(0, rowViews.count - 1) * styleGap(style) + stylePaddingBottom(style)
    }

    private func rowStart(_ index: Int, rowWidth: Int) -> Int
    {
        let style = panelStyle()
        var row = stylePaddingTop(style)
        for current in 0..<index
        {
            row += rowHeight(rowViews[current], width: rowWidth) + styleGap(style)
        }
        return row
    }

    private func rowIndexStarting(at sourceRow: Int, rowWidth: Int) -> Int?
    {
        for index in rowViews.indices where rowStart(index, rowWidth: rowWidth) == sourceRow
        {
            return index
        }
        return nil
    }

    private func shouldKeepTopRowAsScrollBackground(_ row: [TerminalCell], rowWidth: Int) -> Bool
    {
        guard viewOffset > 0
        else
        {
            return false
        }
        let hasText = rowHasNonImageText(row)
        let hasImageBlock = row.contains { $0.text == "▀" }
        if hasText && !hasImageBlock
        {
            return true
        }
        guard let firstVisibleIndex = rowIndexStarting(at: viewOffset, rowWidth: rowWidth),
              let activeFocusedId
        else
        {
            return false
        }
        return !activeFocusedId.hasPrefix("main.items[\(firstVisibleIndex)].")
    }

    private func rowHasNonImageText(_ row: [TerminalCell]) -> Bool
    {
        row.contains
        {
            let text = $0.text.trimmingCharacters(in: .whitespaces)
            return !text.isEmpty && text != "▀" && text != "█"
        }
    }

    private func rowHasNoRenderedContent(_ row: [TerminalCell]) -> Bool
    {
        !row.contains
        {
            !$0.text.trimmingCharacters(in: .whitespaces).isEmpty || $0.text == "▀"
        }
    }

    private func isFocusBandSourceRow(_ sourceRow: Int, rowWidth: Int) -> Bool
    {
        let style = panelStyle()
        let paddingTop = stylePaddingTop(style)
        if sourceRow < paddingTop
        {
            return true
        }
        var cursor = paddingTop
        let gap = styleGap(style)
        for index in rowViews.indices
        {
            cursor += rowHeight(rowViews[index], width: rowWidth)
            if index < rowViews.count - 1
            {
                if sourceRow >= cursor && sourceRow < cursor + gap
                {
                    return true
                }
                cursor += gap
            }
        }
        return sourceRow >= cursor
    }

    private func applyScrollFocusBackground(content: inout [[TerminalCell]], row: Int, focusBackground: Color)
    {
        let style = panelStyle()
        applyFocusBackgroundToBaseCells(
            content: &content,
            row: row,
            focusBackground: focusBackground,
            baseBackgrounds: [style.background, generatedWindowStyle.background]
        )
    }

    private func rowHeight(_ row: ImageListItem, width: Int) -> Int
    {
        max(1, row.generatedContentSizeForWidth(max(1, width)).height)
    }

    private func panelStyle() -> Style
    {
        scrollView().style
    }
}

private final class ImageListView: ImageListViewUI
{
    let scroll = ImageListScroll()
    var activeFocusedId: String?
    var activeEditMode = false
    var activeChildEditMode = false

    override init()
    {
        super.init()
        items.setChild(scroll)
        scroll.suppressGeneratedFocusVisuals = true
    }

    func refresh(records: [ImageRecord], renderMode: String, scrollToEnd: Bool = false, preserveScroll: Bool = false)
    {
        scroll.setItems(records, renderMode: renderMode, scrollToEnd: scrollToEnd, preserveScroll: preserveScroll)
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        let effectiveActiveEditMode = activeEditMode || editMode
        let localItemsFocused = focusedName == "items" ||
            focusedName?.hasPrefix("items[") == true ||
            focusedName?.hasPrefix("items.") == true
        let activeDescendantFocused = activeFocusedId?.hasPrefix("main.items[") == true ||
            activeFocusedId?.hasPrefix("items[") == true
        let localDescendantFocused = focusedName?.hasPrefix("items[") == true
        let effectiveFocusedId: String?
        if activeDescendantFocused
        {
            effectiveFocusedId = activeFocusedId
        }
        else if localDescendantFocused
        {
            effectiveFocusedId = focusedName
        }
        else if localItemsFocused
        {
            effectiveFocusedId = focusedName
        }
        else
        {
            effectiveFocusedId = activeFocusedId
        }
        scroll.activeFocusedId = effectiveFocusedId
        scroll.activeEditMode = effectiveActiveEditMode
        scroll.activeChildEditMode = activeChildEditMode
        let descendantFocused = effectiveFocusedId?.hasPrefix("main.items[") == true ||
            effectiveFocusedId?.hasPrefix("items[") == true
        scroll.activeHostFocused = (focusedName == "items" && !descendantFocused) ||
            effectiveFocusedId == "main.items" ||
            effectiveFocusedId == "items"
        scroll.suppressFocusVisuals = suppressGeneratedFocusVisuals
        let itemsFocused = focusedName == "items"
            || focusedName?.hasPrefix("items[") == true
            || effectiveFocusedId == "main.items"
            || effectiveFocusedId == "items"
            || effectiveFocusedId?.hasPrefix("main.items[") == true
            || effectiveFocusedId?.hasPrefix("items[") == true
        return super.renderContent(size: size, focusedName: itemsFocused ? nil : focusedName, editMode: editMode)
    }
}

private final class GalleryView: GalleryViewUI
{
    let scroll = GalleryScroll()

    init(renderMode: String)
    {
        super.init()
        gallery_scroll.setChild(scroll)
        scroll.populate(renderMode: renderMode)
    }

    func setRenderMode(_ renderMode: String)
    {
        scroll.populate(renderMode: renderMode)
    }
}

final class ImageBrowserApp: ImageBrowserUI
{
    private let modalStack = GeneratedWindowStack()
    private let images = ImageBrowserApp.imageLibrary()
    private var imageRecords: [ImageRecord]
    private var thumbs: [ImageButton] = []
    private let galleryMosaicControl = GalleryMosaic()
    private let imageListButtonControl = ImageListButton()
    private var renderMode = kRenderModeSixel
    private var finished = false
    private weak var pendingBrowseRecord: ImageRecord?
    private weak var pendingDeleteRecord: ImageRecord?

    override init()
    {
        imageRecords = Array(ImageBrowserApp.imageLibrary().prefix(20)).map { ImageRecord($0.name, $0.source, selected: $0.selected) }
        super.init()
        gallery_mosaic.setChild(galleryMosaicControl)
        image_list_btn.setChild(imageListButtonControl)
        imageListButtonControl.setImages(images)
        let slots = [camera_thumb, astro_thumb, coffee_thumb]
        for index in 0..<min(slots.count, images.count)
        {
            let button = ImageButton()
            slots[index]?.setChild(button)
            button.setSource(images[index].source)
            button.setAlt(images[index].name)
            thumbs.append(button)
        }
        applyRenderMode(kRenderModeSixel)
        showImage(index: 0)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.initialFocusName = "camera_thumb"
        options.windowStack = modalStack
        options.onKeyBeforeFocusedElement = { [weak self] key, focusedName, editMode in
            guard editMode,
                  (key == "Down" || key == "Up"),
                  focusedName.hasPrefix("main.items["),
                  let listView = self?.main.child as? ImageListView
            else
            {
                return false
            }
            listView.scroll.ensureFocusedRowOnNextRender = true
            return false
        }
        return options
    }

    override func shouldClose() -> Bool
    {
        finished
    }

    override func onCloseBtnClick()
    {
        finished = true
    }

    override func onModeSelectChange(_ value: String)
    {
        applyRenderMode(value == "Normal" ? kRenderModeSixel : kRenderModeFallback)
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        clearDynamicElements()
        if let listView = main.child as? ImageListView
        {
            let listScopeFocused = focusedName == "main.items" ||
                focusedName == "items" ||
                focusedName?.hasPrefix("main.items[") == true ||
                focusedName?.hasPrefix("main.items.") == true ||
                focusedName?.hasPrefix("items[") == true ||
                focusedName?.hasPrefix("items.") == true
            listView.activeFocusedId = focusedName
            listView.activeEditMode = generatedFocusedNameIsInActiveEditScope(focusedName, editMode: editMode) ||
                (editMode && listScopeFocused)
            listView.activeChildEditMode = generatedFocusedNameIsActivelyEdited(focusedName, editMode: editMode)
            listView.scroll.suppressFocusVisuals = suppressGeneratedFocusVisuals
        }
        let rendered = super.renderContent(size: size, focusedName: focusedName, editMode: editMode)
        registerMainChild()
        return rendered
    }

    override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "camera_thumb"
        {
            showImage(index: 0)
            return true
        }
        if name == "astro_thumb"
        {
            showImage(index: 1)
            return true
        }
        if name == "coffee_thumb"
        {
            showImage(index: 2)
            return true
        }
        if name == "image_list_btn"
        {
            showImageList()
            return true
        }
        if name == "gallery_mosaic"
        {
            showGallery()
            return true
        }
        if name == "main.add_btn"
        {
            openBrowser(record: nil)
            return true
        }
        if name.hasPrefix("main.items["), let index = rowIndex(name), index >= 0, index < imageRecords.count
        {
            let record = imageRecords[index]
            if name.hasSuffix(".show_btn")
            {
                showDialog(record)
                return true
            }
            if name.hasSuffix(".browse_btn")
            {
                openBrowser(record: record)
                return true
            }
            if name.hasSuffix(".delete_btn")
            {
                confirmDelete(record)
                return true
            }
        }
        return super.handleGeneratedButton(name)
    }

    override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name.hasPrefix("main.items["), name.hasSuffix(".sel"), let index = rowIndex(name), index >= 0, index < imageRecords.count
        {
            imageRecords[index].selected = value == "true"
            return true
        }
        return super.handleGeneratedTextChanged(name, value: value)
    }

    private func showImage(index: Int)
    {
        guard index >= 0 && index < images.count else
        {
            return
        }
        selectThumb(index)
        galleryMosaicControl.setSelected(false)
        imageListButtonControl.setSelected(false)
        main.setChild(ImageView(source: images[index].source, captionText: images[index].name, renderMode: renderMode))
    }

    private func showImageList()
    {
        deselectAllThumbs()
        galleryMosaicControl.setSelected(false)
        imageListButtonControl.setSelected(true)
        let view = ImageListView()
        view.refresh(records: imageRecords, renderMode: renderMode)
        main.setChild(view)
    }

    private func showGallery()
    {
        deselectAllThumbs()
        galleryMosaicControl.setSelected(true)
        imageListButtonControl.setSelected(false)
        main.setChild(GalleryView(renderMode: renderMode))
    }

    private func applyRenderMode(_ mode: String)
    {
        renderMode = mode
        for thumb in thumbs
        {
            thumb.setRenderMode(renderMode)
        }
        galleryMosaicControl.setRenderMode(renderMode)
        imageListButtonControl.setRenderMode(renderMode)
        if let imageView = main.child as? ImageView
        {
            imageView.photo.setRenderMode(renderMode)
        }
        else if let galleryView = main.child as? GalleryView
        {
            galleryView.setRenderMode(renderMode)
        }
        else if let listView = main.child as? ImageListView
        {
            listView.refresh(records: imageRecords, renderMode: renderMode, preserveScroll: true)
        }
    }

    private func selectThumb(_ selectedIndex: Int)
    {
        for index in thumbs.indices
        {
            thumbs[index].setSelected(index == selectedIndex)
        }
    }

    private func deselectAllThumbs()
    {
        for thumb in thumbs
        {
            thumb.setSelected(false)
        }
    }

    private func showDialog(_ record: ImageRecord)
    {
        let dialog = ImageShowDialog(source: record.source, captionText: record.name, renderMode: renderMode)
        dialog.onClose = { [weak self] in
            self?.modalStack.popTop()
        }
        modalStack.push(dialog)
    }

    private func openBrowser(record: ImageRecord?)
    {
        pendingBrowseRecord = record
        let startPath = record?.source ?? ImageBrowserApp.imageSampleDir()
        let browser = FileBrowser(root: ImageBrowserApp.projectRoot(), start: startPath, mode: "open", extensionFilter: ImageBrowserApp.imageExtensionFilter)
        browser.onClose = { [weak self] path in
            guard let self else
            {
                return
            }
            self.modalStack.popTop()
            let selectedPath = path.trimmingCharacters(in: .whitespacesAndNewlines)
            guard !selectedPath.isEmpty else
            {
                self.pendingBrowseRecord = nil
                return
            }
            if let pending = self.pendingBrowseRecord
            {
                pending.name = ImageBrowserApp.imageName(fromPath: selectedPath)
                pending.source = selectedPath
                self.refreshImageList(preserveScroll: true)
            }
            else
            {
                self.imageRecords.append(ImageRecord(ImageBrowserApp.imageName(fromPath: selectedPath), selectedPath))
                self.refreshImageList(scrollToEnd: true)
            }
            self.pendingBrowseRecord = nil
        }
        modalStack.push(browser)
    }

    private func confirmDelete(_ record: ImageRecord)
    {
        pendingDeleteRecord = record
        let dialog = MessageBoxYesNo("Delete Image", "Delete " + record.name + "?")
        dialog.onClose = { [weak self] confirmed in
            guard let self else
            {
                return
            }
            self.modalStack.popTop()
            guard confirmed, let pending = self.pendingDeleteRecord else
            {
                self.pendingDeleteRecord = nil
                return
            }
            self.imageRecords.removeAll { $0 === pending }
            self.pendingDeleteRecord = nil
            self.refreshImageList(preserveScroll: true)
        }
        modalStack.push(dialog)
    }

    private func refreshImageList(scrollToEnd: Bool = false, preserveScroll: Bool = false)
    {
        if let listView = main.child as? ImageListView
        {
            listView.refresh(records: imageRecords, renderMode: renderMode, scrollToEnd: scrollToEnd, preserveScroll: preserveScroll)
        }
    }

    private func registerMainChild()
    {
        guard let child = main.child else
        {
            return
        }
        for element in child.elements
        {
            element.frame = Rect(row: main.frame.row + element.frame.row, col: main.frame.col + element.frame.col, width: element.frame.width, height: element.frame.height)
            registerDynamicElement("main." + element.name, element: element, parentFocusHostId: "main")
        }
        main.childFocusOrder = child.elements.filter { $0.focusable }.map { "main." + $0.name }
        if let listView = child as? ImageListView
        {
            let host = listView.items
            host?.childFocusOrder = listView.scroll.focusOrder(hostId: "main.items")
            if let host
            {
                listView.scroll.registerDynamicElements(on: self, hostId: "main.items", hostFrame: host.frame)
            }
        }
    }

    static func imageLibrary() -> [ImageRecord]
    {
        var result = [
            ImageRecord("Camera", "shared/assets/image_samples/camera.png"),
            ImageRecord("Astronaut", "shared/assets/image_samples/astronaut.png"),
            ImageRecord("Coffee", "shared/assets/image_samples/coffee.png"),
            ImageRecord("Coins", "shared/assets/image_samples/coins.png"),
            ImageRecord("Chelsea", "shared/assets/image_samples/chelsea.png"),
        ]
        for index in 6...50
        {
            result.append(ImageRecord("Sample \(index)", "shared/assets/image_samples/sample_\(index)" + sampleExtension(index)))
        }
        return result
    }

    static let imageExtensionFilter = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)"

    static func projectRoot() -> String
    {
        FileManager.default.currentDirectoryPath
    }

    static func imageSampleDir() -> String
    {
        URL(fileURLWithPath: projectRoot()).appendingPathComponent("shared/assets/image_samples").path
    }

    static func imageDisplayPath(_ path: String) -> String
    {
        guard !path.isEmpty else
        {
            return ""
        }
        let absolutePath = URL(fileURLWithPath: path).standardizedFileURL.path
        let root = URL(fileURLWithPath: projectRoot()).standardizedFileURL.path
        let rootPrefix = root.hasSuffix("/") ? root : root + "/"
        if absolutePath.hasPrefix(rootPrefix)
        {
            return String(absolutePath.dropFirst(rootPrefix.count))
        }
        return path
    }

    static func imageName(fromPath path: String) -> String
    {
        let stem = URL(fileURLWithPath: path).deletingPathExtension().lastPathComponent
        guard !stem.isEmpty else
        {
            return path
        }
        let words = stem.replacingOccurrences(of: "_", with: " ")
            .replacingOccurrences(of: "-", with: " ")
            .split(separator: " ")
            .map
            {
                word in
                word.prefix(1).uppercased() + word.dropFirst()
            }
        return words.isEmpty ? stem : words.joined(separator: " ")
    }

    private static func sampleExtension(_ index: Int) -> String
    {
        switch index % 6
        {
        case 0:
            return ".bmp"
        case 1:
            return ".gif"
        case 2:
            return ".png"
        case 3:
            return ".tga"
        default:
            return ".jpg"
        }
    }
}

private func rowIndex(_ id: String) -> Int?
{
    guard let open = id.firstIndex(of: "["),
          let close = id[open...].firstIndex(of: "]")
    else
    {
        return nil
    }
    return Int(id[id.index(after: open)..<close])
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = ImageBrowserApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

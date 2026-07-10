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

private final class ImageRecordStore
{
    var records: [ImageRecord]

    init(_ records: [ImageRecord])
    {
        self.records = records
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
    func populate(renderMode: String)
    {
        let records = Array(ImageBrowserApp.imageLibrary().prefix(5))
        clearChildren()
        for record in records
        {
            let item = GalleryItem(source: record.source, captionText: record.name, renderMode: renderMode)
            let naturalSize = item.generatedContentSize()
            let measuredSize = item.generatedContentSizeForWidth(max(1, naturalSize.width))
            let reusable = ReusableElement("gallery_item")
            reusable.setChild(item)
            reusable.frame = Rect(row: 0, col: 0, width: 0, height: max(1, measuredSize.height))
            _ = scrollView().addChild(reusable)
        }
        scrollToTop()
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
    typealias RecordCallback = (ImageRecord) -> Void

    let record: ImageRecord
    private let onShow: RecordCallback
    private let onBrowse: RecordCallback
    private let onDelete: RecordCallback
    private let onChange: RecordCallback

    init(
        _ record: ImageRecord,
        renderMode: String,
        onShow: @escaping RecordCallback,
        onBrowse: @escaping RecordCallback,
        onDelete: @escaping RecordCallback,
        onChange: @escaping RecordCallback
    )
    {
        self.record = record
        self.onShow = onShow
        self.onBrowse = onBrowse
        self.onDelete = onDelete
        self.onChange = onChange
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

    override func onSelChange(_ value: String)
    {
        record.selected = sel.checked
        onChange(record)
    }

    override func onShowBtnClick()
    {
        onShow(record)
    }

    override func onBrowseBtnClick()
    {
        onBrowse(record)
    }

    override func onDeleteBtnClick()
    {
        onDelete(record)
    }
}

private final class ImageListScroll: ImageListScrollUI
{
    typealias RecordCallback = ImageListItem.RecordCallback

    func setItems(
        _ records: [ImageRecord],
        renderMode: String,
        onShow: @escaping RecordCallback,
        onBrowse: @escaping RecordCallback,
        onDelete: @escaping RecordCallback,
        onChange: @escaping RecordCallback,
        scrollToEnd: Bool = false,
        preserveScroll: Bool = false
    )
    {
        let position = scrollView().scrollPosition()
        clearChildren()
        setAutoScroll(scrollToEnd)
        for record in records
        {
            let item = ImageListItem(
                record,
                renderMode: renderMode,
                onShow: onShow,
                onBrowse: onBrowse,
                onDelete: onDelete,
                onChange: onChange
            )
            let naturalSize = item.generatedContentSize()
            let measuredSize = item.generatedContentSizeForWidth(max(1, naturalSize.width))
            let reusable = ReusableElement("image_list_item")
            reusable.setChild(item)
            reusable.frame = Rect(row: 0, col: 0, width: 0, height: max(1, measuredSize.height))
            _ = scrollView().addChild(reusable)
        }
        if scrollToEnd
        {
            scrollToBottom()
        }
        else if preserveScroll
        {
            scrollView().restoreScrollPosition(position)
        }
        else
        {
            scrollToTop()
        }
    }
}

private final class ImageListView: ImageListViewUI
{
    let scroll = ImageListScroll()
    private let records: ImageRecordStore
    private let modalStack: GeneratedWindowStack
    private var renderMode: String
    private var browser: FileBrowser?
    private var deleteDialog: MessageBoxYesNo?
    private var showDialog: ImageShowDialog?
    private weak var pendingBrowseRecord: ImageRecord?
    private weak var pendingDeleteRecord: ImageRecord?

    init(records: ImageRecordStore, renderMode: String, modalStack: GeneratedWindowStack)
    {
        self.records = records
        self.renderMode = renderMode
        self.modalStack = modalStack
        super.init()
        items.setChild(scroll)
        refreshItems()
    }

    func setRenderMode(_ renderMode: String)
    {
        self.renderMode = renderMode
        refreshItems(preserveScroll: true)
    }

    override func onAddBtnClick()
    {
        openBrowser(record: nil)
    }

    private func refreshItems(scrollToEnd: Bool = false, preserveScroll: Bool = false)
    {
        scroll.setItems(
            records.records,
            renderMode: renderMode,
            onShow: { [weak self] record in self?.showImage(record) },
            onBrowse: { [weak self] record in self?.openBrowser(record: record) },
            onDelete: { [weak self] record in self?.confirmDelete(record) },
            onChange: { _ in },
            scrollToEnd: scrollToEnd,
            preserveScroll: preserveScroll
        )
    }

    private func showImage(_ record: ImageRecord)
    {
        closeShowDialog()
        let dialog = ImageShowDialog(source: record.source, captionText: record.name, renderMode: renderMode)
        showDialog = dialog
        dialog.onClose = { [weak self] in
            self?.closeShowDialog()
        }
        modalStack.push(dialog)
    }

    private func openBrowser(record: ImageRecord?)
    {
        closeBrowser()
        pendingBrowseRecord = record
        let startPath = record?.source ?? ImageBrowserApp.imageSampleDir()
        let browser = FileBrowser(
            root: ImageBrowserApp.projectRoot(),
            start: startPath,
            mode: "open",
            extensionFilter: ImageBrowserApp.imageExtensionFilter
        )
        self.browser = browser
        browser.onClose = { [weak self] path in
            self?.onBrowserClosed(path)
        }
        modalStack.push(browser)
    }

    private func onBrowserClosed(_ path: String)
    {
        let selectedPath = path.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !selectedPath.isEmpty else
        {
            closeBrowser()
            return
        }
        if let pending = pendingBrowseRecord
        {
            pending.name = ImageBrowserApp.imageName(fromPath: selectedPath)
            pending.source = selectedPath
            closeBrowser()
            refreshItems(preserveScroll: true)
            return
        }
        records.records.append(ImageRecord(ImageBrowserApp.imageName(fromPath: selectedPath), selectedPath))
        closeBrowser()
        refreshItems(scrollToEnd: true)
    }

    private func confirmDelete(_ record: ImageRecord)
    {
        closeDeleteDialog()
        pendingDeleteRecord = record
        let dialog = MessageBoxYesNo("Delete Image", "Delete " + record.name + "?")
        deleteDialog = dialog
        dialog.onClose = { [weak self] confirmed in
            self?.deleteConfirmed(confirmed)
        }
        modalStack.push(dialog)
    }

    private func deleteConfirmed(_ confirmed: Bool)
    {
        let record = pendingDeleteRecord
        closeDeleteDialog()
        guard confirmed, let record else
        {
            return
        }
        records.records.removeAll { $0 === record }
        refreshItems(preserveScroll: true)
    }

    private func closeBrowser()
    {
        guard let browser else
        {
            pendingBrowseRecord = nil
            return
        }
        modalStack.remove(browser)
        self.browser = nil
        pendingBrowseRecord = nil
    }

    private func closeDeleteDialog()
    {
        guard let deleteDialog else
        {
            pendingDeleteRecord = nil
            return
        }
        modalStack.remove(deleteDialog)
        self.deleteDialog = nil
        pendingDeleteRecord = nil
    }

    private func closeShowDialog()
    {
        guard let showDialog else
        {
            return
        }
        modalStack.remove(showDialog)
        self.showDialog = nil
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
    private let imageRecords: ImageRecordStore
    private var thumbs: [ImageButton] = []
    private let galleryMosaicControl = GalleryMosaic()
    private let imageListButtonControl = ImageListButton()
    private var renderMode = kRenderModeSixel
    private var finished = false

    override init()
    {
        imageRecords = ImageRecordStore(
            Array(ImageBrowserApp.imageLibrary().prefix(20)).map { ImageRecord($0.name, $0.source, selected: $0.selected) }
        )
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
        return super.handleGeneratedButton(name)
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
        main.setChild(ImageListView(records: imageRecords, renderMode: renderMode, modalStack: modalStack))
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
            listView.setRenderMode(renderMode)
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

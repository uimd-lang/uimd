import Foundation
import Uimd

private let kNotesDirEnv = "UI_TEXT_EDITOR_NOTES_DIR"
private let kDefaultNoteEnv = "UI_TEXT_EDITOR_DEFAULT_NOTE"
private let kBrowserRootDirEnv = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR"
private let kDefaultBrowserDirEnv = "UI_TEXT_EDITOR_BROWSER_DIR"
private let kDefaultFileName = "welcome.txt"
private let kUntitledName = "untitled.txt"
private let kFileBrowserDoubleClickIntervalSeconds = 0.4

private func envString(_ name: String) -> String?
{
    let value = ProcessInfo.processInfo.environment[name] ?? ""
    return value.isEmpty ? nil : value
}

private func absolutePath(_ path: String) -> String
{
    if path.isEmpty
    {
        return FileManager.default.currentDirectoryPath
    }
    if path.hasPrefix("/")
    {
        return URL(fileURLWithPath: path).standardizedFileURL.path
    }
    return URL(fileURLWithPath: FileManager.default.currentDirectoryPath)
        .appendingPathComponent(path)
        .standardizedFileURL
        .path
}

private func pathByAppending(_ base: String, _ child: String) -> String
{
    URL(fileURLWithPath: base).appendingPathComponent(child).standardizedFileURL.path
}

private func parentPath(_ path: String) -> String
{
    URL(fileURLWithPath: path).deletingLastPathComponent().standardizedFileURL.path
}

private func displayFileName(_ path: String) -> String
{
    let name = URL(fileURLWithPath: path).lastPathComponent
    return name.isEmpty ? path : name
}

private func notesDir() -> String
{
    if let path = envString(kNotesDirEnv)
    {
        return absolutePath(path)
    }
    return pathByAppending(URL(fileURLWithPath: #filePath).deletingLastPathComponent().path, "notes")
}

private func defaultNoteName() -> String
{
    envString(kDefaultNoteEnv) ?? kDefaultFileName
}

private func userHomeDir() -> String
{
    let home = ProcessInfo.processInfo.environment["HOME"] ?? ""
    return home.isEmpty ? FileManager.default.currentDirectoryPath : home
}

private func isDirectory(_ path: String) -> Bool
{
    var isDir = ObjCBool(false)
    return FileManager.default.fileExists(atPath: path, isDirectory: &isDir) && isDir.boolValue
}

private func isRegularFile(_ path: String) -> Bool
{
    var isDir = ObjCBool(false)
    return FileManager.default.fileExists(atPath: path, isDirectory: &isDir) && !isDir.boolValue
}

private func browserRootDir() -> String
{
    if let path = envString(kBrowserRootDirEnv)
    {
        return absolutePath(path)
    }
    return userHomeDir()
}

private func defaultBrowserDir() -> String
{
    if let path = envString(kDefaultBrowserDirEnv)
    {
        return absolutePath(path)
    }
    let desktop = pathByAppending(userHomeDir(), "Desktop")
    return isDirectory(desktop) ? desktop : userHomeDir()
}

private func pathStartsWith(_ path: String, root: String) -> Bool
{
    let absoluteCandidate = absolutePath(path)
    let absoluteRoot = absolutePath(root)
    return absoluteCandidate == absoluteRoot || absoluteCandidate.hasPrefix(absoluteRoot + "/")
}

private func readTextFile(_ path: String) -> String
{
    (try? String(contentsOfFile: path, encoding: .utf8)) ?? ""
}

private func writeTextFile(_ path: String, _ text: String)
{
    try? text.write(toFile: path, atomically: true, encoding: .utf8)
}

private func trimmed(_ text: String) -> String
{
    text.trimmingCharacters(in: .whitespacesAndNewlines)
}

private func clamp(_ value: Int, lower: Int, upper: Int) -> Int
{
    min(max(value, lower), upper)
}

private final class MessageBoxBehavior
{
    private var onClose: ((String) -> Void)?
    private var resultByButton: [String: String] = [:]
    private var defaultResult = ""
    private(set) var result = ""
    private(set) var closed = false

    func configure(
        onClose: ((String) -> Void)?,
        resultByButton: [String: String],
        defaultResult: String
    )
    {
        self.onClose = onClose
        self.resultByButton = resultByButton
        self.defaultResult = defaultResult
        result = ""
        closed = false
    }

    func handleButton(_ name: String) -> Bool
    {
        guard let value = resultByButton[name] else
        {
            return false
        }
        close(value)
        return true
    }

    func handleKey(_ key: String) -> Bool
    {
        if key == "Escape" && !defaultResult.isEmpty
        {
            close(defaultResult)
            return true
        }
        return false
    }

    func buttonNameForResult(_ result: String) -> String?
    {
        resultByButton.first { $0.value == result }?.key
    }

    func close(_ result: String)
    {
        self.result = result
        closed = true
        onClose?(result)
    }
}

private final class MessageBoxYesNo: MessageBoxYesNoUI
{
    private let behavior = MessageBoxBehavior()

    init(_ header: String, _ text: String, onClose: ((String) -> Void)? = nil)
    {
        super.init()
        behavior.configure(
            onClose: onClose,
            resultByButton: ["yes_btn": "yes", "no_btn": "no"],
            defaultResult: "no"
        )
        dialog_header.setText(header)
        message.setText(text)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = GeneratedWindowRuntimeOptions()
        options.initialFocusName = behavior.buttonNameForResult("no")
        options.onButton = { [weak self] name in self?.behavior.handleButton(name) ?? false }
        options.onKey = { [weak self] key in self?.behavior.handleKey(key) ?? false }
        options.shouldClose = { [weak self] in self?.behavior.closed ?? false }
        return options
    }

    override func onYesBtnClick()
    {
        behavior.close("yes")
    }

    override func onNoBtnClick()
    {
        behavior.close("no")
    }
}

private final class MessageBoxYesNoCancel: MessageBoxYesNoCancelUI
{
    private let behavior = MessageBoxBehavior()

    init(_ header: String, _ text: String, onClose: ((String) -> Void)? = nil)
    {
        super.init()
        behavior.configure(
            onClose: onClose,
            resultByButton: ["yes_btn": "yes", "no_btn": "no", "cancel_btn": "cancel"],
            defaultResult: "cancel"
        )
        dialog_header.setText(header)
        message.setText(text)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = GeneratedWindowRuntimeOptions()
        options.initialFocusName = behavior.buttonNameForResult("cancel")
        options.onButton = { [weak self] name in self?.behavior.handleButton(name) ?? false }
        options.onKey = { [weak self] key in self?.behavior.handleKey(key) ?? false }
        options.shouldClose = { [weak self] in self?.behavior.closed ?? false }
        return options
    }

    override func onYesBtnClick()
    {
        behavior.close("yes")
    }

    override func onNoBtnClick()
    {
        behavior.close("no")
    }

    override func onCancelBtnClick()
    {
        behavior.close("cancel")
    }
}

private final class FileBrowser: FileBrowserUI
{
    private let rootDir: String
    private let browserMode: String
    private let onClose: ((String) -> Void)?
    private let extensionFilter: String
    private var initialFilename: String
    private var lastClickedEntry = -1
    private var lastEntryClickTime = Date.distantPast
    private(set) var currentDir: String
    private(set) var result = ""
    private(set) var closed = false

    init(
        rootDir: String,
        startPath: String,
        mode: String,
        onClose: ((String) -> Void)?,
        initialFilename: String = "",
        extensionFilter: String = ""
    )
    {
        self.rootDir = absolutePath(rootDir)
        self.browserMode = mode
        self.onClose = onClose
        self.extensionFilter = trimmed(extensionFilter)
        self.initialFilename = initialFilename
        let requestedStart = startPath.isEmpty ? self.rootDir : absolutePath(startPath)
        if isRegularFile(requestedStart)
        {
            self.currentDir = self.rootDir
        }
        else
        {
            self.currentDir = self.rootDir
        }
        super.init()
        if isRegularFile(requestedStart)
        {
            currentDir = clampDir(parentPath(requestedStart))
            if self.initialFilename.isEmpty
            {
                self.initialFilename = displayFileName(requestedStart)
            }
        }
        else
        {
            currentDir = clampDir(requestedStart)
        }
        dialog_header.setText(mode == "save" ? "Save As" : "Open File")
        open_btn.setTitle(mode == "save" ? "Save" : "Open")
        if browserMode == "save"
        {
            filename.setValue(self.initialFilename)
            moveFilenameCursorToEnd()
        }
        refreshEntries()
    }

    func refreshEntries()
    {
        var rows = [".."]
        var dirs: [String] = []
        var files: [String] = []
        let names = (try? FileManager.default.contentsOfDirectory(atPath: currentDir)) ?? []
        for name in names
        {
            let path = pathByAppending(currentDir, name)
            if isDirectory(path)
            {
                dirs.append(name + "/")
            }
            else if isRegularFile(path)
            {
                files.append(name)
            }
        }
        rows.append(contentsOf: dirs.sorted())
        rows.append(contentsOf: files.sorted())
        path_label.setText(currentDir)
        entries.options = rows
        if let selected = rows.firstIndex(of: initialFilename)
        {
            entries.selectedIndex = selected
        }
        else
        {
            entries.selectedIndex = rows.isEmpty ? -1 : 0
        }
        entries.setSelectedValues(entries.selectedIndex >= 0 && entries.selectedIndex < rows.count ? [rows[entries.selectedIndex]] : [])
        lastClickedEntry = -1
        previewSelected()
    }

    @discardableResult
    func acceptCurrent() -> Bool
    {
        let path = selectedPath()
        if path.isEmpty
        {
            return false
        }
        if isDirectory(path)
        {
            currentDir = clampDir(path)
            refreshEntries()
            return true
        }
        let selectedName = displayFileName(path)
        let name = trimmed(filename.value.isEmpty ? selectedName : filename.value)
        if name.isEmpty
        {
            return false
        }
        let namedPath = pathByAppending(currentDir, name)
        if browserMode == "open" && isRegularFile(namedPath) && clampDir(parentPath(namedPath)) == currentDir
        {
            if !pathMatchesFilter(namedPath)
            {
                return false
            }
            close(namedPath)
            return true
        }
        if browserMode == "save"
        {
            return acceptFilename()
        }
        return false
    }

    @discardableResult
    func acceptFilename() -> Bool
    {
        let name = trimmed(filename.value)
        if name.isEmpty
        {
            return false
        }
        let path = pathByAppending(currentDir, name)
        if isDirectory(path) || clampDir(parentPath(path)) != currentDir
        {
            return false
        }
        close(path)
        return true
    }

    func selectEntry(_ index: Int)
    {
        if entries.options.isEmpty
        {
            entries.selectedIndex = -1
        }
        else
        {
            entries.selectedIndex = clamp(index, lower: 0, upper: entries.options.count - 1)
        }
        if entries.selectedIndex >= 0 && entries.selectedIndex < entries.options.count
        {
            entries.setSelectedValues([entries.options[entries.selectedIndex]])
        }
        previewSelected()
    }

    func selectedEntryIsDirectory() -> Bool
    {
        entryIndexIsDirectory(entries.selectedIndex)
    }

    func entryIndexIsDirectory(_ index: Int) -> Bool
    {
        if index < 0 || index >= entries.options.count
        {
            return false
        }
        let selected = entries.options[index]
        return selected == ".." || selected.hasSuffix("/")
    }

    func handleEntryMousePress(_ point: Point) -> Bool
    {
        let frame = entries.frame
        if point.row < frame.row || point.row >= frame.row + frame.height || point.col < frame.col || point.col >= frame.col + frame.width
        {
            return false
        }
        let index = entries.scrollOffsetValue() + point.row - frame.row
        let now = Date()
        let doubleClick = index == lastClickedEntry && now.timeIntervalSince(lastEntryClickTime) <= kFileBrowserDoubleClickIntervalSeconds
        selectEntry(index)
        lastClickedEntry = index
        lastEntryClickTime = now
        if doubleClick && (entryIndexIsDirectory(index) || browserMode == "open")
        {
            let accepted = acceptCurrent()
            lastClickedEntry = -1
            return accepted
        }
        return false
    }

    func moveFilenameCursorToEnd()
    {
        filename.cursor = filename.value.count
    }

    func close(_ path: String)
    {
        result = path
        closed = true
        onClose?(result)
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = GeneratedWindowRuntimeOptions()
        options.initialFocusName = "entries"
        options.startInEditMode = true
        options.onButton = { [weak self] name in
            guard let self else
            {
                return false
            }
            if name == "open_btn"
            {
                if self.browserMode == "save"
                {
                    _ = self.acceptFilename()
                }
                else
                {
                    _ = self.acceptCurrent()
                }
                return true
            }
            if name == "close_btn"
            {
                self.close("")
                return true
            }
            return false
        }
        options.onTextChanged = { [weak self] name, _ in
            if name == "entries"
            {
                self?.previewSelected()
                return true
            }
            if name == "filename"
            {
                self?.updateOpenEnabled()
                return true
            }
            return false
        }
        options.onSelectionChanged = { [weak self] name, _ in
            if name == "entries"
            {
                self?.previewSelected()
                return true
            }
            return false
        }
        options.onTextConfirmed = { [weak self] name, _ in
            if name == "entries"
            {
                self?.previewSelected()
                return true
            }
            return false
        }
        options.onKeyBeforeFocusedElement = { [weak self] key, name, editMode in
            guard let self else
            {
                return false
            }
            if name == "entries" && editMode && key == "Enter"
            {
                if self.selectedEntryIsDirectory()
                {
                    _ = self.acceptCurrent()
                }
                return true
            }
            return false
        }
        options.onMousePressBeforeFocused = { [weak self] point in
            self?.handleEntryMousePress(point) ?? false
        }
        options.onEditStarted = { [weak self] name in
            if name == "filename"
            {
                self?.moveFilenameCursorToEnd()
                return true
            }
            return false
        }
        options.onKey = { [weak self] key in
            if key == "Escape"
            {
                self?.close("")
                return true
            }
            return false
        }
        options.shouldClose = { [weak self] in self?.closed ?? false }
        return options
    }

    private func clampDir(_ path: String) -> String
    {
        let candidate = absolutePath(path)
        if !isDirectory(candidate)
        {
            return rootDir
        }
        if !pathStartsWith(candidate, root: rootDir)
        {
            return rootDir
        }
        return candidate
    }

    private func pathMatchesFilter(_ path: String) -> Bool
    {
        if extensionFilter.isEmpty
        {
            return true
        }
        let ext = URL(fileURLWithPath: path).pathExtension
        if ext.isEmpty
        {
            return false
        }
        if let regex = try? NSRegularExpression(pattern: "^\(extensionFilter)$", options: [.caseInsensitive])
        {
            let range = NSRange(ext.startIndex..<ext.endIndex, in: ext)
            return regex.firstMatch(in: ext, options: [], range: range) != nil
        }
        for part in extensionFilter.split(separator: "|")
        {
            let cleaned = trimmed(String(part)).trimmingCharacters(in: CharacterSet(charactersIn: ".")).lowercased()
            if ext.lowercased() == cleaned
            {
                return true
            }
        }
        return false
    }

    private func selectedPath() -> String
    {
        if entries.options.isEmpty || entries.selectedIndex < 0 || entries.selectedIndex >= entries.options.count
        {
            return ""
        }
        let selected = entries.options[entries.selectedIndex]
        if selected == ".."
        {
            return parentPath(currentDir)
        }
        if selected.hasSuffix("/")
        {
            return pathByAppending(currentDir, String(selected.dropLast()))
        }
        return pathByAppending(currentDir, selected)
    }

    private func selectedPathForOpenState() -> String
    {
        let name = trimmed(filename.value)
        if !name.isEmpty
        {
            return pathByAppending(currentDir, name)
        }
        return selectedPath()
    }

    private func previewSelected()
    {
        if entries.options.isEmpty || entries.selectedIndex < 0 || entries.selectedIndex >= entries.options.count
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
        moveFilenameCursorToEnd()
        updateOpenEnabled()
    }

    private func updateOpenEnabled()
    {
        if browserMode == "save"
        {
            open_btn.enabled = !trimmed(filename.value).isEmpty
            return
        }
        let path = selectedPathForOpenState()
        if path.isEmpty
        {
            open_btn.enabled = false
            return
        }
        if isDirectory(path)
        {
            open_btn.enabled = true
            return
        }
        open_btn.enabled = isRegularFile(path) && pathMatchesFilter(path)
    }
}

private final class TextEditorApp: TextEditorUI
{
    private let modalStack = GeneratedWindowStack()
    private var currentPath: String?
    private var loadedText = ""
    private var pendingAction: (() -> Void)?
    private var afterSaveAction: (() -> Void)?
    private var unsavedDialog: MessageBoxYesNoCancel?
    private var overwriteDialog: MessageBoxYesNo?
    private var browser: FileBrowser?
    private var overwritePath = ""
    private var finished = false

    override init()
    {
        super.init()
        loadPath(pathByAppending(notesDir(), defaultNoteName()))
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        return options
    }

    func newFile()
    {
        currentPath = nil
        filename.setText(kUntitledName)
        setEditorText("")
        loadedText = ""
        refreshStatus("New file")
    }

    func loadPath(_ path: String)
    {
        let absolute = absolutePath(path)
        let text = readTextFile(absolute)
        currentPath = absolute
        filename.setText(displayFileName(absolute))
        setEditorText(text)
        loadedText = text
        refreshStatus("Loaded")
    }

    func saveOrSaveAs()
    {
        guard let currentPath else
        {
            saveAsBrowser()
            return
        }
        savePath(currentPath)
    }

    func openBrowser()
    {
        browser = FileBrowser(
            rootDir: browserRootDir(),
            startPath: defaultBrowserDir(),
            mode: "open",
            onClose: { [weak self] path in self?.onOpenSelected(path) }
        )
        pushBrowserFrame()
    }

    func saveAsBrowser()
    {
        var start = currentPath.map(parentPath) ?? notesDir()
        if !pathStartsWith(start, root: browserRootDir())
        {
            start = defaultBrowserDir()
        }
        let initialFilename = currentPath.map(displayFileName) ?? kUntitledName
        browser = FileBrowser(
            rootDir: browserRootDir(),
            startPath: start,
            mode: "save",
            onClose: { [weak self] path in self?.onSaveSelected(path) },
            initialFilename: initialFilename
        )
        pushBrowserFrame()
    }

    func savePath(_ path: String)
    {
        let absolute = absolutePath(path)
        writeTextFile(absolute, editor.value)
        currentPath = absolute
        filename.setText(displayFileName(absolute))
        loadedText = editor.value
        refreshStatus("Saved")
    }

    func markModified()
    {
        refreshStatus("Modified")
    }

    func modified() -> Bool
    {
        editor.value != loadedText
    }

    func withSavedChanges(_ action: @escaping () -> Void, simulatedChoice: String = "")
    {
        afterSaveAction = nil
        if !modified()
        {
            action()
            return
        }
        pendingAction = action
        if simulatedChoice.isEmpty
        {
            unsavedDialog = MessageBoxYesNoCancel("Unsaved Changes", "Save current file before continuing?")
            pushUnsavedFrame()
            return
        }
        onUnsavedChoice(simulatedChoice)
    }

    func quit()
    {
        finished = true
    }

    override func shouldClose() -> Bool
    {
        finished
    }

    func hasModalOverlay() -> Bool
    {
        browser != nil || unsavedDialog != nil || overwriteDialog != nil
    }

    func onOpenSelected(_ path: String)
    {
        if !path.isEmpty
        {
            loadPath(path)
        }
    }

    func onSaveSelected(_ path: String)
    {
        let action = afterSaveAction
        afterSaveAction = nil
        if !path.isEmpty
        {
            savePath(path)
            action?()
        }
        else if action != nil
        {
            refreshStatus("Cancelled")
        }
    }

    override func onNewBtnClick()
    {
        withSavedChanges { [weak self] in self?.newFile() }
    }

    override func onOpenBtnClick()
    {
        withSavedChanges { [weak self] in self?.openBrowser() }
    }

    override func onSaveBtnClick()
    {
        saveOrSaveAs()
    }

    override func onSaveAsBtnClick()
    {
        saveAsBrowser()
    }

    override func onQuitBtnClick()
    {
        withSavedChanges { [weak self] in self?.quit() }
    }

    override func onEditorChange(_ value: String)
    {
        _ = value
        markModified()
    }

    private func acceptBrowserCurrent()
    {
        _ = browser?.acceptCurrent()
        closeBrowserIfClosed()
    }

    private func acceptBrowserOpenButton()
    {
        guard let browser else
        {
            return
        }
        if browser.open_btn.title == "Save"
        {
            let name = browser.filename.value
            if name.isEmpty
            {
                return
            }
            let path = pathByAppending(browser.currentDir, name)
            if isRegularFile(path)
            {
                overwritePath = path
                overwriteDialog = MessageBoxYesNo("Overwrite File", "File already exists:\n" + path + "\n\nOverwrite it?")
                pushOverwriteFrame()
                return
            }
            closeBrowser(path)
            return
        }
        let name = browser.filename.value
        if !name.isEmpty
        {
            let path = pathByAppending(browser.currentDir, name)
            if isRegularFile(path)
            {
                closeBrowser(path)
                return
            }
        }
        acceptBrowserCurrent()
    }

    private func closeBrowserIfClosed()
    {
        guard let activeBrowser = browser, activeBrowser.closed else
        {
            return
        }
        modalStack.remove(activeBrowser)
        browser = nil
    }

    private func closeBrowser(_ path: String)
    {
        guard let closing = browser else
        {
            return
        }
        closing.close(path)
        modalStack.remove(closing)
        if let activeBrowser = browser, activeBrowser === closing
        {
            browser = nil
        }
    }

    private func closeUnsaved(_ choice: String)
    {
        if let dialog = unsavedDialog
        {
            modalStack.remove(dialog)
        }
        unsavedDialog = nil
        onUnsavedChoice(choice)
    }

    private func closeOverwrite(_ choice: String)
    {
        if let dialog = overwriteDialog
        {
            modalStack.remove(dialog)
        }
        overwriteDialog = nil
        if choice == "yes"
        {
            closeBrowser(overwritePath)
        }
        overwritePath = ""
    }

    private func pushBrowserFrame()
    {
        guard let browser else
        {
            return
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "FileBrowser"
        frame.initialFocusName = "entries"
        frame.startInEditMode = true
        frame.onButton = { [weak self] name in
            if name == "open_btn"
            {
                self?.acceptBrowserOpenButton()
                return true
            }
            if name == "close_btn"
            {
                self?.closeBrowser("")
                return true
            }
            return false
        }
        frame.onTextChanged = { [weak self] name, _ in
            if let browser = self?.browser, name == "entries"
            {
                browser.selectEntry(browser.entries.selectedIndex)
                return true
            }
            return false
        }
        frame.onSelectionChanged = { [weak self] name, _ in
            if let browser = self?.browser, name == "entries"
            {
                browser.selectEntry(browser.entries.selectedIndex)
                return true
            }
            return false
        }
        frame.onTextConfirmed = frame.onTextChanged
        frame.onMousePressBeforeFocused = { [weak self] point in
            self?.browser?.handleEntryMousePress(point) ?? false
        }
        frame.onKeyBeforeFocusedElement = { [weak self] key, name, editMode in
            guard let self, self.browser != nil else
            {
                return false
            }
            if key == "Escape"
            {
                self.closeBrowser("")
                return true
            }
            if name == "entries" && editMode && key == "Enter"
            {
                self.acceptBrowserCurrent()
                return true
            }
            return false
        }
        modalStack.push(browser, frame)
    }

    private func pushUnsavedFrame()
    {
        guard let unsavedDialog else
        {
            return
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "MessageBoxYesNoCancel"
        frame.initialFocusName = "cancel_btn"
        frame.onButton = { [weak self] name in
            if name == "yes_btn"
            {
                self?.closeUnsaved("yes")
                return true
            }
            if name == "no_btn"
            {
                self?.closeUnsaved("no")
                return true
            }
            if name == "cancel_btn"
            {
                self?.closeUnsaved("cancel")
                return true
            }
            return false
        }
        frame.onKey = { [weak self] key in
            if key == "Escape"
            {
                self?.closeUnsaved("cancel")
                return true
            }
            return false
        }
        modalStack.push(unsavedDialog, frame)
    }

    private func pushOverwriteFrame()
    {
        guard let overwriteDialog else
        {
            return
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "MessageBoxYesNo"
        frame.initialFocusName = "no_btn"
        frame.onButton = { [weak self] name in
            if name == "yes_btn"
            {
                self?.closeOverwrite("yes")
                return true
            }
            if name == "no_btn"
            {
                self?.closeOverwrite("no")
                return true
            }
            return false
        }
        frame.onKey = { [weak self] key in
            if key == "Escape"
            {
                self?.closeOverwrite("no")
                return true
            }
            return false
        }
        modalStack.push(overwriteDialog, frame)
    }

    private func onUnsavedChoice(_ choice: String)
    {
        let action = pendingAction
        pendingAction = nil
        if choice == "cancel" || action == nil
        {
            refreshStatus("Cancelled")
            return
        }
        if choice == "yes"
        {
            if currentPath == nil
            {
                afterSaveAction = action
                saveAsBrowser()
                return
            }
            saveOrSaveAs()
            action?()
            return
        }
        if choice == "no"
        {
            action?()
        }
    }

    private func setEditorText(_ text: String)
    {
        editor.setValue(text)
        editor.cursor = 0
    }

    private func refreshStatus(_ prefix: String)
    {
        let text = editor.value
        let modifiedMarker = modified() ? "*" : ""
        let lineCount = text.isEmpty ? 0 : text.filter { $0 == "\n" }.count + 1
        status.setText(prefix + ": " + filename.text + modifiedMarker + " - " + String(lineCount) + " lines, " + String(text.count) + " chars")
    }
}

private func smoke()
{
    let app = TextEditorApp()
    precondition(app.windowTitle == "Text Editor")
    precondition(app.layout.count == 10)
    precondition(app.window_header != nil)
    precondition(app.filename != nil)
    precondition(app.editor != nil)
    precondition(app.new_btn != nil)
    precondition(app.open_btn != nil)
    precondition(app.save_btn != nil)
    precondition(app.save_as_btn != nil)
    precondition(app.quit_btn != nil)
    precondition(app.status != nil)
    precondition(app.filename.text.contains(kDefaultFileName))
}

private func logicTest()
{
    do
    {
        let modalApp = TextEditorApp()
        precondition(!modalApp.hasModalOverlay())
        modalApp.openBrowser()
        precondition(modalApp.hasModalOverlay())
    }

    let app = TextEditorApp()
    precondition(!app.editor.value.isEmpty)
    app.newFile()
    precondition(app.filename.text == kUntitledName)
    precondition(app.editor.value.isEmpty)
    app.editor.setValue("draft")
    app.markModified()
    precondition(app.modified())
    precondition(app.status.text == "Modified: untitled.txt* - 1 lines, 5 chars")
    var actionCalled = false
    app.withSavedChanges({
        actionCalled = true
    }, simulatedChoice: "no")
    precondition(actionCalled)

    let savedPath = pathByAppending(FileManager.default.temporaryDirectory.path, "ui_swift_text_editor_logic.txt")
    app.editor.setValue("saved draft")
    app.onSaveSelected(savedPath)
    precondition(readTextFile(savedPath) == "saved draft")
    precondition(!app.modified())
    try? FileManager.default.removeItem(atPath: savedPath)

    app.withSavedChanges {
        app.quit()
    }
    precondition(app.shouldClose())
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
            let app = TextEditorApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

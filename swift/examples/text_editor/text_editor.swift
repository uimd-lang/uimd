import Foundation
import Uimd

private let kNotesDirEnv = "UI_TEXT_EDITOR_NOTES_DIR"
private let kDefaultNoteEnv = "UI_TEXT_EDITOR_DEFAULT_NOTE"
private let kBrowserRootDirEnv = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR"
private let kDefaultBrowserDirEnv = "UI_TEXT_EDITOR_BROWSER_DIR"
private let kDefaultFileName = "welcome.txt"
private let kUntitledName = "untitled.txt"

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
            root: browserRootDir(),
            start: defaultBrowserDir(),
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
            root: browserRootDir(),
            start: start,
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
            let path = pathByAppending(browser.currentDirectory, name)
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
            let path = pathByAppending(browser.currentDirectory, name)
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
        frame.onKey = { [weak self] key in
            if key == "Escape"
            {
                self?.closeBrowser("")
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

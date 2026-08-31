import XCTest
import Darwin
@testable import Uimd

final class UimdRuntimeSmokeTests: XCTestCase
{
    func testTextInputAlignmentUsesOneRenderAndMouseOffset()
    {
        let input = TextInput("field", "abc", maxLength: 10)
        input.style.textAlign = "right"
        var cursorStyle = Style()
        cursorStyle.background = Color("#facc15")
        input.cursorStyle = cursorStyle

        var rendered = input.render(size: Size(width: 6, height: 1))
        XCTAssertEqual(rendered[0].map(\.text).joined(), "   abc")

        input.cursor = 1
        rendered = input.render(
            size: Size(width: 6, height: 1),
            state: ElementRenderState(editMode: true)
        )
        XCTAssertEqual(rendered[0][4].background, Color("#facc15"))

        input.selectRange(start: 0, end: 2)
        rendered = input.render(
            size: Size(width: 6, height: 1),
            state: ElementRenderState(editMode: true)
        )
        XCTAssertEqual(rendered[0][3].background, Color("#facc15"))
        XCTAssertEqual(rendered[0][4].background, Color("#facc15"))
        XCTAssertNotEqual(rendered[0][5].background, Color("#facc15"))
        XCTAssertEqual(input.cursorForPoint(row: 0, col: 0, size: Size(width: 6, height: 1)), 0)
        XCTAssertEqual(input.cursorForPoint(row: 0, col: 4, size: Size(width: 6, height: 1)), 1)

        input.setValue("abcdefgh")
        input.cursor = 8
        rendered = input.render(
            size: Size(width: 6, height: 1),
            state: ElementRenderState(editMode: true)
        )
        XCTAssertEqual(rendered[0].map(\.text).joined(), "defgh ")
    }

    func testComboBoxMouseSelectionUsesEveryRenderedOptionRow() throws
    {
        let options = (0..<8).map { "Option \($0)" }
        let window = GeneratedWindowBase("Combo mouse")
        let comboBox = window.addElement(ComboBox("choice", options))
        var windowStyle = Style()
        windowStyle.borderWidthHorizontal = 0
        windowStyle.borderWidthVertical = 0
        window.setGeneratedWindowStyle(windowStyle)
        window.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "choice",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 12, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 12, height: 1),
                width: .fixed(12),
                height: .fixed(1),
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 12, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])
        var runtimeOptions = GeneratedWindowRuntimeOptions()
        runtimeOptions.initialFocusName = "choice"
        var selectionChanges = 0
        runtimeOptions.onSelectionChanged = { _, _ in
            selectionChanges += 1
            return true
        }
        let runtime = GeneratedRuntimeController(
            window: window,
            options: runtimeOptions,
            args: ["test", "--viewport", "0,0,20,12"]
        )

        _ = try runtime.handleRequest([
            "method": "press_key",
            "params": ["key": "Enter"],
        ])
        _ = try runtime.handleRequest([
            "method": "mouse_press",
            "params": [
                "x": comboBox.frame.col + 1,
                "y": comboBox.frame.row + 7,
            ],
        ])

        XCTAssertEqual(comboBox.selectedIndex, 6)
        XCTAssertEqual(selectionChanges, 1)
        _ = try runtime.handleRequest([
            "method": "press_key",
            "params": ["key": "Down"],
        ])
        XCTAssertEqual(comboBox.selectedIndex, 6)
    }

    func testReusableGeneratedScrollViewFocusUnderlaysAlphaDescendantBackground()
    {
        let parentBackground = Color("#303545")
        let focusBackground = Color("#ffffff14")
        let descendantBackground = Color("#252a36cc")
        let child = GeneratedScrollViewBase("Alpha focus scroll")
        var windowStyle = Style()
        windowStyle.background = parentBackground
        windowStyle.borderWidthHorizontal = 0
        windowStyle.borderWidthVertical = 0
        child.setGeneratedWindowStyle(windowStyle)
        child.setGeneratedFocusable(true)

        let items = child.addElement(ScrollView("items"))
        items.style.background = parentBackground
        child.setGeneratedScrollView(items)
        let row = items.addChild(Label("row", " "))
        row.style.background = descendantBackground
        child.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "items",
                type: "scrollview",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 1, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 1, height: 1),
                width: .fixed(1),
                height: .fixed(1),
                cellWidth: .fixed(1),
                cellHeight: .fixed(1),
                charsSize: Size(width: 1, height: 1),
                cellCharsSize: Size(width: 1, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let host = ReusableElement("card")
        host.style.background = parentBackground
        host.focusStyle = Style()
        host.focusStyle?.background = focusBackground
        host.setChild(child)
        let size = Size(width: 1, height: 1)
        let unfocused = host.render(
            size: size,
            state: ElementRenderState(parentBackground: parentBackground)
        )[0][0].background
        let focused = host.render(
            size: size,
            state: ElementRenderState(focused: true, parentBackground: parentBackground)
        )[0][0].background

        XCTAssertEqual(unfocused, descendantBackground.blended(over: parentBackground))
        XCTAssertEqual(
            focused,
            descendantBackground.blended(over: focusBackground.blended(over: parentBackground))
        )

        // A structural blank exercises the root ScrollView's own focus
        // surface; the alpha content assertion above remains protected.
        row.style.background = nil
        let opaqueChildFocus = Color("#1e3a5f")
        items.focusStyle = Style()
        items.focusStyle?.background = opaqueChildFocus
        let opaqueFocused = host.render(
            size: size,
            state: ElementRenderState(focused: true, parentBackground: parentBackground)
        )[0][0].background
        XCTAssertEqual(opaqueFocused, opaqueChildFocus)

        let partialChildFocus = Color("#ffffff14")
        items.focusStyle?.background = partialChildFocus
        let partialFocused = host.render(
            size: size,
            state: ElementRenderState(focused: true, parentBackground: parentBackground)
        )[0][0].background
        XCTAssertEqual(
            partialFocused,
            focusBackground.blended(over: parentBackground)
        )
    }

    func testModalBackgroundSuppressesCoveredReusableScrollViewFocusSurface() throws
    {
        let baseBackground = Color("#030712")
        let focusBackground = Color("#ffffff14")
        let hostFocusBackground = Color("#07111f")
        let child = GeneratedScrollViewBase("Covered scroll")
        var childStyle = Style()
        childStyle.background = baseBackground
        childStyle.borderWidthHorizontal = 0
        childStyle.borderWidthVertical = 0
        child.setGeneratedWindowStyle(childStyle)
        child.setGeneratedFocusable(true)
        let scrollView = child.addElement(ScrollView("items"))
        scrollView.style.background = baseBackground
        scrollView.focusStyle = Style()
        scrollView.focusStyle?.background = focusBackground
        _ = scrollView.addChild(Label("row", " "))
        child.setGeneratedScrollView(scrollView)
        child.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "items",
                type: "scrollview",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 3, height: 1),
                width: .fixed(3),
                height: .fixed(1),
                cellWidth: .fixed(3),
                cellHeight: .fixed(1),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 3, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let root = GeneratedWindowBase("Root")
        root.setGeneratedWindowStyle(childStyle)
        let host = root.addElement(ReusableElement("card"))
        host.style.background = baseBackground
        host.focusStyle = Style()
        host.focusStyle?.background = hostFocusBackground
        host.setChild(child)
        root.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "card",
                type: "uielementreusable",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 3, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 3, height: 1),
                width: .fixed(3),
                height: .fixed(1),
                cellWidth: .fixed(3),
                cellHeight: .fixed(1),
                charsSize: Size(width: 3, height: 1),
                cellCharsSize: Size(width: 3, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let modal = GeneratedWindowBase("Modal")
        var modalStyle = Style()
        modalStyle.background = Color("#111111")
        modalStyle.borderWidthHorizontal = 0
        modalStyle.borderWidthVertical = 0
        modal.setGeneratedWindowStyle(modalStyle)
        _ = modal.addElement(Label("message", "M"))
        modal.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "message",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 1, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 1, height: 1),
                width: .fixed(1),
                height: .fixed(1),
                cellWidth: .fixed(1),
                cellHeight: .fixed(1),
                charsSize: Size(width: 1, height: 1),
                cellCharsSize: Size(width: 1, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let stack = GeneratedWindowStack()
        var runtimeOptions = GeneratedWindowRuntimeOptions()
        runtimeOptions.initialFocusName = "card"
        runtimeOptions.windowStack = stack
        let runtime = GeneratedRuntimeController(
            window: root,
            options: runtimeOptions,
            args: ["test", "--viewport", "0,0,5,3"]
        )
        _ = try runtime.handleRequest([
            "method": "enter_edit_mode",
            "params": ["element_id": "card"],
        ])
        let focusedResponse = try runtime.handleRequest([
            "method": "ui.get_render_cell",
            "params": ["x": 1, "y": 1],
        ])
        let focusedCell = try XCTUnwrap(focusedResponse as? [String: Any])
        XCTAssertEqual(focusedCell["background"] as? String, "#07111f")

        stack.push(modal)
        let response = try runtime.handleRequest([
            "method": "ui.get_render_cell",
            "params": ["x": 1, "y": 1],
        ])
        let cell = try XCTUnwrap(response as? [String: Any])
        XCTAssertEqual(cell["background"] as? String, "#010309")
    }

    func testUnfocusedGeneratedScrollDynamicReusableDoesNotReceiveFocusSurface()
    {
        let baseBackground = Color("#030712")
        let focusBackground = Color("#ffffff14")
        let selectedBackground = Color("#ffffff26")

        let itemWindow = GeneratedWindowBase("Item")
        var itemWindowStyle = Style()
        itemWindowStyle.background = baseBackground
        itemWindowStyle.borderWidthHorizontal = 0
        itemWindowStyle.borderWidthVertical = 0
        itemWindow.setGeneratedWindowStyle(itemWindowStyle)
        _ = itemWindow.addElement(Label("caption", " "))
        itemWindow.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "caption",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 1, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 1, height: 1),
                width: .fixed(1),
                height: .fixed(1),
                cellWidth: .fixed(1),
                cellHeight: .fixed(1),
                charsSize: Size(width: 1, height: 1),
                cellCharsSize: Size(width: 1, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let item = ReusableElement("item")
        item.setChild(itemWindow)
        item.frame = Rect(row: 0, col: 0, width: 1, height: 1)

        let listWindow = GeneratedScrollViewBase("List")
        var listWindowStyle = Style()
        listWindowStyle.background = baseBackground
        listWindowStyle.borderWidthHorizontal = 0
        listWindowStyle.borderWidthVertical = 0
        listWindow.setGeneratedWindowStyle(listWindowStyle)
        let items = listWindow.addElement(ScrollView("items"))
        items.style.background = baseBackground
        items.focusStyle = Style()
        items.focusStyle?.background = focusBackground
        items.selectedStyle = Style()
        items.selectedStyle?.background = selectedBackground
        _ = items.addChild(item)
        listWindow.setGeneratedScrollView(items)
        listWindow.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "items",
                type: "scrollview",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 1, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 1, height: 1),
                width: .fixed(1),
                height: .fixed(1),
                cellWidth: .fixed(1),
                cellHeight: .fixed(1),
                charsSize: Size(width: 1, height: 1),
                cellCharsSize: Size(width: 1, height: 1),
                marginRight: 0,
                marginBottom: 0
            ),
        ])

        let rendered = renderGeneratedContentForHost(
            listWindow,
            size: Size(width: 1, height: 1),
            focusedName: nil,
            editMode: false
        )
        XCTAssertEqual(rendered[0][0].background, baseBackground)
    }

    func testPlainTextRenderIncludesTitleAndElements()
    {
        let window = GeneratedWindowBase("Hello")
        let label = window.addElement(Label("headline", "Hello from UIMD"))
        let button = window.addElement(Button("submit", "Submit"))
        window.setGeneratedLayout([
            GeneratedLayoutEntry(
                name: "headline",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 15, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 15, height: 2),
                width: .auto(15),
                height: .auto(1),
                cellWidth: .auto(15),
                cellHeight: .auto(2),
                charsSize: Size(width: 15, height: 1),
                cellCharsSize: Size(width: 15, height: 2),
                marginRight: 0,
                marginBottom: 1
            ),
            GeneratedLayoutEntry(
                name: "submit",
                type: "button",
                cellName: "",
                relative: Rect(row: 1, col: 0, width: 6, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 15, height: 2),
                width: .auto(6),
                height: .auto(1),
                cellWidth: .auto(15),
                cellHeight: .auto(2),
                charsSize: Size(width: 6, height: 1),
                cellCharsSize: Size(width: 15, height: 2),
                marginRight: 9,
                marginBottom: 0
            ),
        ])

        XCTAssertEqual(label.text, "Hello from UIMD")
        XCTAssertEqual(button.terminalText, "Submit")
        XCTAssertEqual(window.renderPlainText(), "Hello from UIMD\nSubmit         ")
    }

    func testGeneratedButtonDispatchCanBeOverridden()
    {
        final class TestWindow: GeneratedWindowBase
        {
            var clicked = false

            override func handleGeneratedButton(_ name: String) -> Bool
            {
                guard name == "go" else
                {
                    return false
                }
                clicked = true
                return true
            }
        }

        let window = TestWindow("Dispatch")

        XCTAssertTrue(window.handleGeneratedButton("go"))
        XCTAssertTrue(window.clicked)
        XCTAssertFalse(window.handleGeneratedButton("missing"))
    }

    func testScrollViewUsesTopAndBottomOffsetsLikeNativeRuntime()
    {
        let scroll = ScrollView("items")
        scroll.addChild([[TerminalCell("A")]])
        scroll.addChild([[TerminalCell("B")]])
        scroll.addChild([[TerminalCell("C")]])
        scroll.addChild([[TerminalCell("D")]])

        func renderedText() -> String
        {
            scroll.render(size: Size(width: 2, height: 2))
                .map { $0.first?.text ?? " " }
                .joined()
        }

        XCTAssertEqual(renderedText(), "AB")
        XCTAssertFalse(scroll.scrollToTop(Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "AB")
        XCTAssertTrue(scroll.handleKey("Down", viewport: Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "BC")
        XCTAssertTrue(scroll.handleKey("Up", viewport: Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "AB")
        XCTAssertTrue(scroll.scrollToBottom(Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "CD")
    }

    func testScrollViewLineOffsetIsClampedAfterChildOffset()
    {
        let scroll = ScrollView("items")
        scroll.addChild([[TerminalCell("A")]])
        scroll.addChild([[TerminalCell("B")]])
        scroll.addChild([[TerminalCell("C")]])
        scroll.addChild([[TerminalCell("D")]])

        XCTAssertTrue(scroll.scrollTo(2, viewport: Size(width: 2, height: 2)))
        XCTAssertEqual(scroll.contentHeight(width: 2), 2)
        XCTAssertEqual(scroll.maxViewOffset(viewport: Size(width: 2, height: 2)), 0)

        let rendered = scroll.render(size: Size(width: 2, height: 2))
            .map { $0.first?.text ?? " " }
            .joined()
        XCTAssertEqual(rendered, "CD")
        XCTAssertFalse(scroll.handleKey("Down", viewport: Size(width: 2, height: 2)))
    }

    func testScrollViewBottomClampAlignsLastNativeChildWithViewportBottom()
    {
        let scroll = ScrollView("items", 1)
        for index in 0..<4
        {
            let child = UIElement("child\(index)")
            child.frame = Rect(row: 0, col: 0, width: 4, height: 3)
            scroll.addChild(child)
        }

        XCTAssertTrue(scroll.scrollToBottom(Size(width: 4, height: 5)))
        let visible = scroll.childViews(size: Size(width: 4, height: 5))
            .filter { $0.visible }

        XCTAssertEqual(scroll.contentHeight(width: 4), 15)
        XCTAssertEqual(scroll.maxViewOffset(viewport: Size(width: 4, height: 5)), 10)
        XCTAssertEqual(visible.last?.frame.row, 2)
        XCTAssertEqual((visible.last?.frame.row ?? 0) + (visible.last?.frame.height ?? 0), 5)
    }

    func testScrollViewRestorePreservesOffsetAcrossRebuildAndClampsAfterShrink()
    {
        let frameSize = Size(width: 4, height: 2)
        let originalContentHeight = 5
        let shrunkContentHeight = 3

        func content(height: Int) -> [[TerminalCell]]
        {
            Array(repeating: [TerminalCell("A")], count: height)
        }

        let scroll = ScrollView("items")
        scroll.frame = Rect(
            row: 0,
            col: 0,
            width: frameSize.width,
            height: frameSize.height
        )
        scroll.addChild(content(height: originalContentHeight))
        _ = scroll.render(size: frameSize)
        XCTAssertTrue(scroll.scrollLines(-1, viewport: frameSize))
        let saved = scroll.scrollPosition()

        scroll.clearChildren()
        scroll.addChild(content(height: originalContentHeight))
        scroll.restoreScrollPosition(saved)
        XCTAssertEqual(scroll.viewOffsetValue(), saved.viewOffset)

        scroll.clearChildren()
        scroll.addChild(content(height: shrunkContentHeight))
        scroll.restoreScrollPosition(saved)
        XCTAssertEqual(scroll.viewOffsetValue(), shrunkContentHeight - frameSize.height)
    }

    func testFileBrowserDirectoryEnterCommitsAndHidesActiveRow() throws
    {
        let root = FileManager.default.temporaryDirectory
            .appendingPathComponent("uimd-swift-file-browser-state-\(UUID().uuidString)")
        let child = root.appendingPathComponent("child")
        try FileManager.default.createDirectory(at: child, withIntermediateDirectories: true)
        defer
        {
            try? FileManager.default.removeItem(at: root)
        }

        let browser = FileBrowser(root: root.path, start: root.path)
        guard let childIndex = browser.entries.options.firstIndex(of: "child/") else
        {
            XCTFail("FileBrowser did not expose the child directory")
            return
        }
        browser.entries.setActiveIndex(childIndex)
        browser.entries.showActiveItem()

        let handled = browser.handleGeneratedListBoxItemActivate(
            "entries",
            elementId: "entries",
            index: childIndex,
            value: "child/"
        )

        XCTAssertTrue(handled)
        XCTAssertFalse(browser.entries.activeItemVisible)
        XCTAssertEqual(browser.currentDirectory, child.path)
        XCTAssertEqual(browser.entries.selectedValues, [".."])
    }

    func testFileBrowserMouseClickEntersDirectoryAndOnlySelectsFile() throws
    {
        let root = FileManager.default.temporaryDirectory
            .appendingPathComponent("uimd-swift-file-browser-mouse-\(UUID().uuidString)")
        let child = root.appendingPathComponent("child")
        let file = root.appendingPathComponent("photo.png")
        try FileManager.default.createDirectory(at: child, withIntermediateDirectories: true)
        try Data("png".utf8).write(to: file)
        defer
        {
            try? FileManager.default.removeItem(at: root)
        }

        let browser = FileBrowser(root: root.path, start: root.path)
        browser.entries.frame = Rect(row: 0, col: 0, width: 20, height: 5)
        let childIndex = try XCTUnwrap(browser.entries.options.firstIndex(of: "child/"))
        XCTAssertEqual(
            browser.runtimeOptions().onMousePressBeforeFocused?(
                Point(row: childIndex, col: 0)
            ),
            true
        )
        XCTAssertEqual(browser.currentDirectory, child.path)

        browser.entries.frame = Rect(row: 0, col: 0, width: 20, height: 5)
        XCTAssertEqual(
            browser.runtimeOptions().onMousePressBeforeFocused?(Point(row: 0, col: 0)),
            true
        )
        XCTAssertEqual(browser.currentDirectory, root.path)

        browser.entries.frame = Rect(row: 0, col: 0, width: 20, height: 5)
        let fileIndex = try XCTUnwrap(browser.entries.options.firstIndex(of: "photo.png"))
        XCTAssertEqual(
            browser.runtimeOptions().onMousePressBeforeFocused?(
                Point(row: fileIndex, col: 0)
            ),
            false
        )
        XCTAssertEqual(browser.entries.selectedValues, ["photo.png"])
        XCTAssertEqual(browser.filename.value, "photo.png")
        XCTAssertFalse(browser.shouldClose())
    }

    func testImageFallbackHalfBlockKeepsTopPixelInForeground() throws
    {
        let directory = FileManager.default.temporaryDirectory
        let path = directory.appendingPathComponent("uimd-fallback-half-block.ppm")
        var data = Data("P6\n1 2\n255\n".utf8)
        data.append(contentsOf: [255, 0, 0, 0, 0, 255])
        try data.write(to: path)
        defer
        {
            try? FileManager.default.removeItem(at: path)
        }

        let image = Image(
            "probe",
            source: path.path,
            fit: "stretch",
            renderMode: "fallback",
            align: "left",
            verticalAlign: "top"
        )

        let rows = image.render(size: Size(width: 1, height: 1))

        XCTAssertEqual(rows.count, 1)
        XCTAssertEqual(rows[0].count, 1)
        XCTAssertEqual(rows[0][0].text, "▀")
        XCTAssertEqual(rows[0][0].foreground?.snapshotValue, "#ff0000")
        XCTAssertEqual(rows[0][0].background?.snapshotValue, "#0000ff")
    }

    func testImageSixelModeFallsBackForAppleTerminal() throws
    {
        let previousTermProgram = ProcessInfo.processInfo.environment["TERM_PROGRAM"]
        let previousTerm = ProcessInfo.processInfo.environment["TERM"]
        let previousItermSession = ProcessInfo.processInfo.environment["ITERM_SESSION_ID"]
        let previousLcTerminal = ProcessInfo.processInfo.environment["LC_TERMINAL"]
        let previousForce = ProcessInfo.processInfo.environment["UIMD_FORCE_SIXEL"]
        let previousDisable = ProcessInfo.processInfo.environment["UIMD_DISABLE_SIXEL"]
        setenv("TERM_PROGRAM", "Apple_Terminal", 1)
        setenv("TERM", "xterm-256color", 1)
        unsetenv("ITERM_SESSION_ID")
        unsetenv("LC_TERMINAL")
        unsetenv("UIMD_FORCE_SIXEL")
        unsetenv("UIMD_DISABLE_SIXEL")
        defer
        {
            restoreEnvironment("TERM_PROGRAM", previousTermProgram)
            restoreEnvironment("TERM", previousTerm)
            restoreEnvironment("ITERM_SESSION_ID", previousItermSession)
            restoreEnvironment("LC_TERMINAL", previousLcTerminal)
            restoreEnvironment("UIMD_FORCE_SIXEL", previousForce)
            restoreEnvironment("UIMD_DISABLE_SIXEL", previousDisable)
        }

        let directory = FileManager.default.temporaryDirectory
        let path = directory.appendingPathComponent("uimd-sixel-fallback.ppm")
        var data = Data("P6\n1 2\n255\n".utf8)
        data.append(contentsOf: [255, 0, 0, 0, 0, 255])
        try data.write(to: path)
        defer
        {
            try? FileManager.default.removeItem(at: path)
        }

        let image = Image(
            "probe",
            source: path.path,
            fit: "stretch",
            renderMode: "sixel",
            align: "left",
            verticalAlign: "top"
        )

        let rows = image.render(size: Size(width: 1, height: 1))

        XCTAssertEqual(rows[0][0].raw, "")
        XCTAssertEqual(rows[0][0].text, "▀")
    }

    func testImageSixelModeUsesRawForItermSession() throws
    {
        let previousTermProgram = ProcessInfo.processInfo.environment["TERM_PROGRAM"]
        let previousTerm = ProcessInfo.processInfo.environment["TERM"]
        let previousItermSession = ProcessInfo.processInfo.environment["ITERM_SESSION_ID"]
        let previousLcTerminal = ProcessInfo.processInfo.environment["LC_TERMINAL"]
        let previousForce = ProcessInfo.processInfo.environment["UIMD_FORCE_SIXEL"]
        let previousDisable = ProcessInfo.processInfo.environment["UIMD_DISABLE_SIXEL"]
        unsetenv("TERM_PROGRAM")
        setenv("TERM", "xterm-256color", 1)
        setenv("ITERM_SESSION_ID", "w0t0p0", 1)
        unsetenv("LC_TERMINAL")
        unsetenv("UIMD_FORCE_SIXEL")
        unsetenv("UIMD_DISABLE_SIXEL")
        defer
        {
            restoreEnvironment("TERM_PROGRAM", previousTermProgram)
            restoreEnvironment("TERM", previousTerm)
            restoreEnvironment("ITERM_SESSION_ID", previousItermSession)
            restoreEnvironment("LC_TERMINAL", previousLcTerminal)
            restoreEnvironment("UIMD_FORCE_SIXEL", previousForce)
            restoreEnvironment("UIMD_DISABLE_SIXEL", previousDisable)
        }

        let directory = FileManager.default.temporaryDirectory
        let path = directory.appendingPathComponent("uimd-sixel-iterm.ppm")
        var data = Data("P6\n1 2\n255\n".utf8)
        data.append(contentsOf: [255, 0, 0, 0, 0, 255])
        try data.write(to: path)
        defer
        {
            try? FileManager.default.removeItem(at: path)
        }

        let image = Image(
            "probe",
            source: path.path,
            fit: "stretch",
            renderMode: "sixel",
            align: "left",
            verticalAlign: "top"
        )

        let rows = image.render(size: Size(width: 1, height: 1))

        XCTAssertFalse(rows[0][0].raw.isEmpty)
    }

    func testImageSixelRenderUsesVisibleClipRegion() throws
    {
        let previousTermProgram = ProcessInfo.processInfo.environment["TERM_PROGRAM"]
        let previousTerm = ProcessInfo.processInfo.environment["TERM"]
        let previousItermSession = ProcessInfo.processInfo.environment["ITERM_SESSION_ID"]
        let previousLcTerminal = ProcessInfo.processInfo.environment["LC_TERMINAL"]
        let previousForce = ProcessInfo.processInfo.environment["UIMD_FORCE_SIXEL"]
        let previousDisable = ProcessInfo.processInfo.environment["UIMD_DISABLE_SIXEL"]
        unsetenv("TERM_PROGRAM")
        setenv("TERM", "xterm-256color", 1)
        setenv("ITERM_SESSION_ID", "w0t0p0", 1)
        unsetenv("LC_TERMINAL")
        unsetenv("UIMD_FORCE_SIXEL")
        unsetenv("UIMD_DISABLE_SIXEL")
        defer
        {
            restoreEnvironment("TERM_PROGRAM", previousTermProgram)
            restoreEnvironment("TERM", previousTerm)
            restoreEnvironment("ITERM_SESSION_ID", previousItermSession)
            restoreEnvironment("LC_TERMINAL", previousLcTerminal)
            restoreEnvironment("UIMD_FORCE_SIXEL", previousForce)
            restoreEnvironment("UIMD_DISABLE_SIXEL", previousDisable)
        }

        let directory = FileManager.default.temporaryDirectory
        let path = directory.appendingPathComponent("uimd-sixel-clip.ppm")
        var data = Data("P6\n1 12\n255\n".utf8)
        for index in 0..<12
        {
            data.append(contentsOf: [UInt8(index * 20), UInt8(255 - index * 10), UInt8(index * 5)])
        }
        try data.write(to: path)
        defer
        {
            try? FileManager.default.removeItem(at: path)
        }

        let image = Image(
            "probe",
            source: path.path,
            fit: "stretch",
            renderMode: "sixel",
            align: "left",
            verticalAlign: "top"
        )

        let topClipped = image.render(
            size: Size(width: 1, height: 6),
            state: ElementRenderState(clipTop: 2, clipBottom: 6)
        )
        for row in 2...5
        {
            XCTAssertEqual(topClipped[row][0].rawHeight, 1)
            XCTAssertFalse(topClipped[row][0].raw.isEmpty)
        }
        XCTAssertTrue(topClipped[0][0].raw.isEmpty)

        let bottomClipped = image.render(
            size: Size(width: 1, height: 6),
            state: ElementRenderState(clipTop: 0, clipBottom: 4)
        )
        for row in 0...3
        {
            XCTAssertEqual(bottomClipped[row][0].rawHeight, 1)
            XCTAssertFalse(bottomClipped[row][0].raw.isEmpty)
        }
        XCTAssertFalse(bottomClipped[4][0].rawSkip)
    }

}

private func restoreEnvironment(_ name: String, _ value: String?)
{
    if let value
    {
        setenv(name, value, 1)
    }
    else
    {
        unsetenv(name)
    }
}

import XCTest
@testable import Uimd

final class UimdRuntimeSmokeTests: XCTestCase
{
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

        XCTAssertEqual(renderedText(), "CD")
        XCTAssertTrue(scroll.scrollToTop(Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "AB")
        XCTAssertTrue(scroll.handleKey("Down", viewport: Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "BC")
        XCTAssertTrue(scroll.handleKey("Up", viewport: Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "AB")
        XCTAssertTrue(scroll.scrollToBottom(Size(width: 2, height: 2)))
        XCTAssertEqual(renderedText(), "CD")
    }
}

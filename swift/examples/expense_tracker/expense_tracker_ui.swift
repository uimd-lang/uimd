// Auto-generated UI code for expense_tracker - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ExpenseTrackerUI: GeneratedWindowBase
{
    public var app_title: Label!
    public var quit_btn: Button!
    public var nav_title: Label!
    public var expenses_btn: Button!
    public var categories_btn: Button!
    public var settings_btn: Button!
    public var main: ViewHost!

    public init()
    {
        super.init("Expense Tracker")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ExpenseTracker", source: "# Expense Tracker\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: experimental\ndescription: \"Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nuses:\n  - expenses_view\n  - categories_view\n  - settings_view\n```\n\n## Members\n\n```yaml\napp_title:\n  type: label\n  text: \"Expense Tracker\"\n\nquit_btn:\n  type: button\n  title: Quit\n\nnav_title:\n  type: label\n  text: Expenses\n\nexpenses_btn:\n  type: button\n  title: Expenses\n\ncategories_btn:\n  type: button\n  title: Categories\n\nsettings_btn:\n  type: button\n  title: Settings\n\nmain:\n  type: viewhost\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@app_title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@nav_title:\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**------------------------------------------+\n| app_title..**.......................... quit_btn... |\n+--nav------------------+--content-**-----------------+\n| nav_title............ | main..**................... |\n|                       | *.......................... |\n| expenses_btn......... | *.......................... |\n|                       | ........................... |\n| categories_btn....... | ........................... |\n|                       | ........................... |\n| settings_btn......... | ........................... |\n*                       * ........................... |\n*                       * ........................... |\n+-----------------------+-----------------------------+\n```\n", description: "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        app_title = addElement(Label("app_title", "Expense Tracker"))
        app_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        app_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        quit_btn = addElement(Button("quit_btn", "Quit"))
        quit_btn.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        quit_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        quit_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        nav_title = addElement(Label("nav_title", "Expenses"))
        nav_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    return style
                }())
        nav_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        expenses_btn = addElement(Button("expenses_btn", "Expenses"))
        expenses_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        expenses_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        expenses_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        categories_btn = addElement(Button("categories_btn", "Categories"))
        categories_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        categories_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        categories_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        settings_btn = addElement(Button("settings_btn", "Settings"))
        settings_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        settings_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        settings_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        main = addElement(ViewHost("main"))
        main.setStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "app_title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 53, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 39, height: 1),
                cellCharsSize: Size(width: 53, height: 1),
                marginRight: 13,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "main",
                type: "viewhost",
                cellName: "content",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 2, col: 24, width: 29, height: 9),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 27, height: 9),
                cellCharsSize: Size(width: 29, height: 9),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "nav_title",
                type: "label",
                cellName: "nav",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 23, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(23),
                cellHeight: .expanded,
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 23, height: 9),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "quit_btn",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 41, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 53, height: 1),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 53, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.padding = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "expenses_btn",
                type: "button",
                cellName: "nav",
                relative: Rect(row: 2, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 23, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(23),
                cellHeight: .expanded,
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 23, height: 9),
                marginRight: 1,
                marginBottom: 6,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "categories_btn",
                type: "button",
                cellName: "nav",
                relative: Rect(row: 4, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 23, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(23),
                cellHeight: .expanded,
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 23, height: 9),
                marginRight: 1,
                marginBottom: 4,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "settings_btn",
                type: "button",
                cellName: "nav",
                relative: Rect(row: 6, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 23, height: 9),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(23),
                cellHeight: .expanded,
                charsSize: Size(width: 21, height: 1),
                cellCharsSize: Size(width: 23, height: 9),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.padding = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onQuitBtnClick() { }
    open func onExpensesBtnClick() { }
    open func onCategoriesBtnClick() { }
    open func onSettingsBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "quit_btn"
        {
            onQuitBtnClick()
            return true
        }
        else if name == "expenses_btn"
        {
            onExpensesBtnClick()
            return true
        }
        else if name == "categories_btn"
        {
            onCategoriesBtnClick()
            return true
        }
        else if name == "settings_btn"
        {
            onSettingsBtnClick()
            return true
        }
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

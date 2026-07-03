// Auto-generated UI code for expenses_list - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class ExpensesListUI: GeneratedScrollViewBase
{

    public init()
    {
        super.init("Expenses List")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "ExpensesList", source: "# Expenses List\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Scrollable list of interactive expense rows.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../expense_row/expense_row\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#172033\"\n@panel:\n  gap: 1\n  padding: 1\n  background: \"#172033\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", description: "Scrollable list of interactive expense rows.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        setGeneratedScrollView(addElement(ScrollView("__scrollview", 1)))
        scrollView().setStyle({
                    var style = Style()
                    style.background = Color("#172033")
                    style.gap = 1
                    style.padding = 1
                    return style
                }())
        scrollView().setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        scrollView().setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        scrollView().setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "__scrollview",
                type: "uiscrollview",
                cellName: "panel",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 54, height: 7),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 54, height: 7),
                cellCharsSize: Size(width: 54, height: 7),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.gap = 1
                    return style
                }(),
                elementStyle: Style()
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

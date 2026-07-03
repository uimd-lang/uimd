// Auto-generated UI code for task_filters - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class TaskFiltersUI: GeneratedWindowBase
{
    public var filters_label: Label!
    public var search_label: Label!
    public var status_label: Label!
    public var owner_label: Label!
    public var search: TextInput!
    public var status_filter: ComboBox!
    public var owner_filter: ComboBox!
    public var apply_filters_btn: Button!
    public var reset_filters_btn: Button!

    public init()
    {
        super.init("Task Filters")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "TaskFilters", source: "# Task Filters\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: draft\ndescription: \"Reusable task board filter panel.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfilters_label:\n  type: label\n  text: FILTERS\n  description: \"Filter panel label.\"\n  expose: false\n\nsearch_label:\n  type: label\n  text: Search\n  description: \"Search filter label.\"\n  expose: false\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Status filter label.\"\n  expose: false\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Assignee filter label.\"\n  expose: false\n\nsearch:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  placeholder: \"Task title or description\"\n  description: \"Filter tasks by title or description.\"\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: \"Filter tasks by status.\"\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: \"Filter tasks by assignee.\"\n\napply_filters_btn:\n  type: button\n  title: \"Apply filter\"\n  description: \"Apply the current task filters.\"\n\nreset_filters_btn:\n  type: button\n  title: Reset\n  description: \"Reset search, status, and assignee filters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  background: \"#15171c\"\n  border-width: 0\nlabel:\n  color: \"#f5f5f5\"\ntextinput:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  cursor-background: \"#93c5fd\"\n  cursor-color: \"#ffffff\"\ncombobox:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  selected-background: \"#2f80ed\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#2f3545\"\n  color: \"#f5f5f5\"\n  focus-background: \"#2f80ed\"\n@filters_label:\n  color: \"#aab2c3\"\n@search_label:\n  color: \"#cfd5e3\"\n@status_label:\n  color: \"#cfd5e3\"\n@owner_label:\n  color: \"#cfd5e3\"\n@apply_filters_btn:\n  background: \"#2f80ed\"\n  focus-background: \"#1d6fd8\"\n@reset_filters_btn:\n  background: \"#343b4b\"\n```\n\n## User Interface\n\n```ui\n+-**---------------------------------------------------------------------------+\n|filters_label..**.............................................................|\n+----------------------------+----------------------+--------------------------+\n|search_label..**........... | status_label..**.... | owner_label..**......... |\n|search..**................. | status_filter.**.... | owner_filter..**........ |\n+-**-------------------------+----------------------+--------------------------+\n|                                                                              |\n|apply_filters_btn.....  reset_filters_btn.....                                |\n|                                                                              |\n+------------------------------------------------------------------------------+\n```\n", description: "Reusable task board filter panel.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#15171c")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        filters_label = addElement(Label("filters_label", "FILTERS"))
        filters_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#aab2c3")
                    return style
                }())
        filters_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        search_label = addElement(Label("search_label", "Search"))
        search_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }())
        search_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        status_label = addElement(Label("status_label", "Status"))
        status_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }())
        status_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        owner_label = addElement(Label("owner_label", "Assignee"))
        owner_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }())
        owner_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        search = addElement(TextInput("search", "", maxLength: 60))
        search.setStyle({
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }())
        search.setFocusStyle({
                    var style = Style()
                    style.background = Color("#303a49")
                    style.color = Color("#ffffff")
                    return style
                }())
        search.setEditStyle({
                    var style = Style()
                    style.background = Color("#344459")
                    style.color = Color("#ffffff")
                    return style
                }())
        search.setCursorStyle({
                    var style = Style()
                    style.background = Color("#93c5fd")
                    style.color = Color("#ffffff")
                    return style
                }())
        search.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        search.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        status_filter = addElement(ComboBox("status_filter", ["Any", "Todo", "Doing", "Blocked", "Done"], selectedIndex: 0))
        status_filter.setStyle({
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }())
        status_filter.setFocusStyle({
                    var style = Style()
                    style.background = Color("#303a49")
                    style.color = Color("#ffffff")
                    return style
                }())
        status_filter.setEditStyle({
                    var style = Style()
                    style.background = Color("#344459")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        status_filter.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2f80ed")
                    style.color = Color("#ffffff")
                    return style
                }())
        status_filter.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        owner_filter = addElement(ComboBox("owner_filter", ["Any", "Ava", "Bruno", "Chen", "Dana"], selectedIndex: 0))
        owner_filter.setStyle({
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }())
        owner_filter.setFocusStyle({
                    var style = Style()
                    style.background = Color("#303a49")
                    style.color = Color("#ffffff")
                    return style
                }())
        owner_filter.setEditStyle({
                    var style = Style()
                    style.background = Color("#344459")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        owner_filter.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2f80ed")
                    style.color = Color("#ffffff")
                    return style
                }())
        owner_filter.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        apply_filters_btn = addElement(Button("apply_filters_btn", "Apply filter"))
        apply_filters_btn.setStyle({
                    var style = Style()
                    style.background = Color("#2f80ed")
                    style.color = Color("#f5f5f5")
                    return style
                }())
        apply_filters_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1d6fd8")
                    style.color = Color("#ffffff")
                    return style
                }())
        apply_filters_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        reset_filters_btn = addElement(Button("reset_filters_btn", "Reset"))
        reset_filters_btn.setStyle({
                    var style = Style()
                    style.background = Color("#343b4b")
                    style.color = Color("#f5f5f5")
                    return style
                }())
        reset_filters_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2f80ed")
                    style.color = Color("#ffffff")
                    return style
                }())
        reset_filters_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "filters_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 78, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#aab2c3")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "search_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .auto(2),
                charsSize: Size(width: 27, height: 1),
                cellCharsSize: Size(width: 28, height: 2),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "owner_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 52, width: 26, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .auto(2),
                charsSize: Size(width: 24, height: 1),
                cellCharsSize: Size(width: 26, height: 2),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status_label",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 29, width: 22, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(2),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 22, height: 2),
                marginRight: 1,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cfd5e3")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "apply_filters_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 1, col: 0, width: 22, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 78, height: 3),
                width: .auto(22),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 22, height: 1),
                cellCharsSize: Size(width: 78, height: 3),
                marginRight: 56,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#2f80ed")
                    style.color = Color("#f5f5f5")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "search",
                type: "textinput",
                cellName: "",
                relative: Rect(row: 1, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 28, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(28),
                cellHeight: .auto(2),
                charsSize: Size(width: 27, height: 1),
                cellCharsSize: Size(width: 28, height: 2),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "owner_filter",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 52, width: 26, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(26),
                cellHeight: .auto(2),
                charsSize: Size(width: 24, height: 1),
                cellCharsSize: Size(width: 26, height: 2),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "status_filter",
                type: "combobox",
                cellName: "",
                relative: Rect(row: 1, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 29, width: 22, height: 2),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(22),
                cellHeight: .auto(2),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 22, height: 2),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#252b35")
                    style.color = Color("#ffffff")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "reset_filters_btn",
                type: "button",
                cellName: "",
                relative: Rect(row: 1, col: 24, width: 22, height: 1),
                sourceCell: Rect(row: 5, col: 0, width: 78, height: 3),
                width: .auto(22),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(3),
                charsSize: Size(width: 22, height: 1),
                cellCharsSize: Size(width: 78, height: 3),
                marginRight: 32,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#343b4b")
                    style.color = Color("#f5f5f5")
                    return style
                }()
            ),
        ]
    }

    open func onSearchChange(_ value: String) { }
    open func onSearchSubmit(_ value: String) { }
    open func onStatusFilterChange(_ value: String) { }
    open func onOwnerFilterChange(_ value: String) { }
    open func onApplyFiltersBtnClick() { }
    open func onResetFiltersBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "apply_filters_btn"
        {
            onApplyFiltersBtnClick()
            return true
        }
        else if name == "reset_filters_btn"
        {
            onResetFiltersBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "search"
        {
            onSearchChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "search"
        {
            onSearchSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "status_filter"
        {
            onStatusFilterChange(value.isEmpty ? "" : value[0])
            return true
        }
        else if name == "owner_filter"
        {
            onOwnerFilterChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}

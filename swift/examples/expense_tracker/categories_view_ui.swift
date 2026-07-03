// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CategoriesViewUI: GeneratedWindowBase
{
    public var view_title: Label!
    public var category_name: TextInput!
    public var add_category_btn: Button!
    public var categories: ListBox!
    public var remove_category_btn: Button!

    public init()
    {
        super.init("Categories View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "CategoriesView", source: "# Categories View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Simple category list management view.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Categories\n\ncategory_name:\n  type: textinput\n  value: \"\"\n  maxlength: 40\n\nadd_category_btn:\n  type: button\n  title: Add\n\ncategories:\n  type: listbox\n  options: [Food, Transport, Home, Health, Other]\n  selected_items: [Food]\n\nremove_category_btn:\n  type: button\n  title: Remove selected\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bbf7d0\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nlistbox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Name:\" category_name..**................................ |\n|                                                            |\n| add_category_btn................ remove_category_btn..... |\n|                                                            |\n| categories..**............................................ |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", description: "Simple category list management view.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        view_title = addElement(Label("view_title", "Categories"))
        view_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bbf7d0")
                    return style
                }())
        view_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        category_name = addElement(TextInput("category_name", "", maxLength: 40))
        category_name.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        category_name.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        category_name.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#ffffff")
                    return style
                }())
        category_name.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        category_name.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        category_name.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        add_category_btn = addElement(Button("add_category_btn", "Add"))
        add_category_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        add_category_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        add_category_btn.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        categories = addElement(ListBox("categories", ["Food", "Transport", "Home", "Health", "Other"], selectedValues: ["Food"]))
        categories.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        categories.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        categories.setEditStyle({
                    var style = Style()
                    style.background = Color("#243a5c")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        categories.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        categories.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        remove_category_btn = addElement(Button("remove_category_btn", "Remove selected"))
        remove_category_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        remove_category_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        remove_category_btn.setDisabledStyle({
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
                name: "view_title",
                type: "label",
                cellName: "view",
                relative: Rect(row: 0, col: 1, width: 58, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 1,
                marginBottom: 10,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#bbf7d0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Name:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 2, col: 1, width: 7, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .auto(7),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 7, height: 1),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 52,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "category_name",
                type: "textinput",
                cellName: "view",
                relative: Rect(row: 2, col: 9, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 49, height: 1),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 2,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "add_category_btn",
                type: "button",
                cellName: "view",
                relative: Rect(row: 4, col: 1, width: 32, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .auto(32),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 32, height: 1),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 27,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "remove_category_btn",
                type: "button",
                cellName: "view",
                relative: Rect(row: 4, col: 34, width: 24, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .auto(24),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 24, height: 1),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 2,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "categories",
                type: "listbox",
                cellName: "view",
                relative: Rect(row: 6, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 11),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .auto(11),
                charsSize: Size(width: 58, height: 5),
                cellCharsSize: Size(width: 60, height: 11),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
        ]
    }

    open func onCategoryNameChange(_ value: String) { }
    open func onCategoryNameSubmit(_ value: String) { }
    open func onAddCategoryBtnClick() { }
    open func onCategoriesSelectionChange(_ value: [String]) { }
    open func onRemoveCategoryBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "add_category_btn"
        {
            onAddCategoryBtnClick()
            return true
        }
        else if name == "remove_category_btn"
        {
            onRemoveCategoryBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "category_name"
        {
            onCategoryNameChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "category_name"
        {
            onCategoryNameSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "categories"
        {
            onCategoriesSelectionChange(value)
            return true
        }
        return false
    }
}

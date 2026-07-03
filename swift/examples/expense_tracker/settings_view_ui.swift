// Auto-generated UI code for settings_view - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class SettingsViewUI: GeneratedWindowBase
{
    public var view_title: Label!
    public var currency: ComboBox!
    public var show_paid: CheckBox!
    public var monthly_limit: NumberInput!
    public var reset_btn: Button!

    public init()
    {
        super.init("Settings View")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("control")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "SettingsView", source: "# Settings View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense tracker settings view.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Settings\n\ncurrency:\n  type: combobox\n  options: [EUR, USD, CZK]\n  selected_item: EUR\n\nshow_paid:\n  type: checkbox\n  title: Show paid expenses\n  value: true\n\nmonthly_limit:\n  type: numberinput\n  value: 500\n  format: \"0\"\n\nreset_btn:\n  type: button\n  title: Reset demo data\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#fde68a\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Currency:\"      currency................................. |\n|                                                            |\n| show_paid................................................. |\n|                                                            |\n| \"Monthly limit:\" monthly_limit............................ |\n|                                                            |\n| reset_btn................................................. |\n+------------------------------------------------------------+\n```\n", description: "Expense tracker settings view.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        view_title = addElement(Label("view_title", "Settings"))
        view_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fde68a")
                    return style
                }())
        view_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        currency = addElement(ComboBox("currency", ["EUR", "USD", "CZK"], selectedIndex: 0))
        currency.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        currency.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        currency.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        currency.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        currency.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        show_paid = addElement(CheckBox("show_paid", "Show paid expenses", true))
        show_paid.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }())
        show_paid.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        show_paid.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#44cc88")
                    return style
                }())
        show_paid.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    return style
                }())
        show_paid.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        monthly_limit = addElement(NumberInput("monthly_limit", 500.0, step: 1.0))
        monthly_limit.setStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        monthly_limit.setFocusStyle({
                    var style = Style()
                    style.background = Color("#25364f")
                    style.color = Color("#ffffff")
                    return style
                }())
        monthly_limit.setEditStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#ffffff")
                    return style
                }())
        monthly_limit.setCursorStyle({
                    var style = Style()
                    style.background = Color("#facc15")
                    style.color = Color("#111827")
                    return style
                }())
        monthly_limit.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        reset_btn = addElement(Button("reset_btn", "Reset demo data"))
        reset_btn.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        reset_btn.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        reset_btn.setDisabledStyle({
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
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 1,
                marginBottom: 8,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#fde68a")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Currency:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 2, col: 1, width: 11, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(11),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 11, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 48,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "currency",
                type: "combobox",
                cellName: "view",
                relative: Rect(row: 2, col: 18, width: 41, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(41),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 41, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 1,
                marginBottom: 6,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "show_paid",
                type: "checkbox",
                cellName: "view",
                relative: Rect(row: 4, col: 1, width: 58, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 1,
                marginBottom: 4,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#b8c4d8")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "\"Monthly limit:\"",
                type: "label",
                cellName: "view",
                relative: Rect(row: 6, col: 1, width: 16, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(16),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 16, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 43,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "monthly_limit",
                type: "numberinput",
                cellName: "view",
                relative: Rect(row: 6, col: 18, width: 41, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(41),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 41, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 1,
                marginBottom: 2,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "reset_btn",
                type: "button",
                cellName: "view",
                relative: Rect(row: 8, col: 1, width: 58, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 60, height: 9),
                width: .auto(58),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(9),
                charsSize: Size(width: 58, height: 1),
                cellCharsSize: Size(width: 60, height: 9),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onCurrencyChange(_ value: String) { }
    open func onShowPaidChange(_ value: String) { }
    open func onMonthlyLimitChange(_ value: String) { }
    open func onMonthlyLimitSubmit(_ value: String) { }
    open func onResetBtnClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "reset_btn"
        {
            onResetBtnClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "show_paid"
        {
            onShowPaidChange(value)
            return true
        }
        else if name == "monthly_limit"
        {
            onMonthlyLimitChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        if name == "monthly_limit"
        {
            onMonthlyLimitSubmit(value)
            return true
        }
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "currency"
        {
            onCurrencyChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}

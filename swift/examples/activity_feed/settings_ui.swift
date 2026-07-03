// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class SettingsUI: GeneratedWindowBase
{
    public var header_title: Label!
    public var auto_scroll: CheckBox!
    public var show_timestamps: CheckBox!
    public var default_type_label: Label!
    public var default_type: ComboBox!
    public var save_button: Button!
    public var cancel_button: Button!

    public init()
    {
        super.init("Activity Feed Settings")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "Settings", source: "# Activity Feed Settings\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Settings dialog for activity feed auto-scroll, timestamps, and default activity type.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed Settings\"\n  description: \"Dialog title.\"\n  expose: false\n\nauto_scroll:\n  type: checkbox\n  title: \"Auto-scroll to newest\"\n  value: 1\n  description: \"Toggle whether new activity items scroll into view.\"\n\nshow_timestamps:\n  type: checkbox\n  title: \"Show timestamps\"\n  value: 1\n  description: \"Toggle whether newly added activity items include timestamps.\"\n\ndefault_type_label:\n  type: label\n  text: \"Default type\"\n  description: \"Label for the default activity type selector.\"\n  expose: false\n\ndefault_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Default activity type used by the main window.\"\n\nsave_button:\n  type: button\n  title: Save\n  description: \"Apply settings and close the dialog.\"\n\ncancel_button:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without applying settings.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@buttons:\n  padding: 0, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-header-----------------------------------+\n|header_title..**..........................|\n+-content----------------------------------+\n|                                          |\n|  auto_scroll...........................  |\n|                                          |\n|  show_timestamps.......................  |\n|                                          |\n|  default_type_label..  default_type....  |\n|                                          |\n+-buttons----------------------------------+\n|  save_button......  cancel_button......  |\n+------------------------------------------+\n```\n", description: "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#162033")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        header_title = addElement(Label("header_title", "Activity Feed Settings"))
        header_title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#ffffff")
                    style.textAlign = "center"
                    return style
                }())
        header_title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        auto_scroll = addElement(CheckBox("auto_scroll", "Auto-scroll to newest", true))
        auto_scroll.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        auto_scroll.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        auto_scroll.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        auto_scroll.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f97316")
                    return style
                }())
        auto_scroll.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        show_timestamps = addElement(CheckBox("show_timestamps", "Show timestamps", true))
        show_timestamps.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }())
        show_timestamps.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff44")
                    style.color = Color("#ffffff")
                    return style
                }())
        show_timestamps.setCheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#22c55e")
                    return style
                }())
        show_timestamps.setUncheckedStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f97316")
                    return style
                }())
        show_timestamps.setDisabledStyle({
                    var style = Style()
                    style.color = Color("#64748b")
                    return style
                }())
        default_type_label = addElement(Label("default_type_label", "Default type"))
        default_type_label.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        default_type_label.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        default_type = addElement(ComboBox("default_type", ["Info", "Task", "Warning", "Deploy", "Note"], selectedIndex: 0))
        default_type.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        default_type.setFocusStyle({
                    var style = Style()
                    style.background = Color("#1e293b")
                    style.color = Color("#ffffff")
                    return style
                }())
        default_type.setEditStyle({
                    var style = Style()
                    style.background = Color("#111827")
                    style.color = Color("#e5e7eb")
                    return style
                }())
        default_type.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        default_type.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#0b1220")
                    style.color = Color("#64748b")
                    return style
                }())
        save_button = addElement(Button("save_button", "Save"))
        save_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        save_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        save_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        cancel_button = addElement(Button("cancel_button", "Cancel"))
        cancel_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        cancel_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        cancel_button.setDisabledStyle({
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
                name: "header_title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 0, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 42, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(1),
                charsSize: Size(width: 42, height: 1),
                cellCharsSize: Size(width: 42, height: 1),
                marginRight: 0,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#1f2937")
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
                name: "save_button",
                type: "button",
                cellName: "buttons",
                relative: Rect(row: 0, col: 2, width: 17, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 42, height: 1),
                width: .auto(17),
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(1),
                charsSize: Size(width: 17, height: 1),
                cellCharsSize: Size(width: 42, height: 1),
                marginRight: 23,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 2
                    style.paddingRight = 2
                    style.paddingTop = 0
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
                name: "cancel_button",
                type: "button",
                cellName: "buttons",
                relative: Rect(row: 0, col: 21, width: 536870911, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 42, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(1),
                charsSize: Size(width: 19, height: 1),
                cellCharsSize: Size(width: 42, height: 1),
                marginRight: 2,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.paddingBottom = 1
                    style.paddingLeft = 2
                    style.paddingRight = 2
                    style.paddingTop = 0
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
                name: "auto_scroll",
                type: "checkbox",
                cellName: "content",
                relative: Rect(row: 1, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 42, height: 7),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(7),
                charsSize: Size(width: 38, height: 1),
                cellCharsSize: Size(width: 42, height: 7),
                marginRight: 2,
                marginBottom: 5,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "show_timestamps",
                type: "checkbox",
                cellName: "content",
                relative: Rect(row: 3, col: 2, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 42, height: 7),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(7),
                charsSize: Size(width: 38, height: 1),
                cellCharsSize: Size(width: 42, height: 7),
                marginRight: 2,
                marginBottom: 3,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#d1d5db")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "default_type_label",
                type: "label",
                cellName: "content",
                relative: Rect(row: 5, col: 2, width: 20, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 42, height: 7),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(7),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 42, height: 7),
                marginRight: 20,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#cbd5e1")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "default_type",
                type: "combobox",
                cellName: "content",
                relative: Rect(row: 5, col: 24, width: 536870911, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 42, height: 7),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(42),
                cellHeight: .auto(7),
                charsSize: Size(width: 16, height: 1),
                cellCharsSize: Size(width: 42, height: 7),
                marginRight: 2,
                marginBottom: 1,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#e5e7eb")
                    return style
                }()
            ),
        ]
    }

    open func onAutoScrollChange(_ value: String) { }
    open func onShowTimestampsChange(_ value: String) { }
    open func onDefaultTypeChange(_ value: String) { }
    open func onSaveButtonClick() { }
    open func onCancelButtonClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "save_button"
        {
            onSaveButtonClick()
            return true
        }
        else if name == "cancel_button"
        {
            onCancelButtonClick()
            return true
        }
        return false
    }


    open override func handleGeneratedTextChanged(_ name: String, value: String) -> Bool
    {
        if name == "auto_scroll"
        {
            onAutoScrollChange(value)
            return true
        }
        else if name == "show_timestamps"
        {
            onShowTimestampsChange(value)
            return true
        }
        return false
    }


    open override func handleGeneratedTextConfirmed(_ name: String, value: String) -> Bool
    {
        return false
    }


    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool
    {
        if name == "default_type"
        {
            onDefaultTypeChange(value.isEmpty ? "" : value[0])
            return true
        }
        return false
    }
}

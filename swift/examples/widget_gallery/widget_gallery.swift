import Foundation
import Uimd

private let kExpectedLayoutEntries = 18
private let kResetCount = 3
private let kDefaultName = "Ada Lovelace"

private func selectedComboValue(_ comboBox: ComboBox) -> String
{
    guard comboBox.selectedIndex >= 0 && comboBox.selectedIndex < comboBox.options.count else
    {
        return ""
    }
    return comboBox.options[comboBox.selectedIndex]
}

private func summaryText(_ window: WidgetGalleryUI) -> String
{
    let theme = selectedComboValue(window.theme_combo)
    let mode = selectedComboValue(window.mode_list)
    return "values:\n  name: \(window.name_input.value)\n  count: \(window.count_input.value)\n  theme: \(theme)\n  mode: \(mode)\n  enabled: \(window.enabled_check.checked ? "True" : "False")"
}

private func refreshSummary(_ window: WidgetGalleryUI)
{
    window.spans.setSpans([
        LabelSpan(text: "SpanLabel", foreground: Color("#facc15")),
        LabelSpan(text: " can mix "),
        LabelSpan(text: "foreground", foreground: Color("#38bdf8")),
        LabelSpan(text: " and "),
        LabelSpan(text: "background", foreground: Color("#111827"), background: Color("#f97316")),
        LabelSpan(text: " colors."),
    ])
    window.summary.setText(summaryText(window))
}

private func resetValues(_ window: WidgetGalleryUI)
{
    window.name_input.setValue(kDefaultName)
    window.count_input.setValue(Double(kResetCount))
    window.enabled_check.setChecked(true)
    window.theme_combo.selectedIndex = 0
    window.mode_list.selectedIndex = 0
    window.mode_list.setSelectedValues([selectedComboValue(window.mode_list)])
    refreshSummary(window)
}

private final class WidgetGalleryApp: WidgetGalleryUI
{
    private var finished = false

    override init()
    {
        super.init()
        refreshSummary(self)
    }

    override func onResetBtnClick()
    {
        resetValues(self)
    }

    override func onApplyBtnClick()
    {
        refreshSummary(self)
    }

    override func onCloseBtnClick()
    {
        finished = true
    }

    override func onNameInputChange(_ value: String)
    {
        _ = value
    }

    override func onCountInputChange(_ value: String)
    {
        _ = value
    }

    override func onEnabledCheckChange(_ value: String)
    {
        _ = value
    }

    override func onThemeComboChange(_ value: String)
    {
        _ = value
    }

    override func onModeListSelectionChange(_ value: [String])
    {
        _ = value
    }

    override func shouldClose() -> Bool
    {
        finished
    }
}

private func smoke()
{
    let window = WidgetGalleryUI()
    precondition(window.windowTitle == "Widget Gallery")
    precondition(window.layout.count == kExpectedLayoutEntries)
    precondition(window.header != nil)
    precondition(window.name_input != nil)
    precondition(window.count_input != nil)
    precondition(window.enabled_check != nil)
    precondition(window.theme_combo != nil)
    precondition(window.mode_list != nil)
    precondition(window.summary != nil)
    refreshSummary(window)
    precondition(window.name_input.value == kDefaultName)
    precondition(window.enabled_check.checked)
    precondition(window.theme_combo.selectedIndex == 0)
    precondition(window.mode_list.selectedIndex == 0)
}

private func logicTest()
{
    let window = WidgetGalleryUI()
    window.name_input.setValue("Grace Hopper")
    window.count_input.setValue(7)
    window.enabled_check.setChecked(false)
    window.theme_combo.selectedIndex = 1
    window.mode_list.selectedIndex = 3
    window.mode_list.setSelectedValues([selectedComboValue(window.mode_list)])
    refreshSummary(window)
    precondition(window.summary.text == "values:\n  name: Grace Hopper\n  count: 7\n  theme: Light\n  mode: Publish\n  enabled: False")

    resetValues(window)
    precondition(window.spans.text == "SpanLabel can mix foreground and background colors.")
    precondition(window.summary.text == "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: True")
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
            let app = WidgetGalleryApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

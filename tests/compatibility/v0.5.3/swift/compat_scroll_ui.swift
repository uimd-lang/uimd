// Auto-generated UI code for compat_scroll - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class CompatScrollUI: GeneratedScrollViewBase
{
    public var alpha_row: Label!

    public init()
    {
        super.init("UIMD 0.5.3 Compatibility Scroll")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("component")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "CompatScroll", source: "# UIMD 0.5.3 Compatibility Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\n```\n\n## Members\n\n```yaml\nalpha_row:\n  type: label\n  text: \"legacy alpha row\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@panel:\n  background: \"#303545\"\n  padding: 0\n@alpha_row:\n  background: \"#252a36cc\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------+\n|alpha_row.....................|\n|..............................|\n+------------------------------+\n```\n", description: "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#303545")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        setGeneratedScrollView(addElement(ScrollView("__scrollview", 0)))
        scrollView().setStyle({
                    var style = Style()
                    style.background = Color("#303545")
                    style.padding = 0
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
        alpha_row = addElement(Label("alpha_row", "legacy alpha row"))
        alpha_row.setStyle({
                    var style = Style()
                    style.background = Color("#252a36cc")
                    style.color = Color("#cbd5e1")
                    return style
                }())
        alpha_row.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
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

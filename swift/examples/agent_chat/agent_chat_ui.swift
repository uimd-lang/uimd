// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
import Foundation
import Uimd

open class AgentChatUI: GeneratedWindowBase
{
    public var title: Label!
    public var simulation_badge: Label!
    public var quit_button: Button!
    public var start_button: Button!
    public var pause_button: Button!
    public var next_step_button: Button!
    public var reset_button: Button!
    public var load_stress_button: Button!
    public var transcript: ReusableElement!
    public var status: Label!

    public init()
    {
        super.init("Agent Chat Simulation")
        setGeneratedLayout(Self.buildLayout())
        setGeneratedKind("window")
        setGeneratedFocusable(false)
        setMcpMetadata(enabled: true, className: "AgentChat", source: "# Agent Chat Simulation\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Deterministic simulated agent chat for rich transcript and large ScrollView testing.\"\ntags: [example, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - transcript/transcript\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Agent Chat Simulation\"\n  description: \"Application title.\"\n  expose: false\n\nsimulation_badge:\n  type: label\n  text: \"SIMULATED · OFFLINE\"\n  description: \"Clarifies that the transcript performs no real agent or tool work.\"\n  expose: false\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the Agent Chat Simulation example.\"\n\nstart_button:\n  type: button\n  title: Start\n  description: \"Run one hundred deterministic simulated messages over three minutes.\"\n\npause_button:\n  type: button\n  title: Pause\n  description: \"Pause the timed simulation without discarding progress.\"\n\nnext_step_button:\n  type: button\n  title: \"Next step\"\n  description: \"Apply one deterministic scripted chat step.\"\n\nreset_button:\n  type: button\n  title: Reset\n  description: \"Reset the transcript to its initial message.\"\n\nload_stress_button:\n  type: button\n  title: \"Load 50k\"\n  description: \"Load an approximately fifty-thousand-row deterministic stress transcript.\"\n\ntranscript:\n  type: transcript\n  cpp-class: \"agent_chat_example::Transcript\"\n  cpp-header: transcript/transcript.hpp\n  description: \"Scrollable deterministic simulated agent transcript.\"\n\nstatus:\n  type: label\n  text: \"Ready · 1 message\"\n  description: \"Shows deterministic simulation progress and transcript size.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\n@simulation_badge:\n  color: \"#c4b5fd\"\n  text-align: right\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@load_stress_button:\n  background: \"#6d28d9\"\n  focus-background: \"#7c3aed\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@content:\n  background: \"#030712\"\n@transcript:\n  background: \"#00000000\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n  padding: 0, 1, 0, 1\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**......................... simulation_badge.... quit_button......... |\n+-actions-**-------------------------------------------------------------------+\n| start_button pause_button next_step_button reset_button load_stress_button.. |\n+-content-**-------------------------------------------------------------------+\n| transcript..**.............................................................. |\n| ............................................................................ |\n* *........................................................................... |\n* *........................................................................... |\n| ............................................................................ |\n+------------------------------------------------------------------------------+\n| status..**.................................................................. |\n+------------------------------------------------------------------------------+\n```\n", description: "Deterministic simulated agent chat for rich transcript and large ScrollView testing.")
        setMcpAppTools([])
        setGeneratedWindowStyle({
                    var style = Style()
                    style.background = Color("#030712")
                    style.borderColor = Color("transparent")
                    style.borderWidthHorizontal = 0
                    style.borderWidthVertical = 0
                    return style
                }())
        title = addElement(Label("title", "Agent Chat Simulation"))
        title.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.textAlign = "center"
                    return style
                }())
        title.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        simulation_badge = addElement(Label("simulation_badge", "SIMULATED · OFFLINE"))
        simulation_badge.setStyle({
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#c4b5fd")
                    style.textAlign = "right"
                    return style
                }())
        simulation_badge.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
        quit_button = addElement(Button("quit_button", "Quit"))
        quit_button.setStyle({
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        quit_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#dc2626")
                    style.color = Color("#ffffff")
                    return style
                }())
        quit_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        start_button = addElement(Button("start_button", "Start"))
        start_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        start_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        start_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        pause_button = addElement(Button("pause_button", "Pause"))
        pause_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        pause_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        pause_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        next_step_button = addElement(Button("next_step_button", "Next step"))
        next_step_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        next_step_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        next_step_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        reset_button = addElement(Button("reset_button", "Reset"))
        reset_button.setStyle({
                    var style = Style()
                    style.background = Color("#334155")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        reset_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#2563eb")
                    style.color = Color("#ffffff")
                    return style
                }())
        reset_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        load_stress_button = addElement(Button("load_stress_button", "Load 50k"))
        load_stress_button.setStyle({
                    var style = Style()
                    style.background = Color("#6d28d9")
                    style.color = Color("#e2e8f0")
                    return style
                }())
        load_stress_button.setFocusStyle({
                    var style = Style()
                    style.background = Color("#7c3aed")
                    style.color = Color("#ffffff")
                    return style
                }())
        load_stress_button.setDisabledStyle({
                    var style = Style()
                    style.background = Color("#1f2937")
                    style.color = Color("#64748b")
                    return style
                }())
        transcript = addElement(ReusableElement("transcript"))
        transcript.setStyle({
                    var style = Style()
                    style.background = Color("#00000000")
                    return style
                }())
        transcript.setFocusStyle({
                    var style = Style()
                    style.background = Color("#ffffff14")
                    return style
                }())
        transcript.setEditStyle({
                    var style = Style()
                    style.background = Color("#ffffff18")
                    return style
                }())
        transcript.setSelectedStyle({
                    var style = Style()
                    style.background = Color("#ffffff26")
                    style.color = Color("#ffffff")
                    return style
                }())
        status = addElement(Label("status", "Ready · 1 message"))
        status.setStyle({
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#93c5fd")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }())
        status.setCursorStyle({
                    var style = Style()
                    style.background = Color("#b8d7ff")
                    style.color = Color("#000000")
                    return style
                }())
    }

    private static func buildLayout() -> [GeneratedLayoutEntry]
    {
        return [
            GeneratedLayoutEntry(
                name: "start_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 1, width: 12, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 1),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 65,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
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
                name: "status",
                type: "label",
                cellName: "",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 10, col: 0, width: 78, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .auto(78),
                cellHeight: .auto(1),
                charsSize: Size(width: 76, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: Style(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#0f172a")
                    style.color = Color("#93c5fd")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "title",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .expanded,
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 34, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 43,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#f8fafc")
                    style.textAlign = "center"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "transcript",
                type: "transcript",
                cellName: "content",
                relative: Rect(row: 0, col: 1, width: 536870911, height: 536870911),
                sourceCell: Rect(row: 4, col: 0, width: 78, height: 5),
                width: .expanded,
                height: .expanded,
                cellWidth: .expanded,
                cellHeight: .expanded,
                charsSize: Size(width: 76, height: 5),
                cellCharsSize: Size(width: 78, height: 5),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#030712")
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#00000000")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "pause_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 14, width: 12, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 1),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 52,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
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
                name: "next_step_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 27, width: 16, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 1),
                width: .auto(16),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 16, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 35,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
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
                name: "simulation_badge",
                type: "label",
                cellName: "header",
                relative: Rect(row: 0, col: 36, width: 20, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 22,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("transparent")
                    style.color = Color("#c4b5fd")
                    style.textAlign = "right"
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "reset_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 44, width: 12, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 1),
                width: .auto(12),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 12, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 22,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
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
                name: "load_stress_button",
                type: "button",
                cellName: "actions",
                relative: Rect(row: 0, col: 57, width: 20, height: 1),
                sourceCell: Rect(row: 2, col: 0, width: 78, height: 1),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#172033")
                    style.paddingBottom = 1
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 1
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#6d28d9")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
            GeneratedLayoutEntry(
                name: "quit_button",
                type: "button",
                cellName: "header",
                relative: Rect(row: 0, col: 57, width: 20, height: 1),
                sourceCell: Rect(row: 0, col: 0, width: 78, height: 1),
                width: .auto(20),
                height: .auto(1),
                cellWidth: .expanded,
                cellHeight: .auto(1),
                charsSize: Size(width: 20, height: 1),
                cellCharsSize: Size(width: 78, height: 1),
                marginRight: 1,
                marginBottom: 0,
                cellStyle: {
                    var style = Style()
                    style.background = Color("#111827")
                    style.paddingBottom = 0
                    style.paddingLeft = 1
                    style.paddingRight = 1
                    style.paddingTop = 0
                    return style
                }(),
                elementStyle: {
                    var style = Style()
                    style.background = Color("#7f1d1d")
                    style.color = Color("#e2e8f0")
                    return style
                }()
            ),
        ]
    }

    open func onQuitButtonClick() { }
    open func onStartButtonClick() { }
    open func onPauseButtonClick() { }
    open func onNextStepButtonClick() { }
    open func onResetButtonClick() { }
    open func onLoadStressButtonClick() { }

    open override func handleGeneratedButton(_ name: String) -> Bool
    {
        if name == "quit_button"
        {
            onQuitButtonClick()
            return true
        }
        else if name == "start_button"
        {
            onStartButtonClick()
            return true
        }
        else if name == "pause_button"
        {
            onPauseButtonClick()
            return true
        }
        else if name == "next_step_button"
        {
            onNextStepButtonClick()
            return true
        }
        else if name == "reset_button"
        {
            onResetButtonClick()
            return true
        }
        else if name == "load_stress_button"
        {
            onLoadStressButtonClick()
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

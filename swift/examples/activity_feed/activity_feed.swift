import Foundation
import Uimd

private let kDefaultActivityType = "Info"
private let kTimestampEnv = "UIMD_ACTIVITY_FEED_TIMESTAMP"
private let kInitialSampleCount = 2
private let kHeaderLayoutTestWidth = 112
private let kHeaderLayoutTestHeight = 38

private struct SampleActivity
{
    let type: String
    let message: String
}

private let kSampleActivities: [SampleActivity] = [
    SampleActivity(type: "Info", message: "Workspace opened"),
    SampleActivity(type: "Task", message: "Report export queued"),
    SampleActivity(type: "Deploy", message: "Version 1.4 deployed"),
    SampleActivity(type: "Warning", message: "Review queue is almost full"),
    SampleActivity(type: "Note", message: "Follow-up scheduled"),
    SampleActivity(type: "Task", message: "Background sync finished"),
]

private struct ActivityItemData
{
    var timestamp: String
    var eventType: String
    var message: String
    var showTimestamp = true
}

private struct ConstrainedPadding
{
    var top = 0
    var right = 0
    var bottom = 0
    var left = 0
}

private func trim(_ text: String) -> String
{
    text.trimmingCharacters(in: .whitespacesAndNewlines)
}

private func currentTimestamp() -> String
{
    if let fixed = ProcessInfo.processInfo.environment[kTimestampEnv], !fixed.isEmpty
    {
        return fixed
    }
    let formatter = DateFormatter()
    formatter.dateFormat = "HH:mm"
    return formatter.string(from: Date())
}

private func selectedComboValue(_ comboBox: ComboBox, fallback: String) -> String
{
    guard comboBox.selectedIndex >= 0 && comboBox.selectedIndex < comboBox.options.count else
    {
        return fallback
    }
    return comboBox.options[comboBox.selectedIndex]
}

private func selectComboValue(_ comboBox: ComboBox, _ value: String)
{
    if let index = comboBox.options.firstIndex(of: value)
    {
        comboBox.selectedIndex = index
    }
    else
    {
        comboBox.selectedIndex = comboBox.options.isEmpty ? -1 : 0
    }
}

private func renderedText(_ rendered: [[TerminalCell]]) -> String
{
    rendered.map { row in row.map(\.text).joined() }.joined(separator: "\n")
}

private func splitLines(_ text: String) -> [String]
{
    let parts = text.split(separator: "\n", omittingEmptySubsequences: false).map(String.init)
    return parts.isEmpty ? [""] : parts
}

private func wrappedLineCount(_ text: String, width: Int) -> Int
{
    let safeWidth = max(1, width)
    return max(1, splitLines(text).reduce(0) { count, line in
        count + max(1, (line.count + safeWidth - 1) / safeWidth)
    })
}

private func generatedSourceHeight(_ view: GeneratedWindowBase) -> Int
{
    max(1, view.layout.reduce(0) { height, entry in
        max(height, entry.sourceCell.row + entry.sourceCell.height)
    })
}

private func compactContentHeight(_ view: GeneratedWindowBase, messageRows: Int) -> Int
{
    max(1, view.layout.reduce(1) { height, entry in
        if entry.name.isEmpty
        {
            return height
        }
        let entryHeight = entry.name == "message"
            ? max(1, messageRows)
            : max(1, entry.charsSize.height)
        return max(height, entry.relative.row + entryHeight)
    })
}

private func markerColor(_ eventType: String) -> Color
{
    switch eventType
    {
    case "Task":
        return Color("#a78bfa")
    case "Warning":
        return Color("#facc15")
    case "Deploy":
        return Color("#22c55e")
    case "Note":
        return Color("#fb7185")
    default:
        return Color("#38bdf8")
    }
}

private func applyLeftMarker(_ rendered: inout [[TerminalCell]], color: Color)
{
    for row in rendered.indices
    {
        guard !rendered[row].isEmpty else
        {
            continue
        }
        rendered[row][0].text = "▐"
        rendered[row][0].foreground = color
    }
}

private func renderActivityItem(_ item: ActivityItemData, width: Int) -> [[TerminalCell]]
{
    let view = ActivityItemUI()
    view.timestamp.setText(item.showTimestamp ? item.timestamp : "")
    view.event_type.setText(item.eventType)
    view.message.setText(item.message)

    _ = view.renderContent(
        size: Size(width: max(1, width), height: generatedSourceHeight(view)),
        focusedName: nil,
        editMode: false
    )
    let messageRows = wrappedLineCount(item.message, width: max(1, view.message.frame.width))
    var rendered = view.renderContent(
        size: Size(width: max(1, width), height: compactContentHeight(view, messageRows: messageRows)),
        focusedName: nil,
        editMode: false
    )
    applyLeftMarker(&rendered, color: markerColor(item.eventType))
    return rendered
}

private final class ActivityFeedPanel: ActivityFeedPanelUI
{
    private var activities: [ActivityItemData] = []
    private var autoScrollEnabled = false

    override init()
    {
        super.init()
        setDynamicChildrenRenderer
        {
            [weak self] width in
            self?.renderActivities(width: width) ?? []
        }
    }

    func clearActivities()
    {
        activities.removeAll()
        clearChildren()
        invalidateDynamicChildren()
    }

    func appendActivity(timestamp: String, eventType: String, message: String, showTimestamp: Bool)
    {
        activities.append(ActivityItemData(
            timestamp: timestamp,
            eventType: eventType,
            message: message,
            showTimestamp: showTimestamp
        ))
        invalidateDynamicChildren()
        if autoScrollEnabled
        {
            scrollToBottom()
        }
    }

    override func setAutoScroll(_ enabled: Bool)
    {
        autoScrollEnabled = enabled
        scrollView().setAutoScroll(enabled)
        if autoScrollEnabled
        {
            scrollToBottom()
        }
    }

    func activityCount() -> Int
    {
        activities.count
    }

    private func renderActivities(width: Int) -> [[[TerminalCell]]]
    {
        activities.map { renderActivityItem($0, width: max(1, width)) }
    }
}

private struct SettingsResult
{
    var autoScroll = true
    var showTimestamps = true
    var defaultType = kDefaultActivityType
}

private final class SettingsDialog: SettingsUI
{
    var onCancel: (() -> Void)?
    private(set) var isOpen = false

    func configure(_ settings: SettingsResult)
    {
        auto_scroll.setChecked(settings.autoScroll)
        show_timestamps.setChecked(settings.showTimestamps)
        selectDefaultType(settings.defaultType)
    }

    func open()
    {
        isOpen = true
    }

    func close()
    {
        isOpen = false
    }

    func result() -> SettingsResult
    {
        SettingsResult(
            autoScroll: auto_scroll.checked,
            showTimestamps: show_timestamps.checked,
            defaultType: selectedDefaultType()
        )
    }

    func selectDefaultType(_ value: String)
    {
        selectComboValue(default_type, value)
    }

    func selectedDefaultType() -> String
    {
        selectedComboValue(default_type, fallback: kDefaultActivityType)
    }

    override func onPreviewKey(_ event: KeyEvent) -> Bool
    {
        if event.key == "Escape" && !event.editMode
        {
            onCancel?()
            return true
        }
        return false
    }
}

private final class ActivityFeedApp: ActivityFeedUI
{
    private let panel = ActivityFeedPanel()
    private let settingsDialog = SettingsDialog()
    private let modalStack = GeneratedWindowStack()
    private var autoScroll = true
    private var showTimestamps = true
    private var quitRequested = false
    private var defaultType = kDefaultActivityType

    override init()
    {
        super.init()
        feed.setChild(panel)
    }

    func open()
    {
        selectComboValue(activity_type, defaultType)
        _ = renderContent(size: generatedContentSize(), focusedName: nil, editMode: false)
        panel.setAutoScroll(autoScroll)
        if panel.activityCount() == 0
        {
            seedInitial()
        }
    }

    func button(_ name: String)
    {
        if name == "add_button"
        {
            addCurrentActivity()
        }
        else if name == "seed_button"
        {
            seedActivities()
        }
        else if name == "clear_button"
        {
            clearActivities()
        }
        else if name == "settings_button"
        {
            openSettings()
        }
        else if name == "quit_button"
        {
            quitRequested = true
        }
    }

    func addCurrentActivity()
    {
        let activityMessage = trim(message.value)
        if activityMessage.isEmpty
        {
            status.setText("Enter an activity message.")
            return
        }
        appendActivity(eventType: selectedComboValue(activity_type, fallback: defaultType), messageText: activityMessage)
        message.setValue("")
        message.cursor = 0
        updateCountStatus()
    }

    func appendActivity(eventType: String, messageText: String)
    {
        panel.appendActivity(
            timestamp: currentTimestamp(),
            eventType: eventType,
            message: messageText,
            showTimestamp: showTimestamps
        )
    }

    func seedActivities()
    {
        for activity in kSampleActivities
        {
            appendActivity(eventType: activity.type, messageText: activity.message)
        }
        updateCountStatus()
    }

    func clearActivities()
    {
        panel.clearActivities()
        status.setText("Feed cleared")
    }

    func activityCount() -> Int
    {
        panel.activityCount()
    }

    func hasModalOverlay() -> Bool
    {
        settingsDialog.isOpen
    }

    override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        return options
    }

    override func onAddButtonClick()
    {
        addCurrentActivity()
    }

    override func onSeedButtonClick()
    {
        seedActivities()
    }

    override func onClearButtonClick()
    {
        clearActivities()
    }

    override func onSettingsButtonClick()
    {
        openSettings()
    }

    override func onQuitButtonClick()
    {
        quitRequested = true
    }

    override func onMessageSubmit(_ value: String)
    {
        message.setValue(value)
    }

    override func shouldClose() -> Bool
    {
        quitRequested
    }

    private func openSettings()
    {
        settingsDialog.configure(SettingsResult(
            autoScroll: autoScroll,
            showTimestamps: showTimestamps,
            defaultType: defaultType
        ))
        settingsDialog.open()
        settingsDialog.onCancel = { [weak self] in
            guard let self else
            {
                return
            }
            status.setText("Settings canceled")
            closeSettings()
        }
        var frame = GeneratedWindowFrameOptions()
        frame.onButton = { [weak self] name in
            guard let self else
            {
                return false
            }
            if name == "save_button"
            {
                applySettings(settingsDialog.result())
            }
            else
            {
                status.setText("Settings canceled")
            }
            closeSettings()
            return true
        }
        modalStack.push(settingsDialog, frame)
    }

    private func closeSettings()
    {
        modalStack.remove(settingsDialog)
        settingsDialog.close()
    }

    private func applySettings(_ settings: SettingsResult)
    {
        autoScroll = settings.autoScroll
        showTimestamps = settings.showTimestamps
        defaultType = settings.defaultType.isEmpty ? kDefaultActivityType : settings.defaultType
        selectComboValue(activity_type, defaultType)
        panel.setAutoScroll(autoScroll)
        status.setText("Settings saved")
    }

    private func seedInitial()
    {
        let count = min(kInitialSampleCount, kSampleActivities.count)
        for index in 0..<count
        {
            appendActivity(eventType: kSampleActivities[index].type, messageText: kSampleActivities[index].message)
        }
        updateCountStatus()
    }

    private func updateCountStatus()
    {
        status.setText("\(panel.activityCount()) activities")
    }
}

private func smoke()
{
    let window = ActivityFeedUI()
    precondition(window.windowTitle == "Activity Feed")
    precondition(window.header_title != nil)
    precondition(window.settings_button != nil)
    precondition(window.quit_button != nil)
    precondition(window.feed != nil)
    precondition(window.activity_type != nil)
    precondition(window.message != nil)
    precondition(window.add_button != nil)
    precondition(window.seed_button != nil)
    precondition(window.clear_button != nil)
    precondition(window.status != nil)

    _ = window.renderContent(
        size: Size(width: kHeaderLayoutTestWidth, height: kHeaderLayoutTestHeight),
        focusedName: nil,
        editMode: false
    )
    precondition(window.header_title.frame.col + window.header_title.frame.width <= window.settings_button.frame.col)
    precondition(window.settings_button.frame.col + window.settings_button.frame.width < window.quit_button.frame.col)
}

private func logicTest()
{
    let app = ActivityFeedApp()
    app.open()

    precondition(app.activityCount() == kInitialSampleCount)
    precondition(app.feed.child != nil)
    let feedSize = Size(width: max(1, app.feed.frame.width), height: max(1, app.feed.frame.height))
    var feedText = renderedText(app.feed.render(size: feedSize))
    precondition(feedText.contains("Workspace opened"))

    app.message.setValue("Manual event")
    app.button("add_button")
    precondition(app.message.value.isEmpty)
    precondition(app.status.text == "3 activities")
    feedText = renderedText(app.feed.render(size: feedSize))
    precondition(feedText.contains("Manual event"))

    app.seedActivities()
    precondition(app.activityCount() == kInitialSampleCount + 1 + kSampleActivities.count)
    feedText = renderedText(app.feed.render(size: feedSize))
    precondition(feedText.contains("Background sync finished"))

    app.clearActivities()
    precondition(app.activityCount() == 0)
    precondition(app.status.text == "Feed cleared")

    app.button("settings_button")
    precondition(app.hasModalOverlay())
    app.button("quit_button")
    let options = app.runtimeOptions()
    precondition(options.shouldClose?() == true)
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
            let app = ActivityFeedApp()
            app.open()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

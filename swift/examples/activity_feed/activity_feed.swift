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

private func stylePaddingTop(_ style: Style) -> Int
{
    max(0, style.paddingTop ?? style.padding ?? 0)
}

private func stylePaddingRight(_ style: Style) -> Int
{
    max(0, style.paddingRight ?? style.padding ?? 0)
}

private func stylePaddingBottom(_ style: Style) -> Int
{
    max(0, style.paddingBottom ?? style.padding ?? 0)
}

private func stylePaddingLeft(_ style: Style) -> Int
{
    max(0, style.paddingLeft ?? style.padding ?? 0)
}

private func constrainedPadding(size: Size, style: Style) -> ConstrainedPadding
{
    let width = max(1, size.width)
    let height = max(1, size.height)
    var padding = ConstrainedPadding()
    padding.left = min(stylePaddingLeft(style), width - 1)
    padding.right = min(stylePaddingRight(style), width - padding.left - 1)
    padding.top = min(stylePaddingTop(style), height - 1)
    padding.bottom = min(stylePaddingBottom(style), height - padding.top - 1)
    return padding
}

private func blankRow(width: Int, style: Style) -> [TerminalCell]
{
    Array(repeating: TerminalCell(" ", foreground: nil, background: style.background), count: max(1, width))
}

private func fitRow(_ row: [TerminalCell], width: Int, fillCell: TerminalCell) -> [TerminalCell]
{
    let safeWidth = max(1, width)
    if row.count > safeWidth
    {
        return Array(row.prefix(safeWidth))
    }
    if row.count < safeWidth
    {
        return row + Array(repeating: fillCell, count: safeWidth - row.count)
    }
    return row
}

private final class ActivityFeedPanel: ActivityFeedPanelUI
{
    private var activities: [ActivityItemData] = []
    private var autoScroll = false
    private var viewOffset = 0

    func clearActivities()
    {
        activities.removeAll()
        viewOffset = 0
    }

    func appendActivity(timestamp: String, eventType: String, message: String, showTimestamp: Bool)
    {
        activities.append(ActivityItemData(
            timestamp: timestamp,
            eventType: eventType,
            message: message,
            showTimestamp: showTimestamp
        ))
        if autoScroll
        {
            viewOffset = 0
        }
    }

    override func setAutoScroll(_ enabled: Bool)
    {
        autoScroll = enabled
        if enabled
        {
            viewOffset = 0
        }
    }

    func activityCount() -> Int
    {
        activities.count
    }

    override func renderContent(size: Size, focusedName: String?, editMode: Bool) -> [[TerminalCell]]
    {
        _ = focusedName
        _ = editMode
        let width = max(1, size.width)
        let height = max(1, size.height)
        let panelStyle = scrollView().style
        let padding = constrainedPadding(size: Size(width: width, height: height), style: panelStyle)
        let viewportWidth = max(1, width - padding.left - padding.right)
        let viewportHeight = max(1, height - padding.top - padding.bottom)
        let gap = max(0, panelStyle.gap ?? 0)
        let fill = blankRow(width: viewportWidth, style: panelStyle)
        let fillCell = fill[0]

        var rows: [[TerminalCell]] = []
        for index in activities.indices
        {
            for row in renderActivityItem(activities[index], width: viewportWidth)
            {
                rows.append(fitRow(row, width: viewportWidth, fillCell: fillCell))
            }
            if index + 1 < activities.count
            {
                rows.append(contentsOf: Array(repeating: fill, count: gap))
            }
        }

        let naturalSkip = max(0, rows.count - viewportHeight)
        if autoScroll
        {
            viewOffset = 0
        }
        viewOffset = min(viewOffset, naturalSkip)
        let skip = naturalSkip - viewOffset
        var visible = Array(rows.dropFirst(skip).prefix(viewportHeight))
        while visible.count < viewportHeight
        {
            visible.append(fill)
        }
        if skip > 0 && !visible.isEmpty && !visible[0].isEmpty
        {
            visible[0][viewportWidth - 1].text = "^"
        }
        if viewOffset > 0 && !visible.isEmpty && !visible[visible.count - 1].isEmpty
        {
            visible[visible.count - 1][viewportWidth - 1].text = "v"
        }

        let fullBlank = blankRow(width: width, style: panelStyle)
        var padded: [[TerminalCell]] = []
        while padded.count < padding.top
        {
            padded.append(fullBlank)
        }
        for row in visible
        {
            var paddedRow = fullBlank
            for col in row.indices where padding.left + col < width
            {
                paddedRow[padding.left + col] = row[col]
            }
            padded.append(paddedRow)
        }
        while padded.count < height
        {
            padded.append(fullBlank)
        }
        return Array(padded.prefix(height))
    }
}

private struct SettingsResult
{
    var autoScroll = true
    var showTimestamps = true
    var defaultType = kDefaultActivityType
}

private final class SettingsDialog
{
    let ui = SettingsUI()
    private(set) var isOpen = false

    func configure(_ settings: SettingsResult)
    {
        ui.auto_scroll.setChecked(settings.autoScroll)
        ui.show_timestamps.setChecked(settings.showTimestamps)
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
            autoScroll: ui.auto_scroll.checked,
            showTimestamps: ui.show_timestamps.checked,
            defaultType: selectedDefaultType()
        )
    }

    func selectDefaultType(_ value: String)
    {
        selectComboValue(ui.default_type, value)
    }

    func selectedDefaultType() -> String
    {
        selectedComboValue(ui.default_type, fallback: kDefaultActivityType)
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
        frame.onKeyBeforeFocusedElement = { [weak self] keyName, _, editMode in
            guard let self else
            {
                return false
            }
            if keyName == "Escape" && !editMode
            {
                status.setText("Settings canceled")
                closeSettings()
                return true
            }
            return false
        }
        modalStack.push(settingsDialog.ui, frame)
    }

    private func closeSettings()
    {
        modalStack.remove(settingsDialog.ui)
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

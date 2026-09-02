import Foundation
import Uimd

private struct TranscriptEntry
{
    let role: String
    let phase: String
    let body: String
    let detail: String
}

private struct ScriptedStep
{
    let replaceLast: Bool
    let entry: TranscriptEntry
}

private let kInitialEntry = TranscriptEntry(
    role: "You",
    phase: "request",
    body: "Audit the release pipeline and propose a safe fix.\n" +
        "Constraints: deterministic output, no network, preserve compatibility.",
    detail: "simulated user message"
)

private let kScriptedSteps = [
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "plan", body: "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate", detail: "simulated reasoning summary")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "search_code", body: "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches", detail: "SIMULATED TOOL · no command executed")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "table", body: "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |", detail: "deterministic comparison table")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "code · python", body: "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```", detail: "simulated code block")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "stream · 1/3", body: "I found the shared invariant. The manifest must reject empty digests", detail: "simulated streaming response")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "stream · 2/3", body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.", detail: "simulated streaming response")),
    ScriptedStep(replaceLast: true, entry: TranscriptEntry(role: "Agent", phase: "stream · complete", body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.", detail: "simulated streaming response")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "patch", body: "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")", detail: "SIMULATED PATCH · repository unchanged")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Tool", phase: "test failure", body: "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed", detail: "SIMULATED ERROR · no test executed")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "structured data", body: "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}", detail: "simulated YAML result")),
    ScriptedStep(replaceLast: false, entry: TranscriptEntry(role: "Agent", phase: "final", body: "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small", detail: "simulated final answer")),
]

private let kStressEntryCount = 5_556
private let kStressTargetRows = 50_000
private let kSimulationMessageTarget = 100
private let kScriptedMessageCount = 10
private let kSimulationEventCount =
    kScriptedSteps.count + kSimulationMessageTarget - kScriptedMessageCount
private let kSimulationDurationSeconds = 180.0
private let kSimulationStepIntervalSeconds =
    kSimulationDurationSeconds / Double(kSimulationEventCount - 1)

private final class Transcript: TranscriptUI
{
    override init()
    {
        super.init()
        setAutoScroll(false)
    }

    func appendEntry(_ entry: TranscriptEntry)
    {
        let view = ChatEntryUI()
        Self.setEntry(view, entry)
        let naturalSize = view.generatedContentSize()
        let measuredSize = view.generatedContentSizeForWidth(max(1, naturalSize.width))
        let reusable = ReusableElement("entry")
        reusable.setChild(view)
        reusable.frame = Rect(row: 0, col: 0, width: 0, height: max(1, measuredSize.height))
        _ = scrollView().addChild(reusable)
    }

    func replaceLastEntry(_ entry: TranscriptEntry)
    {
        guard let reusable = scrollView().children.last as? ReusableElement,
              let view = reusable.child as? ChatEntryUI else
        {
            appendEntry(entry)
            return
        }
        Self.setEntry(view, entry)
    }

    func clearEntries()
    {
        clearChildren()
        scrollToTop()
    }

    func entryCount() -> Int
    {
        scrollView().children.count
    }

    private static func setEntry(_ view: ChatEntryUI, _ entry: TranscriptEntry)
    {
        view.role.setText(entry.role)
        view.phase.setText(entry.phase)
        view.body.setText(entry.body)
        view.detail.setText(entry.detail)
    }
}

private final class AgentChat: AgentChatUI
{
    private let transcriptPanel = Transcript()
    private let clock: () -> TimeInterval
    private var nextStep = 0
    private var simulationRunning = false
    private var nextStepAt: TimeInterval?
    private var quitRequested = false

    override init()
    {
        clock = { ProcessInfo.processInfo.systemUptime }
        super.init()
        transcript.setChild(transcriptPanel)
        resetSimulation()
    }

    init(clock: @escaping () -> TimeInterval)
    {
        self.clock = clock
        super.init()
        transcript.setChild(transcriptPanel)
        resetSimulation()
    }

    override func onStartButtonClick()
    {
        startSimulation()
    }

    override func onPauseButtonClick()
    {
        pauseSimulation()
    }

    override func onNextStepButtonClick()
    {
        applyNextStep()
    }

    override func onResetButtonClick()
    {
        resetSimulation()
    }

    override func onLoadStressButtonClick()
    {
        loadStressTranscript()
    }

    override func onQuitButtonClick()
    {
        quitRequested = true
    }

    override func shouldClose() -> Bool
    {
        quitRequested
    }

    func startSimulation()
    {
        if nextStep >= kSimulationEventCount
        {
            resetSimulation()
        }
        if nextStep == 0
        {
            applyNextTimelineEvent()
        }
        transcriptPanel.scrollToBottom()
        if nextStep >= kSimulationEventCount
        {
            finishSimulation()
            return
        }
        simulationRunning = true
        nextStepAt = clock() + kSimulationStepIntervalSeconds
        setStatus("Running")
    }

    func pauseSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        setStatus("Paused")
    }

    func applyNextStep()
    {
        simulationRunning = false
        nextStepAt = nil
        guard nextStep < kSimulationEventCount else
        {
            setStatus("Scenario complete")
            return
        }
        applyNextTimelineEvent()
        transcriptPanel.scrollToBottom()
        setStatus(nextStep == kSimulationEventCount ? "Scenario complete" : "Step applied")
    }

    func resetSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        transcriptPanel.clearEntries()
        transcriptPanel.appendEntry(kInitialEntry)
        transcriptPanel.scrollToTop()
        nextStep = 0
        setStatus("Ready")
    }

    func loadStressTranscript()
    {
        simulationRunning = false
        nextStepAt = nil
        transcriptPanel.clearEntries()
        for index in 0..<kStressEntryCount
        {
            transcriptPanel.appendEntry(stressEntry(index))
        }
        transcriptPanel.scrollToTop()
        nextStep = kSimulationEventCount
        status.setText("Stress loaded · \(messageCountText(transcriptPanel.entryCount())) · ~\(kStressTargetRows) rows")
    }

    func entryCount() -> Int
    {
        transcriptPanel.entryCount()
    }

    override func tick() -> Bool
    {
        guard simulationRunning, let deadline = nextStepAt else
        {
            return false
        }
        let now = clock()
        var nextDeadline = deadline
        var applied = false
        while now >= nextDeadline && nextStep < kSimulationEventCount
        {
            applyNextTimelineEvent()
            nextDeadline += kSimulationStepIntervalSeconds
            applied = true
        }
        nextStepAt = nextDeadline
        guard applied else
        {
            return false
        }
        transcriptPanel.scrollToBottom()
        if nextStep >= kSimulationEventCount
        {
            finishSimulation()
        }
        else
        {
            setStatus("Running")
        }
        return true
    }

    private func applyStep(_ step: ScriptedStep)
    {
        if step.replaceLast
        {
            transcriptPanel.replaceLastEntry(step.entry)
        }
        else
        {
            transcriptPanel.appendEntry(step.entry)
        }
    }

    private func applyNextTimelineEvent()
    {
        if nextStep < kScriptedSteps.count
        {
            applyStep(kScriptedSteps[nextStep])
        }
        else
        {
            transcriptPanel.appendEntry(stressEntry(nextStep - kScriptedSteps.count))
        }
        nextStep += 1
    }

    private func finishSimulation()
    {
        simulationRunning = false
        nextStepAt = nil
        setStatus("Scenario complete")
    }

    private func setStatus(_ prefix: String)
    {
        status.setText(prefix + " · " + messageCountText(transcriptPanel.entryCount()))
    }
}

private func messageCountText(_ count: Int) -> String
{
    "\(count) " + (count == 1 ? "message" : "messages")
}

private func stressEntry(_ index: Int) -> TranscriptEntry
{
    let detail = "SIMULATED STRESS DATA · no external action"
    let line = (index % 997) + 1
    switch index % 8
    {
    case 0:
        return TranscriptEntry(role: "You", phase: "request", body: "Investigate deterministic case \(index).\n- preserve API\n- compare ports", detail: detail)
    case 1:
        return TranscriptEntry(role: "Agent", phase: "plan", body: "## Plan \(index)\n1. inspect\n2. reproduce\n3. repair\n4. validate", detail: detail)
    case 2:
        return TranscriptEntry(role: "Tool", phase: "search_code", body: "$ search_code case_\(index)\nsrc/module.py:\(line): match\nresult: simulated", detail: detail)
    case 3:
        return TranscriptEntry(role: "Agent", phase: "code", body: "```python\ndef case_\(index)():\n    return {\"ok\": True}\n```", detail: detail)
    case 4:
        return TranscriptEntry(role: "Tool", phase: "diff", body: "--- before_\(index)\n+++ after_\(index)\n@@ -1 +1 @@\n-old\n+new", detail: detail)
    case 5:
        return TranscriptEntry(role: "Agent", phase: "table", body: "| item | value |\n|------|-------|\n| id | \(index) |\n| state | pass |", detail: detail)
    case 6:
        return TranscriptEntry(role: "Tool", phase: "warning", body: "WARNING simulated retry \(index)\ntrace: worker -> parser -> validator\nno external action", detail: detail)
    default:
        return TranscriptEntry(role: "Agent", phase: "result", body: "{\"id\": \(index), \"status\": \"ok\"}\nUnicode: čau · 東京 · 🙂", detail: detail)
    }
}

private func logicTest()
{
    var now = 0.0
    let app = AgentChat(clock: { now })
    precondition(app.entryCount() == 1)
    precondition(app.status.text == "Ready · 1 message")
    app.applyNextStep()
    app.startSimulation()
    precondition(app.entryCount() == 2)
    precondition(app.status.text == "Running · 2 messages")
    app.pauseSimulation()
    now += kSimulationDurationSeconds
    precondition(!app.tick())
    precondition(app.entryCount() == 2)
    app.applyNextStep()
    app.startSimulation()
    now += kSimulationDurationSeconds
    precondition(app.tick())
    precondition(app.entryCount() == kSimulationMessageTarget)
    precondition(app.status.text == "Scenario complete · 100 messages")
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            if CommandLine.arguments.contains("--logic-test")
            {
                logicTest()
                return 0
            }
            let app = AgentChat()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

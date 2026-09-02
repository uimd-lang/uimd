package main

import (
	"fmt"
	"os"
	"strconv"
	"time"

	transcriptview "uimd_examples/agent_chat/transcript"

	uimd "uimd"
)

type scriptedStep struct {
	replaceLast bool
	entry       transcriptview.Entry
}

var initialEntry = transcriptview.Entry{
	Role:   "You",
	Phase:  "request",
	Body:   "Audit the release pipeline and propose a safe fix.\nConstraints: deterministic output, no network, preserve compatibility.",
	Detail: "simulated user message",
}

var scriptedSteps = []scriptedStep{
	{false, transcriptview.Entry{Role: "Agent", Phase: "plan", Body: "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate", Detail: "simulated reasoning summary"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "search_code", Body: "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches", Detail: "SIMULATED TOOL · no command executed"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |", Detail: "deterministic comparison table"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "code · python", Body: "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```", Detail: "simulated code block"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "stream · 1/3", Body: "I found the shared invariant. The manifest must reject empty digests", Detail: "simulated streaming response"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "stream · 2/3", Body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.", Detail: "simulated streaming response"}},
	{true, transcriptview.Entry{Role: "Agent", Phase: "stream · complete", Body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.", Detail: "simulated streaming response"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "patch", Body: "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")", Detail: "SIMULATED PATCH · repository unchanged"}},
	{false, transcriptview.Entry{Role: "Tool", Phase: "test failure", Body: "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed", Detail: "SIMULATED ERROR · no test executed"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "structured data", Body: "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}", Detail: "simulated YAML result"}},
	{false, transcriptview.Entry{Role: "Agent", Phase: "final", Body: "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small", Detail: "simulated final answer"}},
}

const (
	stressEntryCount        = 5_556
	stressTargetRows        = 50_000
	simulationMessageTarget = 100
	scriptedMessageCount    = 10
	simulationDuration      = 180 * time.Second
)

var simulationEventCount = len(scriptedSteps) + simulationMessageTarget - scriptedMessageCount
var simulationStepInterval = simulationDuration / time.Duration(simulationEventCount-1)

type AgentChatApp struct {
	*AgentChatUI
	transcript *transcriptview.Transcript
	nextStep   int
	clock      func() time.Duration
	running    bool
	nextStepAt time.Duration
}

func NewAgentChatApp() *AgentChatApp {
	origin := time.Now()
	return newAgentChatApp(func() time.Duration { return time.Since(origin) })
}

func newAgentChatApp(clock func() time.Duration) *AgentChatApp {
	panel := transcriptview.NewTranscript()
	app := &AgentChatApp{AgentChatUI: NewAgentChatUI(), transcript: panel, clock: clock}
	app.SetEventHandler(app)
	app.Transcript.SetChild(panel)
	app.ResetSimulation()
	return app
}

func (app *AgentChatApp) OnStartButtonClick() {
	app.StartSimulation()
}

func (app *AgentChatApp) OnPauseButtonClick() {
	app.PauseSimulation()
}

func (app *AgentChatApp) OnNextStepButtonClick() {
	app.NextStep()
}

func (app *AgentChatApp) OnResetButtonClick() {
	app.ResetSimulation()
}

func (app *AgentChatApp) OnLoadStressButtonClick() {
	app.LoadStressTranscript()
}

func (app *AgentChatApp) OnQuitButtonClick() {
	app.RequestClose()
}

func (app *AgentChatApp) StartSimulation() {
	if app.nextStep >= simulationEventCount {
		app.ResetSimulation()
	}
	if app.nextStep == 0 {
		app.applyNextTimelineEvent()
	}
	app.transcript.Panel.ScrollToBottom()
	if app.nextStep >= simulationEventCount {
		app.finishSimulation()
		return
	}
	app.running = true
	app.nextStepAt = app.clock() + simulationStepInterval
	app.setStatus("Running")
}

func (app *AgentChatApp) PauseSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.setStatus("Paused")
}

func (app *AgentChatApp) NextStep() {
	app.running = false
	app.nextStepAt = 0
	if app.nextStep >= simulationEventCount {
		app.setStatus("Scenario complete")
		return
	}
	app.applyNextTimelineEvent()
	app.transcript.Panel.ScrollToBottom()
	prefix := "Step applied"
	if app.nextStep == simulationEventCount {
		prefix = "Scenario complete"
	}
	app.setStatus(prefix)
}

func (app *AgentChatApp) ResetSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.transcript.ClearEntries()
	app.transcript.AppendEntry(initialEntry)
	app.transcript.Panel.ScrollToTop()
	app.nextStep = 0
	app.setStatus("Ready")
}

func (app *AgentChatApp) LoadStressTranscript() {
	app.running = false
	app.nextStepAt = 0
	app.transcript.ClearEntries()
	for index := 0; index < stressEntryCount; index++ {
		app.transcript.AppendEntry(stressEntry(index))
	}
	app.transcript.Panel.ScrollToTop()
	app.nextStep = simulationEventCount
	app.Status.SetText(fmt.Sprintf("Stress loaded · %s · ~%d rows", messageCountText(app.transcript.EntryCount()), stressTargetRows))
}

func (app *AgentChatApp) applyStep(step scriptedStep) {
	if step.replaceLast {
		app.transcript.ReplaceLastEntry(step.entry)
	} else {
		app.transcript.AppendEntry(step.entry)
	}
}

func (app *AgentChatApp) applyNextTimelineEvent() {
	if app.nextStep < len(scriptedSteps) {
		app.applyStep(scriptedSteps[app.nextStep])
	} else {
		app.transcript.AppendEntry(stressEntry(app.nextStep - len(scriptedSteps)))
	}
	app.nextStep++
}

func (app *AgentChatApp) finishSimulation() {
	app.running = false
	app.nextStepAt = 0
	app.setStatus("Scenario complete")
}

func (app *AgentChatApp) Tick() bool {
	if !app.running {
		return false
	}
	now := app.clock()
	applied := false
	for now >= app.nextStepAt && app.nextStep < simulationEventCount {
		app.applyNextTimelineEvent()
		app.nextStepAt += simulationStepInterval
		applied = true
	}
	if !applied {
		return false
	}
	app.transcript.Panel.ScrollToBottom()
	if app.nextStep >= simulationEventCount {
		app.finishSimulation()
	} else {
		app.setStatus("Running")
	}
	return true
}

func (app *AgentChatApp) setStatus(prefix string) {
	app.Status.SetText(prefix + " · " + messageCountText(app.transcript.EntryCount()))
}

func messageCountText(count int) string {
	noun := "messages"
	if count == 1 {
		noun = "message"
	}
	return fmt.Sprintf("%d %s", count, noun)
}

func stressEntry(index int) transcriptview.Entry {
	number := strconv.Itoa(index)
	line := strconv.Itoa((index % 997) + 1)
	detail := "SIMULATED STRESS DATA · no external action"
	switch index % 8 {
	case 0:
		return transcriptview.Entry{Role: "You", Phase: "request", Body: "Investigate deterministic case " + number + ".\n- preserve API\n- compare ports", Detail: detail}
	case 1:
		return transcriptview.Entry{Role: "Agent", Phase: "plan", Body: "## Plan " + number + "\n1. inspect\n2. reproduce\n3. repair\n4. validate", Detail: detail}
	case 2:
		return transcriptview.Entry{Role: "Tool", Phase: "search_code", Body: "$ search_code case_" + number + "\nsrc/module.py:" + line + ": match\nresult: simulated", Detail: detail}
	case 3:
		return transcriptview.Entry{Role: "Agent", Phase: "code", Body: "```python\ndef case_" + number + "():\n    return {\"ok\": True}\n```", Detail: detail}
	case 4:
		return transcriptview.Entry{Role: "Tool", Phase: "diff", Body: "--- before_" + number + "\n+++ after_" + number + "\n@@ -1 +1 @@\n-old\n+new", Detail: detail}
	case 5:
		return transcriptview.Entry{Role: "Agent", Phase: "table", Body: "| item | value |\n|------|-------|\n| id | " + number + " |\n| state | pass |", Detail: detail}
	case 6:
		return transcriptview.Entry{Role: "Tool", Phase: "warning", Body: "WARNING simulated retry " + number + "\ntrace: worker -> parser -> validator\nno external action", Detail: detail}
	default:
		return transcriptview.Entry{Role: "Agent", Phase: "result", Body: "{\"id\": " + number + ", \"status\": \"ok\"}\nUnicode: čau · 東京 · 🙂", Detail: detail}
	}
}

func logicTest() {
	now := time.Duration(0)
	app := newAgentChatApp(func() time.Duration { return now })
	if app.transcript.EntryCount() != 1 || app.Status.Text != "Ready · 1 message" {
		panic("agent chat initial state failed")
	}
	app.NextStep()
	app.StartSimulation()
	if app.transcript.EntryCount() != 2 || app.Status.Text != "Running · 2 messages" {
		panic("agent chat timed start failed")
	}
	app.PauseSimulation()
	now += simulationDuration
	if app.Tick() || app.transcript.EntryCount() != 2 {
		panic("agent chat pause failed")
	}
	app.NextStep()
	app.StartSimulation()
	now += simulationDuration
	if !app.Tick() || app.transcript.EntryCount() != simulationMessageTarget || app.Status.Text != "Scenario complete · 100 messages" {
		panic("agent chat scenario failed")
	}
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 && os.Args[1] == "--logic-test" {
			logicTest()
			return 0
		}
		app := NewAgentChatApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}

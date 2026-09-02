using System.Diagnostics;
using Uimd;

internal readonly record struct TranscriptEntry(string Role, string Phase, string Body, string Detail);

internal readonly record struct ScriptedStep(bool ReplaceLast, TranscriptEntry Entry);

internal sealed class Transcript : TranscriptUI
{
    public Transcript()
    {
        SetAutoScroll(false);
    }

    public void AppendEntry(TranscriptEntry entry)
    {
        ChatEntryUI view = new();
        SetEntry(view, entry);
        Size natural = GeneratedWindowRuntime.GeneratedWindowContentSize(view);
        ReusableElement reusable = new("entry");
        reusable.SetChild(view);
        reusable.Frame = new Rect(0, 0, 0, Math.Max(1, natural.Height));
        AddChild(reusable);
    }

    public void ReplaceLastEntry(TranscriptEntry entry)
    {
        if (ScrollView().Children.Count == 0)
        {
            AppendEntry(entry);
            return;
        }
        ReusableElement? reusable = ScrollView().Children[^1] as ReusableElement;
        if (reusable?.Child is ChatEntryUI view)
        {
            SetEntry(view, entry);
        }
    }

    public void ClearEntries()
    {
        ClearChildren();
        _ = ScrollToTop();
    }

    public int EntryCount()
    {
        return ScrollView().Children.Count;
    }

    private static void SetEntry(ChatEntryUI view, TranscriptEntry entry)
    {
        view.role.SetText(entry.Role);
        view.phase.SetText(entry.Phase);
        view.body.SetText(entry.Body);
        view.detail.SetText(entry.Detail);
    }
}

public sealed class AgentChat : AgentChatUI
{
    private const int StressEntryCount = 5_556;
    private const int StressTargetRows = 50_000;
    internal const int SimulationMessageTarget = 100;
    private const int ScriptedMessageCount = 10;
    internal const double SimulationDurationSeconds = 180.0;

    private static readonly TranscriptEntry InitialEntry = new(
        "You",
        "request",
        "Audit the release pipeline and propose a safe fix.\n" +
        "Constraints: deterministic output, no network, preserve compatibility.",
        "simulated user message");

    private static readonly ScriptedStep[] ScriptedSteps =
    {
        new(false, new("Agent", "plan", "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate", "simulated reasoning summary")),
        new(false, new("Tool", "search_code", "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches", "SIMULATED TOOL · no command executed")),
        new(false, new("Agent", "table", "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |", "deterministic comparison table")),
        new(false, new("Agent", "code · python", "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```", "simulated code block")),
        new(false, new("Agent", "stream · 1/3", "I found the shared invariant. The manifest must reject empty digests", "simulated streaming response")),
        new(true, new("Agent", "stream · 2/3", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.", "simulated streaming response")),
        new(true, new("Agent", "stream · complete", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.", "simulated streaming response")),
        new(false, new("Tool", "patch", "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")", "SIMULATED PATCH · repository unchanged")),
        new(false, new("Tool", "test failure", "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed", "SIMULATED ERROR · no test executed")),
        new(false, new("Agent", "structured data", "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}", "simulated YAML result")),
        new(false, new("Agent", "final", "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small", "simulated final answer")),
    };

    private readonly Transcript transcriptPanel = new();
    private readonly Func<double> clock;
    private int nextStep;
    private bool simulationRunning;
    private double? nextStepAt;
    private bool quitRequested;

    private static int SimulationEventCount =>
        ScriptedSteps.Length + SimulationMessageTarget - ScriptedMessageCount;

    private static double SimulationStepIntervalSeconds =>
        SimulationDurationSeconds / (SimulationEventCount - 1);

    public AgentChat(Func<double>? clock = null)
    {
        this.clock = clock ?? MonotonicSeconds;
        transcript.SetChild(transcriptPanel);
        ResetSimulation();
    }

    public void StartSimulation()
    {
        if (nextStep >= SimulationEventCount)
        {
            ResetSimulation();
        }
        if (nextStep == 0)
        {
            ApplyNextTimelineEvent();
        }
        _ = transcriptPanel.ScrollToBottom();
        if (nextStep >= SimulationEventCount)
        {
            FinishSimulation();
            return;
        }
        simulationRunning = true;
        nextStepAt = clock() + SimulationStepIntervalSeconds;
        SetStatus("Running");
    }

    public void PauseSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        SetStatus("Paused");
    }

    public void NextStep()
    {
        simulationRunning = false;
        nextStepAt = null;
        if (nextStep >= SimulationEventCount)
        {
            SetStatus("Scenario complete");
            return;
        }
        ApplyNextTimelineEvent();
        _ = transcriptPanel.ScrollToBottom();
        SetStatus(nextStep == SimulationEventCount ? "Scenario complete" : "Step applied");
    }

    public void ResetSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.ClearEntries();
        transcriptPanel.AppendEntry(InitialEntry);
        _ = transcriptPanel.ScrollToTop();
        nextStep = 0;
        SetStatus("Ready");
    }

    public void LoadStressTranscript()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.ClearEntries();
        for (int index = 0; index < StressEntryCount; ++index)
        {
            transcriptPanel.AppendEntry(StressEntry(index));
        }
        _ = transcriptPanel.ScrollToTop();
        nextStep = SimulationEventCount;
        status.SetText($"Stress loaded · {MessageCountText(transcriptPanel.EntryCount())} · ~{StressTargetRows} rows");
    }

    public int EntryCount()
    {
        return transcriptPanel.EntryCount();
    }

    public override bool Tick()
    {
        if (!simulationRunning || nextStepAt is null)
        {
            return false;
        }
        double now = clock();
        bool applied = false;
        while (now >= nextStepAt.Value && nextStep < SimulationEventCount)
        {
            ApplyNextTimelineEvent();
            nextStepAt += SimulationStepIntervalSeconds;
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        _ = transcriptPanel.ScrollToBottom();
        if (nextStep >= SimulationEventCount)
        {
            FinishSimulation();
        }
        else
        {
            SetStatus("Running");
        }
        return true;
    }

    protected override void onStartButtonClick()
    {
        StartSimulation();
    }

    protected override void onPauseButtonClick()
    {
        PauseSimulation();
    }

    protected override void onNextStepButtonClick()
    {
        NextStep();
    }

    protected override void onResetButtonClick()
    {
        ResetSimulation();
    }

    protected override void onLoadStressButtonClick()
    {
        LoadStressTranscript();
    }

    protected override void onQuitButtonClick()
    {
        quitRequested = true;
    }

    protected override bool shouldClose()
    {
        return quitRequested;
    }

    private void ApplyStep(ScriptedStep step)
    {
        if (step.ReplaceLast)
        {
            transcriptPanel.ReplaceLastEntry(step.Entry);
        }
        else
        {
            transcriptPanel.AppendEntry(step.Entry);
        }
    }

    private void ApplyNextTimelineEvent()
    {
        if (nextStep < ScriptedSteps.Length)
        {
            ApplyStep(ScriptedSteps[nextStep]);
        }
        else
        {
            transcriptPanel.AppendEntry(StressEntry(nextStep - ScriptedSteps.Length));
        }
        ++nextStep;
    }

    private void FinishSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        SetStatus("Scenario complete");
    }

    private void SetStatus(string prefix)
    {
        status.SetText(prefix + " · " + MessageCountText(transcriptPanel.EntryCount()));
    }

    private static string MessageCountText(int count)
    {
        return count + (count == 1 ? " message" : " messages");
    }

    private static double MonotonicSeconds()
    {
        return (double)Stopwatch.GetTimestamp() / Stopwatch.Frequency;
    }

    private static TranscriptEntry StressEntry(int index)
    {
        string number = index.ToString(System.Globalization.CultureInfo.InvariantCulture);
        string line = ((index % 997) + 1).ToString(System.Globalization.CultureInfo.InvariantCulture);
        return (index % 8) switch
        {
            0 => new("You", "request", $"Investigate deterministic case {number}.\n- preserve API\n- compare ports", "SIMULATED STRESS DATA · no external action"),
            1 => new("Agent", "plan", $"## Plan {number}\n1. inspect\n2. reproduce\n3. repair\n4. validate", "SIMULATED STRESS DATA · no external action"),
            2 => new("Tool", "search_code", $"$ search_code case_{number}\nsrc/module.py:{line}: match\nresult: simulated", "SIMULATED STRESS DATA · no external action"),
            3 => new("Agent", "code", $"```python\ndef case_{number}():\n    return {{\"ok\": True}}\n```", "SIMULATED STRESS DATA · no external action"),
            4 => new("Tool", "diff", $"--- before_{number}\n+++ after_{number}\n@@ -1 +1 @@\n-old\n+new", "SIMULATED STRESS DATA · no external action"),
            5 => new("Agent", "table", $"| item | value |\n|------|-------|\n| id | {number} |\n| state | pass |", "SIMULATED STRESS DATA · no external action"),
            6 => new("Tool", "warning", $"WARNING simulated retry {number}\ntrace: worker -> parser -> validator\nno external action", "SIMULATED STRESS DATA · no external action"),
            _ => new("Agent", "result", $"{{\"id\": {number}, \"status\": \"ok\"}}\nUnicode: čau · 東京 · 🙂", "SIMULATED STRESS DATA · no external action"),
        };
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        if (args.Length > 0 && args[0] == "--logic-test")
        {
            double now = 0.0;
            AgentChat test = new(() => now);
            if (test.EntryCount() != 1 || test.status.Text != "Ready · 1 message")
            {
                throw new InvalidOperationException("agent chat initial state failed");
            }
            test.NextStep();
            test.StartSimulation();
            if (test.EntryCount() != 2 || test.status.Text != "Running · 2 messages")
            {
                throw new InvalidOperationException("agent chat timed start failed");
            }
            test.PauseSimulation();
            now += AgentChat.SimulationDurationSeconds;
            if (test.Tick() || test.EntryCount() != 2)
            {
                throw new InvalidOperationException("agent chat pause failed");
            }
            test.NextStep();
            test.StartSimulation();
            now += AgentChat.SimulationDurationSeconds;
            if (!test.Tick() ||
                test.EntryCount() != AgentChat.SimulationMessageTarget ||
                test.status.Text != "Scenario complete · 100 messages")
            {
                throw new InvalidOperationException("agent chat scenario failed");
            }
            return 0;
        }
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            AgentChat app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}

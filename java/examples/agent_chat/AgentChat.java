import java.util.List;
import java.util.function.DoubleSupplier;

import uimd.Element;
import uimd.GeneratedWindowRuntime;
import uimd.ReusableElement;

public final class AgentChat extends AgentChatUI
{
    private static final int STRESS_ENTRY_COUNT = 5_556;
    private static final int STRESS_TARGET_ROWS = 50_000;
    private static final int SIMULATION_MESSAGE_TARGET = 100;
    private static final int SCRIPTED_MESSAGE_COUNT = 10;
    private static final double SIMULATION_DURATION_SECONDS = 180.0;

    private static final TranscriptEntry INITIAL_ENTRY = new TranscriptEntry(
        "You",
        "request",
        "Audit the release pipeline and propose a safe fix.\n" +
            "Constraints: deterministic output, no network, preserve compatibility.",
        "simulated user message");

    private static final List<ScriptedStep> SCRIPTED_STEPS = List.of(
        new ScriptedStep(false, new TranscriptEntry("Agent", "plan", "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate", "simulated reasoning summary")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "search_code", "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches", "SIMULATED TOOL · no command executed")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "table", "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |", "deterministic comparison table")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "code · python", "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```", "simulated code block")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "stream · 1/3", "I found the shared invariant. The manifest must reject empty digests", "simulated streaming response")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "stream · 2/3", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.", "simulated streaming response")),
        new ScriptedStep(true, new TranscriptEntry("Agent", "stream · complete", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.", "simulated streaming response")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "patch", "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")", "SIMULATED PATCH · repository unchanged")),
        new ScriptedStep(false, new TranscriptEntry("Tool", "test failure", "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed", "SIMULATED ERROR · no test executed")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "structured data", "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}", "simulated YAML result")),
        new ScriptedStep(false, new TranscriptEntry("Agent", "final", "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small", "simulated final answer")));

    private final Transcript transcriptPanel;
    private final DoubleSupplier clock;
    private int nextStep;
    private boolean simulationRunning;
    private Double nextStepAt;
    private boolean closeRequested;

    private static int simulationEventCount()
    {
        return SCRIPTED_STEPS.size() + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT;
    }

    private static double simulationStepIntervalSeconds()
    {
        return SIMULATION_DURATION_SECONDS / (simulationEventCount() - 1);
    }

    public AgentChat()
    {
        this(() -> System.nanoTime() / 1_000_000_000.0);
    }

    AgentChat(DoubleSupplier clock)
    {
        this.clock = clock;
        transcriptPanel = new Transcript();
        transcript.setChild(transcriptPanel);
        resetSimulation();
    }

    @Override
    protected void onStartButtonClick()
    {
        startSimulation();
    }

    @Override
    protected void onPauseButtonClick()
    {
        pauseSimulation();
    }

    @Override
    protected void onNextStepButtonClick()
    {
        applyNextStep();
    }

    @Override
    protected void onResetButtonClick()
    {
        resetSimulation();
    }

    @Override
    protected void onLoadStressButtonClick()
    {
        loadStressTranscript();
    }

    @Override
    protected void onQuitButtonClick()
    {
        closeRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    public void startSimulation()
    {
        if (nextStep >= simulationEventCount())
        {
            resetSimulation();
        }
        if (nextStep == 0)
        {
            applyNextTimelineEvent();
        }
        transcriptPanel.scrollToBottom();
        if (nextStep >= simulationEventCount())
        {
            finishSimulation();
            return;
        }
        simulationRunning = true;
        nextStepAt = clock.getAsDouble() + simulationStepIntervalSeconds();
        setStatus("Running");
    }

    public void pauseSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        setStatus("Paused");
    }

    public void applyNextStep()
    {
        simulationRunning = false;
        nextStepAt = null;
        if (nextStep >= simulationEventCount())
        {
            setStatus("Scenario complete");
            return;
        }
        applyNextTimelineEvent();
        transcriptPanel.scrollToBottom();
        setStatus(nextStep == simulationEventCount() ? "Scenario complete" : "Step applied");
    }

    public void resetSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.clearEntries();
        transcriptPanel.appendEntry(INITIAL_ENTRY);
        transcriptPanel.scrollToTop();
        nextStep = 0;
        setStatus("Ready");
    }

    public void loadStressTranscript()
    {
        simulationRunning = false;
        nextStepAt = null;
        transcriptPanel.clearEntries();
        for (int index = 0; index < STRESS_ENTRY_COUNT; ++index)
        {
            transcriptPanel.appendEntry(stressEntry(index));
        }
        transcriptPanel.scrollToTop();
        nextStep = simulationEventCount();
        status.setText(
            "Stress loaded · " + messageCountText(transcriptPanel.entryCount()) +
            " · ~" + STRESS_TARGET_ROWS + " rows");
    }

    public int entryCount()
    {
        return transcriptPanel.entryCount();
    }

    @Override
    public boolean tick()
    {
        if (!simulationRunning || nextStepAt == null)
        {
            return false;
        }
        double now = clock.getAsDouble();
        boolean applied = false;
        while (now >= nextStepAt && nextStep < simulationEventCount())
        {
            applyNextTimelineEvent();
            nextStepAt += simulationStepIntervalSeconds();
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        transcriptPanel.scrollToBottom();
        if (nextStep >= simulationEventCount())
        {
            finishSimulation();
        }
        else
        {
            setStatus("Running");
        }
        return true;
    }

    private void applyStep(ScriptedStep step)
    {
        if (step.replaceLast())
        {
            transcriptPanel.replaceLastEntry(step.entry());
        }
        else
        {
            transcriptPanel.appendEntry(step.entry());
        }
    }

    private void applyNextTimelineEvent()
    {
        if (nextStep < SCRIPTED_STEPS.size())
        {
            applyStep(SCRIPTED_STEPS.get(nextStep));
        }
        else
        {
            transcriptPanel.appendEntry(stressEntry(nextStep - SCRIPTED_STEPS.size()));
        }
        ++nextStep;
    }

    private void finishSimulation()
    {
        simulationRunning = false;
        nextStepAt = null;
        setStatus("Scenario complete");
    }

    private void setStatus(String prefix)
    {
        status.setText(prefix + " · " + messageCountText(transcriptPanel.entryCount()));
    }

    private static String messageCountText(int count)
    {
        return count + (count == 1 ? " message" : " messages");
    }

    private static TranscriptEntry stressEntry(int index)
    {
        String detail = "SIMULATED STRESS DATA · no external action";
        int line = (index % 997) + 1;
        return switch (index % 8)
        {
            case 0 -> new TranscriptEntry("You", "request", "Investigate deterministic case " + index + ".\n- preserve API\n- compare ports", detail);
            case 1 -> new TranscriptEntry("Agent", "plan", "## Plan " + index + "\n1. inspect\n2. reproduce\n3. repair\n4. validate", detail);
            case 2 -> new TranscriptEntry("Tool", "search_code", "$ search_code case_" + index + "\nsrc/module.py:" + line + ": match\nresult: simulated", detail);
            case 3 -> new TranscriptEntry("Agent", "code", "```python\ndef case_" + index + "():\n    return {\"ok\": True}\n```", detail);
            case 4 -> new TranscriptEntry("Tool", "diff", "--- before_" + index + "\n+++ after_" + index + "\n@@ -1 +1 @@\n-old\n+new", detail);
            case 5 -> new TranscriptEntry("Agent", "table", "| item | value |\n|------|-------|\n| id | " + index + " |\n| state | pass |", detail);
            case 6 -> new TranscriptEntry("Tool", "warning", "WARNING simulated retry " + index + "\ntrace: worker -> parser -> validator\nno external action", detail);
            default -> new TranscriptEntry("Agent", "result", "{\"id\": " + index + ", \"status\": \"ok\"}\nUnicode: čau · 東京 · 🙂", detail);
        };
    }

    private static void logicTest()
    {
        double[] now = {0.0};
        AgentChat application = new AgentChat(() -> now[0]);
        if (application.entryCount() != 1 || !"Ready · 1 message".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat initial state failed");
        }
        application.applyNextStep();
        application.startSimulation();
        if (application.entryCount() != 2 || !"Running · 2 messages".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat timed start failed");
        }
        application.pauseSimulation();
        now[0] += SIMULATION_DURATION_SECONDS;
        if (application.tick() || application.entryCount() != 2)
        {
            throw new IllegalStateException("agent chat pause failed");
        }
        application.applyNextStep();
        application.startSimulation();
        now[0] += SIMULATION_DURATION_SECONDS;
        if (!application.tick()
            || application.entryCount() != SIMULATION_MESSAGE_TARGET
            || !"Scenario complete · 100 messages".equals(application.status.text()))
        {
            throw new IllegalStateException("agent chat scenario failed");
        }
    }

    public static void main(String[] arguments)
    {
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return;
        }
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            AgentChat application = new AgentChat();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private record TranscriptEntry(String role, String phase, String body, String detail)
    {
    }

    private record ScriptedStep(boolean replaceLast, TranscriptEntry entry)
    {
    }

    private static final class Transcript extends TranscriptUI
    {
        Transcript()
        {
            setAutoScroll(false);
        }

        void appendEntry(TranscriptEntry entry)
        {
            ChatEntryUI view = new ChatEntryUI();
            setEntry(view, entry);
            addChild(new ReusableElement("entry", view));
        }

        void replaceLastEntry(TranscriptEntry entry)
        {
            List<Element> children = scrollView().children();
            if (children.isEmpty())
            {
                appendEntry(entry);
                return;
            }
            Element last = children.get(children.size() - 1);
            if (last instanceof ReusableElement reusable && reusable.child() instanceof ChatEntryUI view)
            {
                setEntry(view, entry);
            }
        }

        void clearEntries()
        {
            clearChildren();
            scrollToTop();
        }

        int entryCount()
        {
            return scrollView().children().size();
        }

        private static void setEntry(ChatEntryUI view, TranscriptEntry entry)
        {
            view.role.setText(entry.role());
            view.phase.setText(entry.phase());
            view.body.setText(entry.body());
            view.detail.setText(entry.detail());
        }
    }
}

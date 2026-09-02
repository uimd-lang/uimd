// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class AgentChatUI extends GeneratedWindowBase
{
    public Label title;
    public Label simulation_badge;
    public Button quit_button;
    public Button start_button;
    public Button pause_button;
    public Button next_step_button;
    public Button reset_button;
    public Button load_stress_button;
    public ReusableElement transcript;
    public Label status;

    public AgentChatUI()
    {
        super("Agent Chat Simulation");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "AgentChat",
            "# Agent Chat Simulation\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Deterministic simulated agent chat for rich transcript and large ScrollView testing.\"\ntags: [example, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - transcript/transcript\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Agent Chat Simulation\"\n  description: \"Application title.\"\n  expose: false\n\nsimulation_badge:\n  type: label\n  text: \"SIMULATED · OFFLINE\"\n  description: \"Clarifies that the transcript performs no real agent or tool work.\"\n  expose: false\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the Agent Chat Simulation example.\"\n\nstart_button:\n  type: button\n  title: Start\n  description: \"Run one hundred deterministic simulated messages over three minutes.\"\n\npause_button:\n  type: button\n  title: Pause\n  description: \"Pause the timed simulation without discarding progress.\"\n\nnext_step_button:\n  type: button\n  title: \"Next step\"\n  description: \"Apply one deterministic scripted chat step.\"\n\nreset_button:\n  type: button\n  title: Reset\n  description: \"Reset the transcript to its initial message.\"\n\nload_stress_button:\n  type: button\n  title: \"Load 50k\"\n  description: \"Load an approximately fifty-thousand-row deterministic stress transcript.\"\n\ntranscript:\n  type: transcript\n  cpp-class: \"agent_chat_example::Transcript\"\n  cpp-header: transcript/transcript.hpp\n  description: \"Scrollable deterministic simulated agent transcript.\"\n\nstatus:\n  type: label\n  text: \"Ready · 1 message\"\n  description: \"Shows deterministic simulation progress and transcript size.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\n@simulation_badge:\n  color: \"#c4b5fd\"\n  text-align: right\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@load_stress_button:\n  background: \"#6d28d9\"\n  focus-background: \"#7c3aed\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@content:\n  background: \"#030712\"\n@transcript:\n  background: \"#00000000\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n  padding: 0, 1, 0, 1\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**......................... simulation_badge.... quit_button......... |\n+-actions-**-------------------------------------------------------------------+\n| start_button pause_button next_step_button reset_button load_stress_button.. |\n+-content-**-------------------------------------------------------------------+\n| transcript..**.............................................................. |\n| ............................................................................ |\n* *........................................................................... |\n* *........................................................................... |\n| ............................................................................ |\n+------------------------------------------------------------------------------+\n| status..**.................................................................. |\n+------------------------------------------------------------------------------+\n```\n",
            "Deterministic simulated agent chat for rich transcript and large ScrollView testing.",
            List.of(
            new GeneratedElementMetadata("title", "Application title.", false),
            new GeneratedElementMetadata("simulation_badge", "Clarifies that the transcript performs no real agent or tool work.", false),
            new GeneratedElementMetadata("quit_button", "Close the Agent Chat Simulation example.", true),
            new GeneratedElementMetadata("start_button", "Run one hundred deterministic simulated messages over three minutes.", true),
            new GeneratedElementMetadata("pause_button", "Pause the timed simulation without discarding progress.", true),
            new GeneratedElementMetadata("next_step_button", "Apply one deterministic scripted chat step.", true),
            new GeneratedElementMetadata("reset_button", "Reset the transcript to its initial message.", true),
            new GeneratedElementMetadata("load_stress_button", "Load an approximately fifty-thousand-row deterministic stress transcript.", true),
            new GeneratedElementMetadata("transcript", "Scrollable deterministic simulated agent transcript.", true),
            new GeneratedElementMetadata("status", "Shows deterministic simulation progress and transcript size.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#030712", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        title = addElement(new Label("title", "Agent Chat Simulation"));
        title.setStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "text-align", "center"));
        title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        simulation_badge = addElement(new Label("simulation_badge", "SIMULATED · OFFLINE"));
        simulation_badge.setStyle(Style.fromProperties("background", "transparent", "color", "#c4b5fd", "text-align", "right"));
        simulation_badge.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        quit_button = addElement(new Button("quit_button", "Quit"));
        quit_button.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_button.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        start_button = addElement(new Button("start_button", "Start"));
        start_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        start_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        start_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        pause_button = addElement(new Button("pause_button", "Pause"));
        pause_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        pause_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        pause_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        next_step_button = addElement(new Button("next_step_button", "Next step"));
        next_step_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        next_step_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        next_step_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        reset_button = addElement(new Button("reset_button", "Reset"));
        reset_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        reset_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        reset_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        load_stress_button = addElement(new Button("load_stress_button", "Load 50k"));
        load_stress_button.setStyle(Style.fromProperties("background", "#6d28d9", "color", "#e2e8f0"));
        load_stress_button.setFocusStyle(Style.fromProperties("background", "#7c3aed", "color", "#ffffff"));
        load_stress_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        transcript = addElement(new ReusableElement("transcript", "transcript"));
        transcript.setChild(new TranscriptUI());
        transcript.setStyle(Style.fromProperties("background", "#00000000"));
        transcript.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        transcript.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        transcript.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        status = addElement(new Label("status", "Ready · 1 message"));
        status.setStyle(Style.fromProperties("background", "#0f172a", "color", "#93c5fd", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
    }

    private static TextGradient makeTextGradient(
        int intervalMs,
        int step,
        int segmentSize,
        String... colors)
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(intervalMs);
        gradient.setStep(step);
        gradient.setSegmentSize(segmentSize);
        for (String color : colors)
        {
            gradient.colors().add(new Color(color));
        }
        return gradient;
    }

    private static Style styleWithGradients(
        Style style,
        TextGradient colorGradient,
        TextGradient backgroundGradient)
    {
        style.setTextColorGradient(colorGradient);
        style.setTextBackgroundGradient(backgroundGradient);
        return style;
    }

    private static List<GeneratedLayoutEntry> buildLayout()
    {
        return List.of(
            new GeneratedLayoutEntry()
                .setName("start_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 1, 12, 1))
                .setSourceCell(new Rect(2, 0, 78, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(65)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(10, 0, 78, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(78))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(76, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#93c5fd", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0")),
            new GeneratedLayoutEntry()
                .setName("title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(34, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(43)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("transcript")
                .setType("transcript")
                .setCellName("content")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(4, 0, 78, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(76, 5))
                .setCellCharsSize(new Size(78, 5))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(Style.fromProperties("background", "#00000000")),
            new GeneratedLayoutEntry()
                .setName("pause_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 14, 12, 1))
                .setSourceCell(new Rect(2, 0, 78, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(52)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("next_step_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 27, 16, 1))
                .setSourceCell(new Rect(2, 0, 78, 1))
                .setWidth(AxisDimension.auto(16))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(16, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(35)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("simulation_badge")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 36, 20, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(22)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#c4b5fd", "text-align", "right")),
            new GeneratedLayoutEntry()
                .setName("reset_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 44, 12, 1))
                .setSourceCell(new Rect(2, 0, 78, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(22)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("load_stress_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 57, 20, 1))
                .setSourceCell(new Rect(2, 0, 78, 1))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#6d28d9", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("quit_button")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 57, 20, 1))
                .setSourceCell(new Rect(0, 0, 78, 1))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(78, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"))
        );
    }

    protected void onQuitButtonClick()
    {
    }

    protected void onStartButtonClick()
    {
    }

    protected void onPauseButtonClick()
    {
    }

    protected void onNextStepButtonClick()
    {
    }

    protected void onResetButtonClick()
    {
    }

    protected void onLoadStressButtonClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("quit_button".equals(name))
        {
            onQuitButtonClick();
            return true;
        }
        if ("start_button".equals(name))
        {
            onStartButtonClick();
            return true;
        }
        if ("pause_button".equals(name))
        {
            onPauseButtonClick();
            return true;
        }
        if ("next_step_button".equals(name))
        {
            onNextStepButtonClick();
            return true;
        }
        if ("reset_button".equals(name))
        {
            onResetButtonClick();
            return true;
        }
        if ("load_stress_button".equals(name))
        {
            onLoadStressButtonClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }
}

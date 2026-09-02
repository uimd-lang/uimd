// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.SpanLabel;
import uimd.Style;
import uimd.TextGradient;

public class ChatEntryUI extends GeneratedWindowBase
{
    public Label role;
    public Label phase;
    public SpanLabel body;
    public Label detail;

    public ChatEntryUI()
    {
        super("Agent Chat Entry");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "ChatEntry",
            "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Fixed-height simulated agent chat entry for prose and structured transcript blocks.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nrole:\n  type: label\n  text: Agent\n  description: \"Simulated transcript speaker or activity type.\"\n\nphase:\n  type: label\n  text: response\n  description: \"Deterministic scenario phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable simulated transcript content.\"\n\ndetail:\n  type: label\n  text: \"simulated · no external action\"\n  description: \"Simulation-only activity detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@header:\n  background: \"#162033\"\n  padding: 0, 1, 0, 1\n@role:\n  color: \"#a7f3d0\"\n@phase:\n  color: \"#c4b5fd\"\n  text-align: right\n@body_panel:\n  background: \"#0b1220\"\n  padding: 1, 1, 1, 1\n@body:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n@footer:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@detail:\n  color: \"#64748b\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-header-**---------------------------------------------------------------+\n| role..**........................................ phase................. |\n+-body_panel-**-----------------------------------------------------------+\n| body..**............................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n+-footer-**---------------------------------------------------------------+\n| detail..**............................................................. |\n+-------------------------------------------------------------------------+\n```\n",
            "Fixed-height simulated agent chat entry for prose and structured transcript blocks.",
            List.of(
            new GeneratedElementMetadata("role", "Simulated transcript speaker or activity type.", true),
            new GeneratedElementMetadata("phase", "Deterministic scenario phase.", true),
            new GeneratedElementMetadata("body", "Selectable simulated transcript content.", true),
            new GeneratedElementMetadata("detail", "Simulation-only activity detail.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#00000000", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        role = addElement(new Label("role", "Agent"));
        role.setStyle(Style.fromProperties("background", "transparent", "color", "#a7f3d0"));
        role.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        phase = addElement(new Label("phase", "response"));
        phase.setStyle(Style.fromProperties("background", "transparent", "color", "#c4b5fd", "text-align", "right"));
        phase.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        body = addElement(new SpanLabel("body", ""));
        body.setStyle(Style.fromProperties("color", "#d1d5db", "user-select", "text"));
        detail = addElement(new Label("detail", "simulated · no external action"));
        detail.setStyle(Style.fromProperties("background", "transparent", "color", "#64748b", "user-select", "text"));
        detail.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
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
                .setName("body")
                .setType("spanlabel")
                .setCellName("body_panel")
                .setRelative(new Rect(0, 1, 536870911, 6))
                .setSourceCell(new Rect(2, 0, 73, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(6))
                .setCharsSize(new Size(71, 6))
                .setCellCharsSize(new Size(73, 6))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#0b1220", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("color", "#d1d5db", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("detail")
                .setType("label")
                .setCellName("footer")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(9, 0, 73, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(71, 1))
                .setCellCharsSize(new Size(73, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#111827", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#64748b", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("role")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 73, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(48, 1))
                .setCellCharsSize(new Size(73, 1))
                .setMarginRight(24)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#162033", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#a7f3d0")),
            new GeneratedLayoutEntry()
                .setName("phase")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 50, 22, 1))
                .setSourceCell(new Rect(0, 0, 73, 1))
                .setWidth(AxisDimension.auto(22))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(22, 1))
                .setCellCharsSize(new Size(73, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#162033", "padding-bottom", "0", "padding-left", "1", "padding-right", "1", "padding-top", "0"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#c4b5fd", "text-align", "right"))
        );
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
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

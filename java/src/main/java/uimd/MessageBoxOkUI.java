// Auto-generated UI code for message_box_ok - DO NOT EDIT MANUALLY.
package uimd;

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
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class MessageBoxOkUI extends GeneratedWindowBase
{
    public Label dialog_header;
    public Label message;
    public Button ok_btn;

    public MessageBoxOkUI()
    {
        super("Message Box");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("dialog");
        setMcpMetadata(
            true,
            "MessageBoxOk",
            "# Message Box\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: \"Message Box UI source.\"\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: Message\n\nmessage:\n  type: label\n  text: \"\"\n\nok_btn:\n  type: button\n  title: OK\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------+\n|dialog_header...............................|\n+--------------------------------------------+\n|                                            |\n|  message.................................  |\n|  ........................................  |\n|                                            |\n|  ok_btn..................................  |\n|                                            |\n+--------------------------------------------+\n```\n",
            "Message Box UI source.",
            List.of(
            new GeneratedElementMetadata("dialog_header", "Message", true),
            new GeneratedElementMetadata("message", "message", true),
            new GeneratedElementMetadata("ok_btn", "OK", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        dialog_header = addElement(new Label("dialog_header", "Message"));
        dialog_header.setStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center"));
        dialog_header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        message = addElement(new Label("message", ""));
        message.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        message.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        ok_btn = addElement(new Button("ok_btn", "OK"));
        ok_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        ok_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        ok_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("dialog_header")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 44, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(44))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(44, 1))
                .setCellCharsSize(new Size(44, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("message")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 536870911, 2))
                .setSourceCell(new Rect(2, 0, 44, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(2))
                .setCellWidth(AxisDimension.auto(44))
                .setCellHeight(AxisDimension.auto(6))
                .setCharsSize(new Size(40, 2))
                .setCellCharsSize(new Size(44, 6))
                .setMarginRight(2)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("ok_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(4, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 44, 6))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(44))
                .setCellHeight(AxisDimension.auto(6))
                .setCharsSize(new Size(40, 1))
                .setCellCharsSize(new Size(44, 6))
                .setMarginRight(2)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onOkBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("ok_btn".equals(name))
        {
            onOkBtnClick();
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

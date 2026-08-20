// Auto-generated UI code for task_row - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.CheckBox;
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

public class TaskRowUI extends GeneratedWindowBase
{
    public Label task_title;
    public Label meta;
    public CheckBox done;
    public Label spacer;
    public Button open_btn;
    public Button delete_btn;

    public TaskRowUI()
    {
        super("Task Row");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("component");
        setMcpMetadata(
            true,
            "TaskRow",
            "# Task Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable task row with editable controls.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntask_title:\n  type: label\n  text: \"Task\"\n  description: \"Task title.\"\n\nmeta:\n  type: label\n  text: \"Status / assignee / priority\"\n  description: \"Task status, assignee, and priority.\"\n\ndone:\n  type: checkbox\n  title: Done\n  value: false\n  description: \"Whether the task is done.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer before task actions.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this task in the edit dialog.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Ask for confirmation and delete this task.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@task_title:\n  color: \"#f8fafc\"\n  user-select: text\n@meta:\n  color: \"#93c5fd\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------+\n| task_title..**....... meta................... |\n| done.... spacer..**.. open_btn.. delete_btn.. |\n+-----------------------------------------------+\n```\n",
            "Reusable task row with editable controls.",
            List.of(
            new GeneratedElementMetadata("task_title", "Task title.", true),
            new GeneratedElementMetadata("meta", "Task status, assignee, and priority.", true),
            new GeneratedElementMetadata("done", "Whether the task is done.", true),
            new GeneratedElementMetadata("spacer", "Flexible row spacer before task actions.", false),
            new GeneratedElementMetadata("open_btn", "Open this task in the edit dialog.", true),
            new GeneratedElementMetadata("delete_btn", "Ask for confirmation and delete this task.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        task_title = addElement(new Label("task_title", "Task"));
        task_title.setStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text"));
        task_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        meta = addElement(new Label("meta", "Status / assignee / priority"));
        meta.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd", "user-select", "text"));
        meta.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        done = addElement(new CheckBox("done", "Done", false));
        done.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        done.setFocusStyle(Style.fromProperties("background", "#ffffff44", "color", "#facc15"));
        done.setCheckedStyle(Style.fromProperties("background", "transparent", "color", "#22c55e"));
        done.setUncheckedStyle(Style.fromProperties("background", "transparent", "color", "#94a3b8"));
        done.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        spacer = addElement(new Label("spacer", ""));
        spacer.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        spacer.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        open_btn = addElement(new Button("open_btn", "Open"));
        open_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        open_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        open_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        delete_btn = addElement(new Button("delete_btn", "Delete"));
        delete_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        delete_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        delete_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("task_title")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(21, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(25)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#f8fafc", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("meta")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(0, 23, 23, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.auto(23))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("done")
                .setType("checkbox")
                .setCellName("card")
                .setRelative(new Rect(1, 1, 8, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.auto(8))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(8, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(38)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("spacer")
                .setType("label")
                .setCellName("card")
                .setRelative(new Rect(1, 10, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(25)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("open_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 23, 10, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(14)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("delete_btn")
                .setType("button")
                .setCellName("card")
                .setRelative(new Rect(1, 34, 12, 1))
                .setSourceCell(new Rect(0, 0, 47, 2))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(47, 2))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"))
        );
    }

    protected void onDoneChange(String value)
    {
    }

    protected void onOpenBtnClick()
    {
    }

    protected void onDeleteBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("open_btn".equals(name))
        {
            onOpenBtnClick();
            return true;
        }
        if ("delete_btn".equals(name))
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("done".equals(name))
        {
            onDoneChange(value);
            return true;
        }
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

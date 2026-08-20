// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.ComboBox;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextArea;
import uimd.TextGradient;
import uimd.TextInput;

public class TaskDialogUI extends GeneratedWindowBase
{
    public Label header;
    public Label name_label;
    public TextInput name;
    public Label status_label;
    public ComboBox project_status;
    public Label owner_label;
    public ComboBox owner;
    public Label priority_label;
    public ComboBox priority;
    public Label note_label;
    public TextArea note;
    public Label message;
    public Button add_btn;
    public Button cancel_btn;

    public TaskDialogUI()
    {
        super("Task Dialog");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("dialog");
        setMcpMetadata(
            true,
            "TaskDialog",
            "# Task Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: draft\ndescription: \"Dialog form for creating or editing a task.\"\ntags: [example, dialog]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Task\n  description: \"Dialog title.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Title\n  description: \"Label for task title.\"\n  expose: false\n\nname:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  description: \"Required task title.\"\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Label for task status.\"\n  expose: false\n\nproject_status:\n  type: combobox\n  options: [Todo, Doing, Blocked, Done]\n  selected_item: Todo\n  description: \"Task status.\"\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Label for task assignee.\"\n  expose: false\n\nowner:\n  type: combobox\n  options: [Ava, Bruno, Chen, Dana]\n  selected_item: Ava\n  description: \"Task assignee.\"\n\npriority_label:\n  type: label\n  text: Priority\n  description: \"Label for task priority.\"\n  expose: false\n\npriority:\n  type: combobox\n  options: [Low, Med, High, Urgent]\n  selected_item: Med\n  description: \"Task priority.\"\n\nnote_label:\n  type: label\n  text: Description\n  description: \"Label for task description.\"\n  expose: false\n\nnote:\n  type: textarea\n  value: \"\"\n  maxlength: 160\n  description: \"Task description.\"\n\nmessage:\n  type: label\n  text: \"\"\n  description: \"Validation message.\"\n\nadd_btn:\n  type: button\n  title: Save task\n  description: \"Save the task and close the dialog.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without saving the task.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@add_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------------+\n|header................................................|\n+------------------------------------------------------+\n|                                                      |\n|  name_label      name.............................   |\n|                                                      |\n|  status_label    project_status...................   |\n|                                                      |\n|  owner_label     owner............................   |\n|                                                      |\n|  priority_label  priority.........................   |\n|                                                      |\n|  note_label.     note.............................   |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                                                      |\n|  message..........................................   |\n|                                                      |\n+------------------------------------------------------+\n|  add_btn................  cancel_btn..............   |\n|                                                      |\n+------------------------------------------------------+\n```\n",
            "Dialog form for creating or editing a task.",
            List.of(
            new GeneratedElementMetadata("header", "Dialog title.", false),
            new GeneratedElementMetadata("name_label", "Label for task title.", false),
            new GeneratedElementMetadata("name", "Required task title.", true),
            new GeneratedElementMetadata("status_label", "Label for task status.", false),
            new GeneratedElementMetadata("project_status", "Task status.", true),
            new GeneratedElementMetadata("owner_label", "Label for task assignee.", false),
            new GeneratedElementMetadata("owner", "Task assignee.", true),
            new GeneratedElementMetadata("priority_label", "Label for task priority.", false),
            new GeneratedElementMetadata("priority", "Task priority.", true),
            new GeneratedElementMetadata("note_label", "Label for task description.", false),
            new GeneratedElementMetadata("note", "Task description.", true),
            new GeneratedElementMetadata("message", "Validation message.", true),
            new GeneratedElementMetadata("add_btn", "Save the task and close the dialog.", true),
            new GeneratedElementMetadata("cancel_btn", "Close the dialog without saving the task.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Task"));
        header.setStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name_label = addElement(new Label("name_label", "Title"));
        name_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        name_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        name = addElement(new TextInput("name", "", 60));
        name.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        name.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        name.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        name.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        name.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        name.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        status_label = addElement(new Label("status_label", "Status"));
        status_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        status_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        project_status = addElement(new ComboBox("project_status", List.of("Todo", "Doing", "Blocked", "Done")));
        project_status.setSelectedIndex(0);
        project_status.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        project_status.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        project_status.setEditStyle(Style.fromProperties("background", "#111827", "color", "#e5e7eb"));
        project_status.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        project_status.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        owner_label = addElement(new Label("owner_label", "Assignee"));
        owner_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        owner_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        owner = addElement(new ComboBox("owner", List.of("Ava", "Bruno", "Chen", "Dana")));
        owner.setSelectedIndex(0);
        owner.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        owner.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        owner.setEditStyle(Style.fromProperties("background", "#111827", "color", "#e5e7eb"));
        owner.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        owner.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        priority_label = addElement(new Label("priority_label", "Priority"));
        priority_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        priority_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        priority = addElement(new ComboBox("priority", List.of("Low", "Med", "High", "Urgent")));
        priority.setSelectedIndex(1);
        priority.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        priority.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        priority.setEditStyle(Style.fromProperties("background", "#111827", "color", "#e5e7eb"));
        priority.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        priority.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        note_label = addElement(new Label("note_label", "Description"));
        note_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        note_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        note = addElement(new TextArea("note", "", 160));
        note.setStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb"));
        note.setFocusStyle(Style.fromProperties("background", "#1e293b", "color", "#ffffff"));
        note.setEditStyle(Style.fromProperties("background", "#111827", "color", "#ffffff"));
        note.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        note.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        note.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        message = addElement(new Label("message", ""));
        message.setStyle(Style.fromProperties("background", "transparent", "color", "#fbbf24"));
        message.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        add_btn = addElement(new Button("add_btn", "Save task"));
        add_btn.setStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0"));
        add_btn.setFocusStyle(Style.fromProperties("background", "#16a34a", "color", "#ffffff"));
        add_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        cancel_btn = addElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        cancel_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        cancel_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("header")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 54, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(54, 1))
                .setCellCharsSize(new Size(54, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("add_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 2, 23, 1))
                .setSourceCell(new Rect(20, 0, 54, 2))
                .setWidth(AxisDimension.auto(23))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(23, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(29)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#14532d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("cancel_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(0, 27, 536870911, 1))
                .setSourceCell(new Rect(20, 0, 54, 2))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(2))
                .setCharsSize(new Size(24, 1))
                .setCellCharsSize(new Size(54, 2))
                .setMarginRight(3)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("name_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 2, 10, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(42)
                .setMarginBottom(15)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("name")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(1, 18, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(15)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("status_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(3, 2, 12, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(40)
                .setMarginBottom(13)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("project_status")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(3, 18, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(13)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("owner_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(5, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(41)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("owner")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(5, 18, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(11)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("priority_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(7, 2, 14, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(38)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("priority")
                .setType("combobox")
                .setCellName("")
                .setRelative(new Rect(7, 18, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(9)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("note_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(9, 2, 11, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(41)
                .setMarginBottom(7)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("note")
                .setType("textarea")
                .setCellName("")
                .setRelative(new Rect(9, 18, 536870911, 5))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(5))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(33, 5))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("message")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(15, 2, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 54, 17))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(54))
                .setCellHeight(AxisDimension.auto(17))
                .setCharsSize(new Size(49, 1))
                .setCellCharsSize(new Size(54, 17))
                .setMarginRight(3)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#fbbf24"))
        );
    }

    protected void onNameChange(String value)
    {
    }

    protected void onNameSubmit(String value)
    {
    }

    protected void onProjectStatusChange(String value)
    {
    }

    protected void onOwnerChange(String value)
    {
    }

    protected void onPriorityChange(String value)
    {
    }

    protected void onNoteChange(String value)
    {
    }

    protected void onNoteSubmit(String value)
    {
    }

    protected void onAddBtnClick()
    {
    }

    protected void onCancelBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("add_btn".equals(name))
        {
            onAddBtnClick();
            return true;
        }
        if ("cancel_btn".equals(name))
        {
            onCancelBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("name".equals(name))
        {
            onNameChange(value);
            return true;
        }
        if ("note".equals(name))
        {
            onNoteChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("name".equals(name))
        {
            onNameSubmit(value);
            return true;
        }
        if ("note".equals(name))
        {
            onNoteSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("project_status".equals(name))
        {
            onProjectStatusChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("owner".equals(name))
        {
            onOwnerChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        if ("priority".equals(name))
        {
            onPriorityChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}

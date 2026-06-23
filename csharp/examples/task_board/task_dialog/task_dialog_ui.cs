// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
using Uimd;

public class TaskDialogUI : GeneratedWindowBase
{
    public Label header = null!;
    public Label name_label = null!;
    public TextInput name = null!;
    public Label status_label = null!;
    public ComboBox project_status = null!;
    public Label owner_label = null!;
    public ComboBox owner = null!;
    public Label priority_label = null!;
    public ComboBox priority = null!;
    public Label note_label = null!;
    public TextArea note = null!;
    public Label message = null!;
    public Button add_btn = null!;
    public Button cancel_btn = null!;

    public TaskDialogUI() : base("Task Dialog")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("dialog");
        SetMcpMetadata(true, "TaskDialog", """
# Task Dialog

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
status: draft
description: "Dialog form for creating or editing a task."
tags: [example, dialog]
```

## Members

```yaml
header:
  type: label
  text: Task
  description: "Dialog title."
  expose: false

name_label:
  type: label
  text: Title
  description: "Label for task title."
  expose: false

name:
  type: textinput
  value: ""
  maxlength: 60
  description: "Required task title."

status_label:
  type: label
  text: Status
  description: "Label for task status."
  expose: false

project_status:
  type: combobox
  options: [Todo, Doing, Blocked, Done]
  selected_item: Todo
  description: "Task status."

owner_label:
  type: label
  text: Assignee
  description: "Label for task assignee."
  expose: false

owner:
  type: combobox
  options: [Ava, Bruno, Chen, Dana]
  selected_item: Ava
  description: "Task assignee."

priority_label:
  type: label
  text: Priority
  description: "Label for task priority."
  expose: false

priority:
  type: combobox
  options: [Low, Med, High, Urgent]
  selected_item: Med
  description: "Task priority."

note_label:
  type: label
  text: Description
  description: "Label for task description."
  expose: false

note:
  type: textarea
  value: ""
  maxlength: 160
  description: "Task description."

message:
  type: label
  text: ""
  description: "Validation message."

add_btn:
  type: button
  title: Save task
  description: "Save the task and close the dialog."

cancel_btn:
  type: button
  title: Cancel
  description: "Close the dialog without saving the task."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
textinput:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@add_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@message:
  color: "#fbbf24"
```

## User Interface

```ui
+------------------------------------------------------+
|header................................................|
+------------------------------------------------------+
|                                                      |
|  name_label      name.............................   |
|                                                      |
|  status_label    project_status...................   |
|                                                      |
|  owner_label     owner............................   |
|                                                      |
|  priority_label  priority.........................   |
|                                                      |
|  note_label.     note.............................   |
|                  ...............................     |
|                  ...............................     |
|                  ...............................     |
|                  ...............................     |
|                                                      |
|  message..........................................   |
|                                                      |
+------------------------------------------------------+
|  add_btn................  cancel_btn..............   |
|                                                      |
+------------------------------------------------------+
```

""", "Dialog form for creating or editing a task.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Dialog title.", Expose = false },
            new GeneratedElementMetadata { Name = "name_label", Description = "Label for task title.", Expose = false },
            new GeneratedElementMetadata { Name = "name", Description = "Required task title.", Expose = true },
            new GeneratedElementMetadata { Name = "status_label", Description = "Label for task status.", Expose = false },
            new GeneratedElementMetadata { Name = "project_status", Description = "Task status.", Expose = true },
            new GeneratedElementMetadata { Name = "owner_label", Description = "Label for task assignee.", Expose = false },
            new GeneratedElementMetadata { Name = "owner", Description = "Task assignee.", Expose = true },
            new GeneratedElementMetadata { Name = "priority_label", Description = "Label for task priority.", Expose = false },
            new GeneratedElementMetadata { Name = "priority", Description = "Task priority.", Expose = true },
            new GeneratedElementMetadata { Name = "note_label", Description = "Label for task description.", Expose = false },
            new GeneratedElementMetadata { Name = "note", Description = "Task description.", Expose = true },
            new GeneratedElementMetadata { Name = "message", Description = "Validation message.", Expose = true },
            new GeneratedElementMetadata { Name = "add_btn", Description = "Save the task and close the dialog.", Expose = true },
            new GeneratedElementMetadata { Name = "cancel_btn", Description = "Close the dialog without saving the task.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Task"));
        header.SetStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        name_label = AddElement(new Label("name_label", "Title"));
        name_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        name_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        name = AddElement(new TextInput("name", "", 60));
        name.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        name.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        name.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        name.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        name.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        name.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        status_label = AddElement(new Label("status_label", "Status"));
        status_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        status_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        project_status = AddElement(new ComboBox("project_status", new[] {"Todo", "Doing", "Blocked", "Done"}));
        project_status.SetSelectedIndex(0);
        project_status.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        project_status.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        project_status.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#e5e7eb")
    });
        project_status.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        project_status.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        owner_label = AddElement(new Label("owner_label", "Assignee"));
        owner_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        owner_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        owner = AddElement(new ComboBox("owner", new[] {"Ava", "Bruno", "Chen", "Dana"}));
        owner.SetSelectedIndex(0);
        owner.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        owner.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        owner.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#e5e7eb")
    });
        owner.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        owner.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        priority_label = AddElement(new Label("priority_label", "Priority"));
        priority_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        priority_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        priority = AddElement(new ComboBox("priority", new[] {"Low", "Med", "High", "Urgent"}));
        priority.SetSelectedIndex(1);
        priority.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        priority.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        priority.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#e5e7eb")
    });
        priority.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        priority.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        note_label = AddElement(new Label("note_label", "Description"));
        note_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        note_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        note = AddElement(new TextArea("note", "", 160));
        note.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        note.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        note.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#ffffff")
    });
        note.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        note.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        note.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        message = AddElement(new Label("message", ""));
        message.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24")
    });
        message.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        add_btn = AddElement(new Button("add_btn", "Save task"));
        add_btn.SetStyle(new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    });
        add_btn.SetFocusStyle(new Style
    {
        Background = new Color("#16a34a"),
        Color = new Color("#ffffff")
    });
        add_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        cancel_btn = AddElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        cancel_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        cancel_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "header",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 54, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(54, 1),
                CellCharsSize = new Size(54, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "add_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 2, 23, 1),
                SourceCell = new Rect(20, 0, 54, 2),
                Width = AxisDimension.Auto(23),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 29,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "cancel_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 27, 536870911, 1),
                SourceCell = new Rect(20, 0, 54, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(24, 1),
                CellCharsSize = new Size(54, 2),
                MarginRight = 3,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 10, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 42,
                MarginBottom = 15,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(1, 18, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 15,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(3, 2, 12, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 40,
                MarginBottom = 13,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "project_status",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(3, 18, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 13,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "owner_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(5, 2, 11, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 41,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "owner",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(5, 18, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "priority_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(7, 2, 14, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Auto(14),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 38,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "priority",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(7, 18, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "note_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(9, 2, 11, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 41,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "note",
                Type = "textarea",
                CellName = "",
                Relative = new Rect(9, 18, 536870911, 5),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(33, 5),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "message",
                Type = "label",
                CellName = "",
                Relative = new Rect(15, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 17),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(17),
                CharsSize = new Size(49, 1),
                CellCharsSize = new Size(54, 17),
                MarginRight = 3,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24")
    },
            },
        };
    }

    protected virtual void onNameChange(string value) { }
    protected virtual void onNameSubmit(string value) { }
    protected virtual void onProjectStatusChange(string value) { }
    protected virtual void onOwnerChange(string value) { }
    protected virtual void onPriorityChange(string value) { }
    protected virtual void onNoteChange(string value) { }
    protected virtual void onNoteSubmit(string value) { }
    protected virtual void onAddBtnClick() { }
    protected virtual void onCancelBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "add_btn")
        {
            onAddBtnClick();
            return true;
        }
        else if (name == "cancel_btn")
        {
            onCancelBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "name")
        {
            onNameChange(value);
            return true;
        }
        else if (name == "note")
        {
            onNoteChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "name")
        {
            onNameSubmit(value);
            return true;
        }
        else if (name == "note")
        {
            onNoteSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "project_status")
        {
            onProjectStatusChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "owner")
        {
            onOwnerChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "priority")
        {
            onPriorityChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}

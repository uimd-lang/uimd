// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
using Uimd;

public class SettingsUI : GeneratedWindowBase
{
    public Label header_title = null!;
    public CheckBox auto_scroll = null!;
    public CheckBox show_timestamps = null!;
    public Label default_type_label = null!;
    public ComboBox default_type = null!;
    public Button save_button = null!;
    public Button cancel_button = null!;

    public SettingsUI() : base("Activity Feed Settings")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "Settings", """
# Activity Feed Settings

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Settings dialog for activity feed auto-scroll, timestamps, and default activity type."
tags: [example]
```

## Members

```yaml
header_title:
  type: label
  text: "Activity Feed Settings"
  description: "Dialog title."
  expose: false

auto_scroll:
  type: checkbox
  title: "Auto-scroll to newest"
  value: 1
  description: "Toggle whether new activity items scroll into view."

show_timestamps:
  type: checkbox
  title: "Show timestamps"
  value: 1
  description: "Toggle whether newly added activity items include timestamps."

default_type_label:
  type: label
  text: "Default type"
  description: "Label for the default activity type selector."
  expose: false

default_type:
  type: combobox
  options: [Info, Task, Warning, Deploy, Note]
  selected_item: Info
  description: "Default activity type used by the main window."

save_button:
  type: button
  title: Save
  description: "Apply settings and close the dialog."

cancel_button:
  type: button
  title: Cancel
  description: "Close the dialog without applying settings."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  background: "#1f2937"
@header_title:
  text-align: center
  color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
combobox:
  background: "#0f172a"
  color: "#e5e7eb"
  focus-background: "#1e293b"
  edit-background: "#111827"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
checkbox:
  color: "#d1d5db"
  checked-color: "#22c55e"
  unchecked-color: "#f97316"
@buttons:
  padding: 0, 2, 1, 2
```

## User Interface

```ui
+-header-----------------------------------+
|header_title..**..........................|
+-content----------------------------------+
|                                          |
|  auto_scroll...........................  |
|                                          |
|  show_timestamps.......................  |
|                                          |
|  default_type_label..  default_type....  |
|                                          |
+-buttons----------------------------------+
|  save_button......  cancel_button......  |
+------------------------------------------+
```

""", "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.", new[]
        {
            new GeneratedElementMetadata { Name = "header_title", Description = "Dialog title.", Expose = false },
            new GeneratedElementMetadata { Name = "auto_scroll", Description = "Toggle whether new activity items scroll into view.", Expose = true },
            new GeneratedElementMetadata { Name = "show_timestamps", Description = "Toggle whether newly added activity items include timestamps.", Expose = true },
            new GeneratedElementMetadata { Name = "default_type_label", Description = "Label for the default activity type selector.", Expose = false },
            new GeneratedElementMetadata { Name = "default_type", Description = "Default activity type used by the main window.", Expose = true },
            new GeneratedElementMetadata { Name = "save_button", Description = "Apply settings and close the dialog.", Expose = true },
            new GeneratedElementMetadata { Name = "cancel_button", Description = "Close the dialog without applying settings.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header_title = AddElement(new Label("header_title", "Activity Feed Settings"));
        header_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        auto_scroll = AddElement(new CheckBox("auto_scroll", "Auto-scroll to newest", true));
        auto_scroll.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        auto_scroll.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        auto_scroll.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        auto_scroll.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f97316")
    });
        auto_scroll.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        show_timestamps = AddElement(new CheckBox("show_timestamps", "Show timestamps", true));
        show_timestamps.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        show_timestamps.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        show_timestamps.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        show_timestamps.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f97316")
    });
        show_timestamps.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        default_type_label = AddElement(new Label("default_type_label", "Default type"));
        default_type_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        default_type_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        default_type = AddElement(new ComboBox("default_type", new[] {"Info", "Task", "Warning", "Deploy", "Note"}));
        default_type.SetSelectedIndex(0);
        default_type.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    });
        default_type.SetFocusStyle(new Style
    {
        Background = new Color("#1e293b"),
        Color = new Color("#ffffff")
    });
        default_type.SetEditStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#e5e7eb")
    });
        default_type.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        default_type.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        save_button = AddElement(new Button("save_button", "Save"));
        save_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        save_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        cancel_button = AddElement(new Button("cancel_button", "Cancel"));
        cancel_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        cancel_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        cancel_button.SetDisabledStyle(new Style
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
                Name = "header_title",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 42, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(42, 1),
                CellCharsSize = new Size(42, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_button",
                Type = "button",
                CellName = "buttons",
                Relative = new Rect(0, 2, 17, 1),
                SourceCell = new Rect(10, 0, 42, 1),
                Width = AxisDimension.Auto(17),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(42, 1),
                MarginRight = 23,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 2,
        PaddingRight = 2,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "cancel_button",
                Type = "button",
                CellName = "buttons",
                Relative = new Rect(0, 21, 536870911, 1),
                SourceCell = new Rect(10, 0, 42, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(19, 1),
                CellCharsSize = new Size(42, 1),
                MarginRight = 2,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 2,
        PaddingRight = 2,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "auto_scroll",
                Type = "checkbox",
                CellName = "content",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 42, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(38, 1),
                CellCharsSize = new Size(42, 7),
                MarginRight = 2,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "show_timestamps",
                Type = "checkbox",
                CellName = "content",
                Relative = new Rect(3, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 42, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(38, 1),
                CellCharsSize = new Size(42, 7),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "default_type_label",
                Type = "label",
                CellName = "content",
                Relative = new Rect(5, 2, 20, 1),
                SourceCell = new Rect(2, 0, 42, 7),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(42, 7),
                MarginRight = 20,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "default_type",
                Type = "combobox",
                CellName = "content",
                Relative = new Rect(5, 24, 536870911, 1),
                SourceCell = new Rect(2, 0, 42, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(42),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(16, 1),
                CellCharsSize = new Size(42, 7),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#e5e7eb")
    },
            },
        };
    }

    protected virtual void onAutoScrollChange(string value) { }
    protected virtual void onShowTimestampsChange(string value) { }
    protected virtual void onDefaultTypeChange(string value) { }
    protected virtual void onSaveButtonClick() { }
    protected virtual void onCancelButtonClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "save_button")
        {
            onSaveButtonClick();
            return true;
        }
        else if (name == "cancel_button")
        {
            onCancelButtonClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "auto_scroll")
        {
            onAutoScrollChange(value);
            return true;
        }
        else if (name == "show_timestamps")
        {
            onShowTimestampsChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "default_type")
        {
            onDefaultTypeChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}

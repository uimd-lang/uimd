// Auto-generated UI code for widget_gallery - DO NOT EDIT MANUALLY.
using Uimd;

public class WidgetGalleryUI : GeneratedWindowBase
{
    public Label header = null!;
    public Button close_btn = null!;
    public Label plain_label = null!;
    public Label selectable_label = null!;
    public Label spans = null!;
    public Label animation = null!;
    public Label name_label = null!;
    public TextInput name_input = null!;
    public Label count_label = null!;
    public NumberInput count_input = null!;
    public CheckBox enabled_check = null!;
    public Label theme_label = null!;
    public ComboBox theme_combo = null!;
    public Label mode_label = null!;
    public ListBox mode_list = null!;
    public Button apply_btn = null!;
    public Button reset_btn = null!;
    public Label summary = null!;

    public WidgetGalleryUI() : base("Widget Gallery")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "WidgetGallery", """
# Widget Gallery

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons."
tags: [example]
```

## Members

```yaml
header:
  type: label
  text: "Widget Gallery"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the widget gallery."

plain_label:
  type: label
  text: "Plain label with inherited theme colors."
  description: "Plain label using inherited theme colors."

selectable_label:
  type: label
  text: "Select this text with the mouse to test label selection."
  description: "Selectable label used to test mouse text selection."

spans:
  type: spanlabel
  text: "span label"
  description: "Span label demonstrating mixed foreground and background styling."

animation:
  type: label
  text: "Animated label shows gradient-based rendering."
  description: "Animated gradient label."

name_label:
  type: label
  text: Name
  description: "Label for the name input."

name_input:
  type: textinput
  value: "Ada Lovelace"
  maxlength: 80
  description: "Editable name text input."

count_label:
  type: label
  text: Count
  description: "Label for the count input."

count_input:
  type: numberinput
  value: 3
  step_size: 1
  min_value: 0
  max_value: 99
  format_str: 0
  description: "Numeric count input with min, max, and step constraints."

enabled_check:
  type: checkbox
  title: "Feature enabled"
  value: 1
  description: "Toggle for the feature enabled state."

theme_label:
  type: label
  text: Theme
  description: "Label for the theme selector."

theme_combo:
  type: combobox
  options: [Dark, Light, Solarized]
  selected_item: Dark
  description: "Combobox selecting a visual theme value."

mode_label:
  type: label
  text: Mode
  description: "Label for the mode list."

mode_list:
  type: listbox
  options: [Preview, Edit, Review, Publish, Archive]
  selected_items: [Preview]
  multiple: "false"
  description: "Single-select listbox choosing the current workflow mode."

apply_btn:
  type: button
  title: Apply
  description: "Apply the current widget values."

reset_btn:
  type: button
  title: Reset
  description: "Reset widget values to defaults."

summary:
  type: label
  text: "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"
  description: "Read-only summary of current widget values."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#18324f"
  color: "#ffffff"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@title:
  background: "#18324f"
@selectable_label:
  user-select: text
  color: "#a7f3d0"
@spans:
  user-select: text
  color: "#cbd5e1"
@animation:
  color: "#fbbf24"
  text-color-gradient:
    interval: 100
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
textinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
numberinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
checkbox:
  color: "#d1d5db"
  checked-color: "#22c55e"
  unchecked-color: "#f97316"
@summary:
  background: "#0d1524"
  color: "#dbeafe"
```

## User Interface

```ui
+-title------------------------------------------------+
| header................................ close_btn.....|
+-label_section----------------------------------------+
|                                                      |
|  plain_label.......................................  |
|                                                      |
|  selectable_label..................................  |
|                                                      |
|  spans.............................................  |
|                                                      |
|  animation.........................................  |
|                                                      |
+-control_section-----------+--section2----------------+
|  name_label               |  theme_label             |
|  name_input.............  |   theme_combo..........  |
|                           |                          |
|  count_label              |  mode_label              |
|  count_input............  |  mode_list.............  |
|                           |  ......................  |
|  enabled_check..........  |  ......................  |
|                           |  ......................  |
|                           |  ......................  |
+---------------------------+--------------------------+
|                                                      |
|  summary...........................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|  ..................................................  |
|                                                      |
|  apply_btn..............  reset_btn................  |
|                                                      |
+------------------------------------------------------+
```

""", "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Window title.", Expose = false },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the widget gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "plain_label", Description = "Plain label using inherited theme colors.", Expose = true },
            new GeneratedElementMetadata { Name = "selectable_label", Description = "Selectable label used to test mouse text selection.", Expose = true },
            new GeneratedElementMetadata { Name = "spans", Description = "Span label demonstrating mixed foreground and background styling.", Expose = true },
            new GeneratedElementMetadata { Name = "animation", Description = "Animated gradient label.", Expose = true },
            new GeneratedElementMetadata { Name = "name_label", Description = "Label for the name input.", Expose = true },
            new GeneratedElementMetadata { Name = "name_input", Description = "Editable name text input.", Expose = true },
            new GeneratedElementMetadata { Name = "count_label", Description = "Label for the count input.", Expose = true },
            new GeneratedElementMetadata { Name = "count_input", Description = "Numeric count input with min, max, and step constraints.", Expose = true },
            new GeneratedElementMetadata { Name = "enabled_check", Description = "Toggle for the feature enabled state.", Expose = true },
            new GeneratedElementMetadata { Name = "theme_label", Description = "Label for the theme selector.", Expose = true },
            new GeneratedElementMetadata { Name = "theme_combo", Description = "Combobox selecting a visual theme value.", Expose = true },
            new GeneratedElementMetadata { Name = "mode_label", Description = "Label for the mode list.", Expose = true },
            new GeneratedElementMetadata { Name = "mode_list", Description = "Single-select listbox choosing the current workflow mode.", Expose = true },
            new GeneratedElementMetadata { Name = "apply_btn", Description = "Apply the current widget values.", Expose = true },
            new GeneratedElementMetadata { Name = "reset_btn", Description = "Reset widget values to defaults.", Expose = true },
            new GeneratedElementMetadata { Name = "summary", Description = "Read-only summary of current widget values.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Widget Gallery"));
        header.SetStyle(new Style
    {
        Background = new Color("#18324f"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        close_btn = AddElement(new Button("close_btn", "Quit"));
        close_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#ffffff")
    });
        close_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        close_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        plain_label = AddElement(new Label("plain_label", "Plain label with inherited theme colors."));
        plain_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        plain_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        selectable_label = AddElement(new Label("selectable_label", "Select this text with the mouse to test label selection."));
        selectable_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#a7f3d0"),
        UserSelect = "text"
    });
        selectable_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        spans = AddElement(new Label("spans", "span label"));
        spans.SetStyle(new Style
    {
        Color = new Color("#cbd5e1"),
        UserSelect = "text"
    });
        animation = AddElement(new Label("animation", "Animated label shows gradient-based rendering."));
        animation.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 100,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        animation.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        name_label = AddElement(new Label("name_label", "Name"));
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
        name_input = AddElement(new TextInput("name_input", "Ada Lovelace", 80));
        name_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        name_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        name_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        name_input.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        name_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        name_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        count_label = AddElement(new Label("count_label", "Count"));
        count_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        count_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        count_input = AddElement(new NumberInput("count_input", 3.0, 1.0));
        count_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        count_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        count_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        count_input.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        count_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        enabled_check = AddElement(new CheckBox("enabled_check", "Feature enabled", true));
        enabled_check.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        enabled_check.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        enabled_check.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        enabled_check.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f97316")
    });
        enabled_check.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        theme_label = AddElement(new Label("theme_label", "Theme"));
        theme_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        theme_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        theme_combo = AddElement(new ComboBox("theme_combo", new[] {"Dark", "Light", "Solarized"}));
        theme_combo.SetSelectedIndex(0);
        theme_combo.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        theme_combo.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        theme_combo.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#e5e7eb")
    });
        theme_combo.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        theme_combo.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        mode_label = AddElement(new Label("mode_label", "Mode"));
        mode_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        mode_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        mode_list = AddElement(new ListBox("mode_list", new[] {"Preview", "Edit", "Review", "Publish", "Archive"}));
        mode_list.SetSelectedIndex(0);
        mode_list.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    });
        mode_list.SetFocusStyle(new Style
    {
        Background = new Color("#1d2f4d"),
        Color = new Color("#ffffff")
    });
        mode_list.SetEditStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#cbd5e1")
    });
        mode_list.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        mode_list.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        apply_btn = AddElement(new Button("apply_btn", "Apply"));
        apply_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        apply_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        apply_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        reset_btn = AddElement(new Button("reset_btn", "Reset"));
        reset_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        reset_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        reset_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        summary = AddElement(new Label("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true"));
        summary.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#dbeafe")
    });
        summary.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
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
                CellName = "title",
                Relative = new Rect(0, 1, 38, 1),
                SourceCell = new Rect(0, 0, 54, 1),
                Width = AxisDimension.Auto(38),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(38, 1),
                CellCharsSize = new Size(54, 1),
                MarginRight = 15,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#18324f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#18324f"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_label",
                Type = "label",
                CellName = "control_section",
                Relative = new Rect(0, 2, 536870911, 1),
                SourceCell = new Rect(12, 0, 27, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(27, 9),
                MarginRight = 15,
                MarginBottom = 8,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "theme_label",
                Type = "label",
                CellName = "section2",
                Relative = new Rect(0, 2, 536870911, 1),
                SourceCell = new Rect(12, 28, 26, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(26, 9),
                MarginRight = 13,
                MarginBottom = 8,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "title",
                Relative = new Rect(0, 40, 536870911, 1),
                SourceCell = new Rect(0, 0, 54, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(54, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#18324f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_input",
                Type = "textinput",
                CellName = "control_section",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(12, 0, 27, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(27, 9),
                MarginRight = 2,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "plain_label",
                Type = "label",
                CellName = "label_section",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(50, 1),
                CellCharsSize = new Size(54, 9),
                MarginRight = 2,
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
                Name = "summary",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 536870911, 6),
                SourceCell = new Rect(22, 0, 54, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(6),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(10),
                CharsSize = new Size(50, 6),
                CellCharsSize = new Size(54, 10),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#dbeafe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "theme_combo",
                Type = "combobox",
                CellName = "section2",
                Relative = new Rect(1, 3, 536870911, 1),
                SourceCell = new Rect(12, 28, 26, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(26, 9),
                MarginRight = 2,
                MarginBottom = 7,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "count_label",
                Type = "label",
                CellName = "control_section",
                Relative = new Rect(3, 2, 536870911, 1),
                SourceCell = new Rect(12, 0, 27, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(27, 9),
                MarginRight = 14,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "mode_label",
                Type = "label",
                CellName = "section2",
                Relative = new Rect(3, 2, 536870911, 1),
                SourceCell = new Rect(12, 28, 26, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(26, 9),
                MarginRight = 14,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "selectable_label",
                Type = "label",
                CellName = "label_section",
                Relative = new Rect(3, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(50, 1),
                CellCharsSize = new Size(54, 9),
                MarginRight = 2,
                MarginBottom = 5,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#a7f3d0"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "count_input",
                Type = "numberinput",
                CellName = "control_section",
                Relative = new Rect(4, 2, 536870911, 1),
                SourceCell = new Rect(12, 0, 27, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(27, 9),
                MarginRight = 2,
                MarginBottom = 4,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "mode_list",
                Type = "listbox",
                CellName = "section2",
                Relative = new Rect(4, 2, 536870911, 5),
                SourceCell = new Rect(12, 28, 26, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(22, 5),
                CellCharsSize = new Size(26, 9),
                MarginRight = 2,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "spans",
                Type = "spanlabel",
                CellName = "label_section",
                Relative = new Rect(5, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(50, 1),
                CellCharsSize = new Size(54, 9),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Color = new Color("#cbd5e1"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "enabled_check",
                Type = "checkbox",
                CellName = "control_section",
                Relative = new Rect(6, 2, 536870911, 1),
                SourceCell = new Rect(12, 0, 27, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(27, 9),
                MarginRight = 2,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "animation",
                Type = "label",
                CellName = "label_section",
                Relative = new Rect(7, 2, 536870911, 1),
                SourceCell = new Rect(2, 0, 54, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(50, 1),
                CellCharsSize = new Size(54, 9),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#fbbf24"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 100,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "apply_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(8, 2, 23, 1),
                SourceCell = new Rect(22, 0, 54, 10),
                Width = AxisDimension.Auto(23),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(10),
                CharsSize = new Size(23, 1),
                CellCharsSize = new Size(54, 10),
                MarginRight = 29,
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
                Name = "reset_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(8, 27, 536870911, 1),
                SourceCell = new Rect(22, 0, 54, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(54),
                CellHeight = AxisDimension.Auto(10),
                CharsSize = new Size(25, 1),
                CellCharsSize = new Size(54, 10),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onCloseBtnClick() { }
    protected virtual void onNameInputChange(string value) { }
    protected virtual void onNameInputSubmit(string value) { }
    protected virtual void onCountInputChange(string value) { }
    protected virtual void onCountInputSubmit(string value) { }
    protected virtual void onEnabledCheckChange(string value) { }
    protected virtual void onThemeComboChange(string value) { }
    protected virtual void onModeListSelectionChange(List<string> value) { }
    protected virtual void onApplyBtnClick() { }
    protected virtual void onResetBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "close_btn")
        {
            onCloseBtnClick();
            return true;
        }
        else if (name == "apply_btn")
        {
            onApplyBtnClick();
            return true;
        }
        else if (name == "reset_btn")
        {
            onResetBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "name_input")
        {
            onNameInputChange(value);
            return true;
        }
        else if (name == "count_input")
        {
            onCountInputChange(value);
            return true;
        }
        else if (name == "enabled_check")
        {
            onEnabledCheckChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "name_input")
        {
            onNameInputSubmit(value);
            return true;
        }
        else if (name == "count_input")
        {
            onCountInputSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "theme_combo")
        {
            onThemeComboChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "mode_list")
        {
            onModeListSelectionChange(value);
            return true;
        }
        return false;
    }
}

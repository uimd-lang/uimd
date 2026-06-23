// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
using Uimd;

public class SpecialElementsUI : GeneratedWindowBase
{
    public Label header_label = null!;
    public Label info_processing = null!;
    public Label info_loading = null!;
    public Label info_complete = null!;
    public Label info_warning = null!;
    public Label info_multiline = null!;
    public Label status_label = null!;
    public Label normal_info = null!;
    public Label speed_label = null!;
    public Label raw_label = null!;
    public Button raw_button = null!;
    public ComboBox raw_combo = null!;
    public ListBox raw_list = null!;
    public TextInput raw_input = null!;
    public TextArea raw_area = null!;

    public SpecialElementsUI() : base("Special Elements Demo")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "SpecialElements", """
# Special Elements Demo

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text."
tags: [example]
```

## Members

```yaml
header_label:
  type: label
  text: "Special UI Elements"
  description: "Window title."

info_processing:
  type: label
  text: "Processing request..."
  color: "#ff0000"
  description: "Animated processing status label."

info_loading:
  type: label
  text: "Loading data from server..."
  color: "#00ff00"
  description: "Animated loading status label."

info_complete:
  type: label
  text: "Operation completed successfully!"
  color: "#0000ff"
  description: "Animated completed status label."

info_warning:
  type: label
  text: "Warning: Low disk space detected"
  color: "#ffffff"
  description: "Animated warning status label."

info_multiline:
  type: label
  text: "Line one of animated text\nLine two with different phase\nLine three final row"
  description: "Multiline animated label with different text rows."

status_label:
  type: label
  text: "Status: Idle"
  description: "Current status text."

normal_info:
  type: label
  text: "Regular static label for comparison"
  description: "Regular static comparison label."

speed_label:
  type: label
  text: "Fast animation speed demo"
  description: "Fast animation speed demonstration label."

raw_label:
  type: label
  text: "Label tab\tcontrol\x1b bell\x07 combining e\u0301 wide\u4e2d"
  user-select: text
  description: "Label containing tab, control, combining, and wide characters."

raw_button:
  type: button
  title: "Button\tESC\x1bWide\u4e2d"
  description: "Button title containing raw terminal-sensitive characters."

raw_combo:
  type: combobox
  options: ["Plain option", "Tab\toption", "Esc\x1boption", "Wide\u4e2doption"]
  selected_item: "Tab\toption"
  description: "ComboBox options containing raw terminal-sensitive characters."

raw_list:
  type: listbox
  options: ["Plain row", "Tab\trow", "Esc\x1brow", "Wide\u4e2drow"]
  selected_items: ["Tab\trow"]
  description: "ListBox options containing raw terminal-sensitive characters."

raw_input:
  type: textinput
  value: "Input\tvalue\x1bwide\u4e2d"
  maxlength: 80
  description: "Text input containing raw terminal-sensitive characters."

raw_area:
  type: textarea
  value: "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\x1bplaceholder\nWide\u4e2dplaceholder"
  maxlength: 240
  description: "Text area containing multiline raw terminal-sensitive characters."
```

## Style

```yaml
include: dark
@info_processing:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_loading:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_complete:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_warning:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@info_multiline:
  text-color-gradient:
    interval: 70
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
@speed_label:
  text-color-gradient:
    interval: 35
    step: 1
    segment-size: 1
    colors: ["#00000099", "#00000066", "#00000033", "#00000011", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000000", "#00000011", "#00000033", "#00000066", "#00000099"]
textinput:
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  selected-background: "#2563eb"
  selected-color: "#ffffff"
listbox:
  selected-background: "#2563eb"
  selected-color: "#ffffff"
```

## User Interface

```ui
+--------------------------------------------------------------------+
|                                                                    |
|  header_label....................................................  |
|                                                                    |
|  info_processing................  info_loading...................  |
|  info_complete..................  info_warning...................  |
|                                                                    |
|  "Multiline animated text:"                                         |
|  info_multiline..................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
|  status_label...................  normal_info....................  |
|  speed_label.....................................................  |
|                                                                    |
|  "Raw read-only text:"                                             |
|  raw_label.......................................................  |
|                                                                    |
|  "Raw button:" raw_button................                          |
|                                                                    |
|  "Raw combo:"  raw_combo......................                     |
|                                                                    |
|  "Raw list:"                                                       |
|  raw_list........................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
|  "Text input:" raw_input.........................................  |
|                                                                    |
|  "Text area:"                                                      |
|  raw_area........................................................  |
|  ................................................................  |
|  ................................................................  |
|  ................................................................  |
|                                                                    |
+--------------------------------------------------------------------+
```

""", "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.", new[]
        {
            new GeneratedElementMetadata { Name = "header_label", Description = "Window title.", Expose = true },
            new GeneratedElementMetadata { Name = "info_processing", Description = "Animated processing status label.", Expose = true },
            new GeneratedElementMetadata { Name = "info_loading", Description = "Animated loading status label.", Expose = true },
            new GeneratedElementMetadata { Name = "info_complete", Description = "Animated completed status label.", Expose = true },
            new GeneratedElementMetadata { Name = "info_warning", Description = "Animated warning status label.", Expose = true },
            new GeneratedElementMetadata { Name = "info_multiline", Description = "Multiline animated label with different text rows.", Expose = true },
            new GeneratedElementMetadata { Name = "status_label", Description = "Current status text.", Expose = true },
            new GeneratedElementMetadata { Name = "normal_info", Description = "Regular static comparison label.", Expose = true },
            new GeneratedElementMetadata { Name = "speed_label", Description = "Fast animation speed demonstration label.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_label", Description = "Label containing tab, control, combining, and wide characters.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_button", Description = "Button title containing raw terminal-sensitive characters.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_combo", Description = "ComboBox options containing raw terminal-sensitive characters.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_list", Description = "ListBox options containing raw terminal-sensitive characters.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_input", Description = "Text input containing raw terminal-sensitive characters.", Expose = true },
            new GeneratedElementMetadata { Name = "raw_area", Description = "Text area containing multiline raw terminal-sensitive characters.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header_label = AddElement(new Label("header_label", "Special UI Elements"));
        header_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        header_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        info_processing = AddElement(new Label("info_processing", "Processing request..."));
        info_processing.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ff0000"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        info_processing.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        info_loading = AddElement(new Label("info_loading", "Loading data from server..."));
        info_loading.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#00ff00"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        info_loading.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        info_complete = AddElement(new Label("info_complete", "Operation completed successfully!"));
        info_complete.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#0000ff"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        info_complete.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        info_warning = AddElement(new Label("info_warning", "Warning: Low disk space detected"));
        info_warning.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        info_warning.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        info_multiline = AddElement(new Label("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row"));
        info_multiline.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        info_multiline.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        status_label = AddElement(new Label("status_label", "Status: Idle"));
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
        normal_info = AddElement(new Label("normal_info", "Regular static label for comparison"));
        normal_info.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        normal_info.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        speed_label = AddElement(new Label("speed_label", "Fast animation speed demo"));
        speed_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 35,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    });
        speed_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        raw_label = AddElement(new Label("raw_label", "Label tab\tcontrol bell combining é wide中"));
        raw_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        raw_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        raw_button = AddElement(new Button("raw_button", "Button\tESCWide中"));
        raw_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        raw_button.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        raw_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        raw_combo = AddElement(new ComboBox("raw_combo", new[] {"Plain option", "Tab\toption", "Escoption", "Wide中option"}));
        raw_combo.SetSelectedIndex(1);
        raw_combo.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        raw_combo.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        raw_combo.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#e5e7eb")
    });
        raw_combo.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        raw_combo.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        raw_list = AddElement(new ListBox("raw_list", new[] {"Plain row", "Tab\trow", "Escrow", "Wide中row"}));
        raw_list.SetSelectedIndex(1);
        raw_list.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    });
        raw_list.SetFocusStyle(new Style
    {
        Background = new Color("#1d2f4d"),
        Color = new Color("#ffffff")
    });
        raw_list.SetEditStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#cbd5e1")
    });
        raw_list.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        raw_list.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        raw_input = AddElement(new TextInput("raw_input", "Input\tvaluewide中", 80));
        raw_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        raw_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        raw_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        raw_input.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        raw_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        raw_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        raw_area = AddElement(new TextArea("raw_area", "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControlplaceholder\nWide中placeholder", 240));
        raw_area.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        raw_area.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        raw_area.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        raw_area.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        raw_area.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        raw_area.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "header_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 32,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "info_processing",
                Type = "label",
                CellName = "",
                Relative = new Rect(3, 2, 31, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(31),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 35,
                MarginBottom = 30,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "info_loading",
                Type = "label",
                CellName = "",
                Relative = new Rect(3, 35, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 30,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "info_complete",
                Type = "label",
                CellName = "",
                Relative = new Rect(4, 2, 31, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(31),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 35,
                MarginBottom = 29,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "info_warning",
                Type = "label",
                CellName = "",
                Relative = new Rect(4, 35, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 29,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Multiline animated text:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(6, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 40,
                MarginBottom = 27,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "info_multiline",
                Type = "label",
                CellName = "",
                Relative = new Rect(7, 2, 536870911, 3),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 3),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 24,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 70,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(11, 2, 31, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(31),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 35,
                MarginBottom = 22,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "normal_info",
                Type = "label",
                CellName = "",
                Relative = new Rect(11, 35, 536870911, 2),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 2),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 21,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "speed_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(12, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 21,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextColorGradient = new TextGradient
    {
        IntervalMs = 35,
        Step = 1,
        SegmentSize = 1,
        Colors = {new Color("#00000099"), new Color("#00000066"), new Color("#00000033"), new Color("#00000011"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000000"), new Color("#00000011"), new Color("#00000033"), new Color("#00000066"), new Color("#00000099")}
    }
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Raw read-only text:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(14, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(21, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 45,
                MarginBottom = 19,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "raw_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(15, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 18,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Raw button:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(17, 2, 13, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 53,
                MarginBottom = 16,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "raw_button",
                Type = "button",
                CellName = "",
                Relative = new Rect(17, 16, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 26,
                MarginBottom = 16,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Raw combo:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(19, 2, 12, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 54,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "raw_combo",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(19, 16, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 21,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Raw list:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(21, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 55,
                MarginBottom = 12,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "raw_list",
                Type = "listbox",
                CellName = "",
                Relative = new Rect(22, 2, 536870911, 3),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 3),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Text input:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(26, 2, 13, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 53,
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
                Name = "raw_input",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(26, 16, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(50, 1),
                CellCharsSize = new Size(68, 34),
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
                Name = "\"Text area:\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(28, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(68, 34),
                MarginRight = 54,
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
                Name = "raw_area",
                Type = "textarea",
                CellName = "",
                Relative = new Rect(29, 2, 536870911, 4),
                SourceCell = new Rect(0, 0, 68, 34),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(4),
                CellWidth = AxisDimension.Auto(68),
                CellHeight = AxisDimension.Auto(34),
                CharsSize = new Size(64, 4),
                CellCharsSize = new Size(68, 34),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style(),
            },
        };
    }

    protected virtual void onRawButtonClick() { }
    protected virtual void onRawComboChange(string value) { }
    protected virtual void onRawListSelectionChange(List<string> value) { }
    protected virtual void onRawInputChange(string value) { }
    protected virtual void onRawInputSubmit(string value) { }
    protected virtual void onRawAreaChange(string value) { }
    protected virtual void onRawAreaSubmit(string value) { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "raw_button")
        {
            onRawButtonClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "raw_input")
        {
            onRawInputChange(value);
            return true;
        }
        else if (name == "raw_area")
        {
            onRawAreaChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "raw_input")
        {
            onRawInputSubmit(value);
            return true;
        }
        else if (name == "raw_area")
        {
            onRawAreaSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "raw_combo")
        {
            onRawComboChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "raw_list")
        {
            onRawListSelectionChange(value);
            return true;
        }
        return false;
    }
}

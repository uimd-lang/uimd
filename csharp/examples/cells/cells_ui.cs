// Auto-generated UI code for cells - DO NOT EDIT MANUALLY.
using Uimd;

public class CellsUI : GeneratedWindowBase
{
    public Button a1 = null!;
    public Button a2 = null!;
    public Button b1 = null!;
    public Button c1 = null!;
    public Button c2 = null!;
    public Button c3 = null!;
    public Button c4 = null!;
    public Button c5 = null!;
    public Button c6 = null!;
    public Button c7 = null!;
    public Button c8 = null!;
    public Button c9 = null!;
    public Button c10 = null!;
    public Button d1 = null!;
    public Button d2 = null!;
    public Button d3 = null!;
    public Button d4 = null!;
    public Button d5 = null!;
    public Button d6 = null!;
    public Button e1 = null!;
    public Button e2 = null!;
    public Button e3 = null!;
    public Button e4 = null!;
    public Button e5 = null!;
    public Button e6 = null!;
    public Button e7 = null!;
    public Button e8 = null!;
    public Button f1 = null!;
    public Button f2 = null!;
    public Button f3 = null!;

    public CellsUI() : base("Cells")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "Cells", """
# Cells

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior."
tags: [example]
```

## Members

```yaml
a1:
  type: button
  title: A1
  description: "Button in the first red cell."

a2:
  type: button
  title: A2
  description: "Second button in the first red cell."

b1:
  type: button
  title: BBB
  description: "Wide button in the green cell."

c1:
  type: button
  title: C1
  description: "Button in the yellow cell group."

c2:
  type: button
  title: C2
  description: "Button in the yellow cell group."

c3:
  type: button
  title: C3
  description: "Button in the yellow cell group."

c4:
  type: button
  title: C4
  description: "Button in the yellow cell group."

c5:
  type: button
  title: C5
  description: "Button in the yellow cell group."

c6:
  type: button
  title: C6
  description: "Button in the yellow cell group."

c7:
  type: button
  title: C7
  description: "Button in the yellow cell group."

c8:
  type: button
  title: C8
  description: "Button in the yellow cell group."

c9:
  type: button
  title: C9
  description: "Button in the yellow cell group."

c10:
  type: button
  title: C10
  description: "Button in the yellow cell group."

d1:
  type: button
  title: D1
  description: "Top button in the right column."

d2:
  type: button
  title: D2
  description: "Button in the right column."

d3:
  type: button
  title: D3
  description: "Button in the right column."

d4:
  type: button
  title: D4
  description: "Button in the right column."

d5:
  type: button
  title: D5
  description: "Button in the right column."

d6:
  type: button
  title: D6
  description: "Bottom button in the right column."

e1:
  type: button
  title: E
  description: "Single-character E button in the lower left cell."

e2:
  type: button
  title: EE
  description: "Two-character E button in the lower left cell."

e3:
  type: button
  title: EEE
  description: "Three-character E button in the lower left cell."

e4:
  type: button
  title: EEEE
  description: "Four-character E button in the lower left cell."

e5:
  type: button
  title: EEEEE
  description: "Five-character E button in the lower left cell."

e6:
  type: button
  title: EEEEEE
  description: "Six-character E button in the lower left cell."

e7:
  type: button
  title: EEEEEEE
  description: "Seven-character E button in the lower left cell."

e8:
  type: button
  title: EEEEEEEE
  description: "Eight-character E button in the lower left cell."

f1:
  type: button
  title: F1
  description: "Top button in the cyan cell."

f2:
  type: button
  title: F2
  description: "Middle button in the cyan cell with a local style override."

f3:
  type: button
  title: F3
  description: "Bottom button in the cyan cell."
```

## Style

```yaml
this:
  background: "#1f2946"
  border-color: transparent
  border-width: 1
  border-width-vertical: 2
button:
  background: "#526173"
  color: "#eef2f7"
  focus-background: "#6b7c91"
@d1:
  color: "#d7fbe8"
cell1:
  background: "#3b1f2d"
  button:
    background: "#7a3f59"
    focus-background: "#96536d"
cell2:
  background: "#1f463f"
  button:
    background: "#3d7b6d"
    focus-background: "#4f9484"
cl3:
  background: "#26345f"
  button:
    background: "#5267a3"
    focus-background: "#657abd"
cell4:
  background: "#4a4422"
  button:
    background: "#81753b"
    color: "#ecfdf5"
    focus-background: "#9a8d4a"
cell5:
  background: "#482456"
  button:
    background: "#805196"
    focus-background: "#9a65b0"
cl6:
  background: "#174f55"
  button:
    background: "#3a8890"
    focus-background: "#4aa1aa"
  @f2:
    background: "#31565f"
cl7:
  background: "#2f3745"
  button:
    background: "#64748b"
    focus-background: "#7b8ba0"
```

## User Interface

```ui
+-cell1-----------+-cell2---+-cl3+
| a1..... a2..... |  b1...  | d1 |
+-cell4-----------+---------+    |
| c1.. c2.. c3.. c4.. c5..  | d2 |
| c6.. c7.. c8.. c9.. c10.  |    |
+-cell5---------------+-cl6-+ d3 |
|                     | f1. |    |
| e1....... e5....... |     | d4 |
|                     |     |    |
| e2....... e6....... | f2. | d5 |
|                     |     |    |
| e3....... e7....... |     | d6 |
|                     | f3. +-cl7+
| e4....... e8....... |     |    |
|                     |     |    |
+---------------------+-----+----+
```

""", "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.", new[]
        {
            new GeneratedElementMetadata { Name = "a1", Description = "Button in the first red cell.", Expose = true },
            new GeneratedElementMetadata { Name = "a2", Description = "Second button in the first red cell.", Expose = true },
            new GeneratedElementMetadata { Name = "b1", Description = "Wide button in the green cell.", Expose = true },
            new GeneratedElementMetadata { Name = "c1", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c2", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c3", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c4", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c5", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c6", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c7", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c8", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c9", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "c10", Description = "Button in the yellow cell group.", Expose = true },
            new GeneratedElementMetadata { Name = "d1", Description = "Top button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "d2", Description = "Button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "d3", Description = "Button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "d4", Description = "Button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "d5", Description = "Button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "d6", Description = "Bottom button in the right column.", Expose = true },
            new GeneratedElementMetadata { Name = "e1", Description = "Single-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e2", Description = "Two-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e3", Description = "Three-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e4", Description = "Four-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e5", Description = "Five-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e6", Description = "Six-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e7", Description = "Seven-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "e8", Description = "Eight-character E button in the lower left cell.", Expose = true },
            new GeneratedElementMetadata { Name = "f1", Description = "Top button in the cyan cell.", Expose = true },
            new GeneratedElementMetadata { Name = "f2", Description = "Middle button in the cyan cell with a local style override.", Expose = true },
            new GeneratedElementMetadata { Name = "f3", Description = "Bottom button in the cyan cell.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#1f2946"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 1,
        BorderWidthVertical = 2
    });
        a1 = AddElement(new Button("a1", "A1"));
        a1.SetStyle(new Style
    {
        Background = new Color("#7a3f59"),
        Color = new Color("#eef2f7")
    });
        a1.SetFocusStyle(new Style
    {
        Background = new Color("#96536d"),
        Color = new Color("#ffffff")
    });
        a1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        a2 = AddElement(new Button("a2", "A2"));
        a2.SetStyle(new Style
    {
        Background = new Color("#7a3f59"),
        Color = new Color("#eef2f7")
    });
        a2.SetFocusStyle(new Style
    {
        Background = new Color("#96536d"),
        Color = new Color("#ffffff")
    });
        a2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        b1 = AddElement(new Button("b1", "BBB"));
        b1.SetStyle(new Style
    {
        Background = new Color("#3d7b6d"),
        Color = new Color("#eef2f7")
    });
        b1.SetFocusStyle(new Style
    {
        Background = new Color("#4f9484"),
        Color = new Color("#ffffff")
    });
        b1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c1 = AddElement(new Button("c1", "C1"));
        c1.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c1.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c2 = AddElement(new Button("c2", "C2"));
        c2.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c2.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c3 = AddElement(new Button("c3", "C3"));
        c3.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c3.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c3.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c4 = AddElement(new Button("c4", "C4"));
        c4.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c4.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c4.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c5 = AddElement(new Button("c5", "C5"));
        c5.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c5.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c5.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c6 = AddElement(new Button("c6", "C6"));
        c6.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c6.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c6.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c7 = AddElement(new Button("c7", "C7"));
        c7.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c7.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c7.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c8 = AddElement(new Button("c8", "C8"));
        c8.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c8.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c8.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c9 = AddElement(new Button("c9", "C9"));
        c9.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c9.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c9.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        c10 = AddElement(new Button("c10", "C10"));
        c10.SetStyle(new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    });
        c10.SetFocusStyle(new Style
    {
        Background = new Color("#9a8d4a"),
        Color = new Color("#ffffff")
    });
        c10.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d1 = AddElement(new Button("d1", "D1"));
        d1.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#d7fbe8")
    });
        d1.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d2 = AddElement(new Button("d2", "D2"));
        d2.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    });
        d2.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d3 = AddElement(new Button("d3", "D3"));
        d3.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    });
        d3.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d3.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d4 = AddElement(new Button("d4", "D4"));
        d4.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    });
        d4.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d4.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d5 = AddElement(new Button("d5", "D5"));
        d5.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    });
        d5.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d5.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        d6 = AddElement(new Button("d6", "D6"));
        d6.SetStyle(new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    });
        d6.SetFocusStyle(new Style
    {
        Background = new Color("#657abd"),
        Color = new Color("#ffffff")
    });
        d6.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e1 = AddElement(new Button("e1", "E"));
        e1.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e1.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e2 = AddElement(new Button("e2", "EE"));
        e2.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e2.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e3 = AddElement(new Button("e3", "EEE"));
        e3.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e3.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e3.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e4 = AddElement(new Button("e4", "EEEE"));
        e4.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e4.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e4.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e5 = AddElement(new Button("e5", "EEEEE"));
        e5.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e5.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e5.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e6 = AddElement(new Button("e6", "EEEEEE"));
        e6.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e6.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e6.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e7 = AddElement(new Button("e7", "EEEEEEE"));
        e7.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e7.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e7.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        e8 = AddElement(new Button("e8", "EEEEEEEE"));
        e8.SetStyle(new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    });
        e8.SetFocusStyle(new Style
    {
        Background = new Color("#9a65b0"),
        Color = new Color("#ffffff")
    });
        e8.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        f1 = AddElement(new Button("f1", "F1"));
        f1.SetStyle(new Style
    {
        Background = new Color("#3a8890"),
        Color = new Color("#eef2f7")
    });
        f1.SetFocusStyle(new Style
    {
        Background = new Color("#4aa1aa"),
        Color = new Color("#ffffff")
    });
        f1.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        f2 = AddElement(new Button("f2", "F2"));
        f2.SetStyle(new Style
    {
        Background = new Color("#31565f"),
        Color = new Color("#eef2f7")
    });
        f2.SetFocusStyle(new Style
    {
        Background = new Color("#4aa1aa"),
        Color = new Color("#ffffff")
    });
        f2.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        f3 = AddElement(new Button("f3", "F3"));
        f3.SetStyle(new Style
    {
        Background = new Color("#3a8890"),
        Color = new Color("#eef2f7")
    });
        f3.SetFocusStyle(new Style
    {
        Background = new Color("#4aa1aa"),
        Color = new Color("#ffffff")
    });
        f3.SetDisabledStyle(new Style
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
                Name = "",
                Type = "",
                CellName = "cl7",
                Relative = new Rect(0, 0, 4, 2),
                SourceCell = new Rect(12, 28, 4, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 2),
                CellCharsSize = new Size(4, 2),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#2f3745")
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "a1",
                Type = "button",
                CellName = "cell1",
                Relative = new Rect(0, 1, 7, 1),
                SourceCell = new Rect(0, 0, 17, 1),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(17),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(7, 1),
                CellCharsSize = new Size(17, 1),
                MarginRight = 9,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#3b1f2d")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7a3f59"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c1",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(0, 1, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 22,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d1",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 10,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#d7fbe8")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "f1",
                Type = "button",
                CellName = "cl6",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(5, 22, 5, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(5),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(5, 9),
                MarginRight = 1,
                MarginBottom = 8,
                CellStyle = new Style
    {
        Background = new Color("#174f55")
    },
                ElementStyle = new Style
    {
        Background = new Color("#3a8890"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "b1",
                Type = "button",
                CellName = "cell2",
                Relative = new Rect(0, 2, 536870911, 1),
                SourceCell = new Rect(0, 18, 9, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(9),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(5, 1),
                CellCharsSize = new Size(9, 1),
                MarginRight = 2,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f463f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#3d7b6d"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c2",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(0, 6, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 17,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "a2",
                Type = "button",
                CellName = "cell1",
                Relative = new Rect(0, 9, 536870911, 1),
                SourceCell = new Rect(0, 0, 17, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(17),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(7, 1),
                CellCharsSize = new Size(17, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#3b1f2d")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7a3f59"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c3",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(0, 11, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 12,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c4",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(0, 16, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 7,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c5",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(0, 21, 536870911, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c6",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(1, 1, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 22,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e1",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(1, 1, 9, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 11,
                MarginBottom = 7,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c7",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(1, 6, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 17,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c8",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(1, 11, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 12,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e5",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(1, 11, 536870911, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 1,
                MarginBottom = 7,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c9",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(1, 16, 4, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Auto(4),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 7,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "c10",
                Type = "button",
                CellName = "cell4",
                Relative = new Rect(1, 21, 536870911, 1),
                SourceCell = new Rect(2, 0, 27, 2),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(27),
                CellHeight = AxisDimension.Auto(2),
                CharsSize = new Size(4, 1),
                CellCharsSize = new Size(27, 2),
                MarginRight = 2,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#4a4422")
    },
                ElementStyle = new Style
    {
        Background = new Color("#81753b"),
        Color = new Color("#ecfdf5")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d2",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(2, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 8,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e2",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(3, 1, 9, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 11,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "f2",
                Type = "button",
                CellName = "cl6",
                Relative = new Rect(3, 1, 536870911, 1),
                SourceCell = new Rect(5, 22, 5, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(5),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(5, 9),
                MarginRight = 1,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#174f55")
    },
                ElementStyle = new Style
    {
        Background = new Color("#31565f"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e6",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(3, 11, 536870911, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 1,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d3",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(4, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 6,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e3",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(5, 1, 9, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 11,
                MarginBottom = 3,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e7",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(5, 11, 536870911, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 1,
                MarginBottom = 3,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d4",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(6, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 4,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "f3",
                Type = "button",
                CellName = "cl6",
                Relative = new Rect(6, 1, 536870911, 1),
                SourceCell = new Rect(5, 22, 5, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(5),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(3, 1),
                CellCharsSize = new Size(5, 9),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style
    {
        Background = new Color("#174f55")
    },
                ElementStyle = new Style
    {
        Background = new Color("#3a8890"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e4",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(7, 1, 9, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 11,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "e8",
                Type = "button",
                CellName = "cell5",
                Relative = new Rect(7, 11, 536870911, 1),
                SourceCell = new Rect(5, 0, 21, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(9),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(21, 9),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#482456")
    },
                ElementStyle = new Style
    {
        Background = new Color("#805196"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d5",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(8, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "d6",
                Type = "button",
                CellName = "cl3",
                Relative = new Rect(10, 1, 536870911, 1),
                SourceCell = new Rect(0, 28, 4, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(4),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(2, 1),
                CellCharsSize = new Size(4, 11),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#26345f")
    },
                ElementStyle = new Style
    {
        Background = new Color("#5267a3"),
        Color = new Color("#eef2f7")
    },
            },
        };
    }

    protected virtual void onA1Click() { }
    protected virtual void onA2Click() { }
    protected virtual void onB1Click() { }
    protected virtual void onC1Click() { }
    protected virtual void onC2Click() { }
    protected virtual void onC3Click() { }
    protected virtual void onC4Click() { }
    protected virtual void onC5Click() { }
    protected virtual void onC6Click() { }
    protected virtual void onC7Click() { }
    protected virtual void onC8Click() { }
    protected virtual void onC9Click() { }
    protected virtual void onC10Click() { }
    protected virtual void onD1Click() { }
    protected virtual void onD2Click() { }
    protected virtual void onD3Click() { }
    protected virtual void onD4Click() { }
    protected virtual void onD5Click() { }
    protected virtual void onD6Click() { }
    protected virtual void onE1Click() { }
    protected virtual void onE2Click() { }
    protected virtual void onE3Click() { }
    protected virtual void onE4Click() { }
    protected virtual void onE5Click() { }
    protected virtual void onE6Click() { }
    protected virtual void onE7Click() { }
    protected virtual void onE8Click() { }
    protected virtual void onF1Click() { }
    protected virtual void onF2Click() { }
    protected virtual void onF3Click() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "a1")
        {
            onA1Click();
            return true;
        }
        else if (name == "a2")
        {
            onA2Click();
            return true;
        }
        else if (name == "b1")
        {
            onB1Click();
            return true;
        }
        else if (name == "c1")
        {
            onC1Click();
            return true;
        }
        else if (name == "c2")
        {
            onC2Click();
            return true;
        }
        else if (name == "c3")
        {
            onC3Click();
            return true;
        }
        else if (name == "c4")
        {
            onC4Click();
            return true;
        }
        else if (name == "c5")
        {
            onC5Click();
            return true;
        }
        else if (name == "c6")
        {
            onC6Click();
            return true;
        }
        else if (name == "c7")
        {
            onC7Click();
            return true;
        }
        else if (name == "c8")
        {
            onC8Click();
            return true;
        }
        else if (name == "c9")
        {
            onC9Click();
            return true;
        }
        else if (name == "c10")
        {
            onC10Click();
            return true;
        }
        else if (name == "d1")
        {
            onD1Click();
            return true;
        }
        else if (name == "d2")
        {
            onD2Click();
            return true;
        }
        else if (name == "d3")
        {
            onD3Click();
            return true;
        }
        else if (name == "d4")
        {
            onD4Click();
            return true;
        }
        else if (name == "d5")
        {
            onD5Click();
            return true;
        }
        else if (name == "d6")
        {
            onD6Click();
            return true;
        }
        else if (name == "e1")
        {
            onE1Click();
            return true;
        }
        else if (name == "e2")
        {
            onE2Click();
            return true;
        }
        else if (name == "e3")
        {
            onE3Click();
            return true;
        }
        else if (name == "e4")
        {
            onE4Click();
            return true;
        }
        else if (name == "e5")
        {
            onE5Click();
            return true;
        }
        else if (name == "e6")
        {
            onE6Click();
            return true;
        }
        else if (name == "e7")
        {
            onE7Click();
            return true;
        }
        else if (name == "e8")
        {
            onE8Click();
            return true;
        }
        else if (name == "f1")
        {
            onF1Click();
            return true;
        }
        else if (name == "f2")
        {
            onF2Click();
            return true;
        }
        else if (name == "f3")
        {
            onF3Click();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        return false;
    }
}

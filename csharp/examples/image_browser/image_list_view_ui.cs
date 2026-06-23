// Auto-generated UI code for image_list_view - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageListViewUI : GeneratedWindowBase
{
    public Button add_btn = null!;
    public ReusableElement items = null!;

    public ImageListViewUI() : base("Image List View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "ImageListView", """
# Image List View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "ViewHost page with add button and scrollable image item list."
tags: [example, image, list]
```

## Definition

```yaml
uses:
  - image_list_scroll
```

## Members

```yaml
add_btn:
  type: button
  title: Add
  description: "Add an image item."

items:
  type: image_list_scroll
  description: "Scrollable list of image items."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@toolbar:
  background: "#030712"
@items_cell:
  background: "#030712"
button:
  background: "#14532d"
  color: "#e2e8f0"
  focus-background: "#16a34a"
```

## User Interface

```ui
+-toolbar-**------------------------------------------------------------+
| add_btn............                                                   |
+-items_cell-**---------------------------------------------------------+
|items..**..............................................................|
*.......................................................................|
**......................................................................|
|*......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
|.......................................................................|
+-----------------------------------------------------------------------+
```

""", "ViewHost page with add button and scrollable image item list.", new[]
        {
            new GeneratedElementMetadata { Name = "add_btn", Description = "Add an image item.", Expose = true },
            new GeneratedElementMetadata { Name = "items", Description = "Scrollable list of image items.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        add_btn = AddElement(new Button("add_btn", "Add"));
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
        items = AddElement(new ReusableElement("items"));
        items.SetChild(new ImageListScrollUI());
        items.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        items.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        items.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "items",
                Type = "image_list_scroll",
                CellName = "items_cell",
                Relative = new Rect(0, 0, 536870911, 536870911),
                SourceCell = new Rect(2, 0, 71, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(71, 11),
                CellCharsSize = new Size(71, 11),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712")
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "add_btn",
                Type = "button",
                CellName = "toolbar",
                Relative = new Rect(0, 1, 19, 1),
                SourceCell = new Rect(0, 0, 71, 1),
                Width = AxisDimension.Auto(19),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(19, 1),
                CellCharsSize = new Size(71, 1),
                MarginRight = 51,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712")
    },
                ElementStyle = new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onAddBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "add_btn")
        {
            onAddBtnClick();
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

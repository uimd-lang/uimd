// Auto-generated UI code for image_list_scroll - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageListScrollUI : GeneratedScrollViewBase
{

    public ImageListScrollUI() : base("Image List Scroll")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ImageListScroll", """
# Image List Scroll

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
description: "Scrollable image item list."
tags: [example, image, list]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - image_list_item
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
  focus-background: "#1e3a5f"
@panel:
  gap: 1
  padding: 1, 1, 1, 1
  background: "#030712"
```

## User Interface

```ui
+-panel-**----------------------------------------------------------------+
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
*                                                                         |
*                                                                         |
*                                                                         |
*                                                                         |
|                                                                         |
+-------------------------------------------------------------------------+
```

""", "Scrollable image item list.", Array.Empty<GeneratedElementMetadata>());
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        SetGeneratedScrollView(AddElement(new ScrollView("__scrollview", 1)));
        ScrollView().SetStyle(new Style
    {
        Background = new Color("#030712"),
        Gap = 1,
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    });
        ScrollView().SetFocusStyle(new Style
    {
        Background = new Color("#1e3a5f")
    });
        ScrollView().SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        ScrollView().SetSelectedStyle(new Style
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
                Name = "__scrollview",
                Type = "uiscrollview",
                CellName = "panel",
                Relative = new Rect(0, 0, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 73, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(73, 10),
                CellCharsSize = new Size(73, 10),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712"),
        Gap = 1
    },
                ElementStyle = new Style(),
            },
        };
    }

    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
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

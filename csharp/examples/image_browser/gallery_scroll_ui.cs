// Auto-generated UI code for gallery_scroll - DO NOT EDIT MANUALLY.
using Uimd;

public class GalleryScrollUI : GeneratedScrollViewBase
{

    public GalleryScrollUI() : base("Gallery Scroll")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "GalleryScroll", """
# Gallery Scroll

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Scrollable container for gallery image items."
tags: [example, image, gallery]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - gallery_item
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@panel:
  gap: 1
  background: "#030712"
```

## User Interface

```ui
+-panel-**-------------------------------+
|                                        |
*                                        |
*                                        |
|                                        |
|                                        |
|                                        |
|                                        |
+----------------------------------------+
```

""", "Scrollable container for gallery image items.", Array.Empty<GeneratedElementMetadata>());
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
        Gap = 1
    });
        ScrollView().SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
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
                SourceCell = new Rect(0, 0, 40, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(40, 7),
                CellCharsSize = new Size(40, 7),
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

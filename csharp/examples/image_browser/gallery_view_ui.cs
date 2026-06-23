// Auto-generated UI code for gallery_view - DO NOT EDIT MANUALLY.
using Uimd;

public class GalleryViewUI : GeneratedWindowBase
{
    public ReusableElement gallery_scroll = null!;

    public GalleryViewUI() : base("Gallery View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "GalleryView", """
# Gallery View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Gallery view hosting a scrollable list of full-size images."
tags: [example, image, gallery, viewhost]
```

## Definition

```yaml
extends: uicontrol
uses:
  - gallery_scroll
```

## Members

```yaml
gallery_scroll:
  type: gallery_scroll
  description: "Scrollable image list."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
```

## User Interface

```ui
+-view-**-------------------+
| gallery_scroll..**....... |
| ......................... |
| *........................ |
| *........................ |
| ......................... |
| ......................... |
+---------------------------+
```

""", "Gallery view hosting a scrollable list of full-size images.", new[]
        {
            new GeneratedElementMetadata { Name = "gallery_scroll", Description = "Scrollable image list.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        gallery_scroll = AddElement(new ReusableElement("gallery_scroll"));
        gallery_scroll.SetChild(new GalleryScrollUI());
        gallery_scroll.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        gallery_scroll.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        gallery_scroll.SetSelectedStyle(new Style
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
                Name = "gallery_scroll",
                Type = "gallery_scroll",
                CellName = "view",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 27, 6),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(6),
                CharsSize = new Size(25, 6),
                CellCharsSize = new Size(27, 6),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
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

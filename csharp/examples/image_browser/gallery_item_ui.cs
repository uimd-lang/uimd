// Auto-generated UI code for gallery_item - DO NOT EDIT MANUALLY.
using Uimd;

public class GalleryItemUI : GeneratedWindowBase
{
    public Label caption = null!;
    public Image photo = null!;

    public GalleryItemUI() : base("Gallery Item")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "GalleryItem", """
# Gallery Item

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Full-size image item for the gallery scroll view."
tags: [example, image, gallery]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
caption:
  type: label
  text: ""
  description: "Image title."

photo:
  type: image
  source: ""
  fit: contain
  render_mode: auto
  description: "Full-size image."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@caption:
  color: "#bfdbfe"
  text-align: center
image:
  background: transparent
  color: "#d1d5db"
```

## User Interface

```ui
+-view-**--------------------------+
| caption..**..................... |
| photo..**....................... |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
| ................................ |
+----------------------------------+
```

""", "Full-size image item for the gallery scroll view.", new[]
        {
            new GeneratedElementMetadata { Name = "caption", Description = "Image title.", Expose = true },
            new GeneratedElementMetadata { Name = "photo", Description = "Full-size image.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        caption = AddElement(new Label("caption", ""));
        caption.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe"),
        TextAlign = "center"
    });
        caption.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        photo = AddElement(new Image("photo", "", "", "contain", "auto", "center", "middle"));
        photo.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "caption",
                Type = "label",
                CellName = "view",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 34, 21),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(21),
                CharsSize = new Size(32, 1),
                CellCharsSize = new Size(34, 21),
                MarginRight = 1,
                MarginBottom = 20,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "photo",
                Type = "image",
                CellName = "view",
                Relative = new Rect(1, 1, 536870911, 20),
                SourceCell = new Rect(0, 0, 34, 21),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(20),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(21),
                CharsSize = new Size(32, 20),
                CellCharsSize = new Size(34, 21),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
        };
    }

    protected virtual void onPhotoClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "photo")
        {
            onPhotoClick();
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

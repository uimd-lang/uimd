// Auto-generated UI code for image_view - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageViewUI : GeneratedWindowBase
{
    public Label caption = null!;
    public Image photo = null!;

    public ImageViewUI() : base("Image View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "ImageView", """
# Image View

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Full-size image display panel hosted inside a ViewHost."
tags: [example, image, viewhost]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
caption:
  type: label
  text: "Select an image"
  description: "Name of the currently displayed image."

photo:
  type: image
  source: ""
  alt: "photo"
  fit: contain
  render_mode: auto
  description: "Full-size image display."
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
+-view-**-----------------------+
| caption..**.................. |
| photo..**.................... |
| ............................. |
| *............................ |
| *............................ |
| ............................. |
| ............................. |
| ............................. |
| ............................. |
| ............................. |
| ............................. |
| ............................. |
+-------------------------------+
```

""", "Full-size image display panel hosted inside a ViewHost.", new[]
        {
            new GeneratedElementMetadata { Name = "caption", Description = "Name of the currently displayed image.", Expose = true },
            new GeneratedElementMetadata { Name = "photo", Description = "Full-size image display.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        caption = AddElement(new Label("caption", "Select an image"));
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
        photo = AddElement(new Image("photo", "", "photo", "contain", "auto", "center", "middle"));
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
                SourceCell = new Rect(0, 0, 31, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(12),
                CharsSize = new Size(29, 1),
                CellCharsSize = new Size(31, 12),
                MarginRight = 1,
                MarginBottom = 11,
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
                Relative = new Rect(1, 1, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 31, 12),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(12),
                CharsSize = new Size(29, 11),
                CellCharsSize = new Size(31, 12),
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

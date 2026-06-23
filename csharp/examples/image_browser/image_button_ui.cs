// Auto-generated UI code for image_button - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageButtonUI : GeneratedWindowBase
{
    public Image photo = null!;

    public ImageButtonUI() : base("Image Button")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(true);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "ImageButton", """
# Image Button

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Single-image clickable tile with padding around a centered square image."
tags: [example, image, button]
```

## Definition

```yaml
extends: uicontrol
focusable: true
```

## Members

```yaml
photo:
  type: image
  source: ""
  alt: ""
  fit: cover
  render_mode: auto
  align: center
  valign: middle
  description: "Tile image. Click to select."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#172033"
cell:
  padding: 1, 4, 1, 4
  background: transparent
image:
  background: transparent
  color: "#d1d5db"
  focus-background: "#25364f"
```

## User Interface

```ui
+-view-**----------------+
| photo..**............. |
| ...................... |
| ...................... |
| ...................... |
+------------------------+
```

""", "Single-image clickable tile with padding around a centered square image.", new[]
        {
            new GeneratedElementMetadata { Name = "photo", Description = "Tile image. Click to select.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#172033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        photo = AddElement(new Image("photo", "", "", "cover", "auto", "center", "middle"));
        photo.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        photo.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "photo",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 1, 536870911, 4),
                SourceCell = new Rect(0, 0, 24, 4),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(4),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(22, 4),
                CellCharsSize = new Size(24, 4),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("transparent"),
        PaddingBottom = 1,
        PaddingLeft = 4,
        PaddingRight = 4,
        PaddingTop = 1
    },
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

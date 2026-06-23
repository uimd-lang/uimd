// Auto-generated UI code for image_list_button - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageListButtonUI : GeneratedWindowBase
{
    public Image prw_1 = null!;
    public Image prw_2 = null!;
    public Image prw_3 = null!;

    public ImageListButtonUI() : base("Image List Button")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(true);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "ImageListButton", """
# Image List Button

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "Clickable image-list preview tile with three thumbnails."
tags: [example, image, button, list]
```

## Definition

```yaml
extends: uicontrol
focusable: true
```

## Members

```yaml
prw_1:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "Preview image 1"
  fit: cover
  render_mode: auto
  description: "List preview slot 1. Click to open image items."

prw_2:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "Preview image 2"
  fit: cover
  render_mode: auto
  description: "List preview slot 2. Click to open image items."

prw_3:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "Preview image 3"
  fit: cover
  render_mode: auto
  description: "List preview slot 3. Click to open image items."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#172033"
cell:
  padding: 1, 0, 1, 0
  background: "#172033"
image:
  background: transparent
  color: "#d1d5db"
  focus-background: "#25364f"
```

## User Interface

```ui
+-view-**-----------------+
| prw_1.. prw_2.. prw_3.. |
| ....... ....... ....... |
| ....... ....... ....... |
+-------------------------+
```

""", "Clickable image-list preview tile with three thumbnails.", new[]
        {
            new GeneratedElementMetadata { Name = "prw_1", Description = "List preview slot 1. Click to open image items.", Expose = true },
            new GeneratedElementMetadata { Name = "prw_2", Description = "List preview slot 2. Click to open image items.", Expose = true },
            new GeneratedElementMetadata { Name = "prw_3", Description = "List preview slot 3. Click to open image items.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#172033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        prw_1 = AddElement(new Image("prw_1", "shared/assets/image_samples/camera.png", "Preview image 1", "cover", "auto", "center", "middle"));
        prw_1.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        prw_1.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        prw_2 = AddElement(new Image("prw_2", "shared/assets/image_samples/astronaut.png", "Preview image 2", "cover", "auto", "center", "middle"));
        prw_2.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        prw_2.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        prw_3 = AddElement(new Image("prw_3", "shared/assets/image_samples/coffee.png", "Preview image 3", "cover", "auto", "center", "middle"));
        prw_3.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        prw_3.SetFocusStyle(new Style
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
                Name = "prw_1",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 1, 7, 3),
                SourceCell = new Rect(0, 0, 25, 3),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(25, 3),
                MarginRight = 17,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "prw_2",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 9, 7, 3),
                SourceCell = new Rect(0, 0, 25, 3),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(25, 3),
                MarginRight = 9,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "prw_3",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 17, 7, 3),
                SourceCell = new Rect(0, 0, 25, 3),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(25, 3),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 0,
        PaddingRight = 0,
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

    protected virtual void onPrw1Click() { }
    protected virtual void onPrw2Click() { }
    protected virtual void onPrw3Click() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "prw_1")
        {
            onPrw1Click();
            return true;
        }
        else if (name == "prw_2")
        {
            onPrw2Click();
            return true;
        }
        else if (name == "prw_3")
        {
            onPrw3Click();
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

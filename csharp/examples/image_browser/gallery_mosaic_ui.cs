// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
using Uimd;

public class GalleryMosaicUI : GeneratedWindowBase
{
    public Image gal_1 = null!;
    public Image gal_2 = null!;
    public Image gal_3 = null!;
    public Image gal_4 = null!;
    public Image gal_5 = null!;

    public GalleryMosaicUI() : base("Gallery Mosaic")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(true);
        SetGeneratedKind("control");
        SetMcpMetadata(true, "GalleryMosaic", """
# Gallery Mosaic

## Metadata

```yaml
format: uimd
format-version: 1
kind: control
description: "3+2 image mosaic that opens the scrollable gallery when clicked."
tags: [example, image, gallery]
```

## Definition

```yaml
extends: uicontrol
focusable: true
```

## Members

```yaml
gal_1:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "Camera"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 1. Click to open gallery."

gal_2:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "Astronaut"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 2. Click to open gallery."

gal_3:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "Coffee"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 3. Click to open gallery."

gal_4:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "Coins"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 4. Click to open gallery."

gal_5:
  type: image
  source: "shared/assets/image_samples/chelsea.png"
  alt: "Chelsea"
  fit: cover
  render_mode: auto
  description: "Gallery mosaic slot 5. Click to open gallery."
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
+-view-**------------------+
| gal_1.. gal_2.. gal_3... |
| ....... ....... ........ |
| ....... ....... ........ |
|                          |
| gal_4.. gal_5..          |
| ....... .......          |
| ....... .......          |
+--------------------------+
```

""", "3+2 image mosaic that opens the scrollable gallery when clicked.", new[]
        {
            new GeneratedElementMetadata { Name = "gal_1", Description = "Gallery mosaic slot 1. Click to open gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "gal_2", Description = "Gallery mosaic slot 2. Click to open gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "gal_3", Description = "Gallery mosaic slot 3. Click to open gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "gal_4", Description = "Gallery mosaic slot 4. Click to open gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "gal_5", Description = "Gallery mosaic slot 5. Click to open gallery.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#172033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        gal_1 = AddElement(new Image("gal_1", "shared/assets/image_samples/camera.png", "Camera", "cover", "auto", "center", "middle"));
        gal_1.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        gal_1.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        gal_2 = AddElement(new Image("gal_2", "shared/assets/image_samples/astronaut.png", "Astronaut", "cover", "auto", "center", "middle"));
        gal_2.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        gal_2.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        gal_3 = AddElement(new Image("gal_3", "shared/assets/image_samples/coffee.png", "Coffee", "cover", "auto", "center", "middle"));
        gal_3.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        gal_3.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        gal_4 = AddElement(new Image("gal_4", "shared/assets/image_samples/coins.png", "Coins", "cover", "auto", "center", "middle"));
        gal_4.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        gal_4.SetFocusStyle(new Style
    {
        Background = new Color("#25364f")
    });
        gal_5 = AddElement(new Image("gal_5", "shared/assets/image_samples/chelsea.png", "Chelsea", "cover", "auto", "center", "middle"));
        gal_5.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        gal_5.SetFocusStyle(new Style
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
                Name = "gal_1",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 1, 7, 3),
                SourceCell = new Rect(0, 0, 26, 7),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(26, 7),
                MarginRight = 18,
                MarginBottom = 4,
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
                Name = "gal_2",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 9, 7, 3),
                SourceCell = new Rect(0, 0, 26, 7),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(26, 7),
                MarginRight = 10,
                MarginBottom = 4,
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
                Name = "gal_3",
                Type = "image",
                CellName = "view",
                Relative = new Rect(0, 17, 8, 3),
                SourceCell = new Rect(0, 0, 26, 7),
                Width = AxisDimension.Auto(8),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(8, 3),
                CellCharsSize = new Size(26, 7),
                MarginRight = 1,
                MarginBottom = 4,
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
                Name = "gal_4",
                Type = "image",
                CellName = "view",
                Relative = new Rect(4, 1, 7, 3),
                SourceCell = new Rect(0, 0, 26, 7),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(26, 7),
                MarginRight = 18,
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
                Name = "gal_5",
                Type = "image",
                CellName = "view",
                Relative = new Rect(4, 9, 7, 3),
                SourceCell = new Rect(0, 0, 26, 7),
                Width = AxisDimension.Auto(7),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(7, 3),
                CellCharsSize = new Size(26, 7),
                MarginRight = 10,
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

    protected virtual void onGal1Click() { }
    protected virtual void onGal2Click() { }
    protected virtual void onGal3Click() { }
    protected virtual void onGal4Click() { }
    protected virtual void onGal5Click() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "gal_1")
        {
            onGal1Click();
            return true;
        }
        else if (name == "gal_2")
        {
            onGal2Click();
            return true;
        }
        else if (name == "gal_3")
        {
            onGal3Click();
            return true;
        }
        else if (name == "gal_4")
        {
            onGal4Click();
            return true;
        }
        else if (name == "gal_5")
        {
            onGal5Click();
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

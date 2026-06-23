// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageGalleryUI : GeneratedWindowBase
{
    public Label header = null!;
    public Button close_btn = null!;
    public Label fallback_label = null!;
    public Label sixel_label = null!;
    public Label stretch_label = null!;
    public Image camera_fb = null!;
    public Image astro_fb = null!;
    public Image coffee_fb = null!;
    public Image coins_fb = null!;
    public Image camera_ctn = null!;
    public Image astro_ctn = null!;
    public Image coffee_ctn = null!;
    public Image coins_ctn = null!;
    public Image camera_str = null!;
    public Image astro_str = null!;
    public Image coffee_str = null!;
    public Image coins_str = null!;
    public Label status = null!;

    public ImageGalleryUI() : base("Image Gallery")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ImageGallery", """
# Image Gallery

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes."
tags: [example, image]
```

## Members

```yaml
header:
  type: label
  text: "Image Gallery"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the image gallery."

fallback_label:
  type: label
  text: "Fallback cover"
  description: "Fallback row label."

sixel_label:
  type: label
  text: "Sixel contain"
  description: "Sixel contain row label."

stretch_label:
  type: label
  text: "Sixel stretch"
  description: "Sixel stretch row label."

camera_fb:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera fallback"
  fit: cover
  render_mode: fallback
  description: "Camera image forced through fallback rendering."

astro_fb:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut fallback"
  fit: cover
  render_mode: fallback
  description: "Astronaut image forced through fallback rendering."

coffee_fb:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee fallback"
  fit: cover
  render_mode: fallback
  description: "Coffee image forced through fallback rendering."

coins_fb:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins fallback"
  fit: cover
  render_mode: fallback
  description: "Coins image forced through fallback rendering."

camera_ctn:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera sixel"
  fit: contain
  render_mode: sixel
  description: "Camera image in forced Sixel contain mode."

astro_ctn:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut sixel"
  fit: contain
  render_mode: sixel
  description: "Astronaut image in forced Sixel contain mode."

coffee_ctn:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee sixel"
  fit: contain
  render_mode: sixel
  description: "Coffee image in forced Sixel contain mode."

coins_ctn:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins sixel"
  fit: contain
  render_mode: sixel
  description: "Coins image in forced Sixel contain mode."

camera_str:
  type: image
  source: "shared/assets/image_samples/camera.png"
  alt: "camera stretch"
  fit: stretch
  render_mode: sixel
  description: "Camera image in forced Sixel stretch mode."

astro_str:
  type: image
  source: "shared/assets/image_samples/astronaut.png"
  alt: "astronaut stretch"
  fit: stretch
  render_mode: sixel
  description: "Astronaut image in forced Sixel stretch mode."

coffee_str:
  type: image
  source: "shared/assets/image_samples/coffee.png"
  alt: "coffee stretch"
  fit: stretch
  render_mode: sixel
  description: "Coffee image in forced Sixel stretch mode."

coins_str:
  type: image
  source: "shared/assets/image_samples/coins.png"
  alt: "coins stretch"
  fit: stretch
  render_mode: sixel
  description: "Coins image in forced Sixel stretch mode."

status:
  type: label
  text: "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."
  description: "Current image validation status."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@fallback_label:
  color: "#93c5fd"
@sixel_label:
  color: "#93c5fd"
@stretch_label:
  color: "#93c5fd"
image:
  background: transparent
  color: "#d1d5db"
@status:
  color: "#93c5fd"
```

## User Interface

```ui
+--------------------------------------------------------------------------------------+
| header.................................................................. close_btn.. |
|                                                                                      |
| fallback_label...................................................................... |
|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| sixel_label......................................................................... |
|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| stretch_label....................................................................... |
|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|  .................... .................... .................... .................... |
|                                                                                      |
| status.............................................................................. |
+--------------------------------------------------------------------------------------+
```

""", "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Window title.", Expose = false },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the image gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "fallback_label", Description = "Fallback row label.", Expose = true },
            new GeneratedElementMetadata { Name = "sixel_label", Description = "Sixel contain row label.", Expose = true },
            new GeneratedElementMetadata { Name = "stretch_label", Description = "Sixel stretch row label.", Expose = true },
            new GeneratedElementMetadata { Name = "camera_fb", Description = "Camera image forced through fallback rendering.", Expose = true },
            new GeneratedElementMetadata { Name = "astro_fb", Description = "Astronaut image forced through fallback rendering.", Expose = true },
            new GeneratedElementMetadata { Name = "coffee_fb", Description = "Coffee image forced through fallback rendering.", Expose = true },
            new GeneratedElementMetadata { Name = "coins_fb", Description = "Coins image forced through fallback rendering.", Expose = true },
            new GeneratedElementMetadata { Name = "camera_ctn", Description = "Camera image in forced Sixel contain mode.", Expose = true },
            new GeneratedElementMetadata { Name = "astro_ctn", Description = "Astronaut image in forced Sixel contain mode.", Expose = true },
            new GeneratedElementMetadata { Name = "coffee_ctn", Description = "Coffee image in forced Sixel contain mode.", Expose = true },
            new GeneratedElementMetadata { Name = "coins_ctn", Description = "Coins image in forced Sixel contain mode.", Expose = true },
            new GeneratedElementMetadata { Name = "camera_str", Description = "Camera image in forced Sixel stretch mode.", Expose = true },
            new GeneratedElementMetadata { Name = "astro_str", Description = "Astronaut image in forced Sixel stretch mode.", Expose = true },
            new GeneratedElementMetadata { Name = "coffee_str", Description = "Coffee image in forced Sixel stretch mode.", Expose = true },
            new GeneratedElementMetadata { Name = "coins_str", Description = "Coins image in forced Sixel stretch mode.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Current image validation status.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Image Gallery"));
        header.SetStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        close_btn = AddElement(new Button("close_btn", "Quit"));
        close_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#ffffff")
    });
        close_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        close_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        fallback_label = AddElement(new Label("fallback_label", "Fallback cover"));
        fallback_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        fallback_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        sixel_label = AddElement(new Label("sixel_label", "Sixel contain"));
        sixel_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        sixel_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        stretch_label = AddElement(new Label("stretch_label", "Sixel stretch"));
        stretch_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        stretch_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        camera_fb = AddElement(new Image("camera_fb", "shared/assets/image_samples/camera.png", "camera fallback", "cover", "fallback", "center", "middle"));
        camera_fb.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        astro_fb = AddElement(new Image("astro_fb", "shared/assets/image_samples/astronaut.png", "astronaut fallback", "cover", "fallback", "center", "middle"));
        astro_fb.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coffee_fb = AddElement(new Image("coffee_fb", "shared/assets/image_samples/coffee.png", "coffee fallback", "cover", "fallback", "center", "middle"));
        coffee_fb.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coins_fb = AddElement(new Image("coins_fb", "shared/assets/image_samples/coins.png", "coins fallback", "cover", "fallback", "center", "middle"));
        coins_fb.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        camera_ctn = AddElement(new Image("camera_ctn", "shared/assets/image_samples/camera.png", "camera sixel", "contain", "sixel", "center", "middle"));
        camera_ctn.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        astro_ctn = AddElement(new Image("astro_ctn", "shared/assets/image_samples/astronaut.png", "astronaut sixel", "contain", "sixel", "center", "middle"));
        astro_ctn.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coffee_ctn = AddElement(new Image("coffee_ctn", "shared/assets/image_samples/coffee.png", "coffee sixel", "contain", "sixel", "center", "middle"));
        coffee_ctn.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coins_ctn = AddElement(new Image("coins_ctn", "shared/assets/image_samples/coins.png", "coins sixel", "contain", "sixel", "center", "middle"));
        coins_ctn.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        camera_str = AddElement(new Image("camera_str", "shared/assets/image_samples/camera.png", "camera stretch", "stretch", "sixel", "center", "middle"));
        camera_str.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        astro_str = AddElement(new Image("astro_str", "shared/assets/image_samples/astronaut.png", "astronaut stretch", "stretch", "sixel", "center", "middle"));
        astro_str.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coffee_str = AddElement(new Image("coffee_str", "shared/assets/image_samples/coffee.png", "coffee stretch", "stretch", "sixel", "center", "middle"));
        coffee_str.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        coins_str = AddElement(new Image("coins_str", "shared/assets/image_samples/coins.png", "coins stretch", "stretch", "sixel", "center", "middle"));
        coins_str.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        status = AddElement(new Label("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch."));
        status.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        status.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "header",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 72, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(72),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(72, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 13,
                MarginBottom = 38,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 74, 536870911, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 38,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "fallback_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(2, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(84, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 36,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "camera_fb",
                Type = "image",
                CellName = "",
                Relative = new Rect(3, 2, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 64,
                MarginBottom = 26,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "astro_fb",
                Type = "image",
                CellName = "",
                Relative = new Rect(3, 23, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 43,
                MarginBottom = 26,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coffee_fb",
                Type = "image",
                CellName = "",
                Relative = new Rect(3, 44, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 22,
                MarginBottom = 26,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coins_fb",
                Type = "image",
                CellName = "",
                Relative = new Rect(3, 65, 536870911, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(21, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 0,
                MarginBottom = 26,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "sixel_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(14, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(84, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 24,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "camera_ctn",
                Type = "image",
                CellName = "",
                Relative = new Rect(15, 2, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 64,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "astro_ctn",
                Type = "image",
                CellName = "",
                Relative = new Rect(15, 23, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 43,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coffee_ctn",
                Type = "image",
                CellName = "",
                Relative = new Rect(15, 44, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 22,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coins_ctn",
                Type = "image",
                CellName = "",
                Relative = new Rect(15, 65, 536870911, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "stretch_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(26, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(84, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 12,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "camera_str",
                Type = "image",
                CellName = "",
                Relative = new Rect(27, 2, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 64,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "astro_str",
                Type = "image",
                CellName = "",
                Relative = new Rect(27, 23, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 43,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coffee_str",
                Type = "image",
                CellName = "",
                Relative = new Rect(27, 44, 20, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 22,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "coins_str",
                Type = "image",
                CellName = "",
                Relative = new Rect(27, 65, 536870911, 10),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(10),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(20, 10),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(38, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 86, 39),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(86),
                CellHeight = AxisDimension.Auto(39),
                CharsSize = new Size(84, 1),
                CellCharsSize = new Size(86, 39),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
        };
    }

    protected virtual void onCloseBtnClick() { }
    protected virtual void onCameraFbClick() { }
    protected virtual void onAstroFbClick() { }
    protected virtual void onCoffeeFbClick() { }
    protected virtual void onCoinsFbClick() { }
    protected virtual void onCameraCtnClick() { }
    protected virtual void onAstroCtnClick() { }
    protected virtual void onCoffeeCtnClick() { }
    protected virtual void onCoinsCtnClick() { }
    protected virtual void onCameraStrClick() { }
    protected virtual void onAstroStrClick() { }
    protected virtual void onCoffeeStrClick() { }
    protected virtual void onCoinsStrClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "close_btn")
        {
            onCloseBtnClick();
            return true;
        }
        else if (name == "camera_fb")
        {
            onCameraFbClick();
            return true;
        }
        else if (name == "astro_fb")
        {
            onAstroFbClick();
            return true;
        }
        else if (name == "coffee_fb")
        {
            onCoffeeFbClick();
            return true;
        }
        else if (name == "coins_fb")
        {
            onCoinsFbClick();
            return true;
        }
        else if (name == "camera_ctn")
        {
            onCameraCtnClick();
            return true;
        }
        else if (name == "astro_ctn")
        {
            onAstroCtnClick();
            return true;
        }
        else if (name == "coffee_ctn")
        {
            onCoffeeCtnClick();
            return true;
        }
        else if (name == "coins_ctn")
        {
            onCoinsCtnClick();
            return true;
        }
        else if (name == "camera_str")
        {
            onCameraStrClick();
            return true;
        }
        else if (name == "astro_str")
        {
            onAstroStrClick();
            return true;
        }
        else if (name == "coffee_str")
        {
            onCoffeeStrClick();
            return true;
        }
        else if (name == "coins_str")
        {
            onCoinsStrClick();
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

// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageBrowserUI : GeneratedWindowBase
{
    public Label title = null!;
    public Button close_btn = null!;
    public Label render_label = null!;
    public ComboBox mode_select = null!;
    public Label sidebar_title = null!;
    public ReusableElement camera_thumb = null!;
    public ReusableElement astro_thumb = null!;
    public ReusableElement coffee_thumb = null!;
    public Label gallery_label = null!;
    public Label image_list_label = null!;
    public ReusableElement gallery_mosaic = null!;
    public ReusableElement image_list_btn = null!;
    public Label spacer = null!;
    public ViewHost main = null!;

    public ImageBrowserUI() : base("Image Browser")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ImageBrowser", """
# Image Browser

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Image browser with a thumbnail sidebar and full-size image panel."
tags: [example, image, viewhost]
```

## Definition

```yaml
uses:
  - image_view
  - gallery_view
  - gallery_mosaic
  - image_button
  - image_list_button
  - image_list_view
  - image_show_dialog
```

## Members

```yaml
title:
  type: label
  text: "Image Browser"
  description: "Window title."
  expose: false

close_btn:
  type: button
  title: Quit
  description: "Close the image browser."

render_label:
  type: label
  text: "Render"
  description: "Label above the render mode selector."
  expose: false

mode_select:
  type: combobox
  options: [Normal, Fallback]
  selected_item: Normal
  description: "Switch between Sixel and fallback image rendering."

sidebar_title:
  type: label
  text: "Photos"
  description: "Sidebar section label."
  expose: false

camera_thumb:
  type: image_button
  description: "Camera thumbnail. Click to view full size."

astro_thumb:
  type: image_button
  description: "Astronaut thumbnail. Click to view full size."

coffee_thumb:
  type: image_button
  description: "Coffee thumbnail. Click to view full size."

gallery_label:
  type: label
  text: "Gallery"
  description: "Label above the gallery mosaic."
  expose: false

image_list_label:
  type: label
  text: "Image items"
  description: "List preview label."
  expose: false

gallery_mosaic:
  type: gallery_mosaic
  description: "Click any image to open the full scrollable gallery."

image_list_btn:
  type: image_list_button
  description: "Click to open the image item list."

spacer:
  type: label
  text: ""
  description: "Spacer that absorbs leftover sidebar height."
  expose: false

main:
  type: viewhost
  description: "Full-size image panel."
```

## Style

```yaml
include: dark
this:
  border-width: 0
cell:
  padding: 1
@header:
  background: "#1f2937"
  padding: 0
@sidebar:
  background: "#172033"
  padding: 0
@content:
  background: "#030712"
@title:
  color: "#ffffff"
  text-align: center
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
  color: "#ffffff"
@render_label:
  color: "#93c5fd"
@sidebar_title:
  color: "#93c5fd"
@gallery_label:
  color: "#93c5fd"
@image_list_label:
  color: "#93c5fd"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
viewhost:
  background: "#030712"
```

## User Interface

```ui
+--header-**-------------------------------------------------------------------+
| title..**....................................................... close_btn.. |
+--sidebar-------------------+--content-**-------------------------------------+
| render_label.............. | main..**....................................... |
* mode_select............... | ............................................... |
*                            | *.............................................. |
| sidebar_title............. | *.............................................. |
| camera_thumb.............. | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| astro_thumb............... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| coffee_thumb.............. | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| gallery_label............. | ............................................... |
| gallery_mosaic............ | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| image_list_label.......... | ............................................... |
| image_list_btn............ | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
| .......................... | ............................................... |
* spacer.................... | ............................................... |
* .......................... | ............................................... |
| .......................... | ............................................... |
+----------------------------+-------------------------------------------------+
```

""", "Image browser with a thumbnail sidebar and full-size image panel.", new[]
        {
            new GeneratedElementMetadata { Name = "title", Description = "Window title.", Expose = false },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the image browser.", Expose = true },
            new GeneratedElementMetadata { Name = "render_label", Description = "Label above the render mode selector.", Expose = false },
            new GeneratedElementMetadata { Name = "mode_select", Description = "Switch between Sixel and fallback image rendering.", Expose = true },
            new GeneratedElementMetadata { Name = "sidebar_title", Description = "Sidebar section label.", Expose = false },
            new GeneratedElementMetadata { Name = "camera_thumb", Description = "Camera thumbnail. Click to view full size.", Expose = true },
            new GeneratedElementMetadata { Name = "astro_thumb", Description = "Astronaut thumbnail. Click to view full size.", Expose = true },
            new GeneratedElementMetadata { Name = "coffee_thumb", Description = "Coffee thumbnail. Click to view full size.", Expose = true },
            new GeneratedElementMetadata { Name = "gallery_label", Description = "Label above the gallery mosaic.", Expose = false },
            new GeneratedElementMetadata { Name = "image_list_label", Description = "List preview label.", Expose = false },
            new GeneratedElementMetadata { Name = "gallery_mosaic", Description = "Click any image to open the full scrollable gallery.", Expose = true },
            new GeneratedElementMetadata { Name = "image_list_btn", Description = "Click to open the image item list.", Expose = true },
            new GeneratedElementMetadata { Name = "spacer", Description = "Spacer that absorbs leftover sidebar height.", Expose = false },
            new GeneratedElementMetadata { Name = "main", Description = "Full-size image panel.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        title = AddElement(new Label("title", "Image Browser"));
        title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        title.SetCursorStyle(new Style
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
        render_label = AddElement(new Label("render_label", "Render"));
        render_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        render_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        mode_select = AddElement(new ComboBox("mode_select", new[] {"Normal", "Fallback"}));
        mode_select.SetSelectedIndex(0);
        mode_select.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        mode_select.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        mode_select.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        mode_select.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        mode_select.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        sidebar_title = AddElement(new Label("sidebar_title", "Photos"));
        sidebar_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        sidebar_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        camera_thumb = AddElement(new ReusableElement("camera_thumb"));
        camera_thumb.SetChild(new ImageButtonUI());
        camera_thumb.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        camera_thumb.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        camera_thumb.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        astro_thumb = AddElement(new ReusableElement("astro_thumb"));
        astro_thumb.SetChild(new ImageButtonUI());
        astro_thumb.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        astro_thumb.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        astro_thumb.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        coffee_thumb = AddElement(new ReusableElement("coffee_thumb"));
        coffee_thumb.SetChild(new ImageButtonUI());
        coffee_thumb.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        coffee_thumb.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        coffee_thumb.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        gallery_label = AddElement(new Label("gallery_label", "Gallery"));
        gallery_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        gallery_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        image_list_label = AddElement(new Label("image_list_label", "Image items"));
        image_list_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    });
        image_list_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        gallery_mosaic = AddElement(new ReusableElement("gallery_mosaic"));
        gallery_mosaic.SetChild(new GalleryMosaicUI());
        gallery_mosaic.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        gallery_mosaic.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        gallery_mosaic.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        image_list_btn = AddElement(new ReusableElement("image_list_btn"));
        image_list_btn.SetChild(new ImageListButtonUI());
        image_list_btn.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        image_list_btn.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        image_list_btn.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        spacer = AddElement(new Label("spacer", ""));
        spacer.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        spacer.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        main = AddElement(new ViewHost("main"));
        main.SetStyle(new Style
    {
        Background = new Color("#030712")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "main",
                Type = "viewhost",
                CellName = "content",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(2, 29, 49, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(41),
                CharsSize = new Size(47, 41),
                CellCharsSize = new Size(49, 41),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712"),
        Padding = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#030712")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "render_label",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 40,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "title",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(64, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 13,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 66, 11, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#ffffff")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "mode_select",
                Type = "combobox",
                CellName = "sidebar",
                Relative = new Rect(1, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 39,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "sidebar_title",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(3, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 37,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "camera_thumb",
                Type = "image_button",
                CellName = "sidebar",
                Relative = new Rect(4, 1, 536870911, 6),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(6),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 6),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 31,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "astro_thumb",
                Type = "image_button",
                CellName = "sidebar",
                Relative = new Rect(10, 1, 536870911, 6),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(6),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 6),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 25,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "coffee_thumb",
                Type = "image_button",
                CellName = "sidebar",
                Relative = new Rect(16, 1, 536870911, 6),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(6),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 6),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 19,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "gallery_label",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(22, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 18,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "gallery_mosaic",
                Type = "gallery_mosaic",
                CellName = "sidebar",
                Relative = new Rect(23, 1, 536870911, 9),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(9),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 9),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 9,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "image_list_label",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(32, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 1),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 8,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "image_list_btn",
                Type = "image_list_button",
                CellName = "sidebar",
                Relative = new Rect(33, 1, 536870911, 5),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 5),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 3,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "spacer",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(38, 1, 536870911, 3),
                SourceCell = new Rect(2, 0, 28, 41),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Auto(28),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(26, 3),
                CellCharsSize = new Size(28, 41),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        Padding = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
        };
    }

    protected virtual void onCloseBtnClick() { }
    protected virtual void onModeSelectChange(string value) { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "close_btn")
        {
            onCloseBtnClick();
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
        if (name == "mode_select")
        {
            onModeSelectChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}

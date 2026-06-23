// Auto-generated UI code for image_show_dialog - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageShowDialogUI : GeneratedWindowBase
{
    public Label title = null!;
    public Image photo = null!;
    public Button close_btn = null!;

    public ImageShowDialogUI() : base("Image Show Dialog")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("dialog");
        SetMcpMetadata(true, "ImageShowDialog", """
# Image Show Dialog

## Metadata

```yaml
format: uimd
format-version: 1
kind: dialog
description: "Dialog for showing a selected image."
tags: [example, image, dialog]
```

## Members

```yaml
title:
  type: label
  text: "Image"
  description: "Dialog title."
  expose: false

photo:
  type: image
  source: ""
  alt: ""
  fit: contain
  render_mode: auto
  description: "Selected image preview."

close_btn:
  type: button
  title: Close
  description: "Close the dialog."
```

## Style

```yaml
include: dark
this:
  border-width: 1
  background: "#081426"
  margin: 4, 6, 4, 6
cell:
  padding: 1, 1, 1, 1
@title:
  color: "#f8fafc"
  text-align: center
image:
  background: transparent
  color: "#d1d5db"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
```

## User Interface

```ui
+-dialog-**--------------------------------------------------------------------------------+
| title..**............................................................................... |
| photo..**............................................................................... |
* ........................................................................................ |
* *....................................................................................... |
| *....................................................................................... |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
| ........................................................................................ |
+-**-------------------------------+---------------------+-**------------------------------+
|                                  |  close_btn........  |                                 |
+----------------------------------+---------------------+---------------------------------+
```

""", "Dialog for showing a selected image.", new[]
        {
            new GeneratedElementMetadata { Name = "title", Description = "Dialog title.", Expose = false },
            new GeneratedElementMetadata { Name = "photo", Description = "Selected image preview.", Expose = true },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the dialog.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#081426"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 1,
        BorderWidthVertical = 1,
        MarginBottom = 4,
        MarginLeft = 6,
        MarginRight = 6,
        MarginTop = 4
    });
        title = AddElement(new Label("title", "Image"));
        title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        TextAlign = "center"
    });
        title.SetCursorStyle(new Style
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
        close_btn = AddElement(new Button("close_btn", "Close"));
        close_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        close_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        close_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "",
                Type = "",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(22, 0, 34, 1),
                Width = AxisDimension.Auto(536870911),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(34, 1),
                CellCharsSize = new Size(34, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "",
                Type = "",
                CellName = "",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(22, 57, 33, 1),
                Width = AxisDimension.Auto(536870911),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(33, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "title",
                Type = "label",
                CellName = "dialog",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 90, 21),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(88, 1),
                CellCharsSize = new Size(90, 21),
                MarginRight = 1,
                MarginBottom = 20,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(0, 2, 536870911, 1),
                SourceCell = new Rect(22, 35, 21, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(21),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(21, 1),
                MarginRight = 2,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "photo",
                Type = "image",
                CellName = "dialog",
                Relative = new Rect(1, 1, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 90, 21),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(88, 20),
                CellCharsSize = new Size(90, 21),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
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
    protected virtual void onCloseBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "photo")
        {
            onPhotoClick();
            return true;
        }
        else if (name == "close_btn")
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
        return false;
    }
}

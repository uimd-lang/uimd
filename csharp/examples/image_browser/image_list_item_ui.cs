// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
using Uimd;

public class ImageListItemUI : GeneratedWindowBase
{
    public CheckBox sel = null!;
    public Image thumb = null!;
    public Label name_label = null!;
    public Label path_label = null!;
    public Label spacer = null!;
    public Button show_btn = null!;
    public Button browse_btn = null!;
    public Button delete_btn = null!;

    public ImageListItemUI() : base("Image List Item")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ImageListItem", """
# Image List Item

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
description: "Reusable row for one image item."
tags: [example, image, list]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
sel:
  type: checkbox
  title: ""
  value: false
  description: "Select this image item."

thumb:
  type: image
  source: ""
  alt: ""
  fit: contain
  render_mode: auto
  description: "Small image thumbnail."

name_label:
  type: label
  text: ""
  description: "Image file name."

path_label:
  type: label
  text: ""
  description: "Full image path."

spacer:
  type: label
  text: ""
  description: "Flexible spacer before item actions."
  expose: false

show_btn:
  type: button
  title: Show
  description: "Show this image in a dialog."

browse_btn:
  type: button
  title: Browse
  description: "Choose a replacement image."

delete_btn:
  type: button
  title: Delete
  description: "Delete this image item after confirmation."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@card:
  background: "#172033"
  padding: 0, 0, 0, 0
@name_label:
  color: "#f8fafc"
  user-select: text
@path_label:
  color: "#64748b"
  user-select: text
checkbox:
  color: "#cbd5e1"
  focus-color: "#facc15"
  checked-color: "#22c55e"
  unchecked-color: "#94a3b8"
image:
  background: transparent
  color: "#d1d5db"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@show_btn:
  background: "#14532d"
  focus-background: "#16a34a"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
```

## User Interface

```ui
+-card-**---------------------------------------------------------------+
|                                                                       |
|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |
|  ... ........  .....................................................  |
|  ... ........  path_label..**.......................................  |
|  ... ........  .....................................................  |
|  ... ........  spacer..**...........................................  |
|                                                                       |
+-----------------------------------------------------------------------+
```

""", "Reusable row for one image item.", new[]
        {
            new GeneratedElementMetadata { Name = "sel", Description = "Select this image item.", Expose = true },
            new GeneratedElementMetadata { Name = "thumb", Description = "Small image thumbnail.", Expose = true },
            new GeneratedElementMetadata { Name = "name_label", Description = "Image file name.", Expose = true },
            new GeneratedElementMetadata { Name = "path_label", Description = "Full image path.", Expose = true },
            new GeneratedElementMetadata { Name = "spacer", Description = "Flexible spacer before item actions.", Expose = false },
            new GeneratedElementMetadata { Name = "show_btn", Description = "Show this image in a dialog.", Expose = true },
            new GeneratedElementMetadata { Name = "browse_btn", Description = "Choose a replacement image.", Expose = true },
            new GeneratedElementMetadata { Name = "delete_btn", Description = "Delete this image item after confirmation.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        sel = AddElement(new CheckBox("sel", "", false));
        sel.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        sel.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#facc15")
    });
        sel.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#22c55e")
    });
        sel.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#94a3b8")
    });
        sel.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        thumb = AddElement(new Image("thumb", "", "", "contain", "auto", "center", "middle"));
        thumb.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    });
        name_label = AddElement(new Label("name_label", ""));
        name_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        UserSelect = "text"
    });
        name_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        path_label = AddElement(new Label("path_label", ""));
        path_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#64748b"),
        UserSelect = "text"
    });
        path_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
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
        show_btn = AddElement(new Button("show_btn", "Show"));
        show_btn.SetStyle(new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    });
        show_btn.SetFocusStyle(new Style
    {
        Background = new Color("#16a34a"),
        Color = new Color("#ffffff")
    });
        show_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        browse_btn = AddElement(new Button("browse_btn", "Browse"));
        browse_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        browse_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        browse_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        delete_btn = AddElement(new Button("delete_btn", "Delete"));
        delete_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        delete_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        delete_btn.SetDisabledStyle(new Style
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
                Name = "sel",
                Type = "checkbox",
                CellName = "card",
                Relative = new Rect(1, 2, 3, 5),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Auto(3),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(3, 5),
                CellCharsSize = new Size(71, 7),
                MarginRight = 66,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "thumb",
                Type = "image",
                CellName = "card",
                Relative = new Rect(1, 6, 8, 5),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Auto(8),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(8, 5),
                CellCharsSize = new Size(71, 7),
                MarginRight = 57,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_label",
                Type = "label",
                CellName = "card",
                Relative = new Rect(1, 16, 536870911, 2),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(16, 2),
                CellCharsSize = new Size(71, 7),
                MarginRight = 39,
                MarginBottom = 4,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#f8fafc"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "show_btn",
                Type = "button",
                CellName = "card",
                Relative = new Rect(1, 34, 11, 5),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(11, 5),
                CellCharsSize = new Size(71, 7),
                MarginRight = 26,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#14532d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "browse_btn",
                Type = "button",
                CellName = "card",
                Relative = new Rect(1, 46, 11, 5),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(11, 5),
                CellCharsSize = new Size(71, 7),
                MarginRight = 14,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "delete_btn",
                Type = "button",
                CellName = "card",
                Relative = new Rect(1, 58, 11, 5),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(5),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(11, 5),
                CellCharsSize = new Size(71, 7),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "path_label",
                Type = "label",
                CellName = "card",
                Relative = new Rect(3, 16, 536870911, 2),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(2),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(53, 2),
                CellCharsSize = new Size(71, 7),
                MarginRight = 2,
                MarginBottom = 2,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#64748b"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "spacer",
                Type = "label",
                CellName = "card",
                Relative = new Rect(5, 16, 536870911, 1),
                SourceCell = new Rect(0, 0, 71, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(7),
                CharsSize = new Size(53, 1),
                CellCharsSize = new Size(71, 7),
                MarginRight = 2,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 0,
        PaddingLeft = 0,
        PaddingRight = 0,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
        };
    }

    protected virtual void onSelChange(string value) { }
    protected virtual void onThumbClick() { }
    protected virtual void onShowBtnClick() { }
    protected virtual void onBrowseBtnClick() { }
    protected virtual void onDeleteBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "thumb")
        {
            onThumbClick();
            return true;
        }
        else if (name == "show_btn")
        {
            onShowBtnClick();
            return true;
        }
        else if (name == "browse_btn")
        {
            onBrowseBtnClick();
            return true;
        }
        else if (name == "delete_btn")
        {
            onDeleteBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "sel")
        {
            onSelChange(value);
            return true;
        }
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

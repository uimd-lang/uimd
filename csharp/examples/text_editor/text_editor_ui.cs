// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
using Uimd;

public class TextEditorUI : GeneratedWindowBase
{
    public Label window_header = null!;
    public Label filename = null!;
    public TextArea editor = null!;
    public Button new_btn = null!;
    public Button open_btn = null!;
    public Button save_btn = null!;
    public Button save_as_btn = null!;
    public Button quit_btn = null!;
    public Label status = null!;

    public TextEditorUI() : base("Text Editor")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "TextEditor", """
# Text Editor

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes."
tags: [example]
```

## Definition

```yaml
uses:
  - ../../dialogs/file_browser
```

## Members

```yaml
window_header:
  type: label
  text: "Text Editor"
  description: "Application title."
  expose: false

filename:
  type: label
  text: welcome.txt
  description: "Current file path or file name."

editor:
  type: textarea
  value: ""
  description: "Main multiline text editing area."

new_btn:
  type: button
  title: New
  description: "Create a new empty document, asking about unsaved changes when needed."

open_btn:
  type: button
  title: Open
  description: "Open the file browser to load a text file, asking about unsaved changes when needed."

save_btn:
  type: button
  title: Save
  description: "Save the current document to its current path."

save_as_btn:
  type: button
  title: "Save As"
  description: "Open the file browser in save mode and save the document to a chosen path."

quit_btn:
  type: button
  title: Quit
  description: "Close the editor, asking about unsaved changes when needed."

status:
  type: label
  text: Ready
  description: "Shows load, save, cancel, and error messages."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@filename:
  color: "#bfdbfe"
@quit_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@status:
  color: "#93c5fd"
```

## User Interface

```ui
+-title-**------------------------------------------------------------------------------+
|window_header..**......................................................................|
+-filebar-------------------------------------------------------------------------------+
|                                                                                       |
| "file" filename..**.................................................................. |
|                                                                                       |
| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |
+-main----------------------------------------------------------------------------------+
|                                                                                       |
| editor..**............................................................................|
| ..................................................................................... |
* *.................................................................................... |
* *.................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
| ..................................................................................... |
+---------------------------------------------------------------------------------------+
| status............................................................................... |
+---------------------------------------------------------------------------------------+
```

""", "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.", new[]
        {
            new GeneratedElementMetadata { Name = "window_header", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "filename", Description = "Current file path or file name.", Expose = true },
            new GeneratedElementMetadata { Name = "editor", Description = "Main multiline text editing area.", Expose = true },
            new GeneratedElementMetadata { Name = "new_btn", Description = "Create a new empty document, asking about unsaved changes when needed.", Expose = true },
            new GeneratedElementMetadata { Name = "open_btn", Description = "Open the file browser to load a text file, asking about unsaved changes when needed.", Expose = true },
            new GeneratedElementMetadata { Name = "save_btn", Description = "Save the current document to its current path.", Expose = true },
            new GeneratedElementMetadata { Name = "save_as_btn", Description = "Open the file browser in save mode and save the document to a chosen path.", Expose = true },
            new GeneratedElementMetadata { Name = "quit_btn", Description = "Close the editor, asking about unsaved changes when needed.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Shows load, save, cancel, and error messages.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        window_header = AddElement(new Label("window_header", "Text Editor"));
        window_header.SetStyle(new Style
    {
        Background = new Color("#1d314c"),
        Color = new Color("#cbd5e1"),
        TextAlign = "center"
    });
        window_header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        filename = AddElement(new Label("filename", "welcome.txt"));
        filename.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    });
        filename.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        editor = AddElement(new TextArea("editor", "", 0));
        editor.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        editor.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        editor.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        editor.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        editor.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        editor.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        new_btn = AddElement(new Button("new_btn", "New"));
        new_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        new_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        new_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        open_btn = AddElement(new Button("open_btn", "Open"));
        open_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        open_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        open_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        save_btn = AddElement(new Button("save_btn", "Save"));
        save_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        save_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        save_as_btn = AddElement(new Button("save_as_btn", "Save As"));
        save_as_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_as_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        save_as_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        quit_btn = AddElement(new Button("quit_btn", "Quit"));
        quit_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        quit_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        quit_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        status = AddElement(new Label("status", "Ready"));
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
                Name = "window_header",
                Type = "label",
                CellName = "title",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 87, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(87, 1),
                CellCharsSize = new Size(87, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#1d314c"),
        Color = new Color("#cbd5e1"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 85, 1),
                SourceCell = new Rect(18, 0, 87, 1),
                Width = AxisDimension.Auto(85),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(85, 1),
                CellCharsSize = new Size(87, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"file\"",
                Type = "label",
                CellName = "filebar",
                Relative = new Rect(1, 1, 6, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(6),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(6, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 80,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "editor",
                Type = "textarea",
                CellName = "main",
                Relative = new Rect(1, 1, 536870911, 536870911),
                SourceCell = new Rect(7, 0, 87, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(86, 9),
                CellCharsSize = new Size(87, 10),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "filename",
                Type = "label",
                CellName = "filebar",
                Relative = new Rect(1, 8, 536870911, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(78, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 1,
                MarginBottom = 2,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#bfdbfe")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "new_btn",
                Type = "button",
                CellName = "filebar",
                Relative = new Rect(3, 1, 14, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(14),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 72,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "open_btn",
                Type = "button",
                CellName = "filebar",
                Relative = new Rect(3, 16, 15, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(15),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(15, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 56,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_btn",
                Type = "button",
                CellName = "filebar",
                Relative = new Rect(3, 32, 15, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(15),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(15, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 40,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_as_btn",
                Type = "button",
                CellName = "filebar",
                Relative = new Rect(3, 48, 18, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(18),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(18, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 21,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "quit_btn",
                Type = "button",
                CellName = "filebar",
                Relative = new Rect(3, 67, 15, 1),
                SourceCell = new Rect(2, 0, 87, 4),
                Width = AxisDimension.Auto(15),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(87),
                CellHeight = AxisDimension.Auto(4),
                CharsSize = new Size(15, 1),
                CellCharsSize = new Size(87, 4),
                MarginRight = 5,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onEditorChange(string value) { }
    protected virtual void onEditorSubmit(string value) { }
    protected virtual void onNewBtnClick() { }
    protected virtual void onOpenBtnClick() { }
    protected virtual void onSaveBtnClick() { }
    protected virtual void onSaveAsBtnClick() { }
    protected virtual void onQuitBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "new_btn")
        {
            onNewBtnClick();
            return true;
        }
        else if (name == "open_btn")
        {
            onOpenBtnClick();
            return true;
        }
        else if (name == "save_btn")
        {
            onSaveBtnClick();
            return true;
        }
        else if (name == "save_as_btn")
        {
            onSaveAsBtnClick();
            return true;
        }
        else if (name == "quit_btn")
        {
            onQuitBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "editor")
        {
            onEditorChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "editor")
        {
            onEditorSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        return false;
    }
}

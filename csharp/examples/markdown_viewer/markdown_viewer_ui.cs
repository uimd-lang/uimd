// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
using Uimd;

public class MarkdownViewerUI : GeneratedWindowBase
{
    public Label header = null!;
    public Label docs_label = null!;
    public ListBox docs = null!;
    public Button up_btn = null!;
    public Button down_btn = null!;
    public ReusableElement viewer = null!;
    public Button close_btn = null!;
    public Label status = null!;

    public MarkdownViewerUI() : base("Markdown Viewer")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "MarkdownViewer", """
# Markdown Viewer

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents."
tags: [example]
```

## Definition

```yaml
uses:
  - document_view
```

## Members

```yaml
header:
  type: label
  text: "Markdown Viewer"
  description: "Application title."
  expose: false

docs_label:
  type: label
  text: Documents
  description: "Label for the document list."

docs:
  type: listbox
  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]
  selected_items: [overview.markdown]
  multiple: "false"
  description: "Document listbox used to select the markdown document to render."

up_btn:
  type: button
  title: up
  description: "Scroll the rendered document upward."

down_btn:
  type: button
  title: down
  description: "Scroll the rendered document downward."

viewer:
  type: document_view
  cpp-class: "markdown_viewer_example::DocumentView"
  cpp-header: document_view/document_view.hpp
  description: "Scrollable rendered markdown document area."

close_btn:
  type: button
  title: Quit
  width: 9
  description: "Close the markdown viewer."

status:
  type: label
  text: Ready
  description: "Shows the current document and scroll state."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  color: "#ffffff"
@title:
  background: "#223a5a"
@sidebar:
  background: "#101a2b"
@content:
  background: "#000000"
@viewer:
  background: "#000000"
  padding: 1, 2, 1, 2
@docs:
  background: "#ffffff11"
  color: "#cbd5e1"
  focus-background: "#ffffff22"
  edit-background: "#00000000"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2b5f9f"
@close_btn:
  background: "#7f1d1d"
  color: "#e2e8f0"
  focus-background: "#dc2626"
@status:
  color: "#93c5fd"
  background: "#111827"
```

## User Interface

```ui
+-title-**----------------------------------------------------------+
|header..**................................................close_btn|
+-sidebar------------------+-content-**-----------------------------+
|                          |viewer..**..............................|
| docs_label...**......... |........................................|
| docs.........**......... |*.......................................|
| ........................ |*.......................................|
| #....................... |........................................|
| #....................... |........................................|
|                          |........................................|
| up_btn.... down_btn..... |........................................|
*                          |........................................|
*                          |........................................|
|                          |........................................|
+--------------------------+----------------------------------------+
| status..**....................................................... |
+-------------------------------------------------------------------+
```

""", "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "docs_label", Description = "Label for the document list.", Expose = true },
            new GeneratedElementMetadata { Name = "docs", Description = "Document listbox used to select the markdown document to render.", Expose = true },
            new GeneratedElementMetadata { Name = "up_btn", Description = "Scroll the rendered document upward.", Expose = true },
            new GeneratedElementMetadata { Name = "down_btn", Description = "Scroll the rendered document downward.", Expose = true },
            new GeneratedElementMetadata { Name = "viewer", Description = "Scrollable rendered markdown document area.", Expose = true },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the markdown viewer.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Shows the current document and scroll state.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Markdown Viewer"));
        header.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        docs_label = AddElement(new Label("docs_label", "Documents"));
        docs_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        docs_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        docs = AddElement(new ListBox("docs", new[] {"overview.markdown", "styles.markdown", "tables.markdown", "code.markdown"}));
        docs.SetSelectedIndex(0);
        docs.SetStyle(new Style
    {
        Background = new Color("#ffffff11"),
        Color = new Color("#cbd5e1")
    });
        docs.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff22"),
        Color = new Color("#ffffff")
    });
        docs.SetEditStyle(new Style
    {
        Background = new Color("#00000000"),
        Color = new Color("#cbd5e1")
    });
        docs.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        docs.SetActiveStyle(new Style
    {
        Background = new Color("#DDDDDD99"),
        Color = new Color("#FFFFFF")
    });
        docs.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        up_btn = AddElement(new Button("up_btn", "up"));
        up_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        up_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        up_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        down_btn = AddElement(new Button("down_btn", "down"));
        down_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        down_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        down_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        viewer = AddElement(new ReusableElement("viewer"));
        viewer.SetChild(new DocumentViewUI());
        viewer.SetStyle(new Style
    {
        Background = new Color("#000000"),
        PaddingBottom = 1,
        PaddingLeft = 2,
        PaddingRight = 2,
        PaddingTop = 1
    });
        viewer.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        viewer.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        viewer.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        close_btn = AddElement(new Button("close_btn", "Quit"));
        close_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
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
        status = AddElement(new Label("status", "Ready"));
        status.SetStyle(new Style
    {
        Background = new Color("#111827"),
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
                CellName = "title",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 67, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(58, 1),
                CellCharsSize = new Size(67, 1),
                MarginRight = 9,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#223a5a")
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
                Name = "viewer",
                Type = "document_view",
                CellName = "content",
                Relative = new Rect(0, 0, 536870911, 536870911),
                SourceCell = new Rect(2, 27, 40, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(11),
                CharsSize = new Size(40, 11),
                CellCharsSize = new Size(40, 11),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#000000")
    },
                ElementStyle = new Style
    {
        Background = new Color("#000000"),
        PaddingBottom = 1,
        PaddingLeft = 2,
        PaddingRight = 2,
        PaddingTop = 1
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(14, 0, 67, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(67),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(65, 1),
                CellCharsSize = new Size(67, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "title",
                Relative = new Rect(0, 58, 9, 1),
                SourceCell = new Rect(0, 0, 67, 1),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(67, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#223a5a")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "docs_label",
                Type = "label",
                CellName = "sidebar",
                Relative = new Rect(1, 1, 536870911, 1),
                SourceCell = new Rect(2, 0, 26, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(24, 1),
                CellCharsSize = new Size(26, 11),
                MarginRight = 1,
                MarginBottom = 9,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "docs",
                Type = "listbox",
                CellName = "sidebar",
                Relative = new Rect(2, 1, 536870911, 536870910),
                SourceCell = new Rect(2, 0, 26, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(24, 4),
                CellCharsSize = new Size(26, 11),
                MarginRight = 1,
                MarginBottom = 5,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("#ffffff11"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "up_btn",
                Type = "button",
                CellName = "sidebar",
                Relative = new Rect(7, 1, 10, 1),
                SourceCell = new Rect(2, 0, 26, 11),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(26, 11),
                MarginRight = 15,
                MarginBottom = 3,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "down_btn",
                Type = "button",
                CellName = "sidebar",
                Relative = new Rect(7, 12, 536870911, 1),
                SourceCell = new Rect(2, 0, 26, 11),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(26),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(26, 11),
                MarginRight = 1,
                MarginBottom = 3,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onDocsSelectionChange(List<string> value) { }
    protected virtual bool onDocsItemActivate(int index, string value) { return false; }
    protected virtual void onUpBtnClick() { }
    protected virtual void onDownBtnClick() { }
    protected virtual void onCloseBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "up_btn")
        {
            onUpBtnClick();
            return true;
        }
        else if (name == "down_btn")
        {
            onDownBtnClick();
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
        if (name == "docs")
        {
            onDocsSelectionChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedListBoxItemActivate(string name, string elementId, int index, string value)
    {
        if (name == "docs")
        {
            return onDocsItemActivate(index, value);
        }
        return false;
    }
}

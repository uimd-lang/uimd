// Auto-generated UI code for document_view - DO NOT EDIT MANUALLY.
using Uimd;

public class DocumentViewUI : GeneratedScrollViewBase
{

    public DocumentViewUI() : base("Document View")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "DocumentView", """
# Document View

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: stable
description: "Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables."
tags: [component]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - blocks/paragraph
  - blocks/heading
  - blocks/code_block
  - blocks/table_block
```

## Style

```yaml
this:
  background: "#00000000"
  border-width: 0
@panel:
  gap: 1
  background: "#00000000"
  padding: 1, 2, 1, 2
```

## User Interface

```ui
+-panel-**---------------------------------------------+
|                                                      |
|                                                      |
|                                                      |
|                                                      |
|                                                      |
*                                                      |
*                                                      |
*                                                      |
*                                                      |
|                                                      |
+------------------------------------------------------+
```

""", "Reusable scroll view that contains rendered markdown blocks such as paragraphs, headings, code blocks, and tables.", Array.Empty<GeneratedElementMetadata>());
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        SetGeneratedScrollView(AddElement(new ScrollView("__scrollview", 1)));
        ScrollView().SetStyle(new Style
    {
        Background = new Color("#00000000"),
        Gap = 1,
        PaddingBottom = 1,
        PaddingLeft = 2,
        PaddingRight = 2,
        PaddingTop = 1
    });
        ScrollView().SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        ScrollView().SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        ScrollView().SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "__scrollview",
                Type = "uiscrollview",
                CellName = "panel",
                Relative = new Rect(0, 0, 536870911, 536870911),
                SourceCell = new Rect(0, 0, 54, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(54, 10),
                CellCharsSize = new Size(54, 10),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#00000000"),
        Gap = 1
    },
                ElementStyle = new Style(),
            },
        };
    }

    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
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

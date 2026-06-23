// Auto-generated UI code for activity_feed_panel - DO NOT EDIT MANUALLY.
using Uimd;

public class ActivityFeedPanelUI : GeneratedScrollViewBase
{

    public ActivityFeedPanelUI() : base("Activity Feed Panel")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ActivityFeedPanel", """
# Activity Feed Panel

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Reusable scroll view for activity feed items."
tags: [component]
```

## Definition

```yaml
extends: uiscrollview
uses:
  - ../activity_item/activity_item
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@panel:
  gap: 1
  padding: 1, 0, 1, 0
  background: "#00000000"
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
|                                                      |
+------------------------------------------------------+
```

""", "Reusable scroll view for activity feed items.", Array.Empty<GeneratedElementMetadata>());
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
        PaddingLeft = 0,
        PaddingRight = 0,
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
                SourceCell = new Rect(0, 0, 54, 9),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(54, 9),
                CellCharsSize = new Size(54, 9),
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

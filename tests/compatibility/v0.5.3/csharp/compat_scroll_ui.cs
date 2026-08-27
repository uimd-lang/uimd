// Auto-generated UI code for compat_scroll - DO NOT EDIT MANUALLY.
using Uimd;

public class CompatScrollUI : GeneratedScrollViewBase
{
    public Label alpha_row = null!;

    public CompatScrollUI() : base("UIMD 0.5.3 Compatibility Scroll")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "CompatScroll", """
# UIMD 0.5.3 Compatibility Scroll

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: regression
description: "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate."
tags: [compatibility]
```

## Definition

```yaml
extends: uiscrollview
```

## Members

```yaml
alpha_row:
  type: label
  text: "legacy alpha row"
```

## Style

```yaml
this:
  border-width: 0
  background: "#303545"
@panel:
  background: "#303545"
  padding: 0
@alpha_row:
  background: "#252a36cc"
```

## User Interface

```ui
+-panel-**----------------------+
|alpha_row.....................|
|..............................|
+------------------------------+
```

""", "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.", new[]
        {
            new GeneratedElementMetadata { Name = "alpha_row", Description = "legacy alpha row", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#303545"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        SetGeneratedScrollView(AddElement(new ScrollView("__scrollview", 0)));
        ScrollView().SetStyle(new Style
    {
        Background = new Color("#303545"),
        Padding = 0
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
        alpha_row = AddElement(new Label("alpha_row", "legacy alpha row"));
        alpha_row.SetStyle(new Style
    {
        Background = new Color("#252a36cc"),
        Color = new Color("#cbd5e1")
    });
        alpha_row.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
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

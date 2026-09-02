// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
using Uimd;

public class ChatEntryUI : GeneratedWindowBase
{
    public Label role = null!;
    public Label phase = null!;
    public Label body = null!;
    public Label detail = null!;

    public ChatEntryUI() : base("Agent Chat Entry")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("component");
        SetMcpMetadata(true, "ChatEntry", """
# Agent Chat Entry

## Metadata

```yaml
format: uimd
format-version: 1
kind: component
status: draft
description: "Fixed-height simulated agent chat entry for prose and structured transcript blocks."
tags: [component, simulation]
```

## Definition

```yaml
extends: uicontrol
```

## Members

```yaml
role:
  type: label
  text: Agent
  description: "Simulated transcript speaker or activity type."

phase:
  type: label
  text: response
  description: "Deterministic scenario phase."

body:
  type: spanlabel
  text: ""
  description: "Selectable simulated transcript content."

detail:
  type: label
  text: "simulated · no external action"
  description: "Simulation-only activity detail."
```

## Style

```yaml
this:
  border-width: 0
  background: "#00000000"
@header:
  background: "#162033"
  padding: 0, 1, 0, 1
@role:
  color: "#a7f3d0"
@phase:
  color: "#c4b5fd"
  text-align: right
@body_panel:
  background: "#0b1220"
  padding: 1, 1, 1, 1
@body:
  color: "#d1d5db"
  word-wrap: false
  user-select: text
@footer:
  background: "#111827"
  padding: 0, 1, 0, 1
@detail:
  color: "#64748b"
  user-select: text
```

## User Interface

```ui
+-header-**---------------------------------------------------------------+
| role..**........................................ phase................. |
+-body_panel-**-----------------------------------------------------------+
| body..**............................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
| ....................................................................... |
+-footer-**---------------------------------------------------------------+
| detail..**............................................................. |
+-------------------------------------------------------------------------+
```

""", "Fixed-height simulated agent chat entry for prose and structured transcript blocks.", new[]
        {
            new GeneratedElementMetadata { Name = "role", Description = "Simulated transcript speaker or activity type.", Expose = true },
            new GeneratedElementMetadata { Name = "phase", Description = "Deterministic scenario phase.", Expose = true },
            new GeneratedElementMetadata { Name = "body", Description = "Selectable simulated transcript content.", Expose = true },
            new GeneratedElementMetadata { Name = "detail", Description = "Simulation-only activity detail.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#00000000"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        role = AddElement(new Label("role", "Agent"));
        role.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#a7f3d0")
    });
        role.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        phase = AddElement(new Label("phase", "response"));
        phase.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#c4b5fd"),
        TextAlign = "right"
    });
        phase.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        body = AddElement(new Label("body", ""));
        body.SetStyle(new Style
    {
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    });
        detail = AddElement(new Label("detail", "simulated · no external action"));
        detail.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#64748b"),
        UserSelect = "text"
    });
        detail.SetCursorStyle(new Style
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
                Name = "body",
                Type = "spanlabel",
                CellName = "body_panel",
                Relative = new Rect(0, 1, 536870911, 6),
                SourceCell = new Rect(2, 0, 73, 6),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(6),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(6),
                CharsSize = new Size(71, 6),
                CellCharsSize = new Size(73, 6),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#0b1220"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "detail",
                Type = "label",
                CellName = "footer",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(9, 0, 73, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(71, 1),
                CellCharsSize = new Size(73, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#111827"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
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
                Name = "role",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(0, 0, 73, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(48, 1),
                CellCharsSize = new Size(73, 1),
                MarginRight = 24,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#162033"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#a7f3d0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "phase",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 50, 22, 1),
                SourceCell = new Rect(0, 0, 73, 1),
                Width = AxisDimension.Auto(22),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(22, 1),
                CellCharsSize = new Size(73, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#162033"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#c4b5fd"),
        TextAlign = "right"
    },
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

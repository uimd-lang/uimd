// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
using Uimd;

public class AgentChatUI : GeneratedWindowBase
{
    public Label title = null!;
    public Label simulation_badge = null!;
    public Button quit_button = null!;
    public Button start_button = null!;
    public Button pause_button = null!;
    public Button next_step_button = null!;
    public Button reset_button = null!;
    public Button load_stress_button = null!;
    public ReusableElement transcript = null!;
    public Label status = null!;

    public AgentChatUI() : base("Agent Chat Simulation")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "AgentChat", """
# Agent Chat Simulation

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Deterministic simulated agent chat for rich transcript and large ScrollView testing."
tags: [example, simulation, scrollview]
```

## Definition

```yaml
uses:
  - transcript/transcript
```

## Members

```yaml
title:
  type: label
  text: "Agent Chat Simulation"
  description: "Application title."
  expose: false

simulation_badge:
  type: label
  text: "SIMULATED · OFFLINE"
  description: "Clarifies that the transcript performs no real agent or tool work."
  expose: false

quit_button:
  type: button
  title: Quit
  description: "Close the Agent Chat Simulation example."

start_button:
  type: button
  title: Start
  description: "Run one hundred deterministic simulated messages over three minutes."

pause_button:
  type: button
  title: Pause
  description: "Pause the timed simulation without discarding progress."

next_step_button:
  type: button
  title: "Next step"
  description: "Apply one deterministic scripted chat step."

reset_button:
  type: button
  title: Reset
  description: "Reset the transcript to its initial message."

load_stress_button:
  type: button
  title: "Load 50k"
  description: "Load an approximately fifty-thousand-row deterministic stress transcript."

transcript:
  type: transcript
  cpp-class: "agent_chat_example::Transcript"
  cpp-header: transcript/transcript.hpp
  description: "Scrollable deterministic simulated agent transcript."

status:
  type: label
  text: "Ready · 1 message"
  description: "Shows deterministic simulation progress and transcript size."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@header:
  background: "#111827"
  padding: 0, 1, 0, 1
@title:
  color: "#f8fafc"
  text-align: center
@simulation_badge:
  color: "#c4b5fd"
  text-align: right
@actions:
  background: "#172033"
  padding: 1, 1, 1, 1
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@load_stress_button:
  background: "#6d28d9"
  focus-background: "#7c3aed"
@quit_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@content:
  background: "#030712"
@transcript:
  background: "#00000000"
@status:
  background: "#0f172a"
  color: "#93c5fd"
  padding: 0, 1, 0, 1
```

## User Interface

```ui
+-header-**--------------------------------------------------------------------+
| title..**......................... simulation_badge.... quit_button......... |
+-actions-**-------------------------------------------------------------------+
| start_button pause_button next_step_button reset_button load_stress_button.. |
+-content-**-------------------------------------------------------------------+
| transcript..**.............................................................. |
| ............................................................................ |
* *........................................................................... |
* *........................................................................... |
| ............................................................................ |
+------------------------------------------------------------------------------+
| status..**.................................................................. |
+------------------------------------------------------------------------------+
```

""", "Deterministic simulated agent chat for rich transcript and large ScrollView testing.", new[]
        {
            new GeneratedElementMetadata { Name = "title", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "simulation_badge", Description = "Clarifies that the transcript performs no real agent or tool work.", Expose = false },
            new GeneratedElementMetadata { Name = "quit_button", Description = "Close the Agent Chat Simulation example.", Expose = true },
            new GeneratedElementMetadata { Name = "start_button", Description = "Run one hundred deterministic simulated messages over three minutes.", Expose = true },
            new GeneratedElementMetadata { Name = "pause_button", Description = "Pause the timed simulation without discarding progress.", Expose = true },
            new GeneratedElementMetadata { Name = "next_step_button", Description = "Apply one deterministic scripted chat step.", Expose = true },
            new GeneratedElementMetadata { Name = "reset_button", Description = "Reset the transcript to its initial message.", Expose = true },
            new GeneratedElementMetadata { Name = "load_stress_button", Description = "Load an approximately fifty-thousand-row deterministic stress transcript.", Expose = true },
            new GeneratedElementMetadata { Name = "transcript", Description = "Scrollable deterministic simulated agent transcript.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Shows deterministic simulation progress and transcript size.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#030712"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        title = AddElement(new Label("title", "Agent Chat Simulation"));
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
        simulation_badge = AddElement(new Label("simulation_badge", "SIMULATED · OFFLINE"));
        simulation_badge.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#c4b5fd"),
        TextAlign = "right"
    });
        simulation_badge.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        quit_button = AddElement(new Button("quit_button", "Quit"));
        quit_button.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        quit_button.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        quit_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        start_button = AddElement(new Button("start_button", "Start"));
        start_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        start_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        start_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        pause_button = AddElement(new Button("pause_button", "Pause"));
        pause_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        pause_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        pause_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        next_step_button = AddElement(new Button("next_step_button", "Next step"));
        next_step_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        next_step_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        next_step_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        reset_button = AddElement(new Button("reset_button", "Reset"));
        reset_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        reset_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        reset_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        load_stress_button = AddElement(new Button("load_stress_button", "Load 50k"));
        load_stress_button.SetStyle(new Style
    {
        Background = new Color("#6d28d9"),
        Color = new Color("#e2e8f0")
    });
        load_stress_button.SetFocusStyle(new Style
    {
        Background = new Color("#7c3aed"),
        Color = new Color("#ffffff")
    });
        load_stress_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        transcript = AddElement(new ReusableElement("transcript"));
        transcript.SetChild(new TranscriptUI());
        transcript.SetStyle(new Style
    {
        Background = new Color("#00000000")
    });
        transcript.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        transcript.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        transcript.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        status = AddElement(new Label("status", "Ready · 1 message"));
        status.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#93c5fd"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
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
                Name = "start_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 1, 12, 1),
                SourceCell = new Rect(2, 0, 78, 1),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 65,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
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
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(10, 0, 78, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(78),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(76, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#93c5fd"),
        PaddingBottom = 0,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 0
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
                CharsSize = new Size(34, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 43,
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
        Color = new Color("#f8fafc"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "transcript",
                Type = "transcript",
                CellName = "content",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(4, 0, 78, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(76, 5),
                CellCharsSize = new Size(78, 5),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#030712")
    },
                ElementStyle = new Style
    {
        Background = new Color("#00000000")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "pause_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 14, 12, 1),
                SourceCell = new Rect(2, 0, 78, 1),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 52,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
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
                Name = "next_step_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 27, 16, 1),
                SourceCell = new Rect(2, 0, 78, 1),
                Width = AxisDimension.Auto(16),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(16, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 35,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
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
                Name = "simulation_badge",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 36, 20, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 22,
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
        Color = new Color("#c4b5fd"),
        TextAlign = "right"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "reset_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 44, 12, 1),
                SourceCell = new Rect(2, 0, 78, 1),
                Width = AxisDimension.Auto(12),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(12, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 22,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
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
                Name = "load_stress_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 57, 20, 1),
                SourceCell = new Rect(2, 0, 78, 1),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(78, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#172033"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("#6d28d9"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "quit_button",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 57, 20, 1),
                SourceCell = new Rect(0, 0, 78, 1),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(78, 1),
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
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onQuitButtonClick() { }
    protected virtual void onStartButtonClick() { }
    protected virtual void onPauseButtonClick() { }
    protected virtual void onNextStepButtonClick() { }
    protected virtual void onResetButtonClick() { }
    protected virtual void onLoadStressButtonClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "quit_button")
        {
            onQuitButtonClick();
            return true;
        }
        else if (name == "start_button")
        {
            onStartButtonClick();
            return true;
        }
        else if (name == "pause_button")
        {
            onPauseButtonClick();
            return true;
        }
        else if (name == "next_step_button")
        {
            onNextStepButtonClick();
            return true;
        }
        else if (name == "reset_button")
        {
            onResetButtonClick();
            return true;
        }
        else if (name == "load_stress_button")
        {
            onLoadStressButtonClick();
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

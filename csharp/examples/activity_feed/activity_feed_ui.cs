// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
using Uimd;

public class ActivityFeedUI : GeneratedWindowBase
{
    public Label header_title = null!;
    public Button settings_button = null!;
    public Button quit_button = null!;
    public ReusableElement feed = null!;
    public Label type_label = null!;
    public ComboBox activity_type = null!;
    public Label message_label = null!;
    public TextArea message = null!;
    public Button add_button = null!;
    public Button seed_button = null!;
    public Button clear_button = null!;
    public Label status = null!;

    public ActivityFeedUI() : base("Activity Feed")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ActivityFeed", """
# Activity Feed

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Activity feed example with dynamically appended scroll view items and a settings dialog."
tags: [example]
```

## Definition

```yaml
uses:
  - activity_feed_panel/activity_feed_panel
  - settings/settings
```

## Members

```yaml
header_title:
  type: label
  text: "Activity Feed"
  description: "Application title."
  expose: false

settings_button:
  type: button
  title: Settings
  description: "Open the activity feed settings dialog."

quit_button:
  type: button
  title: Quit
  description: "Close the activity feed example."

feed:
  type: activity_feed_panel
  description: "Scrollable activity timeline."

type_label:
  type: label
  text: Type
  description: "Label for activity type selector."
  expose: false

activity_type:
  type: combobox
  options: [Info, Task, Warning, Deploy, Note]
  selected_item: Info
  description: "Activity type selector."

message_label:
  type: label
  text: Message
  description: "Label for activity message input."
  expose: false

message:
  type: textarea
  value: ""
  maxlength: 180
  description: "Activity message input."

add_button:
  type: button
  title: Add
  description: "Append one activity item to the feed."

seed_button:
  type: button
  title: Seed
  description: "Append sample activity items to the feed."

clear_button:
  type: button
  title: Clear
  description: "Remove all activity items from the feed."

status:
  type: label
  text: "Ready"
  description: "Shows activity feed status."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  background: "#1f2937"
@header_title:
  text-align: center
  color: "#ffffff"
@content:
  background: "#030712"
@feed:
  background: "#00000000"
@controls:
  background: "#172033"
  padding: 1, 1, 1, 1
@actions:
  background: "#172033"
  padding: 1, 1, 1, 1
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@clear_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@quit_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
textinput:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
textarea:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  cursor-background: "#facc15"
  cursor-color: "#111827"
combobox:
  background: "#0b1220"
  color: "#e5e7eb"
  focus-background: "#25364f"
  edit-background: "#0b1220"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
@status:
  background: "#0f172a"
  color: "#93c5fd"
```

## User Interface

```ui
+-header-**-------------------------------------------------------+
|header_title..**................. settings_button.. quit_button..|
+-content-**------------------------------------------------------+
| feed..**....................................................... |
| ............................................................... |
| ............................................................... |
* *.............................................................. |
* *.............................................................. |
* *.............................................................. |
| ............................................................... |
+-controls-**-----------------------------------------------------+
| type_label     activity_type.........                           |
|                                                                 |
| message_label  message..**....................................  |
|               ...............................................   |
|               ...............................................   |
+-actions-**------------------------------------------------------+
| add_button......... seed_button......... clear_button.......... |
+-----------------------------------------------------------------+
| status..**..................................................... |
+-----------------------------------------------------------------+
```

""", "Activity feed example with dynamically appended scroll view items and a settings dialog.", new[]
        {
            new GeneratedElementMetadata { Name = "header_title", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "settings_button", Description = "Open the activity feed settings dialog.", Expose = true },
            new GeneratedElementMetadata { Name = "quit_button", Description = "Close the activity feed example.", Expose = true },
            new GeneratedElementMetadata { Name = "feed", Description = "Scrollable activity timeline.", Expose = true },
            new GeneratedElementMetadata { Name = "type_label", Description = "Label for activity type selector.", Expose = false },
            new GeneratedElementMetadata { Name = "activity_type", Description = "Activity type selector.", Expose = true },
            new GeneratedElementMetadata { Name = "message_label", Description = "Label for activity message input.", Expose = false },
            new GeneratedElementMetadata { Name = "message", Description = "Activity message input.", Expose = true },
            new GeneratedElementMetadata { Name = "add_button", Description = "Append one activity item to the feed.", Expose = true },
            new GeneratedElementMetadata { Name = "seed_button", Description = "Append sample activity items to the feed.", Expose = true },
            new GeneratedElementMetadata { Name = "clear_button", Description = "Remove all activity items from the feed.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Shows activity feed status.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header_title = AddElement(new Label("header_title", "Activity Feed"));
        header_title.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header_title.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        settings_button = AddElement(new Button("settings_button", "Settings"));
        settings_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        settings_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        settings_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
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
        feed = AddElement(new ReusableElement("feed"));
        feed.SetChild(new ActivityFeedPanelUI());
        feed.SetStyle(new Style
    {
        Background = new Color("#00000000")
    });
        feed.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff14")
    });
        feed.SetEditStyle(new Style
    {
        Background = new Color("#ffffff18")
    });
        feed.SetSelectedStyle(new Style
    {
        Background = new Color("#ffffff26"),
        Color = new Color("#ffffff")
    });
        type_label = AddElement(new Label("type_label", "Type"));
        type_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        type_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        activity_type = AddElement(new ComboBox("activity_type", new[] {"Info", "Task", "Warning", "Deploy", "Note"}));
        activity_type.SetSelectedIndex(0);
        activity_type.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        activity_type.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        activity_type.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        activity_type.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        activity_type.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        message_label = AddElement(new Label("message_label", "Message"));
        message_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        message_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        message = AddElement(new TextArea("message", "", 180));
        message.SetStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    });
        message.SetFocusStyle(new Style
    {
        Background = new Color("#25364f"),
        Color = new Color("#ffffff")
    });
        message.SetEditStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#ffffff")
    });
        message.SetCursorStyle(new Style
    {
        Background = new Color("#facc15"),
        Color = new Color("#111827")
    });
        message.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        message.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        add_button = AddElement(new Button("add_button", "Add"));
        add_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        add_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        add_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        seed_button = AddElement(new Button("seed_button", "Seed"));
        seed_button.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        seed_button.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        seed_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        clear_button = AddElement(new Button("clear_button", "Clear"));
        clear_button.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        clear_button.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        clear_button.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        status = AddElement(new Label("status", "Ready"));
        status.SetStyle(new Style
    {
        Background = new Color("#0f172a"),
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
                Name = "header_title",
                Type = "label",
                CellName = "header",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 65, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 32,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
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
                Name = "add_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 1, 19, 1),
                SourceCell = new Rect(16, 0, 65, 1),
                Width = AxisDimension.Auto(19),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(19, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 45,
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
                Name = "feed",
                Type = "activity_feed_panel",
                CellName = "content",
                Relative = new Rect(0, 1, 536870911, 536870911),
                SourceCell = new Rect(2, 0, 65, 7),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Expanded(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(63, 7),
                CellCharsSize = new Size(65, 7),
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
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(18, 0, 65, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(65),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(63, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0f172a"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "type_label",
                Type = "label",
                CellName = "controls",
                Relative = new Rect(0, 1, 10, 1),
                SourceCell = new Rect(10, 0, 65, 5),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(65, 5),
                MarginRight = 54,
                MarginBottom = 4,
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
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "activity_type",
                Type = "combobox",
                CellName = "controls",
                Relative = new Rect(0, 16, 22, 1),
                SourceCell = new Rect(10, 0, 65, 5),
                Width = AxisDimension.Auto(22),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(22, 1),
                CellCharsSize = new Size(65, 5),
                MarginRight = 27,
                MarginBottom = 4,
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
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "seed_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 21, 20, 1),
                SourceCell = new Rect(16, 0, 65, 1),
                Width = AxisDimension.Auto(20),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(20, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 24,
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
                Name = "settings_button",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 34, 17, 1),
                SourceCell = new Rect(0, 0, 65, 1),
                Width = AxisDimension.Auto(17),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 14,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
    },
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "clear_button",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(0, 42, 22, 1),
                SourceCell = new Rect(16, 0, 65, 1),
                Width = AxisDimension.Auto(22),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(22, 1),
                CellCharsSize = new Size(65, 1),
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
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "quit_button",
                Type = "button",
                CellName = "header",
                Relative = new Rect(0, 52, 13, 1),
                SourceCell = new Rect(0, 0, 65, 1),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(65, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#1f2937")
    },
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "message_label",
                Type = "label",
                CellName = "controls",
                Relative = new Rect(2, 1, 13, 1),
                SourceCell = new Rect(10, 0, 65, 5),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(65, 5),
                MarginRight = 51,
                MarginBottom = 2,
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
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "message",
                Type = "textarea",
                CellName = "controls",
                Relative = new Rect(2, 16, 536870911, 3),
                SourceCell = new Rect(10, 0, 65, 5),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(3),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(5),
                CharsSize = new Size(47, 3),
                CellCharsSize = new Size(65, 5),
                MarginRight = 2,
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
        Background = new Color("#0b1220"),
        Color = new Color("#e5e7eb")
    },
            },
        };
    }

    protected virtual void onSettingsButtonClick() { }
    protected virtual void onQuitButtonClick() { }
    protected virtual void onActivityTypeChange(string value) { }
    protected virtual void onMessageChange(string value) { }
    protected virtual void onMessageSubmit(string value) { }
    protected virtual void onAddButtonClick() { }
    protected virtual void onSeedButtonClick() { }
    protected virtual void onClearButtonClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "settings_button")
        {
            onSettingsButtonClick();
            return true;
        }
        else if (name == "quit_button")
        {
            onQuitButtonClick();
            return true;
        }
        else if (name == "add_button")
        {
            onAddButtonClick();
            return true;
        }
        else if (name == "seed_button")
        {
            onSeedButtonClick();
            return true;
        }
        else if (name == "clear_button")
        {
            onClearButtonClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "message")
        {
            onMessageChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "message")
        {
            onMessageSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "activity_type")
        {
            onActivityTypeChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        return false;
    }
}

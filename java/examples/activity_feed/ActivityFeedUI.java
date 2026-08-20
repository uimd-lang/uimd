// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.ComboBox;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextArea;
import uimd.TextGradient;

public class ActivityFeedUI extends GeneratedWindowBase
{
    public Label header_title;
    public Button settings_button;
    public Button quit_button;
    public ReusableElement feed;
    public Label type_label;
    public ComboBox activity_type;
    public Label message_label;
    public TextArea message;
    public Button add_button;
    public Button seed_button;
    public Button clear_button;
    public Label status;

    public ActivityFeedUI()
    {
        super("Activity Feed");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ActivityFeed",
            "# Activity Feed\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Activity feed example with dynamically appended scroll view items and a settings dialog.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - activity_feed_panel/activity_feed_panel\n  - settings/settings\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed\"\n  description: \"Application title.\"\n  expose: false\n\nsettings_button:\n  type: button\n  title: Settings\n  description: \"Open the activity feed settings dialog.\"\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the activity feed example.\"\n\nfeed:\n  type: activity_feed_panel\n  description: \"Scrollable activity timeline.\"\n\ntype_label:\n  type: label\n  text: Type\n  description: \"Label for activity type selector.\"\n  expose: false\n\nactivity_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Activity type selector.\"\n\nmessage_label:\n  type: label\n  text: Message\n  description: \"Label for activity message input.\"\n  expose: false\n\nmessage:\n  type: textarea\n  value: \"\"\n  maxlength: 180\n  description: \"Activity message input.\"\n\nadd_button:\n  type: button\n  title: Add\n  description: \"Append one activity item to the feed.\"\n\nseed_button:\n  type: button\n  title: Seed\n  description: \"Append sample activity items to the feed.\"\n\nclear_button:\n  type: button\n  title: Clear\n  description: \"Remove all activity items from the feed.\"\n\nstatus:\n  type: label\n  text: \"Ready\"\n  description: \"Shows activity feed status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\n@content:\n  background: \"#030712\"\n@feed:\n  background: \"#00000000\"\n@controls:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@clear_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**-------------------------------------------------------+\n|header_title..**................. settings_button.. quit_button..|\n+-content-**------------------------------------------------------+\n| feed..**....................................................... |\n| ............................................................... |\n| ............................................................... |\n* *.............................................................. |\n* *.............................................................. |\n* *.............................................................. |\n| ............................................................... |\n+-controls-**-----------------------------------------------------+\n| type_label     activity_type.........                           |\n|                                                                 |\n| message_label  message..**....................................  |\n|               ...............................................   |\n|               ...............................................   |\n+-actions-**------------------------------------------------------+\n| add_button......... seed_button......... clear_button.......... |\n+-----------------------------------------------------------------+\n| status..**..................................................... |\n+-----------------------------------------------------------------+\n```\n",
            "Activity feed example with dynamically appended scroll view items and a settings dialog.",
            List.of(
            new GeneratedElementMetadata("header_title", "Application title.", false),
            new GeneratedElementMetadata("settings_button", "Open the activity feed settings dialog.", true),
            new GeneratedElementMetadata("quit_button", "Close the activity feed example.", true),
            new GeneratedElementMetadata("feed", "Scrollable activity timeline.", true),
            new GeneratedElementMetadata("type_label", "Label for activity type selector.", false),
            new GeneratedElementMetadata("activity_type", "Activity type selector.", true),
            new GeneratedElementMetadata("message_label", "Label for activity message input.", false),
            new GeneratedElementMetadata("message", "Activity message input.", true),
            new GeneratedElementMetadata("add_button", "Append one activity item to the feed.", true),
            new GeneratedElementMetadata("seed_button", "Append sample activity items to the feed.", true),
            new GeneratedElementMetadata("clear_button", "Remove all activity items from the feed.", true),
            new GeneratedElementMetadata("status", "Shows activity feed status.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header_title = addElement(new Label("header_title", "Activity Feed"));
        header_title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        header_title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        settings_button = addElement(new Button("settings_button", "Settings"));
        settings_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        settings_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        settings_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        quit_button = addElement(new Button("quit_button", "Quit"));
        quit_button.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_button.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        feed = addElement(new ReusableElement("feed", "activity_feed_panel"));
        feed.setChild(new ActivityFeedPanelUI());
        feed.setStyle(Style.fromProperties("background", "#00000000"));
        feed.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        feed.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        feed.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        type_label = addElement(new Label("type_label", "Type"));
        type_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        type_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        activity_type = addElement(new ComboBox("activity_type", List.of("Info", "Task", "Warning", "Deploy", "Note")));
        activity_type.setSelectedIndex(0);
        activity_type.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        activity_type.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        activity_type.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        activity_type.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        activity_type.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        message_label = addElement(new Label("message_label", "Message"));
        message_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        message_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        message = addElement(new TextArea("message", "", 180));
        message.setStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"));
        message.setFocusStyle(Style.fromProperties("background", "#25364f", "color", "#ffffff"));
        message.setEditStyle(Style.fromProperties("background", "#0b1220", "color", "#ffffff"));
        message.setCursorStyle(Style.fromProperties("background", "#facc15", "color", "#111827"));
        message.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        message.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        add_button = addElement(new Button("add_button", "Add"));
        add_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        add_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        add_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        seed_button = addElement(new Button("seed_button", "Seed"));
        seed_button.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        seed_button.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        seed_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        clear_button = addElement(new Button("clear_button", "Clear"));
        clear_button.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        clear_button.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        clear_button.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        status = addElement(new Label("status", "Ready"));
        status.setStyle(Style.fromProperties("background", "#0f172a", "color", "#93c5fd"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
    }

    private static TextGradient makeTextGradient(
        int intervalMs,
        int step,
        int segmentSize,
        String... colors)
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(intervalMs);
        gradient.setStep(step);
        gradient.setSegmentSize(segmentSize);
        for (String color : colors)
        {
            gradient.colors().add(new Color(color));
        }
        return gradient;
    }

    private static Style styleWithGradients(
        Style style,
        TextGradient colorGradient,
        TextGradient backgroundGradient)
    {
        style.setTextColorGradient(colorGradient);
        style.setTextBackgroundGradient(backgroundGradient);
        return style;
    }

    private static List<GeneratedLayoutEntry> buildLayout()
    {
        return List.of(
            new GeneratedLayoutEntry()
                .setName("header_title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 65, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(32)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("add_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 1, 19, 1))
                .setSourceCell(new Rect(16, 0, 65, 1))
                .setWidth(AxisDimension.auto(19))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(19, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(45)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("feed")
                .setType("activity_feed_panel")
                .setCellName("content")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(2, 0, 65, 7))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(63, 7))
                .setCellCharsSize(new Size(65, 7))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(Style.fromProperties("background", "#00000000")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(18, 0, 65, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(65))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(63, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0f172a", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("type_label")
                .setType("label")
                .setCellName("controls")
                .setRelative(new Rect(0, 1, 10, 1))
                .setSourceCell(new Rect(10, 0, 65, 5))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(65, 5))
                .setMarginRight(54)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("activity_type")
                .setType("combobox")
                .setCellName("controls")
                .setRelative(new Rect(0, 16, 22, 1))
                .setSourceCell(new Rect(10, 0, 65, 5))
                .setWidth(AxisDimension.auto(22))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(22, 1))
                .setCellCharsSize(new Size(65, 5))
                .setMarginRight(27)
                .setMarginBottom(4)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("seed_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 21, 20, 1))
                .setSourceCell(new Rect(16, 0, 65, 1))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(24)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("settings_button")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 34, 17, 1))
                .setSourceCell(new Rect(0, 0, 65, 1))
                .setWidth(AxisDimension.auto(17))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(14)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("clear_button")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(0, 42, 22, 1))
                .setSourceCell(new Rect(16, 0, 65, 1))
                .setWidth(AxisDimension.auto(22))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(22, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("quit_button")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 52, 13, 1))
                .setSourceCell(new Rect(0, 0, 65, 1))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(65, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("message_label")
                .setType("label")
                .setCellName("controls")
                .setRelative(new Rect(2, 1, 13, 1))
                .setSourceCell(new Rect(10, 0, 65, 5))
                .setWidth(AxisDimension.auto(13))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(65, 5))
                .setMarginRight(51)
                .setMarginBottom(2)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("message")
                .setType("textarea")
                .setCellName("controls")
                .setRelative(new Rect(2, 16, 536870911, 3))
                .setSourceCell(new Rect(10, 0, 65, 5))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(3))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(5))
                .setCharsSize(new Size(47, 3))
                .setCellCharsSize(new Size(65, 5))
                .setMarginRight(2)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "#0b1220", "color", "#e5e7eb"))
        );
    }

    protected void onSettingsButtonClick()
    {
    }

    protected void onQuitButtonClick()
    {
    }

    protected void onActivityTypeChange(String value)
    {
    }

    protected void onMessageChange(String value)
    {
    }

    protected void onMessageSubmit(String value)
    {
    }

    protected void onAddButtonClick()
    {
    }

    protected void onSeedButtonClick()
    {
    }

    protected void onClearButtonClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("settings_button".equals(name))
        {
            onSettingsButtonClick();
            return true;
        }
        if ("quit_button".equals(name))
        {
            onQuitButtonClick();
            return true;
        }
        if ("add_button".equals(name))
        {
            onAddButtonClick();
            return true;
        }
        if ("seed_button".equals(name))
        {
            onSeedButtonClick();
            return true;
        }
        if ("clear_button".equals(name))
        {
            onClearButtonClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("message".equals(name))
        {
            onMessageChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("message".equals(name))
        {
            onMessageSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("activity_type".equals(name))
        {
            onActivityTypeChange(value.isEmpty() ? "" : value.get(0));
            return true;
        }
        return false;
    }
}

// Auto-generated UI code for activity_feed - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "activity_feed_panel/activity_feed_panel_ui.rs"]
mod uimd_dependency_activity_feed_panel;
#[path = "settings/settings_ui.rs"]
mod uimd_dependency_settings;

pub struct ActivityFeedUI
{
    pub base: uimd::GeneratedWindow,
    pub header_title: uimd::LabelRef,
    pub settings_button: uimd::ButtonRef,
    pub quit_button: uimd::ButtonRef,
    pub feed: uimd::ReusableElementRef,
    pub type_label: uimd::LabelRef,
    pub activity_type: uimd::ComboBoxRef,
    pub message_label: uimd::LabelRef,
    pub message: uimd::TextAreaRef,
    pub add_button: uimd::ButtonRef,
    pub seed_button: uimd::ButtonRef,
    pub clear_button: uimd::ButtonRef,
    pub status: uimd::LabelRef,
}

impl ActivityFeedUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Activity Feed");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 32, margin_bottom: 0,
            content: "header_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 16, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 19, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 19, chars_height: 1,
            margin_right: 45, margin_bottom: 0,
            content: "add_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "activity_feed_panel".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 7,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 63, chars_height: 7,
            margin_right: 1, margin_bottom: 0,
            content: "feed".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 18, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 65, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 63, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 5,
            cell_name: "controls".to_string(), cell_width: 0, cell_height: 5,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 54, margin_bottom: 4,
            content: "type_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 0, col: 16,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 5,
            cell_name: "controls".to_string(), cell_width: 0, cell_height: 5,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 22, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 1,
            margin_right: 27, margin_bottom: 4,
            content: "activity_type".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 21,
            cell_row: 16, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 24, margin_bottom: 0,
            content: "seed_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 34,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 17, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 14, margin_bottom: 0,
            content: "settings_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 42,
            cell_row: 16, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 22, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "clear_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 52,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "quit_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 2, col: 1,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 5,
            cell_name: "controls".to_string(), cell_width: 0, cell_height: 5,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 51, margin_bottom: 2,
            content: "message_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 2, col: 16,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 65, cell_chars_height: 5,
            cell_name: "controls".to_string(), cell_width: 0, cell_height: 5,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 47, chars_height: 3,
            margin_right: 2, margin_bottom: 0,
            content: "message".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#162033")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(0),
        border_width_vertical: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("dialog_header", uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("window_header", uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("header", uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    });
        base.set_generated_named_style("header_title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("feed", uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    });
        base.set_generated_named_style("controls", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("actions", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("clear_button", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_button", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ActivityFeed", "# Activity Feed\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Activity feed example with dynamically appended scroll view items and a settings dialog.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - activity_feed_panel/activity_feed_panel\n  - settings/settings\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed\"\n  description: \"Application title.\"\n  expose: false\n\nsettings_button:\n  type: button\n  title: Settings\n  description: \"Open the activity feed settings dialog.\"\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the activity feed example.\"\n\nfeed:\n  type: activity_feed_panel\n  description: \"Scrollable activity timeline.\"\n\ntype_label:\n  type: label\n  text: Type\n  description: \"Label for activity type selector.\"\n  expose: false\n\nactivity_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Activity type selector.\"\n\nmessage_label:\n  type: label\n  text: Message\n  description: \"Label for activity message input.\"\n  expose: false\n\nmessage:\n  type: textarea\n  value: \"\"\n  maxlength: 180\n  description: \"Activity message input.\"\n\nadd_button:\n  type: button\n  title: Add\n  description: \"Append one activity item to the feed.\"\n\nseed_button:\n  type: button\n  title: Seed\n  description: \"Append sample activity items to the feed.\"\n\nclear_button:\n  type: button\n  title: Clear\n  description: \"Remove all activity items from the feed.\"\n\nstatus:\n  type: label\n  text: \"Ready\"\n  description: \"Shows activity feed status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\n@content:\n  background: \"#030712\"\n@feed:\n  background: \"#00000000\"\n@controls:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@clear_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**-------------------------------------------------------+\n|header_title..**................. settings_button.. quit_button..|\n+-content-**------------------------------------------------------+\n| feed..**....................................................... |\n| ............................................................... |\n| ............................................................... |\n* *.............................................................. |\n* *.............................................................. |\n* *.............................................................. |\n| ............................................................... |\n+-controls-**-----------------------------------------------------+\n| type_label     activity_type.........                           |\n|                                                                 |\n| message_label  message..**....................................  |\n|               ...............................................   |\n|               ...............................................   |\n+-actions-**------------------------------------------------------+\n| add_button......... seed_button......... clear_button.......... |\n+-----------------------------------------------------------------+\n| status..**..................................................... |\n+-----------------------------------------------------------------+\n```\n", "Activity feed example with dynamically appended scroll view items and a settings dialog.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header_title", "Application title.", false), uimd::GeneratedElementMetadata::new("settings_button", "Open the activity feed settings dialog.", true), uimd::GeneratedElementMetadata::new("quit_button", "Close the activity feed example.", true), uimd::GeneratedElementMetadata::new("feed", "Scrollable activity timeline.", true), uimd::GeneratedElementMetadata::new("type_label", "Label for activity type selector.", false), uimd::GeneratedElementMetadata::new("activity_type", "Activity type selector.", true), uimd::GeneratedElementMetadata::new("message_label", "Label for activity message input.", false), uimd::GeneratedElementMetadata::new("message", "Activity message input.", true), uimd::GeneratedElementMetadata::new("add_button", "Append one activity item to the feed.", true), uimd::GeneratedElementMetadata::new("seed_button", "Append sample activity items to the feed.", true), uimd::GeneratedElementMetadata::new("clear_button", "Remove all activity items from the feed.", true), uimd::GeneratedElementMetadata::new("status", "Shows activity feed status.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header_title = uimd::new_label("header_title", "Activity Feed");
        base.add_element(header_title.clone());
        header_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        header_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let settings_button = uimd::new_button("settings_button", "Settings");
        base.add_element(settings_button.clone());
        settings_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        settings_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        settings_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let quit_button = uimd::new_button("quit_button", "Quit");
        base.add_element(quit_button.clone());
        quit_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        quit_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        quit_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let feed = uimd::new_reusable_element("feed", "activity_feed_panel");
        base.add_element(feed.clone());
        feed.borrow_mut().set_child_window(uimd_dependency_activity_feed_panel::ActivityFeedPanelUI::new().base);
        feed.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    });
        feed.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        feed.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        feed.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let type_label = uimd::new_label("type_label", "Type");
        base.add_element(type_label.clone());
        type_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        type_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let activity_type = uimd::new_combo_box("activity_type", vec!["Info".to_string(), "Task".to_string(), "Warning".to_string(), "Deploy".to_string(), "Note".to_string()]);
        base.add_element(activity_type.clone());
        activity_type.borrow_mut().set_selected_index(0);
        activity_type.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        activity_type.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        activity_type.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        activity_type.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        activity_type.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let message_label = uimd::new_label("message_label", "Message");
        base.add_element(message_label.clone());
        message_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        message_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let message = uimd::new_text_area_with_max_length("message", "", 180);
        base.add_element(message.clone());
        message.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        message.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        message.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        message.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        message.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        message.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let add_button = uimd::new_button("add_button", "Add");
        base.add_element(add_button.clone());
        add_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        add_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        add_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let seed_button = uimd::new_button("seed_button", "Seed");
        base.add_element(seed_button.clone());
        seed_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        seed_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        seed_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let clear_button = uimd::new_button("clear_button", "Clear");
        base.add_element(clear_button.clone());
        clear_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        clear_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        clear_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let status = uimd::new_label("status", "Ready");
        base.add_element(status.clone());
        status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        status.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            header_title,
            settings_button,
            quit_button,
            feed,
            type_label,
            activity_type,
            message_label,
            message,
            add_button,
            seed_button,
            clear_button,
            status,
        }
    }

    pub fn run<H: ActivityFeedUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ActivityFeedUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ActivityFeedUI
{
    fn default() -> Self { Self::new() }
}

pub trait ActivityFeedUIEvents
{
    fn on_settings_button_click(&mut self, _ui: &mut ActivityFeedUI) {}
    fn on_quit_button_click(&mut self, _ui: &mut ActivityFeedUI) {}
    fn on_add_button_click(&mut self, _ui: &mut ActivityFeedUI) {}
    fn on_seed_button_click(&mut self, _ui: &mut ActivityFeedUI) {}
    fn on_clear_button_click(&mut self, _ui: &mut ActivityFeedUI) {}
    fn on_message_change(&mut self, _ui: &mut ActivityFeedUI, _value: &str) {}
    fn on_message_submit(&mut self, _ui: &mut ActivityFeedUI, _value: &str) {}
    fn on_activity_type_change(&mut self, _ui: &mut ActivityFeedUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ActivityFeedUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ActivityFeedUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ActivityFeedUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ActivityFeedUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ActivityFeedUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ActivityFeedUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ActivityFeedUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ActivityFeedUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ActivityFeedUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ActivityFeedUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ActivityFeedUIRuntime<'a, H: ActivityFeedUIEvents>
{
    ui: &'a mut ActivityFeedUI,
    handler: &'a mut H,
}

impl<H: ActivityFeedUIEvents> uimd::GeneratedApplication for ActivityFeedUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "settings_button" { self.handler.on_settings_button_click(self.ui); return true; }
        if name == "quit_button" { self.handler.on_quit_button_click(self.ui); return true; }
        if name == "add_button" { self.handler.on_add_button_click(self.ui); return true; }
        if name == "seed_button" { self.handler.on_seed_button_click(self.ui); return true; }
        if name == "clear_button" { self.handler.on_clear_button_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "message" { self.handler.on_message_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "message" { self.handler.on_message_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "activity_type" { self.handler.on_activity_type_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        self.handler.handle_dynamic_selection_changed(self.ui, name, value)
    }

    fn handle_focus_changed(&mut self, name: &str, focused: bool) -> bool
    {
        self.handler.on_focus_changed(self.ui, name, focused);
        true
    }

    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool
    {
        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)
    }

    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

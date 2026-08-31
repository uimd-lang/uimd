// Auto-generated UI code for widget_gallery - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct WidgetGalleryUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub close_btn: uimd::ButtonRef,
    pub plain_label: uimd::LabelRef,
    pub selectable_label: uimd::LabelRef,
    pub spans: uimd::SpanLabelRef,
    pub animation: uimd::LabelRef,
    pub name_label: uimd::LabelRef,
    pub name_input: uimd::TextInputRef,
    pub count_label: uimd::LabelRef,
    pub count_input: uimd::NumberInputRef,
    pub enabled_check: uimd::CheckBoxRef,
    pub theme_label: uimd::LabelRef,
    pub theme_combo: uimd::ComboBoxRef,
    pub mode_label: uimd::LabelRef,
    pub mode_list: uimd::ListBoxRef,
    pub apply_btn: uimd::ButtonRef,
    pub reset_btn: uimd::ButtonRef,
    pub summary: uimd::LabelRef,
}

impl WidgetGalleryUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Widget Gallery");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 54, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 38, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 38, chars_height: 1,
            margin_right: 15, margin_bottom: 0,
            content: "header".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#18324f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#18324f")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 2,
            cell_row: 12, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 9,
            cell_name: "control_section".to_string(), cell_width: 27, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 15, margin_bottom: 8,
            content: "name_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 2,
            cell_row: 12, cell_col: 28,
            cell_chars_width: 26, cell_chars_height: 9,
            cell_name: "section2".to_string(), cell_width: 26, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 13, margin_bottom: 8,
            content: "theme_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 40,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 54, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "close_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#18324f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 1, col: 2,
            cell_row: 12, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 9,
            cell_name: "control_section".to_string(), cell_width: 27, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "name_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 9,
            cell_name: "label_section".to_string(), cell_width: 54, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "plain_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 22, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 10,
            cell_name: "".to_string(), cell_width: 54, cell_height: 10,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 6,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 6,
            margin_right: 2, margin_bottom: 3,
            content: "summary".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#dbeafe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 3,
            cell_row: 12, cell_col: 28,
            cell_chars_width: 26, cell_chars_height: 9,
            cell_name: "section2".to_string(), cell_width: 26, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "theme_combo".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 2,
            cell_row: 12, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 9,
            cell_name: "control_section".to_string(), cell_width: 27, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 14, margin_bottom: 5,
            content: "count_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 2,
            cell_row: 12, cell_col: 28,
            cell_chars_width: 26, cell_chars_height: 9,
            cell_name: "section2".to_string(), cell_width: 26, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 14, margin_bottom: 5,
            content: "mode_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 9,
            cell_name: "label_section".to_string(), cell_width: 54, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 1,
            margin_right: 2, margin_bottom: 5,
            content: "selectable_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#a7f3d0")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "numberinput".to_string(),
            row: 4, col: 2,
            cell_row: 12, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 9,
            cell_name: "control_section".to_string(), cell_width: 27, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 2, margin_bottom: 4,
            content: "count_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "listbox".to_string(),
            row: 4, col: 2,
            cell_row: 12, cell_col: 28,
            cell_chars_width: 26, cell_chars_height: 9,
            cell_name: "section2".to_string(), cell_width: 26, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 5,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 5,
            margin_right: 2, margin_bottom: 0,
            content: "mode_list".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "spanlabel".to_string(),
            row: 5, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 9,
            cell_name: "label_section".to_string(), cell_width: 54, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 1,
            margin_right: 2, margin_bottom: 3,
            content: "spans".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 6, col: 2,
            cell_row: 12, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 9,
            cell_name: "control_section".to_string(), cell_width: 27, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 2, margin_bottom: 2,
            content: "enabled_check".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 7, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 9,
            cell_name: "label_section".to_string(), cell_width: 54, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "animation".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fbbf24")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 100, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 8, col: 2,
            cell_row: 22, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 10,
            cell_name: "".to_string(), cell_width: 54, cell_height: 10,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 23, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 29, margin_bottom: 1,
            content: "apply_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 8, col: 27,
            cell_row: 22, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 10,
            cell_name: "".to_string(), cell_width: 54, cell_height: 10,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "reset_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
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
        background: Some(uimd::Color::new("#18324f")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("close_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        base.set_generated_named_style("title", uimd::Style {
        background: Some(uimd::Color::new("#18324f")),
        ..Default::default()
    });
        base.set_generated_named_style("selectable_label", uimd::Style {
        color: Some(uimd::Color::new("#a7f3d0")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("spans", uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("animation", uimd::Style {
        color: Some(uimd::Color::new("#fbbf24")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 100, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("summary", uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#dbeafe")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "WidgetGallery", "# Widget Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Widget Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the widget gallery.\"\n\nplain_label:\n  type: label\n  text: \"Plain label with inherited theme colors.\"\n  description: \"Plain label using inherited theme colors.\"\n\nselectable_label:\n  type: label\n  text: \"Select this text with the mouse to test label selection.\"\n  description: \"Selectable label used to test mouse text selection.\"\n\nspans:\n  type: spanlabel\n  text: \"span label\"\n  description: \"Span label demonstrating mixed foreground and background styling.\"\n\nanimation:\n  type: label\n  text: \"Animated label shows gradient-based rendering.\"\n  description: \"Animated gradient label.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name input.\"\n\nname_input:\n  type: textinput\n  value: \"Ada Lovelace\"\n  maxlength: 80\n  description: \"Editable name text input.\"\n\ncount_label:\n  type: label\n  text: Count\n  description: \"Label for the count input.\"\n\ncount_input:\n  type: numberinput\n  value: 3\n  step_size: 1\n  min_value: 0\n  max_value: 99\n  format_str: 0\n  description: \"Numeric count input with min, max, and step constraints.\"\n\nenabled_check:\n  type: checkbox\n  title: \"Feature enabled\"\n  value: 1\n  description: \"Toggle for the feature enabled state.\"\n\ntheme_label:\n  type: label\n  text: Theme\n  description: \"Label for the theme selector.\"\n\ntheme_combo:\n  type: combobox\n  options: [Dark, Light, Solarized]\n  selected_item: Dark\n  description: \"Combobox selecting a visual theme value.\"\n\nmode_label:\n  type: label\n  text: Mode\n  description: \"Label for the mode list.\"\n\nmode_list:\n  type: listbox\n  options: [Preview, Edit, Review, Publish, Archive]\n  selected_items: [Preview]\n  multiple: \"false\"\n  description: \"Single-select listbox choosing the current workflow mode.\"\n\napply_btn:\n  type: button\n  title: Apply\n  description: \"Apply the current widget values.\"\n\nreset_btn:\n  type: button\n  title: Reset\n  description: \"Reset widget values to defaults.\"\n\nsummary:\n  type: label\n  text: \"values:\\n  name: Ada Lovelace\\n  count: 3\\n  theme: Dark\\n  mode: Preview\\n  enabled: true\"\n  description: \"Read-only summary of current widget values.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#18324f\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@title:\n  background: \"#18324f\"\n@selectable_label:\n  user-select: text\n  color: \"#a7f3d0\"\n@spans:\n  user-select: text\n  color: \"#cbd5e1\"\n@animation:\n  color: \"#fbbf24\"\n  text-color-gradient:\n    interval: 100\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@summary:\n  background: \"#0d1524\"\n  color: \"#dbeafe\"\n```\n\n## User Interface\n\n```ui\n+-title------------------------------------------------+\n| header................................ close_btn.....|\n+-label_section----------------------------------------+\n|                                                      |\n|  plain_label.......................................  |\n|                                                      |\n|  selectable_label..................................  |\n|                                                      |\n|  spans.............................................  |\n|                                                      |\n|  animation.........................................  |\n|                                                      |\n+-control_section-----------+--section2----------------+\n|  name_label               |  theme_label             |\n|  name_input.............  |   theme_combo..........  |\n|                           |                          |\n|  count_label              |  mode_label              |\n|  count_input............  |  mode_list.............  |\n|                           |  ......................  |\n|  enabled_check..........  |  ......................  |\n|                           |  ......................  |\n|                           |  ......................  |\n+---------------------------+--------------------------+\n|                                                      |\n|  summary...........................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|  ..................................................  |\n|                                                      |\n|  apply_btn..............  reset_btn................  |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Widget gallery demonstrating labels, selectable text, span labels, animated text, text input, number input, checkbox, combobox, listbox, and action buttons.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Window title.", false), uimd::GeneratedElementMetadata::new("close_btn", "Close the widget gallery.", true), uimd::GeneratedElementMetadata::new("plain_label", "Plain label using inherited theme colors.", true), uimd::GeneratedElementMetadata::new("selectable_label", "Selectable label used to test mouse text selection.", true), uimd::GeneratedElementMetadata::new("spans", "Span label demonstrating mixed foreground and background styling.", true), uimd::GeneratedElementMetadata::new("animation", "Animated gradient label.", true), uimd::GeneratedElementMetadata::new("name_label", "Label for the name input.", true), uimd::GeneratedElementMetadata::new("name_input", "Editable name text input.", true), uimd::GeneratedElementMetadata::new("count_label", "Label for the count input.", true), uimd::GeneratedElementMetadata::new("count_input", "Numeric count input with min, max, and step constraints.", true), uimd::GeneratedElementMetadata::new("enabled_check", "Toggle for the feature enabled state.", true), uimd::GeneratedElementMetadata::new("theme_label", "Label for the theme selector.", true), uimd::GeneratedElementMetadata::new("theme_combo", "Combobox selecting a visual theme value.", true), uimd::GeneratedElementMetadata::new("mode_label", "Label for the mode list.", true), uimd::GeneratedElementMetadata::new("mode_list", "Single-select listbox choosing the current workflow mode.", true), uimd::GeneratedElementMetadata::new("apply_btn", "Apply the current widget values.", true), uimd::GeneratedElementMetadata::new("reset_btn", "Reset widget values to defaults.", true), uimd::GeneratedElementMetadata::new("summary", "Read-only summary of current widget values.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header = uimd::new_label("header", "Widget Gallery");
        base.add_element(header.clone());
        header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#18324f")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        header.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let close_btn = uimd::new_button("close_btn", "Quit");
        base.add_element(close_btn.clone());
        close_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let plain_label = uimd::new_label("plain_label", "Plain label with inherited theme colors.");
        base.add_element(plain_label.clone());
        plain_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        plain_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let selectable_label = uimd::new_label("selectable_label", "Select this text with the mouse to test label selection.");
        base.add_element(selectable_label.clone());
        selectable_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#a7f3d0")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        selectable_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let spans = uimd::new_span_label("spans", "span label");
        base.add_element(spans.clone());
        spans.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        let animation = uimd::new_label("animation", "Animated label shows gradient-based rendering.");
        base.add_element(animation.clone());
        animation.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fbbf24")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 100, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        animation.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name_label = uimd::new_label("name_label", "Name");
        base.add_element(name_label.clone());
        name_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        name_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name_input = uimd::new_text_input("name_input", "Ada Lovelace", 80);
        base.add_element(name_input.clone());
        name_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        name_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        name_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let count_label = uimd::new_label("count_label", "Count");
        base.add_element(count_label.clone());
        count_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        count_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let count_input = uimd::new_number_input_with_step("count_input", 3.000000, 1.000000);
        base.add_element(count_input.clone());
        count_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        count_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        count_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        count_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        count_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let enabled_check = uimd::new_checkbox("enabled_check", "Feature enabled", true);
        base.add_element(enabled_check.clone());
        enabled_check.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        enabled_check.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        enabled_check.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        enabled_check.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f97316")),
        ..Default::default()
    });
        enabled_check.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let theme_label = uimd::new_label("theme_label", "Theme");
        base.add_element(theme_label.clone());
        theme_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        theme_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let theme_combo = uimd::new_combo_box("theme_combo", vec!["Dark".to_string(), "Light".to_string(), "Solarized".to_string()]);
        base.add_element(theme_combo.clone());
        theme_combo.borrow_mut().set_selected_index(0);
        theme_combo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        theme_combo.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        theme_combo.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        theme_combo.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        theme_combo.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let mode_label = uimd::new_label("mode_label", "Mode");
        base.add_element(mode_label.clone());
        mode_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        mode_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let mode_list = uimd::new_list_box("mode_list", vec!["Preview".to_string(), "Edit".to_string(), "Review".to_string(), "Publish".to_string(), "Archive".to_string()], false);
        base.add_element(mode_list.clone());
        mode_list.borrow_mut().set_selected_index(0);
        mode_list.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        mode_list.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1d2f4d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mode_list.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        mode_list.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mode_list.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        mode_list.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let apply_btn = uimd::new_button("apply_btn", "Apply");
        base.add_element(apply_btn.clone());
        apply_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        apply_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        apply_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let reset_btn = uimd::new_button("reset_btn", "Reset");
        base.add_element(reset_btn.clone());
        reset_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        reset_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        reset_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let summary = uimd::new_label("summary", "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: true");
        base.add_element(summary.clone());
        summary.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#dbeafe")),
        ..Default::default()
    });
        summary.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            header,
            close_btn,
            plain_label,
            selectable_label,
            spans,
            animation,
            name_label,
            name_input,
            count_label,
            count_input,
            enabled_check,
            theme_label,
            theme_combo,
            mode_label,
            mode_list,
            apply_btn,
            reset_btn,
            summary,
        }
    }

    pub fn run<H: WidgetGalleryUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = WidgetGalleryUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for WidgetGalleryUI
{
    fn default() -> Self { Self::new() }
}

pub trait WidgetGalleryUIEvents
{
    fn on_close_btn_click(&mut self, _ui: &mut WidgetGalleryUI) {}
    fn on_apply_btn_click(&mut self, _ui: &mut WidgetGalleryUI) {}
    fn on_reset_btn_click(&mut self, _ui: &mut WidgetGalleryUI) {}
    fn on_name_input_change(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_count_input_change(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_enabled_check_change(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_name_input_submit(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_count_input_submit(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_theme_combo_change(&mut self, _ui: &mut WidgetGalleryUI, _value: &str) {}
    fn on_mode_list_selectionchange(&mut self, _ui: &mut WidgetGalleryUI, _value: &[String]) {}
    fn on_mode_list_itemactivate(&mut self, _ui: &mut WidgetGalleryUI, _index: usize, _value: &str) -> bool { false }
    fn handle_dynamic_button(&mut self, _ui: &mut WidgetGalleryUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut WidgetGalleryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut WidgetGalleryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut WidgetGalleryUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut WidgetGalleryUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut WidgetGalleryUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut WidgetGalleryUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut WidgetGalleryUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut WidgetGalleryUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut WidgetGalleryUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut WidgetGalleryUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct WidgetGalleryUIRuntime<'a, H: WidgetGalleryUIEvents>
{
    ui: &'a mut WidgetGalleryUI,
    handler: &'a mut H,
}

impl<H: WidgetGalleryUIEvents> uimd::GeneratedApplication for WidgetGalleryUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "close_btn" { self.handler.on_close_btn_click(self.ui); return true; }
        if name == "apply_btn" { self.handler.on_apply_btn_click(self.ui); return true; }
        if name == "reset_btn" { self.handler.on_reset_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name_input" { self.handler.on_name_input_change(self.ui, value); return true; }
        if name == "count_input" { self.handler.on_count_input_change(self.ui, value); return true; }
        if name == "enabled_check" { self.handler.on_enabled_check_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name_input" { self.handler.on_name_input_submit(self.ui, value); return true; }
        if name == "count_input" { self.handler.on_count_input_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "theme_combo" { self.handler.on_theme_combo_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "mode_list" { self.handler.on_mode_list_selectionchange(self.ui, value); return true; }
        self.handler.handle_dynamic_selection_changed(self.ui, name, value)
    }

    fn handle_generated_listbox_item_activate(&mut self, name: &str, _element_id: &str, index: usize, value: &str) -> bool
    {
        if name == "mode_list" { return self.handler.on_mode_list_itemactivate(self.ui, index, value); }
        false
    }

    fn handle_focus_changed(&mut self, name: &str, focused: bool) -> bool
    {
        self.handler.on_focus_changed(self.ui, name, focused);
        true
    }

    fn handle_preview_key(&mut self, event: &uimd::KeyEvent) -> bool
    {
        self.handler.on_preview_key(self.ui, event)
    }

    #[allow(deprecated)]
    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool
    {
        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)
    }

    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

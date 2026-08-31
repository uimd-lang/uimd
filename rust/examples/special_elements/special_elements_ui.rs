// Auto-generated UI code for special_elements - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct SpecialElementsUI
{
    pub base: uimd::GeneratedWindow,
    pub header_label: uimd::LabelRef,
    pub info_processing: uimd::LabelRef,
    pub info_loading: uimd::LabelRef,
    pub info_complete: uimd::LabelRef,
    pub info_warning: uimd::LabelRef,
    pub info_multiline: uimd::LabelRef,
    pub status_label: uimd::LabelRef,
    pub normal_info: uimd::LabelRef,
    pub speed_label: uimd::LabelRef,
    pub raw_label: uimd::LabelRef,
    pub raw_button: uimd::ButtonRef,
    pub raw_combo: uimd::ComboBoxRef,
    pub raw_list: uimd::ListBoxRef,
    pub raw_input: uimd::TextInputRef,
    pub raw_area: uimd::TextAreaRef,
}

impl SpecialElementsUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Special Elements Demo");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 1,
            margin_right: 2, margin_bottom: 32,
            content: "header_label".to_string(),
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
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 31, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 35, margin_bottom: 30,
            content: "info_processing".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 35,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 30,
            content: "info_loading".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 4, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 31, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 35, margin_bottom: 29,
            content: "info_complete".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 4, col: 35,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 29,
            content: "info_warning".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 6, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 40, margin_bottom: 27,
            content: "\"Multiline animated text:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 7, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 3,
            margin_right: 2, margin_bottom: 24,
            content: "info_multiline".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 11, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 31, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 35, margin_bottom: 22,
            content: "status_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 11, col: 35,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 2,
            margin_right: 2, margin_bottom: 21,
            content: "normal_info".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 12, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 1,
            margin_right: 2, margin_bottom: 21,
            content: "speed_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 35, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 14, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 45, margin_bottom: 19,
            content: "\"Raw read-only text:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 15, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 1,
            margin_right: 2, margin_bottom: 18,
            content: "raw_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 17, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 53, margin_bottom: 16,
            content: "\"Raw button:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 17, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 26, margin_bottom: 16,
            content: "raw_button".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 19, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 54, margin_bottom: 14,
            content: "\"Raw combo:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 19, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 21, margin_bottom: 14,
            content: "raw_combo".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 21, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 55, margin_bottom: 12,
            content: "\"Raw list:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "listbox".to_string(),
            row: 22, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 3,
            margin_right: 2, margin_bottom: 9,
            content: "raw_list".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 26, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 53, margin_bottom: 7,
            content: "\"Text input:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 26, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 50, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "raw_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 28, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 54, margin_bottom: 5,
            content: "\"Text area:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 29, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 68, cell_chars_height: 34,
            cell_name: "".to_string(), cell_width: 68, cell_height: 34,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 4,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 4,
            margin_right: 2, margin_bottom: 1,
            content: "raw_area".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style::default(),
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
        base.set_generated_named_style("info_processing", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("info_loading", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("info_complete", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("info_warning", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("info_multiline", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_generated_named_style("speed_label", uimd::Style {
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 35, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "SpecialElements", "# Special Elements Demo\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_label:\n  type: label\n  text: \"Special UI Elements\"\n  description: \"Window title.\"\n\ninfo_processing:\n  type: label\n  text: \"Processing request...\"\n  color: \"#ff0000\"\n  description: \"Animated processing status label.\"\n\ninfo_loading:\n  type: label\n  text: \"Loading data from server...\"\n  color: \"#00ff00\"\n  description: \"Animated loading status label.\"\n\ninfo_complete:\n  type: label\n  text: \"Operation completed successfully!\"\n  color: \"#0000ff\"\n  description: \"Animated completed status label.\"\n\ninfo_warning:\n  type: label\n  text: \"Warning: Low disk space detected\"\n  color: \"#ffffff\"\n  description: \"Animated warning status label.\"\n\ninfo_multiline:\n  type: label\n  text: \"Line one of animated text\\nLine two with different phase\\nLine three final row\"\n  description: \"Multiline animated label with different text rows.\"\n\nstatus_label:\n  type: label\n  text: \"Status: Idle\"\n  description: \"Current status text.\"\n\nnormal_info:\n  type: label\n  text: \"Regular static label for comparison\"\n  description: \"Regular static comparison label.\"\n\nspeed_label:\n  type: label\n  text: \"Fast animation speed demo\"\n  description: \"Fast animation speed demonstration label.\"\n\nraw_label:\n  type: label\n  text: \"Label tab\\tcontrol\\x1b bell\\x07 combining e\\u0301 wide\\u4e2d\"\n  user-select: text\n  description: \"Label containing tab, control, combining, and wide characters.\"\n\nraw_button:\n  type: button\n  title: \"Button\\tESC\\x1bWide\\u4e2d\"\n  description: \"Button title containing raw terminal-sensitive characters.\"\n\nraw_combo:\n  type: combobox\n  options: [\"Plain option\", \"Tab\\toption\", \"Esc\\x1boption\", \"Wide\\u4e2doption\"]\n  selected_item: \"Tab\\toption\"\n  description: \"ComboBox options containing raw terminal-sensitive characters.\"\n\nraw_list:\n  type: listbox\n  options: [\"Plain row\", \"Tab\\trow\", \"Esc\\x1brow\", \"Wide\\u4e2drow\"]\n  selected_items: [\"Tab\\trow\"]\n  description: \"ListBox options containing raw terminal-sensitive characters.\"\n\nraw_input:\n  type: textinput\n  value: \"Input\\tvalue\\x1bwide\\u4e2d\"\n  maxlength: 80\n  description: \"Text input containing raw terminal-sensitive characters.\"\n\nraw_area:\n  type: textarea\n  value: \"Python tabs example:\\ndef hello(name):\\n\\tif name:\\n\\t\\tprint(\\\"Hello, \\\" + name)\\n\\telse:\\n\\t\\tprint(\\\"Hello, world!\\\")\\n\\nControl\\x1bplaceholder\\nWide\\u4e2dplaceholder\"\n  maxlength: 240\n  description: \"Text area containing multiline raw terminal-sensitive characters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\n@info_processing:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_loading:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_complete:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_warning:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@info_multiline:\n  text-color-gradient:\n    interval: 70\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\n@speed_label:\n  text-color-gradient:\n    interval: 35\n    step: 1\n    segment-size: 1\n    colors: [\"#00000099\", \"#00000066\", \"#00000033\", \"#00000011\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000000\", \"#00000011\", \"#00000033\", \"#00000066\", \"#00000099\"]\ntextinput:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nlistbox:\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------+\n|                                                                    |\n|  header_label....................................................  |\n|                                                                    |\n|  info_processing................  info_loading...................  |\n|  info_complete..................  info_warning...................  |\n|                                                                    |\n|  \"Multiline animated text:\"                                         |\n|  info_multiline..................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  status_label...................  normal_info....................  |\n|  speed_label.....................................................  |\n|                                                                    |\n|  \"Raw read-only text:\"                                             |\n|  raw_label.......................................................  |\n|                                                                    |\n|  \"Raw button:\" raw_button................                          |\n|                                                                    |\n|  \"Raw combo:\"  raw_combo......................                     |\n|                                                                    |\n|  \"Raw list:\"                                                       |\n|  raw_list........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n|  \"Text input:\" raw_input.........................................  |\n|                                                                    |\n|  \"Text area:\"                                                      |\n|  raw_area........................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|  ................................................................  |\n|                                                                    |\n+--------------------------------------------------------------------+\n```\n", "Demonstration window for static labels, multiline labels, animated text color gradients, and raw terminal-sensitive text.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header_label", "Window title.", true), uimd::GeneratedElementMetadata::new("info_processing", "Animated processing status label.", true), uimd::GeneratedElementMetadata::new("info_loading", "Animated loading status label.", true), uimd::GeneratedElementMetadata::new("info_complete", "Animated completed status label.", true), uimd::GeneratedElementMetadata::new("info_warning", "Animated warning status label.", true), uimd::GeneratedElementMetadata::new("info_multiline", "Multiline animated label with different text rows.", true), uimd::GeneratedElementMetadata::new("status_label", "Current status text.", true), uimd::GeneratedElementMetadata::new("normal_info", "Regular static comparison label.", true), uimd::GeneratedElementMetadata::new("speed_label", "Fast animation speed demonstration label.", true), uimd::GeneratedElementMetadata::new("raw_label", "Label containing tab, control, combining, and wide characters.", true), uimd::GeneratedElementMetadata::new("raw_button", "Button title containing raw terminal-sensitive characters.", true), uimd::GeneratedElementMetadata::new("raw_combo", "ComboBox options containing raw terminal-sensitive characters.", true), uimd::GeneratedElementMetadata::new("raw_list", "ListBox options containing raw terminal-sensitive characters.", true), uimd::GeneratedElementMetadata::new("raw_input", "Text input containing raw terminal-sensitive characters.", true), uimd::GeneratedElementMetadata::new("raw_area", "Text area containing multiline raw terminal-sensitive characters.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header_label = uimd::new_label("header_label", "Special UI Elements");
        base.add_element(header_label.clone());
        header_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        header_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let info_processing = uimd::new_label("info_processing", "Processing request...");
        base.add_element(info_processing.clone());
        info_processing.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ff0000")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        info_processing.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let info_loading = uimd::new_label("info_loading", "Loading data from server...");
        base.add_element(info_loading.clone());
        info_loading.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#00ff00")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        info_loading.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let info_complete = uimd::new_label("info_complete", "Operation completed successfully!");
        base.add_element(info_complete.clone());
        info_complete.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#0000ff")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        info_complete.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let info_warning = uimd::new_label("info_warning", "Warning: Low disk space detected");
        base.add_element(info_warning.clone());
        info_warning.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        info_warning.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let info_multiline = uimd::new_label("info_multiline", "Line one of animated text\nLine two with different phase\nLine three final row");
        base.add_element(info_multiline.clone());
        info_multiline.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 70, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        info_multiline.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let status_label = uimd::new_label("status_label", "Status: Idle");
        base.add_element(status_label.clone());
        status_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        status_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let normal_info = uimd::new_label("normal_info", "Regular static label for comparison");
        base.add_element(normal_info.clone());
        normal_info.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        normal_info.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let speed_label = uimd::new_label("speed_label", "Fast animation speed demo");
        base.add_element(speed_label.clone());
        speed_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_color_gradient: Some(uimd::TextGradient { interval_ms: 35, step: 1, segment_size: 1, colors: vec![uimd::Color::new("#00000099"), uimd::Color::new("#00000066"), uimd::Color::new("#00000033"), uimd::Color::new("#00000011"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000000"), uimd::Color::new("#00000011"), uimd::Color::new("#00000033"), uimd::Color::new("#00000066"), uimd::Color::new("#00000099")] }),
        ..Default::default()
    });
        speed_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let raw_label = uimd::new_label("raw_label", "Label tab\tcontrol\x1b bell\x07 combining é wide中");
        base.add_element(raw_label.clone());
        raw_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        raw_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let raw_button = uimd::new_button("raw_button", "Button\tESC\x1bWide中");
        base.add_element(raw_button.clone());
        raw_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        raw_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let raw_combo = uimd::new_combo_box("raw_combo", vec!["Plain option".to_string(), "Tab\toption".to_string(), "Esc\x1boption".to_string(), "Wide中option".to_string()]);
        base.add_element(raw_combo.clone());
        raw_combo.borrow_mut().set_selected_index(1);
        raw_combo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        raw_combo.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_combo.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        raw_combo.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_combo.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let raw_list = uimd::new_list_box("raw_list", vec!["Plain row".to_string(), "Tab\trow".to_string(), "Esc\x1brow".to_string(), "Wide中row".to_string()], false);
        base.add_element(raw_list.clone());
        raw_list.borrow_mut().set_selected_index(1);
        raw_list.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        raw_list.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1d2f4d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_list.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        raw_list.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_list.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        raw_list.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let raw_input = uimd::new_text_input("raw_input", "Input\tvalue\x1bwide中", 80);
        base.add_element(raw_input.clone());
        raw_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        raw_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        raw_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let raw_area = uimd::new_text_area_with_max_length("raw_area", "Python tabs example:\ndef hello(name):\n\tif name:\n\t\tprint(\"Hello, \" + name)\n\telse:\n\t\tprint(\"Hello, world!\")\n\nControl\x1bplaceholder\nWide中placeholder", 240);
        base.add_element(raw_area.clone());
        raw_area.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        raw_area.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_area.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_area.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        raw_area.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        raw_area.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            header_label,
            info_processing,
            info_loading,
            info_complete,
            info_warning,
            info_multiline,
            status_label,
            normal_info,
            speed_label,
            raw_label,
            raw_button,
            raw_combo,
            raw_list,
            raw_input,
            raw_area,
        }
    }

    pub fn run<H: SpecialElementsUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = SpecialElementsUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for SpecialElementsUI
{
    fn default() -> Self { Self::new() }
}

pub trait SpecialElementsUIEvents
{
    fn on_raw_button_click(&mut self, _ui: &mut SpecialElementsUI) {}
    fn on_raw_input_change(&mut self, _ui: &mut SpecialElementsUI, _value: &str) {}
    fn on_raw_area_change(&mut self, _ui: &mut SpecialElementsUI, _value: &str) {}
    fn on_raw_input_submit(&mut self, _ui: &mut SpecialElementsUI, _value: &str) {}
    fn on_raw_area_submit(&mut self, _ui: &mut SpecialElementsUI, _value: &str) {}
    fn on_raw_combo_change(&mut self, _ui: &mut SpecialElementsUI, _value: &str) {}
    fn on_raw_list_selectionchange(&mut self, _ui: &mut SpecialElementsUI, _value: &[String]) {}
    fn on_raw_list_itemactivate(&mut self, _ui: &mut SpecialElementsUI, _index: usize, _value: &str) -> bool { false }
    fn handle_dynamic_button(&mut self, _ui: &mut SpecialElementsUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut SpecialElementsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut SpecialElementsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut SpecialElementsUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut SpecialElementsUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut SpecialElementsUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut SpecialElementsUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut SpecialElementsUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut SpecialElementsUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut SpecialElementsUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut SpecialElementsUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct SpecialElementsUIRuntime<'a, H: SpecialElementsUIEvents>
{
    ui: &'a mut SpecialElementsUI,
    handler: &'a mut H,
}

impl<H: SpecialElementsUIEvents> uimd::GeneratedApplication for SpecialElementsUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "raw_button" { self.handler.on_raw_button_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "raw_input" { self.handler.on_raw_input_change(self.ui, value); return true; }
        if name == "raw_area" { self.handler.on_raw_area_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "raw_input" { self.handler.on_raw_input_submit(self.ui, value); return true; }
        if name == "raw_area" { self.handler.on_raw_area_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "raw_combo" { self.handler.on_raw_combo_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "raw_list" { self.handler.on_raw_list_selectionchange(self.ui, value); return true; }
        self.handler.handle_dynamic_selection_changed(self.ui, name, value)
    }

    fn handle_generated_listbox_item_activate(&mut self, name: &str, _element_id: &str, index: usize, value: &str) -> bool
    {
        if name == "raw_list" { return self.handler.on_raw_list_itemactivate(self.ui, index, value); }
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

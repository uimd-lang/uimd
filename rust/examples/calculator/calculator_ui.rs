// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct CalculatorUI
{
    pub base: uimd::GeneratedWindow,
    pub display: uimd::LabelRef,
    pub n0: uimd::ButtonRef,
    pub n1: uimd::ButtonRef,
    pub n2: uimd::ButtonRef,
    pub n3: uimd::ButtonRef,
    pub n4: uimd::ButtonRef,
    pub n5: uimd::ButtonRef,
    pub n6: uimd::ButtonRef,
    pub n7: uimd::ButtonRef,
    pub n8: uimd::ButtonRef,
    pub n9: uimd::ButtonRef,
    pub sb: uimd::ButtonRef,
    pub dt: uimd::ButtonRef,
    pub ac: uimd::ButtonRef,
    pub dl: uimd::ButtonRef,
    pub pr: uimd::ButtonRef,
    pub dv: uimd::ButtonRef,
    pub ml: uimd::ButtonRef,
    pub mn: uimd::ButtonRef,
    pub ad: uimd::ButtonRef,
}

impl CalculatorUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Calculator");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 18, chars_height: 1,
            margin_right: 2, margin_bottom: 11,
            content: "display".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#32446d")),
        color: Some(uimd::Color::new("#c8e0ff")),
        text_align: Some("right".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 17, margin_bottom: 9,
            content: "ac".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 7,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 12, margin_bottom: 9,
            content: "dl".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 12,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 7, margin_bottom: 9,
            content: "pr".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 2, margin_bottom: 9,
            content: "dv".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 17, margin_bottom: 7,
            content: "n7".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 7,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 12, margin_bottom: 7,
            content: "n8".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 12,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 7, margin_bottom: 7,
            content: "n9".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "ml".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 17, margin_bottom: 5,
            content: "n4".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 7,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 12, margin_bottom: 5,
            content: "n5".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 12,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 7, margin_bottom: 5,
            content: "n6".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 2, margin_bottom: 5,
            content: "mn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 9, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 17, margin_bottom: 3,
            content: "n1".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 9, col: 7,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 12, margin_bottom: 3,
            content: "n2".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 9, col: 12,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 7, margin_bottom: 3,
            content: "n3".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 9, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 2, margin_bottom: 3,
            content: "sb".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 11, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 8, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 8, chars_height: 1,
            margin_right: 12, margin_bottom: 1,
            content: "n0".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 11, col: 12,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 7, margin_bottom: 1,
            content: "dt".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 11, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 22, cell_chars_height: 13,
            cell_name: "".to_string(), cell_width: 22, cell_height: 13,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "ad".to_string(),
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
        base.set_generated_named_style("display", uimd::Style {
        background: Some(uimd::Color::new("#32446d")),
        color: Some(uimd::Color::new("#c8e0ff")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Calculator", "# Calculator\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ndisplay:\n  type: label\n  text: \"\"\n  description: \"Shows the current number, operator result, or calculation state.\"\n\nn0:\n  type: button\n  title: 0\n  description: \"Enter digit 0.\"\n\nn1:\n  type: button\n  title: 1\n  description: \"Enter digit 1.\"\n\nn2:\n  type: button\n  title: 2\n  description: \"Enter digit 2.\"\n\nn3:\n  type: button\n  title: 3\n  description: \"Enter digit 3.\"\n\nn4:\n  type: button\n  title: 4\n  description: \"Enter digit 4.\"\n\nn5:\n  type: button\n  title: 5\n  description: \"Enter digit 5.\"\n\nn6:\n  type: button\n  title: 6\n  description: \"Enter digit 6.\"\n\nn7:\n  type: button\n  title: 7\n  description: \"Enter digit 7.\"\n\nn8:\n  type: button\n  title: 8\n  description: \"Enter digit 8.\"\n\nn9:\n  type: button\n  title: 9\n  description: \"Enter digit 9.\"\n\nsb:\n  type: button\n  title: \"=\"\n  description: \"Evaluate the pending calculation.\"\n\ndt:\n  type: button\n  title: .\n  description: \"Insert a decimal point into the current number.\"\n\nac:\n  type: button\n  title: AC\n  description: \"Clear the current calculator state.\"\n\ndl:\n  type: button\n  title: DEL\n  description: \"Delete the last entered character.\"\n\npr:\n  type: button\n  title: \"%\"\n  description: \"Convert or apply percentage behavior for the current value.\"\n\ndv:\n  type: button\n  title: /\n  description: \"Choose division as the pending operator.\"\n\nml:\n  type: button\n  title: \"*\"\n  description: \"Choose multiplication as the pending operator.\"\n\nmn:\n  type: button\n  title: -\n  description: \"Choose subtraction as the pending operator.\"\n\nad:\n  type: button\n  title: \"+\"\n  description: \"Choose addition as the pending operator.\"\n```\n\n## Style\n\n```yaml\n@display:\n  background: #32446d\n  color: #c8e0ff\n  text-align: right\n```\n\n## User Interface\n\n```ui\n+----------------------+\n|                      |\n|  display...........  |\n|                      |\n|  ac.  dl.  pr.  dv.  |\n|                      |\n|  n7.  n8.  n9.  ml.  |\n|                      |\n|  n4.  n5.  n6.  mn.  |\n|                      |\n|  n1.  n2.  n3.  sb.  |\n|                      |\n|  n0......  dt.  ad.  |\n|                      |\n+----------------------+\n```\n", "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("display", "Shows the current number, operator result, or calculation state.", true), uimd::GeneratedElementMetadata::new("n0", "Enter digit 0.", true), uimd::GeneratedElementMetadata::new("n1", "Enter digit 1.", true), uimd::GeneratedElementMetadata::new("n2", "Enter digit 2.", true), uimd::GeneratedElementMetadata::new("n3", "Enter digit 3.", true), uimd::GeneratedElementMetadata::new("n4", "Enter digit 4.", true), uimd::GeneratedElementMetadata::new("n5", "Enter digit 5.", true), uimd::GeneratedElementMetadata::new("n6", "Enter digit 6.", true), uimd::GeneratedElementMetadata::new("n7", "Enter digit 7.", true), uimd::GeneratedElementMetadata::new("n8", "Enter digit 8.", true), uimd::GeneratedElementMetadata::new("n9", "Enter digit 9.", true), uimd::GeneratedElementMetadata::new("sb", "Evaluate the pending calculation.", true), uimd::GeneratedElementMetadata::new("dt", "Insert a decimal point into the current number.", true), uimd::GeneratedElementMetadata::new("ac", "Clear the current calculator state.", true), uimd::GeneratedElementMetadata::new("dl", "Delete the last entered character.", true), uimd::GeneratedElementMetadata::new("pr", "Convert or apply percentage behavior for the current value.", true), uimd::GeneratedElementMetadata::new("dv", "Choose division as the pending operator.", true), uimd::GeneratedElementMetadata::new("ml", "Choose multiplication as the pending operator.", true), uimd::GeneratedElementMetadata::new("mn", "Choose subtraction as the pending operator.", true), uimd::GeneratedElementMetadata::new("ad", "Choose addition as the pending operator.", true)]);
        base.set_mcp_app_tools(vec![]);
        let display = uimd::new_label("display", "");
        base.add_element(display.clone());
        display.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#32446d")),
        color: Some(uimd::Color::new("#c8e0ff")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        display.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let n0 = uimd::new_button("n0", "0");
        base.add_element(n0.clone());
        n0.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n0.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n0.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n1 = uimd::new_button("n1", "1");
        base.add_element(n1.clone());
        n1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n2 = uimd::new_button("n2", "2");
        base.add_element(n2.clone());
        n2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n3 = uimd::new_button("n3", "3");
        base.add_element(n3.clone());
        n3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n3.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n4 = uimd::new_button("n4", "4");
        base.add_element(n4.clone());
        n4.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n4.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n4.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n5 = uimd::new_button("n5", "5");
        base.add_element(n5.clone());
        n5.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n5.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n5.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n6 = uimd::new_button("n6", "6");
        base.add_element(n6.clone());
        n6.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n6.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n6.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n7 = uimd::new_button("n7", "7");
        base.add_element(n7.clone());
        n7.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n7.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n7.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n8 = uimd::new_button("n8", "8");
        base.add_element(n8.clone());
        n8.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n8.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n8.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let n9 = uimd::new_button("n9", "9");
        base.add_element(n9.clone());
        n9.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        n9.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        n9.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let sb = uimd::new_button("sb", "=");
        base.add_element(sb.clone());
        sb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        sb.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        sb.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let dt = uimd::new_button("dt", ".");
        base.add_element(dt.clone());
        dt.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        dt.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        dt.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let ac = uimd::new_button("ac", "AC");
        base.add_element(ac.clone());
        ac.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        ac.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        ac.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let dl = uimd::new_button("dl", "DEL");
        base.add_element(dl.clone());
        dl.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        dl.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        dl.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let pr = uimd::new_button("pr", "%");
        base.add_element(pr.clone());
        pr.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        pr.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        pr.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let dv = uimd::new_button("dv", "/");
        base.add_element(dv.clone());
        dv.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        dv.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        dv.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let ml = uimd::new_button("ml", "*");
        base.add_element(ml.clone());
        ml.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        ml.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        ml.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let mn = uimd::new_button("mn", "-");
        base.add_element(mn.clone());
        mn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        mn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let ad = uimd::new_button("ad", "+");
        base.add_element(ad.clone());
        ad.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        ad.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        ad.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            display,
            n0,
            n1,
            n2,
            n3,
            n4,
            n5,
            n6,
            n7,
            n8,
            n9,
            sb,
            dt,
            ac,
            dl,
            pr,
            dv,
            ml,
            mn,
            ad,
        }
    }

    pub fn run<H: CalculatorUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CalculatorUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CalculatorUI
{
    fn default() -> Self { Self::new() }
}

pub trait CalculatorUIEvents
{
    fn on_n0_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n1_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n2_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n3_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n4_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n5_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n6_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n7_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n8_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_n9_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_sb_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_dt_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_ac_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_dl_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_pr_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_dv_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_ml_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_mn_click(&mut self, _ui: &mut CalculatorUI) {}
    fn on_ad_click(&mut self, _ui: &mut CalculatorUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut CalculatorUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CalculatorUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CalculatorUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CalculatorUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CalculatorUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CalculatorUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut CalculatorUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut CalculatorUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CalculatorUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CalculatorUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CalculatorUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CalculatorUIRuntime<'a, H: CalculatorUIEvents>
{
    ui: &'a mut CalculatorUI,
    handler: &'a mut H,
}

impl<H: CalculatorUIEvents> uimd::GeneratedApplication for CalculatorUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "n0" { self.handler.on_n0_click(self.ui); return true; }
        if name == "n1" { self.handler.on_n1_click(self.ui); return true; }
        if name == "n2" { self.handler.on_n2_click(self.ui); return true; }
        if name == "n3" { self.handler.on_n3_click(self.ui); return true; }
        if name == "n4" { self.handler.on_n4_click(self.ui); return true; }
        if name == "n5" { self.handler.on_n5_click(self.ui); return true; }
        if name == "n6" { self.handler.on_n6_click(self.ui); return true; }
        if name == "n7" { self.handler.on_n7_click(self.ui); return true; }
        if name == "n8" { self.handler.on_n8_click(self.ui); return true; }
        if name == "n9" { self.handler.on_n9_click(self.ui); return true; }
        if name == "sb" { self.handler.on_sb_click(self.ui); return true; }
        if name == "dt" { self.handler.on_dt_click(self.ui); return true; }
        if name == "ac" { self.handler.on_ac_click(self.ui); return true; }
        if name == "dl" { self.handler.on_dl_click(self.ui); return true; }
        if name == "pr" { self.handler.on_pr_click(self.ui); return true; }
        if name == "dv" { self.handler.on_dv_click(self.ui); return true; }
        if name == "ml" { self.handler.on_ml_click(self.ui); return true; }
        if name == "mn" { self.handler.on_mn_click(self.ui); return true; }
        if name == "ad" { self.handler.on_ad_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        self.handler.handle_dynamic_selection_changed(self.ui, name, value)
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

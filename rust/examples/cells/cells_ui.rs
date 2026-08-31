// Auto-generated UI code for cells - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct CellsUI
{
    pub base: uimd::GeneratedWindow,
    pub a1: uimd::ButtonRef,
    pub a2: uimd::ButtonRef,
    pub b1: uimd::ButtonRef,
    pub c1: uimd::ButtonRef,
    pub c2: uimd::ButtonRef,
    pub c3: uimd::ButtonRef,
    pub c4: uimd::ButtonRef,
    pub c5: uimd::ButtonRef,
    pub c6: uimd::ButtonRef,
    pub c7: uimd::ButtonRef,
    pub c8: uimd::ButtonRef,
    pub c9: uimd::ButtonRef,
    pub c10: uimd::ButtonRef,
    pub d1: uimd::ButtonRef,
    pub d2: uimd::ButtonRef,
    pub d3: uimd::ButtonRef,
    pub d4: uimd::ButtonRef,
    pub d5: uimd::ButtonRef,
    pub d6: uimd::ButtonRef,
    pub e1: uimd::ButtonRef,
    pub e2: uimd::ButtonRef,
    pub e3: uimd::ButtonRef,
    pub e4: uimd::ButtonRef,
    pub e5: uimd::ButtonRef,
    pub e6: uimd::ButtonRef,
    pub e7: uimd::ButtonRef,
    pub e8: uimd::ButtonRef,
    pub f1: uimd::ButtonRef,
    pub f2: uimd::ButtonRef,
    pub f3: uimd::ButtonRef,
}

impl CellsUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Cells");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "".to_string(),
            row: 0, col: 0,
            cell_row: 12, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 2,
            cell_name: "cl7".to_string(), cell_width: 4, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 2,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 2,
            margin_right: 0, margin_bottom: 0,
            content: "".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#2f3745")),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 17, cell_chars_height: 1,
            cell_name: "cell1".to_string(), cell_width: 17, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 1,
            margin_right: 9, margin_bottom: 0,
            content: "a1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#3b1f2d")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7a3f59")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 22, margin_bottom: 1,
            content: "c1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 10,
            content: "d1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#d7fbe8")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 5, cell_col: 22,
            cell_chars_width: 5, cell_chars_height: 9,
            cell_name: "cl6".to_string(), cell_width: 5, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "f1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#174f55")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#3a8890")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 2,
            cell_row: 0, cell_col: 18,
            cell_chars_width: 9, cell_chars_height: 1,
            cell_name: "cell2".to_string(), cell_width: 9, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 5, chars_height: 1,
            margin_right: 2, margin_bottom: 0,
            content: "b1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f463f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#3d7b6d")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 6,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 17, margin_bottom: 1,
            content: "c2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 9,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 17, cell_chars_height: 1,
            cell_name: "cell1".to_string(), cell_width: 17, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "a2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#3b1f2d")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7a3f59")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 11,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 12, margin_bottom: 1,
            content: "c3".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 7, margin_bottom: 1,
            content: "c4".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 21,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "c5".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 22, margin_bottom: 0,
            content: "c6".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 1,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 11, margin_bottom: 7,
            content: "e1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 6,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 17, margin_bottom: 0,
            content: "c7".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 11,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 12, margin_bottom: 0,
            content: "c8".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 11,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 1, margin_bottom: 7,
            content: "e5".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 4, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 7, margin_bottom: 0,
            content: "c9".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 21,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 2,
            cell_name: "cell4".to_string(), cell_width: 27, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 4, chars_height: 1,
            margin_right: 2, margin_bottom: 0,
            content: "c10".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#4a4422")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "d2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 1,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 11, margin_bottom: 5,
            content: "e2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 1,
            cell_row: 5, cell_col: 22,
            cell_chars_width: 5, cell_chars_height: 9,
            cell_name: "cl6".to_string(), cell_width: 5, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 1, margin_bottom: 5,
            content: "f2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#174f55")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#31565f")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 11,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 1, margin_bottom: 5,
            content: "e6".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 6,
            content: "d3".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 1,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 11, margin_bottom: 3,
            content: "e3".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 5, col: 11,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 1, margin_bottom: 3,
            content: "e7".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 6, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 4,
            content: "d4".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 6, col: 1,
            cell_row: 5, cell_col: 22,
            cell_chars_width: 5, cell_chars_height: 9,
            cell_name: "cl6".to_string(), cell_width: 5, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "f3".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#174f55")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#3a8890")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 1,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 11, margin_bottom: 1,
            content: "e4".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 7, col: 11,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 21, cell_chars_height: 9,
            cell_name: "cell5".to_string(), cell_width: 21, cell_height: 9,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "e8".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#482456")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 8, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "d5".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 10, col: 1,
            cell_row: 0, cell_col: 28,
            cell_chars_width: 4, cell_chars_height: 11,
            cell_name: "cl3".to_string(), cell_width: 4, cell_height: 11,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 2, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "d6".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#26345f")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2946")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(1),
        border_width_vertical: Some(2),
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
        base.set_generated_named_style("d1", uimd::Style {
        color: Some(uimd::Color::new("#d7fbe8")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Cells", "# Cells\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\na1:\n  type: button\n  title: A1\n  description: \"Button in the first red cell.\"\n\na2:\n  type: button\n  title: A2\n  description: \"Second button in the first red cell.\"\n\nb1:\n  type: button\n  title: BBB\n  description: \"Wide button in the green cell.\"\n\nc1:\n  type: button\n  title: C1\n  description: \"Button in the yellow cell group.\"\n\nc2:\n  type: button\n  title: C2\n  description: \"Button in the yellow cell group.\"\n\nc3:\n  type: button\n  title: C3\n  description: \"Button in the yellow cell group.\"\n\nc4:\n  type: button\n  title: C4\n  description: \"Button in the yellow cell group.\"\n\nc5:\n  type: button\n  title: C5\n  description: \"Button in the yellow cell group.\"\n\nc6:\n  type: button\n  title: C6\n  description: \"Button in the yellow cell group.\"\n\nc7:\n  type: button\n  title: C7\n  description: \"Button in the yellow cell group.\"\n\nc8:\n  type: button\n  title: C8\n  description: \"Button in the yellow cell group.\"\n\nc9:\n  type: button\n  title: C9\n  description: \"Button in the yellow cell group.\"\n\nc10:\n  type: button\n  title: C10\n  description: \"Button in the yellow cell group.\"\n\nd1:\n  type: button\n  title: D1\n  description: \"Top button in the right column.\"\n\nd2:\n  type: button\n  title: D2\n  description: \"Button in the right column.\"\n\nd3:\n  type: button\n  title: D3\n  description: \"Button in the right column.\"\n\nd4:\n  type: button\n  title: D4\n  description: \"Button in the right column.\"\n\nd5:\n  type: button\n  title: D5\n  description: \"Button in the right column.\"\n\nd6:\n  type: button\n  title: D6\n  description: \"Bottom button in the right column.\"\n\ne1:\n  type: button\n  title: E\n  description: \"Single-character E button in the lower left cell.\"\n\ne2:\n  type: button\n  title: EE\n  description: \"Two-character E button in the lower left cell.\"\n\ne3:\n  type: button\n  title: EEE\n  description: \"Three-character E button in the lower left cell.\"\n\ne4:\n  type: button\n  title: EEEE\n  description: \"Four-character E button in the lower left cell.\"\n\ne5:\n  type: button\n  title: EEEEE\n  description: \"Five-character E button in the lower left cell.\"\n\ne6:\n  type: button\n  title: EEEEEE\n  description: \"Six-character E button in the lower left cell.\"\n\ne7:\n  type: button\n  title: EEEEEEE\n  description: \"Seven-character E button in the lower left cell.\"\n\ne8:\n  type: button\n  title: EEEEEEEE\n  description: \"Eight-character E button in the lower left cell.\"\n\nf1:\n  type: button\n  title: F1\n  description: \"Top button in the cyan cell.\"\n\nf2:\n  type: button\n  title: F2\n  description: \"Middle button in the cyan cell with a local style override.\"\n\nf3:\n  type: button\n  title: F3\n  description: \"Bottom button in the cyan cell.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#1f2946\"\n  border-color: transparent\n  border-width: 1\n  border-width-vertical: 2\nbutton:\n  background: \"#526173\"\n  color: \"#eef2f7\"\n  focus-background: \"#6b7c91\"\n@d1:\n  color: \"#d7fbe8\"\ncell1:\n  background: \"#3b1f2d\"\n  button:\n    background: \"#7a3f59\"\n    focus-background: \"#96536d\"\ncell2:\n  background: \"#1f463f\"\n  button:\n    background: \"#3d7b6d\"\n    focus-background: \"#4f9484\"\ncl3:\n  background: \"#26345f\"\n  button:\n    background: \"#5267a3\"\n    focus-background: \"#657abd\"\ncell4:\n  background: \"#4a4422\"\n  button:\n    background: \"#81753b\"\n    color: \"#ecfdf5\"\n    focus-background: \"#9a8d4a\"\ncell5:\n  background: \"#482456\"\n  button:\n    background: \"#805196\"\n    focus-background: \"#9a65b0\"\ncl6:\n  background: \"#174f55\"\n  button:\n    background: \"#3a8890\"\n    focus-background: \"#4aa1aa\"\n  @f2:\n    background: \"#31565f\"\ncl7:\n  background: \"#2f3745\"\n  button:\n    background: \"#64748b\"\n    focus-background: \"#7b8ba0\"\n```\n\n## User Interface\n\n```ui\n+-cell1-----------+-cell2---+-cl3+\n| a1..... a2..... |  b1...  | d1 |\n+-cell4-----------+---------+    |\n| c1.. c2.. c3.. c4.. c5..  | d2 |\n| c6.. c7.. c8.. c9.. c10.  |    |\n+-cell5---------------+-cl6-+ d3 |\n|                     | f1. |    |\n| e1....... e5....... |     | d4 |\n|                     |     |    |\n| e2....... e6....... | f2. | d5 |\n|                     |     |    |\n| e3....... e7....... |     | d6 |\n|                     | f3. +-cl7+\n| e4....... e8....... |     |    |\n|                     |     |    |\n+---------------------+-----+----+\n```\n", "Layout stress test that demonstrates nested cells, colored cell backgrounds, varying button widths, and expanded layout behavior.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("a1", "Button in the first red cell.", true), uimd::GeneratedElementMetadata::new("a2", "Second button in the first red cell.", true), uimd::GeneratedElementMetadata::new("b1", "Wide button in the green cell.", true), uimd::GeneratedElementMetadata::new("c1", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c2", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c3", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c4", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c5", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c6", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c7", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c8", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c9", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("c10", "Button in the yellow cell group.", true), uimd::GeneratedElementMetadata::new("d1", "Top button in the right column.", true), uimd::GeneratedElementMetadata::new("d2", "Button in the right column.", true), uimd::GeneratedElementMetadata::new("d3", "Button in the right column.", true), uimd::GeneratedElementMetadata::new("d4", "Button in the right column.", true), uimd::GeneratedElementMetadata::new("d5", "Button in the right column.", true), uimd::GeneratedElementMetadata::new("d6", "Bottom button in the right column.", true), uimd::GeneratedElementMetadata::new("e1", "Single-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e2", "Two-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e3", "Three-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e4", "Four-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e5", "Five-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e6", "Six-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e7", "Seven-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("e8", "Eight-character E button in the lower left cell.", true), uimd::GeneratedElementMetadata::new("f1", "Top button in the cyan cell.", true), uimd::GeneratedElementMetadata::new("f2", "Middle button in the cyan cell with a local style override.", true), uimd::GeneratedElementMetadata::new("f3", "Bottom button in the cyan cell.", true)]);
        base.set_mcp_app_tools(vec![]);
        let a1 = uimd::new_button("a1", "A1");
        base.add_element(a1.clone());
        a1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7a3f59")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        a1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#96536d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        a1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let a2 = uimd::new_button("a2", "A2");
        base.add_element(a2.clone());
        a2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7a3f59")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        a2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#96536d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        a2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let b1 = uimd::new_button("b1", "BBB");
        base.add_element(b1.clone());
        b1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#3d7b6d")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        b1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#4f9484")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        b1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c1 = uimd::new_button("c1", "C1");
        base.add_element(c1.clone());
        c1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c2 = uimd::new_button("c2", "C2");
        base.add_element(c2.clone());
        c2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c3 = uimd::new_button("c3", "C3");
        base.add_element(c3.clone());
        c3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c3.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c4 = uimd::new_button("c4", "C4");
        base.add_element(c4.clone());
        c4.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c4.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c4.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c5 = uimd::new_button("c5", "C5");
        base.add_element(c5.clone());
        c5.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c5.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c5.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c6 = uimd::new_button("c6", "C6");
        base.add_element(c6.clone());
        c6.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c6.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c6.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c7 = uimd::new_button("c7", "C7");
        base.add_element(c7.clone());
        c7.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c7.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c7.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c8 = uimd::new_button("c8", "C8");
        base.add_element(c8.clone());
        c8.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c8.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c8.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c9 = uimd::new_button("c9", "C9");
        base.add_element(c9.clone());
        c9.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c9.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c9.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let c10 = uimd::new_button("c10", "C10");
        base.add_element(c10.clone());
        c10.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#81753b")),
        color: Some(uimd::Color::new("#ecfdf5")),
        ..Default::default()
    });
        c10.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a8d4a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        c10.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d1 = uimd::new_button("d1", "D1");
        base.add_element(d1.clone());
        d1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#d7fbe8")),
        ..Default::default()
    });
        d1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d2 = uimd::new_button("d2", "D2");
        base.add_element(d2.clone());
        d2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        d2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d3 = uimd::new_button("d3", "D3");
        base.add_element(d3.clone());
        d3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        d3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d3.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d4 = uimd::new_button("d4", "D4");
        base.add_element(d4.clone());
        d4.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        d4.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d4.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d5 = uimd::new_button("d5", "D5");
        base.add_element(d5.clone());
        d5.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        d5.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d5.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let d6 = uimd::new_button("d6", "D6");
        base.add_element(d6.clone());
        d6.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#5267a3")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        d6.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#657abd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        d6.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e1 = uimd::new_button("e1", "E");
        base.add_element(e1.clone());
        e1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e2 = uimd::new_button("e2", "EE");
        base.add_element(e2.clone());
        e2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e3 = uimd::new_button("e3", "EEE");
        base.add_element(e3.clone());
        e3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e3.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e4 = uimd::new_button("e4", "EEEE");
        base.add_element(e4.clone());
        e4.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e4.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e4.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e5 = uimd::new_button("e5", "EEEEE");
        base.add_element(e5.clone());
        e5.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e5.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e5.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e6 = uimd::new_button("e6", "EEEEEE");
        base.add_element(e6.clone());
        e6.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e6.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e6.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e7 = uimd::new_button("e7", "EEEEEEE");
        base.add_element(e7.clone());
        e7.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e7.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e7.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let e8 = uimd::new_button("e8", "EEEEEEEE");
        base.add_element(e8.clone());
        e8.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#805196")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        e8.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9a65b0")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        e8.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let f1 = uimd::new_button("f1", "F1");
        base.add_element(f1.clone());
        f1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#3a8890")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        f1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#4aa1aa")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        f1.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let f2 = uimd::new_button("f2", "F2");
        base.add_element(f2.clone());
        f2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#31565f")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        f2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#4aa1aa")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        f2.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let f3 = uimd::new_button("f3", "F3");
        base.add_element(f3.clone());
        f3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#3a8890")),
        color: Some(uimd::Color::new("#eef2f7")),
        ..Default::default()
    });
        f3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#4aa1aa")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        f3.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            a1,
            a2,
            b1,
            c1,
            c2,
            c3,
            c4,
            c5,
            c6,
            c7,
            c8,
            c9,
            c10,
            d1,
            d2,
            d3,
            d4,
            d5,
            d6,
            e1,
            e2,
            e3,
            e4,
            e5,
            e6,
            e7,
            e8,
            f1,
            f2,
            f3,
        }
    }

    pub fn run<H: CellsUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CellsUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CellsUI
{
    fn default() -> Self { Self::new() }
}

pub trait CellsUIEvents
{
    fn on_a1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_a2_click(&mut self, _ui: &mut CellsUI) {}
    fn on_b1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c2_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c3_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c4_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c5_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c6_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c7_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c8_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c9_click(&mut self, _ui: &mut CellsUI) {}
    fn on_c10_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d2_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d3_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d4_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d5_click(&mut self, _ui: &mut CellsUI) {}
    fn on_d6_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e2_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e3_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e4_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e5_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e6_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e7_click(&mut self, _ui: &mut CellsUI) {}
    fn on_e8_click(&mut self, _ui: &mut CellsUI) {}
    fn on_f1_click(&mut self, _ui: &mut CellsUI) {}
    fn on_f2_click(&mut self, _ui: &mut CellsUI) {}
    fn on_f3_click(&mut self, _ui: &mut CellsUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut CellsUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CellsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CellsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CellsUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CellsUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CellsUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut CellsUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut CellsUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CellsUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CellsUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CellsUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CellsUIRuntime<'a, H: CellsUIEvents>
{
    ui: &'a mut CellsUI,
    handler: &'a mut H,
}

impl<H: CellsUIEvents> uimd::GeneratedApplication for CellsUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "a1" { self.handler.on_a1_click(self.ui); return true; }
        if name == "a2" { self.handler.on_a2_click(self.ui); return true; }
        if name == "b1" { self.handler.on_b1_click(self.ui); return true; }
        if name == "c1" { self.handler.on_c1_click(self.ui); return true; }
        if name == "c2" { self.handler.on_c2_click(self.ui); return true; }
        if name == "c3" { self.handler.on_c3_click(self.ui); return true; }
        if name == "c4" { self.handler.on_c4_click(self.ui); return true; }
        if name == "c5" { self.handler.on_c5_click(self.ui); return true; }
        if name == "c6" { self.handler.on_c6_click(self.ui); return true; }
        if name == "c7" { self.handler.on_c7_click(self.ui); return true; }
        if name == "c8" { self.handler.on_c8_click(self.ui); return true; }
        if name == "c9" { self.handler.on_c9_click(self.ui); return true; }
        if name == "c10" { self.handler.on_c10_click(self.ui); return true; }
        if name == "d1" { self.handler.on_d1_click(self.ui); return true; }
        if name == "d2" { self.handler.on_d2_click(self.ui); return true; }
        if name == "d3" { self.handler.on_d3_click(self.ui); return true; }
        if name == "d4" { self.handler.on_d4_click(self.ui); return true; }
        if name == "d5" { self.handler.on_d5_click(self.ui); return true; }
        if name == "d6" { self.handler.on_d6_click(self.ui); return true; }
        if name == "e1" { self.handler.on_e1_click(self.ui); return true; }
        if name == "e2" { self.handler.on_e2_click(self.ui); return true; }
        if name == "e3" { self.handler.on_e3_click(self.ui); return true; }
        if name == "e4" { self.handler.on_e4_click(self.ui); return true; }
        if name == "e5" { self.handler.on_e5_click(self.ui); return true; }
        if name == "e6" { self.handler.on_e6_click(self.ui); return true; }
        if name == "e7" { self.handler.on_e7_click(self.ui); return true; }
        if name == "e8" { self.handler.on_e8_click(self.ui); return true; }
        if name == "f1" { self.handler.on_f1_click(self.ui); return true; }
        if name == "f2" { self.handler.on_f2_click(self.ui); return true; }
        if name == "f3" { self.handler.on_f3_click(self.ui); return true; }
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

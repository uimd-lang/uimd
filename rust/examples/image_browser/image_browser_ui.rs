// Auto-generated UI code for image_browser - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "image_button_ui.rs"]
mod uimd_dependency_image_button;
#[path = "gallery_mosaic_ui.rs"]
mod uimd_dependency_gallery_mosaic;
#[path = "image_list_button_ui.rs"]
mod uimd_dependency_image_list_button;
#[path = "image_view_ui.rs"]
mod uimd_dependency_image_view;
#[path = "gallery_view_ui.rs"]
mod uimd_dependency_gallery_view;
#[path = "image_list_view_ui.rs"]
mod uimd_dependency_image_list_view;
#[path = "image_show_dialog_ui.rs"]
mod uimd_dependency_image_show_dialog;

pub struct ImageBrowserUI
{
    pub base: uimd::GeneratedWindow,
    pub title: uimd::LabelRef,
    pub close_btn: uimd::ButtonRef,
    pub render_label: uimd::LabelRef,
    pub mode_select: uimd::ComboBoxRef,
    pub sidebar_title: uimd::LabelRef,
    pub camera_thumb: uimd::ReusableElementRef,
    pub astro_thumb: uimd::ReusableElementRef,
    pub coffee_thumb: uimd::ReusableElementRef,
    pub gallery_label: uimd::LabelRef,
    pub image_list_label: uimd::LabelRef,
    pub gallery_mosaic: uimd::ReusableElementRef,
    pub image_list_btn: uimd::ReusableElementRef,
    pub spacer: uimd::LabelRef,
    pub main: uimd::ViewHostRef,
}

impl ImageBrowserUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image Browser");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "viewhost".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 29,
            cell_chars_width: 49, cell_chars_height: 41,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 41,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 47, chars_height: 41,
            margin_right: 1, margin_bottom: 0,
            content: "main".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 1, margin_bottom: 40,
            content: "render_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 64, chars_height: 1,
            margin_right: 13, margin_bottom: 0,
            content: "title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        padding: Some(0),
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
            row: 0, col: 66,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "close_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 1, margin_bottom: 39,
            content: "mode_select".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 1, margin_bottom: 37,
            content: "sidebar_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image_button".to_string(),
            row: 4, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 6,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 6,
            margin_right: 1, margin_bottom: 31,
            content: "camera_thumb".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "image_button".to_string(),
            row: 10, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 6,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 6,
            margin_right: 1, margin_bottom: 25,
            content: "astro_thumb".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "image_button".to_string(),
            row: 16, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 6,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 6,
            margin_right: 1, margin_bottom: 19,
            content: "coffee_thumb".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 22, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 1, margin_bottom: 18,
            content: "gallery_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "gallery_mosaic".to_string(),
            row: 23, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 9,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 9,
            margin_right: 1, margin_bottom: 9,
            content: "gallery_mosaic".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 32, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "image_list_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image_list_button".to_string(),
            row: 33, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 5,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 5,
            margin_right: 1, margin_bottom: 3,
            content: "image_list_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 38, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 41,
            cell_name: "sidebar".to_string(), cell_width: 28, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 3,
            margin_right: 1, margin_bottom: 0,
            content: "spacer".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
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
        padding: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("sidebar", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("close_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        base.set_generated_named_style("render_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("sidebar_title", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("gallery_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("image_list_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ImageBrowser", "# Image Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Image browser with a thumbnail sidebar and full-size image panel.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_view\n  - gallery_view\n  - gallery_mosaic\n  - image_button\n  - image_list_button\n  - image_list_view\n  - image_show_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image Browser\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image browser.\"\n\nrender_label:\n  type: label\n  text: \"Render\"\n  description: \"Label above the render mode selector.\"\n  expose: false\n\nmode_select:\n  type: combobox\n  options: [Normal, Fallback]\n  selected_item: Normal\n  description: \"Switch between Sixel and fallback image rendering.\"\n\nsidebar_title:\n  type: label\n  text: \"Photos\"\n  description: \"Sidebar section label.\"\n  expose: false\n\ncamera_thumb:\n  type: image_button\n  description: \"Camera thumbnail. Click to view full size.\"\n\nastro_thumb:\n  type: image_button\n  description: \"Astronaut thumbnail. Click to view full size.\"\n\ncoffee_thumb:\n  type: image_button\n  description: \"Coffee thumbnail. Click to view full size.\"\n\ngallery_label:\n  type: label\n  text: \"Gallery\"\n  description: \"Label above the gallery mosaic.\"\n  expose: false\n\nimage_list_label:\n  type: label\n  text: \"Image items\"\n  description: \"List preview label.\"\n  expose: false\n\ngallery_mosaic:\n  type: gallery_mosaic\n  description: \"Click any image to open the full scrollable gallery.\"\n\nimage_list_btn:\n  type: image_list_button\n  description: \"Click to open the image item list.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Spacer that absorbs leftover sidebar height.\"\n  expose: false\n\nmain:\n  type: viewhost\n  description: \"Full-size image panel.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@sidebar:\n  background: \"#172033\"\n  padding: 0\n@content:\n  background: \"#030712\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@render_label:\n  color: \"#93c5fd\"\n@sidebar_title:\n  color: \"#93c5fd\"\n@gallery_label:\n  color: \"#93c5fd\"\n@image_list_label:\n  color: \"#93c5fd\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**-------------------------------------------------------------------+\n| title..**....................................................... close_btn.. |\n+--sidebar-------------------+--content-**-------------------------------------+\n| render_label.............. | main..**....................................... |\n* mode_select............... | ............................................... |\n*                            | *.............................................. |\n| sidebar_title............. | *.............................................. |\n| camera_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| astro_thumb............... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| coffee_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| gallery_label............. | ............................................... |\n| gallery_mosaic............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| image_list_label.......... | ............................................... |\n| image_list_btn............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n* spacer.................... | ............................................... |\n* .......................... | ............................................... |\n| .......................... | ............................................... |\n+----------------------------+-------------------------------------------------+\n```\n", "Image browser with a thumbnail sidebar and full-size image panel.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title", "Window title.", false), uimd::GeneratedElementMetadata::new("close_btn", "Close the image browser.", true), uimd::GeneratedElementMetadata::new("render_label", "Label above the render mode selector.", false), uimd::GeneratedElementMetadata::new("mode_select", "Switch between Sixel and fallback image rendering.", true), uimd::GeneratedElementMetadata::new("sidebar_title", "Sidebar section label.", false), uimd::GeneratedElementMetadata::new("camera_thumb", "Camera thumbnail. Click to view full size.", true), uimd::GeneratedElementMetadata::new("astro_thumb", "Astronaut thumbnail. Click to view full size.", true), uimd::GeneratedElementMetadata::new("coffee_thumb", "Coffee thumbnail. Click to view full size.", true), uimd::GeneratedElementMetadata::new("gallery_label", "Label above the gallery mosaic.", false), uimd::GeneratedElementMetadata::new("image_list_label", "List preview label.", false), uimd::GeneratedElementMetadata::new("gallery_mosaic", "Click any image to open the full scrollable gallery.", true), uimd::GeneratedElementMetadata::new("image_list_btn", "Click to open the image item list.", true), uimd::GeneratedElementMetadata::new("spacer", "Spacer that absorbs leftover sidebar height.", false), uimd::GeneratedElementMetadata::new("main", "Full-size image panel.", true)]);
        base.set_mcp_app_tools(vec![]);
        let title = uimd::new_label("title", "Image Browser");
        base.add_element(title.clone());
        title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        title.borrow_mut().set_cursor_style(uimd::Style {
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
        let render_label = uimd::new_label("render_label", "Render");
        base.add_element(render_label.clone());
        render_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        render_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let mode_select = uimd::new_combo_box("mode_select", vec!["Normal".to_string(), "Fallback".to_string()]);
        base.add_element(mode_select.clone());
        mode_select.borrow_mut().set_selected_index(0);
        mode_select.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        mode_select.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mode_select.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        mode_select.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mode_select.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let sidebar_title = uimd::new_label("sidebar_title", "Photos");
        base.add_element(sidebar_title.clone());
        sidebar_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        sidebar_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let camera_thumb = uimd::new_reusable_element("camera_thumb", "image_button");
        base.add_element(camera_thumb.clone());
        camera_thumb.borrow_mut().set_child_window(uimd_dependency_image_button::ImageButtonUI::new().base);
        camera_thumb.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        camera_thumb.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        camera_thumb.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let astro_thumb = uimd::new_reusable_element("astro_thumb", "image_button");
        base.add_element(astro_thumb.clone());
        astro_thumb.borrow_mut().set_child_window(uimd_dependency_image_button::ImageButtonUI::new().base);
        astro_thumb.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        astro_thumb.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        astro_thumb.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let coffee_thumb = uimd::new_reusable_element("coffee_thumb", "image_button");
        base.add_element(coffee_thumb.clone());
        coffee_thumb.borrow_mut().set_child_window(uimd_dependency_image_button::ImageButtonUI::new().base);
        coffee_thumb.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        coffee_thumb.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        coffee_thumb.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let gallery_label = uimd::new_label("gallery_label", "Gallery");
        base.add_element(gallery_label.clone());
        gallery_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        gallery_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let image_list_label = uimd::new_label("image_list_label", "Image items");
        base.add_element(image_list_label.clone());
        image_list_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        image_list_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let gallery_mosaic = uimd::new_reusable_element("gallery_mosaic", "gallery_mosaic");
        base.add_element(gallery_mosaic.clone());
        gallery_mosaic.borrow_mut().set_child_window(uimd_dependency_gallery_mosaic::GalleryMosaicUI::new().base);
        gallery_mosaic.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        gallery_mosaic.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        gallery_mosaic.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let image_list_btn = uimd::new_reusable_element("image_list_btn", "image_list_button");
        base.add_element(image_list_btn.clone());
        image_list_btn.borrow_mut().set_child_window(uimd_dependency_image_list_button::ImageListButtonUI::new().base);
        image_list_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        image_list_btn.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        image_list_btn.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let spacer = uimd::new_label("spacer", "");
        base.add_element(spacer.clone());
        spacer.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        spacer.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let main = uimd::new_view_host("main");
        base.add_element(main.clone());
        main.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        Self
        {
            base,
            title,
            close_btn,
            render_label,
            mode_select,
            sidebar_title,
            camera_thumb,
            astro_thumb,
            coffee_thumb,
            gallery_label,
            image_list_label,
            gallery_mosaic,
            image_list_btn,
            spacer,
            main,
        }
    }

    pub fn run<H: ImageBrowserUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageBrowserUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageBrowserUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageBrowserUIEvents
{
    fn on_close_btn_click(&mut self, _ui: &mut ImageBrowserUI) {}
    fn on_mode_select_change(&mut self, _ui: &mut ImageBrowserUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageBrowserUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageBrowserUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageBrowserUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageBrowserUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageBrowserUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageBrowserUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ImageBrowserUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ImageBrowserUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageBrowserUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageBrowserUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageBrowserUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageBrowserUIRuntime<'a, H: ImageBrowserUIEvents>
{
    ui: &'a mut ImageBrowserUI,
    handler: &'a mut H,
}

impl<H: ImageBrowserUIEvents> uimd::GeneratedApplication for ImageBrowserUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "close_btn" { self.handler.on_close_btn_click(self.ui); return true; }
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
        if name == "mode_select" { self.handler.on_mode_select_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

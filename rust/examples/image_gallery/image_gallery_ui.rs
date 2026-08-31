// Auto-generated UI code for image_gallery - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageGalleryUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub close_btn: uimd::ButtonRef,
    pub fallback_label: uimd::LabelRef,
    pub sixel_label: uimd::LabelRef,
    pub stretch_label: uimd::LabelRef,
    pub camera_fb: uimd::ImageRef,
    pub astro_fb: uimd::ImageRef,
    pub coffee_fb: uimd::ImageRef,
    pub coins_fb: uimd::ImageRef,
    pub camera_ctn: uimd::ImageRef,
    pub astro_ctn: uimd::ImageRef,
    pub coffee_ctn: uimd::ImageRef,
    pub coins_ctn: uimd::ImageRef,
    pub camera_str: uimd::ImageRef,
    pub astro_str: uimd::ImageRef,
    pub coffee_str: uimd::ImageRef,
    pub coins_str: uimd::ImageRef,
    pub status: uimd::LabelRef,
}

impl ImageGalleryUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image Gallery");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 72, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 72, chars_height: 1,
            margin_right: 13, margin_bottom: 38,
            content: "header".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 74,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 1, margin_bottom: 38,
            content: "close_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 84, chars_height: 1,
            margin_right: 1, margin_bottom: 36,
            content: "fallback_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 3, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 64, margin_bottom: 26,
            content: "camera_fb".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 3, col: 23,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 43, margin_bottom: 26,
            content: "astro_fb".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 3, col: 44,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 22, margin_bottom: 26,
            content: "coffee_fb".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 3, col: 65,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 10,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 10,
            margin_right: 0, margin_bottom: 26,
            content: "coins_fb".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 14, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 84, chars_height: 1,
            margin_right: 1, margin_bottom: 24,
            content: "sixel_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 15, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 64, margin_bottom: 14,
            content: "camera_ctn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 15, col: 23,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 43, margin_bottom: 14,
            content: "astro_ctn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 15, col: 44,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 22, margin_bottom: 14,
            content: "coffee_ctn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 15, col: 65,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 10,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 1, margin_bottom: 14,
            content: "coins_ctn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 26, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 84, chars_height: 1,
            margin_right: 1, margin_bottom: 12,
            content: "stretch_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 27, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 64, margin_bottom: 2,
            content: "camera_str".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 27, col: 23,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 43, margin_bottom: 2,
            content: "astro_str".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 27, col: 44,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 10,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 22, margin_bottom: 2,
            content: "coffee_str".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 27, col: 65,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 10,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 10,
            margin_right: 1, margin_bottom: 2,
            content: "coins_str".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 38, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 86, cell_chars_height: 39,
            cell_name: "".to_string(), cell_width: 86, cell_height: 39,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 84, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
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
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("close_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        base.set_generated_named_style("fallback_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("sixel_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("stretch_label", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ImageGallery", "# Image Gallery\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: \"Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.\"\ntags: [example, image]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Image Gallery\"\n  description: \"Window title.\"\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: \"Close the image gallery.\"\n\nfallback_label:\n  type: label\n  text: \"Fallback cover\"\n  description: \"Fallback row label.\"\n\nsixel_label:\n  type: label\n  text: \"Sixel contain\"\n  description: \"Sixel contain row label.\"\n\nstretch_label:\n  type: label\n  text: \"Sixel stretch\"\n  description: \"Sixel stretch row label.\"\n\ncamera_fb:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Camera image forced through fallback rendering.\"\n\nastro_fb:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Astronaut image forced through fallback rendering.\"\n\ncoffee_fb:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coffee image forced through fallback rendering.\"\n\ncoins_fb:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins fallback\"\n  fit: cover\n  render_mode: fallback\n  description: \"Coins image forced through fallback rendering.\"\n\ncamera_ctn:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Camera image in forced Sixel contain mode.\"\n\nastro_ctn:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel contain mode.\"\n\ncoffee_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel contain mode.\"\n\ncoins_ctn:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins sixel\"\n  fit: contain\n  render_mode: sixel\n  description: \"Coins image in forced Sixel contain mode.\"\n\ncamera_str:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"camera stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Camera image in forced Sixel stretch mode.\"\n\nastro_str:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"astronaut stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Astronaut image in forced Sixel stretch mode.\"\n\ncoffee_str:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"coffee stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coffee image in forced Sixel stretch mode.\"\n\ncoins_str:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"coins stretch\"\n  fit: stretch\n  render_mode: sixel\n  description: \"Coins image in forced Sixel stretch mode.\"\n\nstatus:\n  type: label\n  text: \"Rows: fallback cover, forced Sixel contain, forced Sixel stretch.\"\n  description: \"Current image validation status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n  color: \"#ffffff\"\n@fallback_label:\n  color: \"#93c5fd\"\n@sixel_label:\n  color: \"#93c5fd\"\n@stretch_label:\n  color: \"#93c5fd\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------------------------------------------------------------+\n| header.................................................................. close_btn.. |\n|                                                                                      |\n| fallback_label...................................................................... |\n|  camera_fb........... astro_fb............ coffee_fb........... coins_fb............. |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| sixel_label......................................................................... |\n|  camera_ctn.......... astro_ctn........... coffee_ctn.......... coins_ctn........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| stretch_label....................................................................... |\n|  camera_str.......... astro_str........... coffee_str.......... coins_str........... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|  .................... .................... .................... .................... |\n|                                                                                      |\n| status.............................................................................. |\n+--------------------------------------------------------------------------------------+\n```\n", "Manual validation surface for the Image element, Sixel rendering, fallback rendering, and sizing modes.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Window title.", false), uimd::GeneratedElementMetadata::new("close_btn", "Close the image gallery.", true), uimd::GeneratedElementMetadata::new("fallback_label", "Fallback row label.", true), uimd::GeneratedElementMetadata::new("sixel_label", "Sixel contain row label.", true), uimd::GeneratedElementMetadata::new("stretch_label", "Sixel stretch row label.", true), uimd::GeneratedElementMetadata::new("camera_fb", "Camera image forced through fallback rendering.", true), uimd::GeneratedElementMetadata::new("astro_fb", "Astronaut image forced through fallback rendering.", true), uimd::GeneratedElementMetadata::new("coffee_fb", "Coffee image forced through fallback rendering.", true), uimd::GeneratedElementMetadata::new("coins_fb", "Coins image forced through fallback rendering.", true), uimd::GeneratedElementMetadata::new("camera_ctn", "Camera image in forced Sixel contain mode.", true), uimd::GeneratedElementMetadata::new("astro_ctn", "Astronaut image in forced Sixel contain mode.", true), uimd::GeneratedElementMetadata::new("coffee_ctn", "Coffee image in forced Sixel contain mode.", true), uimd::GeneratedElementMetadata::new("coins_ctn", "Coins image in forced Sixel contain mode.", true), uimd::GeneratedElementMetadata::new("camera_str", "Camera image in forced Sixel stretch mode.", true), uimd::GeneratedElementMetadata::new("astro_str", "Astronaut image in forced Sixel stretch mode.", true), uimd::GeneratedElementMetadata::new("coffee_str", "Coffee image in forced Sixel stretch mode.", true), uimd::GeneratedElementMetadata::new("coins_str", "Coins image in forced Sixel stretch mode.", true), uimd::GeneratedElementMetadata::new("status", "Current image validation status.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let header = uimd::new_label("header", "Image Gallery");
        base.add_element(header.clone());
        header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
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
        let fallback_label = uimd::new_label("fallback_label", "Fallback cover");
        base.add_element(fallback_label.clone());
        fallback_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        fallback_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let sixel_label = uimd::new_label("sixel_label", "Sixel contain");
        base.add_element(sixel_label.clone());
        sixel_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        sixel_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let stretch_label = uimd::new_label("stretch_label", "Sixel stretch");
        base.add_element(stretch_label.clone());
        stretch_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        stretch_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let camera_fb = uimd::new_image("camera_fb", "shared/assets/image_samples/camera.png", "camera fallback", "cover", "fallback", "center", "middle");
        base.add_element(camera_fb.clone());
        camera_fb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let astro_fb = uimd::new_image("astro_fb", "shared/assets/image_samples/astronaut.png", "astronaut fallback", "cover", "fallback", "center", "middle");
        base.add_element(astro_fb.clone());
        astro_fb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coffee_fb = uimd::new_image("coffee_fb", "shared/assets/image_samples/coffee.png", "coffee fallback", "cover", "fallback", "center", "middle");
        base.add_element(coffee_fb.clone());
        coffee_fb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coins_fb = uimd::new_image("coins_fb", "shared/assets/image_samples/coins.png", "coins fallback", "cover", "fallback", "center", "middle");
        base.add_element(coins_fb.clone());
        coins_fb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let camera_ctn = uimd::new_image("camera_ctn", "shared/assets/image_samples/camera.png", "camera sixel", "contain", "sixel", "center", "middle");
        base.add_element(camera_ctn.clone());
        camera_ctn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let astro_ctn = uimd::new_image("astro_ctn", "shared/assets/image_samples/astronaut.png", "astronaut sixel", "contain", "sixel", "center", "middle");
        base.add_element(astro_ctn.clone());
        astro_ctn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coffee_ctn = uimd::new_image("coffee_ctn", "shared/assets/image_samples/coffee.png", "coffee sixel", "contain", "sixel", "center", "middle");
        base.add_element(coffee_ctn.clone());
        coffee_ctn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coins_ctn = uimd::new_image("coins_ctn", "shared/assets/image_samples/coins.png", "coins sixel", "contain", "sixel", "center", "middle");
        base.add_element(coins_ctn.clone());
        coins_ctn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let camera_str = uimd::new_image("camera_str", "shared/assets/image_samples/camera.png", "camera stretch", "stretch", "sixel", "center", "middle");
        base.add_element(camera_str.clone());
        camera_str.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let astro_str = uimd::new_image("astro_str", "shared/assets/image_samples/astronaut.png", "astronaut stretch", "stretch", "sixel", "center", "middle");
        base.add_element(astro_str.clone());
        astro_str.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coffee_str = uimd::new_image("coffee_str", "shared/assets/image_samples/coffee.png", "coffee stretch", "stretch", "sixel", "center", "middle");
        base.add_element(coffee_str.clone());
        coffee_str.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let coins_str = uimd::new_image("coins_str", "shared/assets/image_samples/coins.png", "coins stretch", "stretch", "sixel", "center", "middle");
        base.add_element(coins_str.clone());
        coins_str.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let status = uimd::new_label("status", "Rows: fallback cover, forced Sixel contain, forced Sixel stretch.");
        base.add_element(status.clone());
        status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
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
            header,
            close_btn,
            fallback_label,
            sixel_label,
            stretch_label,
            camera_fb,
            astro_fb,
            coffee_fb,
            coins_fb,
            camera_ctn,
            astro_ctn,
            coffee_ctn,
            coins_ctn,
            camera_str,
            astro_str,
            coffee_str,
            coins_str,
            status,
        }
    }

    pub fn run<H: ImageGalleryUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageGalleryUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageGalleryUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageGalleryUIEvents
{
    fn on_close_btn_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_camera_fb_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_astro_fb_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coffee_fb_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coins_fb_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_camera_ctn_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_astro_ctn_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coffee_ctn_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coins_ctn_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_camera_str_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_astro_str_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coffee_str_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn on_coins_str_click(&mut self, _ui: &mut ImageGalleryUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageGalleryUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageGalleryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageGalleryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageGalleryUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageGalleryUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageGalleryUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ImageGalleryUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ImageGalleryUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageGalleryUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageGalleryUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageGalleryUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageGalleryUIRuntime<'a, H: ImageGalleryUIEvents>
{
    ui: &'a mut ImageGalleryUI,
    handler: &'a mut H,
}

impl<H: ImageGalleryUIEvents> uimd::GeneratedApplication for ImageGalleryUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "close_btn" { self.handler.on_close_btn_click(self.ui); return true; }
        if name == "camera_fb" { self.handler.on_camera_fb_click(self.ui); return true; }
        if name == "astro_fb" { self.handler.on_astro_fb_click(self.ui); return true; }
        if name == "coffee_fb" { self.handler.on_coffee_fb_click(self.ui); return true; }
        if name == "coins_fb" { self.handler.on_coins_fb_click(self.ui); return true; }
        if name == "camera_ctn" { self.handler.on_camera_ctn_click(self.ui); return true; }
        if name == "astro_ctn" { self.handler.on_astro_ctn_click(self.ui); return true; }
        if name == "coffee_ctn" { self.handler.on_coffee_ctn_click(self.ui); return true; }
        if name == "coins_ctn" { self.handler.on_coins_ctn_click(self.ui); return true; }
        if name == "camera_str" { self.handler.on_camera_str_click(self.ui); return true; }
        if name == "astro_str" { self.handler.on_astro_str_click(self.ui); return true; }
        if name == "coffee_str" { self.handler.on_coffee_str_click(self.ui); return true; }
        if name == "coins_str" { self.handler.on_coins_str_click(self.ui); return true; }
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

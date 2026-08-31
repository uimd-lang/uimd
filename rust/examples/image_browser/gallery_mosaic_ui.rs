// Auto-generated UI code for gallery_mosaic - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct GalleryMosaicUI
{
    pub base: uimd::GeneratedWindow,
    pub gal_1: uimd::ImageRef,
    pub gal_2: uimd::ImageRef,
    pub gal_3: uimd::ImageRef,
    pub gal_4: uimd::ImageRef,
    pub gal_5: uimd::ImageRef,
}

impl GalleryMosaicUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Gallery Mosaic");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 7,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 18, margin_bottom: 4,
            content: "gal_1".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 0, col: 9,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 7,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 10, margin_bottom: 4,
            content: "gal_2".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 0, col: 17,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 7,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 8, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 8, chars_height: 3,
            margin_right: 1, margin_bottom: 4,
            content: "gal_3".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 4, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 7,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 18, margin_bottom: 0,
            content: "gal_4".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 4, col: 9,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 7,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 10, margin_bottom: 0,
            content: "gal_5".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("control");
        base.set_generated_focusable(true);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#172033")),
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
        base.set_mcp_metadata(true, "GalleryMosaic", "# Gallery Mosaic\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"3+2 image mosaic that opens the scrollable gallery when clicked.\"\ntags: [example, image, gallery]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\ngal_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Camera\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 1. Click to open gallery.\"\n\ngal_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Astronaut\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 2. Click to open gallery.\"\n\ngal_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Coffee\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 3. Click to open gallery.\"\n\ngal_4:\n  type: image\n  source: \"shared/assets/image_samples/coins.png\"\n  alt: \"Coins\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 4. Click to open gallery.\"\n\ngal_5:\n  type: image\n  source: \"shared/assets/image_samples/chelsea.png\"\n  alt: \"Chelsea\"\n  fit: cover\n  render_mode: auto\n  description: \"Gallery mosaic slot 5. Click to open gallery.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**------------------+\n| gal_1.. gal_2.. gal_3... |\n| ....... ....... ........ |\n| ....... ....... ........ |\n|                          |\n| gal_4.. gal_5..          |\n| ....... .......          |\n| ....... .......          |\n+--------------------------+\n```\n", "3+2 image mosaic that opens the scrollable gallery when clicked.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("gal_1", "Gallery mosaic slot 1. Click to open gallery.", true), uimd::GeneratedElementMetadata::new("gal_2", "Gallery mosaic slot 2. Click to open gallery.", true), uimd::GeneratedElementMetadata::new("gal_3", "Gallery mosaic slot 3. Click to open gallery.", true), uimd::GeneratedElementMetadata::new("gal_4", "Gallery mosaic slot 4. Click to open gallery.", true), uimd::GeneratedElementMetadata::new("gal_5", "Gallery mosaic slot 5. Click to open gallery.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let gal_1 = uimd::new_image("gal_1", "shared/assets/image_samples/camera.png", "Camera", "cover", "auto", "center", "middle");
        base.add_element(gal_1.clone());
        gal_1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        gal_1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let gal_2 = uimd::new_image("gal_2", "shared/assets/image_samples/astronaut.png", "Astronaut", "cover", "auto", "center", "middle");
        base.add_element(gal_2.clone());
        gal_2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        gal_2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let gal_3 = uimd::new_image("gal_3", "shared/assets/image_samples/coffee.png", "Coffee", "cover", "auto", "center", "middle");
        base.add_element(gal_3.clone());
        gal_3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        gal_3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let gal_4 = uimd::new_image("gal_4", "shared/assets/image_samples/coins.png", "Coins", "cover", "auto", "center", "middle");
        base.add_element(gal_4.clone());
        gal_4.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        gal_4.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let gal_5 = uimd::new_image("gal_5", "shared/assets/image_samples/chelsea.png", "Chelsea", "cover", "auto", "center", "middle");
        base.add_element(gal_5.clone());
        gal_5.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        gal_5.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        Self
        {
            base,
            gal_1,
            gal_2,
            gal_3,
            gal_4,
            gal_5,
        }
    }

    pub fn run<H: GalleryMosaicUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = GalleryMosaicUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for GalleryMosaicUI
{
    fn default() -> Self { Self::new() }
}

pub trait GalleryMosaicUIEvents
{
    fn on_gal_1_click(&mut self, _ui: &mut GalleryMosaicUI) {}
    fn on_gal_2_click(&mut self, _ui: &mut GalleryMosaicUI) {}
    fn on_gal_3_click(&mut self, _ui: &mut GalleryMosaicUI) {}
    fn on_gal_4_click(&mut self, _ui: &mut GalleryMosaicUI) {}
    fn on_gal_5_click(&mut self, _ui: &mut GalleryMosaicUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut GalleryMosaicUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut GalleryMosaicUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut GalleryMosaicUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut GalleryMosaicUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut GalleryMosaicUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut GalleryMosaicUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut GalleryMosaicUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut GalleryMosaicUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut GalleryMosaicUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut GalleryMosaicUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut GalleryMosaicUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct GalleryMosaicUIRuntime<'a, H: GalleryMosaicUIEvents>
{
    ui: &'a mut GalleryMosaicUI,
    handler: &'a mut H,
}

impl<H: GalleryMosaicUIEvents> uimd::GeneratedApplication for GalleryMosaicUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "gal_1" { self.handler.on_gal_1_click(self.ui); return true; }
        if name == "gal_2" { self.handler.on_gal_2_click(self.ui); return true; }
        if name == "gal_3" { self.handler.on_gal_3_click(self.ui); return true; }
        if name == "gal_4" { self.handler.on_gal_4_click(self.ui); return true; }
        if name == "gal_5" { self.handler.on_gal_5_click(self.ui); return true; }
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

// Auto-generated UI code for image_list_button - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageListButtonUI
{
    pub base: uimd::GeneratedWindow,
    pub prw_1: uimd::ImageRef,
    pub prw_2: uimd::ImageRef,
    pub prw_3: uimd::ImageRef,
}

impl ImageListButtonUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image List Button");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 25, cell_chars_height: 3,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 17, margin_bottom: 0,
            content: "prw_1".to_string(),
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
            cell_chars_width: 25, cell_chars_height: 3,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 9, margin_bottom: 0,
            content: "prw_2".to_string(),
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
            cell_chars_width: 25, cell_chars_height: 3,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 3,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 3,
            margin_right: 1, margin_bottom: 0,
            content: "prw_3".to_string(),
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
        base.set_mcp_metadata(true, "ImageListButton", "# Image List Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Clickable image-list preview tile with three thumbnails.\"\ntags: [example, image, button, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nprw_1:\n  type: image\n  source: \"shared/assets/image_samples/camera.png\"\n  alt: \"Preview image 1\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 1. Click to open image items.\"\n\nprw_2:\n  type: image\n  source: \"shared/assets/image_samples/astronaut.png\"\n  alt: \"Preview image 2\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 2. Click to open image items.\"\n\nprw_3:\n  type: image\n  source: \"shared/assets/image_samples/coffee.png\"\n  alt: \"Preview image 3\"\n  fit: cover\n  render_mode: auto\n  description: \"List preview slot 3. Click to open image items.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 0, 1, 0\n  background: \"#172033\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------+\n| prw_1.. prw_2.. prw_3.. |\n| ....... ....... ....... |\n| ....... ....... ....... |\n+-------------------------+\n```\n", "Clickable image-list preview tile with three thumbnails.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("prw_1", "List preview slot 1. Click to open image items.", true), uimd::GeneratedElementMetadata::new("prw_2", "List preview slot 2. Click to open image items.", true), uimd::GeneratedElementMetadata::new("prw_3", "List preview slot 3. Click to open image items.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let prw_1 = uimd::new_image("prw_1", "shared/assets/image_samples/camera.png", "Preview image 1", "cover", "auto", "center", "middle");
        base.add_element(prw_1.clone());
        prw_1.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        prw_1.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let prw_2 = uimd::new_image("prw_2", "shared/assets/image_samples/astronaut.png", "Preview image 2", "cover", "auto", "center", "middle");
        base.add_element(prw_2.clone());
        prw_2.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        prw_2.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        let prw_3 = uimd::new_image("prw_3", "shared/assets/image_samples/coffee.png", "Preview image 3", "cover", "auto", "center", "middle");
        base.add_element(prw_3.clone());
        prw_3.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        prw_3.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        Self
        {
            base,
            prw_1,
            prw_2,
            prw_3,
        }
    }

    pub fn run<H: ImageListButtonUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageListButtonUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageListButtonUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageListButtonUIEvents
{
    fn on_prw_1_click(&mut self, _ui: &mut ImageListButtonUI) {}
    fn on_prw_2_click(&mut self, _ui: &mut ImageListButtonUI) {}
    fn on_prw_3_click(&mut self, _ui: &mut ImageListButtonUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageListButtonUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageListButtonUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageListButtonUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageListButtonUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageListButtonUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageListButtonUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ImageListButtonUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageListButtonUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageListButtonUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageListButtonUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageListButtonUIRuntime<'a, H: ImageListButtonUIEvents>
{
    ui: &'a mut ImageListButtonUI,
    handler: &'a mut H,
}

impl<H: ImageListButtonUIEvents> uimd::GeneratedApplication for ImageListButtonUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "prw_1" { self.handler.on_prw_1_click(self.ui); return true; }
        if name == "prw_2" { self.handler.on_prw_2_click(self.ui); return true; }
        if name == "prw_3" { self.handler.on_prw_3_click(self.ui); return true; }
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

    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool
    {
        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)
    }

    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

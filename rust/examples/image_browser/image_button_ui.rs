// Auto-generated UI code for image_button - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageButtonUI
{
    pub base: uimd::GeneratedWindow,
    pub photo: uimd::ImageRef,
}

impl ImageButtonUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image Button");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 24, cell_chars_height: 4,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 4,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 4,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 4,
            margin_right: 1, margin_bottom: 0,
            content: "photo".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        padding_bottom: Some(1),
        padding_left: Some(4),
        padding_right: Some(4),
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
        base.set_mcp_metadata(true, "ImageButton", "# Image Button\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Single-image clickable tile with padding around a centered square image.\"\ntags: [example, image, button]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nfocusable: true\n```\n\n## Members\n\n```yaml\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: cover\n  render_mode: auto\n  align: center\n  valign: middle\n  description: \"Tile image. Click to select.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#172033\"\ncell:\n  padding: 1, 4, 1, 4\n  background: transparent\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n  focus-background: \"#25364f\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------+\n| photo..**............. |\n| ...................... |\n| ...................... |\n| ...................... |\n+------------------------+\n```\n", "Single-image clickable tile with padding around a centered square image.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("photo", "Tile image. Click to select.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let photo = uimd::new_image("photo", "", "", "cover", "auto", "center", "middle");
        base.add_element(photo.clone());
        photo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        photo.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        ..Default::default()
    });
        Self
        {
            base,
            photo,
        }
    }

    pub fn run<H: ImageButtonUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageButtonUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageButtonUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageButtonUIEvents
{
    fn on_photo_click(&mut self, _ui: &mut ImageButtonUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageButtonUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageButtonUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageButtonUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageButtonUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageButtonUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageButtonUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ImageButtonUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageButtonUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageButtonUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageButtonUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageButtonUIRuntime<'a, H: ImageButtonUIEvents>
{
    ui: &'a mut ImageButtonUI,
    handler: &'a mut H,
}

impl<H: ImageButtonUIEvents> uimd::GeneratedApplication for ImageButtonUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "photo" { self.handler.on_photo_click(self.ui); return true; }
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

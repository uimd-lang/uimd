// Auto-generated UI code for image_view - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageViewUI
{
    pub base: uimd::GeneratedWindow,
    pub caption: uimd::LabelRef,
    pub photo: uimd::ImageRef,
}

impl ImageViewUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image View");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 31, cell_chars_height: 12,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 12,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 29, chars_height: 1,
            margin_right: 1, margin_bottom: 11,
            content: "caption".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 31, cell_chars_height: 12,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 12,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 29, chars_height: 11,
            margin_right: 1, margin_bottom: 0,
            content: "photo".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("control");
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
        base.set_generated_named_style("caption", uimd::Style {
        color: Some(uimd::Color::new("#bfdbfe")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ImageView", "# Image View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Full-size image display panel hosted inside a ViewHost.\"\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ncaption:\n  type: label\n  text: \"Select an image\"\n  description: \"Name of the currently displayed image.\"\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"photo\"\n  fit: contain\n  render_mode: auto\n  description: \"Full-size image display.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@caption:\n  color: \"#bfdbfe\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\n```\n\n## User Interface\n\n```ui\n+-view-**-----------------------+\n| caption..**.................. |\n| photo..**.................... |\n| ............................. |\n| *............................ |\n| *............................ |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n| ............................. |\n+-------------------------------+\n```\n", "Full-size image display panel hosted inside a ViewHost.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("caption", "Name of the currently displayed image.", true), uimd::GeneratedElementMetadata::new("photo", "Full-size image display.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let caption = uimd::new_label("caption", "Select an image");
        base.add_element(caption.clone());
        caption.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        caption.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let photo = uimd::new_image("photo", "", "photo", "contain", "auto", "center", "middle");
        base.add_element(photo.clone());
        photo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        Self
        {
            base,
            caption,
            photo,
        }
    }

    pub fn run<H: ImageViewUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageViewUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageViewUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageViewUIEvents
{
    fn on_photo_click(&mut self, _ui: &mut ImageViewUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageViewUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageViewUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageViewUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageViewUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ImageViewUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ImageViewUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageViewUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageViewUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageViewUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageViewUIRuntime<'a, H: ImageViewUIEvents>
{
    ui: &'a mut ImageViewUI,
    handler: &'a mut H,
}

impl<H: ImageViewUIEvents> uimd::GeneratedApplication for ImageViewUIRuntime<'_, H>
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

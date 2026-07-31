// Auto-generated UI code for image_show_dialog - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageShowDialogUI
{
    pub base: uimd::GeneratedWindow,
    pub title: uimd::LabelRef,
    pub photo: uimd::ImageRef,
    pub close_btn: uimd::ButtonRef,
}

impl ImageShowDialogUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image Show Dialog");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "".to_string(),
            row: 0, col: 0,
            cell_row: 22, cell_col: 0,
            cell_chars_width: 34, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 34, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "".to_string(),
            row: 0, col: 0,
            cell_row: 22, cell_col: 57,
            cell_chars_width: 33, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "dialog".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 88, chars_height: 1,
            margin_right: 1, margin_bottom: 20,
            content: "title".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 2,
            cell_row: 22, cell_col: 35,
            cell_chars_width: 21, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 21, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 2, margin_bottom: 0,
            content: "close_btn".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "dialog".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 88, chars_height: 20,
            margin_right: 1, margin_bottom: 0,
            content: "photo".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
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
        base.set_generated_kind("dialog");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#081426")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(1),
        border_width_vertical: Some(1),
        margin_bottom: Some(4),
        margin_left: Some(6),
        margin_right: Some(6),
        margin_top: Some(4),
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
        base.set_generated_named_style("title", uimd::Style {
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ImageShowDialog", "# Image Show Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\ndescription: \"Dialog for showing a selected image.\"\ntags: [example, image, dialog]\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Image\"\n  description: \"Dialog title.\"\n  expose: false\n\nphoto:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Selected image preview.\"\n\nclose_btn:\n  type: button\n  title: Close\n  description: \"Close the dialog.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 1\n  background: \"#081426\"\n  margin: 4, 6, 4, 6\ncell:\n  padding: 1, 1, 1, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-dialog-**--------------------------------------------------------------------------------+\n| title..**............................................................................... |\n| photo..**............................................................................... |\n* ........................................................................................ |\n* *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+-**-------------------------------+---------------------+-**------------------------------+\n|                                  |  close_btn........  |                                 |\n+----------------------------------+---------------------+---------------------------------+\n```\n", "Dialog for showing a selected image.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title", "Dialog title.", false), uimd::GeneratedElementMetadata::new("photo", "Selected image preview.", true), uimd::GeneratedElementMetadata::new("close_btn", "Close the dialog.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let title = uimd::new_label("title", "Image");
        base.add_element(title.clone());
        title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let photo = uimd::new_image("photo", "", "", "contain", "auto", "center", "middle");
        base.add_element(photo.clone());
        photo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let close_btn = uimd::new_button("close_btn", "Close");
        base.add_element(close_btn.clone());
        close_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            title,
            photo,
            close_btn,
        }
    }

    pub fn run<H: ImageShowDialogUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageShowDialogUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageShowDialogUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageShowDialogUIEvents
{
    fn on_photo_click(&mut self, _ui: &mut ImageShowDialogUI) {}
    fn on_close_btn_click(&mut self, _ui: &mut ImageShowDialogUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageShowDialogUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageShowDialogUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageShowDialogUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageShowDialogUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageShowDialogUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageShowDialogUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ImageShowDialogUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageShowDialogUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageShowDialogUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageShowDialogUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageShowDialogUIRuntime<'a, H: ImageShowDialogUIEvents>
{
    ui: &'a mut ImageShowDialogUI,
    handler: &'a mut H,
}

impl<H: ImageShowDialogUIEvents> uimd::GeneratedApplication for ImageShowDialogUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "photo" { self.handler.on_photo_click(self.ui); return true; }
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

// Auto-generated UI code for paragraph - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ParagraphUI
{
    pub base: uimd::GeneratedWindow,
    pub text: uimd::LabelRef,
}

impl ParagraphUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Paragraph");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 48, cell_chars_height: 3,
            cell_name: "body".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 46, chars_height: 3,
            margin_right: 1, margin_bottom: 0,
            content: "text".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("component");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
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
        base.set_generated_named_style("text", uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Paragraph", "# Paragraph\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown paragraph block.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntext:\n  type: label\n  text: \"\"\n  description: \"Rendered paragraph text with mouse selection enabled.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@text:\n  color: \"#cbd5e1\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-body-**----------------------------------------+\n| text..**...................................... |\n| .............................................. |\n# #............................................ |\n+------------------------------------------------+\n```\n", "Reusable markdown paragraph block.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("text", "Rendered paragraph text with mouse selection enabled.", true)]);
        base.set_mcp_app_tools(vec![]);
        let text = uimd::new_label("text", "");
        base.add_element(text.clone());
        text.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        text.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            text,
        }
    }

    pub fn run<H: ParagraphUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ParagraphUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ParagraphUI
{
    fn default() -> Self { Self::new() }
}

pub trait ParagraphUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut ParagraphUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ParagraphUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ParagraphUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ParagraphUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ParagraphUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ParagraphUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ParagraphUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ParagraphUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ParagraphUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ParagraphUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ParagraphUIRuntime<'a, H: ParagraphUIEvents>
{
    ui: &'a mut ParagraphUI,
    handler: &'a mut H,
}

impl<H: ParagraphUIEvents> uimd::GeneratedApplication for ParagraphUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
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

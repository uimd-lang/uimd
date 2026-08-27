// Auto-generated UI code for compat_scroll - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct CompatScrollUI
{
    pub base: uimd::GeneratedWindow,
    pub alpha_row: uimd::LabelRef,
}

impl CompatScrollUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new_scroll_view_with_gap("UIMD 0.5.3 Compatibility Scroll", 0);
        base.set_generated_layout(vec![
    ]);
        base.set_generated_kind("component");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(0),
        border_width_vertical: Some(0),
        ..Default::default()
    });
        base.set_generated_scroll_view_style(uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        padding: Some(0),
        ..Default::default()
    });
        base.set_generated_scroll_view_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        base.set_generated_scroll_view_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        base.set_generated_scroll_view_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
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
        base.set_generated_named_style("panel", uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        padding: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("alpha_row", uimd::Style {
        background: Some(uimd::Color::new("#252a36cc")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "CompatScroll", "# UIMD 0.5.3 Compatibility Scroll\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\n```\n\n## Members\n\n```yaml\nalpha_row:\n  type: label\n  text: \"legacy alpha row\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@panel:\n  background: \"#303545\"\n  padding: 0\n@alpha_row:\n  background: \"#252a36cc\"\n```\n\n## User Interface\n\n```ui\n+-panel-**----------------------+\n|alpha_row.....................|\n|..............................|\n+------------------------------+\n```\n", "Retained generated ScrollView input for the 0.5.3 patch-compatibility gate.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("alpha_row", "legacy alpha row", true)]);
        base.set_mcp_app_tools(vec![]);
        let alpha_row = uimd::new_label("alpha_row", "legacy alpha row");
        base.add_element(alpha_row.clone());
        alpha_row.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#252a36cc")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        alpha_row.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            alpha_row,
        }
    }

    pub fn run<H: CompatScrollUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CompatScrollUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CompatScrollUI
{
    fn default() -> Self { Self::new() }
}

pub trait CompatScrollUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut CompatScrollUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CompatScrollUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CompatScrollUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CompatScrollUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CompatScrollUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CompatScrollUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut CompatScrollUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CompatScrollUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CompatScrollUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CompatScrollUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CompatScrollUIRuntime<'a, H: CompatScrollUIEvents>
{
    ui: &'a mut CompatScrollUI,
    handler: &'a mut H,
}

impl<H: CompatScrollUIEvents> uimd::GeneratedApplication for CompatScrollUIRuntime<'_, H>
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

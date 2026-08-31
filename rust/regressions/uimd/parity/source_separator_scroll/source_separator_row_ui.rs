// Auto-generated UI code for source_separator_row - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct SourceSeparatorRowUI
{
    pub base: uimd::GeneratedWindow,
    pub sep: uimd::LabelRef,
    pub left: uimd::LabelRef,
}

impl SourceSeparatorRowUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Source Separator Row");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 11, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 11, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "left".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 11, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 11, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "sep".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("component");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
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
        base.set_mcp_metadata(true, "SourceSeparatorRow", "# Source Separator Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Regression row with a design-only ASCII separator between content rows.\"\ntags: [regression, issue-3, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsep:\n  type: label\n  text: Header\n\nleft:\n  type: label\n  text: Left\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#15171c\"\ncell:\n  background: \"#15171c\"\nlabel:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-----------+\n|sep........|\n+-----------+\n|left.......|\n+-----------+\n```\n", "Regression row with a design-only ASCII separator between content rows.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("sep", "Header", true), uimd::GeneratedElementMetadata::new("left", "Left", true)]);
        base.set_mcp_app_tools(vec![]);
        let sep = uimd::new_label("sep", "Header");
        base.add_element(sep.clone());
        sep.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        sep.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let left = uimd::new_label("left", "Left");
        base.add_element(left.clone());
        left.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        left.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            sep,
            left,
        }
    }

    pub fn run<H: SourceSeparatorRowUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = SourceSeparatorRowUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for SourceSeparatorRowUI
{
    fn default() -> Self { Self::new() }
}

pub trait SourceSeparatorRowUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut SourceSeparatorRowUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut SourceSeparatorRowUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut SourceSeparatorRowUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut SourceSeparatorRowUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut SourceSeparatorRowUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct SourceSeparatorRowUIRuntime<'a, H: SourceSeparatorRowUIEvents>
{
    ui: &'a mut SourceSeparatorRowUI,
    handler: &'a mut H,
}

impl<H: SourceSeparatorRowUIEvents> uimd::GeneratedApplication for SourceSeparatorRowUIRuntime<'_, H>
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

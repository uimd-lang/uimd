// Auto-generated UI code for code_entry - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct CodeEntryUI
{
    pub base: uimd::GeneratedWindow,
    pub heading: uimd::SpanLabelRef,
    pub code: uimd::LabelRef,
}

impl CodeEntryUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Agent Chat Code Entry");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "spanlabel".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 5,
            cell_name: "code_panel".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 71, chars_height: 1,
            margin_right: 1, margin_bottom: 4,
            content: "heading".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#0f1b2d")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        color: Some(uimd::Color::new("#ddd6fe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 5,
            cell_name: "code_panel".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 71, chars_height: 4,
            margin_right: 1, margin_bottom: 0,
            content: "code".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#0f1b2d")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
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
        base.set_generated_named_style("code_panel", uimd::Style {
        background: Some(uimd::Color::new("#0f1b2d")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("heading", uimd::Style {
        color: Some(uimd::Color::new("#ddd6fe")),
        ..Default::default()
    });
        base.set_generated_named_style("code", uimd::Style {
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "CodeEntry", "# Agent Chat Code Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style code, activity, command output, or diff block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Code\"\n  description: \"Code language or activity title.\"\n\ncode:\n  type: label\n  text: \"\"\n  description: \"Selectable formatted code, command output, or diff content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@code_panel:\n  background: \"#0f1b2d\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@code:\n  color: \"#d1d5db\"\n  word-wrap: true\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-code_panel-**-----------------------------------------------------------+\n| heading..**............................................................ |\n| code..**............................................................... |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", "Heron-style code, activity, command output, or diff block.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("heading", "Code language or activity title.", true), uimd::GeneratedElementMetadata::new("code", "Selectable formatted code, command output, or diff content.", true)]);
        base.set_mcp_app_tools(vec![]);
        let heading = uimd::new_span_label("heading", "Code");
        base.add_element(heading.clone());
        heading.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#ddd6fe")),
        ..Default::default()
    });
        let code = uimd::new_label("code", "");
        base.add_element(code.clone());
        code.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        code.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            heading,
            code,
        }
    }

    pub fn run<H: CodeEntryUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CodeEntryUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CodeEntryUI
{
    fn default() -> Self { Self::new() }
}

pub trait CodeEntryUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut CodeEntryUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CodeEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CodeEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CodeEntryUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CodeEntryUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CodeEntryUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut CodeEntryUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut CodeEntryUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CodeEntryUI, _key: &str) -> bool { false }
    fn tick(&mut self, _ui: &mut CodeEntryUI) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CodeEntryUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CodeEntryUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CodeEntryUIRuntime<'a, H: CodeEntryUIEvents>
{
    ui: &'a mut CodeEntryUI,
    handler: &'a mut H,
}

impl<H: CodeEntryUIEvents> uimd::GeneratedApplication for CodeEntryUIRuntime<'_, H>
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
    fn tick(&mut self) -> bool { self.handler.tick(self.ui) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

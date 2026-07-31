// Auto-generated UI code for expenses_list - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "../expense_row/expense_row_ui.rs"]
mod uimd_dependency_expense_row;

pub struct ExpensesListUI
{
    pub base: uimd::GeneratedWindow,
}

impl ExpensesListUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new_scroll_view_with_gap("Expenses List", 1);
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 7,
            cell_name: "panel".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 54, chars_height: 7,
            margin_right: 0, margin_bottom: 0,
            content: "".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        gap: Some(1),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
    ]);
        base.set_generated_kind("component");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(0),
        border_width_vertical: Some(0),
        ..Default::default()
    });
        base.set_generated_scroll_view_style(uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        gap: Some(1),
        padding: Some(1),
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
        background: Some(uimd::Color::new("#172033")),
        gap: Some(1),
        padding: Some(1),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ExpensesList", "# Expenses List\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Scrollable list of interactive expense rows.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uiscrollview\nuses:\n  - ../expense_row/expense_row\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#172033\"\n@panel:\n  gap: 1\n  padding: 1\n  background: \"#172033\"\n```\n\n## User Interface\n\n```ui\n+-panel-**---------------------------------------------+\n|                                                      |\n|                                                      |\n|                                                      |\n|                                                      |\n*                                                      |\n*                                                      |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Scrollable list of interactive expense rows.");
        base.set_mcp_element_metadata(vec![]);
        base.set_mcp_app_tools(vec![]);
        Self
        {
            base,
        }
    }

    pub fn run<H: ExpensesListUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ExpensesListUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ExpensesListUI
{
    fn default() -> Self { Self::new() }
}

pub trait ExpensesListUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut ExpensesListUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ExpensesListUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ExpensesListUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ExpensesListUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ExpensesListUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ExpensesListUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ExpensesListUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ExpensesListUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ExpensesListUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ExpensesListUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ExpensesListUIRuntime<'a, H: ExpensesListUIEvents>
{
    ui: &'a mut ExpensesListUI,
    handler: &'a mut H,
}

impl<H: ExpensesListUIEvents> uimd::GeneratedApplication for ExpensesListUIRuntime<'_, H>
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

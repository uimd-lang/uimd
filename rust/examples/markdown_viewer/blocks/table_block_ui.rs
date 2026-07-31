// Auto-generated UI code for table_block - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TableBlockUI
{
    pub base: uimd::GeneratedWindow,
    pub kind: uimd::LabelRef,
    pub table: uimd::MessageTableRef,
}

impl TableBlockUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Table Block");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 48, cell_chars_height: 1,
            cell_name: "head".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 46, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "kind".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "messagetable".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 48, cell_chars_height: 4,
            cell_name: "body".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 46, chars_height: 4,
            margin_right: 1, margin_bottom: 0,
            content: "table".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#0c1a2d")),
        ..Default::default()
    },
            element_style: uimd::Style {
        color: Some(uimd::Color::new("#e5e7eb")),
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
        base.set_generated_named_style("head", uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        ..Default::default()
    });
        base.set_generated_named_style("body", uimd::Style {
        background: Some(uimd::Color::new("#0c1a2d")),
        ..Default::default()
    });
        base.set_generated_named_style("kind", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("table", uimd::Style {
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TableBlock", "# Table Block\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: stable\ndescription: \"Reusable markdown table block with a small header and message table content.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nkind:\n  type: label\n  text: \"markdown table\"\n  description: \"Header label identifying the block as a markdown table.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  background: \"#00000000\"\n  border-width: 0\n@head:\n  background: \"#1d314c\"\n@body:\n  background: \"#0c1a2d\"\n@kind:\n  color: \"#93c5fd\"\n@table:\n  color: \"#e5e7eb\"\n```\n\n## User Interface\n\n```ui\n+-head-**----------------------------------------+\n| kind..**...................................... |\n+-body-**----------------------------------------+\n| table..**..................................... |\n| .............................................. |\n# #............................................ |\n# #............................................ |\n+------------------------------------------------+\n```\n", "Reusable markdown table block with a small header and message table content.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("kind", "Header label identifying the block as a markdown table.", true), uimd::GeneratedElementMetadata::new("table", "Rendered markdown table content.", true)]);
        base.set_mcp_app_tools(vec![]);
        let kind = uimd::new_label("kind", "markdown table");
        base.add_element(kind.clone());
        kind.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        kind.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let table = uimd::new_message_table("table");
        base.add_element(table.clone());
        table.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        Self
        {
            base,
            kind,
            table,
        }
    }

    pub fn run<H: TableBlockUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TableBlockUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TableBlockUI
{
    fn default() -> Self { Self::new() }
}

pub trait TableBlockUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut TableBlockUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TableBlockUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TableBlockUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TableBlockUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TableBlockUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TableBlockUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut TableBlockUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TableBlockUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TableBlockUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TableBlockUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TableBlockUIRuntime<'a, H: TableBlockUIEvents>
{
    ui: &'a mut TableBlockUI,
    handler: &'a mut H,
}

impl<H: TableBlockUIEvents> uimd::GeneratedApplication for TableBlockUIRuntime<'_, H>
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

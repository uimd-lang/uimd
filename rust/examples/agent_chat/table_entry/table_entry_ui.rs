// Auto-generated UI code for table_entry - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TableEntryUI
{
    pub base: uimd::GeneratedWindow,
    pub heading: uimd::SpanLabelRef,
    pub table: uimd::MessageTableRef,
}

impl TableEntryUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Agent Chat Table Entry");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "spanlabel".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 5,
            cell_name: "table_panel".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 70, chars_height: 1,
            margin_right: 2, margin_bottom: 4,
            content: "heading".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
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
            element_type: "messagetable".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 5,
            cell_name: "table_panel".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 71, chars_height: 4,
            margin_right: 1, margin_bottom: 0,
            content: "table".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
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
        base.set_generated_named_style("table_panel", uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
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
        base.set_generated_named_style("table", uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TableEntry", "# Agent Chat Table Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Heron-style simulated agent Markdown table block.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nheading:\n  type: spanlabel\n  text: \"Table\"\n  description: \"Table title.\"\n\ntable:\n  type: messagetable\n  text: \"\"\n  description: \"Rendered Markdown table content.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@table_panel:\n  background: \"#00000000\"\n  padding: 0, 1, 0, 1\n@heading:\n  color: \"#ddd6fe\"\n@table:\n  color: \"#cbd5e1\"\n```\n\n## User Interface\n\n```ui\n+-table_panel-**----------------------------------------------------------+\n| heading..**........................................................... .|\n| table..**.............................................................. |\n| ....................................................................... |\n# #...................................................................... |\n# #...................................................................... |\n+-------------------------------------------------------------------------+\n```\n", "Heron-style simulated agent Markdown table block.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("heading", "Table title.", true), uimd::GeneratedElementMetadata::new("table", "Rendered Markdown table content.", true)]);
        base.set_mcp_app_tools(vec![]);
        let heading = uimd::new_span_label("heading", "Table");
        base.add_element(heading.clone());
        heading.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#ddd6fe")),
        ..Default::default()
    });
        let table = uimd::new_message_table("table");
        base.add_element(table.clone());
        table.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        Self
        {
            base,
            heading,
            table,
        }
    }

    pub fn run<H: TableEntryUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TableEntryUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TableEntryUI
{
    fn default() -> Self { Self::new() }
}

pub trait TableEntryUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut TableEntryUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TableEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TableEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TableEntryUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TableEntryUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TableEntryUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut TableEntryUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut TableEntryUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TableEntryUI, _key: &str) -> bool { false }
    fn tick(&mut self, _ui: &mut TableEntryUI) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TableEntryUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TableEntryUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TableEntryUIRuntime<'a, H: TableEntryUIEvents>
{
    ui: &'a mut TableEntryUI,
    handler: &'a mut H,
}

impl<H: TableEntryUIEvents> uimd::GeneratedApplication for TableEntryUIRuntime<'_, H>
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

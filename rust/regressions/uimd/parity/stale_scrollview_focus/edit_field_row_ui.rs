// Auto-generated UI code for edit_field_row - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct EditFieldRowUI
{
    pub base: uimd::GeneratedWindow,
    pub field_input: uimd::TextInputRef,
    pub field_btn: uimd::ButtonRef,
}

impl EditFieldRowUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Edit Field Row");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 3,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 88, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "field_input".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 3,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 19, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 19, chars_height: 1,
            margin_right: 70, margin_bottom: 0,
            content: "field_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("component");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#162033")),
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
        base.set_generated_named_style("row", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("field_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "EditFieldRow", "# Edit Field Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"One editable field row with an action button inside the edit ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfield_input:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Editable regression field.\"\n\nfield_btn:\n  type: button\n  title: Action\n  description: \"Action button beside the editable field.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@row:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@field_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------------------------------------------+\n| field_input..**......................................................................... |\n|                                                                                          |\n| field_btn.......... .................................................................... |\n+------------------------------------------------------------------------------------------+\n```\n", "One editable field row with an action button inside the edit ScrollView.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("field_input", "Editable regression field.", true), uimd::GeneratedElementMetadata::new("field_btn", "Action button beside the editable field.", true)]);
        base.set_mcp_app_tools(vec![]);
        let field_input = uimd::new_text_input("field_input", "", 120);
        base.add_element(field_input.clone());
        field_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        field_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        field_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        field_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        field_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        field_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let field_btn = uimd::new_button("field_btn", "Action");
        base.add_element(field_btn.clone());
        field_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        field_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        field_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            field_input,
            field_btn,
        }
    }

    pub fn run<H: EditFieldRowUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = EditFieldRowUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for EditFieldRowUI
{
    fn default() -> Self { Self::new() }
}

pub trait EditFieldRowUIEvents
{
    fn on_field_btn_click(&mut self, _ui: &mut EditFieldRowUI) {}
    fn on_field_input_change(&mut self, _ui: &mut EditFieldRowUI, _value: &str) {}
    fn on_field_input_submit(&mut self, _ui: &mut EditFieldRowUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut EditFieldRowUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut EditFieldRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut EditFieldRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut EditFieldRowUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut EditFieldRowUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut EditFieldRowUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut EditFieldRowUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut EditFieldRowUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut EditFieldRowUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut EditFieldRowUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct EditFieldRowUIRuntime<'a, H: EditFieldRowUIEvents>
{
    ui: &'a mut EditFieldRowUI,
    handler: &'a mut H,
}

impl<H: EditFieldRowUIEvents> uimd::GeneratedApplication for EditFieldRowUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "field_btn" { self.handler.on_field_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "field_input" { self.handler.on_field_input_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "field_input" { self.handler.on_field_input_submit(self.ui, value); return true; }
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

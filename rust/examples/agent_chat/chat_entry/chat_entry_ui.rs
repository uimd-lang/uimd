// Auto-generated UI code for chat_entry - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ChatEntryUI
{
    pub base: uimd::GeneratedWindow,
    pub role: uimd::LabelRef,
    pub phase: uimd::LabelRef,
    pub body: uimd::SpanLabelRef,
    pub detail: uimd::LabelRef,
}

impl ChatEntryUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Agent Chat Entry");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "spanlabel".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 6,
            cell_name: "body_panel".to_string(), cell_width: 0, cell_height: 6,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 6,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 71, chars_height: 6,
            margin_right: 1, margin_bottom: 0,
            content: "body".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 9, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 1,
            cell_name: "footer".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 71, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "detail".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 48, chars_height: 1,
            margin_right: 24, margin_bottom: 0,
            content: "role".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#162033")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#a7f3d0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 50,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 73, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 22, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "phase".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#162033")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
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
        base.set_generated_named_style("header", uimd::Style {
        background: Some(uimd::Color::new("#162033")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("role", uimd::Style {
        color: Some(uimd::Color::new("#a7f3d0")),
        ..Default::default()
    });
        base.set_generated_named_style("phase", uimd::Style {
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("body_panel", uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("body", uimd::Style {
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("footer", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("detail", uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ChatEntry", "# Agent Chat Entry\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Fixed-height simulated agent chat entry for prose and structured transcript blocks.\"\ntags: [component, simulation]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nrole:\n  type: label\n  text: Agent\n  description: \"Simulated transcript speaker or activity type.\"\n\nphase:\n  type: label\n  text: response\n  description: \"Deterministic scenario phase.\"\n\nbody:\n  type: spanlabel\n  text: \"\"\n  description: \"Selectable simulated transcript content.\"\n\ndetail:\n  type: label\n  text: \"simulated · no external action\"\n  description: \"Simulation-only activity detail.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#00000000\"\n@header:\n  background: \"#162033\"\n  padding: 0, 1, 0, 1\n@role:\n  color: \"#a7f3d0\"\n@phase:\n  color: \"#c4b5fd\"\n  text-align: right\n@body_panel:\n  background: \"#0b1220\"\n  padding: 1, 1, 1, 1\n@body:\n  color: \"#d1d5db\"\n  word-wrap: false\n  user-select: text\n@footer:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@detail:\n  color: \"#64748b\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-header-**---------------------------------------------------------------+\n| role..**........................................ phase................. |\n+-body_panel-**-----------------------------------------------------------+\n| body..**............................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n| ....................................................................... |\n+-footer-**---------------------------------------------------------------+\n| detail..**............................................................. |\n+-------------------------------------------------------------------------+\n```\n", "Fixed-height simulated agent chat entry for prose and structured transcript blocks.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("role", "Simulated transcript speaker or activity type.", true), uimd::GeneratedElementMetadata::new("phase", "Deterministic scenario phase.", true), uimd::GeneratedElementMetadata::new("body", "Selectable simulated transcript content.", true), uimd::GeneratedElementMetadata::new("detail", "Simulation-only activity detail.", true)]);
        base.set_mcp_app_tools(vec![]);
        let role = uimd::new_label("role", "Agent");
        base.add_element(role.clone());
        role.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#a7f3d0")),
        ..Default::default()
    });
        role.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let phase = uimd::new_label("phase", "response");
        base.add_element(phase.clone());
        phase.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        phase.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let body = uimd::new_span_label("body", "");
        base.add_element(body.clone());
        body.borrow_mut().set_style(uimd::Style {
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        let detail = uimd::new_label("detail", "simulated · no external action");
        base.add_element(detail.clone());
        detail.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        detail.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            role,
            phase,
            body,
            detail,
        }
    }

    pub fn run<H: ChatEntryUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ChatEntryUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ChatEntryUI
{
    fn default() -> Self { Self::new() }
}

pub trait ChatEntryUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut ChatEntryUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ChatEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ChatEntryUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ChatEntryUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ChatEntryUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ChatEntryUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ChatEntryUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ChatEntryUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ChatEntryUI, _key: &str) -> bool { false }
    fn tick(&mut self, _ui: &mut ChatEntryUI) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ChatEntryUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ChatEntryUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ChatEntryUIRuntime<'a, H: ChatEntryUIEvents>
{
    ui: &'a mut ChatEntryUI,
    handler: &'a mut H,
}

impl<H: ChatEntryUIEvents> uimd::GeneratedApplication for ChatEntryUIRuntime<'_, H>
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

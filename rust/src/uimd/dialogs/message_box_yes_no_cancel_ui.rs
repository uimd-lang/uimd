// Auto-generated UI code for message_box_yes_no_cancel - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct MessageBoxYesNoCancelUI
{
    pub base: uimd::GeneratedWindow,
    pub dialog_header: uimd::LabelRef,
    pub message: uimd::LabelRef,
    pub yes_btn: uimd::ButtonRef,
    pub no_btn: uimd::ButtonRef,
    pub cancel_btn: uimd::ButtonRef,
}

impl MessageBoxYesNoCancelUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Message Box");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 46, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 46, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 46, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "dialog_header".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 46, cell_chars_height: 6,
            cell_name: "".to_string(), cell_width: 46, cell_height: 6,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 42, chars_height: 2,
            margin_right: 2, margin_bottom: 3,
            content: "message".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 46, cell_chars_height: 6,
            cell_name: "".to_string(), cell_width: 46, cell_height: 6,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 32, margin_bottom: 1,
            content: "yes_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 46, cell_chars_height: 6,
            cell_name: "".to_string(), cell_width: 46, cell_height: 6,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 17, margin_bottom: 1,
            content: "no_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 31,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 46, cell_chars_height: 6,
            cell_name: "".to_string(), cell_width: 46, cell_height: 6,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "cancel_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("dialog");
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
        base.set_mcp_metadata(true, "MessageBoxYesNoCancel", "# Message Box\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: \"Message Box UI source.\"\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: Message\n\nmessage:\n  type: label\n  text: \"\"\n\nyes_btn:\n  type: button\n  title: \"Yes\"\n\nno_btn:\n  type: button\n  title: \"No\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------+\n|dialog_header.................................|\n+----------------------------------------------+\n|                                              |\n|  message...................................  |\n|  ..........................................  |\n|                                              |\n|  yes_btn.....  no_btn.......  cancel_btn...  |\n|                                              |\n+----------------------------------------------+\n```\n", "Message Box UI source.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("dialog_header", "Message", true), uimd::GeneratedElementMetadata::new("message", "message", true), uimd::GeneratedElementMetadata::new("yes_btn", "Yes", true), uimd::GeneratedElementMetadata::new("no_btn", "No", true), uimd::GeneratedElementMetadata::new("cancel_btn", "Cancel", true)]);
        base.set_mcp_app_tools(vec![]);
        let dialog_header = uimd::new_label("dialog_header", "Message");
        base.add_element(dialog_header.clone());
        dialog_header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        dialog_header.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let message = uimd::new_label("message", "");
        base.add_element(message.clone());
        message.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        message.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let yes_btn = uimd::new_button("yes_btn", "Yes");
        base.add_element(yes_btn.clone());
        yes_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        yes_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        yes_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let no_btn = uimd::new_button("no_btn", "No");
        base.add_element(no_btn.clone());
        no_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        no_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        no_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let cancel_btn = uimd::new_button("cancel_btn", "Cancel");
        base.add_element(cancel_btn.clone());
        cancel_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        cancel_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        cancel_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            dialog_header,
            message,
            yes_btn,
            no_btn,
            cancel_btn,
        }
    }

    pub fn run<H: MessageBoxYesNoCancelUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = MessageBoxYesNoCancelUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for MessageBoxYesNoCancelUI
{
    fn default() -> Self { Self::new() }
}

pub trait MessageBoxYesNoCancelUIEvents
{
    fn on_yes_btn_click(&mut self, _ui: &mut MessageBoxYesNoCancelUI) {}
    fn on_no_btn_click(&mut self, _ui: &mut MessageBoxYesNoCancelUI) {}
    fn on_cancel_btn_click(&mut self, _ui: &mut MessageBoxYesNoCancelUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str, _value: &[String]) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut MessageBoxYesNoCancelUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct MessageBoxYesNoCancelUIRuntime<'a, H: MessageBoxYesNoCancelUIEvents>
{
    ui: &'a mut MessageBoxYesNoCancelUI,
    handler: &'a mut H,
}

impl<H: MessageBoxYesNoCancelUIEvents> uimd::GeneratedApplication for MessageBoxYesNoCancelUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "yes_btn" { self.handler.on_yes_btn_click(self.ui); return true; }
        if name == "no_btn" { self.handler.on_no_btn_click(self.ui); return true; }
        if name == "cancel_btn" { self.handler.on_cancel_btn_click(self.ui); return true; }
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

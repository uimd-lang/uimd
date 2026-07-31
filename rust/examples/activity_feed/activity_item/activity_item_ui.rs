// Auto-generated UI code for activity_item - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ActivityItemUI
{
    pub base: uimd::GeneratedWindow,
    pub timestamp: uimd::LabelRef,
    pub event_type: uimd::LabelRef,
    pub message: uimd::LabelRef,
}

impl ActivityItemUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Activity Item");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 10, cell_chars_height: 6,
            cell_name: "marker".to_string(), cell_width: 1, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "fit-content".to_string(),
            width: 1, height: uimd::FIT_CONTENT,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 6,
            margin_right: 0, margin_bottom: 0,
            content: "".to_string(),
            cell_style: uimd::Style {
        background_texture: Some("▐".to_string()),
        background_texture_color: Some(uimd::Color::new("#38bdf8")),
        ..Default::default()
    },
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 11,
            cell_chars_width: 42, cell_chars_height: 6,
            cell_name: "body".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 30, margin_bottom: 5,
            content: "timestamp".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2e46")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#94a3b8")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 13,
            cell_row: 0, cell_col: 11,
            cell_chars_width: 42, cell_chars_height: 6,
            cell_name: "body".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 15, margin_bottom: 5,
            content: "event_type".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2e46")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 11,
            cell_chars_width: 42, cell_chars_height: 6,
            cell_name: "body".to_string(), cell_width: 0, cell_height: uimd::FIT_CONTENT,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "fit-content".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 11, chars_height: 5,
            margin_right: 30, margin_bottom: 0,
            content: "message".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2e46")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        user_select: Some("text".to_string()),
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
        base.set_generated_named_style("marker", uimd::Style {
        background_texture: Some("▐".to_string()),
        background_texture_color: Some(uimd::Color::new("#38bdf8")),
        ..Default::default()
    });
        base.set_generated_named_style("marker_info", uimd::Style {
        background_texture_color: Some(uimd::Color::new("#38bdf8")),
        ..Default::default()
    });
        base.set_generated_named_style("marker_task", uimd::Style {
        background_texture_color: Some(uimd::Color::new("#a78bfa")),
        ..Default::default()
    });
        base.set_generated_named_style("marker_warning", uimd::Style {
        background_texture_color: Some(uimd::Color::new("#facc15")),
        ..Default::default()
    });
        base.set_generated_named_style("marker_deploy", uimd::Style {
        background_texture_color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        base.set_generated_named_style("marker_note", uimd::Style {
        background_texture_color: Some(uimd::Color::new("#fb7185")),
        ..Default::default()
    });
        base.set_generated_named_style("body", uimd::Style {
        background: Some(uimd::Color::new("#1f2e46")),
        ..Default::default()
    });
        base.set_generated_named_style("timestamp", uimd::Style {
        color: Some(uimd::Color::new("#94a3b8")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("event_type", uimd::Style {
        color: Some(uimd::Color::new("#bfdbfe")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("message", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ActivityItem", "# Activity Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable activity feed item with timestamp, event type, and message text.\"\ntags: [component]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntimestamp:\n  type: label\n  text: \"10:42\"\n  description: \"Optional activity timestamp.\"\n\nevent_type:\n  type: label\n  text: Info\n  description: \"Activity category label.\"\n\nmessage:\n  type: label\n  text: \"Activity message\"\n  description: \"Selectable activity message body.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@marker:\n  background-texture: \"▐\"\n  background-texture-color: \"#38bdf8\"\n@marker_info:\n  background-texture-color: \"#38bdf8\"\n@marker_task:\n  background-texture-color: \"#a78bfa\"\n@marker_warning:\n  background-texture-color: \"#facc15\"\n@marker_deploy:\n  background-texture-color: \"#22c55e\"\n@marker_note:\n  background-texture-color: \"#fb7185\"\n@body:\n  background: \"#1f2e46\"\n@timestamp:\n  color: \"#94a3b8\"\n  user-select: text\n@event_type:\n  color: \"#bfdbfe\"\n  user-select: text\n@message:\n  color: \"#ffffff\"\n  user-select: text\n```\n\n## User Interface\n\n```ui\n+-marker-1-+-body-**----------------------------------+\n|          | timestamp.. event_type..**               |\n|          | message..**                              |\n|          | ........................................ |\n|          | ........................................ |\n#          # #                                        |\n#          # #                                        |\n+----------+------------------------------------------+\n```\n", "Reusable activity feed item with timestamp, event type, and message text.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("timestamp", "Optional activity timestamp.", true), uimd::GeneratedElementMetadata::new("event_type", "Activity category label.", true), uimd::GeneratedElementMetadata::new("message", "Selectable activity message body.", true)]);
        base.set_mcp_app_tools(vec![]);
        let timestamp = uimd::new_label("timestamp", "10:42");
        base.add_element(timestamp.clone());
        timestamp.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#94a3b8")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        timestamp.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let event_type = uimd::new_label("event_type", "Info");
        base.add_element(event_type.clone());
        event_type.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        event_type.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let message = uimd::new_label("message", "Activity message");
        base.add_element(message.clone());
        message.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        message.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            timestamp,
            event_type,
            message,
        }
    }

    pub fn run<H: ActivityItemUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ActivityItemUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ActivityItemUI
{
    fn default() -> Self { Self::new() }
}

pub trait ActivityItemUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut ActivityItemUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ActivityItemUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ActivityItemUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ActivityItemUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ActivityItemUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ActivityItemUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ActivityItemUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ActivityItemUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ActivityItemUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ActivityItemUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ActivityItemUIRuntime<'a, H: ActivityItemUIEvents>
{
    ui: &'a mut ActivityItemUI,
    handler: &'a mut H,
}

impl<H: ActivityItemUIEvents> uimd::GeneratedApplication for ActivityItemUIRuntime<'_, H>
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

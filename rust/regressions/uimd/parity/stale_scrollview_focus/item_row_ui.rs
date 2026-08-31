// Auto-generated UI code for item_row - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ItemRowUI
{
    pub base: uimd::GeneratedWindow,
    pub name: uimd::LabelRef,
    pub summary: uimd::LabelRef,
    pub spacer: uimd::LabelRef,
    pub open_btn: uimd::ButtonRef,
}

impl ItemRowUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Item Row");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 59, chars_height: 1,
            margin_right: 30, margin_bottom: 1,
            content: "name".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 61,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 28, height: 2,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 28, chars_height: 2,
            margin_right: 1, margin_bottom: 0,
            content: "summary".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 74, chars_height: 1,
            margin_right: 15, margin_bottom: 0,
            content: "spacer".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 76,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "open_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
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
        base.set_generated_named_style("card", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("name", uimd::Style {
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("summary", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("open_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ItemRow", "# Item Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Interactive row with an Open button inside a ScrollView.\"\ntags: [regression, issue-5, scrollview]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nname:\n  type: label\n  text: \"Item\"\n  description: \"Item name.\"\n\nsummary:\n  type: label\n  text: \"Summary\"\n  description: \"Item summary.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this item in the edit page.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@name:\n  color: \"#f8fafc\"\n  user-select: text\n@summary:\n  color: \"#93c5fd\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@open_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-card-**----------------------------------------------------------------------------------+\n| name..**................................................... summary..................... |\n| spacer..**................................................................ open_btn..... |\n+------------------------------------------------------------------------------------------+\n```\n", "Interactive row with an Open button inside a ScrollView.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("name", "Item name.", true), uimd::GeneratedElementMetadata::new("summary", "Item summary.", true), uimd::GeneratedElementMetadata::new("spacer", "Flexible row spacer.", false), uimd::GeneratedElementMetadata::new("open_btn", "Open this item in the edit page.", true)]);
        base.set_mcp_app_tools(vec![]);
        let name = uimd::new_label("name", "Item");
        base.add_element(name.clone());
        name.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        name.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let summary = uimd::new_label("summary", "Summary");
        base.add_element(summary.clone());
        summary.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        summary.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let spacer = uimd::new_label("spacer", "");
        base.add_element(spacer.clone());
        spacer.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        spacer.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let open_btn = uimd::new_button("open_btn", "Open");
        base.add_element(open_btn.clone());
        open_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        open_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        open_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            name,
            summary,
            spacer,
            open_btn,
        }
    }

    pub fn run<H: ItemRowUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ItemRowUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ItemRowUI
{
    fn default() -> Self { Self::new() }
}

pub trait ItemRowUIEvents
{
    fn on_open_btn_click(&mut self, _ui: &mut ItemRowUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ItemRowUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ItemRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ItemRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ItemRowUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ItemRowUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ItemRowUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ItemRowUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ItemRowUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ItemRowUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ItemRowUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ItemRowUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ItemRowUIRuntime<'a, H: ItemRowUIEvents>
{
    ui: &'a mut ItemRowUI,
    handler: &'a mut H,
}

impl<H: ItemRowUIEvents> uimd::GeneratedApplication for ItemRowUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "open_btn" { self.handler.on_open_btn_click(self.ui); return true; }
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

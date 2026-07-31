// Auto-generated UI code for settings - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct SettingsUI
{
    pub base: uimd::GeneratedWindow,
    pub header_title: uimd::LabelRef,
    pub auto_scroll: uimd::CheckBoxRef,
    pub show_timestamps: uimd::CheckBoxRef,
    pub default_type_label: uimd::LabelRef,
    pub default_type: uimd::ComboBoxRef,
    pub save_button: uimd::ButtonRef,
    pub cancel_button: uimd::ButtonRef,
}

impl SettingsUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Activity Feed Settings");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 42, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 42, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "header_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 2,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 1,
            cell_name: "buttons".to_string(), cell_width: 42, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 17, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 23, margin_bottom: 0,
            content: "save_button".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 21,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 1,
            cell_name: "buttons".to_string(), cell_width: 42, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 19, chars_height: 1,
            margin_right: 2, margin_bottom: 0,
            content: "cancel_button".to_string(),
            cell_style: uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 1, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 7,
            cell_name: "content".to_string(), cell_width: 42, cell_height: 7,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 38, chars_height: 1,
            margin_right: 2, margin_bottom: 5,
            content: "auto_scroll".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 3, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 7,
            cell_name: "content".to_string(), cell_width: 42, cell_height: 7,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 38, chars_height: 1,
            margin_right: 2, margin_bottom: 3,
            content: "show_timestamps".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 5, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 7,
            cell_name: "content".to_string(), cell_width: 42, cell_height: 7,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 20, margin_bottom: 1,
            content: "default_type_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 5, col: 24,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 42, cell_chars_height: 7,
            cell_name: "content".to_string(), cell_width: 42, cell_height: 7,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 16, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "default_type".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
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
        base.set_generated_named_style("header", uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        ..Default::default()
    });
        base.set_generated_named_style("header_title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("buttons", uimd::Style {
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Settings", "# Activity Feed Settings\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Settings dialog for activity feed auto-scroll, timestamps, and default activity type.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader_title:\n  type: label\n  text: \"Activity Feed Settings\"\n  description: \"Dialog title.\"\n  expose: false\n\nauto_scroll:\n  type: checkbox\n  title: \"Auto-scroll to newest\"\n  value: 1\n  description: \"Toggle whether new activity items scroll into view.\"\n\nshow_timestamps:\n  type: checkbox\n  title: \"Show timestamps\"\n  value: 1\n  description: \"Toggle whether newly added activity items include timestamps.\"\n\ndefault_type_label:\n  type: label\n  text: \"Default type\"\n  description: \"Label for the default activity type selector.\"\n  expose: false\n\ndefault_type:\n  type: combobox\n  options: [Info, Task, Warning, Deploy, Note]\n  selected_item: Info\n  description: \"Default activity type used by the main window.\"\n\nsave_button:\n  type: button\n  title: Save\n  description: \"Apply settings and close the dialog.\"\n\ncancel_button:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without applying settings.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@header_title:\n  text-align: center\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\ncheckbox:\n  color: \"#d1d5db\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#f97316\"\n@buttons:\n  padding: 0, 2, 1, 2\n```\n\n## User Interface\n\n```ui\n+-header-----------------------------------+\n|header_title..**..........................|\n+-content----------------------------------+\n|                                          |\n|  auto_scroll...........................  |\n|                                          |\n|  show_timestamps.......................  |\n|                                          |\n|  default_type_label..  default_type....  |\n|                                          |\n+-buttons----------------------------------+\n|  save_button......  cancel_button......  |\n+------------------------------------------+\n```\n", "Settings dialog for activity feed auto-scroll, timestamps, and default activity type.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header_title", "Dialog title.", false), uimd::GeneratedElementMetadata::new("auto_scroll", "Toggle whether new activity items scroll into view.", true), uimd::GeneratedElementMetadata::new("show_timestamps", "Toggle whether newly added activity items include timestamps.", true), uimd::GeneratedElementMetadata::new("default_type_label", "Label for the default activity type selector.", false), uimd::GeneratedElementMetadata::new("default_type", "Default activity type used by the main window.", true), uimd::GeneratedElementMetadata::new("save_button", "Apply settings and close the dialog.", true), uimd::GeneratedElementMetadata::new("cancel_button", "Close the dialog without applying settings.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header_title = uimd::new_label("header_title", "Activity Feed Settings");
        base.add_element(header_title.clone());
        header_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        header_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let auto_scroll = uimd::new_checkbox("auto_scroll", "Auto-scroll to newest", true);
        base.add_element(auto_scroll.clone());
        auto_scroll.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        auto_scroll.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        auto_scroll.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        auto_scroll.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f97316")),
        ..Default::default()
    });
        auto_scroll.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let show_timestamps = uimd::new_checkbox("show_timestamps", "Show timestamps", true);
        base.add_element(show_timestamps.clone());
        show_timestamps.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        show_timestamps.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        show_timestamps.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        show_timestamps.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f97316")),
        ..Default::default()
    });
        show_timestamps.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let default_type_label = uimd::new_label("default_type_label", "Default type");
        base.add_element(default_type_label.clone());
        default_type_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        default_type_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let default_type = uimd::new_combo_box("default_type", vec!["Info".to_string(), "Task".to_string(), "Warning".to_string(), "Deploy".to_string(), "Note".to_string()]);
        base.add_element(default_type.clone());
        default_type.borrow_mut().set_selected_index(0);
        default_type.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        default_type.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        default_type.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        default_type.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        default_type.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let save_button = uimd::new_button("save_button", "Save");
        base.add_element(save_button.clone());
        save_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        save_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        save_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let cancel_button = uimd::new_button("cancel_button", "Cancel");
        base.add_element(cancel_button.clone());
        cancel_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        cancel_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        cancel_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            header_title,
            auto_scroll,
            show_timestamps,
            default_type_label,
            default_type,
            save_button,
            cancel_button,
        }
    }

    pub fn run<H: SettingsUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = SettingsUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for SettingsUI
{
    fn default() -> Self { Self::new() }
}

pub trait SettingsUIEvents
{
    fn on_save_button_click(&mut self, _ui: &mut SettingsUI) {}
    fn on_cancel_button_click(&mut self, _ui: &mut SettingsUI) {}
    fn on_auto_scroll_change(&mut self, _ui: &mut SettingsUI, _value: &str) {}
    fn on_show_timestamps_change(&mut self, _ui: &mut SettingsUI, _value: &str) {}
    fn on_default_type_change(&mut self, _ui: &mut SettingsUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut SettingsUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut SettingsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut SettingsUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut SettingsUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut SettingsUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut SettingsUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut SettingsUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut SettingsUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut SettingsUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut SettingsUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct SettingsUIRuntime<'a, H: SettingsUIEvents>
{
    ui: &'a mut SettingsUI,
    handler: &'a mut H,
}

impl<H: SettingsUIEvents> uimd::GeneratedApplication for SettingsUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "save_button" { self.handler.on_save_button_click(self.ui); return true; }
        if name == "cancel_button" { self.handler.on_cancel_button_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "auto_scroll" { self.handler.on_auto_scroll_change(self.ui, value); return true; }
        if name == "show_timestamps" { self.handler.on_show_timestamps_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "default_type" { self.handler.on_default_type_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

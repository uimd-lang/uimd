// Auto-generated UI code for settings_view - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct SettingsViewUI
{
    pub base: uimd::GeneratedWindow,
    pub view_title: uimd::LabelRef,
    pub currency: uimd::ComboBoxRef,
    pub show_paid: uimd::CheckBoxRef,
    pub monthly_limit: uimd::NumberInputRef,
    pub reset_btn: uimd::ButtonRef,
}

impl SettingsViewUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Settings View");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "view_title".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fde68a")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 48, margin_bottom: 6,
            content: "\"Currency:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 2, col: 18,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 41, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 41, chars_height: 1,
            margin_right: 1, margin_bottom: 6,
            content: "currency".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 4, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 4,
            content: "show_paid".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 6, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 16, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 16, chars_height: 1,
            margin_right: 43, margin_bottom: 2,
            content: "\"Monthly limit:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "numberinput".to_string(),
            row: 6, col: 18,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 41, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 41, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "monthly_limit".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 8, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 9,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 9,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "reset_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("control");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
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
        base.set_generated_named_style("view_title", uimd::Style {
        color: Some(uimd::Color::new("#fde68a")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "SettingsView", "# Settings View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense tracker settings view.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Settings\n\ncurrency:\n  type: combobox\n  options: [EUR, USD, CZK]\n  selected_item: EUR\n\nshow_paid:\n  type: checkbox\n  title: Show paid expenses\n  value: true\n\nmonthly_limit:\n  type: numberinput\n  value: 500\n  format: \"0\"\n\nreset_btn:\n  type: button\n  title: Reset demo data\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#fde68a\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Currency:\"      currency................................. |\n|                                                            |\n| show_paid................................................. |\n|                                                            |\n| \"Monthly limit:\" monthly_limit............................ |\n|                                                            |\n| reset_btn................................................. |\n+------------------------------------------------------------+\n```\n", "Expense tracker settings view.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("view_title", "Settings", true), uimd::GeneratedElementMetadata::new("currency", "currency", true), uimd::GeneratedElementMetadata::new("show_paid", "Show paid expenses", true), uimd::GeneratedElementMetadata::new("monthly_limit", "monthly_limit", true), uimd::GeneratedElementMetadata::new("reset_btn", "Reset demo data", true)]);
        base.set_mcp_app_tools(vec![]);
        let view_title = uimd::new_label("view_title", "Settings");
        base.add_element(view_title.clone());
        view_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fde68a")),
        ..Default::default()
    });
        view_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let currency = uimd::new_combo_box("currency", vec!["EUR".to_string(), "USD".to_string(), "CZK".to_string()]);
        base.add_element(currency.clone());
        currency.borrow_mut().set_selected_index(0);
        currency.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        currency.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        currency.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        currency.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        currency.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let show_paid = uimd::new_checkbox("show_paid", "Show paid expenses", true);
        base.add_element(show_paid.clone());
        show_paid.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    });
        show_paid.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        show_paid.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#44cc88")),
        ..Default::default()
    });
        show_paid.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        ..Default::default()
    });
        show_paid.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let monthly_limit = uimd::new_number_input_with_step("monthly_limit", 500.000000, 1.000000);
        base.add_element(monthly_limit.clone());
        monthly_limit.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        monthly_limit.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        monthly_limit.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        monthly_limit.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        monthly_limit.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let reset_btn = uimd::new_button("reset_btn", "Reset demo data");
        base.add_element(reset_btn.clone());
        reset_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        reset_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        reset_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            view_title,
            currency,
            show_paid,
            monthly_limit,
            reset_btn,
        }
    }

    pub fn run<H: SettingsViewUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = SettingsViewUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for SettingsViewUI
{
    fn default() -> Self { Self::new() }
}

pub trait SettingsViewUIEvents
{
    fn on_reset_btn_click(&mut self, _ui: &mut SettingsViewUI) {}
    fn on_show_paid_change(&mut self, _ui: &mut SettingsViewUI, _value: &str) {}
    fn on_monthly_limit_change(&mut self, _ui: &mut SettingsViewUI, _value: &str) {}
    fn on_monthly_limit_submit(&mut self, _ui: &mut SettingsViewUI, _value: &str) {}
    fn on_currency_change(&mut self, _ui: &mut SettingsViewUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut SettingsViewUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut SettingsViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut SettingsViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut SettingsViewUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut SettingsViewUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut SettingsViewUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut SettingsViewUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut SettingsViewUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut SettingsViewUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut SettingsViewUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct SettingsViewUIRuntime<'a, H: SettingsViewUIEvents>
{
    ui: &'a mut SettingsViewUI,
    handler: &'a mut H,
}

impl<H: SettingsViewUIEvents> uimd::GeneratedApplication for SettingsViewUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "reset_btn" { self.handler.on_reset_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "show_paid" { self.handler.on_show_paid_change(self.ui, value); return true; }
        if name == "monthly_limit" { self.handler.on_monthly_limit_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "monthly_limit" { self.handler.on_monthly_limit_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "currency" { self.handler.on_currency_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

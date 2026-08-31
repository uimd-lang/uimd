// Auto-generated UI code for expense_row - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ExpenseRowUI
{
    pub base: uimd::GeneratedWindow,
    pub paid: uimd::CheckBoxRef,
    pub item: uimd::LabelRef,
    pub amount: uimd::LabelRef,
    pub category: uimd::ComboBoxRef,
    pub delete_btn: uimd::ButtonRef,
}

impl ExpenseRowUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Expense Row");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 44, margin_bottom: 1,
            content: "paid".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 11,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 29, chars_height: 2,
            margin_right: 14, margin_bottom: 0,
            content: "item".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 41,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "amount".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 14, margin_bottom: 0,
            content: "category".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 41,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "row".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "delete_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
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
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("item", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        base.set_generated_named_style("amount", uimd::Style {
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        base.set_generated_named_style("delete_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ExpenseRow", "# Expense Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: experimental\ndescription: \"Reusable expense row with paid state, category selector, and delete action.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\npaid:\n  type: checkbox\n  title: Paid\n  value: false\n\nitem:\n  type: label\n  text: Item\n  user-select: text\n\namount:\n  type: label\n  text: \"0.00\"\n\ncategory:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ndelete_btn:\n  type: button\n  title: Delete\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@row:\n  background: \"#172033\"\n  padding: 1\n@item:\n  color: \"#ffffff\"\n@amount:\n  color: \"#bfdbfe\"\ncheckbox:\n  color: \"#cbd5e1\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-row-**-----------------------------------------------+\n| paid..... item..**..................... amount...... |\n| category..**........................... delete_btn.. |\n+------------------------------------------------------+\n```\n", "Reusable expense row with paid state, category selector, and delete action.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("paid", "Paid", true), uimd::GeneratedElementMetadata::new("item", "Item", true), uimd::GeneratedElementMetadata::new("amount", "0.00", true), uimd::GeneratedElementMetadata::new("category", "category", true), uimd::GeneratedElementMetadata::new("delete_btn", "Delete", true)]);
        base.set_mcp_app_tools(vec![]);
        let paid = uimd::new_checkbox("paid", "Paid", false);
        base.add_element(paid.clone());
        paid.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        paid.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        paid.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        paid.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#94a3b8")),
        ..Default::default()
    });
        paid.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let item = uimd::new_label("item", "Item");
        base.add_element(item.clone());
        item.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        item.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let amount = uimd::new_label("amount", "0.00");
        base.add_element(amount.clone());
        amount.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        amount.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let category = uimd::new_combo_box("category", vec!["Food".to_string(), "Transport".to_string(), "Home".to_string(), "Health".to_string(), "Other".to_string()]);
        base.add_element(category.clone());
        category.borrow_mut().set_selected_index(0);
        category.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let delete_btn = uimd::new_button("delete_btn", "Delete");
        base.add_element(delete_btn.clone());
        delete_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        delete_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        delete_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            paid,
            item,
            amount,
            category,
            delete_btn,
        }
    }

    pub fn run<H: ExpenseRowUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ExpenseRowUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ExpenseRowUI
{
    fn default() -> Self { Self::new() }
}

pub trait ExpenseRowUIEvents
{
    fn on_delete_btn_click(&mut self, _ui: &mut ExpenseRowUI) {}
    fn on_paid_change(&mut self, _ui: &mut ExpenseRowUI, _value: &str) {}
    fn on_category_change(&mut self, _ui: &mut ExpenseRowUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ExpenseRowUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ExpenseRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ExpenseRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ExpenseRowUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ExpenseRowUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ExpenseRowUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ExpenseRowUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ExpenseRowUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ExpenseRowUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ExpenseRowUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ExpenseRowUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ExpenseRowUIRuntime<'a, H: ExpenseRowUIEvents>
{
    ui: &'a mut ExpenseRowUI,
    handler: &'a mut H,
}

impl<H: ExpenseRowUIEvents> uimd::GeneratedApplication for ExpenseRowUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "delete_btn" { self.handler.on_delete_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "paid" { self.handler.on_paid_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "category" { self.handler.on_category_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

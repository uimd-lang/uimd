// Auto-generated UI code for expenses_view - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "expenses_list/expenses_list_ui.rs"]
mod uimd_dependency_expenses_list;

pub struct ExpensesViewUI
{
    pub base: uimd::GeneratedWindow,
    pub view_title: uimd::LabelRef,
    pub item_input: uimd::TextInputRef,
    pub amount_input: uimd::NumberInputRef,
    pub category_input: uimd::ComboBoxRef,
    pub currency_label: uimd::LabelRef,
    pub add_expense_btn: uimd::ButtonRef,
    pub expenses: uimd::ReusableElementRef,
}

impl ExpensesViewUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Expenses View");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 14,
            content: "view_title".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 1,
            margin_right: 52, margin_bottom: 12,
            content: "\"Item:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 2, col: 13,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 46, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 46, chars_height: 1,
            margin_right: 1, margin_bottom: 12,
            content: "item_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 4, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 50, margin_bottom: 10,
            content: "\"Amount:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "numberinput".to_string(),
            row: 4, col: 13,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 25, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 22, margin_bottom: 10,
            content: "amount_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 4, col: 39,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 1, margin_bottom: 10,
            content: "currency_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 6, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 48, margin_bottom: 8,
            content: "\"Category:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 6, col: 13,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 46, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 46, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "category_input".to_string(),
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
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 6,
            content: "add_expense_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "expenses_list".to_string(),
            row: 10, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 15,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 15,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 58, chars_height: 5,
            margin_right: 1, margin_bottom: 0,
            content: "expenses".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
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
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        base.set_generated_named_style("currency_label", uimd::Style {
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        base.set_generated_named_style("expenses", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ExpensesView", "# Expenses View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Expense entry view with a dynamic scrollable list.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - expenses_list/expenses_list\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Expenses\n\nitem_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n\namount_input:\n  type: numberinput\n  value: 0\n  format: \"0.00\"\n\ncategory_input:\n  type: combobox\n  options: [Food, Transport, Home, Health, Other]\n  selected_item: Food\n\ncurrency_label:\n  type: label\n  text: EUR\n\nadd_expense_btn:\n  type: button\n  title: Add expense\n\nexpenses:\n  type: expenses_list\n  cpp-class: expense_tracker_example::ExpensesList\n  cpp-header: \"expenses_list/expenses_list.hpp\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bfdbfe\"\n@currency_label:\n  color: \"#bfdbfe\"\n@expenses:\n  background: \"#172033\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nnumberinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Item:\"     item_input.................................... |\n|                                                            |\n| \"Amount:\"   amount_input............. currency_label...... |\n|                                                            |\n| \"Category:\" category_input................................ |\n|                                                            |\n| add_expense_btn........................................... |\n|                                                            |\n| expenses..**.............................................. |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", "Expense entry view with a dynamic scrollable list.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("view_title", "Expenses", true), uimd::GeneratedElementMetadata::new("item_input", "item_input", true), uimd::GeneratedElementMetadata::new("amount_input", "amount_input", true), uimd::GeneratedElementMetadata::new("category_input", "category_input", true), uimd::GeneratedElementMetadata::new("currency_label", "EUR", true), uimd::GeneratedElementMetadata::new("add_expense_btn", "Add expense", true), uimd::GeneratedElementMetadata::new("expenses", "expenses", true)]);
        base.set_mcp_app_tools(vec![]);
        let view_title = uimd::new_label("view_title", "Expenses");
        base.add_element(view_title.clone());
        view_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        view_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let item_input = uimd::new_text_input("item_input", "", 80);
        base.add_element(item_input.clone());
        item_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        item_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        item_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        item_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        item_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        item_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let amount_input = uimd::new_number_input_with_step("amount_input", 0.000000, 1.000000);
        base.add_element(amount_input.clone());
        amount_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        amount_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        amount_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        amount_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        amount_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let category_input = uimd::new_combo_box("category_input", vec!["Food".to_string(), "Transport".to_string(), "Home".to_string(), "Health".to_string(), "Other".to_string()]);
        base.add_element(category_input.clone());
        category_input.borrow_mut().set_selected_index(0);
        category_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let currency_label = uimd::new_label("currency_label", "EUR");
        base.add_element(currency_label.clone());
        currency_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        currency_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let add_expense_btn = uimd::new_button("add_expense_btn", "Add expense");
        base.add_element(add_expense_btn.clone());
        add_expense_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        add_expense_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        add_expense_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let expenses = uimd::new_reusable_element("expenses", "expenses_list");
        base.add_element(expenses.clone());
        expenses.borrow_mut().set_child_window(uimd_dependency_expenses_list::ExpensesListUI::new().base);
        expenses.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    });
        expenses.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        expenses.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        expenses.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        Self
        {
            base,
            view_title,
            item_input,
            amount_input,
            category_input,
            currency_label,
            add_expense_btn,
            expenses,
        }
    }

    pub fn run<H: ExpensesViewUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ExpensesViewUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ExpensesViewUI
{
    fn default() -> Self { Self::new() }
}

pub trait ExpensesViewUIEvents
{
    fn on_add_expense_btn_click(&mut self, _ui: &mut ExpensesViewUI) {}
    fn on_item_input_change(&mut self, _ui: &mut ExpensesViewUI, _value: &str) {}
    fn on_amount_input_change(&mut self, _ui: &mut ExpensesViewUI, _value: &str) {}
    fn on_item_input_submit(&mut self, _ui: &mut ExpensesViewUI, _value: &str) {}
    fn on_amount_input_submit(&mut self, _ui: &mut ExpensesViewUI, _value: &str) {}
    fn on_category_input_change(&mut self, _ui: &mut ExpensesViewUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ExpensesViewUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ExpensesViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ExpensesViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ExpensesViewUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ExpensesViewUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ExpensesViewUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ExpensesViewUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ExpensesViewUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ExpensesViewUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ExpensesViewUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ExpensesViewUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ExpensesViewUIRuntime<'a, H: ExpensesViewUIEvents>
{
    ui: &'a mut ExpensesViewUI,
    handler: &'a mut H,
}

impl<H: ExpensesViewUIEvents> uimd::GeneratedApplication for ExpensesViewUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "add_expense_btn" { self.handler.on_add_expense_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "item_input" { self.handler.on_item_input_change(self.ui, value); return true; }
        if name == "amount_input" { self.handler.on_amount_input_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "item_input" { self.handler.on_item_input_submit(self.ui, value); return true; }
        if name == "amount_input" { self.handler.on_amount_input_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "category_input" { self.handler.on_category_input_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

mod categories_view_ui;
mod expense_tracker_ui;
mod expenses_view_ui;
mod settings_view_ui;
#[path = "expense_row/expense_row_ui.rs"]
mod expense_row_ui;
#[path = "expenses_list/expenses_list_ui.rs"]
mod expenses_list_ui;

use categories_view_ui::CategoriesViewUI;
use expense_row_ui::ExpenseRowUI;
use expense_tracker_ui::*;
use expenses_list_ui::ExpensesListUI;
use expenses_view_ui::ExpensesViewUI;
use settings_view_ui::SettingsViewUI;

const DEFAULT_CATEGORIES: [&str; 5] = ["Food", "Transport", "Home", "Health", "Other"];
const DEFAULT_CURRENCY: &str = "EUR";
const DEFAULT_MONTHLY_LIMIT: f64 = 500.0;
const FIRST_NEW_EXPENSE_NUMBER: u32 = 13;

#[derive(Clone, Debug)]
struct Expense
{
    id: String,
    item: String,
    amount: f64,
    category: String,
    paid: bool,
}

struct ExpensesRefs
{
    item: uimd::ElementRef,
    amount: uimd::ElementRef,
    category: uimd::ElementRef,
    currency: uimd::ElementRef,
    list: uimd::ElementRef,
}

struct CategoriesRefs
{
    name: uimd::ElementRef,
    categories: uimd::ElementRef,
}

struct SettingsRefs
{
    currency: uimd::ElementRef,
    show_paid: uimd::ElementRef,
    monthly_limit: uimd::ElementRef,
}

enum CurrentView
{
    Expenses(ExpensesRefs),
    Categories(CategoriesRefs),
    Settings(SettingsRefs),
}

struct ExpenseTrackerApp
{
    categories: Vec<String>,
    expenses: Vec<Expense>,
    currency: String,
    show_paid: bool,
    monthly_limit: f64,
    next_expense_number: u32,
    pending_delete_id: Option<String>,
    delete_dialog: Option<uimd::MessageBoxYesNo>,
    current_view: Option<CurrentView>,
    normal_nav_style: Option<uimd::Style>,
}

impl Default for ExpenseTrackerApp
{
    fn default() -> Self
    {
        Self
        {
            categories: DEFAULT_CATEGORIES.into_iter().map(str::to_string).collect(),
            expenses: seed_expenses(),
            currency: DEFAULT_CURRENCY.to_string(),
            show_paid: true,
            monthly_limit: DEFAULT_MONTHLY_LIMIT,
            next_expense_number: FIRST_NEW_EXPENSE_NUMBER,
            pending_delete_id: None,
            delete_dialog: None,
            current_view: None,
            normal_nav_style: None,
        }
    }
}

impl ExpenseTrackerApp
{
    fn initialize(&mut self, ui: &mut ExpenseTrackerUI)
    {
        self.normal_nav_style = Some(ui.expenses_btn.borrow().style.clone());
        ui.base.set_runtime_options(uimd::GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "expenses_btn".to_string(),
            ..Default::default()
        });
        self.show_expenses(ui);
    }

    fn visible_expenses(&self) -> Vec<Expense>
    {
        self.expenses
            .iter()
            .filter(|expense| self.show_paid || !expense.paid)
            .cloned()
            .collect()
    }

    fn show_expenses(&mut self, ui: &mut ExpenseTrackerUI)
    {
        let view = ExpensesViewUI::new();
        view.currency_label.borrow_mut().set_text(&self.currency);
        view.category_input
            .borrow_mut()
            .set_options(self.categories.clone());
        let list = ExpensesListUI::new();
        view.expenses.borrow_mut().set_child_window(list.base);
        let refs = ExpensesRefs
        {
            item: view.item_input.clone().into(),
            amount: view.amount_input.clone().into(),
            category: view.category_input.clone().into(),
            currency: view.currency_label.clone().into(),
            list: view.expenses.clone().into(),
        };
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::Expenses(refs));
        self.select_nav(ui, "expenses_btn");
        self.refresh_expenses();
    }

    fn show_categories(&mut self, ui: &mut ExpenseTrackerUI)
    {
        let view = CategoriesViewUI::new();
        view.categories.borrow_mut().set_options(self.categories.clone());
        if !self.categories.is_empty()
        {
            view.categories.borrow_mut().set_selected_index(0);
        }
        let refs = CategoriesRefs
        {
            name: view.category_name.clone().into(),
            categories: view.categories.clone().into(),
        };
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::Categories(refs));
        self.select_nav(ui, "categories_btn");
    }

    fn show_settings(&mut self, ui: &mut ExpenseTrackerUI)
    {
        let view = SettingsViewUI::new();
        select_value(&view.currency, &self.currency);
        view.show_paid.borrow_mut().set_checked(self.show_paid);
        view.monthly_limit
            .borrow_mut()
            .set_number_value(self.monthly_limit);
        let refs = SettingsRefs
        {
            currency: view.currency.clone().into(),
            show_paid: view.show_paid.clone().into(),
            monthly_limit: view.monthly_limit.clone().into(),
        };
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::Settings(refs));
        self.select_nav(ui, "settings_btn");
    }

    fn select_nav(&self, ui: &ExpenseTrackerUI, selected: &str)
    {
        let Some(normal) = &self.normal_nav_style else { return };
        for button in [&ui.expenses_btn, &ui.categories_btn, &ui.settings_btn]
        {
            let mut value = button.borrow_mut();
            value.style = normal.clone();
            if value.name == selected
                && let Some(focus) = value.focus_style.clone()
            {
                value.style.merge(&focus);
            }
        }
    }

    fn refresh_expenses(&self)
    {
        let Some(CurrentView::Expenses(view)) = &self.current_view else { return };
        view.currency.borrow_mut().set_text(&self.currency);
        let previous_category = view.category.borrow().selected_value();
        view.category
            .borrow_mut()
            .set_options(self.categories.clone());
        select_value(&view.category, &previous_category);
        let visible = self.visible_expenses();
        let mut list_host = view.list.borrow_mut();
        let viewport = uimd::Size
        {
            width: list_host.frame.width,
            height: list_host.frame.height,
        };
        let Some(list) = list_host.child_window_mut() else { return };
        list.clear_children();
        for (index, expense) in visible.iter().enumerate()
        {
            let row = ExpenseRowUI::new();
            row.item.borrow_mut().set_text(&expense.item);
            row.amount
                .borrow_mut()
                .set_text(format!("{:.2}", expense.amount));
            row.paid.borrow_mut().set_checked(expense.paid);
            row.category
                .borrow_mut()
                .set_options(self.categories.clone());
            select_value(&row.category, &expense.category);
            list.add_named_child_window(format!("main.expenses[{index}]"), row.base);
        }
        if viewport.width > 0 && viewport.height > 0
        {
            list.scroll_to_top_in(viewport);
        }
    }

    fn refresh_categories(&self)
    {
        let Some(CurrentView::Categories(view)) = &self.current_view else { return };
        view.categories
            .borrow_mut()
            .set_options(self.categories.clone());
        if !self.categories.is_empty()
        {
            view.categories.borrow_mut().set_selected_index(0);
        }
    }

    fn add_expense(&mut self)
    {
        let Some(CurrentView::Expenses(view)) = &self.current_view else { return };
        let item = {
            let value = view.item.borrow().value().trim().to_string();
            if value.is_empty() { "Untitled expense".to_string() } else { value }
        };
        let amount = view.amount.borrow().number_value();
        let mut category = view.category.borrow().selected_value();
        if !self.categories.contains(&category)
        {
            category = self.categories.first().cloned().unwrap_or_default();
        }
        self.expenses.insert(0, Expense
        {
            id: format!("e-{}", self.next_expense_number),
            item,
            amount,
            category,
            paid: false,
        });
        self.next_expense_number += 1;
        {
            let mut item = view.item.borrow_mut();
            item.set_value("");
            item.set_cursor(0);
        }
        view.amount.borrow_mut().set_number_value(0.0);
        self.refresh_expenses();
    }

    fn add_category(&mut self)
    {
        let Some(CurrentView::Categories(view)) = &self.current_view else { return };
        let name = view.name.borrow().value().trim().to_string();
        if name.is_empty()
        {
            return;
        }
        if !self.categories.contains(&name)
        {
            self.categories.push(name.clone());
        }
        {
            let mut input = view.name.borrow_mut();
            input.set_value("");
            input.set_cursor(0);
        }
        self.refresh_categories();
        view.categories
            .borrow_mut()
            .set_selected_items(&[name]);
    }

    fn remove_category(&mut self)
    {
        let Some(CurrentView::Categories(view)) = &self.current_view else { return };
        let selected = view.categories.borrow().selected_value();
        if selected.is_empty() || self.categories.len() <= 1
        {
            return;
        }
        let before = self.categories.len();
        self.categories.retain(|category| category != &selected);
        if self.categories.len() == before
        {
            return;
        }
        let fallback = self.categories.first().cloned().unwrap_or_default();
        for expense in &mut self.expenses
        {
            if expense.category == selected
            {
                expense.category = fallback.clone();
            }
        }
        self.refresh_categories();
    }

    fn row_change(&mut self, ui: &ExpenseTrackerUI, id: &str)
    {
        let Some(index) = self.expenses.iter().position(|expense| expense.id == id) else
        {
            return;
        };
        let Some((visible_index, _)) = self
            .visible_expenses()
            .iter()
            .enumerate()
            .find(|(_, expense)| expense.id == id)
        else
        {
            return;
        };
        let category_id = format!("main.expenses[{visible_index}].category");
        let paid_id = format!("main.expenses[{visible_index}].paid");
        if let Some(category) = ui.base.find_element(&category_id)
        {
            self.expenses[index].category = category.borrow().selected_value();
        }
        if let Some(paid) = ui.base.find_element(&paid_id)
        {
            self.expenses[index].paid = paid.borrow().checked();
        }
    }

    fn confirm_delete(&mut self, ui: &mut ExpenseTrackerUI, id: &str)
    {
        let item = self
            .expenses
            .iter()
            .find(|expense| expense.id == id)
            .map_or("expense", |expense| expense.item.as_str())
            .to_string();
        self.pending_delete_id = Some(id.to_string());
        let mut dialog = uimd::MessageBoxYesNo::new(
            "Delete Expense",
            &format!("Delete {item}?"),
        );
        ui.base.open_window(dialog.take_window());
        self.delete_dialog = Some(dialog);
    }

    fn delete_confirmed(
        &mut self,
        result: uimd::MessageBoxResult,
    )
    {
        let id = self.pending_delete_id.take();
        if result != uimd::MessageBoxResult::Yes
        {
            return;
        }
        if let Some(id) = id
        {
            self.expenses.retain(|expense| expense.id != id);
            self.refresh_expenses();
        }
    }

    fn reset_demo_data(&mut self, ui: &mut ExpenseTrackerUI)
    {
        self.categories = DEFAULT_CATEGORIES.into_iter().map(str::to_string).collect();
        self.expenses = seed_expenses();
        self.currency = DEFAULT_CURRENCY.to_string();
        self.show_paid = true;
        self.monthly_limit = DEFAULT_MONTHLY_LIMIT;
        self.next_expense_number = FIRST_NEW_EXPENSE_NUMBER;
        self.show_settings(ui);
    }

    fn dynamic_row<'a>(&self, element_id: &'a str) -> Option<(Expense, &'a str)>
    {
        let (index, member) = indexed_member(element_id, "main.expenses")?;
        Some((self.visible_expenses().get(index)?.clone(), member))
    }
}

impl ExpenseTrackerUIEvents for ExpenseTrackerApp
{
    fn on_quit_btn_click(&mut self, ui: &mut ExpenseTrackerUI)
    {
        ui.base.request_close();
    }

    fn on_expenses_btn_click(&mut self, ui: &mut ExpenseTrackerUI)
    {
        self.show_expenses(ui);
    }

    fn on_categories_btn_click(&mut self, ui: &mut ExpenseTrackerUI)
    {
        self.show_categories(ui);
    }

    fn on_settings_btn_click(&mut self, ui: &mut ExpenseTrackerUI)
    {
        self.show_settings(ui);
    }

    fn handle_dynamic_button(&mut self, ui: &mut ExpenseTrackerUI, name: &str) -> bool
    {
        match name
        {
            "main.add_expense_btn" =>
            {
                self.add_expense();
                true
            }
            "main.add_category_btn" =>
            {
                self.add_category();
                true
            }
            "main.remove_category_btn" =>
            {
                self.remove_category();
                true
            }
            "main.reset_btn" =>
            {
                self.reset_demo_data(ui);
                true
            }
            _ =>
            {
                let Some((expense, member)) = self.dynamic_row(name) else { return false };
                if member != "delete_btn"
                {
                    return false;
                }
                self.confirm_delete(ui, &expense.id);
                true
            }
        }
    }

    fn handle_dynamic_text_changed(
        &mut self,
        ui: &mut ExpenseTrackerUI,
        name: &str,
        _value: &str,
    ) -> bool
    {
        if let Some((expense, member)) = self.dynamic_row(name)
            && matches!(member, "paid" | "category")
        {
            self.row_change(ui, &expense.id);
            return true;
        }
        let Some(CurrentView::Settings(view)) = &self.current_view else { return false };
        match name
        {
            "main.currency" =>
            {
                self.currency = view.currency.borrow().selected_value();
                true
            }
            "main.show_paid" =>
            {
                self.show_paid = view.show_paid.borrow().checked();
                true
            }
            "main.monthly_limit" =>
            {
                self.monthly_limit = view.monthly_limit.borrow().number_value();
                true
            }
            _ => false,
        }
    }

    fn handle_dynamic_text_confirmed(
        &mut self,
        _ui: &mut ExpenseTrackerUI,
        name: &str,
        _value: &str,
    ) -> bool
    {
        let Some(CurrentView::Settings(view)) = &self.current_view else { return false };
        if name == "main.monthly_limit"
        {
            self.monthly_limit = view.monthly_limit.borrow().number_value();
            return true;
        }
        false
    }

    fn handle_dynamic_selection_changed(
        &mut self,
        ui: &mut ExpenseTrackerUI,
        name: &str,
        _value: &[String],
    ) -> bool
    {
        self.handle_dynamic_text_changed(ui, name, "")
    }

    fn on_window_closed(
        &mut self,
        _ui: &mut ExpenseTrackerUI,
        window: uimd::GeneratedWindow,
    )
    {
        if !self
            .delete_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            return;
        }
        let Some(dialog) = self.delete_dialog.take() else { return };
        if let Some(result) = dialog.result()
        {
            self.delete_confirmed(result);
        }
    }
}

fn select_value(element: &uimd::ElementRef, value: &str)
{
    let index = element
        .borrow()
        .options()
        .iter()
        .position(|option| option == value)
        .map_or(0, |index| index as i32);
    element.borrow_mut().set_selected_index(index);
}

fn indexed_member<'a>(id: &'a str, prefix: &str) -> Option<(usize, &'a str)>
{
    let suffix = id.strip_prefix(prefix)?.strip_prefix('[')?;
    let (index, member) = suffix.split_once("].")?;
    Some((index.parse().ok()?, member))
}

fn seed_expenses() -> Vec<Expense>
{
    [
        ("e-1", "Groceries", 42.80, "Food", true),
        ("e-2", "Bus ticket", 2.50, "Transport", true),
        ("e-3", "Light bulbs", 8.90, "Home", false),
        ("e-4", "Pharmacy", 14.20, "Health", true),
        ("e-5", "Notebook", 5.40, "Other", false),
        ("e-6", "Internet bill", 29.99, "Home", true),
        ("e-7", "Team lunch", 18.50, "Food", false),
        ("e-8", "Train pass", 24.00, "Transport", true),
        ("e-9", "Vitamins", 11.75, "Health", false),
        ("e-10", "Desk lamp", 21.30, "Home", false),
        ("e-11", "Cloud storage", 9.99, "Other", true),
        ("e-12", "Replacement keyboard", 44.00, "Other", false),
    ]
    .into_iter()
    .map(|(id, item, amount, category, paid)| Expense
    {
        id: id.to_string(),
        item: item.to_string(),
        amount,
        category: category.to_string(),
        paid,
    })
    .collect()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ExpenseTrackerUI::new();
        let mut app = ExpenseTrackerApp::default();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

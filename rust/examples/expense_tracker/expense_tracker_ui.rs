// Auto-generated UI code for expense_tracker - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "expenses_view_ui.rs"]
mod uimd_dependency_expenses_view;
#[path = "categories_view_ui.rs"]
mod uimd_dependency_categories_view;
#[path = "settings_view_ui.rs"]
mod uimd_dependency_settings_view;

pub struct ExpenseTrackerUI
{
    pub base: uimd::GeneratedWindow,
    pub app_title: uimd::LabelRef,
    pub quit_btn: uimd::ButtonRef,
    pub nav_title: uimd::LabelRef,
    pub expenses_btn: uimd::ButtonRef,
    pub categories_btn: uimd::ButtonRef,
    pub settings_btn: uimd::ButtonRef,
    pub main: uimd::ViewHostRef,
}

impl ExpenseTrackerUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Expense Tracker");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 53, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 13, margin_bottom: 0,
            content: "app_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        padding: Some(0),
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
            element_type: "viewhost".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 24,
            cell_chars_width: 29, cell_chars_height: 9,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 27, chars_height: 9,
            margin_right: 1, margin_bottom: 0,
            content: "main".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 23, cell_chars_height: 9,
            cell_name: "nav".to_string(), cell_width: 23, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 1, margin_bottom: 8,
            content: "nav_title".to_string(),
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
            element_type: "button".to_string(),
            row: 0, col: 41,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 53, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "quit_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        padding: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 2, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 23, cell_chars_height: 9,
            cell_name: "nav".to_string(), cell_width: 23, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 1, margin_bottom: 6,
            content: "expenses_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
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
            row: 4, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 23, cell_chars_height: 9,
            cell_name: "nav".to_string(), cell_width: 23, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 1, margin_bottom: 4,
            content: "categories_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
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
            row: 6, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 23, cell_chars_height: 9,
            cell_name: "nav".to_string(), cell_width: 23, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "settings_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
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
        padding: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("app_title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("nav", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("nav_title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ExpenseTracker", "# Expense Tracker\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: experimental\ndescription: \"Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.\"\ntags: [example, viewhost, expenses]\n```\n\n## Definition\n\n```yaml\nuses:\n  - expenses_view\n  - categories_view\n  - settings_view\n```\n\n## Members\n\n```yaml\napp_title:\n  type: label\n  text: \"Expense Tracker\"\n\nquit_btn:\n  type: button\n  title: Quit\n\nnav_title:\n  type: label\n  text: Expenses\n\nexpenses_btn:\n  type: button\n  title: Expenses\n\ncategories_btn:\n  type: button\n  title: Categories\n\nsettings_btn:\n  type: button\n  title: Settings\n\nmain:\n  type: viewhost\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: \"#1f2937\"\n  padding: 0\n@app_title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@nav_title:\n  color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+--header-**------------------------------------------+\n| app_title..**.......................... quit_btn... |\n+--nav------------------+--content-**-----------------+\n| nav_title............ | main..**................... |\n|                       | *.......................... |\n| expenses_btn......... | *.......................... |\n|                       | ........................... |\n| categories_btn....... | ........................... |\n|                       | ........................... |\n| settings_btn......... | ........................... |\n*                       * ........................... |\n*                       * ........................... |\n+-----------------------+-----------------------------+\n```\n", "Simple expense tracker with sidebar navigation and dynamic expenses hosted in a ViewHost.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("app_title", "Expense Tracker", true), uimd::GeneratedElementMetadata::new("quit_btn", "Quit", true), uimd::GeneratedElementMetadata::new("nav_title", "Expenses", true), uimd::GeneratedElementMetadata::new("expenses_btn", "Expenses", true), uimd::GeneratedElementMetadata::new("categories_btn", "Categories", true), uimd::GeneratedElementMetadata::new("settings_btn", "Settings", true), uimd::GeneratedElementMetadata::new("main", "main", true)]);
        base.set_mcp_app_tools(vec![]);
        let app_title = uimd::new_label("app_title", "Expense Tracker");
        base.add_element(app_title.clone());
        app_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        app_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let quit_btn = uimd::new_button("quit_btn", "Quit");
        base.add_element(quit_btn.clone());
        quit_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        quit_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        quit_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let nav_title = uimd::new_label("nav_title", "Expenses");
        base.add_element(nav_title.clone());
        nav_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        nav_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let expenses_btn = uimd::new_button("expenses_btn", "Expenses");
        base.add_element(expenses_btn.clone());
        expenses_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        expenses_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        expenses_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let categories_btn = uimd::new_button("categories_btn", "Categories");
        base.add_element(categories_btn.clone());
        categories_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        categories_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        categories_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let settings_btn = uimd::new_button("settings_btn", "Settings");
        base.add_element(settings_btn.clone());
        settings_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        settings_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        settings_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let main = uimd::new_view_host("main");
        base.add_element(main.clone());
        main.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        Self
        {
            base,
            app_title,
            quit_btn,
            nav_title,
            expenses_btn,
            categories_btn,
            settings_btn,
            main,
        }
    }

    pub fn run<H: ExpenseTrackerUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ExpenseTrackerUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ExpenseTrackerUI
{
    fn default() -> Self { Self::new() }
}

pub trait ExpenseTrackerUIEvents
{
    fn on_quit_btn_click(&mut self, _ui: &mut ExpenseTrackerUI) {}
    fn on_expenses_btn_click(&mut self, _ui: &mut ExpenseTrackerUI) {}
    fn on_categories_btn_click(&mut self, _ui: &mut ExpenseTrackerUI) {}
    fn on_settings_btn_click(&mut self, _ui: &mut ExpenseTrackerUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ExpenseTrackerUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ExpenseTrackerUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ExpenseTrackerUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ExpenseTrackerUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ExpenseTrackerUIRuntime<'a, H: ExpenseTrackerUIEvents>
{
    ui: &'a mut ExpenseTrackerUI,
    handler: &'a mut H,
}

impl<H: ExpenseTrackerUIEvents> uimd::GeneratedApplication for ExpenseTrackerUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "quit_btn" { self.handler.on_quit_btn_click(self.ui); return true; }
        if name == "expenses_btn" { self.handler.on_expenses_btn_click(self.ui); return true; }
        if name == "categories_btn" { self.handler.on_categories_btn_click(self.ui); return true; }
        if name == "settings_btn" { self.handler.on_settings_btn_click(self.ui); return true; }
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

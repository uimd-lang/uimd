// Auto-generated UI code for categories_view - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct CategoriesViewUI
{
    pub base: uimd::GeneratedWindow,
    pub view_title: uimd::LabelRef,
    pub category_name: uimd::TextInputRef,
    pub add_category_btn: uimd::ButtonRef,
    pub categories: uimd::ListBoxRef,
    pub remove_category_btn: uimd::ButtonRef,
}

impl CategoriesViewUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Categories View");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 58, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 1, margin_bottom: 10,
            content: "view_title".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bbf7d0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 2, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 7, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 7, chars_height: 1,
            margin_right: 52, margin_bottom: 8,
            content: "\"Name:\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 2, col: 9,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 49, chars_height: 1,
            margin_right: 2, margin_bottom: 8,
            content: "category_name".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 32, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 32, chars_height: 1,
            margin_right: 27, margin_bottom: 6,
            content: "add_category_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 34,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 24, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 24, chars_height: 1,
            margin_right: 2, margin_bottom: 6,
            content: "remove_category_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "listbox".to_string(),
            row: 6, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 60, cell_chars_height: 11,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 58, chars_height: 5,
            margin_right: 1, margin_bottom: 0,
            content: "categories".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
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
        color: Some(uimd::Color::new("#bbf7d0")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "CategoriesView", "# Categories View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: experimental\ndescription: \"Simple category list management view.\"\ntags: [example, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nview_title:\n  type: label\n  text: Categories\n\ncategory_name:\n  type: textinput\n  value: \"\"\n  maxlength: 40\n\nadd_category_btn:\n  type: button\n  title: Add\n\ncategories:\n  type: listbox\n  options: [Food, Transport, Home, Health, Other]\n  selected_items: [Food]\n\nremove_category_btn:\n  type: button\n  title: Remove selected\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@view_title:\n  color: \"#bbf7d0\"\ntextinput:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  edit-background: \"#0b1220\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nlistbox:\n  background: \"#0b1220\"\n  color: \"#e5e7eb\"\n  focus-background: \"#25364f\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n```\n\n## User Interface\n\n```ui\n+-view-**----------------------------------------------------+\n| view_title................................................ |\n|                                                            |\n| \"Name:\" category_name..**................................ |\n|                                                            |\n| add_category_btn................ remove_category_btn..... |\n|                                                            |\n| categories..**............................................ |\n| *......................................................... |\n| *......................................................... |\n| .......................................................... |\n| .......................................................... |\n+------------------------------------------------------------+\n```\n", "Simple category list management view.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("view_title", "Categories", true), uimd::GeneratedElementMetadata::new("category_name", "category_name", true), uimd::GeneratedElementMetadata::new("add_category_btn", "Add", true), uimd::GeneratedElementMetadata::new("categories", "categories", true), uimd::GeneratedElementMetadata::new("remove_category_btn", "Remove selected", true)]);
        base.set_mcp_app_tools(vec![]);
        let view_title = uimd::new_label("view_title", "Categories");
        base.add_element(view_title.clone());
        view_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bbf7d0")),
        ..Default::default()
    });
        view_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let category_name = uimd::new_text_input("category_name", "", 40);
        base.add_element(category_name.clone());
        category_name.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category_name.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category_name.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category_name.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        category_name.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category_name.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let add_category_btn = uimd::new_button("add_category_btn", "Add");
        base.add_element(add_category_btn.clone());
        add_category_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        add_category_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        add_category_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let categories = uimd::new_list_box("categories", vec!["Food".to_string(), "Transport".to_string(), "Home".to_string(), "Health".to_string(), "Other".to_string()], false);
        base.add_element(categories.clone());
        categories.borrow_mut().set_selected_index(0);
        categories.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        categories.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#25364f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        categories.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        categories.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        categories.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        categories.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let remove_category_btn = uimd::new_button("remove_category_btn", "Remove selected");
        base.add_element(remove_category_btn.clone());
        remove_category_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        remove_category_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        remove_category_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            view_title,
            category_name,
            add_category_btn,
            categories,
            remove_category_btn,
        }
    }

    pub fn run<H: CategoriesViewUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CategoriesViewUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CategoriesViewUI
{
    fn default() -> Self { Self::new() }
}

pub trait CategoriesViewUIEvents
{
    fn on_add_category_btn_click(&mut self, _ui: &mut CategoriesViewUI) {}
    fn on_remove_category_btn_click(&mut self, _ui: &mut CategoriesViewUI) {}
    fn on_category_name_change(&mut self, _ui: &mut CategoriesViewUI, _value: &str) {}
    fn on_category_name_submit(&mut self, _ui: &mut CategoriesViewUI, _value: &str) {}
    fn on_categories_selectionchange(&mut self, _ui: &mut CategoriesViewUI, _value: &[String]) {}
    fn handle_dynamic_button(&mut self, _ui: &mut CategoriesViewUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CategoriesViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CategoriesViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CategoriesViewUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CategoriesViewUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CategoriesViewUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut CategoriesViewUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CategoriesViewUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CategoriesViewUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CategoriesViewUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CategoriesViewUIRuntime<'a, H: CategoriesViewUIEvents>
{
    ui: &'a mut CategoriesViewUI,
    handler: &'a mut H,
}

impl<H: CategoriesViewUIEvents> uimd::GeneratedApplication for CategoriesViewUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "add_category_btn" { self.handler.on_add_category_btn_click(self.ui); return true; }
        if name == "remove_category_btn" { self.handler.on_remove_category_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "category_name" { self.handler.on_category_name_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "category_name" { self.handler.on_category_name_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "categories" { self.handler.on_categories_selectionchange(self.ui, value); return true; }
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

// Auto-generated UI code for task_filters - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TaskFiltersUI
{
    pub base: uimd::GeneratedWindow,
    pub filters_label: uimd::LabelRef,
    pub search_label: uimd::LabelRef,
    pub status_label: uimd::LabelRef,
    pub owner_label: uimd::LabelRef,
    pub search: uimd::TextInputRef,
    pub status_filter: uimd::ComboBoxRef,
    pub owner_filter: uimd::ComboBoxRef,
    pub apply_filters_btn: uimd::ButtonRef,
    pub reset_filters_btn: uimd::ButtonRef,
}

impl TaskFiltersUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Task Filters");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 78, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "filters_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#aab2c3")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 28, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 27, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "search_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 52,
            cell_chars_width: 26, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 26, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 24, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "owner_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 29,
            cell_chars_width: 22, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 22, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "status_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 0,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 3,
            cell_name: "".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 22, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 1,
            margin_right: 56, margin_bottom: 1,
            content: "apply_filters_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#f5f5f5")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 1, col: 0,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 28, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 28, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 27, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "search".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 52,
            cell_chars_width: 26, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 26, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 24, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "owner_filter".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 29,
            cell_chars_width: 22, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 22, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status_filter".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 24,
            cell_row: 5, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 3,
            cell_name: "".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 22, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 22, chars_height: 1,
            margin_right: 32, margin_bottom: 1,
            content: "reset_filters_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        color: Some(uimd::Color::new("#f5f5f5")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("control");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
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
        base.set_generated_named_style("filters_label", uimd::Style {
        color: Some(uimd::Color::new("#aab2c3")),
        ..Default::default()
    });
        base.set_generated_named_style("search_label", uimd::Style {
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        base.set_generated_named_style("status_label", uimd::Style {
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        base.set_generated_named_style("owner_label", uimd::Style {
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        base.set_generated_named_style("apply_filters_btn", uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        ..Default::default()
    });
        base.set_generated_named_style("reset_filters_btn", uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TaskFilters", "# Task Filters\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\nstatus: draft\ndescription: \"Reusable task board filter panel.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nfilters_label:\n  type: label\n  text: FILTERS\n  description: \"Filter panel label.\"\n  expose: false\n\nsearch_label:\n  type: label\n  text: Search\n  description: \"Search filter label.\"\n  expose: false\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Status filter label.\"\n  expose: false\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Assignee filter label.\"\n  expose: false\n\nsearch:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  placeholder: \"Task title or description\"\n  description: \"Filter tasks by title or description.\"\n\nstatus_filter:\n  type: combobox\n  options: [Any, Todo, Doing, Blocked, Done]\n  selected_item: Any\n  description: \"Filter tasks by status.\"\n\nowner_filter:\n  type: combobox\n  options: [Any, Ava, Bruno, Chen, Dana]\n  selected_item: Any\n  description: \"Filter tasks by assignee.\"\n\napply_filters_btn:\n  type: button\n  title: \"Apply filter\"\n  description: \"Apply the current task filters.\"\n\nreset_filters_btn:\n  type: button\n  title: Reset\n  description: \"Reset search, status, and assignee filters.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  background: \"#15171c\"\n  border-width: 0\nlabel:\n  color: \"#f5f5f5\"\ntextinput:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  cursor-background: \"#93c5fd\"\n  cursor-color: \"#ffffff\"\ncombobox:\n  background: \"#252b35\"\n  color: \"#ffffff\"\n  focus-background: \"#303a49\"\n  edit-background: \"#344459\"\n  selected-background: \"#2f80ed\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#2f3545\"\n  color: \"#f5f5f5\"\n  focus-background: \"#2f80ed\"\n@filters_label:\n  color: \"#aab2c3\"\n@search_label:\n  color: \"#cfd5e3\"\n@status_label:\n  color: \"#cfd5e3\"\n@owner_label:\n  color: \"#cfd5e3\"\n@apply_filters_btn:\n  background: \"#2f80ed\"\n  focus-background: \"#1d6fd8\"\n@reset_filters_btn:\n  background: \"#343b4b\"\n```\n\n## User Interface\n\n```ui\n+-**---------------------------------------------------------------------------+\n|filters_label..**.............................................................|\n+----------------------------+----------------------+--------------------------+\n|search_label..**........... | status_label..**.... | owner_label..**......... |\n|search..**................. | status_filter.**.... | owner_filter..**........ |\n+-**-------------------------+----------------------+--------------------------+\n|                                                                              |\n|apply_filters_btn.....  reset_filters_btn.....                                |\n|                                                                              |\n+------------------------------------------------------------------------------+\n```\n", "Reusable task board filter panel.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("filters_label", "Filter panel label.", false), uimd::GeneratedElementMetadata::new("search_label", "Search filter label.", false), uimd::GeneratedElementMetadata::new("status_label", "Status filter label.", false), uimd::GeneratedElementMetadata::new("owner_label", "Assignee filter label.", false), uimd::GeneratedElementMetadata::new("search", "Filter tasks by title or description.", true), uimd::GeneratedElementMetadata::new("status_filter", "Filter tasks by status.", true), uimd::GeneratedElementMetadata::new("owner_filter", "Filter tasks by assignee.", true), uimd::GeneratedElementMetadata::new("apply_filters_btn", "Apply the current task filters.", true), uimd::GeneratedElementMetadata::new("reset_filters_btn", "Reset search, status, and assignee filters.", true)]);
        base.set_mcp_app_tools(vec![]);
        let filters_label = uimd::new_label("filters_label", "FILTERS");
        base.add_element(filters_label.clone());
        filters_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#aab2c3")),
        ..Default::default()
    });
        filters_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let search_label = uimd::new_label("search_label", "Search");
        base.add_element(search_label.clone());
        search_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        search_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let status_label = uimd::new_label("status_label", "Status");
        base.add_element(status_label.clone());
        status_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        status_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let owner_label = uimd::new_label("owner_label", "Assignee");
        base.add_element(owner_label.clone());
        owner_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cfd5e3")),
        ..Default::default()
    });
        owner_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let search = uimd::new_text_input("search", "", 60);
        base.add_element(search.clone());
        search.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        search.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#303a49")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        search.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#344459")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        search.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#93c5fd")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        search.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        search.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let status_filter = uimd::new_combo_box("status_filter", vec!["Any".to_string(), "Todo".to_string(), "Doing".to_string(), "Blocked".to_string(), "Done".to_string()]);
        base.add_element(status_filter.clone());
        status_filter.borrow_mut().set_selected_index(0);
        status_filter.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        status_filter.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#303a49")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        status_filter.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#344459")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        status_filter.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        status_filter.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let owner_filter = uimd::new_combo_box("owner_filter", vec!["Any".to_string(), "Ava".to_string(), "Bruno".to_string(), "Chen".to_string(), "Dana".to_string()]);
        base.add_element(owner_filter.clone());
        owner_filter.borrow_mut().set_selected_index(0);
        owner_filter.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#252b35")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        owner_filter.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#303a49")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        owner_filter.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#344459")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        owner_filter.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        owner_filter.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let apply_filters_btn = uimd::new_button("apply_filters_btn", "Apply filter");
        base.add_element(apply_filters_btn.clone());
        apply_filters_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#f5f5f5")),
        ..Default::default()
    });
        apply_filters_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1d6fd8")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        apply_filters_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let reset_filters_btn = uimd::new_button("reset_filters_btn", "Reset");
        base.add_element(reset_filters_btn.clone());
        reset_filters_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        color: Some(uimd::Color::new("#f5f5f5")),
        ..Default::default()
    });
        reset_filters_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        reset_filters_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            filters_label,
            search_label,
            status_label,
            owner_label,
            search,
            status_filter,
            owner_filter,
            apply_filters_btn,
            reset_filters_btn,
        }
    }

    pub fn run<H: TaskFiltersUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TaskFiltersUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TaskFiltersUI
{
    fn default() -> Self { Self::new() }
}

pub trait TaskFiltersUIEvents
{
    fn on_apply_filters_btn_click(&mut self, _ui: &mut TaskFiltersUI) {}
    fn on_reset_filters_btn_click(&mut self, _ui: &mut TaskFiltersUI) {}
    fn on_search_change(&mut self, _ui: &mut TaskFiltersUI, _value: &str) {}
    fn on_search_submit(&mut self, _ui: &mut TaskFiltersUI, _value: &str) {}
    fn on_status_filter_change(&mut self, _ui: &mut TaskFiltersUI, _value: &str) {}
    fn on_owner_filter_change(&mut self, _ui: &mut TaskFiltersUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut TaskFiltersUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TaskFiltersUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TaskFiltersUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TaskFiltersUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TaskFiltersUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TaskFiltersUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut TaskFiltersUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TaskFiltersUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TaskFiltersUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TaskFiltersUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TaskFiltersUIRuntime<'a, H: TaskFiltersUIEvents>
{
    ui: &'a mut TaskFiltersUI,
    handler: &'a mut H,
}

impl<H: TaskFiltersUIEvents> uimd::GeneratedApplication for TaskFiltersUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "apply_filters_btn" { self.handler.on_apply_filters_btn_click(self.ui); return true; }
        if name == "reset_filters_btn" { self.handler.on_reset_filters_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "search" { self.handler.on_search_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "search" { self.handler.on_search_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "status_filter" { self.handler.on_status_filter_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "owner_filter" { self.handler.on_owner_filter_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

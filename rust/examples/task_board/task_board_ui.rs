// Auto-generated UI code for task_board - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "task_filters/task_filters_ui.rs"]
mod uimd_dependency_task_filters;
#[path = "task_list/task_list_ui.rs"]
mod uimd_dependency_task_list;
#[path = "task_dialog/task_dialog_ui.rs"]
mod uimd_dependency_task_dialog;

pub struct TaskBoardUI
{
    pub base: uimd::GeneratedWindow,
    pub title: uimd::LabelRef,
    pub quit_btn: uimd::ButtonRef,
    pub filters: uimd::ReusableElementRef,
    pub new_task_btn: uimd::ButtonRef,
    pub mark_all_done_btn: uimd::ButtonRef,
    pub clear_board_btn: uimd::ButtonRef,
    pub board: uimd::ReusableElementRef,
    pub status: uimd::LabelRef,
}

impl TaskBoardUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Task Board");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "task_list".to_string(),
            row: 0, col: 1,
            cell_row: 9, cell_col: 28,
            cell_chars_width: 50, cell_chars_height: 12,
            cell_name: "board_cell".to_string(), cell_width: 0, cell_height: 12,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 48, chars_height: 12,
            margin_right: 1, margin_bottom: 0,
            content: "board".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "task_filters".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 6,
            cell_name: "filter_cell".to_string(), cell_width: 0, cell_height: 6,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 76, chars_height: 6,
            margin_right: 1, margin_bottom: 0,
            content: "filters".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 9, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 12,
            cell_name: "actions".to_string(), cell_width: 24, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 1, margin_bottom: 11,
            content: "new_task_btn".to_string(),
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
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 52, margin_bottom: 0,
            content: "title".to_string(),
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
            row: 0, col: 65,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "quit_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
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
            cell_row: 9, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 12,
            cell_name: "actions".to_string(), cell_width: 24, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 1, margin_bottom: 9,
            content: "mark_all_done_btn".to_string(),
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
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 4, col: 1,
            cell_row: 9, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 12,
            cell_name: "actions".to_string(), cell_width: 24, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 1,
            margin_right: 1, margin_bottom: 7,
            content: "clear_board_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#711212")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 6, col: 1,
            cell_row: 9, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 12,
            cell_name: "actions".to_string(), cell_width: 24, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 25, chars_height: 3,
            margin_right: 1, margin_bottom: 3,
            content: "status".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
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
        base.set_generated_named_style("title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("filter_cell", uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    });
        base.set_generated_named_style("actions", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("board_cell", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("filters", uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    });
        base.set_generated_named_style("board", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("new_task_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_generated_named_style("mark_all_done_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_generated_named_style("clear_board_btn", uimd::Style {
        background: Some(uimd::Color::new("#711212")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TaskBoard", "# Task Board\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.\"\ntags: [example, mcp]\n```\n\n## Definition\n\n```yaml\nuses:\n  - task_filters/task_filters\n  - task_list/task_list\n  - task_dialog/task_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Task Board\"\n  description: \"Application title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the task list example.\"\n\nfilters:\n  type: task_filters\n  cpp-class: task_board_example::TaskFilters\n  cpp-header: \"task_filters/task_filters.hpp\"\n  description: \"Reusable task filter panel.\"\n\nnew_task_btn:\n  type: button\n  title: New task\n  description: \"Open a dialog for creating a task.\"\n\nmark_all_done_btn:\n  type: button\n  title: Mark all done\n  description: \"Ask before marking every task as done.\"\n\nclear_board_btn:\n  type: button\n  title: Clear board\n  description: \"Ask before deleting every task.\"\n\nboard:\n  type: task_list\n  cpp-class: task_board_example::TaskList\n  cpp-header: \"task_list/task_list.hpp\"\n  description: \"Scrollable list of interactive task rows.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Current board status.\"\n```\n\n## Tools\n\n```yaml\nquery_tasks:\n  description: \"Return tasks matching optional title, status, assignee, priority, and done filters.\"\n  input:\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    done: bool\n  output:\n    tasks:\n      - id: string\n        title: string\n        status: string\n        assignee: string\n        priority: string\n        description: string\n        done: bool\n\nget_task:\n  description: \"Return one task by id.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\nupdate_task:\n  description: \"Update a task by id and return the updated task.\"\n  input:\n    id: string\n    title: string\n    status: string\n    assignee: string\n    priority: string\n    description: string\n    done: bool\n    required: [id]\n  output:\n    task:\n      id: string\n      title: string\n      status: string\n      assignee: string\n      priority: string\n      description: string\n      done: bool\n\ndelete_task:\n  description: \"Delete a task by id and return whether it was removed.\"\n  input:\n    id: string\n    required: [id]\n  output:\n    deleted: bool\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@filter_cell:\n  background: \"#15171c\"\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@board_cell:\n  background: \"#030712\"\n@filters:\n  background: \"#15171c\"\n@board:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@new_task_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@mark_all_done_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@clear_board_btn:\n  background: \"#711212\"\n  focus-background: \"#b91c1c\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**................ ..................................... quit_btn.... |\n+-filter_cell-**---------------------------------------------------------------+\n| filters..**................................................................. |\n| *........................................................................... |\n| *........................................................................... |\n| ............................................................................ |\n| ............................................................................ |\n| ............................................................................ |\n+-actions-24----------------+-board_cell-**------------------------------------+\n| new_task_btn..**......... | board..**....................................... |\n|                           | ................................................ |\n* mark_all_done_btn..**.... | ................................................ |\n*                           | ................................................ |\n| clear_board_btn..**...... | ................................................ |\n|                           | *............................................... |\n| status..**............... | *............................................... |\n| ......................... | ................................................ |\n| ......................... | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n|                           | ................................................ |\n+---------------------------+--------------------------------------------------+\n```\n", "ClickUp-style task list example with interactive scroll view rows and app-specific MCP tools.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title", "Application title.", false), uimd::GeneratedElementMetadata::new("quit_btn", "Close the task list example.", true), uimd::GeneratedElementMetadata::new("filters", "Reusable task filter panel.", true), uimd::GeneratedElementMetadata::new("new_task_btn", "Open a dialog for creating a task.", true), uimd::GeneratedElementMetadata::new("mark_all_done_btn", "Ask before marking every task as done.", true), uimd::GeneratedElementMetadata::new("clear_board_btn", "Ask before deleting every task.", true), uimd::GeneratedElementMetadata::new("board", "Scrollable list of interactive task rows.", true), uimd::GeneratedElementMetadata::new("status", "Current board status.", true)]);
        base.set_mcp_app_tools(vec![uimd::GeneratedAppToolMetadata::from_json("delete_task", "Delete a task by id and return whether it was removed.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"deleted\":{\"type\":\"boolean\"}}}"), uimd::GeneratedAppToolMetadata::from_json("get_task", "Return one task by id.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}"), uimd::GeneratedAppToolMetadata::from_json("query_tasks", "Return tasks matching optional title, status, assignee, priority, and done filters.", "{\"type\":\"object\",\"properties\":{\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}", "{\"type\":\"object\",\"properties\":{\"tasks\":{\"type\":\"array\",\"items\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}}"), uimd::GeneratedAppToolMetadata::from_json("update_task", "Update a task by id and return the updated task.", "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}},\"required\":[\"id\"]}", "{\"type\":\"object\",\"properties\":{\"task\":{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"string\"},\"title\":{\"type\":\"string\"},\"status\":{\"type\":\"string\"},\"assignee\":{\"type\":\"string\"},\"priority\":{\"type\":\"string\"},\"description\":{\"type\":\"string\"},\"done\":{\"type\":\"boolean\"}}}}}")]);
        let title = uimd::new_label("title", "Task Board");
        base.add_element(title.clone());
        title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        title.borrow_mut().set_cursor_style(uimd::Style {
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
        let filters = uimd::new_reusable_element("filters", "task_filters");
        base.add_element(filters.clone());
        filters.borrow_mut().set_child_window(uimd_dependency_task_filters::TaskFiltersUI::new().base);
        filters.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#15171c")),
        ..Default::default()
    });
        filters.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        filters.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        filters.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let new_task_btn = uimd::new_button("new_task_btn", "New task");
        base.add_element(new_task_btn.clone());
        new_task_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        new_task_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        new_task_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let mark_all_done_btn = uimd::new_button("mark_all_done_btn", "Mark all done");
        base.add_element(mark_all_done_btn.clone());
        mark_all_done_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        mark_all_done_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        mark_all_done_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let clear_board_btn = uimd::new_button("clear_board_btn", "Clear board");
        base.add_element(clear_board_btn.clone());
        clear_board_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#711212")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        clear_board_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#b91c1c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        clear_board_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let board = uimd::new_reusable_element("board", "task_list");
        base.add_element(board.clone());
        board.borrow_mut().set_child_window(uimd_dependency_task_list::TaskListUI::new().base);
        board.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    });
        board.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#07111f")),
        ..Default::default()
    });
        board.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        ..Default::default()
    });
        board.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let status = uimd::new_label("status", "Ready");
        base.add_element(status.clone());
        status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        status.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            title,
            quit_btn,
            filters,
            new_task_btn,
            mark_all_done_btn,
            clear_board_btn,
            board,
            status,
        }
    }

    pub fn run<H: TaskBoardUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TaskBoardUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TaskBoardUI
{
    fn default() -> Self { Self::new() }
}

pub trait TaskBoardUIEvents
{
    fn on_quit_btn_click(&mut self, _ui: &mut TaskBoardUI) {}
    fn on_new_task_btn_click(&mut self, _ui: &mut TaskBoardUI) {}
    fn on_mark_all_done_btn_click(&mut self, _ui: &mut TaskBoardUI) {}
    fn on_clear_board_btn_click(&mut self, _ui: &mut TaskBoardUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut TaskBoardUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TaskBoardUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TaskBoardUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TaskBoardUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TaskBoardUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TaskBoardUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut TaskBoardUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut TaskBoardUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TaskBoardUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TaskBoardUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TaskBoardUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TaskBoardUIRuntime<'a, H: TaskBoardUIEvents>
{
    ui: &'a mut TaskBoardUI,
    handler: &'a mut H,
}

impl<H: TaskBoardUIEvents> uimd::GeneratedApplication for TaskBoardUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "quit_btn" { self.handler.on_quit_btn_click(self.ui); return true; }
        if name == "new_task_btn" { self.handler.on_new_task_btn_click(self.ui); return true; }
        if name == "mark_all_done_btn" { self.handler.on_mark_all_done_btn_click(self.ui); return true; }
        if name == "clear_board_btn" { self.handler.on_clear_board_btn_click(self.ui); return true; }
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

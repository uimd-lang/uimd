// Auto-generated UI code for task_dialog - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TaskDialogUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub name_label: uimd::LabelRef,
    pub name: uimd::TextInputRef,
    pub status_label: uimd::LabelRef,
    pub project_status: uimd::ComboBoxRef,
    pub owner_label: uimd::LabelRef,
    pub owner: uimd::ComboBoxRef,
    pub priority_label: uimd::LabelRef,
    pub priority: uimd::ComboBoxRef,
    pub note_label: uimd::LabelRef,
    pub note: uimd::TextAreaRef,
    pub message: uimd::LabelRef,
    pub add_btn: uimd::ButtonRef,
    pub cancel_btn: uimd::ButtonRef,
}

impl TaskDialogUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Task Dialog");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 54, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 54, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "header".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 2,
            cell_row: 20, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 54, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 23, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 29, margin_bottom: 1,
            content: "add_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 27,
            cell_row: 20, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 54, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 24, chars_height: 1,
            margin_right: 3, margin_bottom: 1,
            content: "cancel_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 42, margin_bottom: 15,
            content: "name_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 1, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 3, margin_bottom: 15,
            content: "name".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 40, margin_bottom: 13,
            content: "status_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 3, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 3, margin_bottom: 13,
            content: "project_status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 5, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 41, margin_bottom: 11,
            content: "owner_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 5, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 3, margin_bottom: 11,
            content: "owner".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 7, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 38, margin_bottom: 9,
            content: "priority_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 7, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 3, margin_bottom: 9,
            content: "priority".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 9, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 41, margin_bottom: 7,
            content: "note_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 9, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 5,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 5,
            margin_right: 3, margin_bottom: 3,
            content: "note".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 15, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 54, cell_chars_height: 17,
            cell_name: "".to_string(), cell_width: 54, cell_height: 17,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 49, chars_height: 1,
            margin_right: 3, margin_bottom: 1,
            content: "message".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fbbf24")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("dialog");
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
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("add_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_generated_named_style("message", uimd::Style {
        color: Some(uimd::Color::new("#fbbf24")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TaskDialog", "# Task Dialog\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: draft\ndescription: \"Dialog form for creating or editing a task.\"\ntags: [example, dialog]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Task\n  description: \"Dialog title.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Title\n  description: \"Label for task title.\"\n  expose: false\n\nname:\n  type: textinput\n  value: \"\"\n  maxlength: 60\n  description: \"Required task title.\"\n\nstatus_label:\n  type: label\n  text: Status\n  description: \"Label for task status.\"\n  expose: false\n\nproject_status:\n  type: combobox\n  options: [Todo, Doing, Blocked, Done]\n  selected_item: Todo\n  description: \"Task status.\"\n\nowner_label:\n  type: label\n  text: Assignee\n  description: \"Label for task assignee.\"\n  expose: false\n\nowner:\n  type: combobox\n  options: [Ava, Bruno, Chen, Dana]\n  selected_item: Ava\n  description: \"Task assignee.\"\n\npriority_label:\n  type: label\n  text: Priority\n  description: \"Label for task priority.\"\n  expose: false\n\npriority:\n  type: combobox\n  options: [Low, Med, High, Urgent]\n  selected_item: Med\n  description: \"Task priority.\"\n\nnote_label:\n  type: label\n  text: Description\n  description: \"Label for task description.\"\n  expose: false\n\nnote:\n  type: textarea\n  value: \"\"\n  maxlength: 160\n  description: \"Task description.\"\n\nmessage:\n  type: label\n  text: \"\"\n  description: \"Validation message.\"\n\nadd_btn:\n  type: button\n  title: Save task\n  description: \"Save the task and close the dialog.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the dialog without saving the task.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@add_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------------+\n|header................................................|\n+------------------------------------------------------+\n|                                                      |\n|  name_label      name.............................   |\n|                                                      |\n|  status_label    project_status...................   |\n|                                                      |\n|  owner_label     owner............................   |\n|                                                      |\n|  priority_label  priority.........................   |\n|                                                      |\n|  note_label.     note.............................   |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                  ...............................     |\n|                                                      |\n|  message..........................................   |\n|                                                      |\n+------------------------------------------------------+\n|  add_btn................  cancel_btn..............   |\n|                                                      |\n+------------------------------------------------------+\n```\n", "Dialog form for creating or editing a task.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Dialog title.", false), uimd::GeneratedElementMetadata::new("name_label", "Label for task title.", false), uimd::GeneratedElementMetadata::new("name", "Required task title.", true), uimd::GeneratedElementMetadata::new("status_label", "Label for task status.", false), uimd::GeneratedElementMetadata::new("project_status", "Task status.", true), uimd::GeneratedElementMetadata::new("owner_label", "Label for task assignee.", false), uimd::GeneratedElementMetadata::new("owner", "Task assignee.", true), uimd::GeneratedElementMetadata::new("priority_label", "Label for task priority.", false), uimd::GeneratedElementMetadata::new("priority", "Task priority.", true), uimd::GeneratedElementMetadata::new("note_label", "Label for task description.", false), uimd::GeneratedElementMetadata::new("note", "Task description.", true), uimd::GeneratedElementMetadata::new("message", "Validation message.", true), uimd::GeneratedElementMetadata::new("add_btn", "Save the task and close the dialog.", true), uimd::GeneratedElementMetadata::new("cancel_btn", "Close the dialog without saving the task.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header = uimd::new_label("header", "Task");
        base.add_element(header.clone());
        header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        header.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name_label = uimd::new_label("name_label", "Title");
        base.add_element(name_label.clone());
        name_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        name_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name = uimd::new_text_input("name", "", 60);
        base.add_element(name.clone());
        name.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        name.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        name.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let status_label = uimd::new_label("status_label", "Status");
        base.add_element(status_label.clone());
        status_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        status_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let project_status = uimd::new_combo_box("project_status", vec!["Todo".to_string(), "Doing".to_string(), "Blocked".to_string(), "Done".to_string()]);
        base.add_element(project_status.clone());
        project_status.borrow_mut().set_selected_index(0);
        project_status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        project_status.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        project_status.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        project_status.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        project_status.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let owner_label = uimd::new_label("owner_label", "Assignee");
        base.add_element(owner_label.clone());
        owner_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        owner_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let owner = uimd::new_combo_box("owner", vec!["Ava".to_string(), "Bruno".to_string(), "Chen".to_string(), "Dana".to_string()]);
        base.add_element(owner.clone());
        owner.borrow_mut().set_selected_index(0);
        owner.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        owner.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        owner.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        owner.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        owner.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let priority_label = uimd::new_label("priority_label", "Priority");
        base.add_element(priority_label.clone());
        priority_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        priority_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let priority = uimd::new_combo_box("priority", vec!["Low".to_string(), "Med".to_string(), "High".to_string(), "Urgent".to_string()]);
        base.add_element(priority.clone());
        priority.borrow_mut().set_selected_index(1);
        priority.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        priority.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        priority.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        priority.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        priority.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let note_label = uimd::new_label("note_label", "Description");
        base.add_element(note_label.clone());
        note_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        note_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let note = uimd::new_text_area_with_max_length("note", "", 160);
        base.add_element(note.clone());
        note.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        note.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        note.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        note.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        note.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        note.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let message = uimd::new_label("message", "");
        base.add_element(message.clone());
        message.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fbbf24")),
        ..Default::default()
    });
        message.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let add_btn = uimd::new_button("add_btn", "Save task");
        base.add_element(add_btn.clone());
        add_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        add_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        add_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let cancel_btn = uimd::new_button("cancel_btn", "Cancel");
        base.add_element(cancel_btn.clone());
        cancel_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        cancel_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        cancel_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        Self
        {
            base,
            header,
            name_label,
            name,
            status_label,
            project_status,
            owner_label,
            owner,
            priority_label,
            priority,
            note_label,
            note,
            message,
            add_btn,
            cancel_btn,
        }
    }

    pub fn run<H: TaskDialogUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TaskDialogUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TaskDialogUI
{
    fn default() -> Self { Self::new() }
}

pub trait TaskDialogUIEvents
{
    fn on_add_btn_click(&mut self, _ui: &mut TaskDialogUI) {}
    fn on_cancel_btn_click(&mut self, _ui: &mut TaskDialogUI) {}
    fn on_name_change(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_note_change(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_name_submit(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_note_submit(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_project_status_change(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_owner_change(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn on_priority_change(&mut self, _ui: &mut TaskDialogUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut TaskDialogUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TaskDialogUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TaskDialogUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TaskDialogUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TaskDialogUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TaskDialogUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut TaskDialogUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut TaskDialogUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TaskDialogUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TaskDialogUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TaskDialogUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TaskDialogUIRuntime<'a, H: TaskDialogUIEvents>
{
    ui: &'a mut TaskDialogUI,
    handler: &'a mut H,
}

impl<H: TaskDialogUIEvents> uimd::GeneratedApplication for TaskDialogUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "add_btn" { self.handler.on_add_btn_click(self.ui); return true; }
        if name == "cancel_btn" { self.handler.on_cancel_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name" { self.handler.on_name_change(self.ui, value); return true; }
        if name == "note" { self.handler.on_note_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name" { self.handler.on_name_submit(self.ui, value); return true; }
        if name == "note" { self.handler.on_note_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "project_status" { self.handler.on_project_status_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "owner" { self.handler.on_owner_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "priority" { self.handler.on_priority_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

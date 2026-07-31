// Auto-generated UI code for contacts_manager - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "contact_form/contact_form_ui.rs"]
mod uimd_dependency_contact_form;

pub struct ContactsManagerUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub contacts_label: uimd::LabelRef,
    pub contacts: uimd::ListBoxRef,
    pub detail: uimd::LabelRef,
    pub add_btn: uimd::ButtonRef,
    pub edit_btn: uimd::ButtonRef,
    pub delete_btn: uimd::ButtonRef,
    pub save_btn: uimd::ButtonRef,
    pub reload_btn: uimd::ButtonRef,
    pub close_btn: uimd::ButtonRef,
    pub status: uimd::LabelRef,
}

impl ContactsManagerUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Contacts Manager");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 77, chars_height: 1,
            margin_right: 11, margin_bottom: 0,
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
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 6, cell_col: 0,
            cell_chars_width: 35, cell_chars_height: 10,
            cell_name: "list".to_string(), cell_width: 32, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 33, chars_height: 1,
            margin_right: 1, margin_bottom: 9,
            content: "contacts_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
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
            row: 0, col: 1,
            cell_row: 6, cell_col: 36,
            cell_chars_width: 52, cell_chars_height: 10,
            cell_name: "detail_cell".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 50, chars_height: 10,
            margin_right: 1, margin_bottom: 0,
            content: "detail".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#0b1120")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 17, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 88, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 86, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 77,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "close_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 3,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 73, margin_bottom: 1,
            content: "add_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "listbox".to_string(),
            row: 1, col: 1,
            cell_row: 6, cell_col: 0,
            cell_chars_width: 35, cell_chars_height: 10,
            cell_name: "list".to_string(), cell_width: 32, cell_height: 0,
            cell_width_mode: "fixed".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 33, chars_height: 8,
            margin_right: 1, margin_bottom: 1,
            content: "contacts".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#ffffff11")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 3,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 15, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 15, chars_height: 1,
            margin_right: 57, margin_bottom: 1,
            content: "edit_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 32,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 3,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 17, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 39, margin_bottom: 1,
            content: "delete_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 50,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 3,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 15, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 15, chars_height: 1,
            margin_right: 23, margin_bottom: 1,
            content: "save_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 66,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 88, cell_chars_height: 3,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 3,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 17, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 5, margin_bottom: 1,
            content: "reload_btn".to_string(),
            cell_style: uimd::Style::default(),
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
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("list", uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
        ..Default::default()
    });
        base.set_generated_named_style("contacts", uimd::Style {
        background: Some(uimd::Color::new("#ffffff11")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        base.set_generated_named_style("detail_cell", uimd::Style {
        background: Some(uimd::Color::new("#0b1120")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("detail", uimd::Style {
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("delete_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("close_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ContactsManager", "# Contacts Manager\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - contact_form\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Contacts Manager\"\n  description: \"Application title.\"\n  expose: false\n\ncontacts_label:\n  type: label\n  text: Contacts\n  description: \"Label for the contacts list.\"\n\ncontacts:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  description: \"List of contact records; selecting a row updates the detail panel.\"\n\ndetail:\n  type: label\n  text: \"Select a contact.\"\n  description: \"Read-only details for the selected contact.\"\n\nadd_btn:\n  type: button\n  title: Add\n  description: \"Open the contact form to create a new contact.\"\n\nedit_btn:\n  type: button\n  title: Edit\n  description: \"Open the selected contact for editing.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete the selected contact.\"\n\nsave_btn:\n  type: button\n  title: \"Save CSV\"\n  description: \"Save contacts to the configured CSV file.\"\n\nreload_btn:\n  type: button\n  title: Reload\n  description: \"Reload contacts from the configured CSV file.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the contacts manager.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows save, load, and validation status messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@list:\n  background: \"#101a2b\"\n@contacts:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@detail_cell:\n  background: \"#0b1120\"\n  padding: 1, 1, 1, 1\n@detail:\n  color: \"#d1d5db\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**-------------------------------------------------------------------------------+\n|header..**...................................................................close_btn..|\n+-actions-**-----------------------------------------------------------------------------+\n|                                                                                        |\n| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |\n|                                                                                        |\n+-list-32---------------------------+-detail_cell-**-------------------------------------+\n| contacts_label.**................ | detail..**........................................ |\n| contacts.......**................ | .................................................. |\n| #................................ | #................................................. |\n| #................................ | #................................................. |\n| ................................. | .................................................. |\n* ................................. | .................................................. |\n* ................................. * .................................................. |\n| ................................. * .................................................. |\n| ................................. | .................................................. |\n|                                   | .................................................. |\n+-----------------------------------+----------------------------------------------------+\n| status..**............................................................................ |\n+----------------------------------------------------------------------------------------+\n```\n", "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Application title.", false), uimd::GeneratedElementMetadata::new("contacts_label", "Label for the contacts list.", true), uimd::GeneratedElementMetadata::new("contacts", "List of contact records; selecting a row updates the detail panel.", true), uimd::GeneratedElementMetadata::new("detail", "Read-only details for the selected contact.", true), uimd::GeneratedElementMetadata::new("add_btn", "Open the contact form to create a new contact.", true), uimd::GeneratedElementMetadata::new("edit_btn", "Open the selected contact for editing.", true), uimd::GeneratedElementMetadata::new("delete_btn", "Delete the selected contact.", true), uimd::GeneratedElementMetadata::new("save_btn", "Save contacts to the configured CSV file.", true), uimd::GeneratedElementMetadata::new("reload_btn", "Reload contacts from the configured CSV file.", true), uimd::GeneratedElementMetadata::new("close_btn", "Close the contacts manager.", true), uimd::GeneratedElementMetadata::new("status", "Shows save, load, and validation status messages.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header = uimd::new_label("header", "Contacts Manager");
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
        let contacts_label = uimd::new_label("contacts_label", "Contacts");
        base.add_element(contacts_label.clone());
        contacts_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        contacts_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let contacts = uimd::new_list_box("contacts", vec![], false);
        base.add_element(contacts.clone());
        contacts.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff11")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        contacts.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff22")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        contacts.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        contacts.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        contacts.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        contacts.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let detail = uimd::new_label("detail", "Select a contact.");
        base.add_element(detail.clone());
        detail.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        detail.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let add_btn = uimd::new_button("add_btn", "Add");
        base.add_element(add_btn.clone());
        add_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        add_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        add_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let edit_btn = uimd::new_button("edit_btn", "Edit");
        base.add_element(edit_btn.clone());
        edit_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        edit_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        edit_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
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
        let save_btn = uimd::new_button("save_btn", "Save CSV");
        base.add_element(save_btn.clone());
        save_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        save_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        save_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let reload_btn = uimd::new_button("reload_btn", "Reload");
        base.add_element(reload_btn.clone());
        reload_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        reload_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        reload_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let close_btn = uimd::new_button("close_btn", "Quit");
        base.add_element(close_btn.clone());
        close_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        close_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
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
            header,
            contacts_label,
            contacts,
            detail,
            add_btn,
            edit_btn,
            delete_btn,
            save_btn,
            reload_btn,
            close_btn,
            status,
        }
    }

    pub fn run<H: ContactsManagerUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ContactsManagerUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ContactsManagerUI
{
    fn default() -> Self { Self::new() }
}

pub trait ContactsManagerUIEvents
{
    fn on_add_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_edit_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_delete_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_save_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_reload_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_close_btn_click(&mut self, _ui: &mut ContactsManagerUI) {}
    fn on_contacts_selectionchange(&mut self, _ui: &mut ContactsManagerUI, _value: &[String]) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ContactsManagerUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ContactsManagerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ContactsManagerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ContactsManagerUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ContactsManagerUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ContactsManagerUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut ContactsManagerUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ContactsManagerUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ContactsManagerUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ContactsManagerUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ContactsManagerUIRuntime<'a, H: ContactsManagerUIEvents>
{
    ui: &'a mut ContactsManagerUI,
    handler: &'a mut H,
}

impl<H: ContactsManagerUIEvents> uimd::GeneratedApplication for ContactsManagerUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "add_btn" { self.handler.on_add_btn_click(self.ui); return true; }
        if name == "edit_btn" { self.handler.on_edit_btn_click(self.ui); return true; }
        if name == "delete_btn" { self.handler.on_delete_btn_click(self.ui); return true; }
        if name == "save_btn" { self.handler.on_save_btn_click(self.ui); return true; }
        if name == "reload_btn" { self.handler.on_reload_btn_click(self.ui); return true; }
        if name == "close_btn" { self.handler.on_close_btn_click(self.ui); return true; }
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
        if name == "contacts" { self.handler.on_contacts_selectionchange(self.ui, value); return true; }
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

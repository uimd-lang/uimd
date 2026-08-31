// Auto-generated UI code for contact_form - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ContactFormUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub first_label: uimd::LabelRef,
    pub first_name: uimd::TextInputRef,
    pub last_label: uimd::LabelRef,
    pub last_name: uimd::TextInputRef,
    pub email_label: uimd::LabelRef,
    pub email: uimd::TextInputRef,
    pub phone_label: uimd::LabelRef,
    pub phone: uimd::TextInputRef,
    pub company_label: uimd::LabelRef,
    pub company: uimd::TextInputRef,
    pub city_label: uimd::LabelRef,
    pub city: uimd::TextInputRef,
    pub category_label: uimd::LabelRef,
    pub category: uimd::ComboBoxRef,
    pub active: uimd::CheckBoxRef,
    pub notes_label: uimd::LabelRef,
    pub notes: uimd::TextAreaRef,
    pub save_btn: uimd::ButtonRef,
    pub cancel_btn: uimd::ButtonRef,
    pub message: uimd::LabelRef,
}

impl ContactFormUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Contact Form");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 58, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
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
            cell_row: 26, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 58, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 26, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 30, margin_bottom: 1,
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
            row: 0, col: 30,
            cell_row: 26, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 2,
            cell_name: "".to_string(), cell_width: 58, cell_height: 2,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 26, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
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
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 45, margin_bottom: 21,
            content: "first_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 1, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 21,
            content: "first_name".to_string(),
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
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 46, margin_bottom: 19,
            content: "last_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 3, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 19,
            content: "last_name".to_string(),
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
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 45, margin_bottom: 17,
            content: "email_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 5, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 17,
            content: "email".to_string(),
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
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 45, margin_bottom: 15,
            content: "phone_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 7, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 15,
            content: "phone".to_string(),
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
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 43, margin_bottom: 13,
            content: "company_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 9, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 13,
            content: "company".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 11, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 46, margin_bottom: 11,
            content: "city_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 11, col: 17,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 39, chars_height: 1,
            margin_right: 2, margin_bottom: 11,
            content: "city".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 13, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 42, margin_bottom: 9,
            content: "category_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 13, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 38, chars_height: 1,
            margin_right: 2, margin_bottom: 9,
            content: "category".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 15, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 54, chars_height: 1,
            margin_right: 2, margin_bottom: 7,
            content: "active".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 17, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 45, margin_bottom: 5,
            content: "notes_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 17, col: 14,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 3,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 42, chars_height: 3,
            margin_right: 2, margin_bottom: 3,
            content: "notes".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 21, col: 2,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 58, cell_chars_height: 23,
            cell_name: "".to_string(), cell_width: 58, cell_height: 23,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 54, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "message".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#fbbf24")),
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
        base.set_generated_named_style("message", uimd::Style {
        color: Some(uimd::Color::new("#fbbf24")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ContactForm", "# Contact Form\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: Contact\n  description: \"Dialog title showing that a contact is being edited.\"\n  expose: false\n\nfirst_label:\n  type: label\n  text: \"First name\"\n  description: \"Label for the first name input.\"\n\nfirst_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required first name field.\"\n\nlast_label:\n  type: label\n  text: \"Last name\"\n  description: \"Label for the last name input.\"\n\nlast_name:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Required last name field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email input.\"\n\nemail:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Required email address field.\"\n\nphone_label:\n  type: label\n  text: Phone\n  description: \"Label for the phone input.\"\n\nphone:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Phone number field.\"\n\ncompany_label:\n  type: label\n  text: Company\n  description: \"Label for the company input.\"\n\ncompany:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n  description: \"Company name field.\"\n\ncity_label:\n  type: label\n  text: City\n  description: \"Label for the city input.\"\n\ncity:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"City field.\"\n\ncategory_label:\n  type: label\n  text: Category\n  description: \"Label for the contact category selector.\"\n\ncategory:\n  type: combobox\n  options: [Client, Vendor, Partner, Personal]\n  selected_item: Client\n  description: \"Contact category combobox with client, vendor, partner, and personal options.\"\n\nactive:\n  type: checkbox\n  title: \"Active contact\"\n  value: 1\n  description: \"Toggle whether the contact is active.\"\n\nnotes_label:\n  type: label\n  text: Notes\n  description: \"Label for the notes field.\"\n\nnotes:\n  type: textarea\n  value: \"\"\n  maxlength: 300\n  description: \"Multiline notes field for the contact.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Validate and save the contact form.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Close the form without saving changes.\"\n\nmessage:\n  type: label\n  text: \"Required: first name, last name, email.\"\n  description: \"Validation and status message area.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ncombobox:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@message:\n  color: \"#fbbf24\"\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------------+\n|header....................................................|\n+----------------------------------------------------------+\n|                                                          |\n|  first_label    first_name.............................  |\n|                                                          |\n|  last_label     last_name..............................  |\n|                                                          |\n|  email_label    email..................................  |\n|                                                          |\n|  phone_label    phone..................................  |\n|                                                          |\n|  company_label  company................................  |\n|                                                          |\n|  city_label     city...................................  |\n|                                                          |\n|  category_label  category..............................  |\n|                                                          |\n|  active................................................  |\n|                                                          |\n|  notes_label notes.....................................  |\n|              ..........................................  |\n|              ..........................................  |\n|                                                          |\n|  message...............................................  |\n|                                                          |\n+----------------------------------------------------------+\n|  save_btn..................  cancel_btn................  |\n|                                                          |\n+----------------------------------------------------------+\n```\n", "Dialog form for creating or editing one contact record with personal details, category, active state, notes, and validation feedback.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Dialog title showing that a contact is being edited.", false), uimd::GeneratedElementMetadata::new("first_label", "Label for the first name input.", true), uimd::GeneratedElementMetadata::new("first_name", "Required first name field.", true), uimd::GeneratedElementMetadata::new("last_label", "Label for the last name input.", true), uimd::GeneratedElementMetadata::new("last_name", "Required last name field.", true), uimd::GeneratedElementMetadata::new("email_label", "Label for the email input.", true), uimd::GeneratedElementMetadata::new("email", "Required email address field.", true), uimd::GeneratedElementMetadata::new("phone_label", "Label for the phone input.", true), uimd::GeneratedElementMetadata::new("phone", "Phone number field.", true), uimd::GeneratedElementMetadata::new("company_label", "Label for the company input.", true), uimd::GeneratedElementMetadata::new("company", "Company name field.", true), uimd::GeneratedElementMetadata::new("city_label", "Label for the city input.", true), uimd::GeneratedElementMetadata::new("city", "City field.", true), uimd::GeneratedElementMetadata::new("category_label", "Label for the contact category selector.", true), uimd::GeneratedElementMetadata::new("category", "Contact category combobox with client, vendor, partner, and personal options.", true), uimd::GeneratedElementMetadata::new("active", "Toggle whether the contact is active.", true), uimd::GeneratedElementMetadata::new("notes_label", "Label for the notes field.", true), uimd::GeneratedElementMetadata::new("notes", "Multiline notes field for the contact.", true), uimd::GeneratedElementMetadata::new("save_btn", "Validate and save the contact form.", true), uimd::GeneratedElementMetadata::new("cancel_btn", "Close the form without saving changes.", true), uimd::GeneratedElementMetadata::new("message", "Validation and status message area.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header = uimd::new_label("header", "Contact");
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
        let first_label = uimd::new_label("first_label", "First name");
        base.add_element(first_label.clone());
        first_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        first_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let first_name = uimd::new_text_input("first_name", "", 80);
        base.add_element(first_name.clone());
        first_name.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        first_name.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        first_name.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        first_name.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        first_name.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        first_name.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let last_label = uimd::new_label("last_label", "Last name");
        base.add_element(last_label.clone());
        last_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        last_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let last_name = uimd::new_text_input("last_name", "", 80);
        base.add_element(last_name.clone());
        last_name.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        last_name.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        last_name.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        last_name.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        last_name.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        last_name.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let email_label = uimd::new_label("email_label", "Email");
        base.add_element(email_label.clone());
        email_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        email_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let email = uimd::new_text_input("email", "", 120);
        base.add_element(email.clone());
        email.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        email.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        email.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let phone_label = uimd::new_label("phone_label", "Phone");
        base.add_element(phone_label.clone());
        phone_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        phone_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let phone = uimd::new_text_input("phone", "", 80);
        base.add_element(phone.clone());
        phone.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        phone.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        phone.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        phone.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        phone.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        phone.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let company_label = uimd::new_label("company_label", "Company");
        base.add_element(company_label.clone());
        company_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        company_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let company = uimd::new_text_input("company", "", 120);
        base.add_element(company.clone());
        company.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        company.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        company.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        company.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        company.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        company.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let city_label = uimd::new_label("city_label", "City");
        base.add_element(city_label.clone());
        city_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        city_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let city = uimd::new_text_input("city", "", 80);
        base.add_element(city.clone());
        city.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        city.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        city.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        city.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        city.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        city.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let category_label = uimd::new_label("category_label", "Category");
        base.add_element(category_label.clone());
        category_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        category_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let category = uimd::new_combo_box("category", vec!["Client".to_string(), "Vendor".to_string(), "Partner".to_string(), "Personal".to_string()]);
        base.add_element(category.clone());
        category.borrow_mut().set_selected_index(0);
        category.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        category.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        category.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
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
        let active = uimd::new_checkbox("active", "Active contact", true);
        base.add_element(active.clone());
        active.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    });
        active.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        active.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#44cc88")),
        ..Default::default()
    });
        active.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        ..Default::default()
    });
        active.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let notes_label = uimd::new_label("notes_label", "Notes");
        base.add_element(notes_label.clone());
        notes_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        notes_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let notes = uimd::new_text_area_with_max_length("notes", "", 300);
        base.add_element(notes.clone());
        notes.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        notes.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        notes.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        notes.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
        ..Default::default()
    });
        notes.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        notes.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let save_btn = uimd::new_button("save_btn", "Save");
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
        let message = uimd::new_label("message", "Required: first name, last name, email.");
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
        Self
        {
            base,
            header,
            first_label,
            first_name,
            last_label,
            last_name,
            email_label,
            email,
            phone_label,
            phone,
            company_label,
            company,
            city_label,
            city,
            category_label,
            category,
            active,
            notes_label,
            notes,
            save_btn,
            cancel_btn,
            message,
        }
    }

    pub fn run<H: ContactFormUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ContactFormUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ContactFormUI
{
    fn default() -> Self { Self::new() }
}

pub trait ContactFormUIEvents
{
    fn on_save_btn_click(&mut self, _ui: &mut ContactFormUI) {}
    fn on_cancel_btn_click(&mut self, _ui: &mut ContactFormUI) {}
    fn on_first_name_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_last_name_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_email_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_phone_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_company_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_city_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_active_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_notes_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_first_name_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_last_name_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_email_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_phone_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_company_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_city_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_notes_submit(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn on_category_change(&mut self, _ui: &mut ContactFormUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ContactFormUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ContactFormUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ContactFormUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ContactFormUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ContactFormUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ContactFormUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ContactFormUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ContactFormUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ContactFormUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ContactFormUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ContactFormUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ContactFormUIRuntime<'a, H: ContactFormUIEvents>
{
    ui: &'a mut ContactFormUI,
    handler: &'a mut H,
}

impl<H: ContactFormUIEvents> uimd::GeneratedApplication for ContactFormUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "save_btn" { self.handler.on_save_btn_click(self.ui); return true; }
        if name == "cancel_btn" { self.handler.on_cancel_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "first_name" { self.handler.on_first_name_change(self.ui, value); return true; }
        if name == "last_name" { self.handler.on_last_name_change(self.ui, value); return true; }
        if name == "email" { self.handler.on_email_change(self.ui, value); return true; }
        if name == "phone" { self.handler.on_phone_change(self.ui, value); return true; }
        if name == "company" { self.handler.on_company_change(self.ui, value); return true; }
        if name == "city" { self.handler.on_city_change(self.ui, value); return true; }
        if name == "active" { self.handler.on_active_change(self.ui, value); return true; }
        if name == "notes" { self.handler.on_notes_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "first_name" { self.handler.on_first_name_submit(self.ui, value); return true; }
        if name == "last_name" { self.handler.on_last_name_submit(self.ui, value); return true; }
        if name == "email" { self.handler.on_email_submit(self.ui, value); return true; }
        if name == "phone" { self.handler.on_phone_submit(self.ui, value); return true; }
        if name == "company" { self.handler.on_company_submit(self.ui, value); return true; }
        if name == "city" { self.handler.on_city_submit(self.ui, value); return true; }
        if name == "notes" { self.handler.on_notes_submit(self.ui, value); return true; }
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

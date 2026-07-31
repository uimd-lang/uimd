// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct FormularUI
{
    pub base: uimd::GeneratedWindow,
    pub title_label: uimd::LabelRef,
    pub name_label: uimd::LabelRef,
    pub name_input: uimd::TextInputRef,
    pub email_label: uimd::LabelRef,
    pub email_input: uimd::TextInputRef,
    pub age_label: uimd::LabelRef,
    pub age_input: uimd::NumberInputRef,
    pub description_input: uimd::TextAreaRef,
    pub country_label: uimd::LabelRef,
    pub country_combo: uimd::ComboBoxRef,
    pub role_label: uimd::LabelRef,
    pub role_listbox: uimd::ListBoxRef,
    pub notify_check: uimd::CheckBoxRef,
    pub terms_check: uimd::CheckBoxRef,
    pub save_btn: uimd::ButtonRef,
    pub cancel_btn: uimd::ButtonRef,
}

impl FormularUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("FormApp");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 48, chars_height: 1,
            margin_right: 2, margin_bottom: 24,
            content: "title_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 3, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 40, margin_bottom: 22,
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
            row: 3, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 22,
            content: "name_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 5, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 1,
            margin_right: 39, margin_bottom: 20,
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
            row: 5, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 20,
            content: "email_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 7, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 41, margin_bottom: 18,
            content: "age_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "numberinput".to_string(),
            row: 7, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 18,
            content: "age_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 9, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 37, margin_bottom: 16,
            content: "country_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 9, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 1,
            margin_right: 2, margin_bottom: 16,
            content: "country_combo".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 11, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 13, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 37, margin_bottom: 14,
            content: "\"Description\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 11, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 4,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 4,
            margin_right: 2, margin_bottom: 11,
            content: "description_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 16, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 40, margin_bottom: 9,
            content: "role_label".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "listbox".to_string(),
            row: 16, col: 19,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 4,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 31, chars_height: 4,
            margin_right: 2, margin_bottom: 6,
            content: "role_listbox".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 21, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 48, chars_height: 1,
            margin_right: 2, margin_bottom: 4,
            content: "notify_check".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 22, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 48, chars_height: 1,
            margin_right: 2, margin_bottom: 3,
            content: "terms_check".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 24, col: 7,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 31, margin_bottom: 1,
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
            row: 24, col: 27,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 52, cell_chars_height: 26,
            cell_name: "".to_string(), cell_width: 52, cell_height: 26,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 8, margin_bottom: 1,
            content: "cancel_btn".to_string(),
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
        base.set_generated_named_style("title_label", uimd::Style {
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Formular", "# FormApp\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ntitle_label:\n  type: label\n  text: \"User profile\"\n  description: \"Title of the form.\"\n  expose: false\n\nname_label:\n  type: label\n  text: Name\n  description: \"Label for the name field.\"\n\nname_input:\n  type: textinput\n  value: \"\"\n  description: \"User name text field.\"\n\nemail_label:\n  type: label\n  text: Email\n  description: \"Label for the email field.\"\n\nemail_input:\n  type: textinput\n  value: \"\"\n  description: \"Email address text field.\"\n\nage_label:\n  type: label\n  text: Age\n  description: \"Label for the age field.\"\n\nage_input:\n  type: numberinput\n  description: \"Numeric age input.\"\n\ndescription_input:\n  type: textarea\n  description: \"Multiline profile description field.\"\n\ncountry_label:\n  type: label\n  text: Country\n  description: \"Label for the country selector.\"\n\ncountry_combo:\n  type: combobox\n  options: [Slovakia, \"Czech Republic\", Poland, Hungary, Austria]\n  selected_item: Slovakia\n  description: \"Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.\"\n\nrole_label:\n  type: label\n  text: Role\n  description: \"Label for the role selector.\"\n\nrole_listbox:\n  type: listbox\n  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]\n  selected_items: [Developer]\n  multiple: \"true\"\n  description: \"Multi-select role listbox for choosing one or more user roles.\"\n\nnotify_check:\n  type: checkbox\n  title: \"Email notifications\"\n  description: \"Toggle email notifications.\"\n\nterms_check:\n  type: checkbox\n  title: \"I accept the terms and conditions\"\n  description: \"Toggle acceptance of terms and conditions.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Submit the form and serialize the entered data.\"\n\ncancel_btn:\n  type: button\n  title: Cancel\n  description: \"Cancel the form action.\"\n```\n\n## Style\n\n```yaml\n@title_label:\n  text-align: center\n```\n\n## User Interface\n\n```ui\n+----------------------------------------------------+\n|                                                    |\n|  title_label.....................................  |\n|                                                    |\n|  name_label       name_input.....................  |\n|                                                    |\n|  email_label      email_input....................  |\n|                                                    |\n|  age_label        age_input......................  |\n|                                                    |\n|  country_label    country_combo..................  |\n|                                                    |\n|  \"Description\"    description_input..............  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  role_label       role_listbox...................  |\n|                  ..............................    |\n|                  ..............................    |\n|                  ..............................    |\n|                                                    |\n|  notify_check....................................  |\n|  terms_check.....................................  |\n|                                                    |\n|       save_btn......      cancel_btn.......        |\n|                                                    |\n+----------------------------------------------------+\n```\n", "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title_label", "Title of the form.", false), uimd::GeneratedElementMetadata::new("name_label", "Label for the name field.", true), uimd::GeneratedElementMetadata::new("name_input", "User name text field.", true), uimd::GeneratedElementMetadata::new("email_label", "Label for the email field.", true), uimd::GeneratedElementMetadata::new("email_input", "Email address text field.", true), uimd::GeneratedElementMetadata::new("age_label", "Label for the age field.", true), uimd::GeneratedElementMetadata::new("age_input", "Numeric age input.", true), uimd::GeneratedElementMetadata::new("description_input", "Multiline profile description field.", true), uimd::GeneratedElementMetadata::new("country_label", "Label for the country selector.", true), uimd::GeneratedElementMetadata::new("country_combo", "Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.", true), uimd::GeneratedElementMetadata::new("role_label", "Label for the role selector.", true), uimd::GeneratedElementMetadata::new("role_listbox", "Multi-select role listbox for choosing one or more user roles.", true), uimd::GeneratedElementMetadata::new("notify_check", "Toggle email notifications.", true), uimd::GeneratedElementMetadata::new("terms_check", "Toggle acceptance of terms and conditions.", true), uimd::GeneratedElementMetadata::new("save_btn", "Submit the form and serialize the entered data.", true), uimd::GeneratedElementMetadata::new("cancel_btn", "Cancel the form action.", true)]);
        base.set_mcp_app_tools(vec![]);
        let title_label = uimd::new_label("title_label", "User profile");
        base.add_element(title_label.clone());
        title_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        title_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name_label = uimd::new_label("name_label", "Name");
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
        let name_input = uimd::new_text_input("name_input", "", 0);
        base.add_element(name_input.clone());
        name_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        name_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        name_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_disabled_style(uimd::Style {
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
        let email_input = uimd::new_text_input("email_input", "", 0);
        base.add_element(email_input.clone());
        email_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        email_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        email_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        email_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let age_label = uimd::new_label("age_label", "Age");
        base.add_element(age_label.clone());
        age_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        age_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let age_input = uimd::new_number_input_with_step("age_input", 0.000000, 1.000000);
        base.add_element(age_input.clone());
        age_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        age_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        age_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        age_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        age_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let description_input = uimd::new_text_area_with_max_length("description_input", "", 0);
        base.add_element(description_input.clone());
        description_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        description_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        description_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        description_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        description_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        description_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let country_label = uimd::new_label("country_label", "Country");
        base.add_element(country_label.clone());
        country_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        country_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let country_combo = uimd::new_combo_box("country_combo", vec!["Slovakia".to_string(), "Czech Republic".to_string(), "Poland".to_string(), "Hungary".to_string(), "Austria".to_string()]);
        base.add_element(country_combo.clone());
        country_combo.borrow_mut().set_selected_index(0);
        country_combo.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        country_combo.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        country_combo.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        country_combo.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        country_combo.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let role_label = uimd::new_label("role_label", "Role");
        base.add_element(role_label.clone());
        role_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        role_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let role_listbox = uimd::new_list_box("role_listbox", vec!["Developer".to_string(), "Designer".to_string(), "Manager".to_string(), "QA".to_string(), "DevOps".to_string(), "Tester".to_string(), "Hacker".to_string()], true);
        base.add_element(role_listbox.clone());
        role_listbox.borrow_mut().set_selected_index(0);
        role_listbox.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        role_listbox.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1d2f4d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        role_listbox.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        role_listbox.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        role_listbox.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        role_listbox.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let notify_check = uimd::new_checkbox("notify_check", "Email notifications", false);
        base.add_element(notify_check.clone());
        notify_check.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    });
        notify_check.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        notify_check.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#44cc88")),
        ..Default::default()
    });
        notify_check.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        ..Default::default()
    });
        notify_check.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let terms_check = uimd::new_checkbox("terms_check", "I accept the terms and conditions", false);
        base.add_element(terms_check.clone());
        terms_check.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#b8c4d8")),
        ..Default::default()
    });
        terms_check.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        terms_check.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#44cc88")),
        ..Default::default()
    });
        terms_check.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        ..Default::default()
    });
        terms_check.borrow_mut().set_disabled_style(uimd::Style {
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
        background: Some(uimd::Color::new("#2b5f9f")),
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
        background: Some(uimd::Color::new("#2b5f9f")),
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
            title_label,
            name_label,
            name_input,
            email_label,
            email_input,
            age_label,
            age_input,
            description_input,
            country_label,
            country_combo,
            role_label,
            role_listbox,
            notify_check,
            terms_check,
            save_btn,
            cancel_btn,
        }
    }

    pub fn run<H: FormularUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = FormularUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for FormularUI
{
    fn default() -> Self { Self::new() }
}

pub trait FormularUIEvents
{
    fn on_save_btn_click(&mut self, _ui: &mut FormularUI) {}
    fn on_cancel_btn_click(&mut self, _ui: &mut FormularUI) {}
    fn on_name_input_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_email_input_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_age_input_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_description_input_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_notify_check_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_terms_check_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_name_input_submit(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_email_input_submit(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_age_input_submit(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_description_input_submit(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_country_combo_change(&mut self, _ui: &mut FormularUI, _value: &str) {}
    fn on_role_listbox_selectionchange(&mut self, _ui: &mut FormularUI, _value: &[String]) {}
    fn handle_dynamic_button(&mut self, _ui: &mut FormularUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut FormularUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut FormularUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut FormularUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut FormularUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut FormularUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut FormularUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut FormularUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut FormularUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut FormularUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct FormularUIRuntime<'a, H: FormularUIEvents>
{
    ui: &'a mut FormularUI,
    handler: &'a mut H,
}

impl<H: FormularUIEvents> uimd::GeneratedApplication for FormularUIRuntime<'_, H>
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
        if name == "name_input" { self.handler.on_name_input_change(self.ui, value); return true; }
        if name == "email_input" { self.handler.on_email_input_change(self.ui, value); return true; }
        if name == "age_input" { self.handler.on_age_input_change(self.ui, value); return true; }
        if name == "description_input" { self.handler.on_description_input_change(self.ui, value); return true; }
        if name == "notify_check" { self.handler.on_notify_check_change(self.ui, value); return true; }
        if name == "terms_check" { self.handler.on_terms_check_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name_input" { self.handler.on_name_input_submit(self.ui, value); return true; }
        if name == "email_input" { self.handler.on_email_input_submit(self.ui, value); return true; }
        if name == "age_input" { self.handler.on_age_input_submit(self.ui, value); return true; }
        if name == "description_input" { self.handler.on_description_input_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "country_combo" { self.handler.on_country_combo_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
        if name == "role_listbox" { self.handler.on_role_listbox_selectionchange(self.ui, value); return true; }
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

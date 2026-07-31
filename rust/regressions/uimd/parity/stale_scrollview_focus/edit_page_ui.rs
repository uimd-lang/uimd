// Auto-generated UI code for edit_page - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "edit_fields_scroll_ui.rs"]
mod uimd_dependency_edit_fields_scroll;

pub struct EditPageUI
{
    pub base: uimd::GeneratedWindow,
    pub edit_title: uimd::LabelRef,
    pub back_btn: uimd::ButtonRef,
    pub save_btn: uimd::ButtonRef,
    pub name_label: uimd::LabelRef,
    pub name_input: uimd::TextInputRef,
    pub body_label: uimd::LabelRef,
    pub fields: uimd::ViewHostRef,
    pub status: uimd::LabelRef,
}

impl EditPageUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Edit Page");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "top".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 77, margin_bottom: 0,
            content: "back_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "form".to_string(), cell_width: 0, cell_height: 21,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 75, margin_bottom: 20,
            content: "name_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 14,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "top".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 60, chars_height: 1,
            margin_right: 16, margin_bottom: 0,
            content: "edit_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
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
            element_type: "textinput".to_string(),
            row: 0, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "form".to_string(), cell_width: 0, cell_height: 21,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 73, chars_height: 1,
            margin_right: 1, margin_bottom: 20,
            content: "name_input".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 75,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "top".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "save_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
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
            row: 2, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "form".to_string(), cell_width: 0, cell_height: 21,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 88, chars_height: 1,
            margin_right: 1, margin_bottom: 18,
            content: "status".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 4, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "form".to_string(), cell_width: 0, cell_height: 21,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 88, chars_height: 1,
            margin_right: 1, margin_bottom: 16,
            content: "body_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "viewhost".to_string(),
            row: 5, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 21,
            cell_name: "form".to_string(), cell_width: 0, cell_height: 21,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 88, chars_height: 15,
            margin_right: 1, margin_bottom: 1,
            content: "fields".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("component");
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
        base.set_generated_named_style("top", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    });
        base.set_generated_named_style("edit_title", uimd::Style {
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("form", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("fields", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("save_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "EditPage", "# Edit Page\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: regression\ndescription: \"Edit page shown after a ScrollView row button replaces the active ViewHost page.\"\ntags: [regression, issue-5, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - edit_fields_scroll\n```\n\n## Members\n\n```yaml\nedit_title:\n  type: label\n  text: \"Edit item\"\n  description: \"Edit page title.\"\n  expose: false\n\nback_btn:\n  type: button\n  title: Back\n  description: \"Return to the list page.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Dummy save action used by the regression app.\"\n\nname_label:\n  type: label\n  text: Name\n  description: \"Name field label.\"\n  expose: false\n\nname_input:\n  type: textinput\n  value: \"\"\n  maxlength: 80\n  description: \"Editable item name.\"\n\nbody_label:\n  type: label\n  text: Fields\n  description: \"Fields panel label.\"\n  expose: false\n\nfields:\n  type: viewhost\n  description: \"Scrollable edit fields panel.\"\n\nstatus:\n  type: label\n  text: \"Opened from ScrollView\"\n  description: \"Edit page status.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@top:\n  background: \"#172033\"\n@edit_title:\n  color: \"#ffffff\"\n  text-align: center\n@form:\n  background: \"#111827\"\n  padding: 1, 1, 1, 1\n@fields:\n  background: \"#030712\"\n  focus-background: \"#07111f\"\n  edit-background: \"#0f172a\"\n  scope-dim-background: \"#0000004d\"\n@status:\n  color: \"#93c5fd\"\nlabel:\n  color: \"#e5e7eb\"\ntextinput:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\ntextarea:\n  background: \"#0f172a\"\n  color: \"#e5e7eb\"\n  focus-background: \"#1e293b\"\n  edit-background: \"#111827\"\n  cursor-background: \"#facc15\"\n  cursor-color: \"#111827\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@save_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n```\n\n## User Interface\n\n```ui\n+-top-**-----------------------------------------------------------------------------------+\n| back_btn.... edit_title..**.............................................. save_btn...... |\n+-form-**----------------------------------------------------------------------------------+\n| name_label.... name_input..**........................................................... |\n|                                                                                          |\n| status..**.............................................................................. |\n|                                                                                          |\n| body_label..**.......................................................................... |\n| fields..**.............................................................................. |\n| *....................................................................................... |\n| *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n|                                                                                          |\n+------------------------------------------------------------------------------------------+\n```\n", "Edit page shown after a ScrollView row button replaces the active ViewHost page.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("edit_title", "Edit page title.", false), uimd::GeneratedElementMetadata::new("back_btn", "Return to the list page.", true), uimd::GeneratedElementMetadata::new("save_btn", "Dummy save action used by the regression app.", true), uimd::GeneratedElementMetadata::new("name_label", "Name field label.", false), uimd::GeneratedElementMetadata::new("name_input", "Editable item name.", true), uimd::GeneratedElementMetadata::new("body_label", "Fields panel label.", false), uimd::GeneratedElementMetadata::new("fields", "Scrollable edit fields panel.", true), uimd::GeneratedElementMetadata::new("status", "Edit page status.", true)]);
        base.set_mcp_app_tools(vec![]);
        let edit_title = uimd::new_label("edit_title", "Edit item");
        base.add_element(edit_title.clone());
        edit_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        edit_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let back_btn = uimd::new_button("back_btn", "Back");
        base.add_element(back_btn.clone());
        back_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        back_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        back_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let save_btn = uimd::new_button("save_btn", "Save");
        base.add_element(save_btn.clone());
        save_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        save_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        save_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let name_label = uimd::new_label("name_label", "Name");
        base.add_element(name_label.clone());
        name_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        name_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let name_input = uimd::new_text_input("name_input", "", 80);
        base.add_element(name_input.clone());
        name_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        name_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#1e293b")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        name_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#facc15")),
        color: Some(uimd::Color::new("#111827")),
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
        let body_label = uimd::new_label("body_label", "Fields");
        base.add_element(body_label.clone());
        body_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        body_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let fields = uimd::new_view_host("fields");
        base.add_element(fields.clone());
        fields.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        scope_dim_background: Some(uimd::Color::new("#0000004d")),
        ..Default::default()
    });
        fields.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#07111f")),
        ..Default::default()
    });
        fields.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        ..Default::default()
    });
        let status = uimd::new_label("status", "Opened from ScrollView");
        base.add_element(status.clone());
        status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
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
            edit_title,
            back_btn,
            save_btn,
            name_label,
            name_input,
            body_label,
            fields,
            status,
        }
    }

    pub fn run<H: EditPageUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = EditPageUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for EditPageUI
{
    fn default() -> Self { Self::new() }
}

pub trait EditPageUIEvents
{
    fn on_back_btn_click(&mut self, _ui: &mut EditPageUI) {}
    fn on_save_btn_click(&mut self, _ui: &mut EditPageUI) {}
    fn on_name_input_change(&mut self, _ui: &mut EditPageUI, _value: &str) {}
    fn on_name_input_submit(&mut self, _ui: &mut EditPageUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut EditPageUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut EditPageUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut EditPageUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut EditPageUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut EditPageUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut EditPageUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut EditPageUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut EditPageUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut EditPageUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut EditPageUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct EditPageUIRuntime<'a, H: EditPageUIEvents>
{
    ui: &'a mut EditPageUI,
    handler: &'a mut H,
}

impl<H: EditPageUIEvents> uimd::GeneratedApplication for EditPageUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "back_btn" { self.handler.on_back_btn_click(self.ui); return true; }
        if name == "save_btn" { self.handler.on_save_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name_input" { self.handler.on_name_input_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "name_input" { self.handler.on_name_input_submit(self.ui, value); return true; }
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

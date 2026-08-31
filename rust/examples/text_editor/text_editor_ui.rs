// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TextEditorUI
{
    pub base: uimd::GeneratedWindow,
    pub window_header: uimd::LabelRef,
    pub filename: uimd::LabelRef,
    pub editor: uimd::TextAreaRef,
    pub new_btn: uimd::ButtonRef,
    pub open_btn: uimd::ButtonRef,
    pub save_btn: uimd::ButtonRef,
    pub save_as_btn: uimd::ButtonRef,
    pub quit_btn: uimd::ButtonRef,
    pub status: uimd::LabelRef,
}

impl TextEditorUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Text Editor");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 87, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "window_header".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 18, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 87, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 85, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 85, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 6, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 6, chars_height: 1,
            margin_right: 80, margin_bottom: 2,
            content: "\"file\"".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "textarea".to_string(),
            row: 1, col: 1,
            cell_row: 7, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 10,
            cell_name: "main".to_string(), cell_width: 87, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 86, chars_height: 9,
            margin_right: 0, margin_bottom: 0,
            content: "editor".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style::default(),
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 8,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 78, chars_height: 1,
            margin_right: 1, margin_bottom: 2,
            content: "filename".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 14, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 14, chars_height: 1,
            margin_right: 72, margin_bottom: 0,
            content: "new_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 16,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 15, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 15, chars_height: 1,
            margin_right: 56, margin_bottom: 0,
            content: "open_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 32,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 15, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 15, chars_height: 1,
            margin_right: 40, margin_bottom: 0,
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
            row: 3, col: 48,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 18, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 18, chars_height: 1,
            margin_right: 21, margin_bottom: 0,
            content: "save_as_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 3, col: 67,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 87, cell_chars_height: 4,
            cell_name: "filebar".to_string(), cell_width: 87, cell_height: 4,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: 15, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 15, chars_height: 1,
            margin_right: 5, margin_bottom: 0,
            content: "quit_btn".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
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
        base.set_generated_named_style("filename", uimd::Style {
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TextEditor", "# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: \"Text Editor\"\n  description: \"Application title.\"\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n  description: \"Current file path or file name.\"\n\neditor:\n  type: textarea\n  value: \"\"\n  description: \"Main multiline text editing area.\"\n\nnew_btn:\n  type: button\n  title: New\n  description: \"Create a new empty document, asking about unsaved changes when needed.\"\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open the file browser to load a text file, asking about unsaved changes when needed.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Save the current document to its current path.\"\n\nsave_as_btn:\n  type: button\n  title: \"Save As\"\n  description: \"Open the file browser in save mode and save the document to a chosen path.\"\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the editor, asking about unsaved changes when needed.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows load, save, cancel, and error messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: \"#bfdbfe\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| \"file\" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n", "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("window_header", "Application title.", false), uimd::GeneratedElementMetadata::new("filename", "Current file path or file name.", true), uimd::GeneratedElementMetadata::new("editor", "Main multiline text editing area.", true), uimd::GeneratedElementMetadata::new("new_btn", "Create a new empty document, asking about unsaved changes when needed.", true), uimd::GeneratedElementMetadata::new("open_btn", "Open the file browser to load a text file, asking about unsaved changes when needed.", true), uimd::GeneratedElementMetadata::new("save_btn", "Save the current document to its current path.", true), uimd::GeneratedElementMetadata::new("save_as_btn", "Open the file browser in save mode and save the document to a chosen path.", true), uimd::GeneratedElementMetadata::new("quit_btn", "Close the editor, asking about unsaved changes when needed.", true), uimd::GeneratedElementMetadata::new("status", "Shows load, save, cancel, and error messages.", true)]);
        base.set_mcp_app_tools(vec![]);
        let window_header = uimd::new_label("window_header", "Text Editor");
        base.add_element(window_header.clone());
        window_header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        color: Some(uimd::Color::new("#cbd5e1")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        window_header.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let filename = uimd::new_label("filename", "welcome.txt");
        base.add_element(filename.clone());
        filename.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#bfdbfe")),
        ..Default::default()
    });
        filename.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let editor = uimd::new_text_area_with_max_length("editor", "", 0);
        base.add_element(editor.clone());
        editor.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        editor.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        editor.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        editor.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        editor.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        editor.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let new_btn = uimd::new_button("new_btn", "New");
        base.add_element(new_btn.clone());
        new_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        new_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        new_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let open_btn = uimd::new_button("open_btn", "Open");
        base.add_element(open_btn.clone());
        open_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        open_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        open_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
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
        let save_as_btn = uimd::new_button("save_as_btn", "Save As");
        base.add_element(save_as_btn.clone());
        save_as_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        save_as_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        save_as_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
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
        let status = uimd::new_label("status", "Ready");
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
            window_header,
            filename,
            editor,
            new_btn,
            open_btn,
            save_btn,
            save_as_btn,
            quit_btn,
            status,
        }
    }

    pub fn run<H: TextEditorUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TextEditorUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TextEditorUI
{
    fn default() -> Self { Self::new() }
}

pub trait TextEditorUIEvents
{
    fn on_new_btn_click(&mut self, _ui: &mut TextEditorUI) {}
    fn on_open_btn_click(&mut self, _ui: &mut TextEditorUI) {}
    fn on_save_btn_click(&mut self, _ui: &mut TextEditorUI) {}
    fn on_save_as_btn_click(&mut self, _ui: &mut TextEditorUI) {}
    fn on_quit_btn_click(&mut self, _ui: &mut TextEditorUI) {}
    fn on_editor_change(&mut self, _ui: &mut TextEditorUI, _value: &str) {}
    fn on_editor_submit(&mut self, _ui: &mut TextEditorUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut TextEditorUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TextEditorUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TextEditorUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TextEditorUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TextEditorUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TextEditorUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut TextEditorUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut TextEditorUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TextEditorUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TextEditorUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TextEditorUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TextEditorUIRuntime<'a, H: TextEditorUIEvents>
{
    ui: &'a mut TextEditorUI,
    handler: &'a mut H,
}

impl<H: TextEditorUIEvents> uimd::GeneratedApplication for TextEditorUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "new_btn" { self.handler.on_new_btn_click(self.ui); return true; }
        if name == "open_btn" { self.handler.on_open_btn_click(self.ui); return true; }
        if name == "save_btn" { self.handler.on_save_btn_click(self.ui); return true; }
        if name == "save_as_btn" { self.handler.on_save_as_btn_click(self.ui); return true; }
        if name == "quit_btn" { self.handler.on_quit_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "editor" { self.handler.on_editor_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "editor" { self.handler.on_editor_submit(self.ui, value); return true; }
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

// Auto-generated UI code for compatibility - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "../source/compat_scroll_ui.rs"]
mod uimd_dependency_compat_scroll;

pub struct CompatibilityUI
{
    pub base: uimd::GeneratedWindow,
    pub aligned_input: uimd::TextInputRef,
    pub choice: uimd::ComboBoxRef,
    pub scroller: uimd::ReusableElementRef,
}

impl CompatibilityUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("UIMD 0.5.3 Compatibility Application");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "textinput".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 32, cell_chars_height: 5,
            cell_name: "".to_string(), cell_width: 32, cell_height: 5,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 32, chars_height: 1,
            margin_right: 0, margin_bottom: 4,
            content: "aligned_input".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        text_align: Some("right".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "combobox".to_string(),
            row: 1, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 32, cell_chars_height: 5,
            cell_name: "".to_string(), cell_width: 32, cell_height: 5,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 32, chars_height: 2,
            margin_right: 0, margin_bottom: 2,
            content: "choice".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "compat_scroll".to_string(),
            row: 3, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 32, cell_chars_height: 5,
            cell_name: "".to_string(), cell_width: 32, cell_height: 5,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 32, chars_height: 2,
            margin_right: 0, margin_bottom: 0,
            content: "scroller".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#303545")),
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
        base.set_generated_named_style("aligned_input", uimd::Style {
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("scroller", uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "Compatibility", "# UIMD 0.5.3 Compatibility Application\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Retained generated application for patch-level runtime compatibility.\"\ntags: [compatibility]\n```\n\n## Definition\n\n```yaml\nuses:\n  - compat_scroll\n```\n\n## Members\n\n```yaml\naligned_input:\n  type: textinput\n  value: \"abc\"\n  maxlength: 16\n\nchoice:\n  type: combobox\n  options: [one, two, three, four, five, six, seven, eight]\n  selected_item: one\n\nscroller:\n  type: compat_scroll\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n  background: \"#303545\"\n@aligned_input:\n  text-align: right\n@scroller:\n  background: \"#303545\"\n  focus-background: \"#ffffff14\"\n```\n\n## User Interface\n\n```ui\n+--------------------------------+\n|aligned_input...................|\n|choice..........................|\n|................................|\n|scroller........................|\n|................................|\n+--------------------------------+\n```\n", "Retained generated application for patch-level runtime compatibility.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("aligned_input", "aligned_input", true), uimd::GeneratedElementMetadata::new("choice", "choice", true), uimd::GeneratedElementMetadata::new("scroller", "scroller", true)]);
        base.set_mcp_app_tools(vec![]);
        let aligned_input = uimd::new_text_input("aligned_input", "abc", 16);
        base.add_element(aligned_input.clone());
        aligned_input.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        aligned_input.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        aligned_input.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        aligned_input.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff")),
        color: Some(uimd::Color::new("#0e1117")),
        ..Default::default()
    });
        aligned_input.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        aligned_input.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let choice = uimd::new_combo_box("choice", vec!["one".to_string(), "two".to_string(), "three".to_string(), "four".to_string(), "five".to_string(), "six".to_string(), "seven".to_string(), "eight".to_string()]);
        base.add_element(choice.clone());
        choice.borrow_mut().set_selected_index(0);
        choice.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0d1524")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        choice.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#243a5c")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        choice.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#2d456d")),
        color: Some(uimd::Color::new("#e5e7eb")),
        ..Default::default()
    });
        choice.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        choice.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#0b1220")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let scroller = uimd::new_reusable_element("scroller", "compat_scroll");
        base.add_element(scroller.clone());
        scroller.borrow_mut().set_child_window(uimd_dependency_compat_scroll::CompatScrollUI::new().base);
        scroller.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#303545")),
        ..Default::default()
    });
        scroller.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        scroller.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        scroller.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        Self
        {
            base,
            aligned_input,
            choice,
            scroller,
        }
    }

    pub fn run<H: CompatibilityUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = CompatibilityUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for CompatibilityUI
{
    fn default() -> Self { Self::new() }
}

pub trait CompatibilityUIEvents
{
    fn on_aligned_input_change(&mut self, _ui: &mut CompatibilityUI, _value: &str) {}
    fn on_aligned_input_submit(&mut self, _ui: &mut CompatibilityUI, _value: &str) {}
    fn on_choice_change(&mut self, _ui: &mut CompatibilityUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut CompatibilityUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut CompatibilityUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut CompatibilityUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut CompatibilityUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut CompatibilityUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut CompatibilityUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut CompatibilityUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut CompatibilityUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut CompatibilityUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut CompatibilityUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct CompatibilityUIRuntime<'a, H: CompatibilityUIEvents>
{
    ui: &'a mut CompatibilityUI,
    handler: &'a mut H,
}

impl<H: CompatibilityUIEvents> uimd::GeneratedApplication for CompatibilityUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "aligned_input" { self.handler.on_aligned_input_change(self.ui, value); return true; }
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "aligned_input" { self.handler.on_aligned_input_submit(self.ui, value); return true; }
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        if name == "choice" { self.handler.on_choice_change(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }
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

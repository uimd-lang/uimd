// Auto-generated UI code for stale_scrollview_focus - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "list_page_ui.rs"]
mod uimd_dependency_list_page;
#[path = "edit_page_ui.rs"]
mod uimd_dependency_edit_page;

pub struct StaleScrollviewFocusUI
{
    pub base: uimd::GeneratedWindow,
    pub title: uimd::LabelRef,
    pub quit_btn: uimd::ButtonRef,
    pub tab_list: uimd::ButtonRef,
    pub tab_other: uimd::ButtonRef,
    pub hint: uimd::LabelRef,
    pub page: uimd::ViewHostRef,
}

impl StaleScrollviewFocusUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Stale ScrollView Focus");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "viewhost".to_string(),
            row: 0, col: 1,
            cell_row: 4, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 29,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 88, chars_height: 29,
            margin_right: 1, margin_bottom: 0,
            content: "page".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "nav".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 16, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 16, chars_height: 1,
            margin_right: 73, margin_bottom: 0,
            content: "tab_list".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 75, chars_height: 1,
            margin_right: 14, margin_bottom: 0,
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
            row: 0, col: 18,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "nav".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 17, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 17, chars_height: 1,
            margin_right: 55, margin_bottom: 0,
            content: "tab_other".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 36,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
            cell_name: "nav".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 53, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "hint".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 77,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 90, cell_chars_height: 1,
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
        base.set_generated_named_style("nav", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("hint", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_generated_named_style("tab_list", uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        ..Default::default()
    });
        base.set_generated_named_style("tab_other", uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "StaleScrollviewFocus", "# Stale ScrollView Focus\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.\"\ntags: [regression, issue-5, viewhost, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - list_page\n  - edit_page\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Scroll Focus Repro\"\n  description: \"Regression window title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the regression app.\"\n\ntab_list:\n  type: button\n  title: List\n  description: \"Show the list page.\"\n\ntab_other:\n  type: button\n  title: Other\n  description: \"Inactive top-level navigation button used to expose stale focus.\"\n\nhint:\n  type: label\n  text: \"Focus list, press Enter, choose Open.\"\n  description: \"Manual reproduction hint.\"\n  expose: false\n\npage:\n  type: viewhost\n  description: \"Dynamic page host.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@hint:\n  color: \"#93c5fd\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@tab_list:\n  background: \"#2f80ed\"\n  focus-background: \"#22c55e\"\n@tab_other:\n  background: \"#343b4b\"\n  focus-background: \"#9333ea\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------------------+\n| title..**.................................................................. quit_btn.... |\n+-nav-**-----------------------------------------------------------------------------------+\n| tab_list........ tab_other........ hint..**............................................. |\n+-content-**-------------------------------------------------------------------------------+\n| page..**................................................................................ |\n* *....................................................................................... |\n* *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+------------------------------------------------------------------------------------------+\n```\n", "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title", "Regression window title.", false), uimd::GeneratedElementMetadata::new("quit_btn", "Close the regression app.", true), uimd::GeneratedElementMetadata::new("tab_list", "Show the list page.", true), uimd::GeneratedElementMetadata::new("tab_other", "Inactive top-level navigation button used to expose stale focus.", true), uimd::GeneratedElementMetadata::new("hint", "Manual reproduction hint.", false), uimd::GeneratedElementMetadata::new("page", "Dynamic page host.", true)]);
        base.set_mcp_app_tools(vec![]);
        let title = uimd::new_label("title", "Scroll Focus Repro");
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
        let tab_list = uimd::new_button("tab_list", "List");
        base.add_element(tab_list.clone());
        tab_list.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#2f80ed")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        tab_list.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#22c55e")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        tab_list.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let tab_other = uimd::new_button("tab_other", "Other");
        base.add_element(tab_other.clone());
        tab_other.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#343b4b")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        tab_other.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#9333ea")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        tab_other.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let hint = uimd::new_label("hint", "Focus list, press Enter, choose Open.");
        base.add_element(hint.clone());
        hint.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        hint.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let page = uimd::new_view_host("page");
        base.add_element(page.clone());
        page.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        Self
        {
            base,
            title,
            quit_btn,
            tab_list,
            tab_other,
            hint,
            page,
        }
    }

    pub fn run<H: StaleScrollviewFocusUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = StaleScrollviewFocusUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for StaleScrollviewFocusUI
{
    fn default() -> Self { Self::new() }
}

pub trait StaleScrollviewFocusUIEvents
{
    fn on_quit_btn_click(&mut self, _ui: &mut StaleScrollviewFocusUI) {}
    fn on_tab_list_click(&mut self, _ui: &mut StaleScrollviewFocusUI) {}
    fn on_tab_other_click(&mut self, _ui: &mut StaleScrollviewFocusUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut StaleScrollviewFocusUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut StaleScrollviewFocusUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut StaleScrollviewFocusUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut StaleScrollviewFocusUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct StaleScrollviewFocusUIRuntime<'a, H: StaleScrollviewFocusUIEvents>
{
    ui: &'a mut StaleScrollviewFocusUI,
    handler: &'a mut H,
}

impl<H: StaleScrollviewFocusUIEvents> uimd::GeneratedApplication for StaleScrollviewFocusUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "quit_btn" { self.handler.on_quit_btn_click(self.ui); return true; }
        if name == "tab_list" { self.handler.on_tab_list_click(self.ui); return true; }
        if name == "tab_other" { self.handler.on_tab_other_click(self.ui); return true; }
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

    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool
    {
        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)
    }

    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

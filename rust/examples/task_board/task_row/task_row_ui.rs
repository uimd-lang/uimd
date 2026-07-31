// Auto-generated UI code for task_row - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct TaskRowUI
{
    pub base: uimd::GeneratedWindow,
    pub task_title: uimd::LabelRef,
    pub meta: uimd::LabelRef,
    pub done: uimd::CheckBoxRef,
    pub spacer: uimd::LabelRef,
    pub open_btn: uimd::ButtonRef,
    pub delete_btn: uimd::ButtonRef,
}

impl TaskRowUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Task Row");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 21, chars_height: 1,
            margin_right: 25, margin_bottom: 1,
            content: "task_title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 23,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 23, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 23, chars_height: 1,
            margin_right: 1, margin_bottom: 1,
            content: "meta".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 1, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 8, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 8, chars_height: 1,
            margin_right: 38, margin_bottom: 0,
            content: "done".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
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
            row: 1, col: 10,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 25, margin_bottom: 0,
            content: "spacer".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 23,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 14, margin_bottom: 0,
            content: "open_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 34,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 47, cell_chars_height: 2,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 2,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "delete_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("component");
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
        base.set_generated_named_style("card", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("task_title", uimd::Style {
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("meta", uimd::Style {
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("delete_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "TaskRow", "# Task Row\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\nstatus: draft\ndescription: \"Reusable task row with editable controls.\"\ntags: [component, example]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\ntask_title:\n  type: label\n  text: \"Task\"\n  description: \"Task title.\"\n\nmeta:\n  type: label\n  text: \"Status / assignee / priority\"\n  description: \"Task status, assignee, and priority.\"\n\ndone:\n  type: checkbox\n  title: Done\n  value: false\n  description: \"Whether the task is done.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible row spacer before task actions.\"\n  expose: false\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open this task in the edit dialog.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Ask for confirmation and delete this task.\"\n```\n\n## Style\n\n```yaml\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\n@task_title:\n  color: \"#f8fafc\"\n  user-select: text\n@meta:\n  color: \"#93c5fd\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------+\n| task_title..**....... meta................... |\n| done.... spacer..**.. open_btn.. delete_btn.. |\n+-----------------------------------------------+\n```\n", "Reusable task row with editable controls.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("task_title", "Task title.", true), uimd::GeneratedElementMetadata::new("meta", "Task status, assignee, and priority.", true), uimd::GeneratedElementMetadata::new("done", "Whether the task is done.", true), uimd::GeneratedElementMetadata::new("spacer", "Flexible row spacer before task actions.", false), uimd::GeneratedElementMetadata::new("open_btn", "Open this task in the edit dialog.", true), uimd::GeneratedElementMetadata::new("delete_btn", "Ask for confirmation and delete this task.", true)]);
        base.set_mcp_app_tools(vec![]);
        let task_title = uimd::new_label("task_title", "Task");
        base.add_element(task_title.clone());
        task_title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        task_title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let meta = uimd::new_label("meta", "Status / assignee / priority");
        base.add_element(meta.clone());
        meta.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#93c5fd")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        meta.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let done = uimd::new_checkbox("done", "Done", false);
        base.add_element(done.clone());
        done.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        done.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#facc15")),
        ..Default::default()
    });
        done.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        done.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#94a3b8")),
        ..Default::default()
    });
        done.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let spacer = uimd::new_label("spacer", "");
        base.add_element(spacer.clone());
        spacer.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        spacer.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
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
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        open_btn.borrow_mut().set_disabled_style(uimd::Style {
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
        Self
        {
            base,
            task_title,
            meta,
            done,
            spacer,
            open_btn,
            delete_btn,
        }
    }

    pub fn run<H: TaskRowUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = TaskRowUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for TaskRowUI
{
    fn default() -> Self { Self::new() }
}

pub trait TaskRowUIEvents
{
    fn on_open_btn_click(&mut self, _ui: &mut TaskRowUI) {}
    fn on_delete_btn_click(&mut self, _ui: &mut TaskRowUI) {}
    fn on_done_change(&mut self, _ui: &mut TaskRowUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut TaskRowUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut TaskRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut TaskRowUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut TaskRowUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut TaskRowUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut TaskRowUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut TaskRowUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut TaskRowUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut TaskRowUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut TaskRowUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct TaskRowUIRuntime<'a, H: TaskRowUIEvents>
{
    ui: &'a mut TaskRowUI,
    handler: &'a mut H,
}

impl<H: TaskRowUIEvents> uimd::GeneratedApplication for TaskRowUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "open_btn" { self.handler.on_open_btn_click(self.ui); return true; }
        if name == "delete_btn" { self.handler.on_delete_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "done" { self.handler.on_done_change(self.ui, value); return true; }
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

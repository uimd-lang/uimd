// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "document_view/document_view_ui.rs"]
mod uimd_dependency_document_view;

pub struct MarkdownViewerUI
{
    pub base: uimd::GeneratedWindow,
    pub header: uimd::LabelRef,
    pub docs_label: uimd::LabelRef,
    pub docs: uimd::ListBoxRef,
    pub up_btn: uimd::ButtonRef,
    pub down_btn: uimd::ButtonRef,
    pub viewer: uimd::ReusableElementRef,
    pub close_btn: uimd::ButtonRef,
    pub status: uimd::LabelRef,
}

impl MarkdownViewerUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Markdown Viewer");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 0,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 67, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 58, chars_height: 1,
            margin_right: 9, margin_bottom: 0,
            content: "header".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#223a5a")),
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
            element_type: "document_view".to_string(),
            row: 0, col: 0,
            cell_row: 2, cell_col: 27,
            cell_chars_width: 40, cell_chars_height: 11,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 11,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 40, chars_height: 11,
            margin_right: 0, margin_bottom: 0,
            content: "viewer".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#000000")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#000000")),
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(1),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 14, cell_col: 0,
            cell_chars_width: 67, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 67, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 65, chars_height: 1,
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
            row: 0, col: 58,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 67, cell_chars_height: 1,
            cell_name: "title".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 9, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 9, chars_height: 1,
            margin_right: 0, margin_bottom: 0,
            content: "close_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#223a5a")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 11,
            cell_name: "sidebar".to_string(), cell_width: 26, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 24, chars_height: 1,
            margin_right: 1, margin_bottom: 9,
            content: "docs_label".to_string(),
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
            element_type: "listbox".to_string(),
            row: 2, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 11,
            cell_name: "sidebar".to_string(), cell_width: 26, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::FIT_CONTENT,
            width_mode: "expanded".to_string(), height_mode: "fit-content".to_string(),
            chars_width: 24, chars_height: 4,
            margin_right: 1, margin_bottom: 5,
            content: "docs".to_string(),
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
            row: 7, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 11,
            cell_name: "sidebar".to_string(), cell_width: 26, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: 10, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 10, chars_height: 1,
            margin_right: 15, margin_bottom: 3,
            content: "up_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
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
            row: 7, col: 12,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 26, cell_chars_height: 11,
            cell_name: "sidebar".to_string(), cell_width: 26, cell_height: 0,
            cell_width_mode: "auto".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 13, chars_height: 1,
            margin_right: 1, margin_bottom: 3,
            content: "down_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
        ..Default::default()
    },
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
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("title", uimd::Style {
        background: Some(uimd::Color::new("#223a5a")),
        ..Default::default()
    });
        base.set_generated_named_style("sidebar", uimd::Style {
        background: Some(uimd::Color::new("#101a2b")),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        base.set_generated_named_style("viewer", uimd::Style {
        background: Some(uimd::Color::new("#000000")),
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("docs", uimd::Style {
        background: Some(uimd::Color::new("#ffffff11")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        base.set_generated_named_style("close_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        color: Some(uimd::Color::new("#93c5fd")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "MarkdownViewer", "# Markdown Viewer\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - document_view\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Markdown Viewer\"\n  description: \"Application title.\"\n  expose: false\n\ndocs_label:\n  type: label\n  text: Documents\n  description: \"Label for the document list.\"\n\ndocs:\n  type: listbox\n  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]\n  selected_items: [overview.markdown]\n  multiple: \"false\"\n  description: \"Document listbox used to select the markdown document to render.\"\n\nup_btn:\n  type: button\n  title: up\n  description: \"Scroll the rendered document upward.\"\n\ndown_btn:\n  type: button\n  title: down\n  description: \"Scroll the rendered document downward.\"\n\nviewer:\n  type: document_view\n  cpp-class: \"markdown_viewer_example::DocumentView\"\n  cpp-header: document_view/document_view.hpp\n  description: \"Scrollable rendered markdown document area.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the markdown viewer.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows the current document and scroll state.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  color: \"#ffffff\"\n@title:\n  background: \"#223a5a\"\n@sidebar:\n  background: \"#101a2b\"\n@content:\n  background: \"#000000\"\n@viewer:\n  background: \"#000000\"\n  padding: 1, 2, 1, 2\n@docs:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2b5f9f\"\n@close_btn:\n  background: \"#7f1d1d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n  background: \"#111827\"\n```\n\n## User Interface\n\n```ui\n+-title-**----------------------------------------------------------+\n|header..**................................................close_btn|\n+-sidebar------------------+-content-**-----------------------------+\n|                          |viewer..**..............................|\n| docs_label...**......... |........................................|\n| docs.........**......... |*.......................................|\n| ........................ |*.......................................|\n| #....................... |........................................|\n| #....................... |........................................|\n|                          |........................................|\n| up_btn.... down_btn..... |........................................|\n*                          |........................................|\n*                          |........................................|\n|                          |........................................|\n+--------------------------+----------------------------------------+\n| status..**....................................................... |\n+-------------------------------------------------------------------+\n```\n", "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("header", "Application title.", false), uimd::GeneratedElementMetadata::new("docs_label", "Label for the document list.", true), uimd::GeneratedElementMetadata::new("docs", "Document listbox used to select the markdown document to render.", true), uimd::GeneratedElementMetadata::new("up_btn", "Scroll the rendered document upward.", true), uimd::GeneratedElementMetadata::new("down_btn", "Scroll the rendered document downward.", true), uimd::GeneratedElementMetadata::new("viewer", "Scrollable rendered markdown document area.", true), uimd::GeneratedElementMetadata::new("close_btn", "Close the markdown viewer.", true), uimd::GeneratedElementMetadata::new("status", "Shows the current document and scroll state.", true)]);
        base.set_mcp_app_tools(vec![]);
        let header = uimd::new_label("header", "Markdown Viewer");
        base.add_element(header.clone());
        header.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#ffffff")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        header.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let docs_label = uimd::new_label("docs_label", "Documents");
        base.add_element(docs_label.clone());
        docs_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        docs_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let docs = uimd::new_list_box("docs", vec!["overview.markdown".to_string(), "styles.markdown".to_string(), "tables.markdown".to_string(), "code.markdown".to_string()], false);
        base.add_element(docs.clone());
        docs.borrow_mut().set_selected_index(0);
        docs.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff11")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        docs.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff22")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        docs.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        docs.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        docs.borrow_mut().set_active_style(uimd::Style {
        background: Some(uimd::Color::new("#DDDDDD99")),
        color: Some(uimd::Color::new("#FFFFFF")),
        ..Default::default()
    });
        docs.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let up_btn = uimd::new_button("up_btn", "up");
        base.add_element(up_btn.clone());
        up_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        up_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        up_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let down_btn = uimd::new_button("down_btn", "down");
        base.add_element(down_btn.clone());
        down_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        down_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2b5f9f")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        down_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let viewer = uimd::new_reusable_element("viewer", "document_view");
        base.add_element(viewer.clone());
        viewer.borrow_mut().set_child_window(uimd_dependency_document_view::DocumentViewUI::new().base);
        viewer.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#000000")),
        padding_bottom: Some(1),
        padding_left: Some(2),
        padding_right: Some(2),
        padding_top: Some(1),
        ..Default::default()
    });
        viewer.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        viewer.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        viewer.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
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
            docs_label,
            docs,
            up_btn,
            down_btn,
            viewer,
            close_btn,
            status,
        }
    }

    pub fn run<H: MarkdownViewerUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = MarkdownViewerUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for MarkdownViewerUI
{
    fn default() -> Self { Self::new() }
}

pub trait MarkdownViewerUIEvents
{
    fn on_up_btn_click(&mut self, _ui: &mut MarkdownViewerUI) {}
    fn on_down_btn_click(&mut self, _ui: &mut MarkdownViewerUI) {}
    fn on_close_btn_click(&mut self, _ui: &mut MarkdownViewerUI) {}
    fn on_docs_selectionchange(&mut self, _ui: &mut MarkdownViewerUI, _value: &[String]) {}
    fn handle_dynamic_button(&mut self, _ui: &mut MarkdownViewerUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut MarkdownViewerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut MarkdownViewerUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut MarkdownViewerUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut MarkdownViewerUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut MarkdownViewerUI, _name: &str, _focused: bool) {}
    fn handle_key_before_focused(&mut self, _ui: &mut MarkdownViewerUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut MarkdownViewerUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut MarkdownViewerUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut MarkdownViewerUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct MarkdownViewerUIRuntime<'a, H: MarkdownViewerUIEvents>
{
    ui: &'a mut MarkdownViewerUI,
    handler: &'a mut H,
}

impl<H: MarkdownViewerUIEvents> uimd::GeneratedApplication for MarkdownViewerUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "up_btn" { self.handler.on_up_btn_click(self.ui); return true; }
        if name == "down_btn" { self.handler.on_down_btn_click(self.ui); return true; }
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
        if name == "docs" { self.handler.on_docs_selectionchange(self.ui, value); return true; }
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

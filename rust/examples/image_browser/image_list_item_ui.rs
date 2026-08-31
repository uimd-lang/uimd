// Auto-generated UI code for image_list_item - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};

pub struct ImageListItemUI
{
    pub base: uimd::GeneratedWindow,
    pub sel: uimd::CheckBoxRef,
    pub thumb: uimd::ImageRef,
    pub name_label: uimd::LabelRef,
    pub path_label: uimd::LabelRef,
    pub spacer: uimd::LabelRef,
    pub show_btn: uimd::ButtonRef,
    pub browse_btn: uimd::ButtonRef,
    pub delete_btn: uimd::ButtonRef,
}

impl ImageListItemUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Image List Item");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "checkbox".to_string(),
            row: 1, col: 2,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 3, height: 5,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 3, chars_height: 5,
            margin_right: 66, margin_bottom: 1,
            content: "sel".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "image".to_string(),
            row: 1, col: 6,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 8, height: 5,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 8, chars_height: 5,
            margin_right: 57, margin_bottom: 1,
            content: "thumb".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 1, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 16, chars_height: 2,
            margin_right: 39, margin_bottom: 4,
            content: "name_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
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
            element_type: "button".to_string(),
            row: 1, col: 34,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 5,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 5,
            margin_right: 26, margin_bottom: 1,
            content: "show_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 1, col: 46,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 5,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 5,
            margin_right: 14, margin_bottom: 1,
            content: "browse_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
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
            row: 1, col: 58,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 11, height: 5,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 11, chars_height: 5,
            margin_right: 2, margin_bottom: 1,
            content: "delete_btn".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
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
            row: 3, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 2,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 53, chars_height: 2,
            margin_right: 2, margin_bottom: 2,
            content: "path_label".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 5, col: 16,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 71, cell_chars_height: 7,
            cell_name: "card".to_string(), cell_width: 0, cell_height: 7,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 53, chars_height: 1,
            margin_right: 2, margin_bottom: 1,
            content: "spacer".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
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
        padding_bottom: Some(0),
        padding_left: Some(0),
        padding_right: Some(0),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("name_label", uimd::Style {
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("path_label", uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("show_btn", uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        ..Default::default()
    });
        base.set_generated_named_style("delete_btn", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "ImageListItem", "# Image List Item\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: component\ndescription: \"Reusable row for one image item.\"\ntags: [example, image, list]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\n```\n\n## Members\n\n```yaml\nsel:\n  type: checkbox\n  title: \"\"\n  value: false\n  description: \"Select this image item.\"\n\nthumb:\n  type: image\n  source: \"\"\n  alt: \"\"\n  fit: contain\n  render_mode: auto\n  description: \"Small image thumbnail.\"\n\nname_label:\n  type: label\n  text: \"\"\n  description: \"Image file name.\"\n\npath_label:\n  type: label\n  text: \"\"\n  description: \"Full image path.\"\n\nspacer:\n  type: label\n  text: \"\"\n  description: \"Flexible spacer before item actions.\"\n  expose: false\n\nshow_btn:\n  type: button\n  title: Show\n  description: \"Show this image in a dialog.\"\n\nbrowse_btn:\n  type: button\n  title: Browse\n  description: \"Choose a replacement image.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete this image item after confirmation.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@card:\n  background: \"#172033\"\n  padding: 0, 0, 0, 0\n@name_label:\n  color: \"#f8fafc\"\n  user-select: text\n@path_label:\n  color: \"#64748b\"\n  user-select: text\ncheckbox:\n  color: \"#cbd5e1\"\n  focus-color: \"#facc15\"\n  checked-color: \"#22c55e\"\n  unchecked-color: \"#94a3b8\"\nimage:\n  background: transparent\n  color: \"#d1d5db\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@show_btn:\n  background: \"#14532d\"\n  focus-background: \"#16a34a\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n```\n\n## User Interface\n\n```ui\n+-card-**---------------------------------------------------------------+\n|                                                                       |\n|  sel thumb...  name_label..**..  show_btn... browse_btn. delete_btn.  |\n|  ... ........  .....................................................  |\n|  ... ........  path_label..**.......................................  |\n|  ... ........  .....................................................  |\n|  ... ........  spacer..**...........................................  |\n|                                                                       |\n+-----------------------------------------------------------------------+\n```\n", "Reusable row for one image item.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("sel", "Select this image item.", true), uimd::GeneratedElementMetadata::new("thumb", "Small image thumbnail.", true), uimd::GeneratedElementMetadata::new("name_label", "Image file name.", true), uimd::GeneratedElementMetadata::new("path_label", "Full image path.", true), uimd::GeneratedElementMetadata::new("spacer", "Flexible spacer before item actions.", false), uimd::GeneratedElementMetadata::new("show_btn", "Show this image in a dialog.", true), uimd::GeneratedElementMetadata::new("browse_btn", "Choose a replacement image.", true), uimd::GeneratedElementMetadata::new("delete_btn", "Delete this image item after confirmation.", true)]);
        base.set_mcp_app_tools(vec![]);
        uimd::require_sixel_for_image_rendering();
        let sel = uimd::new_checkbox("sel", "", false);
        base.add_element(sel.clone());
        sel.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#cbd5e1")),
        ..Default::default()
    });
        sel.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff44")),
        color: Some(uimd::Color::new("#facc15")),
        ..Default::default()
    });
        sel.borrow_mut().set_checked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#22c55e")),
        ..Default::default()
    });
        sel.borrow_mut().set_unchecked_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#94a3b8")),
        ..Default::default()
    });
        sel.borrow_mut().set_disabled_style(uimd::Style {
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let thumb = uimd::new_image("thumb", "", "", "contain", "auto", "center", "middle");
        base.add_element(thumb.clone());
        thumb.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#d1d5db")),
        ..Default::default()
    });
        let name_label = uimd::new_label("name_label", "");
        base.add_element(name_label.clone());
        name_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        name_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let path_label = uimd::new_label("path_label", "");
        base.add_element(path_label.clone());
        path_label.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#64748b")),
        user_select: Some("text".to_string()),
        ..Default::default()
    });
        path_label.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
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
        let show_btn = uimd::new_button("show_btn", "Show");
        base.add_element(show_btn.clone());
        show_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#14532d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        show_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#16a34a")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        show_btn.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let browse_btn = uimd::new_button("browse_btn", "Browse");
        base.add_element(browse_btn.clone());
        browse_btn.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        browse_btn.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        browse_btn.borrow_mut().set_disabled_style(uimd::Style {
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
            sel,
            thumb,
            name_label,
            path_label,
            spacer,
            show_btn,
            browse_btn,
            delete_btn,
        }
    }

    pub fn run<H: ImageListItemUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = ImageListItemUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for ImageListItemUI
{
    fn default() -> Self { Self::new() }
}

pub trait ImageListItemUIEvents
{
    fn on_thumb_click(&mut self, _ui: &mut ImageListItemUI) {}
    fn on_show_btn_click(&mut self, _ui: &mut ImageListItemUI) {}
    fn on_browse_btn_click(&mut self, _ui: &mut ImageListItemUI) {}
    fn on_delete_btn_click(&mut self, _ui: &mut ImageListItemUI) {}
    fn on_sel_change(&mut self, _ui: &mut ImageListItemUI, _value: &str) {}
    fn handle_dynamic_button(&mut self, _ui: &mut ImageListItemUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut ImageListItemUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut ImageListItemUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut ImageListItemUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut ImageListItemUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut ImageListItemUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut ImageListItemUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut ImageListItemUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut ImageListItemUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut ImageListItemUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut ImageListItemUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct ImageListItemUIRuntime<'a, H: ImageListItemUIEvents>
{
    ui: &'a mut ImageListItemUI,
    handler: &'a mut H,
}

impl<H: ImageListItemUIEvents> uimd::GeneratedApplication for ImageListItemUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "thumb" { self.handler.on_thumb_click(self.ui); return true; }
        if name == "show_btn" { self.handler.on_show_btn_click(self.ui); return true; }
        if name == "browse_btn" { self.handler.on_browse_btn_click(self.ui); return true; }
        if name == "delete_btn" { self.handler.on_delete_btn_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        if name == "sel" { self.handler.on_sel_change(self.ui, value); return true; }
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

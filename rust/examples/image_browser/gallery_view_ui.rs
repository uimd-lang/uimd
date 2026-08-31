// Auto-generated UI code for gallery_view - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "gallery_scroll_ui.rs"]
mod uimd_dependency_gallery_scroll;

pub struct GalleryViewUI
{
    pub base: uimd::GeneratedWindow,
    pub gallery_scroll: uimd::ReusableElementRef,
}

impl GalleryViewUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Gallery View");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "gallery_scroll".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 27, cell_chars_height: 6,
            cell_name: "view".to_string(), cell_width: 0, cell_height: 6,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 25, chars_height: 6,
            margin_right: 1, margin_bottom: 0,
            content: "gallery_scroll".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style::default(),
        },
    ]);
        base.set_generated_kind("control");
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
        base.set_mcp_metadata(true, "GalleryView", "# Gallery View\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: control\ndescription: \"Gallery view hosting a scrollable list of full-size images.\"\ntags: [example, image, gallery, viewhost]\n```\n\n## Definition\n\n```yaml\nextends: uicontrol\nuses:\n  - gallery_scroll\n```\n\n## Members\n\n```yaml\ngallery_scroll:\n  type: gallery_scroll\n  description: \"Scrollable image list.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-view-**-------------------+\n| gallery_scroll..**....... |\n| ......................... |\n| *........................ |\n| *........................ |\n| ......................... |\n| ......................... |\n+---------------------------+\n```\n", "Gallery view hosting a scrollable list of full-size images.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("gallery_scroll", "Scrollable image list.", true)]);
        base.set_mcp_app_tools(vec![]);
        let gallery_scroll = uimd::new_reusable_element("gallery_scroll", "gallery_scroll");
        base.add_element(gallery_scroll.clone());
        gallery_scroll.borrow_mut().set_child_window(uimd_dependency_gallery_scroll::GalleryScrollUI::new().base);
        gallery_scroll.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        gallery_scroll.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        gallery_scroll.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        Self
        {
            base,
            gallery_scroll,
        }
    }

    pub fn run<H: GalleryViewUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = GalleryViewUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for GalleryViewUI
{
    fn default() -> Self { Self::new() }
}

pub trait GalleryViewUIEvents
{
    fn handle_dynamic_button(&mut self, _ui: &mut GalleryViewUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut GalleryViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut GalleryViewUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut GalleryViewUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut GalleryViewUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut GalleryViewUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut GalleryViewUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut GalleryViewUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut GalleryViewUI, _key: &str) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut GalleryViewUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut GalleryViewUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct GalleryViewUIRuntime<'a, H: GalleryViewUIEvents>
{
    ui: &'a mut GalleryViewUI,
    handler: &'a mut H,
}

impl<H: GalleryViewUIEvents> uimd::GeneratedApplication for GalleryViewUIRuntime<'_, H>
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

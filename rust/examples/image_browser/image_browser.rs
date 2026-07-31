mod gallery_item_ui;
mod gallery_mosaic_ui;
mod gallery_scroll_ui;
mod gallery_view_ui;
mod image_browser_ui;
mod image_button_ui;
mod image_list_button_ui;
mod image_list_item_ui;
mod image_list_scroll_ui;
mod image_list_view_ui;
mod image_show_dialog_ui;
mod image_view_ui;

use gallery_item_ui::GalleryItemUI;
use gallery_mosaic_ui::GalleryMosaicUI;
use gallery_scroll_ui::GalleryScrollUI;
use gallery_view_ui::GalleryViewUI;
use image_browser_ui::*;
use image_button_ui::ImageButtonUI;
use image_list_button_ui::ImageListButtonUI;
use image_list_item_ui::ImageListItemUI;
use image_list_scroll_ui::ImageListScrollUI;
use image_list_view_ui::ImageListViewUI;
use image_show_dialog_ui::ImageShowDialogUI;
use image_view_ui::ImageViewUI;
use std::path::{Path, PathBuf};

const INITIAL_LIST_COUNT: usize = 20;
const FIRST_GENERATED_SAMPLE_INDEX: i32 = 6;
const LAST_GENERATED_SAMPLE_INDEX: i32 = 50;
const IMAGE_SAMPLE_DIR: &str = "shared/assets/image_samples";
const IMAGE_EXTENSION_FILTER: &str = "(png|jpg|jpeg|gif|bmp|tga|ppm|pgm)";
const THUMB_NORMAL_BACKGROUND: &str = "transparent";
const THUMB_SELECTED_BACKGROUND: &str = "#2563eb";
const MOSAIC_NORMAL_BACKGROUND: &str = "#172033";
const RENDER_MODE_SIXEL: &str = "sixel";
const RENDER_MODE_FALLBACK: &str = "fallback";

#[derive(Clone, Debug)]
struct ImageEntry
{
    caption: String,
    source: String,
}

#[derive(Clone, Debug)]
struct ImageRecord
{
    name: String,
    source: String,
    selected: bool,
}

#[derive(Clone)]
enum CurrentView
{
    Image { photo: uimd::ElementRef },
    Gallery { scroll: uimd::ElementRef },
    List { items: uimd::ElementRef },
}

struct ImageBrowserApp
{
    records: Vec<ImageRecord>,
    render_mode: String,
    current_view: Option<CurrentView>,
    browser: Option<uimd::FileBrowser>,
    delete_dialog: Option<uimd::MessageBoxYesNo>,
    pending_browse_index: Option<usize>,
    pending_delete_index: Option<usize>,
}

impl Default for ImageBrowserApp
{
    fn default() -> Self
    {
        Self
        {
            records: initial_image_records(),
            render_mode: RENDER_MODE_SIXEL.to_string(),
            current_view: None,
            browser: None,
            delete_dialog: None,
            pending_browse_index: None,
            pending_delete_index: None,
        }
    }
}

impl ImageBrowserApp
{
    fn initialize(&mut self, ui: &mut ImageBrowserUI)
    {
        ui.base.set_runtime_options(uimd::GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "camera_thumb".to_string(),
            ..Default::default()
        });
        let sidebar = sidebar_images();
        for (index, host) in [&ui.camera_thumb, &ui.astro_thumb, &ui.coffee_thumb]
            .into_iter()
            .enumerate()
        {
            let mut button = ImageButtonUI::new();
            button.base.set_generated_control_activation("photo");
            {
                let mut photo = button.photo.borrow_mut();
                photo.set_source(&sidebar[index].source);
                photo.set_alt(&sidebar[index].caption);
                photo.set_render_mode(&self.render_mode);
                photo.focusable = false;
            }
            host.borrow_mut().set_child_window(button.base);
        }

        let mut mosaic = GalleryMosaicUI::new();
        mosaic.base.set_generated_control_activation("gal_1");
        for image in [
            &mosaic.gal_1,
            &mosaic.gal_2,
            &mosaic.gal_3,
            &mosaic.gal_4,
            &mosaic.gal_5,
        ]
        {
            let mut image = image.borrow_mut();
            image.set_render_mode(&self.render_mode);
            image.focusable = false;
        }
        ui.gallery_mosaic.borrow_mut().set_child_window(mosaic.base);

        let previews = image_library();
        let mut list_button = ImageListButtonUI::new();
        list_button.base.set_generated_control_activation("prw_1");
        for (index, image) in [&list_button.prw_1, &list_button.prw_2, &list_button.prw_3]
            .into_iter()
            .enumerate()
        {
            let mut image = image.borrow_mut();
            image.set_source(&previews[index].source);
            image.set_alt(&previews[index].caption);
            image.set_render_mode(&self.render_mode);
            image.focusable = false;
        }
        ui.image_list_btn
            .borrow_mut()
            .set_child_window(list_button.base);
        self.show_image(ui, 0);
    }

    fn show_image(&mut self, ui: &mut ImageBrowserUI, index: usize)
    {
        let sidebar = sidebar_images();
        let Some(image) = sidebar.get(index) else { return };
        self.select_sidebar(ui, Some(index), false, false);
        let view = ImageViewUI::new();
        view.caption.borrow_mut().set_text(&image.caption);
        {
            let mut photo = view.photo.borrow_mut();
            photo.set_source(&image.source);
            photo.set_alt(&image.caption);
            photo.set_render_mode(&self.render_mode);
        }
        let photo = view.photo.clone();
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::Image { photo: photo.into() });
    }

    fn show_gallery(&mut self, ui: &mut ImageBrowserUI)
    {
        self.select_sidebar(ui, None, true, false);
        let view = GalleryViewUI::new();
        let scroll = GalleryScrollUI::new();
        view.gallery_scroll.borrow_mut().set_child_window(scroll.base);
        let scroll_host = view.gallery_scroll.clone();
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::Gallery
        {
            scroll: scroll_host.into(),
        });
        self.populate_gallery();
    }

    fn show_image_list(&mut self, ui: &mut ImageBrowserUI)
    {
        self.select_sidebar(ui, None, false, true);
        let view = ImageListViewUI::new();
        let scroll = ImageListScrollUI::new();
        view.items.borrow_mut().set_child_window(scroll.base);
        let items = view.items.clone();
        ui.main.borrow_mut().set_child_window(view.base);
        self.current_view = Some(CurrentView::List { items: items.into() });
        self.refresh_items(false, false);
    }

    fn populate_gallery(&self)
    {
        let Some(CurrentView::Gallery { scroll }) = &self.current_view else { return };
        let mut host = scroll.borrow_mut();
        let Some(scroll) = host.child_window_mut() else { return };
        scroll.clear_children();
        for (index, image) in image_library().into_iter().take(5).enumerate()
        {
            let item = GalleryItemUI::new();
            item.caption.borrow_mut().set_text(image.caption);
            {
                let mut photo = item.photo.borrow_mut();
                photo.set_source(image.source);
                photo.set_render_mode(&self.render_mode);
            }
            scroll.add_named_child_window(
                format!("main.gallery_scroll[{index}]"),
                item.base,
            );
        }
        scroll.set_auto_scroll(false);
    }

    fn refresh_items(&self, scroll_to_end: bool, preserve_scroll: bool)
    {
        let Some(CurrentView::List { items }) = &self.current_view else { return };
        let viewport = {
            let host = items.borrow();
            uimd::Size
            {
                width: host.frame.width.max(1),
                height: host.frame.height.max(1),
            }
        };
        let mut host = items.borrow_mut();
        let Some(scroll) = host.child_window_mut() else { return };
        let saved_position = scroll.scroll_position();
        scroll.clear_children();
        for (index, record) in self.records.iter().enumerate()
        {
            let row = ImageListItemUI::new();
            {
                let mut thumb = row.thumb.borrow_mut();
                thumb.set_source(&record.source);
                thumb.set_alt(&record.name);
                thumb.set_fit("contain");
                thumb.set_render_mode(&self.render_mode);
                thumb.focusable = false;
            }
            row.name_label.borrow_mut().set_text(&record.name);
            row.path_label
                .borrow_mut()
                .set_text(image_display_path(&record.source));
            row.sel.borrow_mut().set_checked(record.selected);
            scroll.add_named_child_window(format!("main.items[{index}]"), row.base);
        }
        if scroll_to_end
        {
            scroll.scroll_to_bottom();
        }
        else if preserve_scroll
        {
            scroll.restore_scroll_position_in(saved_position, viewport);
        }
        else
        {
            scroll.set_auto_scroll(false);
            scroll.scroll_to_top_in(viewport);
        }
    }

    fn apply_render_mode(&mut self, ui: &mut ImageBrowserUI, mode: &str)
    {
        self.render_mode = mode.to_string();
        for host in [
            &ui.camera_thumb,
            &ui.astro_thumb,
            &ui.coffee_thumb,
            &ui.gallery_mosaic,
            &ui.image_list_btn,
        ]
        {
            set_all_child_image_render_modes(host, mode);
        }
        match self.current_view.clone()
        {
            Some(CurrentView::Image { photo }) =>
            {
                photo.borrow_mut().set_render_mode(mode);
            }
            Some(CurrentView::Gallery { .. }) => self.populate_gallery(),
            Some(CurrentView::List { .. }) => self.refresh_items(false, true),
            None => {}
        }
    }

    fn select_sidebar(
        &self,
        ui: &ImageBrowserUI,
        selected_thumb: Option<usize>,
        gallery_selected: bool,
        list_selected: bool,
    )
    {
        for (index, host) in [&ui.camera_thumb, &ui.astro_thumb, &ui.coffee_thumb]
            .into_iter()
            .enumerate()
        {
            set_child_cell_background(
                host,
                if selected_thumb == Some(index)
                {
                    THUMB_SELECTED_BACKGROUND
                }
                else
                {
                    THUMB_NORMAL_BACKGROUND
                },
            );
        }
        set_child_cell_background(
            &ui.gallery_mosaic,
            if gallery_selected
            {
                THUMB_SELECTED_BACKGROUND
            }
            else
            {
                MOSAIC_NORMAL_BACKGROUND
            },
        );
        set_child_cell_background(
            &ui.image_list_btn,
            if list_selected
            {
                THUMB_SELECTED_BACKGROUND
            }
            else
            {
                MOSAIC_NORMAL_BACKGROUND
            },
        );
    }

    fn show_record(&mut self, ui: &mut ImageBrowserUI, index: usize)
    {
        let Some(record) = self.records.get(index).cloned() else { return };
        let dialog = ImageShowDialogUI::new();
        dialog.title.borrow_mut().set_text(&record.name);
        {
            let mut photo = dialog.photo.borrow_mut();
            photo.set_source(record.source);
            photo.set_alt(record.name);
            photo.set_render_mode(&self.render_mode);
        }
        let mut window = dialog.base;
        window.set_runtime_options(uimd::GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "close_btn".to_string(),
            ..Default::default()
        });
        let _ = uimd::configure_dialog_actions(
            &mut window,
            [("close_btn", "close")],
            "close",
        );
        ui.base.open_window(window);
    }

    fn open_browser(&mut self, ui: &mut ImageBrowserUI, record_index: Option<usize>)
    {
        let start_path = record_index
            .and_then(|index| self.records.get(index))
            .map(|record| project_root().join(&record.source))
            .unwrap_or_else(image_sample_path);
        let mut browser = uimd::FileBrowser::new(
            project_root(),
            Some(start_path),
            "open",
            "",
            IMAGE_EXTENSION_FILTER,
        );
        let window = browser.window();
        self.browser = Some(browser);
        self.pending_browse_index = record_index;
        ui.base.open_window(window);
    }

    fn handle_browser_outcome(
        &mut self,
        outcome: uimd::FileBrowserOutcome,
    )
    {
        match outcome
        {
            uimd::FileBrowserOutcome::Pending => {}
            uimd::FileBrowserOutcome::Cancelled =>
            {
                self.browser = None;
                self.pending_browse_index = None;
            }
            uimd::FileBrowserOutcome::ConfirmOverwrite(_) => {}
            uimd::FileBrowserOutcome::Selected(path) =>
            {
                let source = path.to_string_lossy().into_owned();
                if let Some(index) = self.pending_browse_index
                {
                    if let Some(record) = self.records.get_mut(index)
                    {
                        record.name = image_name_from_path(&path);
                        record.source = source;
                    }
                    self.refresh_items(false, true);
                }
                else
                {
                    self.records.push(ImageRecord
                    {
                        name: image_name_from_path(&path),
                        source,
                        selected: false,
                    });
                    self.refresh_items(true, false);
                }
                self.browser = None;
                self.pending_browse_index = None;
            }
        }
    }

    fn confirm_delete(&mut self, ui: &mut ImageBrowserUI, index: usize)
    {
        let Some(record) = self.records.get(index) else { return };
        self.pending_delete_index = Some(index);
        let mut dialog = uimd::MessageBoxYesNo::new(
            "Delete Image",
            &format!("Delete {}?", record.name),
        );
        ui.base.open_window(dialog.take_window());
        self.delete_dialog = Some(dialog);
    }

    fn delete_confirmed(&mut self, confirmed: bool)
    {
        let index = self.pending_delete_index.take();
        if confirmed
            && let Some(index) = index.filter(|index| *index < self.records.len())
        {
            self.records.remove(index);
            self.refresh_items(false, true);
        }
    }

    fn handle_root_button(&mut self, ui: &mut ImageBrowserUI, name: &str) -> bool
    {
        let sidebar_action = [
            ("camera_thumb", 0usize),
            ("astro_thumb", 1usize),
            ("coffee_thumb", 2usize),
        ]
        .into_iter()
        .find(|(prefix, _)|
        {
            name.starts_with(&format!("{prefix}."))
        });
        if let Some((_, index)) = sidebar_action
        {
            self.show_image(ui, index);
            return true;
        }
        if name.starts_with("gallery_mosaic.")
        {
            self.show_gallery(ui);
            return true;
        }
        if name.starts_with("image_list_btn.")
        {
            self.show_image_list(ui);
            return true;
        }
        if name == "main.add_btn"
        {
            self.open_browser(ui, None);
            return true;
        }
        if let Some(index) = image_list_action_index(name, "show_btn")
        {
            self.show_record(ui, index);
            return true;
        }
        if let Some(index) = image_list_action_index(name, "browse_btn")
        {
            self.open_browser(ui, Some(index));
            return true;
        }
        if let Some(index) = image_list_action_index(name, "delete_btn")
        {
            self.confirm_delete(ui, index);
            return true;
        }
        false
    }
}

impl ImageBrowserUIEvents for ImageBrowserApp
{
    fn on_close_btn_click(&mut self, ui: &mut ImageBrowserUI)
    {
        ui.base.request_close();
    }

    fn on_mode_select_change(&mut self, ui: &mut ImageBrowserUI, value: &str)
    {
        self.apply_render_mode(
            ui,
            if value == "Normal" { RENDER_MODE_SIXEL } else { RENDER_MODE_FALLBACK },
        );
    }

    fn handle_dynamic_button(&mut self, ui: &mut ImageBrowserUI, name: &str) -> bool
    {
        self.handle_root_button(ui, name)
    }

    fn handle_dynamic_text_changed(
        &mut self,
        ui: &mut ImageBrowserUI,
        name: &str,
        _value: &str,
    ) -> bool
    {
        if let Some(index) = image_list_action_index(name, "sel")
        {
            if let Some(record) = self.records.get_mut(index)
            {
                record.selected = ui
                    .base
                    .find_element(name)
                    .is_some_and(|element| element.borrow().checked());
            }
            return true;
        }
        false
    }

    fn on_window_closed(
        &mut self,
        _ui: &mut ImageBrowserUI,
        window: uimd::GeneratedWindow,
    )
    {
        if self
            .browser
            .as_ref()
            .is_some_and(|browser| browser.owns_window(&window) && browser.closed())
        {
            let outcome = self
                .browser
                .as_ref()
                .map(uimd::FileBrowser::result)
                .unwrap_or(uimd::FileBrowserOutcome::Pending);
            self.handle_browser_outcome(outcome);
            return;
        }
        if self
            .delete_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            let dialog = self
                .delete_dialog
                .take()
                .expect("matching delete dialog must remain owned");
            self.delete_confirmed(dialog.result() == Some(uimd::MessageBoxResult::Yes));
        }
    }
}

fn sample_extension(index: i32) -> &'static str
{
    match index.rem_euclid(6)
    {
        0 => ".bmp",
        1 => ".gif",
        2 => ".png",
        3 => ".tga",
        _ => ".jpg",
    }
}

fn image_library() -> Vec<ImageEntry>
{
    let mut images = vec![
        ImageEntry
        {
            caption: "Camera".to_string(),
            source: "shared/assets/image_samples/camera.png".to_string(),
        },
        ImageEntry
        {
            caption: "Astronaut".to_string(),
            source: "shared/assets/image_samples/astronaut.png".to_string(),
        },
        ImageEntry
        {
            caption: "Coffee".to_string(),
            source: "shared/assets/image_samples/coffee.png".to_string(),
        },
        ImageEntry
        {
            caption: "Coins".to_string(),
            source: "shared/assets/image_samples/coins.png".to_string(),
        },
        ImageEntry
        {
            caption: "Chelsea".to_string(),
            source: "shared/assets/image_samples/chelsea.png".to_string(),
        },
    ];
    for index in FIRST_GENERATED_SAMPLE_INDEX..=LAST_GENERATED_SAMPLE_INDEX
    {
        images.push(ImageEntry
        {
            caption: format!("Sample {index}"),
            source: format!(
                "shared/assets/image_samples/sample_{index}{}",
                sample_extension(index),
            ),
        });
    }
    images
}

fn sidebar_images() -> Vec<ImageEntry>
{
    image_library().into_iter().take(3).collect()
}

fn initial_image_records() -> Vec<ImageRecord>
{
    image_library()
        .into_iter()
        .take(INITIAL_LIST_COUNT)
        .map(|image| ImageRecord
        {
            name: image.caption,
            source: image.source,
            selected: false,
        })
        .collect()
}

fn project_root() -> PathBuf
{
    PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("../../..")
        .canonicalize()
        .unwrap_or_else(|_| PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../../.."))
}

fn image_sample_path() -> PathBuf
{
    project_root().join(IMAGE_SAMPLE_DIR)
}

fn image_name_from_path(path: &Path) -> String
{
    let stem = path
        .file_stem()
        .and_then(|name| name.to_str())
        .unwrap_or_default();
    if stem.is_empty()
    {
        return path.to_string_lossy().into_owned();
    }
    stem.replace(['_', '-'], " ")
        .split_whitespace()
        .map(|word|
        {
            let mut chars = word.chars();
            chars.next().map_or_else(String::new, |first|
            {
                first.to_uppercase().collect::<String>() + chars.as_str()
            })
        })
        .collect::<Vec<_>>()
        .join(" ")
}

fn image_display_path(path: &str) -> String
{
    let source = PathBuf::from(path);
    let absolute = if source.is_absolute()
    {
        source
    }
    else
    {
        project_root().join(source)
    };
    absolute
        .strip_prefix(project_root())
        .map_or_else(
            |_| path.to_string(),
            |relative| relative.to_string_lossy().into_owned(),
        )
}

fn set_child_cell_background(host: &uimd::ElementRef, background: &str)
{
    let mut host = host.borrow_mut();
    let Some(window) = host.child_window_mut() else { return };
    for item in &mut window.layout
    {
        item.cell_style.background = Some(uimd::Color::new(background));
    }
}

fn set_all_child_image_render_modes(host: &uimd::ElementRef, mode: &str)
{
    let mut host = host.borrow_mut();
    let Some(window) = host.child_window_mut() else { return };
    for element in &window.elements
    {
        let mut element = element.borrow_mut();
        if element.kind() == uimd::ElementKind::Image
        {
            element.set_render_mode(mode);
        }
    }
}

fn image_list_action_index(name: &str, action: &str) -> Option<usize>
{
    let suffix = format!("].{action}");
    let index = name.strip_prefix("main.items[")?.strip_suffix(&suffix)?;
    index.parse().ok()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageBrowserUI::new();
        let mut app = ImageBrowserApp::default();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

use crate::{
    GeneratedApplication, GeneratedWindow, GeneratedWindowBehavior, KeyEvent,
    GeneratedWindowFrameOptions, GeneratedWindowRuntimeOptions,
};
use std::cell::RefCell;
use std::fs;
use std::path::{Path, PathBuf};
use std::rc::Rc;

#[path = "../dialogs/file_browser_ui.rs"]
mod file_browser_ui;
#[path = "../dialogs/message_box_ok_ui.rs"]
mod message_box_ok_ui;
#[path = "../dialogs/message_box_ok_cancel_ui.rs"]
mod message_box_ok_cancel_ui;
#[path = "../dialogs/message_box_yes_no_ui.rs"]
mod message_box_yes_no_ui;
#[path = "../dialogs/message_box_yes_no_cancel_ui.rs"]
mod message_box_yes_no_cancel_ui;

const PARENT_ENTRY: &str = "..";

struct DialogApplication<'a>
{
    window: &'a mut GeneratedWindow,
}

impl GeneratedApplication for DialogApplication<'_>
{
    fn window(&self) -> &GeneratedWindow
    {
        self.window
    }

    fn window_mut(&mut self) -> &mut GeneratedWindow
    {
        self.window
    }
}

fn run_dialog_window(window: &mut GeneratedWindow) -> i32
{
    let options = window.runtime_options();
    let mut app = DialogApplication { window };
    crate::run_generated_window(&mut app, options, &[])
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum MessageBoxResult
{
    Ok,
    Cancel,
    Yes,
    No,
}

#[derive(Clone)]
pub struct DialogActionHandle
{
    window_identity: u64,
    result: Rc<RefCell<Option<String>>>,
}

impl DialogActionHandle
{
    pub fn result(&self) -> Option<String>
    {
        self.result.borrow().clone()
    }

    pub fn owns_window(&self, window: &GeneratedWindow) -> bool
    {
        self.window_identity == window.identity()
    }
}

struct DialogActionBehavior
{
    result_by_button: Vec<(String, String)>,
    default_result: String,
    result: Rc<RefCell<Option<String>>>,
}

impl GeneratedWindowBehavior for DialogActionBehavior
{
    fn handle_button(&mut self, window: &mut GeneratedWindow, name: &str) -> bool
    {
        let result = self
            .result_by_button
            .iter()
            .find_map(|(button, result)| (button == name).then(|| result.clone()));
        let Some(result) = result else { return false };
        self.result.replace(Some(result));
        window.request_close();
        true
    }

    fn handle_key(&mut self, window: &mut GeneratedWindow, key: &str) -> bool
    {
        if key != "Escape" || self.default_result.is_empty()
        {
            return false;
        }
        self.result.replace(Some(self.default_result.clone()));
        window.request_close();
        true
    }

    fn should_close(&self) -> bool
    {
        self.result.borrow().is_some()
    }
}

pub fn configure_dialog_actions(
    window: &mut GeneratedWindow,
    result_by_button: impl IntoIterator<Item = (impl Into<String>, impl Into<String>)>,
    default_result: impl Into<String>,
) -> DialogActionHandle
{
    let result = Rc::new(RefCell::new(None));
    let window_identity = window.identity();
    window.set_behavior(DialogActionBehavior
    {
        result_by_button: result_by_button
            .into_iter()
            .map(|(button, value)| (button.into(), value.into()))
            .collect(),
        default_result: default_result.into(),
        result: result.clone(),
    });
    DialogActionHandle { window_identity, result }
}

#[derive(Clone)]
struct MessageBoxState
{
    result_by_button: Vec<(&'static str, MessageBoxResult)>,
    default_result: MessageBoxResult,
    result: Option<MessageBoxResult>,
    closed: bool,
    on_close: Option<Rc<dyn Fn(MessageBoxResult)>>,
}

impl MessageBoxState
{
    fn close(&mut self, result: MessageBoxResult)
    {
        self.result = Some(result);
        self.closed = true;
        if let Some(callback) = self.on_close.clone()
        {
            callback(result);
        }
    }
}

struct MessageBoxBehavior
{
    state: Rc<RefCell<MessageBoxState>>,
}

impl GeneratedWindowBehavior for MessageBoxBehavior
{
    fn handle_button(&mut self, window: &mut GeneratedWindow, name: &str) -> bool
    {
        let result = self
            .state
            .borrow()
            .result_by_button
            .iter()
            .find_map(|(button, result)| (*button == name).then_some(*result));
        let Some(result) = result else { return false };
        self.state.borrow_mut().close(result);
        window.request_close();
        true
    }

    fn handle_key(&mut self, window: &mut GeneratedWindow, key: &str) -> bool
    {
        if key != "Escape"
        {
            return false;
        }
        let result = self.state.borrow().default_result;
        self.state.borrow_mut().close(result);
        window.request_close();
        true
    }

    fn should_close(&self) -> bool
    {
        self.state.borrow().closed
    }
}

struct MessageBox
{
    window: Option<GeneratedWindow>,
    window_identity: u64,
    state: Rc<RefCell<MessageBoxState>>,
}

impl MessageBox
{
    fn new(
        mut window: GeneratedWindow,
        result_by_button: Vec<(&'static str, MessageBoxResult)>,
        default_result: MessageBoxResult,
        on_close: Option<Rc<dyn Fn(MessageBoxResult)>>,
    ) -> Self
    {
        let initial_focus_name = result_by_button
            .iter()
            .find_map(|(button, result)| (*result == default_result).then_some(*button))
            .unwrap_or_default()
            .to_string();
        window.set_runtime_options(GeneratedWindowRuntimeOptions
        {
            initial_focus_name,
            ..Default::default()
        });
        let state = Rc::new(RefCell::new(MessageBoxState
        {
            result_by_button,
            default_result,
            result: None,
            closed: false,
            on_close,
        }));
        window.set_behavior(MessageBoxBehavior { state: state.clone() });
        let window_identity = window.identity();
        Self { window: Some(window), window_identity, state }
    }

    fn take_window(&mut self) -> GeneratedWindow
    {
        self.window
            .take()
            .expect("a concrete message box window can only be opened once")
    }

    fn result(&self) -> Option<MessageBoxResult>
    {
        self.state.borrow().result
    }

    fn closed(&self) -> bool
    {
        self.state.borrow().closed
    }

    fn owns_window(&self, window: &GeneratedWindow) -> bool
    {
        self.window_identity == window.identity()
    }

    fn run(&mut self) -> i32
    {
        run_dialog_window(
            self.window
                .as_mut()
                .expect("a concrete message box can only run before its window is moved"),
        )
    }
}

macro_rules! define_message_box
{
    (
        $name:ident,
        $ui:path,
        $default:expr,
        [$($button:literal => $result:expr),+ $(,)?]
    ) =>
    {
        pub struct $name
        {
            inner: MessageBox,
        }

        impl $name
        {
            pub fn new(header: &str, message: &str) -> Self
            {
                let ui = <$ui>::new();
                let window = configure_message_window(
                    ui.base,
                    &ui.dialog_header,
                    &ui.message,
                    header,
                    message,
                    "",
                );
                Self
                {
                    inner: MessageBox::new(
                        window,
                        vec![$(($button, $result)),+],
                        $default,
                        None,
                    ),
                }
            }

            pub fn with_callback(
                header: &str,
                message: &str,
                on_close: Rc<dyn Fn(MessageBoxResult)>,
            ) -> Self
            {
                let ui = <$ui>::new();
                let window = configure_message_window(
                    ui.base,
                    &ui.dialog_header,
                    &ui.message,
                    header,
                    message,
                    "",
                );
                Self
                {
                    inner: MessageBox::new(
                        window,
                        vec![$(($button, $result)),+],
                        $default,
                        Some(on_close),
                    ),
                }
            }

            pub fn take_window(&mut self) -> GeneratedWindow
            {
                self.inner.take_window()
            }

            pub fn result(&self) -> Option<MessageBoxResult>
            {
                self.inner.result()
            }

            pub fn closed(&self) -> bool
            {
                self.inner.closed()
            }

            pub fn owns_window(&self, window: &GeneratedWindow) -> bool
            {
                self.inner.owns_window(window)
            }

            pub fn run(&mut self) -> i32
            {
                self.inner.run()
            }
        }
    };
}

define_message_box!(
    MessageBoxOk,
    message_box_ok_ui::MessageBoxOkUI,
    MessageBoxResult::Ok,
    ["ok_btn" => MessageBoxResult::Ok]
);
define_message_box!(
    MessageBoxOkCancel,
    message_box_ok_cancel_ui::MessageBoxOkCancelUI,
    MessageBoxResult::Cancel,
    [
        "ok_btn" => MessageBoxResult::Ok,
        "cancel_btn" => MessageBoxResult::Cancel,
    ]
);
define_message_box!(
    MessageBoxYesNo,
    message_box_yes_no_ui::MessageBoxYesNoUI,
    MessageBoxResult::No,
    [
        "yes_btn" => MessageBoxResult::Yes,
        "no_btn" => MessageBoxResult::No,
    ]
);
define_message_box!(
    MessageBoxYesNoCancel,
    message_box_yes_no_cancel_ui::MessageBoxYesNoCancelUI,
    MessageBoxResult::Cancel,
    [
        "yes_btn" => MessageBoxResult::Yes,
        "no_btn" => MessageBoxResult::No,
        "cancel_btn" => MessageBoxResult::Cancel,
    ]
);

fn configure_message_window(
    mut window: GeneratedWindow,
    header_element: &crate::ElementRef,
    message_element: &crate::ElementRef,
    header: &str,
    message: &str,
    initial_focus_name: &str,
) -> GeneratedWindow
{
    header_element.borrow_mut().set_text(header);
    message_element.borrow_mut().set_text(message);
    window.set_runtime_options(GeneratedWindowRuntimeOptions
    {
        initial_focus_name: initial_focus_name.to_string(),
        ..Default::default()
    });
    window
}

pub(crate) fn standard_escape_button_name(class_name: &str) -> Option<&'static str>
{
    match class_name
    {
        "FileBrowser" => Some("close_btn"),
        "MessageBoxOk" => Some("ok_btn"),
        "MessageBoxOkCancel" => Some("cancel_btn"),
        "MessageBoxYesNo" => Some("no_btn"),
        "MessageBoxYesNoCancel" => Some("cancel_btn"),
        _ => None,
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum FileBrowserOutcome
{
    Pending,
    Selected(PathBuf),
    ConfirmOverwrite(PathBuf),
    Cancelled,
}

pub type FileBrowserCallback = Rc<dyn Fn(String)>;

struct FileBrowserMousePressResult
{
    outcome: FileBrowserOutcome,
    consumed: bool,
}

#[derive(Clone)]
struct FileBrowserState
{
    root_dir: PathBuf,
    current_dir: PathBuf,
    mode: String,
    initial_filename: String,
    extension_filter: String,
    on_close: Option<FileBrowserCallback>,
    outcome: FileBrowserOutcome,
}

impl FileBrowserState
{
    pub fn new(
        root_dir: impl AsRef<Path>,
        start_path: Option<impl AsRef<Path>>,
        mode: impl Into<String>,
        initial_filename: impl Into<String>,
        extension_filter: impl Into<String>,
        on_close: Option<FileBrowserCallback>,
    ) -> Self
    {
        let root_dir = absolute_existing_path(root_dir.as_ref());
        let requested = start_path
            .as_ref()
            .map(|path| absolute_existing_path(path.as_ref()))
            .unwrap_or_else(|| root_dir.clone());
        let mut initial_filename = initial_filename.into();
        let requested_is_file = requested.is_file();
        let current_dir = if requested_is_file
        {
            if initial_filename.is_empty()
            {
                initial_filename = requested
                    .file_name()
                    .and_then(|name| name.to_str())
                    .unwrap_or_default()
                    .to_string();
            }
            requested.parent().unwrap_or(&root_dir).to_path_buf()
        }
        else
        {
            requested
        };
        let mut browser = Self
        {
            root_dir: root_dir.clone(),
            current_dir,
            mode: mode.into(),
            initial_filename,
            extension_filter: extension_filter.into().trim().to_string(),
            on_close,
            outcome: FileBrowserOutcome::Pending,
        };
        browser.current_dir = browser.clamp_dir(browser.current_dir.clone());
        browser
    }

    pub fn window(&mut self) -> GeneratedWindow
    {
        let ui = file_browser_ui::FileBrowserUI::new();
        ui.dialog_header
            .borrow_mut()
            .set_text(if self.mode == "save" { "Save As" } else { "Open File" });
        ui.open_btn
            .borrow_mut()
            .set_title(if self.mode == "save" { "Save" } else { "Open" });
        if self.mode == "save"
        {
            ui.filename.borrow_mut().set_value(&self.initial_filename);
        }
        let mut window = ui.base;
        window.set_runtime_options(GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "entries".to_string(),
            start_in_edit_mode: true,
            ..Default::default()
        });
        self.refresh_entries(&mut window);
        window
    }

    pub fn handle_button(
        &mut self,
        window: &mut GeneratedWindow,
        button_name: &str,
    ) -> FileBrowserOutcome
    {
        match button_name
        {
            "open_btn" if self.mode == "save" => self.accept_filename(window),
            "open_btn" => self.accept_current(window),
            "close_btn" => FileBrowserOutcome::Cancelled,
            _ => FileBrowserOutcome::Pending,
        }
    }

    fn handle_listbox_item_activate(
        &mut self,
        window: &mut GeneratedWindow,
        name: &str,
        index: usize,
    ) -> bool
    {
        if name != "entries"
        {
            return false;
        }
        let Some(entries) = window.find_element("entries") else { return false };
        {
            let mut entries = entries.borrow_mut();
            entries.set_selected_index(index.try_into().unwrap_or(i32::MAX));
            entries.hide_active_item();
        }
        self.preview_selected(window);
        if self.selected_entry_is_directory(window)
        {
            let _ = self.accept_current(window);
            return true;
        }
        false
    }

    pub fn handle_selection_changed(&self, window: &mut GeneratedWindow, name: &str)
    {
        if name == "entries"
        {
            self.preview_selected(window);
        }
        else if name == "filename"
        {
            self.update_open_enabled(window);
        }
    }

    fn complete(&mut self, outcome: FileBrowserOutcome)
    {
        self.outcome = outcome.clone();
        let result = match outcome
        {
            FileBrowserOutcome::Selected(path) =>
                Some(path.to_string_lossy().into_owned()),
            FileBrowserOutcome::Cancelled => Some(String::new()),
            FileBrowserOutcome::Pending | FileBrowserOutcome::ConfirmOverwrite(_) => None,
        };
        if let Some(result) = result
            && let Some(callback) = self.on_close.clone()
        {
            callback(result);
        }
    }

    fn handle_entry_mouse_press(
        &mut self,
        window: &mut GeneratedWindow,
        point: crate::Point,
    ) -> FileBrowserMousePressResult
    {
        let Some(entries) = window.find_element("entries") else
        {
            return FileBrowserMousePressResult
            {
                outcome: FileBrowserOutcome::Pending,
                consumed: false,
            };
        };
        let frame = entries.borrow().frame;
        if !frame.contains(point)
        {
            return FileBrowserMousePressResult
            {
                outcome: FileBrowserOutcome::Pending,
                consumed: false,
            };
        }
        let index = entries.borrow().list_scroll_offset() + point.row - frame.row;
        if index < 0 || index >= entries.borrow().options().len() as i32
        {
            return FileBrowserMousePressResult
            {
                outcome: FileBrowserOutcome::Pending,
                consumed: false,
            };
        }
        entries.borrow_mut().set_selected_index(index);
        self.preview_selected(window);
        if self.selected_entry_is_directory(window)
        {
            let outcome = self.accept_current(window);
            return FileBrowserMousePressResult { outcome, consumed: true };
        }
        FileBrowserMousePressResult
        {
            outcome: FileBrowserOutcome::Pending,
            consumed: false,
        }
    }

    fn refresh_entries(&mut self, window: &mut GeneratedWindow)
    {
        let mut directories = Vec::new();
        let mut files = Vec::new();
        let mut disabled_files = Vec::new();
        if let Ok(items) = fs::read_dir(&self.current_dir)
        {
            for item in items.flatten()
            {
                let name = item.file_name().to_string_lossy().to_string();
                if item.path().is_dir()
                {
                    directories.push(format!("{name}/"));
                }
                else if item.path().is_file()
                {
                    if !self.path_matches_filter(&item.path())
                    {
                        disabled_files.push(name.clone());
                    }
                    files.push(name);
                }
            }
        }
        directories.sort();
        files.sort();
        let mut rows = vec![PARENT_ENTRY.to_string()];
        rows.extend(directories);
        rows.extend(files);
        if let Some(path_label) = window.find_element("path_label")
        {
            path_label
                .borrow_mut()
                .set_text(self.current_dir.to_string_lossy());
        }
        if let Some(entries) = window.find_element("entries")
        {
            let selected = rows
                .iter()
                .position(|row| row == &self.initial_filename)
                .unwrap_or_default() as i32;
            let mut entries = entries.borrow_mut();
            entries.set_options(rows);
            entries.set_disabled_values(disabled_files);
            entries.set_selected_index(selected);
        }
        self.preview_selected(window);
    }

    fn preview_selected(&self, window: &mut GeneratedWindow)
    {
        let selected = window
            .find_element("entries")
            .map(|entries| entries.borrow().selected_value())
            .unwrap_or_default();
        if let Some(filename) = window.find_element("filename")
        {
            if !selected.is_empty() && selected != PARENT_ENTRY
            {
                filename.borrow_mut().set_value(selected.trim_end_matches('/'));
            }
            else if self.mode == "open"
            {
                filename.borrow_mut().set_value("");
            }
            let mut filename = filename.borrow_mut();
            let cursor = filename.value().len();
            filename.select_range(cursor, cursor);
        }
        self.update_open_enabled(window);
    }

    fn selected_entry_is_directory(&self, window: &GeneratedWindow) -> bool
    {
        window
            .find_element("entries")
            .map(|entries|
            {
                let value = entries.borrow().selected_value();
                value == PARENT_ENTRY || value.ends_with('/')
            })
            .unwrap_or(false)
    }

    fn accept_current(&mut self, window: &mut GeneratedWindow) -> FileBrowserOutcome
    {
        let Some(selected_path) = self.selected_path(window) else
        {
            return FileBrowserOutcome::Pending;
        };
        if selected_path.is_dir()
        {
            self.current_dir = self.clamp_dir(selected_path);
            self.refresh_entries(window);
            return FileBrowserOutcome::Pending;
        }
        let selected_name = selected_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or_default();
        let typed_name = window
            .find_element("filename")
            .map(|filename| filename.borrow().value().trim().to_string())
            .unwrap_or_default();
        let name = if typed_name.is_empty()
        {
            selected_name.to_string()
        }
        else
        {
            typed_name
        };
        if name.is_empty()
        {
            return FileBrowserOutcome::Pending;
        }
        let path = absolute_path(&self.current_dir.join(name));
        let parent = path.parent().unwrap_or(&self.current_dir);
        if self.mode == "open"
            && path.is_file()
            && self.clamp_dir(parent.to_path_buf()) == self.current_dir
        {
            return if self.path_matches_filter(&path)
            {
                FileBrowserOutcome::Selected(path)
            }
            else
            {
                FileBrowserOutcome::Pending
            };
        }
        if self.mode == "save"
        {
            return self.accept_filename(window);
        }
        FileBrowserOutcome::Pending
    }

    fn accept_filename(&self, window: &GeneratedWindow) -> FileBrowserOutcome
    {
        let name = window
            .find_element("filename")
            .map(|filename| filename.borrow().value().trim().to_string())
            .unwrap_or_default();
        if name.is_empty()
        {
            return FileBrowserOutcome::Pending;
        }
        let path = absolute_path(&self.current_dir.join(name));
        let parent = path.parent().unwrap_or(&self.current_dir);
        if path.is_dir() || self.clamp_dir(parent.to_path_buf()) != self.current_dir
        {
            return FileBrowserOutcome::Pending;
        }
        if path.is_file()
        {
            FileBrowserOutcome::ConfirmOverwrite(path)
        }
        else
        {
            FileBrowserOutcome::Selected(path)
        }
    }

    fn selected_path(&self, window: &GeneratedWindow) -> Option<PathBuf>
    {
        let selected = window
            .find_element("entries")
            .map(|entries| entries.borrow().selected_value())
            .unwrap_or_default();
        if selected.is_empty()
        {
            return None;
        }
        if selected == PARENT_ENTRY
        {
            return Some(
                self.current_dir
                    .parent()
                    .unwrap_or(&self.current_dir)
                    .to_path_buf(),
            );
        }
        Some(
            self.current_dir.join(selected.trim_end_matches('/')),
        )
    }

    fn update_open_enabled(&self, window: &mut GeneratedWindow)
    {
        let Some(open_button) = window.find_element("open_btn") else { return };
        let filename = window
            .find_element("filename")
            .map(|element| element.borrow().value().trim().to_string())
            .unwrap_or_default();
        if self.mode == "save"
        {
            open_button.borrow_mut().enabled = !filename.is_empty();
            return;
        }
        let selected = window
            .find_element("entries")
            .map(|element| element.borrow().selected_value())
            .unwrap_or_default();
        let path = if !filename.is_empty()
        {
            self.current_dir.join(filename)
        }
        else if selected == PARENT_ENTRY
        {
            self.current_dir.parent().unwrap_or(&self.current_dir).to_path_buf()
        }
        else
        {
            self.current_dir.join(selected.trim_end_matches('/'))
        };
        open_button.borrow_mut().enabled =
            path.is_dir() || (path.is_file() && self.path_matches_filter(&path));
    }

    fn clamp_dir(&self, path: PathBuf) -> PathBuf
    {
        let path = absolute_existing_path(&path);
        if path.starts_with(&self.root_dir) && path.is_dir()
        {
            path
        }
        else
        {
            self.root_dir.clone()
        }
    }

    fn path_matches_filter(&self, path: &Path) -> bool
    {
        if self.extension_filter.is_empty()
        {
            return true;
        }
        let extension = path
            .extension()
            .and_then(|value| value.to_str())
            .unwrap_or_default()
            .to_ascii_lowercase();
        let filter = regex::RegexBuilder::new(&format!("^(?:{})$", self.extension_filter))
            .case_insensitive(true)
            .build();
        match filter
        {
            Ok(filter) => filter.is_match(&extension),
            Err(_) => self
                .extension_filter
                .split('|')
                .map(|item| {
                    item.trim()
                        .trim_start_matches('.')
                        .to_ascii_lowercase()
                })
                .any(|item| item == extension),
        }
    }
}

struct FileBrowserBehavior
{
    state: Rc<RefCell<FileBrowserState>>,
}

impl FileBrowserBehavior
{
    fn finish_outcome(
        &self,
        window: &mut GeneratedWindow,
        outcome: FileBrowserOutcome,
    ) -> bool
    {
        match outcome
        {
            FileBrowserOutcome::Pending => false,
            FileBrowserOutcome::ConfirmOverwrite(path) =>
            {
                let state = self.state.clone();
                let selected_path = path.clone();
                let mut dialog = MessageBoxYesNo::with_callback(
                    "Overwrite File",
                    &format!(
                        "File already exists:\n{}\n\nOverwrite it?",
                        path.to_string_lossy(),
                    ),
                    Rc::new(move |result|
                    {
                        if result == MessageBoxResult::Yes
                        {
                            state
                                .borrow_mut()
                                .complete(FileBrowserOutcome::Selected(selected_path.clone()));
                        }
                    }),
                );
                window.open_window(dialog.take_window());
                true
            }
            outcome =>
            {
                self.state.borrow_mut().complete(outcome);
                window.request_close();
                true
            }
        }
    }
}

impl GeneratedWindowBehavior for FileBrowserBehavior
{
    fn handle_button(&mut self, window: &mut GeneratedWindow, name: &str) -> bool
    {
        let outcome = self.state.borrow_mut().handle_button(window, name);
        self.finish_outcome(window, outcome)
    }

    fn handle_preview_key(&mut self, window: &mut GeneratedWindow, event: &KeyEvent) -> bool
    {
        if event.key != "Escape" || event.edit_mode
        {
            return false;
        }
        self.finish_outcome(window, FileBrowserOutcome::Cancelled)
    }

    fn handle_listbox_item_activate(
        &mut self,
        window: &mut GeneratedWindow,
        name: &str,
        _element_id: &str,
        index: usize,
        _value: &str,
    ) -> bool
    {
        self.state
            .borrow_mut()
            .handle_listbox_item_activate(window, name, index)
    }

    fn handle_mouse_press(&mut self, window: &mut GeneratedWindow, point: crate::Point) -> bool
    {
        let result = self
            .state
            .borrow_mut()
            .handle_entry_mouse_press(window, point);
        let finished = self.finish_outcome(window, result.outcome);
        result.consumed || finished
    }

    fn handle_text_changed(
        &mut self,
        window: &mut GeneratedWindow,
        name: &str,
        _value: &str,
    ) -> bool
    {
        self.state
            .borrow()
            .handle_selection_changed(window, name);
        matches!(name, "entries" | "filename")
    }

    fn handle_text_confirmed(
        &mut self,
        window: &mut GeneratedWindow,
        name: &str,
        _value: &str,
    ) -> bool
    {
        self.state
            .borrow()
            .handle_selection_changed(window, name);
        matches!(name, "entries" | "filename")
    }

    fn handle_selection_changed(
        &mut self,
        window: &mut GeneratedWindow,
        name: &str,
        _values: &[String],
    ) -> bool
    {
        self.state
            .borrow()
            .handle_selection_changed(window, name);
        matches!(name, "entries" | "filename")
    }

    fn handle_edit_started(&mut self, window: &mut GeneratedWindow, name: &str)
    {
        if name == "filename"
            && let Some(filename) = window.find_element("filename")
        {
            let end = filename.borrow().value().len();
            filename.borrow_mut().select_range(end, end);
        }
    }

    fn should_close(&self) -> bool
    {
        !matches!(self.state.borrow().outcome, FileBrowserOutcome::Pending)
    }
}

pub struct FileBrowser
{
    window: Option<GeneratedWindow>,
    window_identity: u64,
    state: Rc<RefCell<FileBrowserState>>,
}

impl FileBrowser
{
    pub fn new(
        root_dir: impl AsRef<Path>,
        start_path: Option<impl AsRef<Path>>,
        mode: impl Into<String>,
        initial_filename: impl Into<String>,
        extension_filter: impl Into<String>,
    ) -> Self
    {
        Self::new_with_callback(
            root_dir,
            start_path,
            mode,
            initial_filename,
            extension_filter,
            None,
        )
    }

    fn new_with_callback(
        root_dir: impl AsRef<Path>,
        start_path: Option<impl AsRef<Path>>,
        mode: impl Into<String>,
        initial_filename: impl Into<String>,
        extension_filter: impl Into<String>,
        on_close: Option<FileBrowserCallback>,
    ) -> Self
    {
        let mut state = FileBrowserState::new(
            root_dir,
            start_path,
            mode,
            initial_filename,
            extension_filter,
            on_close,
        );
        let mut window = state.window();
        let state = Rc::new(RefCell::new(state));
        window.set_behavior(FileBrowserBehavior { state: state.clone() });
        let window_identity = window.identity();
        Self { window: Some(window), window_identity, state }
    }

    pub fn with_callback(
        root_dir: impl AsRef<Path>,
        start_path: Option<impl AsRef<Path>>,
        mode: impl Into<String>,
        initial_filename: impl Into<String>,
        extension_filter: impl Into<String>,
        on_close: FileBrowserCallback,
    ) -> Self
    {
        Self::new_with_callback(
            root_dir,
            start_path,
            mode,
            initial_filename,
            extension_filter,
            Some(on_close),
        )
    }

    pub fn open(root_dir: impl AsRef<Path>, start_path: Option<impl AsRef<Path>>) -> Self
    {
        Self::new(root_dir, start_path, "open", "", "")
    }

    pub fn save(
        root_dir: impl AsRef<Path>,
        start_path: Option<impl AsRef<Path>>,
        initial_filename: impl Into<String>,
    ) -> Self
    {
        Self::new(root_dir, start_path, "save", initial_filename, "")
    }

    pub fn current_dir(&self) -> PathBuf
    {
        self.state.borrow().current_dir.clone()
    }

    pub fn refresh_entries(&mut self)
    {
        let state = self.state.clone();
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser entries can only be refreshed before its window is moved");
        state.borrow_mut().refresh_entries(window);
    }

    pub fn result(&self) -> FileBrowserOutcome
    {
        self.state.borrow().outcome.clone()
    }

    pub fn closed(&self) -> bool
    {
        !matches!(self.state.borrow().outcome, FileBrowserOutcome::Pending)
    }

    pub fn owns_window(&self, window: &GeneratedWindow) -> bool
    {
        self.window_identity == window.identity()
    }

    pub fn take_window(&mut self) -> GeneratedWindow
    {
        self.window
            .take()
            .expect("a concrete FileBrowser window can only be opened once")
    }

    pub fn window(&mut self) -> GeneratedWindow
    {
        self.take_window()
    }

    pub fn accept_current(&mut self) -> bool
    {
        let state = self.state.clone();
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser can only accept before its window is moved");
        let selected_directory = state.borrow().selected_entry_is_directory(window);
        let outcome = state.borrow_mut().accept_current(window);
        let accepted =
            selected_directory || !matches!(outcome, FileBrowserOutcome::Pending);
        let finished =
            FileBrowserBehavior { state }.finish_outcome(window, outcome);
        accepted || finished
    }

    pub fn accept_filename(&mut self) -> bool
    {
        let state = self.state.clone();
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser can only accept before its window is moved");
        let outcome = state.borrow().accept_filename(window);
        let accepted = !matches!(outcome, FileBrowserOutcome::Pending);
        let finished =
            FileBrowserBehavior { state }.finish_outcome(window, outcome);
        accepted || finished
    }

    pub fn select_entry(&mut self, index: i32)
    {
        let state = self.state.clone();
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser entries can only be selected before its window is moved");
        if let Some(entries) = window.find_element("entries")
        {
            entries.borrow_mut().set_selected_index(index);
        }
        state.borrow().preview_selected(window);
    }

    pub fn selected_entry_is_directory(&self) -> bool
    {
        self.window
            .as_ref()
            .is_some_and(|window| self.state.borrow().selected_entry_is_directory(window))
    }

    pub fn entry_index_is_directory(&self, index: i32) -> bool
    {
        let Some(window) = self.window.as_ref() else { return false };
        let Some(entries) = window.find_element("entries") else { return false };
        let entries = entries.borrow();
        let options = entries.options();
        let Some(selected) = usize::try_from(index)
            .ok()
            .and_then(|index| options.get(index))
        else
        {
            return false;
        };
        selected == PARENT_ENTRY || selected.ends_with('/')
    }

    pub fn handle_entry_mouse_press(&mut self, point: crate::Point) -> bool
    {
        let state = self.state.clone();
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser mouse input requires its owned window");
        let result = state.borrow_mut().handle_entry_mouse_press(window, point);
        let finished =
            FileBrowserBehavior { state }.finish_outcome(window, result.outcome);
        result.consumed || finished
    }

    pub fn move_filename_cursor_to_end(&mut self)
    {
        let window = self
            .window
            .as_mut()
            .expect("FileBrowser filename cursor requires its owned window");
        if let Some(filename) = window.find_element("filename")
        {
            let end = filename.borrow().value().len();
            filename.borrow_mut().select_range(end, end);
        }
    }

    pub fn close(&mut self, path: impl Into<String>)
    {
        let path = path.into();
        self.state.borrow_mut().complete(if path.is_empty()
        {
            FileBrowserOutcome::Cancelled
        }
        else
        {
            FileBrowserOutcome::Selected(PathBuf::from(path))
        });
        if let Some(window) = self.window.as_mut()
        {
            window.request_close();
        }
    }

    pub fn stack_frame_options(&self) -> GeneratedWindowFrameOptions
    {
        GeneratedWindowFrameOptions::from_window(
            self.window
                .as_ref()
                .expect("FileBrowser frame options require its owned window"),
        )
    }

    pub fn run(&mut self) -> i32
    {
        run_dialog_window(
            self.window
                .as_mut()
                .expect("FileBrowser can only run before its window is moved"),
        )
    }

    pub fn handle_button(
        &mut self,
        window: &mut GeneratedWindow,
        button_name: &str,
    ) -> FileBrowserOutcome
    {
        self.state.borrow_mut().handle_button(window, button_name)
    }

    #[deprecated(since = "0.5.4", note = "use ListBox item activation; removal in UIMD 0.7.0")]
    pub fn handle_key_before_focused(
        &mut self,
        window: &mut GeneratedWindow,
        key: &str,
        focused_name: &str,
        edit_mode: bool,
    ) -> bool
    {
        if key != "Enter" || focused_name != "entries" || !edit_mode
        {
            return false;
        }
        let Some(entries) = window.find_element("entries") else { return false };
        let active_index = entries.borrow().active_index().max(0) as usize;
        self.state
            .borrow_mut()
            .handle_listbox_item_activate(window, focused_name, active_index)
    }

    pub fn handle_selection_changed(&self, window: &mut GeneratedWindow, name: &str)
    {
        self.state
            .borrow()
            .handle_selection_changed(window, name);
    }

    pub fn handle_escape(&self) -> FileBrowserOutcome
    {
        FileBrowserOutcome::Cancelled
    }
}

fn absolute_existing_path(path: &Path) -> PathBuf
{
    path.canonicalize().unwrap_or_else(|_| absolute_path(path))
}

fn absolute_path(path: &Path) -> PathBuf
{
    if path.is_absolute()
    {
        path.to_path_buf()
    }
    else
    {
        std::env::current_dir()
            .unwrap_or_else(|_| PathBuf::from("."))
            .join(path)
    }
}

#[cfg(test)]
mod tests
{
    use super::*;
    use std::time::{SystemTime, UNIX_EPOCH};

    #[test]
    fn parent_list_selection_takes_precedence_over_typed_open_filename()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-{}-{unique}",
            std::process::id(),
        ));
        let child = root.join("child");
        fs::create_dir_all(&child).expect("temporary browser root must be created");
        let target = child.join("open_target.txt");
        fs::write(&target, "Opened target\n").expect("temporary target must be written");

        let mut browser = FileBrowser::open(&root, Some(&child));
        let mut window = browser.window();
        window
            .find_element("filename")
            .expect("generated FileBrowser must expose filename")
            .borrow_mut()
            .set_value("open_target.txt");

        assert_eq!(
            browser.handle_button(&mut window, "open_btn"),
            FileBrowserOutcome::Pending,
        );
        assert_eq!(
            browser.current_dir(),
            root.canonicalize().expect("temporary root must canonicalize"),
        );

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }

    #[test]
    fn file_browser_public_surface_and_single_click_directory_match_cpp()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-public-{}-{unique}",
            std::process::id(),
        ));
        let child = root.join("child");
        fs::create_dir_all(&child).expect("temporary browser child must be created");
        fs::write(root.join("blocked.txt"), "blocked")
            .expect("temporary filtered file must be written");

        let mut browser = FileBrowser::new(
            &root,
            Some(&root),
            "open",
            "",
            "png",
        );
        let entries = browser
            .window
            .as_ref()
            .and_then(|window| window.find_element("entries"))
            .expect("generated FileBrowser must expose entries");
        entries.borrow_mut().set_frame(crate::Rect
        {
            row: 0,
            col: 0,
            width: 20,
            height: 5,
        });

        assert!(browser.entry_index_is_directory(0));
        assert!(browser.entry_index_is_directory(1));
        assert!(!browser.entry_index_is_directory(2));
        assert!(browser.handle_entry_mouse_press(crate::Point { row: 1, col: 0 }));
        assert_eq!(
            browser.current_dir(),
            child.canonicalize().expect("temporary child must canonicalize"),
        );
        assert_eq!(entries.borrow().selected_index(), 0);
        assert_eq!(entries.borrow().selected_indices(), &[0]);
        assert!(!entries.borrow().active_item_visible());

        let frame = browser.stack_frame_options();
        assert_eq!(frame.class_name, "FileBrowser");
        assert_eq!(frame.initial_focus_name, "entries");
        assert!(frame.start_in_edit_mode);

        let _message_run: fn(&mut MessageBoxOk) -> i32 = MessageBoxOk::run;
        let _browser_run: fn(&mut FileBrowser) -> i32 = FileBrowser::run;

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }

    #[test]
    fn filtered_file_click_only_selects_and_remains_unconsumed_like_cpp()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-filtered-{}-{unique}",
            std::process::id(),
        ));
        fs::create_dir_all(&root).expect("temporary browser root must be created");
        fs::write(root.join("blocked.txt"), "blocked")
            .expect("temporary filtered file must be written");

        let mut browser = FileBrowser::new(
            &root,
            Some(&root),
            "open",
            "",
            "png",
        );
        let entries = browser
            .window
            .as_ref()
            .and_then(|window| window.find_element("entries"))
            .expect("generated FileBrowser must expose entries");
        entries.borrow_mut().set_frame(crate::Rect
        {
            row: 0,
            col: 0,
            width: 20,
            height: 5,
        });

        assert!(!browser.handle_entry_mouse_press(crate::Point { row: 1, col: 0 }));
        assert!(!browser.handle_entry_mouse_press(crate::Point { row: 1, col: 0 }));
        assert_eq!(entries.borrow().selected_value(), "blocked.txt");
        assert!(!browser.closed());

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }

    #[test]
    fn save_rejects_a_typed_nested_path_like_cpp()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-nested-{}-{unique}",
            std::process::id(),
        ));
        fs::create_dir_all(root.join("child"))
            .expect("temporary browser child must be created");

        let mut browser = FileBrowser::save(&root, Some(&root), "");
        let mut window = browser.window();
        window
            .find_element("filename")
            .expect("generated FileBrowser must expose filename")
            .borrow_mut()
            .set_value("child/nested.txt");

        assert_eq!(
            browser.handle_button(&mut window, "open_btn"),
            FileBrowserOutcome::Pending,
        );

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }

    #[test]
    fn file_browser_uses_byte_end_cursor_and_delivers_close_callback()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-callback-{}-{unique}",
            std::process::id(),
        ));
        fs::create_dir_all(&root).expect("temporary browser root must be created");
        let callback_result = Rc::new(RefCell::new(None));
        let callback_capture = callback_result.clone();
        let mut browser = FileBrowser::with_callback(
            &root,
            Some(&root),
            "save",
            "ž.txt",
            "",
            Rc::new(move |result|
            {
                callback_capture.replace(Some(result));
            }),
        );
        let mut window = browser.window();
        let mut behavior = FileBrowserBehavior
        {
            state: browser.state.clone(),
        };
        behavior.handle_edit_started(&mut window, "filename");
        let filename = window
            .find_element("filename")
            .expect("generated FileBrowser must expose filename");
        assert_eq!(filename.borrow().cursor(), "ž.txt".len());

        let selected = root.join("ž.txt");
        browser
            .state
            .borrow_mut()
            .complete(FileBrowserOutcome::Selected(selected.clone()));
        assert_eq!(
            callback_result.borrow().as_deref(),
            Some(selected.to_string_lossy().as_ref()),
        );

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }

    #[test]
    #[allow(deprecated)]
    fn directory_navigation_restores_the_initial_filename_selection()
    {
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system clock must follow Unix epoch")
            .as_nanos();
        let root = std::env::temp_dir().join(format!(
            "uimd-rust-file-browser-selection-{}-{unique}",
            std::process::id(),
        ));
        let child = root.join("images");
        fs::create_dir_all(&child).expect("temporary browser directory must be created");
        fs::write(child.join("initial.png"), "image")
            .expect("temporary selected file must be written");

        let mut browser = FileBrowser::new(
            &root,
            Some(&child),
            "open",
            "initial.png",
            "png",
        );
        let mut window = browser.window();
        assert_eq!(
            window
                .find_element("entries")
                .expect("generated FileBrowser must expose entries")
                .borrow()
                .selected_value(),
            "initial.png",
        );

        window
            .find_element("entries")
            .expect("generated FileBrowser must expose entries")
            .borrow_mut()
            .set_selected_items(&[PARENT_ENTRY.to_string()]);
        assert!(browser.handle_key_before_focused(&mut window, "Enter", "entries", true));

        window
            .find_element("entries")
            .expect("generated FileBrowser must expose entries")
            .borrow_mut()
            .set_selected_items(&["images/".to_string()]);
        assert!(browser.handle_key_before_focused(&mut window, "Enter", "entries", true));
        assert_eq!(
            window
                .find_element("entries")
                .expect("generated FileBrowser must expose entries")
                .borrow()
                .selected_value(),
            "initial.png",
        );

        fs::remove_dir_all(&root).expect("temporary browser root must be removed");
    }
}

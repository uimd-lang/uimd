mod text_editor_ui;

use std::fs;
use std::path::{Path, PathBuf};
use text_editor_ui::*;

const DEFAULT_FILE_NAME: &str = "welcome.txt";
const UNTITLED_NAME: &str = "untitled.txt";

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum PendingAction
{
    New,
    Open,
    Quit,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum BrowserPurpose
{
    Open,
    Save,
}

#[derive(Default)]
struct TextEditorApp
{
    current_path: Option<PathBuf>,
    loaded_text: String,
    pending_action: Option<PendingAction>,
    after_save_action: Option<PendingAction>,
    unsaved_dialog: Option<uimd::MessageBoxYesNoCancel>,
    browser: Option<uimd::FileBrowser>,
    browser_purpose: Option<BrowserPurpose>,
}

impl TextEditorApp
{
    fn initialize(&mut self, ui: &mut TextEditorUI)
    {
        self.load_path(ui, notes_dir().join(default_note_name()));
    }

    fn new_file(&mut self, ui: &mut TextEditorUI)
    {
        self.current_path = None;
        ui.filename.borrow_mut().set_text(UNTITLED_NAME);
        self.set_editor_text(ui, "");
        self.loaded_text.clear();
        self.refresh_status(ui, "New file");
    }

    fn load_path(&mut self, ui: &mut TextEditorUI, path: impl AsRef<Path>)
    {
        let path = absolute_path(path.as_ref());
        let text = fs::read_to_string(&path).unwrap_or_default();
        self.current_path = Some(path.clone());
        ui.filename.borrow_mut().set_text(display_file_name(&path));
        self.set_editor_text(ui, &text);
        self.loaded_text = text;
        self.refresh_status(ui, "Loaded");
    }

    fn save_or_save_as(&mut self, ui: &mut TextEditorUI)
    {
        if let Some(path) = self.current_path.clone()
        {
            self.save_path(ui, path);
        }
        else
        {
            self.open_save_browser(ui);
        }
    }

    fn save_path(&mut self, ui: &mut TextEditorUI, path: impl AsRef<Path>)
    {
        let path = absolute_path(path.as_ref());
        if let Some(parent) = path.parent()
        {
            let _ = fs::create_dir_all(parent);
        }
        let text = ui.editor.borrow().value().to_string();
        let _ = fs::write(&path, &text);
        self.current_path = Some(path.clone());
        ui.filename.borrow_mut().set_text(display_file_name(&path));
        self.loaded_text = text;
        self.refresh_status(ui, "Saved");
    }

    fn with_saved_changes(&mut self, ui: &mut TextEditorUI, action: PendingAction)
    {
        self.after_save_action = None;
        if !self.modified(ui)
        {
            self.perform_action(ui, action);
            return;
        }
        self.pending_action = Some(action);
        let mut dialog = uimd::MessageBoxYesNoCancel::new(
            "Unsaved Changes",
            "Save current file before continuing?",
        );
        ui.base.open_window(dialog.take_window());
        self.unsaved_dialog = Some(dialog);
    }

    fn perform_action(&mut self, ui: &mut TextEditorUI, action: PendingAction)
    {
        match action
        {
            PendingAction::New => self.new_file(ui),
            PendingAction::Open => self.open_browser(ui),
            PendingAction::Quit => ui.base.request_close(),
        }
    }

    fn open_browser(&mut self, ui: &mut TextEditorUI)
    {
        let root = browser_root_dir();
        let mut browser = uimd::FileBrowser::new(
            &root,
            Some(default_browser_dir()),
            "open",
            "",
            "",
        );
        let window = browser.window();
        self.browser = Some(browser);
        self.browser_purpose = Some(BrowserPurpose::Open);
        ui.base.open_window(window);
    }

    fn open_save_browser(&mut self, ui: &mut TextEditorUI)
    {
        let root = browser_root_dir();
        let mut start = self
            .current_path
            .as_ref()
            .and_then(|path| path.parent())
            .map(Path::to_path_buf)
            .unwrap_or_else(notes_dir);
        if !absolute_path(&start).starts_with(absolute_path(&root))
        {
            start = default_browser_dir();
        }
        let initial_filename = self
            .current_path
            .as_ref()
            .and_then(|path| path.file_name())
            .and_then(|name| name.to_str())
            .unwrap_or(UNTITLED_NAME)
            .to_string();
        let mut browser = uimd::FileBrowser::new(
            &root,
            Some(start),
            "save",
            initial_filename,
            "",
        );
        let window = browser.window();
        self.browser = Some(browser);
        self.browser_purpose = Some(BrowserPurpose::Save);
        ui.base.open_window(window);
    }

    fn handle_unsaved_result(&mut self, ui: &mut TextEditorUI, result: uimd::MessageBoxResult)
    {
        let action = self.pending_action.take();
        let Some(action) = action else
        {
            self.refresh_status(ui, "Cancelled");
            return;
        };
        match result
        {
            uimd::MessageBoxResult::Cancel =>
            {
                self.refresh_status(ui, "Cancelled");
            }
            uimd::MessageBoxResult::Yes =>
            {
                if self.current_path.is_none()
                {
                    self.after_save_action = Some(action);
                    self.open_save_browser(ui);
                }
                else
                {
                    self.save_or_save_as(ui);
                    self.perform_action(ui, action);
                }
            }
            uimd::MessageBoxResult::No =>
            {
                self.perform_action(ui, action);
            }
            uimd::MessageBoxResult::Ok => {}
        }
    }

    fn handle_browser_outcome(
        &mut self,
        ui: &mut TextEditorUI,
        outcome: uimd::FileBrowserOutcome,
    )
    {
        match outcome
        {
            uimd::FileBrowserOutcome::Pending => {}
            uimd::FileBrowserOutcome::Cancelled =>
            {
                self.browser = None;
                self.browser_purpose = None;
                if self.after_save_action.take().is_some()
                {
                    self.refresh_status(ui, "Cancelled");
                }
            }
            uimd::FileBrowserOutcome::ConfirmOverwrite(path) =>
            {
                let _ = path;
            }
            uimd::FileBrowserOutcome::Selected(path) =>
            {
                self.complete_browser_selection(ui, path);
            }
        }
    }

    fn complete_browser_selection(&mut self, ui: &mut TextEditorUI, path: PathBuf)
    {
        let purpose = self.browser_purpose.take();
        self.browser = None;
        match purpose
        {
            Some(BrowserPurpose::Open) => self.load_path(ui, path),
            Some(BrowserPurpose::Save) =>
            {
                self.save_path(ui, path);
                if let Some(action) = self.after_save_action.take()
                {
                    self.perform_action(ui, action);
                }
            }
            None => {}
        }
    }

    fn modified(&self, ui: &TextEditorUI) -> bool
    {
        ui.editor.borrow().value() != self.loaded_text
    }

    fn set_editor_text(&self, ui: &mut TextEditorUI, text: &str)
    {
        let mut editor = ui.editor.borrow_mut();
        editor.set_value(text);
        editor.set_cursor(0);
        editor.clear_selection();
        editor.scroll_to(0);
    }

    fn refresh_status(&self, ui: &mut TextEditorUI, prefix: &str)
    {
        let text = ui.editor.borrow().value().to_string();
        let line_count = if text.is_empty()
        {
            0
        }
        else
        {
            text.chars().filter(|ch| *ch == '\n').count() + 1
        };
        let modified = if self.modified(ui) { "*" } else { "" };
        ui.status.borrow_mut().set_text(format!(
            "{prefix}: {}{modified} - {line_count} lines, {} chars",
            ui.filename.borrow().text(),
            text.chars().count(),
        ));
    }
}

impl TextEditorUIEvents for TextEditorApp
{
    fn handle_active_window_button(&mut self, ui: &mut TextEditorUI, name: &str) -> bool
    {
        if name != "open_btn" || self.browser_purpose != Some(BrowserPurpose::Open)
        {
            return false;
        }
        let filename = ui
            .base
            .active_window()
            .find_element("filename")
            .map(|element| element.borrow().value().trim().to_string())
            .unwrap_or_default();
        if filename.is_empty()
        {
            return false;
        }
        let Some(browser) = self.browser.as_mut() else { return false };
        let path = browser.current_dir().join(filename);
        if !path.is_file()
        {
            return false;
        }
        browser.close(path.to_string_lossy());
        true
    }

    fn on_new_btn_click(&mut self, ui: &mut TextEditorUI)
    {
        self.with_saved_changes(ui, PendingAction::New);
    }

    fn on_open_btn_click(&mut self, ui: &mut TextEditorUI)
    {
        self.with_saved_changes(ui, PendingAction::Open);
    }

    fn on_save_btn_click(&mut self, ui: &mut TextEditorUI)
    {
        self.save_or_save_as(ui);
    }

    fn on_save_as_btn_click(&mut self, ui: &mut TextEditorUI)
    {
        self.open_save_browser(ui);
    }

    fn on_quit_btn_click(&mut self, ui: &mut TextEditorUI)
    {
        self.with_saved_changes(ui, PendingAction::Quit);
    }

    fn on_editor_change(&mut self, ui: &mut TextEditorUI, _value: &str)
    {
        self.refresh_status(ui, "Modified");
    }

    fn on_window_closed(
        &mut self,
        ui: &mut TextEditorUI,
        window: uimd::GeneratedWindow,
    )
    {
        if self
            .unsaved_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            let dialog = self
                .unsaved_dialog
                .take()
                .expect("matching unsaved dialog must remain owned");
            if let Some(result) = dialog.result()
            {
                self.handle_unsaved_result(ui, result);
            }
            return;
        }
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
            self.handle_browser_outcome(ui, outcome);
        }
    }
}

fn env_path(name: &str) -> Option<PathBuf>
{
    std::env::var_os(name).filter(|value| !value.is_empty()).map(PathBuf::from)
}

fn notes_dir() -> PathBuf
{
    env_path("UI_TEXT_EDITOR_NOTES_DIR")
        .unwrap_or_else(|| PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("notes"))
}

fn default_note_name() -> String
{
    std::env::var("UI_TEXT_EDITOR_DEFAULT_NOTE")
        .ok()
        .filter(|value| !value.is_empty())
        .unwrap_or_else(|| DEFAULT_FILE_NAME.to_string())
}

fn browser_root_dir() -> PathBuf
{
    env_path("UI_TEXT_EDITOR_BROWSER_ROOT_DIR")
        .or_else(|| env_path("HOME"))
        .unwrap_or_else(|| std::env::current_dir().unwrap_or_else(|_| PathBuf::from(".")))
}

fn default_browser_dir() -> PathBuf
{
    env_path("UI_TEXT_EDITOR_BROWSER_DIR").unwrap_or_else(browser_root_dir)
}

fn absolute_path(path: &Path) -> PathBuf
{
    path.canonicalize().unwrap_or_else(|_|
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
    })
}

fn display_file_name(path: &Path) -> String
{
    path.file_name()
        .and_then(|name| name.to_str())
        .unwrap_or_else(|| path.to_str().unwrap_or_default())
        .to_string()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = TextEditorUI::new();
        let mut app = TextEditorApp::default();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

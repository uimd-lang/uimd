mod activity_feed_ui;
#[path = "activity_feed_panel/activity_feed_panel_ui.rs"]
mod activity_feed_panel_ui;
#[path = "activity_item/activity_item_ui.rs"]
mod activity_item_ui;
#[path = "settings/settings_ui.rs"]
mod settings_ui;

use activity_feed_panel_ui::ActivityFeedPanelUI;
use activity_feed_ui::*;
use activity_item_ui::ActivityItemUI;
use settings_ui::SettingsUI;
use std::cell::RefCell;
use std::cmp::max;
use std::rc::Rc;

const DEFAULT_ACTIVITY_TYPE: &str = "Info";
const TIMESTAMP_ENV: &str = "UIMD_ACTIVITY_FEED_TIMESTAMP";
const INITIAL_SAMPLE_COUNT: usize = 2;
const SAMPLE_ACTIVITIES: [(&str, &str); 6] = [
    ("Info", "Workspace opened"),
    ("Task", "Report export queued"),
    ("Deploy", "Version 1.4 deployed"),
    ("Warning", "Review queue is almost full"),
    ("Note", "Follow-up scheduled"),
    ("Task", "Background sync finished"),
];

#[derive(Clone)]
struct ActivityItemData
{
    timestamp: String,
    event_type: String,
    message: String,
    show_timestamp: bool,
}

struct ActivityFeedApp
{
    auto_scroll: bool,
    show_timestamps: bool,
    default_type: String,
    activities: Rc<RefCell<Vec<ActivityItemData>>>,
    settings_dialog: Option<uimd::DialogActionHandle>,
}

impl Default for ActivityFeedApp
{
    fn default() -> Self
    {
        Self
        {
            auto_scroll: true,
            show_timestamps: true,
            default_type: DEFAULT_ACTIVITY_TYPE.to_string(),
            activities: Rc::new(RefCell::new(Vec::new())),
            settings_dialog: None,
        }
    }
}

fn wrapped_line_count(text: &str, width: i32) -> i32
{
    let width = max(1, width) as usize;
    max(
        1,
        text.split('\n')
            .map(|line| max(1, line.len().div_ceil(width) as i32))
            .sum(),
    )
}

fn generated_source_height(window: &uimd::GeneratedWindow) -> i32
{
    window
        .layout
        .iter()
        .map(|item| item.cell_row + item.cell_chars_height)
        .max()
        .unwrap_or(1)
        .max(1)
}

fn compact_content_height(window: &uimd::GeneratedWindow, message_rows: i32) -> i32
{
    window
        .layout
        .iter()
        .filter(|item| !item.element_type.is_empty())
        .map(|item|
        {
            item.row
                + if item.content == "message"
                {
                    max(1, message_rows)
                }
                else
                {
                    max(1, item.chars_height)
                }
        })
        .max()
        .unwrap_or(1)
        .max(1)
}

fn render_activity_item(activity: &ActivityItemData, width: i32) -> uimd::TerminalBuffer
{
    let mut item = ActivityItemUI::new();
    item.timestamp.borrow_mut().set_text(if activity.show_timestamp
    {
        activity.timestamp.clone()
    }
    else
    {
        String::new()
    });
    item.event_type.borrow_mut().set_text(&activity.event_type);
    item.message.borrow_mut().set_text(&activity.message);
    let marker_style = format!("marker_{}", activity.event_type.to_ascii_lowercase());
    item.base.apply_named_cell_style("marker", &marker_style);

    let width = max(1, width);
    let source_height = generated_source_height(&item.base);
    let _ = uimd::render_generated_window_content(&mut item.base, width, source_height);
    let message_width = max(1, item.message.borrow().frame.width);
    let message_rows = wrapped_line_count(&activity.message, message_width);
    let compact_height = compact_content_height(&item.base, message_rows);
    uimd::render_generated_window_content(&mut item.base, width, compact_height)
}

impl ActivityFeedApp
{
    fn open(&mut self, ui: &mut ActivityFeedUI)
    {
        let mut panel = ActivityFeedPanelUI::new();
        let activities = Rc::clone(&self.activities);
        panel.base.set_dynamic_children_renderer(move |width|
        {
            activities
                .borrow()
                .iter()
                .map(|activity| render_activity_item(activity, width))
                .collect()
        });
        ui.feed.borrow_mut().set_child_window(panel.base);
        self.select_activity_type(ui, &self.default_type.clone());
        self.with_feed_panel(ui, |feed| feed.set_auto_scroll(self.auto_scroll));
        for (event_type, message) in SAMPLE_ACTIVITIES.iter().take(INITIAL_SAMPLE_COUNT)
        {
            self.append_activity(ui, event_type, message);
        }
        self.update_count_status(ui);
    }

    fn with_feed_panel(
        &self,
        ui: &ActivityFeedUI,
        operation: impl FnOnce(&mut uimd::GeneratedWindow),
    )
    {
        let mut feed = ui.feed.borrow_mut();
        if let Some(panel) = feed.child_window_mut()
        {
            operation(panel);
        }
    }

    fn activity_count(&self, _ui: &ActivityFeedUI) -> usize
    {
        self.activities.borrow().len()
    }

    fn current_timestamp() -> String
    {
        if let Ok(value) = std::env::var(TIMESTAMP_ENV)
            && !value.is_empty()
        {
            return value;
        }
        let seconds = std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .map_or(0, |duration| duration.as_secs());
        format!("{:02}:{:02}", (seconds / 3_600) % 24, (seconds / 60) % 60)
    }

    fn append_activity(&self, ui: &ActivityFeedUI, event_type: &str, message: &str)
    {
        self.activities.borrow_mut().push(ActivityItemData
        {
            timestamp: Self::current_timestamp(),
            event_type: event_type.to_string(),
            message: message.to_string(),
            show_timestamp: self.show_timestamps,
        });
        self.with_feed_panel(ui, |feed|
        {
            feed.invalidate_dynamic_children();
            if self.auto_scroll
            {
                let _ = feed.scroll_to_bottom();
            }
        });
    }

    fn selected_activity_type(&self, ui: &ActivityFeedUI) -> String
    {
        let selected = ui.activity_type.borrow().selected_value();
        if selected.is_empty() { self.default_type.clone() } else { selected }
    }

    fn add_current_activity(&self, ui: &mut ActivityFeedUI)
    {
        let message = ui.message.borrow().text().trim().to_string();
        if message.is_empty()
        {
            ui.status.borrow_mut().set_text("Enter an activity message.");
            return;
        }
        let event_type = self.selected_activity_type(ui);
        self.append_activity(ui, &event_type, &message);
        let mut input = ui.message.borrow_mut();
        input.set_value("");
        input.set_cursor(0);
        drop(input);
        self.update_count_status(ui);
    }

    fn seed_activities(&self, ui: &mut ActivityFeedUI)
    {
        for (event_type, message) in SAMPLE_ACTIVITIES
        {
            self.append_activity(ui, event_type, message);
        }
        self.update_count_status(ui);
    }

    fn clear_activities(&self, ui: &mut ActivityFeedUI)
    {
        self.activities.borrow_mut().clear();
        self.with_feed_panel(ui, uimd::GeneratedWindow::clear_children);
        ui.status.borrow_mut().set_text("Feed cleared");
    }

    fn update_count_status(&self, ui: &mut ActivityFeedUI)
    {
        ui.status
            .borrow_mut()
            .set_text(format!("{} activities", self.activity_count(ui)));
    }

    fn select_activity_type(&self, ui: &ActivityFeedUI, value: &str)
    {
        let index = ui
            .activity_type
            .borrow()
            .options()
            .iter()
            .position(|option| option == value)
            .map_or(0, |index| index as i32);
        ui.activity_type.borrow_mut().set_selected_index(index);
    }

    fn open_settings(&mut self, ui: &mut ActivityFeedUI)
    {
        let settings = SettingsUI::new();
        settings.auto_scroll.borrow_mut().set_checked(self.auto_scroll);
        settings.show_timestamps.borrow_mut().set_checked(self.show_timestamps);
        let index = settings
            .default_type
            .borrow()
            .options()
            .iter()
            .position(|option| option == &self.default_type)
            .map_or(0, |index| index as i32);
        settings.default_type.borrow_mut().set_selected_index(index);
        let mut window = settings.base;
        self.settings_dialog = Some(uimd::configure_dialog_actions(
            &mut window,
            [("save_button", "save"), ("cancel_button", "cancel")],
            "cancel",
        ));
        ui.base.open_window(window);
    }

    fn close_settings(
        &mut self,
        ui: &mut ActivityFeedUI,
        window: &uimd::GeneratedWindow,
        save: bool,
    )
    {
        if save
        {
            self.auto_scroll = window
                .find_element("auto_scroll")
                .is_some_and(|element| element.borrow().checked());
            self.show_timestamps = window
                .find_element("show_timestamps")
                .is_some_and(|element| element.borrow().checked());
            self.default_type = window
                .find_element("default_type")
                .map_or_else(|| DEFAULT_ACTIVITY_TYPE.to_string(), |element| element.borrow().selected_value());
        }
        if save
        {
            self.select_activity_type(ui, &self.default_type);
            self.with_feed_panel(ui, |feed| feed.set_auto_scroll(self.auto_scroll));
            ui.status.borrow_mut().set_text("Settings saved");
        }
        else
        {
            ui.status.borrow_mut().set_text("Settings canceled");
        }
    }
}

impl ActivityFeedUIEvents for ActivityFeedApp
{
    fn on_settings_button_click(&mut self, ui: &mut ActivityFeedUI)
    {
        self.open_settings(ui);
    }

    fn on_quit_button_click(&mut self, ui: &mut ActivityFeedUI)
    {
        ui.base.request_close();
    }

    fn on_add_button_click(&mut self, ui: &mut ActivityFeedUI)
    {
        self.add_current_activity(ui);
    }

    fn on_seed_button_click(&mut self, ui: &mut ActivityFeedUI)
    {
        self.seed_activities(ui);
    }

    fn on_clear_button_click(&mut self, ui: &mut ActivityFeedUI)
    {
        self.clear_activities(ui);
    }

    fn on_message_submit(&mut self, ui: &mut ActivityFeedUI, value: &str)
    {
        ui.message.borrow_mut().set_value(value);
    }

    fn on_window_closed(
        &mut self,
        ui: &mut ActivityFeedUI,
        window: uimd::GeneratedWindow,
    )
    {
        if !self
            .settings_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            return;
        }
        let result = self
            .settings_dialog
            .take()
            .and_then(|dialog| dialog.result());
        self.close_settings(ui, &window, result.as_deref() == Some("save"));
    }
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ActivityFeedUI::new();
        let mut app = ActivityFeedApp::default();
        app.open(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

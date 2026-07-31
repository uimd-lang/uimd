mod task_board_ui;
#[path = "task_dialog/task_dialog_ui.rs"]
mod task_dialog_ui;
#[path = "task_filters/task_filters_ui.rs"]
mod task_filters_ui;
#[path = "task_list/task_list_ui.rs"]
mod task_list_ui;
#[path = "task_row/task_row_ui.rs"]
mod task_row_ui;

use serde_json::{json, Map, Value};
use task_board_ui::*;
use task_dialog_ui::TaskDialogUI;
use task_filters_ui::TaskFiltersUI;
use task_list_ui::TaskListUI;
use task_row_ui::TaskRowUI;
use std::cell::RefCell;
use std::rc::Rc;

const ANY_FILTER: &str = "Any";
const DEFAULT_ASSIGNEE: &str = "Ava";
const DEFAULT_PRIORITY: &str = "Med";
const DEFAULT_STATUS: &str = "Todo";
const DONE_STATUS: &str = "Done";
const TASK_ID_PREFIX: &str = "t-";
const INITIAL_TASK_NUMBER_BASE: u32 = 100;
const CANCELED_STATUS: &str = "Action canceled.";
const MARK_ALL_DONE_STATUS: &str = "All tasks marked done.";
const CLEAR_BOARD_STATUS: &str = "Board cleared.";

#[derive(Clone, Debug)]
struct Task
{
    id: String,
    title: String,
    status: String,
    assignee: String,
    priority: String,
    description: String,
    done: bool,
}

impl Task
{
    fn json(&self) -> Value
    {
        json!({
            "id": self.id,
            "title": self.title,
            "status": self.status,
            "assignee": self.assignee,
            "priority": self.priority,
            "description": self.description,
            "done": self.done,
        })
    }
}

struct FilterRefs
{
    search: uimd::ElementRef,
    status: uimd::ElementRef,
    owner: uimd::ElementRef,
}

struct DialogRefs
{
    task_id: String,
    name: uimd::ElementRef,
    status: uimd::ElementRef,
    owner: uimd::ElementRef,
    priority: uimd::ElementRef,
    note: uimd::ElementRef,
}

enum PendingConfirmation
{
    Delete(String),
    MarkAllDone,
    ClearBoard,
}

struct TaskBoardApp
{
    tasks: Vec<Task>,
    filters: Option<FilterRefs>,
    dialog: Option<DialogRefs>,
    task_dialog_identity: Option<u64>,
    task_dialog_result: Option<Rc<RefCell<Option<String>>>>,
    pending_confirmation: Option<PendingConfirmation>,
    confirmation_dialog: Option<uimd::MessageBoxYesNo>,
}

impl Default for TaskBoardApp
{
    fn default() -> Self
    {
        Self
        {
            tasks: seed_tasks(),
            filters: None,
            dialog: None,
            task_dialog_identity: None,
            task_dialog_result: None,
            pending_confirmation: None,
            confirmation_dialog: None,
        }
    }
}

struct TaskDialogBehavior
{
    result: Rc<RefCell<Option<String>>>,
}

impl uimd::GeneratedWindowBehavior for TaskDialogBehavior
{
    fn handle_button(
        &mut self,
        window: &mut uimd::GeneratedWindow,
        name: &str,
    ) -> bool
    {
        if name == "cancel_btn"
        {
            self.result.replace(Some("cancel".to_string()));
            window.request_close();
            return true;
        }
        if name != "add_btn"
        {
            return false;
        }
        let title = window
            .find_element("name")
            .map(|name| name.borrow().value().trim().to_string())
            .unwrap_or_default();
        if title.is_empty()
        {
            if let Some(message) = window.find_element("message")
            {
                message.borrow_mut().set_text("Title is required.");
            }
            return true;
        }
        self.result.replace(Some("save".to_string()));
        window.request_close();
        true
    }

    fn handle_key(&mut self, window: &mut uimd::GeneratedWindow, key: &str) -> bool
    {
        if key != "Escape"
        {
            return false;
        }
        self.result.replace(Some("cancel".to_string()));
        window.request_close();
        true
    }

    fn should_close(&self) -> bool
    {
        self.result.borrow().is_some()
    }
}

impl TaskBoardApp
{
    fn initialize(&mut self, ui: &mut TaskBoardUI)
    {
        let filters = TaskFiltersUI::new();
        self.filters = Some(FilterRefs
        {
            search: filters.search.clone().into(),
            status: filters.status_filter.clone().into(),
            owner: filters.owner_filter.clone().into(),
        });
        ui.filters.borrow_mut().set_child_window(filters.base);

        let list = TaskListUI::new();
        ui.board.borrow_mut().set_child_window(list.base);
        self.refresh_board(ui);
    }

    fn matching_tasks(&self) -> Vec<Task>
    {
        let Some(filters) = &self.filters else { return self.tasks.clone() };
        let query = filters.search.borrow().value().to_ascii_lowercase();
        let status = selected_value(&filters.status, ANY_FILTER);
        let owner = selected_value(&filters.owner, ANY_FILTER);
        self.tasks
            .iter()
            .filter(|task|
            {
                let haystack = format!(
                    "{} {} {} {}",
                    task.title,
                    task.description,
                    task.assignee,
                    task.priority,
                )
                .to_ascii_lowercase();
                (query.is_empty() || haystack.contains(&query))
                    && (status == ANY_FILTER || task.status == status)
                    && (owner == ANY_FILTER || task.assignee == owner)
            })
            .cloned()
            .collect()
    }

    fn refresh_board(&self, ui: &mut TaskBoardUI)
    {
        let visible = self.matching_tasks();
        let mut board = ui.board.borrow_mut();
        let viewport = uimd::Size
        {
            width: board.frame.width,
            height: board.frame.height,
        };
        let Some(list) = board.child_window_mut() else { return };
        list.clear_children();
        for (index, task) in visible.iter().enumerate()
        {
            let row = TaskRowUI::new();
            row.task_title.borrow_mut().set_text(&task.title);
            row.meta.borrow_mut().set_text(format!(
                "{} / {} / {}",
                task.status,
                task.assignee,
                task.priority,
            ));
            row.done.borrow_mut().set_checked(task.done);
            list.add_named_child_window(format!("board[{index}]"), row.base);
        }
        if viewport.width > 0 && viewport.height > 0
        {
            list.scroll_to_top_in(viewport);
        }
        drop(board);
        ui.status.borrow_mut().set_text(format!(
            "{} visible / {} total",
            visible.len(),
            self.tasks.len(),
        ));
    }

    fn reset_filters(&self)
    {
        let Some(filters) = &self.filters else { return };
        {
            let mut search = filters.search.borrow_mut();
            search.set_value("");
            search.set_cursor(0);
            search.clear_selection();
        }
        select_value(&filters.status, ANY_FILTER);
        select_value(&filters.owner, ANY_FILTER);
    }

    fn open_task_dialog(&mut self, ui: &mut TaskBoardUI, task_id: Option<&str>)
    {
        let task = task_id.and_then(|id| self.task_by_id(id)).cloned();
        let dialog = TaskDialogUI::new();
        let stored_id = task.as_ref().map_or_else(String::new, |task| task.id.clone());
        if let Some(task) = &task
        {
            dialog.header.borrow_mut().set_text("Edit task");
            dialog.name.borrow_mut().set_value(&task.title);
            select_value(&dialog.project_status, &task.status);
            select_value(&dialog.owner, &task.assignee);
            select_value(&dialog.priority, &task.priority);
            dialog.note.borrow_mut().set_value(&task.description);
        }
        else
        {
            dialog.header.borrow_mut().set_text("New task");
        }
        self.dialog = Some(DialogRefs
        {
            task_id: stored_id,
            name: dialog.name.clone().into(),
            status: dialog.project_status.clone().into(),
            owner: dialog.owner.clone().into(),
            priority: dialog.priority.clone().into(),
            note: dialog.note.clone().into(),
        });
        let mut window = dialog.base;
        let result = Rc::new(RefCell::new(None));
        self.task_dialog_identity = Some(window.identity());
        self.task_dialog_result = Some(result.clone());
        window.set_behavior(TaskDialogBehavior { result });
        ui.base.open_window(window);
    }

    fn save_dialog(&mut self, ui: &mut TaskBoardUI)
    {
        let Some(dialog) = self.dialog.as_ref() else { return };
        let title = dialog.name.borrow().value().trim().to_string();
        let task_id = dialog.task_id.clone();
        let status = selected_value(&dialog.status, DEFAULT_STATUS);
        let assignee = selected_value(&dialog.owner, DEFAULT_ASSIGNEE);
        let priority = selected_value(&dialog.priority, DEFAULT_PRIORITY);
        let description = dialog.note.borrow().value().trim().to_string();
        let done = status == DONE_STATUS;
        self.dialog = None;

        let index = if task_id.is_empty()
        {
            None
        }
        else
        {
            self.tasks.iter().position(|task| task.id == task_id)
        };
        let index = index.unwrap_or_else(||
        {
            self.tasks.insert(0, Task
            {
                id: self.next_task_id(),
                title: String::new(),
                status: DEFAULT_STATUS.to_string(),
                assignee: DEFAULT_ASSIGNEE.to_string(),
                priority: DEFAULT_PRIORITY.to_string(),
                description: String::new(),
                done: false,
            });
            self.reset_filters();
            0
        });
        let id = self.tasks[index].id.clone();
        self.tasks[index] = sync_done_status(Task
        {
            id,
            title,
            status,
            assignee,
            priority,
            description,
            done,
        });
        self.refresh_board(ui);
        ui.status.borrow_mut().set_text("Task saved.");
    }

    fn cancel_dialog(&mut self, ui: &mut TaskBoardUI)
    {
        self.dialog = None;
        ui.status.borrow_mut().set_text(CANCELED_STATUS);
    }

    fn set_task_done(&mut self, ui: &mut TaskBoardUI, id: &str, done: bool)
    {
        let Some(index) = self.tasks.iter().position(|task| task.id == id) else { return };
        self.tasks[index].done = done;
        let task = sync_done_status(self.tasks[index].clone());
        let title = task.title.clone();
        self.tasks[index] = task;
        ui.status
            .borrow_mut()
            .set_text(format!("Updated {title}"));
    }

    fn confirm_delete(&mut self, ui: &mut TaskBoardUI, id: &str)
    {
        let Some(task) = self.task_by_id(id) else { return };
        let title = task.title.clone();
        self.pending_confirmation = Some(PendingConfirmation::Delete(id.to_string()));
        let mut dialog = uimd::MessageBoxYesNo::new(
            "Delete Task",
            &format!("Delete {title}?"),
        );
        ui.base.open_window(dialog.take_window());
        self.confirmation_dialog = Some(dialog);
    }

    fn confirm_bulk(
        &mut self,
        ui: &mut TaskBoardUI,
        pending: PendingConfirmation,
        header: &str,
        message: &str,
    )
    {
        self.pending_confirmation = Some(pending);
        let mut dialog = uimd::MessageBoxYesNo::new(header, message);
        ui.base.open_window(dialog.take_window());
        self.confirmation_dialog = Some(dialog);
    }

    fn handle_confirmation(
        &mut self,
        ui: &mut TaskBoardUI,
        result: uimd::MessageBoxResult,
    )
    {
        let pending = self.pending_confirmation.take();
        if result != uimd::MessageBoxResult::Yes
        {
            ui.status.borrow_mut().set_text(CANCELED_STATUS);
            return;
        }
        match pending
        {
            Some(PendingConfirmation::Delete(id)) =>
            {
                let before = self.tasks.len();
                self.tasks.retain(|task| task.id != id);
                if self.tasks.len() != before
                {
                    self.refresh_board(ui);
                    ui.status.borrow_mut().set_text("Task deleted.");
                }
            }
            Some(PendingConfirmation::MarkAllDone) =>
            {
                for task in &mut self.tasks
                {
                    task.done = true;
                    task.status = DONE_STATUS.to_string();
                }
                self.reset_filters();
                self.refresh_board(ui);
                ui.status.borrow_mut().set_text(MARK_ALL_DONE_STATUS);
            }
            Some(PendingConfirmation::ClearBoard) =>
            {
                self.tasks.clear();
                self.reset_filters();
                self.refresh_board(ui);
                ui.status.borrow_mut().set_text(CLEAR_BOARD_STATUS);
            }
            None => {}
        }
    }

    fn dynamic_element_action(
        &mut self,
        ui: &mut TaskBoardUI,
        element_id: &str,
    ) -> bool
    {
        let Some((index, member)) = indexed_member(element_id, "board") else { return false };
        let Some(task) = self.matching_tasks().get(index).cloned() else { return false };
        match member
        {
            "open_btn" => self.open_task_dialog(ui, Some(&task.id)),
            "delete_btn" => self.confirm_delete(ui, &task.id),
            _ => return false,
        }
        true
    }

    fn dynamic_element_change(
        &mut self,
        ui: &mut TaskBoardUI,
        element_id: &str,
        _value: &str,
    ) -> bool
    {
        let Some((index, member)) = indexed_member(element_id, "board") else { return false };
        if member != "done"
        {
            return false;
        }
        let Some(task) = self.matching_tasks().get(index).cloned() else { return false };
        let done = ui
            .base
            .find_element(element_id)
            .is_some_and(|element| element.borrow().checked());
        self.set_task_done(ui, &task.id, done);
        true
    }

    fn query_tasks(&self, arguments: &Map<String, Value>) -> Value
    {
        let query = string_arg(arguments, "title")
            .or_else(|| string_arg(arguments, "name"))
            .unwrap_or_default()
            .to_ascii_lowercase();
        let status = string_arg(arguments, "status").unwrap_or(ANY_FILTER);
        let assignee = string_arg(arguments, "assignee")
            .or_else(|| string_arg(arguments, "owner"))
            .unwrap_or(ANY_FILTER);
        let priority = string_arg(arguments, "priority").unwrap_or(ANY_FILTER);
        let done = arguments.get("done").and_then(Value::as_bool);
        let tasks = self
            .tasks
            .iter()
            .filter(|task|
            {
                let haystack = format!(
                    "{} {} {} {}",
                    task.title,
                    task.description,
                    task.assignee,
                    task.priority,
                )
                .to_ascii_lowercase();
                (query.is_empty() || haystack.contains(&query))
                    && (status == ANY_FILTER || task.status == status)
                    && (assignee == ANY_FILTER || task.assignee == assignee)
                    && (priority == ANY_FILTER || task.priority == priority)
                    && done.is_none_or(|done| task.done == done)
            })
            .map(Task::json)
            .collect::<Vec<_>>();
        json!({ "tasks": tasks })
    }

    fn get_task(&self, arguments: &Map<String, Value>) -> Value
    {
        let task = string_arg(arguments, "id")
            .and_then(|id| self.task_by_id(id))
            .map(Task::json);
        json!({ "task": task })
    }

    fn update_task(&mut self, ui: &mut TaskBoardUI, arguments: &Map<String, Value>) -> Value
    {
        let Some(id) = string_arg(arguments, "id") else
        {
            return json!({ "task": null });
        };
        let Some(index) = self.tasks.iter().position(|task| task.id == id) else
        {
            return json!({ "task": null });
        };
        let mut task = self.tasks[index].clone();
        if let Some(value) = string_arg(arguments, "title") { task.title = value.to_string(); }
        if let Some(value) = string_arg(arguments, "status") { task.status = value.to_string(); }
        if let Some(value) = string_arg(arguments, "assignee") { task.assignee = value.to_string(); }
        if let Some(value) = string_arg(arguments, "priority") { task.priority = value.to_string(); }
        if let Some(value) = string_arg(arguments, "description") { task.description = value.to_string(); }
        if let Some(value) = arguments.get("done").and_then(Value::as_bool) { task.done = value; }
        task = sync_done_status(task);
        self.tasks[index] = task.clone();
        self.refresh_board(ui);
        json!({ "task": task.json() })
    }

    fn delete_task(&mut self, ui: &mut TaskBoardUI, arguments: &Map<String, Value>) -> Value
    {
        let id = string_arg(arguments, "id").unwrap_or_default();
        let before = self.tasks.len();
        self.tasks.retain(|task| task.id != id);
        let deleted = self.tasks.len() != before;
        if deleted
        {
            self.refresh_board(ui);
            ui.status.borrow_mut().set_text("Task deleted.");
        }
        json!({ "deleted": deleted })
    }

    fn task_by_id(&self, id: &str) -> Option<&Task>
    {
        self.tasks.iter().find(|task| task.id == id)
    }

    fn next_task_id(&self) -> String
    {
        let next = self
            .tasks
            .iter()
            .filter_map(|task| task.id.strip_prefix(TASK_ID_PREFIX)?.parse::<u32>().ok())
            .max()
            .map_or(INITIAL_TASK_NUMBER_BASE + 1, |number| number + 1);
        format!("{TASK_ID_PREFIX}{next}")
    }
}

impl TaskBoardUIEvents for TaskBoardApp
{
    fn on_quit_btn_click(&mut self, ui: &mut TaskBoardUI)
    {
        ui.base.request_close();
    }

    fn on_new_task_btn_click(&mut self, ui: &mut TaskBoardUI)
    {
        self.open_task_dialog(ui, None);
    }

    fn on_mark_all_done_btn_click(&mut self, ui: &mut TaskBoardUI)
    {
        self.confirm_bulk(
            ui,
            PendingConfirmation::MarkAllDone,
            "Mark All Done",
            "Mark every task as done?",
        );
    }

    fn on_clear_board_btn_click(&mut self, ui: &mut TaskBoardUI)
    {
        self.confirm_bulk(
            ui,
            PendingConfirmation::ClearBoard,
            "Clear Board",
            "Delete every task from the board?",
        );
    }

    fn handle_dynamic_button(&mut self, ui: &mut TaskBoardUI, name: &str) -> bool
    {
        if name == "filters.apply_filters_btn"
        {
            self.refresh_board(ui);
            return true;
        }
        if name == "filters.reset_filters_btn"
        {
            self.reset_filters();
            self.refresh_board(ui);
            return true;
        }
        self.dynamic_element_action(ui, name)
    }

    fn handle_dynamic_text_changed(
        &mut self,
        ui: &mut TaskBoardUI,
        name: &str,
        value: &str,
    ) -> bool
    {
        self.dynamic_element_change(ui, name, value)
    }

    fn on_window_closed(
        &mut self,
        ui: &mut TaskBoardUI,
        window: uimd::GeneratedWindow,
    )
    {
        if self.task_dialog_identity == Some(window.identity())
        {
            let result = self
                .task_dialog_result
                .take()
                .and_then(|result| result.borrow().clone());
            self.task_dialog_identity = None;
            if result.as_deref() == Some("save")
            {
                self.save_dialog(ui);
            }
            else
            {
                self.cancel_dialog(ui);
            }
            return;
        }
        if !self
            .confirmation_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            return;
        }
        let Some(dialog) = self.confirmation_dialog.take() else { return };
        if let Some(result) = dialog.result()
        {
            self.handle_confirmation(ui, result);
        }
    }

    fn mcp_tool_names(&self) -> Vec<String>
    {
        ["query_tasks", "get_task", "update_task", "delete_task"]
            .into_iter()
            .map(str::to_string)
            .collect()
    }

    fn handle_mcp_tool(
        &mut self,
        ui: &mut TaskBoardUI,
        name: &str,
        arguments: &Map<String, Value>,
    ) -> Option<Value>
    {
        match name
        {
            "query_tasks" => Some(self.query_tasks(arguments)),
            "get_task" => Some(self.get_task(arguments)),
            "update_task" => Some(self.update_task(ui, arguments)),
            "delete_task" => Some(self.delete_task(ui, arguments)),
            _ => None,
        }
    }
}

fn selected_value(element: &uimd::ElementRef, fallback: &str) -> String
{
    let selected = element.borrow().selected_value();
    if selected.is_empty() { fallback.to_string() } else { selected }
}

fn select_value(element: &uimd::ElementRef, value: &str)
{
    let index = element
        .borrow()
        .options()
        .iter()
        .position(|option| option == value)
        .map_or(0, |index| index as i32);
    element.borrow_mut().set_selected_index(index);
}

fn indexed_member<'a>(id: &'a str, prefix: &str) -> Option<(usize, &'a str)>
{
    let suffix = id.strip_prefix(prefix)?.strip_prefix('[')?;
    let (index, member) = suffix.split_once("].")?;
    Some((index.parse().ok()?, member))
}

fn string_arg<'a>(arguments: &'a Map<String, Value>, name: &str) -> Option<&'a str>
{
    arguments.get(name).and_then(Value::as_str)
}

fn sync_done_status(mut task: Task) -> Task
{
    if task.done
    {
        task.status = DONE_STATUS.to_string();
    }
    else if task.status == DONE_STATUS
    {
        task.status = DEFAULT_STATUS.to_string();
    }
    task
}

fn seed_tasks() -> Vec<Task>
{
    [
        ("t-101", "Review invoice export", "Doing", "Ava", "High", "Check exported columns and totals before finance signs off.", false),
        ("t-102", "Rewrite signup emails", "Blocked", "Bruno", "Med", "Waiting for brand review on the onboarding sequence.", false),
        ("t-103", "Improve search ranking", "Doing", "Chen", "High", "Tune ranking for exact title matches and recent activity.", false),
        ("t-104", "Draft data deletion policy", "Todo", "Dana", "Urgent", "Prepare policy draft for privacy review.", false),
        ("t-105", "Update support templates", "Todo", "Ava", "Med", "Refresh billing and account recovery snippets.", false),
        ("t-106", "Fix API contract tests", "Blocked", "Chen", "High", "Coordinate schema change with partner sandbox.", false),
        ("t-107", "Clean left navigation", "Todo", "Bruno", "Low", "Align labels and collapse unused spacing in admin views.", false),
        ("t-108", "Finalize launch checklist", "Done", "Dana", "Med", "Checklist completed and reviewed by release owner.", true),
        ("t-109", "Define calendar sharing", "Doing", "Bruno", "Med", "Document private, team, and organization visibility.", false),
        ("t-110", "Clean knowledge editor", "Todo", "Chen", "Low", "Remove stale toolbar actions and simplify publish flow.", false),
        ("t-111", "Publish usage report", "Doing", "Dana", "Med", "Send account summary to customer success leads.", false),
        ("t-112", "Retry failed imports", "Blocked", "Ava", "Urgent", "Needs import worker patch before rerun.", false),
        ("t-113", "Tune digest settings", "Todo", "Bruno", "Med", "Review default digest cadence for new workspaces.", false),
        ("t-114", "Verify account password reset cleanup", "Done", "Chen", "Low", "Old reset token cleanup verified in staging.", true),
    ]
    .into_iter()
    .map(|(id, title, status, assignee, priority, description, done)| Task
    {
        id: id.to_string(),
        title: title.to_string(),
        status: status.to_string(),
        assignee: assignee.to_string(),
        priority: priority.to_string(),
        description: description.to_string(),
        done,
    })
    .collect()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = TaskBoardUI::new();
        let mut app = TaskBoardApp::default();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

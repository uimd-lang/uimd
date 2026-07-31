mod edit_field_row_ui;
mod edit_fields_scroll_ui;
mod edit_page_ui;
mod item_row_ui;
mod list_page_ui;
mod stale_scrollview_focus_ui;

use edit_field_row_ui::EditFieldRowUI;
use edit_fields_scroll_ui::EditFieldsScrollUI;
use edit_page_ui::EditPageUI;
use item_row_ui::ItemRowUI;
use list_page_ui::ListPageUI;
use stale_scrollview_focus_ui::*;

const FIELD_COUNT: usize = 40;

#[derive(Clone)]
struct ReproItem
{
    name: &'static str,
    summary: &'static str,
    body: &'static str,
}

const REPRO_ITEMS: [ReproItem; 5] = [
    ReproItem
    {
        name: "Alpha Apartments",
        summary: "First row, visible without scrolling",
        body: "Alpha item was opened from the top of the ScrollView.\nThe edit page should not keep any stale list focus background.",
    },
    ReproItem
    {
        name: "Bravo Offices",
        summary: "Second row, used by the automated repro",
        body: "Bravo item was opened by pressing Enter on a row button.\nThe old ScrollView page must be gone before this page renders.",
    },
    ReproItem
    {
        name: "Charlie Lofts",
        summary: "Third row, keeps the source list taller",
        body: "Charlie item keeps the source page closer to the reported list shape.\nThe opened edit page still owns the new nested ScrollView.",
    },
    ReproItem
    {
        name: "Delta Studios",
        summary: "Fourth row, manual repro filler",
        body: "Delta item is extra manual test data for scroll and focus traversal.\nActivating Open should replace the whole page cleanly.",
    },
    ReproItem
    {
        name: "Echo Retail",
        summary: "Fifth row, enough rows to browse",
        body: "Echo item makes the source ScrollView visibly list-like.\nThe edit form below contains many alternating controls.",
    },
];

#[derive(Default)]
struct StaleScrollviewFocusApp;

impl StaleScrollviewFocusApp
{
    fn show_list_page(ui: &mut StaleScrollviewFocusUI)
    {
        let mut page = ListPageUI::new();
        for (index, item) in REPRO_ITEMS.iter().enumerate()
        {
            let row = ItemRowUI::new();
            row.name.borrow_mut().set_text(item.name);
            row.summary.borrow_mut().set_text(item.summary);
            page.base
                .add_named_child_window(format!("[{index}]"), row.base);
        }
        ui.page.borrow_mut().set_child_window(page.base);
        ui.hint
            .borrow_mut()
            .set_text("Focus list, press Enter, choose Open.");
    }

    fn show_edit_page(ui: &mut StaleScrollviewFocusUI, item: &ReproItem)
    {
        let page = EditPageUI::new();
        page.edit_title
            .borrow_mut()
            .set_text(format!("Edit {}", item.name));
        page.name_input.borrow_mut().set_value(item.name);
        let mut fields = EditFieldsScrollUI::new();
        for index in 0..FIELD_COUNT
        {
            let row = EditFieldRowUI::new();
            row.field_input
                .borrow_mut()
                .set_value(field_value(item, index));
            row.field_btn
                .borrow_mut()
                .set_title(format!("Action {:02}", index + 1));
            fields
                .base
                .add_named_child_window(format!("[{index}]"), row.base);
        }
        page.fields.borrow_mut().set_child_window(fields.base);
        ui.page.borrow_mut().set_child_window(page.base);
        ui.hint
            .borrow_mut()
            .set_text("Edit page opened from ScrollView row.");
    }
}

impl StaleScrollviewFocusUIEvents for StaleScrollviewFocusApp
{
    fn on_quit_btn_click(&mut self, ui: &mut StaleScrollviewFocusUI)
    {
        ui.base.request_close();
    }

    fn on_tab_list_click(&mut self, ui: &mut StaleScrollviewFocusUI)
    {
        Self::show_list_page(ui);
    }

    fn on_tab_other_click(&mut self, ui: &mut StaleScrollviewFocusUI)
    {
        ui.hint
            .borrow_mut()
            .set_text("Other tab intentionally leaves the list unchanged.");
    }

    fn handle_dynamic_button(
        &mut self,
        ui: &mut StaleScrollviewFocusUI,
        name: &str,
    ) -> bool
    {
        if name == "page.back_btn"
        {
            Self::show_list_page(ui);
            return true;
        }
        if name == "page.save_btn"
        {
            if let Some(status) = ui.base.find_element("page.status")
            {
                status.borrow_mut().set_text("Saved in memory");
            }
            return true;
        }
        if let Some(index) = indexed_action(name, "page", "open_btn")
            && let Some(item) = REPRO_ITEMS.get(index)
        {
            Self::show_edit_page(ui, item);
            return true;
        }
        if let Some(index) = indexed_action(name, "page.fields", "field_btn")
        {
            let input_id = format!("page.fields[{index}].field_input");
            let value = ui
                .base
                .find_element(&input_id)
                .map_or_else(String::new, |input| input.borrow().value().to_string());
            if let Some(status) = ui.base.find_element("page.status")
            {
                status
                    .borrow_mut()
                    .set_text(format!("Action {:02}: {value}", index + 1));
            }
            return true;
        }
        false
    }
}

fn field_value(item: &ReproItem, index: usize) -> String
{
    if index == 0
    {
        return format!("{} primary field", item.name);
    }
    if index == 1
    {
        return item.body.lines().next().unwrap_or_default().to_string();
    }
    format!("{} detail field {:02}", item.name, index + 1)
}

fn indexed_action(name: &str, prefix: &str, action: &str) -> Option<usize>
{
    let suffix = format!("].{action}");
    name.strip_prefix(prefix)?
        .strip_prefix('[')?
        .strip_suffix(&suffix)?
        .parse()
        .ok()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = StaleScrollviewFocusUI::new();
        StaleScrollviewFocusApp::show_list_page(&mut ui);
        let mut app = StaleScrollviewFocusApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

mod contacts_manager_ui;
#[path = "contact_form/contact_form_ui.rs"]
mod contact_form_ui;

use contact_form_ui::ContactFormUI;
use contacts_manager_ui::*;
use std::cell::RefCell;
use std::fs;
use std::path::PathBuf;
use std::rc::Rc;

#[derive(Clone, Debug, Default)]
struct Contact
{
    id: String,
    first_name: String,
    last_name: String,
    email: String,
    phone: String,
    company: String,
    city: String,
    category: String,
    active: bool,
    notes: String,
}

#[derive(Default)]
struct ContactsManagerApp
{
    contacts: Vec<Contact>,
    dirty: bool,
    edit_contact_id: String,
    delete_contact_id: String,
    contact_form_identity: Option<u64>,
    contact_form_result: Option<Rc<RefCell<Option<String>>>>,
    delete_dialog: Option<uimd::MessageBoxYesNo>,
}

struct ContactFormBehavior
{
    result: Rc<RefCell<Option<String>>>,
}

impl uimd::GeneratedWindowBehavior for ContactFormBehavior
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
        if name != "save_btn"
        {
            return false;
        }
        let required = ["first_name", "last_name", "email"]
            .into_iter()
            .all(|field| !element_value(window, field).trim().is_empty());
        if !required
        {
            set_value(window, "message", "Required fields are missing.");
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

impl ContactsManagerApp
{
    fn initialize(&mut self, ui: &mut ContactsManagerUI)
    {
        self.reload(ui);
    }

    fn reload(&mut self, ui: &mut ContactsManagerUI)
    {
        self.contacts = load_contacts();
        self.dirty = false;
        self.refresh_list(ui, "");
        self.set_status(ui, &format!("Loaded {} contacts.", self.contacts.len()));
    }

    fn save(&mut self, ui: &mut ContactsManagerUI)
    {
        save_contacts(&self.contacts);
        self.dirty = false;
        self.set_status(ui, &format!("Saved {} contacts.", self.contacts.len()));
    }

    fn open_form(&mut self, ui: &mut ContactsManagerUI, contact: Option<Contact>)
    {
        let form = ContactFormUI::new();
        if let Some(contact) = contact
        {
            self.edit_contact_id.clone_from(&contact.id);
            set_value(&form.base, "first_name", &contact.first_name);
            set_value(&form.base, "last_name", &contact.last_name);
            set_value(&form.base, "email", &contact.email);
            set_value(&form.base, "phone", &contact.phone);
            set_value(&form.base, "company", &contact.company);
            set_value(&form.base, "city", &contact.city);
            if let Some(category) = form.base.find_element("category")
            {
                let index = category
                    .borrow()
                    .options()
                    .iter()
                    .position(|value| value == &contact.category)
                    .unwrap_or_default() as i32;
                category.borrow_mut().set_selected_index(index);
            }
            if let Some(active) = form.base.find_element("active")
            {
                active.borrow_mut().set_checked(contact.active);
            }
            set_value(&form.base, "notes", &contact.notes);
        }
        else
        {
            self.edit_contact_id.clear();
        }
        let mut window = form.base;
        let result = Rc::new(RefCell::new(None));
        self.contact_form_identity = Some(window.identity());
        self.contact_form_result = Some(result.clone());
        window.set_behavior(ContactFormBehavior { result });
        ui.base.open_window(window);
    }

    fn save_form(&mut self, ui: &mut ContactsManagerUI, form: &uimd::GeneratedWindow)
    {
        let first_name = element_value(form, "first_name");
        let last_name = element_value(form, "last_name");
        let email = element_value(form, "email");
        let category = form
            .find_element("category")
            .map(|value| value.borrow().selected_value())
            .unwrap_or_default();
        let active = form
            .find_element("active")
            .is_some_and(|value| value.borrow().checked());
        let mut contact = Contact
        {
            id: self.edit_contact_id.clone(),
            first_name,
            last_name,
            email,
            phone: element_value(form, "phone"),
            company: element_value(form, "company"),
            city: element_value(form, "city"),
            category,
            active,
            notes: element_value(form, "notes"),
        };
        if contact.id.is_empty()
        {
            contact.id = self.next_id().to_string();
            let selected_id = contact.id.clone();
            self.contacts.push(contact);
            self.set_status(ui, "Contact added.");
            self.dirty = true;
            self.refresh_list(ui, &selected_id);
        }
        else if let Some(index) = self.contacts.iter().position(|value| value.id == contact.id)
        {
            let selected_id = contact.id.clone();
            self.contacts[index] = contact;
            self.set_status(ui, "Contact updated.");
            self.dirty = true;
            self.refresh_list(ui, &selected_id);
        }
        self.edit_contact_id.clear();
    }

    fn refresh_list(&self, ui: &mut ContactsManagerUI, selected_id: &str)
    {
        let labels: Vec<String> = self.contacts.iter().map(row_label).collect();
        ui.header
            .borrow_mut()
            .set_text(format!("Contacts Manager - {} records", labels.len()));
        ui.contacts.borrow_mut().set_options(labels);
        let selected_index = if self.contacts.is_empty()
        {
            -1
        }
        else
        {
            self.contacts
                .iter()
                .position(|contact| contact.id == selected_id)
                .unwrap_or_default() as i32
        };
        ui.contacts.borrow_mut().set_selected_index(selected_index);
        self.refresh_detail(ui);
    }

    fn refresh_detail(&self, ui: &mut ContactsManagerUI)
    {
        let text = self
            .selected_contact(ui)
            .map(detail_text)
            .unwrap_or_else(|| "No contact selected.".to_string());
        ui.detail.borrow_mut().set_text(text);
    }

    fn selected_contact(&self, ui: &ContactsManagerUI) -> Option<&Contact>
    {
        let index = ui.contacts.borrow().selected_index();
        (index >= 0)
            .then_some(index as usize)
            .and_then(|index| self.contacts.get(index))
    }

    fn next_id(&self) -> i32
    {
        self.contacts
            .iter()
            .filter_map(|contact| contact.id.parse::<i32>().ok())
            .max()
            .unwrap_or_default()
            + 1
    }

    fn set_status(&self, ui: &mut ContactsManagerUI, value: &str)
    {
        ui.status
            .borrow_mut()
            .set_text(format!("{value}{}", if self.dirty { " *" } else { "" }));
    }
}

impl ContactsManagerUIEvents for ContactsManagerApp
{
    fn on_contacts_selectionchange(&mut self, ui: &mut ContactsManagerUI, _value: &[String])
    {
        self.refresh_detail(ui);
    }

    fn on_add_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        self.open_form(ui, None);
    }

    fn on_edit_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        if let Some(contact) = self.selected_contact(ui).cloned()
        {
            self.open_form(ui, Some(contact));
        }
        else
        {
            self.set_status(ui, "Select a contact to edit.");
        }
    }

    fn on_delete_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        if let Some(contact) = self.selected_contact(ui).cloned()
        {
            self.delete_contact_id.clone_from(&contact.id);
            let mut dialog = uimd::MessageBoxYesNo::new(
                "Delete Contact",
                &format!("Delete {}?", row_label(&contact)),
            );
            ui.base.open_window(dialog.take_window());
            self.delete_dialog = Some(dialog);
        }
        else
        {
            self.set_status(ui, "Select a contact to delete.");
        }
    }

    fn on_save_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        self.save(ui);
    }

    fn on_reload_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        self.reload(ui);
    }

    fn on_close_btn_click(&mut self, ui: &mut ContactsManagerUI)
    {
        ui.base.request_close();
    }

    fn on_window_closed(
        &mut self,
        ui: &mut ContactsManagerUI,
        window: uimd::GeneratedWindow,
    )
    {
        if self.contact_form_identity == Some(window.identity())
        {
            let result = self
                .contact_form_result
                .take()
                .and_then(|result| result.borrow().clone());
            self.contact_form_identity = None;
            if result.as_deref() == Some("save")
            {
                self.save_form(ui, &window);
            }
            else
            {
                self.edit_contact_id.clear();
                self.set_status(ui, "Edit cancelled.");
                self.refresh_detail(ui);
            }
            return;
        }
        if !self
            .delete_dialog
            .as_ref()
            .is_some_and(|dialog| dialog.owns_window(&window))
        {
            return;
        }
        let Some(dialog) = self.delete_dialog.take() else { return };
        if dialog.result() == Some(uimd::MessageBoxResult::Yes)
        {
            let previous_len = self.contacts.len();
            self.contacts
                .retain(|contact| contact.id != self.delete_contact_id);
            self.delete_contact_id.clear();
            if self.contacts.len() != previous_len
            {
                self.dirty = true;
                self.refresh_list(ui, "");
                self.set_status(ui, "Contact deleted.");
            }
        }
        else
        {
            self.set_status(ui, "Delete cancelled.");
        }
    }
}

fn set_value(window: &uimd::GeneratedWindow, name: &str, value: impl Into<String>)
{
    if let Some(element) = window.find_element(name)
    {
        element.borrow_mut().set_value(value);
    }
}

fn element_value(window: &uimd::GeneratedWindow, name: &str) -> String
{
    window
        .find_element(name)
        .map(|element| element.borrow().value().to_string())
        .unwrap_or_default()
}

fn row_label(contact: &Contact) -> String
{
    format!("{} {}", contact.first_name, contact.last_name)
        .trim()
        .to_string()
}

fn detail_text(contact: &Contact) -> String
{
    format!(
        "{} {}\nEmail: {}\nPhone: {}\nCompany: {}\nCity: {}\nCategory: {} ({})\n\n{}",
        contact.first_name,
        contact.last_name,
        contact.email,
        contact.phone,
        contact.company,
        contact.city,
        contact.category,
        if contact.active { "active" } else { "inactive" },
        contact.notes,
    )
}

fn data_path() -> PathBuf
{
    std::env::var_os("UI_CONTACTS_MANAGER_DATA_PATH")
        .map(PathBuf::from)
        .unwrap_or_else(||
        {
            PathBuf::from(env!("CARGO_MANIFEST_DIR"))
                .join("data")
                .join("contacts.csv")
        })
}

fn load_contacts() -> Vec<Contact>
{
    let Ok(content) = fs::read_to_string(data_path()) else { return Vec::new() };
    content
        .lines()
        .skip(1)
        .filter_map(|line|
        {
            let cells: Vec<_> = line.split(',').collect();
            (cells.len() >= 10).then(||
            {
                Contact
                {
                    id: cells[0].to_string(),
                    first_name: cells[1].to_string(),
                    last_name: cells[2].to_string(),
                    email: cells[3].to_string(),
                    phone: cells[4].to_string(),
                    company: cells[5].to_string(),
                    city: cells[6].to_string(),
                    category: cells[7].to_string(),
                    active: matches!(cells[8], "1" | "true"),
                    notes: cells[9].to_string(),
                }
            })
        })
        .collect()
}

fn save_contacts(contacts: &[Contact])
{
    let path = data_path();
    if let Some(parent) = path.parent()
    {
        let _ = fs::create_dir_all(parent);
    }
    let mut output =
        "id,first_name,last_name,email,phone,company,city,category,active,notes\n".to_string();
    for contact in contacts
    {
        output.push_str(&format!(
            "{},{},{},{},{},{},{},{},{},{}\n",
            contact.id,
            contact.first_name,
            contact.last_name,
            contact.email,
            contact.phone,
            contact.company,
            contact.city,
            contact.category,
            if contact.active { "true" } else { "false" },
            contact.notes,
        ));
    }
    let _ = fs::write(path, output);
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ContactsManagerUI::new();
        let mut app = ContactsManagerApp::default();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

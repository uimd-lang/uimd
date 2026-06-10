"""CSV-backed contacts manager example."""

import csv
import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_here)))
sys.path.insert(0, os.path.join(_here, "contact_form"))

from runtime import UIApplication
from dialogs import MessageBoxYesNo
from contacts_manager_ui import ContactsManagerUI
from contact_form import ContactForm


DATA_PATH_ENV = "UI_CONTACTS_MANAGER_DATA_PATH"
DATA_PATH = os.environ.get(DATA_PATH_ENV, os.path.join(_here, "data", "contacts.csv"))
CSV_COLUMNS = [
    "id",
    "first_name",
    "last_name",
    "email",
    "phone",
    "company",
    "city",
    "category",
    "active",
    "notes",
]


class ContactsManager(ContactsManagerUI):
    def open(self):
        super().open()
        self._contacts = []
        self._dirty = False
        self._load_contacts()

    def on_contacts_selection_change(self, value):
        self._refresh_detail()

    def on_add_btn_click(self):
        self._open_form(None)

    def on_edit_btn_click(self):
        contact = self._selected_contact()
        if contact:
            self._open_form(contact)
        else:
            self._set_status("Select a contact to edit.")

    def on_delete_btn_click(self):
        self._confirm_delete_selected()

    def on_save_btn_click(self):
        self._save_contacts()

    def on_reload_btn_click(self):
        self._load_contacts()

    def on_close_btn_click(self):
        self._quit()

    def _load_contacts(self):
        with open(DATA_PATH, "r", encoding="utf-8", newline="") as handle:
            self._contacts = [dict(row) for row in csv.DictReader(handle)]
        self._dirty = False
        self._refresh_list()
        self._set_status(f"Loaded {len(self._contacts)} contacts.")

    def _save_contacts(self):
        os.makedirs(os.path.dirname(DATA_PATH), exist_ok=True)
        with open(DATA_PATH, "w", encoding="utf-8", newline="") as handle:
            writer = csv.DictWriter(handle, fieldnames=CSV_COLUMNS, lineterminator="\n")
            writer.writeheader()
            for contact in self._contacts:
                writer.writerow({key: contact.get(key, "") for key in CSV_COLUMNS})
        self._dirty = False
        self._set_status(f"Saved {len(self._contacts)} contacts.")

    def _refresh_list(self):
        rows = [self._row_label(contact) for contact in self._contacts]
        self._row_ids = {
            self._row_label(contact): contact.get("id", "")
            for contact in self._contacts
        }
        selected_id = self._selected_id()
        self.contacts._options = rows
        if rows:
            selected_row = self._row_for_id(selected_id) if selected_id else rows[0]
            self.contacts.selected_items = [selected_row if selected_row in rows else rows[0]]
        else:
            self.contacts.selected_items = []
        self.header.text = f"Contacts Manager - {len(rows)} records"
        self._refresh_detail()

    def _refresh_detail(self):
        contact = self._selected_contact()
        if not contact:
            self.detail.text = "No contact selected."
            return
        active = "active" if _to_bool(contact.get("active")) else "inactive"
        self.detail.text = "\n".join([
            f"{contact.get('first_name', '')} {contact.get('last_name', '')}",
            f"Email: {contact.get('email', '')}",
            f"Phone: {contact.get('phone', '')}",
            f"Company: {contact.get('company', '')}",
            f"City: {contact.get('city', '')}",
            f"Category: {contact.get('category', '')} ({active})",
            "",
            contact.get("notes", ""),
        ])

    def _open_form(self, contact):
        form = ContactForm(contact=contact, on_close=self._on_form_closed)
        form._app = self._app
        self._app.open(form)

    def _on_form_closed(self, result):
        if result is None:
            self._set_status("Edit cancelled.")
            return
        if not result.get("id"):
            result["id"] = self._next_id()
            self._contacts.append(result)
            self._set_status("Contact added.")
        else:
            for index, contact in enumerate(self._contacts):
                if contact.get("id") == result.get("id"):
                    self._contacts[index] = result
                    self._set_status("Contact updated.")
                    break
        self._dirty = True
        self._refresh_list()
        row = self._row_for_id(result.get("id"))
        if row:
            self.contacts.selected_items = [row]
            self._refresh_detail()

    def _confirm_delete_selected(self):
        contact = self._selected_contact()
        if not contact:
            self._set_status("Select a contact to delete.")
            return
        contact_id = contact.get("id")
        display_name = self._row_label(contact) or "selected contact"
        message = MessageBoxYesNo(
            header="Delete Contact",
            message=f"Delete {display_name}?",
            on_close=lambda result, selected_id=contact_id: self._delete_confirmed(selected_id, result),
        )
        message._app = self._app
        self._app.open(message)

    def _delete_confirmed(self, contact_id, result):
        if result != "yes":
            self._set_status("Delete cancelled.")
            return
        before_count = len(self._contacts)
        self._contacts = [item for item in self._contacts if item.get("id") != contact_id]
        if len(self._contacts) == before_count:
            self._set_status("Contact no longer exists.")
            return
        self._dirty = True
        self._refresh_list()
        self._set_status("Contact deleted.")

    def _selected_contact(self):
        selected_id = self._selected_id()
        for contact in self._contacts:
            if contact.get("id") == selected_id:
                return contact
        return None

    def _selected_id(self):
        row = self.contacts.selected_item
        if not row:
            return ""
        return getattr(self, "_row_ids", {}).get(row, "")

    def _row_for_id(self, contact_id):
        for contact in self._contacts:
            if contact.get("id") == contact_id:
                return self._row_label(contact)
        return ""

    @staticmethod
    def _row_label(contact):
        return f"{contact.get('first_name', '')} {contact.get('last_name', '')}".strip()

    def _next_id(self):
        ids = []
        for contact in self._contacts:
            try:
                ids.append(int(contact.get("id", "0")))
            except ValueError:
                continue
        return str(max(ids, default=0) + 1)

    def _set_status(self, message):
        marker = " *" if self._dirty else ""
        self.status.text = f"{message}{marker}"

    def _quit(self):
        if self._app is not None:
            self._app._running = False


def _to_bool(value):
    return str(value).strip().lower() in ("1", "true", "yes", "y")


def main():
    app = UIApplication()
    window = ContactsManager()
    app.open(window)
    return app.run()


if __name__ == "__main__":
    sys.exit(main())

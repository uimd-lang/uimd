"""Add/edit contact dialog."""

import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(_here))))

from contact_form_ui import ContactFormUI


CATEGORY_DEFAULT = "Client"


class ContactForm(ContactFormUI):
    def __init__(self, contact=None, on_close=None):
        super().__init__()
        self._contact = dict(contact or {})
        self._on_close = on_close
        self.result = None

    def open(self):
        super().open()
        self._load_contact(self._contact)

    def on_save_btn_click(self):
        self._submit()

    def on_cancel_btn_click(self):
        self._cancel()

    def _load_contact(self, contact):
        self.first_name.value = contact.get("first_name", "")
        self.last_name.value = contact.get("last_name", "")
        self.email.value = contact.get("email", "")
        self.phone.value = contact.get("phone", "")
        self.company.value = contact.get("company", "")
        self.city.value = contact.get("city", "")
        self.category.selected_item = contact.get("category") or CATEGORY_DEFAULT
        self.active.value = _to_bool(contact.get("active", "true"))
        self.notes.value = contact.get("notes", "")

    def _submit(self):
        error = self._validation_error()
        if error:
            self.message.text = error
            return
        updated = dict(self._contact)
        updated.update({
            "first_name": self.first_name.value.strip(),
            "last_name": self.last_name.value.strip(),
            "email": self.email.value.strip(),
            "phone": self.phone.value.strip(),
            "company": self.company.value.strip(),
            "city": self.city.value.strip(),
            "category": self.category.selected_item or CATEGORY_DEFAULT,
            "active": "true" if self.active.value else "false",
            "notes": self.notes.value.strip(),
        })
        self.result = updated
        self._close(updated)

    def _cancel(self):
        self.result = None
        self._close(None)

    def _close(self, result):
        if self._on_close:
            self._on_close(result)
        if self._app is not None:
            self._app.close_current()

    def _validation_error(self):
        if not self.first_name.value.strip():
            return "First name is required."
        if not self.last_name.value.strip():
            return "Last name is required."
        email = self.email.value.strip()
        if not email:
            return "Email is required."
        at = email.find("@")
        if at <= 0 or "." not in email[at + 1:]:
            return "Email must look like name@example.com."
        return ""


def _to_bool(value):
    return str(value).strip().lower() in ("1", "true", "yes", "y")

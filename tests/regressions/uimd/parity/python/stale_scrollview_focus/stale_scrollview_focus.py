"""Manual regression app for replacing a focused ScrollView page."""

import os
import sys
from dataclasses import dataclass


_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.abspath(os.path.join(_HERE, "..", "..", "..", "..", "..", ".."))
sys.path.insert(0, os.path.join(_ROOT, "src"))
sys.path.insert(0, _HERE)

from edit_field_row_ui import EditFieldRowUI
from edit_fields_scroll_ui import EditFieldsScrollUI
from edit_page_ui import EditPageUI
from item_row_ui import ItemRowUI
from list_page_ui import ListPageUI
from stale_scrollview_focus_ui import StaleScrollviewFocusUI
from uimd.runtime import UIApplication


MINIMUM_RENDER_HEIGHT = 1
FIELD_COUNT = 40
PRIMARY_FIELD_INDEX = 0
BODY_FIELD_INDEX = 1
DISPLAY_INDEX_OFFSET = 1
FIELD_NUMBER_WIDTH = 2


@dataclass(frozen=True)
class ReproItem:
    name: str
    summary: str
    body: str


ITEMS = (
    ReproItem(
        "Alpha Apartments",
        "First row, visible without scrolling",
        "Alpha item was opened from the top of the ScrollView.\n"
        "The edit page should not keep any stale list focus background.",
    ),
    ReproItem(
        "Bravo Offices",
        "Second row, used by the automated repro",
        "Bravo item was opened by pressing Enter on a row button.\n"
        "The old ScrollView page must be gone before this page renders.",
    ),
    ReproItem(
        "Charlie Lofts",
        "Third row, keeps the source list taller",
        "Charlie item keeps the source page closer to the reported list shape.\n"
        "The opened edit page still owns the new nested ScrollView.",
    ),
    ReproItem(
        "Delta Studios",
        "Fourth row, manual repro filler",
        "Delta item is extra manual test data for scroll and focus traversal.\n"
        "Activating Open should replace the whole page cleanly.",
    ),
    ReproItem(
        "Echo Retail",
        "Fifth row, enough rows to browse",
        "Echo item makes the source ScrollView visibly list-like.\n"
        "The edit form below contains many alternating controls.",
    ),
)


def field_number(index):
    return str(index + DISPLAY_INDEX_OFFSET).zfill(FIELD_NUMBER_WIDTH)


def field_value(item, index):
    if index == PRIMARY_FIELD_INDEX:
        return f"{item.name} primary field"
    if index == BODY_FIELD_INDEX:
        return item.body.splitlines()[PRIMARY_FIELD_INDEX]
    return f"{item.name} detail field {field_number(index)}"


def render_expand_width_child(child, width):
    _natural_width, natural_height = child.get_natural_size()
    child.resize(width, max(MINIMUM_RENDER_HEIGHT, natural_height))
    return child.render()


class ItemRow(ItemRowUI):
    """One interactive row inside the regression ScrollView."""

    def __init__(self, item, on_open):
        super().__init__()
        self._item = item
        self._on_open = on_open

    def open(self):
        super().open()
        self.name.text = self._item.name
        self.summary.text = self._item.summary

    def on_open_btn_click(self):
        self._on_open(self._item)


class ListPage(ListPageUI):
    """ScrollView page whose row button replaces the ViewHost page."""

    def __init__(self, items, on_open):
        super().__init__()
        self._items = items
        self._on_open = on_open

    def open(self):
        super().open()
        self.populate()

    def populate(self):
        self.clear_children()
        for item in self._items:
            row = ItemRow(item, self._on_open)
            row.parent = self
            row._app = self._app
            row.mode = "expand_width"
            row.open()
            self.add_child(row)
        self.scroll_to_top()

    def _render_child(self, child, width):
        return render_expand_width_child(child, width)


class EditFieldRow(EditFieldRowUI):
    """One text input followed by an action button in the edit ScrollView."""

    def __init__(self, index, value, on_action):
        super().__init__()
        self._index = index
        self._value = value
        self._on_action = on_action

    def open(self):
        super().open()
        number = field_number(self._index)
        self.field_input.value = self._value
        self.field_btn.title = f"Action {number}"

    def on_field_btn_click(self):
        self._on_action(self._index, self.field_input.value)


class EditFieldsScroll(EditFieldsScrollUI):
    """Large edit form body with many alternating text inputs and buttons."""

    def __init__(self, item, on_action):
        super().__init__()
        self._item = item
        self._on_action = on_action

    def open(self):
        super().open()
        self.populate()

    def populate(self):
        self.clear_children()
        for index in range(FIELD_COUNT):
            row = EditFieldRow(index, field_value(self._item, index), self._on_action)
            row.parent = self
            row._app = self._app
            row.mode = "expand_width"
            row.open()
            self.add_child(row)
        self.scroll_to_top()

    def _render_child(self, child, width):
        return render_expand_width_child(child, width)


class EditPage(EditPageUI):
    """Edit page displayed after the ScrollView row button fires."""

    def __init__(self, item, on_back):
        super().__init__()
        self._item = item
        self._on_back = on_back

    def open(self):
        super().open()
        self.edit_title.text = f"Edit {self._item.name}"
        self.name_input.value = self._item.name
        self.fields.set_view(EditFieldsScroll(self._item, self._field_action))

    def on_back_btn_click(self):
        self._on_back()

    def on_save_btn_click(self):
        self.status.text = "Saved in memory"

    def _field_action(self, index, value):
        self.status.text = f"Action {field_number(index)}: {value}"


class StaleScrollviewFocus(StaleScrollviewFocusUI):
    """Root app that swaps a ViewHost from list page to edit page."""

    def open(self):
        super().open()
        self.show_list_page()

    def show_list_page(self):
        self.page.set_view(ListPage(ITEMS, self.show_edit_page))
        self.hint.text = "Focus list, press Enter, choose Open."

    def show_edit_page(self, item):
        self.page.set_view(EditPage(item, self.show_list_page))
        self.hint.text = "Edit page opened from ScrollView row."

    def on_tab_list_click(self):
        self.show_list_page()

    def on_tab_other_click(self):
        self.hint.text = "Other tab intentionally leaves the list unchanged."

    def on_quit_btn_click(self):
        if self._app is not None:
            self._app._running = False


def main():
    app = UIApplication()
    app.open(StaleScrollviewFocus())
    return app.run()


if __name__ == "__main__":
    sys.exit(main())

from abc import ABC, abstractmethod


class UIInstance(ABC):
    """Base class for all UI instances (windows and elements)."""

    def __init__(self):
        self._enabled = True
        self._parent = None

    @property
    def enabled(self):
        return self._enabled

    @enabled.setter
    def enabled(self, value):
        self._enabled = bool(value)

    @property
    def parent(self):
        return self._parent

    @parent.setter
    def parent(self, value):
        self._parent = value

    # Virtual methods that can be overridden
    def open(self):
        pass

    def save(self):
        pass

    def close(self):
        pass

    def focus(self):
        pass

    def blur(self):
        pass

    def resize(self, width, height):
        pass

    def confirmed(self, element, value):
        pass

    def listbox_item_activate(self, element_id, index, value):
        """Handle a ListBox activation request before its default Enter action."""
        return False

    def elementchanged(self, element, value):
        pass

    def selectionchanged(self, element, value):
        pass

    def _dispatch_confirmed(self, element, value):
        self.confirmed(element, value)

    def _dispatch_listbox_item_activate(self, element, element_id, index, value):
        return bool(self.listbox_item_activate(element_id, index, value))

    def _dispatch_element_changed(self, element, value):
        self.elementchanged(element, value)

    def _dispatch_selection_changed(self, element, value):
        self.selectionchanged(element, value)

    def focused(self, element, value):
        pass

    def enabled(self, element, value):
        pass

    def on_preview_key(self, event):
        """Inspect a logical key before the focused element handles it."""
        return False

    def on_key(self, key):
        """Handle a logical key left unhandled by the focused element."""
        return False

    def render(self):
        return []

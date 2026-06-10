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

    def elementchanged(self, element, value):
        pass

    def selectionchanged(self, element, value):
        pass

    def _dispatch_confirmed(self, element, value):
        self.confirmed(element, value)

    def _dispatch_element_changed(self, element, value):
        self.elementchanged(element, value)

    def _dispatch_selection_changed(self, element, value):
        self.selectionchanged(element, value)

    def focused(self, element, value):
        pass

    def enabled(self, element, value):
        pass

    def render(self):
        return []

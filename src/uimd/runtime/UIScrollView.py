from uimd.runtime.UIBase import USER_SELECT_TEXT
from uimd.runtime.UIControl import UIControl
from uimd.runtime.rendering import TerminalCell, cells_to_ansi_lines
from uimd.runtime.style import Color, Style


COPYABLE_LABEL_TYPES = {"label", "spanlabel"}
MOUSE_WHEEL_DELTA_UP = 1
MOUSE_WHEEL_DELTA_DOWN = -1


class UIScrollView(UIControl):
    """Scrollable container that renders children in order within a viewport area.

    Use this when you need a child control to render multiple sub-children
    (messages, list items, etc.) inside a bounded cell rectangle. New views
    start at the top; call ``scroll_to_bottom()`` or enable auto-scroll for
    feeds where the newest/last child should stay visible at the bottom edge.

    Subclass this and override ``_render_child(child, width)`` to customise
    how each child is rendered.  The default delegates to ``child.render()``.
    """

    SCROLL_INDICATOR_ABOVE = "^"
    SCROLL_INDICATOR_BELOW = "v"

    LINES_PER_SCROLL = 4
    MAX_WHEEL_SCROLL_VIEWPORT_FRACTION = 3

    def __init__(self, title="", layout=None, members=None, style=None):
        super().__init__(title=title, layout=layout, members=members, style=style)
        self.focus_style = None
        self.edit_style = None
        self.selected_style = None
        self._apply_container_state_styles("uiscrollview")
        self._children = []  # list of child controls
        self._scroll_offset = 0
        self._view_offset = 0  # line-level offset from natural bottom (>0 = scrolled up)
        self._h_offset = 0    # horizontal scroll offset (columns from left)
        self._last_natural_skip = 0  # updated on each render; used to clamp scroll-up
        self._auto_scroll = False
        self._autoscroll_direction = None  # "up" / "down" while drag is held outside viewport
        self._autoscroll_col = 0
        self._cached_child_heights = None  # Cache for child heights (None = not yet cached)
        self._pending_terminal_scroll_delta = 0
        self._selected_focus_child = None
        self._pending_proxy_focus_restore = False

    def _apply_container_state_styles(self, selector_name):
        props = getattr(self, "_compiled_styles", {}).get(selector_name, {})
        for key, value in props.items():
            self._apply_container_state_style_property(key, value)

        for key in ("focus-background", "focus-color", "edit-background", "edit-color",
                    "selected-background", "selected-color"):
            value = getattr(self.style, key.replace("-", "_"), None)
            if value is not None:
                self._apply_container_state_style_property(key, value)

    def _apply_container_state_style_property(self, key, value):
        if key.startswith("focus-"):
            if self.focus_style is None:
                self.focus_style = Style()
            self.focus_style.set(key[len("focus-"):], value)
        elif key.startswith("edit-"):
            if self.edit_style is None:
                self.edit_style = Style()
            self.edit_style.set(key[len("edit-"):], value)
        elif key.startswith("selected-"):
            if self.selected_style is None:
                self.selected_style = Style()
            self.selected_style.set(key[len("selected-"):], value)

    def open(self):
        """Open the scroll view and attach current children to the app."""
        super().open()
        for child in self._children:
            child.parent = self
            child._app = self._app

    def add_child(self, child):
        """Add a child control to the scroll view."""
        child_index = len(self._children)
        self._children.append(child)
        child.parent = self
        child._app = self._app
        self._restore_pending_child_focus(child, child_index)
        self._cached_child_heights = None  # Invalidate height cache
        if self._auto_scroll:
            self._view_offset = 0
            self.scroll_to_bottom()
        self.invalidate_layout()

    def clear_children(self):
        """Remove all child controls and clear focus that points into them."""
        self._clear_removed_child_focus()
        self._children.clear()
        self._cached_child_heights = None
        self._scroll_offset = 0
        self._view_offset = 0
        self.invalidate_layout()

    def _clear_removed_child_focus(self):
        proxy = getattr(self, "parent", None)
        focus_restore = self._focused_child_restore()
        if focus_restore is not None:
            self._pending_focus_restore = focus_restore
        self._clear_selected_focus_child()
        self._focused = False

        owner = proxy
        while owner is not None:
            focused = getattr(owner, "_focused_element", None)
            last_descendant = getattr(owner, "_scrollview_last_descendant", None)
            removed_last_descendant = False
            if isinstance(last_descendant, dict):
                previous = last_descendant.get(id(self))
                removed_last_descendant = (
                    previous is not None
                    and any(self._child_contains_element(child, previous) for child in self._children)
                )
            removed_descendant_focus = any(
                self._child_has_focused_descendant(child)
                for child in self._children
            )
            removed_focus = (
                removed_descendant_focus
                or removed_last_descendant
                or bool(getattr(proxy, "focused", False))
                or (
                    focused is not None
                    and focused is not proxy
                    and any(self._child_contains_element(child, focused) for child in self._children)
                )
            )

            scope = getattr(owner, "_active_scrollview_scope", None)
            scope_belongs_to_scrollview = scope is not None and scope.get("scrollview") is self
            owner_focuses_proxy = focused is proxy
            focused_removed_element = (
                focused is proxy
                or (
                    focused is not None
                    and focused is not proxy
                    and any(self._child_contains_element(child, focused) for child in self._children)
                )
            )
            focused_external_to_owner = (
                focused is not None
                and not self._view_contains_element(owner, focused)
            )
            should_restore_scrollview_focus = scope_belongs_to_scrollview or (removed_focus and owner_focuses_proxy)
            should_restore_proxy_focus = (
                (
                    (
                        removed_last_descendant
                        and (focused is None or focused_external_to_owner or focused_removed_element)
                    )
                    or (
                        removed_descendant_focus
                        and not focused_removed_element
                        and (focused is None or focused_external_to_owner)
                    )
                )
                and not scope_belongs_to_scrollview
                and proxy is not None
                and owner is not proxy
                and hasattr(owner, "_set_element_focus_state")
            )

            if should_restore_scrollview_focus:
                if proxy is not None and hasattr(owner, "_scrollview_proxy_rect"):
                    owner._active_scrollview_scope = {
                        "proxy": proxy,
                        "scrollview": self,
                        "scrollview_rect": owner._scrollview_proxy_rect(proxy),
                    }
                    owner._edit_mode = True
                    owner._edit_snapshot = None
                    if hasattr(owner, "_set_element_focus_state"):
                        owner._focused_element = proxy
                        owner._set_element_focus_state(proxy, True)
                else:
                    owner._active_scrollview_scope = None
                    if getattr(owner, "_edit_mode", False):
                        owner._edit_mode = False
                        owner._edit_snapshot = None

            if removed_focus:
                if hasattr(owner, "_clear_descendant_focus_state"):
                    owner._clear_descendant_focus_state(self)
                if focused is not None and hasattr(focused, "focused") and focused_removed_element:
                    focused.focused = False
                if should_restore_scrollview_focus and proxy is not None and hasattr(owner, "_set_element_focus_state"):
                    owner._focused_element = proxy
                    owner._set_element_focus_state(proxy, True)
                elif should_restore_proxy_focus:
                    self._pending_proxy_focus_restore = True
                    owner._active_scrollview_scope = None
                    if hasattr(owner, "_edit_mode"):
                        owner._edit_mode = False
                    if hasattr(owner, "_edit_snapshot"):
                        owner._edit_snapshot = None
                    owner._focused_element = proxy
                    owner._set_element_focus_state(proxy, True)
                    self._focused = True
                elif focused_removed_element:
                    owner._focused_element = None
                if not should_restore_scrollview_focus and proxy is not None and hasattr(proxy, "focused"):
                    proxy.focused = should_restore_proxy_focus

            owner = getattr(owner, "parent", None)

    def _focused_child_restore(self):
        for index, child in enumerate(self._children):
            focused = getattr(child, "_focused_element", None)
            if focused is not None and self._child_contains_element(child, focused):
                name = getattr(focused, "name", None)
                if name:
                    return {"index": index, "name": name}
            for element in getattr(child, "_elements", {}).values():
                if getattr(element, "focused", False):
                    name = getattr(element, "name", None)
                    if name:
                        return {"index": index, "name": name}
        return None

    def _restore_pending_child_focus(self, child, child_index):
        pending = getattr(self, "_pending_focus_restore", None)
        if pending is None:
            return
        target_index = pending.get("index")
        if child_index < target_index:
            return
        self._pending_focus_restore = None
        if child_index != target_index:
            return
        target_name = pending.get("name")
        if target_name is None:
            return
        target = getattr(child, "_elements", {}).get(target_name)
        if target is None:
            return
        target.focused = True
        if hasattr(child, "_focused_element"):
            child._focused_element = target
        if hasattr(self, "_focused_element"):
            self._focused_element = target
        self._focused = True
        self._mark_selected_focus_child(child)
        proxy = getattr(self, "parent", None)
        owner = proxy
        while owner is not None:
            if hasattr(owner, "_focused_element"):
                owner._focused_element = target
            last_descendant = getattr(owner, "_scrollview_last_descendant", None)
            if isinstance(last_descendant, dict):
                last_descendant[id(self)] = target
            owner = getattr(owner, "parent", None)

    def _mark_selected_focus_child(self, child):
        self._selected_focus_child = child

    def _clear_selected_focus_child(self):
        self._selected_focus_child = None

    def _clear_pending_proxy_focus_restore(self):
        self._pending_proxy_focus_restore = False

    def _should_select_focused_child(self, child):
        if self._child_has_multiple_focusable_descendants(child):
            return False
        if getattr(self, "_selected_focus_child", None) is child:
            return True
        return getattr(self, "_focused", False) and self._child_has_focused_descendant(child)

    @staticmethod
    def _child_has_multiple_focusable_descendants(child):
        focusable = getattr(child, "_local_focusable_elements", None)
        if callable(focusable):
            try:
                if len(focusable()) > 1:
                    return True
            except Exception:
                return False
        return False

    # ------------------------------------------------------------------
    # Public API – scroll control
    # ------------------------------------------------------------------

    def scroll_to(self, index):
        """Scroll to the given child *index* (0-based).

        Clamps index so that at least one child is visible.
        """
        total = len(self._children)
        if total == 0:
            self._scroll_offset = 0
            self.invalidate()
            return

        previous_offset = self._scroll_offset
        viewport = self._get_viewport_rect()
        if not viewport:
            max_offset = total - 1
        else:
            gap = max(0, int(viewport.get("gap", 0) or 0))
            max_visible = self._max_children_in_viewport(viewport)
            max_offset = max(0, total - max_visible)

        self._scroll_offset = max(0, min(index, max_offset))
        if self._scroll_offset == 0:
            self._auto_scroll = False
        if self._scroll_offset != previous_offset:
            self.invalidate()

    def scroll_position(self):
        """Return the current child/line scroll position for later restore."""
        return {
            "scroll_offset": self._scroll_offset,
            "view_offset": self._view_offset,
            "auto_scroll": self._auto_scroll,
        }

    def restore_scroll_position(self, position):
        """Restore a position returned by ``scroll_position`` after child refresh."""
        if not position:
            return False
        previous = self.scroll_position()
        total = len(self._children)
        self._scroll_offset = max(0, min(int(position.get("scroll_offset", 0) or 0), max(0, total - 1)))
        self._view_offset = max(0, int(position.get("view_offset", 0) or 0))
        self._auto_scroll = bool(position.get("auto_scroll", False))
        max_view_offset = self._max_view_offset()
        self._view_offset = min(self._view_offset, max_view_offset)
        changed = previous != self.scroll_position()
        if changed:
            self.invalidate()
        return changed

    def scroll_to_bottom(self):
        """Scroll to the last child so it is visible at the bottom."""
        previous_view_offset = self._view_offset
        previous_scroll_offset = self._scroll_offset
        self._scroll_offset = 0
        self._view_offset = 0
        self._auto_scroll = True
        if previous_view_offset != self._view_offset or previous_scroll_offset != self._scroll_offset:
            self.invalidate()
            return True
        return False

    def scroll_by(self, delta):
        """Scroll by *delta* children.

        Positive delta scrolls down (towards the end), negative scrolls up
        (towards the beginning).
        """
        self.scroll_to(self._scroll_offset + delta)

    def scroll_lines(self, delta):
        """Scroll by line rows using the same view offset as mouse wheel.

        Positive delta scrolls up towards the start of the content, negative
        delta scrolls down towards the end.
        """
        max_offset = self._max_view_offset()
        previous_offset = max(0, min(max_offset, self._view_offset))
        self._view_offset = max(0, min(max_offset, previous_offset + delta))
        self._auto_scroll = self._view_offset == 0
        if self._view_offset != previous_offset:
            self._pending_terminal_scroll_delta += self._view_offset - previous_offset
            self.invalidate()
            return True
        return False

    def consume_terminal_scroll_delta(self):
        delta = self._pending_terminal_scroll_delta
        self._pending_terminal_scroll_delta = 0
        return delta

    def scroll_to_top(self):
        previous_offset = self._view_offset
        self._view_offset = self._max_view_offset()
        self._auto_scroll = False
        if self._view_offset != previous_offset:
            self.invalidate()
            return True
        return False

    def _scroll_h(self, delta):
        """Scroll horizontally by delta columns (positive = right, negative = left)."""
        previous = self._h_offset
        self._h_offset = max(0, self._h_offset + delta)
        if self._h_offset != previous:
            self.invalidate()
            return True
        return False

    def scroll_page_up(self):
        """Scroll up by approximately one viewport height of children."""
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return False
        return self.scroll_lines(max(1, int(viewport.get("height", 1) or 1)))

    def scroll_page_down(self):
        """Scroll down by approximately one viewport height of children."""
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return False
        return self.scroll_lines(-max(1, int(viewport.get("height", 1) or 1)))

    def _wheel_scroll_lines(self):
        """Return the number of line rows to move for one wheel event."""
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return self.LINES_PER_SCROLL
        viewport_height = max(1, int(viewport.get("height", 1) or 1))
        viewport_limited = max(1, viewport_height // self.MAX_WHEEL_SCROLL_VIEWPORT_FRACTION)
        return max(1, min(self.LINES_PER_SCROLL, viewport_limited))

    def _can_scroll_up(self):
        """Return True if there are children above the current viewport."""
        return self._scroll_offset > 0

    def _can_scroll_down(self):
        """Return True if there are children below the current viewport."""
        total = len(self._children)
        if total == 0:
            return False
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return False
        max_visible = self._max_children_in_viewport(viewport)
        visible_from_offset = total - self._scroll_offset
        return visible_from_offset > max_visible

    def _max_view_offset(self):
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return 0
        children = self._get_children()
        if not children:
            self._last_natural_skip = 0
            return 0
        viewport_width = viewport["width"]
        gap = max(0, int(viewport.get("gap", 0) or 0))
        if (
            self._cached_child_heights is None
            or len(self._cached_child_heights) != len(children)
        ):
            self._cached_child_heights = [
                self._measure_child_height(child, viewport_width)
                for child in children
            ]
        remaining = len(children) - self._scroll_offset
        total_height = sum(self._cached_child_heights[self._scroll_offset:]) + gap * max(0, remaining - 1)
        natural_skip = max(0, total_height - viewport["height"])
        self._sync_view_offset_for_natural_skip(natural_skip)
        self._last_natural_skip = natural_skip
        return natural_skip

    def _sync_view_offset_for_natural_skip(self, natural_skip):
        previous_natural_skip = max(0, int(getattr(self, "_last_natural_skip", 0) or 0))
        natural_skip = max(0, int(natural_skip or 0))
        if self._auto_scroll:
            self._view_offset = 0
        elif self._view_offset >= previous_natural_skip:
            self._view_offset = natural_skip
        elif self._view_offset > natural_skip:
            self._view_offset = natural_skip

    def _content_window(self, viewport=None):
        viewport = viewport or self._clamped_viewport_rect()
        if not viewport:
            return None
        children = self._get_children()
        if not children:
            self._last_natural_skip = 0
            return {
                "viewport": viewport,
                "actual_skip": 0,
                "natural_skip": 0,
                "gap": 0,
            }
        max_offset = self._max_view_offset()
        clamped_view_offset = min(self._view_offset, max_offset)
        return {
            "viewport": viewport,
            "actual_skip": max_offset - clamped_view_offset,
            "natural_skip": max_offset,
            "gap": max(0, int(viewport.get("gap", 0) or 0)),
        }

    def child_view_entries(self, viewport=None):
        """Return child positions relative to this scrollview's coordinate space."""
        window = self._content_window(viewport)
        if window is None:
            return []
        viewport = window["viewport"]
        children = self._get_children()
        if not children:
            return []
        if (
            self._cached_child_heights is None
            or len(self._cached_child_heights) != len(children)
        ):
            self._cached_child_heights = [
                self._measure_child_height(child, viewport["width"])
                for child in children
            ]

        entries = []
        sequence_row = 0
        viewport_top = viewport["row"]
        viewport_bottom = viewport["row"] + viewport["height"]
        actual_skip = window["actual_skip"]
        gap = window["gap"]
        for index in range(self._scroll_offset, len(children)):
            child = children[index]
            child_height = self._cached_child_heights[index]
            child_row = viewport["row"] + sequence_row - actual_skip
            child_bottom = child_row + child_height
            entries.append({
                "index": index,
                "child": child,
                "row": child_row,
                "col": viewport["col"],
                "width": viewport["width"],
                "height": child_height,
                "visible": child_bottom > viewport_top and child_row < viewport_bottom,
            })
            sequence_row += child_height
            if index + 1 < len(children):
                sequence_row += gap
        return entries

    def ensure_element_visible(self, element):
        """Scroll just enough for a descendant element to be visible."""
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return False
        children = self._get_children()
        if not children:
            return False
        if (
            self._cached_child_heights is None
            or len(self._cached_child_heights) != len(children)
        ):
            self._max_view_offset()

        gap = max(0, int(viewport.get("gap", 0) or 0))
        sequence_row = 0
        target_top = None
        target_bottom = None
        for index in range(self._scroll_offset, len(children)):
            child = children[index]
            child_height = self._cached_child_heights[index]
            if hasattr(child, "_element_focus_rect"):
                rect = child._element_focus_rect(element)
                if rect is not None and self._child_contains_element(child, element):
                    target_top = sequence_row + rect["top"]
                    target_bottom = sequence_row + rect["bottom"]
                    break
            sequence_row += child_height
            if index + 1 < len(children):
                sequence_row += gap

        if target_top is None or target_bottom is None:
            return False

        natural_skip = self._max_view_offset()
        current_actual_skip = natural_skip - min(self._view_offset, natural_skip)
        next_actual_skip = current_actual_skip
        if target_top < current_actual_skip:
            next_actual_skip = target_top
        elif target_bottom > current_actual_skip + viewport["height"]:
            next_actual_skip = target_bottom - viewport["height"]
        next_actual_skip = max(0, min(natural_skip, next_actual_skip))
        next_view_offset = natural_skip - next_actual_skip
        if next_view_offset == self._view_offset:
            return False
        self._view_offset = next_view_offset
        self._auto_scroll = self._view_offset == 0
        self.invalidate()
        return True

    @staticmethod
    def _child_contains_element(child, element):
        if child is element:
            return True
        if hasattr(child, "_elements") and element in child._elements.values():
            return True
        for nested in getattr(child, "_elements", {}).values() if hasattr(child, "_elements") else []:
            nested_child = getattr(nested, "_child_instance", None)
            if nested_child is not None and UIScrollView._child_contains_element(nested_child, element):
                return True
        return False

    @classmethod
    def _view_contains_element(cls, view, element):
        if view is None or element is None:
            return False
        if view is element:
            return True
        if hasattr(view, "_elements") and element in view._elements.values():
            return True
        for nested in getattr(view, "_elements", {}).values() if hasattr(view, "_elements") else []:
            nested_child = getattr(nested, "_child_instance", None)
            if nested_child is not None and cls._view_contains_element(nested_child, element):
                return True
        get_children = getattr(view, "_get_children", None)
        if callable(get_children):
            for child in get_children():
                if cls._view_contains_element(child, element):
                    return True
        return False

    def _max_children_in_viewport(self, viewport):
        """Estimate the maximum number of children that can fit in *viewport*.

        Uses an average height heuristic based on the first few children.
        """
        gap = max(0, int(viewport.get("gap", 0) or 0))
        if not self._children:
            return 0

        total_height = viewport["height"]
        sample_size = min(5, len(self._children))

        if sample_size == 0:
            return 1

        # Use cached heights if available and valid
        if (hasattr(self, '_cached_child_heights') and
                self._cached_child_heights is not None and
                len(self._cached_child_heights) == len(self._children)):
            avg_height = sum(self._cached_child_heights[:sample_size]) / sample_size
        else:
            avg_height = 0
            for child in self._children[:sample_size]:
                avg_height += self._measure_child_height(child, viewport["width"])

            avg_height = avg_height / sample_size

        avg_height = max(1, avg_height)
        children_count = (total_height + gap) // (avg_height + gap) if avg_height + gap > 0 else 1
        return max(1, int(children_count))

    def _clamped_viewport_rect(self):
        viewport = self._get_viewport_rect()
        if not viewport:
            return None
        row = max(0, int(viewport.get("row", 0) or 0))
        col = max(0, int(viewport.get("col", 0) or 0))
        width = max(0, min(
            int(viewport.get("width", 0) or 0),
            max(0, int(getattr(self, "_window_width", 0) or 0) - col),
        ))
        height = max(0, min(
            int(viewport.get("height", 0) or 0),
            max(0, int(getattr(self, "_window_height", 0) or 0) - row),
        ))
        if width <= 0 or height <= 0:
            return None
        clamped = dict(viewport)
        clamped.update({"row": row, "col": col, "width": width, "height": height})
        return clamped

    # ------------------------------------------------------------------
    # Public API – children management
    # ------------------------------------------------------------------

    def get_visible_children(self, viewport):
        """Return list of ``{"child": ..., "rendered": [...], "row": ...}`` for
        children visible inside *viewport*.

        Renders bottom-up so the last child appears at the bottom.
        Supports partial visibility (children clipped to viewport edges) and
        returns metadata ``has_more_above`` / ``has_more_below``.
        """
        gap = max(0, int(viewport.get("gap", 0) or 0))
        viewport_height = viewport["height"]

        children = self._get_children()

        # Render all children first (from scroll_offset onwards)
        rendered_children = []
        for child in children[self._scroll_offset:]:
            rendered = self._render_child(child, viewport["width"])
            rendered_children.append((child, rendered))

        # Build cumulative heights from the bottom (last child first)
        total_children = len(rendered_children)

        # Calculate heights from bottom up to find which children are visible
        # We want the last N children that fit in viewport, starting from scroll_offset
        if total_children == 0:
            return {
                "entries": [],
                "has_more_above": False,
                "has_more_below": False,
            }

        # Compute heights of all children (with gaps) from scroll_offset
        child_heights = []
        for idx, (child, rendered) in enumerate(rendered_children):
            h = len(rendered) if isinstance(rendered, list) else 0
            child_heights.append(h)

        # Calculate total height needed for all children from scroll_offset
        if child_heights:
            total_needed = sum(child_heights) + gap * max(0, len(child_heights) - 1)
        else:
            total_needed = 0

        has_more_below = (self._scroll_offset + total_children) < len(children)

        # Now determine which children fit from the bottom
        visible_entries = []
        used_height = 0
        first_visible_idx = total_children  # index into rendered_children

        for i in range(total_children - 1, -1, -1):
            child, rendered = rendered_children[i]
            ch = len(rendered) if isinstance(rendered, list) else 0
            needed_height = ch + (gap if visible_entries else 0)

            if used_height + needed_height > viewport_height:
                # Partial visibility: clip this child to remaining space
                if not visible_entries and used_height < viewport_height:
                    remaining = viewport_height - used_height
                    if remaining > 0 and isinstance(rendered, list) and len(rendered) > 0:
                        clipped = rendered[:remaining]
                        visible_entries.append((child, clipped))
                        used_height += len(clipped)
                break

            visible_entries.append((child, rendered))
            used_height += needed_height
            first_visible_idx = i

        visible_entries.reverse()

        # has_more_above: there are children before scroll_offset
        has_more_above = self._scroll_offset > 0

        # Build result with absolute rows
        result = []
        row = viewport["row"]
        for index, (child, rendered) in enumerate(visible_entries):
            result.append({
                "child": child,
                "rendered": rendered,
                "row": row,
            })
            rh = len(rendered) if isinstance(rendered, list) else 0
            row += rh
            if index < len(visible_entries) - 1:
                row += gap

        return {
            "entries": result,
            "has_more_above": has_more_above,
            "has_more_below": has_more_below,
        }

    def _render_child(self, child, width):
        """Render a single child at the given *width*.

        Override in subclasses to customise rendering (e.g. resize first).
        """
        self._prepare_child_for_viewport(child, width)
        return child.render()

    def _measure_child_height(self, child, width):
        """Measure a child without rendering it when layout metadata is enough."""
        if hasattr(child, "get_natural_size") and hasattr(child, "resize"):
            try:
                _natural_width, natural_height = child.get_natural_size()
            except Exception:
                natural_height = None
            self._prepare_child_for_viewport(child, width)
            resolved_height = getattr(child, "_window_height", natural_height)
            try:
                return max(1, int(resolved_height or natural_height or 1))
            except (TypeError, ValueError):
                return 1
        rendered = self._render_child(child, width)
        return len(rendered) if isinstance(rendered, list) else 0

    def _render_child_cells(self, child, width, clip=None):
        """Render a single child to structured cells."""
        previous_clip = getattr(child, "_render_cell_clip", None)
        if clip is not None:
            child._render_cell_clip = clip
        if type(self)._render_child is not UIScrollView._render_child:
            try:
                rendered = [self._row_to_cells(line) for line in self._render_child(child, width)]
                if hasattr(child, "render_cells"):
                    rendered = self._preserve_child_raw_cells(rendered, child.render_cells())
                return rendered
            finally:
                if clip is not None:
                    child._render_cell_clip = previous_clip
        try:
            self._prepare_child_for_viewport(child, width)
            restore_focused = None
            if getattr(self, "_focused", False) and not self._child_has_focused_descendant(child):
                restore_focused = self._focused
                self._focused = False
            if hasattr(child, "render_cells"):
                return child.render_cells()
            return [self._row_to_cells(line) for line in child.render()]
        finally:
            if 'restore_focused' in locals() and restore_focused is not None:
                self._focused = restore_focused
            if clip is not None:
                child._render_cell_clip = previous_clip

    def _prepare_child_for_viewport(self, child, width):
        """Give generated child controls the viewport width before measuring/rendering."""
        if not (hasattr(child, "get_natural_size") and hasattr(child, "resize")):
            return
        try:
            _natural_width, natural_height = child.get_natural_size()
        except Exception:
            return
        target_width = max(1, int(width or 1))
        target_height = max(1, int(natural_height or 1))
        child.resize(target_width, target_height)
        resolved = getattr(child, "_resolved_cells_by_name", None)
        if resolved:
            actual_height = max(
                int(info.get("row", 0) or 0) + int(info.get("height", 0) or 0)
                for info in resolved.values()
            )
            if actual_height > target_height:
                child.resize(target_width, actual_height)

    # ------------------------------------------------------------------
    # Rendering
    # ------------------------------------------------------------------

    def render(self):
        """Render the scroll view using only visible children for efficiency."""
        return cells_to_ansi_lines(self.render_cells())

    def render_cells(self):
        """Render the scroll view to structured terminal cells."""
        return self._render_scroll_content(use_cell_render=True)

    def _render_scroll_content(self, use_cell_render=False):
        """Render visible scroll content using string or cell rows."""
        viewport = self._clamped_viewport_rect()
        if not viewport:
            return super().render_cells() if use_cell_render else super().render()
        self._restore_pending_proxy_focus()

        # Collect base element grid from non-child elements
        elem_grid = self._base_element_grid_for_render(use_cell_render)

        # Render only rows intersecting the viewport into an offscreen buffer.
        h_offset = max(0, int(self._h_offset))
        viewport_width = viewport["width"]
        render_width = viewport_width + h_offset  # extra width for H scroll slicing
        offscreen_rows = []

        children = self._get_children()
        gap = max(0, int(viewport.get("gap", 0) or 0))

        child_row_indexes = set()
        selected_child_row_indexes = set()
        focused_descendant_background_map = {}

        if not children:
            rendered = self._render_cell_frame(elem_grid) if use_cell_render else self._render(elem_grid)
            return self._apply_focus_backgrounds(rendered, viewport, child_row_indexes) if use_cell_render else rendered

        remaining = len(children) - self._scroll_offset

        # Calculate total height using cached heights (or measure if no cache)
        if (hasattr(self, '_cached_child_heights') and
                self._cached_child_heights is not None and
                len(self._cached_child_heights) == len(children)):
            total_height = sum(self._cached_child_heights[self._scroll_offset:]) + gap * max(0, remaining - 1)
        else:
            # First render or cache invalidated: measure all children to build cache
            self._cached_child_heights = [
                self._measure_child_height(child, viewport_width)
                for child in children
            ]
            total_height = sum(self._cached_child_heights[self._scroll_offset:]) + gap * max(0, remaining - 1)

        # Calculate skip: natural bottom-anchor minus line-level view offset
        natural_skip = max(0, total_height - viewport["height"])
        self._sync_view_offset_for_natural_skip(natural_skip)
        self._last_natural_skip = natural_skip
        clamped_view_offset = min(self._view_offset, natural_skip)
        actual_skip = natural_skip - clamped_view_offset

        viewport_start = actual_skip
        viewport_end = actual_skip + viewport["height"]
        sequence_row = 0
        visible_start = self._scroll_offset
        has_visible_child = False

        for index in range(self._scroll_offset, len(children)):
            if index > self._scroll_offset and gap > 0:
                gap_start = sequence_row
                gap_end = sequence_row + gap
                if gap_end > viewport_start and gap_start < viewport_end:
                    start = max(viewport_start, gap_start)
                    end = min(viewport_end, gap_end)
                    if use_cell_render:
                        offscreen_rows.extend(
                            [TerminalCell(" ") for _col in range(render_width)]
                            for _row in range(end - start)
                        )
                    else:
                        offscreen_rows.extend(" " * render_width for _ in range(end - start))
                sequence_row = gap_end

            child_height = self._cached_child_heights[index]
            child_start = sequence_row
            child_end = sequence_row + child_height
            if child_end > viewport_start and child_start < viewport_end:
                if not has_visible_child:
                    visible_start = index
                has_visible_child = True
                start = max(viewport_start, child_start) - child_start
                end = min(viewport_end, child_end) - child_start
                partial_child = start > 0 or end < child_height
                rendered = (
                    self._render_child_cells(
                        children[index],
                        render_width,
                        clip={"top": start, "bottom": end} if partial_child else None,
                    )
                    if use_cell_render
                    else self._render_child(children[index], render_width)
                )
                if use_cell_render:
                    rendered = self._apply_selected_background_to_focused_child(rendered, children[index])
                if len(rendered) != child_height:
                    self._cached_child_heights[index] = len(rendered)
                    self.invalidate_layout()
                    return self.render_cells() if use_cell_render else self.render()
                if use_cell_render:
                    child_row_indexes.update(range(len(offscreen_rows), len(offscreen_rows) + max(0, end - start)))
                    if (
                        self._child_has_focused_descendant(children[index])
                        and self._should_select_focused_child(children[index])
                    ):
                        selected_child_row_indexes.update(
                            range(
                                len(offscreen_rows),
                                len(offscreen_rows) + max(0, end - start),
                            )
                        )
                        focus_background = (
                            getattr(self.focus_style, "background", None)
                            if self.focus_style is not None
                            else None
                        )
                        focused_descendant_background_map.update(
                            self._focused_descendant_background_map(
                                focus_background,
                                children[index],
                            )
                        )
                offscreen_rows.extend(rendered[start:end])
            sequence_row = child_end
            if sequence_row >= viewport_end:
                break

        if not offscreen_rows:
            rendered = self._render_cell_frame(elem_grid) if use_cell_render else self._render(elem_grid)
            return self._apply_focus_backgrounds(rendered, viewport, child_row_indexes) if use_cell_render else rendered

        # Overlay visible portion of offscreen buffer into elem_grid
        for i, line in enumerate(offscreen_rows):
            target_row = viewport["row"] + i
            if target_row >= viewport["row"] + viewport["height"]:
                break
            if target_row not in elem_grid:
                elem_grid[target_row] = {}
            if h_offset > 0:
                line = (
                    self._slice_rendered_cells(line, h_offset, viewport_width)
                    if use_cell_render
                    else self._slice_rendered_text(line, h_offset, viewport_width)
                )
            elem_grid[target_row][viewport["col"]] = line

        # Determine scroll metadata for indicators
        has_more_above = actual_skip > 0 or (visible_start > self._scroll_offset)
        has_more_below = clamped_view_offset > 0

        # Add scroll indicators
        if has_more_above:
            self._add_scroll_indicator(elem_grid, viewport, above=True, use_cell_render=use_cell_render)
        if has_more_below:
            self._add_scroll_indicator(elem_grid, viewport, above=False, use_cell_render=use_cell_render)

        rendered = self._render_cell_frame(elem_grid) if use_cell_render else self._render(elem_grid)
        if use_cell_render:
            rendered = self._apply_selected_background_to_viewport_rows(
                rendered,
                viewport,
                selected_child_row_indexes,
                focused_descendant_background_map,
            )
        return self._apply_focus_backgrounds(rendered, viewport, child_row_indexes) if use_cell_render else rendered

    def _restore_pending_proxy_focus(self):
        if not getattr(self, "_pending_proxy_focus_restore", False):
            return
        self._pending_proxy_focus_restore = False
        proxy = getattr(self, "parent", None)
        if proxy is not None and hasattr(proxy, "focused"):
            proxy.focused = True
        self._focused = True
        owner = proxy
        while owner is not None:
            if hasattr(owner, "_focused_element"):
                owner._focused_element = proxy
            owner = getattr(owner, "parent", None)

    def _apply_selected_background_to_viewport_rows(
        self,
        rendered,
        viewport,
        row_indexes,
        focused_descendant_background_map=None,
    ):
        if not row_indexes or self.selected_style is None:
            return rendered
        focused_descendant_background_map = focused_descendant_background_map or {}
        selected_background = getattr(self.selected_style, "background", None)
        if selected_background is None:
            return rendered
        style_background = getattr(getattr(self, "style", None), "background", None)
        base_keys = {
            self._color_key(background)
            for background in (style_background,)
            if background is not None
        }
        transparent_self_background = (
            style_background is not None
            and getattr(style_background, "alpha", 1.0) <= 0.0
        )
        top = max(0, int(viewport["row"]) - (1 if transparent_self_background else 0))
        left = max(0, int(viewport["col"]) - (1 if transparent_self_background else 0))
        right = int(viewport["col"]) + int(viewport["width"]) + 1
        selected_rows = {top + row_index for row_index in row_indexes}
        next_rows = []
        for row_index, row_cells in enumerate(rendered):
            if row_index not in selected_rows:
                next_rows.append(row_cells)
                continue
            next_row = []
            for col_index, cell in enumerate(row_cells):
                if col_index < left or col_index >= right:
                    next_row.append(cell)
                    continue
                cell_background_key = self._color_key(cell.background)
                if (
                    not transparent_self_background
                    and col_index == right - 1
                    and style_background is not None
                ):
                    focus_background = (
                        getattr(self.focus_style, "background", None)
                        if self.focus_style is not None
                        else None
                    )
                    edge_background = (
                        self._blend_color_over_exact_alpha(focus_background, style_background)
                        if focus_background is not None
                        else self._blend_color_over(selected_background, style_background)
                    )
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        edge_background,
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                    continue
                if cell.background is not None and cell_background_key in focused_descendant_background_map:
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        focused_descendant_background_map[cell_background_key],
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                    continue
                background = cell.background if cell.background is not None else style_background
                if cell.background is None or self._color_key(cell.background) in base_keys:
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        self._blend_color_over(selected_background, background),
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                else:
                    next_row.append(cell)
            next_rows.append(next_row)
        return next_rows

    def _apply_selected_background_to_focused_child(self, rendered, child):
        if (
            not self._child_has_focused_descendant(child)
            or not self._should_select_focused_child(child)
        ):
            return rendered
        selected_background = (
            getattr(self.selected_style, "background", None)
            if self.selected_style is not None
            else None
        )
        if selected_background is None:
            return rendered
        child_style = getattr(child, "style", None)
        child_background = getattr(child_style, "background", None) if child_style is not None else None
        if child_background is None and hasattr(child, "_get_cell_background"):
            child_background = child._get_cell_background()
        if child_background is None:
            child_background = self._viewport_cell_background()
        if child_background is None:
            child_background = getattr(getattr(self, "style", None), "background", None)
        selected_keys = self._selected_background_keys(selected_background, child, child_background)
        focus_background = (
            getattr(self.focus_style, "background", None)
            if self.focus_style is not None
            else None
        )
        focused_descendant_background_map = self._focused_descendant_background_map(focus_background, child)
        descendant_base_background_keys = {
            self._color_key(background)
            for background in self._descendant_base_style_backgrounds(child)
            if background is not None
        }
        style_background = getattr(getattr(self, "style", None), "background", None)
        skip_blank_rows = not (
            style_background is not None
            and getattr(style_background, "alpha", 1.0) <= 0.0
        )
        styled = []
        for raw_row in rendered:
            row = self._row_to_cells(raw_row)
            if skip_blank_rows and not self._row_has_rendered_content(row):
                styled.append(row)
                continue
            next_row = []
            last_col_index = len(row) - 1
            for col_index, cell in enumerate(row):
                if skip_blank_rows and (col_index == 0 or col_index == last_col_index):
                    next_row.append(cell)
                    continue
                cell_background_key = self._color_key(cell.background)
                if cell.background is not None and cell_background_key in focused_descendant_background_map:
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        focused_descendant_background_map[cell_background_key],
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                    continue
                if (
                    cell.background is not None
                    and focus_background is not None
                    and cell_background_key in descendant_base_background_keys
                ):
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        self._blend_color_over_exact_alpha(focus_background, cell.background),
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                    continue
                if cell.background is not None and self._color_key(cell.background) in selected_keys:
                    next_row.append(cell)
                    continue
                background = cell.background if cell.background is not None else child_background
                next_row.append(TerminalCell(
                    cell.text,
                    cell.foreground,
                    self._blend_color_over(selected_background, background),
                    cell.raw,
                    cell.raw_width,
                    cell.raw_height,
                    cell.raw_skip,
                ))
            styled.append(next_row)
        return styled

    @staticmethod
    def _row_has_rendered_content(row):
        for cell in row:
            if getattr(cell, "raw", None):
                return True
            text = getattr(cell, "text", "")
            if text and text != " ":
                return True
        return False

    @staticmethod
    def _preserve_child_raw_cells(rendered, structured):
        if not rendered or not structured:
            return rendered
        for row_index, structured_row in enumerate(structured):
            if row_index >= len(rendered):
                break
            rendered_row = rendered[row_index]
            for col_index, structured_cell in enumerate(UIScrollView._row_to_cells(structured_row)):
                if col_index >= len(rendered_row):
                    break
                if not getattr(structured_cell, "raw", "") and not getattr(structured_cell, "raw_skip", False):
                    continue
                base_cell = rendered_row[col_index]
                rendered_row[col_index] = TerminalCell(
                    base_cell.text,
                    base_cell.foreground,
                    base_cell.background,
                    structured_cell.raw,
                    structured_cell.raw_width,
                    structured_cell.raw_height,
                    structured_cell.raw_skip,
                )
        return rendered

    def _focused_descendant_background_map(self, focus_background, child):
        if focus_background is None:
            return {}
        backgrounds = {}
        for background in self._descendant_base_style_backgrounds(child):
            if background is None:
                continue
            exact = self._blend_color_over_exact_alpha(focus_background, background)
            base_key = self._color_key(background)
            if base_key is not None:
                backgrounds[base_key] = exact
            for blended in (
                self._blend_color_over(focus_background, background),
                exact,
            ):
                key = self._color_key(blended)
                if key is not None:
                    backgrounds[key] = exact
        return backgrounds

    @classmethod
    def _descendant_base_style_backgrounds(cls, child):
        backgrounds = []
        for style_name in ("style", "focus_style", "edit_style"):
            style = getattr(child, style_name, None)
            background = getattr(style, "background", None) if style is not None else None
            if cls._is_opaque_background(background):
                backgrounds.append(background)
        child_instance = getattr(child, "_child_instance", None)
        if child_instance is not None:
            backgrounds.extend(cls._descendant_base_style_backgrounds(child_instance))
        for nested in getattr(child, "_elements", {}).values():
            nested_child = getattr(nested, "_child_instance", None)
            if nested_child is not None:
                backgrounds.extend(cls._descendant_base_style_backgrounds(nested_child))
        return backgrounds

    @staticmethod
    def _is_opaque_background(background):
        return background is not None and getattr(background, "r", -1) >= 0 and getattr(background, "alpha", 1.0) >= 1.0

    def _selected_background_keys(self, selected_background, child, child_background):
        candidates = [
            child_background,
            self._viewport_cell_background(),
            getattr(getattr(self, "style", None), "background", None),
            getattr(getattr(child, "style", None), "background", None),
        ]
        candidates.extend(self._descendant_style_backgrounds(child))
        keys = {
            self._color_key(self._blend_color_over(selected_background, background))
            for background in candidates
            if background is not None
        }
        keys.update(
            self._exact_alpha_blend_key(selected_background, background)
            for background in candidates
            if background is not None
        )
        keys.discard(None)
        return keys

    @staticmethod
    def _exact_alpha_blend_key(color, background):
        blended = UIScrollView._blend_color_over_exact_alpha(color, background)
        return UIScrollView._color_key(blended)

    @staticmethod
    def _blend_color_over_exact_alpha(color, background):
        hex_value = getattr(color, "hex", "")
        if (
            not isinstance(hex_value, str)
            or len(hex_value) != 9
            or background is None
        ):
            return UIScrollView._blend_color_over(color, background)
        if isinstance(background, str) and len(background) == 9 and background.isdigit():
            background = Color(
                f"#{int(background[0:3]):02x}{int(background[3:6]):02x}{int(background[6:9]):02x}"
            )
        if getattr(background, "r", -1) < 0:
            return UIScrollView._blend_color_over(color, background)
        alpha = int(hex_value[7:9], 16) / 255
        r = round(alpha * color.r + (1 - alpha) * background.r)
        g = round(alpha * color.g + (1 - alpha) * background.g)
        b = round(alpha * color.b + (1 - alpha) * background.b)
        return Color(f"#{r:02x}{g:02x}{b:02x}")

    @classmethod
    def _descendant_style_backgrounds(cls, child):
        backgrounds = []
        for style_name in (
            "style",
            "focus_style",
            "edit_style",
            "selected_style",
            "checked_style",
            "unchecked_style",
            "error_style",
        ):
            style = getattr(child, style_name, None)
            background = getattr(style, "background", None) if style is not None else None
            if background is not None:
                backgrounds.append(background)
        child_instance = getattr(child, "_child_instance", None)
        if child_instance is not None:
            backgrounds.extend(cls._descendant_style_backgrounds(child_instance))
        for nested in getattr(child, "_elements", {}).values():
            for style_name in (
                "style",
                "focus_style",
                "edit_style",
                "selected_style",
                "checked_style",
                "unchecked_style",
                "error_style",
            ):
                style = getattr(nested, style_name, None)
                background = getattr(style, "background", None) if style is not None else None
                if background is not None:
                    backgrounds.append(background)
            nested_child = getattr(nested, "_child_instance", None)
            if nested_child is not None:
                backgrounds.extend(cls._descendant_style_backgrounds(nested_child))
        return backgrounds

    @staticmethod
    def _child_has_focused_descendant(child):
        focused = getattr(child, "_focused_element", None)
        if focused is not None:
            return True
        for nested in getattr(child, "_elements", {}).values():
            if getattr(nested, "focused", False):
                return True
            nested_child = getattr(nested, "_child_instance", None)
            if nested_child is not None and UIScrollView._child_has_focused_descendant(nested_child):
                return True
        return False

    def _apply_focus_backgrounds(self, rendered, viewport, child_row_indexes):
        rendered = self._apply_self_focus_to_viewport(rendered, viewport)
        rendered = self._apply_active_focus_to_gap_rows(rendered, viewport, child_row_indexes)
        return self._restore_unfocused_child_content_backgrounds(rendered, viewport)

    def _restore_unfocused_child_content_backgrounds(self, rendered, viewport):
        if (
            self._ancestor_active_scrollview_scope_proxy() is None
            and not any(
                self._active_scrollview_scope_for(child)
                for child in self._get_children()
                if hasattr(self, "_active_scrollview_scope_for")
            )
        ):
            return rendered
        style_background = getattr(getattr(self, "style", None), "background", None)
        if style_background is None or getattr(style_background, "alpha", 1.0) > 0.0:
            return rendered
        if any(self._child_has_focused_descendant(child) for child in self._get_children()):
            return rendered
        for child in self._get_children():
            focusable = getattr(child, "_local_focusable_elements", None)
            if callable(focusable) and focusable():
                return rendered
        focus_background = (
            getattr(self.focus_style, "background", None)
            if self.focus_style is not None
            else None
        )
        viewport_background = self._viewport_cell_background()
        if focus_background is None or viewport_background is None:
            return rendered
        focused_viewport_background = self._blend_color_over_exact_alpha(
            focus_background,
            viewport_background,
        )
        focused_key = self._color_key(focused_viewport_background)
        if focused_key is None:
            return rendered
        top = max(0, int(viewport["row"]))
        bottom = min(len(rendered), int(viewport["row"]) + int(viewport["height"]))
        next_rows = []
        for row_index, row_cells in enumerate(rendered):
            content_bounds = self._row_rendered_content_bounds(row_cells)
            if (
                row_index < top
                or row_index >= bottom
                or content_bounds is None
            ):
                next_rows.append(row_cells)
                continue
            content_start_col = max(0, content_bounds[0] - 1)
            content_end_col = max(
                content_bounds[1],
                int(viewport["col"]) + int(viewport["width"]) - 1,
            )
            next_row = []
            for col_index, cell in enumerate(row_cells):
                if (
                    col_index >= content_start_col
                    and col_index <= content_end_col
                    and self._color_key(cell.background) == focused_key
                ):
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        viewport_background,
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                else:
                    next_row.append(cell)
            next_rows.append(next_row)
        return next_rows

    def _apply_self_focus_to_viewport(self, rendered, viewport):
        proxy = getattr(self, "parent", None)
        proxy_focused = bool(getattr(proxy, "focused", False))
        self_focused = bool(getattr(self, "_focused", False))
        focus_style = (
            getattr(self, "focus_style", None)
            if self_focused
            else getattr(proxy, "focus_style", None)
        )
        focus_background = getattr(focus_style, "background", None) if focus_style is not None else None
        focused = bool(self_focused or proxy_focused)
        if not focused or focus_background is None:
            return rendered
        if proxy_focused and not self_focused and getattr(focus_background, "alpha", 1.0) < 1.0:
            return rendered

        viewport_background = self._viewport_cell_background()
        focused_viewport_background = (
            self._blend_color_over_exact_alpha(focus_background, viewport_background)
            if viewport_background is not None
            else None
        )
        viewport_background_key = self._color_key(viewport_background)
        focused_viewport_background_key = self._color_key(focused_viewport_background)
        focused_descendant_background_map = {}
        unfocused_descendant_background_keys = set()
        if self_focused:
            for child in self._get_children():
                if self._child_has_focused_descendant(child):
                    focused_descendant_background_map.update(
                        self._focused_descendant_background_map(focus_background, child)
                    )
                else:
                    unfocused_descendant_background_keys.update(
                        self._color_key(background)
                        for background in self._descendant_base_style_backgrounds(child)
                        if background is not None
                    )
            unfocused_descendant_background_keys.discard(None)
        if self_focused:
            top = 0
            bottom = len(rendered)
            left = 0
            right = max((len(row) for row in rendered), default=0)
        else:
            top = max(0, int(viewport["row"]))
            bottom = min(len(rendered), int(viewport["row"]) + int(viewport["height"]))
            left = max(0, int(viewport["col"]))
            right = int(viewport["col"]) + int(viewport["width"])
        next_rows = []
        for row_index, row_cells in enumerate(rendered):
            if row_index < top or row_index >= bottom:
                next_rows.append(row_cells)
                continue
            next_row = []
            content_start_col = self._row_rendered_content_start(row_cells)
            if content_start_col is not None:
                content_start_col = max(left, content_start_col - 1)
            for col_index, cell in enumerate(row_cells):
                if col_index < left or col_index >= right:
                    next_row.append(cell)
                    continue
                background = cell.background if cell.background is not None else viewport_background
                background_key = self._color_key(background)
                if cell.background is not None and background_key in focused_descendant_background_map:
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        focused_descendant_background_map[background_key],
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                    continue
                if (
                    cell.background is not None
                    and background_key in unfocused_descendant_background_keys
                    and background_key != viewport_background_key
                ):
                    next_row.append(cell)
                    continue
                matches_base_background = (
                    cell.background is None
                    or background_key == viewport_background_key
                    or (
                        focused_viewport_background is not None
                        and background_key == focused_viewport_background_key
                    )
                )
                if (
                    content_start_col is not None
                    and col_index >= content_start_col
                    and background_key != viewport_background_key
                ):
                    next_row.append(cell)
                    continue
                if not matches_base_background:
                    next_row.append(cell)
                    continue
                if (
                    focused_viewport_background is not None
                    and background_key == focused_viewport_background_key
                ):
                    next_row.append(cell)
                    continue
                next_row.append(TerminalCell(
                    cell.text,
                    cell.foreground,
                    self._blend_color_over_exact_alpha(focus_background, background),
                    cell.raw,
                    cell.raw_width,
                    cell.raw_height,
                    cell.raw_skip,
                ))
            next_rows.append(next_row)
        return next_rows

    @staticmethod
    def _row_rendered_content_start(row):
        bounds = UIScrollView._row_rendered_content_bounds(row)
        return bounds[0] if bounds is not None else None

    @staticmethod
    def _row_rendered_content_bounds(row):
        first = None
        last = None
        for index, cell in enumerate(row):
            has_content = False
            if getattr(cell, "raw", None):
                has_content = True
            text = getattr(cell, "text", "")
            if text and text != " " and text not in ("^", "v"):
                has_content = True
            if has_content:
                if first is None:
                    first = index
                last = index
        if first is None:
            return None
        return first, last

    def _apply_active_focus_to_gap_rows(self, rendered, viewport, child_row_indexes):
        if getattr(self, "_focused", False):
            return rendered
        focus_background = self._ancestor_active_scrollview_focus_background()
        viewport_background = self._viewport_cell_background()
        if (
            focus_background is None
            or viewport_background is None
            or getattr(viewport_background, "alpha", 1.0) < 1.0
        ):
            return rendered

        top = max(0, int(viewport["row"]))
        bottom = min(len(rendered), int(viewport["row"]) + int(viewport["height"]))
        left = max(0, int(viewport["col"]))
        right = int(viewport["col"]) + int(viewport["width"])
        focused_viewport_background = self._blend_color_over(focus_background, viewport_background)
        viewport_background_key = self._color_key(viewport_background)
        focused_viewport_background_key = self._color_key(focused_viewport_background)
        child_rows = {top + row_index for row_index in child_row_indexes}
        next_rows = []
        for row_index, row_cells in enumerate(rendered):
            if row_index < top or row_index >= bottom:
                next_rows.append(row_cells)
                continue
            if row_index in child_rows:
                next_rows.append(row_cells)
                continue

            next_row = []
            content_start_col = self._row_rendered_content_start(row_cells)
            if content_start_col is not None:
                content_start_col = max(left, content_start_col - 1)
            for col_index, cell in enumerate(row_cells):
                if col_index < left or col_index >= right:
                    next_row.append(cell)
                    continue
                if content_start_col is not None and col_index >= content_start_col:
                    next_row.append(cell)
                    continue
                background = cell.background
                background_key = self._color_key(background)
                matches_base_background = background is None or background_key == viewport_background_key
                matches_focused_base_background = (
                    focused_viewport_background is not None
                    and background_key == focused_viewport_background_key
                )
                if not matches_base_background and not matches_focused_base_background:
                    next_row.append(cell)
                    continue
                if matches_focused_base_background:
                    next_row.append(cell)
                    continue
                blended_background = self._blend_color_over(focus_background, background)
                if matches_base_background and not matches_focused_base_background:
                    blended_background = self._blend_color_over(focus_background, blended_background)
                next_row.append(TerminalCell(
                    cell.text,
                    cell.foreground,
                    blended_background,
                    cell.raw,
                    cell.raw_width,
                    cell.raw_height,
                    cell.raw_skip,
                ))
            next_rows.append(next_row)
        return next_rows

    def _viewport_cell_background(self):
        for cell_name in ("viewport", "panel"):
            style = self._cell_style(cell_name)
            background = getattr(style, "background", None) if style is not None else None
            if background is not None:
                return background
        return None

    def _add_scroll_indicator(self, elem_grid, viewport, above=True, use_cell_render=False):
        """Add a scroll indicator character at the edge of the viewport."""
        if above:
            row = viewport["row"]
        else:
            row = viewport["row"] + viewport["height"] - 1

        col = viewport["col"] + viewport["width"] - 1
        indicator = self.SCROLL_INDICATOR_ABOVE if above else self.SCROLL_INDICATOR_BELOW

        if row not in elem_grid:
            elem_grid[row] = {}
        row_entries = elem_grid[row]
        if not use_cell_render:
            row_entries[col] = indicator
            return
        rendered_cells = self._rendered_row_cells_by_col(row_entries)
        existing_cell = rendered_cells.get(col)
        foreground = existing_cell.foreground if existing_cell else None
        if foreground is None:
            foreground = self._infer_scroll_indicator_foreground(
                rendered_cells, viewport["col"], col
            )
        background = existing_cell.background if existing_cell else None
        row_entries[col] = [TerminalCell(indicator, foreground, background)]

    def _rendered_row_cells_by_col(self, row_entries):
        """Return final row cells keyed by absolute column before overlays."""
        rendered_cells = {}
        if not isinstance(row_entries, dict):
            return rendered_cells
        for col, value in sorted(row_entries.items()):
            for offset, cell in enumerate(self._row_to_cells(value)):
                rendered_cells[col + offset] = cell
        return rendered_cells

    def _infer_scroll_indicator_foreground(self, rendered_cells, start_col, indicator_col):
        """Infer the scroll indicator foreground from the visible row text."""
        foreground = None
        for col in range(max(0, int(start_col)), int(indicator_col) + 1):
            cell = rendered_cells.get(col)
            if cell is not None and cell.foreground is not None:
                foreground = cell.foreground
        return foreground

    def _get_children(self):
        """Return the list of child controls to render.

        Subclasses can override this to return a different source (e.g.
        ``self._messages`` instead of ``self._children``).
        """
        return self._children

    def _child_selection_entries(self, viewport):
        """Return child frame metadata for selection without rendering rows."""
        entries = self.child_view_entries(viewport)
        for entry in entries:
            self._prepare_child_for_viewport(entry["child"], entry["width"])
        return entries

    def _get_viewport_rect(self):
        """Return the viewport rectangle from a layout cell."""
        for cell_name in ("viewport", "panel"):
            rect = self.get_cell_rect(cell_name)
            if rect:
                return self._apply_viewport_padding(cell_name, rect)
        return None

    def _apply_viewport_padding(self, cell_name, rect):
        style = self._cell_style(cell_name)
        if not style or style.padding == (0, 0, 0, 0):
            return rect
        pad_top, pad_right, pad_bottom, pad_left = style.padding
        rect["row"] += pad_top
        rect["col"] += pad_left
        rect["width"] = max(0, rect["width"] - pad_left - pad_right)
        rect["height"] = max(0, rect["height"] - pad_top - pad_bottom)
        return rect

    def _base_element_grid_for_render(self, use_cell_render=False):
        try:
            return self._base_element_grid(use_cell_render=use_cell_render)
        except TypeError:
            return self._base_element_grid()

    def _base_element_grid(self, use_cell_render=False):
        """Collect rendered output from all non-cell elements."""
        elem_grid = {}
        for elem in self.get_all_elements():
            if getattr(elem, "name", "").startswith("_cell_"):
                continue
            rendered = (
                elem.render_cells()
                if use_cell_render and hasattr(elem, "render_cells")
                else elem.render()
            )
            abs_row = getattr(elem, "_cell_row", 0) + elem.row
            abs_col = getattr(elem, "_cell_col", 0) + elem.col
            for row_offset, line in enumerate(rendered):
                gr = abs_row + row_offset
                if gr not in elem_grid:
                    elem_grid[gr] = {}
                elem_grid[gr][abs_col] = line
        return elem_grid

    # ------------------------------------------------------------------
    # Mouse selection across children (universal)
    # ------------------------------------------------------------------

    def _selection_endpoint_at(self, row, col, allow_outside=False):
        """Find a selection endpoint at (*row*, *col*) across all children.

        Returns ``(child, label, pos)`` or ``None``.
        """
        viewport = self._get_viewport_rect()
        if not viewport:
            return None

        entries = self._child_selection_entries(viewport)
        if not entries:
            return None

        for entry in entries:
            top = entry["row"]
            bottom = top + entry["height"]
            if row < top or row >= bottom:
                continue

            child = entry["child"]
            endpoint = self._child_label_selection_endpoint_at(
                child,
                row - top,
                col - entry["col"],
                allow_outside=allow_outside,
            )
            if endpoint is None:
                return None
            label, pos = endpoint
            return child, label, pos

        if not allow_outside:
            return None

        first = entries[0]
        if row < first["row"]:
            endpoint = self._child_first_label_selection_endpoint(first["child"])
            if endpoint is None:
                return None
            label, pos = endpoint
            return first["child"], label, pos

        last = entries[-1]
        last_bottom = last["row"] + last["height"]
        if row >= last_bottom:
            endpoint = self._child_last_label_selection_endpoint(last["child"])
            if endpoint is None:
                return None
            label, pos = endpoint
            return last["child"], label, pos

        previous = None
        for entry in entries:
            if row < entry["row"]:
                if previous is None:
                    endpoint = self._child_first_label_selection_endpoint(entry["child"])
                    if endpoint is None:
                        return None
                    label, pos = endpoint
                    return entry["child"], label, pos
                endpoint = self._child_last_label_selection_endpoint(previous["child"])
                if endpoint is None:
                    return None
                label, pos = endpoint
                return previous["child"], label, pos
            previous = entry

        endpoint = self._child_last_label_selection_endpoint(last["child"])
        if endpoint is None:
            return None
        label, pos = endpoint
        return last["child"], label, pos

    def _set_selection(self, anchor, target):
        """Set selection spanning multiple children."""
        if anchor is None or target is None:
            return False

        viewport = self._get_viewport_rect()
        if not viewport:
            return False

        entries = self._child_selection_entries(viewport)
        children = [entry["child"] for entry in entries]
        if anchor[0] not in children or target[0] not in children:
            return False

        self._clear_selections()
        start = anchor
        end = target
        if self._endpoint_key(end, children) < self._endpoint_key(start, children):
            start, end = end, start

        start_child, start_label, start_pos = start
        end_child, end_label, end_pos = end
        start_idx = children.index(start_child)
        end_idx = children.index(end_child)

        for idx in range(start_idx, end_idx + 1):
            child = children[idx]
            if start_idx == end_idx:
                self._child_set_label_selection_from_endpoints(
                    child,
                    (start_label, start_pos),
                    (end_label, end_pos),
                )
            elif idx == start_idx:
                self._child_set_label_selection_from_endpoints(
                    child,
                    (start_label, start_pos),
                    self._child_last_label_selection_endpoint(child),
                )
            elif idx == end_idx:
                self._child_set_label_selection_from_endpoints(
                    child,
                    self._child_first_label_selection_endpoint(child),
                    (end_label, end_pos),
                )
            else:
                self._child_set_label_selection_from_endpoints(
                    child,
                    self._child_first_label_selection_endpoint(child),
                    self._child_last_label_selection_endpoint(child),
                )
        return True

    @staticmethod
    def _endpoint_key(endpoint, children):
        child, label, pos = endpoint
        labels = UIScrollView._child_copyable_labels(child)
        return children.index(child), labels.index(label), pos

    def _clear_selections(self):
        """Clear selections on all children."""
        for child in self._get_children():
            self._child_clear_label_selections(child)

    def _copy_selection(self, notify=True):
        """Copy selected text from all children to clipboard."""
        viewport = self._get_viewport_rect()
        if not viewport:
            return False

        from uimd.runtime.elements import _copy_text_to_clipboard, _notify_copied

        entries = self._child_selection_entries(viewport)
        texts = []
        notify_element = None
        for entry in entries:
            child = entry["child"]
            text = self._child_selected_label_text(child)
            if text:
                texts.append(text)
                if notify_element is None:
                    for label in self._child_copyable_labels(child):
                        if label.selected_text():
                            notify_element = label
                            break
        if not texts:
            return False
        _copy_text_to_clipboard("\n".join(texts))
        if notify and notify_element is not None:
            _notify_copied(notify_element)
        return True

    @staticmethod
    def _child_copyable_labels(child):
        if hasattr(child, "_copyable_labels"):
            return child._copyable_labels()
        if getattr(child, "ELEMENT_TYPE", None) not in COPYABLE_LABEL_TYPES:
            return []
        style = child.get_style() if hasattr(child, "get_style") else None
        return [child] if getattr(style, "user_select", None) == USER_SELECT_TEXT else []

    @classmethod
    def _child_label_selection_endpoint_at(cls, child, row, col, allow_outside=False):
        if hasattr(child, "_label_selection_endpoint_at"):
            return child._label_selection_endpoint_at(row, col, allow_outside=allow_outside)
        labels = cls._child_copyable_labels(child)
        if not labels:
            return None
        label = labels[0]
        height = max(1, int(getattr(label, "height", 1) or 1))
        if row < 0:
            if not allow_outside:
                return None
            return label, 0
        if row >= height:
            if not allow_outside:
                return None
            return label, len(getattr(label, "text", "") or "")
        return label, label.text_position_from_point(row, col)

    @classmethod
    def _child_first_label_selection_endpoint(cls, child):
        if hasattr(child, "_first_label_selection_endpoint"):
            return child._first_label_selection_endpoint()
        labels = cls._child_copyable_labels(child)
        return None if not labels else (labels[0], 0)

    @classmethod
    def _child_last_label_selection_endpoint(cls, child):
        if hasattr(child, "_last_label_selection_endpoint"):
            return child._last_label_selection_endpoint()
        labels = cls._child_copyable_labels(child)
        if not labels:
            return None
        label = labels[-1]
        return label, len(getattr(label, "text", "") or "")

    @classmethod
    def _child_set_label_selection_from_endpoints(cls, child, anchor, target):
        if anchor is None or target is None:
            return False
        if hasattr(child, "_set_label_selection_from_endpoints"):
            return child._set_label_selection_from_endpoints(anchor, target)
        labels = cls._child_copyable_labels(child)
        anchor_label, anchor_pos = anchor
        target_label, target_pos = target
        if anchor_label not in labels or target_label not in labels:
            return False
        if (labels.index(target_label), target_pos) < (labels.index(anchor_label), anchor_pos):
            anchor_label, target_label = target_label, anchor_label
            anchor_pos, target_pos = target_pos, anchor_pos
        if anchor_label is target_label:
            anchor_label.select_range(anchor_pos, target_pos)
            return True
        return False

    @staticmethod
    def _child_clear_label_selections(child):
        if hasattr(child, "_clear_label_selections"):
            child._clear_label_selections()
            return
        if hasattr(child, "clear_selection"):
            child.clear_selection()

    @staticmethod
    def _child_selected_label_text(child):
        if hasattr(child, "_selected_label_text"):
            return child._selected_label_text()
        if hasattr(child, "selected_text"):
            return child.selected_text()
        return ""

    # ------------------------------------------------------------------
    # Keyboard / mouse dispatch
    # ------------------------------------------------------------------

    def handle_key(self, key):
        """Handle keyboard input.  Arrow keys and page keys scroll the view;
        mouse events are intercepted for cross-child selection; everything
        else is delegated to the parent."""
        if isinstance(key, dict) and key.get("type") == "mouse":
            return self._handle_mouse_selection(key)

        if isinstance(key, dict) and key.get("type") == "mouse_wheel":
            if "wheel_delta" in key:
                wheel_delta = int(key.get("wheel_delta") or 0)
            else:
                wheel_delta = MOUSE_WHEEL_DELTA_UP if key.get("direction") == "up" else MOUSE_WHEEL_DELTA_DOWN
            if wheel_delta == 0:
                return False
            scroll_lines = self._wheel_scroll_lines()
            return self.scroll_lines(scroll_lines * wheel_delta)

        if isinstance(key, str):
            scroll_x = getattr(getattr(self, "style", None), "scroll_x", False)
            scroll_y = getattr(getattr(self, "style", None), "scroll_y", True)
            if key == "Alt+Up":
                return self.scroll_lines(self._wheel_scroll_lines())
            elif key == "Alt+Down":
                return self.scroll_lines(-self._wheel_scroll_lines())
            elif key == "Ctrl+Up":
                return self.scroll_page_up()
            elif key == "Ctrl+Down":
                return self.scroll_page_down()
            if scroll_y:
                if key in ("ArrowUp", "Up"):
                    return self.scroll_lines(1)
                elif key in ("ArrowDown", "Down"):
                    return self.scroll_lines(-1)
                elif key == "PageUp":
                    return self.scroll_page_up()
                elif key == "PageDown":
                    return self.scroll_page_down()
                elif key == "Home":
                    return self.scroll_to_top()
                elif key == "End":
                    self.scroll_to_bottom()
                    return True
            if scroll_x:
                if key in ("ArrowLeft", "Left"):
                    return self._scroll_h(-1)
                elif key in ("ArrowRight", "Right"):
                    return self._scroll_h(1)

        return super().handle_key(key)

    def tick(self):
        """Called by the application loop when no input arrives. Performs auto-scroll and propagates to children."""
        super().tick()
        if self._autoscroll_direction is None or self._mouse_selection_anchor is None:
            return
        viewport = self._get_viewport_rect()
        if not viewport:
            return
        if self._autoscroll_direction == "up":
            previous_offset = self._view_offset
            new_offset = self._view_offset + 1
            self._view_offset = min(new_offset, self._last_natural_skip)
            self._auto_scroll = False
            row = viewport["row"]
        else:
            previous_offset = self._view_offset
            self._view_offset = max(0, self._view_offset - 1)
            if self._view_offset == 0:
                self._auto_scroll = True
            row = viewport["row"] + viewport["height"] - 1
        if self._view_offset != previous_offset:
            self.invalidate()
        endpoint = self._selection_endpoint_at(row, self._autoscroll_col, allow_outside=True)
        self._set_selection(self._mouse_selection_anchor, endpoint)

    def _handle_mouse_selection(self, event):
        """Handle mouse events for cross-child text selection."""
        viewport = self._get_viewport_rect()
        if not viewport:
            return False

        event_type = event.get("event")
        row = event.get("row", 0)
        col = event.get("col", 0)

        if event_type in ("drag", "release") and self._mouse_selection_anchor is not None:
            if event_type == "drag":
                viewport_top = viewport["row"]
                viewport_bottom = viewport["row"] + viewport["height"] - 1
                if row < viewport_top:
                    new_offset = self._view_offset + 1
                    self._view_offset = min(new_offset, self._last_natural_skip)
                    self._auto_scroll = False
                    row = viewport_top
                    self._autoscroll_direction = "up"
                    self._autoscroll_col = col
                elif row > viewport_bottom:
                    self._view_offset = max(0, self._view_offset - 1)
                    if self._view_offset == 0:
                        self._auto_scroll = True
                    row = viewport_bottom
                    self._autoscroll_direction = "down"
                    self._autoscroll_col = col
                else:
                    self._autoscroll_direction = None
            elif event_type == "release":
                self._autoscroll_direction = None

            endpoint = self._selection_endpoint_at(row, col, allow_outside=True)
            handled = self._set_selection(self._mouse_selection_anchor, endpoint)
            if event_type == "drag" and handled:
                self._copy_selection(notify=False)
            if event_type == "release":
                if handled:
                    self._copy_selection()
                self._mouse_selection_anchor = None
                self._mouse_selection_element = None
            return handled

        endpoint = self._selection_endpoint_at(row, col, allow_outside=False)
        if endpoint is None:
            if event_type == "press":
                self._clear_selections()
                self._mouse_selection_anchor = None
                self._mouse_selection_element = None
            return False

        if event_type == "press":
            self._clear_selections()
            self._mouse_selection_anchor = endpoint
            self._mouse_selection_element = endpoint[0]
            endpoint[1].select_range(endpoint[2], endpoint[2])
            return True

        return False

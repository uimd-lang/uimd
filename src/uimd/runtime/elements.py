import shutil
import subprocess
import sys
import threading
import time as _time_module

from .uielement import UIElement
from .style import Color
from .rendering import TerminalCell, terminal_cell_text

_TEXT_CLIPBOARD = ""
CLIPBOARD_COPY_TIMEOUT_SECONDS = 0.2
CLIPBOARD_COMMANDS = (
    ("pbcopy",),
    ("wl-copy",),
    ("xclip", "-selection", "clipboard"),
    ("xsel", "--clipboard", "--input"),
)
COPY_NOTIFICATION = "Copied to clipboard"
LISTBOX_DEFAULT_HEIGHT = 5
LISTBOX_WHEEL_SCROLL_ROWS = 1
TEXTINPUT_WHEEL_SCROLL_ROWS = 1
MOUSE_WHEEL_DELTA_UP = 1
MOUSE_WHEEL_DELTA_DOWN = -1
TEXT_TAB_WIDTH = 4
INFO_LABEL_GRADIENT_INTERVAL_MS = 70
INFO_LABEL_GRADIENT_STEP = 1
INFO_LABEL_GRADIENT_SEGMENT_SIZE = 1
INFO_LABEL_GRADIENT_ALPHA = ("66", "88", "aa", "cc", "ff", "cc", "aa", "88", "66")
_GRADIENT_RENDER_CONTEXT = threading.local()


class GradientRenderTime:
    def __init__(self, time_ms):
        self._time_ms = None if time_ms is None else int(time_ms)
        self._previous = None
        self._had_previous = False

    def __enter__(self):
        self._had_previous = hasattr(_GRADIENT_RENDER_CONTEXT, "time_ms")
        self._previous = getattr(_GRADIENT_RENDER_CONTEXT, "time_ms", None)
        if self._time_ms is None:
            if self._had_previous:
                delattr(_GRADIENT_RENDER_CONTEXT, "time_ms")
        else:
            _GRADIENT_RENDER_CONTEXT.time_ms = self._time_ms

    def __exit__(self, exc_type, exc, tb):
        if self._had_previous:
            _GRADIENT_RENDER_CONTEXT.time_ms = self._previous
        elif hasattr(_GRADIENT_RENDER_CONTEXT, "time_ms"):
            delattr(_GRADIENT_RENDER_CONTEXT, "time_ms")


def _normalize_insert_text(text):
    return str(text).replace("\r\n", "\n").replace("\r", "\n")


def _copy_text_to_clipboard(text):
    """Copy text to the runtime clipboard and best-effort system clipboard."""
    global _TEXT_CLIPBOARD
    _TEXT_CLIPBOARD = text or ""
    if not _TEXT_CLIPBOARD:
        return
    if not sys.stdin.isatty() or not sys.stdout.isatty():
        return

    for command in CLIPBOARD_COMMANDS:
        if shutil.which(command[0]) is None:
            continue
        try:
            subprocess.run(
                command,
                input=_TEXT_CLIPBOARD,
                text=True,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                timeout=CLIPBOARD_COPY_TIMEOUT_SECONDS,
                check=False,
            )
            return
        except (OSError, subprocess.SubprocessError):
            continue


def _mouse_wheel_delta(key):
    if isinstance(key, dict) and "wheel_delta" in key:
        return int(key.get("wheel_delta") or 0)
    if isinstance(key, dict):
        return MOUSE_WHEEL_DELTA_UP if key.get("direction") == "up" else MOUSE_WHEEL_DELTA_DOWN
    return MOUSE_WHEEL_DELTA_UP if key == "up" else MOUSE_WHEEL_DELTA_DOWN


def _notify_copied(element):
    app = getattr(element, "_app", None)
    if app is not None and hasattr(app, "notify"):
        app.notify(COPY_NOTIFICATION)


def _blend_color(fg, bg):
    """Blend fg color over bg using fg.alpha."""
    if fg is None or bg is None:
        return fg
    a = getattr(fg, 'alpha', 1.0)
    if a is None or a >= 1.0:
        return fg
    r = round(a * fg.r + (1 - a) * bg.r)
    g = round(a * fg.g + (1 - a) * bg.g)
    b = round(a * fg.b + (1 - a) * bg.b)
    return type(fg)(f"#{r:02x}{g:02x}{b:02x}")


def _overlay_background(overlay, base, parent_bg=None):
    if overlay is None:
        return base
    effective_base = base
    if effective_base is not None and getattr(effective_base, 'alpha', 1.0) < 1.0:
        effective_base = _blend_color(effective_base, parent_bg or Color("#000000"))
    a = getattr(overlay, 'alpha', 1.0)
    if a is not None and a < 1.0 and effective_base is not None and getattr(effective_base, 'r', -1) >= 0:
        if a <= 0.0:
            return effective_base
        return _blend_color(overlay, effective_base)
    return overlay


def _ansi_color(text, fg=None, bg=None, parent_bg=None, parent_fg=None):
    """Wrap text in ANSI color codes with optional alpha blending."""
    if not text:
        return ""
    if parent_bg is None:
        parent_bg = Color("#000000")

    blended_bg = bg
    if bg is not None:
        a = getattr(bg, 'alpha', 1.0)
        if a is not None and a < 1.0:
            if a == 0.0:
                blended_bg = None  # fully transparent: let terminal bg show through
            else:
                blended_bg = _blend_color(bg, parent_bg)

    blended_fg = fg
    fg_parent = blended_bg if blended_bg is not None else parent_bg
    if parent_fg is not None:
        blended_fg = parent_fg
    elif fg is not None:
        a = getattr(fg, 'alpha', 1.0)
        if a is not None and a < 1.0:
            blended_fg = _blend_color(fg, fg_parent)

    parts = []
    if blended_fg is not None and blended_fg.r >= 0:
        parts.append(f"\x1b[38;2;{blended_fg.r};{blended_fg.g};{blended_fg.b}m")
    if blended_bg is not None and blended_bg.r >= 0:
        parts.append(f"\x1b[48;2;{blended_bg.r};{blended_bg.g};{blended_bg.b}m")
    if parts:
        return "".join(parts) + text + "\x1b[0m"
    return text


def _styled_cells(text, fg=None, bg=None, parent_bg=None, parent_fg=None):
    """Return terminal cells using the same color blending as _ansi_color."""
    if not text:
        return []
    if parent_bg is None:
        parent_bg = Color("#000000")

    blended_bg = bg
    if bg is not None:
        alpha = getattr(bg, 'alpha', 1.0)
        if alpha is not None and alpha < 1.0:
            blended_bg = None if alpha == 0.0 else _blend_color(bg, parent_bg)

    blended_fg = fg
    fg_parent = blended_bg if blended_bg is not None else parent_bg
    if parent_fg is not None:
        blended_fg = parent_fg
    elif fg is not None:
        alpha = getattr(fg, 'alpha', 1.0)
        if alpha is not None and alpha < 1.0:
            blended_fg = _blend_color(fg, fg_parent)

    return [TerminalCell(ch, blended_fg, blended_bg) for ch in text]


def _ansi_code_for_color(color, is_foreground):
    if color is None:
        return None
    if isinstance(color, int):
        return str(color)
    if isinstance(color, str):
        if color == "transparent":
            return None
        if len(color) == 9 and color.isdigit():
            return f"{'38' if is_foreground else '48'};2;{int(color[0:3])};{int(color[3:6])};{int(color[6:9])}"
        return None
    if getattr(color, "r", -1) >= 0:
        return f"{'38' if is_foreground else '48'};2;{color.r};{color.g};{color.b}"
    return None


def _cells_to_ansi_compat_line(row):
    if not row:
        return ""
    parts = []
    current_fg = current_bg = None
    active_style = False
    for cell in row:
        fg = getattr(cell, "foreground", None)
        bg = getattr(cell, "background", None)
        if fg != current_fg or bg != current_bg:
            if active_style:
                parts.append("\x1b[0m")
                active_style = False
            fg_code = _ansi_code_for_color(fg, True)
            bg_code = _ansi_code_for_color(bg, False)
            if fg_code is not None:
                parts.append(f"\x1b[{fg_code}m")
                active_style = True
            if bg_code is not None:
                parts.append(f"\x1b[{bg_code}m")
                active_style = True
            current_fg = fg
            current_bg = bg
        parts.append(cell.normalized_text() if hasattr(cell, "normalized_text") else str(cell.text or " ")[:1])
    if active_style:
        parts.append("\x1b[0m")
    return "".join(parts)


def _cells_to_ansi_compat_lines(rows):
    return [_cells_to_ansi_compat_line(row) for row in rows]


def _ansi_visible_len(text):
    visible = 0
    index = 0
    while index < len(text):
        if text[index] == "\x1b" and index + 1 < len(text) and text[index + 1] == "[":
            index += 2
            while index < len(text) and text[index] != "m":
                index += 1
            if index < len(text):
                index += 1
            continue
        visible += 1
        index += 1
    return visible


def _gradient_frame(gradient):
    if not gradient:
        return 0
    interval = max(1, int(gradient.get("interval", INFO_LABEL_GRADIENT_INTERVAL_MS) or INFO_LABEL_GRADIENT_INTERVAL_MS))
    step = int(gradient.get("step", 1) or 1)
    time_ms = getattr(_GRADIENT_RENDER_CONTEXT, "time_ms", None)
    if time_ms is None:
        time_ms = _time_module.time() * 1000
    return int(time_ms // interval) * step


def _gradient_color_at(gradient, source_index):
    if not gradient:
        return None
    colors = gradient.get("colors") or []
    if not colors:
        return None
    segment_size = max(1, int(gradient.get("segment_size", 1) or 1))
    frame = _gradient_frame(gradient)
    color_index = ((int(source_index) // segment_size) - frame) % len(colors)
    return colors[color_index]


def _blend_gradient_color(overlay, base):
    if overlay is None:
        return base
    alpha = getattr(overlay, "alpha", 1.0)
    if alpha is not None and alpha <= 0.0:
        return base
    if alpha is not None and alpha < 1.0 and base is not None and getattr(base, "r", -1) >= 0:
        return _blend_color(overlay, base)
    return overlay


class _VisualTextCell:
    __slots__ = ("text", "source_start", "source_end")

    def __init__(self, text, source_start=None, source_end=None):
        self.text = text
        self.source_start = source_start
        self.source_end = source_end


class _VisualTextRow:
    __slots__ = ("start", "end", "cells")

    def __init__(self, start, end, cells):
        self.start = start
        self.end = end
        self.cells = cells

    @property
    def text(self):
        return "".join(cell.text for cell in self.cells)


def _tab_spaces_for_column(column):
    return TEXT_TAB_WIDTH - (max(0, int(column)) % TEXT_TAB_WIDTH)


def _text_needs_visual_mapping(text):
    for ch in str(text or ""):
        if ch == "\n":
            continue
        if ch == "\t" or terminal_cell_text(ch) != ch:
            return True
    return False


def _visual_cells_for_segment(segment, source_start=0, start_column=0):
    cells = []
    column = max(0, int(start_column))
    for offset, ch in enumerate(segment):
        raw_start = source_start + offset
        raw_end = raw_start + 1
        if ch == "\r":
            continue
        if ch == "\t":
            for _ in range(_tab_spaces_for_column(column)):
                cells.append(_VisualTextCell(" ", raw_start, raw_end))
                column += 1
            continue
        safe = terminal_cell_text(ch)
        cells.append(_VisualTextCell(safe, raw_start, raw_end))
        column += 1
    return cells


def _visual_cells_text(cells):
    return "".join(cell.text for cell in cells)


def _visual_text(text, start_column=0):
    return _visual_cells_text(_visual_cells_for_segment(str(text or ""), 0, start_column))


def _visual_width(text, start_column=0):
    return len(_visual_cells_for_segment(str(text or ""), 0, start_column))


def _slice_visual_cells(cells, width):
    return list(cells[:max(0, int(width))])


def _build_visual_cell_rows(text, width):
    width = max(1, int(width))
    rows = []
    abs_pos = 0
    segments = str(text or "").split("\n")
    for segment in segments:
        if not segment:
            rows.append(_VisualTextRow(abs_pos, abs_pos, []))
        else:
            cells = _visual_cells_for_segment(segment, abs_pos)
            offset = 0
            while offset < len(cells):
                remaining = len(cells) - offset
                chunk_size = min(remaining, width)
                chunk = cells[offset:offset + chunk_size]
                if chunk_size == width:
                    break_at = None
                    for index in range(len(chunk) - 1, 0, -1):
                        source = chunk[index].source_start
                        if source is not None and source < len(text) and text[source] == " ":
                            break_at = index
                            break
                    if break_at is not None:
                        chunk = chunk[:break_at]
                        offset += break_at + 1
                    else:
                        offset += chunk_size
                else:
                    offset += chunk_size
                if chunk:
                    rows.append(_VisualTextRow(chunk[0].source_start, chunk[-1].source_end, list(chunk)))
                else:
                    rows.append(_VisualTextRow(abs_pos, abs_pos, []))
        abs_pos += len(segment) + 1
    return rows or [_VisualTextRow(0, 0, [])]


def _cursor_row_idx_from_visual_rows(rows, width, cursor_pos):
    if not rows:
        return 0
    for idx, row in enumerate(rows):
        if cursor_pos <= row.end and (len(row.cells) < width or cursor_pos < row.end):
            return idx
    return len(rows) - 1


def _visual_col_for_cursor(row, cursor_pos, width):
    if cursor_pos < row.start or cursor_pos > row.end:
        return None
    for col, cell in enumerate(row.cells):
        if cell.source_start is not None and cursor_pos <= cell.source_start:
            return col
        if (
            cell.source_start is not None
            and cell.source_end is not None
            and cell.source_start < cursor_pos < cell.source_end
        ):
            return col
    if row.start <= cursor_pos <= row.end:
        return min(len(row.cells), max(0, width - 1))
    return None


def _raw_index_for_visual_col(row, col):
    col = max(0, int(col))
    if not row.cells:
        return row.start
    if col >= len(row.cells):
        return row.end
    cell = row.cells[col]
    run_start = col
    run_end = col + 1
    while (
        run_start > 0
        and row.cells[run_start - 1].source_start == cell.source_start
        and row.cells[run_start - 1].source_end == cell.source_end
    ):
        run_start -= 1
    while (
        run_end < len(row.cells)
        and row.cells[run_end].source_start == cell.source_start
        and row.cells[run_end].source_end == cell.source_end
    ):
        run_end += 1
    midpoint = run_start + ((run_end - run_start) / 2.0)
    if col >= midpoint:
        return cell.source_end if cell.source_end is not None else row.end
    return cell.source_start if cell.source_start is not None else row.start


def _align_visual_cells(cells, width, align):
    cells = _slice_visual_cells(cells, width)
    padding = max(0, int(width) - len(cells))
    if align == "center":
        left = padding // 2
    elif align == "right":
        left = padding
    else:
        left = 0
    right = padding - left
    return (
        [_VisualTextCell(" ") for _ in range(left)]
        + cells
        + [_VisualTextCell(" ") for _ in range(right)]
    )


def _align_text(text, width, align):
    """Pad/align text to width: left, center, or right."""
    cells = _align_visual_cells(_visual_cells_for_segment(str(text or "")), width, align)
    return _visual_cells_text(cells)


def _build_visual_rows(text, w):
    """Split text into visual rows of width w, respecting \\n, with word wrap."""
    rows = []
    segs = text.split('\n') if '\n' in text else [text]
    abs_pos = 0
    for seg in segs:
        if not seg:
            rows.append((abs_pos, ''))
        else:
            offset = 0
            while offset < len(seg):
                chunk = seg[offset:offset + w]
                if len(chunk) < w:
                    rows.append((abs_pos + offset, chunk))
                    offset += len(chunk)
                else:
                    last_space = chunk.rfind(' ')
                    if last_space > 0:
                        rows.append((abs_pos + offset, chunk[:last_space]))
                        offset += last_space + 1
                    else:
                        rows.append((abs_pos + offset, chunk))
                        offset += w
        abs_pos += len(seg) + 1
    return rows


def _cursor_row_idx(rows, w, cursor_pos):
    """Return the visual row index that contains cursor_pos."""
    for idx, (rstart, rtext) in enumerate(rows):
        rend = rstart + len(rtext)
        if cursor_pos <= rend and (len(rtext) < w or cursor_pos < rend):
            return idx
    return len(rows) - 1


class Label(UIElement):
    """Static text label element."""

    ELEMENT_TYPE = "label"

    def __init__(self, name="", text="", row=0, col=0, width=None, height=1):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._text = text
        self._selection_start = None
        self._selection_end = None
        self._visual_rows_cache = {}  # (width, word_wrap) -> list of rows

    @property
    def text(self):
        return self._text

    @text.setter
    def text(self, value):
        self._text = str(value)
        self._visual_rows_cache.clear()
        self._render_cache.clear()
        self._mark_layout_dirty()
        self._mark_dirty()

    def _visual_rows(self, width):
        s = self.get_style()
        do_wrap = True
        if s and hasattr(s, 'word_wrap'):
            do_wrap = s.word_wrap
        cache_key = (width, do_wrap)
        if cache_key in self._visual_rows_cache:
            return self._visual_rows_cache[cache_key]
        if _text_needs_visual_mapping(self._text):
            if (self.height or 1) == 1:
                first_line = self._text.split("\n", 1)[0] if "\n" in self._text else self._text
                result = _build_visual_cell_rows(first_line, width)[:1]
            elif do_wrap:
                result = _build_visual_cell_rows(self._text, width)
            else:
                first_line = self._text.split("\n", 1)[0] if "\n" in self._text else self._text
                result = _build_visual_cell_rows(first_line, width)[:1]
        elif (self.height or 1) == 1:
            result = [(0, self._text.split("\n", 1)[0] if "\n" in self._text else self._text)]
        elif do_wrap:
            result = _build_visual_rows(self._text, width)
        else:
            result = [(0, self._text.split("\n", 1)[0] if "\n" in self._text else self._text)]
        self._visual_rows_cache[cache_key] = result
        return result

    @staticmethod
    def _aligned_text_offset(text, width, align):
        text_width = int(text) if isinstance(text, int) else _visual_width(text)
        if align == "center":
            return max(0, (width - text_width) // 2)
        if align == "right":
            return max(0, width - text_width)
        return 0

    def _selection_bounds(self):
        if self._selection_start is None or self._selection_end is None:
            return None, None
        if self._selection_start == self._selection_end:
            return None, None
        return (
            max(0, min(self._selection_start, self._selection_end)),
            max(0, max(self._selection_start, self._selection_end)),
        )

    def select_range(self, start, end):
        text_len = len(self._text)
        self._selection_start = max(0, min(text_len, int(start)))
        self._selection_end = max(0, min(text_len, int(end)))
        self._render_cache.clear()
        self._mark_dirty()

    def clear_selection(self):
        was_selected = self._selection_start is not None or self._selection_end is not None
        self._selection_start = None
        self._selection_end = None
        if was_selected:
            self._render_cache.clear()
            self._mark_dirty()

    def selected_text(self):
        start, end = self._selection_bounds()
        if start is None:
            return ""
        return self._text[start:end]

    def copy_selection(self):
        selected = self.selected_text()
        if selected:
            _copy_text_to_clipboard(selected)
            _notify_copied(self)

    def text_position_from_point(self, rel_row, rel_col):
        width = max(1, int(self.width or len(self._text) or 1))
        rows = self._visual_rows(width)
        if not rows:
            return 0

        if int(rel_row) < 0:
            return 0
        if int(rel_row) >= len(rows):
            return len(self._text)

        row_idx = int(rel_row)
        s = self.get_style()
        align = (s.text_align if s and s.text_align else "left")
        row = rows[row_idx]
        if isinstance(row, _VisualTextRow):
            visible_cells = _slice_visual_cells(row.cells, width)
            offset = self._aligned_text_offset(len(visible_cells), width, align)
            return min(len(self._text), _raw_index_for_visual_col(
                _VisualTextRow(row.start, row.end, visible_cells),
                int(rel_col) - offset,
            ))
        row_start, raw = row
        visible = raw[:width] if width > 0 else raw
        offset = self._aligned_text_offset(visible, width, align)
        text_col = max(0, min(len(visible), int(rel_col) - offset))
        return min(len(self._text), row_start + text_col)

    def _render_visual_row(self, row_start, raw, width, align, fg, bg, cur_fg, cur_bg, parent_bg):
        return _cells_to_ansi_compat_line(self._render_visual_row_cells(
            row_start,
            raw,
            width,
            align,
            fg,
            bg,
            cur_fg,
            cur_bg,
            parent_bg,
        ))

    def _render_visual_row_cells(self, row_start, raw, width, align, fg, bg, cur_fg, cur_bg, parent_bg):
        if isinstance(raw, _VisualTextRow):
            visible_cells = _slice_visual_cells(raw.cells, width)
            aligned_cells = _align_visual_cells(visible_cells, width, align)
            offset = self._aligned_text_offset(len(visible_cells), width, align)
        else:
            visible = raw[:width] if width > 0 else raw
            visible_cells = _visual_cells_for_segment(visible, row_start)
            aligned_cells = _align_visual_cells(visible_cells, width, align)
            offset = self._aligned_text_offset(visible, width, align)
        selection_start, selection_end = self._selection_bounds()
        s = self.get_style()
        text_color_gradient = getattr(s, "text_color_gradient", None) if s else None
        text_background_gradient = getattr(s, "text_background_gradient", None) if s else None
        if selection_start is None and not text_color_gradient and not text_background_gradient:
            return _styled_cells(_visual_cells_text(aligned_cells), fg, bg, parent_bg, parent_fg=self._get_cell_color())

        cells = []
        for col, visual_cell in enumerate(aligned_cells):
            ch = visual_cell.text
            source_index = visual_cell.source_start
            char_selected = (
                selection_start is not None
                and source_index is not None
                and source_index >= selection_start
                and source_index < selection_end
            )
            if char_selected:
                cells.extend(_styled_cells(ch, cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
                continue

            char_fg = fg
            char_bg = bg
            parent_fg = self._get_cell_color()
            if source_index is not None:
                fg_overlay = _gradient_color_at(text_color_gradient, source_index)
                bg_overlay = _gradient_color_at(text_background_gradient, source_index)
                if fg_overlay is not None:
                    char_fg = _blend_gradient_color(fg_overlay, fg if fg is not None else self._get_cell_color())
                    parent_fg = None
                if bg_overlay is not None:
                    char_bg = _overlay_background(bg_overlay, bg, parent_bg)
            cells.extend(_styled_cells(ch, char_fg, char_bg, parent_bg, parent_fg=parent_fg))
        return cells

    def render_cells(self):
        lines = []
        h = self.height or 1
        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()
        align = (s.text_align if s and s.text_align else "left")
        w = self.width if self.width else len(self._text)
        cs = self.cursor_style
        cur_fg = cs.color if cs and cs.color is not None else bg
        cur_bg = cs.background if cs and cs.background is not None else fg

        for row in self._visual_rows(max(1, w)):
            if isinstance(row, _VisualTextRow):
                row_start = row.start
                raw = row
            else:
                row_start, raw = row
            lines.append(self._render_visual_row_cells(
                row_start,
                raw,
                w,
                align,
                fg,
                bg,
                cur_fg,
                cur_bg,
                parent_bg,
            ))
        while len(lines) < h:
            lines.append(_styled_cells(" " * w, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        result = lines[:h]
        return result

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())

    @staticmethod
    def _color_cache_key(color):
        if color is None:
            return None
        return (getattr(color, "hex", color), getattr(color, "alpha", None))

    @classmethod
    def _style_cache_key(cls, style):
        if style is None:
            return None
        return (
            cls._color_cache_key(getattr(style, "color", None)),
            cls._color_cache_key(getattr(style, "background", None)),
            getattr(style, "text_align", None),
            getattr(style, "word_wrap", None),
            cls._static_gradient_cache_key(getattr(style, "text_color_gradient", None)),
            cls._static_gradient_cache_key(getattr(style, "text_background_gradient", None)),
        )

    @staticmethod
    def _static_gradient_cache_key(gradient):
        if not gradient:
            return None
        return (
            gradient.get("interval"),
            gradient.get("step"),
            gradient.get("segment_size"),
            tuple(Label._color_cache_key(color) for color in gradient.get("colors", [])),
        )

    def _gradient_cache_key(self, gradient):
        if not gradient:
            return None
        return (self._static_gradient_cache_key(gradient), _gradient_frame(gradient))

    def render_line(self, line_idx):
        rendered = self.render()
        if 0 <= line_idx < len(rendered):
            return rendered[line_idx]
        return ""

    def tick(self):
        s = self.get_style()
        gradients = [
            gradient for gradient in (
                getattr(s, "text_color_gradient", None) if s else None,
                getattr(s, "text_background_gradient", None) if s else None,
            )
            if gradient
        ]
        if not gradients:
            return
        interval = min(max(1, int(gradient.get("interval", INFO_LABEL_GRADIENT_INTERVAL_MS) or INFO_LABEL_GRADIENT_INTERVAL_MS)) for gradient in gradients)
        now = _time_module.time()
        if hasattr(self, "_last_text_gradient_tick"):
            if now - self._last_text_gradient_tick < interval / 1000.0:
                return
        self._last_text_gradient_tick = now
        self._render_cache.clear()
        self._mark_dirty()


class FrameBufferView(UIElement):
    """2D terminal-cell framebuffer rendered through the normal UI pipeline."""

    ELEMENT_TYPE = "framebufferview"

    _CORNER_TL = "┌"
    _CORNER_TR = "┐"
    _CORNER_BL = "└"
    _CORNER_BR = "┘"

    def __init__(self, name="", row=0, col=0, width=None, height=1):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._frames = {}
        self._target_order = []
        self._regions = {}
        self._pan_row = 0
        self._pan_col = 0
        self._dimmed = False
        self._panel_focused = False
        self._panel_edit = False

    def clear_frames(self):
        self._frames = {}
        self._target_order = []
        self._regions = {}
        self._render_cache.clear()
        self._mark_dirty()

    def set_frames(self, frames, target_order=None, regions=None):
        self._frames = dict(frames or {})
        self._target_order = list(target_order or self._frames.keys())
        self._regions = {str(name): dict(region) for name, region in (regions or {}).items()}
        self._render_cache.clear()
        self._mark_dirty()

    def set_pan(self, row=0, col=0):
        row = max(0, int(row or 0))
        col = max(0, int(col or 0))
        if row == self._pan_row and col == self._pan_col:
            return
        self._pan_row = row
        self._pan_col = col
        self._render_cache.clear()
        self._mark_dirty()

    def set_dimmed(self, dimmed):
        dimmed = bool(dimmed)
        if dimmed == self._dimmed:
            return
        self._dimmed = dimmed
        self._render_cache.clear()
        self._mark_dirty()

    def set_interaction_state(self, focused=False, edit=False):
        focused = bool(focused)
        edit = bool(edit)
        if focused == self._panel_focused and edit == self._panel_edit:
            return
        self._panel_focused = focused
        self._panel_edit = edit
        self._render_cache.clear()
        self._mark_dirty()

    @staticmethod
    def _cell_parts(cell):
        if isinstance(cell, dict):
            return (
                str(cell.get("text", cell.get("char", " ")) or " "),
                cell.get("fg", cell.get("foreground")),
                cell.get("bg", cell.get("background")),
            )
        if isinstance(cell, (list, tuple)):
            text = str(cell[0] if len(cell) > 0 and cell[0] is not None else " ")
            fg = cell[1] if len(cell) > 1 else None
            bg = cell[2] if len(cell) > 2 else None
            return text, fg, bg
        return str(cell or " "), None, None

    @staticmethod
    def _color(value):
        if not value:
            return None
        if isinstance(value, Color):
            return value
        try:
            return Color(str(value))
        except Exception:
            return None

    @staticmethod
    def _dim_hex_color(value, dim_alpha=0.4):
        if not value or not isinstance(value, str) or len(value) != 7 or not value.startswith("#"):
            return value
        try:
            r = round((1.0 - dim_alpha) * int(value[1:3], 16))
            g = round((1.0 - dim_alpha) * int(value[3:5], 16))
            b = round((1.0 - dim_alpha) * int(value[5:7], 16))
            return f"#{r:02x}{g:02x}{b:02x}"
        except (ValueError, IndexError):
            return value

    @staticmethod
    def _split_dimension(size, parts):
        size = max(1, int(size))
        parts = max(1, min(int(parts), size))
        base = size // parts
        extra = size % parts
        return [base + (1 if index < extra else 0) for index in range(parts)]

    def _default_regions(self, width, height):
        count = max(1, len(self._target_order))
        widths = self._split_dimension(width, count)
        regions = {}
        col = 0
        for index, name in enumerate(self._target_order):
            regions[name] = {"row": 0, "col": col, "width": widths[index], "height": height}
            col += widths[index]
        return regions

    def _overlay_corners(self, grid, regions, style):
        if not (self._panel_focused or self._panel_edit):
            return
        state_style = self.edit_style if self._panel_edit else self.focus_style
        color = getattr(state_style, "color", None) if state_style is not None else None
        if color is None:
            color = getattr(style, "color", None)
        if color is None:
            color = Color("#22c55e" if self._panel_edit else "#facc15")
        for name in self._target_order:
            region = regions.get(name)
            if not region:
                continue
            top = int(region.get("row", 0))
            left = int(region.get("col", 0))
            width = int(region.get("width", 0))
            height = int(region.get("height", 0))
            if width < 2 or height < 2:
                continue
            points = (
                (top, left, self._CORNER_TL),
                (top, left + 1, "─"),
                (top, left + width - 2, "─"),
                (top, left + width - 1, self._CORNER_TR),
                (top + height - 1, left, self._CORNER_BL),
                (top + height - 1, left + 1, "─"),
                (top + height - 1, left + width - 2, "─"),
                (top + height - 1, left + width - 1, self._CORNER_BR),
            )
            for row, col, char in points:
                if 0 <= row < len(grid) and 0 <= col < len(grid[row]):
                    _text, _fg, bg = grid[row][col]
                    grid[row][col] = (char, color, bg)

    def render_cells(self):
        width = max(1, int(self.width or 1))
        height = max(1, int(self.height or 1))
        style = self.get_style()
        fg = getattr(style, "color", None) if style is not None else None
        bg = getattr(style, "background", None) if style is not None else None

        regions = self._regions or self._default_regions(width, height)
        grid = [[(" ", fg, bg) for _ in range(width)] for _ in range(height)]
        for name in self._target_order:
            region = regions.get(name)
            if not region:
                continue
            frame = self._frames.get(name) or {}
            cells = frame.get("cells", []) if isinstance(frame, dict) else frame
            top = max(0, int(region.get("row", 0)))
            left = max(0, int(region.get("col", 0)))
            region_width = max(0, int(region.get("width", 0)))
            region_height = max(0, int(region.get("height", 0)))
            for local_row in range(region_height):
                out_row = top + local_row
                if out_row < 0 or out_row >= height:
                    continue
                source_row = self._pan_row + local_row
                row_cells = cells[source_row] if source_row < len(cells) else []
                for local_col in range(region_width):
                    out_col = left + local_col
                    if out_col < 0 or out_col >= width:
                        continue
                    source_col = self._pan_col + local_col
                    if source_col < len(row_cells):
                        text, cell_fg, cell_bg = self._cell_parts(row_cells[source_col])
                        if self._dimmed:
                            cell_fg = self._dim_hex_color(cell_fg)
                            cell_bg = self._dim_hex_color(cell_bg)
                        grid[out_row][out_col] = (
                            (text or " ")[:1],
                            self._color(cell_fg) or fg,
                            self._color(cell_bg) or bg,
                        )
        self._overlay_corners(grid, regions, style)
        return [
            [TerminalCell(text, cell_fg, cell_bg) for text, cell_fg, cell_bg in row]
            for row in grid
        ]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class InfoLabel(Label):
    """Compatibility alias for the old animated informational label."""
    ELEMENT_TYPE = "infolabel"

    def __init__(self, name="", text="", row=0, col=0, width=None, height=1):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        if self.style.text_color_gradient is None:
            self.style.text_color_gradient = {
                "interval": INFO_LABEL_GRADIENT_INTERVAL_MS,
                "step": INFO_LABEL_GRADIENT_STEP,
                "segment_size": INFO_LABEL_GRADIENT_SEGMENT_SIZE,
                "colors": [Color(f"#ffffff{alpha}") for alpha in INFO_LABEL_GRADIENT_ALPHA],
            }


class SpanLabel(Label):
    """Label that renders text in multiple colors via (text, fg[, bg]) span tuples."""

    ELEMENT_TYPE = "spanlabel"

    def __init__(self, name="", text="", row=0, col=0, width=None, height=1):
        super().__init__(name=name, text=text, row=row, col=col, width=width, height=height)
        self._char_fg = []
        self._char_bg = []

    @staticmethod
    def _to_color(value):
        if value is None or isinstance(value, Color):
            return value
        return Color(str(value))

    def set_spans(self, spans):
        """Set content from a list of (text, fg) or (text, fg, bg) tuples."""
        texts, fg_list, bg_list = [], [], []
        for span in spans:
            t = str(span[0])
            fg = self._to_color(span[1] if len(span) > 1 else None)
            bg = self._to_color(span[2] if len(span) > 2 else None)
            texts.append(t)
            for _ in t:
                fg_list.append(fg)
                bg_list.append(bg)
        self._text = "".join(texts)
        self._char_fg = fg_list
        self._char_bg = bg_list
        self._visual_rows_cache.clear()
        self._render_cache.clear()
        self._mark_layout_dirty()
        self._mark_dirty()

    def _visual_rows(self, width):
        text = self._text
        if not text:
            return [(0, "")]
        rows = []
        pos = 0
        for line in text.split("\n"):
            rows.append((pos, line))
            pos += len(line) + 1
        if (self.height or 1) == 1:
            return rows[:1]
        return rows

    def render_cells(self):
        lines = []
        h = self.height or 1
        s = self.get_style()
        default_fg = s.color if s else None
        default_bg = s.background if s else None
        parent_bg = self._get_cell_background()
        parent_fg = self._get_cell_color()
        w = max(1, self.width if self.width else len(self._text))
        cs = self.cursor_style
        effective_bg = default_bg if default_bg is not None else parent_bg
        cur_fg = cs.color if cs and cs.color is not None else effective_bg
        cur_bg = cs.background if cs and cs.background is not None else default_fg
        sel_start, sel_end = self._selection_bounds()

        for row_start, raw in self._visual_rows(w):
            lines.append(self._render_span_row_cells(
                row_start, raw, w,
                default_fg, default_bg, parent_bg, parent_fg,
                cur_fg, cur_bg, sel_start, sel_end,
            ))

        while len(lines) < h:
            lines.append(_styled_cells(" " * w, default_fg, default_bg, parent_bg, parent_fg=parent_fg))
        return lines[:h]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())

    def _render_span_row(self, row_start, raw, width,
                         default_fg, default_bg, parent_bg, parent_fg,
                         cur_fg=None, cur_bg=None, sel_start=None, sel_end=None):
        return _cells_to_ansi_compat_line(self._render_span_row_cells(
            row_start, raw, width,
            default_fg, default_bg, parent_bg, parent_fg,
            cur_fg, cur_bg, sel_start, sel_end,
        ))

    def _render_span_row_cells(self, row_start, raw, width,
                               default_fg, default_bg, parent_bg, parent_fg,
                               cur_fg=None, cur_bg=None, sel_start=None, sel_end=None):
        visible_cells = _slice_visual_cells(_visual_cells_for_segment(raw, row_start), width)
        result = []
        segment = ""
        seg_fg = seg_bg = None
        seg_has_span = False
        first = True

        for visual_cell in visible_cells:
            ch = visual_cell.text
            src = visual_cell.source_start if visual_cell.source_start is not None else row_start
            selected = (sel_start is not None and sel_start <= src < sel_end)
            if selected:
                sel_fg = cur_fg if cur_fg is not None else (parent_fg if parent_fg is not None else default_fg)
                actual_fg, actual_bg, has_span = sel_fg, cur_bg, True
            else:
                char_fg = self._char_fg[src] if self._char_fg and src < len(self._char_fg) else None
                char_bg = self._char_bg[src] if self._char_bg and src < len(self._char_bg) else None
                has_span = char_fg is not None or char_bg is not None
                actual_fg = char_fg if char_fg is not None else default_fg
                actual_bg = char_bg if char_bg is not None else default_bg

            if first:
                seg_fg, seg_bg, seg_has_span = actual_fg, actual_bg, has_span
                segment = ch
                first = False
            elif actual_fg == seg_fg and actual_bg == seg_bg and has_span == seg_has_span:
                segment += ch
            else:
                pf = None if seg_has_span else parent_fg
                result.extend(_styled_cells(segment, seg_fg, seg_bg, parent_bg, parent_fg=pf))
                segment = ch
                seg_fg, seg_bg, seg_has_span = actual_fg, actual_bg, has_span

        if segment:
            pf = None if seg_has_span else parent_fg
            result.extend(_styled_cells(segment, seg_fg, seg_bg, parent_bg, parent_fg=pf))

        pad = width - len(visible_cells)
        if pad > 0:
            result.extend(_styled_cells(" " * pad, default_fg, default_bg, parent_bg, parent_fg=parent_fg))
        return result


class TextInput(UIElement):
    """Text input element with editing capabilities."""

    ELEMENT_TYPE = "textinput"
    MULTILINE = False

    def __init__(self, name="", value="", row=0, col=0, width=None, height=1,
                 maxlength=None, password=False, format_regex=None):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._value = self._normalize_value(value)
        self.cursor_pos = len(self._value)
        self.maxlength = maxlength
        self.password = password
        self.format_regex = format_regex
        self._selection_start = None
        self._scroll_offset = 0
        self._manual_scroll_away_from_cursor = False

    def _normalize_value(self, value):
        normalized = _normalize_insert_text(value) if value else ""
        if self.MULTILINE:
            return normalized
        return normalized.replace("\n", " ")

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, v):
        self._value = self._normalize_value(v)
        self._manual_scroll_away_from_cursor = False
        if self.cursor_pos > len(self._value):
            self.cursor_pos = len(self._value)
            self._scroll_offset = 0
        self._mark_layout_dirty()
        self._mark_dirty()

    @property
    def error(self):
        if self.format_regex and self._value:
            import re
            try:
                return not bool(re.match(self.format_regex, self._value))
            except re.error:
                return False
        return False

    def _delete_selection(self):
        start = min(self._selection_start, self.cursor_pos)
        end = max(self._selection_start, self.cursor_pos)
        self._value = self._value[:start] + self._value[end:]
        self.cursor_pos = start
        self._selection_start = None
        self._mark_dirty()

    def _selected_text(self):
        if self._selection_start is None or self._selection_start == self.cursor_pos:
            return ""
        start = min(self._selection_start, self.cursor_pos)
        end = max(self._selection_start, self.cursor_pos)
        return self._value[start:end]

    def handle_key(self, key):
        global _TEXT_CLIPBOARD
        has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos

        if isinstance(key, dict) and key.get("type") == "mouse_wheel":
            return self._handle_mouse_wheel(key)

        self._manual_scroll_away_from_cursor = False

        if key == "Shift+Left":
            if self._selection_start is None:
                self._selection_start = self.cursor_pos
            self.cursor_pos = max(0, self.cursor_pos - 1)
            return True
        if key == "Shift+Right":
            if self._selection_start is None:
                self._selection_start = self.cursor_pos
            self.cursor_pos = min(len(self._value), self.cursor_pos + 1)
            return True
        if key == "Shift+Home":
            if self._selection_start is None:
                self._selection_start = self.cursor_pos
            self.cursor_pos = 0
            return True
        if key == "Shift+End":
            if self._selection_start is None:
                self._selection_start = self.cursor_pos
            self.cursor_pos = len(self._value)
            return True
        if key == "Left":
            if has_sel:
                self.cursor_pos = min(self._selection_start, self.cursor_pos)
                self._selection_start = None
            else:
                self.cursor_pos = max(0, self.cursor_pos - 1)
                self._selection_start = None
            return True
        if key == "Right":
            if has_sel:
                self.cursor_pos = max(self._selection_start, self.cursor_pos)
                self._selection_start = None
            else:
                self.cursor_pos = min(len(self._value), self.cursor_pos + 1)
                self._selection_start = None
            return True
        if key == "Alt+Enter":
            if not self.MULTILINE:
                return False
            self._value = self._value[:self.cursor_pos] + '\n' + self._value[self.cursor_pos:]
            self.cursor_pos += 1
            self._selection_start = None
            self._mark_dirty()
            return True
        if key == "Alt+Left":
            pos = self.cursor_pos
            while pos > 0 and not (self._value[pos - 1].isalnum() or self._value[pos - 1] == '_'):
                pos -= 1
            while pos > 0 and (self._value[pos - 1].isalnum() or self._value[pos - 1] == '_'):
                pos -= 1
            self.cursor_pos = pos
            self._selection_start = None
            return True
        if key == "Alt+Right":
            pos = self.cursor_pos
            n = len(self._value)
            while pos < n and (self._value[pos].isalnum() or self._value[pos] == '_'):
                pos += 1
            while pos < n and not (self._value[pos].isalnum() or self._value[pos] == '_'):
                pos += 1
            self.cursor_pos = pos
            self._selection_start = None
            return True
        if key == "Shift+Up":
            if self.MULTILINE and (self.height or 1) > 1:
                _, rows, idx = self._visual_nav_context()
                if idx == 0:
                    return True
                if self._selection_start is None:
                    self._selection_start = self.cursor_pos
                cur_col = self._visual_col_for_row_cursor(rows[idx], self.width or len(self._value) + 1)
                self.cursor_pos = self._raw_index_for_row_col(rows[idx - 1], cur_col)
                return True
            return False
        if key == "Shift+Down":
            if self.MULTILINE and (self.height or 1) > 1:
                _, rows, idx = self._visual_nav_context()
                if idx == len(rows) - 1:
                    return True
                if self._selection_start is None:
                    self._selection_start = self.cursor_pos
                cur_col = self._visual_col_for_row_cursor(rows[idx], self.width or len(self._value) + 1)
                self.cursor_pos = self._raw_index_for_row_col(rows[idx + 1], cur_col)
                return True
            return False
        if key == "Up":
            if self.MULTILINE and (self.height or 1) > 1:
                _, rows, idx = self._visual_nav_context()
                if idx == 0:
                    return False
                cur_col = self._visual_col_for_row_cursor(rows[idx], self.width or len(self._value) + 1)
                self.cursor_pos = self._raw_index_for_row_col(rows[idx - 1], cur_col)
                self._selection_start = None
                return True
            return False
        if key == "Down":
            if self.MULTILINE and (self.height or 1) > 1:
                _, rows, idx = self._visual_nav_context()
                if idx == len(rows) - 1:
                    return False
                cur_col = self._visual_col_for_row_cursor(rows[idx], self.width or len(self._value) + 1)
                self.cursor_pos = self._raw_index_for_row_col(rows[idx + 1], cur_col)
                self._selection_start = None
                return True
            return False
        if key == "Home":
            self.cursor_pos = 0
            self._selection_start = None
            return True
        if key == "End":
            self.cursor_pos = len(self._value)
            self._selection_start = None
            return True
        if key == "Backspace":
            if has_sel:
                self._delete_selection()
            elif self.cursor_pos > 0:
                self._value = self._value[:self.cursor_pos - 1] + self._value[self.cursor_pos:]
                self.cursor_pos -= 1
            return True
        if key == "Delete":
            if has_sel:
                self._delete_selection()
            elif self.cursor_pos < len(self._value):
                self._value = self._value[:self.cursor_pos] + self._value[self.cursor_pos + 1:]
            return True
        if key == "cmd_c":
            _copy_text_to_clipboard(self._selected_text() if has_sel else self._value)
            return True
        if key == "cmd_v":
            if _TEXT_CLIPBOARD:
                self._insert_text(_TEXT_CLIPBOARD)
            return True
        if isinstance(key, dict) and key.get("type") == "paste":
            self._insert_text(key.get("text", ""))
            return True
        if key == "Enter" and self.MULTILINE:
            self._insert_text("\n")
            return True
        if isinstance(key, str) and len(key) == 1:
            if self.maxlength and len(self._value) >= self.maxlength:
                return True
            if has_sel:
                self._delete_selection()
            self._value = self._value[:self.cursor_pos] + key + self._value[self.cursor_pos:]
            self.cursor_pos += 1
            return True
        return False

    def _handle_mouse_wheel(self, event):
        wheel_delta = _mouse_wheel_delta(event)
        if wheel_delta == 0:
            return False
        return self.scroll_by_rows(-wheel_delta * TEXTINPUT_WHEEL_SCROLL_ROWS, manual=True)

    def scroll_by_rows(self, delta, manual=True):
        if not self.MULTILINE:
            return False
        h = self.height or 1
        if h <= 1:
            return False
        w = self.width or len(self._value) + 1
        rows = _build_visual_cell_rows(self._value, w) if _text_needs_visual_mapping(self._value) else _build_visual_rows(self._value, w)
        max_offset = max(0, len(rows) - h)
        if max_offset <= 0:
            return False
        next_offset = max(0, min(max_offset, self._scroll_offset + int(delta)))
        if next_offset == self._scroll_offset:
            return False
        self._scroll_offset = next_offset
        self._manual_scroll_away_from_cursor = bool(manual)
        self._mark_dirty()
        return True

    def _insert_text(self, insert_text):
        if not insert_text:
            return
        insert_text = self._normalize_value(insert_text)
        has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos
        if self.maxlength:
            selected_len = len(self._selected_text()) if has_sel else 0
            available = max(0, self.maxlength - (len(self._value) - selected_len))
            insert_text = insert_text[:available]
        if has_sel:
            self._delete_selection()
        self._value = self._value[:self.cursor_pos] + insert_text + self._value[self.cursor_pos:]
        self.cursor_pos += len(insert_text)
        self._mark_dirty()

    def select_range(self, start, end):
        self._manual_scroll_away_from_cursor = False
        self._selection_start = start
        self.cursor_pos = end

    def _visual_nav_context(self):
        w = self.width or len(self._value) + 1
        if _text_needs_visual_mapping(self._value):
            rows = _build_visual_cell_rows(self._value, w)
            idx = _cursor_row_idx_from_visual_rows(rows, w, self.cursor_pos)
        else:
            rows = _build_visual_rows(self._value, w)
            idx = _cursor_row_idx(rows, w, self.cursor_pos)
        return w, rows, idx

    def _visual_col_for_row_cursor(self, row, width):
        if isinstance(row, _VisualTextRow):
            col = _visual_col_for_cursor(row, self.cursor_pos, width)
            return 0 if col is None else col
        return self.cursor_pos - row[0]

    @staticmethod
    def _raw_index_for_row_col(row, col):
        if isinstance(row, _VisualTextRow):
            return _raw_index_for_visual_col(row, col)
        rstart, rtext = row
        return rstart + min(col, len(rtext))

    def cursor_position_from_point(self, rel_row, rel_col):
        width = max(1, int(self.width or len(self._value) + 1))
        rel_row = max(0, int(rel_row))
        rel_col = max(0, int(rel_col))
        if self.MULTILINE and (self.height or 1) > 1:
            if _text_needs_visual_mapping(self._value):
                rows = _build_visual_cell_rows(self._value, width)
                row_idx = min(rel_row + int(getattr(self, "_scroll_offset", 0)), len(rows) - 1)
                return min(len(self._value), _raw_index_for_visual_col(rows[row_idx], rel_col))
            rows = _build_visual_rows(self._value, width)
            row_idx = min(rel_row + int(getattr(self, "_scroll_offset", 0)), len(rows) - 1)
            rstart, rtext = rows[row_idx]
            return min(len(self._value), rstart + min(rel_col, len(rtext)))
        if _text_needs_visual_mapping(self._value):
            row = _VisualTextRow(0, len(self._value), _visual_cells_for_segment(self._value))
            visible = _VisualTextRow(row.start, row.end, row.cells[int(getattr(self, "_scroll_offset", 0)):])
            return min(len(self._value), _raw_index_for_visual_col(visible, rel_col))
        return min(len(self._value), int(getattr(self, "_scroll_offset", 0)) + rel_col)

    def copy_selection(self):
        if self._selected_text():
            _copy_text_to_clipboard(self._selected_text())
            _notify_copied(self)

    def _render_edit_line(self, text_padded, w, cur_col, sel_a, sel_b,
                          fg, bg, cur_fg, cur_bg, parent_bg):
        return _cells_to_ansi_compat_line(self._render_edit_line_cells(
            text_padded, w, cur_col, sel_a, sel_b,
            fg, bg, cur_fg, cur_bg, parent_bg,
        ))

    def _render_edit_line_cells(self, text_padded, w, cur_col, sel_a, sel_b,
                                fg, bg, cur_fg, cur_bg, parent_bg):
        """Render one text row with cursor or selection highlight."""
        if sel_a is not None and sel_a < sel_b:
            line = []
            if sel_a > 0:
                line.extend(_styled_cells(text_padded[:sel_a], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            line.extend(_styled_cells(text_padded[sel_a:sel_b], cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
            if sel_b < w:
                line.extend(_styled_cells(text_padded[sel_b:], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            return line
        if cur_col is not None and 0 <= cur_col < w:
            line = []
            if cur_col > 0:
                line.extend(_styled_cells(text_padded[:cur_col], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            line.extend(_styled_cells(text_padded[cur_col:cur_col + 1] or " ", cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
            if cur_col + 1 < w:
                line.extend(_styled_cells(text_padded[cur_col + 1:], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            return line
        return _styled_cells(text_padded, fg, bg, parent_bg, parent_fg=self._get_cell_color())

    def _render_mapped_row_cells(self, row, width, fg, bg, cur_fg, cur_bg, parent_bg,
                                 cur_col=None, sel_lo=None, sel_hi=None, indicator=None):
        visual_cells = _slice_visual_cells(row.cells, width)
        while len(visual_cells) < width:
            visual_cells.append(_VisualTextCell(" "))
        visual_cells = visual_cells[:width]
        if indicator and width > 0 and len(row.cells) < width:
            visual_cells[width - 1] = _VisualTextCell(indicator)

        rendered = []
        has_selection = sel_lo is not None and sel_hi is not None
        for col, visual_cell in enumerate(visual_cells):
            selected = (
                has_selection
                and visual_cell.source_start is not None
                and sel_lo <= visual_cell.source_start < sel_hi
            )
            if selected or (not has_selection and cur_col is not None and col == cur_col):
                rendered.extend(_styled_cells(visual_cell.text, cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
            else:
                rendered.extend(_styled_cells(visual_cell.text, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        return rendered

    def _render_mapped_cells(self, display_value, height, width, fg, bg, cur_fg, cur_bg, parent_bg):
        lines = []
        if height == 1:
            row = _VisualTextRow(0, len(display_value), _visual_cells_for_segment(display_value))
            total_width = len(row.cells)
            keep_scroll_start = bool(getattr(self, "_force_scroll_start", False))
            cursor_col = _visual_col_for_cursor(row, self.cursor_pos, max(width, total_width + 1))
            cursor_col = total_width if cursor_col is None else cursor_col
            if keep_scroll_start or total_width <= width:
                self._scroll_offset = 0
            if self._edit_mode and not keep_scroll_start:
                if cursor_col < self._scroll_offset:
                    self._scroll_offset = cursor_col
                elif cursor_col >= self._scroll_offset + width:
                    self._scroll_offset = max(0, cursor_col - width + 1)
            scroll = int(self._scroll_offset)
            visible_cells = row.cells[scroll:scroll + width]
            visible_row = _VisualTextRow(row.start, row.end, visible_cells)
            indicator = None
            if not self._edit_mode and total_width > scroll + width and width > 0:
                indicator = ">"
            cur_col = None
            sel_lo = sel_hi = None
            if self._edit_mode:
                cur_col = max(0, min(width - 1, cursor_col - scroll))
                has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos
                if has_sel:
                    sel_lo = min(self._selection_start, self.cursor_pos)
                    sel_hi = max(self._selection_start, self.cursor_pos)
            lines.append(self._render_mapped_row_cells(
                visible_row, width, fg, bg, cur_fg, cur_bg, parent_bg,
                cur_col=cur_col, sel_lo=sel_lo, sel_hi=sel_hi, indicator=indicator,
            ))
            return lines

        visual_rows = _build_visual_cell_rows(display_value, width)
        max_offset = max(0, len(visual_rows) - height)
        self._scroll_offset = max(0, min(int(self._scroll_offset), max_offset))
        if self._edit_mode and not self._manual_scroll_away_from_cursor:
            cursor_row_idx = _cursor_row_idx_from_visual_rows(visual_rows, width, self.cursor_pos)
            if cursor_row_idx < self._scroll_offset:
                self._scroll_offset = cursor_row_idx
            elif cursor_row_idx >= self._scroll_offset + height:
                self._scroll_offset = max(0, cursor_row_idx - height + 1)

        has_sel = (
            self._edit_mode
            and self._selection_start is not None
            and self._selection_start != self.cursor_pos
        )
        sel_lo = min(self._selection_start, self.cursor_pos) if has_sel else None
        sel_hi = max(self._selection_start, self.cursor_pos) if has_sel else None
        scroll = int(self._scroll_offset)
        has_above = scroll > 0
        has_below = (scroll + height) < len(visual_rows)
        for i in range(height):
            row_idx = i + scroll
            if row_idx >= len(visual_rows):
                lines.append(_styled_cells(" " * width, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
                continue
            row = visual_rows[row_idx]
            indicator = None
            if i == 0 and has_above:
                indicator = "^"
            elif i == height - 1 and has_below:
                indicator = "v"
            cur_col = None
            if self._edit_mode:
                cur_col = _visual_col_for_cursor(row, self.cursor_pos, width)
                if cur_col is not None:
                    cur_col = max(0, min(width - 1, cur_col))
            if indicator and cur_col == width - 1:
                indicator = None
            lines.append(self._render_mapped_row_cells(
                row, width, fg, bg, cur_fg, cur_bg, parent_bg,
                cur_col=cur_col, sel_lo=sel_lo, sel_hi=sel_hi, indicator=indicator,
            ))
        return lines

    def render_cells(self):
        display_value = "*" * len(self._value) if self.password else self._value
        lines = []
        h = (self.height or 1) if self.MULTILINE else 1
        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()
        w = self.width if self.width else len(display_value) + 1

        cs = self.cursor_style
        cur_fg = cs.color if cs and cs.color is not None else bg
        cur_bg = cs.background if cs and cs.background is not None else fg

        if not self.password and _text_needs_visual_mapping(display_value):
            lines = self._render_mapped_cells(display_value, h, w, fg, bg, cur_fg, cur_bg, parent_bg)
            while len(lines) < h:
                lines.append(_styled_cells(" " * w, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            return lines[:h]

        if h == 1:
            keep_scroll_start = bool(getattr(self, "_force_scroll_start", False))
            if keep_scroll_start or len(display_value) <= w:
                self._scroll_offset = 0
            if self._edit_mode and not keep_scroll_start:
                if self.cursor_pos < self._scroll_offset:
                    self._scroll_offset = self.cursor_pos
                elif self.cursor_pos >= self._scroll_offset + w:
                    self._scroll_offset = max(0, self.cursor_pos - w + 1)

            visible = display_value[self._scroll_offset:self._scroll_offset + w]

            if not self._edit_mode:
                if len(display_value) > self._scroll_offset + w and w > 0:
                    visible = visible[:max(0, w - 1)] + ">"
                lines.append(_styled_cells(visible.ljust(w), fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            else:
                cur_vis = max(0, min(self.cursor_pos - self._scroll_offset, w - 1))
                has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos
                sel_a = sel_b = None
                if has_sel:
                    raw_a = min(self._selection_start, self.cursor_pos) - self._scroll_offset
                    raw_b = max(self._selection_start, self.cursor_pos) - self._scroll_offset
                    sel_a = max(0, raw_a)
                    sel_b = min(w, raw_b)
                text = visible.ljust(w)[:w]
                lines.append(self._render_edit_line_cells(text, w, cur_vis, sel_a, sel_b,
                                                          fg, bg, cur_fg, cur_bg, parent_bg))
        else:
            visual_rows = _build_visual_rows(display_value, w)

            max_offset = max(0, len(visual_rows) - h)
            self._scroll_offset = max(0, min(int(self._scroll_offset), max_offset))
            if self._edit_mode and not self._manual_scroll_away_from_cursor:
                cursor_row_idx = _cursor_row_idx(visual_rows, w, self.cursor_pos)
                if cursor_row_idx < self._scroll_offset:
                    self._scroll_offset = cursor_row_idx
                elif cursor_row_idx >= self._scroll_offset + h:
                    self._scroll_offset = max(0, cursor_row_idx - h + 1)

            has_sel = (self._edit_mode and self._selection_start is not None
                       and self._selection_start != self.cursor_pos)
            sel_lo = min(self._selection_start, self.cursor_pos) if has_sel else None
            sel_hi = max(self._selection_start, self.cursor_pos) if has_sel else None

            scroll = self._scroll_offset
            has_above = scroll > 0
            has_below = (scroll + h) < len(visual_rows)
            for i in range(h):
                row_idx = i + scroll
                if row_idx >= len(visual_rows):
                    lines.append(_styled_cells(' ' * w, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
                    continue

                rstart, rtext = visual_rows[row_idx]
                rend = rstart + len(rtext)
                text_padded = rtext.ljust(w)[:w]
                indicator = None
                if i == 0 and has_above:
                    indicator = "^"
                elif i == h - 1 and has_below:
                    indicator = "v"

                if not self._edit_mode:
                    if indicator and len(rtext) < w:
                        text_padded = text_padded[:w - 1] + indicator
                    lines.append(_styled_cells(text_padded, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
                    continue

                cur_col = None
                if rstart <= self.cursor_pos < rend:
                    cur_col = self.cursor_pos - rstart
                elif self.cursor_pos == rend:
                    if len(rtext) < w:
                        cur_col = len(rtext)
                    elif row_idx == len(visual_rows) - 1:
                        cur_col = w - 1

                row_sel_a = row_sel_b = None
                if has_sel and sel_lo < rend and sel_hi > rstart:
                    row_sel_a = max(0, sel_lo - rstart)
                    row_sel_b = min(len(rtext), sel_hi - rstart)

                indicator_overlaps_cursor = cur_col == w - 1
                indicator_overlaps_selection = (
                    row_sel_a is not None
                    and row_sel_b is not None
                    and row_sel_a <= w - 1 < row_sel_b
                )
                if (
                    indicator
                    and len(rtext) < w
                    and not indicator_overlaps_cursor
                    and not indicator_overlaps_selection
                ):
                    text_padded = text_padded[:w - 1] + indicator

                lines.append(self._render_edit_line_cells(text_padded, w, cur_col, row_sel_a, row_sel_b,
                                                          fg, bg, cur_fg, cur_bg, parent_bg))

        while len(lines) < h:
            lines.append(_styled_cells(" " * w, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        return lines[:h]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())

    def render_line(self, line_idx):
        rendered = self.render()
        if 0 <= line_idx < len(rendered):
            return rendered[line_idx]
        return ""


class TextArea(TextInput):
    """Multiline text editor element."""

    ELEMENT_TYPE = "textarea"
    MULTILINE = True

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.commit_mode = "leave"


class NumberInput(UIElement):
    """Numeric input element with up/down arrow support."""

    ELEMENT_TYPE = "numberinput"

    def __init__(self, name="", value=0.0, row=0, col=0, width=None, height=1,
                 step_size=1.0, min_value=None, max_value=None, format_str="0"):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._value = float(value)
        self.step_size = step_size
        self.min_value = min_value
        self.max_value = max_value
        self.format_str = format_str
        self.cursor_pos = len(self.display_value)
        self._selection_start = None
        self._input_text = None

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, v):
        self._value = float(v)
        self._input_text = None
        self.cursor_pos = min(self.cursor_pos, len(self.display_value))
        self._mark_layout_dirty()
        self._mark_dirty()

    @property
    def display_value(self):
        if self._edit_mode and self._input_text is not None:
            return self._input_text
        return self._formatted_value()

    def _formatted_value(self):
        try:
            decimals = len(self.format_str.split(".")[-1]) if "." in self.format_str else 0
            return f"{self._value:.{decimals}f}"
        except (ValueError, IndexError):
            return str(self._value)

    def begin_edit(self):
        self._input_text = self._formatted_value()

    def end_edit(self, commit=True):
        self._input_text = None

    def handle_key(self, key):
        global _TEXT_CLIPBOARD
        has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos

        if key == "cmd_c":
            _copy_text_to_clipboard(self._selected_text() if has_sel else self.display_value)
            return True
        if key == "cmd_v":
            if _TEXT_CLIPBOARD:
                self._insert_text(_TEXT_CLIPBOARD)
            return True
        if isinstance(key, dict) and key.get("type") == "paste":
            self._insert_text(key.get("text", ""))
            return True
        if key == "Left":
            if has_sel:
                self.cursor_pos = min(self._selection_start, self.cursor_pos)
                self._selection_start = None
            else:
                self.cursor_pos = max(0, self.cursor_pos - 1)
            return True
        if key == "Right":
            if has_sel:
                self.cursor_pos = max(self._selection_start, self.cursor_pos)
                self._selection_start = None
            else:
                self.cursor_pos = min(len(self.display_value), self.cursor_pos + 1)
            return True
        if key == "Up":
            new_val = self._value + self.step_size
            if self.max_value is not None:
                new_val = min(new_val, self.max_value)
            self._value = new_val
            self._input_text = self._formatted_value()
            self.cursor_pos = len(self.display_value)
            self._selection_start = None
            return True
        if key == "Down":
            new_val = self._value - self.step_size
            if self.min_value is not None:
                new_val = max(new_val, self.min_value)
            self._value = new_val
            self._input_text = self._formatted_value()
            self.cursor_pos = len(self.display_value)
            self._selection_start = None
            return True
        if key == "Backspace":
            if has_sel:
                self._replace_selection("")
            elif self.cursor_pos > 0:
                text = self.display_value
                self._set_from_text(text[:self.cursor_pos - 1] + text[self.cursor_pos:])
                self.cursor_pos = max(0, self.cursor_pos - 1)
            return True
        if isinstance(key, str) and len(key) == 1 and key.isdigit():
            if has_sel:
                self._replace_selection(key)
            else:
                self._insert_text(key)
            return True
        if key == "-":
            self._value = -self._value
            self._input_text = self._formatted_value()
            self.cursor_pos = len(self.display_value)
            self._selection_start = None
            return True
        if key == ".":
            if "." not in self.display_value:
                if has_sel:
                    self._replace_selection(key)
                else:
                    self._insert_text(key)
            return True
        return False

    def _selected_text(self):
        if self._selection_start is None or self._selection_start == self.cursor_pos:
            return ""
        start = min(self._selection_start, self.cursor_pos)
        end = max(self._selection_start, self.cursor_pos)
        return self.display_value[start:end]

    def _replace_selection(self, text):
        start = min(self._selection_start, self.cursor_pos)
        end = max(self._selection_start, self.cursor_pos)
        value_text = self.display_value
        new_text = value_text[:start] + text + value_text[end:]
        if not self._set_from_text(new_text):
            return
        self.cursor_pos = min(start + len(text), len(self.display_value))
        self._selection_start = None

    def _insert_text(self, text):
        value_text = self.display_value
        if not self._set_from_text(value_text[:self.cursor_pos] + text + value_text[self.cursor_pos:]):
            return
        self.cursor_pos = min(self.cursor_pos + len(text), len(self.display_value))
        self._selection_start = None

    def _set_from_text(self, text):
        if self._edit_mode:
            self._input_text = text
        try:
            self._value = float(text) if text not in ("", "-", ".", "-.") else 0.0
        except ValueError:
            return False
        return True

    def select_range(self, start, end):
        self._selection_start = max(0, min(start, len(self.display_value)))
        self.cursor_pos = max(0, min(end, len(self.display_value)))

    def copy_selection(self):
        if self._selected_text():
            _copy_text_to_clipboard(self._selected_text())
            _notify_copied(self)

    def render_cells(self):
        w = self.width if self.width else len(self.display_value) + 2
        text = self.display_value.ljust(w)[:w]
        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()

        if not self._edit_mode:
            return [_styled_cells(text, fg, bg, parent_bg, parent_fg=self._get_cell_color())]

        cs = self.cursor_style
        cur_fg = cs.color if cs and cs.color is not None else bg
        cur_bg = cs.background if cs and cs.background is not None else fg
        has_sel = self._selection_start is not None and self._selection_start != self.cursor_pos

        if has_sel:
            sel_a = max(0, min(w, min(self._selection_start, self.cursor_pos)))
            sel_b = max(0, min(w, max(self._selection_start, self.cursor_pos)))
            if sel_a < sel_b:
                line = []
                if sel_a > 0:
                    line.extend(_styled_cells(text[:sel_a], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
                line.extend(_styled_cells(text[sel_a:sel_b], cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
                if sel_b < w:
                    line.extend(_styled_cells(text[sel_b:], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
                return [line]

        cur_col = max(0, min(w - 1, self.cursor_pos)) if w > 0 else None
        if cur_col is None:
            return [_styled_cells(text, fg, bg, parent_bg, parent_fg=self._get_cell_color())]

        line = []
        if cur_col > 0:
            line.extend(_styled_cells(text[:cur_col], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        line.extend(_styled_cells(text[cur_col:cur_col + 1] or " ", cur_fg, cur_bg, parent_bg, parent_fg=self._get_cell_color()))
        if cur_col + 1 < w:
            line.extend(_styled_cells(text[cur_col + 1:], fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        return [line]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class Button(UIElement):
    """Button element."""

    ELEMENT_TYPE = "button"

    def __init__(self, name="", title="Button", row=0, col=0, width=None):
        super().__init__(name=name, row=row, col=col, width=width, height=1)
        self._title = title

    @property
    def title(self):
        return self._title

    @title.setter
    def title(self, value):
        self._title = str(value)
        self._mark_layout_dirty()
        self._mark_dirty()

    def render_cells(self):
        title_width = _visual_width(self._title)
        w = self.width if self.width else title_width + 4
        s = self.get_style()
        align = (s.text_align if s and s.text_align else "center")
        visible_title = _visual_text(self._title)
        if self._focused and w >= title_width + 2:
            if w >= title_width + 6:
                inner_width = w - len(" [] ")
                display = " [" + _align_text(f" {visible_title} ", inner_width, align)[:inner_width] + "] "
            elif w >= title_width + 4:
                inner_width = w - len("  []")
                display = " [" + _align_text(visible_title, inner_width, align)[:inner_width] + "] "
            else:
                display = f"[{visible_title}]"
        else:
            display = visible_title
        text = _align_text(display, w, align)
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()
        return [_styled_cells(text, fg, bg, parent_bg, parent_fg=self._get_cell_color())]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class CheckBox(UIElement):
    """Checkbox element."""

    ELEMENT_TYPE = "checkbox"

    def __init__(self, name="", title="", value=False, row=0, col=0):
        super().__init__(name=name, row=row, col=col, width=None, height=1)
        self._title = title
        self._value = value

    @property
    def value(self):
        return self._value

    @value.setter
    def value(self, v):
        self._value = bool(v)
        self._mark_dirty()

    @property
    def title(self):
        return self._title

    @title.setter
    def title(self, value):
        self._title = str(value)
        self._mark_layout_dirty()
        self._mark_dirty()

    def handle_key(self, key):
        if key == "Enter" or key == " ":
            self._value = not self._value
            self._mark_dirty()
            return True
        return False

    def render_cells(self):
        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()
        align = (s.text_align if s and s.text_align else "left")

        ind_style = self.checked_style if self._value else self.unchecked_style
        ind_fg = ind_style.color if ind_style and ind_style.color is not None else fg
        ind_bg = ind_style.background if ind_style and ind_style.background is not None else bg
        if ind_bg is not None and getattr(ind_bg, "alpha", 1.0) <= 0.0 and (self._focused or self._edit_mode):
            ind_bg = bg

        indicator = "[x]" if self._value else "[ ]"
        content = f"{indicator} {self._title}" if self._title else indicator
        content_width = len(indicator) + (_visual_width(" " + self._title, len(indicator)) if self._title else 0)
        w = self.width or content_width

        prefix_len = 0
        if align == "center":
            prefix_len = max(0, (w - content_width) // 2)
        elif align == "right":
            prefix_len = max(0, w - content_width)
        suffix_len = max(0, w - content_width - prefix_len)

        parts = []
        if prefix_len:
            parts.extend(_styled_cells(" " * prefix_len, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        parts.extend(_styled_cells(indicator, ind_fg, ind_bg, parent_bg, parent_fg=self._get_cell_color()))
        if self._title:
            parts.extend(_styled_cells(_visual_text(f" {self._title}", len(indicator)), fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        if suffix_len:
            parts.extend(_styled_cells(" " * suffix_len, fg, bg, parent_bg, parent_fg=self._get_cell_color()))
        return [parts]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class ComboBox(UIElement):
    """Combobox with selectable options."""

    ELEMENT_TYPE = "combobox"

    def __init__(self, name="", options=None, selected_item=None, row=0, col=0, width=None):
        super().__init__(name=name, row=row, col=col, width=width, height=1)
        self._options = options or []
        self._selected_item = selected_item
        self._menu_open = False

    @property
    def selected_item(self):
        return self._selected_item

    @selected_item.setter
    def selected_item(self, value):
        self._selected_item = value
        self._mark_layout_dirty()
        self._mark_dirty()

    @property
    def selected_items(self):
        if isinstance(self._selected_item, list):
            return self._selected_item
        return [self._selected_item] if self._selected_item else []

    @selected_items.setter
    def selected_items(self, items):
        if len(items) == 1:
            self._selected_item = items[0]
        else:
            self._selected_item = items
        self._mark_layout_dirty()
        self._mark_dirty()

    def handle_key(self, key):
        if not self._menu_open:
            if key == "Enter":
                self._menu_open = True
                return True
        else:
            if key == "Escape":
                self._menu_open = False
                return True
            if key == "Up":
                idx = self._options.index(self._selected_item) if self._selected_item in self._options else 0
                idx = max(0, idx - 1)
                self._selected_item = self._options[idx]
                return True
            if key == "Down":
                idx = self._options.index(self._selected_item) if self._selected_item in self._options else 0
                idx = min(len(self._options) - 1, idx + 1)
                self._selected_item = self._options[idx]
                return True
            if key == "Enter":
                self._menu_open = False
                return True
        return False

    def render_cells(self):
        w = self.width if self.width else 20
        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()

        sel = self.selected_style
        sel_fg = sel.color if sel else None
        sel_bg = sel.background if sel else None

        arrow = "v" if self._menu_open else (">" if self._focused else " ")
        display = _align_text(self._selected_item or "", max(0, w - 1), "left")
        lines = [_styled_cells(display + arrow, fg, bg, parent_bg, parent_fg=self._get_cell_color())]

        if self._menu_open:
            for opt in self._options:
                is_sel = opt == self._selected_item
                item_fg = sel_fg if is_sel and sel_fg is not None else fg
                item_bg = _overlay_background(sel_bg, bg, parent_bg) if is_sel and sel_bg is not None else bg
                lines.append(_styled_cells(_align_text(f"  {opt}", w, "left"), item_fg, item_bg, parent_bg, parent_fg=self._get_cell_color()))

        return lines[:self.height] if (self.height or 1) > 1 else lines

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class ListBox(UIElement):
    """Listbox with selectable items."""

    ELEMENT_TYPE = "listbox"
    DEFAULT_ITEM_PADDING_LEFT = 0
    DEFAULT_ITEM_PADDING_RIGHT = 0

    def __init__(self, name="", options=None, selected_items=None, row=0, col=0,
                 width=None, height=LISTBOX_DEFAULT_HEIGHT, multiple=False):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._options = options or []
        self._selected_items = selected_items if selected_items is not None else []
        self.multiple = multiple
        self._scroll_offset = 0
        self._disabled_items = set()

    @property
    def selected_item(self):
        return self._selected_items[0] if self._selected_items else None

    @selected_item.setter
    def selected_item(self, value):
        self._selected_items = [value] if value else []
        self._mark_dirty()

    @property
    def selected_items(self):
        return self._selected_items

    @selected_items.setter
    def selected_items(self, items):
        self._selected_items = list(items) if items else []
        self._mark_dirty()

    @property
    def disabled_items(self):
        return list(self._disabled_items)

    @disabled_items.setter
    def disabled_items(self, items):
        self._disabled_items = set(items or [])
        self._mark_dirty()

    def _ensure_selection_visible(self, idx):
        h = self.height or LISTBOX_DEFAULT_HEIGHT
        if idx < self._scroll_offset:
            self._scroll_offset = idx
        elif idx >= self._scroll_offset + h:
            self._scroll_offset = idx - h + 1

    def _max_scroll_offset(self):
        h = max(1, int(self.height or LISTBOX_DEFAULT_HEIGHT))
        return max(0, len(self._options) - h)

    def _handle_mouse_wheel(self, direction):
        max_offset = self._max_scroll_offset()
        if max_offset <= 0:
            return False
        wheel_delta = _mouse_wheel_delta(direction)
        if wheel_delta == 0:
            return False
        next_offset = max(
            0,
            min(max_offset, self._scroll_offset - wheel_delta * LISTBOX_WHEEL_SCROLL_ROWS),
        )
        if next_offset == self._scroll_offset:
            return False
        self._scroll_offset = next_offset
        self._mark_dirty()
        return True

    def handle_key(self, key):
        if isinstance(key, dict) and key.get("type") == "mouse_wheel":
            return self._handle_mouse_wheel(key)

        if not self._options:
            return False

        if not self._selected_items:
            if key in ("Up", "Down"):
                self._selected_items = [self._options[0]]
                self._scroll_offset = 0
                return True
            return False

        current = self._selected_items[-1]
        idx = self._options.index(current) if current in self._options else 0

        if key == "Up":
            idx = max(0, idx - 1)
            self._selected_items[-1] = self._options[idx]
            self._ensure_selection_visible(idx)
            return True
        if key == "Down":
            idx = min(len(self._options) - 1, idx + 1)
            self._selected_items[-1] = self._options[idx]
            self._ensure_selection_visible(idx)
            return True
        if key == "Enter":
            if not self.multiple:
                pass  # window will exit edit mode
            elif current in self._selected_items:
                self._selected_items.remove(current)
            else:
                self._selected_items.append(current)
            return True
        if key == "Escape":
            return True
        return False

    def render_cells(self):
        w = self.width if self.width else 20
        lines = []

        s = self.get_style()
        fg = s.color if s else None
        bg = s.background if s else None
        parent_bg = self._get_cell_background()
        pad_top, pad_right, pad_bottom, pad_left = s.padding if s else (0, 0, 0, 0)
        if (pad_top, pad_right, pad_bottom, pad_left) == (0, 0, 0, 0):
            pad_left = self.DEFAULT_ITEM_PADDING_LEFT
            pad_right = self.DEFAULT_ITEM_PADDING_RIGHT

        sel = self.selected_style
        sel_fg = sel.color if sel else None
        sel_bg = sel.background if sel else None
        disabled = self.disabled_style
        disabled_fg = disabled.color if disabled and disabled.color is not None else fg
        disabled_bg = disabled.background if disabled and disabled.background is not None else bg

        h = self.height or LISTBOX_DEFAULT_HEIGHT
        has_above = self._scroll_offset > 0
        has_below = (self._scroll_offset + h) < len(self._options)

        data_rows = 0
        for i in range(h):
            opt_idx = i + self._scroll_offset
            if opt_idx >= len(self._options):
                break
            opt = self._options[opt_idx]
            is_sel = opt in self._selected_items
            is_disabled = opt in self._disabled_items
            item_fg = disabled_fg if is_disabled else (sel_fg if is_sel and sel_fg is not None else fg)
            item_bg = disabled_bg if is_disabled else (_overlay_background(sel_bg, bg, parent_bg) if is_sel and sel_bg is not None else bg)
            item_text = self._format_item_text(opt, w, pad_left, pad_right)
            if i == 0 and has_above and w > 0:
                item_text = item_text[:w - 1] + "^"
            elif i == h - 1 and has_below and w > 0:
                item_text = item_text[:w - 1] + "v"
            lines.append(_styled_cells(item_text, item_fg, item_bg, parent_bg, parent_fg=self._get_cell_color()))
            data_rows += 1

        # If last data row is not the last visible row but has_below, show indicator on last data row
        if has_below and data_rows < h and data_rows > 0 and w > 0:
            last_line_plain = lines[data_rows - 1]
            # Already rendered — replace last char with 'v' by re-rendering
            opt_idx = data_rows - 1 + self._scroll_offset
            opt = self._options[opt_idx]
            is_sel = opt in self._selected_items
            is_disabled = opt in self._disabled_items
            item_fg = disabled_fg if is_disabled else (sel_fg if is_sel and sel_fg is not None else fg)
            item_bg = disabled_bg if is_disabled else (_overlay_background(sel_bg, bg, parent_bg) if is_sel and sel_bg is not None else bg)
            item_text = self._format_item_text(opt, w, pad_left, pad_right)[:w - 1] + "v"
            lines[data_rows - 1] = _styled_cells(item_text, item_fg, item_bg, parent_bg, parent_fg=self._get_cell_color())

        while len(lines) < h:
            lines.append(_styled_cells(" " * w, fg, bg, parent_bg, parent_fg=self._get_cell_color()))

        return lines[:h]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())

    def _format_item_text(self, text, width, pad_left, pad_right):
        content_width = max(0, width - pad_left - pad_right)
        content = _align_text(str(text), content_width, "left")
        padded = (" " * pad_left) + content + (" " * pad_right)
        return padded.ljust(width)[:width]


class MessageTable(UIElement):
    """Renders a markdown table using box-drawing ASCII characters.

    Parses markdown table format:
        | Header 1 | Header 2 |
        | -------- | -------- |
        | Cell 1   | Cell 2   |

    And renders with box-drawing characters:
        ┌──────────┬──────────┐
        │ Header 1 │ Header 2 │
        ├──────────┼──────────┤
        │ Cell 1   │ Cell 2   │
        └──────────┴──────────┘

    Supports alignment detection from separator row:
        :---  = left,   ---: = right,   :---: = center
    """

    ELEMENT_TYPE = "messagetable"
    MIN_RENDER_WIDTH = 1
    CELL_PADDING_WIDTH = 2
    BORDER_COLUMN_WIDTH = 1

    # Box drawing characters
    BOX = {
        'TL': '\u250c',  # ┌ top-left corner
        'TR': '\u2510',  # ┐ top-right corner
        'BL': '\u2514',  # └ bottom-left corner
        'BR': '\u2518',  # ┘ bottom-right corner
        'H': '\u2500',   # ─ horizontal line
        'V': '\u2502',   # │ vertical line (data rows)
        'TH': '\u252c',  # ┬ top T-junction (top border between cols)
        'BH': '\u2534',  # ┴ bottom T-junction (bottom border between cols)
        'LV': '\u251c',  # ├ left T-junction (separator between header/data)
        'RV': '\u2524',  # ┤ right T-junction (separator between header/data)
        'MV': '\u253c',  # ┼ multi T-junction (separator between cols)
    }

    def __init__(self, name="", text="", row=0, col=0, width=None, height=None):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._text = text
        self._parsed_table = None

    @property
    def text(self):
        return self._text

    @text.setter
    def text(self, value):
        self._text = str(value) if value else ""
        self._parsed_table = None
        self._mark_layout_dirty()
        self._mark_dirty()

    @staticmethod
    def _parse_markdown_table(text):
        """Parse markdown table format and return structured data.

        Returns dict with:
            - headers: list of header strings
            - alignments: list of 'left', 'center', 'right' per column
            - rows: list of row lists (each row is a list of cell strings)
        """
        if not text:
            return None

        lines = text.strip().split('\n')
        if len(lines) < 3:
            return None

        def parse_row(line):
            line = line.strip()
            if not line.startswith('|'):
                return None
            cells = line.split('|')[1:-1]
            return [c.strip() for c in cells]

        header = parse_row(lines[0])
        if not header:
            return None

        separator = parse_row(lines[1])
        if not separator or len(separator) != len(header):
            return None

        alignments = []
        for sep in separator:
            sep_stripped = sep.strip()
            if sep_stripped.startswith(':') and sep_stripped.endswith(':'):
                alignments.append('center')
            elif sep_stripped.endswith(':'):
                alignments.append('right')
            else:
                alignments.append('left')

        rows = []
        for line in lines[2:]:
            row = parse_row(line)
            if row and len(row) == len(header):
                rows.append(row)

        if not rows:
            return None

        return {
            'headers': header,
            'alignments': alignments,
            'rows': rows,
        }

    def _compute_column_widths(self, table):
        """Compute minimum column widths based on content."""
        headers = table['headers']
        rows = table['rows']
        num_cols = len(headers)

        widths = [_visual_width(h) for h in headers]
        for row in rows:
            for i, cell in enumerate(row):
                if i < num_cols:
                    widths[i] = max(widths[i], _visual_width(cell))

        return widths

    def _fit_column_widths_to_element(self, widths):
        """Expand table columns so the rendered table uses the element width."""
        if not widths:
            return widths

        available_width = int(self.width or 0)
        natural_width = (
            sum(widths)
            + self.CELL_PADDING_WIDTH * len(widths)
            + self.BORDER_COLUMN_WIDTH * (len(widths) + 1)
        )
        extra_width = available_width - natural_width
        if extra_width <= 0:
            return widths

        per_column, remainder = divmod(extra_width, len(widths))
        return [
            width + per_column + (self.BORDER_COLUMN_WIDTH if index < remainder else 0)
            for index, width in enumerate(widths)
        ]

    def _render_cell(self, text, width, align, fg, bg, parent_bg):
        """Render a single cell with padding and alignment."""
        aligned = _align_text(text, width, align)
        return _ansi_color(aligned, fg, bg, parent_bg, parent_fg=self._get_cell_color())

    def _render_cell_cells(self, text, width, align, fg, bg, parent_bg):
        """Render a single table cell as terminal cells."""
        aligned = _align_text(text, width, align)
        return _styled_cells(aligned, fg, bg, parent_bg, parent_fg=self._get_cell_color())

    def _render_table_line(self, cells, widths, top=False, separator=False, bottom=False):
        """Render one horizontal line of the table (header/separator/data/footer)."""
        num_cols = len(widths)
        parts = []

        if top:
            parts.append(self.BOX['TL'])
        elif separator:
            parts.append(self.BOX['TH'])
        elif bottom:
            parts.append(self.BOX['BL'])
        else:
            parts.append(self.BOX['V'])

        for i, (cell_text, width) in enumerate(zip(cells, widths)):
            parts.append(self.BOX['H'] * (width + 2))  # +2 for padding spaces
            if i < num_cols - 1:
                if separator:
                    parts.append(self.BOX['MV'])
                elif top or bottom:
                    parts.append(self.BOX['TR'] if i == num_cols - 1 else self.BOX['H'])
                    # Actually for top/bottom we need the corner at end
                else:
                    parts.append(self.BOX['V'])

        # Close the right border
        if top:
            parts.append(self.BOX['TR'])
        elif bottom:
            parts.append(self.BOX['BR'])
        else:
            # For separator, we already added MV between cols, need V at end
            if not separator:
                parts.append(self.BOX['V'])

        return "".join(parts)

    def _render_table_line_v2(self, widths, line_type):
        """Render a horizontal separator line with correct box-drawing chars.

        line_type: 'top', 'separator', 'bottom'
        """
        num_cols = len(widths)

        if line_type == 'top':
            left_char = self.BOX['TL']
            between = self.BOX['TH']
            right_char = self.BOX['TR']
        elif line_type == 'separator':
            left_char = self.BOX['LV']
            between = self.BOX['MV']
            right_char = self.BOX['RV']
        else:  # bottom
            left_char = self.BOX['BL']
            between = self.BOX['BH']
            right_char = self.BOX['BR']

        parts = []
        parts.append(left_char)

        for i, width in enumerate(widths):
            parts.append(self.BOX['H'] * (width + 2))
            if i < num_cols - 1:
                parts.append(between)

        parts.append(right_char)

        return "".join(parts)

    def _render_table_line_v2_cells(self, widths, line_type, fg, bg):
        return _styled_cells(self._render_table_line_v2(widths, line_type), fg, bg)

    def render_cells(self):
        if not self._parsed_table:
            self._parsed_table = self._parse_markdown_table(self._text)

        if not self._parsed_table:
            s = self.get_style()
            fg = s.color if s else None
            bg = s.background if s else None
            parent_bg = self._get_cell_background()
            w = max(self.MIN_RENDER_WIDTH, self.width or len(self._text or ""))
            h = self.height or 1
            return [_styled_cells(" " * w, fg, bg, parent_bg, parent_fg=self._get_cell_color())]

        table = self._parsed_table
        headers = table['headers']
        rows = table['rows']
        alignments = table['alignments']
        widths = self._fit_column_widths_to_element(self._compute_column_widths(table))

        s = self.get_style()
        header_fg = Color(s.color) if s and s.color else None
        header_bg = Color(s.background) if s and s.background else None
        data_fg = Color(s.color) if s and s.color else None
        data_bg = Color(s.background) if s and s.background else None
        parent_bg = self._get_cell_background()

        # Get cell styles from style object if available
        header_style = getattr(s, 'header_style', None)
        data_style = getattr(s, 'data_style', None)
        separator_style = getattr(s, 'separator_style', None)

        if header_style:
            header_fg = Color(header_style.color) if header_style and header_style.color else header_fg
            header_bg = Color(header_style.background) if header_style and header_style.background else header_bg
        if data_style:
            data_fg = Color(data_style.color) if data_style and data_style.color else data_fg
            data_bg = Color(data_style.background) if data_style and data_style.background else data_bg

        # Vertical divider color - same as text color for visibility
        border_fg = Color(s.color) if s and s.color else None
        border_bg = Color(s.background) if s and s.background else None

        lines = []

        # Top border with ANSI color
        lines.append(self._render_table_line_v2_cells(widths, 'top', border_fg, border_bg))

        # Header row with colored vertical dividers
        header_cells = [h for h in headers]
        header_line = _styled_cells(self.BOX['V'], border_fg, border_bg)
        for i, (cell_text, width) in enumerate(zip(header_cells, widths)):
            aligned = _align_text(cell_text, width, alignments[i])
            header_line.extend(_styled_cells(" " + aligned + " ", header_fg, header_bg, parent_bg))
            if i < len(widths) - 1:
                header_line.extend(_styled_cells(self.BOX['V'], border_fg, border_bg))
        header_line.extend(_styled_cells(self.BOX['V'], border_fg, border_bg))
        lines.append(header_line)

        # Separator row with ANSI color
        lines.append(self._render_table_line_v2_cells(widths, 'separator', border_fg, border_bg))

        # Data rows with colored vertical dividers
        for row in rows:
            data_line = _styled_cells(self.BOX['V'], border_fg, border_bg)
            for i, (cell_text, width) in enumerate(zip(row, widths)):
                aligned = _align_text(cell_text, width, alignments[i])
                data_line.extend(_styled_cells(" " + aligned + " ", data_fg, data_bg, parent_bg))
                if i < len(widths) - 1:
                    data_line.extend(_styled_cells(self.BOX['V'], border_fg, border_bg))
            data_line.extend(_styled_cells(self.BOX['V'], border_fg, border_bg))
            lines.append(data_line)

        # Bottom border with ANSI color
        lines.append(self._render_table_line_v2_cells(widths, 'bottom', border_fg, border_bg))

        # Set height to actual table height (never clip bottom border)
        self.height = len(lines)

        # Pad to height if needed
        while len(lines) < (self.height or 1):
            lines.append([])

        return lines[:self.height]

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class UIElementReusable(UIElement):
    """Reusable UI element that loads its own layout from a compiled module."""

    ELEMENT_TYPE = "uielement"

    def __init__(self, name="", layout_class=None, row=0, col=0, width=None, height=None, text=""):
        super().__init__(name=name, row=row, col=col, width=width, height=height)
        self._layout_class = layout_class
        self._child_instance = None
        self._child_opened = False
        self._text = text

    @property
    def text(self):
        return self._text

    @text.setter
    def text(self, value):
        self._text = str(value)

    @property
    def child(self):
        return self._ensure_child()

    def get_child(self, name):
        child = self._ensure_child()
        if child is None:
            return None
        return getattr(child, "_elements", {}).get(name)

    def __getattr__(self, name):
        if name.startswith("_"):
            raise AttributeError(name)
        child = self._ensure_child()
        if child is not None and hasattr(child, name):
            return getattr(child, name)
        raise AttributeError(name)

    def open(self):
        self._sync_child(open_child=True)

    def resize(self, width, height):
        self.width = width
        self.height = height
        self._sync_child(open_child=self._child_opened)

    def _ensure_child(self):
        if self._layout_class is None:
            return None
        if self._child_instance is None:
            self._child_instance = self._layout_class()
            self._attach_child(self._child_instance)
        self._child_instance._app = getattr(self, "_app", None)
        return self._child_instance

    def _attach_child(self, child):
        child.parent = self
        child.mode = "fullscreen"
        child._app = getattr(self, "_app", None)
        return child

    def _sync_child(self, open_child=False):
        child = self._ensure_child()
        if child is None:
            return None

        width = self.width
        height = self.height
        if width is not None and height is not None:
            child.resize(width, height)

        if open_child and not self._child_opened:
            child.open()
            self._child_opened = True

        self._apply_style_to_child(child)
        return child

    def _apply_style_to_child(self, child):
        child_style = getattr(child, "style", None)
        if child_style is None:
            return

        own_style = self.style
        background = getattr(own_style, "background", None) if own_style else None
        child_background = getattr(child_style, "background", None)
        if (
            background is not None
            and getattr(child_background, "alpha", 1.0) >= 1.0
        ):
            child_style.background = background

        color = getattr(own_style, "color", None) if own_style else None
        if color is not None:
            child_style.color = color

    def handle_key(self, key):
        child = self._sync_child(open_child=True)
        if child is None:
            return False
        return bool(child.handle_key(key))

    def render_cells(self):
        if not self._layout_class and self._child_instance is None:
            fallback_width = max((len(line) for line in (self._text or "").split('\n')), default=1)
            w = self.width if self.width else fallback_width
            h = self.height or 1
            s = self.get_style()
            fg = s.color if s else None
            bg = s.background if s else None
            parent_bg = self._get_cell_background()
            all_lines = (self._text or "").split('\n')
            visible = all_lines[-h:] if len(all_lines) > h else all_lines
            lines = []
            for i in range(h):
                raw = visible[i] if i < len(visible) else ""
                line = raw[:w] if w > 0 else ""
                lines.append(_styled_cells(line.ljust(w), fg, bg, parent_bg, parent_fg=self._get_cell_color()))
            return lines[:h]

        child = self._sync_child(open_child=True)
        scope_focused = False
        owner = getattr(self, "parent", None)
        while owner is not None:
            active_scope = getattr(owner, "_active_scrollview_scope", None)
            if active_scope is not None and active_scope.get("proxy") is self:
                scope_focused = True
                break
            owner = getattr(owner, "parent", None)
        descendant_focused = (
            child is not None
            and getattr(child, "_focused_element", None) is not None
        )
        scrollview_child = (
            child is not None
            and hasattr(child, "child_view_entries")
            and hasattr(child, "_clamped_viewport_rect")
        )
        scrollview_child_focused = (
            scrollview_child
            and bool(getattr(child, "_focused", False))
        )
        fill_fg = None
        fill_bg = None
        state_style = self.edit_style if self._edit_mode and self.edit_style is not None else self.focus_style
        state_background = getattr(state_style, "background", None) if state_style is not None else None
        if (
            child is not None
            and hasattr(child, "child_view_entries")
            and hasattr(child, "_clamped_viewport_rect")
            and state_background is not None
            and getattr(state_background, "alpha", 1.0) < 1.0
        ):
            state_style = None
        state_background = getattr(state_style, "background", None) if state_style is not None else None
        descendant_state = (
            descendant_focused
            and not self._focused
            and not scope_focused
            and not self._edit_mode
        )
        apply_state = (
            not scrollview_child
            and (
                self._focused
                or scope_focused
                or self._edit_mode
                or (
                    descendant_state
                    and not (
                        state_background is not None
                        and getattr(state_background, "alpha", 1.0) < 1.0
                    )
                )
            )
        )
        if child is not None and hasattr(child, "style") and apply_state and state_style is not None:
            previous_style = child.style
            focused_style = child.style.copy()
            original_bg = focused_style.background
            focused_style.merge(state_style)
            # For transparent/semi-transparent child backgrounds, restore original to prevent
            # double application via _parent_background() chain. For opaque backgrounds the
            # merge should apply the focus color directly (no double-blend occurs).
            original_alpha = getattr(original_bg, "alpha", 1.0) if original_bg is not None else 1.0
            if original_alpha < 1.0:
                focused_style.background = original_bg
            child.style = focused_style
            try:
                rendered = child.render_cells() if hasattr(child, "render_cells") else [
                    self._row_to_cells(line) for line in child.render()
                ]
                fill_fg = getattr(child.style, "color", None)
                focus_bg = getattr(state_style, "background", None)
                cell_bg = self._get_cell_background()
                if focus_bg is not None:
                    from .UIBase import UIBase
                    fill_bg = UIBase._blend_color_over(focus_bg, cell_bg)
                    rendered = self._apply_state_background_to_rendered_cells(
                        rendered,
                        focus_bg,
                        (original_bg, cell_bg, getattr(self.style, "background", None)),
                    )
                else:
                    fill_bg = original_bg
            finally:
                child.style = previous_style
        else:
            rendered = (
                child.render_cells()
                if child is not None and hasattr(child, "render_cells")
                else [self._row_to_cells(line) for line in child.render()] if child is not None else []
            )
            if child is not None:
                child_style = getattr(child, "style", None)
                fill_fg = getattr(child_style, "color", None)
                fill_bg = getattr(child_style, "background", None)
        h = self.height or 1
        w = self.width or 1
        parent_bg = self._get_cell_background()
        parent_fg = self._get_cell_color()
        normalized = []
        for raw in rendered[:h]:
            line = self._row_to_cells(raw)
            missing = max(0, w - len(line))
            if missing:
                line = line + _styled_cells(" " * missing, fill_fg, fill_bg, parent_bg, parent_fg=parent_fg)
            normalized.append(line)
        rendered = normalized
        while len(rendered) < h:
            rendered.append(_styled_cells(" " * w, fill_fg, fill_bg, parent_bg, parent_fg=parent_fg))
        return rendered[:h]

    @staticmethod
    def _row_to_cells(row):
        if not row:
            return []
        if isinstance(row, str):
            from .rendering import parse_ansi_cells
            return parse_ansi_cells(row)
        return [cell if isinstance(cell, TerminalCell) else TerminalCell(str(cell or " ")[:1]) for cell in row]

    @classmethod
    def _background_key(cls, background):
        if background is None:
            return None
        hex_value = getattr(background, "hex", None)
        if isinstance(hex_value, str):
            return hex_value[:7] if hex_value.startswith("#") else hex_value
        if isinstance(background, str):
            if len(background) == 9 and background.isdigit():
                return f"#{int(background[0:3]):02x}{int(background[3:6]):02x}{int(background[6:9]):02x}"
            return background[:7] if background.startswith("#") else background
        return str(background)

    @classmethod
    def _apply_state_background_to_rendered_cells(cls, rendered, state_background, base_backgrounds):
        from .UIBase import UIBase

        base_keys = {cls._background_key(background) for background in base_backgrounds if background is not None}
        fallback_background = next((background for background in base_backgrounds if background is not None), None)
        replace_all = getattr(state_background, "alpha", 1.0) >= 1.0
        styled = []
        for raw_row in rendered:
            row = cls._row_to_cells(raw_row)
            next_row = []
            for cell in row:
                cell_background = cell.background
                if replace_all or cell_background is None or cls._background_key(cell_background) in base_keys:
                    blend_background = cell_background if cell_background is not None else fallback_background
                    next_row.append(TerminalCell(
                        cell.text,
                        cell.foreground,
                        UIBase._blend_color_over(state_background, blend_background),
                        cell.raw,
                        cell.raw_width,
                        cell.raw_height,
                        cell.raw_skip,
                    ))
                else:
                    next_row.append(cell)
            styled.append(next_row)
        return styled

    def render(self):
        return _cells_to_ansi_compat_lines(self.render_cells())


class ViewHost(UIElementReusable):
    """Dynamic host for one active generated UIMD view."""

    ELEMENT_TYPE = "viewhost"

    def __init__(self, name="", row=0, col=0, width=None, height=None, text=""):
        super().__init__(name=name, layout_class=None, row=row, col=col, width=width, height=height, text=text)

    def set_view(self, view_instance):
        """Replace the hosted view instance."""
        if view_instance is self._child_instance:
            return view_instance
        self._detach_current_view()
        self._child_instance = view_instance
        if self._child_instance is not None:
            self._attach_child(self._child_instance)
            self._sync_child(open_child=self._child_opened)
        self._mark_layout_dirty()
        self._mark_dirty()
        return view_instance

    def clear_view(self):
        """Remove the current hosted view."""
        self._detach_current_view()
        self._child_instance = None
        self._mark_layout_dirty()
        self._mark_dirty()

    def current_view(self):
        """Return the current hosted view, if any."""
        return self._child_instance

    def _ensure_child(self):
        if self._child_instance is not None:
            self._child_instance._app = getattr(self, "_app", None)
        return self._child_instance

    def _detach_current_view(self):
        child = self._child_instance
        if child is None:
            return
        owner = self._owning_container()
        focused = getattr(owner, "_focused_element", None) if owner is not None else None
        if focused is not None and self._contains_element(child, focused):
            owner.set_focus(None)
        self._clear_child_focus(child)
        child.parent = None
        child._app = None
        self._child_opened = False

    def _owning_container(self):
        parent = getattr(self, "parent", None)
        while parent is not None:
            if hasattr(parent, "set_focus"):
                return parent
            parent = getattr(parent, "parent", None)
        return None

    @classmethod
    def _contains_element(cls, view, element):
        if view is element:
            return True
        for child in getattr(view, "_elements", {}).values():
            if child is element:
                return True
            nested = getattr(child, "_child_instance", None)
            if nested is not None and cls._contains_element(nested, element):
                return True
        return False

    @classmethod
    def _clear_child_focus(cls, view):
        if hasattr(view, "_focused_element"):
            focused = getattr(view, "_focused_element", None)
            if focused is not None:
                focused.focused = False
            view._focused_element = None
        if hasattr(view, "_edit_mode"):
            view._edit_mode = False
        for child in getattr(view, "_elements", {}).values():
            child.focused = False
            nested = getattr(child, "_child_instance", None)
            if nested is not None:
                cls._clear_child_focus(nested)

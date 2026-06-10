from dataclasses import dataclass
import unicodedata


ANSI_BASE_ROW = 1
ANSI_BASE_COL = 1
ANSI_DEFAULT_FOREGROUND = 39
ANSI_DEFAULT_BACKGROUND = 49
ANSI_RESET = "\x1b[0m"
ANSI_CLEAR_SCREEN = "\x1b[H\x1b[2J"
ANSI_SYNC_UPDATE_BEGIN = "\x1b[?2026h"
ANSI_SYNC_UPDATE_END = "\x1b[?2026l"
RGB_COMPACT_LENGTH = 9
RGB_HEX_LENGTH = 7
MIN_CELL_TEXT_LENGTH = 1
TERMINAL_CONTROL_PLACEHOLDER = "?"


@dataclass(frozen=True)
class TerminalCell:
    text: str = " "
    foreground: object = None
    background: object = None
    raw: str = ""
    raw_width: int = 1
    raw_height: int = 1
    raw_skip: bool = False

    def normalized_text(self):
        return terminal_cell_text(self.text)


def _is_unsafe_width_char(ch):
    if not ch:
        return True
    codepoint = ord(ch)
    category = unicodedata.category(ch)
    if category in ("Mn", "Me") or category.startswith("C"):
        return True
    if unicodedata.east_asian_width(ch) in ("F", "W"):
        return True
    if 0xFE00 <= codepoint <= 0xFE0F:
        return True
    if 0xE0100 <= codepoint <= 0xE01EF:
        return True
    return False


def terminal_cell_text(value):
    text = str(value or " ")
    if not text:
        return " "
    ch = text[:MIN_CELL_TEXT_LENGTH]
    if ch == "\t":
        return " "
    if ch in ("\n", "\r") or ord(ch) < 32 or ord(ch) == 127:
        return TERMINAL_CONTROL_PLACEHOLDER
    if _is_unsafe_width_char(ch):
        return TERMINAL_CONTROL_PLACEHOLDER
    return ch


class TerminalBufferRenderStats:
    def __init__(self):
        self.changed_cells = 0
        self.changed_runs = 0
        self.output_bytes = 0
        self.full_redraw = False
        self.scroll_region_uses = 0
        self.scroll_region_fallbacks = 0


def color_key(color):
    if color is None:
        return (0, None)
    red = getattr(color, "r", None)
    green = getattr(color, "g", None)
    blue = getattr(color, "b", None)
    alpha = getattr(color, "alpha", None)
    if red is not None and green is not None and blue is not None:
        return (1, int(red), int(green), int(blue), alpha)
    if isinstance(color, str):
        return (2, color)
    if isinstance(color, int):
        return (3, color)
    return (4, str(color))


def same_style(left, right):
    return (
        color_key(left.foreground) == color_key(right.foreground)
        and color_key(left.background) == color_key(right.background)
        and getattr(left, "raw", "") == getattr(right, "raw", "")
        and int(getattr(left, "raw_width", 1) or 1) == int(getattr(right, "raw_width", 1) or 1)
        and int(getattr(left, "raw_height", 1) or 1) == int(getattr(right, "raw_height", 1) or 1)
        and bool(getattr(left, "raw_skip", False)) == bool(getattr(right, "raw_skip", False))
    )


def coerce_cell(value):
    if isinstance(value, TerminalCell):
        return value
    if isinstance(value, dict):
        return TerminalCell(
            terminal_cell_text(value.get("text", value.get("char", " "))),
            value.get("foreground", value.get("fg")),
            value.get("background", value.get("bg")),
            str(value.get("raw", "") or ""),
            int(value.get("raw_width", 1) or 1),
            int(value.get("raw_height", 1) or 1),
            bool(value.get("raw_skip", False)),
        )
    if isinstance(value, (list, tuple)):
        text = value[0] if len(value) > 0 else " "
        foreground = value[1] if len(value) > 1 else None
        background = value[2] if len(value) > 2 else None
        return TerminalCell(terminal_cell_text(text), foreground, background)
    return TerminalCell(terminal_cell_text(value), None, None)


def blank_row(width, cell=None):
    cell = cell or TerminalCell()
    return [cell for _col in range(max(0, int(width)))]


def blank_content(width, height, cell=None):
    return [blank_row(width, cell) for _row in range(max(0, int(height)))]


def _rgb_from_compact(value):
    if not (isinstance(value, str) and len(value) == RGB_COMPACT_LENGTH and value.isdigit()):
        return None
    return int(value[0:3]), int(value[3:6]), int(value[6:9])


def _rgb_from_hex(value):
    if not (isinstance(value, str) and value.startswith("#") and len(value) >= RGB_HEX_LENGTH):
        return None
    try:
        return int(value[1:3], 16), int(value[3:5], 16), int(value[5:7], 16)
    except ValueError:
        return None


def snapshot_color(color):
    if color is None:
        return None
    if isinstance(color, str):
        if color == "transparent":
            return None
        if color.startswith("#") and len(color) >= RGB_HEX_LENGTH:
            return color[:RGB_HEX_LENGTH]
        compact = _rgb_from_compact(color)
        if compact is not None:
            return f"#{compact[0]:02x}{compact[1]:02x}{compact[2]:02x}"
        return color
    red = getattr(color, "r", -1)
    green = getattr(color, "g", -1)
    blue = getattr(color, "b", -1)
    alpha = getattr(color, "alpha", 1.0)
    if red < 0 or green < 0 or blue < 0 or alpha <= 0:
        return None
    return f"#{int(red):02x}{int(green):02x}{int(blue):02x}"


def _sgr_for_color(color, foreground):
    if color is None:
        return str(ANSI_DEFAULT_FOREGROUND if foreground else ANSI_DEFAULT_BACKGROUND)
    if isinstance(color, int):
        return str(color)
    if isinstance(color, str):
        if color == "transparent":
            return str(ANSI_DEFAULT_FOREGROUND if foreground else ANSI_DEFAULT_BACKGROUND)
        rgb = _rgb_from_hex(color) or _rgb_from_compact(color)
        if rgb is not None:
            return f"{'38' if foreground else '48'};2;{rgb[0]};{rgb[1]};{rgb[2]}"
        return str(ANSI_DEFAULT_FOREGROUND if foreground else ANSI_DEFAULT_BACKGROUND)
    red = getattr(color, "r", -1)
    green = getattr(color, "g", -1)
    blue = getattr(color, "b", -1)
    alpha = getattr(color, "alpha", 1.0)
    if red >= 0 and green >= 0 and blue >= 0 and alpha > 0:
        return f"{'38' if foreground else '48'};2;{int(red)};{int(green)};{int(blue)}"
    return str(ANSI_DEFAULT_FOREGROUND if foreground else ANSI_DEFAULT_BACKGROUND)


def style_sequence(foreground=None, background=None):
    return f"\x1b[{_sgr_for_color(foreground, True)};{_sgr_for_color(background, False)}m"


def cell_style_sequence(cell):
    return style_sequence(cell.foreground, cell.background)


def cells_to_ansi_line(row):
    if not row:
        return ANSI_RESET
    cells = [coerce_cell(raw_cell) for raw_cell in row]
    if all(
        cell.foreground is None
        and (
            cell.background is None
            or (
                getattr(cell.background, "hex", None) == "#000000"
                and getattr(cell.background, "alpha", 1.0) >= 1.0
            )
        )
        and not cell.raw
        and not cell.raw_skip
        for cell in cells
    ):
        return "".join(cell.normalized_text() for cell in cells)
    parts = []
    current = None
    for cell in cells:
        style = (color_key(cell.foreground), color_key(cell.background))
        if style != current:
            parts.append(cell_style_sequence(cell))
            current = style
        parts.append(cell.normalized_text())
    parts.append(ANSI_RESET)
    return "".join(parts)


def cells_to_ansi_lines(rows):
    return [cells_to_ansi_line(row) for row in rows]


def cells_to_snapshot_rows(rows):
    return [
        [
            {
                "char": coerce_cell(cell).normalized_text(),
                "foreground": snapshot_color(coerce_cell(cell).foreground),
                "background": snapshot_color(coerce_cell(cell).background),
                "attributes": [],
            }
            for cell in row
        ]
        for row in rows
    ]


def parse_ansi_cells(text):
    result = []
    foreground = None
    background = None
    index = 0
    text = text or ""
    while index < len(text):
        if text[index] == "\x1b" and index + 1 < len(text) and text[index + 1] == "[":
            end = index + 2
            while end < len(text) and not (0x40 <= ord(text[end]) <= 0x7E):
                end += 1
            if end < len(text) and text[end] == "m":
                foreground, background = apply_sgr(text[index + 2:end], foreground, background)
            index = end + 1
            continue
        result.append(TerminalCell(text[index], foreground, background))
        index += 1
    return result


def apply_sgr(raw, foreground, background):
    if raw == "":
        return None, None
    try:
        codes = [int(part) if part else 0 for part in raw.split(";")]
    except ValueError:
        return foreground, background
    index = 0
    while index < len(codes):
        code = codes[index]
        if code == 0:
            foreground = None
            background = None
        elif code == 39:
            foreground = None
        elif code == 49:
            background = None
        elif 30 <= code <= 37 or 90 <= code <= 97:
            foreground = code
        elif 40 <= code <= 47 or 100 <= code <= 107:
            background = code
        elif code == 38 and index + 4 < len(codes) and codes[index + 1] == 2:
            foreground = f"{codes[index + 2]:03d}{codes[index + 3]:03d}{codes[index + 4]:03d}"
            index += 4
        elif code == 48 and index + 4 < len(codes) and codes[index + 1] == 2:
            background = f"{codes[index + 2]:03d}{codes[index + 3]:03d}{codes[index + 4]:03d}"
            index += 4
        index += 1
    return foreground, background


class TerminalBuffer:
    def __init__(self, width, height):
        self.width = max(0, int(width))
        self.height = max(0, int(height))
        self.cells = blank_content(self.width, self.height)
        self.previous = blank_content(self.width, self.height)
        self.force_full_redraw = True
        self._stats = TerminalBufferRenderStats()

    def resize(self, width, height):
        width = max(0, int(width))
        height = max(0, int(height))
        if width == self.width and height == self.height:
            return False
        self.width = width
        self.height = height
        self.cells = blank_content(self.width, self.height)
        self.previous = blank_content(self.width, self.height)
        self.force_full_redraw = True
        return True

    def clear(self, cell=None):
        self.cells = blank_content(self.width, self.height, cell or TerminalCell())

    def set_cell(self, row, col, cell):
        row = int(row)
        col = int(col)
        if row < 0 or row >= self.height or col < 0 or col >= self.width:
            return
        self.cells[row][col] = coerce_cell(cell)

    def cell(self, row, col):
        if row < 0 or row >= self.height or col < 0 or col >= self.width:
            raise IndexError("terminal cell is out of bounds")
        return self.cells[row][col]

    def previous_cell(self, row, col):
        if row < 0 or row >= self.height or col < 0 or col >= self.width:
            raise IndexError("terminal previous cell is out of bounds")
        return self.previous[row][col]

    def copy_previous_to_current(self, row, col):
        self.set_cell(row, col, self.previous_cell(row, col))

    def request_full_redraw(self):
        self.force_full_redraw = True

    def begin_render_stats(self):
        self._stats = TerminalBufferRenderStats()

    def render_stats(self):
        return self._stats

    def render_diff(self, row_offset=0, col_offset=0):
        return self.render_diff_region(row_offset, col_offset, 0, 0, self.height, self.width)

    def render_diff_region(self, row_offset, col_offset, start_row, start_col, height, width):
        output = []
        full_redraw = self.force_full_redraw
        synchronize_update = False
        raw_emitted = False
        self._stats.full_redraw = self._stats.full_redraw or full_redraw
        first_row = max(0, int(start_row))
        first_col = max(0, int(start_col))
        last_row = min(self.height, int(start_row) + max(0, int(height)))
        last_col = min(self.width, int(start_col) + max(0, int(width)))

        for row in range(first_row, last_row):
            col = first_col
            while col < last_col:
                current = self.cells[row][col]
                previous = self.previous[row][col]
                if getattr(current, "raw_skip", False):
                    self.previous[row][col] = current
                    if full_redraw or current != previous:
                        self._stats.changed_cells += 1
                    col += 1
                    continue
                if not full_redraw and current == previous:
                    col += 1
                    continue

                style_cell = current
                run_col = col
                run = []
                raw = getattr(current, "raw", "")
                if raw:
                    synchronize_update = True
                    raw_width = max(1, int(getattr(current, "raw_width", 1) or 1))
                    raw_height = max(1, int(getattr(current, "raw_height", 1) or 1))
                    clear_width = min(raw_width, self.width - col)
                    clear_height = min(raw_height, self.height - row)
                    for clear_row in range(row, row + clear_height):
                        output.append(
                            f"\x1b[{clear_row + int(row_offset) + ANSI_BASE_ROW};"
                            f"{col + int(col_offset) + ANSI_BASE_COL}H"
                        )
                        output.append(cell_style_sequence(style_cell))
                        output.append(" " * clear_width)
                    if clear_height >= raw_height:
                        output.append(
                            f"\x1b[{row + int(row_offset) + ANSI_BASE_ROW};"
                            f"{col + int(col_offset) + ANSI_BASE_COL}H"
                        )
                        output.append(raw)
                        raw_emitted = True
                    for covered_row in range(row, row + clear_height):
                        for covered_col in range(col, col + clear_width):
                            self.previous[covered_row][covered_col] = self.cells[covered_row][covered_col]
                            self._stats.changed_cells += 1
                    self._stats.changed_runs += clear_height + 1
                    col += clear_width
                    continue
                while col < last_col:
                    current = self.cells[row][col]
                    previous = self.previous[row][col]
                    if not full_redraw and current == previous:
                        break
                    if getattr(current, "raw_skip", False):
                        break
                    if getattr(current, "raw", ""):
                        break
                    if not same_style(current, style_cell):
                        break
                    run.append(current.normalized_text())
                    self.previous[row][col] = current
                    self._stats.changed_cells += 1
                    col += 1

                output.append(
                    f"\x1b[{row + int(row_offset) + ANSI_BASE_ROW};"
                    f"{run_col + int(col_offset) + ANSI_BASE_COL}H"
                )
                output.append(cell_style_sequence(style_cell))
                output.append("".join(run))
                self._stats.changed_runs += 1

        if output:
            if raw_emitted:
                output.extend(self._raw_overlay_repaint(first_row, first_col, last_row, last_col, row_offset, col_offset))
            output.append(ANSI_RESET)
        rendered = "".join(output)
        if rendered and synchronize_update:
            rendered = ANSI_SYNC_UPDATE_BEGIN + rendered + ANSI_SYNC_UPDATE_END
        self._stats.output_bytes += len(rendered)
        self.force_full_redraw = False
        return rendered

    def _raw_overlay_repaint(self, first_row, first_col, last_row, last_col, row_offset, col_offset):
        output = []
        row_offset = int(row_offset)
        col_offset = int(col_offset)
        for row in range(first_row, last_row):
            col = first_col
            while col < last_col:
                cell = self.cells[row][col]
                if getattr(cell, "raw_skip", False) or getattr(cell, "raw", ""):
                    col += 1
                    continue
                style_cell = cell
                run_col = col
                run = []
                while col < last_col:
                    current = self.cells[row][col]
                    if getattr(current, "raw_skip", False) or getattr(current, "raw", ""):
                        break
                    if not same_style(current, style_cell):
                        break
                    run.append(current.normalized_text())
                    col += 1
                output.append(
                    f"\x1b[{row + row_offset + ANSI_BASE_ROW};"
                    f"{run_col + col_offset + ANSI_BASE_COL}H"
                )
                output.append(cell_style_sequence(style_cell))
                output.append("".join(run))
                self._stats.changed_runs += 1
        return output

    def render_scroll_region(self, row_offset, start_row, height, delta):
        first_row = max(0, int(start_row))
        last_row = min(self.height, int(start_row) + max(0, int(height)))
        region_height = last_row - first_row
        distance = min(abs(int(delta)), region_height)
        if self.force_full_redraw or region_height <= 1 or distance <= 0 or distance >= region_height:
            self._stats.scroll_region_fallbacks += 1
            return ""

        before = [list(row) for row in self.previous]
        if delta > 0:
            for row in range(last_row - 1, first_row + distance - 1, -1):
                for col in range(self.width):
                    self.previous[row][col] = before[row - distance][col]
            for row in range(first_row, first_row + distance):
                for col in range(self.width):
                    self.previous[row][col] = TerminalCell()
        else:
            for row in range(first_row, last_row - distance):
                for col in range(self.width):
                    self.previous[row][col] = before[row + distance][col]
            for row in range(last_row - distance, last_row):
                for col in range(self.width):
                    self.previous[row][col] = TerminalCell()

        command = "T" if delta > 0 else "S"
        output = (
            f"\x1b[{first_row + int(row_offset) + ANSI_BASE_ROW};"
            f"{last_row + int(row_offset)}r"
            f"\x1b[{first_row + int(row_offset) + ANSI_BASE_ROW};1H"
            f"\x1b[{distance}{command}"
            "\x1b[r"
        )
        self._stats.scroll_region_uses += 1
        self._stats.output_bytes += len(output)
        return output

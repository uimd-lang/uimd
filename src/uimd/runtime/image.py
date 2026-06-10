import fcntl
import os
import struct
import termios
from contextlib import contextmanager
from functools import lru_cache
from io import BytesIO

from PIL import Image as PillowImage

from .rendering import TerminalCell
from .style import Style
from .uielement import UIElement

_LIBSIXEL_AVAILABLE = False
_LIBSIXEL_CHECKED = False
_LIBSIXEL_ERROR = None
sixel_output_new = None
sixel_dither_new = None
sixel_dither_initialize = None
sixel_dither_unref = None
sixel_output_unref = None
sixel_encode = None
SIXEL_PIXELFORMAT_RGB888 = None
SIXEL_LARGE_AUTO = None
SIXEL_REP_AUTO = None
SIXEL_QUALITY_HIGH = None


IMAGE_CELL_PIXEL_WIDTH = 8
IMAGE_CELL_PIXEL_HEIGHT = 16
IMAGE_FALLBACK_VERTICAL_SAMPLES_PER_CELL = 2
IMAGE_SIXEL_MAX_COLORS = 256
TEST_FALLBACK_BLEND_DENOMINATOR = 255
TEST_FALLBACK_CHECKER_TILE_PIXELS = 4
TEST_FALLBACK_CHECKER_LIGHT_ALPHA = 160
TEST_FALLBACK_CHECKER_DARK_ALPHA = 0
TEST_FALLBACK_CHECKER_RGB = (255, 255, 255)
TEST_FALLBACK_COLOR_QUANTUM = 32
SIXEL_COLOR_COMPONENT_SCALE = 100
SIXEL_BITS_PER_GLYPH = 6
SIXEL_ESC = "\x1b"
FALLBACK_UPPER_HALF_BLOCK = "▀"
FALLBACK_FULL_BLOCK = "█"
DEFAULT_IMAGE_FIT = "contain"
DEFAULT_IMAGE_RENDER_MODE = "auto"
DEFAULT_IMAGE_ALIGN = "center"
DEFAULT_IMAGE_VALIGN = "middle"
MISSING_IMAGE_PLACEHOLDER = "image"
SIXEL_UNAVAILABLE_MESSAGE = (
    "libsixel is required for non-fallback Image rendering. "
    "Install libsixel and the Python libsixel binding, or use render_mode: fallback."
)
_FORCE_FALLBACK_RENDERING_DEPTH = 0
_FORCE_CELL_BACKGROUND_RENDERING_DEPTH = 0


@contextmanager
def force_image_fallback_rendering():
    global _FORCE_FALLBACK_RENDERING_DEPTH
    _FORCE_FALLBACK_RENDERING_DEPTH += 1
    try:
        yield
    finally:
        _FORCE_FALLBACK_RENDERING_DEPTH = max(0, _FORCE_FALLBACK_RENDERING_DEPTH - 1)


@contextmanager
def force_image_cell_background_rendering():
    global _FORCE_CELL_BACKGROUND_RENDERING_DEPTH
    _FORCE_CELL_BACKGROUND_RENDERING_DEPTH += 1
    try:
        yield
    finally:
        _FORCE_CELL_BACKGROUND_RENDERING_DEPTH = max(0, _FORCE_CELL_BACKGROUND_RENDERING_DEPTH - 1)


class SixelUnavailableError(RuntimeError):
    """Raised when a non-fallback Image needs libsixel but it is unavailable."""


class Image(UIElement):
    """Static terminal image element with Sixel and character fallback output."""

    ELEMENT_TYPE = "image"

    def __init__(
        self,
        name,
        source="",
        alt="",
        fit=DEFAULT_IMAGE_FIT,
        render_mode=DEFAULT_IMAGE_RENDER_MODE,
        align=DEFAULT_IMAGE_ALIGN,
        valign=DEFAULT_IMAGE_VALIGN,
    ):
        super().__init__(name=name)
        self.source = str(source or "")
        self.alt = str(alt or "")
        self.fit = str(fit or DEFAULT_IMAGE_FIT).strip().lower()
        self.render_mode = str(render_mode or DEFAULT_IMAGE_RENDER_MODE).strip().lower()
        self.align = str(align or DEFAULT_IMAGE_ALIGN).strip().lower()
        self.valign = str(valign or DEFAULT_IMAGE_VALIGN).strip().lower()

    def render(self):
        return _cells_to_strings(self.render_cells())

    def render_cells(self):
        width = max(1, int(getattr(self, "width", 0) or 0))
        height = max(1, int(getattr(self, "height", 0) or 0))
        style = self._effective_style()
        mode = self._resolved_render_mode()
        if mode == "sixel":
            require_sixel_for_image_rendering()
        clip = getattr(self, "_render_cell_clip", None)
        if mode == "deterministic":
            return self._deterministic_fallback_cells(width, height, style)
        if mode == "cell_background":
            return self._cell_background_cells(width, height, style)
        if mode == "sixel":
            try:
                src_w, src_h = _load_image(self._image_path()).size
            except Exception:
                src_w, src_h = 0, 0
            cols, rows, col_off, row_off = self._image_cell_region(width, height, src_w, src_h)
            region_fit = "cover" if self.fit == "contain" else self.fit
            visible_top = row_off
            visible_bottom = row_off + rows
            if clip is not None:
                clip_top = max(0, int(clip.get("top", 0) or 0))
                clip_bottom = min(height, int(clip.get("bottom", height) or height))
                visible_top = max(visible_top, clip_top)
                visible_bottom = min(visible_bottom, clip_bottom)
            visible_rows = max(0, visible_bottom - visible_top)
            raw = self._sixel_payload(
                cols,
                visible_rows,
                region_fit,
                source_rows=rows,
                crop_top=visible_top - row_off,
            ) if visible_rows > 0 else ""
            if raw:
                color = getattr(style, "color", None)
                bg = getattr(style, "background", None)
                cells = self._blank_cells(width, height, style)
                for row in range(visible_top, visible_bottom):
                    for col in range(col_off, col_off + cols):
                        cells[row][col] = TerminalCell(" ", color, bg, raw_skip=True)
                cells[visible_top][col_off] = TerminalCell(" ", color, bg, raw, cols, visible_rows)
                return cells
        return self._fallback_cells(width, height, style)

    def _effective_style(self):
        return self.get_style()

    def _letterbox_rgb(self, style):
        """Fallback letterbox fill: resolve a transparent image background to the
        structural cell background (or the owning window background) so it matches
        the Sixel path, which leaves the surrounding cells transparent and shows
        the panel behind them."""
        background = getattr(style, "background", None)
        if background is None or getattr(background, "alpha", 1.0) < 1.0:
            resolved = self._get_cell_background() or self._owner_window_background()
            if background is not None and resolved is not None:
                background = Style._blend_color_over(background, resolved)
            else:
                background = resolved
        red = getattr(background, "r", -1)
        green = getattr(background, "g", -1)
        blue = getattr(background, "b", -1)
        if red >= 0 and green >= 0 and blue >= 0:
            return int(red), int(green), int(blue)
        return 0, 0, 0

    def _owner_window_background(self):
        """Walk up the parent chain to the first opaque window/container background."""
        owner = getattr(self, "parent", None)
        while owner is not None:
            owner_style = getattr(owner, "style", None)
            background = getattr(owner_style, "background", None) if owner_style else None
            if background is not None and getattr(background, "alpha", 1.0) >= 1.0:
                return background
            owner = getattr(owner, "parent", None)
        return None

    def _resolved_render_mode(self):
        if _FORCE_CELL_BACKGROUND_RENDERING_DEPTH > 0:
            return "cell_background"
        if _FORCE_FALLBACK_RENDERING_DEPTH > 0:
            return "fallback"
        if deterministic_image_fallback_enabled():
            return "deterministic"
        mode = self.render_mode
        if mode == "sixel":
            return "fallback" if sixel_disabled() else "sixel"
        if mode == "fallback":
            return "fallback"
        return "sixel" if terminal_supports_sixel() else "fallback"

    def _image_path(self):
        if not self.source:
            return ""
        if os.path.isabs(self.source):
            return self.source
        owner = self.parent
        while owner is not None and not hasattr(owner, "_source_dir"):
            owner = getattr(owner, "parent", None)
        source_dir = getattr(owner, "_source_dir", "") if owner is not None else ""
        if source_dir:
            return os.path.abspath(os.path.join(source_dir, self.source))
        return os.path.abspath(self.source)

    def _fallback_cells(self, width, height, style):
        path = self._image_path()
        try:
            source = _load_image(path)
        except Exception:
            return self._placeholder_cells(width, height, style)
        cells = self._blank_cells(width, height, style)
        src_w, src_h = source.size
        cols, rows, col_off, row_off = self._image_cell_region(width, height, src_w, src_h)
        region_fit = "cover" if self.fit == "contain" else self.fit
        image = _fit_image(
            source,
            cols,
            rows * IMAGE_FALLBACK_VERTICAL_SAMPLES_PER_CELL,
            region_fit,
            background=self._letterbox_rgb(style),
            align=self.align,
            valign=self.valign,
        )
        for cell_row in range(rows):
            target_row = row_off + cell_row
            if target_row < 0 or target_row >= height:
                continue
            top_y = cell_row * IMAGE_FALLBACK_VERTICAL_SAMPLES_PER_CELL
            bottom_y = min(image.height - 1, top_y + 1)
            for x in range(cols):
                target_col = col_off + x
                if target_col < 0 or target_col >= width:
                    continue
                top = image.getpixel((x, top_y))[:3]
                bottom = image.getpixel((x, bottom_y))[:3]
                cells[target_row][target_col] = TerminalCell(
                    FALLBACK_UPPER_HALF_BLOCK,
                    _rgb_hex(top),
                    _rgb_hex(bottom),
                )
        return cells

    def _cell_background_cells(self, width, height, style):
        path = self._image_path()
        try:
            source = _load_image(path)
        except Exception:
            return self._placeholder_cells(width, height, style)
        cells = self._blank_cells(width, height, style)
        src_w, src_h = source.size
        cols, rows, col_off, row_off = self._image_cell_region(width, height, src_w, src_h)
        region_fit = "cover" if self.fit == "contain" else self.fit
        image = _fit_image(
            source,
            cols,
            rows,
            region_fit,
            background=self._letterbox_rgb(style),
            align=self.align,
            valign=self.valign,
        )
        for y in range(rows):
            target_row = row_off + y
            if target_row < 0 or target_row >= height:
                continue
            for x in range(cols):
                target_col = col_off + x
                if target_col < 0 or target_col >= width:
                    continue
                color = _rgb_hex(image.getpixel((x, y))[:3])
                cells[target_row][target_col] = TerminalCell(FALLBACK_FULL_BLOCK, color, color)
        return cells

    def _deterministic_fallback_cells(self, width, height, style):
        path = self._image_path()
        try:
            image = _load_image(path)
        except Exception:
            return self._placeholder_cells(width, height, style)
        cells = self._blank_cells(width, height, style)
        src_w, src_h = image.size
        cols, rows, col_off, row_off = self._image_cell_region(width, height, src_w, src_h)
        region_fit = "cover" if self.fit == "contain" else self.fit
        sample_height = rows * IMAGE_FALLBACK_VERTICAL_SAMPLES_PER_CELL
        background = self._letterbox_rgb(style)
        checker = _test_fallback_checker_enabled(self.render_mode)
        for row in range(row_off, row_off + rows):
            for col in range(col_off, col_off + cols):
                sample_col = col - col_off
                sample_top = (row - row_off) * IMAGE_FALLBACK_VERTICAL_SAMPLES_PER_CELL
                cells[row][col] = TerminalCell(
                    FALLBACK_UPPER_HALF_BLOCK,
                    _test_fallback_sample_color(
                        image, sample_col, sample_top, cols, sample_height,
                        region_fit, self.align, self.valign, background, checker,
                    ),
                    _test_fallback_sample_color(
                        image, sample_col, sample_top + 1, cols, sample_height,
                        region_fit, self.align, self.valign, background, checker,
                    ),
                )
        return cells

    def _sixel_payload(self, cols, rows, fit, source_rows=None, crop_top=0):
        path = self._image_path()
        bg = _style_background_rgb(self._effective_style())
        try:
            return _sixel_for_image(path, cols, rows, fit, self.align, self.valign, bg, source_rows, crop_top)
        except SixelUnavailableError:
            raise
        except Exception:
            return ""

    def _image_cell_region(self, width, height, src_w, src_h):
        """Cells the image occupies in 'contain'; surrounding cells stay transparent."""
        if self.fit != "contain" or src_w <= 0 or src_h <= 0:
            return width, height, 0, 0
        cell_px = _terminal_cell_px()
        px_w = cell_px[0] if cell_px else IMAGE_CELL_PIXEL_WIDTH
        px_h = cell_px[1] if cell_px else IMAGE_CELL_PIXEL_HEIGHT
        scale = min(width * px_w / src_w, height * px_h / src_h)
        cols = max(1, min(width, round(src_w * scale / px_w)))
        rows = max(1, min(height, round(src_h * scale / px_h)))
        col_off = _alignment_offset(width, cols, self.align, "left", "right")
        row_off = _alignment_offset(height, rows, self.valign, "top", "bottom")
        return cols, rows, col_off, row_off

    def _blank_cells(self, width, height, style):
        return [
            [
                TerminalCell(" ", getattr(style, "color", None), getattr(style, "background", None))
                for _col in range(width)
            ]
            for _row in range(height)
        ]

    def _placeholder_cells(self, width, height, style):
        text = self.alt or os.path.basename(self.source) or MISSING_IMAGE_PLACEHOLDER
        cells = self._blank_cells(width, height, style)
        row = height // 2
        col = max(0, (width - len(text)) // 2)
        for index, ch in enumerate(text[:width]):
            target_col = col + index
            if target_col >= width:
                break
            cells[row][target_col] = TerminalCell(ch, getattr(style, "color", None), getattr(style, "background", None))
        return cells


_terminal_cell_px_cache = None
_terminal_cell_px_queried = False
_terminal_cell_px_override = None


def set_terminal_cell_px(width, height):
    """Set terminal cell pixel metrics discovered by the application backend."""
    global _terminal_cell_px_override, _terminal_cell_px_cache, _terminal_cell_px_queried
    try:
        width = int(width)
        height = int(height)
    except (TypeError, ValueError):
        return
    if width <= 0 or height <= 0:
        return
    _terminal_cell_px_override = (width, height)
    _terminal_cell_px_cache = _terminal_cell_px_override
    _terminal_cell_px_queried = True


def reset_terminal_cell_px_cache():
    """Clear cached terminal cell metrics."""
    global _terminal_cell_px_cache, _terminal_cell_px_queried, _terminal_cell_px_override
    _terminal_cell_px_cache = None
    _terminal_cell_px_queried = False
    _terminal_cell_px_override = None


def _terminal_cell_px():
    """Return (cell_width_px, cell_height_px) from TIOCGWINSZ, or None if unavailable."""
    global _terminal_cell_px_cache, _terminal_cell_px_queried
    if _terminal_cell_px_override is not None:
        return _terminal_cell_px_override
    if _terminal_cell_px_queried:
        return _terminal_cell_px_cache
    _terminal_cell_px_queried = True
    try:
        import sys
        buf = fcntl.ioctl(sys.stdout.fileno(), termios.TIOCGWINSZ, b'\x00' * 8)
        rows, cols, xpx, ypx = struct.unpack('HHHH', buf)
        if rows > 0 and cols > 0 and xpx > 0 and ypx > 0:
            _terminal_cell_px_cache = (xpx // cols, ypx // rows)
    except Exception:
        pass
    return _terminal_cell_px_cache


def terminal_supports_sixel():
    if os.environ.get("UIMD_FORCE_SIXEL", "").strip().lower() in {"1", "true", "yes", "on"}:
        return True
    if sixel_disabled():
        return False
    term = os.environ.get("TERM", "").lower()
    if "sixel" in term:
        return True
    return False


def sixel_disabled():
    return os.environ.get("UIMD_DISABLE_SIXEL", "").strip().lower() in {"1", "true", "yes", "on"}


def deterministic_image_fallback_enabled():
    return os.environ.get("UIMD_DETERMINISTIC_IMAGE_FALLBACK", "").strip().lower() in {"1", "true", "yes", "on"}


def require_sixel_for_image_rendering():
    """Ensure libsixel is available when non-fallback image rendering requires it."""
    if deterministic_image_fallback_enabled() or sixel_disabled():
        return
    if not _load_libsixel():
        detail = f" ({_LIBSIXEL_ERROR})" if _LIBSIXEL_ERROR else ""
        raise SixelUnavailableError(f"{SIXEL_UNAVAILABLE_MESSAGE}{detail}")


def _load_libsixel():
    global _LIBSIXEL_AVAILABLE, _LIBSIXEL_CHECKED, _LIBSIXEL_ERROR
    global sixel_output_new, sixel_dither_new, sixel_dither_initialize
    global sixel_dither_unref, sixel_output_unref, sixel_encode
    global SIXEL_PIXELFORMAT_RGB888, SIXEL_LARGE_AUTO, SIXEL_REP_AUTO, SIXEL_QUALITY_HIGH

    if _LIBSIXEL_CHECKED:
        return _LIBSIXEL_AVAILABLE
    _LIBSIXEL_CHECKED = True
    try:
        from libsixel import (
            sixel_output_new as loaded_sixel_output_new,
            sixel_dither_new as loaded_sixel_dither_new,
            sixel_dither_initialize as loaded_sixel_dither_initialize,
            sixel_dither_unref as loaded_sixel_dither_unref,
            sixel_output_unref as loaded_sixel_output_unref,
            sixel_encode as loaded_sixel_encode,
            SIXEL_PIXELFORMAT_RGB888 as loaded_SIXEL_PIXELFORMAT_RGB888,
            SIXEL_LARGE_AUTO as loaded_SIXEL_LARGE_AUTO,
            SIXEL_REP_AUTO as loaded_SIXEL_REP_AUTO,
            SIXEL_QUALITY_HIGH as loaded_SIXEL_QUALITY_HIGH,
        )
    except Exception as exc:
        _LIBSIXEL_ERROR = str(exc)
        _LIBSIXEL_AVAILABLE = False
        return False

    sixel_output_new = loaded_sixel_output_new
    sixel_dither_new = loaded_sixel_dither_new
    sixel_dither_initialize = loaded_sixel_dither_initialize
    sixel_dither_unref = loaded_sixel_dither_unref
    sixel_output_unref = loaded_sixel_output_unref
    sixel_encode = loaded_sixel_encode
    SIXEL_PIXELFORMAT_RGB888 = loaded_SIXEL_PIXELFORMAT_RGB888
    SIXEL_LARGE_AUTO = loaded_SIXEL_LARGE_AUTO
    SIXEL_REP_AUTO = loaded_SIXEL_REP_AUTO
    SIXEL_QUALITY_HIGH = loaded_SIXEL_QUALITY_HIGH
    _LIBSIXEL_ERROR = None
    _LIBSIXEL_AVAILABLE = True
    return True


@lru_cache(maxsize=64)
def _load_image(path):
    with PillowImage.open(path) as image:
        return image.convert("RGBA")


@lru_cache(maxsize=128)
def _sixel_for_image(path, width_cells, height_cells, fit, align, valign, background_rgb=(0, 0, 0), source_height_cells=None, crop_top_cells=0):
    cell_px = _terminal_cell_px()
    cell_w = cell_px[0] if cell_px else IMAGE_CELL_PIXEL_WIDTH
    cell_h = cell_px[1] if cell_px else IMAGE_CELL_PIXEL_HEIGHT
    pixel_width = max(1, int(width_cells) * cell_w)
    source_height_cells = int(source_height_cells) if source_height_cells is not None else int(height_cells)
    source_pixel_height = max(1, source_height_cells * cell_h)
    crop_top = max(0, int(crop_top_cells) * cell_h)
    crop_height = max(1, int(height_cells) * cell_h)
    rgba = _fit_image(_load_image(path), pixel_width, source_pixel_height, fit, background=background_rgb, align=align, valign=valign)
    if crop_top > 0 or crop_height < rgba.height:
        rgba = rgba.crop((0, crop_top, pixel_width, min(rgba.height, crop_top + crop_height)))
    require_sixel_for_image_rendering()
    try:
        return _sixel_encode_libsixel(rgba.convert("RGB"))
    except Exception:
        pass
    return _sixel_encode_python(rgba)


def _sixel_encode_libsixel(rgb_image):
    width, height = rgb_image.size
    data = rgb_image.tobytes()
    buffer = BytesIO()
    output = sixel_output_new(lambda chunk, _priv: buffer.write(chunk))
    dither = sixel_dither_new(IMAGE_SIXEL_MAX_COLORS)
    try:
        sixel_dither_initialize(
            dither, data, width, height, SIXEL_PIXELFORMAT_RGB888,
            SIXEL_LARGE_AUTO, SIXEL_REP_AUTO, SIXEL_QUALITY_HIGH,
        )
        sixel_encode(data, width, height, 3, dither, output)
    finally:
        sixel_dither_unref(dither)
        sixel_output_unref(output)
    return buffer.getvalue().decode("latin-1")


def _sixel_encode_python(rgba):
    alpha_flat = [px[3] for px in rgba.getdata()]
    image = rgba.convert("RGB").convert("P", palette=PillowImage.Palette.ADAPTIVE, colors=IMAGE_SIXEL_MAX_COLORS)
    palette = image.getpalette() or []
    pixel_data = image.get_flattened_data() if hasattr(image, "get_flattened_data") else image.getdata()
    used_colors = sorted(set(p for i, p in enumerate(pixel_data) if alpha_flat[i] > 0))
    color_map = {color: index for index, color in enumerate(used_colors)}

    parts = [SIXEL_ESC + "Pq"]
    for color, index in color_map.items():
        palette_offset = int(color) * 3
        red = palette[palette_offset] if palette_offset < len(palette) else 0
        green = palette[palette_offset + 1] if palette_offset + 1 < len(palette) else 0
        blue = palette[palette_offset + 2] if palette_offset + 2 < len(palette) else 0
        parts.append(
            f"#{index};2;"
            f"{_sixel_component(red)};"
            f"{_sixel_component(green)};"
            f"{_sixel_component(blue)}"
        )

    pixels = image.load()
    for y in range(0, image.height, SIXEL_BITS_PER_GLYPH):
        for color, index in color_map.items():
            encoded = []
            previous = None
            count = 0
            has_pixels = False
            for x in range(image.width):
                bits = 0
                for bit in range(SIXEL_BITS_PER_GLYPH):
                    py = y + bit
                    if py < image.height and pixels[x, py] == color and alpha_flat[py * image.width + x] > 0:
                        bits |= 1 << bit
                if bits:
                    has_pixels = True
                char = chr(63 + bits)
                if char == previous:
                    count += 1
                else:
                    _append_sixel_run(encoded, previous, count)
                    previous = char
                    count = 1
            if not has_pixels:
                continue
            _append_sixel_run(encoded, previous, count)
            parts.append(f"#{index}")
            parts.append("".join(encoded))
            parts.append("$")
        parts.append("-")
    parts.append(SIXEL_ESC + "\\")
    return "".join(parts)


def _fit_image(image, width, height, fit, background=(0, 0, 0), align=DEFAULT_IMAGE_ALIGN, valign=DEFAULT_IMAGE_VALIGN):
    width = max(1, int(width))
    height = max(1, int(height))
    fit = str(fit or DEFAULT_IMAGE_FIT).strip().lower()
    src = image if image.mode == "RGBA" else image.convert("RGBA")
    if fit == "stretch":
        return src.resize((width, height), PillowImage.Resampling.LANCZOS)

    scale = max(width / src.width, height / src.height) if fit == "cover" else min(width / src.width, height / src.height)
    resized_width = max(1, round(src.width * scale))
    resized_height = max(1, round(src.height * scale))
    resized = src.resize((resized_width, resized_height), PillowImage.Resampling.LANCZOS)
    if fit == "cover":
        canvas = PillowImage.new("RGBA", (width, height), (*background, 255))
        left = _alignment_offset(resized_width, width, align, "left", "right")
        top = _alignment_offset(resized_height, height, valign, "top", "bottom")
        canvas.alpha_composite(resized.crop((left, top, left + width, top + height)))
        return canvas

    canvas = PillowImage.new("RGBA", (width, height), (*background, 255))
    left = _alignment_offset(width, resized_width, align, "left", "right")
    top = _alignment_offset(height, resized_height, valign, "top", "bottom")
    canvas.alpha_composite(resized, dest=(left, top))
    return canvas


def _alignment_offset(outer, inner, value, start_value, end_value):
    span = max(0, int(outer) - int(inner))
    value = str(value or "").strip().lower()
    if value == start_value:
        return 0
    if value == end_value:
        return span
    return span // 2


def _append_sixel_run(parts, char, count):
    if not char or count <= 0:
        return
    if count >= 4:
        parts.append(f"!{count}{char}")
    else:
        parts.append(char * count)


def _sixel_component(value):
    return max(0, min(SIXEL_COLOR_COMPONENT_SCALE, round(int(value) * SIXEL_COLOR_COMPONENT_SCALE / 255)))


def _rgb_hex(rgb):
    red, green, blue = rgb
    return f"#{int(red):02x}{int(green):02x}{int(blue):02x}"


def _test_fallback_checker_enabled(render_mode):
    return str(render_mode or "").strip().lower() != "fallback"


def _test_fallback_checker_light_square(x, y):
    return (
        int(x) // TEST_FALLBACK_CHECKER_TILE_PIXELS
        + int(y) // TEST_FALLBACK_CHECKER_TILE_PIXELS
    ) % 2 == 0


def _test_fallback_checker_tint(color, x, y):
    alpha = (
        TEST_FALLBACK_CHECKER_LIGHT_ALPHA
        if _test_fallback_checker_light_square(x, y)
        else TEST_FALLBACK_CHECKER_DARK_ALPHA
    )
    return _blend_rgb(TEST_FALLBACK_CHECKER_RGB, color, alpha)


def _test_fallback_sample_color(image, x, y, target_width, target_height, fit, align, valign, background_rgb, checker=False):
    src = image if image.mode == "RGBA" else image.convert("RGBA")
    target_width = max(1, int(target_width))
    target_height = max(1, int(target_height))
    x = max(0, min(target_width - 1, int(x)))
    y = max(0, min(target_height - 1, int(y)))
    base = tuple(int(channel) for channel in (background_rgb or (0, 0, 0)))
    if checker:
        base = _test_fallback_checker_tint(base, x, y)

    fit = str(fit or DEFAULT_IMAGE_FIT).strip().lower()
    if fit == "stretch":
        source_x = int(x * src.width / target_width)
        source_y = int(y * src.height / target_height)
    else:
        scale = max(target_width / src.width, target_height / src.height) if fit == "cover" else min(target_width / src.width, target_height / src.height)
        drawn_width = src.width * scale
        drawn_height = src.height * scale
        x_offset = _alignment_offset_float(target_width, drawn_width, align, "left", "right")
        y_offset = _alignment_offset_float(target_height, drawn_height, valign, "top", "bottom")
        source_x_float = (x - x_offset) / scale
        source_y_float = (y - y_offset) / scale
        if source_x_float < 0 or source_y_float < 0 or source_x_float >= src.width or source_y_float >= src.height:
            return _rgb_hex(_test_fallback_quantize_color(base))
        source_x = int(source_x_float)
        source_y = int(source_y_float)

    source_x = max(0, min(src.width - 1, source_x))
    source_y = max(0, min(src.height - 1, source_y))
    red, green, blue, alpha = src.getpixel((source_x, source_y))
    color = _blend_rgb((red, green, blue), base, alpha)
    if checker:
        color = _test_fallback_checker_tint(color, x, y)
    return _rgb_hex(_test_fallback_quantize_color(color))


def _blend_rgb(source, destination, alpha):
    alpha = max(0, min(TEST_FALLBACK_BLEND_DENOMINATOR, int(alpha)))
    return tuple(
        (
            int(source[index]) * alpha
            + int(destination[index]) * (TEST_FALLBACK_BLEND_DENOMINATOR - alpha)
        ) // TEST_FALLBACK_BLEND_DENOMINATOR
        for index in range(3)
    )


def _test_fallback_quantize_color(color):
    return tuple(
        max(0, min(255, (int(channel) // TEST_FALLBACK_COLOR_QUANTUM) * TEST_FALLBACK_COLOR_QUANTUM))
        for channel in color
    )


def _alignment_offset_float(outer, inner, value, start_value, end_value):
    span = max(0.0, float(outer) - float(inner))
    value = str(value or "").strip().lower()
    if value == start_value:
        return 0.0
    if value == end_value:
        return span
    return span / 2.0


def _style_background_rgb(style):
    background = getattr(style, "background", None)
    red = getattr(background, "r", -1)
    green = getattr(background, "g", -1)
    blue = getattr(background, "b", -1)
    if red >= 0 and green >= 0 and blue >= 0:
        return int(red), int(green), int(blue)
    return 0, 0, 0


def _cells_to_strings(rows):
    return ["".join(cell.normalized_text() for cell in row) for row in rows]

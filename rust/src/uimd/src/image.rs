use crate::{
    current_element_parent_background, Color, Element, ElementRenderState, Size, Style,
    TerminalBuffer, MINIMUM_RENDERABLE_SIZE,
};
#[cfg(test)]
use crate::core::OptionalColorExt;
use std::cell::Cell;
use std::cmp::{max, min};
use std::collections::{BTreeSet, HashMap};
use std::ffi::{c_int, c_void};
#[cfg(unix)]
use std::ffi::{c_char, CString};
use std::fs;
use std::hash::{Hash, Hasher};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicI32, Ordering};
use std::sync::{Arc, Mutex, OnceLock};

const IMAGE_CELL_PIXEL_WIDTH: i32 = 8;
const IMAGE_CELL_PIXEL_HEIGHT: i32 = 16;
const IMAGE_FALLBACK_VERTICAL_SAMPLES: i32 = 2;
const IMAGE_CHANNEL_MAX: i32 = 255;
const IMAGE_FALLBACK_CHECKER_TILE_PIXELS: i32 = 4;
const IMAGE_FALLBACK_CHECKER_LIGHT_ALPHA: i32 = 160;
const IMAGE_FALLBACK_CHECKER_DARK_ALPHA: i32 = 0;
const IMAGE_FALLBACK_COLOR_QUANTUM: i32 = 32;
const IMAGE_INFO_SAMPLE_GRID_SIZE: i32 = 3;
const IMAGE_INFO_COLOR_QUANTUM: i32 = 64;
const IMAGE_SIXEL_BITS_PER_GLYPH: i32 = 6;
const IMAGE_SIXEL_COLOR_COMPONENT_SCALE: i32 = 100;
const IMAGE_SIXEL_COLOR_LEVELS: i32 = 6;
const IMAGE_SIXEL_RUN_LENGTH_THRESHOLD: usize = 4;
const IMAGE_SIXEL_MAX_COLORS: i32 = 256;
const IMAGE_SIXEL_FALSE_STATUS_MASK: i32 = 0x1000;
const IMAGE_SIXEL_PIXEL_FORMAT_RGB888: i32 = 0x03;
const IMAGE_SIXEL_LARGE_AUTO: i32 = 0;
const IMAGE_SIXEL_REP_AUTO: i32 = 0;
const IMAGE_SIXEL_QUALITY_HIGH: i32 = 1;
const IMAGE_FALLBACK_UPPER_HALF_BLOCK: &str = "▀";
const IMAGE_FALLBACK_FULL_BLOCK: &str = "█";
const IMAGE_SIXEL_INTRODUCER: &str = "\x1bPq";
const IMAGE_SIXEL_TERMINATOR: &str = "\x1b\\";
static TERMINAL_CELL_PIXEL_WIDTH: AtomicI32 = AtomicI32::new(IMAGE_CELL_PIXEL_WIDTH);
static TERMINAL_CELL_PIXEL_HEIGHT: AtomicI32 = AtomicI32::new(IMAGE_CELL_PIXEL_HEIGHT);
const IMAGE_MINIMUM_SAMPLE_AREA: f64 = 0.000_001;
pub const DEFAULT_IMAGE_FIT: &str = "contain";
pub const DEFAULT_IMAGE_RENDER_MODE: &str = "auto";
pub const DEFAULT_IMAGE_ALIGN: &str = "center";
pub const DEFAULT_IMAGE_VERTICAL_ALIGN: &str = "middle";
const MISSING_IMAGE_PLACEHOLDER: &str = "image";
const SIXEL_UNAVAILABLE_MESSAGE: &str =
    "libsixel is required for non-fallback Image rendering. Install libsixel, or use render_mode: fallback.";

thread_local!
{
    static IMAGE_FALLBACK_RENDERING_DEPTH: Cell<usize> = const { Cell::new(0) };
    static IMAGE_CELL_BACKGROUND_RENDERING_DEPTH: Cell<usize> = const { Cell::new(0) };
}

#[derive(Debug)]
pub struct ScopedImageFallbackRendering;

impl ScopedImageFallbackRendering
{
    pub fn new() -> Self
    {
        IMAGE_FALLBACK_RENDERING_DEPTH.with(|depth| depth.set(depth.get() + 1));
        Self
    }
}

impl Default for ScopedImageFallbackRendering
{
    fn default() -> Self
    {
        Self::new()
    }
}

impl Drop for ScopedImageFallbackRendering
{
    fn drop(&mut self)
    {
        IMAGE_FALLBACK_RENDERING_DEPTH.with(
            |depth| depth.set(depth.get().saturating_sub(1)),
        );
    }
}

#[derive(Debug)]
pub struct ScopedImageCellBackgroundRendering;

impl ScopedImageCellBackgroundRendering
{
    pub fn new() -> Self
    {
        IMAGE_CELL_BACKGROUND_RENDERING_DEPTH.with(
            |depth| depth.set(depth.get() + 1),
        );
        Self
    }
}

impl Default for ScopedImageCellBackgroundRendering
{
    fn default() -> Self
    {
        Self::new()
    }
}

impl Drop for ScopedImageCellBackgroundRendering
{
    fn drop(&mut self)
    {
        IMAGE_CELL_BACKGROUND_RENDERING_DEPTH.with(
            |depth| depth.set(depth.get().saturating_sub(1)),
        );
    }
}

#[derive(Clone, Copy, Debug, Default, Eq, Hash, Ord, PartialEq, PartialOrd)]
struct ImageRgb
{
    red: i32,
    green: i32,
    blue: i32,
}

#[derive(Clone, Debug, Default)]
struct ImageRaster
{
    width: i32,
    height: i32,
    pixels: Vec<ImageRgb>,
    alpha: Vec<i32>,
}

unsafe extern "C"
{
    fn uimd_stbi_load_rgba_from_memory(
        data: *const u8,
        length: c_int,
        width: *mut c_int,
        height: *mut c_int,
        source_channels: *mut c_int,
    ) -> *mut u8;
    fn uimd_stbi_image_free(data: *mut c_void);
}

struct StbiImage(*mut u8);

impl Drop for StbiImage
{
    fn drop(&mut self)
    {
        unsafe
        {
            uimd_stbi_image_free(self.0.cast());
        }
    }
}

#[cfg(unix)]
#[repr(C)]
struct SixelOutput
{
    _private: [u8; 0],
}

#[cfg(unix)]
#[repr(C)]
struct SixelDither
{
    _private: [u8; 0],
}

#[cfg(unix)]
type SixelWriteFunction =
    unsafe extern "C" fn(*mut c_char, c_int, *mut c_void) -> c_int;
#[cfg(unix)]
type SixelOutputNewFunction = unsafe extern "C" fn(
    *mut *mut SixelOutput,
    SixelWriteFunction,
    *mut c_void,
    *mut c_void,
) -> c_int;
#[cfg(unix)]
type SixelDitherNewFunction =
    unsafe extern "C" fn(*mut *mut SixelDither, c_int, *mut c_void) -> c_int;
#[cfg(unix)]
type SixelDitherInitializeFunction = unsafe extern "C" fn(
    *mut SixelDither,
    *mut u8,
    c_int,
    c_int,
    c_int,
    c_int,
    c_int,
    c_int,
) -> c_int;
#[cfg(unix)]
type SixelEncodeFunction = unsafe extern "C" fn(
    *mut u8,
    c_int,
    c_int,
    c_int,
    *mut SixelDither,
    *mut SixelOutput,
) -> c_int;
#[cfg(unix)]
type SixelOutputUnrefFunction = unsafe extern "C" fn(*mut SixelOutput);
#[cfg(unix)]
type SixelDitherUnrefFunction = unsafe extern "C" fn(*mut SixelDither);

#[cfg(unix)]
#[derive(Clone, Copy)]
struct SixelApi
{
    output_new: SixelOutputNewFunction,
    dither_new: SixelDitherNewFunction,
    dither_initialize: SixelDitherInitializeFunction,
    encode: SixelEncodeFunction,
    output_unref: SixelOutputUnrefFunction,
    dither_unref: SixelDitherUnrefFunction,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
struct ImageCellRegion
{
    cols: i32,
    rows: i32,
    col_offset: i32,
    row_offset: i32,
}

#[derive(Clone, Debug, Eq)]
struct ImageRenderCacheKey
{
    source: String,
    width: i32,
    height: i32,
    fit: String,
    align: String,
    vertical_align: String,
    background: ImageRgb,
    source_height: i32,
    crop_top: i32,
}

impl PartialEq for ImageRenderCacheKey
{
    fn eq(&self, other: &Self) -> bool
    {
        self.source == other.source
            && self.width == other.width
            && self.height == other.height
            && self.fit == other.fit
            && self.align == other.align
            && self.vertical_align == other.vertical_align
            && self.background == other.background
            && self.source_height == other.source_height
            && self.crop_top == other.crop_top
    }
}

impl Hash for ImageRenderCacheKey
{
    fn hash<H: Hasher>(&self, state: &mut H)
    {
        self.source.hash(state);
        self.width.hash(state);
        self.height.hash(state);
        self.fit.hash(state);
        self.align.hash(state);
        self.vertical_align.hash(state);
        self.background.hash(state);
        self.source_height.hash(state);
        self.crop_top.hash(state);
    }
}

#[derive(Clone, Debug, Default)]
pub struct ImageRenderInfo
{
    pub source: String,
    pub fit: String,
    pub configured_render_mode: String,
    pub resolved_render_mode: String,
    pub source_loaded: bool,
    pub source_width: i32,
    pub source_height: i32,
    pub element_width: i32,
    pub element_height: i32,
    pub cell_pixel_width: i32,
    pub cell_pixel_height: i32,
    pub image_left: i32,
    pub image_top: i32,
    pub image_width: i32,
    pub image_height: i32,
    pub visible_left: i32,
    pub visible_top: i32,
    pub visible_width: i32,
    pub visible_height: i32,
    pub raw_expected: bool,
    pub raw_present: bool,
    pub sample_signature: Vec<String>,
}

pub fn set_terminal_cell_pixel_size(size: Size)
{
    if size.width > 0 && size.height > 0
    {
        TERMINAL_CELL_PIXEL_WIDTH.store(size.width, Ordering::Relaxed);
        TERMINAL_CELL_PIXEL_HEIGHT.store(size.height, Ordering::Relaxed);
    }
}

fn terminal_cell_pixel_size() -> Size
{
    Size
    {
        width: max(
            MINIMUM_RENDERABLE_SIZE,
            TERMINAL_CELL_PIXEL_WIDTH.load(Ordering::Relaxed),
        ),
        height: max(
            MINIMUM_RENDERABLE_SIZE,
            TERMINAL_CELL_PIXEL_HEIGHT.load(Ordering::Relaxed),
        ),
    }
}

fn raster_cache() -> &'static Mutex<HashMap<String, Arc<ImageRaster>>>
{
    static CACHE: OnceLock<Mutex<HashMap<String, Arc<ImageRaster>>>> = OnceLock::new();
    CACHE.get_or_init(|| Mutex::new(HashMap::new()))
}

fn sixel_cache() -> &'static Mutex<HashMap<ImageRenderCacheKey, String>>
{
    static CACHE: OnceLock<Mutex<HashMap<ImageRenderCacheKey, String>>> = OnceLock::new();
    CACHE.get_or_init(|| Mutex::new(HashMap::new()))
}

#[cfg(target_os = "macos")]
fn sixel_library_names() -> &'static [&'static str]
{
    &["libsixel.1.dylib", "libsixel.dylib"]
}

#[cfg(all(unix, not(target_os = "macos")))]
fn sixel_library_names() -> &'static [&'static str]
{
    &["libsixel.so.1", "libsixel.so"]
}

#[cfg(unix)]
fn append_sixel_search_directory(directories: &mut Vec<PathBuf>, directory: PathBuf)
{
    if directory.as_os_str().is_empty() || directories.iter().any(|known| known == &directory)
    {
        return;
    }
    directories.push(directory);
}

#[cfg(unix)]
fn sixel_search_directories() -> Vec<PathBuf>
{
    let mut directories = Vec::new();
    for variable in ["LD_LIBRARY_PATH", "DYLD_LIBRARY_PATH", "DYLD_FALLBACK_LIBRARY_PATH"]
    {
        if let Some(value) = std::env::var_os(variable)
        {
            for directory in std::env::split_paths(&value)
            {
                append_sixel_search_directory(&mut directories, directory);
            }
        }
    }
    if let Some(prefix) = std::env::var_os("HOMEBREW_PREFIX")
    {
        let prefix = PathBuf::from(prefix);
        append_sixel_search_directory(&mut directories, prefix.join("lib"));
        append_sixel_search_directory(
            &mut directories,
            prefix.join("opt").join("libsixel").join("lib"),
        );
    }
    if let Some(prefix) = std::env::var_os("MACPORTS_PREFIX")
    {
        append_sixel_search_directory(&mut directories, PathBuf::from(prefix).join("lib"));
    }
    if let Ok(executable) = std::env::current_exe()
        && let Some(directory) = executable.parent()
    {
        append_sixel_search_directory(&mut directories, directory.to_path_buf());
        append_sixel_search_directory(&mut directories, directory.join("lib"));
        append_sixel_search_directory(&mut directories, directory.join("../lib"));
        append_sixel_search_directory(&mut directories, directory.join("../../lib"));
    }
    #[cfg(target_os = "macos")]
    for directory in [
        "/opt/homebrew/opt/libsixel/lib",
        "/opt/homebrew/lib",
        "/usr/local/opt/libsixel/lib",
        "/usr/local/lib",
        "/opt/local/lib",
    ]
    {
        append_sixel_search_directory(&mut directories, PathBuf::from(directory));
    }
    #[cfg(not(target_os = "macos"))]
    for directory in [
        "/usr/local/lib",
        "/usr/lib",
        "/usr/lib64",
        "/lib",
        "/lib64",
        "/usr/lib/x86_64-linux-gnu",
        "/usr/lib/aarch64-linux-gnu",
        "/usr/lib/arm-linux-gnueabihf",
    ]
    {
        append_sixel_search_directory(&mut directories, PathBuf::from(directory));
    }
    directories
}

#[cfg(unix)]
fn open_sixel_library() -> Option<*mut c_void>
{
    let open = |path: &Path| -> Option<*mut c_void>
    {
        let path = CString::new(path.to_string_lossy().as_bytes()).ok()?;
        let handle = unsafe
        {
            libc::dlopen(path.as_ptr(), libc::RTLD_LAZY | libc::RTLD_LOCAL)
        };
        (!handle.is_null()).then_some(handle)
    };
    for name in sixel_library_names()
    {
        if let Some(handle) = open(Path::new(name))
        {
            return Some(handle);
        }
    }
    for directory in sixel_search_directories()
    {
        for name in sixel_library_names()
        {
            if let Some(handle) = open(&directory.join(name))
            {
                return Some(handle);
            }
        }
    }
    if let Some(path) = std::env::var_os("UIMD_LIBSIXEL_PATH")
        && let Some(handle) = open(Path::new(&path))
    {
        return Some(handle);
    }
    if let Some(value) = std::env::var_os("UIMD_LIBSIXEL_DIR")
    {
        for directory in std::env::split_paths(&value)
        {
            for name in sixel_library_names()
            {
                if let Some(handle) = open(&directory.join(name))
                {
                    return Some(handle);
                }
            }
        }
    }
    None
}

#[cfg(unix)]
fn load_sixel_api() -> Option<&'static SixelApi>
{
    static API: OnceLock<Option<SixelApi>> = OnceLock::new();
    API.get_or_init(||
    {
        let handle = open_sixel_library()?;
        let symbol = |name: &str| -> Option<*mut c_void>
        {
            let name = CString::new(name).ok()?;
            let value = unsafe { libc::dlsym(handle, name.as_ptr()) };
            (!value.is_null()).then_some(value)
        };
        Some(SixelApi
        {
            output_new: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelOutputNewFunction>(
                    symbol("sixel_output_new")?,
                )
            },
            dither_new: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelDitherNewFunction>(
                    symbol("sixel_dither_new")?,
                )
            },
            dither_initialize: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelDitherInitializeFunction>(
                    symbol("sixel_dither_initialize")?,
                )
            },
            encode: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelEncodeFunction>(
                    symbol("sixel_encode")?,
                )
            },
            output_unref: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelOutputUnrefFunction>(
                    symbol("sixel_output_unref")?,
                )
            },
            dither_unref: unsafe
            {
                std::mem::transmute::<*mut c_void, SixelDitherUnrefFunction>(
                    symbol("sixel_dither_unref")?,
                )
            },
        })
    })
    .as_ref()
}

#[cfg(not(unix))]
fn load_sixel_api() -> Option<&'static ()>
{
    None
}

pub(crate) fn with_image_cell_background_rendering<T>(render: impl FnOnce() -> T) -> T
{
    let _scope = ScopedImageCellBackgroundRendering::new();
    render()
}

fn image_cell_background_rendering_active() -> bool
{
    IMAGE_CELL_BACKGROUND_RENDERING_DEPTH.with(|depth| depth.get() > 0)
}

fn image_fallback_rendering_active() -> bool
{
    IMAGE_FALLBACK_RENDERING_DEPTH.with(|depth| depth.get() > 0)
}

pub fn render_image_element(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    state: ElementRenderState,
)
{
    let fit = normalized_image_value(element.fit(), DEFAULT_IMAGE_FIT);
    let render_mode = normalized_image_value(element.render_mode(), DEFAULT_IMAGE_RENDER_MODE);
    let align = normalized_image_value(element.align(), DEFAULT_IMAGE_ALIGN);
    let vertical_align =
        normalized_image_value(element.vertical_align(), DEFAULT_IMAGE_VERTICAL_ALIGN);
    let force_fallback = image_fallback_rendering_active();
    let force_cell_background = image_cell_background_rendering_active();
    let sixel = !force_fallback
        && !force_cell_background
        && should_render_sixel_for_mode(&render_mode);
    if sixel
    {
        require_sixel_for_image_rendering();
    }
    let raster = load_image_raster(element.source());
    if raster.width <= 0 || raster.height <= 0
    {
        render_image_placeholder(buffer, element, style);
        return;
    }

    let background = fallback_background(style);
    if force_cell_background
    {
        render_cell_background_image(
            buffer,
            &raster,
            &fit,
            &align,
            &vertical_align,
            background,
        );
        return;
    }
    if force_fallback
    {
        render_fallback_image(
            buffer,
            &raster,
            &fit,
            &align,
            &vertical_align,
            background,
        );
        return;
    }
    if deterministic_image_fallback_enabled()
    {
        render_deterministic_image(
            buffer,
            &raster,
            &fit,
            &align,
            &vertical_align,
            background,
            render_mode != "fallback",
        );
        return;
    }
    if sixel
        && render_sixel_image(
            buffer,
            element.source(),
            &raster,
            &fit,
            &align,
            &vertical_align,
            image_background(style),
            state,
        )
    {
        return;
    }
    render_fallback_image(
        buffer,
        &raster,
        &fit,
        &align,
        &vertical_align,
        background,
    );
}

pub fn image_render_info(
    element: &Element,
    size: Size,
    state: ElementRenderState,
) -> ImageRenderInfo
{
    let width = max(MINIMUM_RENDERABLE_SIZE, size.width);
    let height = max(MINIMUM_RENDERABLE_SIZE, size.height);
    let fit = normalized_image_value(element.fit(), DEFAULT_IMAGE_FIT);
    let render_mode =
        normalized_image_value(element.render_mode(), DEFAULT_IMAGE_RENDER_MODE);
    let align = normalized_image_value(element.align(), DEFAULT_IMAGE_ALIGN);
    let vertical_align =
        normalized_image_value(element.vertical_align(), DEFAULT_IMAGE_VERTICAL_ALIGN);
    let style = element.effective_style(state);
    let mut info = ImageRenderInfo
    {
        source: element.source().to_string(),
        fit: fit.clone(),
        configured_render_mode: render_mode.clone(),
        resolved_render_mode: resolved_image_render_mode(&render_mode),
        element_width: width,
        element_height: height,
        cell_pixel_width: terminal_cell_pixel_size().width,
        cell_pixel_height: terminal_cell_pixel_size().height,
        ..ImageRenderInfo::default()
    };
    let raster = load_image_raster(element.source());
    if raster.width <= 0 || raster.height <= 0
    {
        info.resolved_render_mode = "placeholder".to_string();
        return info;
    }

    let region = image_region(width, height, raster.width, raster.height, &fit, &align, &vertical_align);
    let visible_top = max(region.row_offset, state.clip_top.unwrap_or(0));
    let visible_bottom = min(
        region.row_offset + region.rows,
        state.clip_bottom.unwrap_or(height),
    );
    let visible_height = max(0, visible_bottom - visible_top);
    let visible_width = if visible_height > 0 { region.cols } else { 0 };
    let region_fit = if fit == "contain" { "cover" } else { fit.as_str() };
    let signature_rows = if info.resolved_render_mode == "sixel"
    {
        max(MINIMUM_RENDERABLE_SIZE, visible_height)
    }
    else
    {
        region.rows
    };

    info.source_loaded = true;
    info.source_width = raster.width;
    info.source_height = raster.height;
    info.image_left = region.col_offset;
    info.image_top = region.row_offset;
    info.image_width = region.cols;
    info.image_height = region.rows;
    info.visible_left = region.col_offset;
    info.visible_top = visible_top;
    info.visible_width = visible_width;
    info.visible_height = visible_height;
    info.raw_expected = info.resolved_render_mode == "sixel" && visible_height > 0;
    info.raw_present = info.raw_expected;
    info.sample_signature = image_info_sample_signature(
        &raster,
        region.cols,
        max(MINIMUM_RENDERABLE_SIZE, signature_rows),
        region_fit,
        &align,
        &vertical_align,
        fallback_background(&style),
    );
    info
}

fn render_image_placeholder(buffer: &mut TerminalBuffer, element: &Element, style: &Style)
{
    let foreground = style.color.clone();
    let background = style.background.clone();
    let label = if !element.alt().is_empty()
    {
        element.alt().to_string()
    }
    else
    {
        Path::new(element.source())
            .file_name()
            .and_then(|name| name.to_str())
            .filter(|name| !name.is_empty())
            .unwrap_or(MISSING_IMAGE_PLACEHOLDER)
            .to_string()
    };
    let row = max(0, (buffer.height - 1) / 2);
    let col = max(0, (buffer.width - label.chars().count() as i32) / 2);
    buffer.draw_text(row, col, &label, &foreground, &background, buffer.width - col);
}

#[allow(clippy::too_many_arguments)]
fn render_deterministic_image(
    buffer: &mut TerminalBuffer,
    raster: &ImageRaster,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
    checker: bool,
)
{
    let region = image_region(
        buffer.width,
        buffer.height,
        raster.width,
        raster.height,
        fit,
        align,
        vertical_align,
    );
    let region_fit = if fit == "contain" { "cover" } else { fit };
    let sample_height = region.rows * IMAGE_FALLBACK_VERTICAL_SAMPLES;
    for row in region.row_offset..region.row_offset + region.rows
    {
        if row < 0 || row >= buffer.height
        {
            continue;
        }
        for col in region.col_offset..region.col_offset + region.cols
        {
            if col < 0 || col >= buffer.width
            {
                continue;
            }
            let sample_col = col - region.col_offset;
            let sample_top =
                (row - region.row_offset) * IMAGE_FALLBACK_VERTICAL_SAMPLES;
            let top = fallback_sample_color(
                raster,
                sample_col,
                sample_top,
                region.cols,
                sample_height,
                region_fit,
                align,
                vertical_align,
                background,
                checker,
            );
            let bottom = fallback_sample_color(
                raster,
                sample_col,
                sample_top + 1,
                region.cols,
                sample_height,
                region_fit,
                align,
                vertical_align,
                background,
                checker,
            );
            if let Some(cell) = buffer.cell_mut(row, col)
            {
                cell.text = IMAGE_FALLBACK_UPPER_HALF_BLOCK.to_string();
                cell.foreground = Some(color_from_image_rgb(top));
                cell.background = Some(color_from_image_rgb(bottom));
                cell.raw.clear();
                cell.raw_skip = false;
            }
        }
    }
}

#[allow(clippy::too_many_arguments)]
fn render_fallback_image(
    buffer: &mut TerminalBuffer,
    raster: &ImageRaster,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
)
{
    let region = image_region(
        buffer.width,
        buffer.height,
        raster.width,
        raster.height,
        fit,
        align,
        vertical_align,
    );
    let region_fit = if fit == "contain" { "cover" } else { fit };
    let resized = resize_image_raster(
        raster,
        region.cols,
        region.rows * IMAGE_FALLBACK_VERTICAL_SAMPLES,
        region_fit,
        align,
        vertical_align,
        background,
    );
    if resized.width <= 0 || resized.height <= 0
    {
        return;
    }
    for row in 0..region.rows
    {
        let target_row = region.row_offset + row;
        if target_row < 0 || target_row >= buffer.height
        {
            continue;
        }
        let top_row = row * IMAGE_FALLBACK_VERTICAL_SAMPLES;
        let bottom_row = min(resized.height - 1, top_row + 1);
        for col in 0..region.cols
        {
            let target_col = region.col_offset + col;
            if target_col < 0 || target_col >= buffer.width
            {
                continue;
            }
            if let Some(cell) = buffer.cell_mut(target_row, target_col)
            {
                cell.text = IMAGE_FALLBACK_UPPER_HALF_BLOCK.to_string();
                cell.foreground = Some(color_from_image_rgb(
                    pixel_at_image(&resized, col, top_row),
                ));
                cell.background = Some(color_from_image_rgb(
                    pixel_at_image(&resized, col, bottom_row),
                ));
                cell.raw.clear();
                cell.raw_skip = false;
            }
        }
    }
}

fn render_cell_background_image(
    buffer: &mut TerminalBuffer,
    raster: &ImageRaster,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
)
{
    let region = image_region(
        buffer.width,
        buffer.height,
        raster.width,
        raster.height,
        fit,
        align,
        vertical_align,
    );
    let region_fit = if fit == "contain" { "cover" } else { fit };
    let resized = resize_image_raster(
        raster,
        region.cols,
        region.rows,
        region_fit,
        align,
        vertical_align,
        background,
    );
    for row in 0..region.rows
    {
        let target_row = region.row_offset + row;
        if target_row < 0 || target_row >= buffer.height
        {
            continue;
        }
        for col in 0..region.cols
        {
            let target_col = region.col_offset + col;
            if target_col < 0 || target_col >= buffer.width
            {
                continue;
            }
            let color = color_from_image_rgb(pixel_at_image(&resized, col, row));
            if let Some(cell) = buffer.cell_mut(target_row, target_col)
            {
                cell.text = IMAGE_FALLBACK_FULL_BLOCK.to_string();
                cell.foreground = Some(color.clone());
                cell.background = Some(color);
                cell.raw.clear();
                cell.raw_skip = false;
            }
        }
    }
}

#[allow(clippy::too_many_arguments)]
fn render_sixel_image(
    buffer: &mut TerminalBuffer,
    source_path: &str,
    raster: &ImageRaster,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
    state: ElementRenderState,
) -> bool
{
    let region = image_region(
        buffer.width,
        buffer.height,
        raster.width,
        raster.height,
        fit,
        align,
        vertical_align,
    );
    let visible_top = max(region.row_offset, state.clip_top.unwrap_or(0));
    let visible_bottom = min(
        region.row_offset + region.rows,
        state.clip_bottom.unwrap_or(buffer.height),
    );
    let visible_rows = max(0, visible_bottom - visible_top);
    if visible_rows <= 0
    {
        return false;
    }
    let region_fit = if fit == "contain" { "cover" } else { fit };
    let raw = cached_sixel_image_payload(
        source_path,
        raster,
        region.cols,
        visible_rows,
        region_fit,
        align,
        vertical_align,
        background,
        region.rows,
        visible_top - region.row_offset,
    );
    if raw.is_empty()
    {
        return false;
    }

    for row in visible_top..visible_bottom
    {
        for col in region.col_offset..region.col_offset + region.cols
        {
            if let Some(cell) = buffer.cell_mut(row, col)
            {
                cell.raw.clear();
                cell.raw_skip = true;
            }
        }
    }
    let Some(anchor) = buffer.cell_mut(visible_top, region.col_offset) else
    {
        return false;
    };
    anchor.raw = raw;
    anchor.raw_width = region.cols;
    anchor.raw_height = visible_rows;
    anchor.raw_skip = false;
    true
}

fn fallback_background(style: &Style) -> ImageRgb
{
    if let Some(crate::Rgba { red, green, blue, alpha }) =
        style.background.as_ref().and_then(Color::rgba)
        && alpha == u8::MAX
    {
        return ImageRgb
        {
            red: i32::from(red),
            green: i32::from(green),
            blue: i32::from(blue),
        };
    }
    current_element_parent_background()
        .and_then(|color| color.rgba())
        .map_or_else(ImageRgb::default, |crate::Rgba { red, green, blue, .. }|
        {
            ImageRgb
            {
                red: i32::from(red),
                green: i32::from(green),
                blue: i32::from(blue),
            }
        })
}

fn image_background(style: &Style) -> ImageRgb
{
    style.background.as_ref().and_then(Color::rgba).map_or_else(
        ImageRgb::default,
        |crate::Rgba { red, green, blue, .. }|
        {
            ImageRgb
            {
                red: i32::from(red),
                green: i32::from(green),
                blue: i32::from(blue),
            }
        },
    )
}

fn resolved_image_render_mode(render_mode: &str) -> String
{
    if image_cell_background_rendering_active()
    {
        "cell_background".to_string()
    }
    else if deterministic_image_fallback_enabled()
    {
        "deterministic".to_string()
    }
    else if !image_fallback_rendering_active()
        && should_render_sixel_for_mode(render_mode)
    {
        "sixel".to_string()
    }
    else
    {
        "fallback".to_string()
    }
}

fn should_render_sixel_for_mode(render_mode: &str) -> bool
{
    matches!(render_mode, "sixel" | "auto") && terminal_supports_sixel()
}

pub fn image_mode_needs_sixel_fallback_warning(render_mode: &str) -> bool
{
    if deterministic_image_fallback_enabled()
        || truthy_environment("UIMD_DISABLE_SIXEL")
        || truthy_environment("UIMD_FORCE_SIXEL")
    {
        return false;
    }
    let render_mode = normalized_image_value(render_mode, DEFAULT_IMAGE_RENDER_MODE);
    render_mode != "fallback" && !terminal_supports_sixel()
}

pub fn require_sixel_for_image_rendering()
{
    if deterministic_image_fallback_enabled() || truthy_environment("UIMD_DISABLE_SIXEL")
    {
        return;
    }
    assert!(load_sixel_api().is_some(), "{SIXEL_UNAVAILABLE_MESSAGE}");
}

fn terminal_supports_sixel() -> bool
{
    if truthy_environment("UIMD_FORCE_SIXEL")
    {
        return true;
    }
    if truthy_environment("UIMD_DISABLE_SIXEL")
    {
        return false;
    }
    let term_program = normalized_environment("TERM_PROGRAM");
    let term = normalized_environment("TERM");
    let color_term = normalized_environment("COLORTERM");
    let iterm_session = normalized_environment("ITERM_SESSION_ID");
    let lc_terminal = normalized_environment("LC_TERMINAL");
    if term_program.contains("apple_terminal")
    {
        return false;
    }
    if !iterm_session.is_empty() || lc_terminal.contains("iterm")
    {
        return true;
    }
    if ["iterm", "wezterm", "mlterm", "foot", "contour"]
        .iter()
        .any(|token| term_program.contains(token))
    {
        return true;
    }
    if term.contains("sixel") || color_term.contains("sixel")
    {
        return true;
    }
    ["mlterm", "foot", "contour"]
        .iter()
        .any(|token| term.contains(token))
}

fn deterministic_image_fallback_enabled() -> bool
{
    truthy_environment("UIMD_DETERMINISTIC_IMAGE_FALLBACK")
}

fn truthy_environment(name: &str) -> bool
{
    matches!(
        normalized_environment(name).as_str(),
        "1" | "true" | "yes" | "on"
    )
}

fn normalized_environment(name: &str) -> String
{
    std::env::var(name)
        .unwrap_or_default()
        .trim()
        .to_ascii_lowercase()
}

pub(crate) fn normalized_image_value(value: &str, fallback: &str) -> String
{
    let value = value.trim().to_ascii_lowercase();
    if value.is_empty() { fallback.to_string() } else { value }
}

fn load_image_raster(path: &str) -> Arc<ImageRaster>
{
    if let Some(cached) = raster_cache()
        .lock()
        .expect("image raster cache lock poisoned")
        .get(path)
        .cloned()
    {
        return cached;
    }
    let raster = fs::read(path)
        .ok()
        .and_then(|data| decode_image(&data))
        .unwrap_or_default();
    let raster = Arc::new(raster);
    raster_cache()
        .lock()
        .expect("image raster cache lock poisoned")
        .insert(path.to_string(), raster.clone());
    raster
}

fn decode_image(data: &[u8]) -> Option<ImageRaster>
{
    let length = c_int::try_from(data.len()).ok()?;
    let mut width = 0;
    let mut height = 0;
    let mut source_channels = 0;
    let decoded = StbiImage(unsafe
    {
        uimd_stbi_load_rgba_from_memory(
            data.as_ptr(),
            length,
            &mut width,
            &mut height,
            &mut source_channels,
        )
    });
    if decoded.0.is_null() || width <= 0 || height <= 0
    {
        return None;
    }
    let byte_count = usize::try_from(width)
        .ok()?
        .checked_mul(usize::try_from(height).ok()?)?
        .checked_mul(4)?;
    let bytes = unsafe { std::slice::from_raw_parts(decoded.0, byte_count) };
    let pixel_count = byte_count / 4;
    let mut raster = ImageRaster
    {
        width,
        height,
        pixels: Vec::with_capacity(pixel_count),
        alpha: Vec::with_capacity(pixel_count),
    };
    for components in bytes.chunks_exact(4)
    {
        raster.pixels.push(ImageRgb
        {
            red: i32::from(components[0]),
            green: i32::from(components[1]),
            blue: i32::from(components[2]),
        });
        raster.alpha.push(i32::from(components[3]));
    }
    Some(raster)
}

fn image_region(
    width: i32,
    height: i32,
    source_width: i32,
    source_height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
) -> ImageCellRegion
{
    if fit != "contain" || source_width <= 0 || source_height <= 0
    {
        return ImageCellRegion { cols: width, rows: height, ..ImageCellRegion::default() };
    }
    let cell_pixels = terminal_cell_pixel_size();
    let canvas_width = f64::from(width * cell_pixels.width);
    let canvas_height = f64::from(height * cell_pixels.height);
    let scale = (canvas_width / f64::from(source_width))
        .min(canvas_height / f64::from(source_height));
    let cols = ((f64::from(source_width) * scale / f64::from(cell_pixels.width)).round()
        as i32)
        .clamp(MINIMUM_RENDERABLE_SIZE, width);
    let rows = ((f64::from(source_height) * scale / f64::from(cell_pixels.height)).round()
        as i32)
        .clamp(MINIMUM_RENDERABLE_SIZE, height);
    ImageCellRegion
    {
        cols,
        rows,
        col_offset: alignment_offset(f64::from(width), f64::from(cols), align, "left", "right")
            as i32,
        row_offset: alignment_offset(
            f64::from(height),
            f64::from(rows),
            vertical_align,
            "top",
            "bottom",
        ) as i32,
    }
}

#[allow(clippy::too_many_arguments)]
fn fallback_sample_color(
    raster: &ImageRaster,
    x: i32,
    y: i32,
    target_width: i32,
    target_height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
    checker: bool,
) -> ImageRgb
{
    let target_width = max(MINIMUM_RENDERABLE_SIZE, target_width);
    let target_height = max(MINIMUM_RENDERABLE_SIZE, target_height);
    let x = x.clamp(0, target_width - 1);
    let y = y.clamp(0, target_height - 1);
    let mut base = background;
    if checker
    {
        base = fallback_checker_tint(base, x, y);
    }
    if raster.width <= 0 || raster.height <= 0 || raster.pixels.is_empty()
    {
        return quantize_fallback_color(base);
    }
    let (source_x, source_y) = if fit == "stretch"
    {
        (
            (f64::from(x) * f64::from(raster.width) / f64::from(target_width)) as i32,
            (f64::from(y) * f64::from(raster.height) / f64::from(target_height)) as i32,
        )
    }
    else
    {
        let mut scale = (f64::from(target_width) / f64::from(raster.width))
            .min(f64::from(target_height) / f64::from(raster.height));
        if fit == "cover"
        {
            scale = (f64::from(target_width) / f64::from(raster.width))
                .max(f64::from(target_height) / f64::from(raster.height));
        }
        let drawn_width = f64::from(raster.width) * scale;
        let drawn_height = f64::from(raster.height) * scale;
        let x_offset =
            alignment_offset(f64::from(target_width), drawn_width, align, "left", "right");
        let y_offset = alignment_offset(
            f64::from(target_height),
            drawn_height,
            vertical_align,
            "top",
            "bottom",
        );
        let source_x = (f64::from(x) - x_offset) / scale;
        let source_y = (f64::from(y) - y_offset) / scale;
        if source_x < 0.0
            || source_y < 0.0
            || source_x >= f64::from(raster.width)
            || source_y >= f64::from(raster.height)
        {
            return quantize_fallback_color(base);
        }
        (source_x as i32, source_y as i32)
    };
    let source_x = source_x.clamp(0, raster.width - 1);
    let source_y = source_y.clamp(0, raster.height - 1);
    let mut pixel = blend_image_rgb(
        pixel_at_image(raster, source_x, source_y),
        alpha_at_image(raster, source_x, source_y),
        base,
    );
    if checker
    {
        pixel = fallback_checker_tint(pixel, x, y);
    }
    quantize_fallback_color(pixel)
}

fn fallback_checker_tint(color: ImageRgb, x: i32, y: i32) -> ImageRgb
{
    let alpha = if ((x / IMAGE_FALLBACK_CHECKER_TILE_PIXELS)
        + (y / IMAGE_FALLBACK_CHECKER_TILE_PIXELS))
        % 2
        == 0
    {
        IMAGE_FALLBACK_CHECKER_LIGHT_ALPHA
    }
    else
    {
        IMAGE_FALLBACK_CHECKER_DARK_ALPHA
    };
    blend_image_rgb(
        ImageRgb
        {
            red: IMAGE_CHANNEL_MAX,
            green: IMAGE_CHANNEL_MAX,
            blue: IMAGE_CHANNEL_MAX,
        },
        alpha,
        color,
    )
}

fn blend_image_rgb(source: ImageRgb, alpha: i32, destination: ImageRgb) -> ImageRgb
{
    let alpha = alpha.clamp(0, IMAGE_CHANNEL_MAX);
    let blend = |source: i32, destination: i32|
    {
        (source * alpha + destination * (IMAGE_CHANNEL_MAX - alpha)) / IMAGE_CHANNEL_MAX
    };
    ImageRgb
    {
        red: blend(source.red, destination.red),
        green: blend(source.green, destination.green),
        blue: blend(source.blue, destination.blue),
    }
}

fn quantize_fallback_color(color: ImageRgb) -> ImageRgb
{
    let quantize = |channel: i32|
    {
        ((channel / IMAGE_FALLBACK_COLOR_QUANTUM) * IMAGE_FALLBACK_COLOR_QUANTUM)
            .clamp(0, IMAGE_CHANNEL_MAX)
    };
    ImageRgb
    {
        red: quantize(color.red),
        green: quantize(color.green),
        blue: quantize(color.blue),
    }
}

fn alignment_offset(
    outer: f64,
    inner: f64,
    value: &str,
    start_value: &str,
    end_value: &str,
) -> f64
{
    let span = (outer - inner).max(0.0);
    match value.trim().to_ascii_lowercase().as_str()
    {
        value if value == start_value => 0.0,
        value if value == end_value => span,
        _ => span / 2.0,
    }
}

#[allow(clippy::too_many_arguments)]
fn resize_image_raster(
    source: &ImageRaster,
    target_width: i32,
    target_height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
) -> ImageRaster
{
    let target_width = max(MINIMUM_RENDERABLE_SIZE, target_width);
    let target_height = max(MINIMUM_RENDERABLE_SIZE, target_height);
    if source.width <= 0 || source.height <= 0 || source.pixels.is_empty()
    {
        return ImageRaster::default();
    }
    let mut result = ImageRaster
    {
        width: target_width,
        height: target_height,
        pixels: vec![background; (target_width * target_height) as usize],
        alpha: vec![IMAGE_CHANNEL_MAX; (target_width * target_height) as usize],
    };
    let fit = normalized_image_value(fit, DEFAULT_IMAGE_FIT);
    let stretch = fit == "stretch";
    let cover = fit == "cover";
    let mut scale = 0.0;
    if !stretch
    {
        scale = (f64::from(target_width) / f64::from(source.width))
            .min(f64::from(target_height) / f64::from(source.height));
        if cover
        {
            scale = (f64::from(target_width) / f64::from(source.width))
                .max(f64::from(target_height) / f64::from(source.height));
        }
    }
    let (drawn_width, drawn_height) = if stretch
    {
        (f64::from(target_width), f64::from(target_height))
    }
    else
    {
        (
            (f64::from(source.width) * scale).max(1.0),
            (f64::from(source.height) * scale).max(1.0),
        )
    };
    let (x_offset, y_offset) = if stretch
    {
        (0.0, 0.0)
    }
    else if cover
    {
        (
            alignment_offset(drawn_width, f64::from(target_width), align, "left", "right"),
            alignment_offset(
                drawn_height,
                f64::from(target_height),
                vertical_align,
                "top",
                "bottom",
            ),
        )
    }
    else
    {
        (
            alignment_offset(f64::from(target_width), drawn_width, align, "left", "right"),
            alignment_offset(
                f64::from(target_height),
                drawn_height,
                vertical_align,
                "top",
                "bottom",
            ),
        )
    };
    for y in 0..target_height
    {
        for x in 0..target_width
        {
            let (left, right, top, bottom) = if stretch
            {
                (
                    f64::from(x) * f64::from(source.width) / f64::from(target_width),
                    f64::from(x + 1) * f64::from(source.width) / f64::from(target_width),
                    f64::from(y) * f64::from(source.height) / f64::from(target_height),
                    f64::from(y + 1) * f64::from(source.height) / f64::from(target_height),
                )
            }
            else if cover
            {
                (
                    (f64::from(x) + x_offset) / scale,
                    (f64::from(x + 1) + x_offset) / scale,
                    (f64::from(y) + y_offset) / scale,
                    (f64::from(y + 1) + y_offset) / scale,
                )
            }
            else
            {
                (
                    (f64::from(x) - x_offset) / scale,
                    (f64::from(x + 1) - x_offset) / scale,
                    (f64::from(y) - y_offset) / scale,
                    (f64::from(y + 1) - y_offset) / scale,
                )
            };
            result.pixels[(y * target_width + x) as usize] =
                sample_raster_area(source, left, top, right, bottom, background);
        }
    }
    result
}

fn sample_raster_area(
    source: &ImageRaster,
    left: f64,
    top: f64,
    right: f64,
    bottom: f64,
    background: ImageRgb,
) -> ImageRgb
{
    let full_area = ((right - left) * (bottom - top)).max(IMAGE_MINIMUM_SAMPLE_AREA);
    if source.width <= 0
        || source.height <= 0
        || source.pixels.is_empty()
        || right <= 0.0
        || bottom <= 0.0
        || left >= f64::from(source.width)
        || top >= f64::from(source.height)
    {
        return background;
    }
    let mut red = f64::from(background.red) * full_area;
    let mut green = f64::from(background.green) * full_area;
    let mut blue = f64::from(background.blue) * full_area;
    let start_x = max(0, left.floor() as i32);
    let end_x = min(source.width, right.ceil() as i32);
    let start_y = max(0, top.floor() as i32);
    let end_y = min(source.height, bottom.ceil() as i32);
    for y in start_y..end_y
    {
        let y_weight = interval_overlap(top, bottom, f64::from(y), f64::from(y + 1));
        if y_weight <= 0.0
        {
            continue;
        }
        for x in start_x..end_x
        {
            let x_weight = interval_overlap(left, right, f64::from(x), f64::from(x + 1));
            if x_weight <= 0.0
            {
                continue;
            }
            let weight = x_weight * y_weight;
            let alpha =
                f64::from(alpha_at_image(source, x, y)) / f64::from(IMAGE_CHANNEL_MAX);
            let pixel = pixel_at_image(source, x, y);
            red += (f64::from(pixel.red) - f64::from(background.red)) * alpha * weight;
            green += (f64::from(pixel.green) - f64::from(background.green)) * alpha * weight;
            blue += (f64::from(pixel.blue) - f64::from(background.blue)) * alpha * weight;
        }
    }
    ImageRgb
    {
        red: image_channel(red / full_area),
        green: image_channel(green / full_area),
        blue: image_channel(blue / full_area),
    }
}

fn interval_overlap(first_start: f64, first_end: f64, second_start: f64, second_end: f64) -> f64
{
    (first_end.min(second_end) - first_start.max(second_start)).max(0.0)
}

fn image_channel(value: f64) -> i32
{
    (value.round() as i32).clamp(0, IMAGE_CHANNEL_MAX)
}

fn pixel_at_image(raster: &ImageRaster, x: i32, y: i32) -> ImageRgb
{
    if raster.width <= 0 || raster.height <= 0 || raster.pixels.is_empty()
    {
        return ImageRgb::default();
    }
    let x = x.clamp(0, raster.width - 1);
    let y = y.clamp(0, raster.height - 1);
    raster.pixels[(y * raster.width + x) as usize]
}

fn alpha_at_image(raster: &ImageRaster, x: i32, y: i32) -> i32
{
    if raster.alpha.is_empty()
    {
        return IMAGE_CHANNEL_MAX;
    }
    let x = x.clamp(0, raster.width - 1);
    let y = y.clamp(0, raster.height - 1);
    raster.alpha[(y * raster.width + x) as usize]
}

fn color_from_image_rgb(color: ImageRgb) -> Color
{
    Color::new(format!(
        "#{:02x}{:02x}{:02x}",
        color.red.clamp(0, IMAGE_CHANNEL_MAX),
        color.green.clamp(0, IMAGE_CHANNEL_MAX),
        color.blue.clamp(0, IMAGE_CHANNEL_MAX),
    ))
}

fn image_rgb_hex(color: ImageRgb) -> String
{
    format!(
        "#{:02x}{:02x}{:02x}",
        color.red.clamp(0, IMAGE_CHANNEL_MAX),
        color.green.clamp(0, IMAGE_CHANNEL_MAX),
        color.blue.clamp(0, IMAGE_CHANNEL_MAX),
    )
}

fn image_info_sample_signature(
    raster: &ImageRaster,
    target_width: i32,
    target_height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
) -> Vec<String>
{
    let target_width = max(MINIMUM_RENDERABLE_SIZE, target_width);
    let target_height = max(MINIMUM_RENDERABLE_SIZE, target_height);
    let fitted = resize_image_raster(
        raster,
        target_width,
        target_height,
        fit,
        align,
        vertical_align,
        background,
    );
    let mut signature =
        Vec::with_capacity((IMAGE_INFO_SAMPLE_GRID_SIZE * IMAGE_INFO_SAMPLE_GRID_SIZE) as usize);
    for row in image_info_sample_positions(target_height)
    {
        for col in image_info_sample_positions(target_width)
        {
            let color = pixel_at_image(&fitted, col, row);
            signature.push(image_rgb_hex(ImageRgb
            {
                red: quantize_image_info_channel(color.red),
                green: quantize_image_info_channel(color.green),
                blue: quantize_image_info_channel(color.blue),
            }));
        }
    }
    signature
}

fn image_info_sample_positions(extent: i32) -> Vec<i32>
{
    let extent = max(MINIMUM_RENDERABLE_SIZE, extent);
    if IMAGE_INFO_SAMPLE_GRID_SIZE == MINIMUM_RENDERABLE_SIZE
    {
        return vec![extent / 2];
    }
    (0..IMAGE_INFO_SAMPLE_GRID_SIZE)
        .map(|index|
        {
            (f64::from(index) * f64::from(extent - 1)
                / f64::from(IMAGE_INFO_SAMPLE_GRID_SIZE - 1))
                .round() as i32
        })
        .map(|position| position.clamp(0, extent - 1))
        .collect()
}

fn quantize_image_info_channel(value: i32) -> i32
{
    ((value / IMAGE_INFO_COLOR_QUANTUM) * IMAGE_INFO_COLOR_QUANTUM)
        .clamp(0, IMAGE_CHANNEL_MAX)
}

fn sixel_status_succeeded(status: i32) -> bool
{
    status & IMAGE_SIXEL_FALSE_STATUS_MASK == 0
}

#[cfg(unix)]
unsafe extern "C" fn libsixel_write(
    data: *mut c_char,
    size: c_int,
    private: *mut c_void,
) -> c_int
{
    if data.is_null() || private.is_null() || size <= 0
    {
        return 0;
    }
    let output = unsafe { &mut *private.cast::<Vec<u8>>() };
    let bytes = unsafe { std::slice::from_raw_parts(data.cast::<u8>(), size as usize) };
    output.extend_from_slice(bytes);
    size
}

#[cfg(unix)]
fn libsixel_encode(raster: &ImageRaster) -> String
{
    if raster.width <= 0
        || raster.height <= 0
        || raster.pixels.len() < (raster.width * raster.height) as usize
    {
        return String::new();
    }
    let Some(api) = load_sixel_api() else
    {
        return String::new();
    };
    let mut rgb = Vec::with_capacity(raster.pixels.len() * 3);
    for pixel in &raster.pixels
    {
        rgb.push(pixel.red.clamp(0, IMAGE_CHANNEL_MAX) as u8);
        rgb.push(pixel.green.clamp(0, IMAGE_CHANNEL_MAX) as u8);
        rgb.push(pixel.blue.clamp(0, IMAGE_CHANNEL_MAX) as u8);
    }
    let mut bytes = Vec::new();
    let mut output = std::ptr::null_mut();
    let mut dither = std::ptr::null_mut();
    let output_status = unsafe
    {
        (api.output_new)(
            &mut output,
            libsixel_write,
            (&mut bytes as *mut Vec<u8>).cast(),
            std::ptr::null_mut(),
        )
    };
    if !sixel_status_succeeded(output_status)
    {
        return String::new();
    }
    let dither_status = unsafe
    {
        (api.dither_new)(
            &mut dither,
            IMAGE_SIXEL_MAX_COLORS,
            std::ptr::null_mut(),
        )
    };
    if !sixel_status_succeeded(dither_status)
    {
        unsafe { (api.output_unref)(output) };
        return String::new();
    }
    let initialize_status = unsafe
    {
        (api.dither_initialize)(
            dither,
            rgb.as_mut_ptr(),
            raster.width,
            raster.height,
            IMAGE_SIXEL_PIXEL_FORMAT_RGB888,
            IMAGE_SIXEL_LARGE_AUTO,
            IMAGE_SIXEL_REP_AUTO,
            IMAGE_SIXEL_QUALITY_HIGH,
        )
    };
    if sixel_status_succeeded(initialize_status)
    {
        unsafe
        {
            (api.encode)(
                rgb.as_mut_ptr(),
                raster.width,
                raster.height,
                3,
                dither,
                output,
            );
        }
    }
    unsafe
    {
        (api.dither_unref)(dither);
        (api.output_unref)(output);
    }
    String::from_utf8(bytes).unwrap_or_default()
}

#[cfg(not(unix))]
fn libsixel_encode(_raster: &ImageRaster) -> String
{
    String::new()
}

#[allow(clippy::too_many_arguments)]
fn cached_sixel_image_payload(
    source_path: &str,
    source: &ImageRaster,
    width: i32,
    height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
    source_height: i32,
    crop_top: i32,
) -> String
{
    let width = max(MINIMUM_RENDERABLE_SIZE, width);
    let height = max(MINIMUM_RENDERABLE_SIZE, height);
    let source_height = if source_height <= 0 { height } else { source_height };
    let crop_top = max(0, crop_top);
    let key = ImageRenderCacheKey
    {
        source: source_path.to_string(),
        width,
        height,
        fit: fit.to_string(),
        align: align.to_string(),
        vertical_align: vertical_align.to_string(),
        background,
        source_height,
        crop_top,
    };
    if let Some(cached) = sixel_cache()
        .lock()
        .expect("image sixel cache lock poisoned")
        .get(&key)
        .cloned()
    {
        return cached;
    }
    let raw = sixel_image_payload(
        source,
        width,
        height,
        fit,
        align,
        vertical_align,
        background,
        source_height,
        crop_top,
    );
    sixel_cache()
        .lock()
        .expect("image sixel cache lock poisoned")
        .insert(key, raw.clone());
    raw
}

#[allow(clippy::too_many_arguments)]
fn sixel_image_payload(
    source: &ImageRaster,
    width: i32,
    height: i32,
    fit: &str,
    align: &str,
    vertical_align: &str,
    background: ImageRgb,
    source_height: i32,
    crop_top: i32,
) -> String
{
    let cell_pixels = terminal_cell_pixel_size();
    let fitted = resize_image_raster(
        source,
        width * cell_pixels.width,
        source_height * cell_pixels.height,
        fit,
        align,
        vertical_align,
        background,
    );
    let cropped = crop_image_raster_rows(
        &fitted,
        crop_top * cell_pixels.height,
        height * cell_pixels.height,
    );
    let optimized = libsixel_encode(&cropped);
    if optimized.is_empty()
    {
        sixel_payload(&quantize_image_raster(cropped))
    }
    else
    {
        optimized
    }
}

fn crop_image_raster_rows(source: &ImageRaster, top: i32, height: i32) -> ImageRaster
{
    if source.width <= 0 || source.height <= 0 || height <= 0 || source.pixels.is_empty()
    {
        return ImageRaster::default();
    }
    let top = top.clamp(0, source.height);
    let bottom = (top + height).clamp(top, source.height);
    if bottom <= top
    {
        return ImageRaster::default();
    }
    let start = (top * source.width) as usize;
    let end = (bottom * source.width) as usize;
    ImageRaster
    {
        width: source.width,
        height: bottom - top,
        pixels: source.pixels[start..end].to_vec(),
        alpha: if source.alpha.len() >= (source.width * source.height) as usize
        {
            source.alpha[start..end].to_vec()
        }
        else
        {
            Vec::new()
        },
    }
}

fn quantize_image_raster(mut raster: ImageRaster) -> ImageRaster
{
    for color in &mut raster.pixels
    {
        color.red = quantize_sixel_channel(color.red);
        color.green = quantize_sixel_channel(color.green);
        color.blue = quantize_sixel_channel(color.blue);
    }
    raster
}

fn quantize_sixel_channel(value: i32) -> i32
{
    let index = (f64::from(value.clamp(0, IMAGE_CHANNEL_MAX) * (IMAGE_SIXEL_COLOR_LEVELS - 1))
        / f64::from(IMAGE_CHANNEL_MAX))
        .round() as i32;
    (index * IMAGE_CHANNEL_MAX / (IMAGE_SIXEL_COLOR_LEVELS - 1))
        .clamp(0, IMAGE_CHANNEL_MAX)
}

fn sixel_payload(raster: &ImageRaster) -> String
{
    if raster.width <= 0
        || raster.height <= 0
        || raster.pixels.len() < (raster.width * raster.height) as usize
    {
        return String::new();
    }
    let mut color_set = BTreeSet::new();
    for (index, color) in raster.pixels.iter().enumerate()
    {
        if raster.alpha.get(index).is_some_and(|alpha| *alpha <= 0)
        {
            continue;
        }
        color_set.insert(*color);
    }
    let colors: Vec<ImageRgb> = color_set.into_iter().collect();
    let mut output = String::from(IMAGE_SIXEL_INTRODUCER);
    for (index, color) in colors.iter().enumerate()
    {
        output.push_str(&format!(
            "#{index};2;{};{};{}",
            sixel_color_component(color.red),
            sixel_color_component(color.green),
            sixel_color_component(color.blue),
        ));
    }
    for y in (0..raster.height).step_by(IMAGE_SIXEL_BITS_PER_GLYPH as usize)
    {
        for (color_index, color) in colors.iter().enumerate()
        {
            let mut run = String::new();
            let mut previous = '\0';
            let mut count = 0usize;
            let mut has_pixels = false;
            for x in 0..raster.width
            {
                let mut bits = 0i32;
                for bit in 0..IMAGE_SIXEL_BITS_PER_GLYPH
                {
                    let pixel_y = y + bit;
                    if pixel_y >= raster.height
                    {
                        continue;
                    }
                    let pixel_index = (pixel_y * raster.width + x) as usize;
                    if raster.alpha.get(pixel_index).is_some_and(|alpha| *alpha <= 0)
                    {
                        continue;
                    }
                    if raster.pixels[pixel_index] == *color
                    {
                        bits |= 1 << bit;
                    }
                }
                if bits != 0
                {
                    has_pixels = true;
                }
                let character = char::from_u32((63 + bits) as u32).unwrap_or('?');
                if count > 0 && character == previous
                {
                    count += 1;
                    continue;
                }
                append_sixel_run(&mut run, previous, count);
                previous = character;
                count = 1;
            }
            if !has_pixels
            {
                continue;
            }
            append_sixel_run(&mut run, previous, count);
            output.push_str(&format!("#{color_index}{run}$"));
        }
        output.push('-');
    }
    output.push_str(IMAGE_SIXEL_TERMINATOR);
    output
}

fn sixel_color_component(value: i32) -> i32
{
    (f64::from(value.clamp(0, IMAGE_CHANNEL_MAX) * IMAGE_SIXEL_COLOR_COMPONENT_SCALE)
        / f64::from(IMAGE_CHANNEL_MAX))
        .round() as i32
}

fn append_sixel_run(output: &mut String, character: char, count: usize)
{
    if character == '\0' || count == 0
    {
        return;
    }
    if count >= IMAGE_SIXEL_RUN_LENGTH_THRESHOLD
    {
        output.push_str(&format!("!{count}{character}"));
    }
    else
    {
        output.extend(std::iter::repeat_n(character, count));
    }
}

#[cfg(test)]
mod tests
{
    use super::*;
    use crate::new_image;

    fn test_raster() -> ImageRaster
    {
        ImageRaster
        {
            width: 2,
            height: 2,
            pixels: vec![
                ImageRgb { red: 255, green: 0, blue: 0 },
                ImageRgb { red: 0, green: 255, blue: 0 },
                ImageRgb { red: 0, green: 0, blue: 255 },
                ImageRgb { red: 255, green: 255, blue: 255 },
            ],
            alpha: vec![255; 4],
        }
    }

    fn psd_fixture() -> Vec<u8>
    {
        vec![
            b'8', b'B', b'P', b'S',
            0, 1,
            0, 0, 0, 0, 0, 0,
            0, 4,
            0, 0, 0, 1,
            0, 0, 0, 1,
            0, 8,
            0, 3,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0,
            12, 34, 56, 255,
        ]
    }

    fn pic_fixture() -> Vec<u8>
    {
        let mut data = vec![0x53, 0x80, 0xf6, 0x34];
        data.extend(std::iter::repeat_n(0, 84));
        data.extend_from_slice(b"PICT");
        data.extend_from_slice(&[0, 1, 0, 1]);
        data.extend_from_slice(&[0, 0, 0, 0, 0, 0, 0, 0]);
        data.extend_from_slice(&[0, 8, 0, 0xe0]);
        data.extend_from_slice(&[12, 34, 56]);
        data
    }

    fn rgba_tga_fixture() -> Vec<u8>
    {
        let mut data = vec![0; 18];
        data[2] = 2;
        data[12] = 1;
        data[14] = 1;
        data[16] = 32;
        data[17] = 0x28;
        data.extend_from_slice(&[56, 34, 12, 78]);
        data
    }

    #[test]
    fn deterministic_fallback_keeps_top_sample_in_foreground()
    {
        let mut buffer = TerminalBuffer::new(1, 1);
        render_deterministic_image(
            &mut buffer,
            &test_raster(),
            "stretch",
            "center",
            "middle",
            ImageRgb::default(),
            false,
        );
        let cell = buffer.cell(0, 0).expect("fallback cell");
        assert_eq!(cell.text, IMAGE_FALLBACK_UPPER_HALF_BLOCK);
        assert_eq!(cell.foreground.normalized(), "#e00000");
        assert_eq!(cell.background.normalized(), "#0000e0");
    }

    #[test]
    fn normal_fallback_uses_reference_area_sampling_without_test_quantization()
    {
        let mut buffer = TerminalBuffer::new(1, 1);
        render_fallback_image(
            &mut buffer,
            &test_raster(),
            "stretch",
            "center",
            "middle",
            ImageRgb::default(),
        );
        let cell = buffer.cell(0, 0).expect("fallback cell");
        assert_eq!(cell.text, IMAGE_FALLBACK_UPPER_HALF_BLOCK);
        assert_eq!(cell.foreground.normalized(), "#808000");
        assert_eq!(cell.background.normalized(), "#8080ff");
    }

    #[test]
    fn image_setters_and_stb_decoding_match_reference_contracts()
    {
        let image = new_image(
            "test",
            "",
            "",
            " COVER ",
            " SIXEL ",
            " LEFT ",
            " TOP ",
        );
        assert_eq!(image.fit(), "cover");
        assert_eq!(image.render_mode(), "sixel");
        assert_eq!(image.align(), "left");
        assert_eq!(image.vertical_align(), "top");
        image.set_fit("");
        image.set_render_mode("");
        image.set_align("");
        image.set_vertical_align("");
        assert_eq!(image.fit(), DEFAULT_IMAGE_FIT);
        assert_eq!(image.render_mode(), DEFAULT_IMAGE_RENDER_MODE);
        assert_eq!(image.align(), DEFAULT_IMAGE_ALIGN);
        assert_eq!(image.vertical_align(), DEFAULT_IMAGE_VERTICAL_ALIGN);

        assert!(decode_image(b"not an image").is_none());
        assert!(
            decode_image(b"P3\n2 1\n255\n255 0 0 0 255 0\n").is_none(),
            "stb_image intentionally accepts only binary PNM like C++",
        );

        let binary = decode_image(b"P6\n2 1\n255\n\xff\x00\x00\x00\xff\x00")
            .expect("binary PNM through stb");
        assert_eq!(binary.width, 2);
        assert_eq!(binary.height, 1);
        assert_eq!(binary.pixels[0], ImageRgb { red: 255, green: 0, blue: 0 });
        assert_eq!(
            binary.pixels[1],
            ImageRgb { red: 0, green: 255, blue: 0 },
        );

        let psd = decode_image(&psd_fixture()).expect("PSD through stb");
        assert_eq!(psd.pixels, vec![ImageRgb { red: 12, green: 34, blue: 56 }]);
        assert_eq!(psd.alpha, vec![255]);

        let hdr = decode_image(
            b"#?RADIANCE\nFORMAT=32-bit_rle_rgbe\n\n-Y 1 +X 1\n\x80\x40\x20\x81",
        )
        .expect("HDR through stb");
        assert_eq!((hdr.width, hdr.height, hdr.pixels.len()), (1, 1, 1));

        let pic = decode_image(&pic_fixture()).expect("PIC through stb");
        assert_eq!(pic.pixels, vec![ImageRgb { red: 12, green: 34, blue: 56 }]);
        assert_eq!(pic.alpha, vec![255]);

        let tga = decode_image(&rgba_tga_fixture()).expect("RGBA TGA through stb");
        assert_eq!(tga.pixels, vec![ImageRgb { red: 12, green: 34, blue: 56 }]);
        assert_eq!(tga.alpha, vec![78]);
    }

    #[test]
    fn contain_region_uses_terminal_cell_aspect_ratio()
    {
        assert_eq!(
            image_region(20, 10, 384, 303, "contain", "center", "middle"),
            ImageCellRegion { cols: 20, rows: 8, col_offset: 0, row_offset: 1 },
        );
    }

    #[test]
    fn sixel_encoder_returns_complete_payload()
    {
        let payload = sixel_payload(&quantize_image_raster(test_raster()));
        assert!(payload.starts_with(IMAGE_SIXEL_INTRODUCER));
        assert!(payload.ends_with(IMAGE_SIXEL_TERMINATOR));
    }

    #[test]
    fn sixel_visible_region_clips_top_bottom_and_both_edges()
    {
        let cases = [
            (Some(2), Some(8), 2, 6),
            (Some(0), Some(5), 0, 5),
            (Some(2), Some(6), 2, 4),
        ];
        for (clip_top, clip_bottom, expected_anchor_row, expected_rows) in cases
        {
            let mut buffer = TerminalBuffer::new(4, 8);
            assert!(render_sixel_image(
                &mut buffer,
                "test-raster",
                &test_raster(),
                "stretch",
                "center",
                "middle",
                ImageRgb::default(),
                ElementRenderState
                {
                    clip_top,
                    clip_bottom,
                    ..ElementRenderState::default()
                },
            ));

            let anchor = buffer
                .cell(expected_anchor_row, 0)
                .expect("clipped Sixel anchor");
            assert!(anchor.raw.starts_with(IMAGE_SIXEL_INTRODUCER));
            assert_eq!(anchor.raw_width, 4);
            assert_eq!(anchor.raw_height, expected_rows);
            for row in 0..expected_anchor_row
            {
                assert!((0..buffer.width).all(|col|
                {
                    buffer
                        .cell(row, col)
                        .is_some_and(|cell| cell.raw.is_empty() && !cell.raw_skip)
                }));
            }
            for row in expected_anchor_row + expected_rows..buffer.height
            {
                assert!((0..buffer.width).all(|col|
                {
                    buffer
                        .cell(row, col)
                        .is_some_and(|cell| cell.raw.is_empty() && !cell.raw_skip)
                }));
            }
        }
    }
}

use std::cmp::{max, min};

pub const EXPANDED: i32 = 0x1fff_ffff;
pub const FIT_CONTENT: i32 = 0x1fff_fffe;
pub const MINIMUM_RENDERABLE_SIZE: i32 = 1;
pub const DEFAULT_VIEWPORT_WIDTH: i32 = 100;
pub const DEFAULT_VIEWPORT_HEIGHT: i32 = 32;
pub const TEXT_TAB_WIDTH: i32 = 4;
pub const DEFAULT_CELL_GAP: i32 = 1;
pub const DEFAULT_CELL_PADDING: i32 = 0;
const ANSI_RESET_SCROLL_REGION: &str = "\x1b[r";
const MINIMUM_SCROLL_REGION_ROWS: i32 = 2;

pub fn tab_spaces_for_column(column: i32) -> i32
{
    let normalized = max(0, column);
    TEXT_TAB_WIDTH - normalized.rem_euclid(TEXT_TAB_WIDTH)
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct VisualGlyph
{
    pub text: String,
    pub source_start: usize,
    pub source_end: usize,
}

pub fn is_unsafe_terminal_codepoint(ch: char) -> bool
{
    let codepoint = ch as u32;
    codepoint < 0x20
        || codepoint == 0x7f
        || matches!(
            codepoint,
            0x0300..=0x036f
                | 0x1ab0..=0x1aff
                | 0x1dc0..=0x1dff
                | 0x20d0..=0x20ff
                | 0xfe20..=0xfe2f
                | 0x200b..=0x200f
                | 0x202a..=0x202e
                | 0x2060..=0x206f
                | 0xfe00..=0xfe0f
                | 0xe0100..=0xe01ef
                | 0x1100..=0x115f
                | 0x2329..=0x232a
                | 0x2e80..=0xa4cf
                | 0xac00..=0xd7a3
                | 0xf900..=0xfaff
                | 0xfe10..=0xfe19
                | 0xfe30..=0xfe6f
                | 0xff00..=0xff60
                | 0xffe0..=0xffe6
                | 0x1f000..=0x1faff
                | 0x20000..=0x3fffd
        )
}

pub fn visual_glyphs(text: &str, start_column: i32, source_start: usize) -> Vec<VisualGlyph>
{
    let mut glyphs = Vec::new();
    let mut column = max(0, start_column);
    for (offset, ch) in text.char_indices()
    {
        let raw_start = source_start + offset;
        let raw_end = raw_start + ch.len_utf8();
        if ch == '\r'
        {
            continue;
        }
        if ch == '\t'
        {
            let spaces = tab_spaces_for_column(column);
            for _ in 0..spaces
            {
                glyphs.push(VisualGlyph
                {
                    text: " ".to_string(),
                    source_start: raw_start,
                    source_end: raw_end,
                });
                column += 1;
            }
            continue;
        }
        glyphs.push(VisualGlyph
        {
            text: if is_unsafe_terminal_codepoint(ch)
            {
                "?".to_string()
            }
            else
            {
                ch.to_string()
            },
            source_start: raw_start,
            source_end: raw_end,
        });
        column += 1;
    }
    glyphs
}

pub fn safe_terminal_cell_text(text: &str) -> String
{
    let Some(ch) = text.chars().next() else
    {
        return " ".to_string();
    };
    if ch == '\t'
    {
        return " ".to_string();
    }
    if is_unsafe_terminal_codepoint(ch)
    {
        return "?".to_string();
    }
    ch.to_string()
}

pub fn visible_text(text: &str, start_column: i32) -> String
{
    visual_glyphs(text, start_column, 0)
        .into_iter()
        .map(|glyph| glyph.text)
        .collect()
}

pub fn visible_width_at(text: &str, start_column: i32) -> i32
{
    visual_glyphs(text, start_column, 0).len() as i32
}

pub fn visible_width(text: &str) -> i32
{
    visible_width_at(text, 0)
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct Point
{
    pub row: i32,
    pub col: i32,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct Size
{
    pub width: i32,
    pub height: i32,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct Rect
{
    pub row: i32,
    pub col: i32,
    pub width: i32,
    pub height: i32,
}

impl Rect
{
    pub fn contains(self, point: Point) -> bool
    {
        point.row >= self.row
            && point.row < self.row + self.height
            && point.col >= self.col
            && point.col < self.col + self.width
    }

    pub fn empty(self) -> bool
    {
        self.width <= 0 || self.height <= 0
    }

    pub fn origin(self) -> Point
    {
        Point { row: self.row, col: self.col }
    }

    pub fn size(self) -> Size
    {
        Size { width: self.width, height: self.height }
    }

    pub fn intersect(self, other: Rect) -> Rect
    {
        let row = max(self.row, other.row);
        let col = max(self.col, other.col);
        let bottom = min(self.row + self.height, other.row + other.height);
        let right = min(self.col + self.width, other.col + other.width);
        Rect
        {
            row,
            col,
            width: max(0, right - col),
            height: max(0, bottom - row),
        }
    }
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub enum ColorKind
{
    #[default]
    Unset,
    Transparent,
    Rgb,
    Named,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Rgba
{
    pub red: u8,
    pub green: u8,
    pub blue: u8,
    pub alpha: u8,
}

impl Default for Rgba
{
    fn default() -> Self
    {
        Self { red: 0, green: 0, blue: 0, alpha: u8::MAX }
    }
}

impl std::ops::Index<usize> for Rgba
{
    type Output = u8;

    fn index(&self, index: usize) -> &Self::Output
    {
        match index
        {
            0 => &self.red,
            1 => &self.green,
            2 => &self.blue,
            3 => &self.alpha,
            _ => panic!("RGBA channel index must be in 0..4"),
        }
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct Color
{
    kind: ColorKind,
    text: String,
    rgba: Option<Rgba>,
}

impl Color
{
    pub fn new(value: impl Into<String>) -> Self
    {
        Self::try_new(value).unwrap_or_else(|error| panic!("{error}"))
    }

    pub fn try_new(value: impl Into<String>) -> Result<Self, String>
    {
        let text = value.into().to_lowercase();
        if text.is_empty()
        {
            return Ok(Self::default());
        }
        if text == "transparent"
        {
            return Ok(Self
            {
                kind: ColorKind::Transparent,
                text,
                rgba: None,
            });
        }
        if !text.starts_with('#')
        {
            return Ok(Self
            {
                kind: ColorKind::Named,
                text,
                rgba: None,
            });
        }
        let hex = &text[1..];
        if !hex.chars().all(|ch| ch.is_ascii_hexdigit())
        {
            return Err("invalid color hex text".to_string());
        }
        if !matches!(hex.len(), 3 | 6 | 8)
        {
            return Err("unsupported color hex length".to_string());
        }
        let rgba = parse_rgba(&text);
        Ok(Self { kind: ColorKind::Rgb, text, rgba })
    }

    pub fn transparent() -> Self
    {
        Self::new("transparent")
    }

    pub fn rgb(red: u8, green: u8, blue: u8) -> Self
    {
        Self::rgba_color(red, green, blue, u8::MAX)
    }

    pub fn rgba_color(red: u8, green: u8, blue: u8, alpha: u8) -> Self
    {
        Self
        {
            kind: ColorKind::Rgb,
            text: String::new(),
            rgba: Some(Rgba { red, green, blue, alpha }),
        }
    }

    pub fn named(value: impl Into<String>) -> Self
    {
        Self
        {
            kind: ColorKind::Named,
            text: value.into().to_lowercase(),
            rgba: None,
        }
    }

    pub fn kind(&self) -> ColorKind
    {
        self.kind
    }

    pub fn empty(&self) -> bool
    {
        self.kind == ColorKind::Unset
    }

    pub fn is_set(&self) -> bool
    {
        self.kind != ColorKind::Unset
    }

    pub fn is_transparent(&self) -> bool
    {
        self.kind == ColorKind::Transparent
    }

    pub fn unset_or_transparent(&self) -> bool
    {
        self.empty()
            || self.is_transparent()
            || self.rgba.is_some_and(|rgba| rgba[3] == 0)
    }

    pub fn translucent(&self) -> bool
    {
        self.rgba.is_some_and(|rgba| rgba[3] < u8::MAX)
    }

    pub fn has_partial_alpha(&self) -> bool
    {
        !self.is_transparent() && self.translucent()
    }

    pub fn as_str(&self) -> &str
    {
        &self.text
    }

    pub fn rgba(&self) -> Option<Rgba>
    {
        self.rgba
    }

    pub fn normalized(&self) -> String
    {
        if self.kind == ColorKind::Transparent
        {
            return self.text.clone();
        }
        if let Some(Rgba { red, green, blue, .. }) = self.rgba
        {
            return format!("#{red:02x}{green:02x}{blue:02x}");
        }
        self.text.clone()
    }

    pub fn blend_over(&self, background: &Color) -> Color
    {
        let Some(Rgba { red, green, blue, alpha }) = self.rgba else
        {
            return self.clone();
        };
        if alpha == u8::MAX
        {
            return self.clone();
        }
        if alpha == 0
        {
            return background.clone();
        }
        let Some(Rgba
        {
            red: base_red,
            green: base_green,
            blue: base_blue,
            ..
        }) = background.rgba else
        {
            return self.clone();
        };
        let mix = |foreground: u8, base: u8|
        {
            let ratio = f64::from(alpha) / f64::from(u8::MAX);
            (ratio * f64::from(foreground) + (1.0 - ratio) * f64::from(base))
                .round_ties_even() as u8
        };
        Color::new(format!(
            "#{:02x}{:02x}{:02x}",
            mix(red, base_red),
            mix(green, base_green),
            mix(blue, base_blue)
        ))
    }

    pub fn dimmed(&self, factor: f64) -> Color
    {
        let Some(Rgba { red, green, blue, alpha }) = self.rgba else
        {
            return self.clone();
        };
        if alpha == 0
        {
            return self.clone();
        }
        let scale = |component: u8|
        {
            (f64::from(component) * factor.clamp(0.0, 1.0)) as u8
        };
        Color::new(format!(
            "#{:02x}{:02x}{:02x}{alpha:02x}",
            scale(red),
            scale(green),
            scale(blue)
        ))
    }
}

pub fn transparent_color(color: &Option<Color>) -> bool
{
    color.as_ref().is_none_or(Color::unset_or_transparent)
}

pub fn optional_color_normalized(color: &Option<Color>) -> String
{
    color.as_ref().map_or_else(String::new, Color::normalized)
}

pub(crate) trait OptionalColorExt
{
    #[cfg(test)]
    fn rgba(&self) -> Option<Rgba>;
    #[cfg(test)]
    fn normalized(&self) -> String;
    fn unset_or_transparent(&self) -> bool;
    fn translucent(&self) -> bool;
    fn dimmed(&self, factor: f64) -> Option<Color>;
}

impl OptionalColorExt for Option<Color>
{
    #[cfg(test)]
    fn rgba(&self) -> Option<Rgba>
    {
        self.as_ref().and_then(Color::rgba)
    }

    #[cfg(test)]
    fn normalized(&self) -> String
    {
        optional_color_normalized(self)
    }

    fn unset_or_transparent(&self) -> bool
    {
        transparent_color(self)
    }

    fn translucent(&self) -> bool
    {
        self.as_ref().is_some_and(Color::translucent)
    }

    fn dimmed(&self, factor: f64) -> Option<Color>
    {
        self.as_ref().map(|color| color.dimmed(factor))
    }
}

fn blend_background_over_existing(
    color: &Option<Color>,
    existing: &Option<Color>,
) -> Option<Color>
{
    if transparent_color(color)
    {
        return existing.clone();
    }
    let foreground = color.as_ref().expect("non-transparent color");
    if foreground.rgba().is_some_and(|rgba| rgba.alpha < u8::MAX)
        && existing.is_some()
    {
        return Some(foreground.blend_over(existing.as_ref().expect("existing color")));
    }
    color.clone()
}

fn blend_foreground_over_background(
    color: &Option<Color>,
    inherited: &Option<Color>,
    background: &Option<Color>,
) -> Option<Color>
{
    if transparent_color(color)
    {
        return inherited.clone();
    }
    let foreground = color.as_ref().expect("non-transparent color");
    if foreground.rgba().is_some_and(|rgba| rgba.alpha < u8::MAX)
        && background.is_some()
    {
        return Some(foreground.blend_over(background.as_ref().expect("background color")));
    }
    color.clone()
}

impl std::fmt::Display for Color
{
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        formatter.write_str(&self.normalized())
    }
}

fn parse_rgba(value: &str) -> Option<Rgba>
{
    if value == "transparent"
    {
        return Some(Rgba { red: 0, green: 0, blue: 0, alpha: 0 });
    }
    let hex = value.strip_prefix('#')?;
    if hex.len() == 3
    {
        let component = |index: usize|
        {
            let value = &hex[index..index + 1];
            u8::from_str_radix(&format!("{value}{value}"), 16).ok()
        };
        return Some(Rgba
        {
            red: component(0)?,
            green: component(1)?,
            blue: component(2)?,
            alpha: u8::MAX,
        });
    }
    if hex.len() != 6 && hex.len() != 8
    {
        return None;
    }
    let component = |start| u8::from_str_radix(&hex[start..start + 2], 16).ok();
    Some(Rgba
    {
        red: component(0)?,
        green: component(2)?,
        blue: component(4)?,
        alpha: if hex.len() == 8 { component(6)? } else { u8::MAX },
    })
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct TextGradient
{
    pub interval_ms: i32,
    pub step: i32,
    pub segment_size: i32,
    pub colors: Vec<Color>,
}

impl Default for TextGradient
{
    fn default() -> Self
    {
        Self
        {
            interval_ms: 70,
            step: 1,
            segment_size: 1,
            colors: Vec::new(),
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Style
{
    pub color: Option<Color>,
    pub background: Option<Color>,
    pub background_texture: Option<String>,
    pub background_texture_color: Option<Color>,
    pub scope_dim_background: Option<Color>,
    pub border_color: Option<Color>,
    pub border_width_horizontal: Option<i32>,
    pub border_width_vertical: Option<i32>,
    pub padding: Option<i32>,
    pub padding_top: Option<i32>,
    pub padding_right: Option<i32>,
    pub padding_bottom: Option<i32>,
    pub padding_left: Option<i32>,
    pub margin: Option<i32>,
    pub margin_top: Option<i32>,
    pub margin_right: Option<i32>,
    pub margin_bottom: Option<i32>,
    pub margin_left: Option<i32>,
    pub gap: Option<i32>,
    pub text_align: Option<String>,
    pub user_select: Option<String>,
    pub scroll_x: Option<bool>,
    pub scroll_y: Option<bool>,
    pub text_color_gradient: Option<TextGradient>,
    pub text_background_gradient: Option<TextGradient>,
}

impl Default for Style
{
    fn default() -> Self
    {
        Self
        {
            color: None,
            background: None,
            background_texture: None,
            background_texture_color: None,
            scope_dim_background: Some(Color::new("#00000066")),
            border_color: None,
            border_width_horizontal: None,
            border_width_vertical: None,
            padding: None,
            padding_top: None,
            padding_right: None,
            padding_bottom: None,
            padding_left: None,
            margin: None,
            margin_top: None,
            margin_right: None,
            margin_bottom: None,
            margin_left: None,
            gap: None,
            text_align: None,
            user_select: None,
            scroll_x: None,
            scroll_y: None,
            text_color_gradient: None,
            text_background_gradient: None,
        }
    }
}

impl Style
{
    pub fn merge(&mut self, other: &Style)
    {
        macro_rules! merge_option
        {
            ($field:ident) =>
            {
                if other.$field.is_some() { self.$field = other.$field.clone(); }
            };
        }
        merge_option!(color);
        merge_option!(background);
        merge_option!(background_texture);
        merge_option!(background_texture_color);
        merge_option!(scope_dim_background);
        merge_option!(border_color);
        merge_option!(border_width_horizontal);
        merge_option!(border_width_vertical);
        merge_option!(padding);
        merge_option!(padding_top);
        merge_option!(padding_right);
        merge_option!(padding_bottom);
        merge_option!(padding_left);
        merge_option!(margin);
        merge_option!(margin_top);
        merge_option!(margin_right);
        merge_option!(margin_bottom);
        merge_option!(margin_left);
        merge_option!(gap);
        merge_option!(text_align);
        merge_option!(user_select);
        merge_option!(scroll_x);
        merge_option!(scroll_y);
        merge_option!(text_color_gradient);
        merge_option!(text_background_gradient);
    }

    pub fn padding_values(&self) -> [i32; 4]
    {
        let all = self.padding.unwrap_or(0);
        [
            self.padding_top.unwrap_or(all),
            self.padding_right.unwrap_or(all),
            self.padding_bottom.unwrap_or(all),
            self.padding_left.unwrap_or(all),
        ]
    }

    pub fn margin_values(&self) -> [i32; 4]
    {
        let all = self.margin.unwrap_or(0);
        [
            self.margin_top.unwrap_or(all),
            self.margin_right.unwrap_or(all),
            self.margin_bottom.unwrap_or(all),
            self.margin_left.unwrap_or(all),
        ]
    }
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub enum DimensionMode
{
    #[default]
    Auto,
    Fixed,
    Expanded,
    FitContent,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub enum LayoutDirection
{
    Row,
    #[default]
    Column,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct AxisDimension
{
    pub mode: DimensionMode,
    pub value: i32,
}

impl AxisDimension
{
    pub fn fixed(value: i32) -> Self
    {
        Self { mode: DimensionMode::Fixed, value }
    }

    pub fn expanded() -> Self
    {
        Self { mode: DimensionMode::Expanded, value: 0 }
    }

    pub fn fit_content() -> Self
    {
        Self { mode: DimensionMode::FitContent, value: 0 }
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct LayoutElement
{
    pub name: String,
    pub relative: Rect,
    pub width: AxisDimension,
    pub height: AxisDimension,
    pub fit_content_size: Size,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct LayoutCell
{
    pub name: String,
    pub width: AxisDimension,
    pub height: AxisDimension,
    pub source_size: Size,
    pub fit_content_size: Size,
    pub padding: i32,
    pub gap: i32,
    pub direction: LayoutDirection,
    pub elements: Vec<LayoutElement>,
    pub children: Vec<LayoutCell>,
}

impl Default for LayoutCell
{
    fn default() -> Self
    {
        Self
        {
            name: String::new(),
            width: AxisDimension::default(),
            height: AxisDimension::default(),
            source_size: Size::default(),
            fit_content_size: Size::default(),
            padding: DEFAULT_CELL_PADDING,
            gap: DEFAULT_CELL_GAP,
            direction: LayoutDirection::Column,
            elements: Vec::new(),
            children: Vec::new(),
        }
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct ResolvedElement
{
    pub name: String,
    pub rect: Rect,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct ResolvedCell
{
    pub name: String,
    pub rect: Rect,
    pub elements: Vec<ResolvedElement>,
    pub children: Vec<ResolvedCell>,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct FlatResolvedCell
{
    pub name: String,
    pub rect: Rect,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct FlatResolvedElement
{
    pub cell_name: String,
    pub name: String,
    pub rect: Rect,
}

pub fn find_cell_by_name<'a>(root: &'a ResolvedCell, name: &str) -> Option<&'a ResolvedCell>
{
    if root.name == name
    {
        return Some(root);
    }
    root.children
        .iter()
        .find_map(|child| find_cell_by_name(child, name))
}

pub fn find_element_by_name<'a>(
    root: &'a ResolvedCell,
    name: &str,
) -> Option<&'a ResolvedElement>
{
    root.elements
        .iter()
        .find(|element| element.name == name)
        .or_else(||
        {
            root.children
                .iter()
                .find_map(|child| find_element_by_name(child, name))
        })
}

pub fn flatten_cells(root: &ResolvedCell) -> Vec<FlatResolvedCell>
{
    let mut cells = vec![FlatResolvedCell
    {
        name: root.name.clone(),
        rect: root.rect,
    }];
    for child in &root.children
    {
        cells.extend(flatten_cells(child));
    }
    cells
}

pub fn flatten_elements(root: &ResolvedCell) -> Vec<FlatResolvedElement>
{
    let mut elements = root
        .elements
        .iter()
        .map(|element| FlatResolvedElement
        {
            cell_name: root.name.clone(),
            name: element.name.clone(),
            rect: element.rect,
        })
        .collect::<Vec<_>>();
    for child in &root.children
    {
        elements.extend(flatten_elements(child));
    }
    elements
}

#[derive(Clone, Copy, Debug, Default)]
pub struct LayoutResolver;

impl LayoutResolver
{
    pub fn resolve(&self, root: &LayoutCell, bounds: Rect) -> ResolvedCell
    {
        let measured = self.measure_cell(root);
        let width = if bounds.width > 0
        {
            bounds.width
        }
        else
        {
            self.resolve_axis(root.width, root.source_size.width, measured.width)
        };
        let height = if bounds.height > 0
        {
            bounds.height
        }
        else
        {
            self.resolve_axis(root.height, root.source_size.height, measured.height)
        };
        self.resolve_cell(
            root,
            Rect { row: bounds.row, col: bounds.col, width, height },
        )
    }

    fn resolve_cell(&self, cell: &LayoutCell, bounds: Rect) -> ResolvedCell
    {
        let mut resolved = ResolvedCell
        {
            name: cell.name.clone(),
            rect: bounds,
            ..Default::default()
        };
        let content_rect = inset_layout_rect(bounds, cell.padding);
        resolved.elements = cell
            .elements
            .iter()
            .map(|element| self.resolve_element(element, content_rect))
            .collect();
        if cell.children.is_empty()
        {
            return resolved;
        }

        let horizontal = cell.direction == LayoutDirection::Row;
        let available_main = if horizontal { content_rect.width } else { content_rect.height };
        let available_cross = if horizontal { content_rect.height } else { content_rect.width };
        let main_sizes = self.resolve_child_axis_sizes(cell, available_main);
        let mut cursor = if horizontal { content_rect.col } else { content_rect.row };
        for (index, child) in cell.children.iter().enumerate()
        {
            let main_size = main_sizes[index];
            let cross_size = child_cross_size(child, horizontal, available_cross, self);
            let child_rect = if horizontal
            {
                Rect
                {
                    row: content_rect.row,
                    col: cursor,
                    width: main_size,
                    height: cross_size,
                }
            }
            else
            {
                Rect
                {
                    row: cursor,
                    col: content_rect.col,
                    width: cross_size,
                    height: main_size,
                }
            };
            resolved.children.push(self.resolve_cell(child, child_rect));
            cursor += main_size + cell.gap;
        }
        resolved
    }

    fn measure_cell(&self, cell: &LayoutCell) -> Size
    {
        let child_size = self.measure_children(cell);
        let mut content_width = cell.fit_content_size.width.max(child_size.width);
        let mut content_height = cell.fit_content_size.height.max(child_size.height);
        for element in &cell.elements
        {
            let width =
                self.resolve_axis(element.width, element.relative.width, element.fit_content_size.width);
            let height =
                self.resolve_axis(element.height, element.relative.height, element.fit_content_size.height);
            content_width = content_width.max(element.relative.col + width);
            content_height = content_height.max(element.relative.row + height);
        }
        let padding = cell.padding.max(0);
        Size
        {
            width: content_width + padding * 2,
            height: content_height + padding * 2,
        }
    }

    fn measure_children(&self, cell: &LayoutCell) -> Size
    {
        if cell.children.is_empty()
        {
            return Size::default();
        }
        let horizontal = cell.direction == LayoutDirection::Row;
        let mut main = 0;
        let mut cross = 0;
        for child in &cell.children
        {
            let child_size = self.measure_cell(child);
            main += if horizontal { child_size.width } else { child_size.height };
            cross = cross.max(if horizontal { child_size.height } else { child_size.width });
        }
        main += cell.gap.max(0) * (cell.children.len() as i32 - 1);
        if horizontal
        {
            Size { width: main, height: cross }
        }
        else
        {
            Size { width: cross, height: main }
        }
    }

    fn resolve_axis(&self, dimension: AxisDimension, fallback: i32, fit_content: i32) -> i32
    {
        match dimension.mode
        {
            DimensionMode::Fixed => dimension.value.max(0),
            DimensionMode::FitContent => fit_content.max(0),
            DimensionMode::Expanded => fallback.max(0),
            DimensionMode::Auto => if fallback > 0 { fallback } else { fit_content }.max(0),
        }
    }

    fn resolve_child_axis_sizes(&self, cell: &LayoutCell, available: i32) -> Vec<i32>
    {
        let horizontal = cell.direction == LayoutDirection::Row;
        let mut sizes = vec![0; cell.children.len()];
        let mut expanded_count = 0;
        let mut fixed_total = 0;
        for (index, child) in cell.children.iter().enumerate()
        {
            let dimension = if horizontal { child.width } else { child.height };
            if dimension.mode == DimensionMode::Expanded
            {
                expanded_count += 1;
                continue;
            }
            sizes[index] = child_axis_fixed_size(child, horizontal, self);
            fixed_total += sizes[index];
        }

        let gaps = cell.gap.max(0) * cell.children.len().saturating_sub(1) as i32;
        let mut remaining = (available - fixed_total - gaps).max(0);
        let expanded_share = if expanded_count > 0
        {
            remaining / expanded_count
        }
        else
        {
            0
        };
        for (index, child) in cell.children.iter().enumerate()
        {
            let dimension = if horizontal { child.width } else { child.height };
            if dimension.mode == DimensionMode::Expanded
            {
                sizes[index] = expanded_share;
                remaining -= expanded_share;
                expanded_count -= 1;
                if expanded_count == 0
                {
                    sizes[index] += remaining;
                }
            }
        }
        sizes
    }

    fn resolve_element(&self, element: &LayoutElement, content_rect: Rect) -> ResolvedElement
    {
        let width =
            self.resolve_axis(element.width, element.relative.width, element.fit_content_size.width);
        let height =
            self.resolve_axis(element.height, element.relative.height, element.fit_content_size.height);
        ResolvedElement
        {
            name: element.name.clone(),
            rect: Rect
            {
                row: content_rect.row + element.relative.row,
                col: content_rect.col + element.relative.col,
                width,
                height,
            },
        }
    }
}

fn child_axis_fixed_size(
    child: &LayoutCell,
    horizontal: bool,
    resolver: &LayoutResolver,
) -> i32
{
    let measured = resolver.resolve(child, Rect::default()).rect.size();
    let dimension = if horizontal { child.width } else { child.height };
    let source = if horizontal { child.source_size.width } else { child.source_size.height };
    let fit = if horizontal { measured.width } else { measured.height };
    match dimension.mode
    {
        DimensionMode::Fixed => dimension.value.max(0),
        DimensionMode::FitContent => fit,
        DimensionMode::Auto => if source > 0 { source } else { fit },
        DimensionMode::Expanded => 0,
    }
}

fn child_cross_size(
    child: &LayoutCell,
    horizontal: bool,
    available: i32,
    resolver: &LayoutResolver,
) -> i32
{
    let measured = resolver.resolve(child, Rect::default()).rect.size();
    let dimension = if horizontal { child.height } else { child.width };
    let source = if horizontal { child.source_size.height } else { child.source_size.width };
    let fit = if horizontal { measured.height } else { measured.width };
    match dimension.mode
    {
        DimensionMode::Fixed => dimension.value.max(0),
        DimensionMode::Expanded => available.max(0),
        DimensionMode::FitContent => fit,
        DimensionMode::Auto => if source > 0 { source } else { fit },
    }
}

fn inset_layout_rect(rect: Rect, padding: i32) -> Rect
{
    let padding = padding.max(0);
    Rect
    {
        row: rect.row + padding,
        col: rect.col + padding,
        width: (rect.width - padding * 2).max(0),
        height: (rect.height - padding * 2).max(0),
    }
}

#[derive(Clone, Debug, Default)]
pub struct LayoutItem
{
    pub element_type: String,
    pub row: i32,
    pub col: i32,
    pub cell_row: i32,
    pub cell_col: i32,
    pub cell_chars_width: i32,
    pub cell_chars_height: i32,
    pub cell_name: String,
    pub cell_width: i32,
    pub cell_height: i32,
    pub cell_width_mode: String,
    pub cell_height_mode: String,
    pub width: i32,
    pub height: i32,
    pub width_mode: String,
    pub height_mode: String,
    pub chars_width: i32,
    pub chars_height: i32,
    pub margin_right: i32,
    pub margin_bottom: i32,
    pub content: String,
    pub cell_style: Style,
    pub element_style: Style,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Cell
{
    pub text: String,
    pub foreground: Option<Color>,
    pub background: Option<Color>,
    pub raw: String,
    pub raw_width: i32,
    pub raw_height: i32,
    pub raw_skip: bool,
}

pub type TerminalCell = Cell;

pub trait TerminalCellColor
{
    fn terminal_cell_color(&self) -> Option<Color>;
}

impl TerminalCellColor for Color
{
    fn terminal_cell_color(&self) -> Option<Color>
    {
        Some(self.clone())
    }
}

impl TerminalCellColor for Option<Color>
{
    fn terminal_cell_color(&self) -> Option<Color>
    {
        self.clone()
    }
}

impl Default for Cell
{
    fn default() -> Self
    {
        Self
        {
            text: " ".to_string(),
            foreground: None,
            background: None,
            raw: String::new(),
            raw_width: MINIMUM_RENDERABLE_SIZE,
            raw_height: MINIMUM_RENDERABLE_SIZE,
            raw_skip: false,
        }
    }
}

#[derive(Clone, Debug)]
pub struct TerminalBuffer
{
    pub(crate) width: i32,
    pub(crate) height: i32,
    cells: Vec<Cell>,
    previous: Vec<Cell>,
    force_full_redraw: bool,
    render_stats: TerminalBufferRenderStats,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct TerminalBufferRenderStats
{
    pub changed_cells: usize,
    pub changed_runs: usize,
    pub output_bytes: usize,
    pub full_redraw: bool,
}

impl TerminalBuffer
{
    pub fn new(width: i32, height: i32) -> Self
    {
        assert!(
            width >= 0 && height >= 0,
            "terminal buffer dimensions must not be negative",
        );
        Self
        {
            width,
            height,
            cells: vec![Cell::default(); (width * height) as usize],
            previous: vec![Cell::default(); (width * height) as usize],
            force_full_redraw: false,
            render_stats: TerminalBufferRenderStats::default(),
        }
    }

    pub fn resize(&mut self, width: i32, height: i32)
    {
        assert!(
            width >= 0 && height >= 0,
            "terminal buffer dimensions must not be negative",
        );
        if self.width == width && self.height == height
        {
            return;
        }
        self.width = width;
        self.height = height;
        self.cells = vec![Cell::default(); (width * height) as usize];
        self.previous = self.cells.clone();
        self.force_full_redraw = true;
    }

    pub fn width(&self) -> i32
    {
        self.width
    }

    pub fn height(&self) -> i32
    {
        self.height
    }

    pub fn clear(&mut self)
    {
        self.clear_with(TerminalCell::default());
    }

    pub fn clear_with(&mut self, cell: TerminalCell)
    {
        self.cells.fill(cell);
    }

    pub fn set_cell(&mut self, row: i32, col: i32, cell: Cell)
    {
        let Some(index) = self.index(row, col) else { return };
        self.cells[index] = cell;
    }

    pub fn replace_current(&mut self, current: &TerminalBuffer)
    {
        if self.width != current.width || self.height != current.height
        {
            self.resize(current.width, current.height);
        }
        self.cells.clone_from(&current.cells);
    }

    #[cfg(test)]
    pub(crate) fn import_previous_frame(&mut self, previous: Option<&TerminalBuffer>)
    {
        if let Some(previous) = previous.filter(|previous|
        {
            previous.width == self.width && previous.height == self.height
        })
        {
            self.previous.clone_from(&previous.cells);
        }
        else
        {
            self.force_full_redraw = true;
        }
    }

    pub fn request_full_redraw(&mut self)
    {
        self.force_full_redraw = true;
    }

    pub fn begin_render_stats(&mut self)
    {
        self.render_stats = TerminalBufferRenderStats::default();
    }

    pub fn render_stats(&self) -> TerminalBufferRenderStats
    {
        self.render_stats
    }

    pub fn cell(&self, row: i32, col: i32) -> Option<&Cell>
    {
        self.index(row, col).map(|index| &self.cells[index])
    }

    pub fn cell_mut(&mut self, row: i32, col: i32) -> Option<&mut Cell>
    {
        self.index(row, col).map(|index| &mut self.cells[index])
    }

    fn index(&self, row: i32, col: i32) -> Option<usize>
    {
        if row < 0 || col < 0 || row >= self.height || col >= self.width
        {
            return None;
        }
        Some((row * self.width + col) as usize)
    }

    pub fn fill<Foreground, Background>(
        &mut self,
        rect: Rect,
        ch: char,
        foreground: &Foreground,
        background: &Background,
    )
    where
        Foreground: TerminalCellColor,
        Background: TerminalCellColor,
    {
        let clipped = rect.intersect(Rect { row: 0, col: 0, width: self.width, height: self.height });
        for row in clipped.row..clipped.row + clipped.height
        {
            for col in clipped.col..clipped.col + clipped.width
            {
                if let Some(cell) = self.cell_mut(row, col)
                {
                    cell.text = ch.to_string();
                    cell.foreground = foreground.terminal_cell_color();
                    cell.background = background.terminal_cell_color();
                    cell.raw.clear();
                    cell.raw_width = MINIMUM_RENDERABLE_SIZE;
                    cell.raw_height = MINIMUM_RENDERABLE_SIZE;
                    cell.raw_skip = false;
                }
            }
        }
    }

    pub fn draw_text<Foreground, Background>(
        &mut self,
        row: i32,
        col: i32,
        text: &str,
        foreground: &Foreground,
        background: &Background,
        max_width: i32,
    )
    where
        Foreground: TerminalCellColor,
        Background: TerminalCellColor,
    {
        for (offset, glyph) in visual_glyphs(text, 0, 0).into_iter().enumerate()
        {
            let current_col = col + offset as i32;
            if current_col - col >= max_width
            {
                break;
            }
            if let Some(cell) = self.cell_mut(row, current_col)
            {
                cell.text = glyph.text;
                cell.foreground = foreground.terminal_cell_color();
                cell.background = background.terminal_cell_color();
                cell.raw.clear();
                cell.raw_width = MINIMUM_RENDERABLE_SIZE;
                cell.raw_height = MINIMUM_RENDERABLE_SIZE;
                cell.raw_skip = false;
            }
        }
    }

    pub fn overlay(&mut self, source: &TerminalBuffer, row_offset: i32, col_offset: i32)
    {
        self.overlay_clipped(
            source,
            row_offset,
            col_offset,
            Rect { row: 0, col: 0, width: self.width, height: self.height },
        );
    }

    pub fn overlay_clipped(
        &mut self,
        source: &TerminalBuffer,
        row_offset: i32,
        col_offset: i32,
        clip: Rect,
    )
    {
        for row in 0..source.height
        {
            for col in 0..source.width
            {
                let Some(source_cell) = source.cell(row, col) else { continue };
                let target_row = row + row_offset;
                let target_col = col + col_offset;
                if !clip.contains(Point { row: target_row, col: target_col })
                {
                    continue;
                }
                let Some(target) = self.cell_mut(target_row, target_col) else { continue };
                let existing = target.clone();
                let mut cell = source_cell.clone();
                cell.background =
                    blend_background_over_existing(&cell.background, &existing.background);
                cell.foreground = blend_foreground_over_background(
                    &cell.foreground,
                    &existing.foreground,
                    &cell.background,
                );
                *target = cell;
            }
        }
    }

    pub fn plain_text(&self) -> String
    {
        let mut output = String::new();
        for row in 0..self.height
        {
            let mut line = String::new();
            for col in 0..self.width
            {
                line.push_str(
                    self.cell(row, col)
                        .map_or(" ", |cell| cell.text.as_str()),
                );
            }
            output.push_str(line.trim_end());
            if row + 1 < self.height
            {
                output.push('\n');
            }
        }
        output
    }

    pub fn cells(&self) -> &[Cell]
    {
        &self.cells
    }

    pub fn render_diff(&mut self, row_offset: i32, col_offset: i32) -> String
    {
        self.render_diff_region(
            row_offset,
            col_offset,
            0,
            0,
            self.height,
            self.width,
        )
    }

    pub fn render_diff_region(
        &mut self,
        row_offset: i32,
        col_offset: i32,
        start_row: i32,
        start_col: i32,
        height: i32,
        width: i32,
    ) -> String
    {
        let mut output = String::new();
        let full_redraw = self.force_full_redraw;
        let mut synchronize_update = false;
        let mut raw_emitted = false;
        self.render_stats.full_redraw |= full_redraw;
        let first_row = max(0, start_row);
        let first_col = max(0, start_col);
        let last_row = min(self.height, start_row + max(0, height));
        let last_col = min(self.width, start_col + max(0, width));

        for row in first_row..last_row
        {
            let mut col = first_col;
            while col < last_col
            {
                let index = self.index(row, col).expect("render coordinate");
                if self.cells[index].raw_skip
                {
                    if full_redraw || self.cells[index] != self.previous[index]
                    {
                        self.render_stats.changed_cells += 1;
                    }
                    self.previous[index] = self.cells[index].clone();
                    col += 1;
                    continue;
                }
                if !full_redraw && self.cells[index] == self.previous[index]
                {
                    col += 1;
                    continue;
                }

                let style_cell = self.cells[index].clone();
                if !style_cell.raw.is_empty()
                {
                    synchronize_update = true;
                    let raw_width = max(MINIMUM_RENDERABLE_SIZE, style_cell.raw_width);
                    let raw_height = max(MINIMUM_RENDERABLE_SIZE, style_cell.raw_height);
                    let clear_width = min(raw_width, self.width - col);
                    let clear_height = min(raw_height, self.height - row);
                    for clear_row in row..row + clear_height
                    {
                        push_terminal_cursor(
                            &mut output,
                            clear_row + row_offset,
                            col + col_offset,
                        );
                        output.push_str(&ansi_for_cell(&style_cell));
                        output.push_str(&" ".repeat(clear_width as usize));
                    }
                    if clear_height >= raw_height
                    {
                        let anchor_row = row + row_offset;
                        let no_scroll_region = raw_no_scroll_region(
                            anchor_row,
                            raw_height,
                            row_offset + self.height,
                        );
                        output.push_str(&no_scroll_region);
                        push_terminal_cursor(
                            &mut output,
                            row + row_offset,
                            col + col_offset,
                        );
                        output.push_str(&style_cell.raw);
                        if !no_scroll_region.is_empty()
                        {
                            output.push_str(ANSI_RESET_SCROLL_REGION);
                        }
                        raw_emitted = true;
                    }
                    for covered_row in row..row + clear_height
                    {
                        for covered_col in col..col + clear_width
                        {
                            let covered_index = self
                                .index(covered_row, covered_col)
                                .expect("covered render coordinate");
                            self.previous[covered_index] =
                                self.cells[covered_index].clone();
                            self.render_stats.changed_cells += 1;
                        }
                    }
                    col += clear_width;
                    self.render_stats.changed_runs += clear_height as usize + 1;
                    continue;
                }

                let run_col = col;
                let mut run = String::new();
                while col < last_col
                {
                    let current_index = self.index(row, col).expect("render coordinate");
                    let current = &self.cells[current_index];
                    if (!full_redraw && *current == self.previous[current_index])
                        || current.raw_skip
                        || !current.raw.is_empty()
                        || current.foreground != style_cell.foreground
                        || current.background != style_cell.background
                    {
                        break;
                    }
                    run.push_str(&safe_terminal_cell_text(&current.text));
                    self.previous[current_index] = current.clone();
                    col += 1;
                    self.render_stats.changed_cells += 1;
                }
                push_terminal_cursor(
                    &mut output,
                    row + row_offset,
                    run_col + col_offset,
                );
                output.push_str(&ansi_for_cell(&style_cell));
                output.push_str(&run);
                self.render_stats.changed_runs += 1;
            }
        }

        if !output.is_empty() && raw_emitted
        {
            for row in first_row..last_row
            {
                let mut col = first_col;
                while col < last_col
                {
                    let cell = &self.cells[self.index(row, col).expect("render coordinate")];
                    if cell.raw_skip || !cell.raw.is_empty()
                    {
                        col += 1;
                        continue;
                    }
                    let style_cell = cell.clone();
                    let run_col = col;
                    let mut run = String::new();
                    while col < last_col
                    {
                        let current =
                            &self.cells[self.index(row, col).expect("render coordinate")];
                        if current.raw_skip
                            || !current.raw.is_empty()
                            || current.foreground != style_cell.foreground
                            || current.background != style_cell.background
                        {
                            break;
                        }
                        run.push_str(&safe_terminal_cell_text(&current.text));
                        col += 1;
                    }
                    push_terminal_cursor(
                        &mut output,
                        row + row_offset,
                        run_col + col_offset,
                    );
                    output.push_str(&ansi_for_cell(&style_cell));
                    output.push_str(&run);
                    self.render_stats.changed_runs += 1;
                }
            }
        }
        if !output.is_empty()
        {
            output.push_str("\x1b[0m");
        }
        if !output.is_empty() && synchronize_update
        {
            output = format!("\x1b[?2026h{output}\x1b[?2026l");
        }
        self.render_stats.output_bytes += output.len();
        self.force_full_redraw = false;
        output
    }

    pub fn render_scroll_region(
        &mut self,
        row_offset: i32,
        start_row: i32,
        height: i32,
        delta: i32,
    ) -> String
    {
        let first_row = max(0, start_row);
        let last_row = min(self.height, start_row + max(0, height));
        let region_height = last_row - first_row;
        let distance = min(delta.unsigned_abs() as i32, region_height);
        if self.force_full_redraw
            || region_height <= 1
            || distance <= 0
            || distance >= region_height
            || self.scroll_region_has_raw_cells(first_row, last_row)
        {
            return String::new();
        }

        let before = self.previous.clone();
        if delta > 0
        {
            for row in (first_row + distance..last_row).rev()
            {
                for col in 0..self.width
                {
                    let target = self.index(row, col).expect("scroll target");
                    let source = self
                        .index(row - distance, col)
                        .expect("scroll source");
                    self.previous[target] = before[source].clone();
                }
            }
            for row in first_row..first_row + distance
            {
                for col in 0..self.width
                {
                    let index = self.index(row, col).expect("scroll clear");
                    self.previous[index] = Cell::default();
                }
            }
        }
        else
        {
            for row in first_row..last_row - distance
            {
                for col in 0..self.width
                {
                    let target = self.index(row, col).expect("scroll target");
                    let source = self
                        .index(row + distance, col)
                        .expect("scroll source");
                    self.previous[target] = before[source].clone();
                }
            }
            for row in last_row - distance..last_row
            {
                for col in 0..self.width
                {
                    let index = self.index(row, col).expect("scroll clear");
                    self.previous[index] = Cell::default();
                }
            }
        }

        let command = if delta > 0 { 'T' } else { 'S' };
        let output = format!(
            "\x1b[{};{}r\x1b[{};1H\x1b[{distance}{command}\x1b[r",
            first_row + row_offset + 1,
            last_row + row_offset,
            first_row + row_offset + 1,
        );
        self.render_stats.output_bytes += output.len();
        output
    }

    fn scroll_region_has_raw_cells(&self, first_row: i32, last_row: i32) -> bool
    {
        for row in first_row..last_row
        {
            for col in 0..self.width
            {
                let index = self.index(row, col).expect("raw scan coordinate");
                let current = &self.cells[index];
                let previous = &self.previous[index];
                if !current.raw.is_empty()
                    || current.raw_skip
                    || !previous.raw.is_empty()
                    || previous.raw_skip
                {
                    return true;
                }
            }
        }
        false
    }
}

fn raw_no_scroll_region(anchor_row: i32, raw_height: i32, buffer_bottom_exclusive: i32) -> String
{
    if anchor_row >= MINIMUM_SCROLL_REGION_ROWS
    {
        return format!("\x1b[1;{anchor_row}r");
    }
    let raw_bottom_exclusive = anchor_row + max(MINIMUM_RENDERABLE_SIZE, raw_height);
    if buffer_bottom_exclusive - raw_bottom_exclusive >= MINIMUM_SCROLL_REGION_ROWS
    {
        return format!(
            "\x1b[{};{buffer_bottom_exclusive}r",
            raw_bottom_exclusive + MINIMUM_RENDERABLE_SIZE,
        );
    }
    String::new()
}

fn push_terminal_cursor(output: &mut String, row: i32, col: i32)
{
    output.push_str(&format!("\x1b[{};{}H", row + 1, col + 1));
}

fn ansi_for_cell(cell: &Cell) -> String
{
    format!(
        "\x1b[{};{}m",
        ansi_color(&cell.foreground, true),
        ansi_color(&cell.background, false),
    )
}

fn ansi_color(color: &Option<Color>, foreground: bool) -> String
{
    if transparent_color(color)
    {
        return if foreground { "39" } else { "49" }.to_string();
    }
    if let Some(Rgba { red, green, blue, .. }) =
        color.as_ref().and_then(Color::rgba)
    {
        return format!(
            "{};2;{red};{green};{blue}",
            if foreground { 38 } else { 48 },
        );
    }
    if foreground { "39" } else { "49" }.to_string()
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn alpha_color_blends_like_reference()
    {
        assert_eq!(
            Color::new("#ff000080").blend_over(&Color::new("#0000ff")).normalized(),
            "#80007f"
        );
        assert_eq!(
            Color::new("#12345600").blend_over(&Color::named("red")),
            Color::named("red"),
        );
        assert_eq!(
            Color::new("#12345600").blend_over(&Color::default()),
            Color::default(),
        );
        assert_eq!(
            Color::new("#12345600").blend_over(&Color::transparent()),
            Color::transparent(),
        );
    }

    #[test]
    fn short_hex_and_text_visual_helpers_match_reference()
    {
        assert_eq!(
            Color::new("#aBc").rgba(),
            Some(Rgba { red: 0xaa, green: 0xbb, blue: 0xcc, alpha: 0xff }),
        );
        assert_eq!(Color::new("#aBc").normalized(), "#aabbcc");
        assert_eq!(Color::new("#aBc").kind(), ColorKind::Rgb);
        assert_eq!(Color::new("RED").as_str(), "red");
        assert_eq!(Color::new("RED").kind(), ColorKind::Named);
        assert_eq!(Color::transparent().kind(), ColorKind::Transparent);
        assert!(Color::transparent().is_transparent());
        assert_eq!(Color::rgb(1, 2, 3).to_string(), "#010203");
        assert_eq!(Color::rgba_color(1, 2, 3, 4).to_string(), "#010203");
        assert_eq!(safe_terminal_cell_text(""), " ");
        assert_eq!(safe_terminal_cell_text("\t"), " ");
        assert_eq!(safe_terminal_cell_text("\u{007f}"), "?");
        assert_eq!(safe_terminal_cell_text("\u{0085}"), "\u{0085}");
        assert_eq!(safe_terminal_cell_text("\u{0301}"), "?");
        assert_eq!(safe_terminal_cell_text("界"), "?");
        assert_eq!(visible_text("a\tb", 1), "a  b");
        assert_eq!(visible_text("\u{0085}\u{0301}界", 0), "\u{0085}??");
        assert_eq!(visible_width("\u{0085}\u{0301}界"), 3);
        assert_eq!(visible_width_at("\tb", 2), 3);
        assert_eq!(tab_spaces_for_column(2), 2);
        assert_eq!(
            TextGradient::default(),
            TextGradient
            {
                interval_ms: 70,
                step: 1,
                segment_size: 1,
                colors: Vec::new(),
            },
        );
    }

    #[test]
    fn style_optional_fields_preserve_absence_and_explicit_values_like_cpp()
    {
        let default_style = Style::default();
        assert!(default_style.color.is_none());
        assert!(default_style.background.is_none());
        assert!(default_style.background_texture.is_none());
        assert!(default_style.background_texture_color.is_none());
        assert!(default_style.border_color.is_none());
        assert!(default_style.text_align.is_none());
        assert!(default_style.user_select.is_none());
        assert_eq!(
            default_style.scope_dim_background,
            Some(Color::new("#00000066")),
        );

        let mut style = Style
        {
            color: Some(Color::new("#123456")),
            background: Some(Color::new("#654321")),
            background_texture: Some("x".to_string()),
            text_align: Some("right".to_string()),
            user_select: Some("text".to_string()),
            ..Style::default()
        };
        style.merge(&Style
        {
            color: Some(Color::default()),
            background: Some(Color::transparent()),
            background_texture: Some(String::new()),
            text_align: Some(String::new()),
            user_select: Some(String::new()),
            scope_dim_background: None,
            ..Style::default()
        });

        assert_eq!(style.color, Some(Color::default()));
        assert_eq!(style.background, Some(Color::transparent()));
        assert_eq!(style.background_texture.as_deref(), Some(""));
        assert_eq!(style.text_align.as_deref(), Some(""));
        assert_eq!(style.user_select.as_deref(), Some(""));
        assert_eq!(
            style.scope_dim_background,
            Some(Color::new("#00000066")),
        );
    }

    #[test]
    fn terminal_cell_optional_color_overlay_matches_cpp_matrix()
    {
        let base_cell = TerminalCell
        {
            text: "B".to_string(),
            foreground: Some(Color::named("white")),
            background: Some(Color::new("#0000ff")),
            ..TerminalCell::default()
        };
        let composite = |foreground: Option<Color>, background: Option<Color>|
        {
            let mut target = TerminalBuffer::new(1, 1);
            target.set_cell(0, 0, base_cell.clone());
            let mut source = TerminalBuffer::new(1, 1);
            source.set_cell(
                0,
                0,
                TerminalCell
                {
                    text: "S".to_string(),
                    foreground,
                    background,
                    ..TerminalCell::default()
                },
            );
            target.overlay(&source, 0, 0);
            target.cell(0, 0).expect("composited cell").clone()
        };

        assert_ne!(None, Some(Color::default()));
        for inherited in [
            None,
            Some(Color::default()),
            Some(Color::transparent()),
            Some(Color::new("#ff000000")),
        ]
        {
            let cell = composite(inherited.clone(), inherited);
            assert_eq!(cell.foreground, base_cell.foreground);
            assert_eq!(cell.background, base_cell.background);
        }

        let named = composite(
            Some(Color::named("yellow")),
            Some(Color::named("navy")),
        );
        assert_eq!(named.foreground, Some(Color::named("yellow")));
        assert_eq!(named.background, Some(Color::named("navy")));

        let opaque = composite(
            Some(Color::new("#00ff00")),
            Some(Color::new("#ff0000")),
        );
        assert_eq!(opaque.foreground, Some(Color::new("#00ff00")));
        assert_eq!(opaque.background, Some(Color::new("#ff0000")));

        let alpha = composite(
            Some(Color::new("#ffffff80")),
            Some(Color::new("#ff000080")),
        );
        assert_eq!(alpha.background, Some(Color::new("#80007f")));
        assert_eq!(alpha.foreground, Some(Color::new("#c080bf")));

        for color in [None, Some(Color::default()), Some(Color::transparent())]
        {
            let mut buffer = TerminalBuffer::new(1, 1);
            buffer.set_cell(
                0,
                0,
                TerminalCell
                {
                    text: "x".to_string(),
                    foreground: color.clone(),
                    background: color,
                    ..TerminalCell::default()
                },
            );
            buffer.request_full_redraw();
            assert!(buffer.render_diff(0, 0).contains("\x1b[39;49m"));
        }
    }

    #[test]
    fn public_layout_resolver_matches_reference_tree_contract()
    {
        const LAYOUT_WIDTH: i32 = 20;
        const LAYOUT_HEIGHT: i32 = 8;
        const NARROW_WIDTH: i32 = 5;
        const GAP: i32 = 1;
        const PADDING: i32 = 2;

        let resolver = LayoutResolver;
        let row = LayoutCell
        {
            name: "root".to_string(),
            width: AxisDimension::fixed(LAYOUT_WIDTH),
            height: AxisDimension::fixed(LAYOUT_HEIGHT),
            gap: GAP,
            direction: LayoutDirection::Row,
            children: vec![
                LayoutCell
                {
                    name: "fixed".to_string(),
                    width: AxisDimension::fixed(NARROW_WIDTH),
                    height: AxisDimension::expanded(),
                    ..Default::default()
                },
                LayoutCell
                {
                    name: "fill".to_string(),
                    width: AxisDimension::expanded(),
                    height: AxisDimension::expanded(),
                    ..Default::default()
                },
            ],
            ..Default::default()
        };
        let resolved = resolver.resolve(&row, Rect::default());
        assert_eq!(resolved.rect.size(), Size { width: LAYOUT_WIDTH, height: LAYOUT_HEIGHT });
        assert_eq!(resolved.children[0].rect.width, NARROW_WIDTH);
        assert_eq!(resolved.children[0].rect.height, LAYOUT_HEIGHT);
        assert_eq!(resolved.children[1].rect.col, NARROW_WIDTH + GAP);
        assert_eq!(
            resolved.children[1].rect.width,
            LAYOUT_WIDTH - NARROW_WIDTH - GAP,
        );

        let lookup = LayoutCell
        {
            name: "lookup-root".to_string(),
            width: AxisDimension::fixed(LAYOUT_WIDTH),
            height: AxisDimension::fixed(LAYOUT_HEIGHT),
            padding: PADDING,
            children: vec![LayoutCell
            {
                name: "child".to_string(),
                width: AxisDimension::expanded(),
                height: AxisDimension::fit_content(),
                fit_content_size: Size { width: 3, height: 2 },
                elements: vec![LayoutElement
                {
                    name: "nested-label".to_string(),
                    relative: Rect { row: 0, col: 1, width: 3, height: 1 },
                    ..Default::default()
                }],
                ..Default::default()
            }],
            ..Default::default()
        };
        let resolved_lookup = resolver.resolve(&lookup, Rect::default());
        assert!(find_cell_by_name(&resolved_lookup, "child").is_some());
        assert!(find_cell_by_name(&resolved_lookup, "missing").is_none());
        assert_eq!(
            find_element_by_name(&resolved_lookup, "nested-label")
                .expect("nested element")
                .rect
                .col,
            PADDING + 1,
        );
        assert_eq!(
            flatten_cells(&resolved_lookup)
                .iter()
                .map(|cell| cell.name.as_str())
                .collect::<Vec<_>>(),
            vec!["lookup-root", "child"],
        );
        let flat_elements = flatten_elements(&resolved_lookup);
        assert_eq!(flat_elements.len(), 1);
        assert_eq!(flat_elements[0].cell_name, "child");
        assert_eq!(flat_elements[0].name, "nested-label");
    }

    #[test]
    #[should_panic(expected = "unsupported color hex length")]
    fn invalid_hex_color_is_rejected_like_reference()
    {
        let _ = Color::new("#12");
    }

    #[test]
    fn buffer_clips_text()
    {
        let mut buffer = TerminalBuffer::new(3, 1);
        buffer.draw_text(0, 1, "abcd", &Color::new("#ffffff"), &Color::new("#000000"), 2);
        assert_eq!(buffer.plain_text(), " ab");
    }

    #[test]
    fn overlay_inherits_transparent_element_colors()
    {
        let mut base = TerminalBuffer::new(1, 1);
        base.fill(
            Rect { row: 0, col: 0, width: 1, height: 1 },
            ' ',
            &Color::new("#94a3b8"),
            &Color::new("#162033"),
        );
        let mut overlay = TerminalBuffer::new(1, 1);
        overlay.fill(
            Rect { row: 0, col: 0, width: 1, height: 1 },
            'U',
            &Color::new("#cbd5e1"),
            &Color::new("transparent"),
        );
        base.overlay(&overlay, 0, 0);
        let cell = base.cell(0, 0).expect("composited cell");
        assert_eq!(cell.text, "U");
        assert_eq!(cell.foreground.normalized(), "#cbd5e1");
        assert_eq!(cell.background.normalized(), "#162033");
    }

    #[test]
    fn terminal_buffer_owns_diff_history_stats_resize_and_raw_scroll_safety()
    {
        let mut empty = TerminalBuffer::new(0, 0);
        assert_eq!(empty.width(), 0);
        assert_eq!(empty.height(), 0);
        assert!(empty.render_diff(0, 0).is_empty());
        empty.resize(0, 2);
        assert_eq!(empty.width(), 0);
        assert_eq!(empty.height(), 2);
        assert!(empty.render_diff(0, 0).is_empty());

        let mut buffer = TerminalBuffer::new(4, 2);
        assert!(buffer.render_diff(0, 0).is_empty());

        buffer.set_cell(
            0,
            0,
            TerminalCell
            {
                text: "Ž".to_string(),
                ..TerminalCell::default()
            },
        );
        let one_cell = buffer.render_diff(0, 0);
        assert!(one_cell.contains("\x1b[1;1H"));
        assert!(one_cell.contains('Ž'));
        assert!(buffer.render_diff(0, 0).is_empty());

        buffer.clear_with(TerminalCell
        {
            text: "x".to_string(),
            ..TerminalCell::default()
        });
        assert!(buffer.cells().iter().all(|cell| cell.text == "x"));
        buffer.clear();
        assert!(buffer.cells().iter().all(|cell| cell == &TerminalCell::default()));

        buffer.set_cell(
            0,
            1,
            TerminalCell
            {
                text: "B".to_string(),
                foreground: Some(Color::new("#ff0000")),
                ..TerminalCell::default()
            },
        );
        buffer.set_cell(
            0,
            2,
            TerminalCell
            {
                text: "C".to_string(),
                foreground: Some(Color::new("#00ff00")),
                ..TerminalCell::default()
            },
        );
        let color_split = buffer.render_diff(0, 0);
        assert!(color_split.contains("38;2;255;0;0"));
        assert!(color_split.contains("38;2;0;255;0"));

        buffer.begin_render_stats();
        buffer.request_full_redraw();
        let full = buffer.render_diff(0, 0);
        assert!(!full.is_empty());
        assert!(buffer.render_stats().full_redraw);
        assert!(buffer.render_stats().changed_cells >= buffer.cells().len());

        buffer.resize(2, 1);
        assert!(buffer.render_diff(0, 0).contains("\x1b[1;1H"));

        let mut raw = TerminalBuffer::new(2, 1);
        {
            let anchor = raw.cell_mut(0, 0).expect("raw anchor");
            anchor.raw = "RAW".to_string();
            anchor.raw_width = 2;
        }
        raw.cell_mut(0, 1).expect("raw coverage").raw_skip = true;
        let payload = raw.render_diff(0, 0);
        assert!(payload.starts_with("\x1b[?2026h"));
        assert!(payload.contains("RAW"));
        assert!(payload.ends_with("\x1b[?2026l"));

        let mut guarded = TerminalBuffer::new(4, 8);
        {
            let anchor = guarded.cell_mut(3, 1).expect("guarded raw anchor");
            anchor.raw = "RAW".to_string();
            anchor.raw_width = 2;
            anchor.raw_height = 5;
        }
        let guarded_payload = guarded.render_diff(0, 0);
        let guard_start = guarded_payload.find("\x1b[1;3r").expect("scroll guard");
        let raw_start = guarded_payload.find("RAW").expect("raw payload");
        let guard_end = guarded_payload[raw_start..].find("\x1b[r").expect("guard reset") + raw_start;
        assert!(guard_start < raw_start && raw_start < guard_end);

        let mut bottom_clipped = TerminalBuffer::new(2, 1);
        {
            let anchor = bottom_clipped.cell_mut(0, 0).expect("raw anchor");
            anchor.raw = "RAW".to_string();
            anchor.raw_width = 2;
            anchor.raw_height = 2;
        }
        bottom_clipped
            .cell_mut(0, 1)
            .expect("raw coverage")
            .raw_skip = true;
        let clipped = bottom_clipped.render_diff(0, 0);
        assert!(!clipped.contains("RAW"));
        assert!(clipped.contains("\x1b[1;1H"));

        fn put_raw_cells(buffer: &mut TerminalBuffer, row: i32, raw: &str)
        {
            for covered_row in row..row + 3
            {
                for covered_col in 2..7
                {
                    buffer
                        .cell_mut(covered_row, covered_col)
                        .expect("raw coverage")
                        .raw_skip = true;
                }
            }
            let anchor = buffer.cell_mut(row, 2).expect("raw anchor");
            anchor.raw = raw.to_string();
            anchor.raw_width = 5;
            anchor.raw_height = 3;
            anchor.raw_skip = false;
        }

        let mut previous_raw = TerminalBuffer::new(20, 10);
        put_raw_cells(&mut previous_raw, 4, "RAW");
        assert!(previous_raw.render_diff(0, 0).contains("RAW"));
        previous_raw.clear();
        put_raw_cells(&mut previous_raw, 3, "RAW");
        assert!(previous_raw.render_scroll_region(0, 0, 10, -1).is_empty());
        assert!(previous_raw.render_diff(0, 0).contains("RAW"));
    }

    #[test]
    fn clipped_overlay_preserves_cells_outside_the_clip()
    {
        let mut target = TerminalBuffer::new(4, 1);
        target.fill(
            Rect { row: 0, col: 0, width: 4, height: 1 },
            ' ',
            &Color::new("#ffffff"),
            &Color::new("#172033"),
        );
        let mut source = TerminalBuffer::new(4, 1);
        source.fill(
            Rect { row: 0, col: 0, width: 4, height: 1 },
            'x',
            &Color::new("#ffffff"),
            &Color::new("#7f1d1d"),
        );

        target.overlay_clipped(
            &source,
            0,
            0,
            Rect { row: 0, col: 1, width: 2, height: 1 },
        );

        assert_eq!(target.cell(0, 0).expect("left cell").text, " ");
        assert_eq!(target.cell(0, 1).expect("first clipped cell").text, "x");
        assert_eq!(target.cell(0, 2).expect("second clipped cell").text, "x");
        assert_eq!(target.cell(0, 3).expect("right cell").text, " ");
    }
}

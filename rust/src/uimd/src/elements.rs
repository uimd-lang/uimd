use crate::{Color, Point, Rect, Style, TerminalBuffer};
#[cfg(test)]
use crate::core::OptionalColorExt;
use std::cell::{Cell, RefCell};
use std::cmp::{max, min};
use std::collections::{BTreeMap, HashMap};
use std::ops::Deref;
use std::rc::{Rc, Weak};
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

pub const COMMIT_MODE_STANDARD: &str = "standard";
pub const COMMIT_MODE_LEAVE: &str = "leave";
pub const DEFAULT_SCROLL_VIEW_GAP: i32 = 0;
pub const DEFAULT_WHEEL_SCROLL_LINES: i32 = 4;
pub const WHEEL_SCROLL_VIEWPORT_FRACTION: i32 = 3;
pub const SCROLL_INDICATOR_ABOVE: &str = "^";
pub const SCROLL_INDICATOR_BELOW: &str = "v";

pub type ElementRef = Rc<RefCell<Element>>;

#[derive(Clone, Debug)]
pub struct Control(ElementRef);

pub type ControlRef = Control;

impl Control
{
    pub fn from_element(element: ElementRef) -> Self
    {
        Self(element)
    }

    pub fn into_element(self) -> ElementRef
    {
        self.0
    }
}

impl Deref for Control
{
    type Target = ElementRef;

    fn deref(&self) -> &Self::Target
    {
        &self.0
    }
}

impl From<ElementRef> for Control
{
    fn from(element: ElementRef) -> Self
    {
        Self::from_element(element)
    }
}

impl From<Control> for ElementRef
{
    fn from(control: Control) -> Self
    {
        control.0
    }
}

macro_rules! typed_element_ref
{
    ($name:ident, $kind:ident) =>
    {
        #[derive(Clone, Debug)]
        pub struct $name(ElementRef);

        impl $name
        {
            pub fn into_element(self) -> ElementRef
            {
                self.0
            }
        }

        impl Deref for $name
        {
            type Target = ElementRef;

            fn deref(&self) -> &Self::Target
            {
                &self.0
            }
        }

        impl From<ElementRef> for $name
        {
            fn from(element: ElementRef) -> Self
            {
                assert_eq!(
                    element.borrow().kind(),
                    ElementKind::$kind,
                    "{} requires an element with kind {:?}",
                    stringify!($name),
                    ElementKind::$kind,
                );
                Self(element)
            }
        }

        impl From<$name> for ElementRef
        {
            fn from(element: $name) -> Self
            {
                element.0
            }
        }

        impl From<$name> for Control
        {
            fn from(element: $name) -> Self
            {
                Self::from_element(element.0)
            }
        }
    };
}

typed_element_ref!(LabelRef, Label);
typed_element_ref!(SpanLabelRef, SpanLabel);
typed_element_ref!(InfoLabelRef, InfoLabel);
typed_element_ref!(FrameBufferViewRef, FrameBufferView);
typed_element_ref!(MessageTableRef, MessageTable);
typed_element_ref!(ImageRef, Image);
typed_element_ref!(ButtonRef, Button);
typed_element_ref!(CheckBoxRef, CheckBox);
typed_element_ref!(TextInputRef, TextInput);
typed_element_ref!(TextAreaRef, TextArea);
typed_element_ref!(NumberInputRef, NumberInput);
typed_element_ref!(ComboBoxRef, ComboBox);
typed_element_ref!(ListBoxRef, ListBox);
typed_element_ref!(ScrollViewRef, ScrollView);
typed_element_ref!(ReusableElementRef, Reusable);
typed_element_ref!(ViewHostRef, ViewHost);

macro_rules! typed_element_common_api
{
    ($($name:ident),+ $(,)?) =>
    {
        $(
            impl $name
            {
                pub fn name(&self) -> String
                {
                    self.borrow().name().to_string()
                }

                pub fn identity(&self) -> u64
                {
                    self.borrow().identity()
                }

                pub fn set_commit_mode(&self, mode: impl Into<String>)
                {
                    self.borrow_mut().set_commit_mode(mode);
                }

                pub fn commit_mode(&self) -> String
                {
                    self.borrow().commit_mode().to_string()
                }

                pub fn set_focusable(&self, focusable: bool)
                {
                    self.borrow_mut().set_focusable(focusable);
                }

                pub fn focusable(&self) -> bool
                {
                    self.borrow().focusable()
                }

                pub fn set_enabled(&self, enabled: bool)
                {
                    self.borrow_mut().set_enabled(enabled);
                }

                pub fn enabled(&self) -> bool
                {
                    self.borrow().enabled()
                }

                pub fn set_frame(&self, frame: Rect)
                {
                    self.borrow_mut().set_frame(frame);
                }

                pub fn frame(&self) -> Rect
                {
                    self.borrow().frame()
                }

                pub fn set_style(&self, style: Style)
                {
                    self.borrow_mut().set_style(style);
                }

                pub fn style(&self) -> Style
                {
                    self.borrow().style().clone()
                }

                pub fn effective_style(&self, state: ElementRenderState) -> Style
                {
                    self.borrow().effective_style(state)
                }

                pub fn set_focus_style(&self, style: Style)
                {
                    self.borrow_mut().set_focus_style(style);
                }

                pub fn focus_style(&self) -> Option<Style>
                {
                    self.borrow().focus_style.clone()
                }

                pub fn set_edit_style(&self, style: Style)
                {
                    self.borrow_mut().set_edit_style(style);
                }

                pub fn edit_style(&self) -> Option<Style>
                {
                    self.borrow().edit_style.clone()
                }

                pub fn set_cursor_style(&self, style: Style)
                {
                    self.borrow_mut().set_cursor_style(style);
                }

                pub fn cursor_style(&self) -> Option<Style>
                {
                    self.borrow().cursor_style.clone()
                }

                pub fn set_selected_style(&self, style: Style)
                {
                    self.borrow_mut().set_selected_style(style);
                }

                pub fn selected_style(&self) -> Option<Style>
                {
                    self.borrow().selected_style.clone()
                }

                pub fn set_active_style(&self, style: Style)
                {
                    self.borrow_mut().set_active_style(style);
                }

                pub fn active_style(&self) -> Option<Style>
                {
                    self.borrow().active_style.clone()
                }

                pub fn set_checked_style(&self, style: Style)
                {
                    self.borrow_mut().set_checked_style(style);
                }

                pub fn checked_style(&self) -> Option<Style>
                {
                    self.borrow().checked_style.clone()
                }

                pub fn set_unchecked_style(&self, style: Style)
                {
                    self.borrow_mut().set_unchecked_style(style);
                }

                pub fn unchecked_style(&self) -> Option<Style>
                {
                    self.borrow().unchecked_style.clone()
                }

                pub fn set_disabled_style(&self, style: Style)
                {
                    self.borrow_mut().set_disabled_style(style);
                }

                pub fn disabled_style(&self) -> Option<Style>
                {
                    self.borrow().disabled_style.clone()
                }

                pub fn parent(&self) -> Option<ElementRef>
                {
                    self.borrow().parent()
                }

                pub fn add_child<T: Into<ElementRef>>(&self, child: T)
                {
                    self.borrow_mut().add_child(child);
                }

                pub fn clear_child_elements(&self)
                {
                    self.borrow_mut().clear_child_elements();
                }

                pub fn children(&self) -> Vec<ElementRef>
                {
                    self.borrow().children().to_vec()
                }

                pub fn render(
                    &self,
                    size: crate::Size,
                    state: ElementRenderState,
                ) -> TerminalBuffer
                {
                    self.borrow().render_with_state(
                        size.width,
                        size.height,
                        state,
                    )
                }
            }
        )+
    };
}

typed_element_common_api!(
    LabelRef,
    SpanLabelRef,
    InfoLabelRef,
    FrameBufferViewRef,
    MessageTableRef,
    ImageRef,
    ButtonRef,
    CheckBoxRef,
    TextInputRef,
    TextAreaRef,
    NumberInputRef,
    ComboBoxRef,
    ListBoxRef,
    ScrollViewRef,
    ReusableElementRef,
    ViewHostRef,
);

macro_rules! typed_label_api
{
    ($($name:ident),+ $(,)?) =>
    {
        $(
            impl $name
            {
                pub fn text(&self) -> String
                {
                    self.borrow().text().to_string()
                }

                pub fn set_text(&self, text: impl Into<String>)
                {
                    self.borrow_mut().set_text(text);
                }

                pub fn set_spans(&self, spans: Vec<LabelSpan>)
                {
                    self.borrow_mut().set_spans(spans);
                }

                pub fn select_range(&self, start: usize, end: usize)
                {
                    self.borrow_mut().select_range(start, end);
                }

                pub fn clear_selection(&self)
                {
                    self.borrow_mut().clear_selection();
                }

                pub fn has_selection(&self) -> bool
                {
                    self.borrow().has_selection()
                }

                pub fn selected_text(&self) -> String
                {
                    self.borrow().selected_text()
                }

                pub fn text_position_from_point(
                    &self,
                    local_row: i32,
                    local_col: i32,
                    size: crate::Size,
                ) -> usize
                {
                    let mut element = self.borrow().clone();
                    element.frame.width = size.width;
                    element.frame.height = size.height;
                    element.cursor_for_point(local_row, local_col)
                }
            }
        )+
    };
}

typed_label_api!(LabelRef, SpanLabelRef, InfoLabelRef);

impl ButtonRef
{
    pub fn title(&self) -> String
    {
        self.borrow().title().to_string()
    }

    pub fn set_title(&self, title: impl Into<String>)
    {
        self.borrow_mut().set_title(title);
    }
}

impl CheckBoxRef
{
    pub fn title(&self) -> String
    {
        self.borrow().title().to_string()
    }

    pub fn checked(&self) -> bool
    {
        self.borrow().checked()
    }

    pub fn set_checked(&self, checked: bool)
    {
        self.borrow_mut().set_checked(checked);
    }

    pub fn handle_key(&self, key: &str) -> bool
    {
        self.borrow_mut().handle_key(key)
    }
}

macro_rules! typed_text_input_api
{
    ($($name:ident),+ $(,)?) =>
    {
        $(
            impl $name
            {
                pub fn value(&self) -> String
                {
                    self.borrow().value().to_string()
                }

                pub fn set_value(&self, value: impl Into<String>)
                {
                    self.borrow_mut().set_value(value);
                }

                pub fn cursor(&self) -> usize
                {
                    self.borrow().cursor()
                }

                pub fn set_cursor(&self, cursor: usize)
                {
                    self.borrow_mut().set_cursor(cursor);
                }

                pub fn select_range(&self, start: usize, end: usize)
                {
                    self.borrow_mut().select_range(start, end);
                }

                pub fn selected_text(&self) -> String
                {
                    self.borrow().selected_text()
                }

                pub fn selection_start(&self) -> usize
                {
                    self.borrow().selection_start()
                }

                pub fn selection_end(&self) -> usize
                {
                    self.borrow().selection_end()
                }

                pub fn has_selection(&self) -> bool
                {
                    self.borrow().has_selection()
                }

                pub fn cursor_for_point(
                    &self,
                    row: i32,
                    col: i32,
                    size: crate::Size,
                ) -> usize
                {
                    let mut element = self.borrow().clone();
                    element.frame.width = size.width;
                    element.frame.height = size.height;
                    element.cursor_for_point(row, col)
                }

                pub fn scroll_by_rows(
                    &self,
                    delta: i32,
                    viewport_height: i32,
                    manual: bool,
                ) -> bool
                {
                    self.borrow_mut()
                        .scroll_text_rows(delta, viewport_height, manual)
                }

                pub fn insert_text(&self, text: &str)
                {
                    self.borrow_mut().insert_text(text);
                }

                pub fn handle_key(&self, key: &str) -> bool
                {
                    self.borrow_mut().handle_key(key)
                }
            }
        )+
    };
}

typed_text_input_api!(TextInputRef, TextAreaRef);

impl NumberInputRef
{
    pub fn value(&self) -> f64
    {
        self.borrow().number_value()
    }

    pub fn set_value(&self, value: f64)
    {
        self.borrow_mut().set_number_value(value);
    }

    pub fn set_edit_cursor(
        &self,
        cursor: usize,
        preserve_replace_on_first_text_input: bool,
    )
    {
        self.borrow_mut().set_number_edit_cursor(
            cursor,
            preserve_replace_on_first_text_input,
        );
    }

    pub fn set_edit_text(&self, text: impl Into<String>)
    {
        self.borrow_mut().set_number_edit_text(text);
    }

    pub fn begin_edit(&self)
    {
        self.borrow_mut().begin_number_edit();
    }

    pub fn cancel_edit(&self)
    {
        self.borrow_mut().cancel_number_edit();
    }

    pub fn commit_edit(&self)
    {
        self.borrow_mut().commit_number_edit();
    }

    pub fn display_value(&self) -> String
    {
        format_number(self.borrow().number_value())
    }

    pub fn handle_key(&self, key: &str) -> bool
    {
        self.borrow_mut().handle_key(key)
    }
}

impl ComboBoxRef
{
    pub fn selected_index(&self) -> i32
    {
        self.borrow().selected_index()
    }

    pub fn options(&self) -> Vec<String>
    {
        self.borrow().options().to_vec()
    }

    pub fn set_options(&self, options: Vec<String>)
    {
        self.borrow_mut().set_options(options);
    }

    pub fn set_selected_index(&self, index: i32)
    {
        self.borrow_mut().set_selected_index(index);
    }

    pub fn handle_key(&self, key: &str) -> bool
    {
        self.borrow_mut().handle_key(key)
    }
}

impl ListBoxRef
{
    pub fn selected_index(&self) -> i32
    {
        self.borrow().selected_index()
    }

    pub fn selected_indices(&self) -> Vec<usize>
    {
        self.borrow().selected_indices().to_vec()
    }

    pub fn selected_values(&self) -> Vec<String>
    {
        self.borrow().selected_values()
    }

    pub fn multiple(&self) -> bool
    {
        self.borrow().multiple()
    }

    pub fn scroll_offset(&self) -> i32
    {
        self.borrow().list_scroll_offset()
    }

    pub fn options(&self) -> Vec<String>
    {
        self.borrow().options().to_vec()
    }

    pub fn disabled_values(&self) -> Vec<String>
    {
        self.borrow().disabled_values().to_vec()
    }

    pub fn active_item_visible(&self) -> bool
    {
        self.borrow().active_item_visible()
    }

    pub fn active_index(&self) -> i32
    {
        self.borrow().active_index()
    }

    pub fn set_options(&self, options: Vec<String>)
    {
        self.borrow_mut().set_options(options);
    }

    pub fn set_disabled_values(&self, values: Vec<String>)
    {
        self.borrow_mut().set_disabled_values(values);
    }

    pub fn set_multiple(&self, multiple: bool)
    {
        self.borrow_mut().set_multiple(multiple);
    }

    pub fn set_selected_index(&self, index: i32)
    {
        self.borrow_mut().set_selected_index(index);
    }

    pub fn set_selected_indices(&self, indices: Vec<usize>)
    {
        self.borrow_mut().set_selected_indices(indices);
    }

    pub fn set_selected_values(&self, values: &[String])
    {
        self.borrow_mut().set_selected_items(values);
    }

    pub fn set_active_index(&self, index: i32)
    {
        self.borrow_mut().set_active_index(index);
    }

    pub fn scroll_by(&self, delta: i32, viewport_height: i32)
    {
        let mut element = self.borrow_mut();
        let previous_height = element.frame.height;
        element.frame.height = viewport_height;
        let offset = element.list_scroll_offset();
        let _ = element.scroll_to(offset + delta);
        element.frame.height = previous_height;
    }

    pub fn show_active_item(&self)
    {
        self.borrow_mut().show_active_item();
    }

    pub fn hide_active_item(&self)
    {
        self.borrow_mut().hide_active_item();
    }

    pub fn handle_key(&self, key: &str) -> bool
    {
        self.borrow_mut().handle_key(key)
    }
}

impl ImageRef
{
    pub fn source(&self) -> String { self.borrow().source().to_string() }
    pub fn alt(&self) -> String { self.borrow().alt().to_string() }
    pub fn fit(&self) -> String { self.borrow().fit().to_string() }
    pub fn render_mode(&self) -> String { self.borrow().render_mode().to_string() }
    pub fn align(&self) -> String { self.borrow().align().to_string() }
    pub fn vertical_align(&self) -> String
    {
        self.borrow().vertical_align().to_string()
    }

    pub fn set_source(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_source(value);
    }

    pub fn set_alt(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_alt(value);
    }

    pub fn set_fit(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_fit(value);
    }

    pub fn set_render_mode(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_render_mode(value);
    }

    pub fn set_align(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_align(value);
    }

    pub fn set_vertical_align(&self, value: impl Into<String>)
    {
        self.borrow_mut().set_vertical_align(value);
    }

    pub fn render_info(
        &self,
        size: crate::Size,
        state: ElementRenderState,
    ) -> crate::ImageRenderInfo
    {
        crate::image_render_info(&self.borrow(), size, state)
    }
}

impl FrameBufferViewRef
{
    pub fn clear_frames(&self)
    {
        self.borrow_mut().clear_frames();
    }

    pub fn set_frames(
        &self,
        frames: BTreeMap<String, Vec<Vec<FrameBufferCell>>>,
        target_order: Vec<String>,
        regions: BTreeMap<String, FrameBufferRegion>,
    )
    {
        self.borrow_mut().set_frames(frames, target_order, regions);
    }

    pub fn set_pan(&self, row: i32, col: i32)
    {
        self.borrow_mut().set_pan(row, col);
    }

    pub fn set_dimmed(&self, dimmed: bool)
    {
        self.borrow_mut().set_dimmed(dimmed);
    }

    pub fn set_interaction_state(&self, focused: bool, edit: bool)
    {
        self.borrow_mut().set_interaction_state(focused, edit);
    }
}

impl MessageTableRef
{
    pub fn text(&self) -> String
    {
        self.borrow().text().to_string()
    }

    pub fn set_text(&self, text: impl Into<String>)
    {
        self.borrow_mut().set_text(text);
    }

    pub fn fit_content_height(&self) -> i32
    {
        message_table_fit_content_height(self.borrow().text())
    }
}

impl ReusableElementRef
{
    pub fn set_child(&self, child: crate::GeneratedWindow)
    {
        self.borrow_mut().set_child_window(child);
    }

    pub fn child(&self) -> Option<crate::GeneratedWindow>
    {
        self.borrow().child_window().cloned()
    }
}

impl ViewHostRef
{
    pub fn set_view(&self, view: crate::GeneratedWindow)
    {
        self.borrow_mut().set_view(view);
    }

    pub fn clear_view(&self)
    {
        self.borrow_mut().clear_view();
    }

    pub fn current_view(&self) -> Option<crate::GeneratedWindow>
    {
        self.borrow().current_view().cloned()
    }
}

impl ScrollViewRef
{
    pub fn set_gap(&self, gap: i32)
    {
        self.borrow_mut()
            .scroll_view_mut()
            .expect("ScrollViewRef must contain ScrollView state")
            .set_gap(gap);
    }

    pub fn gap(&self) -> i32
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .gap()
    }

    pub fn add_rendered_child(&self, child: TerminalBuffer)
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .add_rendered_child(child);
    }

    pub fn clear_children(&self)
    {
        self.borrow_mut().clear_children();
    }

    pub fn invalidate_height_cache(&self)
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .invalidate_height_cache();
    }

    pub fn set_dynamic_children_renderer<F>(&self, renderer: F)
    where
        F: Fn(i32) -> Vec<TerminalBuffer> + 'static,
    {
        self.borrow_mut()
            .scroll_view_mut()
            .expect("ScrollViewRef must contain ScrollView state")
            .set_dynamic_children_renderer(renderer);
    }

    pub fn invalidate_dynamic_children(&self)
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .invalidate_dynamic_children();
    }

    pub fn set_auto_scroll(&self, auto_scroll: bool)
    {
        self.borrow().set_auto_scroll(auto_scroll);
    }

    pub fn set_descendant_focus_style(&self, style: Style)
    {
        self.borrow_mut().set_descendant_focus_style(style);
    }

    pub fn descendant_focus_style(&self) -> Option<Style>
    {
        self.borrow().descendant_focus_style().cloned()
    }

    pub fn auto_scroll(&self) -> bool
    {
        self.borrow().auto_scroll()
    }

    pub fn view_offset(&self) -> i32
    {
        self.borrow().view_offset()
    }

    pub fn scroll_offset(&self) -> i32
    {
        self.borrow().scroll_offset()
    }

    pub fn h_view_offset(&self) -> i32
    {
        self.borrow().h_view_offset()
    }

    pub(crate) fn last_natural_skip(&self) -> i32
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .last_natural_skip()
    }

    pub(crate) fn set_view_offset(&self, view_offset: i32)
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .set_view_offset(view_offset);
    }

    pub(crate) fn refresh_dynamic_children(&self, width: i32)
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .refresh_dynamic_children(width);
    }

    pub(crate) fn rendered_children(&self) -> Vec<TerminalBuffer>
    {
        self.borrow()
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .rendered_children()
            .clone()
    }

    pub fn scroll_position(&self) -> ScrollViewPosition
    {
        self.borrow().scroll_position()
    }

    pub fn restore_scroll_position(&self, position: ScrollViewPosition)
    {
        self.borrow().restore_scroll_position(position);
    }

    pub fn cached_child_heights(&self) -> Vec<i32>
    {
        self.borrow().cached_child_heights()
    }

    pub fn content_height_for_width(&self, width: i32) -> i32
    {
        self.borrow().content_height(width)
    }

    pub fn content_height(&self) -> i32
    {
        self.borrow().content_height_current()
    }

    pub fn max_view_offset(&self, viewport: crate::Size) -> i32
    {
        self.borrow().max_view_offset(viewport)
    }

    pub fn content_window(
        &self,
        viewport: crate::Size,
    ) -> Option<ScrollViewContentWindow>
    {
        self.borrow().content_window(viewport)
    }

    pub fn child_views(&self, viewport: crate::Size) -> Vec<ScrollViewChildView>
    {
        self.borrow().child_views(viewport)
    }

    pub fn scroll_to(&self, index: i32, viewport: crate::Size) -> bool
    {
        let element = self.borrow();
        let viewport = element.scroll_view_padded_viewport(viewport);
        let heights = element.scroll_view_native_child_heights(viewport.width);
        element
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .scroll_to(index, viewport, &heights)
    }

    pub fn scroll_by(&self, delta: i32, viewport: crate::Size) -> bool
    {
        let element = self.borrow();
        let viewport = element.scroll_view_padded_viewport(viewport);
        let heights = element.scroll_view_native_child_heights(viewport.width);
        element
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .scroll_by(delta, viewport, &heights)
    }

    pub fn scroll_lines(&self, delta: i32, viewport: crate::Size) -> bool
    {
        self.borrow().scroll_lines(delta, viewport)
    }

    pub fn scroll_to_top(&self, viewport: crate::Size) -> bool
    {
        self.borrow().scroll_to_top(viewport)
    }

    pub fn scroll_page_up(&self, viewport: crate::Size) -> bool
    {
        self.borrow().scroll_page_up(viewport)
    }

    pub fn scroll_page_down(&self, viewport: crate::Size) -> bool
    {
        self.borrow().scroll_page_down(viewport)
    }

    pub fn scroll_horizontal(&self, delta: i32) -> bool
    {
        self.borrow().scroll_horizontal(delta)
    }

    pub fn wheel_scroll_lines(&self, viewport: crate::Size) -> i32
    {
        self.borrow().wheel_scroll_lines(viewport)
    }

    pub fn can_scroll_up(&self, viewport: crate::Size) -> bool
    {
        self.borrow().can_scroll_up(viewport)
    }

    pub fn can_scroll_down(&self, viewport: crate::Size) -> bool
    {
        self.borrow().can_scroll_down(viewport)
    }

    pub fn handle_wheel(&self, wheel_delta: i32, viewport: crate::Size) -> bool
    {
        self.borrow().handle_wheel(wheel_delta, viewport)
    }

    pub fn handle_key(&self, key: &str, viewport: crate::Size) -> bool
    {
        let element = self.borrow();
        let viewport = element.scroll_view_padded_viewport(viewport);
        let heights = element.scroll_view_native_child_heights(viewport.width);
        let scroll_y = element.style.scroll_y.unwrap_or(true);
        let scroll_x = element.style.scroll_x.unwrap_or(false);
        element
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .handle_key(key, viewport, &heights, scroll_y, scroll_x)
    }

    pub fn ensure_child_visible(
        &self,
        child: &ElementRef,
        viewport: crate::Size,
    ) -> bool
    {
        self.borrow().ensure_child_visible(child, viewport)
    }

    pub fn scroll_to_bottom(&self, viewport: crate::Size) -> bool
    {
        let element = self.borrow();
        let viewport = element.scroll_view_padded_viewport(viewport);
        let heights = element.scroll_view_native_child_heights(viewport.width);
        element
            .scroll_view()
            .expect("ScrollViewRef must contain ScrollView state")
            .scroll_to_bottom(viewport, &heights)
    }

    pub fn consume_terminal_scroll_delta(&self) -> i32
    {
        self.borrow().consume_terminal_scroll_delta()
    }
}

static NEXT_ELEMENT_IDENTITY: AtomicU64 = AtomicU64::new(1);

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct LabelSpan
{
    pub text: String,
    pub foreground: String,
    pub background: String,
}

impl LabelSpan
{
    pub fn new(
        text: impl Into<String>,
        foreground: impl Into<String>,
        background: impl Into<String>,
    ) -> Self
    {
        Self
        {
            text: text.into(),
            foreground: foreground.into(),
            background: background.into(),
        }
    }
}

thread_local!
{
    static RENDER_TIME_OVERRIDE_MS: RefCell<Option<i64>> = const { RefCell::new(None) };
    static ELEMENT_PARENT_BACKGROUND: RefCell<Option<Color>> = const { RefCell::new(None) };
    static LIVE_ELEMENTS: RefCell<HashMap<u64, Weak<RefCell<Element>>>> =
        RefCell::new(HashMap::new());
}

pub struct ScopedRenderTimeOverride
{
    previous: Option<i64>,
}

impl ScopedRenderTimeOverride
{
    pub fn new(time_ms: Option<i64>) -> Self
    {
        let previous = RENDER_TIME_OVERRIDE_MS.with(|current| current.replace(time_ms));
        Self { previous }
    }
}

impl Drop for ScopedRenderTimeOverride
{
    fn drop(&mut self)
    {
        RENDER_TIME_OVERRIDE_MS.with(|current|
        {
            current.replace(self.previous);
        });
    }
}

pub struct ScopedElementParentBackground
{
    previous: Option<Color>,
}

impl ScopedElementParentBackground
{
    pub fn new(background: Option<Color>) -> Self
    {
        let previous = ELEMENT_PARENT_BACKGROUND.with(|current| current.replace(background));
        Self { previous }
    }
}

impl Drop for ScopedElementParentBackground
{
    fn drop(&mut self)
    {
        ELEMENT_PARENT_BACKGROUND.with(|current|
        {
            current.replace(self.previous.clone());
        });
    }
}

pub fn with_render_time_override<T>(time_ms: Option<i64>, render: impl FnOnce() -> T) -> T
{
    let _scope = ScopedRenderTimeOverride::new(time_ms);
    render()
}

pub fn with_element_parent_background<T>(
    background: Option<Color>,
    render: impl FnOnce() -> T,
) -> T
{
    let _scope = ScopedElementParentBackground::new(background);
    render()
}

pub(crate) fn current_element_parent_background() -> Option<Color>
{
    ELEMENT_PARENT_BACKGROUND.with(|current| current.borrow().clone())
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct ElementRenderState
{
    pub focused: bool,
    pub edit_mode: bool,
    pub passive_focus: bool,
    pub clip_top: Option<i32>,
    pub clip_bottom: Option<i32>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct FrameBufferCell
{
    pub text: String,
    pub foreground: String,
    pub background: String,
}

impl Default for FrameBufferCell
{
    fn default() -> Self
    {
        Self
        {
            text: " ".to_string(),
            foreground: String::new(),
            background: String::new(),
        }
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct FrameBufferRegion
{
    pub row: i32,
    pub col: i32,
    pub width: i32,
    pub height: i32,
}

impl Default for FrameBufferRegion
{
    fn default() -> Self
    {
        Self { row: 0, col: 0, width: 1, height: 1 }
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum ElementKind
{
    Label,
    SpanLabel,
    InfoLabel,
    FrameBufferView,
    MessageTable,
    Image,
    TextInput,
    TextArea,
    NumberInput,
    Button,
    CheckBox,
    ComboBox,
    ListBox,
    ScrollView,
    Reusable,
    ViewHost,
}

#[derive(Clone, Debug, Default)]
pub struct FrameBufferViewState
{
    frames: BTreeMap<String, Vec<Vec<FrameBufferCell>>>,
    target_order: Vec<String>,
    regions: BTreeMap<String, FrameBufferRegion>,
    pan_row: i32,
    pan_col: i32,
    dimmed: bool,
    panel_focused: bool,
    panel_edit: bool,
}

#[derive(Clone, Debug)]
pub struct NumberInputState
{
    value: f64,
    step: f64,
    edit: RefCell<NumberInputEditState>,
}

#[derive(Clone, Debug)]
struct NumberInputEditState
{
    edit_text: String,
    edit_cursor: usize,
    edit_original_value: f64,
    editing: bool,
    replace_on_first_text_input: bool,
}

impl Default for NumberInputState
{
    fn default() -> Self
    {
        Self
        {
            value: 0.0,
            step: 1.0,
            edit: RefCell::new(NumberInputEditState
            {
                edit_text: String::new(),
                edit_cursor: 0,
                edit_original_value: 0.0,
                editing: false,
                replace_on_first_text_input: false,
            }),
        }
    }
}

#[derive(Clone, Debug)]
pub struct ImageState
{
    source: String,
    alt: String,
    fit: String,
    render_mode: String,
    align: String,
    vertical_align: String,
}

impl Default for ImageState
{
    fn default() -> Self
    {
        Self
        {
            source: String::new(),
            alt: String::new(),
            fit: "contain".to_string(),
            render_mode: "auto".to_string(),
            align: "center".to_string(),
            vertical_align: "middle".to_string(),
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct ReusableElementState
{
    reusable_type: String,
    child: Option<Box<crate::GeneratedWindow>>,
}

#[derive(Clone, Debug, Default)]
pub struct ButtonState
{
    title: String,
}

#[derive(Clone, Debug, Default)]
pub struct CheckBoxState
{
    title: String,
    checked: bool,
}

#[derive(Clone, Debug, Default)]
pub struct LabelState
{
    text: String,
    spans: Vec<LabelSpan>,
    selection_start: Option<usize>,
    selection_end: Option<usize>,
}

#[derive(Clone, Debug)]
pub struct TextInputState
{
    value: String,
    cursor: usize,
    max_length: usize,
    multiline: bool,
    selection_anchor: Option<usize>,
    col_scroll_offset: Cell<i32>,
    row_scroll_offset: Cell<i32>,
    manual_row_scroll: Cell<bool>,
}

impl TextInputState
{
    fn new(value: &str, max_length: i32, multiline: bool) -> Self
    {
        let value = if multiline
        {
            value.to_string()
        }
        else
        {
            value
                .chars()
                .map(|ch| if matches!(ch, '\r' | '\n') { ' ' } else { ch })
                .collect()
        };
        let cursor = value.len();
        Self
        {
            value,
            cursor,
            max_length: max(0, max_length) as usize,
            multiline,
            selection_anchor: None,
            col_scroll_offset: Cell::new(0),
            row_scroll_offset: Cell::new(0),
            manual_row_scroll: Cell::new(false),
        }
    }

    fn set_value(&mut self, value: impl Into<String>)
    {
        let mut value = value.into();
        if !self.multiline
        {
            value = value
                .chars()
                .map(|ch| if matches!(ch, '\r' | '\n') { ' ' } else { ch })
                .collect();
        }
        self.value = value;
        self.set_cursor(self.cursor);
    }

    fn set_cursor(&mut self, cursor: usize)
    {
        self.manual_row_scroll.set(false);
        self.cursor = clamp_utf8_boundary(
            &self.value,
            min(cursor, self.value.len()),
        );
    }

    fn select_range(&mut self, anchor: usize, cursor: usize)
    {
        self.manual_row_scroll.set(false);
        let anchor = clamp_utf8_boundary(
            &self.value,
            min(anchor, self.value.len()),
        );
        let cursor = clamp_utf8_boundary(
            &self.value,
            min(cursor, self.value.len()),
        );
        self.cursor = cursor;
        self.selection_anchor = (anchor != cursor).then_some(anchor);
    }

    fn selection(&self) -> Option<(usize, usize)>
    {
        self.selection_anchor.and_then(|anchor|
        {
            (anchor != self.cursor).then_some((
                min(anchor, self.cursor).min(self.value.len()),
                max(anchor, self.cursor).min(self.value.len()),
            ))
        })
    }

    fn selected_text(&self) -> String
    {
        let Some((start, end)) = self.selection() else
        {
            return String::new();
        };
        self.value.get(start..end).unwrap_or_default().to_string()
    }

    fn delete_selection(&mut self)
    {
        let Some((start, end)) = self.selection() else
        {
            return;
        };
        self.value.replace_range(start..end, "");
        self.cursor = start;
        self.selection_anchor = None;
    }

    fn cursor_for_point(&self, local_row: i32, local_col: i32, size: crate::Size) -> usize
    {
        let length = self.value.len();
        let width = max(1, size.width) as usize;
        let column = local_col.max(0) as usize;
        if !self.multiline
        {
            let row = single_visual_text_row(&self.value);
            return min(
                length,
                raw_index_for_visual_column(
                    &row,
                    self.col_scroll_offset.get().max(0) as usize + column,
                ),
            );
        }

        let rows = build_visual_text_rows(&self.value, width);
        let height = max(1, size.height) as usize;
        let max_offset = rows.len().saturating_sub(height);
        let row_offset = min(self.row_scroll_offset.get().max(0) as usize, max_offset);
        let row_index =
            row_offset + (local_row.max(0) as usize).min(height.saturating_sub(1));
        if row_index >= rows.len()
        {
            return length;
        }
        let row = &rows[row_index];
        raw_index_for_visual_column(row, column).clamp(row.start, row.end)
    }

    fn scroll_by_rows(&mut self, delta: i32, size: crate::Size, manual: bool) -> bool
    {
        if !self.multiline
        {
            return false;
        }
        let height = max(1, size.height);
        if height <= 1
        {
            return false;
        }
        let rows = build_visual_text_rows(
            &self.value,
            max(1, size.width) as usize,
        );
        let max_offset = max(0, rows.len() as i32 - height);
        let next = (self.row_scroll_offset.get() + delta).clamp(0, max_offset);
        if next == self.row_scroll_offset.get()
        {
            return false;
        }
        self.row_scroll_offset.set(next);
        self.manual_row_scroll.set(manual);
        true
    }

    fn max_scroll_offset(&self, size: crate::Size) -> i32
    {
        if !self.multiline
        {
            return 0;
        }
        let rows = build_visual_text_rows(
            &self.value,
            max(1, size.width) as usize,
        );
        max(0, rows.len() as i32 - max(1, size.height))
    }

    #[allow(clippy::needless_return)]
    fn handle_key(&mut self, key: &str, width: i32) -> bool
    {
        self.manual_row_scroll.set(false);
        let selection = self.selection();
        match key
        {
            "Left" =>
            {
                if let Some((start, _)) = selection
                {
                    self.cursor = start;
                    self.selection_anchor = None;
                }
                else
                {
                    self.cursor = previous_utf8_boundary(&self.value, self.cursor);
                }
                true
            }
            "Right" =>
            {
                if let Some((_, end)) = selection
                {
                    self.cursor = end;
                    self.selection_anchor = None;
                }
                else
                {
                    self.cursor = next_utf8_boundary(&self.value, self.cursor);
                }
                true
            }
            "Alt+Left" | "Alt+Right" =>
            {
                self.selection_anchor = None;
                for _ in 0..5
                {
                    self.cursor = if key == "Alt+Left"
                    {
                        previous_utf8_boundary(&self.value, self.cursor)
                    }
                    else
                    {
                        next_utf8_boundary(&self.value, self.cursor)
                    };
                }
                true
            }
            "Up" | "Down" if self.multiline =>
            {
                self.cursor = move_multiline_cursor(
                    &self.value,
                    max(1, width) as usize,
                    self.cursor,
                    if key == "Up" { -1 } else { 1 },
                );
                true
            }
            "Alt+Up" | "Alt+Down" if self.multiline =>
            {
                self.selection_anchor = None;
                for _ in 0..3
                {
                    self.cursor = move_multiline_cursor(
                        &self.value,
                        max(1, width) as usize,
                        self.cursor,
                        if key == "Alt+Up" { -1 } else { 1 },
                    );
                }
                true
            }
            "Shift+Left" | "Shift+Right" =>
            {
                if self.selection_anchor.is_none()
                {
                    self.selection_anchor = Some(self.cursor);
                }
                self.cursor = if key == "Shift+Left"
                {
                    previous_utf8_boundary(&self.value, self.cursor)
                }
                else
                {
                    next_utf8_boundary(&self.value, self.cursor)
                };
                true
            }
            "Shift+Up" | "Shift+Down" if self.multiline =>
            {
                if self.selection_anchor.is_none()
                {
                    self.selection_anchor = Some(self.cursor);
                }
                self.cursor = move_multiline_cursor(
                    &self.value,
                    max(1, width) as usize,
                    self.cursor,
                    if key == "Shift+Up" { -1 } else { 1 },
                );
                true
            }
            "Home" =>
            {
                self.cursor = if self.multiline
                {
                    line_start_for_cursor(&self.value, self.cursor)
                }
                else
                {
                    0
                };
                true
            }
            "End" =>
            {
                self.cursor = if self.multiline
                {
                    line_end_for_cursor(&self.value, self.cursor)
                }
                else
                {
                    self.value.len()
                };
                true
            }
            "Backspace" =>
            {
                if selection.is_some()
                {
                    self.delete_selection();
                }
                else if self.cursor > 0
                {
                    let start = previous_utf8_boundary(&self.value, self.cursor);
                    self.value.replace_range(start..self.cursor, "");
                    self.cursor = start;
                }
                true
            }
            "Enter" | "Alt+Enter" if self.multiline =>
            {
                if self.max_length > 0
                    && self.value.len() >= self.max_length
                    && selection.is_none()
                {
                    return true;
                }
                if selection.is_some()
                {
                    self.delete_selection();
                }
                self.value.insert(self.cursor, '\n');
                self.cursor += 1;
                self.selection_anchor = None;
                true
            }
            _ =>
            {
                if key.chars().count() != 1
                {
                    return false;
                }
                let replaced_bytes = selection.map_or(0, |(start, end)| end - start);
                if self.max_length > 0
                    && ((selection.is_none() && self.value.len() >= self.max_length)
                        || (!key.is_ascii()
                            && self
                                .value
                                .len()
                                .saturating_sub(replaced_bytes)
                                .saturating_add(key.len())
                                > self.max_length))
                {
                    return true;
                }
                if selection.is_some()
                {
                    self.delete_selection();
                }
                self.value.insert_str(self.cursor, key);
                self.cursor += key.len();
                self.selection_anchor = None;
                true
            }
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct MessageTableState
{
    text: String,
}

#[derive(Clone, Debug, Default)]
pub struct ComboBoxState
{
    options: Vec<String>,
    selected_index: i32,
}

#[derive(Clone, Debug, Default)]
pub struct ListBoxState
{
    options: Vec<String>,
    disabled_values: Vec<String>,
    selected_index: i32,
    active_index: i32,
    selected_indices: Vec<usize>,
    multiple: bool,
    scroll_offset: Cell<i32>,
    active_item_visible: bool,
    last_viewport_height: Cell<i32>,
}

fn ensure_list_index_visible(state: &ListBoxState, index: i32, height: i32)
{
    let mut scroll_offset = state.scroll_offset.get();
    if index < scroll_offset
    {
        scroll_offset = index;
    }
    else if index >= scroll_offset + height
    {
        scroll_offset = index - height + 1;
    }
    let max_offset = max(0, state.options.len() as i32 - max(1, height));
    state.scroll_offset.set(scroll_offset.clamp(0, max_offset));
}

#[derive(Clone, Debug)]
pub struct ScrollViewChildView
{
    pub element: Option<ElementRef>,
    pub frame: Rect,
    pub visible: bool,
    pub index: i32,
    pub clipped: bool,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct ScrollViewContentWindow
{
    pub viewport: crate::Size,
    pub actual_skip: i32,
    pub natural_skip: i32,
    pub gap: i32,
}

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct ScrollViewPosition
{
    pub scroll_offset: i32,
    pub view_offset: i32,
    pub auto_scroll: bool,
}

#[derive(Clone)]
struct ScrollViewDynamicChildrenRenderer
{
    callback: Rc<dyn Fn(i32) -> Vec<TerminalBuffer>>,
}

impl std::fmt::Debug for ScrollViewDynamicChildrenRenderer
{
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        formatter
            .debug_struct("ScrollViewDynamicChildrenRenderer")
            .finish_non_exhaustive()
    }
}

#[derive(Clone, Debug)]
pub struct ScrollViewState
{
    gap: i32,
    auto_scroll: Cell<bool>,
    scroll_offset: Cell<i32>,
    view_offset: Cell<i32>,
    h_view_offset: Cell<i32>,
    last_natural_skip: Cell<i32>,
    pending_terminal_scroll_delta: Cell<i32>,
    rendered_children: RefCell<Vec<TerminalBuffer>>,
    dynamic_children_renderer: Option<ScrollViewDynamicChildrenRenderer>,
    dynamic_children_width: Cell<Option<i32>>,
    cached_child_heights_width: Cell<Option<i32>>,
    cached_child_heights: RefCell<Option<Vec<i32>>>,
    descendant_focus_style: Option<Style>,
}

impl Default for ScrollViewState
{
    fn default() -> Self
    {
        Self::new(DEFAULT_SCROLL_VIEW_GAP)
    }
}

impl ScrollViewState
{
    pub fn new(gap: i32) -> Self
    {
        Self
        {
            gap: max(0, gap),
            auto_scroll: Cell::new(false),
            scroll_offset: Cell::new(0),
            view_offset: Cell::new(0),
            h_view_offset: Cell::new(0),
            last_natural_skip: Cell::new(0),
            pending_terminal_scroll_delta: Cell::new(0),
            rendered_children: RefCell::new(Vec::new()),
            dynamic_children_renderer: None,
            dynamic_children_width: Cell::new(None),
            cached_child_heights_width: Cell::new(None),
            cached_child_heights: RefCell::new(None),
            descendant_focus_style: None,
        }
    }

    pub fn gap(&self) -> i32
    {
        self.gap
    }

    pub fn set_gap(&mut self, gap: i32)
    {
        self.gap = max(0, gap);
        self.invalidate_height_cache();
    }

    pub fn set_auto_scroll(&self, auto_scroll: bool)
    {
        self.auto_scroll.set(auto_scroll);
        if auto_scroll
        {
            self.view_offset.set(0);
        }
    }

    pub fn auto_scroll(&self) -> bool
    {
        self.auto_scroll.get()
    }

    pub fn scroll_offset(&self) -> i32
    {
        self.scroll_offset.get()
    }

    pub fn view_offset(&self) -> i32
    {
        self.view_offset.get()
    }

    pub fn h_view_offset(&self) -> i32
    {
        self.h_view_offset.get()
    }

    pub(crate) fn last_natural_skip(&self) -> i32
    {
        self.last_natural_skip.get()
    }

    pub(crate) fn set_view_offset(&self, value: i32)
    {
        self.view_offset.set(max(0, value));
    }

    pub fn scroll_position(&self) -> ScrollViewPosition
    {
        ScrollViewPosition
        {
            scroll_offset: self.scroll_offset.get(),
            view_offset: self.view_offset.get(),
            auto_scroll: self.auto_scroll.get(),
        }
    }

    pub fn set_descendant_focus_style(&mut self, style: Style)
    {
        self.descendant_focus_style = Some(style);
    }

    pub fn descendant_focus_style(&self) -> Option<&Style>
    {
        self.descendant_focus_style.as_ref()
    }

    pub fn add_rendered_child(&self, child: TerminalBuffer)
    {
        self.rendered_children.borrow_mut().push(child);
        self.invalidate_height_cache();
        if self.auto_scroll.get()
        {
            self.view_offset.set(0);
        }
    }

    pub fn clear_children_state(&self)
    {
        self.rendered_children.borrow_mut().clear();
        self.scroll_offset.set(0);
        self.view_offset.set(0);
        self.last_natural_skip.set(0);
        self.dynamic_children_width.set(None);
        self.invalidate_height_cache();
    }

    pub fn set_dynamic_children_renderer<F>(&mut self, renderer: F)
    where
        F: Fn(i32) -> Vec<TerminalBuffer> + 'static,
    {
        self.dynamic_children_renderer = Some(ScrollViewDynamicChildrenRenderer
        {
            callback: Rc::new(renderer),
        });
        self.invalidate_dynamic_children();
    }

    pub fn invalidate_dynamic_children(&self)
    {
        self.dynamic_children_width.set(None);
        self.invalidate_height_cache();
    }

    pub(crate) fn refresh_dynamic_children(&self, width: i32)
    {
        let width = max(1, width);
        if self.dynamic_children_width.get() == Some(width)
        {
            return;
        }
        let Some(renderer) = self.dynamic_children_renderer.as_ref() else
        {
            return;
        };
        self.rendered_children.replace((renderer.callback)(width));
        self.dynamic_children_width.set(Some(width));
        self.invalidate_height_cache();
        if self.auto_scroll.get()
        {
            self.view_offset.set(0);
        }
    }

    pub(crate) fn rendered_children(&self) -> std::cell::Ref<'_, Vec<TerminalBuffer>>
    {
        self.rendered_children.borrow()
    }

    pub(crate) fn rendered_child_count(&self) -> usize
    {
        self.rendered_children.borrow().len()
    }

    pub fn invalidate_height_cache(&self)
    {
        self.cached_child_heights_width.set(None);
        self.cached_child_heights.replace(None);
    }

    fn ensure_height_cache(&self, width: i32, native_child_heights: &[i32])
    {
        let width = max(1, width);
        if self.cached_child_heights_width.get() == Some(width)
            && self
                .cached_child_heights
                .borrow()
                .as_ref()
                .is_some_and(|heights|
                {
                    heights.len()
                        == self.rendered_children.borrow().len()
                            + native_child_heights.len()
                })
        {
            return;
        }
        self.refresh_dynamic_children(width);
        let rendered_children = self.rendered_children.borrow();
        let mut heights = Vec::with_capacity(
            rendered_children.len() + native_child_heights.len(),
        );
        heights.extend(rendered_children.iter().map(|child| child.height));
        heights.extend(native_child_heights.iter().map(|height| max(0, *height)));
        self.cached_child_heights_width.set(Some(width));
        self.cached_child_heights.replace(Some(heights));
    }

    pub fn cached_child_heights(
        &self,
        width: i32,
        native_child_heights: &[i32],
    ) -> Vec<i32>
    {
        self.ensure_height_cache(width, native_child_heights);
        self.cached_child_heights
            .borrow()
            .as_ref()
            .cloned()
            .unwrap_or_default()
    }

    pub fn content_height(&self, width: i32, native_child_heights: &[i32]) -> i32
    {
        let heights = self.cached_child_heights(width, native_child_heights);
        let offset = self.scroll_offset.get().clamp(0, heights.len() as i32) as usize;
        let count = heights.len().saturating_sub(offset);
        heights[offset..].iter().sum::<i32>() + self.gap * max(0, count as i32 - 1)
    }

    #[allow(clippy::if_same_then_else)]
    fn sync_view_offset_for_natural_skip(&self, natural_skip: i32)
    {
        let previous_natural_skip = max(0, self.last_natural_skip.get());
        let natural_skip = max(0, natural_skip);
        if self.auto_scroll.get()
        {
            self.view_offset.set(0);
        }
        else if self.view_offset.get() >= previous_natural_skip
        {
            self.view_offset.set(natural_skip);
        }
        else if self.view_offset.get() > natural_skip
        {
            self.view_offset.set(natural_skip);
        }
    }

    pub fn max_view_offset(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> i32
    {
        let natural_skip = max(
            0,
            self.content_height(max(1, viewport.width), native_child_heights)
                - max(1, viewport.height),
        );
        self.sync_view_offset_for_natural_skip(natural_skip);
        self.last_natural_skip.set(natural_skip);
        natural_skip
    }

    pub fn content_window(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> Option<ScrollViewContentWindow>
    {
        self.refresh_dynamic_children(max(1, viewport.width));
        let total = self.rendered_children.borrow().len() + native_child_heights.len();
        if total == 0
        {
            self.last_natural_skip.set(0);
            return Some(ScrollViewContentWindow
            {
                viewport,
                actual_skip: 0,
                natural_skip: 0,
                gap: 0,
            });
        }
        let natural_skip = self.max_view_offset(viewport, native_child_heights);
        let clamped_view_offset = min(self.view_offset.get(), natural_skip);
        Some(ScrollViewContentWindow
        {
            viewport,
            actual_skip: natural_skip - clamped_view_offset,
            natural_skip,
            gap: max(0, self.gap),
        })
    }

    fn max_children_in_viewport(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> i32
    {
        self.refresh_dynamic_children(max(1, viewport.width));
        let total = self.rendered_children.borrow().len() + native_child_heights.len();
        if total == 0
        {
            return 0;
        }
        let sample_size = min(WHEEL_SCROLL_VIEWPORT_FRACTION + 2, total as i32);
        let heights = self.cached_child_heights(viewport.width, native_child_heights);
        let measured: i32 = heights.iter().take(sample_size as usize).sum();
        let average_height = max(
            1,
            if sample_size == 0 { 1 } else { measured / sample_size },
        );
        let denominator = average_height + self.gap;
        if denominator <= 0
        {
            return 1;
        }
        max(1, (max(1, viewport.height) + self.gap) / denominator)
    }

    pub fn scroll_to(
        &self,
        index: i32,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        self.refresh_dynamic_children(max(1, viewport.width));
        let total = self.rendered_children.borrow().len() + native_child_heights.len();
        let previous = self.scroll_offset.get();
        if total == 0
        {
            self.scroll_offset.set(0);
            return previous != 0;
        }
        let max_visible = self.max_children_in_viewport(viewport, native_child_heights);
        let max_offset = max(0, total as i32 - max_visible);
        self.scroll_offset.set(index.clamp(0, max_offset));
        if self.scroll_offset.get() == 0
        {
            self.auto_scroll.set(false);
        }
        self.scroll_offset.get() != previous
    }

    pub fn scroll_by(
        &self,
        delta: i32,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        self.scroll_to(
            self.scroll_offset.get() + delta,
            viewport,
            native_child_heights,
        )
    }

    pub fn scroll_lines(
        &self,
        delta: i32,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        let max_offset = self.max_view_offset(viewport, native_child_heights);
        let previous = self.view_offset.get().clamp(0, max_offset);
        self.view_offset
            .set((previous + delta).clamp(0, max_offset));
        self.auto_scroll.set(self.view_offset.get() == 0);
        if self.view_offset.get() != previous
        {
            self.pending_terminal_scroll_delta.set(
                self.pending_terminal_scroll_delta.get()
                    + self.view_offset.get()
                    - previous,
            );
        }
        self.view_offset.get() != previous
    }

    pub fn scroll_to_top(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        let previous = self.view_offset.get();
        self.view_offset
            .set(self.max_view_offset(viewport, native_child_heights));
        self.auto_scroll.set(false);
        self.view_offset.get() != previous
    }

    pub fn scroll_to_bottom(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        let _ = self.max_view_offset(viewport, native_child_heights);
        let changed = self.scroll_offset.get() != 0
            || self.view_offset.get() != 0
            || !self.auto_scroll.get();
        if self.view_offset.get() != 0
        {
            self.pending_terminal_scroll_delta.set(
                self.pending_terminal_scroll_delta.get() - self.view_offset.get(),
            );
        }
        self.scroll_offset.set(0);
        self.view_offset.set(0);
        self.auto_scroll.set(true);
        changed
    }

    pub fn scroll_horizontal(&self, delta: i32) -> bool
    {
        let previous = self.h_view_offset.get();
        self.h_view_offset.set(max(0, previous + delta));
        self.h_view_offset.get() != previous
    }

    pub fn scroll_page_up(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        self.scroll_lines(max(1, viewport.height), viewport, native_child_heights)
    }

    pub fn scroll_page_down(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        self.scroll_lines(-max(1, viewport.height), viewport, native_child_heights)
    }

    pub fn wheel_scroll_lines(&self, viewport: crate::Size) -> i32
    {
        let viewport_limited =
            max(1, max(1, viewport.height) / WHEEL_SCROLL_VIEWPORT_FRACTION);
        viewport_limited.clamp(1, DEFAULT_WHEEL_SCROLL_LINES)
    }

    pub fn can_scroll_up(&self) -> bool
    {
        self.scroll_offset.get() > 0
    }

    pub fn can_scroll_down(
        &self,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        self.refresh_dynamic_children(max(1, viewport.width));
        let total = self.rendered_children.borrow().len() + native_child_heights.len();
        total > 0
            && total as i32 - self.scroll_offset.get()
                > self.max_children_in_viewport(viewport, native_child_heights)
    }

    pub fn handle_wheel(
        &self,
        wheel_delta: i32,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        if wheel_delta == 0
        {
            return false;
        }
        let step = self.wheel_scroll_lines(viewport) * max(1, wheel_delta.abs());
        self.scroll_lines(
            if wheel_delta > 0 { step } else { -step },
            viewport,
            native_child_heights,
        )
    }

    pub fn handle_key(
        &self,
        key: &str,
        viewport: crate::Size,
        native_child_heights: &[i32],
        scroll_y: bool,
        scroll_x: bool,
    ) -> bool
    {
        match key
        {
            "Alt+Up" =>
                self.scroll_lines(
                    self.wheel_scroll_lines(viewport),
                    viewport,
                    native_child_heights,
                ),
            "Alt+Down" =>
                self.scroll_lines(
                    -self.wheel_scroll_lines(viewport),
                    viewport,
                    native_child_heights,
                ),
            "Ctrl+Up" => self.scroll_page_up(viewport, native_child_heights),
            "Ctrl+Down" => self.scroll_page_down(viewport, native_child_heights),
            "ArrowUp" | "Up" if scroll_y =>
                self.scroll_lines(1, viewport, native_child_heights),
            "ArrowDown" | "Down" if scroll_y =>
                self.scroll_lines(-1, viewport, native_child_heights),
            "PageUp" if scroll_y => self.scroll_page_up(viewport, native_child_heights),
            "PageDown" if scroll_y =>
                self.scroll_page_down(viewport, native_child_heights),
            "Home" if scroll_y => self.scroll_to_top(viewport, native_child_heights),
            "End" if scroll_y => self.scroll_to_bottom(viewport, native_child_heights),
            "ArrowLeft" | "Left" if scroll_x => self.scroll_horizontal(-1),
            "ArrowRight" | "Right" if scroll_x => self.scroll_horizontal(1),
            _ => false,
        }
    }

    pub fn ensure_item_visible(
        &self,
        index: usize,
        viewport: crate::Size,
        native_child_heights: &[i32],
    ) -> bool
    {
        let heights = self.cached_child_heights(viewport.width, native_child_heights);
        let scroll_offset = self.scroll_offset.get().clamp(0, heights.len() as i32) as usize;
        if index < scroll_offset || index >= heights.len()
        {
            return false;
        }
        let mut row = 0;
        for item in scroll_offset..index
        {
            row += heights[item];
            if item + 1 < heights.len()
            {
                row += self.gap;
            }
        }
        let child_height = heights[index];
        let natural_skip = self.max_view_offset(viewport, native_child_heights);
        let current_actual_skip =
            natural_skip - min(self.view_offset.get(), natural_skip);
        let mut next_actual_skip = current_actual_skip;
        if row < current_actual_skip
        {
            next_actual_skip = row;
        }
        else if row + child_height > current_actual_skip + max(1, viewport.height)
        {
            next_actual_skip = row + child_height - max(1, viewport.height);
        }
        next_actual_skip = next_actual_skip.clamp(0, natural_skip);
        let next_view_offset = natural_skip - next_actual_skip;
        let previous = self.view_offset.get();
        self.view_offset.set(next_view_offset);
        self.auto_scroll.set(self.view_offset.get() == 0);
        self.view_offset.get() != previous
    }

    pub fn consume_terminal_scroll_delta(&self) -> i32
    {
        let delta = self.pending_terminal_scroll_delta.get();
        self.pending_terminal_scroll_delta.set(0);
        delta
    }

    pub fn restore_scroll_position(
        &self,
        position: ScrollViewPosition,
        viewport: crate::Size,
        native_child_heights: &[i32],
    )
    {
        self.refresh_dynamic_children(max(1, viewport.width));
        let total = self.rendered_children.borrow().len() + native_child_heights.len();
        self.scroll_offset
            .set(position.scroll_offset.clamp(0, max(0, total as i32 - 1)));
        self.view_offset.set(max(0, position.view_offset));
        self.auto_scroll.set(position.auto_scroll);
        self.invalidate_height_cache();
        let natural_skip = max(
            0,
            self.content_height(viewport.width, native_child_heights)
                - max(1, viewport.height),
        );
        self.view_offset.set(if self.auto_scroll.get()
        {
            0
        }
        else
        {
            min(self.view_offset.get(), natural_skip)
        });
        self.last_natural_skip.set(natural_skip);
        self.pending_terminal_scroll_delta.set(0);
    }
}

#[derive(Clone, Debug)]
pub enum ElementData
{
    Label(LabelState),
    SpanLabel(LabelState),
    InfoLabel(LabelState),
    FrameBufferView(FrameBufferViewState),
    MessageTable(MessageTableState),
    Image(ImageState),
    TextInput(TextInputState),
    TextArea(TextInputState),
    NumberInput(NumberInputState),
    Button(ButtonState),
    CheckBox(CheckBoxState),
    ComboBox(ComboBoxState),
    ListBox(ListBoxState),
    ScrollView(Box<ScrollViewState>),
    Reusable(ReusableElementState),
    ViewHost(ReusableElementState),
}

impl ElementData
{
    fn for_kind(kind: &ElementKind) -> Self
    {
        match kind
        {
            ElementKind::Label => Self::Label(LabelState::default()),
            ElementKind::SpanLabel => Self::SpanLabel(LabelState::default()),
            ElementKind::InfoLabel => Self::InfoLabel(LabelState::default()),
            ElementKind::FrameBufferView =>
                Self::FrameBufferView(FrameBufferViewState::default()),
            ElementKind::MessageTable => Self::MessageTable(MessageTableState::default()),
            ElementKind::Image => Self::Image(ImageState::default()),
            ElementKind::TextInput => Self::TextInput(TextInputState::new("", 0, false)),
            ElementKind::TextArea => Self::TextArea(TextInputState::new("", 0, true)),
            ElementKind::NumberInput => Self::NumberInput(NumberInputState::default()),
            ElementKind::Button => Self::Button(ButtonState::default()),
            ElementKind::CheckBox => Self::CheckBox(CheckBoxState::default()),
            ElementKind::ComboBox => Self::ComboBox(ComboBoxState::default()),
            ElementKind::ListBox => Self::ListBox(ListBoxState::default()),
            ElementKind::ScrollView => Self::ScrollView(Box::default()),
            ElementKind::Reusable => Self::Reusable(ReusableElementState::default()),
            ElementKind::ViewHost => Self::ViewHost(ReusableElementState::default()),
        }
    }

    fn kind(&self) -> ElementKind
    {
        match self
        {
            Self::Label(_) => ElementKind::Label,
            Self::SpanLabel(_) => ElementKind::SpanLabel,
            Self::InfoLabel(_) => ElementKind::InfoLabel,
            Self::FrameBufferView(_) => ElementKind::FrameBufferView,
            Self::MessageTable(_) => ElementKind::MessageTable,
            Self::Image(_) => ElementKind::Image,
            Self::TextInput(_) => ElementKind::TextInput,
            Self::TextArea(_) => ElementKind::TextArea,
            Self::NumberInput(_) => ElementKind::NumberInput,
            Self::Button(_) => ElementKind::Button,
            Self::CheckBox(_) => ElementKind::CheckBox,
            Self::ComboBox(_) => ElementKind::ComboBox,
            Self::ListBox(_) => ElementKind::ListBox,
            Self::ScrollView(_) => ElementKind::ScrollView,
            Self::Reusable(_) => ElementKind::Reusable,
            Self::ViewHost(_) => ElementKind::ViewHost,
        }
    }
}

#[derive(Clone, Debug)]
pub struct ElementBase
{
    pub(crate) identity: u64,
    pub name: String,
    pub frame: Rect,
    parent: Option<Weak<RefCell<Element>>>,
    pub style: Style,
    pub focus_style: Option<Style>,
    pub edit_style: Option<Style>,
    pub cursor_style: Option<Style>,
    pub selected_style: Option<Style>,
    pub active_style: Option<Style>,
    pub checked_style: Option<Style>,
    pub unchecked_style: Option<Style>,
    pub disabled_style: Option<Style>,
    pub enabled: bool,
    pub focusable: bool,
    pub commit_mode: String,
    pub children: Vec<ElementRef>,
}

#[derive(Clone, Debug)]
pub struct Element
{
    pub base: ElementBase,
    data: ElementData,
}

impl Deref for Element
{
    type Target = ElementBase;

    fn deref(&self) -> &Self::Target
    {
        &self.base
    }
}

impl std::ops::DerefMut for Element
{
    fn deref_mut(&mut self) -> &mut Self::Target
    {
        &mut self.base
    }
}

impl Element
{
    pub fn rendering_parent_background() -> Option<Color>
    {
        current_element_parent_background()
    }

    pub fn kind(&self) -> ElementKind
    {
        self.data.kind()
    }

    pub fn name(&self) -> &str
    {
        &self.name
    }

    pub fn identity(&self) -> u64
    {
        self.identity
    }

    pub fn commit_mode(&self) -> &str
    {
        &self.commit_mode
    }

    pub fn set_focusable(&mut self, focusable: bool)
    {
        self.focusable = focusable;
    }

    pub fn focusable(&self) -> bool
    {
        self.focusable
    }

    pub fn set_enabled(&mut self, enabled: bool)
    {
        self.enabled = enabled;
    }

    pub fn enabled(&self) -> bool
    {
        self.enabled
    }

    pub fn frame(&self) -> Rect
    {
        self.frame
    }

    pub fn set_frame(&mut self, frame: Rect)
    {
        self.frame = frame;
    }

    pub fn style(&self) -> &Style
    {
        &self.style
    }

    pub fn children(&self) -> &[ElementRef]
    {
        &self.children
    }

    fn base(name: impl Into<String>, kind: ElementKind) -> Self
    {
        let name = name.into();
        let data = ElementData::for_kind(&kind);
        Self
        {
            base: ElementBase
            {
                identity: NEXT_ELEMENT_IDENTITY.fetch_add(1, Ordering::Relaxed),
                name,
                frame: Rect::default(),
                parent: None,
                style: Style::default(),
                focus_style: None,
                edit_style: None,
                cursor_style: None,
                selected_style: None,
                active_style: None,
                checked_style: None,
                unchecked_style: None,
                disabled_style: None,
                enabled: true,
                focusable: true,
                commit_mode: COMMIT_MODE_STANDARD.to_string(),
                children: Vec::new(),
            },
            data,
        }
    }

    fn combo_box(&self) -> Option<&ComboBoxState>
    {
        match &self.data
        {
            ElementData::ComboBox(state) => Some(state),
            _ => None,
        }
    }

    fn combo_box_mut(&mut self) -> Option<&mut ComboBoxState>
    {
        match &mut self.data
        {
            ElementData::ComboBox(state) => Some(state),
            _ => None,
        }
    }

    fn list_box(&self) -> Option<&ListBoxState>
    {
        match &self.data
        {
            ElementData::ListBox(state) => Some(state),
            _ => None,
        }
    }

    fn list_box_mut(&mut self) -> Option<&mut ListBoxState>
    {
        match &mut self.data
        {
            ElementData::ListBox(state) => Some(state),
            _ => None,
        }
    }

    pub(crate) fn scroll_view(&self) -> Option<&ScrollViewState>
    {
        match &self.data
        {
            ElementData::ScrollView(state) => Some(state),
            _ => None,
        }
    }

    pub(crate) fn scroll_view_mut(&mut self) -> Option<&mut ScrollViewState>
    {
        match &mut self.data
        {
            ElementData::ScrollView(state) => Some(state),
            _ => None,
        }
    }

    fn scroll_view_native_child_heights(&self, width: i32) -> Vec<i32>
    {
        self.children
            .iter()
            .map(|child|
            {
                let child = child.borrow();
                child.child_window().map_or_else(
                    || max(1, child.frame.height),
                    |window| crate::generated_window_content_size_for_width(window, width).height,
                )
            })
            .collect()
    }

    fn scroll_view_padding(&self, viewport: crate::Size) -> [i32; 4]
    {
        let requested = self.style.padding_values();
        let width = max(1, viewport.width);
        let height = max(1, viewport.height);
        let left = min(max(0, requested[3]), width - 1);
        let right = min(max(0, requested[1]), width - left - 1);
        let top = min(max(0, requested[0]), height - 1);
        let bottom = min(max(0, requested[2]), height - top - 1);
        [top, right, bottom, left]
    }

    fn scroll_view_padded_viewport(&self, viewport: crate::Size) -> crate::Size
    {
        let padding = self.scroll_view_padding(viewport);
        crate::Size
        {
            width: max(1, viewport.width - padding[1] - padding[3]),
            height: max(1, viewport.height - padding[0] - padding[2]),
        }
    }

    fn frame_buffer_view(&self) -> &FrameBufferViewState
    {
        match &self.data
        {
            ElementData::FrameBufferView(state) => state,
            _ => panic!("FrameBufferView operation used on a different element type"),
        }
    }

    fn frame_buffer_view_mut(&mut self) -> &mut FrameBufferViewState
    {
        match &mut self.data
        {
            ElementData::FrameBufferView(state) => state,
            _ => panic!("FrameBufferView operation used on a different element type"),
        }
    }

    fn number_input(&self) -> &NumberInputState
    {
        match &self.data
        {
            ElementData::NumberInput(state) => state,
            _ => panic!("NumberInput operation used on a different element type"),
        }
    }

    fn number_input_mut(&mut self) -> &mut NumberInputState
    {
        match &mut self.data
        {
            ElementData::NumberInput(state) => state,
            _ => panic!("NumberInput operation used on a different element type"),
        }
    }

    pub fn number_value(&self) -> f64
    {
        self.number_input().value
    }

    pub fn number_display_value(&self) -> String
    {
        format_number(self.number_input().value)
    }

    fn image(&self) -> &ImageState
    {
        match &self.data
        {
            ElementData::Image(state) => state,
            _ => panic!("Image operation used on a different element type"),
        }
    }

    fn image_mut(&mut self) -> &mut ImageState
    {
        match &mut self.data
        {
            ElementData::Image(state) => state,
            _ => panic!("Image operation used on a different element type"),
        }
    }

    pub fn source(&self) -> &str { &self.image().source }
    pub fn alt(&self) -> &str { &self.image().alt }
    pub fn fit(&self) -> &str { &self.image().fit }
    pub fn render_mode(&self) -> &str { &self.image().render_mode }
    pub fn align(&self) -> &str { &self.image().align }
    pub fn vertical_align(&self) -> &str { &self.image().vertical_align }

    pub fn set_source(&mut self, value: impl Into<String>)
    {
        self.image_mut().source = value.into();
    }

    pub fn set_alt(&mut self, value: impl Into<String>)
    {
        self.image_mut().alt = value.into();
    }

    pub fn set_fit(&mut self, value: impl Into<String>)
    {
        self.image_mut().fit = crate::image::normalized_image_value(
            &value.into(),
            crate::image::DEFAULT_IMAGE_FIT,
        );
    }

    pub fn set_render_mode(&mut self, value: impl Into<String>)
    {
        self.image_mut().render_mode = crate::image::normalized_image_value(
            &value.into(),
            crate::image::DEFAULT_IMAGE_RENDER_MODE,
        );
    }

    pub fn set_align(&mut self, value: impl Into<String>)
    {
        self.image_mut().align = crate::image::normalized_image_value(
            &value.into(),
            crate::image::DEFAULT_IMAGE_ALIGN,
        );
    }

    pub fn set_vertical_align(&mut self, value: impl Into<String>)
    {
        self.image_mut().vertical_align = crate::image::normalized_image_value(
            &value.into(),
            crate::image::DEFAULT_IMAGE_VERTICAL_ALIGN,
        );
    }

    fn reusable(&self) -> Option<&ReusableElementState>
    {
        match &self.data
        {
            ElementData::Reusable(state) | ElementData::ViewHost(state) => Some(state),
            _ => None,
        }
    }

    fn reusable_mut(&mut self) -> Option<&mut ReusableElementState>
    {
        match &mut self.data
        {
            ElementData::Reusable(state) | ElementData::ViewHost(state) => Some(state),
            _ => None,
        }
    }

    fn button(&self) -> Option<&ButtonState>
    {
        match &self.data
        {
            ElementData::Button(state) => Some(state),
            _ => None,
        }
    }

    fn button_mut(&mut self) -> Option<&mut ButtonState>
    {
        match &mut self.data
        {
            ElementData::Button(state) => Some(state),
            _ => None,
        }
    }

    fn checkbox(&self) -> Option<&CheckBoxState>
    {
        match &self.data
        {
            ElementData::CheckBox(state) => Some(state),
            _ => None,
        }
    }

    fn checkbox_mut(&mut self) -> Option<&mut CheckBoxState>
    {
        match &mut self.data
        {
            ElementData::CheckBox(state) => Some(state),
            _ => None,
        }
    }

    pub fn title(&self) -> &str
    {
        if let Some(state) = self.button()
        {
            &state.title
        }
        else if let Some(state) = self.checkbox()
        {
            &state.title
        }
        else
        {
            ""
        }
    }

    pub fn checked(&self) -> bool
    {
        self.checkbox().is_some_and(|state| state.checked)
    }

    pub fn set_checked(&mut self, checked: bool)
    {
        self.checkbox_mut()
            .expect("checked state assigned to a non-CheckBox element")
            .checked = checked;
    }

    fn label(&self) -> Option<&LabelState>
    {
        match &self.data
        {
            ElementData::Label(state)
            | ElementData::SpanLabel(state)
            | ElementData::InfoLabel(state) => Some(state),
            _ => None,
        }
    }

    fn label_mut(&mut self) -> Option<&mut LabelState>
    {
        match &mut self.data
        {
            ElementData::Label(state)
            | ElementData::SpanLabel(state)
            | ElementData::InfoLabel(state) => Some(state),
            _ => None,
        }
    }

    fn text_input(&self) -> Option<&TextInputState>
    {
        match &self.data
        {
            ElementData::TextInput(state) | ElementData::TextArea(state) => Some(state),
            _ => None,
        }
    }

    fn text_input_mut(&mut self) -> Option<&mut TextInputState>
    {
        match &mut self.data
        {
            ElementData::TextInput(state) | ElementData::TextArea(state) => Some(state),
            _ => None,
        }
    }

    fn message_table(&self) -> Option<&MessageTableState>
    {
        match &self.data
        {
            ElementData::MessageTable(state) => Some(state),
            _ => None,
        }
    }

    fn message_table_mut(&mut self) -> Option<&mut MessageTableState>
    {
        match &mut self.data
        {
            ElementData::MessageTable(state) => Some(state),
            _ => None,
        }
    }

    pub fn value(&self) -> &str
    {
        &self
            .text_input()
            .expect("text value requested from a non-TextInput element")
            .value
    }

    pub fn cursor(&self) -> usize
    {
        self.text_input()
            .expect("text cursor requested from a non-TextInput element")
            .cursor
    }

    pub fn set_cursor(&mut self, cursor: usize)
    {
        self.text_input_mut()
            .expect("text cursor assigned to a non-TextInput element")
            .set_cursor(cursor);
    }

    pub fn selection_anchor(&self) -> Option<usize>
    {
        self.text_input()
            .expect("text selection requested from a non-TextInput element")
            .selection_anchor
    }

    pub fn has_selection(&self) -> bool
    {
        self.selection_range().is_some()
    }

    pub fn selection_start(&self) -> usize
    {
        self.selection_range().map_or(self.cursor(), |selection| selection.0)
    }

    pub fn selection_end(&self) -> usize
    {
        self.selection_range().map_or(self.cursor(), |selection| selection.1)
    }

    pub fn insert_text(&mut self, text: &str)
    {
        let mut characters = text.chars().peekable();
        while let Some(character) = characters.next()
        {
            if character == '\r' && characters.peek() == Some(&'\n')
            {
                let _ = characters.next();
            }
            let key = if matches!(character, '\r' | '\n')
            {
                if self
                    .text_input()
                    .expect("text inserted into a non-TextInput element")
                    .multiline
                {
                    "Enter".to_string()
                }
                else
                {
                    " ".to_string()
                }
            }
            else
            {
                character.to_string()
            };
            let _ = self.handle_key(&key);
        }
    }

    pub fn set_text(&mut self, value: impl Into<String>)
    {
        let value = value.into();
        if matches!(self.kind(), ElementKind::Button | ElementKind::CheckBox)
        {
            self.set_title(value);
        }
        else if let Some(state) = self.label_mut()
        {
            state.text = value;
            state.spans.clear();
        }
        else if let Some(state) = self.message_table_mut()
        {
            state.text = value;
        }
        else if let Some(state) = self.text_input_mut()
        {
            state.set_value(value);
        }
        else
        {
            panic!("text assigned to an element without text content");
        }
    }

    pub fn set_title(&mut self, value: impl Into<String>)
    {
        let value = value.into();
        if let Some(state) = self.button_mut()
        {
            state.title = value;
        }
        else if let Some(state) = self.checkbox_mut()
        {
            state.title = value;
        }
    }

    pub fn set_spans(&mut self, spans: Vec<LabelSpan>)
    {
        let state = self
            .label_mut()
            .expect("label spans assigned to a non-Label element");
        state.text = spans.iter().map(|span| span.text.as_str()).collect();
        state.spans = spans;
    }

    pub fn text(&self) -> &str
    {
        match self.kind()
        {
            ElementKind::Button | ElementKind::CheckBox => self.title(),
            ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel =>
                &self.label().expect("label state").text,
            ElementKind::MessageTable =>
                &self.message_table().expect("MessageTable state").text,
            ElementKind::TextInput | ElementKind::TextArea => self.value(),
            _ => "",
        }
    }

    pub fn set_value(&mut self, value: impl Into<String>)
    {
        let value = value.into();
        if let Some(state) = self.text_input_mut()
        {
            state.set_value(value);
            return;
        }
        panic!("value assigned to a non-TextInput element");
    }

    pub fn set_number_value(&mut self, value: f64)
    {
        let display = format_number(value);
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        let was_editing = edit.editing;
        state.value = value;
        edit.edit_original_value = value;
        edit.edit_text = if was_editing { display } else { String::new() };
        edit.edit_cursor = if was_editing { edit.edit_text.len() } else { 0 };
        edit.editing = was_editing;
        edit.replace_on_first_text_input = false;
    }

    pub fn set_number_edit_cursor(
        &mut self,
        cursor: usize,
        preserve_replace_on_first_text_input: bool,
    )
    {
        self.ensure_number_edit_text();
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        edit.edit_cursor = clamp_utf8_boundary(&edit.edit_text, cursor);
        if !preserve_replace_on_first_text_input
        {
            edit.replace_on_first_text_input = false;
        }
    }

    pub fn set_number_edit_text(&mut self, text: impl Into<String>)
    {
        let text = text.into();
        let cursor = text.len();
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        edit.edit_text.clone_from(&text);
        edit.edit_cursor = cursor;
        edit.editing = true;
        edit.replace_on_first_text_input = false;
    }

    pub fn begin_number_edit(&mut self)
    {
        if self.number_input().edit.borrow().editing
        {
            return;
        }
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        edit.edit_original_value = state.value;
        edit.edit_text = format_number(state.value);
        edit.edit_cursor = edit.edit_text.len();
        edit.editing = true;
        edit.replace_on_first_text_input = state.value == 0.0;
    }

    pub fn cancel_number_edit(&mut self)
    {
        if !self.number_input().edit.borrow().editing
        {
            return;
        }
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        state.value = edit.edit_original_value;
        edit.edit_text.clear();
        edit.edit_cursor = 0;
        edit.editing = false;
        edit.replace_on_first_text_input = false;
    }

    pub fn commit_number_edit(&mut self)
    {
        self.ensure_number_edit_text();
        let state = self.number_input_mut();
        let edit = state.edit.get_mut();
        state.value = if edit.edit_text.is_empty()
        {
            0.0
        }
        else
        {
            parse_number_like_stod(&edit.edit_text)
                .unwrap_or(edit.edit_original_value)
        };
        edit.edit_text.clear();
        edit.edit_cursor = 0;
        edit.editing = false;
        edit.replace_on_first_text_input = false;
    }

    fn ensure_number_edit_text(&mut self)
    {
        if !self.number_input().edit.borrow().editing
        {
            self.begin_number_edit();
        }
    }

    pub fn select_range(&mut self, anchor: usize, cursor: usize)
    {
        if let Some(state) = self.label_mut()
        {
            let anchor = clamp_utf8_boundary(
                &state.text,
                min(anchor, state.text.len()),
            );
            let cursor = clamp_utf8_boundary(
                &state.text,
                min(cursor, state.text.len()),
            );
            if anchor == cursor
            {
                state.selection_start = None;
                state.selection_end = None;
            }
            else
            {
                state.selection_start = Some(anchor);
                state.selection_end = Some(cursor);
            }
            return;
        }
        if let Some(state) = self.text_input_mut()
        {
            state.select_range(anchor, cursor);
        }
    }

    pub fn clear_selection(&mut self)
    {
        if let Some(state) = self.label_mut()
        {
            state.selection_start = None;
            state.selection_end = None;
        }
        else if let Some(state) = self.text_input_mut()
        {
            state.selection_anchor = None;
        }
    }

    pub fn selected_text(&self) -> String
    {
        if let Some(state) = self.label()
        {
            let (Some(anchor), Some(cursor)) =
                (state.selection_start, state.selection_end)
            else
            {
                return String::new();
            };
            let start = min(anchor, cursor).min(state.text.len());
            let end = max(anchor, cursor).min(state.text.len());
            return state.text.get(start..end).unwrap_or_default().to_string();
        }
        if let Some(state) = self.text_input()
        {
            return state.selected_text();
        }
        String::new()
    }

    fn selection_range(&self) -> Option<(usize, usize)>
    {
        if let Some(state) = self.label()
        {
            let (Some(anchor), Some(cursor)) =
                (state.selection_start, state.selection_end)
            else
            {
                return None;
            };
            return (anchor != cursor).then_some((
                min(anchor, cursor),
                max(anchor, cursor),
            ));
        }
        if let Some(state) = self.text_input()
        {
            return state.selection();
        }
        None
    }

    pub fn cursor_for_point(&self, local_row: i32, local_col: i32) -> usize
    {
        let width = max(1, self.frame.width) as usize;
        if matches!(
            self.kind(),
            ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel
        )
        {
            let text = self.text();
            let length = text.len();
            if self.frame.height <= 1
            {
                let segment = text.split_once('\n').map_or(text, |(first, _)| first);
                let row = single_visual_text_row(segment);
                let offset = aligned_col(
                    width as i32,
                    row.cells.len() as i32,
                    self.style.text_align.as_deref().unwrap_or_default(),
                );
                let visual_col = max(0, local_col - offset) as usize;
                return raw_index_for_visual_column(&row, visual_col).min(length);
            }
            let rows = build_visual_text_rows(text, width);
            if local_row < 0
            {
                return 0;
            }
            let Some(row) = rows.get(local_row as usize) else
            {
                return length;
            };
            let offset = aligned_col(
                width as i32,
                row.cells.len() as i32,
                self.style.text_align.as_deref().unwrap_or_default(),
            );
            let visual_col = max(0, local_col - offset) as usize;
            return raw_index_for_visual_column(row, visual_col).min(length);
        }
        if let Some(state) = self.text_input()
        {
            return state.cursor_for_point(
                local_row,
                local_col,
                crate::Size
                {
                    width: self.frame.width,
                    height: self.frame.height,
                },
            );
        }
        0
    }

    pub fn scroll_text_rows(&mut self, delta: i32, viewport_height: i32, manual: bool) -> bool
    {
        let size = crate::Size
        {
            width: self.frame.width,
            height: viewport_height,
        };
        self.text_input_mut()
            .is_some_and(|state| state.scroll_by_rows(delta, size, manual))
    }

    pub fn selected_values(&self) -> Vec<String>
    {
        if let Some(state) = self.list_box()
        {
            return state
                .selected_indices
                .iter()
                .filter_map(|index| state.options.get(*index).cloned())
                .collect();
        }
        let state = self
            .combo_box()
            .expect("selected values requested from a non-choice element");
        state.options
            .get(state.selected_index.max(0) as usize)
            .cloned()
            .into_iter()
            .collect()
    }

    pub fn selected_value(&self) -> String
    {
        self.selected_values().into_iter().next().unwrap_or_default()
    }

    pub fn options(&self) -> &[String]
    {
        if let Some(state) = self.combo_box()
        {
            &state.options
        }
        else if let Some(state) = self.list_box()
        {
            &state.options
        }
        else
        {
            &[]
        }
    }

    pub fn disabled_values(&self) -> &[String]
    {
        &self
            .list_box()
            .expect("disabled values requested from a non-ListBox element")
            .disabled_values
    }

    pub fn selected_index(&self) -> i32
    {
        if let Some(state) = self.combo_box()
        {
            state.selected_index
        }
        else
        {
            self.list_box()
                .expect("selected index requested from a non-choice element")
                .selected_index
        }
    }

    pub fn active_index(&self) -> i32
    {
        self.list_box()
            .expect("active index requested from a non-ListBox element")
            .active_index
    }

    pub fn selected_indices(&self) -> &[usize]
    {
        &self
            .list_box()
            .expect("selected indices requested from a non-ListBox element")
            .selected_indices
    }

    pub fn multiple(&self) -> bool
    {
        self.list_box()
            .expect("multiple state requested from a non-ListBox element")
            .multiple
    }

    pub fn list_scroll_offset(&self) -> i32
    {
        self.list_box()
            .expect("list scroll offset requested from a non-ListBox element")
            .scroll_offset
            .get()
    }

    pub fn active_item_visible(&self) -> bool
    {
        self.list_box()
            .expect("active item visibility requested from a non-ListBox element")
            .active_item_visible
    }

    pub fn set_selected_index(&mut self, index: i32)
    {
        if let Some(state) = self.combo_box_mut()
        {
            if state.options.is_empty()
            {
                state.selected_index = 0;
            }
            else
            {
                state.selected_index = index.clamp(0, state.options.len() as i32 - 1);
            }
            return;
        }
        let state = self
            .list_box_mut()
            .expect("selected index assigned to a non-choice element");
        if state.options.is_empty()
        {
            state.selected_index = 0;
            state.active_index = 0;
            state.selected_indices.clear();
            state.scroll_offset.set(0);
            return;
        }
        state.selected_index = index.clamp(0, state.options.len() as i32 - 1);
        state.active_index = state.selected_index;
        if !state.multiple
        {
            state.selected_indices = vec![state.selected_index as usize];
        }
        let viewport_height = state.last_viewport_height.get();
        if viewport_height > 0
        {
            ensure_list_index_visible(state, state.selected_index, viewport_height);
        }
    }

    pub fn set_selected_items(&mut self, values: &[String])
    {
        let indices =
        {
            let state = self
                .list_box()
                .expect("selected items assigned to a non-ListBox element");
            values
                .iter()
                .filter_map(|value| state.options.iter().position(|option| option == value))
                .collect()
        };
        self.set_selected_indices(indices);
    }

    pub fn set_selected_indices(&mut self, indices: Vec<usize>)
    {
        let state = self
            .list_box_mut()
            .expect("selected indices assigned to a non-ListBox element");
        state.selected_indices.clear();
        if state.options.is_empty()
        {
            state.selected_index = 0;
            state.active_index = 0;
            state.scroll_offset.set(0);
            return;
        }
        for index in indices
        {
            let clamped = min(index, state.options.len() - 1);
            if !state.selected_indices.contains(&clamped)
            {
                state.selected_indices.push(clamped);
            }
            if !state.multiple
            {
                break;
            }
        }
        if let Some(index) = state.selected_indices.last()
        {
            state.selected_index = *index as i32;
            state.active_index = state.selected_index;
            let viewport_height = state.last_viewport_height.get();
            if viewport_height > 0
            {
                let selected_index = state.selected_index;
                ensure_list_index_visible(state, selected_index, viewport_height);
            }
        }
        else
        {
            state.selected_index =
                state.selected_index.clamp(0, state.options.len() as i32 - 1);
            state.active_index = state.selected_index;
        }
    }

    pub fn set_options(&mut self, options: Vec<String>)
    {
        if let Some(state) = self.combo_box_mut()
        {
            state.options = options;
            if state.options.is_empty()
            {
                state.selected_index = 0;
            }
            else
            {
                state.selected_index =
                    state.selected_index.clamp(0, state.options.len() as i32 - 1);
            }
            return;
        }
        let (selected_index, selected_indices) =
        {
            let state = self
                .list_box_mut()
                .expect("options assigned to a non-choice element");
            state.options = options;
            (state.selected_index, state.selected_indices.clone())
        };
        if selected_indices.is_empty()
        {
            self.set_selected_index(selected_index);
        }
        else
        {
            self.set_selected_indices(selected_indices);
        }
    }

    pub fn set_disabled_values(&mut self, values: Vec<String>)
    {
        self.list_box_mut()
            .expect("disabled values assigned to a non-ListBox element")
            .disabled_values = values;
    }

    pub fn set_multiple(&mut self, multiple: bool)
    {
        let selected_index = self.selected_index();
        let reset_selection =
        {
            let state = self
                .list_box_mut()
                .expect("multiple state assigned to a non-ListBox element");
            state.multiple = multiple;
            !multiple && state.selected_indices.len() > 1
        };
        if reset_selection
        {
            self.set_selected_index(selected_index);
        }
    }

    pub fn set_active_index(&mut self, active_index: i32)
    {
        let state = self
            .list_box_mut()
            .expect("active index assigned to a non-ListBox element");
        if state.options.is_empty()
        {
            state.selected_index = 0;
            state.active_index = 0;
            state.scroll_offset.set(0);
            return;
        }
        state.active_index = active_index.clamp(0, state.options.len() as i32 - 1);
        let viewport_height = state.last_viewport_height.get();
        if viewport_height > 0
        {
            ensure_list_index_visible(state, state.active_index, viewport_height);
        }
    }

    pub fn show_active_item(&mut self)
    {
        self.list_box_mut()
            .expect("active item shown on a non-ListBox element")
            .active_item_visible = true;
    }

    pub fn hide_active_item(&mut self)
    {
        self.list_box_mut()
            .expect("active item hidden on a non-ListBox element")
            .active_item_visible = false;
    }

    pub fn set_style(&mut self, style: Style) { self.style = style; }
    pub fn set_focus_style(&mut self, style: Style)
    {
        self.focus_style = Some(style);
    }
    pub fn set_edit_style(&mut self, style: Style)
    {
        self.edit_style = Some(style);
    }
    pub fn set_cursor_style(&mut self, style: Style) { self.cursor_style = Some(style); }
    pub fn set_selected_style(&mut self, style: Style) { self.selected_style = Some(style); }
    pub fn set_active_style(&mut self, style: Style) { self.active_style = Some(style); }
    pub fn set_checked_style(&mut self, style: Style) { self.checked_style = Some(style); }
    pub fn set_unchecked_style(&mut self, style: Style) { self.unchecked_style = Some(style); }
    pub fn set_disabled_style(&mut self, style: Style) { self.disabled_style = Some(style); }
    pub fn set_commit_mode(&mut self, mode: impl Into<String>) { self.commit_mode = mode.into(); }

    pub fn set_child_window(&mut self, child: crate::GeneratedWindow)
    {
        self.reusable_mut()
            .expect("reusable child assigned to a non-reusable element")
            .child = Some(Box::new(child));
    }

    pub fn set_view(&mut self, view: crate::GeneratedWindow)
    {
        self.set_child_window(view);
    }

    pub fn clear_view(&mut self)
    {
        self.reusable_mut()
            .expect("view cleared on a non-reusable element")
            .child = None;
    }

    pub fn current_view(&self) -> Option<&crate::GeneratedWindow>
    {
        self.child_window()
    }

    pub fn clear_frames(&mut self)
    {
        let state = self.frame_buffer_view_mut();
        state.frames.clear();
        state.target_order.clear();
        state.regions.clear();
    }

    pub fn set_frames(
        &mut self,
        frames: BTreeMap<String, Vec<Vec<FrameBufferCell>>>,
        target_order: Vec<String>,
        regions: BTreeMap<String, FrameBufferRegion>,
    )
    {
        let state = self.frame_buffer_view_mut();
        state.frames = frames;
        state.target_order = target_order;
        state.regions = regions;
    }

    pub fn set_pan(&mut self, row: i32, col: i32)
    {
        let state = self.frame_buffer_view_mut();
        state.pan_row = max(0, row);
        state.pan_col = max(0, col);
    }

    pub fn set_dimmed(&mut self, dimmed: bool)
    {
        self.frame_buffer_view_mut().dimmed = dimmed;
    }

    pub fn set_interaction_state(&mut self, focused: bool, edit: bool)
    {
        let state = self.frame_buffer_view_mut();
        state.panel_focused = focused;
        state.panel_edit = edit;
    }

    pub fn child_window(&self) -> Option<&crate::GeneratedWindow>
    {
        self.reusable()?.child.as_deref()
    }

    pub fn child_window_mut(&mut self) -> Option<&mut crate::GeneratedWindow>
    {
        self.reusable_mut()?.child.as_deref_mut()
    }

    pub fn add_child<T: Into<ElementRef>>(&mut self, child: T)
    {
        let child = child.into();
        assert!(
            is_live_element(&child),
            "a stale child element cannot be added to a live element tree",
        );
        assert!(
            child.borrow().parent().is_none(),
            "a child element cannot be owned by more than one parent",
        );
        child.borrow_mut().parent = LIVE_ELEMENTS.with(|elements|
        {
            elements.borrow().get(&self.identity).cloned()
        });
        register_live_element_tree(&child);
        self.children.push(child);
        if let Some(scroll_view) = self.scroll_view()
        {
            scroll_view.invalidate_height_cache();
            if scroll_view.auto_scroll()
            {
                scroll_view.set_view_offset(0);
            }
        }
    }

    pub fn clear_children(&mut self)
    {
        if let Some(scroll_view) = self.scroll_view()
        {
            scroll_view.clear_children_state();
        }
        self.clear_child_elements();
    }

    pub fn clear_child_elements(&mut self)
    {
        for child in &self.children
        {
            unregister_live_element_tree(child);
            child.borrow_mut().parent = None;
        }
        self.children.clear();
    }

    pub fn parent(&self) -> Option<ElementRef>
    {
        self.parent.as_ref().and_then(Weak::upgrade)
    }

    pub fn set_gap(&mut self, gap: i32)
    {
        self.scroll_view_mut()
            .expect("gap changed on a non-ScrollView element")
            .set_gap(gap);
    }

    pub fn gap(&self) -> i32
    {
        self.scroll_view()
            .expect("gap requested from a non-ScrollView element")
            .gap()
    }

    pub fn add_rendered_child(&self, child: TerminalBuffer)
    {
        self.scroll_view()
            .expect("rendered child added to a non-ScrollView element")
            .add_rendered_child(child);
    }

    pub fn set_dynamic_children_renderer<F>(&mut self, renderer: F)
    where
        F: Fn(i32) -> Vec<TerminalBuffer> + 'static,
    {
        self.scroll_view_mut()
            .expect("dynamic children set on a non-ScrollView element")
            .set_dynamic_children_renderer(renderer);
    }

    pub fn invalidate_dynamic_children(&self)
    {
        self.scroll_view()
            .expect("dynamic children invalidated on a non-ScrollView element")
            .invalidate_dynamic_children();
    }

    pub fn set_auto_scroll(&self, auto_scroll: bool)
    {
        self.scroll_view()
            .expect("auto-scroll changed on a non-ScrollView element")
            .set_auto_scroll(auto_scroll);
    }

    pub fn auto_scroll(&self) -> bool
    {
        self.scroll_view()
            .expect("auto-scroll requested from a non-ScrollView element")
            .auto_scroll()
    }

    pub fn view_offset(&self) -> i32
    {
        self.scroll_view()
            .expect("view offset requested from a non-ScrollView element")
            .view_offset()
    }

    pub fn scroll_offset(&self) -> i32
    {
        self.scroll_view()
            .expect("scroll offset requested from a non-ScrollView element")
            .scroll_offset()
    }

    pub fn h_view_offset(&self) -> i32
    {
        self.scroll_view()
            .expect("horizontal view offset requested from a non-ScrollView element")
            .h_view_offset()
    }

    pub fn set_descendant_focus_style(&mut self, style: Style)
    {
        self.scroll_view_mut()
            .expect("descendant focus style set on a non-ScrollView element")
            .set_descendant_focus_style(style);
    }

    pub fn descendant_focus_style(&self) -> Option<&Style>
    {
        self.scroll_view()
            .expect("descendant focus style requested from a non-ScrollView element")
            .descendant_focus_style()
    }

    pub fn scroll_position(&self) -> ScrollViewPosition
    {
        self.scroll_view()
            .expect("scroll position requested from a non-ScrollView element")
            .scroll_position()
    }

    pub fn restore_scroll_position(&self, position: ScrollViewPosition)
    {
        self.scroll_view()
            .expect("scroll position restored on a non-ScrollView element")
            .refresh_dynamic_children(max(1, self.frame.width));
        let viewport = self.scroll_view_padded_viewport(crate::Size
        {
            width: self.frame.width,
            height: self.frame.height,
        });
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("scroll position restored on a non-ScrollView element")
            .restore_scroll_position(position, viewport, &heights);
    }

    pub fn content_height(&self, width: i32) -> i32
    {
        let viewport = self.scroll_view_padded_viewport(crate::Size
        {
            width,
            height: self.frame.height,
        });
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("content height requested from a non-ScrollView element")
            .content_height(viewport.width, &heights)
    }

    pub fn max_view_offset(&self, viewport: crate::Size) -> i32
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("maximum view offset requested from a non-ScrollView element")
            .max_view_offset(viewport, &heights)
    }

    pub fn cached_child_heights(&self) -> Vec<i32>
    {
        let viewport = self.scroll_view_padded_viewport(crate::Size
        {
            width: self.frame.width,
            height: self.frame.height,
        });
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("cached child heights requested from a non-ScrollView element")
            .cached_child_heights(viewport.width, &heights)
    }

    pub fn content_height_current(&self) -> i32
    {
        self.content_height(self.frame.width)
    }

    pub fn content_window(
        &self,
        viewport: crate::Size,
    ) -> Option<ScrollViewContentWindow>
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("content window requested from a non-ScrollView element")
            .content_window(viewport, &heights)
    }

    pub fn child_views(&self, viewport: crate::Size) -> Vec<ScrollViewChildView>
    {
        let padding = self.scroll_view_padding(viewport);
        let viewport = self.scroll_view_padded_viewport(viewport);
        let native_heights = self.scroll_view_native_child_heights(viewport.width);
        let scroll_view = self
            .scroll_view()
            .expect("child views requested from a non-ScrollView element");
        let Some(window) = scroll_view.content_window(viewport, &native_heights) else
        {
            return Vec::new();
        };
        let heights =
            scroll_view.cached_child_heights(viewport.width, &native_heights);
        let rendered_count = scroll_view.rendered_child_count();
        let scroll_offset = scroll_view
            .scroll_offset()
            .clamp(0, heights.len() as i32) as usize;
        let viewport_top = padding[0];
        let viewport_bottom = padding[0] + viewport.height;
        let mut row = 0;
        let mut views = Vec::new();
        for index in scroll_offset..heights.len()
        {
            let height = heights[index];
            let local_row = padding[0] + row - window.actual_skip;
            let visible_top = max(local_row, viewport_top);
            let visible_bottom = min(local_row + height, viewport_bottom);
            let visible = visible_bottom > visible_top;
            let clipped =
                visible && (visible_top > local_row || visible_bottom < local_row + height);
            let element = index
                .checked_sub(rendered_count)
                .and_then(|native_index| self.children.get(native_index))
                .cloned();
            views.push(ScrollViewChildView
            {
                element,
                frame: if index < rendered_count
                {
                    Rect
                    {
                        row: visible_top,
                        col: padding[3],
                        width: viewport.width,
                        height: if visible { visible_bottom - visible_top } else { 0 },
                    }
                }
                else
                {
                    Rect
                    {
                        row: local_row,
                        col: padding[3],
                        width: viewport.width,
                        height,
                    }
                },
                visible,
                index: index as i32,
                clipped,
            });
            row += height;
            if index + 1 < heights.len()
            {
                row += window.gap;
            }
        }
        views
    }

    pub fn ensure_child_visible(
        &self,
        child: &ElementRef,
        viewport: crate::Size,
    ) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let native_heights = self.scroll_view_native_child_heights(viewport.width);
        let rendered_count = self
            .scroll_view()
            .expect("child visibility requested from a non-ScrollView element")
            .rendered_child_count();
        let Some(index) = self
            .children
            .iter()
            .position(|candidate| Rc::ptr_eq(candidate, child))
        else
        {
            return false;
        };
        self.scroll_view()
            .expect("child visibility requested from a non-ScrollView element")
            .ensure_item_visible(
                rendered_count + index,
                viewport,
                &native_heights,
            )
    }

    pub fn scroll_to(&mut self, offset: i32) -> bool
    {
        if self.kind() == ElementKind::TextArea
        {
            let max_offset = self.max_scroll_offset();
            if let Some(state) = self.text_input_mut()
            {
                let previous = state.row_scroll_offset.get();
                state.row_scroll_offset.set(offset.clamp(0, max_offset));
                state.manual_row_scroll.set(true);
                return state.row_scroll_offset.get() != previous;
            }
            return false;
        }
        if self.kind() == ElementKind::ListBox
        {
            let viewport_height = max(1, self.frame.height);
            let state = self.list_box_mut().expect("ListBox state");
            let max_offset = max(0, state.options.len() as i32 - viewport_height);
            let previous = state.scroll_offset.get();
            state.scroll_offset.set(offset.clamp(0, max_offset));
            return state.scroll_offset.get() != previous;
        }
        let viewport = self.scroll_view_padded_viewport(crate::Size
        {
            width: self.frame.width,
            height: self.frame.height,
        });
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("scroll requested from a non-scrollable element")
            .scroll_to(offset, viewport, &heights)
    }

    pub fn scroll_by(&mut self, delta: i32) -> bool
    {
        if self.kind() == ElementKind::TextArea
        {
            return self.scroll_text_rows(delta, self.frame.height, true);
        }
        if self.kind() == ElementKind::ListBox
        {
            let offset = self.list_scroll_offset();
            return self.scroll_to(offset + delta);
        }
        let offset = self.scroll_offset();
        self.scroll_to(offset + delta)
    }

    pub fn scroll_lines(&self, delta: i32, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("line scroll requested from a non-ScrollView element")
            .scroll_lines(delta, viewport, &heights)
    }

    pub fn scroll_to_top(&self, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("top scroll requested from a non-ScrollView element")
            .scroll_to_top(viewport, &heights)
    }

    pub fn scroll_to_bottom(&mut self) -> bool
    {
        if self.kind() != ElementKind::ScrollView
        {
            let max_offset = self.max_scroll_offset();
            return self.scroll_to(max_offset);
        }
        let viewport = self.scroll_view_padded_viewport(crate::Size
        {
            width: self.frame.width,
            height: self.frame.height,
        });
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("bottom scroll requested from a non-ScrollView element")
            .scroll_to_bottom(viewport, &heights)
    }

    pub fn scroll_page_up(&self, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("page scroll requested from a non-ScrollView element")
            .scroll_page_up(viewport, &heights)
    }

    pub fn scroll_page_down(&self, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("page scroll requested from a non-ScrollView element")
            .scroll_page_down(viewport, &heights)
    }

    pub fn scroll_horizontal(&self, delta: i32) -> bool
    {
        self.scroll_view()
            .expect("horizontal scroll requested from a non-ScrollView element")
            .scroll_horizontal(delta)
    }

    pub fn wheel_scroll_lines(&self, viewport: crate::Size) -> i32
    {
        self.scroll_view()
            .expect("wheel size requested from a non-ScrollView element")
            .wheel_scroll_lines(self.scroll_view_padded_viewport(viewport))
    }

    pub fn can_scroll_up(&self, _viewport: crate::Size) -> bool
    {
        self.scroll_view()
            .expect("scroll query requested from a non-ScrollView element")
            .can_scroll_up()
    }

    pub fn can_scroll_down(&self, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("scroll query requested from a non-ScrollView element")
            .can_scroll_down(viewport, &heights)
    }

    pub fn handle_wheel(&self, wheel_delta: i32, viewport: crate::Size) -> bool
    {
        let viewport = self.scroll_view_padded_viewport(viewport);
        let heights = self.scroll_view_native_child_heights(viewport.width);
        self.scroll_view()
            .expect("wheel delivered to a non-ScrollView element")
            .handle_wheel(wheel_delta, viewport, &heights)
    }

    pub fn consume_terminal_scroll_delta(&self) -> i32
    {
        self.scroll_view()
            .expect("terminal scroll delta requested from a non-ScrollView element")
            .consume_terminal_scroll_delta()
    }

    pub fn max_scroll_offset(&self) -> i32
    {
        if self.kind() == ElementKind::ListBox
        {
            return max(0, self.options().len() as i32 - max(1, self.frame.height));
        }
        if self.kind() == ElementKind::TextArea
        {
            return self.text_input().map_or(0, |state|
            {
                state.max_scroll_offset(crate::Size
                {
                    width: self.frame.width,
                    height: self.frame.height,
                })
            });
        }
        if self.kind() != ElementKind::ScrollView
        {
            return 0;
        }
        self.max_view_offset(crate::Size
        {
            width: self.frame.width,
            height: self.frame.height,
        })
    }

    pub fn ensure_selected_visible(&mut self)
    {
        let frame_height = max(1, self.frame.height);
        let state = self
            .list_box_mut()
            .expect("selected visibility requested from a non-ListBox element");
        if state.selected_index < 0
        {
            return;
        }
        ensure_list_index_visible(state, state.selected_index, frame_height);
    }

    pub fn point_inside(&self, point: Point) -> bool
    {
        self.frame.contains(point)
    }

    pub fn handle_key(&mut self, key: &str) -> bool
    {
        match self.kind()
        {
            ElementKind::TextInput | ElementKind::TextArea => self.handle_text_key(key),
            ElementKind::NumberInput => self.handle_number_key(key),
            ElementKind::CheckBox =>
            {
                if matches!(key, "Enter" | " ")
                {
                    let checked = !self.checked();
                    self.set_checked(checked);
                    true
                }
                else
                {
                    false
                }
            }
            ElementKind::ComboBox => self.handle_combo_key(key),
            ElementKind::ListBox => self.handle_list_key(key),
            ElementKind::ScrollView =>
            {
                let viewport = self.scroll_view_padded_viewport(crate::Size
                {
                    width: self.frame.width,
                    height: self.frame.height,
                });
                let heights = self.scroll_view_native_child_heights(viewport.width);
                self.scroll_view()
                    .expect("ScrollView key delivered to missing state")
                    .handle_key(
                        key,
                        viewport,
                        &heights,
                        self.style.scroll_y.unwrap_or(true),
                        self.style.scroll_x.unwrap_or(false),
                    )
            }
            _ => false,
        }
    }

    fn handle_text_key(&mut self, key: &str) -> bool
    {
        let width = self.frame.width;
        self.text_input_mut()
            .expect("text key delivered to a non-TextInput element")
            .handle_key(key, width)
    }

    fn handle_number_key(&mut self, key: &str) -> bool
    {
        self.ensure_number_edit_text();
        {
            let state = self.number_input_mut();
            let edit = state.edit.get_mut();
            match key
            {
                "Up" | "Down" =>
                {
                    state.value += if key == "Up" { state.step } else { -state.step };
                    edit.edit_text = format_number(state.value);
                    edit.edit_cursor = edit.edit_text.len();
                    edit.replace_on_first_text_input = false;
                }
                "Left" =>
                {
                    edit.edit_cursor =
                        previous_utf8_boundary(&edit.edit_text, edit.edit_cursor);
                    edit.replace_on_first_text_input = false;
                }
                "Right" =>
                {
                    edit.edit_cursor =
                        next_utf8_boundary(&edit.edit_text, edit.edit_cursor);
                    edit.replace_on_first_text_input = false;
                }
                "Home" =>
                {
                    edit.edit_cursor = 0;
                    edit.replace_on_first_text_input = false;
                }
                "End" =>
                {
                    edit.edit_cursor = edit.edit_text.len();
                    edit.replace_on_first_text_input = false;
                }
                "Backspace" =>
                {
                    edit.replace_on_first_text_input = false;
                    if edit.edit_cursor > 0
                    {
                        let start =
                            previous_utf8_boundary(&edit.edit_text, edit.edit_cursor);
                        edit.edit_text.replace_range(start..edit.edit_cursor, "");
                        edit.edit_cursor = start;
                    }
                }
                "Enter" =>
                {
                    self.commit_number_edit();
                    return true;
                }
                _ if key.chars().count() == 1
                    && key
                        .chars()
                        .next()
                        .is_some_and(|ch| ch.is_ascii_digit() || matches!(ch, '.' | '-')) =>
                {
                    if edit.replace_on_first_text_input
                    {
                        edit.edit_text.clear();
                        edit.edit_cursor = 0;
                        edit.replace_on_first_text_input = false;
                    }
                    edit.edit_text.insert_str(edit.edit_cursor, key);
                    edit.edit_cursor += key.len();
                }
                _ => return false,
            }
        }
        true
    }

    fn handle_combo_key(&mut self, key: &str) -> bool
    {
        if self.options().is_empty()
        {
            return false;
        }
        match key
        {
            "Up" => self.set_selected_index(self.selected_index() - 1),
            "Down" => self.set_selected_index(self.selected_index() + 1),
            _ => return false,
        }
        true
    }

    fn handle_list_key(&mut self, key: &str) -> bool
    {
        if self.options().is_empty()
        {
            return false;
        }
        match key
        {
            "Up" =>
            {
                self.set_active_index(self.active_index() - 1);
                self.show_active_item();
            }
            "Down" =>
            {
                self.set_active_index(self.active_index() + 1);
                self.show_active_item();
            }
            "Enter" if self.multiple() =>
            {
                let index = self.active_index().max(0) as usize;
                let state = self.list_box_mut().expect("ListBox state");
                if let Some(position) =
                    state.selected_indices.iter().position(|value| *value == index)
                {
                    state.selected_indices.remove(position);
                }
                else
                {
                    state.selected_indices.push(index);
                }
            }
            "Enter" =>
            {
                let active_index = self.active_index();
                self.set_selected_index(active_index);
                self.hide_active_item();
            }
            _ => return false,
        }
        true
    }

    pub fn render(&self, width: i32, height: i32, focused: bool, edit_mode: bool) -> TerminalBuffer
    {
        self.render_with_state(
            width,
            height,
            ElementRenderState
            {
                focused,
                edit_mode,
                ..ElementRenderState::default()
            },
        )
    }

    pub fn render_with_state(
        &self,
        width: i32,
        height: i32,
        state: ElementRenderState,
    ) -> TerminalBuffer
    {
        let width = if width > 0
        {
            if self.kind() == ElementKind::MessageTable
            {
                max(width, message_table_natural_width(self.text()))
            }
            else
            {
                width
            }
        }
        else
        {
            match self.kind()
            {
                ElementKind::Label | ElementKind::SpanLabel =>
                    max(1, crate::visible_width(self.text())),
                ElementKind::InfoLabel => max(1, self.text().len() as i32),
                ElementKind::Button =>
                    max(1, crate::visible_width(self.title())) + 4,
                ElementKind::CheckBox =>
                {
                    let indicator = if self.checked() { "[x]" } else { "[ ]" };
                    let text = if self.title().is_empty()
                    {
                        indicator.to_string()
                    }
                    else
                    {
                        format!("{indicator} {}", self.title())
                    };
                    max(1, crate::visible_width(&text))
                }
                ElementKind::TextInput | ElementKind::TextArea =>
                    max(1, crate::visible_width(self.value())) + 1,
                ElementKind::NumberInput =>
                {
                    let number = self.number_input();
                    let display;
                    let edit = number.edit.borrow();
                    let text = if state.edit_mode && edit.editing
                    {
                        edit.edit_text.as_str()
                    }
                    else
                    {
                        display = format_number(number.value);
                        display.as_str()
                    };
                    max(1, crate::visible_width(text))
                }
                ElementKind::ComboBox =>
                    max(1, crate::visible_width(&self.selected_value())) + 2,
                ElementKind::ListBox =>
                {
                    let list = self.list_box().expect("ListBox state");
                    let index = if state.edit_mode && list.active_item_visible
                    {
                        list.active_index
                    }
                    else
                    {
                        list.selected_index
                    };
                    let text = list
                        .options
                        .get(index.max(0) as usize)
                        .map(String::as_str)
                        .unwrap_or_default();
                    max(1, crate::visible_width(text)) + 2
                }
                ElementKind::MessageTable => message_table_natural_width(self.text()),
                _ => 1,
            }
        };
        let height = match self.kind()
        {
            ElementKind::Button
            | ElementKind::CheckBox
            | ElementKind::TextInput
            | ElementKind::NumberInput => 1,
            ElementKind::ComboBox if state.edit_mode => self.options().len() as i32 + 1,
            ElementKind::ComboBox => 1,
            ElementKind::MessageTable => message_table_fit_content_height(self.text()),
            _ => max(1, height),
        };
        let style = self.effective_style(state);
        let mut buffer = TerminalBuffer::new(width, height);
        buffer.fill(
            Rect { row: 0, col: 0, width, height },
            ' ',
            &style.color,
            &style.background,
        );
        match self.kind()
        {
            ElementKind::Button => render_button(&mut buffer, self, &style, state.focused),
            ElementKind::CheckBox =>
                render_checkbox(&mut buffer, self, &style, state.focused || state.edit_mode),
            ElementKind::TextInput | ElementKind::TextArea =>
                render_text_input(&mut buffer, self, &style, state.edit_mode),
            ElementKind::NumberInput =>
                render_number_input(&mut buffer, self, &style, state.edit_mode),
            ElementKind::ComboBox =>
                render_combo_box(&mut buffer, self, &style, state.focused, state.edit_mode),
            ElementKind::ListBox => render_list_box(&mut buffer, self, &style, state.edit_mode),
            ElementKind::ScrollView =>
            {
                render_scroll_view(&mut buffer, self, &style, state.edit_mode);
                if state.passive_focus
                    && self
                        .focus_style
                        .as_ref()
                        .is_some_and(|style| style.background.is_some())
                {
                    let focus_background = &self
                        .focus_style
                        .as_ref()
                        .expect("checked focus style")
                        .background;
                    apply_scroll_view_passive_focus_background(
                        &mut buffer,
                        focus_background,
                        &self.style.background,
                        &style.background,
                    );
                }
            }
            ElementKind::Image =>
                crate::render_image_element(&mut buffer, self, &style, state),
            ElementKind::InfoLabel =>
                render_info_label(&mut buffer, self, &style),
            ElementKind::FrameBufferView =>
                render_frame_buffer_view(&mut buffer, self, &style),
            ElementKind::SpanLabel => render_span_label(&mut buffer, self, &style),
            ElementKind::MessageTable => render_message_table(&mut buffer, self, &style),
            ElementKind::Reusable | ElementKind::ViewHost =>
            {
                if let Some(child) = self.child_window()
                {
                    buffer = crate::render_embedded_window(child, width, height);
                }
            }
            _ => render_label(&mut buffer, self, &style),
        }
        buffer
    }

    pub fn effective_style(&self, state: ElementRenderState) -> Style
    {
        let mut style = self.style.clone();
        if !self.enabled && self.disabled_style.is_some()
        {
            merge_optional_element_state_style(&mut style, &self.disabled_style);
            return style;
        }
        if state.edit_mode && self.edit_style.is_some()
        {
            merge_optional_element_state_style(&mut style, &self.edit_style);
            return style;
        }
        if state.focused && self.focus_style.is_some()
        {
            merge_optional_element_state_style(&mut style, &self.focus_style);
        }
        style
    }
}

impl Drop for Element
{
    fn drop(&mut self)
    {
        LIVE_ELEMENTS.with(|elements|
        {
            elements.borrow_mut().remove(&self.identity);
        });
    }
}

fn merge_element_state_style(base: &mut Style, state: &Style)
{
    let mut base_background = base.background.clone();
    if base_background
        .as_ref()
        .is_some_and(Color::has_partial_alpha)
    {
        ELEMENT_PARENT_BACKGROUND.with(|parent|
        {
            if let Some(parent) = parent
                .borrow()
                .as_ref()
                .filter(|parent| parent.rgba().is_some())
            {
                base.background = Some(
                    base_background
                        .as_ref()
                        .expect("partial-alpha base background")
                        .blend_over(parent),
                );
                base_background = base.background.clone();
            }
        });
    }
    let state_background = state.background.clone();
    base.merge(state);
    if state_background
        .as_ref()
        .is_some_and(Color::has_partial_alpha)
        && base_background.as_ref().and_then(Color::rgba).is_some()
    {
        base.background = if state_background
            .as_ref()
            .and_then(Color::rgba)
            .is_some_and(|rgba| rgba.alpha == 0)
        {
            base_background
        }
        else
        {
            Some(
                state_background
                    .as_ref()
                    .expect("partial-alpha state background")
                    .blend_over(base_background.as_ref().expect("base background")),
            )
        };
    }
}

fn merge_optional_element_state_style(base: &mut Style, state: &Option<Style>)
{
    if let Some(state) = state
    {
        merge_element_state_style(base, state);
    }
}

fn reference(element: Element) -> ElementRef
{
    let element = Rc::new(RefCell::new(element));
    register_live_element_tree(&element);
    element
}

fn register_live_element_tree(element: &ElementRef)
{
    let identity = element.borrow().identity;
    LIVE_ELEMENTS.with(|elements|
    {
        elements.borrow_mut().insert(identity, Rc::downgrade(element));
    });
    let children = element.borrow().children.clone();
    for child in children
    {
        register_live_element_tree(&child);
    }
}

fn unregister_live_element_tree(element: &ElementRef)
{
    let identity = element.borrow().identity;
    LIVE_ELEMENTS.with(|elements|
    {
        elements.borrow_mut().remove(&identity);
    });
    let children = element.borrow().children.clone();
    for child in children
    {
        unregister_live_element_tree(&child);
    }
}

pub fn is_live_element(element: &ElementRef) -> bool
{
    let identity = element.borrow().identity;
    LIVE_ELEMENTS.with(|elements|
    {
        elements
            .borrow()
            .get(&identity)
            .and_then(Weak::upgrade)
            .is_some_and(|live| Rc::ptr_eq(&live, element))
    })
}

pub fn new_label(name: &str, text: &str) -> LabelRef
{
    let mut element = Element::base(name, ElementKind::Label);
    element.set_text(text);
    reference(element).into()
}

pub fn new_span_label(name: &str, text: &str) -> SpanLabelRef
{
    let mut element = Element::base(name, ElementKind::SpanLabel);
    element.set_text(text);
    reference(element).into()
}

pub fn new_info_label(name: &str, text: &str) -> InfoLabelRef
{
    let mut element = Element::base(name, ElementKind::InfoLabel);
    element.set_text(text);
    reference(element).into()
}

pub fn new_frame_buffer_view(name: &str) -> FrameBufferViewRef
{
    reference(Element::base(name, ElementKind::FrameBufferView)).into()
}

pub fn new_message_table(name: &str) -> MessageTableRef
{
    reference(Element::base(name, ElementKind::MessageTable)).into()
}

pub fn new_image(name: &str, source: &str, alt: &str, fit: &str, render_mode: &str, align: &str, valign: &str) -> ImageRef
{
    let mut element = Element::base(name, ElementKind::Image);
    element.set_source(source);
    element.set_alt(alt);
    element.set_fit(fit);
    element.set_render_mode(render_mode);
    element.set_align(align);
    element.set_vertical_align(valign);
    reference(element).into()
}

pub fn new_button(name: &str, title: &str) -> ButtonRef
{
    let mut element = Element::base(name, ElementKind::Button);
    element.set_title(title);
    reference(element).into()
}

pub fn new_checkbox(name: &str, title: &str, checked: bool) -> CheckBoxRef
{
    let mut element = Element::base(name, ElementKind::CheckBox);
    element.set_title(title);
    element.set_checked(checked);
    reference(element).into()
}

pub fn new_text_input(name: &str, value: &str, max_length: i32) -> TextInputRef
{
    let mut element = Element::base(name, ElementKind::TextInput);
    element.data = ElementData::TextInput(TextInputState::new(
        value,
        max_length,
        false,
    ));
    reference(element).into()
}

pub fn new_text_area(name: &str, value: &str) -> TextAreaRef
{
    new_text_area_with_max_length(name, value, 0)
}

pub fn new_text_area_with_max_length(
    name: &str,
    value: &str,
    max_length: i32,
) -> TextAreaRef
{
    let mut element = Element::base(name, ElementKind::TextArea);
    element.data = ElementData::TextArea(TextInputState::new(
        value,
        max_length,
        true,
    ));
    element.commit_mode = COMMIT_MODE_LEAVE.to_string();
    reference(element).into()
}

pub fn new_number_input(name: &str, value: f64) -> NumberInputRef
{
    new_number_input_with_step(name, value, 1.0)
}

pub fn new_number_input_with_step(name: &str, value: f64, step: f64) -> NumberInputRef
{
    let mut element = Element::base(name, ElementKind::NumberInput);
    element.number_input_mut().step = step;
    element.set_number_value(value);
    reference(element).into()
}

pub fn new_combo_box(name: &str, options: Vec<String>) -> ComboBoxRef
{
    let mut element = Element::base(name, ElementKind::ComboBox);
    element.set_options(options);
    reference(element).into()
}

pub fn new_list_box(name: &str, options: Vec<String>, multi: bool) -> ListBoxRef
{
    let mut element = Element::base(name, ElementKind::ListBox);
    element.set_options(options);
    element.set_multiple(multi);
    reference(element).into()
}

pub fn new_scroll_view(name: &str, gap: i32) -> ScrollViewRef
{
    let mut element = Element::base(name, ElementKind::ScrollView);
    element.data = ElementData::ScrollView(Box::new(ScrollViewState::new(gap)));
    reference(element).into()
}

pub fn new_reusable_element(name: &str, reusable_type: &str) -> ReusableElementRef
{
    let mut element = Element::base(name, ElementKind::Reusable);
    element
        .reusable_mut()
        .expect("new reusable element state")
        .reusable_type = reusable_type.to_string();
    reference(element).into()
}

pub fn new_view_host(name: &str) -> ViewHostRef
{
    reference(Element::base(name, ElementKind::ViewHost)).into()
}

fn format_number(value: f64) -> String
{
    if value.is_nan()
    {
        return if value.is_sign_negative()
        {
            "-nan".to_string()
        }
        else
        {
            "nan".to_string()
        };
    }
    if value.is_infinite()
    {
        return if value.is_sign_negative()
        {
            "-inf".to_string()
        }
        else
        {
            "inf".to_string()
        };
    }
    if value == 0.0
    {
        return if value.is_sign_negative()
        {
            "-0".to_string()
        }
        else
        {
            "0".to_string()
        };
    }

    let scientific = format!("{value:.11e}");
    let (mantissa, exponent) = scientific
        .split_once('e')
        .expect("Rust scientific float formatting must include an exponent");
    let exponent = exponent
        .parse::<i32>()
        .expect("Rust scientific float exponent must be numeric");
    let negative = mantissa.starts_with('-');
    let unsigned_mantissa = mantissa.trim_start_matches('-');
    let mut digits = unsigned_mantissa.replace('.', "");
    while digits.len() > 1 && digits.ends_with('0')
    {
        digits.pop();
    }

    if !(-4..12).contains(&exponent)
    {
        let mantissa = if digits.len() == 1
        {
            digits
        }
        else
        {
            format!("{}.{}", &digits[..1], &digits[1..])
        };
        return format!(
            "{}{}e{}{exponent:02}",
            if negative { "-" } else { "" },
            mantissa,
            if exponent < 0 { "-" } else { "+" },
            exponent = exponent.abs(),
        );
    }

    let decimal_position = exponent + 1;
    let mut fixed = if decimal_position <= 0
    {
        format!(
            "0.{}{}",
            "0".repeat((-decimal_position) as usize),
            digits,
        )
    }
    else if decimal_position as usize >= digits.len()
    {
        format!(
            "{}{}",
            digits,
            "0".repeat(decimal_position as usize - digits.len()),
        )
    }
    else
    {
        let position = decimal_position as usize;
        format!("{}.{}", &digits[..position], &digits[position..])
    };
    if negative
    {
        fixed.insert(0, '-');
    }
    fixed
}

pub(crate) fn parse_number_like_stod(text: &str) -> Option<f64>
{
    if let Ok(value) = text.parse::<f64>()
    {
        return Some(value);
    }

    let trimmed = text.trim_start();
    let bytes = trimmed.as_bytes();
    let mut index = 0;
    if bytes.get(index).is_some_and(|byte| matches!(*byte, b'+' | b'-'))
    {
        index += 1;
    }
    let integer_start = index;
    while bytes.get(index).is_some_and(u8::is_ascii_digit)
    {
        index += 1;
    }
    let mut digit_count = index - integer_start;
    if bytes.get(index) == Some(&b'.')
    {
        index += 1;
        let fraction_start = index;
        while bytes.get(index).is_some_and(u8::is_ascii_digit)
        {
            index += 1;
        }
        digit_count += index - fraction_start;
    }
    if digit_count == 0
    {
        return None;
    }
    if bytes.get(index).is_some_and(|byte| matches!(*byte, b'e' | b'E'))
    {
        let exponent_marker = index;
        index += 1;
        if bytes.get(index).is_some_and(|byte| matches!(*byte, b'+' | b'-'))
        {
            index += 1;
        }
        let exponent_start = index;
        while bytes.get(index).is_some_and(u8::is_ascii_digit)
        {
            index += 1;
        }
        if exponent_start == index
        {
            index = exponent_marker;
        }
    }
    trimmed[..index].parse::<f64>().ok()
}

fn style_colors(style: &Style) -> (Option<Color>, Option<Color>)
{
    (style.color.clone(), style.background.clone())
}

fn aligned_col(width: i32, text_width: i32, align: &str) -> i32
{
    match align
    {
        "right" => max(0, width - text_width),
        "center" => max(0, (width - text_width) / 2),
        _ => 0,
    }
}

fn aligned_text(text: &str, width: i32, align: &str) -> String
{
    let width = max(0, width) as usize;
    let glyphs = crate::visual_glyphs(text, 0, 0);
    let mut content = glyphs
        .iter()
        .take(width)
        .map(|glyph| glyph.text.as_str())
        .collect::<String>();
    let content_width = min(glyphs.len(), width);
    if content_width >= width
    {
        return content;
    }
    let remaining = width - content_width;
    let left = match align
    {
        "right" => remaining,
        "center" => remaining / 2,
        _ => 0,
    };
    let right = remaining - left;
    content = format!("{}{}{}", " ".repeat(left), content, " ".repeat(right));
    content
}

fn render_label(buffer: &mut TerminalBuffer, element: &Element, style: &Style)
{
    let text = element.text();
    let (foreground, background) = style_colors(style);
    let selection = element.selection_range();
    let (selection_start, selection_end) = selection.unwrap_or_default();
    let has_selection = selection.is_some();
    let mut selection_style = style.clone();
    merge_optional_element_state_style(&mut selection_style, &element.cursor_style);
    let (selection_foreground, selection_background) = style_colors(&selection_style);
    let gradient_color = |gradient: &Option<crate::TextGradient>, source_index: usize|
    {
        let gradient = gradient.as_ref()?;
        if gradient.colors.is_empty()
        {
            return None;
        }
        let interval = i64::from(max(1, gradient.interval_ms));
        let time_ms = RENDER_TIME_OVERRIDE_MS.with(|current|
        {
            current.borrow().unwrap_or_else(||
            {
                SystemTime::now()
                    .duration_since(UNIX_EPOCH)
                    .map_or(0, |duration| duration.as_millis() as i64)
            })
        });
        let frame = (time_ms / interval) * i64::from(gradient.step);
        let segment_size = max(1, gradient.segment_size) as usize;
        let color_count = gradient.colors.len() as i64;
        let index = ((source_index / segment_size) as i64 - frame).rem_euclid(color_count);
        Some(gradient.colors[index as usize].clone())
    };
    let blend_gradient = |overlay: Option<Color>, base: &Option<Color>|
    {
        let Some(overlay) = overlay else { return base.clone() };
        let Some(rgba) = overlay.rgba() else { return Some(overlay) };
        if rgba.alpha == 0
        {
            return base.clone();
        }
        if rgba.alpha < u8::MAX && base.is_some()
        {
            return Some(overlay.blend_over(base.as_ref().expect("gradient base")));
        }
        Some(overlay)
    };
    let mut rows = Vec::new();
    if buffer.height > 1
    {
        let mut segment_start = 0;
        loop
        {
            let rest = &text[segment_start..];
            let newline = rest.find('\n').map(|offset| segment_start + offset);
            let segment_end = newline.unwrap_or(text.len());
            let segment = &text[segment_start..segment_end];
            let glyphs = crate::visual_glyphs(segment, 0, segment_start);
            if glyphs.is_empty()
            {
                rows.push(Vec::new());
            }
            else
            {
                let mut offset = 0;
                while offset < glyphs.len()
                {
                    let chunk_size = min(glyphs.len() - offset, buffer.width as usize);
                    let mut chunk = glyphs[offset..offset + chunk_size].to_vec();
                    if chunk_size == buffer.width as usize
                    {
                        let last_space = chunk.iter().enumerate().rev().find_map(|(index, glyph)|
                        {
                            if index > 0
                                && glyph.text == " "
                                && text.as_bytes().get(glyph.source_start) == Some(&b' ')
                            {
                                Some(index)
                            }
                            else
                            {
                                None
                            }
                        });
                        if let Some(last_space) = last_space
                        {
                            chunk.truncate(last_space);
                            offset += last_space + 1;
                            rows.push(chunk);
                            continue;
                        }
                    }
                    offset += chunk_size;
                    rows.push(chunk);
                }
            }
            let Some(newline) = newline else { break };
            segment_start = newline + 1;
            if segment_start > text.len()
            {
                break;
            }
        }
    }
    else
    {
        let segment = text.split_once('\n').map_or(text, |(first, _)| first);
        rows.push(crate::visual_glyphs(segment, 0, 0));
    }
    for (row, glyphs) in rows.iter().enumerate()
    {
        if row as i32 >= buffer.height
        {
            break;
        }
        let col = aligned_col(
            buffer.width,
            glyphs.len() as i32,
            style.text_align.as_deref().unwrap_or_default(),
        );
        for (offset, glyph) in glyphs.iter().enumerate()
        {
            if col + offset as i32 >= buffer.width
            {
                break;
            }
            let selected = has_selection
                && glyph.source_start >= selection_start
                && glyph.source_start < selection_end;
            let char_foreground = if selected
            {
                selection_foreground.clone()
            }
            else
            {
                blend_gradient(
                    gradient_color(&style.text_color_gradient, glyph.source_start),
                    &foreground,
                )
            };
            let char_background = if selected
            {
                selection_background.clone()
            }
            else
            {
                blend_gradient(
                    gradient_color(&style.text_background_gradient, glyph.source_start),
                    &background,
                )
            };
            buffer.draw_text(
                row as i32,
                col + offset as i32,
                &glyph.text,
                &char_foreground,
                &char_background,
                1,
            );
        }
    }
}

#[derive(Clone, Copy)]
enum TableAlignment
{
    Left,
    Center,
    Right,
}

struct ParsedMessageTable
{
    headers: Vec<String>,
    alignments: Vec<TableAlignment>,
    rows: Vec<Vec<String>>,
}

fn split_table_cells(line: &str) -> Vec<String>
{
    line.trim()
        .trim_start_matches('|')
        .trim_end_matches('|')
        .split('|')
        .map(|cell| cell.trim().to_string())
        .collect()
}

fn parse_message_table(text: &str) -> Option<ParsedMessageTable>
{
    let lines: Vec<&str> = text.lines().collect();
    if lines.len() < 3
    {
        return None;
    }
    let headers = split_table_cells(lines[0]);
    let separators = split_table_cells(lines[1]);
    if headers.is_empty()
        || headers.len() != separators.len()
        || separators.iter().any(|cell|
        {
            cell.is_empty() || cell.chars().any(|ch| !matches!(ch, '-' | ':'))
        })
    {
        return None;
    }
    let alignments = separators
        .iter()
        .map(|cell|
        {
            if cell.starts_with(':') && cell.ends_with(':')
            {
                TableAlignment::Center
            }
            else if cell.ends_with(':')
            {
                TableAlignment::Right
            }
            else
            {
                TableAlignment::Left
            }
        })
        .collect();
    let rows: Vec<Vec<String>> = lines[2..]
        .iter()
        .map(|line| split_table_cells(line))
        .filter(|row| row.len() == headers.len())
        .collect();
    if rows.is_empty()
    {
        return None;
    }
    Some(ParsedMessageTable { headers, alignments, rows })
}

pub fn message_table_fit_content_height(text: &str) -> i32
{
    parse_message_table(text)
        .map_or(1, |table| table.rows.len() as i32 + 4)
}

fn message_table_natural_width(text: &str) -> i32
{
    parse_message_table(text).map_or(1, |table|
    {
        let widths = message_table_column_widths(&table, 0);
        widths.iter().sum::<usize>() as i32 + widths.len() as i32 * 3 + 1
    })
}

fn message_table_column_widths(table: &ParsedMessageTable, target_width: i32) -> Vec<usize>
{
    let mut widths: Vec<usize> = table
        .headers
        .iter()
        .map(|text| crate::visual_glyphs(text, 0, 0).len())
        .collect();
    for row in &table.rows
    {
        for (index, text) in row.iter().enumerate()
        {
            widths[index] = max(widths[index], crate::visual_glyphs(text, 0, 0).len());
        }
    }
    let natural_width = widths.iter().sum::<usize>() + widths.len() * 3 + 1;
    let extra = max(0, target_width - natural_width as i32) as usize;
    let count = widths.len();
    if let Some(per_column) = extra.checked_div(count)
    {
        let remainder = extra % count;
        for (index, width) in widths.iter_mut().enumerate()
        {
            *width += per_column + usize::from(index < remainder);
        }
    }
    widths
}

fn aligned_table_text(text: &str, width: usize, alignment: TableAlignment) -> String
{
    let mut visible: String = crate::visual_glyphs(text, 0, 0)
        .iter()
        .take(width)
        .map(|glyph| glyph.text.as_str())
        .collect();
    let visible_width = crate::visual_glyphs(&visible, 0, 0).len();
    if visible_width >= width
    {
        return visible;
    }
    let padding = width - visible_width;
    match alignment
    {
        TableAlignment::Right => format!("{}{}", " ".repeat(padding), visible),
        TableAlignment::Center =>
        {
            let left = padding / 2;
            visible = format!(
                "{}{}{}",
                " ".repeat(left),
                visible,
                " ".repeat(padding - left),
            );
            visible
        }
        TableAlignment::Left => format!("{}{}", visible, " ".repeat(padding)),
    }
}

fn message_table_separator(
    widths: &[usize],
    left: char,
    middle: char,
    right: char,
) -> String
{
    let mut line = String::from(left);
    for (index, width) in widths.iter().enumerate()
    {
        line.push_str(&"─".repeat(width + 2));
        line.push(if index + 1 < widths.len() { middle } else { right });
    }
    line
}

fn message_table_data_row(
    cells: &[String],
    widths: &[usize],
    alignments: &[TableAlignment],
) -> String
{
    let mut line = String::from('│');
    for (index, width) in widths.iter().enumerate()
    {
        let value = cells.get(index).map(String::as_str).unwrap_or_default();
        line.push(' ');
        line.push_str(&aligned_table_text(value, *width, alignments[index]));
        line.push(' ');
        line.push('│');
    }
    line
}

fn render_message_table(buffer: &mut TerminalBuffer, element: &Element, style: &Style)
{
    let Some(table) = parse_message_table(element.text()) else { return };
    let widths = message_table_column_widths(&table, buffer.width);
    let mut lines = vec![
        message_table_separator(&widths, '┌', '┬', '┐'),
        message_table_data_row(&table.headers, &widths, &table.alignments),
        message_table_separator(&widths, '├', '┼', '┤'),
    ];
    for row in &table.rows
    {
        lines.push(message_table_data_row(row, &widths, &table.alignments));
    }
    lines.push(message_table_separator(&widths, '└', '┴', '┘'));
    let (foreground, background) = style_colors(style);
    for (row, line) in lines.iter().enumerate()
    {
        if row as i32 >= buffer.height
        {
            break;
        }
        buffer.draw_text(
            row as i32,
            0,
            line,
            &foreground,
            &background,
            buffer.width,
        );
    }
}

fn render_info_label(buffer: &mut TerminalBuffer, element: &Element, style: &Style)
{
    const INFO_LABEL_GRADIENT_INTERVAL_MS: i32 = 70;
    const INFO_LABEL_GRADIENT_STEP: i32 = 1;
    const INFO_LABEL_GRADIENT_SEGMENT_SIZE: i32 = 1;

    let mut style = style.clone();
    if style.text_color_gradient.is_none()
    {
        style.text_color_gradient = Some(crate::TextGradient
        {
            interval_ms: INFO_LABEL_GRADIENT_INTERVAL_MS,
            step: INFO_LABEL_GRADIENT_STEP,
            segment_size: INFO_LABEL_GRADIENT_SEGMENT_SIZE,
            colors: [
                "#ffffff66",
                "#ffffff88",
                "#ffffffaa",
                "#ffffffcc",
                "#ffffffff",
                "#ffffffcc",
                "#ffffffaa",
                "#ffffff88",
                "#ffffff66",
            ]
                .into_iter()
                .map(Color::new)
                .collect(),
        });
    }
    render_label(buffer, element, &style);
}

fn split_frame_buffer_dimension(size: i32, parts: i32) -> Vec<i32>
{
    let size = max(1, size);
    let parts = max(1, min(parts, size));
    let base = size / parts;
    let extra = size % parts;
    (0..parts)
        .map(|index| base + i32::from(index < extra))
        .collect()
}

fn default_frame_buffer_regions(
    state: &FrameBufferViewState,
    width: i32,
    height: i32,
) -> BTreeMap<String, FrameBufferRegion>
{
    let widths = split_frame_buffer_dimension(
        width,
        max(1, state.target_order.len() as i32),
    );
    let mut regions = BTreeMap::new();
    let mut col = 0;
    for (index, name) in state.target_order.iter().enumerate()
    {
        let region_width = widths[index];
        regions.insert(
            name.clone(),
            FrameBufferRegion
            {
                row: 0,
                col,
                width: region_width,
                height: max(1, height),
            },
        );
        col += region_width;
    }
    regions
}

fn dim_frame_buffer_color(value: &str) -> String
{
    const DIM_FACTOR: f64 = 0.6;
    if value.len() != 7 || !value.starts_with('#')
    {
        return value.to_string();
    }
    let Ok(color) = Color::try_new(value) else
    {
        return value.to_string();
    };
    let Some(crate::Rgba { red, green, blue, .. }) = color.rgba() else
    {
        return value.to_string();
    };
    let dim = |component: u8|
    {
        (f64::from(component) * DIM_FACTOR).round() as u8
    };
    format!("#{:02x}{:02x}{:02x}", dim(red), dim(green), dim(blue))
}

fn render_frame_buffer_view(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
)
{
    let state = element.frame_buffer_view();
    let regions = if state.regions.is_empty()
    {
        default_frame_buffer_regions(state, buffer.width, buffer.height)
    }
    else
    {
        state.regions.clone()
    };
    for name in &state.target_order
    {
        let Some(region) = regions.get(name) else { continue };
        let Some(rows) = state.frames.get(name) else { continue };
        for local_row in 0..region.height
        {
            let output_row = region.row + local_row;
            let source_row = state.pan_row + local_row;
            if output_row < 0
                || output_row >= buffer.height
                || source_row < 0
                || source_row >= rows.len() as i32
            {
                continue;
            }
            let source_cells = &rows[source_row as usize];
            for local_col in 0..region.width
            {
                let output_col = region.col + local_col;
                let source_col = state.pan_col + local_col;
                if output_col < 0
                    || output_col >= buffer.width
                    || source_col < 0
                    || source_col >= source_cells.len() as i32
                {
                    continue;
                }
                let source = &source_cells[source_col as usize];
                let foreground_text = if state.dimmed
                {
                    dim_frame_buffer_color(&source.foreground)
                }
                else
                {
                    source.foreground.clone()
                };
                let background_text = if state.dimmed
                {
                    dim_frame_buffer_color(&source.background)
                }
                else
                {
                    source.background.clone()
                };
                let foreground = if foreground_text.is_empty()
                {
                    style.color.clone()
                }
                else
                {
                    Color::try_new(foreground_text)
                        .ok()
                        .or_else(|| style.color.clone())
                };
                let background = if background_text.is_empty()
                {
                    style.background.clone()
                }
                else
                {
                    Color::try_new(background_text)
                        .ok()
                        .or_else(|| style.background.clone())
                };
                if let Some(cell) = buffer.cell_mut(output_row, output_col)
                {
                    cell.text = crate::safe_terminal_cell_text(&source.text);
                    cell.foreground = foreground;
                    cell.background = background;
                }
            }
        }
    }

    if !state.panel_focused && !state.panel_edit
    {
        return;
    }
    let interaction_style = if state.panel_edit
    {
        element.edit_style.as_ref().unwrap_or(style)
    }
    else if state.panel_focused
    {
        element.focus_style.as_ref().unwrap_or(style)
    }
    else
    {
        style
    };
    let corner_color = interaction_style.color.clone().or_else(||
    {
        Some(Color::new(if state.panel_edit { "#22c55e" } else { "#facc15" }))
    });
    for name in &state.target_order
    {
        let Some(region) = regions.get(name) else { continue };
        if region.width < 2 || region.height < 2
        {
            continue;
        }
        for (row, col, ch) in [
            (region.row, region.col, '┌'),
            (region.row, region.col + 1, '─'),
            (region.row, region.col + region.width - 2, '─'),
            (region.row, region.col + region.width - 1, '┐'),
            (region.row + region.height - 1, region.col, '└'),
            (region.row + region.height - 1, region.col + 1, '─'),
            (region.row + region.height - 1, region.col + region.width - 2, '─'),
            (region.row + region.height - 1, region.col + region.width - 1, '┘'),
        ]
        {
            if let Some(cell) = buffer.cell_mut(row, col)
            {
                cell.text = ch.to_string();
                cell.foreground = corner_color.clone();
            }
        }
    }
}

fn render_span_label(buffer: &mut TerminalBuffer, element: &Element, style: &Style)
{
    let spans = &element.label().expect("SpanLabel state").spans;
    if spans.is_empty()
    {
        render_label(buffer, element, style);
        return;
    }
    let (default_foreground, default_background) = style_colors(style);
    let (selection_start, selection_end) =
        element.selection_range().unwrap_or_default();
    let mut selection_style = style.clone();
    merge_optional_element_state_style(&mut selection_style, &element.cursor_style);
    let (selection_foreground, selection_background) = style_colors(&selection_style);
    let mut row = 0;
    let mut col = 0;
    let mut source_index = 0;
    let mut clipping_line = false;
    for span in spans
    {
        let foreground = if span.foreground.is_empty()
        {
            default_foreground.clone()
        }
        else
        {
            Some(Color::new(&span.foreground))
        };
        let background = if span.background.is_empty()
        {
            default_background.clone()
        }
        else
        {
            Some(Color::new(&span.background))
        };
        for ch in span.text.chars()
        {
            if ch == '\r'
            {
                source_index += 1;
                continue;
            }
            if ch == '\n'
            {
                source_index += 1;
                row += 1;
                col = 0;
                clipping_line = false;
                if row >= buffer.height
                {
                    return;
                }
                continue;
            }
            if col >= buffer.width
            {
                clipping_line = true;
            }
            if !clipping_line
            {
                for glyph in crate::visual_glyphs(&ch.to_string(), col, 0)
                {
                    if col >= buffer.width
                    {
                        clipping_line = true;
                        break;
                    }
                    buffer.draw_text(
                        row,
                        col,
                        &glyph.text,
                        if source_index >= selection_start && source_index < selection_end
                        {
                            &selection_foreground
                        }
                        else
                        {
                            &foreground
                        },
                        if source_index >= selection_start && source_index < selection_end
                        {
                            &selection_background
                        }
                        else
                        {
                            &background
                        },
                        1,
                    );
                    col += 1;
                }
            }
            source_index += 1;
        }
    }
}

fn render_button(buffer: &mut TerminalBuffer, element: &Element, style: &Style, focused: bool)
{
    let (foreground, background) = style_colors(style);
    let title = element.title();
    let title_width = crate::visible_width(title);
    let mut body = title.to_string();
    if focused && buffer.width >= title_width + 2
    {
        if buffer.width >= title_width + 6
        {
            body = format!(
                " [{}] ",
                aligned_text(&format!(" {title} "), buffer.width - 4, "center")
            );
        }
        else if buffer.width >= title_width + 4
        {
            body = format!(
                " [{}] ",
                aligned_text(title, buffer.width - 4, "center")
            );
        }
        else
        {
            body = format!("[{title}]");
        }
    }
    let body = aligned_text(&body, buffer.width, "center");
    let row = max(0, (buffer.height - 1) / 2);
    buffer.draw_text(row, 0, &body, &foreground, &background, buffer.width);
}

fn render_checkbox(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    focused_or_editing: bool,
)
{
    let (foreground, background) = style_colors(style);
    let mut indicator_style = style.clone();
    merge_optional_element_state_style(
        &mut indicator_style,
        if element.checked() { &element.checked_style } else { &element.unchecked_style },
    );
    if focused_or_editing
        && indicator_style
            .background
            .as_ref()
            .is_some_and(Color::is_transparent)
    {
        indicator_style.background = style.background.clone();
    }
    let (indicator_foreground, indicator_background) = style_colors(&indicator_style);
    let indicator = if element.checked() { "[x]" } else { "[ ]" };
    let text = if element.title().is_empty()
    {
        indicator.to_string()
    }
    else
    {
        format!("{indicator} {}", element.title())
    };
    buffer.draw_text(0, 0, &text, &foreground, &background, buffer.width);
    buffer.draw_text(
        0,
        0,
        indicator,
        &indicator_foreground,
        &indicator_background,
        min(3, buffer.width),
    );
}

fn render_text_input(buffer: &mut TerminalBuffer, element: &Element, style: &Style, edit_mode: bool)
{
    if element.kind() == ElementKind::TextArea
    {
        render_text_area(buffer, element, style, edit_mode);
        return;
    }
    let (foreground, background) = style_colors(style);
    let state = element.text_input().expect("TextInput state");
    let row = single_visual_text_row(&state.value);
    let text_width = row.cells.len();
    let visible_width = max(1, buffer.width) as usize;
    let mut offset = state.col_scroll_offset.get().max(0) as usize;
    if text_width <= visible_width
    {
        offset = 0;
    }
    let cursor_column = visual_column_for_cursor(
        &row,
        state.cursor,
        max(visible_width, text_width + 1),
    ).unwrap_or_default();
    if edit_mode
    {
        if cursor_column < offset
        {
            offset = cursor_column;
        }
        else if cursor_column >= offset + visible_width
        {
            offset = cursor_column - visible_width + 1;
        }
    }
    state.col_scroll_offset.set(offset as i32);
    let visible_cells = if offset < text_width
    {
        &row.cells[offset..min(text_width, offset + visible_width)]
    }
    else
    {
        &[]
    };
    for (column, cell) in visible_cells.iter().enumerate()
    {
        buffer.draw_text(
            0,
            column as i32,
            &cell.ch.to_string(),
            &foreground,
            &background,
            1,
        );
    }
    if !edit_mode && text_width > offset + visible_width
    {
        buffer.draw_text(
            0,
            buffer.width - 1,
            ">",
            &foreground,
            &background,
            1,
        );
    }
    let has_selection = edit_mode
        && state
            .selection_anchor
            .is_some_and(|anchor| anchor != state.cursor);
    if has_selection && let Some(anchor) = state.selection_anchor
    {
        let start = min(anchor, state.cursor);
        let end = max(anchor, state.cursor);
        let mut selected_style = style.clone();
        merge_optional_element_state_style(&mut selected_style, &element.cursor_style);
        let (selected_foreground, selected_background) = style_colors(&selected_style);
        for (column, cell) in visible_cells.iter().enumerate()
        {
            if cell.source_start < start || cell.source_start >= end
            {
                continue;
            }
            buffer.draw_text(
                0,
                column as i32,
                &cell.ch.to_string(),
                &selected_foreground,
                &selected_background,
                1,
            );
        }
    }
    else if edit_mode
    {
        let cursor_col = min(
            cursor_column.saturating_sub(offset),
            visible_width.saturating_sub(1),
        );
        let mut cursor_style = style.clone();
        merge_optional_element_state_style(&mut cursor_style, &element.cursor_style);
        let (cursor_foreground, cursor_background) = style_colors(&cursor_style);
        let cursor_char = visible_cells
            .get(cursor_col)
            .map_or(' ', |cell| cell.ch);
        buffer.draw_text(
            0,
            cursor_col as i32,
            &cursor_char.to_string(),
            &cursor_foreground,
            &cursor_background,
            1,
        );
    }
}

fn render_number_input(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    edit_mode: bool,
)
{
    let (foreground, background) = style_colors(style);
    let state = element.number_input();
    let mut edit = state.edit.borrow_mut();
    if edit_mode && !edit.editing
    {
        edit.edit_original_value = state.value;
        edit.edit_text = format_number(state.value);
        edit.edit_cursor = edit.edit_text.len();
        edit.editing = true;
        edit.replace_on_first_text_input = state.value == 0.0;
    }
    let display_value;
    let text = if edit_mode
    {
        edit.edit_text.as_str()
    }
    else
    {
        display_value = format_number(state.value);
        display_value.as_str()
    };
    buffer.draw_text(0, 0, text, &foreground, &background, buffer.width);
    if !edit_mode
    {
        return;
    }
    let mut cursor_style = style.clone();
    merge_optional_element_state_style(&mut cursor_style, &element.cursor_style);
    let (cursor_foreground, cursor_background) = style_colors(&cursor_style);
    if edit.replace_on_first_text_input
    {
        for (column, ch) in text.chars().take(buffer.width as usize).enumerate()
        {
            buffer.draw_text(
                0,
                column as i32,
                &ch.to_string(),
                &cursor_foreground,
                &cursor_background,
                1,
            );
        }
        return;
    }
    let cursor_index = clamp_utf8_boundary(text, edit.edit_cursor);
    let cursor_column = min(
        crate::visible_width(&text[..cursor_index]) as usize,
        buffer.width.saturating_sub(1) as usize,
    );
    let cursor_char = text[cursor_index..]
        .chars()
        .next()
        .unwrap_or(' ');
    buffer.draw_text(
        0,
        cursor_column as i32,
        &cursor_char.to_string(),
        &cursor_foreground,
        &cursor_background,
        1,
    );
}

#[derive(Clone, Debug)]
struct VisualTextRow
{
    start: usize,
    end: usize,
    cells: Vec<VisualTextCell>,
}

#[derive(Clone, Debug)]
struct VisualTextCell
{
    ch: char,
    source_start: usize,
    source_end: usize,
}

fn make_visual_text_row(fallback_start: usize, cells: Vec<VisualTextCell>) -> VisualTextRow
{
    if cells.is_empty()
    {
        return VisualTextRow {
            start: fallback_start,
            end: fallback_start,
            cells,
        };
    }
    VisualTextRow
    {
        start: cells[0].source_start,
        end: cells.last().map_or(fallback_start, |cell| cell.source_end),
        cells,
    }
}

fn single_visual_text_row(value: &str) -> VisualTextRow
{
    make_visual_text_row(
        0,
        crate::visual_glyphs(value, 0, 0)
            .into_iter()
            .map(|glyph|
            {
                VisualTextCell
                {
                    ch: glyph.text.chars().next().unwrap_or(' '),
                    source_start: glyph.source_start,
                    source_end: glyph.source_end,
                }
            })
            .collect(),
    )
}

fn build_visual_text_rows(value: &str, width: usize) -> Vec<VisualTextRow>
{
    let width = max(1, width);
    let mut rows = Vec::new();
    let mut segment_start = 0;
    loop
    {
        let segment_end = value[segment_start..]
            .find('\n')
            .map_or(value.len(), |offset| segment_start + offset);
        if segment_start == segment_end
        {
            rows.push(make_visual_text_row(segment_start, Vec::new()));
        }
        else
        {
            let glyphs: Vec<VisualTextCell> = crate::visual_glyphs(
                &value[segment_start..segment_end],
                0,
                segment_start,
            )
                .into_iter()
                .map(|glyph|
                {
                    VisualTextCell
                    {
                        ch: glyph.text.chars().next().unwrap_or(' '),
                        source_start: glyph.source_start,
                        source_end: glyph.source_end,
                    }
                })
                .collect();

            let mut offset = 0;
            while offset < glyphs.len()
            {
                let remaining = glyphs.len() - offset;
                let chunk_size = min(remaining, width);
                let mut chunk = glyphs[offset..offset + chunk_size].to_vec();
                if chunk_size == width
                    && let Some(last_space) = (1..chunk_size)
                        .rev()
                        .find(|index|
                        {
                            let cell = &chunk[*index];
                                cell.ch == ' '
                                && value.as_bytes().get(cell.source_start) == Some(&b' ')
                        })
                {
                    chunk.truncate(last_space);
                    rows.push(make_visual_text_row(segment_start, chunk));
                    offset += last_space + 1;
                    continue;
                }
                rows.push(make_visual_text_row(segment_start, chunk));
                offset += chunk_size;
            }
        }
        if segment_end == value.len()
        {
            break;
        }
        segment_start = segment_end + 1;
    }
    if rows.is_empty()
    {
        rows.push(VisualTextRow { start: 0, end: 0, cells: Vec::new() });
    }
    rows
}

fn raw_index_for_visual_column(row: &VisualTextRow, column: usize) -> usize
{
    if row.cells.is_empty()
    {
        return row.start;
    }
    if column >= row.cells.len()
    {
        return row.end;
    }
    let cell = &row.cells[column];
    let mut run_start = column;
    let mut run_end = column + 1;
    while run_start > 0
        && row.cells[run_start - 1].source_start == cell.source_start
        && row.cells[run_start - 1].source_end == cell.source_end
    {
        run_start -= 1;
    }
    while run_end < row.cells.len()
        && row.cells[run_end].source_start == cell.source_start
        && row.cells[run_end].source_end == cell.source_end
    {
        run_end += 1;
    }
    let midpoint = run_start as f64 + (run_end - run_start) as f64 / 2.0;
    if column as f64 >= midpoint
    {
        cell.source_end
    }
    else
    {
        cell.source_start
    }
}

fn clamp_utf8_boundary(value: &str, mut index: usize) -> usize
{
    index = min(index, value.len());
    while index > 0 && !value.is_char_boundary(index)
    {
        index -= 1;
    }
    index
}

fn previous_utf8_boundary(value: &str, index: usize) -> usize
{
    let index = clamp_utf8_boundary(value, index);
    value[..index]
        .char_indices()
        .next_back()
        .map_or(0, |(offset, _)| offset)
}

fn next_utf8_boundary(value: &str, index: usize) -> usize
{
    let index = clamp_utf8_boundary(value, index);
    value[index..]
        .chars()
        .next()
        .map_or(value.len(), |ch| index + ch.len_utf8())
}

fn visual_row_for_cursor(rows: &[VisualTextRow], width: usize, cursor: usize) -> usize
{
    rows.iter()
        .position(|row| cursor <= row.end && (row.cells.len() < width || cursor < row.end))
        .unwrap_or_else(|| rows.len().saturating_sub(1))
}

fn visual_column_for_cursor(row: &VisualTextRow, cursor: usize, width: usize) -> Option<usize>
{
    if cursor < row.start || cursor > row.end
    {
        return None;
    }
    if let Some(index) = row.cells.iter().position(|cell|
    {
        cursor <= cell.source_start
            || (cursor > cell.source_start && cursor < cell.source_end)
    })
    {
        return Some(index);
    }
    Some(min(row.cells.len(), width.saturating_sub(1)))
}

fn move_multiline_cursor(value: &str, width: usize, cursor: usize, delta: i32) -> usize
{
    let rows = build_visual_text_rows(value, width);
    let current_index = visual_row_for_cursor(&rows, width, cursor);
    let target_index = current_index as i32 + delta;
    if target_index < 0 || target_index >= rows.len() as i32
    {
        return cursor;
    }
    let current = &rows[current_index];
    let target = &rows[target_index as usize];
    let column = visual_column_for_cursor(current, cursor, width)
        .unwrap_or_default()
        .min(current.cells.len());
    raw_index_for_visual_column(target, column)
}

fn line_start_for_cursor(value: &str, cursor: usize) -> usize
{
    let cursor = clamp_utf8_boundary(value, cursor);
    value[..cursor]
        .rfind('\n')
        .map_or(0, |index| index + 1)
}

fn line_end_for_cursor(value: &str, cursor: usize) -> usize
{
    let cursor = clamp_utf8_boundary(value, cursor);
    value[cursor..]
        .find('\n')
        .map_or(value.len(), |offset| cursor + offset)
}

fn render_text_area(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    edit_mode: bool,
)
{
    let (foreground, background) = style_colors(style);
    let state = element.text_input().expect("TextArea state");
    let rows = build_visual_text_rows(&state.value, buffer.width as usize);
    let cursor_row = visual_row_for_cursor(&rows, buffer.width as usize, state.cursor);
    let max_offset = rows.len().saturating_sub(buffer.height as usize);
    let mut row_offset = state.row_scroll_offset.get().max(0) as usize;
    row_offset = min(row_offset, max_offset);
    if edit_mode && !state.manual_row_scroll.get()
    {
        if cursor_row < row_offset
        {
            row_offset = cursor_row;
        }
        else if cursor_row >= row_offset + buffer.height as usize
        {
            row_offset = cursor_row - buffer.height as usize + 1;
        }
    }
    state.row_scroll_offset.set(row_offset as i32);
    let has_selection = edit_mode
        && state
            .selection_anchor
            .is_some_and(|anchor| anchor != state.cursor);
    let selection_start =
        state.selection_anchor.map_or(state.cursor, |anchor| min(anchor, state.cursor));
    let selection_end =
        state.selection_anchor.map_or(state.cursor, |anchor| max(anchor, state.cursor));
    let mut cursor_style = style.clone();
    merge_optional_element_state_style(&mut cursor_style, &element.cursor_style);
    let (cursor_foreground, cursor_background) = style_colors(&cursor_style);
    let has_above = row_offset > 0;
    let has_below = row_offset + (buffer.height as usize) < rows.len();

    for output_row in 0..buffer.height as usize
    {
        let row_index = row_offset + output_row;
        let Some(row) = rows.get(row_index) else { continue };
        for (column, cell) in row.cells.iter().enumerate().take(buffer.width as usize)
        {
            let selected = has_selection
                && cell.source_start >= selection_start
                && cell.source_start < selection_end;
            buffer.draw_text(
                output_row as i32,
                column as i32,
                &cell.ch.to_string(),
                if selected { &cursor_foreground } else { &foreground },
                if selected { &cursor_background } else { &background },
                1,
            );
        }
        let cursor_column = if edit_mode && row_index == cursor_row
        {
            visual_column_for_cursor(row, state.cursor, buffer.width as usize)
        }
        else
        {
            None
        };
        let first_visible_row = output_row == 0;
        let last_visible_row = output_row + 1 == buffer.height as usize;
        let indicator_visible = buffer.width > 0
            && row.cells.len() < buffer.width as usize
            && ((first_visible_row && has_above) || (last_visible_row && has_below));
        let indicator_column = buffer.width.saturating_sub(1) as usize;
        if indicator_visible && cursor_column != Some(indicator_column)
        {
            buffer.draw_text(
                output_row as i32,
                indicator_column as i32,
                if first_visible_row && has_above { "^" } else { "v" },
                &foreground,
                &background,
                1,
            );
        }
        if edit_mode
            && !has_selection
            && let Some(column) = cursor_column
        {
            let cursor_char = row.cells.get(column).map_or(' ', |cell| cell.ch);
            buffer.draw_text(
                output_row as i32,
                column as i32,
                &cursor_char.to_string(),
                &cursor_foreground,
                &cursor_background,
                1,
            );
        }
    }
}

fn render_combo_box(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    focused: bool,
    edit_mode: bool,
)
{
    let (foreground, background) = style_colors(style);
    let value = element.selected_value();
    buffer.draw_text(0, 0, &value, &foreground, &background, buffer.width);
    if focused || edit_mode
    {
        buffer.draw_text(
            0,
            max(0, buffer.width - 1),
            if edit_mode { "v" } else { ">" },
            &foreground,
            &background,
            1,
        );
    }
    if edit_mode
    {
        for (index, option) in element.options().iter().enumerate()
        {
            let mut option_style = style.clone();
            if index as i32 == element.selected_index()
            {
                merge_optional_element_state_style(
                    &mut option_style,
                    &element.selected_style,
                );
            }
            let (option_foreground, option_background) = style_colors(&option_style);
            let row = index as i32 + 1;
            buffer.fill(
                Rect { row, col: 0, width: buffer.width, height: 1 },
                ' ',
                &option_foreground,
                &option_background,
            );
            buffer.draw_text(
                row,
                0,
                &format!("  {option}"),
                &option_foreground,
                &option_background,
                buffer.width,
            );
        }
    }
}

fn render_list_box(buffer: &mut TerminalBuffer, element: &Element, style: &Style, edit_mode: bool)
{
    let state = element.list_box().expect("ListBox state");
    state.last_viewport_height.set(buffer.height);
    let max_offset = max(0, state.options.len() as i32 - buffer.height);
    state
        .scroll_offset
        .set(state.scroll_offset.get().clamp(0, max_offset));
    if edit_mode && state.active_item_visible
    {
        ensure_list_index_visible(state, state.active_index, buffer.height);
    }
    else
    {
        ensure_list_index_visible(state, state.selected_index, buffer.height);
    }
    for row in 0..buffer.height
    {
        let index = state.scroll_offset.get() + row;
        let Some(option) = state.options.get(index as usize) else { continue };
        let selected = state.selected_indices.contains(&(index as usize));
        let disabled = state.disabled_values.iter().any(|value| value == option);
        let mut row_style = style.clone();
        if selected
        {
            merge_optional_list_box_row_layer(&mut row_style, &element.selected_style);
        }
        if edit_mode && state.active_item_visible && state.active_index == index
        {
            if element.active_style.is_none()
            {
                if !selected
                {
                    merge_optional_list_box_row_layer(
                        &mut row_style,
                        &element.selected_style,
                    );
                }
            }
            else if let Some(active_style) = &element.active_style
            {
                merge_list_box_row_layer(&mut row_style, active_style);
            }
        }
        if disabled
        {
            row_style = style.clone();
            merge_optional_element_state_style(&mut row_style, &element.disabled_style);
        }
        let (row_foreground, row_background) = style_colors(&row_style);
        buffer.fill(Rect { row, col: 0, width: buffer.width, height: 1 }, ' ', &row_foreground, &row_background);
        buffer.draw_text(row, 0, option, &row_foreground, &row_background, buffer.width);
        if row == 0 && state.scroll_offset.get() > 0
        {
            buffer.draw_text(row, max(0, buffer.width - 1), "^", &row_foreground, &row_background, 1);
        }
        else if row == buffer.height - 1
            && state.scroll_offset.get() + buffer.height < state.options.len() as i32
        {
            buffer.draw_text(row, max(0, buffer.width - 1), "v", &row_foreground, &row_background, 1);
        }
    }
}

fn merge_list_box_row_layer(base: &mut Style, layer: &Style)
{
    let inherited_background = base.background.clone();
    base.merge(layer);
    if layer
        .background
        .as_ref()
        .is_some_and(Color::translucent)
        && let Some(inherited_background) = inherited_background.as_ref()
    {
        base.background = Some(
            layer
                .background
                .as_ref()
                .expect("translucent row background")
                .blend_over(inherited_background),
        );
    }
}

fn merge_optional_list_box_row_layer(base: &mut Style, layer: &Option<Style>)
{
    if let Some(layer) = layer
    {
        merge_list_box_row_layer(base, layer);
    }
}

fn render_scroll_view(
    buffer: &mut TerminalBuffer,
    element: &Element,
    style: &Style,
    _edit_mode: bool,
)
{
    let padding = style.padding_values();
    let left = min(max(0, padding[3]), buffer.width - 1);
    let right = min(max(0, padding[1]), buffer.width - left - 1);
    let top = min(max(0, padding[0]), buffer.height - 1);
    let bottom = min(max(0, padding[2]), buffer.height - top - 1);
    let content_width = max(1, buffer.width - left - right);
    let content_height = max(1, buffer.height - top - bottom);
    let scroll_view = element
        .scroll_view()
        .expect("ScrollView render requested from missing state");
    let h_offset = max(0, scroll_view.h_view_offset());
    let render_width = content_width + h_offset;
    let native_heights = element.scroll_view_native_child_heights(render_width);
    scroll_view.refresh_dynamic_children(render_width);
    let natural_skip = scroll_view.max_view_offset(
        crate::Size { width: render_width, height: content_height },
        &native_heights,
    );
    let actual_skip = natural_skip - min(scroll_view.view_offset(), natural_skip);
    let clip = Rect
    {
        row: top,
        col: left,
        width: content_width,
        height: content_height,
    };
    let mut row = top - actual_skip;
    let gap = scroll_view.gap();
    let rendered_children = scroll_view.rendered_children();
    for child in rendered_children.iter()
    {
        if row + child.height > top && row < top + content_height
        {
            buffer.overlay_clipped(child, row, left - h_offset, clip);
        }
        row += child.height + gap;
    }
    drop(rendered_children);
    if actual_skip > 0
    {
        buffer.draw_text(
            top,
            left + content_width - 1,
            SCROLL_INDICATOR_ABOVE,
            &style.color,
            &style.background,
            1,
        );
    }
    if scroll_view.view_offset() > 0
    {
        buffer.draw_text(
            top + content_height - 1,
            left + content_width - 1,
            SCROLL_INDICATOR_BELOW,
            &style.color,
            &style.background,
            1,
        );
    }
}

fn apply_scroll_view_passive_focus_background(
    buffer: &mut TerminalBuffer,
    focus_background: &Option<Color>,
    base_background: &Option<Color>,
    focused_background: &Option<Color>,
)
{
    let focus_background = focus_background
        .as_ref()
        .expect("passive focus requires a background");
    let focused_base_background = base_background.as_ref().map(|base_background|
    {
        if focus_background.rgba().is_some()
        {
            focus_background.blend_over(base_background)
        }
        else
        {
            focus_background.clone()
        }
    });
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            let matches_base = cell.background.is_none()
                || cell.background == *base_background
                || cell.background == *focused_background
                || focused_base_background
                    .as_ref()
                    .is_some_and(|background| cell.background.as_ref() == Some(background));
            if !matches_base
            {
                continue;
            }
            cell.background = if let Some(cell_background) = cell
                .background
                .as_ref()
                .filter(|_| focus_background.rgba().is_some())
            {
                Some(focus_background.blend_over(cell_background))
            }
            else
            {
                Some(focus_background.clone())
            };
        }
    }
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn listbox_keeps_selected_item_visible()
    {
        let list = new_list_box("items", (0..10).map(|value| value.to_string()).collect(), false);
        list.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 3 };
        list.borrow_mut().set_selected_index(8);
        assert_eq!(list.borrow().list_scroll_offset(), 0);
        let _ = list.borrow().render(5, 3, true, false);
        assert_eq!(list.borrow().list_scroll_offset(), 6);
    }

    #[test]
    fn listbox_constructor_preserves_the_cpp_initial_selection_before_multi_mode()
    {
        for multiple in [false, true]
        {
            let list = new_list_box(
                "items",
                vec!["Developer".to_string(), "Designer".to_string()],
                multiple,
            );
            assert_eq!(list.selected_index(), 0);
            assert_eq!(list.selected_indices(), vec![0]);

            list.set_style(Style
            {
                background: Some(Color::new("#0d1524")),
                ..Default::default()
            });
            list.set_selected_style(Style
            {
                background: Some(Color::new("#2563eb")),
                ..Default::default()
            });
            let rendered = list.render(
                crate::Size { width: 12, height: 2 },
                ElementRenderState::default(),
            );
            assert_eq!(
                rendered
                    .cell(0, 0)
                    .and_then(|cell| cell.background.as_ref())
                    .map(Color::normalized)
                    .as_deref(),
                Some("#2563eb"),
            );
        }
    }

    #[test]
    fn concrete_controls_match_cpp_natural_render_sizes()
    {
        let natural = crate::Size { width: 0, height: 2 };
        let state = ElementRenderState::default();

        assert_eq!(new_label("label", "界").render(natural, state).width, 1);
        assert_eq!(new_info_label("info", "界").render(natural, state).width, 3);
        assert_eq!(new_button("button", "Go").render(natural, state).width, 6);
        assert_eq!(
            new_checkbox("check", "Go", false).render(natural, state).width,
            6,
        );
        assert_eq!(
            new_text_input("input", "界", 0).render(natural, state).width,
            2,
        );
        assert_eq!(
            new_text_area("area", "界").render(natural, state).width,
            2,
        );
        assert_eq!(
            new_number_input("number", 12.5).render(natural, state).width,
            4,
        );
        assert_eq!(
            new_combo_box("combo", vec!["One".to_string()])
                .render(natural, state)
                .width,
            5,
        );
        assert_eq!(
            new_list_box("list", vec!["One".to_string()], false)
                .render(natural, state)
                .width,
            5,
        );

        let table = new_message_table("table");
        table.set_text("| A | B |\n| - | - |\n| 1 | 2 |");
        let rendered = table.render(
            crate::Size { width: 2, height: 1 },
            state,
        );
        assert!(rendered.width > 2);
        assert_eq!(rendered.height, table.fit_content_height());
    }

    #[test]
    fn direct_number_input_edit_render_establishes_the_reference_snapshot()
    {
        let input = new_number_input("number", 0.0);
        let rendered = input.render(
            crate::Size { width: 0, height: 1 },
            ElementRenderState
            {
                edit_mode: true,
                ..ElementRenderState::default()
            },
        );
        assert_eq!(rendered.plain_text(), "0");

        let value = input.borrow();
        let state = value.number_input();
        let edit = state.edit.borrow();
        assert!(edit.editing);
        assert_eq!(edit.edit_original_value, 0.0);
        assert_eq!(edit.edit_text, "0");
        assert_eq!(edit.edit_cursor, 1);
        assert!(edit.replace_on_first_text_input);
    }

    #[test]
    fn number_input_public_edit_text_keeps_utf8_cursor_on_scalar_boundaries()
    {
        let input = new_number_input("number", 0.0);
        input.set_edit_text("1界2");
        input.set_edit_cursor(2, false);
        {
            let value = input.borrow();
            let edit = value.number_input().edit.borrow();
            assert_eq!(edit.edit_cursor, 1);
        }

        assert!(input.handle_key("Right"));
        {
            let value = input.borrow();
            let edit = value.number_input().edit.borrow();
            assert_eq!(edit.edit_cursor, 4);
        }
        assert!(input.handle_key("Backspace"));
        {
            let value = input.borrow();
            let edit = value.number_input().edit.borrow();
            assert_eq!(edit.edit_text, "12");
            assert_eq!(edit.edit_cursor, 1);
        }

        input.set_edit_text("1界2");
        input.set_edit_cursor(4, false);
        let rendered = input.render(
            crate::Size { width: 6, height: 1 },
            ElementRenderState
            {
                edit_mode: true,
                ..ElementRenderState::default()
            },
        );
        assert_eq!(rendered.plain_text(), "1?2");
    }

    #[test]
    fn text_input_edits_unicode_by_character()
    {
        let input = new_text_input("name", "až", 0);
        assert_eq!(input.borrow().cursor(), "až".len());
        input.borrow_mut().handle_key("Left");
        input.borrow_mut().handle_key("x");
        assert_eq!(input.borrow().value(), "axž");
        assert!(!input.borrow_mut().handle_key("Delete"));
        assert_eq!(input.borrow().value(), "axž");

        let limited = new_text_input("limited", "a", 2);
        assert!(limited.borrow_mut().handle_key("ž"));
        assert_eq!(limited.borrow().value(), "a");
    }

    #[test]
    fn text_area_vertical_cursor_uses_the_cpp_wide_glyph_midpoint()
    {
        let area = new_text_area("description", "ab\n界x");
        area.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 4, height: 2 };
        area.borrow_mut().set_cursor(1);

        assert!(area.borrow_mut().handle_key("Down"));
        assert_eq!(area.borrow().cursor(), "ab\n界".len());
    }

    #[test]
    fn text_area_point_mapping_clamps_the_viewport_before_content_lookup()
    {
        let size = crate::Size { width: 2, height: 2 };
        let area = new_text_area("description", "ab\ncd\nef");
        assert_eq!(area.cursor_for_point(99, 0, size), 3);

        assert!(area.scroll_by_rows(1, size.height, true));
        assert_eq!(area.cursor_for_point(99, 0, size), 6);

        let short = new_text_area("short", "ab");
        assert_eq!(
            short.cursor_for_point(1, 0, crate::Size { width: 2, height: 3 }),
            2,
        );
    }

    #[test]
    fn text_controls_enforce_max_length_and_reference_option_movements()
    {
        let input = new_text_input("name", "abcdefghi", 9);
        assert!(input.borrow_mut().handle_key("Alt+Left"));
        assert_eq!(input.borrow().cursor(), 4);
        assert!(input.borrow_mut().handle_key("Alt+Right"));
        assert_eq!(input.borrow().cursor(), 9);
        assert!(input.borrow_mut().handle_key("x"));
        assert_eq!(input.borrow().value(), "abcdefghi");
        input.borrow_mut().select_range(7, 9);
        assert!(input.borrow_mut().handle_key("X"));
        assert_eq!(input.borrow().value(), "abcdefgX");

        let over_limit = new_text_input("over_limit", "abcd", 3);
        over_limit.select_range(1, 2);
        assert!(over_limit.handle_key("X"));
        assert_eq!(over_limit.value(), "aXcd");

        let area = new_text_area_with_max_length("description", "a\nb", 3);
        assert!(area.borrow_mut().handle_key("Enter"));
        assert_eq!(area.borrow().value(), "a\nb");
        area.borrow_mut().select_range(2, 3);
        assert!(area.borrow_mut().handle_key("Enter"));
        assert_eq!(area.borrow().value(), "a\n\n");
    }

    #[test]
    fn text_area_manual_scroll_is_shared_by_render_and_mouse_mapping()
    {
        let area = new_text_area("description", "zero\none\ntwo\nthree");
        {
            let mut value = area.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 8, height: 2 };
            value.set_cursor(0);
            assert!(value.scroll_text_rows(2, 2, true));
        }

        let rendered = area.borrow().render(8, 2, true, true);
        assert!(rendered.plain_text().starts_with("two"));
        assert_eq!(area.borrow().cursor_for_point(0, 0), 9);
        assert_eq!(
            area.borrow().text_input().expect("TextArea state").row_scroll_offset.get(),
            2,
        );
    }

    #[test]
    fn number_input_uses_its_step_and_own_edit_state_machine()
    {
        let input = new_number_input_with_step("amount", 2.0, 2.5);
        {
            let mut value = input.borrow_mut();
            value.begin_number_edit();
            assert!(value.handle_key("Up"));
            assert_eq!(value.number_value(), 4.5);
            value.cancel_number_edit();
            assert_eq!(value.number_value(), 2.0);
            value.set_number_value(0.0);
            value.begin_number_edit();
            assert!(
                value
                    .number_input()
                    .edit
                    .borrow()
                    .replace_on_first_text_input
            );
            assert!(value.handle_key("7"));
            value.commit_number_edit();
            assert_eq!(value.number_value(), 7.0);
        }
    }

    #[test]
    fn number_input_matches_cpp_defaultfloat_and_stod_prefix_contracts()
    {
        assert_eq!(format_number(1.234567890123456), "1.23456789012");
        assert_eq!(format_number(1_234_567_890_123.0), "1.23456789012e+12");
        assert_eq!(format_number(0.00001234567890123456), "1.23456789012e-05");
        assert_eq!(format_number(999_999_999_999.4), "999999999999");
        assert_eq!(format_number(999_999_999_999.6), "1e+12");
        assert_eq!(format_number(-0.0), "-0");

        let input = new_number_input("amount", 7.0);
        input.set_edit_text("1.5.9");
        input.commit_edit();
        assert_eq!(input.value(), 1.5);

        input.begin_edit();
        input.set_edit_text("not-a-number");
        input.commit_edit();
        assert_eq!(input.value(), 1.5);
    }

    #[test]
    fn single_line_controls_render_at_their_natural_height()
    {
        let button = new_button("show", "Show");
        let checkbox = new_checkbox("selected", "", false);
        let input = new_text_input("name", "Camera", 0);
        let combo = new_combo_box("mode", vec!["Normal".to_string()]);

        let controls: Vec<ElementRef> = vec![
            button.clone().into(),
            checkbox.into(),
            input.into(),
            combo.into(),
        ];
        for control in &controls
        {
            let rendered = control.borrow().render(12, 5, false, false);
            assert_eq!(rendered.height, 1);
        }
        assert!(button.borrow().render(12, 5, false, false).plain_text().contains("Show"));
    }

    #[test]
    fn text_area_expands_tabs_before_wrapping_and_rendering()
    {
        let input = new_text_area("description", "def hello(name):\n\tif name:\n\t\tprint(name)");
        let rendered = input.borrow().render(24, 3, false, false);
        let plain = rendered.plain_text();
        let rows: Vec<&str> = plain.lines().collect();
        assert_eq!(rows[0], "def hello(name):");
        assert_eq!(rows[1], "    if name:");
        assert_eq!(rows[2], "        print(name)");
    }

    #[test]
    fn text_area_mouse_point_uses_visual_rows_and_source_offsets()
    {
        let input = new_text_area("description", "abc\ndef");
        input.borrow_mut().frame = Rect { row: 4, col: 7, width: 8, height: 3 };
        input.borrow_mut().set_cursor(7);

        assert_eq!(input.borrow().cursor_for_point(1, 1), 5);
    }

    #[test]
    fn text_area_shift_horizontal_selects_renders_and_replaces_multiple_characters()
    {
        let input = new_text_area("description", "abcdef\nghij");
        {
            let mut value = input.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 8, height: 2 };
            value.set_cursor_style(Style
            {
                background: Some(Color::new("#2563eb")),
                ..Style::default()
            });
            value.set_cursor(6);
            assert!(value.handle_key("Shift+Left"));
            assert!(value.handle_key("Shift+Left"));
            assert_eq!(value.selected_text(), "ef");
        }

        let rendered = input.borrow().render(8, 2, true, true);
        assert_eq!(
            rendered.cell(0, 4).map(|cell| cell.background.normalized()),
            Some("#2563eb".to_string()),
        );
        assert_eq!(
            rendered.cell(0, 5).map(|cell| cell.background.normalized()),
            Some("#2563eb".to_string()),
        );

        assert!(input.borrow_mut().handle_key("X"));
        assert_eq!(input.borrow().value(), "abcdX\nghij");
        assert!(input.borrow().selected_text().is_empty());
    }

    #[test]
    fn selectable_label_maps_aligned_mouse_points_and_renders_its_selection()
    {
        let label = new_label("copy", "abcdef");
        {
            let mut value = label.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 8, height: 1 };
            value.style.text_align = Some("center".to_string());
            value.style.user_select = Some("text".to_string());
            value.set_cursor_style(Style
            {
                color: Some(Color::new("#ffffff")),
                background: Some(Color::new("#2563eb")),
                ..Style::default()
            });
            assert_eq!(value.cursor_for_point(0, 2), 1);
            value.select_range(1, 4);
        }

        let rendered = label.borrow().render(8, 1, false, false);
        assert_eq!(
            rendered.cell(0, 2).map(|cell| cell.background.normalized()),
            Some("#2563eb".to_string()),
        );
        assert_eq!(label.borrow().selected_text(), "bcd");
        label.borrow_mut().clear_selection();
        assert!(label.borrow().selected_text().is_empty());
    }

    #[test]
    fn selectable_label_point_mapping_matches_cpp_visual_rows_and_wide_glyphs()
    {
        let wide = new_label("wide", "界x");
        {
            let mut value = wide.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 3, height: 1 };
            assert_eq!(value.cursor_for_point(0, 0), 0);
            assert_eq!(value.cursor_for_point(0, 1), "界".len());
        }

        let one_row = new_label("one_row", "ab cd");
        {
            let mut value = one_row.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 4, height: 1 };
            value.style.text_align = Some("center".to_string());
            assert_eq!(value.cursor_for_point(0, 1), 1);
        }

        let multiline = new_label("multiline", "ab\n界x");
        {
            let mut value = multiline.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 3, height: 2 };
            assert_eq!(value.cursor_for_point(-1, 0), 0);
            assert_eq!(value.cursor_for_point(1, 1), "ab\n界".len());
            assert_eq!(value.cursor_for_point(2, 0), "ab\n界x".len());
        }
    }

    #[test]
    fn label_content_setters_preserve_cpp_selection_state()
    {
        let label = new_label("copy", "abcdef");
        label.select_range(2, 6);
        label.set_text("abc");
        assert!(label.has_selection());
        assert_eq!(label.selected_text(), "c");

        label.set_text("a");
        assert!(label.has_selection());
        assert_eq!(label.selected_text(), "");

        label.set_spans(vec![
            LabelSpan
            {
                text: "xy".to_string(),
                ..LabelSpan::default()
            },
            LabelSpan
            {
                text: "z".to_string(),
                ..LabelSpan::default()
            },
        ]);
        assert!(label.has_selection());
        assert_eq!(label.selected_text(), "z");
    }

    #[test]
    fn message_table_renders_markdown_with_reference_box_glyphs()
    {
        let table = new_message_table("table");
        table
            .borrow_mut()
            .set_text("| Name | Value |\n| :--- | ---: |\n| One | 1 |");
        let rendered = table.borrow().render(24, 5, false, false);
        let plain = rendered.plain_text();
        let rows: Vec<&str> = plain.lines().collect();

        assert!(rows[0].starts_with('┌'));
        assert!(rows[0].contains('┬'));
        assert!(rows[2].starts_with('├'));
        assert!(rows[4].starts_with('└'));
        assert_eq!(message_table_fit_content_height(table.borrow().text()), 5);
    }

    #[test]
    fn message_table_keeps_its_natural_right_border_when_the_requested_width_is_smaller()
    {
        let table = new_message_table("table");
        table.borrow_mut().set_text(
            "| Element | Runtime role | Interactive |\n\
             | --- | --- | ---: |\n\
             | Label | Static selectable text | no |",
        );
        let rendered = table.borrow().render(1, 5, false, false);
        assert!(rendered.width > 1);
        assert_eq!(
            rendered.cell(0, rendered.width - 1).map(|cell| cell.text.as_str()),
            Some("┐"),
        );
    }

    #[test]
    fn info_label_applies_the_reference_default_gradient()
    {
        let label = new_info_label("info", "Glow\nNext");
        label.borrow_mut().set_style(Style
        {
            color: Some(Color::new("#111111")),
            ..Style::default()
        });
        let rendered = with_render_time_override(Some(0), ||
        {
            label.borrow().render(8, 2, false, false)
        });

        assert_eq!(rendered.plain_text().lines().next(), Some("Glow"));
        assert_eq!(
            rendered.cell(0, 0).map(|cell| cell.foreground.normalized()),
            Some("#707070".to_string()),
        );
        assert_eq!(
            rendered.cell(0, 1).map(|cell| cell.foreground.normalized()),
            Some("#909090".to_string()),
        );
    }

    #[test]
    fn frame_buffer_view_matches_pan_dim_and_interaction_corner_contract()
    {
        let view = new_frame_buffer_view("frame");
        let cells = |text: &str| FrameBufferCell
        {
            text: text.to_string(),
            foreground: "#ffffff".to_string(),
            background: "#102030".to_string(),
        };
        let mut frames = BTreeMap::new();
        frames.insert(
            "app".to_string(),
            vec![
                vec![cells("A"), cells("B"), cells("C")],
                vec![cells("D"), cells("E"), cells("F")],
            ],
        );
        {
            let mut value = view.borrow_mut();
            value.set_style(Style
            {
                color: Some(Color::new("#123456")),
                ..Style::default()
            });
            value.set_frames(frames, vec!["app".to_string()], BTreeMap::new());
            value.set_pan(0, 1);
            value.set_dimmed(true);
        }

        let rendered = view.borrow().render(2, 2, false, false);
        assert_eq!(rendered.plain_text(), "BC\nEF");
        assert_eq!(
            rendered.cell(0, 0).map(|cell| cell.foreground.normalized()),
            Some("#999999".to_string()),
        );
        assert_eq!(
            rendered.cell(0, 0).map(|cell| cell.background.normalized()),
            Some("#0a131d".to_string()),
        );

        view.borrow_mut().set_interaction_state(true, false);
        let focused = view.borrow().render(2, 2, false, false);
        assert_eq!(
            focused.cell(0, 0).map(|cell| cell.text.as_str()),
            Some("─"),
        );
        assert_eq!(
            focused.cell(0, 0).map(|cell| cell.foreground.normalized()),
            Some("#123456".to_string()),
        );
    }

    #[test]
    fn view_host_aliases_child_ownership_and_scroll_view_passive_focus()
    {
        let host = new_view_host("main");
        host.borrow_mut().set_view(crate::GeneratedWindow::new("Child"));
        assert_eq!(
            host.borrow().current_view().map(|window| window.title.as_str()),
            Some("Child"),
        );
        host.borrow_mut().clear_view();
        assert!(host.borrow().current_view().is_none());

        let scroll = new_scroll_view("items", 0);
        let child = new_label("row", "A");
        {
            let mut value = child.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 1, height: 1 };
            value.set_style(Style
            {
                background: Some(Color::new("#111111")),
                ..Style::default()
            });
        }
        {
            let mut value = scroll.borrow_mut();
            value.set_style(Style
            {
                background: Some(Color::new("#111111")),
                ..Style::default()
            });
            value.set_focus_style(Style
            {
                background: Some(Color::new("#ffffff80")),
                ..Style::default()
            });
            value.add_child(child);
        }
        let rendered = scroll.borrow().render_with_state(
            1,
            1,
            ElementRenderState
            {
                focused: true,
                passive_focus: true,
                ..ElementRenderState::default()
            },
        );
        assert_eq!(
            rendered.cell(0, 0).map(|cell| cell.background.normalized()),
            Some("#c4c4c4".to_string()),
        );
    }

    #[test]
    fn child_parent_and_liveness_follow_reference_ownership()
    {
        let parent = new_scroll_view("parent", 0);
        let child = new_button("child", "Run");
        parent.borrow_mut().add_child(child.clone());

        assert!(is_live_element(&child));
        assert!(child
            .borrow()
            .parent()
            .is_some_and(|owner| Rc::ptr_eq(&owner, &parent)));

        let second_parent = new_scroll_view("second_parent", 0);
        assert!(std::panic::catch_unwind(std::panic::AssertUnwindSafe(||
        {
            second_parent.borrow_mut().add_child(child.clone());
        }))
        .is_err());

        parent.borrow_mut().clear_children();

        assert!(!is_live_element(&child));
        assert!(child.borrow().parent().is_none());
        assert!(std::panic::catch_unwind(std::panic::AssertUnwindSafe(||
        {
            parent.borrow_mut().add_child(child.clone());
        }))
        .is_err());
    }

    #[test]
    fn typed_control_refs_forward_the_complete_cpp_base_element_surface()
    {
        let parent = new_scroll_view("parent", 0);
        let child = new_button("child", "Run");
        child.set_style(Style
        {
            background: Some(Color::new("#101010")),
            ..Style::default()
        });
        child.set_focus_style(Style
        {
            background: Some(Color::new("#202020")),
            ..Style::default()
        });
        child.set_edit_style(Style { color: Some(Color::new("#303030")), ..Style::default() });
        child.set_cursor_style(Style { color: Some(Color::new("#404040")), ..Style::default() });
        child.set_selected_style(Style { color: Some(Color::new("#505050")), ..Style::default() });
        child.set_active_style(Style { color: Some(Color::new("#606060")), ..Style::default() });
        child.set_checked_style(Style { color: Some(Color::new("#707070")), ..Style::default() });
        child.set_unchecked_style(Style { color: Some(Color::new("#808080")), ..Style::default() });
        child.set_disabled_style(Style { color: Some(Color::new("#909090")), ..Style::default() });

        assert_eq!(child.focus_style().map(|style| style.background.normalized()),
                   Some("#202020".to_string()));
        assert!(child.edit_style().is_some());
        assert!(child.cursor_style().is_some());
        assert!(child.selected_style().is_some());
        assert!(child.active_style().is_some());
        assert!(child.checked_style().is_some());
        assert!(child.unchecked_style().is_some());
        assert!(child.disabled_style().is_some());
        assert_eq!(
            child
                .effective_style(ElementRenderState
                {
                    focused: true,
                    ..ElementRenderState::default()
                })
                .background
                .normalized(),
            "#202020",
        );

        parent.add_child(child.clone());
        assert!(child
            .parent()
            .is_some_and(|owner| Rc::ptr_eq(&owner, &parent.clone().into_element())));
        assert_eq!(parent.children().len(), 1);
        parent.clear_child_elements();
        assert!(parent.children().is_empty());
        assert!(child.parent().is_none());
    }

    #[test]
    fn base_and_reusable_focusable_state_matches_cpp_property_lifecycle()
    {
        assert!(new_label("label", "Label").focusable());
        assert!(new_image("image", "", "", "contain", "auto", "center", "middle").focusable());

        let reusable = new_reusable_element("child", "panel");
        assert!(reusable.focusable());
        reusable.set_focusable(false);
        reusable.set_child(crate::GeneratedWindow::new_scroll_view("Rows"));
        assert!(!reusable.focusable());

        let host = new_view_host("host");
        assert!(host.focusable());
        host.set_focusable(false);
        host.set_view(crate::GeneratedWindow::new("Panel"));
        host.clear_view();
        assert!(!host.focusable());
    }

    #[test]
    #[should_panic(expected = "ButtonRef requires an element with kind Button")]
    fn typed_element_refs_reject_a_different_concrete_control_kind()
    {
        let _: ButtonRef = new_label("label", "Label").into_element().into();
    }

    #[test]
    fn public_control_and_typed_scroll_view_api_match_reference_roles()
    {
        let button = new_button("run", "Run");
        let control: Control = button.into();
        assert_eq!(control.borrow().name(), "run");
        assert!(control.borrow().focusable());

        let scroll = new_scroll_view("items", 1);
        scroll.borrow_mut().set_frame(
            Rect { row: 0, col: 0, width: 6, height: 3 },
        );
        let child = new_label("row", "Row");
        child.borrow_mut().set_frame(
            Rect { row: 0, col: 0, width: 6, height: 2 },
        );
        scroll.add_child(child);

        assert_eq!(scroll.gap(), 1);
        assert_eq!(scroll.cached_child_heights(), [2]);
        assert_eq!(scroll.content_height(), 2);
        assert_eq!(scroll.content_height_for_width(6), 2);
        assert!(scroll
            .content_window(crate::Size { width: 6, height: 3 })
            .is_some());
    }

    #[test]
    fn scroll_view_empty_content_and_clear_delta_match_reference_lifecycle()
    {
        let scroll = new_scroll_view("items", 0);
        scroll.borrow_mut().set_frame(
            Rect { row: 0, col: 0, width: 4, height: 1 },
        );
        assert_eq!(
            scroll
                .content_window(crate::Size { width: 4, height: 1 }),
            Some(ScrollViewContentWindow
            {
                viewport: crate::Size { width: 4, height: 1 },
                actual_skip: 0,
                natural_skip: 0,
                gap: 0,
            }),
        );

        let child = new_label("row", "one\ntwo\nthree");
        child.borrow_mut().set_frame(
            Rect { row: 0, col: 0, width: 4, height: 3 },
        );
        scroll.add_child(child);
        let viewport = crate::Size { width: 4, height: 1 };
        assert!(scroll.scroll_lines(-1, viewport));
        scroll.clear_children();
        assert_eq!(scroll.consume_terminal_scroll_delta(), -1);
        assert_eq!(scroll.consume_terminal_scroll_delta(), 0);
    }

    #[test]
    fn scroll_view_set_gap_always_invalidates_the_reference_height_cache()
    {
        let scroll = new_scroll_view("items", 1);
        scroll
            .borrow()
            .add_rendered_child(TerminalBuffer::new(4, 2));
        assert_eq!(scroll.borrow().content_height(4), 2);
        {
            let value = scroll.borrow();
            let state = value.scroll_view().expect("ScrollView state");
            assert_eq!(state.cached_child_heights_width.get(), Some(4));
            assert!(state.cached_child_heights.borrow().is_some());
        }

        scroll.set_gap(1);

        let value = scroll.borrow();
        let state = value.scroll_view().expect("ScrollView state");
        assert_eq!(state.cached_child_heights_width.get(), None);
        assert!(state.cached_child_heights.borrow().is_none());
    }

    #[test]
    fn scroll_view_restore_refreshes_raw_then_padded_width_like_cpp()
    {
        let widths = Rc::new(RefCell::new(Vec::new()));
        let observed = widths.clone();
        let scroll = new_scroll_view("items", 0);
        {
            let mut value = scroll.borrow_mut();
            value.set_frame(Rect { row: 0, col: 0, width: 10, height: 4 });
            value.set_style(Style
            {
                padding_left: Some(1),
                padding_right: Some(1),
                ..Style::default()
            });
            value.set_dynamic_children_renderer(move |width|
            {
                observed.borrow_mut().push(width);
                vec![TerminalBuffer::new(width, 2)]
            });
        }

        scroll.restore_scroll_position(ScrollViewPosition::default());

        assert_eq!(*widths.borrow(), [10, 8]);
    }

    #[test]
    fn scroll_view_owns_the_reference_offsets_sources_cache_and_terminal_delta()
    {
        let scroll = new_scroll_view("items", 1);
        scroll
            .borrow()
            .add_rendered_child(TerminalBuffer::new(6, 2));
        let first = new_label("first", "First");
        let second = new_label("second", "Second");
        first.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 3 };
        second.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 3 };
        {
            let mut value = scroll.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 6, height: 3 };
            value.add_child(first.clone());
            value.add_child(second.clone());
        }

        let viewport = crate::Size { width: 6, height: 3 };
        assert_eq!(scroll.borrow().content_height(6), 10);
        assert_eq!(scroll.borrow().max_view_offset(viewport), 7);
        assert_eq!(scroll.borrow().view_offset(), 7);
        assert!(!scroll.borrow().scroll_to_top(viewport));
        assert!(scroll.borrow().scroll_lines(-2, viewport));
        assert_eq!(scroll.borrow().view_offset(), 5);
        assert_eq!(scroll.borrow().consume_terminal_scroll_delta(), -2);
        assert_eq!(scroll.borrow().consume_terminal_scroll_delta(), 0);

        let views = scroll.borrow().child_views(viewport);
        assert_eq!(views.len(), 3);
        assert!(views[0].element.is_none());
        assert!(views[1]
            .element
            .as_ref()
            .is_some_and(|element| Rc::ptr_eq(element, &first)));
        assert!(views[1].visible);
        assert!(views[1].clipped);

        assert!(scroll.borrow().ensure_child_visible(&second, viewport));
        assert_eq!(scroll.borrow().view_offset(), 0);
        assert!(scroll.borrow().auto_scroll());

        let position = ScrollViewPosition
        {
            scroll_offset: 0,
            view_offset: 4,
            auto_scroll: false,
        };
        scroll.borrow().restore_scroll_position(position);
        assert_eq!(scroll.borrow().scroll_position(), position);
        assert!(scroll.borrow().scroll_horizontal(2));
        assert_eq!(scroll.borrow().h_view_offset(), 2);
    }

    #[test]
    fn scroll_view_base_render_reserves_native_rows_for_runtime_overlay()
    {
        let scroll = new_scroll_view("items", 0);
        let native = new_label("native", "Native");
        native.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 1 };
        scroll.borrow_mut().add_child(native);

        let rendered = scroll.borrow().render(6, 1, false, false);

        assert_eq!(rendered.plain_text(), "");
        assert!((0..6).all(|col|
        {
            rendered.cell(0, col).is_some_and(|cell| cell.text == " ")
        }));
        let views = scroll
            .borrow()
            .child_views(crate::Size { width: 6, height: 1 });
        assert_eq!(views.len(), 1);
        assert!(views[0].visible);
        assert!(views[0].element.is_some());
    }

    #[test]
    fn scroll_view_dynamic_children_refresh_only_for_a_new_width()
    {
        let render_count = Rc::new(Cell::new(0));
        let scroll = new_scroll_view("items", 0);
        {
            let render_count = render_count.clone();
            scroll
                .borrow_mut()
                .set_dynamic_children_renderer(move |width|
                {
                    render_count.set(render_count.get() + 1);
                    vec![TerminalBuffer::new(width, 2)]
                });
        }

        assert_eq!(scroll.borrow().content_height(6), 2);
        assert_eq!(scroll.borrow().content_height(6), 2);
        assert_eq!(render_count.get(), 1);
        assert_eq!(scroll.borrow().content_height(7), 2);
        assert_eq!(render_count.get(), 2);
        scroll.borrow().invalidate_dynamic_children();
        assert_eq!(scroll.borrow().content_height(7), 2);
        assert_eq!(render_count.get(), 3);
    }

    #[test]
    fn element_parent_background_blends_only_during_cpp_state_style_merge()
    {
        let label = new_label("status", "Ready");
        label.set_style(Style
        {
            background: Some(Color::new("#ff000080")),
            ..Style::default()
        });

        let base_only = with_element_parent_background(
            Some(Color::new("#0000ff")),
            || label.borrow().effective_style(ElementRenderState::default()),
        );
        assert_eq!(
            base_only.background.rgba(),
            Some(crate::Rgba { red: 255, green: 0, blue: 0, alpha: 128 }),
        );

        label.borrow_mut().set_focus_style(Style::default());
        let focused = with_element_parent_background(
            Some(Color::new("#0000ff")),
            || label.borrow().effective_style(ElementRenderState
            {
                focused: true,
                ..ElementRenderState::default()
            }),
        );
        assert_eq!(focused.background.normalized(), "#80007f");

        let named_parent = with_element_parent_background(
            Some(Color::named("blue")),
            || label.borrow().effective_style(ElementRenderState
            {
                focused: true,
                ..ElementRenderState::default()
            }),
        );
        assert_eq!(
            named_parent.background.rgba(),
            Some(crate::Rgba { red: 255, green: 0, blue: 0, alpha: 128 }),
        );

        label.set_enabled(false);
        label.borrow_mut().set_disabled_style(Style
        {
            background: Some(Color::new("#00ff00")),
            ..Style::default()
        });
        let disabled = with_element_parent_background(
            Some(Color::new("#0000ff")),
            || label.borrow().effective_style(ElementRenderState
            {
                focused: true,
                edit_mode: true,
                ..ElementRenderState::default()
            }),
        );
        assert_eq!(disabled.background.normalized(), "#00ff00");
    }

    #[test]
    fn disabled_element_without_disabled_style_continues_cpp_state_precedence()
    {
        let label = new_label("status", "Ready");
        label.set_style(Style
        {
            background: Some(Color::new("#101010")),
            ..Style::default()
        });
        label.set_focus_style(Style
        {
            background: Some(Color::new("#202020")),
            ..Style::default()
        });
        label.set_edit_style(Style
        {
            background: Some(Color::new("#303030")),
            ..Style::default()
        });
        label.set_enabled(false);

        assert_eq!(
            label
                .effective_style(ElementRenderState
                {
                    focused: true,
                    ..ElementRenderState::default()
                })
                .background,
            Some(Color::new("#202020")),
        );
        assert_eq!(
            label
                .effective_style(ElementRenderState
                {
                    focused: true,
                    edit_mode: true,
                    ..ElementRenderState::default()
                })
                .background,
            Some(Color::new("#303030")),
        );

        label.set_disabled_style(Style
        {
            background: Some(Color::new("#404040")),
            ..Style::default()
        });
        assert_eq!(
            label
                .effective_style(ElementRenderState
                {
                    focused: true,
                    edit_mode: true,
                    ..ElementRenderState::default()
                })
                .background,
            Some(Color::new("#404040")),
        );
    }

    #[test]
    fn render_scopes_restore_previous_state_after_unwind()
    {
        assert_eq!(FrameBufferCell::default().text, " ");
        let result = std::panic::catch_unwind(||
        {
            let _time = ScopedRenderTimeOverride::new(Some(42));
            let _background =
                ScopedElementParentBackground::new(Some(Color::new("#123456")));
            assert_eq!(
                RENDER_TIME_OVERRIDE_MS.with(|current| *current.borrow()),
                Some(42),
            );
            assert_eq!(
                Element::rendering_parent_background()
                    .map(|color| color.normalized()),
                Some("#123456".to_string()),
            );
            panic!("exercise scope cleanup");
        });
        assert!(result.is_err());
        assert_eq!(
            RENDER_TIME_OVERRIDE_MS.with(|current| *current.borrow()),
            None,
        );
        assert_eq!(Element::rendering_parent_background(), None);
    }
}

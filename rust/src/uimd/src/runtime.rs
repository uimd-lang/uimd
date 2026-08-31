#![allow(deprecated)]

use crate::{
    Color, ElementKind, ElementRef, ElementRenderState, LayoutItem, Point, Rect, Size, Style,
    ScrollViewPosition, ScrollViewRef, TerminalBuffer, DEFAULT_VIEWPORT_HEIGHT,
    DEFAULT_VIEWPORT_WIDTH, EXPANDED, FIT_CONTENT,
};
use crate::core::OptionalColorExt;
use serde_json::Value;
use std::cmp::{max, min};
use std::collections::HashMap;
use std::io::{self, IsTerminal};
use std::ops::{Deref, DerefMut};
use std::panic::{catch_unwind, AssertUnwindSafe};
use std::rc::Rc;
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{Duration, Instant};
use unicode_width::UnicodeWidthStr;

static NEXT_GENERATED_WINDOW_IDENTITY: AtomicU64 = AtomicU64::new(1);
const COPY_NOTIFICATION_DURATION: Duration = Duration::from_secs(3);
pub(crate) const DIALOG_BUTTON_CLOSE_DURATION: Duration = Duration::from_millis(180);

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct KeyEvent
{
    pub key: String,
    pub focused_element_id: String,
    pub edit_mode: bool,
}

pub trait GeneratedApplication
{
    fn window(&self) -> &GeneratedWindow;
    fn window_mut(&mut self) -> &mut GeneratedWindow;

    fn active_window(&self) -> &GeneratedWindow { self.window().active_window() }
    fn active_window_mut(&mut self) -> &mut GeneratedWindow { self.window_mut().active_window_mut() }
    fn handle_active_window_button(&mut self, _name: &str) -> bool { false }
    fn handle_generated_button(&mut self, _name: &str) -> bool { false }
    fn handle_generated_text_changed(&mut self, _name: &str, _value: &str) -> bool { false }
    fn handle_generated_text_confirmed(&mut self, _name: &str, _value: &str) -> bool { false }
    fn handle_generated_selection_changed(&mut self, _name: &str, _values: &[String]) -> bool { false }
    fn handle_generated_listbox_item_activate(
        &mut self,
        _name: &str,
        _element_id: &str,
        _index: usize,
        _value: &str,
    ) -> bool { false }
    fn handle_focus_changed(&mut self, _name: &str, _focused: bool) -> bool { false }
    fn handle_preview_key(&mut self, _event: &KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _key: &str) -> bool { false }
    fn handle_generated_window_closed(&mut self, _window: GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _name: &str, _arguments: &serde_json::Map<String, Value>) -> Option<Value> { None }
}

pub type ButtonCallback = Rc<dyn Fn(&str)>;
pub type KeyBeforeFocusedElementCallback = Rc<dyn Fn(&str, &str, bool) -> bool>;
pub type KeyCallback = Rc<dyn Fn(&str) -> bool>;
pub type MousePressCallback = Rc<dyn Fn(Point) -> bool>;
pub type MouseWheelBeforeFocusedCallback = Rc<dyn Fn(Point, i32) -> bool>;
pub type MouseWheelCallback = Rc<dyn Fn(&str, i32) -> bool>;
pub type TextCallback = Rc<dyn Fn(&str, &str)>;
pub type SelectionCallback = Rc<dyn Fn(&str, &[String])>;
pub type EditStartedCallback = Rc<dyn Fn(&str)>;
pub type FocusChangedCallback = Rc<dyn Fn(&str, bool)>;
pub type RenderOverlayCallback = Rc<dyn Fn(&mut TerminalBuffer)>;
pub type BoolCallback = Rc<dyn Fn() -> bool>;

pub trait GeneratedWindowBehavior
{
    fn handle_button(&mut self, _window: &mut GeneratedWindow, _name: &str) -> bool
    {
        false
    }

    fn handle_preview_key(&mut self, _window: &mut GeneratedWindow, _event: &KeyEvent) -> bool
    {
        false
    }

    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(
        &mut self,
        _window: &mut GeneratedWindow,
        _key: &str,
        _name: &str,
        _edit_mode: bool,
    ) -> bool
    {
        false
    }

    fn handle_key(&mut self, _window: &mut GeneratedWindow, _key: &str) -> bool
    {
        false
    }

    fn handle_listbox_item_activate(
        &mut self,
        _window: &mut GeneratedWindow,
        _name: &str,
        _element_id: &str,
        _index: usize,
        _value: &str,
    ) -> bool
    {
        false
    }

    fn handle_mouse_press(&mut self, _window: &mut GeneratedWindow, _point: Point) -> bool
    {
        false
    }

    fn handle_mouse_wheel(
        &mut self,
        _window: &mut GeneratedWindow,
        _point: Point,
        _delta: i32,
    ) -> bool
    {
        false
    }

    fn handle_text_changed(
        &mut self,
        _window: &mut GeneratedWindow,
        _name: &str,
        _value: &str,
    ) -> bool
    {
        false
    }

    fn handle_text_confirmed(
        &mut self,
        _window: &mut GeneratedWindow,
        _name: &str,
        _value: &str,
    ) -> bool
    {
        false
    }

    fn handle_selection_changed(
        &mut self,
        _window: &mut GeneratedWindow,
        _name: &str,
        _values: &[String],
    ) -> bool
    {
        false
    }

    fn handle_edit_started(&mut self, _window: &mut GeneratedWindow, _name: &str) {}

    fn handle_focus_changed(
        &mut self,
        _window: &mut GeneratedWindow,
        _name: &str,
        _focused: bool,
    )
    {
    }

    fn should_close(&self) -> bool
    {
        false
    }
}

#[derive(Clone)]
pub struct GeneratedWindowBehaviorRef(Rc<std::cell::RefCell<Box<dyn GeneratedWindowBehavior>>>);

impl GeneratedWindowBehaviorRef
{
    pub fn new(behavior: impl GeneratedWindowBehavior + 'static) -> Self
    {
        Self(Rc::new(std::cell::RefCell::new(Box::new(behavior))))
    }
}

impl std::fmt::Debug for GeneratedWindowBehaviorRef
{
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        formatter.write_str("GeneratedWindowBehaviorRef")
    }
}

#[derive(Clone, Default)]
pub struct GeneratedWindowRuntimeOptions
{
    pub footer: String,
    pub initial_focus_name: String,
    pub start_in_edit_mode: bool,
    pub keep_edit_mode_after_confirm: bool,
    pub keep_edit_mode_after_escape: bool,
    pub on_button: Option<ButtonCallback>,
    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    pub on_key_before_focused_element: Option<KeyBeforeFocusedElementCallback>,
    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    pub on_key_before_focused: Option<KeyCallback>,
    pub on_key: Option<KeyCallback>,
    pub on_mouse_press_before_focused: Option<MousePressCallback>,
    pub on_mouse_wheel_before_focused: Option<MouseWheelBeforeFocusedCallback>,
    pub on_mouse_wheel: Option<MouseWheelCallback>,
    pub on_text_changed: Option<TextCallback>,
    pub on_text_confirmed: Option<TextCallback>,
    pub on_selection_changed: Option<SelectionCallback>,
    pub on_edit_started: Option<EditStartedCallback>,
    pub on_focus_changed: Option<FocusChangedCallback>,
    pub on_render_overlay: Option<RenderOverlayCallback>,
    pub should_dim_overlay_background: Option<BoolCallback>,
    pub should_close: Option<BoolCallback>,
}

impl std::fmt::Debug for GeneratedWindowRuntimeOptions
{
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        formatter
            .debug_struct("GeneratedWindowRuntimeOptions")
            .field("footer", &self.footer)
            .field("initial_focus_name", &self.initial_focus_name)
            .field("start_in_edit_mode", &self.start_in_edit_mode)
            .field("keep_edit_mode_after_confirm", &self.keep_edit_mode_after_confirm)
            .field("keep_edit_mode_after_escape", &self.keep_edit_mode_after_escape)
            .finish_non_exhaustive()
    }
}

#[derive(Clone, Debug, Default)]
pub struct McpMetadata
{
    pub enabled: bool,
    pub class_name: String,
    pub source: String,
    pub description: String,
}

#[derive(Clone, Debug, Default)]
pub struct GeneratedElementMetadata
{
    pub name: String,
    pub description: String,
    pub expose: bool,
}

impl GeneratedElementMetadata
{
    pub fn new(
        name: impl Into<String>,
        description: impl Into<String>,
        expose: bool,
    ) -> Self
    {
        Self
        {
            name: name.into(),
            description: description.into(),
            expose,
        }
    }
}

#[derive(Clone, Debug)]
pub struct GeneratedAppToolMetadata
{
    pub name: String,
    pub description: String,
    pub input_schema: Value,
    pub output_schema: Option<Value>,
}

impl GeneratedAppToolMetadata
{
    pub fn from_json(
        name: impl Into<String>,
        description: impl Into<String>,
        input_schema_json: &str,
        output_schema_json: &str,
    ) -> Self
    {
        Self
        {
            name: name.into(),
            description: description.into(),
            input_schema: serde_json::from_str(input_schema_json)
                .unwrap_or_else(|_| serde_json::json!({"type": "object", "properties": {}})),
            output_schema: (!output_schema_json.is_empty())
                .then(|| serde_json::from_str(output_schema_json).unwrap_or(Value::Null)),
        }
    }
}

#[derive(Clone, Debug)]
pub struct Window
{
    pub title: String,
    pub elements: Vec<ElementRef>,
}

impl Window
{
    pub fn new(title: impl Into<String>) -> Self
    {
        Self { title: title.into(), elements: Vec::new() }
    }

    pub fn add_element<T: Into<ElementRef>>(&mut self, element: T) -> ElementRef
    {
        let element = element.into();
        self.elements.push(element.clone());
        element
    }

    pub fn title(&self) -> &str
    {
        &self.title
    }

    pub fn elements(&self) -> &[ElementRef]
    {
        &self.elements
    }
}

#[derive(Clone, Debug, Default)]
pub struct Application
{
    windows: Vec<Window>,
}

impl Application
{
    pub fn open(&mut self, window: Window) -> &mut Window
    {
        self.windows.push(window);
        self.windows
            .last_mut()
            .expect("a just-opened window must be active")
    }

    pub fn active_window(&self) -> Option<&Window>
    {
        self.windows.last()
    }

    pub fn active_window_mut(&mut self) -> Option<&mut Window>
    {
        self.windows.last_mut()
    }

    pub fn windows(&self) -> &[Window]
    {
        &self.windows
    }
}

#[derive(Clone)]
pub struct GeneratedWindowFrameOptions
{
    pub class_name: String,
    pub initial_focus_name: String,
    pub start_in_edit_mode: bool,
    pub keep_edit_mode_after_confirm: bool,
    pub keep_edit_mode_after_escape: bool,
    pub dim_background: bool,
    pub on_button: Option<ButtonCallback>,
    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    pub on_key_before_focused_element: Option<KeyBeforeFocusedElementCallback>,
    #[deprecated(since = "0.5.4", note = "use handle_preview_key; removal in UIMD 0.7.0")]
    pub on_key_before_focused: Option<KeyCallback>,
    pub on_key: Option<KeyCallback>,
    pub on_mouse_press_before_focused: Option<MousePressCallback>,
    pub on_mouse_wheel_before_focused: Option<MouseWheelBeforeFocusedCallback>,
    pub on_text_changed: Option<TextCallback>,
    pub on_text_confirmed: Option<TextCallback>,
    pub on_selection_changed: Option<SelectionCallback>,
    pub on_edit_started: Option<EditStartedCallback>,
    pub on_focus_changed: Option<FocusChangedCallback>,
    pub should_close: Option<BoolCallback>,
}

impl Default for GeneratedWindowFrameOptions
{
    fn default() -> Self
    {
        Self
        {
            class_name: String::new(),
            initial_focus_name: String::new(),
            start_in_edit_mode: false,
            keep_edit_mode_after_confirm: false,
            keep_edit_mode_after_escape: false,
            dim_background: true,
            on_button: None,
            on_key_before_focused_element: None,
            on_key_before_focused: None,
            on_key: None,
            on_mouse_press_before_focused: None,
            on_mouse_wheel_before_focused: None,
            on_text_changed: None,
            on_text_confirmed: None,
            on_selection_changed: None,
            on_edit_started: None,
            on_focus_changed: None,
            should_close: None,
        }
    }
}

impl std::fmt::Debug for GeneratedWindowFrameOptions
{
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result
    {
        formatter
            .debug_struct("GeneratedWindowFrameOptions")
            .field("class_name", &self.class_name)
            .field("initial_focus_name", &self.initial_focus_name)
            .field("start_in_edit_mode", &self.start_in_edit_mode)
            .field("keep_edit_mode_after_confirm", &self.keep_edit_mode_after_confirm)
            .field("keep_edit_mode_after_escape", &self.keep_edit_mode_after_escape)
            .field("dim_background", &self.dim_background)
            .finish_non_exhaustive()
    }
}

impl GeneratedWindowFrameOptions
{
    pub fn from_window(window: &GeneratedWindow) -> Self
    {
        let runtime = window.runtime_options();
        Self
        {
            class_name: window.metadata.class_name.clone(),
            initial_focus_name: runtime.initial_focus_name,
            start_in_edit_mode: runtime.start_in_edit_mode,
            keep_edit_mode_after_confirm: runtime.keep_edit_mode_after_confirm,
            keep_edit_mode_after_escape: runtime.keep_edit_mode_after_escape,
            dim_background: true,
            on_button: runtime.on_button,
            on_key_before_focused_element: runtime.on_key_before_focused_element,
            on_key_before_focused: runtime.on_key_before_focused,
            on_key: runtime.on_key,
            on_mouse_press_before_focused: runtime.on_mouse_press_before_focused,
            on_mouse_wheel_before_focused: runtime.on_mouse_wheel_before_focused,
            on_text_changed: runtime.on_text_changed,
            on_text_confirmed: runtime.on_text_confirmed,
            on_selection_changed: runtime.on_selection_changed,
            on_edit_started: runtime.on_edit_started,
            on_focus_changed: runtime.on_focus_changed,
            should_close: runtime.should_close,
        }
    }

    fn runtime_options(&self) -> GeneratedWindowRuntimeOptions
    {
        GeneratedWindowRuntimeOptions
        {
            initial_focus_name: self.initial_focus_name.clone(),
            start_in_edit_mode: self.start_in_edit_mode,
            keep_edit_mode_after_confirm: self.keep_edit_mode_after_confirm,
            keep_edit_mode_after_escape: self.keep_edit_mode_after_escape,
            on_button: self.on_button.clone(),
            on_key_before_focused_element: self.on_key_before_focused_element.clone(),
            on_key_before_focused: self.on_key_before_focused.clone(),
            on_key: self.on_key.clone(),
            on_mouse_press_before_focused: self.on_mouse_press_before_focused.clone(),
            on_mouse_wheel_before_focused: self.on_mouse_wheel_before_focused.clone(),
            on_text_changed: self.on_text_changed.clone(),
            on_text_confirmed: self.on_text_confirmed.clone(),
            on_selection_changed: self.on_selection_changed.clone(),
            on_edit_started: self.on_edit_started.clone(),
            on_focus_changed: self.on_focus_changed.clone(),
            should_close: self.should_close.clone(),
            ..Default::default()
        }
    }
}

#[derive(Clone, Debug)]
pub struct GeneratedWindowStackFrame
{
    pub window: GeneratedWindow,
    pub options: GeneratedWindowFrameOptions,
    pub bounds: Rect,
    pub focused_index: i32,
    pub edit_mode: bool,
    pub active_scroll_view: Option<ElementRef>,
    pub active_scroll_view_edit_element: Option<ElementRef>,
    pub suppress_active_scroll_view_scope_visuals: bool,
    pub edit_snapshot: Option<EditSnapshot>,
    pub scroll_view_last_descendant: HashMap<u64, RememberedScrollViewDescendant>,
}

impl GeneratedWindowStackFrame
{
    fn new(window: GeneratedWindow, options: GeneratedWindowFrameOptions) -> Self
    {
        let focused_index = if options.initial_focus_name.is_empty()
        {
            if options.start_in_edit_mode
                && !focusable_elements(&window).is_empty()
            {
                0
            }
            else
            {
                -1
            }
        }
        else
        {
            let initial = window.find_element(&options.initial_focus_name);
            initial
                .as_ref()
                .and_then(|initial|
                {
                    focusable_elements(&window)
                        .iter()
                        .position(|element| std::rc::Rc::ptr_eq(element, initial))
                })
                .map_or(-1, |index| index as i32)
        };
        let edit_mode = options.start_in_edit_mode;
        Self
        {
            window,
            options,
            bounds: Rect::default(),
            focused_index,
            edit_mode,
            active_scroll_view: None,
            active_scroll_view_edit_element: None,
            suppress_active_scroll_view_scope_visuals: false,
            edit_snapshot: None,
            scroll_view_last_descendant: HashMap::new(),
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct GeneratedWindowStack
{
    frames: Vec<GeneratedWindowStackFrame>,
}

impl GeneratedWindowStack
{
    fn push_frame(&mut self, mut frame: GeneratedWindowStackFrame) -> usize
    {
        let nested_frames = std::mem::take(&mut frame.window.window_stack.frames);
        frame.window.owns_window_stack = false;
        let index = self.frames.len();
        self.frames.push(frame);
        for nested_frame in nested_frames
        {
            self.push_frame(nested_frame);
        }
        index
    }

    pub fn push(
        &mut self,
        window: GeneratedWindow,
        options: GeneratedWindowFrameOptions,
    ) -> &mut GeneratedWindowStackFrame
    {
        let index = self.push_frame(GeneratedWindowStackFrame::new(window, options));
        &mut self.frames[index]
    }

    pub fn pop(&mut self) -> Option<GeneratedWindowStackFrame>
    {
        self.frames.pop()
    }

    pub fn clear(&mut self)
    {
        self.frames.clear();
    }

    pub fn remove(&mut self, window: &GeneratedWindow)
    {
        self.frames.retain(|frame| frame.window.identity != window.identity);
    }

    pub fn top(&self) -> Option<&GeneratedWindowStackFrame>
    {
        self.frames.last()
    }

    pub fn top_mut(&mut self) -> Option<&mut GeneratedWindowStackFrame>
    {
        self.frames.last_mut()
    }

    pub fn frames(&self) -> &[GeneratedWindowStackFrame]
    {
        &self.frames
    }

    pub fn frames_mut(&mut self) -> &mut [GeneratedWindowStackFrame]
    {
        &mut self.frames
    }
}

#[derive(Clone, Debug)]
pub struct GeneratedWindow
{
    pub window: Window,
    identity: u64,
    pub layout: Vec<LayoutItem>,
    pub kind: String,
    pub generated_focusable: bool,
    generated_control_activation: Option<String>,
    pub window_style: Style,
    pub metadata: McpMetadata,
    pub mcp_element_metadata: Vec<GeneratedElementMetadata>,
    pub mcp_app_tools: Vec<GeneratedAppToolMetadata>,
    pub named_styles: HashMap<String, Style>,
    generated_scroll_view: Option<ScrollViewRef>,
    pub window_stack: GeneratedWindowStack,
    owns_window_stack: bool,
    pending_windows: Vec<(GeneratedWindow, GeneratedWindowFrameOptions)>,
    pub close_requested: bool,
    pub notification: String,
    notification_expires_at: Option<Instant>,
    runtime_options: GeneratedWindowRuntimeOptions,
    behavior: Option<GeneratedWindowBehaviorRef>,
}

impl Deref for GeneratedWindow
{
    type Target = Window;

    fn deref(&self) -> &Self::Target
    {
        &self.window
    }
}

impl DerefMut for GeneratedWindow
{
    fn deref_mut(&mut self) -> &mut Self::Target
    {
        &mut self.window
    }
}

impl GeneratedWindow
{
    pub fn new(title: impl Into<String>) -> Self
    {
        Self
        {
            window: Window::new(title),
            identity: NEXT_GENERATED_WINDOW_IDENTITY.fetch_add(1, Ordering::Relaxed),
            layout: Vec::new(),
            kind: "window".to_string(),
            generated_focusable: false,
            generated_control_activation: None,
            window_style: Style::default(),
            metadata: McpMetadata::default(),
            mcp_element_metadata: Vec::new(),
            mcp_app_tools: Vec::new(),
            named_styles: HashMap::new(),
            generated_scroll_view: None,
            window_stack: GeneratedWindowStack::default(),
            owns_window_stack: true,
            pending_windows: Vec::new(),
            close_requested: false,
            notification: String::new(),
            notification_expires_at: None,
            runtime_options: GeneratedWindowRuntimeOptions::default(),
            behavior: None,
        }
    }

    pub fn new_scroll_view(title: impl Into<String>) -> Self
    {
        Self::new_scroll_view_with_gap(title, 0)
    }

    pub fn new_scroll_view_with_gap(title: impl Into<String>, gap: i32) -> Self
    {
        let mut window = Self::new(title);
        let scroll_view = crate::new_scroll_view("__scrollview", gap);
        window.add_element(scroll_view.clone());
        window.generated_scroll_view = Some(scroll_view);
        window
    }

    pub fn identity(&self) -> u64
    {
        self.identity
    }

    pub fn generated_layout(&self) -> &[LayoutItem]
    {
        &self.layout
    }

    pub fn generated_window_style(&self) -> &Style
    {
        &self.window_style
    }

    pub fn generated_focusable(&self) -> bool
    {
        self.generated_focusable
    }

    pub fn set_generated_control_activation(&mut self, name: impl Into<String>)
    {
        self.generated_control_activation = Some(name.into());
    }

    pub fn clear_generated_control_activation(&mut self)
    {
        self.generated_control_activation = None;
    }

    pub fn activate_generated_control(&self) -> Option<&str>
    {
        self.generated_control_activation.as_deref()
    }

    pub fn generated_kind(&self) -> &str
    {
        &self.kind
    }

    pub fn mcp_enabled(&self) -> bool
    {
        self.metadata.enabled
    }

    pub fn mcp_source_markdown(&self) -> &str
    {
        &self.metadata.source
    }

    pub fn mcp_class_name(&self) -> &str
    {
        &self.metadata.class_name
    }

    pub fn mcp_window_description(&self) -> &str
    {
        &self.metadata.description
    }

    pub fn mcp_app_tools(&self) -> &[GeneratedAppToolMetadata]
    {
        &self.mcp_app_tools
    }

    pub fn runtime_options(&self) -> GeneratedWindowRuntimeOptions
    {
        self.runtime_options.clone()
    }

    pub fn set_runtime_options(&mut self, options: GeneratedWindowRuntimeOptions)
    {
        self.runtime_options = options;
    }

    pub fn set_behavior(&mut self, behavior: impl GeneratedWindowBehavior + 'static)
    {
        self.behavior = Some(GeneratedWindowBehaviorRef::new(behavior));
    }

    pub fn set_behavior_ref(&mut self, behavior: GeneratedWindowBehaviorRef)
    {
        self.behavior = Some(behavior);
    }

    pub fn behavior_ref(&self) -> Option<GeneratedWindowBehaviorRef>
    {
        self.behavior.clone()
    }

    fn behavior_handles_button(&mut self, name: &str) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_button(self, name)
    }

    fn behavior_handles_preview_key(&mut self, event: &KeyEvent) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_preview_key(self, event)
    }

    fn behavior_handles_key_before_focused(
        &mut self,
        key: &str,
        name: &str,
        edit_mode: bool,
    ) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior
            .0
            .borrow_mut()
            .handle_key_before_focused(self, key, name, edit_mode)
    }

    fn behavior_handles_key(&mut self, key: &str) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_key(self, key)
    }

    fn behavior_handles_listbox_item_activate(
        &mut self,
        name: &str,
        element_id: &str,
        index: usize,
        value: &str,
    ) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_listbox_item_activate(
            self,
            name,
            element_id,
            index,
            value,
        )
    }

    fn behavior_handles_mouse_press(&mut self, point: Point) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_mouse_press(self, point)
    }

    fn behavior_handles_mouse_wheel(&mut self, point: Point, delta: i32) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_mouse_wheel(self, point, delta)
    }

    fn behavior_handles_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior
            .0
            .borrow_mut()
            .handle_text_changed(self, name, value)
    }

    fn behavior_handles_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior
            .0
            .borrow_mut()
            .handle_text_confirmed(self, name, value)
    }

    fn behavior_handles_selection_changed(&mut self, name: &str, values: &[String]) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior
            .0
            .borrow_mut()
            .handle_selection_changed(self, name, values)
    }

    fn behavior_handles_edit_started(&mut self, name: &str) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior.0.borrow_mut().handle_edit_started(self, name);
        true
    }

    fn behavior_handles_focus_changed(&mut self, name: &str, focused: bool) -> bool
    {
        let Some(behavior) = self.behavior.clone() else { return false };
        behavior
            .0
            .borrow_mut()
            .handle_focus_changed(self, name, focused);
        true
    }

    fn behavior_should_close(&self) -> bool
    {
        self.behavior
            .as_ref()
            .is_some_and(|behavior| behavior.0.borrow().should_close())
    }

    pub fn set_generated_layout(&mut self, layout: Vec<LayoutItem>) { self.layout = layout; }
    pub fn set_generated_kind(&mut self, kind: impl Into<String>) { self.kind = kind.into(); }
    pub fn set_generated_focusable(&mut self, focusable: bool) { self.generated_focusable = focusable; }
    pub fn set_generated_window_style(&mut self, style: Style) { self.window_style = style; }
    pub fn set_generated_scroll_view_style(&mut self, style: Style)
    {
        if let Some(scroll_view) = self.generated_scroll_view.as_ref()
        {
            scroll_view.set_gap(style.gap.unwrap_or(0));
            scroll_view.borrow_mut().style = style;
        }
    }
    pub fn set_generated_scroll_view_focus_style(&mut self, style: Style)
    {
        if let Some(scroll_view) = self.generated_scroll_view.as_ref()
        {
            scroll_view.borrow_mut().focus_style = Some(style);
        }
    }
    pub fn set_generated_scroll_view_edit_style(&mut self, style: Style)
    {
        if let Some(scroll_view) = self.generated_scroll_view.as_ref()
        {
            scroll_view.borrow_mut().edit_style = Some(style);
        }
    }
    pub fn set_generated_scroll_view_selected_style(&mut self, style: Style)
    {
        if let Some(scroll_view) = self.generated_scroll_view.as_ref()
        {
            scroll_view.borrow_mut().selected_style = Some(style);
        }
    }
    pub fn set_generated_scroll_view_descendant_focus_style(&mut self, style: Style)
    {
        if let Some(scroll_view) = self.generated_scroll_view.as_ref()
        {
            scroll_view.set_descendant_focus_style(style);
        }
    }

    pub fn generated_scroll_view(&self) -> Option<ScrollViewRef>
    {
        self.generated_scroll_view.clone()
    }

    pub fn scroll_view(&self) -> ScrollViewRef
    {
        self.generated_scroll_view()
            .expect("generated scroll view has not been initialized")
    }

    pub fn is_generated_scroll_view(&self) -> bool
    {
        self.generated_scroll_view.is_some()
    }

    fn generated_scroll_view_style(&self) -> Style
    {
        self.scroll_view().borrow().style.clone()
    }

    fn generated_scroll_view_focus_style(&self) -> Style
    {
        self.scroll_view()
            .borrow()
            .focus_style
            .clone()
            .unwrap_or_default()
    }

    fn generated_scroll_view_descendant_focus_style(&self) -> Style
    {
        self.scroll_view()
            .descendant_focus_style()
            .unwrap_or_default()
    }

    pub fn set_generated_named_style(&mut self, name: impl Into<String>, style: Style)
    {
        self.named_styles.insert(name.into(), style);
    }

    pub fn apply_named_cell_style(&mut self, cell_name: &str, style_name: &str) -> bool
    {
        let Some(style) = self.named_styles.get(style_name).cloned() else
        {
            return false;
        };
        let mut matched = false;
        for item in &mut self.layout
        {
            if item.cell_name == cell_name
            {
                item.cell_style.merge(&style);
                matched = true;
            }
        }
        matched
    }

    pub fn set_mcp_metadata(
        &mut self,
        enabled: bool,
        class_name: impl Into<String>,
        source: impl Into<String>,
        description: impl Into<String>,
    )
    {
        self.metadata = McpMetadata
        {
            enabled,
            class_name: class_name.into(),
            source: source.into(),
            description: description.into(),
        };
    }

    pub fn set_mcp_element_metadata(&mut self, metadata: Vec<GeneratedElementMetadata>)
    {
        self.mcp_element_metadata = metadata;
    }

    pub fn set_mcp_app_tools(&mut self, tools: Vec<GeneratedAppToolMetadata>)
    {
        self.mcp_app_tools = tools;
    }

    pub fn mcp_element_metadata(&self, name: &str) -> Option<&GeneratedElementMetadata>
    {
        self.mcp_element_metadata
            .iter()
            .find(|metadata| metadata.name == name)
    }

    pub fn add_child_window(&mut self, child: GeneratedWindow) -> ElementRef
    {
        let child_name = child.metadata.class_name.clone();
        self.add_named_child_window(child_name, child)
    }

    pub fn add_named_child_window(
        &mut self,
        name: impl Into<String>,
        child: GeneratedWindow,
    ) -> ElementRef
    {
        let child_name = name.into();
        let child_element = crate::new_reusable_element(&child_name, &child.metadata.class_name);
        child_element.borrow_mut().set_child_window(child);
        let child_element = child_element.into_element();
        self.scroll_view_ref().add_child(child_element.clone());
        child_element
    }

    pub fn clear_children(&mut self)
    {
        self.scroll_view_ref().clear_children();
    }

    pub fn add_child<T: Into<ElementRef>>(&self, child: T)
    {
        self.scroll_view_ref().add_child(child);
    }

    pub fn frame(&self) -> Rect
    {
        self.scroll_view_ref().borrow().frame
    }

    pub fn add_rendered_child(&self, child: TerminalBuffer)
    {
        self.scroll_view_ref().add_rendered_child(child);
    }

    pub fn set_dynamic_children_renderer<F>(&mut self, renderer: F)
    where
        F: Fn(i32) -> Vec<TerminalBuffer> + 'static,
    {
        self.scroll_view_ref().set_dynamic_children_renderer(renderer);
    }

    pub fn invalidate_dynamic_children(&self)
    {
        self.scroll_view_ref().invalidate_dynamic_children();
    }

    fn scroll_view_ref(&self) -> ScrollViewRef
    {
        self.scroll_view()
    }

    pub(crate) fn scroll_view_children(&self) -> Vec<ElementRef>
    {
        self.scroll_view_ref().borrow().children.clone()
    }

    fn scroll_view_native_child_heights(&self, width: i32) -> Vec<i32>
    {
        self.scroll_view_children()
            .iter()
            .map(|child|
            {
                let child = child.borrow();
                child.child_window().map_or_else(
                    || max(1, child.frame.height),
                    |window|
                        generated_window_content_size_for_width(window, width).height,
                )
            })
            .collect()
    }

    pub fn set_auto_scroll(&self, auto_scroll: bool)
    {
        self.scroll_view_ref().set_auto_scroll(auto_scroll);
    }

    pub fn auto_scroll(&self) -> bool
    {
        self.scroll_view_ref().auto_scroll()
    }

    pub fn view_offset(&self) -> i32
    {
        self.scroll_view_ref().view_offset()
    }

    pub fn scroll_offset(&self) -> i32
    {
        self.scroll_view_ref().scroll_offset()
    }

    pub fn h_view_offset(&self) -> i32
    {
        self.scroll_view_ref().h_view_offset()
    }

    pub(crate) fn last_natural_skip(&self) -> i32
    {
        self.scroll_view_ref().last_natural_skip()
    }

    pub(crate) fn set_view_offset(&self, view_offset: i32)
    {
        self.scroll_view_ref().set_view_offset(view_offset);
    }

    pub fn scroll_position(&self) -> ScrollViewPosition
    {
        self.scroll_view_ref().scroll_position()
    }

    pub fn restore_scroll_position(&self, position: ScrollViewPosition)
    {
        self.scroll_view_ref().restore_scroll_position(position);
    }

    pub fn restore_scroll_position_in(
        &mut self,
        position: ScrollViewPosition,
        viewport: Size,
    )
    {
        layout_generated_scroll_children(self, viewport);
        self.restore_scroll_position(position);
    }

    pub fn content_height(&self) -> i32
    {
        self.scroll_view_ref().content_height()
    }

    pub fn content_height_for_width(&self, width: i32) -> i32
    {
        self.scroll_view_ref().content_height_for_width(width)
    }

    pub fn scroll_to(&mut self, index: i32, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_to(index, viewport)
    }

    pub fn scroll_by(&mut self, delta: i32, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_by(delta, viewport)
    }

    pub fn scroll_lines(&mut self, delta: i32, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_lines(delta, viewport)
    }

    pub fn scroll_to_top(&self) -> bool
    {
        let frame = self.frame();
        self.scroll_view_ref().scroll_to_top(Size
        {
            width: frame.width,
            height: frame.height,
        })
    }

    pub fn scroll_to_top_in(&mut self, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_to_top(viewport)
    }

    pub fn scroll_to_bottom(&mut self) -> bool
    {
        let frame = self.frame();
        self.scroll_view_ref().scroll_to_bottom(Size
        {
            width: frame.width,
            height: frame.height,
        })
    }

    pub fn scroll_to_bottom_in(&mut self, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_to_bottom(viewport)
    }

    pub fn scroll_page_up(&mut self, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_page_up(viewport)
    }

    pub fn scroll_page_down(&mut self, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().scroll_page_down(viewport)
    }

    pub fn scroll_horizontal(&self, delta: i32) -> bool
    {
        self.scroll_view_ref().scroll_horizontal(delta)
    }

    pub fn wheel_scroll_lines(&self, viewport: Size) -> i32
    {
        self.scroll_view_ref().wheel_scroll_lines(viewport)
    }

    pub fn can_scroll_up(&self) -> bool
    {
        let frame = self.scroll_view_ref().borrow().frame;
        self.scroll_view_ref().can_scroll_up(Size
        {
            width: frame.width,
            height: frame.height,
        })
    }

    pub fn can_scroll_down(&mut self, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().can_scroll_down(viewport)
    }

    pub fn handle_wheel(&mut self, wheel_delta: i32, viewport: Size) -> bool
    {
        layout_generated_scroll_children(self, viewport);
        self.scroll_view_ref().handle_wheel(wheel_delta, viewport)
    }

    pub fn consume_terminal_scroll_delta(&self) -> i32
    {
        self.scroll_view_ref().consume_terminal_scroll_delta()
    }

    pub fn handle_key(&self, key: &str) -> bool
    {
        let frame = self.frame();
        self.scroll_view_ref().handle_key(
            key,
            Size
            {
                width: frame.width,
                height: frame.height,
            },
        )
    }

    pub fn open_window(&mut self, window: GeneratedWindow)
    {
        let options = GeneratedWindowFrameOptions::from_window(&window);
        if self.owns_window_stack
        {
            self.window_stack.push(window, options);
        }
        else
        {
            self.pending_windows.push((window, options));
        }
    }

    pub fn close_active_window(&mut self) -> Option<GeneratedWindow>
    {
        if !self.owns_window_stack
        {
            return None;
        }
        self.window_stack.pop().map(|frame| frame.window)
    }

    pub fn active_window(&self) -> &GeneratedWindow
    {
        if !self.owns_window_stack
        {
            return self;
        }
        self.window_stack.frames.last().map_or(self, |frame| &frame.window)
    }

    pub fn active_window_mut(&mut self) -> &mut GeneratedWindow
    {
        if !self.owns_window_stack || self.window_stack.frames.is_empty()
        {
            return self;
        }
        &mut self
            .window_stack
            .frames
            .last_mut()
            .expect("non-empty modal stack must have a final frame")
            .window
    }

    pub fn modal_depth(&self) -> usize
    {
        if self.owns_window_stack
        {
            self.window_stack.frames.len()
        }
        else
        {
            0
        }
    }

    fn modal_frame(&self, depth: usize) -> Option<&GeneratedWindowStackFrame>
    {
        self.owns_window_stack
            .then(|| self.window_stack.frames.get(depth))
            .flatten()
    }

    fn modal_frame_mut(
        &mut self,
        depth: usize,
    ) -> Option<&mut GeneratedWindowStackFrame>
    {
        self.owns_window_stack
            .then(|| self.window_stack.frames.get_mut(depth))
            .flatten()
    }

    fn promote_pending_windows(&mut self)
    {
        if !self.owns_window_stack
        {
            return;
        }
        loop
        {
            let pending = self
                .window_stack
                .frames
                .last_mut()
                .map(|frame| std::mem::take(&mut frame.window.pending_windows))
                .unwrap_or_else(|| std::mem::take(&mut self.pending_windows));
            if pending.is_empty()
            {
                break;
            }
            for (window, options) in pending
            {
                self.window_stack.push(window, options);
            }
        }
    }

    pub fn find_element(&self, name: &str) -> Option<ElementRef>
    {
        if let Some(element) = self.find_direct_element(name)
        {
            return Some(element);
        }
        all_window_elements(self)
            .into_iter()
            .find(|element|
            {
                self.element_id(element)
                .as_deref()
                .is_some_and(|element_id| element_id == name)
            })
    }

    pub fn element_id(&self, target: &ElementRef) -> Option<String>
    {
        element_id_for_element(self, target)
    }

    pub fn find_direct_element(&self, name: &str) -> Option<ElementRef>
    {
        self.elements
            .iter()
            .find(|element| element.borrow().name == name)
            .cloned()
    }

    pub fn request_close(&mut self)
    {
        self.close_requested = true;
    }

    pub fn should_close(&self) -> bool
    {
        self.close_requested
    }

    pub fn show_notification(&mut self, message: impl Into<String>)
    {
        self.notification = message.into();
        self.notification_expires_at = Some(Instant::now() + COPY_NOTIFICATION_DURATION);
    }

    pub fn expire_notification(&mut self) -> bool
    {
        if self
            .notification_expires_at
            .is_some_and(|deadline| Instant::now() >= deadline)
        {
            self.notification.clear();
            self.notification_expires_at = None;
            return true;
        }
        false
    }
}

#[derive(Clone, Debug)]
pub struct RuntimeState
{
    pub focused_index: i32,
    pub edit_mode: bool,
    pub scope_dim_element: Option<ElementRef>,
    pub scope_edit_element: Option<ElementRef>,
    pub viewport: Size,
    edit_snapshot: Option<EditSnapshot>,
    render_background_override: Option<Color>,
    render_clip_top: Option<i32>,
    render_clip_bottom: Option<i32>,
    render_inside_scope: bool,
    suppress_active_scroll_scope_visuals: bool,
    pub(crate) mouse_selection_element: Option<ElementRef>,
    pub(crate) mouse_selection_anchor: usize,
    pub(crate) mouse_click_candidate: Option<ElementRef>,
    pub(crate) mouse_click_candidate_moved: bool,
    pub(crate) scroll_selection: ScrollSelection,
    pending_standard_escape_button: Option<(String, Instant)>,
    scroll_view_last_descendant: HashMap<u64, RememberedScrollViewDescendant>,
    modal_depth: usize,
    active_window_identity: u64,
    root_interaction_snapshot: Option<InteractionSnapshot>,
}

#[derive(Clone, Debug, Default)]
pub(crate) struct ScrollSelection
{
    pub(crate) element: Option<ElementRef>,
    pub(crate) anchor: Point,
    pub(crate) current: Point,
    pub(crate) active: bool,
    pub(crate) changed: bool,
}

#[derive(Clone, Debug)]
pub struct EditSnapshot
{
    pub element: ElementRef,
    pub text_value: String,
    pub text_cursor: usize,
    pub text_selection_anchor: Option<usize>,
    pub number_value: f64,
    pub selected_index: i32,
}

#[derive(Clone, Debug)]
pub struct RememberedScrollViewDescendant
{
    pub scope: ElementRef,
    pub element: ElementRef,
    pub scope_identity: u64,
    pub element_identity: u64,
}

#[derive(Clone, Debug)]
struct InteractionSnapshot
{
    focused_index: i32,
    edit_mode: bool,
    scope_dim_element: Option<ElementRef>,
    scope_edit_element: Option<ElementRef>,
    edit_snapshot: Option<EditSnapshot>,
    scroll_view_last_descendant: HashMap<u64, RememberedScrollViewDescendant>,
    scope_scroll_position: Option<ScrollViewPosition>,
}

#[derive(Clone, Debug, Default)]
struct InteractionElementIds
{
    focused: Option<String>,
    scope_edit: Option<String>,
    scope_dim: Option<String>,
    edit_snapshot: Option<String>,
}

impl RuntimeState
{
    pub fn new(window: &GeneratedWindow, options: &GeneratedWindowRuntimeOptions, viewport: Size) -> Self
    {
        let focusable = focusable_elements(window);
        let focused_index = if options.initial_focus_name.is_empty()
        {
            if options.start_in_edit_mode && !focusable.is_empty() { 0 } else { -1 }
        }
        else
        {
            focusable
                .iter()
                .position(|element| element.borrow().name == options.initial_focus_name)
                .map_or(-1, |index| index as i32)
        };
        let edit_mode = options.start_in_edit_mode
            && focused_index >= 0
            && focusable
                .get(focused_index as usize)
                .is_some_and(is_editable_element);
        let mut state = Self
        {
            focused_index,
            edit_mode,
            scope_dim_element: None,
            scope_edit_element: None,
            viewport,
            edit_snapshot: None,
            render_background_override: None,
            render_clip_top: None,
            render_clip_bottom: None,
            render_inside_scope: false,
            suppress_active_scroll_scope_visuals: false,
            mouse_selection_element: None,
            mouse_selection_anchor: 0,
            mouse_click_candidate: None,
            mouse_click_candidate_moved: false,
            scroll_selection: ScrollSelection::default(),
            pending_standard_escape_button: None,
            scroll_view_last_descendant: HashMap::new(),
            modal_depth: 0,
            active_window_identity: window.identity,
            root_interaction_snapshot: None,
        };
        if state.edit_mode
            && let Some(element) = state.focused_element(window)
        {
            state.begin_element_edit(&element);
        }
        state
    }

    pub(crate) fn notify_initial_edit_started<A: GeneratedApplication>(
        &self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
    )
    {
        if let Some(name) = self.current_editing_element(app.window())
        {
            dispatch_edit_started(app, options, &name);
        }
    }

    fn interaction_snapshot(&self) -> InteractionSnapshot
    {
        let scope_scroll_position = self.scope_edit_element.as_ref().and_then(|scope|
        {
            let value = scope.borrow();
            if value.kind() == ElementKind::ScrollView
            {
                Some(value.scroll_position())
            }
            else
            {
                value
                    .child_window()
                    .filter(|window| window.is_generated_scroll_view())
                    .map(GeneratedWindow::scroll_position)
            }
        });
        InteractionSnapshot
        {
            focused_index: self.focused_index,
            edit_mode: self.edit_mode,
            scope_dim_element: self.scope_dim_element.clone(),
            scope_edit_element: self.scope_edit_element.clone(),
            edit_snapshot: self.edit_snapshot.clone(),
            scroll_view_last_descendant: self.scroll_view_last_descendant.clone(),
            scope_scroll_position,
        }
    }

    fn restore_interaction(
        &mut self,
        snapshot: InteractionSnapshot,
        window: &GeneratedWindow,
    )
    {
        let scope_scroll_position = snapshot.scope_scroll_position;
        self.focused_index = snapshot.focused_index;
        self.edit_mode = snapshot.edit_mode;
        self.scope_dim_element = snapshot.scope_dim_element;
        self.scope_edit_element = snapshot.scope_edit_element;
        self.edit_snapshot = snapshot.edit_snapshot;
        self.scroll_view_last_descendant = snapshot.scroll_view_last_descendant;
        self.repair_live_interaction_with_scope_restore(
            window,
            Some(snapshot.focused_index),
        );
        if let (Some(scope), Some(position)) = (
            self.scope_edit_element.clone(),
            scope_scroll_position,
        )
        {
            let frame = scope.borrow().frame;
            let is_scroll_view = scope.borrow().kind() == ElementKind::ScrollView;
            if is_scroll_view
            {
                scope.borrow().restore_scroll_position(position);
            }
            else if let Some(scroll_view) = scope.borrow_mut().child_window_mut()
                && scroll_view.is_generated_scroll_view()
            {
                scroll_view.restore_scroll_position_in(
                    position,
                    Size
                    {
                        width: max(1, frame.width),
                        height: max(1, frame.height),
                    },
                );
            }
        }
    }

    fn reset_interaction(
        &mut self,
        window: &GeneratedWindow,
        options: &GeneratedWindowRuntimeOptions,
    )
    {
        let next = RuntimeState::new(window, options, self.viewport);
        self.focused_index = next.focused_index;
        self.edit_mode = next.edit_mode;
        self.scope_dim_element = next.scope_dim_element;
        self.scope_edit_element = next.scope_edit_element;
        self.edit_snapshot = next.edit_snapshot;
        self.scroll_view_last_descendant = next.scroll_view_last_descendant;
    }

    fn write_interaction_to_frame(
        &self,
        frame: &mut GeneratedWindowStackFrame,
    )
    {
        frame.focused_index = self.focused_index;
        frame.edit_mode = self.edit_mode;
        frame.active_scroll_view = self.scope_edit_element.clone();
        frame.active_scroll_view_edit_element = self.scope_dim_element.clone();
        frame.suppress_active_scroll_view_scope_visuals =
            self.suppress_active_scroll_scope_visuals;
        frame.edit_snapshot = self.edit_snapshot.clone();
        frame.scroll_view_last_descendant = self.scroll_view_last_descendant.clone();
    }

    fn interaction_from_frame(frame: &GeneratedWindowStackFrame) -> InteractionSnapshot
    {
        let scope_scroll_position = frame.active_scroll_view.as_ref().and_then(|scope|
        {
            let value = scope.borrow();
            if value.kind() == ElementKind::ScrollView
            {
                Some(value.scroll_position())
            }
            else
            {
                value
                    .child_window()
                    .filter(|window| window.is_generated_scroll_view())
                    .map(GeneratedWindow::scroll_position)
            }
        });
        InteractionSnapshot
        {
            focused_index: frame.focused_index,
            edit_mode: frame.edit_mode,
            scope_dim_element: frame.active_scroll_view_edit_element.clone(),
            scope_edit_element: frame.active_scroll_view.clone(),
            edit_snapshot: frame.edit_snapshot.clone(),
            scroll_view_last_descendant: frame.scroll_view_last_descendant.clone(),
            scope_scroll_position,
        }
    }

    pub fn sync_active_window(
        &mut self,
        root: &mut GeneratedWindow,
        root_options: &GeneratedWindowRuntimeOptions,
    )
    {
        root.promote_pending_windows();
        let next_depth = root.modal_depth();
        while self.modal_depth < next_depth
        {
            let snapshot = self.interaction_snapshot();
            if self.modal_depth == 0
            {
                self.root_interaction_snapshot = Some(snapshot);
            }
            else if let Some(frame) = root.modal_frame_mut(self.modal_depth - 1)
            {
                self.write_interaction_to_frame(frame);
            }
            let Some(next_frame) = root.modal_frame(self.modal_depth) else
            {
                break;
            };
            let active_options = next_frame.window.runtime_options();
            self.reset_interaction(&next_frame.window, &active_options);
            self.modal_depth += 1;
            self.active_window_identity = next_frame.window.identity;
        }
        while self.modal_depth > next_depth
        {
            self.modal_depth -= 1;
            self.active_window_identity = root.active_window().identity;
            let previous = if self.modal_depth == 0
            {
                self.root_interaction_snapshot.take()
            }
            else
            {
                root.modal_frame(self.modal_depth - 1)
                    .map(Self::interaction_from_frame)
            };
            if let Some(previous) = previous
            {
                self.restore_interaction(previous, root.active_window());
            }
            else
            {
                let active_options = if self.modal_depth == 0
                {
                    root_options.clone()
                }
                else
                {
                    root.active_window().runtime_options()
                };
                self.reset_interaction(root.active_window(), &active_options);
            }
        }
        if self.active_window_identity != root.active_window().identity
        {
            let active_options = root.active_window().runtime_options();
            self.reset_interaction(root.active_window(), &active_options);
            self.active_window_identity = root.active_window().identity;
        }
    }

    pub fn focused_element(&self, window: &GeneratedWindow) -> Option<ElementRef>
    {
        if self.focused_index < 0
        {
            return None;
        }
        let focusable = focusable_elements_for(window, self.scope_edit_element.as_ref());
        focusable.get(self.focused_index as usize).cloned()
    }

    pub(crate) fn focus_element(
        &mut self,
        window: &GeneratedWindow,
        target: &ElementRef,
    ) -> bool
    {
        let focusable = focusable_elements_for(window, self.scope_edit_element.as_ref());
        let Some(index) = focusable
            .iter()
            .position(|element| std::rc::Rc::ptr_eq(element, target))
        else
        {
            return false;
        };
        self.focused_index = index as i32;
        true
    }

    fn repair_live_interaction(&mut self, window: &GeneratedWindow)
    {
        let element_ids = self.interaction_element_ids(window);
        self.repair_live_interaction_with_ids(window, element_ids, None);
    }

    fn repair_live_interaction_with_scope_restore(
        &mut self,
        window: &GeneratedWindow,
        scope_focused_index: Option<i32>,
    )
    {
        let element_ids = self.interaction_element_ids(window);
        self.repair_live_interaction_with_ids(
            window,
            element_ids,
            scope_focused_index,
        );
    }

    fn interaction_element_ids(
        &self,
        window: &GeneratedWindow,
    ) -> InteractionElementIds
    {
        InteractionElementIds
        {
            focused: self
                .focused_element(window)
                .as_ref()
                .and_then(|element| window.element_id(element)),
            scope_edit: self
                .scope_edit_element
                .as_ref()
                .and_then(|element| window.element_id(element)),
            scope_dim: self
                .scope_dim_element
                .as_ref()
                .and_then(|element| window.element_id(element)),
            edit_snapshot: self
                .edit_snapshot
                .as_ref()
                .and_then(|snapshot| window.element_id(&snapshot.element)),
        }
    }

    fn repair_live_interaction_with_ids(
        &mut self,
        window: &GeneratedWindow,
        element_ids: InteractionElementIds,
        scope_focused_index: Option<i32>,
    )
    {
        if window.identity != self.active_window_identity
        {
            return;
        }
        let had_focused = element_ids.focused.is_some()
            || scope_focused_index.is_some_and(|index| index >= 0);
        let had_scope = self.scope_edit_element.is_some();
        self.scope_edit_element = element_ids
            .scope_edit
            .as_deref()
            .and_then(|id| window.find_element(id));
        if had_scope
            && self.scope_edit_element.as_ref().is_some_and(|scope|
            {
                let value = scope.borrow();
                value.kind() == ElementKind::ViewHost
                    && !value
                        .child_window()
                        .is_some_and(GeneratedWindow::is_generated_scroll_view)
            })
        {
            self.focused_index = -1;
            self.edit_mode = false;
            self.scope_edit_element = None;
            self.scope_dim_element = None;
            self.edit_snapshot = None;
            self.scroll_view_last_descendant.clear();
            return;
        }
        self.scope_dim_element = element_ids
            .scope_dim
            .as_deref()
            .and_then(|id| window.find_element(id));
        let focused = element_ids
            .focused
            .as_deref()
            .and_then(|id| window.find_element(id));
        let focused_is_live = focused
            .as_ref()
            .is_some_and(|element| self.focus_element(window, element));
        if had_scope
            && self.scope_edit_element.is_some()
            && had_focused
            && !focused_is_live
        {
            if let Some(saved_index) = scope_focused_index
            {
                let scope = self
                    .scope_edit_element
                    .clone()
                    .expect("live restored scope disappeared");
                let active_focusable =
                    focusable_elements_for(window, self.scope_edit_element.as_ref());
                if active_focusable.is_empty()
                {
                    self.focused_index = -1;
                }
                else
                {
                    let index = saved_index
                        .max(0)
                        .min(active_focusable.len() as i32 - 1) as usize;
                    self.focused_index = index as i32;
                    let focused = active_focusable[index].clone();
                    if !std::rc::Rc::ptr_eq(&scope, &focused)
                    {
                        self.remember_scope_descendant(&scope, &focused);
                        ensure_focused_visible_in_scroll(&scope, &focused);
                    }
                }
                self.edit_mode = true;
                self.scope_dim_element = None;
                self.edit_snapshot = None;
            }
            else
            {
                self.focused_index = -1;
                self.edit_mode = false;
                self.scope_edit_element = None;
                self.scope_dim_element = None;
                self.edit_snapshot = None;
            }
        }
        self.scroll_view_last_descendant.retain(|scope_identity, remembered|
        {
            *scope_identity == remembered.scope_identity
                && remembered.scope.borrow().identity == remembered.scope_identity
                && remembered.element.borrow().identity == remembered.element_identity
                && window_contains_element(window, &remembered.scope)
                && window_contains_element(window, &remembered.element)
        });
        if let Some(snapshot) = &mut self.edit_snapshot
        {
            if let Some(element) = element_ids
                .edit_snapshot
                .as_deref()
                .and_then(|id| window.find_element(id))
            {
                snapshot.element = element;
            }
            else
            {
                self.edit_snapshot = None;
            }
        }
    }

    pub(crate) fn remember_scope_descendant(
        &mut self,
        scope: &ElementRef,
        descendant: &ElementRef,
    )
    {
        if std::rc::Rc::ptr_eq(scope, descendant)
        {
            return;
        }
        let scope_identity = scope.borrow().identity;
        let element_identity = descendant.borrow().identity;
        self.scroll_view_last_descendant.insert(
            scope_identity,
            RememberedScrollViewDescendant
            {
                scope: scope.clone(),
                element: descendant.clone(),
                scope_identity,
                element_identity,
            },
        );
    }

    fn remembered_scope_descendant(
        &mut self,
        scope: &ElementRef,
        descendants: &[ElementRef],
    ) -> Option<ElementRef>
    {
        let scope_identity = scope.borrow().identity;
        let remembered = self
            .scroll_view_last_descendant
            .get(&scope_identity)
            .cloned();
        if let Some(remembered) = remembered
        {
            let represented = remembered.scope_identity == scope_identity
                && remembered.scope.borrow().identity == remembered.scope_identity
                && remembered.element.borrow().identity == remembered.element_identity
                && descendants
                    .iter()
                    .any(|candidate| std::rc::Rc::ptr_eq(candidate, &remembered.element));
            if represented
            {
                return Some(remembered.element);
            }
            self.scroll_view_last_descendant.remove(&scope_identity);
        }
        None
    }

    fn begin_element_edit(&mut self, element: &ElementRef)
    {
        let element_value = element.borrow();
        let is_text = matches!(
            element_value.kind(),
            ElementKind::TextInput | ElementKind::TextArea
        );
        let is_choice = matches!(
            element_value.kind(),
            ElementKind::ComboBox | ElementKind::ListBox
        );
        self.edit_snapshot = Some(EditSnapshot
        {
            element: element.clone(),
            text_value: if is_text
            {
                element_value.value().to_string()
            }
            else
            {
                String::new()
            },
            text_cursor: if is_text { element_value.cursor() } else { 0 },
            text_selection_anchor: if is_text
            {
                element_value.selection_anchor()
            }
            else
            {
                None
            },
            number_value: if element_value.kind() == ElementKind::NumberInput
            {
                element_value.number_value()
            }
            else
            {
                0.0
            },
            selected_index: if is_choice
            {
                element_value.selected_index()
            }
            else
            {
                0
            },
        });
        drop(element_value);
        let mut element_value = element.borrow_mut();
        if matches!(element_value.kind(), ElementKind::TextInput | ElementKind::TextArea)
        {
            let cursor = element_value.value().len();
            element_value.select_range(cursor, cursor);
        }
        else if element_value.kind() == ElementKind::NumberInput
        {
            element_value.begin_number_edit();
        }
        if element_value.kind() == ElementKind::ListBox
        {
            element_value.hide_active_item();
        }
    }

    pub fn enter_element_edit(&mut self, element: &ElementRef)
    {
        self.begin_element_edit(element);
        self.edit_mode = true;
    }

    pub fn commit_element_edit(&mut self, element: &ElementRef)
    {
        self.end_element_edit(element, true);
        self.edit_mode = false;
    }

    pub(crate) fn leave_scope_navigation(&mut self, window: &GeneratedWindow)
    {
        if let (Some(scope), Some(focused)) =
            (self.scope_edit_element.clone(), self.focused_element(window))
        {
            self.remember_scope_descendant(&scope, &focused);
        }
        self.edit_snapshot = None;
        self.edit_mode = false;
    }

    pub(crate) fn exit_scope_navigation(&mut self, window: &GeneratedWindow)
    {
        let focused = self.focused_element(window);
        let scope = self.scope_edit_element.take();
        if let (Some(scope), Some(focused)) = (&scope, focused)
        {
            self.remember_scope_descendant(scope, &focused);
        }
        self.edit_snapshot = None;
        self.scope_dim_element = None;
        self.focused_index = scope
            .as_ref()
            .and_then(|scope|
            {
                focusable_elements(window)
                    .iter()
                    .position(|candidate| std::rc::Rc::ptr_eq(candidate, scope))
            })
            .map_or(-1, |index| index as i32);
        self.edit_mode = false;
    }

    pub fn exit_current_edit<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
        notify: bool,
    )
    {
        let Some(element) = self.focused_element(app.active_window()) else
        {
            self.edit_snapshot = None;
            self.edit_mode = false;
            return;
        };
        if !self.edit_mode
        {
            self.edit_snapshot = None;
            return;
        }
        if let Some(scope) = self.scope_edit_element.clone()
        {
            self.remember_scope_descendant(&scope, &element);
        }
        let leave_commit = {
            let value = element.borrow();
            value.commit_mode == crate::COMMIT_MODE_LEAVE
                || (value.kind() == ElementKind::ListBox && value.multiple())
        };
        self.end_element_edit(&element, true);
        self.edit_mode = false;
        if notify && !leave_commit
        {
            dispatch_confirm(app, self, options, &element);
        }
    }

    fn end_element_edit(&mut self, element: &ElementRef, commit: bool)
    {
        let leave_commit = {
            let element = element.borrow();
            element.commit_mode == crate::COMMIT_MODE_LEAVE
                || (element.kind() == ElementKind::ListBox && element.multiple())
        };
        if element.borrow().kind() == ElementKind::NumberInput
        {
            if commit
            {
                element.borrow_mut().commit_number_edit();
            }
            else if !leave_commit
            {
                let snapshot_value = self
                    .edit_snapshot
                    .as_ref()
                    .filter(|snapshot| std::rc::Rc::ptr_eq(&snapshot.element, element))
                    .map(|snapshot| snapshot.number_value);
                let mut value = element.borrow_mut();
                value.cancel_number_edit();
                if let Some(snapshot_value) = snapshot_value
                {
                    value.set_number_value(snapshot_value);
                }
            }
            self.edit_snapshot = None;
            return;
        }
        if !commit && !leave_commit
        {
            if let Some(snapshot) = self.edit_snapshot.take()
                && std::rc::Rc::ptr_eq(&snapshot.element, element)
            {
                let mut value = element.borrow_mut();
                match value.kind()
                {
                    ElementKind::TextInput | ElementKind::TextArea =>
                    {
                        value.set_value(snapshot.text_value);
                        value.select_range(
                            snapshot
                                .text_selection_anchor
                                .unwrap_or(snapshot.text_cursor),
                            snapshot.text_cursor,
                        );
                    }
                    ElementKind::ComboBox | ElementKind::ListBox =>
                        value.set_selected_index(snapshot.selected_index),
                    _ => {}
                }
            }
        }
        else
        {
            self.edit_snapshot = None;
        }
    }

    pub fn handle_key<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
        key: &str,
    ) -> bool
    {
        let previous_focus = self
            .focused_element(app.active_window())
            .and_then(|element| app.active_window().element_id(&element));
        let previous_edit = self.current_editing_element(app.active_window());
        let handled = self.handle_key_inner(app, options, key);
        let focused = self
            .focused_element(app.active_window())
            .and_then(|element| app.active_window().element_id(&element));
        if previous_focus != focused
        {
            if let Some(previous_focus) = previous_focus
            {
                dispatch_focus_changed(app, options, &previous_focus, false);
            }
            if let Some(focused) = focused
            {
                dispatch_focus_changed(app, options, &focused, true);
            }
        }
        let editing = self.current_editing_element(app.active_window());
        if editing != previous_edit
            && let Some(editing) = editing
        {
            dispatch_edit_started(app, options, &editing);
        }
        handled
    }

    fn current_editing_element(&self, window: &GeneratedWindow) -> Option<String>
    {
        if !self.edit_mode
        {
            return None;
        }
        self.scope_dim_element
            .clone()
            .or_else(|| self.focused_element(window))
            .and_then(|element| window.element_id(&element))
    }

    fn handle_key_inner<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
        key: &str,
    ) -> bool
    {
        if key.is_empty()
        {
            return false;
        }
        clear_label_selections(app.active_window_mut());
        self.scroll_selection = ScrollSelection::default();
        if key == "Ctrl+C"
        {
            app.window_mut().request_close();
            return true;
        }
        let focused_name = self
            .focused_element(app.active_window())
            .and_then(|element| app.active_window().element_id(&element))
            .unwrap_or_default();
        if dispatch_preview_key(app, &KeyEvent
        {
            key: key.to_string(),
            focused_element_id: focused_name.clone(),
            edit_mode: self.edit_mode,
        })
        {
            return true;
        }
        if key != "Escape"
            && dispatch_key_before_focused(
                app,
                options,
                key,
                &focused_name,
                self.edit_mode,
            )
        {
            return true;
        }
        if self.scope_edit_element.is_some()
        {
            if key == "Escape"
            {
                if let Some(inner) = self.scope_dim_element.take()
                {
                    self.end_element_edit(&inner, false);
                    return true;
                }
                self.exit_scope_navigation(app.active_window());
                return true;
            }
            if let Some(inner) = self.scope_dim_element.clone()
            {
                if key == "Enter"
                    && inner.borrow().kind() == ElementKind::ListBox
                    && dispatch_listbox_item_activate(app, &inner)
                {
                    return true;
                }
                let before = element_value(&inner);
                let handled = inner.borrow_mut().handle_key(key);
                if key == "Enter"
                {
                    dispatch_change_if_needed(app, self, options, &inner, before);
                    if inner.borrow().kind() == ElementKind::ListBox
                        && inner.borrow().multiple()
                    {
                        return true;
                    }
                    self.confirm_scope_element(app, options, &inner);
                    return true;
                }
                if handled
                {
                    dispatch_change_if_needed(app, self, options, &inner, before);
                    return true;
                }
                return true;
            }
            let scope_focusable = self
                .scope_edit_element
                .as_ref()
                .map_or_else(Vec::new, focusable_scope_descendants);
            let scroll_key = matches!(
                key,
                "Alt+Up" | "Alt+Down" | "Ctrl+Up" | "Ctrl+Down"
                    | "PageUp" | "PageDown" | "Home" | "End"
            ) || (scope_focusable.is_empty()
                && matches!(key, "Up" | "Down" | "Left" | "Right" | "ArrowUp" | "ArrowDown" | "ArrowLeft" | "ArrowRight"));
            if scroll_key
                && let Some(scope) = self.scope_edit_element.clone()
            {
                let handled =
                {
                    let mut scope_value = scope.borrow_mut();
                    if scope_value.kind() == ElementKind::ScrollView
                    {
                        scope_value.handle_key(key)
                    }
                    else
                    {
                        scope_value.child_window_mut().is_some_and(|window|
                        {
                            window.is_generated_scroll_view()
                                && window.handle_key(key)
                        })
                    }
                };
                if handled || scope_focusable.is_empty()
                {
                    return true;
                }
            }
            match key
            {
                "Up" | "Down" | "Left" | "Right"
                | "ArrowUp" | "ArrowDown" | "ArrowLeft" | "ArrowRight" =>
                {
                    let direction = key.strip_prefix("Arrow").unwrap_or(key);
                    self.move_focus_within_scope(app.active_window(), direction);
                    return true;
                }
                "Enter" | " " =>
                {
                    self.activate_scope_focused(app, options);
                    return true;
                }
                _ => {}
            }
        }
        let focused = self.focused_element(app.active_window());
        if self.edit_mode
            && let Some(element) = focused
        {
            if key == "Escape"
            {
                self.end_element_edit(&element, false);
                self.edit_mode = false;
                if options.keep_edit_mode_after_escape
                {
                    self.begin_element_edit(&element);
                    self.edit_mode = true;
                }
                return true;
            }
            let kind = element.borrow().kind();
            if key == "Enter" && kind == ElementKind::ListBox
            {
                if dispatch_listbox_item_activate(app, &element)
                {
                    return true;
                }
                let before = element_value(&element);
                element.borrow_mut().handle_key(key);
                dispatch_change_if_needed(app, self, options, &element, before);
                if element.borrow().multiple()
                {
                    self.edit_snapshot = None;
                    return true;
                }
                self.end_element_edit(&element, true);
                dispatch_confirm(app, self, options, &element);
                self.edit_mode = false;
                if options.keep_edit_mode_after_confirm
                {
                    self.begin_element_edit(&element);
                    self.edit_mode = true;
                }
                return true;
            }
            let before = element_value(&element);
            if key == "Enter"
            {
                element.borrow_mut().handle_key(key);
                dispatch_change_if_needed(app, self, options, &element, before);
                let leave_commit = element.borrow().commit_mode == crate::COMMIT_MODE_LEAVE;
                if leave_commit
                {
                    self.edit_snapshot = None;
                    return true;
                }
                self.end_element_edit(&element, true);
                dispatch_confirm(app, self, options, &element);
                self.edit_mode = false;
                if options.keep_edit_mode_after_confirm
                {
                    self.begin_element_edit(&element);
                    self.edit_mode = true;
                }
                return true;
            }
            let handled = element.borrow_mut().handle_key(key);
            if handled
            {
                dispatch_change_if_needed(app, self, options, &element, before);
                return true;
            }
            return true;
        }
        if dispatch_key(app, options, key)
        {
            return true;
        }
        match key
        {
            "Tab" => self.move_focus_linear(app.active_window(), 1),
            "Shift+Tab" => self.move_focus_linear(app.active_window(), -1),
            "Up" | "Down" | "Left" | "Right" => self.move_focus_spatial(app.active_window(), key),
            "Enter" | " " => self.activate_focused(app, options),
            "Escape" => return true,
            _ => return false,
        }
        true
    }

    pub(crate) fn begin_standard_escape<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        _options: &GeneratedWindowRuntimeOptions,
    ) -> bool
    {
        if self.pending_standard_escape_button.is_some()
        {
            return true;
        }
        if self.edit_mode
        {
            return false;
        }
        let class_name = app.active_window().metadata.class_name.clone();
        let Some(button_name) = crate::standard_escape_button_name(&class_name)
        else
        {
            return false;
        };
        let Some(button) = app.active_window().find_element(button_name)
        else
        {
            return false;
        };
        let focusable = focusable_elements(app.active_window());
        let Some(index) = focusable
            .iter()
            .position(|element| std::rc::Rc::ptr_eq(element, &button))
        else
        {
            return false;
        };
        self.focused_index = index as i32;
        self.pending_standard_escape_button = Some((
            button_name.to_string(),
            Instant::now() + DIALOG_BUTTON_CLOSE_DURATION,
        ));
        true
    }

    pub(crate) fn has_pending_standard_escape(&self) -> bool
    {
        self.pending_standard_escape_button.is_some()
    }

    pub(crate) fn complete_pending_standard_escape<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
    ) -> bool
    {
        let Some((_, deadline)) = self.pending_standard_escape_button.as_ref()
        else
        {
            return false;
        };
        if Instant::now() < *deadline
        {
            return false;
        }
        let (button_name, _) = self
            .pending_standard_escape_button
            .take()
            .expect("pending standard Escape disappeared");
        dispatch_button(app, self, options, &button_name);
        true
    }

    pub(crate) fn complete_pending_standard_escape_now<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
    ) -> bool
    {
        if let Some((_, deadline)) = self.pending_standard_escape_button.as_mut()
        {
            *deadline = Instant::now();
        }
        self.complete_pending_standard_escape(app, options)
    }

    fn move_focus_linear(&mut self, window: &GeneratedWindow, delta: i32)
    {
        let focusable = focusable_elements(window);
        if focusable.is_empty()
        {
            self.focused_index = -1;
            return;
        }
        self.focused_index = if self.focused_index < 0
        {
            0
        }
        else
        {
            (self.focused_index + delta).rem_euclid(focusable.len() as i32)
        };
    }

    fn move_focus_spatial(&mut self, window: &GeneratedWindow, direction: &str)
    {
        let focusable = focusable_elements(window);
        if focusable.is_empty()
        {
            return;
        }
        if self.focused_index < 0 || self.focused_index >= focusable.len() as i32
        {
            self.focused_index = 0;
            return;
        }
        if let Some(index) = spatial_focus_target_index(&focusable, self.focused_index as usize, direction)
        {
            self.focused_index = index as i32;
        }
    }

    fn activate_focused<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
    )
    {
        let Some(element) = self.focused_element(app.active_window()) else { return };
        let kind = element.borrow().kind();
        match kind
        {
            ElementKind::Button | ElementKind::Image =>
            {
                dispatch_element_button(app, self, options, &element);
            }
            ElementKind::CheckBox =>
            {
                let before = element_value(&element);
                element.borrow_mut().handle_key("Enter");
                dispatch_change_if_needed(app, self, options, &element, before);
            }
            ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::ComboBox
            | ElementKind::ListBox =>
            {
                self.enter_element_edit(&element);
            }
            ElementKind::ScrollView =>
            {
                let descendants = focusable_scope_descendants(&element);
                let target = self
                    .remembered_scope_descendant(&element, &descendants)
                    .or_else(|| descendants.first().cloned());
                self.scope_edit_element = Some(element.clone());
                self.edit_mode = true;
                self.scope_dim_element = None;
                self.edit_snapshot = None;
                if let Some(target) = target
                {
                    self.focus_element(app.active_window(), &target);
                    self.remember_scope_descendant(&element, &target);
                    ensure_focused_visible_in_scroll(&element, &target);
                }
                else
                {
                    self.focus_element(app.active_window(), &element);
                }
            }
            ElementKind::Reusable | ElementKind::ViewHost =>
            {
                if activate_reusable_control(app, self, options, &element)
                {
                    self.edit_mode = false;
                    self.scope_dim_element = None;
                    return;
                }
                let is_scroll_view = {
                    let value = element.borrow();
                    value
                        .child_window()
                        .is_some_and(GeneratedWindow::is_generated_scroll_view)
                };
                let descendants = if is_scroll_view
                {
                    focusable_scope_descendants(&element)
                }
                else
                {
                    let value = element.borrow();
                    value
                        .child_window()
                        .map_or_else(Vec::new, focusable_descendants)
                };
                let target = self
                    .remembered_scope_descendant(&element, &descendants)
                    .or_else(||
                    {
                        let scope_frame = element.borrow().frame;
                        descendants.iter().find(|descendant|
                        {
                            let visible = scope_frame.intersect(descendant.borrow().frame);
                            visible.width > 0 && visible.height > 0
                        }).cloned()
                    })
                    .or_else(|| descendants.first().cloned());
                if let Some(target) = target
                {
                    self.scope_edit_element = Some(element.clone());
                    self.edit_mode = true;
                    self.focus_element(app.active_window(), &target);
                    self.remember_scope_descendant(&element, &target);
                    ensure_focused_visible_in_scroll(&element, &target);
                }
                else if is_scroll_view
                {
                    self.scope_edit_element = Some(element.clone());
                    self.edit_mode = true;
                    self.focus_element(app.active_window(), &element);
                }
            }
            _ => {}
        }
    }

    fn move_focus_within_scope(&mut self, window: &GeneratedWindow, direction: &str)
    {
        let Some(scope) = self.scope_edit_element.clone() else { return };
        let focusable = focusable_scope_descendants(&scope);
        let Some(current) = self.focused_element(window) else { return };
        let Some(index) = focusable
            .iter()
            .position(|element| std::rc::Rc::ptr_eq(element, &current))
        else
        {
            return;
        };
        if let Some(next) = spatial_focus_target_index(&focusable, index, direction)
        {
            if let Some(focused) = focusable.get(next).cloned()
            {
                self.focus_element(window, &focused);
                self.remember_scope_descendant(&scope, &focused);
                ensure_focused_visible_in_scroll(&scope, &focused);
            }
            return;
        }
        if !matches!(direction, "Up" | "Down")
        {
            return;
        }
        let scrolled =
        {
            let mut scope_value = scope.borrow_mut();
            if scope_value.kind() == ElementKind::ScrollView
            {
                scope_value.handle_key(direction)
            }
            else
            {
                scope_value.child_window_mut().is_some_and(|window|
                {
                    window.is_generated_scroll_view()
                        && window.handle_key(direction)
                })
            }
        };
        if !scrolled
        {
            return;
        }
        let refreshed = focusable_scope_descendants(&scope);
        let Some(current_index) = refreshed
            .iter()
            .position(|element| std::rc::Rc::ptr_eq(element, &current))
        else { return };
        if let Some(next) = spatial_focus_target_index(&refreshed, current_index, direction)
            && let Some(focused) = refreshed.get(next).cloned()
        {
            self.focus_element(window, &focused);
            self.remember_scope_descendant(&scope, &focused);
            ensure_focused_visible_in_scroll(&scope, &focused);
        }
    }

    fn confirm_scope_element<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
        confirmed: &ElementRef,
    )
    {
        let scope = self.scope_edit_element.clone();
        self.end_element_edit(confirmed, true);
        self.scope_dim_element = None;
        dispatch_confirm(app, self, options, confirmed);

        let Some(scope) = scope else { return };
        let scope_is_live = self
            .scope_edit_element
            .as_ref()
            .is_some_and(|current| Rc::ptr_eq(current, &scope))
            && window_contains_element(app.active_window(), &scope);
        if !scope_is_live
        {
            self.focused_index = -1;
            self.edit_mode = false;
            self.scope_edit_element = None;
            self.scope_dim_element = None;
            self.edit_snapshot = None;
            return;
        }

        let descendants = focusable_scope_descendants(&scope);
        let retained = descendants
            .iter()
            .find(|candidate| Rc::ptr_eq(candidate, confirmed))
            .cloned();
        let Some(retained) = retained else
        {
            self.scroll_view_last_descendant.remove(&scope.borrow().identity);
            if let Some(fallback) = descendants.first()
            {
                self.focus_element(app.active_window(), fallback);
                self.remember_scope_descendant(&scope, fallback);
                ensure_focused_visible_in_scroll(&scope, fallback);
            }
            else
            {
                self.focused_index = -1;
            }
            return;
        };

        self.focus_element(app.active_window(), &retained);
        self.remember_scope_descendant(&scope, &retained);
        ensure_focused_visible_in_scroll(&scope, &retained);
        let editable = matches!(
            retained.borrow().kind(),
            ElementKind::TextInput
                | ElementKind::TextArea
                | ElementKind::NumberInput
                | ElementKind::ComboBox
                | ElementKind::ListBox
        );
        if !options.keep_edit_mode_after_confirm || !editable
        {
            return;
        }

        self.begin_element_edit(&retained);
        self.scope_dim_element = Some(retained.clone());
        self.edit_mode = true;
        if let Some(id) = app.active_window().element_id(&retained)
        {
            dispatch_edit_started(app, options, &id);
        }
    }

    fn activate_scope_focused<A: GeneratedApplication>(
        &mut self,
        app: &mut A,
        options: &GeneratedWindowRuntimeOptions,
    )
    {
        let Some(element) = self.focused_element(app.active_window()) else { return };
        let kind = element.borrow().kind();
        match kind
        {
            ElementKind::Button | ElementKind::Image =>
            {
                let name = app
                    .active_window()
                    .element_id(&element)
                    .unwrap_or_else(|| element.borrow().name.clone());
                dispatch_button(app, self, options, &name);
            }
            ElementKind::CheckBox =>
            {
                let before = element_value(&element);
                element.borrow_mut().handle_key("Enter");
                dispatch_change_if_needed(app, self, options, &element, before);
            }
            ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::ComboBox
            | ElementKind::ListBox =>
            {
                self.begin_element_edit(&element);
                self.scope_dim_element = Some(element);
                self.edit_mode = true;
            }
            ElementKind::Reusable | ElementKind::ViewHost =>
            {
                let _ = activate_reusable_control(app, self, options, &element);
            }
            _ => {}
        }
    }
}

fn focus_rect_within_window(
    window: &mut GeneratedWindow,
    target: &ElementRef,
    viewport: Size,
) -> Option<Rect>
{
    if !window.is_generated_scroll_view()
    {
        resolve_layout_geometry(window, viewport, true);
    }
    for element in &window.elements
    {
        if std::rc::Rc::ptr_eq(element, target)
        {
            return Some(element.borrow().frame);
        }
    }
    for element in &window.elements
    {
        let frame = element.borrow().frame;
        let contains = element
            .borrow()
            .child_window()
            .is_some_and(|child| window_contains_element(child, target));
        if !contains
        {
            continue;
        }
        let mut element = element.borrow_mut();
        let child = element
            .child_window_mut()
            .expect("reusable child window disappeared while resolving focus");
        let inner = focus_rect_within_window(
            child,
            target,
            Size { width: max(1, frame.width), height: max(1, frame.height) },
        )?;
        return Some(Rect
        {
            row: frame.row + inner.row,
            col: frame.col + inner.col,
            width: inner.width,
            height: inner.height,
        });
    }
    None
}

fn focusable_scope_descendants(scope: &ElementRef) -> Vec<ElementRef>
{
    let scope_frame = scope.borrow().frame;
    if scope.borrow().kind() == ElementKind::ScrollView
    {
        let views = scope.borrow().child_views(Size
        {
            width: max(1, scope_frame.width),
            height: max(1, scope_frame.height),
        });
        let mut descendants = Vec::new();
        for view in views
        {
            let Some(child) = view.element else { continue };
            let child_frame = Rect
            {
                row: scope_frame.row + view.frame.row,
                col: scope_frame.col + view.frame.col,
                width: view.frame.width,
                height: view.frame.height,
            };
            child.borrow_mut().frame = child_frame;
            let child_focusable =
            {
                let value = child.borrow();
                value.enabled
                    && value.focusable
                    && is_focusable_type(&value.kind())
            };
            if child_focusable
            {
                descendants.push(child.clone());
            }
            let nested = child
                .borrow()
                .child_window()
                .map_or_else(Vec::new, focusable_descendants);
            for descendant in nested
            {
                let inner =
                {
                    let mut value = child.borrow_mut();
                    let Some(window) = value.child_window_mut() else { continue };
                    focus_rect_within_window(
                        window,
                        &descendant,
                        Size
                        {
                            width: max(1, child_frame.width),
                            height: max(1, child_frame.height),
                        },
                    )
                };
                let Some(inner) = inner else { continue };
                descendant.borrow_mut().frame = Rect
                {
                    row: child_frame.row + inner.row,
                    col: child_frame.col + inner.col,
                    width: inner.width,
                    height: inner.height,
                };
                descendants.push(descendant);
            }
        }
        return descendants;
    }
    let mut scope_value = scope.borrow_mut();
    let Some(window) = scope_value.child_window_mut() else { return Vec::new() };
    if !window.is_generated_scroll_view()
    {
        return focusable_descendants(window);
    }
    let viewport = Size
    {
        width: max(1, scope_frame.width),
        height: max(1, scope_frame.height),
    };
    let (_, _, _, max_offset) = layout_generated_scroll_children(window, viewport);
    let actual_skip = max_offset - window.view_offset().clamp(0, max_offset);
    let horizontal_skip = window.h_view_offset();
    let descendants = focusable_descendants(window);
    let mut resolved = Vec::new();
    for descendant in &descendants
    {
        let rows = window.scroll_view_children();
        let row = rows.iter().find(|row|
        {
            std::rc::Rc::ptr_eq(row, descendant)
                || row
                    .borrow()
                    .child_window()
                    .is_some_and(|child| window_contains_element(child, descendant))
        });
        let Some(row) = row.cloned() else { continue };
        let row_frame = row.borrow().frame;
        let inner = if std::rc::Rc::ptr_eq(&row, descendant)
        {
            Rect
            {
                row: 0,
                col: 0,
                width: row_frame.width,
                height: row_frame.height,
            }
        }
        else
        {
            let mut row_value = row.borrow_mut();
            let Some(row_window) = row_value.child_window_mut() else { continue };
            let Some(inner) = focus_rect_within_window(
                row_window,
                descendant,
                Size
                {
                    width: max(1, row_frame.width),
                    height: max(1, row_frame.height),
                },
            ) else { continue };
            inner
        };
        resolved.push((
            descendant.clone(),
            Rect
            {
                row: scope_frame.row + row_frame.row - actual_skip + inner.row,
                col: scope_frame.col + row_frame.col - horizontal_skip + inner.col,
                width: inner.width,
                height: inner.height,
            },
        ));
    }
    drop(scope_value);
    for (element, frame) in resolved
    {
        element.borrow_mut().frame = frame;
    }
    descendants
}

pub(crate) fn ensure_focused_visible_in_scroll(scope: &ElementRef, target: &ElementRef)
{
    let scope_frame = scope.borrow().frame;
    if scope.borrow().kind() == ElementKind::ScrollView
    {
        let top_level = scope.borrow().children.iter().find(|child|
        {
            Rc::ptr_eq(child, target)
                || element_tree_contains(child, target)
        }).cloned();
        if let Some(top_level) = top_level
        {
            let _ = scope.borrow().ensure_child_visible(
                &top_level,
                Size
                {
                    width: max(1, scope_frame.width),
                    height: max(1, scope_frame.height),
                },
            );
            let _ = focusable_scope_descendants(scope);
        }
        return;
    }
    let mut scope = scope.borrow_mut();
    let Some(window) = scope.child_window_mut() else { return };
    if !window.is_generated_scroll_view()
    {
        return;
    }
    let viewport = Size
    {
        width: max(1, scope_frame.width),
        height: max(1, scope_frame.height),
    };
    let (padding, _, content_height, max_offset) =
        layout_generated_scroll_children(window, viewport);
    let rows = window.scroll_view_children();
    let row = rows.iter().find(|child|
    {
        std::rc::Rc::ptr_eq(child, target)
            || child
                .borrow()
                .child_window()
                .is_some_and(|window| window_contains_element(window, target))
    });
    let Some(row) = row.cloned() else { return };
    let row_frame = row.borrow().frame;
    let target_frame = if std::rc::Rc::ptr_eq(&row, target)
    {
        row_frame
    }
    else
    {
        let mut row_value = row.borrow_mut();
        let Some(row_window) = row_value.child_window_mut() else { return };
        let Some(inner) = focus_rect_within_window(
            row_window,
            target,
            Size { width: max(1, row_frame.width), height: max(1, row_frame.height) },
        ) else { return };
        Rect
        {
            row: row_frame.row + inner.row,
            col: row_frame.col + inner.col,
            width: inner.width,
            height: inner.height,
        }
    };
    let mut visible_skip = max_offset - window.view_offset().clamp(0, max_offset);
    let visible_top = visible_skip + padding[0];
    let visible_bottom = visible_top + content_height;
    if target_frame.row < visible_top
    {
        visible_skip = max(0, target_frame.row - padding[0]);
    }
    else if target_frame.row + target_frame.height > visible_bottom
    {
        visible_skip = target_frame.row + target_frame.height - padding[0] - content_height;
    }
    visible_skip = visible_skip.clamp(0, max_offset);
    window.set_view_offset(max_offset - visible_skip);
    window.set_auto_scroll(false);
}

fn element_tree_contains(root: &ElementRef, target: &ElementRef) -> bool
{
    if Rc::ptr_eq(root, target)
    {
        return true;
    }
    let value = root.borrow();
    value
        .children
        .iter()
        .any(|child| element_tree_contains(child, target))
        || value
            .child_window()
            .is_some_and(|window| window_contains_element(window, target))
}

pub(crate) fn element_value(element: &ElementRef) -> Value
{
    let element = element.borrow();
    match element.kind()
    {
        ElementKind::CheckBox => Value::Bool(element.checked()),
        ElementKind::NumberInput => serde_json::json!(element.number_value()),
        ElementKind::ComboBox | ElementKind::ListBox => serde_json::json!(element.selected_values()),
        _ => Value::String(element.text().to_string()),
    }
}

fn active_callback_options(
    root: &GeneratedWindow,
    root_options: &GeneratedWindowRuntimeOptions,
) -> (GeneratedWindowRuntimeOptions, bool)
{
    root.window_stack.top().map_or_else(
        || (root_options.clone(), false),
        |frame| (frame.options.runtime_options(), true),
    )
}

pub(crate) fn active_window_point(root: &GeneratedWindow, position: Point) -> Point
{
    root.window_stack.top().map_or(position, |frame| Point
    {
        row: position.row - frame.bounds.row,
        col: position.col - frame.bounds.col,
    })
}

fn dispatch_preview_key<A: GeneratedApplication>(app: &mut A, event: &KeyEvent) -> bool
{
    let modal = app.window().window_stack.top().is_some();
    if app
        .active_window_mut()
        .behavior_handles_preview_key(event)
    {
        return true;
    }
    !modal && app.handle_preview_key(event)
}

fn dispatch_listbox_item_activate<A: GeneratedApplication>(
    app: &mut A,
    element: &ElementRef,
) -> bool
{
    let (name, options, active_index) =
    {
        let element = element.borrow();
        (
            element.name().to_string(),
            element.options().to_vec(),
            element.active_index(),
        )
    };
    if options.is_empty()
    {
        return false;
    }
    let index = (active_index.max(0) as usize).min(options.len() - 1);
    let value = options[index].clone();
    let element_id = app
        .active_window()
        .element_id(element)
        .unwrap_or_else(|| name.clone());
    if app
        .active_window_mut()
        .behavior_handles_listbox_item_activate(&name, &element_id, index, &value)
    {
        return true;
    }
    let modal = app.window().window_stack.top().is_some();
    !modal && app.handle_generated_listbox_item_activate(&name, &element_id, index, &value)
}

fn dispatch_key_before_focused<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    key: &str,
    focused_name: &str,
    edit_mode: bool,
) -> bool
{
    let (options, modal) = active_callback_options(app.window(), root_options);
    if options
        .on_key_before_focused_element
        .as_ref()
        .is_some_and(|callback| callback(key, focused_name, edit_mode))
    {
        return true;
    }
    if options
        .on_key_before_focused
        .as_ref()
        .is_some_and(|callback| callback(key))
    {
        return true;
    }
    if app
        .active_window_mut()
        .behavior_handles_key_before_focused(key, focused_name, edit_mode)
    {
        return true;
    }
    !modal && app.handle_key_before_focused(key, focused_name, edit_mode)
}

fn dispatch_key<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    key: &str,
) -> bool
{
    let (options, modal) = active_callback_options(app.window(), root_options);
    if app.active_window_mut().behavior_handles_key(key)
    {
        return true;
    }
    if !modal && app.handle_key(key)
    {
        return true;
    }
    options.on_key.as_ref().is_some_and(|callback| callback(key))
}

pub(crate) fn dispatch_focus_changed<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
    focused: bool,
)
{
    let (options, modal) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_focus_changed
    {
        callback(name, focused);
    }
    else if app
        .active_window_mut()
        .behavior_handles_focus_changed(name, focused)
    {
    }
    else if !modal
    {
        app.handle_focus_changed(name, focused);
    }
}

pub(crate) fn dispatch_edit_started<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
)
{
    let (options, _) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_edit_started
    {
        callback(name);
    }
    else
    {
        app.active_window_mut().behavior_handles_edit_started(name);
    }
}

pub(crate) fn dispatch_mouse_press_before_focused<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    position: Point,
) -> bool
{
    let (options, _) = active_callback_options(app.window(), root_options);
    if options
        .on_mouse_press_before_focused
        .as_ref()
        .is_some_and(|callback| callback(position))
    {
        return true;
    }
    app.active_window_mut().behavior_handles_mouse_press(position)
}

pub(crate) fn dispatch_mouse_wheel_before_focused<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    position: Point,
    delta: i32,
) -> bool
{
    let (options, _) = active_callback_options(app.window(), root_options);
    if options
        .on_mouse_wheel_before_focused
        .as_ref()
        .is_some_and(|callback| callback(position, delta))
    {
        return true;
    }
    app.active_window_mut()
        .behavior_handles_mouse_wheel(position, delta)
}

pub(crate) fn dispatch_mouse_wheel<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
    delta: i32,
) -> bool
{
    let (options, _) = active_callback_options(app.window(), root_options);
    options
        .on_mouse_wheel
        .as_ref()
        .is_some_and(|callback| callback(name, delta))
}

pub(crate) fn generated_application_should_close<A: GeneratedApplication>(
    app: &A,
    root_options: &GeneratedWindowRuntimeOptions,
) -> bool
{
    (app.window().should_close() || app.window().behavior_should_close())
        || root_options
            .should_close
            .as_ref()
        .is_some_and(|callback| callback())
}

pub(crate) fn close_completed_modal<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
) -> bool
{
    let mut closed = false;
    loop
    {
        if app.window().modal_depth() == 0
        {
            break;
        }
        let active = app.active_window();
        let runtime_options = active.runtime_options();
        let should_close = active.should_close()
            || active.behavior_should_close()
            || runtime_options
                .should_close
                .as_ref()
                .is_some_and(|callback| callback());
        if !should_close
        {
            break;
        }
        let closed_window = app.window_mut().close_active_window();
        if let Some(window) = closed_window
        {
            app.handle_generated_window_closed(window);
        }
        state.sync_active_window(app.window_mut(), root_options);
        state.repair_live_interaction(app.active_window());
        if app.window().modal_depth() == 0
        {
            cleanup_background_focus_after_modal_close(
                app,
                state,
                root_options,
            );
        }
        closed = true;
    }
    closed
}

fn cleanup_background_focus_after_modal_close<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
)
{
    let focused = state.focused_element(app.active_window());
    if state.edit_mode
    {
        let editing = state.scope_dim_element.clone().or_else(||
        {
            focused.as_ref().filter(|element|
            {
                let value = element.borrow();
                value.commit_mode == crate::COMMIT_MODE_LEAVE
                    || (value.kind() == ElementKind::ListBox && value.multiple())
            }).cloned()
        });
        if let Some(editing) = editing
        {
            state.commit_element_edit(&editing);
            dispatch_change_if_needed(
                app,
                state,
                root_options,
                &editing,
                Value::Null,
            );
        }
    }
    state.edit_snapshot = None;
    state.scope_dim_element = None;
    let Some(scope) = state.scope_edit_element.clone() else
    {
        state.edit_mode = false;
        return;
    };
    if !crate::is_live_element(&scope)
    {
        state.scope_edit_element = None;
        state.edit_mode = false;
        return;
    }
    state.edit_mode = true;
    if let Some(focused) = state.focused_element(app.active_window())
        && !Rc::ptr_eq(&scope, &focused)
    {
        state.remember_scope_descendant(&scope, &focused);
        ensure_focused_visible_in_scroll(&scope, &focused);
    }
}

pub(crate) fn dispatch_text_changed<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
    value: &str,
)
{
    let (options, modal) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_text_changed
    {
        callback(name, value);
    }
    else if app
        .active_window_mut()
        .behavior_handles_text_changed(name, value)
    {
    }
    else if !modal
    {
        app.handle_generated_text_changed(name, value);
    }
}

pub(crate) fn dispatch_selection_changed<A: GeneratedApplication>(
    app: &mut A,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
    values: &[String],
)
{
    let (options, modal) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_selection_changed
    {
        callback(name, values);
    }
    else if app
        .active_window_mut()
        .behavior_handles_selection_changed(name, values)
    {
    }
    else if !modal
    {
        app.handle_generated_selection_changed(name, values);
    }
}

pub(crate) fn dispatch_change_if_needed<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
    before: Value,
)
{
    let interaction_ids = state.interaction_element_ids(app.active_window());
    let after = element_value(element);
    if before == after
    {
        return;
    }
    let id = app
        .active_window()
        .element_id(element)
        .unwrap_or_else(|| element.borrow().name.clone());
    let (kind, value, number_value, checked, selected_values) =
    {
        let element = element.borrow();
        let selection_element = matches!(
            element.kind(),
            ElementKind::ComboBox | ElementKind::ListBox
        );
        (
            element.kind(),
            element.text().to_string(),
            if element.kind() == ElementKind::NumberInput
            {
                element.number_value()
            }
            else
            {
                0.0
            },
            element.checked(),
            if selection_element
            {
                element.selected_values()
            }
            else
            {
                Vec::new()
            },
        )
    };
    match kind
    {
        ElementKind::ComboBox | ElementKind::ListBox =>
        {
            dispatch_selection_changed(app, root_options, &id, &selected_values);
        }
        ElementKind::CheckBox =>
        {
            let value = if checked { "true" } else { "false" };
            dispatch_text_changed(app, root_options, &id, value);
        }
        ElementKind::NumberInput =>
        {
            let value = number_value.to_string();
            dispatch_text_changed(app, root_options, &id, &value);
        }
        _ =>
        {
            dispatch_text_changed(app, root_options, &id, &value);
        }
    };
    state.repair_live_interaction_with_ids(
        app.active_window(),
        interaction_ids,
        None,
    );
}

fn dispatch_confirm<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
)
{
    let interaction_ids = state.interaction_element_ids(app.active_window());
    let id = app
        .active_window()
        .element_id(element)
        .unwrap_or_else(|| element.borrow().name.clone());
    let value =
    {
        let element = element.borrow();
        match element.kind()
        {
            ElementKind::NumberInput => element.number_value().to_string(),
            _ => element.text().to_string(),
        }
    };
    let (options, modal) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_text_confirmed
    {
        callback(&id, &value);
    }
    else if app
        .active_window_mut()
        .behavior_handles_text_confirmed(&id, &value)
    {
    }
    else if !modal
    {
        app.handle_generated_text_confirmed(&id, &value);
    }
    state.repair_live_interaction_with_ids(
        app.active_window(),
        interaction_ids,
        None,
    );
}

pub(crate) fn dispatch_button<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
    name: &str,
)
{
    let interaction_ids = state.interaction_element_ids(app.active_window());
    let (options, modal) = active_callback_options(app.window(), root_options);
    if let Some(callback) = options.on_button
    {
        callback(name);
    }
    else
    {
        let handled = (modal && app.handle_active_window_button(name))
            || app.active_window_mut().behavior_handles_button(name);
        if !handled && !modal
        {
            app.handle_generated_button(name);
        }
    }
    state.repair_live_interaction_with_ids(
        app.active_window(),
        interaction_ids,
        None,
    );
}

fn native_element_tree_contains(
    root: &ElementRef,
    target: &ElementRef,
) -> bool
{
    if Rc::ptr_eq(root, target)
    {
        return true;
    }
    root.borrow()
        .children
        .iter()
        .any(|child| native_element_tree_contains(child, target))
}

fn window_directly_owns_element(
    window: &GeneratedWindow,
    target: &ElementRef,
) -> bool
{
    window
        .elements
        .iter()
        .any(|element| native_element_tree_contains(element, target))
}

fn nested_owner_handles_button(
    window: &mut GeneratedWindow,
    target: &ElementRef,
    local_name: &str,
) -> Option<bool>
{
    let elements = window.elements.clone();
    for element in elements
    {
        if native_element_tree_contains(&element, target)
        {
            return Some(window.behavior_handles_button(local_name));
        }
        let child_contains =
        {
            let value = element.borrow();
            value
                .child_window()
                .is_some_and(|child| window_contains_element(child, target))
        };
        if !child_contains
        {
            continue;
        }
        let mut value = element.borrow_mut();
        let child = value
            .child_window_mut()
            .expect("owner child window disappeared during button dispatch");
        return nested_owner_handles_button(child, target, local_name);
    }
    None
}

pub(crate) fn dispatch_element_button<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
)
{
    let interaction_ids = state.interaction_element_ids(app.active_window());
    let local_name = element.borrow().name.clone();
    let qualified_name = app
        .active_window()
        .element_id(element)
        .unwrap_or_else(|| local_name.clone());
    let root_owned =
        window_directly_owns_element(app.active_window(), element);
    if root_owned
    {
        let (options, modal) = active_callback_options(app.window(), root_options);
        if let Some(callback) = options.on_button
        {
            callback(&local_name);
        }
        else
        {
            let handled = (modal && app.handle_active_window_button(&local_name))
                || app
                    .active_window_mut()
                    .behavior_handles_button(&local_name);
            if !handled && !modal
            {
                app.handle_generated_button(&local_name);
            }
        }
    }
    else
    {
        let handled = nested_owner_handles_button(
            app.active_window_mut(),
            element,
            &local_name,
        )
        .unwrap_or(false);
        if !handled
        {
            // Rust moves generated child windows into their reusable hosts.
            // A child without object-owned behavior therefore uses the
            // qualified root hook as the smallest ownership adapter.
            app.handle_generated_button(&qualified_name);
        }
    }
    state.repair_live_interaction_with_ids(
        app.active_window(),
        interaction_ids,
        None,
    );
}

pub(crate) fn activate_reusable_control<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    root_options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
) -> bool
{
    let local_activation = {
        let value = element.borrow();
        if !value.enabled
            || !matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
        {
            return false;
        }
        value
            .child_window()
            .filter(|child| child.generated_focusable())
            .and_then(GeneratedWindow::activate_generated_control)
            .map(str::to_string)
    };
    let Some(local_activation) = local_activation else { return false };
    let host_id = app
        .active_window()
        .element_id(element)
        .unwrap_or_else(|| element.borrow().name.clone());
    let activation_id = if local_activation.is_empty()
    {
        host_id
    }
    else
    {
        format!("{host_id}.{local_activation}")
    };
    dispatch_button(app, state, root_options, &activation_id);
    true
}

pub(crate) fn is_focusable_type(kind: &ElementKind) -> bool
{
    matches!(
        kind,
        ElementKind::Button
            | ElementKind::CheckBox
            | ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::ComboBox
            | ElementKind::ListBox
            | ElementKind::ScrollView
    )
}

fn is_editable_element(element: &ElementRef) -> bool
{
    let value = element.borrow();
    if !value.enabled
    {
        return false;
    }
    if matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
    {
        return value
            .child_window()
            .is_some_and(GeneratedWindow::is_generated_scroll_view);
    }
    matches!(
        value.kind(),
        ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::ComboBox
            | ElementKind::ListBox
            | ElementKind::ScrollView
    )
}

pub fn focusable_elements(window: &GeneratedWindow) -> Vec<ElementRef>
{
    focusable_elements_for(window, None)
}

fn focusable_elements_for(
    window: &GeneratedWindow,
    active_scroll_view: Option<&ElementRef>,
) -> Vec<ElementRef>
{
    let mut result = Vec::new();
    for element in &window.elements
    {
        let value = element.borrow();
        if matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
            && let Some(child) = value.child_window()
        {
            if child.is_generated_scroll_view()
            {
                if value.enabled && value.focusable
                {
                    result.push(element.clone());
                }
                let is_active = active_scroll_view
                    .is_some_and(|active| std::rc::Rc::ptr_eq(active, element));
                drop(value);
                if is_active
                {
                    result.extend(focusable_scope_descendants(element));
                }
                continue;
            }
            let child_focusable = focusable_elements_for(child, active_scroll_view);
            if child_focusable.is_empty()
                && value.kind() != ElementKind::ViewHost
                && value.enabled
                && value.focusable
                && child.generated_focusable
            {
                result.push(element.clone());
            }
            else
            {
                result.extend(child_focusable);
            }
            continue;
        }
        if value.kind() == ElementKind::ScrollView
        {
            if value.enabled && value.focusable
            {
                result.push(element.clone());
            }
            let is_active = active_scroll_view
                .is_some_and(|active| std::rc::Rc::ptr_eq(active, element));
            drop(value);
            if is_active
            {
                result.extend(focusable_scope_descendants(element));
            }
            continue;
        }
        if value.enabled && value.focusable && is_focusable_type(&value.kind())
        {
            result.push(element.clone());
        }
    }
    result
}

pub(crate) fn clear_label_selections(window: &mut GeneratedWindow)
{
    for element in &window.elements
    {
        clear_element_label_selections(element);
    }
}

fn clear_element_label_selections(element: &ElementRef)
{
    let children = element.borrow().children.clone();
    {
        let mut value = element.borrow_mut();
        if matches!(
            value.kind(),
            ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel
        )
        {
            value.clear_selection();
        }
        if let Some(child) = value.child_window_mut()
        {
            clear_label_selections(child);
        }
    }
    for child in children
    {
        clear_element_label_selections(&child);
    }
}

fn focusable_descendants(window: &GeneratedWindow) -> Vec<ElementRef>
{
    fn append_element(
        element: &ElementRef,
        result: &mut Vec<ElementRef>,
        include_element: bool,
    )
    {
        let value = element.borrow();
        let child_focusable = value.child_window().map(focusable_descendants);
        let generated_control_focusable = matches!(value.kind(), ElementKind::Reusable)
            && value.enabled
            && value.focusable
            && value.child_window().is_some_and(|child|
            {
                child.generated_focusable()
                    && child_focusable.as_ref().is_some_and(Vec::is_empty)
            });
        if include_element
            && value.enabled
            && value.focusable
            && (is_focusable_type(&value.kind()) || generated_control_focusable)
        {
            result.push(element.clone());
        }
        let children = value.children.clone();
        if let Some(child_focusable) = child_focusable
        {
            result.extend(child_focusable);
        }
        drop(value);
        for child in children
        {
            append_element(&child, result, true);
        }
    }

    let mut result = Vec::new();
    let generated_scroll_view = window.generated_scroll_view();
    for element in &window.elements
    {
        let include_element = generated_scroll_view
            .as_ref()
            .is_none_or(|scroll_view| !Rc::ptr_eq(scroll_view, element));
        append_element(element, &mut result, include_element);
    }
    result
}

pub fn containing_scroll_proxy(
    window: &GeneratedWindow,
    target: &ElementRef,
) -> Option<ElementRef>
{
    for element in &window.elements
    {
        let value = element.borrow();
        let Some(child) = value.child_window() else { continue };
        if child.is_generated_scroll_view()
            && window_contains_element(child, target)
        {
            return Some(element.clone());
        }
        if let Some(found) = containing_scroll_proxy(child, target)
        {
            return Some(found);
        }
    }
    None
}

fn spatial_focus_target_index(focusable: &[ElementRef], current_index: usize, direction: &str) -> Option<usize>
{
    let current = focusable.get(current_index)?.borrow().frame;
    let current_row = current.row + current.height / 2;
    let current_col = current.col + current.width / 2;
    let mut best: Option<(i32, i32, i32, i32, usize)> = None;
    for (index, candidate) in focusable.iter().enumerate()
    {
        if index == current_index { continue; }
        let candidate = candidate.borrow().frame;
        let candidate_row = candidate.row + candidate.height / 2;
        let candidate_col = candidate.col + candidate.width / 2;
        let (primary, band, gap, edge) = match direction
        {
            "Up" if candidate_row < current_row
                && candidate.row + candidate.height <= current.row =>
            {
                let Some(band) = band_scan_rank(
                    current.col,
                    current.col + current.width,
                    candidate.col,
                    candidate.col + candidate.width,
                ) else { continue };
                (
                    max(0, current.row - (candidate.row + candidate.height)),
                    band,
                    axis_gap(
                        current.col,
                        current.col + current.width,
                        candidate.col,
                        candidate.col + candidate.width,
                    ),
                    (candidate.col - current.col).abs(),
                )
            }
            "Down" if candidate_row > current_row
                && candidate.row >= current.row + current.height =>
            {
                let Some(band) = band_scan_rank(
                    current.col,
                    current.col + current.width,
                    candidate.col,
                    candidate.col + candidate.width,
                ) else { continue };
                (
                    max(0, candidate.row - (current.row + current.height)),
                    band,
                    axis_gap(
                        current.col,
                        current.col + current.width,
                        candidate.col,
                        candidate.col + candidate.width,
                    ),
                    (candidate.col - current.col).abs(),
                )
            }
            "Left" if candidate_col < current_col
                && candidate.col + candidate.width <= current.col =>
            {
                let Some(band) = band_scan_rank(
                    current.row,
                    current.row + current.height,
                    candidate.row,
                    candidate.row + candidate.height,
                ) else { continue };
                (
                    max(0, current.col - (candidate.col + candidate.width)),
                    band,
                    axis_gap(
                        current.row,
                        current.row + current.height,
                        candidate.row,
                        candidate.row + candidate.height,
                    ),
                    (candidate.row - current.row).abs(),
                )
            }
            "Right" if candidate_col > current_col
                && candidate.col >= current.col + current.width =>
            {
                let Some(band) = band_scan_rank(
                    current.row,
                    current.row + current.height,
                    candidate.row,
                    candidate.row + candidate.height,
                ) else { continue };
                (
                    max(0, candidate.col - (current.col + current.width)),
                    band,
                    axis_gap(
                        current.row,
                        current.row + current.height,
                        candidate.row,
                        candidate.row + candidate.height,
                    ),
                    (candidate.row - current.row).abs(),
                )
            }
            _ => continue,
        };
        let rank = (band, primary, gap, edge, index);
        if best.is_none_or(|current_best| rank < current_best)
        {
            best = Some(rank);
        }
    }
    best.map(|value| value.4)
}

fn axis_overlaps(start_a: i32, end_a: i32, start_b: i32, end_b: i32) -> bool
{
    max(start_a, start_b) < min(end_a, end_b)
}

fn axis_gap(start_a: i32, end_a: i32, start_b: i32, end_b: i32) -> i32
{
    if end_a <= start_b
    {
        start_b - end_a
    }
    else if end_b <= start_a
    {
        start_a - end_b
    }
    else
    {
        0
    }
}

fn band_scan_rank(
    current_start: i32,
    current_end: i32,
    candidate_start: i32,
    candidate_end: i32,
) -> Option<i32>
{
    if axis_overlaps(current_start, current_end, candidate_start, candidate_end)
    {
        return Some(0);
    }

    let max_distance = [
        (candidate_start - current_start).abs(),
        (candidate_end - current_end).abs(),
        (candidate_start - current_end).abs(),
        (candidate_end - current_start).abs(),
    ]
    .into_iter()
    .max()
    .unwrap_or_default()
        + max(1, current_end - current_start)
        + max(1, candidate_end - candidate_start);

    let mut rank = 1;
    for distance in 1..=max_distance
    {
        for offset in [distance, -distance]
        {
            if axis_overlaps(
                current_start + offset,
                current_end + offset,
                candidate_start,
                candidate_end,
            )
            {
                return Some(rank);
            }
            rank += 1;
        }
    }
    None
}

struct EmbeddedApplication
{
    window: GeneratedWindow,
}

impl GeneratedApplication for EmbeddedApplication
{
    fn window(&self) -> &GeneratedWindow { &self.window }
    fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
}

struct BorrowedEmbeddedApplication<'a>
{
    window: &'a mut GeneratedWindow,
}

impl GeneratedApplication for BorrowedEmbeddedApplication<'_>
{
    fn window(&self) -> &GeneratedWindow { self.window }
    fn window_mut(&mut self) -> &mut GeneratedWindow { self.window }
}

pub fn render_embedded_window(window: &GeneratedWindow, width: i32, height: i32) -> TerminalBuffer
{
    let mut app = EmbeddedApplication { window: window.clone() };
    let options = app.window.runtime_options();
    let mut state = RuntimeState::new(
        &app.window,
        &options,
        Size { width: max(1, width), height: max(1, height) },
    );
    render_generated_window_internal(&mut app, &mut state, &options, true, false)
}

pub fn render_generated_window_content(
    window: &mut GeneratedWindow,
    width: i32,
    height: i32,
) -> TerminalBuffer
{
    let options = window.runtime_options();
    let mut state = RuntimeState::new(
        window,
        &options,
        Size
        {
            width: max(1, width),
            height: max(1, height),
        },
    );
    let mut app = BorrowedEmbeddedApplication { window };
    render_generated_window(&mut app, &mut state, &options)
}

pub(crate) fn window_contains_element(window: &GeneratedWindow, target: &ElementRef) -> bool
{
    for element in &window.elements
    {
        if element_tree_contains(element, target)
        {
            return true;
        }
    }
    false
}

fn append_element_tree(element: &ElementRef, result: &mut Vec<ElementRef>)
{
    result.push(element.clone());
    let value = element.borrow();
    let children = value.children.clone();
    if let Some(window) = value.child_window()
    {
        append_window_elements(window, result);
    }
    drop(value);
    for child in children
    {
        append_element_tree(&child, result);
    }
}

fn append_window_elements(window: &GeneratedWindow, result: &mut Vec<ElementRef>)
{
    for element in &window.elements
    {
        append_element_tree(element, result);
    }
}

fn all_window_elements(window: &GeneratedWindow) -> Vec<ElementRef>
{
    let mut elements = Vec::new();
    append_window_elements(window, &mut elements);
    elements
}

fn scroll_child_id(child: &ElementRef, target: &ElementRef) -> Option<String>
{
    if Rc::ptr_eq(child, target)
    {
        return Some(child.borrow().name.clone());
    }
    let value = child.borrow();
    if let Some(window) = value.child_window()
        && let Some(id) = element_id_for_element(window, target)
    {
        return Some(id);
    }
    for nested in &value.children
    {
        if let Some(id) = scroll_child_id(nested, target)
        {
            return Some(id);
        }
    }
    None
}

fn scroll_view_child_id(
    scroll_name: &str,
    children: &[ElementRef],
    target: &ElementRef,
) -> Option<String>
{
    for (index, child) in children.iter().enumerate()
    {
        if let Some(id) = scroll_child_id(child, target)
        {
            return Some(format!("{scroll_name}[{index}].{id}"));
        }
    }
    None
}

fn element_id_for_element(
    window: &GeneratedWindow,
    target: &ElementRef,
) -> Option<String>
{
    for element in &window.elements
    {
        if Rc::ptr_eq(element, target)
        {
            return Some(element.borrow().name.clone());
        }
        let value = element.borrow();
        if matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
            && let Some(child_window) = value.child_window()
        {
            let host_name = value.name.clone();
            if let Some(scroll_view) = child_window.generated_scroll_view()
            {
                if Rc::ptr_eq(&scroll_view, target)
                {
                    return Some(host_name);
                }
                if let Some(id) = scroll_view_child_id(
                    &host_name,
                    &scroll_view.borrow().children,
                    target,
                )
                {
                    return Some(id);
                }
            }
            if let Some(id) = element_id_for_element(child_window, target)
            {
                return Some(format!("{host_name}.{id}"));
            }
        }
        if value.kind() == ElementKind::ScrollView
            && let Some(id) = scroll_view_child_id(
                &value.name,
                &value.children,
                target,
            )
        {
            return Some(id);
        }
    }
    None
}

fn focused_element_is_editing(state: &RuntimeState, element: &ElementRef) -> bool
{
    state.edit_mode
        && !state.suppress_active_scroll_scope_visuals
        && (state.scope_edit_element.is_none()
            || state
                .scope_dim_element
                .as_ref()
                .is_some_and(|editing| std::rc::Rc::ptr_eq(editing, element)))
}

fn append_window_base_background(backgrounds: &mut Vec<Color>, background: &Option<Color>)
{
    let Some(background) = background else { return };
    if background.empty() || background.translucent()
    {
        return;
    }
    if !backgrounds
        .iter()
        .any(|known| known.normalized() == background.normalized())
    {
        backgrounds.push(background.clone());
    }
}

fn collect_window_base_backgrounds(window: &GeneratedWindow, backgrounds: &mut Vec<Color>)
{
    append_window_base_background(backgrounds, &window.window_style.background);
    if window.is_generated_scroll_view()
    {
        append_window_base_background(
            backgrounds,
            &window.generated_scroll_view_style().background,
        );
    }
    for item in &window.layout
    {
        append_window_base_background(backgrounds, &item.cell_style.background);
    }
}

fn apply_focus_to_descendant_backgrounds(
    buffer: &mut TerminalBuffer,
    focus: &Color,
    backgrounds: &[Color],
)
{
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            for background in backgrounds
            {
                let focused = if focus.has_partial_alpha()
                {
                    focus.blend_over(background)
                }
                else
                {
                    focus.clone()
                };
                if cell
                    .background
                    .as_ref()
                    .is_some_and(|cell_background|
                        cell_background.normalized() == background.normalized())
                {
                    cell.background = Some(focused);
                    break;
                }
                if cell
                    .background
                    .as_ref()
                    .is_some_and(|cell_background|
                        cell_background.normalized() == focused.normalized())
                {
                    break;
                }
            }
        }
    }
}

fn apply_scroll_focus_background_gaps(
    buffer: &mut TerminalBuffer,
    focus: &Color,
    base: &Option<Color>,
    padding: [i32; 4],
)
{
    if focus.empty()
    {
        return;
    }
    let focused_base = base.as_ref().map(|base|
    {
        if focus.has_partial_alpha()
        {
            focus.blend_over(base)
        }
        else
        {
            focus.clone()
        }
    });
    let transparent_viewport = base.translucent();
    for row in 0..buffer.height
    {
        let row_has_content = (0..buffer.width).any(|col|
        {
            buffer.cell(row, col).is_some_and(|cell|
            {
                !cell.raw.is_empty()
                    || !matches!(cell.text.as_str(), " " | "^" | "v")
            })
        });
        let protected_start = padding[3].clamp(0, buffer.width);
        let protected_end = (buffer.width - padding[1] - 1).clamp(-1, buffer.width - 1);
        for col in 0..buffer.width
        {
            if transparent_viewport
                && row_has_content
                && col >= protected_start
                && col <= protected_end
            {
                continue;
            }
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            if focused_base
                .as_ref()
                .is_some_and(|focused_base|
                    cell.background.as_ref() == Some(focused_base))
            {
                continue;
            }
            if cell.background == *base
                || (base.as_ref().is_some_and(Color::is_transparent)
                    && cell.background.unset_or_transparent())
            {
                cell.background = focused_base
                    .clone()
                    .or_else(|| Some(focus.clone()));
            }
        }
    }
}

fn same_rendered_color(left: &Color, right: &Color) -> bool
{
    match (left.rgba(), right.rgba())
    {
        (Some(left), Some(right)) => left == right,
        _ => left.normalized() == right.normalized(),
    }
}

fn focused_background(focus: &Color, base: &Color) -> Color
{
    if focus.rgba().is_some()
    {
        focus.blend_over(base)
    }
    else
    {
        focus.clone()
    }
}

fn apply_reusable_focus_background(
    buffer: &mut TerminalBuffer,
    focus: &Color,
    base: Option<&Color>,
)
{
    if focus.empty()
    {
        return;
    }
    let focused_base = base.map(|base| focused_background(focus, base));
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            if focused_base.as_ref().is_some_and(|focused_base|
                cell.background.as_ref().is_some_and(|background|
                    same_rendered_color(background, focus))
                    && !same_rendered_color(focus, focused_base))
            {
                cell.background =
                    Some(focused_background(focus, focused_base.as_ref().unwrap()));
                continue;
            }
            let matches_base = cell.background.is_none()
                || base.is_some_and(|base|
                    cell.background.as_ref().is_some_and(|background|
                        same_rendered_color(background, base)));
            let matches_focused_base = focused_base.as_ref().is_some_and(|focused_base|
                cell.background.as_ref().is_some_and(|background|
                    same_rendered_color(background, focused_base)));
            if !matches_base || matches_focused_base
            {
                continue;
            }
            cell.background = if let Some(cell_background) = cell
                .background
                .as_ref()
                .filter(|_| focus.rgba().is_some())
            {
                Some(focused_background(focus, cell_background))
            }
            else
            {
                Some(focus.clone())
            };
        }
    }
}

fn append_unique_opaque_background(backgrounds: &mut Vec<Color>, background: &Option<Color>)
{
    let Some(background) = background else { return };
    if background.rgba().is_none_or(|rgba| rgba[3] != u8::MAX)
        || backgrounds.iter().any(|candidate| same_rendered_color(candidate, background))
    {
        return;
    }
    backgrounds.push(background.clone());
}

fn collect_descendant_base_style_backgrounds(
    window: &GeneratedWindow,
    backgrounds: &mut Vec<Color>,
)
{
    for element in &window.elements
    {
        let element = element.borrow();
        append_unique_opaque_background(backgrounds, &element.style.background);
        if let Some(style) = &element.focus_style
        {
            append_unique_opaque_background(backgrounds, &style.background);
        }
        if let Some(style) = &element.edit_style
        {
            append_unique_opaque_background(backgrounds, &style.background);
        }
        if let Some(child) = element.child_window()
        {
            collect_descendant_base_style_backgrounds(child, backgrounds);
        }
    }
}

fn relative_element_render_clip(
    element: Rect,
    clip: Rect,
    render_clip_top: Option<i32>,
    render_clip_bottom: Option<i32>,
    buffer_height: i32,
) -> (Option<i32>, Option<i32>)
{
    let base_clip_top = max(0, clip.row);
    let base_clip_bottom = max(
        base_clip_top,
        min(buffer_height, clip.row + clip.height),
    );
    let absolute_clip_top = max(
        base_clip_top,
        render_clip_top.unwrap_or(base_clip_top),
    );
    let absolute_clip_bottom = max(
        absolute_clip_top,
        min(
            base_clip_bottom,
            render_clip_bottom.unwrap_or(base_clip_bottom),
        ),
    );
    let visible_top = max(element.row, absolute_clip_top);
    let visible_bottom = min(element.row + element.height, absolute_clip_bottom);
    let local_clip_top = max(0, visible_top - element.row);
    let local_clip_bottom = max(0, visible_bottom - element.row);
    if local_clip_top == 0 && local_clip_bottom == element.height
    {
        (None, None)
    }
    else
    {
        (Some(local_clip_top), Some(local_clip_bottom))
    }
}

fn apply_reusable_focus_to_descendant_backgrounds(
    buffer: &mut TerminalBuffer,
    focus: &Color,
    backgrounds: &[Color],
)
{
    if focus.empty() || backgrounds.is_empty()
    {
        return;
    }
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            for background in backgrounds
            {
                let focused = focused_background(focus, background);
                if cell.background.as_ref().is_some_and(|cell_background|
                    same_rendered_color(cell_background, background)
                        || same_rendered_color(cell_background, &focused))
                {
                    cell.background = Some(focused);
                    break;
                }
            }
        }
    }
}

fn apply_generated_scroll_view_focus_underlay_to_structural_backgrounds(
    buffer: &mut TerminalBuffer,
    structural_background: Option<&Color>,
    focus_background: Option<&Color>,
)
{
    let (Some(structural_background), Some(focus_background)) =
        (structural_background, focus_background)
    else
    {
        return;
    };
    let focused = focused_background(focus_background, structural_background);
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            if cell.background.is_none()
                || cell.background.as_ref().is_some_and(|background|
                    same_rendered_color(background, structural_background))
            {
                cell.background = Some(focused.clone());
            }
        }
    }
}

#[allow(clippy::too_many_arguments)]
fn render_embedded_window_with_interaction(
    window: &mut GeneratedWindow,
    width: i32,
    height: i32,
    parent_state: &RuntimeState,
    focused_target_id: Option<String>,
    focus_background_override: Option<Color>,
    clip_top: Option<i32>,
    clip_bottom: Option<i32>,
    use_host_viewport_for_root_scroll_indicators: bool,
) -> TerminalBuffer
{
    let options = window.runtime_options();
    let render_height = if window.is_generated_scroll_view()
    {
        max(1, height)
    }
    else
    {
        max(
            max(1, height),
            generated_window_content_size_for_width(window, width).height,
        )
    };
    let mut state = RuntimeState::new(
        window,
        &options,
        Size { width: max(1, width), height: render_height },
    );
    state.focused_index = -1;
    state.edit_mode = false;
    state.scope_dim_element = None;
    state.scope_edit_element = None;
    state.edit_snapshot = None;
    state.render_background_override = focus_background_override
        .filter(|_| !parent_state.suppress_active_scroll_scope_visuals)
        .filter(|background| !background.empty());
    state.render_clip_top = clip_top;
    state.render_clip_bottom = clip_bottom;
    state.suppress_active_scroll_scope_visuals =
        parent_state.suppress_active_scroll_scope_visuals;
    if let Some(focused_id) = focused_target_id
        && let Some(focused) = window.find_element(&focused_id)
    {
        if parent_state.scope_edit_element.is_some()
        {
            state.scope_edit_element = window
                .generated_scroll_view()
                .map(Into::into)
                .or_else(||
                {
                    let value = focused.borrow();
                    value
                        .child_window()
                        .is_some_and(GeneratedWindow::is_generated_scroll_view)
                        .then(|| focused.clone())
                })
                .or_else(|| parent_state.scope_edit_element.clone());
        }
        state.focus_element(window, &focused);
        state.edit_mode = parent_state.edit_mode;
        state.scope_dim_element = parent_state.scope_dim_element.clone();
        state.edit_snapshot = parent_state.edit_snapshot.clone();
        state.render_inside_scope =
            parent_state.render_inside_scope || parent_state.scope_edit_element.is_some();
        if state.render_background_override.is_none()
            && !state.suppress_active_scroll_scope_visuals
        {
            state.render_background_override =
            {
                let focus_style = window
                    .is_generated_scroll_view()
                    .then(|| window.generated_scroll_view_focus_style());
                if focus_style
                    .as_ref()
                    .is_some_and(|style| style.background.is_some())
                {
                    focus_style.and_then(|style| style.background)
                }
                else
                {
                    None
                }
            };
        }
    }
    let mut app = BorrowedEmbeddedApplication { window };
    render_generated_window_with_root_scroll_indicator_mode(
        &mut app,
        &mut state,
        &options,
        true,
        use_host_viewport_for_root_scroll_indicators,
    )
}

fn layout_generated_scroll_children(
    window: &mut GeneratedWindow,
    viewport: Size,
) -> ([i32; 4], i32, i32, i32)
{
    let scroll_view = window.scroll_view_ref();
    let style = scroll_view.borrow().style.clone();
    let width = max(1, viewport.width);
    let height = max(1, viewport.height);
    let padding = style.padding_values();
    let gap = max(0, style.gap.unwrap_or(0));
    let content_width = max(1, width - padding[1] - padding[3]);
    let content_height = max(1, height - padding[0] - padding[2]);
    scroll_view.borrow_mut().frame = Rect
    {
        row: 0,
        col: 0,
        width,
        height,
    };
    scroll_view.set_gap(gap);
    scroll_view.refresh_dynamic_children(content_width);
    let native_heights = window.scroll_view_native_child_heights(content_width);
    let mut next_row = padding[0];
    for child in scroll_view.rendered_children()
    {
        next_row += child.height + gap;
    }
    for (index, child) in window.scroll_view_children().iter().enumerate()
    {
        let child_height = native_heights.get(index).copied().unwrap_or(1);
        child.borrow_mut().frame = Rect
        {
            row: next_row,
            col: padding[3],
            width: content_width,
            height: child_height,
        };
        next_row += child_height + gap;
    }
    let max_offset = scroll_view.max_view_offset(Size { width, height });
    (
        padding,
        content_width,
        content_height,
        max_offset,
    )
}

fn render_generated_scroll_window(
    window: &mut GeneratedWindow,
    state: &RuntimeState,
    viewport: Size,
    use_host_viewport_for_root_scroll_indicators: bool,
) -> TerminalBuffer
{
    let width = max(1, viewport.width);
    let height = max(1, viewport.height);
    let natural = natural_content_size(window);
    let scroll_viewport = Size
    {
        width: max(width, natural.width),
        height: max(height, natural.height),
    };
    let scroll_view = window.scroll_view_ref();
    let mut style = scroll_view.borrow().style.clone();
    if style.background.is_none()
    {
        style.background = window.window_style.background.clone();
    }
    let foreground = style.color.clone();
    let background = style.background.clone();
    let mut buffer = TerminalBuffer::new(width, height);
    buffer.fill(
        Rect { row: 0, col: 0, width, height },
        ' ',
        &foreground,
        &background,
    );
    let (padding, content_width, content_height, _) =
        layout_generated_scroll_children(window, scroll_viewport);
    let scroll_clip_top = max(
        padding[0],
        state.render_clip_top.unwrap_or(padding[0]),
    );
    let scroll_clip_bottom = max(
        scroll_clip_top,
        min(
            padding[0] + content_height,
            state
                .render_clip_bottom
                .unwrap_or(padding[0] + content_height),
        ),
    );
    let focused = state.focused_element(window);
    let content_window = scroll_view
        .content_window(scroll_viewport)
        .unwrap_or_default();
    let actual_skip = content_window.actual_skip;
    let max_offset = content_window.natural_skip;
    let child_views = scroll_view.child_views(scroll_viewport);
    let mut descendant_combo_box: Option<(ElementRef, Rect)> = None;
    let mut rendered_child_row = padding[0];
    {
        let rendered_children = scroll_view.rendered_children();
        for child in &rendered_children
        {
            let render_row = rendered_child_row - actual_skip;
            if render_row + child.height > scroll_clip_top
                && render_row < scroll_clip_bottom
            {
                buffer.overlay_clipped(
                    child,
                    render_row,
                    padding[3] - window.h_view_offset(),
                    Rect
                    {
                        row: scroll_clip_top,
                        col: padding[3],
                        width: content_width,
                        height: max(0, scroll_clip_bottom - scroll_clip_top),
                    },
                );
            }
            rendered_child_row += child.height + max(0, style.gap.unwrap_or(0));
        }
    }
    for view in child_views.iter().filter(|view| view.element.is_some())
    {
        let child = view
            .element
            .as_ref()
            .expect("native ScrollView child view lost its element");
        let frame = view.frame;
        let render_row = frame.row;
        if render_row + frame.height <= scroll_clip_top
            || render_row >= scroll_clip_bottom
        {
            continue;
        }
        child.borrow_mut().frame = frame;
        let child_clip_top = max(0, scroll_clip_top - render_row);
        let child_clip_bottom = max(
            child_clip_top,
            min(frame.height, scroll_clip_bottom - render_row),
        );
        let child_clip = if child_clip_top == 0 && child_clip_bottom == frame.height
        {
            (None, None)
        }
        else
        {
            (Some(child_clip_top), Some(child_clip_bottom))
        };
        let parent_background = buffer
            .cell(render_row, frame.col - window.h_view_offset())
            .and_then(|cell| cell.background.clone());
        let is_focused = focused
            .as_ref()
            .is_some_and(|focused| std::rc::Rc::ptr_eq(focused, child));
        let rendered = crate::with_element_parent_background(parent_background, ||
        {
            let has_child_window = child.borrow().child_window().is_some();
            if has_child_window
            {
                let focused_combo_in_child = focused.as_ref().is_some_and(|focused|
                {
                    !std::rc::Rc::ptr_eq(focused, child)
                        && focused.borrow().kind() == ElementKind::ComboBox
                        && focused_element_is_editing(state, focused)
                        && child
                            .borrow()
                            .child_window()
                            .is_some_and(|window| window_contains_element(window, focused))
                });
                let child_focus_background = state
                    .render_background_override
                    .as_ref()
                    .filter(|background| background.has_partial_alpha())
                    .filter(|_|
                    {
                        focused.as_ref().is_some_and(|focused|
                        {
                            let child_value = child.borrow();
                            let focused_in_child = std::rc::Rc::ptr_eq(focused, child)
                                || child_value
                                    .child_window()
                                    .is_some_and(|window| window_contains_element(window, focused));
                            let has_multiple_focusable = child_value
                                .child_window()
                                .is_some_and(|window| focusable_descendants(window).len() > 1);
                            focused_in_child && !has_multiple_focusable
                        })
                    })
                    .cloned();
                let focused_id =
                {
                    let child_value = child.borrow();
                    child_value.child_window().and_then(|child_window|
                    {
                        focused
                            .as_ref()
                            .and_then(|focused| child_window.element_id(focused))
                    })
                };
                let mut child_value = child.borrow_mut();
                let use_host_viewport_for_root_scroll_indicators =
                    child_value.kind() == ElementKind::ViewHost
                        && child_value
                            .child_window()
                            .is_some_and(GeneratedWindow::is_generated_scroll_view);
                let child_window = child_value
                    .child_window_mut()
                    .expect("reusable child window disappeared while rendering");
                let mut child_base_backgrounds = Vec::new();
                if child_focus_background.is_some()
                {
                    collect_window_base_backgrounds(child_window, &mut child_base_backgrounds);
                }
                let mut rendered = render_embedded_window_with_interaction(
                    child_window,
                    frame.width,
                    frame.height,
                    state,
                    focused_id,
                    child_focus_background.clone(),
                    child_clip.0,
                    child_clip.1,
                    use_host_viewport_for_root_scroll_indicators,
                );
                translate_window_frames(
                    child_window,
                    render_row,
                    frame.col - window.h_view_offset(),
                );
                drop(child_value);
                if let Some(focus) = child_focus_background.as_ref()
                {
                    apply_focus_to_descendant_backgrounds(
                        &mut rendered,
                        focus,
                        &child_base_backgrounds,
                    );
                }
                if focused_combo_in_child
                {
                    let focused = focused
                        .as_ref()
                        .expect("focused ComboBox disappeared while rendering");
                    descendant_combo_box = Some((focused.clone(), focused.borrow().frame));
                }
                rendered
            }
            else
            {
                child.borrow().render_with_state(
                    frame.width,
                    frame.height,
                    ElementRenderState
                    {
                        focused: is_focused,
                        edit_mode: is_focused && focused_element_is_editing(state, child),
                        passive_focus: state.scope_edit_element.is_some()
                            && is_focused
                            && !focused_element_is_editing(state, child),
                        clip_top: child_clip.0,
                        clip_bottom: child_clip.1,
                    },
                )
            }
        });
        buffer.overlay_clipped(
            &rendered,
            render_row,
            frame.col - window.h_view_offset(),
            Rect {
                row: scroll_clip_top,
                col: padding[3],
                width: content_width,
                height: max(0, scroll_clip_bottom - scroll_clip_top),
            },
        );
    }
    if let Some((combo_box, frame)) = descendant_combo_box
    {
        overlay_focused_combo_box(
            &mut buffer,
            &combo_box,
            frame,
            Rect {
                row: padding[0],
                col: padding[3],
                width: content_width,
                height: content_height,
            },
        );
    }
    if !use_host_viewport_for_root_scroll_indicators
    {
        let indicator_col = max(padding[3], width - padding[1] - 1);
        if actual_skip > 0
        {
            draw_scroll_indicator(
                &mut buffer,
                padding[0],
                indicator_col,
                padding[3],
                "^",
                &foreground,
                &background,
            );
        }
        if actual_skip < max_offset
        {
            draw_scroll_indicator(
                &mut buffer,
                padding[0] + content_height - 1,
                indicator_col,
                padding[3],
                "v",
                &foreground,
                &background,
            );
        }
    }
    else if let Some(item) = window.layout.first()
    {
        let source_viewport = Rect
        {
            row: item.cell_row + padding[0],
            col: item.cell_col + padding[3],
            width: min(
                max(0, item.cell_chars_width - padding[3] - padding[1]),
                max(0, width - item.cell_col - padding[3]),
            ),
            height: min(
                max(0, item.cell_chars_height - padding[0] - padding[2]),
                max(0, height - item.cell_row - padding[0]),
            ),
        };
        if source_viewport.width > 0 && source_viewport.height > 0
        {
            let child_content_above = child_views
                .iter()
                .filter(|view| view.element.is_some())
                .any(|view| view.frame.row < source_viewport.row);
            let child_content_below = child_views
                .iter()
                .filter(|view| view.element.is_some())
                .any(|view|
                {
                    view.frame.row + view.frame.height
                        > source_viewport.row + source_viewport.height
                });
            let indicator_col =
                source_viewport.col + source_viewport.width - 1;
            if actual_skip > 0 || child_content_above
            {
                draw_scroll_indicator(
                    &mut buffer,
                    source_viewport.row,
                    indicator_col,
                    source_viewport.col,
                    "^",
                    &foreground,
                    &background,
                );
            }
            if actual_skip < max_offset || child_content_below
            {
                draw_scroll_indicator(
                    &mut buffer,
                    source_viewport.row + source_viewport.height - 1,
                    indicator_col,
                    source_viewport.col,
                    "v",
                    &foreground,
                    &background,
                );
            }
        }
    }
    if let Some(focus) = state.render_background_override.as_ref()
    {
        apply_scroll_focus_background_gaps(
            &mut buffer,
            focus,
            &style.background,
            padding,
        );
    }
    buffer
}

fn draw_scroll_indicator(
    buffer: &mut TerminalBuffer,
    row: i32,
    col: i32,
    foreground_start_col: i32,
    indicator: &str,
    fallback_foreground: &Option<Color>,
    fallback_background: &Option<Color>,
)
{
    let (mut foreground, background) = buffer
        .cell(row, col)
        .map(|cell| (cell.foreground.clone(), cell.background.clone()))
        .unwrap_or_else(|| (fallback_foreground.clone(), fallback_background.clone()));
    if foreground.unset_or_transparent()
    {
        for scan_col in foreground_start_col..=col
        {
            if let Some(cell) = buffer.cell(row, scan_col)
                && !cell.foreground.unset_or_transparent()
            {
                foreground = cell.foreground.clone();
            }
        }
    }
    buffer.draw_text(row, col, indicator, &foreground, &background, 1);
}

fn translate_window_frames(window: &GeneratedWindow, row_offset: i32, col_offset: i32)
{
    for element in &window.elements
    {
        let mut value = element.borrow_mut();
        value.frame.row += row_offset;
        value.frame.col += col_offset;
        if let Some(child) = value.child_window()
        {
            translate_window_frames(child, row_offset, col_offset);
        }
    }
}

fn sync_element_descendant_frames(element: &ElementRef)
{
    let (kind, frame, child_views) =
    {
        let mut value = element.borrow_mut();
        let frame = value.frame;
        if let Some(child) = value.child_window_mut()
        {
            sync_window_element_frames_to(child, frame);
            return;
        }
        let kind = value.kind();
        let child_views = (kind == ElementKind::ScrollView).then(||
        {
            value.child_views(Size
            {
                width: frame.width,
                height: frame.height,
            })
        });
        (kind, frame, child_views)
    };
    if kind != ElementKind::ScrollView
    {
        return;
    }
    for view in child_views.unwrap_or_default()
    {
        let Some(child) = view.element else { continue };
        if !view.visible
        {
            child.borrow_mut().frame = Rect::default();
            continue;
        }
        let child_frame = Rect
        {
            row: frame.row + view.frame.row,
            col: frame.col + view.frame.col,
            width: view.frame.width,
            height: view.frame.height,
        };
        child.borrow_mut().frame = child_frame;
        sync_element_descendant_frames(&child);
    }
}

fn sync_window_element_frames_to(window: &GeneratedWindow, frame: Rect)
{
    let width = max(1, frame.width);
    let height = max(1, frame.height);
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let mut runtime_cells = collect_runtime_cells(window);
    let resolved = resolve_runtime_cells(
        window,
        &mut runtime_cells,
        Rect
        {
            row: 0,
            col: 0,
            width,
            height,
        },
        horizontal_separator,
        vertical_separator,
        WindowMode::Fullscreen,
    );
    for item in &window.layout
    {
        if item.content.is_empty()
        {
            continue;
        }
        let Some(element) = window.elements.iter().find(|element|
        {
            element.borrow().name == item.content
        }).cloned() else { continue };
        let Some(cell) = resolved.iter().find(|cell|
        {
            same_runtime_cell(&cell.cell, item)
        }) else { continue };
        let mut element_frame =
            resolved_item_rect(&window.layout, item, cell.rect, Some(&element));
        element_frame.row += frame.row;
        element_frame.col += frame.col;
        element.borrow_mut().frame = element_frame;
        sync_element_descendant_frames(&element);
    }
    if let Some(scroll_view) = window.generated_scroll_view()
    {
        scroll_view.borrow_mut().frame = frame;
        sync_element_descendant_frames(&scroll_view);
    }
}

pub(crate) fn sync_reusable_child_frames(element: &ElementRef)
{
    let mut value = element.borrow_mut();
    let frame = value.frame;
    if frame.width <= 0 || frame.height <= 0
    {
        return;
    }
    let Some(child) = value.child_window_mut() else { return };
    sync_window_element_frames_to(child, frame);
}

pub(crate) fn sync_window_descendant_frames(window: &GeneratedWindow)
{
    for element in &window.elements
    {
        sync_element_descendant_frames(element);
    }
}

pub fn render_generated_window<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
) -> TerminalBuffer
{
    render_generated_window_with_root_scroll_indicator_mode(
        app,
        state,
        options,
        false,
        false,
    )
}

fn render_generated_window_internal<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    force_fullscreen_layout: bool,
    use_host_viewport_for_root_scroll_indicators: bool,
) -> TerminalBuffer
{
    render_generated_window_with_root_scroll_indicator_mode(
        app,
        state,
        options,
        force_fullscreen_layout,
        use_host_viewport_for_root_scroll_indicators,
    )
}

fn render_generated_window_with_root_scroll_indicator_mode<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    _options: &GeneratedWindowRuntimeOptions,
    force_fullscreen_layout: bool,
    use_host_viewport_for_root_scroll_indicators: bool,
) -> TerminalBuffer
{
    if app.window().is_generated_scroll_view()
    {
        let bounds = window_bounds(
            app.window(),
            state.viewport,
            force_fullscreen_layout,
        );
        let rendered = render_generated_scroll_window(
            app.window_mut(),
            state,
            Size
            {
                width: bounds.width,
                height: bounds.height,
            },
            use_host_viewport_for_root_scroll_indicators,
        );
        translate_window_frames(app.window(), bounds.row, bounds.col);
        let mut buffer = TerminalBuffer::new(state.viewport.width, state.viewport.height);
        buffer.overlay(&rendered, bounds.row, bounds.col);
        return buffer;
    }
    let resolution =
        resolve_layout_geometry(app.window_mut(), state.viewport, force_fullscreen_layout);
    let window = app.window();
    let render_edit_mode =
        state.edit_mode && !state.suppress_active_scroll_scope_visuals;
    let mut buffer = TerminalBuffer::new(state.viewport.width, state.viewport.height);
    let base_background = window.window_style.background.clone();
    let mut background = base_background.clone();
    if let Some(override_background) = state.render_background_override.as_ref()
    {
        background = Some(override_background.clone());
    }
    let foreground = window.window_style.color.clone();
    buffer.fill(
        resolution.bounds,
        ' ',
        &window.window_style.border_color,
        &background,
    );
    buffer.fill(resolution.content, ' ', &foreground, &background);
    let mut painted_cells: Vec<(i32, i32, i32, i32)> = Vec::new();
    for item in &window.layout
    {
        let Some(cell) = resolution.cells.iter().find(|cell| same_runtime_cell(&cell.cell, item)) else { continue };
        let identity = (
            cell.cell.source_row,
            cell.cell.source_col,
            cell.cell.source_width,
            cell.cell.source_height,
        );
        if painted_cells.contains(&identity)
        {
            continue;
        }
        painted_cells.push(identity);
        if item.cell_style.background.is_none()
            && item.cell_style.color.is_none()
            && item.cell_style.background_texture.is_none()
        {
            continue;
        }
        let cell_foreground = item.cell_style.color.clone();
        let cell_background = item.cell_style.background.clone();
        let mut rendered = TerminalBuffer::new(cell.rect.width, cell.rect.height);
        rendered.fill(
            Rect { row: 0, col: 0, width: cell.rect.width, height: cell.rect.height },
            ' ',
            &cell_foreground,
            &cell_background,
        );
        let texture = crate::visual_glyphs(
            item.cell_style
                .background_texture
                .as_deref()
                .unwrap_or_default(),
            0,
            0,
        );
        if !texture.is_empty()
        {
            let texture_foreground = item
                .cell_style
                .background_texture_color
                .clone()
                .or_else(|| cell_foreground.clone());
            for row in 0..cell.rect.height
            {
                for col in 0..cell.rect.width
                {
                    let glyph = &texture[(col as usize) % texture.len()];
                    rendered.draw_text(
                        row,
                        col,
                        &glyph.text,
                        &texture_foreground,
                        &cell_background,
                        1,
                    );
                }
            }
        }
        buffer.overlay(&rendered, cell.rect.row, cell.rect.col);
    }
    let focused = state.focused_element(window);
    let mut deferred_combo_box: Option<(ElementRef, Rect, Rect)> = None;
    let mut descendant_combo_box: Option<(ElementRef, Rect)> = None;
    for item in &window.layout
    {
        let Some(cell) = resolution.cells.iter().find(|cell| same_runtime_cell(&cell.cell, item)) else { continue };
        let Some(element) = window.find_direct_element(&item.content) else
        {
            if item.content.starts_with('"') && item.content.ends_with('"')
            {
                let text = item.content.trim_matches('"');
                let rect = resolved_item_rect(&window.layout, item, cell.rect, None);
                let literal_foreground = item.element_style.color.clone();
                let literal_background = item.element_style.background.clone();
                let mut rendered = TerminalBuffer::new(rect.width, rect.height);
                rendered.fill(
                    Rect { row: 0, col: 0, width: rect.width, height: rect.height },
                    ' ',
                    &literal_foreground,
                    &literal_background,
                );
                let literal_col = match item
                    .element_style
                    .text_align
                    .as_deref()
                    .unwrap_or_default()
                {
                    "right" => max(0, rect.width - text.chars().count() as i32),
                    "center" => max(0, (rect.width - text.chars().count() as i32) / 2),
                    _ => 0,
                };
                rendered.draw_text(
                    0,
                    literal_col,
                    text,
                    &literal_foreground,
                    &literal_background,
                    rect.width - literal_col,
                );
                buffer.overlay_clipped(
                    &rendered,
                    rect.row,
                    rect.col,
                    cell_content_clip(item, cell.rect),
                );
            }
            continue;
        };
        let (rect, kind, has_child_window) =
        {
            let element_value = element.borrow();
            (
                element_value.frame,
                element_value.kind(),
                element_value.child_window().is_some(),
            )
        };
        let mut clip = cell_content_clip(item, cell.rect);
        let element_clip = relative_element_render_clip(
            rect,
            clip,
            state.render_clip_top,
            state.render_clip_bottom,
            buffer.height,
        );
        let suppress_element_focus = state.suppress_active_scroll_scope_visuals
            && matches!(
                kind,
                ElementKind::Reusable | ElementKind::ViewHost | ElementKind::ScrollView
            );
        let is_focused = !suppress_element_focus
            && focused.as_ref().is_some_and(|focused|
            {
                std::rc::Rc::ptr_eq(focused, &element)
            });
        if kind == ElementKind::ComboBox
            && is_focused
            && focused_element_is_editing(state, &element)
        {
            deferred_combo_box = Some((element.clone(), rect, clip));
            continue;
        }
        let parent_background = buffer
            .cell(rect.row, rect.col)
            .and_then(|cell| cell.background.clone());
        let rendered = crate::with_element_parent_background(parent_background.clone(), ||
        {
            if has_child_window
            {
                let focused_combo_in_child = focused.as_ref().is_some_and(|focused|
                {
                    state.scope_edit_element.is_none()
                        && !std::rc::Rc::ptr_eq(focused, &element)
                        && focused.borrow().kind() == ElementKind::ComboBox
                        && focused_element_is_editing(state, focused)
                        && element
                            .borrow()
                            .child_window()
                            .is_some_and(|child| window_contains_element(child, focused))
                });
                let generated_scroll_proxy_active =
                    !state.suppress_active_scroll_scope_visuals
                    && is_focused
                    && state
                        .scope_edit_element
                        .as_ref()
                        .is_some_and(|scope| std::rc::Rc::ptr_eq(scope, &element))
                    && element
                        .borrow()
                        .child_window()
                        .is_some_and(GeneratedWindow::is_generated_scroll_view);
                let navigation_scope_descendant_focused =
                    !state.suppress_active_scroll_scope_visuals
                    && !render_edit_mode
                    && state
                        .scope_edit_element
                        .as_ref()
                        .is_some_and(|scope| std::rc::Rc::ptr_eq(scope, &element))
                    && focused.as_ref().is_some_and(|focused|
                    {
                        element
                            .borrow()
                            .child_window()
                            .is_some_and(|child|
                            {
                                child.is_generated_scroll_view()
                                    && window_contains_element(child, focused)
                            })
                    });
                let focus_background_override = if (is_focused && !render_edit_mode)
                    || generated_scroll_proxy_active
                    || navigation_scope_descendant_focused
                    || (render_edit_mode
                    && focused.as_ref().is_some_and(|focused|
                    {
                        element
                            .borrow()
                            .child_window()
                            .is_some_and(|child|
                            {
                                child.is_generated_scroll_view()
                                    && window_contains_element(child, focused)
                            })
                    }))
                {
                    let element_value = element.borrow();
                    let host_focus = element_value
                        .focus_style
                        .as_ref()
                        .and_then(|style| style.background.clone())
                        .filter(|background| !background.empty());
                    if element_value.child_window().is_some_and(|child|
                    {
                        child.is_generated_scroll_view()
                            && host_focus
                                .as_ref()
                                .is_some_and(Color::has_partial_alpha)
                    })
                    {
                        element_value.child_window().and_then(|child|
                        {
                            let descendant_focus =
                                child.generated_scroll_view_descendant_focus_style();
                            let focus_style = child.generated_scroll_view_focus_style();
                            descendant_focus
                                .background
                                .or(focus_style.background)
                                .filter(|focus| !focus.empty())
                        })
                    }
                    else
                    {
                        host_focus
                    }
                }
                else
                {
                    None
                };
                let focused_id =
                {
                    let element_value = element.borrow();
                    element_value.child_window().and_then(|child_window|
                    {
                        focused
                            .as_ref()
                            .and_then(|focused| child_window.element_id(focused))
                    })
                };
                let mut element_value = element.borrow_mut();
                let reusable_base_background = element_value
                    .style
                    .background
                    .clone()
                    .or_else(|| item.cell_style.background.clone());
                let reusable_focus_background = element_value
                    .focus_style
                    .as_ref()
                    .and_then(|style| style.background.clone())
                    .filter(|background| !background.empty());
                let child_window = element_value
                    .child_window_mut()
                    .expect("reusable child window disappeared while rendering");
                let use_host_viewport_for_root_scroll_indicators =
                    kind == ElementKind::ViewHost
                        && child_window.is_generated_scroll_view();
                let generated_scroll_view = child_window.generated_scroll_view();
                let direct_generated_scroll_focus = child_window.is_generated_scroll_view()
                    && reusable_focus_background
                        .as_ref()
                        .is_some_and(Color::has_partial_alpha)
                    && (is_focused
                        || focused.as_ref().is_some_and(|focused|
                        {
                            generated_scroll_view
                                .as_ref()
                                .is_some_and(|scroll_view| Rc::ptr_eq(scroll_view, focused))
                        }));
                let direct_generated_scroll_focus_background =
                    direct_generated_scroll_focus.then(||
                    {
                        focus_background_override
                            .clone()
                            .or_else(|| reusable_focus_background.clone())
                    }).flatten();
                let previous_child_window_background =
                    child_window.window_style.background.clone();
                let previous_generated_scroll_view_style = generated_scroll_view
                    .as_ref()
                    .map(|scroll_view| scroll_view.borrow().style.clone());
                let previous_generated_scroll_view_focus_style = generated_scroll_view
                    .as_ref()
                    .and_then(|scroll_view| scroll_view.borrow().focus_style.clone());
                let mut direct_focus_structural_background = None;
                if direct_generated_scroll_focus
                {
                    let structural_background = previous_generated_scroll_view_style
                        .as_ref()
                        .and_then(|style| style.background.clone())
                        .or_else(|| previous_child_window_background.clone())
                        .or_else(|| reusable_base_background.clone())
                        .or_else(|| parent_background.clone());
                    direct_focus_structural_background = structural_background.clone();
                    if let Some(focus_background) =
                        direct_generated_scroll_focus_background.as_ref()
                    {
                        if let (Some(scroll_view), Some(previous_focus_style)) = (
                            generated_scroll_view.as_ref(),
                            previous_generated_scroll_view_focus_style.as_ref(),
                        ) && previous_focus_style.background.as_ref().is_some_and(|background|
                            same_rendered_color(background, focus_background))
                        {
                            let mut child_focus_style = previous_focus_style.clone();
                            child_focus_style.background = None;
                            scroll_view.borrow_mut().focus_style = Some(child_focus_style);
                        }
                        let focused_background = structural_background
                            .as_ref()
                            .map(|background| focus_background.blend_over(background))
                            .unwrap_or_else(|| focus_background.clone());
                        child_window.window_style.background = Some(focused_background.clone());
                        if let Some(scroll_view) = generated_scroll_view.as_ref()
                        {
                            scroll_view.borrow_mut().style.background = Some(focused_background);
                        }
                    }
                }
                let mut rendered = render_embedded_window_with_interaction(
                    child_window,
                    rect.width,
                    rect.height,
                    state,
                    focused_id,
                    if direct_generated_scroll_focus
                    {
                        None
                    }
                    else
                    {
                        focus_background_override.clone()
                    },
                    element_clip.0,
                    element_clip.1,
                    use_host_viewport_for_root_scroll_indicators,
                );
                if direct_generated_scroll_focus
                {
                    apply_generated_scroll_view_focus_underlay_to_structural_backgrounds(
                        &mut rendered,
                        direct_focus_structural_background.as_ref(),
                        direct_generated_scroll_focus_background.as_ref(),
                    );
                    child_window.window_style.background = previous_child_window_background;
                    if let (Some(scroll_view), Some(previous_style)) = (
                        generated_scroll_view.as_ref(),
                        previous_generated_scroll_view_style,
                    )
                    {
                        scroll_view.borrow_mut().style = previous_style;
                    }
                    if let Some(scroll_view) = generated_scroll_view.as_ref()
                    {
                        scroll_view.borrow_mut().focus_style =
                            previous_generated_scroll_view_focus_style;
                    }
                }
                let descendant_focus_background = focus_background_override.as_ref();
                if let Some(focus_background) =
                    descendant_focus_background
                        .filter(|background| !background.empty())
                        .filter(|_|
                        {
                            !child_window.is_generated_scroll_view()
                        })
                {
                    let mut descendant_backgrounds = Vec::new();
                    collect_descendant_base_style_backgrounds(
                        child_window,
                        &mut descendant_backgrounds,
                    );
                    apply_reusable_focus_to_descendant_backgrounds(
                        &mut rendered,
                        focus_background,
                        &descendant_backgrounds,
                    );
                    apply_reusable_focus_background(
                        &mut rendered,
                        focus_background,
                        reusable_base_background.as_ref(),
                    );
                }
                translate_window_frames(child_window, rect.row, rect.col);
                drop(element_value);
                if focused_combo_in_child
                {
                    let focused = focused
                        .as_ref()
                        .expect("focused ComboBox disappeared while rendering");
                    descendant_combo_box = Some((focused.clone(), focused.borrow().frame));
                }
                rendered
            }
            else if kind == ElementKind::ScrollView
            {
                render_scroll_view_element_with_interaction(
                    &element,
                    rect,
                    state,
                    focused.as_ref(),
                    is_focused,
                )
            }
            else
            {
                element.borrow().render_with_state(
                    rect.width,
                    rect.height,
                    ElementRenderState
                    {
                        focused: is_focused,
                        edit_mode: is_focused && focused_element_is_editing(state, &element),
                        passive_focus: state.scope_edit_element.is_some()
                            && is_focused
                            && !focused_element_is_editing(state, &element),
                        clip_top: element_clip.0,
                        clip_bottom: element_clip.1,
                    },
                )
            }
        });
        if has_child_window
            && focused.as_ref().is_some_and(|focused|
            {
                focused.borrow().kind() == ElementKind::ComboBox
                    && focused_element_is_editing(state, focused)
                    && element
                        .borrow()
                        .child_window()
                        .is_some_and(|child| window_contains_element(child, focused))
            })
        {
            let clip_bottom = max(clip.row + clip.height, rect.row + rendered.height);
            let clip_right = max(clip.col + clip.width, rect.col + rendered.width);
            clip.height = max(0, clip_bottom - clip.row);
            clip.width = max(0, clip_right - clip.col);
        }
        buffer.overlay_clipped(&rendered, rect.row, rect.col, clip);
    }
    if let Some((element, rect, mut clip)) = deferred_combo_box
    {
        let parent_background = buffer
            .cell(rect.row, rect.col)
            .and_then(|cell| cell.background.clone());
        let rendered = crate::with_element_parent_background(parent_background, ||
        {
            element.borrow().render(rect.width, rect.height, true, true)
        });
        let clip_bottom = max(clip.row + clip.height, rect.row + rendered.height);
        clip.height = max(0, clip_bottom - clip.row);
        buffer.overlay_clipped(&rendered, rect.row, rect.col, clip);
    }
    if let Some((combo_box, frame)) = descendant_combo_box
    {
        let clip = Rect { row: 0, col: 0, width: buffer.width, height: buffer.height };
        overlay_focused_combo_box(
            &mut buffer,
            &combo_box,
            frame,
            clip,
        );
    }
    if render_edit_mode && !state.render_inside_scope
        && let Some(scope) = state.scope_edit_element.as_ref()
    {
        let (active, dim) =
        {
            let scope = scope.borrow();
            let dim = scope.style.scope_dim_background.clone();
            (scope.frame, dim)
        };
        dim_outside_active_scope(&mut buffer, active, &dim);
    }
    if !window.notification.is_empty()
    {
        let text = &window.notification;
        let col = max(0, buffer.width - text.chars().count() as i32 - 1);
        buffer.draw_text(0, col, text, &Color::new("#ffffff"), &Color::new("#334155"), buffer.width - col);
    }
    if let Some(focus_background) =
        state
            .render_background_override
            .as_ref()
            .filter(|_| !state.suppress_active_scroll_scope_visuals)
            .filter(|background| !background.empty())
    {
        apply_reusable_focus_background(
            &mut buffer,
            focus_background,
            base_background.as_ref(),
        );
    }
    buffer
}

fn render_scroll_view_element_with_interaction(
    element: &ElementRef,
    frame: Rect,
    state: &RuntimeState,
    focused: Option<&ElementRef>,
    self_focused: bool,
) -> TerminalBuffer
{
    let scope_active = state
        .scope_edit_element
        .as_ref()
        .is_some_and(|scope| Rc::ptr_eq(scope, element));
    let mut rendered = element.borrow().render_with_state(
        frame.width,
        frame.height,
        ElementRenderState
        {
            focused: self_focused,
            edit_mode: scope_active,
            passive_focus: self_focused && state.scope_edit_element.is_some(),
            ..ElementRenderState::default()
        },
    );
    let views = element.borrow().child_views(Size
    {
        width: max(1, frame.width),
        height: max(1, frame.height),
    });
    let clip = Rect
    {
        row: 0,
        col: 0,
        width: rendered.width,
        height: rendered.height,
    };
    for view in views
    {
        let Some(child) = view.element else { continue };
        if !view.visible
        {
            continue;
        }
        let child_global_frame = Rect
        {
            row: frame.row + view.frame.row,
            col: frame.col + view.frame.col,
            width: view.frame.width,
            height: view.frame.height,
        };
        child.borrow_mut().frame = child_global_frame;
        let child_focused = focused
            .is_some_and(|focused| Rc::ptr_eq(focused, &child));
        let child_rendered =
        {
            let has_child_window = child.borrow().child_window().is_some();
            if has_child_window
            {
                let focused_id =
                {
                    let child_value = child.borrow();
                    child_value.child_window().and_then(|child_window|
                    {
                        focused.and_then(|focused| child_window.element_id(focused))
                    })
                };
                let mut child_value = child.borrow_mut();
                let child_window = child_value
                    .child_window_mut()
                    .expect("ScrollView reusable child disappeared while rendering");
                let rendered = render_embedded_window_with_interaction(
                    child_window,
                    view.frame.width,
                    view.frame.height,
                    state,
                    focused_id,
                    None,
                    None,
                    None,
                    false,
                );
                translate_window_frames(
                    child_window,
                    child_global_frame.row,
                    child_global_frame.col,
                );
                rendered
            }
            else
            {
                child.borrow().render_with_state(
                    view.frame.width,
                    view.frame.height,
                    ElementRenderState
                    {
                        focused: child_focused,
                        edit_mode: child_focused
                            && focused_element_is_editing(state, &child),
                        passive_focus: scope_active
                            && child_focused
                            && !focused_element_is_editing(state, &child),
                        ..ElementRenderState::default()
                    },
                )
            }
        };
        rendered.overlay_clipped(
            &child_rendered,
            view.frame.row,
            view.frame.col,
            clip,
        );
    }
    rendered
}

fn overlay_focused_combo_box(
    buffer: &mut TerminalBuffer,
    combo_box: &ElementRef,
    frame: Rect,
    clip: Rect,
)
{
    let parent_background = buffer
        .cell(frame.row, frame.col)
        .and_then(|cell| cell.background.clone());
    let rendered = crate::with_element_parent_background(parent_background, ||
    {
        combo_box.borrow().render(frame.width, frame.height, true, true)
    });
    buffer.overlay_clipped(&rendered, frame.row, frame.col, clip);
}

fn dim_color_over_existing(
    dim: &Color,
    color: &Option<Color>,
    fill_when_missing: bool,
) -> Option<Color>
{
    if dim.rgba().is_some() && color.is_some()
    {
        return Some(dim.blend_over(color.as_ref().expect("existing color")));
    }
    if fill_when_missing || color.is_some()
    {
        return Some(dim.clone());
    }
    None
}

fn dim_outside_active_scope(
    buffer: &mut TerminalBuffer,
    active: Rect,
    dim: &Option<Color>,
)
{
    let Some(dim) = dim else { return };
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            if active.contains(Point { row, col })
            {
                continue;
            }
            if let Some(cell) = buffer.cell_mut(row, col)
            {
                if cell.text == "▀"
                {
                    cell.foreground =
                        dim_color_over_existing(dim, &cell.foreground, false);
                }
                cell.background =
                    dim_color_over_existing(dim, &cell.background, true);
            }
        }
    }
}

const MODAL_BACKGROUND_DIM_FACTOR: f64 = 0.5;

pub fn dim_generated_window_modal_background(buffer: &mut TerminalBuffer)
{
    for row in 0..buffer.height
    {
        for col in 0..buffer.width
        {
            if let Some(cell) = buffer.cell_mut(row, col)
            {
                cell.foreground = cell.foreground.dimmed(MODAL_BACKGROUND_DIM_FACTOR);
                cell.background = cell.background.dimmed(MODAL_BACKGROUND_DIM_FACTOR);
            }
        }
    }
}

fn modal_window_copies(
    root: &GeneratedWindow,
) -> Vec<(GeneratedWindow, GeneratedWindowFrameOptions)>
{
    root.window_stack
        .frames()
        .iter()
        .map(|frame| (frame.window.clone(), frame.options.clone()))
        .collect()
}

fn refresh_modal_frame_bounds(root: &mut GeneratedWindow, viewport: Size)
{
    for frame in root.window_stack.frames_mut()
    {
        frame.bounds = window_bounds(&frame.window, viewport, false);
    }
}

fn apply_runtime_overlay(
    buffer: &mut TerminalBuffer,
    options: &GeneratedWindowRuntimeOptions,
)
{
    let Some(callback) = options.on_render_overlay.as_ref() else
    {
        return;
    };
    if options
        .should_dim_overlay_background
        .as_ref()
        .is_some_and(|should_dim| should_dim())
    {
        dim_generated_window_modal_background(buffer);
    }
    callback(buffer);
}

fn point_in_scroll_selection(point: Point, selection: &ScrollSelection) -> bool
{
    let Some(element) = selection.element.as_ref() else
    {
        return false;
    };
    if !selection.changed
    {
        return false;
    }
    let mut start = selection.anchor;
    let mut end = selection.current;
    if (end.row, end.col) < (start.row, start.col)
    {
        std::mem::swap(&mut start, &mut end);
    }
    if point.row < start.row || point.row > end.row
    {
        return false;
    }
    if start.row == end.row
    {
        return point.col >= start.col && point.col <= end.col;
    }
    if point.row == start.row
    {
        return point.col >= start.col;
    }
    if point.row == end.row
    {
        return point.col <= end.col;
    }
    let frame = element.borrow().frame;
    point.col >= frame.col && point.col < frame.col + frame.width
}

fn apply_scroll_selection(buffer: &mut TerminalBuffer, selection: &ScrollSelection)
{
    let Some(element) = selection.element.as_ref() else
    {
        return;
    };
    if !selection.changed
    {
        return;
    }
    let value = element.borrow();
    let mut selection_style = value.style.clone();
    if let Some(selected_style) = value.selected_style.as_ref()
    {
        selection_style.merge(selected_style);
    }
    else if let Some(cursor_style) = value.cursor_style.as_ref()
    {
        selection_style.merge(cursor_style);
    }
    let frame = value.frame;
    for row in max(0, frame.row)..min(buffer.height, frame.row + frame.height)
    {
        for col in max(0, frame.col)..min(buffer.width, frame.col + frame.width)
        {
            if !point_in_scroll_selection(Point { row, col }, selection)
            {
                continue;
            }
            let Some(cell) = buffer.cell_mut(row, col) else { continue };
            let original_foreground = cell.foreground.clone();
            let original_background = cell.background.clone();
            cell.foreground = if selection_style.color.is_none()
            {
                original_background
            }
            else
            {
                selection_style.color.clone()
            };
            cell.background = if selection_style.background.is_none()
            {
                original_foreground
            }
            else
            {
                selection_style.background.clone()
            };
        }
    }
}

pub(crate) fn selected_scroll_text(
    buffer: &TerminalBuffer,
    selection: &ScrollSelection,
) -> String
{
    let Some(element) = selection.element.as_ref() else
    {
        return String::new();
    };
    if !selection.changed
    {
        return String::new();
    }
    let mut start = selection.anchor;
    let mut end = selection.current;
    if (end.row, end.col) < (start.row, start.col)
    {
        std::mem::swap(&mut start, &mut end);
    }
    let frame = element.borrow().frame;
    let mut result = String::new();
    for row in max(0, start.row)..=min(buffer.height - 1, end.row)
    {
        let first_col = max(0, if row == start.row { start.col } else { frame.col });
        let last_col = min(
            buffer.width - 1,
            if row == end.row
            {
                end.col
            }
            else
            {
                frame.col + frame.width - 1
            },
        );
        let mut line = String::new();
        for col in first_col..=last_col
        {
            if let Some(cell) = buffer.cell(row, col)
            {
                line.push_str(&cell.text);
            }
        }
        while line.ends_with(' ')
        {
            line.pop();
        }
        if !result.is_empty()
        {
            result.push('\n');
        }
        result.push_str(&line);
    }
    result
}

fn render_modal_window_local<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    bounds: Rect,
) -> (TerminalBuffer, Option<(ElementRef, Rect)>)
{
    let style = &mut app.window_mut().window_style;
    style.margin = Some(0);
    style.margin_top = Some(0);
    style.margin_right = Some(0);
    style.margin_bottom = Some(0);
    style.margin_left = Some(0);
    let previous_viewport = state.viewport;
    state.viewport = Size
    {
        width: max(1, bounds.width),
        height: max(1, bounds.height),
    };
    let rendered = render_generated_window_internal(app, state, options, true, false);
    let focused_combo_box = state.focused_element(app.window()).filter(|element|
    {
        element.borrow().kind() == ElementKind::ComboBox
            && focused_element_is_editing(state, element)
    });
    let focused_combo_box = focused_combo_box.map(|combo_box|
    {
        let frame = combo_box.borrow().frame;
        (combo_box, frame)
    });
    state.viewport = previous_viewport;
    (rendered, focused_combo_box)
}

pub fn render_generated_application<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
) -> TerminalBuffer
{
    app.window_mut().active_window_mut().expire_notification();
    state.sync_active_window(app.window_mut(), options);
    refresh_modal_frame_bounds(app.window_mut(), state.viewport);
    state.repair_live_interaction(app.active_window());
    let modal_windows = modal_window_copies(app.window());
    if modal_windows.is_empty()
    {
        let mut buffer = render_generated_window(app, state, options);
        apply_runtime_overlay(&mut buffer, options);
        apply_scroll_selection(&mut buffer, &state.scroll_selection);
        return buffer;
    }

    let mut root_state = RuntimeState::new(app.window(), options, state.viewport);
    if let Some(interaction) = state.root_interaction_snapshot.clone()
    {
        root_state.restore_interaction(interaction, app.window());
    }
    root_state.suppress_active_scroll_scope_visuals = true;
    let mut buffer = crate::with_image_cell_background_rendering(||
    {
        render_generated_window(app, &mut root_state, options)
    });

    let dim_backgrounds = modal_windows
        .iter()
        .map(|(_, frame_options)| frame_options.dim_background)
        .collect::<Vec<_>>();
    if dim_backgrounds.first().copied().unwrap_or(false)
    {
        dim_generated_window_modal_background(&mut buffer);
    }
    let modal_count = modal_windows.len();
    for (index, (window, _frame_options)) in modal_windows.into_iter().enumerate()
    {
        let modal_options = window.runtime_options();
        let bounds = window_bounds(&window, state.viewport, false);
        let mut modal_app = EmbeddedApplication { window };
        let mut inactive_state;
        let modal_state = if index + 1 == app.window().modal_depth()
        {
            &mut *state
        }
        else
        {
            inactive_state = RuntimeState::new(
                &modal_app.window,
                &modal_options,
                state.viewport,
            );
            if let Some(interaction) = app
                .window()
                .modal_frame(index)
                .map(RuntimeState::interaction_from_frame)
            {
                inactive_state.restore_interaction(interaction, &modal_app.window);
            }
            inactive_state.edit_mode = false;
            inactive_state.scope_dim_element = None;
            inactive_state.scope_edit_element = None;
            inactive_state.edit_snapshot = None;
            &mut inactive_state
        };
        let (mut rendered, focused_combo_box) = if index + 1 < modal_count
        {
            crate::with_image_cell_background_rendering(||
            {
                render_modal_window_local(
                    &mut modal_app,
                    modal_state,
                    &modal_options,
                    bounds,
                )
            })
        }
        else
        {
            render_modal_window_local(
                &mut modal_app,
                modal_state,
                &modal_options,
                bounds,
            )
        };
        if dim_backgrounds.get(index + 1).copied().unwrap_or(false)
        {
            dim_generated_window_modal_background(&mut rendered);
        }
        buffer.overlay(&rendered, bounds.row, bounds.col);
        if let Some((combo_box, frame)) = focused_combo_box
        {
            overlay_focused_combo_box(
                &mut buffer,
                &combo_box,
                Rect
                {
                    row: bounds.row + frame.row,
                    col: bounds.col + frame.col,
                    width: frame.width,
                    height: frame.height,
                },
                Rect
                {
                    row: 0,
                    col: 0,
                    width: state.viewport.width,
                    height: state.viewport.height,
                },
            );
        }
    }
    apply_runtime_overlay(&mut buffer, options);
    apply_scroll_selection(&mut buffer, &state.scroll_selection);
    buffer
}

pub(crate) fn render_active_generated_window<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
) -> TerminalBuffer
{
    state.sync_active_window(app.window_mut(), options);
    let mut active_app = EmbeddedApplication
    {
        window: app.active_window().clone(),
    };
    let active_options = active_app.window.runtime_options();
    let mut active_state = state.clone();
    active_state.viewport = active_window_reported_size(&active_app.window, state.viewport);
    active_state.modal_depth = 0;
    active_state.root_interaction_snapshot = None;
    render_generated_window(&mut active_app, &mut active_state, &active_options)
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum WindowMode
{
    Normal,
    ExpandWidth,
    ExpandHeight,
    Fullscreen,
}

#[derive(Clone, Debug, Eq, PartialEq)]
struct RuntimeCell
{
    source_row: i32,
    source_col: i32,
    source_width: i32,
    source_height: i32,
    logical_width: Option<i32>,
    logical_height: Option<i32>,
}

#[derive(Clone, Debug)]
struct RuntimeNode
{
    orientation: &'static str,
    children: Vec<RuntimeNode>,
    cell: Option<RuntimeCell>,
    left: i32,
    top: i32,
    right: i32,
    bottom: i32,
    logical_width: Option<i32>,
    logical_height: Option<i32>,
}

#[derive(Clone, Debug)]
struct ResolvedRuntimeCell
{
    cell: RuntimeCell,
    rect: Rect,
}

#[derive(Clone, Debug)]
struct LayoutResolution
{
    bounds: Rect,
    content: Rect,
    cells: Vec<ResolvedRuntimeCell>,
}

fn dimension_mode(mode: &str, value: i32) -> &str
{
    if value == EXPANDED || mode == "expanded"
    {
        "expanded"
    }
    else if value == FIT_CONTENT || mode == "fit-content"
    {
        "fit-content"
    }
    else if mode == "fixed"
    {
        "fixed"
    }
    else
    {
        "auto"
    }
}

fn logical_cell_size(mode: &str, value: i32, source_size: i32) -> Option<i32>
{
    match dimension_mode(mode, value)
    {
        "expanded" => None,
        "fixed" => Some(max(1, value)),
        _ => Some(max(1, if value > 0 && value != FIT_CONTENT { value } else { source_size })),
    }
}

fn same_runtime_cell(cell: &RuntimeCell, item: &LayoutItem) -> bool
{
    cell.source_row == item.cell_row
        && cell.source_col == item.cell_col
        && cell.source_width == item.cell_chars_width
        && cell.source_height == item.cell_chars_height
}

fn same_source_cell(first: &LayoutItem, second: &LayoutItem) -> bool
{
    first.cell_row == second.cell_row
        && first.cell_col == second.cell_col
        && first.cell_chars_width == second.cell_chars_width
        && first.cell_chars_height == second.cell_chars_height
}

fn text_line_count(text: &str) -> i32
{
    if text.is_empty() { 0 } else { text.lines().count() as i32 + i32::from(text.ends_with('\n')) }
}

fn wrapped_text_line_count(text: &str, width: i32) -> i32
{
    let width = max(1, width) as usize;
    if text.is_empty()
    {
        return 1;
    }
    let mut rows = 0;
    for segment in text.split('\n')
    {
        if segment.is_empty()
        {
            rows += 1;
            continue;
        }
        let mut remaining = segment;
        while !remaining.is_empty()
        {
            let mut byte_end = remaining.len();
            let mut cell_width = 0;
            for (index, ch) in remaining.char_indices()
            {
                let next = unicode_width::UnicodeWidthChar::width(ch).unwrap_or(0);
                if cell_width + next > width
                {
                    byte_end = index;
                    break;
                }
                cell_width += next;
            }
            if byte_end == 0
            {
                byte_end = remaining.chars().next().map_or(remaining.len(), char::len_utf8);
            }
            let chunk = &remaining[..byte_end];
            rows += 1;
            if byte_end < remaining.len()
                && let Some(space) = chunk.rfind(' ').filter(|position| *position > 0)
            {
                remaining = &remaining[space + 1..];
                continue;
            }
            remaining = &remaining[byte_end..];
        }
    }
    max(1, rows)
}

fn fit_content_height_for(element: Option<&ElementRef>, item: &LayoutItem, width: Option<i32>) -> i32
{
    let fallback = max(1, if item.chars_height > 0 { item.chars_height } else { item.cell_chars_height });
    let Some(element) = element else { return fallback };
    let element = element.borrow();
    if element.kind() == ElementKind::ListBox && !element.options().is_empty()
    {
        return max(1, element.options().len() as i32);
    }
    if matches!(
        element.kind(),
        ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel
    )
    {
        if let Some(width) = width.filter(|_| item.element_type != "spanlabel" && item.element_type != "infolabel")
        {
            return wrapped_text_line_count(element.text(), width);
        }
        return max(1, text_line_count(element.text()));
    }
    if element.kind() == ElementKind::MessageTable
    {
        return crate::message_table_fit_content_height(element.text());
    }
    fallback
}

fn fit_entry_width(window: &GeneratedWindow, item: &LayoutItem) -> i32
{
    let fallback = max(1, if item.chars_width > 0 { item.chars_width } else { item.cell_chars_width });
    let mut content_width = fallback;
    if let Some(element) = window.find_element(&item.content)
    {
        let element = element.borrow();
        if element.kind() == ElementKind::ListBox
        {
            for option in element.options()
            {
                content_width = max(content_width, UnicodeWidthStr::width(option.as_str()) as i32);
            }
        }
        else if matches!(element.kind(), ElementKind::Label | ElementKind::InfoLabel)
        {
            for line in element.text().lines()
            {
                content_width = max(content_width, UnicodeWidthStr::width(line) as i32);
            }
        }
    }
    max(1, item.col + content_width + item.margin_right)
}

fn fit_entry_height(window: &GeneratedWindow, item: &LayoutItem, available_cell_width: Option<i32>) -> i32
{
    let content_width = available_cell_width.map(|available| max(1, available - item.col - item.margin_right));
    let element = window.find_element(&item.content);
    let content_height = if dimension_mode(&item.height_mode, item.height) == "fit-content"
    {
        fit_content_height_for(element.as_ref(), item, content_width)
    }
    else
    {
        max(1, if item.chars_height > 0 { item.chars_height } else { item.cell_chars_height })
    };
    max(1, item.row + content_height + item.margin_bottom)
}

fn fit_cell_width(window: &GeneratedWindow, cell_item: &LayoutItem) -> i32
{
    let mut content_width = max(1, cell_item.cell_chars_width);
    for item in &window.layout
    {
        if same_source_cell(item, cell_item)
        {
            content_width = max(content_width, fit_entry_width(window, item));
        }
    }
    let padding = cell_item.cell_style.padding_values();
    max(1, content_width + padding[1] + padding[3])
}

fn fit_cell_height(window: &GeneratedWindow, cell_item: &LayoutItem, resolved_width: Option<i32>) -> i32
{
    let padding = cell_item.cell_style.padding_values();
    let available_width = resolved_width.map(|width| max(1, width - padding[1] - padding[3]));
    let mut content_height = max(1, cell_item.cell_chars_height);
    for item in &window.layout
    {
        if same_source_cell(item, cell_item)
        {
            content_height = max(content_height, fit_entry_height(window, item, available_width));
        }
    }
    max(1, content_height + padding[0] + padding[2])
}

fn collect_runtime_cells(window: &GeneratedWindow) -> Vec<RuntimeCell>
{
    let mut cells = Vec::new();
    for item in &window.layout
    {
        if cells.iter().any(|cell| same_runtime_cell(cell, item))
        {
            continue;
        }
        let mut logical_width = logical_cell_size(&item.cell_width_mode, item.cell_width, item.cell_chars_width);
        let mut logical_height = logical_cell_size(&item.cell_height_mode, item.cell_height, item.cell_chars_height);
        let width_mode = dimension_mode(&item.cell_width_mode, item.cell_width);
        let height_mode = dimension_mode(&item.cell_height_mode, item.cell_height);
        if width_mode == "fit-content"
        {
            logical_width = Some(fit_cell_width(window, item));
        }
        if height_mode == "fit-content"
        {
            logical_height = Some(fit_cell_height(window, item, None));
        }
        let padding = item.cell_style.padding_values();
        if let Some(width) = logical_width.as_mut().filter(|_| !matches!(width_mode, "expanded" | "fit-content"))
        {
            *width = max(1, *width + padding[1] + padding[3]);
        }
        if let Some(height) = logical_height.as_mut().filter(|_| !matches!(height_mode, "expanded" | "fit-content"))
        {
            *height = max(1, *height + padding[0] + padding[2]);
        }
        cells.push(RuntimeCell
        {
            source_row: item.cell_row,
            source_col: item.cell_col,
            source_width: max(1, item.cell_chars_width),
            source_height: max(1, item.cell_chars_height),
            logical_width,
            logical_height,
        });
    }
    cells
}

fn make_leaf_node(cell: RuntimeCell) -> RuntimeNode
{
    RuntimeNode
    {
        orientation: "",
        children: Vec::new(),
        left: cell.source_col,
        top: cell.source_row,
        right: cell.source_col + cell.source_width + 1,
        bottom: cell.source_row + cell.source_height + 1,
        logical_width: cell.logical_width,
        logical_height: cell.logical_height,
        cell: Some(cell),
    }
}

fn intervals_cover(mut spans: Vec<(i32, i32)>, start: i32, end: i32) -> bool
{
    if spans.is_empty()
    {
        return false;
    }
    spans.sort_unstable();
    let mut cursor = start;
    for (span_start, span_end) in spans
    {
        if span_start > cursor
        {
            return false;
        }
        cursor = max(cursor, span_end);
        if cursor >= end
        {
            return true;
        }
    }
    cursor >= end
}

fn horizontal_cuts(nodes: &[RuntimeNode], left: i32, top: i32, right: i32, bottom: i32) -> Vec<i32>
{
    let mut candidates = Vec::new();
    for node in nodes
    {
        candidates.extend([node.top, node.bottom]);
    }
    candidates.sort_unstable();
    candidates.dedup();
    candidates
        .into_iter()
        .filter(|candidate|
        {
            *candidate > top
                && *candidate < bottom
                && intervals_cover(
                    nodes
                        .iter()
                        .filter(|node| node.top == *candidate || node.bottom == *candidate)
                        .map(|node| (node.left, node.right))
                        .collect(),
                    left,
                    right,
                )
        })
        .collect()
}

fn vertical_cuts(nodes: &[RuntimeNode], left: i32, top: i32, right: i32, bottom: i32) -> Vec<i32>
{
    let mut candidates = Vec::new();
    for node in nodes
    {
        candidates.extend([node.left, node.right]);
    }
    candidates.sort_unstable();
    candidates.dedup();
    candidates
        .into_iter()
        .filter(|candidate|
        {
            *candidate > left
                && *candidate < right
                && intervals_cover(
                    nodes
                        .iter()
                        .filter(|node| node.left == *candidate || node.right == *candidate)
                        .map(|node| (node.top, node.bottom))
                        .collect(),
                    top,
                    bottom,
                )
        })
        .collect()
}

fn slice_by_rows(nodes: &[RuntimeNode], boundaries: &[i32]) -> Option<Vec<Vec<RuntimeNode>>>
{
    let mut used = vec![false; nodes.len()];
    let mut groups = Vec::new();
    for bounds in boundaries.windows(2)
    {
        let mut group: Vec<(usize, RuntimeNode)> = nodes
            .iter()
            .enumerate()
            .filter(|(_, node)| node.top >= bounds[0] && node.bottom <= bounds[1])
            .map(|(index, node)| (index, node.clone()))
            .collect();
        if group.is_empty()
        {
            return None;
        }
        group.sort_by_key(|(_, node)| (node.top, node.left));
        for (index, _) in &group
        {
            used[*index] = true;
        }
        groups.push(group.into_iter().map(|(_, node)| node).collect());
    }
    used.into_iter().all(|value| value).then_some(groups)
}

fn slice_by_cols(nodes: &[RuntimeNode], boundaries: &[i32]) -> Option<Vec<Vec<RuntimeNode>>>
{
    let mut used = vec![false; nodes.len()];
    let mut groups = Vec::new();
    for bounds in boundaries.windows(2)
    {
        let mut group: Vec<(usize, RuntimeNode)> = nodes
            .iter()
            .enumerate()
            .filter(|(_, node)| node.left >= bounds[0] && node.right <= bounds[1])
            .map(|(index, node)| (index, node.clone()))
            .collect();
        if group.is_empty()
        {
            return None;
        }
        group.sort_by_key(|(_, node)| (node.left, node.top));
        for (index, _) in &group
        {
            used[*index] = true;
        }
        groups.push(group.into_iter().map(|(_, node)| node).collect());
    }
    used.into_iter().all(|value| value).then_some(groups)
}

#[allow(clippy::too_many_arguments)]
fn make_group_node(
    orientation: &'static str,
    children: Vec<RuntimeNode>,
    left: i32,
    top: i32,
    right: i32,
    bottom: i32,
    horizontal_separator: i32,
    vertical_separator: i32,
) -> RuntimeNode
{
    let mut logical_width = None;
    let mut logical_height = None;
    if orientation == "vertical"
    {
        if children.iter().all(|child| child.logical_width.is_some())
        {
            logical_width = children.iter().filter_map(|child| child.logical_width).max();
        }
        if children.iter().all(|child| child.logical_height.is_some())
        {
            logical_height = Some(
                children.iter().filter_map(|child| child.logical_height).sum::<i32>()
                    + max(0, children.len() as i32 - 1) * horizontal_separator,
            );
        }
    }
    else
    {
        if children.iter().all(|child| child.logical_height.is_some())
        {
            logical_height = children.iter().filter_map(|child| child.logical_height).max();
        }
        if children.iter().all(|child| child.logical_width.is_some())
        {
            logical_width = Some(
                children.iter().filter_map(|child| child.logical_width).sum::<i32>()
                    + max(0, children.len() as i32 - 1) * vertical_separator,
            );
        }
    }
    RuntimeNode
    {
        orientation,
        children,
        cell: None,
        left,
        top,
        right,
        bottom,
        logical_width,
        logical_height,
    }
}

fn build_runtime_tree(mut nodes: Vec<RuntimeNode>, horizontal_separator: i32, vertical_separator: i32) -> Option<RuntimeNode>
{
    if nodes.is_empty()
    {
        return None;
    }
    if nodes.len() == 1
    {
        return nodes.pop();
    }
    nodes.sort_by_key(|node| (node.top, node.left, node.bottom, node.right));
    let left = nodes.iter().map(|node| node.left).min().unwrap_or(0);
    let top = nodes.iter().map(|node| node.top).min().unwrap_or(0);
    let right = nodes.iter().map(|node| node.right).max().unwrap_or(1);
    let bottom = nodes.iter().map(|node| node.bottom).max().unwrap_or(1);

    let cuts = horizontal_cuts(&nodes, left, top, right, bottom);
    if !cuts.is_empty()
    {
        let mut boundaries = vec![top];
        boundaries.extend(cuts);
        boundaries.push(bottom);
        if let Some(groups) = slice_by_rows(&nodes, &boundaries).filter(|groups| groups.len() > 1)
        {
            let children = groups
                .into_iter()
                .filter_map(|group| build_runtime_tree(group, horizontal_separator, vertical_separator))
                .collect();
            return Some(make_group_node(
                "vertical",
                children,
                left,
                top,
                right,
                bottom,
                horizontal_separator,
                vertical_separator,
            ));
        }
    }

    let cuts = vertical_cuts(&nodes, left, top, right, bottom);
    if !cuts.is_empty()
    {
        let mut boundaries = vec![left];
        boundaries.extend(cuts);
        boundaries.push(right);
        if let Some(groups) = slice_by_cols(&nodes, &boundaries).filter(|groups| groups.len() > 1)
        {
            let children = groups
                .into_iter()
                .filter_map(|group| build_runtime_tree(group, horizontal_separator, vertical_separator))
                .collect();
            return Some(make_group_node(
                "horizontal",
                children,
                left,
                top,
                right,
                bottom,
                horizontal_separator,
                vertical_separator,
            ));
        }
    }
    nodes.into_iter().next()
}

fn runtime_tree(cells: &[RuntimeCell], horizontal_separator: i32, vertical_separator: i32) -> Option<RuntimeNode>
{
    build_runtime_tree(
        cells.iter().cloned().map(make_leaf_node).collect(),
        horizontal_separator,
        vertical_separator,
    )
}

fn resolve_runtime_node(
    node: &RuntimeNode,
    mut rect: Rect,
    resolved: &mut Vec<ResolvedRuntimeCell>,
    horizontal_separator: i32,
    vertical_separator: i32,
    mode: WindowMode,
)
{
    rect.width = max(1, rect.width);
    rect.height = max(1, rect.height);
    if node.children.is_empty()
    {
        if let Some(cell) = &node.cell
        {
            resolved.push(ResolvedRuntimeCell { cell: cell.clone(), rect });
        }
        return;
    }
    let vertical = node.orientation == "vertical";
    let fixed_total: i32 = node
        .children
        .iter()
        .filter_map(|child| if vertical { child.logical_height } else { child.logical_width })
        .sum();
    let expanded_count = node
        .children
        .iter()
        .filter(|child| if vertical { child.logical_height.is_none() } else { child.logical_width.is_none() })
        .count() as i32;
    let separator = if vertical { horizontal_separator } else { vertical_separator };
    let available = if vertical { rect.height } else { rect.width };
    let remaining = max(0, available - fixed_total - max(0, node.children.len() as i32 - 1) * separator);
    let distribute_fixed_extra = expanded_count == 0 && remaining > 0 && mode == WindowMode::Fullscreen;
    let append_trailing_extra = expanded_count == 0
        && remaining > 0
        && mode != WindowMode::Fullscreen
        && (!vertical || horizontal_separator > 0);
    let targets = if distribute_fixed_extra { node.children.len() as i32 } else { expanded_count };
    let share = if targets > 0 { remaining / targets } else { 0 };
    let remainder = if targets > 0 { remaining % targets } else { 0 };
    let mut expand_index = 0;
    let mut cursor = if vertical { rect.row } else { rect.col };
    for (index, child) in node.children.iter().enumerate()
    {
        let logical = if vertical { child.logical_height } else { child.logical_width };
        let mut child_size = logical.unwrap_or(0);
        if logical.is_none() || distribute_fixed_extra
        {
            child_size = if distribute_fixed_extra { child_size } else { 0 }
                + share
                + i32::from(expand_index < remainder);
            expand_index += 1;
        }
        else if append_trailing_extra && index + 1 == node.children.len()
        {
            child_size += remaining;
        }
        child_size = max(1, child_size);
        let child_rect = if vertical
        {
            Rect { row: cursor, col: rect.col, width: rect.width, height: child_size }
        }
        else
        {
            Rect { row: rect.row, col: cursor, width: child_size, height: rect.height }
        };
        resolve_runtime_node(child, child_rect, resolved, horizontal_separator, vertical_separator, mode);
        cursor += child_size + separator;
    }
}

fn window_mode(window: &GeneratedWindow) -> WindowMode
{
    let expands_width = window
        .layout
        .iter()
        .any(|item| dimension_mode(&item.cell_width_mode, item.cell_width) == "expanded");
    let expands_height = window
        .layout
        .iter()
        .any(|item| dimension_mode(&item.cell_height_mode, item.cell_height) == "expanded");
    match (expands_width, expands_height)
    {
        (true, true) => WindowMode::Fullscreen,
        (true, false) => WindowMode::ExpandWidth,
        (false, true) => WindowMode::ExpandHeight,
        (false, false) => WindowMode::Normal,
    }
}

fn layout_size(root: Option<&RuntimeNode>) -> Size
{
    let Some(root) = root else { return Size { width: 1, height: 1 } };
    Size
    {
        width: max(1, root.logical_width.unwrap_or(max(1, root.right - root.left - 1))),
        height: max(1, root.logical_height.unwrap_or(max(1, root.bottom - root.top - 1))),
    }
}

fn layout_content_rect(root: Option<&RuntimeNode>, mut content: Rect, mode: WindowMode) -> Rect
{
    let natural = layout_size(root);
    if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandWidth)
    {
        content.width = max(content.width, natural.width);
    }
    if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandHeight)
    {
        content.height = max(content.height, natural.height);
    }
    content
}

fn recompute_fit_content_heights(
    window: &GeneratedWindow,
    cells: &mut [RuntimeCell],
    resolved: &[ResolvedRuntimeCell],
) -> bool
{
    let mut changed = false;
    for cell in cells
    {
        let Some(item) = window.layout.iter().find(|item|
        {
            same_runtime_cell(cell, item)
                && dimension_mode(&item.cell_height_mode, item.cell_height) == "fit-content"
        }) else { continue };
        let Some(resolved_cell) = resolved.iter().find(|resolved_cell| resolved_cell.cell == *cell) else { continue };
        let height = fit_cell_height(window, item, Some(resolved_cell.rect.width));
        if cell.logical_height != Some(height)
        {
            cell.logical_height = Some(height);
            changed = true;
        }
    }
    changed
}

fn resolve_runtime_cells(
    window: &GeneratedWindow,
    cells: &mut [RuntimeCell],
    content: Rect,
    horizontal_separator: i32,
    vertical_separator: i32,
    mode: WindowMode,
) -> Vec<ResolvedRuntimeCell>
{
    let mut tree = runtime_tree(cells, horizontal_separator, vertical_separator);
    let mut resolved = Vec::new();
    if let Some(root) = &tree
    {
        resolve_runtime_node(
            root,
            layout_content_rect(Some(root), content, mode),
            &mut resolved,
            horizontal_separator,
            vertical_separator,
            mode,
        );
    }
    if recompute_fit_content_heights(window, cells, &resolved)
    {
        tree = runtime_tree(cells, horizontal_separator, vertical_separator);
        resolved.clear();
        if let Some(root) = &tree
        {
            resolve_runtime_node(
                root,
                layout_content_rect(Some(root), content, mode),
                &mut resolved,
                horizontal_separator,
                vertical_separator,
                mode,
            );
        }
    }
    resolved
}

fn border_width_horizontal(style: &Style) -> i32
{
    max(0, style.border_width_horizontal.unwrap_or(0))
}

fn border_width_vertical(style: &Style) -> i32
{
    max(0, style.border_width_vertical.unwrap_or(0))
}

fn natural_content_size(window: &GeneratedWindow) -> Size
{
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let cells = collect_runtime_cells(window);
    layout_size(runtime_tree(&cells, horizontal_separator, vertical_separator).as_ref())
}

fn resolved_content_extent(resolved: &[ResolvedRuntimeCell], fallback: Size) -> Size
{
    let mut result = Size
    {
        width: max(1, fallback.width),
        height: max(1, fallback.height),
    };
    for cell in resolved
    {
        result.width = max(result.width, cell.rect.col + cell.rect.width);
        result.height = max(result.height, cell.rect.row + cell.rect.height);
    }
    result
}

pub fn generated_window_content_size(window: &GeneratedWindow) -> Size
{
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let mut cells = collect_runtime_cells(window);
    let natural =
        layout_size(runtime_tree(&cells, horizontal_separator, vertical_separator).as_ref());
    let resolved = resolve_runtime_cells(
        window,
        &mut cells,
        Rect
        {
            row: 0,
            col: 0,
            width: natural.width,
            height: natural.height,
        },
        horizontal_separator,
        vertical_separator,
        window_mode(window),
    );
    resolved_content_extent(&resolved, natural)
}

pub fn generated_window_content_size_for_width(
    window: &GeneratedWindow,
    width: i32,
) -> Size
{
    let width = max(1, width);
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let mut cells = collect_runtime_cells(window);
    let natural =
        layout_size(runtime_tree(&cells, horizontal_separator, vertical_separator).as_ref());
    let resolved = resolve_runtime_cells(
        window,
        &mut cells,
        Rect { row: 0, col: 0, width, height: natural.height },
        horizontal_separator,
        vertical_separator,
        window_mode(window),
    );
    let mut result = resolved_content_extent(
        &resolved,
        Size { width, height: natural.height },
    );
    for item in &window.layout
    {
        let Some(cell) = resolved
            .iter()
            .find(|cell| same_runtime_cell(&cell.cell, item))
        else
        {
            continue;
        };
        let element = window.find_element(&item.content);
        let size = render_size_for(item, cell.rect, element.as_ref());
        let padding = item.cell_style.padding_values();
        result.width = max(
            result.width,
            cell.rect.col + padding[3] + item.col + size.width,
        );
        result.height = max(
            result.height,
            cell.rect.row + padding[0] + item.row + size.height,
        );
    }
    result
}

pub fn generated_window_resolved_content_size(
    window: &GeneratedWindow,
    size: Size,
) -> Size
{
    let size = Size
    {
        width: max(1, size.width),
        height: max(1, size.height),
    };
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let mut cells = collect_runtime_cells(window);
    let resolved = resolve_runtime_cells(
        window,
        &mut cells,
        Rect { row: 0, col: 0, width: size.width, height: size.height },
        horizontal_separator,
        vertical_separator,
        window_mode(window),
    );
    resolved_content_extent(&resolved, size)
}

pub(crate) fn generated_window_reported_size(window: &GeneratedWindow) -> Size
{
    let mut size = generated_window_content_size(window);
    size.width += border_width_vertical(&window.window_style) * 2;
    size.height += border_width_horizontal(&window.window_style) * 2;
    size
}

pub(crate) fn active_window_reported_size(window: &GeneratedWindow, viewport: Size) -> Size
{
    let mut size = generated_window_reported_size(window);

    let margins = window.window_style.margin_values();
    let available_width = max(1, viewport.width - max(0, margins[1]) - max(0, margins[3]));
    let available_height = max(1, viewport.height - max(0, margins[0]) - max(0, margins[2]));
    match window_mode(window)
    {
        WindowMode::Fullscreen =>
        {
            size.width = available_width;
            size.height = available_height;
        }
        WindowMode::ExpandWidth =>
        {
            size.width = available_width;
            size.height = min(size.height, available_height);
        }
        WindowMode::ExpandHeight =>
        {
            size.width = min(size.width, available_width);
            size.height = available_height;
        }
        WindowMode::Normal =>
        {
            size.width = min(size.width, available_width);
            size.height = min(size.height, available_height);
        }
    }
    size
}

pub(crate) fn generated_window_mode_name(window: &GeneratedWindow) -> &'static str
{
    match window_mode(window)
    {
        WindowMode::Fullscreen => "fullscreen",
        WindowMode::ExpandWidth => "expand_width",
        WindowMode::ExpandHeight => "expand_height",
        WindowMode::Normal => "normal",
    }
}

fn window_bounds(window: &GeneratedWindow, viewport: Size, force_fullscreen: bool) -> Rect
{
    let mode = if force_fullscreen { WindowMode::Fullscreen } else { window_mode(window) };
    let natural = generated_window_content_size(window);
    let horizontal_border = border_width_horizontal(&window.window_style);
    let vertical_border = border_width_vertical(&window.window_style);
    let margins = window.window_style.margin_values();
    let available_width = max(1, viewport.width - margins[1] - margins[3]);
    let available_height = max(1, viewport.height - margins[0] - margins[2]);
    let natural_width = natural.width + vertical_border * 2;
    let natural_height = natural.height + horizontal_border * 2;
    let width = if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandWidth)
    {
        available_width
    }
    else
    {
        min(natural_width, available_width)
    };
    let height = if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandHeight)
    {
        available_height
    }
    else
    {
        min(natural_height, available_height)
    };
    Rect
    {
        row: margins[0]
            + if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandHeight)
            {
                0
            }
            else
            {
                max(0, (available_height - height) / 2)
            },
        col: margins[3]
            + if matches!(mode, WindowMode::Fullscreen | WindowMode::ExpandWidth)
            {
                0
            }
            else
            {
                max(0, (available_width - width) / 2)
            },
        width,
        height,
    }
}

fn content_bounds(bounds: Rect, style: &Style) -> Rect
{
    let horizontal = border_width_horizontal(style);
    let vertical = border_width_vertical(style);
    Rect
    {
        row: bounds.row + horizontal,
        col: bounds.col + vertical,
        width: max(1, bounds.width - vertical * 2),
        height: max(1, bounds.height - horizontal * 2),
    }
}

fn resolve_layout_geometry(window: &mut GeneratedWindow, viewport: Size, force_fullscreen: bool) -> LayoutResolution
{
    let bounds = window_bounds(window, viewport, force_fullscreen);
    let content = content_bounds(bounds, &window.window_style);
    let mode = if force_fullscreen { WindowMode::Fullscreen } else { window_mode(window) };
    let horizontal_separator = border_width_horizontal(&window.window_style);
    let vertical_separator = border_width_vertical(&window.window_style);
    let mut runtime_cells = collect_runtime_cells(window);
    let cells = resolve_runtime_cells(
        window,
        &mut runtime_cells,
        content,
        horizontal_separator,
        vertical_separator,
        mode,
    );
    let by_name: HashMap<String, ElementRef> = window
        .elements
        .iter()
        .map(|element| (element.borrow().name.clone(), element.clone()))
        .collect();
    for item in &window.layout
    {
        let Some(cell) = cells.iter().find(|cell| same_runtime_cell(&cell.cell, item)) else { continue };
        if let Some(element) = by_name.get(&item.content)
        {
            let rect = resolved_item_rect(&window.layout, item, cell.rect, Some(element));
            let mut element_value = element.borrow_mut();
            element_value.frame = rect;
            if element_value.kind() == ElementKind::ListBox
            {
                element_value.ensure_selected_visible();
            }
        }
    }
    LayoutResolution { bounds, content, cells }
}

fn dimension_within(mode: &str, value: i32, available: i32, fallback: i32) -> i32
{
    let available = max(1, available);
    match dimension_mode(mode, value)
    {
        "expanded" => available,
        "fit-content" => max(1, min(max(1, fallback), available)),
        _ => max(1, if value > 0 { value } else { fallback }),
    }
}

fn render_size_for(item: &LayoutItem, cell_rect: Rect, element: Option<&ElementRef>) -> Size
{
    let padding = item.cell_style.padding_values();
    let available_width = cell_rect.width - padding[1] - padding[3] - item.col - item.margin_right;
    let available_height = cell_rect.height - padding[0] - padding[2] - item.row - item.margin_bottom;
    let fallback_width = max(1, if item.chars_width > 0 { item.chars_width } else { item.cell_chars_width });
    let mut fallback_height = max(1, if item.chars_height > 0 { item.chars_height } else { item.cell_chars_height });
    if dimension_mode(&item.height_mode, item.height) == "fit-content"
    {
        fallback_height = fit_content_height_for(element, item, Some(max(1, available_width)));
    }
    if element.is_none() && item.element_type == "label"
    {
        return Size
        {
            width: max(1, min(fallback_width, max(1, cell_rect.width - padding[3] - item.col))),
            height: dimension_within(&item.height_mode, item.height, available_height, fallback_height),
        };
    }
    Size
    {
        width: dimension_within(&item.width_mode, item.width, available_width, fallback_width),
        height: dimension_within(&item.height_mode, item.height, available_height, fallback_height),
    }
}

fn source_col_span(item: &LayoutItem) -> (i32, i32)
{
    let width = max(1, if item.chars_width > 0 { item.chars_width } else { item.cell_chars_width });
    (item.col, item.col + width)
}

fn spans_overlap(first: (i32, i32), second: (i32, i32)) -> bool
{
    first.0 < second.1 && second.0 < first.1
}

fn has_expanded_before_in_row(layout: &[LayoutItem], item: &LayoutItem) -> bool
{
    layout.iter().any(|other|
    {
        same_source_cell(other, item)
            && other.row == item.row
            && other.col < item.col
            && dimension_mode(&other.width_mode, other.width) == "expanded"
    })
}

fn has_expanded_before_in_column(layout: &[LayoutItem], item: &LayoutItem) -> bool
{
    layout.iter().any(|other|
    {
        same_source_cell(other, item)
            && other.row < item.row
            && spans_overlap(source_col_span(other), source_col_span(item))
            && dimension_mode(&other.height_mode, other.height) == "expanded"
    })
}

fn cell_content_clip(item: &LayoutItem, cell_rect: Rect) -> Rect
{
    let padding = item.cell_style.padding_values();
    Rect
    {
        row: cell_rect.row + padding[0],
        col: cell_rect.col + padding[3],
        width: max(0, cell_rect.width - padding[3] - padding[1]),
        height: max(0, cell_rect.height - padding[0] - padding[2]),
    }
}

fn resolved_item_rect(
    layout: &[LayoutItem],
    item: &LayoutItem,
    cell_rect: Rect,
    element: Option<&ElementRef>,
) -> Rect
{
    let size = render_size_for(item, cell_rect, element);
    let padding = item.cell_style.padding_values();
    let base_col = padding[3] + item.col;
    let width_delta = max(0, cell_rect.width - item.cell_chars_width);
    let col = if item.margin_right == 0
        && dimension_mode(&item.cell_width_mode, item.cell_width) == "expanded"
        && item.cell_chars_width > 0
        && cell_rect.width > item.cell_chars_width
        && dimension_mode(&item.width_mode, item.width) != "expanded"
    {
        cell_rect.col + max(base_col, cell_rect.width - size.width - item.margin_right - padding[1])
    }
    else if has_expanded_before_in_row(layout, item)
        && dimension_mode(&item.cell_width_mode, item.cell_width) == "expanded"
        && width_delta > 0
        && dimension_mode(&item.width_mode, item.width) != "expanded"
    {
        cell_rect.col + max(base_col, base_col + width_delta)
    }
    else
    {
        cell_rect.col + base_col
    };
    let base_row = padding[0] + item.row;
    let height_delta = max(0, cell_rect.height - item.cell_chars_height);
    let row = if has_expanded_before_in_column(layout, item)
        && dimension_mode(&item.cell_height_mode, item.cell_height) == "expanded"
        && height_delta > 0
        && dimension_mode(&item.height_mode, item.height) != "expanded"
    {
        cell_rect.row + max(base_row, base_row + height_delta)
    }
    else
    {
        cell_rect.row + base_row
    };
    Rect { row, col, width: size.width, height: size.height }
}

pub fn resolve_layout(window: &mut GeneratedWindow, viewport: Size, force_fullscreen: bool)
{
    let _ = resolve_layout_geometry(window, viewport, force_fullscreen);
}

pub fn run_generated_app_main(run: impl FnOnce() -> i32) -> i32
{
    match catch_unwind(AssertUnwindSafe(run))
    {
        Ok(code) => code,
        Err(payload) =>
        {
            let message = payload
                .downcast_ref::<String>()
                .map(String::as_str)
                .or_else(|| payload.downcast_ref::<&str>().copied())
                .unwrap_or("unknown Rust runtime error");
            eprintln!("uimd: error: {message}");
            1
        }
    }
}

pub fn run_generated_window<A: GeneratedApplication>(
    app: &mut A,
    options: GeneratedWindowRuntimeOptions,
    args: &[String],
) -> i32
{
    let config = crate::McpRuntimeConfig::parse(args);
    if config.enabled && !config.gui
    {
        return crate::serve_mcp(app, options, config);
    }
    if !io::stdout().is_terminal()
    {
        let viewport = Size { width: DEFAULT_VIEWPORT_WIDTH, height: DEFAULT_VIEWPORT_HEIGHT };
        let mut state = RuntimeState::new(app.window(), &options, viewport);
        state.notify_initial_edit_started(app, &options);
        let rendered = render_generated_application(app, &mut state, &options);
        println!("{}", rendered.plain_text());
        return 0;
    }
    crate::run_interactive_terminal(app, options, config)
}

#[cfg(test)]
mod tests
{
    use super::*;
    use crate::{
        new_button, new_checkbox, new_combo_box, new_label, new_list_box,
        new_reusable_element, new_scroll_view, new_text_input, new_view_host,
        MessageBoxResult, MessageBoxYesNo,
    };

    fn render_diff_between(
        current: &TerminalBuffer,
        previous: Option<&TerminalBuffer>,
    ) -> String
    {
        let mut owned = current.clone();
        owned.import_previous_frame(previous);
        owned.render_diff(0, 0)
    }

    struct App
    {
        window: GeneratedWindow,
        activated: bool,
    }

    impl GeneratedApplication for App
    {
        fn window(&self) -> &GeneratedWindow { &self.window }
        fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
        fn handle_generated_button(&mut self, name: &str) -> bool
        {
            self.activated = name == "ok";
            self.activated
        }
    }

    #[test]
    fn enter_activates_focused_button_without_edit_mode()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        app.window.add_element(new_button("ok", "OK"));
        let options = GeneratedWindowRuntimeOptions { initial_focus_name: "ok".to_string(), ..Default::default() };
        let mut state = RuntimeState::new(&app.window, &options, Size { width: 10, height: 3 });
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(app.activated);
        assert!(!state.edit_mode);
    }

    #[test]
    fn single_line_text_input_consumes_vertical_arrows_without_leaving_edit_mode()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let name = new_text_input("name", "Marek", 0);
        name.borrow_mut().set_frame(Rect
        {
            row: 0,
            col: 0,
            width: 10,
            height: 1,
        });
        let email = new_text_input("email", "marek@example.com", 0);
        email.borrow_mut().set_frame(Rect
        {
            row: 2,
            col: 0,
            width: 20,
            height: 1,
        });
        app.window.add_element(name.clone());
        app.window.add_element(email);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "name".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 24, height: 6 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.edit_mode);
        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(state.edit_mode);
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &name)));
        assert_eq!(name.borrow().value(), "Marek");
        assert!(state.handle_key(&mut app, &options, "Up"));
        assert!(state.edit_mode);
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &name)));
    }

    #[test]
    fn scoped_single_line_text_input_consumes_vertical_arrows_without_navigation()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        let name = scroll.add_element(new_text_input("name", "Marek", 0));
        name.borrow_mut().set_frame(Rect
        {
            row: 0,
            col: 0,
            width: 10,
            height: 1,
        });
        let email = scroll.add_element(new_text_input("email", "marek@example.com", 0));
        email.borrow_mut().set_frame(Rect
        {
            row: 2,
            col: 0,
            width: 20,
            height: 1,
        });
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let panel = app.window.add_element(new_reusable_element("panel", "Items"));
        panel.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "panel".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 24, height: 6 },
        );
        state.scope_edit_element = Some(panel);
        assert!(state.focus_element(&app.window, &name));

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.edit_mode);
        assert!(state.scope_dim_element.as_ref().is_some_and(|editing|
        {
            Rc::ptr_eq(editing, &name)
        }));
        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(state.edit_mode);
        assert!(state.scope_dim_element.as_ref().is_some_and(|editing|
        {
            Rc::ptr_eq(editing, &name)
        }));
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &name)));
        assert_eq!(name.borrow().value(), "Marek");
        assert!(state.handle_key(&mut app, &options, "Up"));
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &name)));
    }

    #[test]
    fn scoped_confirm_retains_live_input_and_rebases_focus_after_mutation()
    {
        struct ConfirmApp
        {
            window: GeneratedWindow,
            leading: ElementRef,
            trailing_activations: usize,
        }

        impl GeneratedApplication for ConfirmApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
            fn handle_generated_text_confirmed(&mut self, _name: &str, _value: &str) -> bool
            {
                self.leading.borrow_mut().set_enabled(false);
                true
            }
            fn handle_generated_button(&mut self, name: &str) -> bool
            {
                if name.ends_with("trailing")
                {
                    self.trailing_activations += 1;
                }
                true
            }
        }

        let fixture = |keep_edit_mode_after_confirm|
        {
            let mut scroll = GeneratedWindow::new_scroll_view("items");
            let leading = scroll.add_element(new_button("leading", "Leading"));
            leading.borrow_mut().set_frame(Rect
            {
                row: 0,
                col: 0,
                width: 12,
                height: 1,
            });
            let input = scroll.add_element(new_text_input("filter", "", 0));
            input.borrow_mut().set_frame(Rect
            {
                row: 1,
                col: 0,
                width: 12,
                height: 1,
            });
            let trailing = scroll.add_element(new_button("trailing", "Trailing"));
            trailing.borrow_mut().set_frame(Rect
            {
                row: 2,
                col: 0,
                width: 12,
                height: 1,
            });
            let host: ElementRef = new_reusable_element("items", "Items").into();
            host.borrow_mut().set_child_window(scroll);
            let mut window = GeneratedWindow::new("test");
            window.add_element(host.clone());
            let app = ConfirmApp
            {
                window,
                leading: leading.clone(),
                trailing_activations: 0,
            };
            let options = GeneratedWindowRuntimeOptions
            {
                initial_focus_name: "items".to_string(),
                keep_edit_mode_after_confirm,
                ..Default::default()
            };
            let mut state = RuntimeState::new(
                &app.window,
                &options,
                Size { width: 16, height: 5 },
            );
            state.scope_edit_element = Some(host);
            assert!(state.focus_element(&app.window, &input));
            (app, state, options, leading, input, trailing)
        };

        let (mut app, mut state, options, leading, input, _) = fixture(true);
        for key in ["Enter", "a", "Enter"]
        {
            assert!(state.handle_key(&mut app, &options, key));
        }
        assert!(!leading.borrow().enabled());
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &input)));
        assert!(state.scope_dim_element.as_ref().is_some_and(|editing|
        {
            Rc::ptr_eq(editing, &input)
        }));
        assert!(state.edit_snapshot.as_ref().is_some_and(|snapshot|
        {
            Rc::ptr_eq(&snapshot.element, &input)
        }));
        assert!(state.handle_key(&mut app, &options, "b"));
        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert_eq!(input.borrow().value(), "a");

        let (mut app, mut state, options, _, input, _) = fixture(false);
        for key in ["Enter", "a", "Enter"]
        {
            assert!(state.handle_key(&mut app, &options, key));
        }
        assert!(state.scope_dim_element.is_none());
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &input)));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.scope_dim_element.as_ref().is_some_and(|editing|
        {
            Rc::ptr_eq(editing, &input)
        }));
        assert_eq!(app.trailing_activations, 0);
    }

    #[test]
    fn enter_activates_the_child_owned_reusable_control_name_like_cpp()
    {
        struct ReusableApp
        {
            window: GeneratedWindow,
            activated: String,
        }

        impl GeneratedApplication for ReusableApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
            fn handle_generated_button(&mut self, name: &str) -> bool
            {
                self.activated = name.to_string();
                true
            }
        }

        let mut child = GeneratedWindow::new("image_button");
        child.set_generated_focusable(true);
        child.set_generated_control_activation("photo");
        let reusable = new_reusable_element("tile", "ImageButton");
        reusable.borrow_mut().set_child_window(child);
        let mut app = ReusableApp
        {
            window: GeneratedWindow::new("test"),
            activated: String::new(),
        };
        app.window.add_element(reusable);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "tile".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 10, height: 3 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert_eq!(app.activated, "tile.photo");
        assert!(!state.edit_mode);
        assert!(state.scope_dim_element.is_none());
    }

    #[test]
    fn scroll_view_scoped_reusable_control_activates_with_enter_and_space()
    {
        struct ReusableApp
        {
            window: GeneratedWindow,
            activation_count: usize,
        }

        impl GeneratedApplication for ReusableApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
            fn handle_generated_button(&mut self, _name: &str) -> bool
            {
                self.activation_count += 1;
                true
            }
        }

        let mut child = GeneratedWindow::new("action");
        child.set_generated_focusable(true);
        child.set_generated_control_activation("run");
        let action = new_reusable_element("action", "Action");
        action.borrow_mut().set_child_window(child);
        action.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 12, height: 1 };
        let scroll = GeneratedWindow::new_scroll_view("items");
        scroll.add_child(action.clone());
        let host = new_reusable_element("items", "Items");
        host.borrow_mut().set_child_window(scroll);
        host.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 12, height: 1 };
        let mut app = ReusableApp
        {
            window: GeneratedWindow::new("test"),
            activation_count: 0,
        };
        app.window.add_element(host);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "items".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 12, height: 3 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state
            .focused_element(&app.window)
            .is_some_and(|focused| Rc::ptr_eq(&focused, &action)));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert_eq!(app.activation_count, 1);
        assert!(state.handle_key(&mut app, &options, " "));
        assert_eq!(app.activation_count, 2);
    }

    #[test]
    fn root_initial_edit_requires_an_editable_element_and_notifies_once_like_cpp()
    {
        let events = Rc::new(std::cell::RefCell::new(Vec::<String>::new()));
        let callback_events = events.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "value".to_string(),
            start_in_edit_mode: true,
            on_edit_started: Some(Rc::new(move |name|
            {
                callback_events.borrow_mut().push(name.to_string());
            })),
            ..Default::default()
        };
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        app.window.add_element(new_text_input("value", "text", 0));
        let state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 10, height: 3 },
        );
        assert!(state.edit_mode);
        state.notify_initial_edit_started(&mut app, &options);
        assert_eq!(events.borrow().as_slice(), ["value"]);

        let mut button_app =
            App { window: GeneratedWindow::new("button"), activated: false };
        button_app.window.add_element(new_button("run", "Run"));
        let button_options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "run".to_string(),
            start_in_edit_mode: true,
            ..Default::default()
        };
        let button_state = RuntimeState::new(
            &button_app.window,
            &button_options,
            Size { width: 10, height: 3 },
        );
        assert!(!button_state.edit_mode);
    }

    #[test]
    fn checkbox_change_dispatch_never_queries_choice_values()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let checkbox = new_checkbox("enabled", "Enabled", false);
        app.window.add_element(checkbox.clone());
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "enabled".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 10, height: 3 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(checkbox.borrow().checked());
    }

    #[test]
    fn window_stack_public_state_and_default_frame_policy_match_cpp()
    {
        let defaults = GeneratedWindowFrameOptions::default();
        assert!(defaults.dim_background);

        let mut first = GeneratedWindow::new("first");
        first.add_element(new_text_input("value", "", 0));
        let first_handle = first.clone();
        let second = GeneratedWindow::new("second");
        let mut stack = GeneratedWindowStack::default();
        stack.push(
            first,
            GeneratedWindowFrameOptions
            {
                start_in_edit_mode: true,
                ..defaults
            },
        );
        assert_eq!(stack.top().map(|frame| frame.focused_index), Some(0));
        assert!(stack.top().is_some_and(|frame| frame.edit_mode));
        stack.push(second, GeneratedWindowFrameOptions::default());
        assert_eq!(stack.frames().len(), 2);
        stack.frames_mut()[0].focused_index = 3;
        stack.remove(&first_handle);
        assert_eq!(stack.frames().len(), 1);
        assert_eq!(stack.top().map(|frame| frame.window.title.as_str()), Some("second"));
    }

    #[test]
    fn nested_modal_opening_uses_one_flat_root_window_stack()
    {
        let mut root = GeneratedWindow::new("root");
        root.open_window(GeneratedWindow::new("first"));
        root.active_window_mut()
            .open_window(GeneratedWindow::new("second"));
        assert_eq!(root.window_stack.frames().len(), 1);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &root,
            &options,
            Size { width: 20, height: 8 },
        );
        state.sync_active_window(&mut root, &options);

        assert_eq!(root.modal_depth(), 2);
        assert_eq!(root.window_stack.frames().len(), 2);
        assert_eq!(root.active_window().title(), "second");
        assert!(
            root.window_stack
                .frames()
                .iter()
                .all(|frame| frame.window.window_stack.frames().is_empty()),
        );
    }

    #[test]
    fn runtime_and_modal_frame_callbacks_follow_the_reference_dispatch_contract()
    {
        let root_events = Rc::new(std::cell::RefCell::new(Vec::<String>::new()));
        let root_button_events = root_events.clone();
        let root_focus_events = root_events.clone();
        let root_edit_events = root_events.clone();
        let root_overlay_events = root_events.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "open".to_string(),
            on_button: Some(Rc::new(move |name|
            {
                root_button_events.borrow_mut().push(format!("button:{name}"));
            })),
            on_focus_changed: Some(Rc::new(move |name, focused|
            {
                root_focus_events
                    .borrow_mut()
                    .push(format!("focus:{name}:{focused}"));
            })),
            on_edit_started: Some(Rc::new(move |name|
            {
                root_edit_events.borrow_mut().push(format!("edit:{name}"));
            })),
            on_render_overlay: Some(Rc::new(move |buffer|
            {
                root_overlay_events.borrow_mut().push("overlay".to_string());
                buffer.draw_text(
                    0,
                    0,
                    "X",
                    &Color::new("#ffffff"),
                    &Color::new("#000000"),
                    1,
                );
            })),
            should_dim_overlay_background: Some(Rc::new(|| true)),
            ..Default::default()
        };
        let mut app = App { window: GeneratedWindow::new("root"), activated: false };
        app.window.add_element(new_button("open", "Open"));
        app.window.add_element(new_text_input("name", "", 0));
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 6 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(!app.activated, "runtime option callback owns the root button event");
        assert!(state.handle_key(&mut app, &options, "Tab"));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        let rendered = render_generated_application(&mut app, &mut state, &options);
        assert_eq!(rendered.cell(0, 0).expect("overlay cell").text, "X");
        assert!(root_events.borrow().iter().any(|event| event == "button:open"));
        assert!(root_events.borrow().iter().any(|event| event == "focus:open:false"));
        assert!(root_events.borrow().iter().any(|event| event == "focus:name:true"));
        assert!(root_events.borrow().iter().any(|event| event == "edit:name"));
        assert!(root_events.borrow().iter().any(|event| event == "overlay"));

        let modal_events = Rc::new(std::cell::RefCell::new(Vec::<String>::new()));
        let modal_button_events = modal_events.clone();
        let mut modal = GeneratedWindow::new("modal");
        modal.set_mcp_metadata(true, "Modal", "", "");
        modal.add_element(new_button("confirm", "Confirm"));
        modal.set_runtime_options(GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "confirm".to_string(),
            on_button: Some(Rc::new(move |name|
            {
                modal_button_events.borrow_mut().push(name.to_string());
            })),
            ..Default::default()
        });
        app.window.open_window(modal);
        let _ = render_generated_application(&mut app, &mut state, &options);
        assert!(app
            .window
            .window_stack
            .top()
            .is_some_and(|frame| frame.bounds.width > 0 && frame.bounds.height > 0));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert_eq!(modal_events.borrow().as_slice(), ["confirm"]);
        assert!(!root_events.borrow().iter().any(|event| event == "button:confirm"));
    }

    #[test]
    fn keyboard_focus_movement_notifies_only_actual_previous_and_next_elements()
    {
        let events = Rc::new(std::cell::RefCell::new(Vec::<String>::new()));
        let callback_events = events.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "first".to_string(),
            on_focus_changed: Some(Rc::new(move |name, focused|
            {
                callback_events
                    .borrow_mut()
                    .push(format!("{name}:{focused}"));
            })),
            ..Default::default()
        };
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let first = new_button("first", "First");
        first.borrow_mut().set_frame(Rect
        {
            row: 0,
            col: 0,
            width: 8,
            height: 1,
        });
        let second = new_button("second", "Second");
        second.borrow_mut().set_frame(Rect
        {
            row: 2,
            col: 0,
            width: 8,
            height: 1,
        });
        app.window.add_element(first);
        app.window.add_element(second);
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 6 },
        );

        assert!(state.handle_key(&mut app, &options, "Tab"));
        assert_eq!(events.borrow().as_slice(), ["first:false", "second:true"]);
        events.borrow_mut().clear();

        let _ = state.handle_key(&mut app, &options, "Right");
        assert!(events.borrow().is_empty());
    }

    #[test]
    fn concrete_message_box_owns_escape_result_and_modal_close_lifecycle()
    {
        struct ModalApp
        {
            window: GeneratedWindow,
            closed_identities: Vec<u64>,
        }

        impl GeneratedApplication for ModalApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }
            fn handle_generated_window_closed(&mut self, window: GeneratedWindow)
            {
                self.closed_identities.push(window.identity());
            }
        }

        let mut app = ModalApp
        {
            window: GeneratedWindow::new("root"),
            closed_identities: Vec::new(),
        };
        app.window.add_element(new_button("open", "Open"));
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "open".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 10 },
        );
        let mut dialog = MessageBoxYesNo::new("Confirm", "Continue?");
        let window = dialog.take_window();
        let dialog_identity = window.identity();
        app.window.open_window(window);
        let _ = render_generated_application(&mut app, &mut state, &options);

        assert!(state.begin_standard_escape(&mut app, &options));
        state
            .pending_standard_escape_button
            .as_mut()
            .expect("pending semantic Escape button")
            .1 = Instant::now();
        assert!(state.complete_pending_standard_escape(&mut app, &options));
        assert!(close_completed_modal(&mut app, &mut state, &options));

        assert_eq!(dialog.result(), Some(MessageBoxResult::No));
        assert_eq!(app.window.modal_depth(), 0);
        assert_eq!(app.closed_identities, [dialog_identity]);
        assert_eq!(
            state
                .focused_element(&app.window)
                .expect("root focus restored")
                .borrow()
                .name,
            "open",
        );
    }

    #[test]
    fn file_browser_escape_flashes_close_before_dispatch_like_cpp()
    {
        let mut app = App { window: GeneratedWindow::new("browser"), activated: false };
        app.window.set_mcp_metadata(true, "FileBrowser", "", "");
        app.window.add_element(new_button("open_btn", "Open"));
        app.window.add_element(new_button("close_btn", "Close"));
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "open_btn".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );

        assert!(state.begin_standard_escape(&mut app, &options));
        assert!(state.has_pending_standard_escape());
        assert_eq!(
            state
                .focused_element(&app.window)
                .expect("FileBrowser close focus")
                .borrow()
                .name,
            "close_btn",
        );
    }

    #[test]
    fn file_browser_escape_leaves_list_edit_before_flashing_close_like_cpp()
    {
        let mut app = App { window: GeneratedWindow::new("browser"), activated: false };
        app.window.set_mcp_metadata(true, "FileBrowser", "", "");
        app.window.add_element(new_list_box(
            "entries",
            vec!["..".to_string(), "document.txt".to_string()],
            false,
        ));
        app.window.add_element(new_button("open_btn", "Open"));
        app.window.add_element(new_button("close_btn", "Close"));
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "entries".to_string(),
            start_in_edit_mode: true,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );

        assert!(!state.begin_standard_escape(&mut app, &options));
        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(!state.edit_mode);
        assert!(!state.has_pending_standard_escape());
        assert!(state.begin_standard_escape(&mut app, &options));
        assert!(state.has_pending_standard_escape());
    }

    #[test]
    fn terminal_diff_skips_unchanged_raw_images()
    {
        let mut previous = TerminalBuffer::new(4, 2);
        previous.cell_mut(0, 0).expect("raw anchor").raw =
            "\x1bPqimage\x1b\\".to_string();
        let mut current = previous.clone();
        let unchanged = render_diff_between(&current, Some(&previous));
        assert!(unchanged.is_empty());

        current.cell_mut(1, 3).expect("changed text cell").text = "x".to_string();
        let output = render_diff_between(&current, Some(&previous));
        assert!(output.contains("\x1b[2;4H"));
        assert!(!output.contains("image"));
    }

    #[test]
    fn terminal_diff_preserves_raw_image_when_only_covered_cell_style_changes()
    {
        let mut previous = TerminalBuffer::new(4, 2);
        {
            let anchor = previous.cell_mut(0, 0).expect("raw anchor");
            anchor.raw = "\x1bPqimage\x1b\\".to_string();
            anchor.raw_width = 2;
            anchor.raw_height = 2;
        }
        for (row, col) in [(0, 1), (1, 0), (1, 1)]
        {
            previous.cell_mut(row, col).expect("raw covered cell").raw_skip = true;
        }
        let mut current = previous.clone();
        for (row, col) in [(0, 1), (1, 0), (1, 1)]
        {
            current
                .cell_mut(row, col)
                .expect("changed covered cell")
                .background = Some(Color::new("#2563eb"));
        }

        let diff = render_diff_between(&current, Some(&previous));
        assert!(
            diff.is_empty(),
            "covered Sixel cells must not emit spaces that erase the unchanged image",
        );
    }

    #[test]
    fn terminal_diff_reemits_raw_image_when_anchor_style_changes()
    {
        let mut previous = TerminalBuffer::new(4, 2);
        {
            let anchor = previous.cell_mut(0, 0).expect("raw anchor");
            anchor.raw = "\x1bPqimage\x1b\\".to_string();
            anchor.raw_width = 2;
            anchor.raw_height = 2;
        }
        for (row, col) in [(0, 1), (1, 0), (1, 1)]
        {
            previous.cell_mut(row, col).expect("raw covered cell").raw_skip = true;
        }
        previous.cell_mut(0, 3).expect("text overlay cell").text = "T".to_string();
        let mut current = previous.clone();
        current.cell_mut(0, 0).expect("changed raw anchor").background =
            Some(Color::new("#2563eb"));

        let output = render_diff_between(&current, Some(&previous));
        assert!(output.contains("\x1b[?2026h"));
        assert!(output.contains("\x1bPqimage\x1b\\"));
        assert!(output.contains('T'));
        assert!(output.contains("\x1b[?2026l"));
    }

    #[test]
    fn reentering_edit_mode_replaces_the_escape_snapshot()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let input = app.window.add_element(new_text_input("name", "before", 0));
        let options = GeneratedWindowRuntimeOptions { initial_focus_name: "name".to_string(), ..Default::default() };
        let mut state = RuntimeState::new(&app.window, &options, Size { width: 20, height: 3 });

        state.enter_element_edit(&input);
        input.borrow_mut().set_value("baseline");
        state.enter_element_edit(&input);
        input.borrow_mut().set_value("changed");

        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert_eq!(input.borrow().value(), "baseline");
        assert!(!state.edit_mode);
    }

    #[test]
    fn single_select_listbox_enter_commits_and_leaves_edit_mode()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let list = app.window.add_element(new_list_box(
            "mode",
            vec!["Preview".to_string(), "Review".to_string()],
            false,
        ));
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "mode".to_string(),
            start_in_edit_mode: true,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 3 },
        );

        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert_eq!(list.borrow().selected_value(), "Review");
        assert!(!state.edit_mode);
    }

    #[test]
    fn reusable_child_ids_are_qualified_without_changing_layout_names()
    {
        let mut child = GeneratedWindow::new("child");
        let button = child.add_element(new_button("save", "Save"));
        let host = new_reusable_element("panel", "Child");
        host.borrow_mut().set_child_window(child);
        let mut window = GeneratedWindow::new("parent");
        window.add_element(host.clone());

        assert_eq!(host.borrow().name, "panel");
        assert_eq!(window.element_id(&host).as_deref(), Some("panel"));
        assert_eq!(button.borrow().name, "save");
        assert_eq!(window.element_id(&button).as_deref(), Some("panel.save"));
    }

    #[test]
    fn indexed_reusable_child_ids_join_without_an_extra_separator()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("Items");
        let mut first_row = GeneratedWindow::new("Row");
        first_row.add_element(new_button("open_btn", "Open"));
        scroll.add_named_child_window("first", first_row);
        let mut second_row = GeneratedWindow::new("Row");
        let second_button = second_row.add_element(new_button("open_btn", "Open"));
        scroll.add_named_child_window("second", second_row);
        let host = new_reusable_element("page", "Items");
        host.borrow_mut().set_child_window(scroll);
        let mut window = GeneratedWindow::new("parent");
        window.add_element(host);
        let child = window
            .find_element("page[1].open_btn")
            .expect("indexed child");
        assert!(Rc::ptr_eq(&child, &second_button));
    }

    #[test]
    fn interaction_rebinds_to_recreated_descendant_with_the_same_id()
    {
        let mut first_child = GeneratedWindow::new("child");
        let first_done = first_child.add_element(new_checkbox("done", "Done", false));
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let host = app.window.add_element(new_reusable_element("panel", "Child"));
        host.borrow_mut().set_child_window(first_child);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 3 },
        );
        state.scope_edit_element = Some(host.clone());
        assert!(state.focus_element(&app.window, &first_done));
        state.edit_mode = true;
        let interaction_ids = state.interaction_element_ids(&app.window);

        let mut replacement_child = GeneratedWindow::new("child");
        let replacement_done =
            replacement_child.add_element(new_checkbox("done", "Done", true));
        host.borrow_mut().set_child_window(replacement_child);
        state.repair_live_interaction_with_ids(
            &app.window,
            interaction_ids,
            None,
        );

        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("recreated descendant focus"),
            &replacement_done,
        ));
        assert!(std::rc::Rc::ptr_eq(
            &state.scope_edit_element.expect("live reusable scope"),
            &host,
        ));
    }

    #[test]
    fn interaction_clears_when_a_scoped_page_replacement_removes_the_descendant()
    {
        let mut first_child = GeneratedWindow::new_scroll_view("list");
        let first_open = first_child.add_element(new_button("open", "Open"));
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let host = app.window.add_element(new_view_host("page"));
        host.borrow_mut().set_child_window(first_child);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 3 },
        );
        state.scope_edit_element = Some(host.clone());
        assert!(state.focus_element(&app.window, &first_open));
        state.edit_mode = true;

        let mut replacement_child = GeneratedWindow::new("edit");
        replacement_child.add_element(new_button("back", "Back"));
        host.borrow_mut().set_child_window(replacement_child);
        state.repair_live_interaction(&app.window);

        assert!(state.focused_element(&app.window).is_none());
        assert!(state.scope_edit_element.is_none());
        assert!(state.scope_dim_element.is_none());
        assert!(!state.edit_mode);
        assert_eq!(state.focused_index, -1);
    }

    #[test]
    fn modal_return_preserves_empty_scroll_scope_after_focused_row_is_removed()
    {
        let mut row = GeneratedWindow::new("row");
        let delete = row.add_element(new_button("delete", "Delete"));
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.add_named_child_window("[0]", row);

        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let host = app.window.add_element(new_reusable_element("items", "Items"));
        host.borrow_mut().frame = Rect
        {
            row: 0,
            col: 0,
            width: 20,
            height: 5,
        };
        host.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );
        state.focused_index = 0;
        state.scope_edit_element = Some(host.clone());
        assert!(state.focus_element(&app.window, &delete));
        state.edit_mode = true;

        app.window.open_window(GeneratedWindow::new("confirm"));
        state.sync_active_window(&mut app.window, &options);
        host.borrow_mut()
            .child_window_mut()
            .expect("scroll view")
            .clear_children();
        app.window.close_active_window();
        state.sync_active_window(&mut app.window, &options);

        assert!(state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            state.scope_edit_element.as_ref().expect("restored scroll scope"),
            &host,
        ));
        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("restored proxy focus"),
            &host,
        ));
        assert!(state.scope_dim_element.is_none());
    }

    #[test]
    fn modal_close_callback_mutation_precedes_background_scope_restore()
    {
        struct CloseMutationApp
        {
            window: GeneratedWindow,
        }

        impl GeneratedApplication for CloseMutationApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }

            fn handle_generated_window_closed(&mut self, _window: GeneratedWindow)
            {
                let scope = self
                    .window
                    .find_element("items")
                    .expect("scroll scope remains in the background window");
                scope
                    .borrow_mut()
                    .child_window_mut()
                    .expect("generated scroll view")
                    .clear_children();
            }
        }

        let mut row = GeneratedWindow::new("row");
        let delete = row.add_element(new_button("delete", "Delete"));
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.add_named_child_window("[0]", row);

        let mut app = CloseMutationApp { window: GeneratedWindow::new("test") };
        let host = app.window.add_element(new_reusable_element("items", "Items"));
        host.borrow_mut().frame = Rect
        {
            row: 0,
            col: 0,
            width: 20,
            height: 5,
        };
        host.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );
        state.focused_index = 0;
        state.scope_edit_element = Some(host.clone());
        assert!(state.focus_element(&app.window, &delete));
        state.edit_mode = true;

        let mut dialog = MessageBoxYesNo::new("Delete", "Delete the row?");
        app.window.open_window(dialog.take_window());
        state.sync_active_window(&mut app.window, &options);
        dispatch_button(&mut app, &mut state, &options, "yes_btn");

        assert!(close_completed_modal(&mut app, &mut state, &options));
        assert!(state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            state.scope_edit_element.as_ref().expect("restored scroll scope"),
            &host,
        ));
        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("restored proxy focus"),
            &host,
        ));
        assert!(state.scope_dim_element.is_none());
    }

    #[test]
    fn modal_close_reactivates_a_live_invoking_scroll_scope_like_cpp()
    {
        let mut row = GeneratedWindow::new("row");
        let open = row.add_element(new_button("open", "Open"));
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.add_named_child_window("items[0]", row);

        let mut app = App
        {
            window: GeneratedWindow::new("test"),
            activated: false,
        };
        let host = app.window.add_element(new_reusable_element("items", "Items"));
        host.borrow_mut().frame = Rect
        {
            row: 1,
            col: 2,
            width: 20,
            height: 5,
        };
        host.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 10 },
        );
        state.scope_edit_element = Some(host.clone());
        assert!(state.focus_element(&app.window, &open));
        state.remember_scope_descendant(&host, &open);
        state.edit_mode = false;

        app.window.open_window(GeneratedWindow::new("dialog"));
        state.sync_active_window(&mut app.window, &options);
        app.window.active_window_mut().request_close();

        assert!(close_completed_modal(&mut app, &mut state, &options));
        assert!(state.edit_mode);
        assert!(state.scope_edit_element.as_ref().is_some_and(|scope|
        {
            Rc::ptr_eq(scope, &host)
        }));
        assert!(Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("invoking descendant restored"),
            &open,
        ));
        assert!(state.scope_dim_element.is_none());
    }

    #[test]
    fn file_browser_owns_nested_overwrite_modal_lifecycle()
    {
        struct DialogLifecycleApp
        {
            window: GeneratedWindow,
            closed_window_ids: Vec<u64>,
        }

        impl GeneratedApplication for DialogLifecycleApp
        {
            fn window(&self) -> &GeneratedWindow { &self.window }
            fn window_mut(&mut self) -> &mut GeneratedWindow { &mut self.window }

            fn handle_generated_window_closed(&mut self, window: GeneratedWindow)
            {
                self.closed_window_ids.push(window.identity());
            }
        }

        let unique = format!(
            "uimd-rust-file-browser-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .expect("system time after Unix epoch")
                .as_nanos(),
        );
        let root = std::env::temp_dir().join(unique);
        std::fs::create_dir_all(&root).expect("create FileBrowser fixture");
        let existing = root.join("existing.txt");
        std::fs::write(&existing, "existing").expect("write FileBrowser fixture");

        let mut browser = crate::FileBrowser::save(&root, Some(&root), "existing.txt");
        let browser_window = browser.take_window();
        let browser_window_id = browser_window.identity();
        let mut app = DialogLifecycleApp
        {
            window: GeneratedWindow::new("test"),
            closed_window_ids: Vec::new(),
        };
        app.window.open_window(browser_window);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 80, height: 24 },
        );
        state.sync_active_window(&mut app.window, &options);

        dispatch_button(&mut app, &mut state, &options, "open_btn");
        state.sync_active_window(&mut app.window, &options);
        assert_eq!(app.window.modal_depth(), 2);
        assert_eq!(
            app.active_window().metadata.class_name,
            "MessageBoxYesNo",
        );

        dispatch_button(&mut app, &mut state, &options, "yes_btn");
        assert!(close_completed_modal(&mut app, &mut state, &options));
        assert_eq!(app.window.modal_depth(), 0);
        assert_eq!(
            browser.result(),
            crate::FileBrowserOutcome::Selected(
                existing.canonicalize().expect("canonical fixture path"),
            ),
        );
        assert_eq!(app.closed_window_ids.len(), 2);
        assert_eq!(
            app.closed_window_ids.last().copied(),
            Some(browser_window_id),
        );

        std::fs::remove_dir_all(&root).expect("remove FileBrowser fixture");
    }

    #[test]
    fn modal_file_browser_directory_click_uses_local_frames_like_cpp()
    {
        let unique = format!(
            "uimd-rust-file-browser-mouse-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .expect("system time after Unix epoch")
                .as_nanos(),
        );
        let root = std::env::temp_dir().join(unique);
        let child = root.join("child");
        std::fs::create_dir_all(&child).expect("create FileBrowser child fixture");

        let mut browser = crate::FileBrowser::open(&root, Some(&root));
        let mut app = App
        {
            window: GeneratedWindow::new("test"),
            activated: false,
        };
        app.window.open_window(browser.take_window());
        let options = GeneratedWindowRuntimeOptions::default();
        let viewport = Size { width: 90, height: 35 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);
        let _ = render_generated_application(&mut app, &mut state, &options);

        let bounds = app
            .window
            .window_stack
            .top()
            .expect("FileBrowser modal frame")
            .bounds;
        let entries_frame = app
            .active_window()
            .find_element("entries")
            .expect("FileBrowser entries")
            .borrow()
            .frame;
        assert!(entries_frame.row < bounds.row);
        crate::mcp::perform_mouse_press(
            &mut app,
            &mut state,
            &options,
            viewport,
            bounds.col + entries_frame.col,
            bounds.row + entries_frame.row + 1,
        );

        assert_eq!(
            browser.current_dir(),
            child.canonicalize().expect("FileBrowser child canonical path"),
        );
        let entries = app
            .active_window()
            .find_element("entries")
            .expect("FileBrowser entries after navigation");
        assert_eq!(entries.borrow().selected_value(), "..");
        assert_eq!(entries.borrow().selected_indices(), &[0]);
        assert!(!entries.borrow().active_item_visible());

        std::fs::remove_dir_all(&root).expect("remove FileBrowser mouse fixture");
    }

    #[test]
    fn embedded_child_natural_height_includes_cell_padding()
    {
        let mut child = GeneratedWindow::new("child");
        child.set_generated_layout(vec![LayoutItem
        {
            element_type: "button".to_string(),
            row: 1,
            cell_chars_width: 8,
            cell_chars_height: 2,
            cell_width: 8,
            cell_height: 2,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 6,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 6,
            chars_height: 1,
            content: "save".to_string(),
            cell_style: Style
            {
                padding_top: Some(1),
                padding_right: Some(1),
                padding_bottom: Some(1),
                padding_left: Some(1),
                ..Default::default()
            },
            ..Default::default()
        }]);
        child.add_element(new_button("save", "Save"));

        assert_eq!(
            generated_window_content_size_for_width(&child, 20).height,
            4,
        );
    }

    #[test]
    fn embedded_child_renders_the_live_parent_focus_and_edit_state()
    {
        let mut child = GeneratedWindow::new("child");
        child.set_generated_layout(vec![LayoutItem
        {
            element_type: "textinput".to_string(),
            cell_chars_width: 8,
            cell_chars_height: 1,
            cell_width: 8,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 8,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 8,
            chars_height: 1,
            content: "name".to_string(),
            ..Default::default()
        }]);
        let input = child.add_element(new_text_input("name", "Coffee", 80));
        input.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#0b1220")),
            color: Some(Color::new("#e5e7eb")),
            ..Default::default()
        });
        input.borrow_mut().set_edit_style(Style
        {
            background: Some(Color::new("#0b1220")),
            color: Some(Color::new("#ffffff")),
            ..Default::default()
        });
        let options = GeneratedWindowRuntimeOptions::default();
        let mut parent_state = RuntimeState::new(
            &child,
            &options,
            Size { width: 8, height: 1 },
        );
        assert!(parent_state.focus_element(&child, &input));
        parent_state.edit_mode = true;
        let input_id = child
            .element_id(&input)
            .expect("embedded input id");

        let rendered = render_embedded_window_with_interaction(
            &mut child,
            8,
            1,
            &parent_state,
            Some(input_id),
            None,
            None,
            None,
            false,
        );

        assert_eq!(
            rendered.cell(0, 0).expect("edited text cell").foreground.normalized(),
            "#ffffff",
        );
    }

    #[test]
    fn embedded_image_clip_is_relative_to_the_child_frame()
    {
        let element = Rect { row: 3, col: 2, width: 4, height: 8 };
        let cell_clip = Rect { row: 0, col: 0, width: 20, height: 20 };

        assert_eq!(
            relative_element_render_clip(
                element,
                cell_clip,
                Some(5),
                Some(11),
                20,
            ),
            (Some(2), Some(8)),
        );
        assert_eq!(
            relative_element_render_clip(
                element,
                cell_clip,
                Some(0),
                Some(8),
                20,
            ),
            (Some(0), Some(5)),
        );
        assert_eq!(
            relative_element_render_clip(
                element,
                cell_clip,
                Some(5),
                Some(9),
                20,
            ),
            (Some(2), Some(6)),
        );
    }

    #[test]
    fn generated_scroll_view_is_the_single_canonical_element_and_child_owner()
    {
        let mut window = GeneratedWindow::new_scroll_view("items");
        let scroll_view = window
            .generated_scroll_view()
            .expect("generated ScrollView element");

        assert_eq!(window.elements().len(), 1);
        assert!(Rc::ptr_eq(&window.elements()[0], &scroll_view));
        assert!(Rc::ptr_eq(&window.scroll_view(), &scroll_view));

        window.set_generated_scroll_view_style(Style
        {
            gap: Some(2),
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        let row = new_label("row", "row");
        window.add_child(row.clone());

        assert_eq!(scroll_view.gap(), 2);
        assert_eq!(
            scroll_view.borrow().style.background.normalized(),
            "#030712",
        );
        assert_eq!(scroll_view.borrow().children.len(), 1);
        assert!(Rc::ptr_eq(
            &window
                .find_element("__scrollview[0].row")
                .expect("owned child lookup"),
            &row,
        ));
        assert_eq!(
            window.element_id(&row).as_deref(),
            Some("__scrollview[0].row"),
        );

        scroll_view.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 8, height: 3 };
        let position = window.scroll_position();
        window.restore_scroll_position(position);
        assert_eq!(window.frame(), scroll_view.borrow().frame);
        assert_eq!(window.content_height(), scroll_view.content_height());
        assert!(!window.handle_key("Unknown"));
        assert!(!window.scroll_to_top());
    }

    #[test]
    fn scroll_indicator_preserves_the_rendered_cell_background()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        let first = new_label("first", "first");
        first.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 3 };
        first.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#172033")),
            ..Default::default()
        });
        let second = new_label("second", "second");
        second.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 1 };
        let scroll_view = scroll.generated_scroll_view().expect("generated scroll view");
        scroll_view.add_child(first);
        scroll_view.add_child(second);
        let options = GeneratedWindowRuntimeOptions::default();
        let state = RuntimeState::new(
            &scroll,
            &options,
            Size { width: 5, height: 3 },
        );

        let rendered =
            render_generated_scroll_window(&mut scroll, &state, state.viewport, false);
        let indicator = rendered.cell(2, 4).expect("bottom scroll indicator");

        assert_eq!(indicator.text, "v");
        assert_eq!(indicator.background.normalized(), "#172033");
    }

    #[test]
    fn embedded_root_scroll_layout_keeps_its_natural_width()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.set_generated_layout(vec![LayoutItem
        {
            cell_chars_width: 10,
            cell_chars_height: 3,
            cell_width: EXPANDED,
            cell_height: EXPANDED,
            cell_width_mode: "expanded".to_string(),
            cell_height_mode: "expanded".to_string(),
            width: EXPANDED,
            height: EXPANDED,
            width_mode: "expanded".to_string(),
            height_mode: "expanded".to_string(),
            ..Default::default()
        }]);
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(Color::new("#030712")),
            padding: Some(1),
            ..Default::default()
        });
        let child = new_label("child", "child");
        child.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#172033")),
            ..Default::default()
        });
        scroll
            .generated_scroll_view()
            .expect("generated scroll view")
            .add_child(child);
        let options = GeneratedWindowRuntimeOptions::default();
        let state = RuntimeState::new(
            &scroll,
            &options,
            Size { width: 8, height: 3 },
        );

        let rendered =
            render_generated_scroll_window(&mut scroll, &state, state.viewport, false);

        assert_eq!(
            rendered.cell(1, 7).expect("natural-width child edge").background.normalized(),
            "#172033",
        );
    }

    #[test]
    fn scroll_scope_navigation_enters_moves_and_restores_proxy_focus()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        let first = scroll.add_element(new_button("first", "First"));
        first.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 1 };
        let second = scroll.add_element(new_button("second", "Second"));
        second.borrow_mut().frame = Rect { row: 2, col: 0, width: 6, height: 1 };

        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let panel = app.window.add_element(new_reusable_element("panel", "Items"));
        panel.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "panel".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 6 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("first descendant focus"),
            &first,
        ));
        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("second descendant focus"),
            &second,
        ));
        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(!state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("restored proxy focus"),
            &panel,
        ));
    }

    #[test]
    fn concrete_scroll_view_uses_the_same_scope_navigation_state_machine()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let scroll = app.window.add_element(new_scroll_view("items", 0));
        scroll.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 8, height: 1 };
        let first = new_button("first", "First");
        let second = new_button("second", "Second");
        first.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 8, height: 1 };
        second.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 8, height: 1 };
        scroll.borrow_mut().add_child(first.clone());
        scroll.borrow_mut().add_child(second.clone());
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "items".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 8, height: 1 },
        );

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.edit_mode);
        assert!(state.scope_edit_element.as_ref().is_some_and(|scope|
        {
            Rc::ptr_eq(scope, &scroll)
        }));
        assert!(Rc::ptr_eq(
            &state.focused_element(&app.window).expect("first child focus"),
            &first,
        ));
        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(Rc::ptr_eq(
            &state.focused_element(&app.window).expect("second child focus"),
            &second,
        ));
        assert_eq!(scroll.borrow().view_offset(), 0);
        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(Rc::ptr_eq(
            &state.focused_element(&app.window).expect("restored ScrollView focus"),
            &scroll,
        ));
    }

    #[test]
    fn spatial_focus_requires_the_candidate_edge_to_be_in_the_requested_direction()
    {
        let paid = new_checkbox("paid", "Paid", false);
        paid.borrow_mut().frame = Rect { row: 0, col: 1, width: 9, height: 1 };
        let category = new_combo_box("category", vec!["Food".to_string()]);
        category.borrow_mut().frame = Rect { row: 1, col: 1, width: 39, height: 1 };
        let delete_button = new_button("delete_btn", "Delete");
        delete_button.borrow_mut().frame = Rect { row: 1, col: 41, width: 12, height: 1 };
        let focusable: Vec<ElementRef> = vec![
            paid.into(),
            category.into(),
            delete_button.into(),
        ];

        assert_eq!(spatial_focus_target_index(&focusable, 0, "Right"), Some(2));
        assert_eq!(spatial_focus_target_index(&focusable, 0, "Down"), Some(1));
    }

    #[test]
    fn spatial_focus_reaches_a_scroll_proxy_nested_in_a_view_host()
    {
        let mut app = App { window: GeneratedWindow::new("test"), activated: false };

        let close_button = app.window.add_element(new_button("close_btn", "Quit"));
        close_button.borrow_mut().frame =
            Rect { row: 0, col: 78, width: 11, height: 1 };

        let mut mosaic_window = GeneratedWindow::new("mosaic");
        mosaic_window.set_generated_focusable(true);
        let mosaic =
            app.window.add_element(new_reusable_element("gallery_mosaic", "GalleryMosaic"));
        mosaic.borrow_mut().frame =
            Rect { row: 24, col: 1, width: 26, height: 9 };
        mosaic.borrow_mut().set_child_window(mosaic_window);

        let mut gallery_view = GeneratedWindow::new("gallery_view");
        let gallery_scroll =
            gallery_view.add_element(new_reusable_element("gallery_scroll", "GalleryScroll"));
        gallery_scroll.borrow_mut().frame =
            Rect { row: 2, col: 31, width: 56, height: 40 };
        gallery_scroll
            .borrow_mut()
            .set_child_window(GeneratedWindow::new_scroll_view("gallery_scroll"));
        let main = app.window.add_element(new_view_host("main"));
        main.borrow_mut().set_child_window(gallery_view);

        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "gallery_mosaic".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 90, height: 35 },
        );

        assert!(state.handle_key(&mut app, &options, "Right"));
        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("nested gallery ScrollView focus"),
            &gallery_scroll,
        ));
    }

    #[test]
    fn directly_focused_scroll_descendant_enters_inner_edit_before_leaving_scope()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("items");
        let category = scroll.add_element(new_combo_box("category", vec!["Food".to_string()]));
        category.borrow_mut().frame = Rect { row: 0, col: 0, width: 12, height: 1 };

        let mut app = App { window: GeneratedWindow::new("test"), activated: false };
        let panel = app.window.add_element(new_reusable_element("panel", "Items"));
        panel.borrow_mut().set_child_window(scroll);
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "panel".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 6 },
        );
        state.scope_edit_element = Some(panel.clone());
        assert!(state.focus_element(&app.window, &category));
        state.edit_mode = false;

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(state.edit_mode);
        assert!(state.scope_dim_element.as_ref().is_some_and(|element|
        {
            std::rc::Rc::ptr_eq(element, &category)
        }));

        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(state.edit_mode);
        assert!(state.scope_dim_element.is_none());
        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(!state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("restored proxy focus"),
            &panel,
        ));
    }

    #[test]
    fn each_background_window_is_dimmed_once_with_nested_modals()
    {
        let mut root = GeneratedWindow::new("root");
        root.window_style.background = Some(Color::new("#1d314c"));
        root.set_generated_layout(vec![LayoutItem
        {
            cell_width_mode: "expanded".to_string(),
            cell_height_mode: "expanded".to_string(),
            cell_width: EXPANDED,
            cell_height: EXPANDED,
            ..Default::default()
        }]);

        let mut first_modal = GeneratedWindow::new("first");
        first_modal.window_style.background = Some(Color::new("#162033"));
        first_modal.open_window(GeneratedWindow::new("second"));
        root.open_window(first_modal);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut app = App { window: root, activated: false };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 5, height: 3 },
        );
        let rendered = render_generated_application(&mut app, &mut state, &options);

        assert_eq!(
            rendered
                .cell(0, 0)
                .expect("root background cell")
                .background
                .normalized(),
            "#0e1826",
        );
    }

    #[test]
    fn active_scroll_scope_dims_only_cells_outside_its_frame()
    {
        let mut buffer = TerminalBuffer::new(3, 1);
        buffer.fill(
            Rect { row: 0, col: 0, width: 3, height: 1 },
            ' ',
            &Color::new("#ffffff"),
            &Color::new("#1f2937"),
        );
        dim_outside_active_scope(
            &mut buffer,
            Rect { row: 0, col: 1, width: 1, height: 1 },
            &Some(Color::new("#00000066")),
        );
        assert_eq!(
            buffer.cell(0, 0).map(|cell| cell.background.normalized()),
            Some("#131921".to_string()),
        );
        assert_eq!(
            buffer.cell(0, 1).map(|cell| cell.background.normalized()),
            Some("#1f2937".to_string()),
        );
        assert_eq!(
            buffer.cell(0, 2).map(|cell| cell.background.normalized()),
            Some("#131921".to_string()),
        );
    }

    #[test]
    fn nested_generated_scroll_proxy_applies_its_focus_background_once()
    {
        let mut scroll = GeneratedWindow::new_scroll_view("gallery_scroll");
        scroll.set_generated_window_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        scroll.set_generated_scroll_view_focus_style(Style
        {
            background: Some(Color::new("#ffffff14")),
            ..Default::default()
        });
        let mut gallery_item = GeneratedWindow::new("gallery_item");
        gallery_item.set_generated_layout(vec![LayoutItem
        {
            element_type: "image".to_string(),
            cell_chars_width: 3,
            cell_chars_height: 1,
            cell_width: 3,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 3,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 3,
            chars_height: 1,
            content: "photo".to_string(),
            cell_style: Style
            {
                background: Some(Color::new("#030712")),
                ..Default::default()
            },
            ..Default::default()
        }]);
        gallery_item.set_generated_window_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        gallery_item.add_element(crate::new_image(
            "photo",
            "",
            "",
            "contain",
            "fallback",
            "center",
            "middle",
        ));
        scroll.add_named_child_window("main.gallery_scroll[0]", gallery_item);

        let mut gallery = GeneratedWindow::new("gallery");
        gallery.set_generated_layout(vec![LayoutItem
        {
            element_type: "uielementreusable".to_string(),
            cell_chars_width: 3,
            cell_chars_height: 1,
            cell_width: 3,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 3,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 3,
            chars_height: 1,
            content: "gallery_scroll".to_string(),
            cell_style: Style
            {
                background: Some(Color::new("#030712")),
                ..Default::default()
            },
            ..Default::default()
        }]);
        gallery.set_generated_window_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        let gallery_scroll =
            gallery.add_element(new_reusable_element("gallery_scroll", "GalleryScroll"));
        gallery_scroll.borrow_mut().set_focus_style(Style
        {
            background: Some(Color::new("#ffffff14")),
            ..Default::default()
        });
        gallery_scroll.borrow_mut().set_child_window(scroll);

        let mut root = GeneratedWindow::new("root");
        root.set_generated_layout(vec![LayoutItem
        {
            element_type: "viewhost".to_string(),
            cell_chars_width: 3,
            cell_chars_height: 1,
            cell_width: 3,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 3,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 3,
            chars_height: 1,
            content: "main".to_string(),
            cell_style: Style
            {
                background: Some(Color::new("#030712")),
                ..Default::default()
            },
            ..Default::default()
        }]);
        root.set_generated_window_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        let main = root.add_element(new_view_host("main"));
        main.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        main.borrow_mut().set_child_window(gallery);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &root,
            &options,
            Size { width: 3, height: 1 },
        );
        assert!(state.focus_element(&root, &gallery_scroll));
        let mut app = App { window: root, activated: false };

        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("generated ScrollView proxy focus"),
            &gallery_scroll,
        ));
        let rendered = render_generated_application(&mut app, &mut state, &options);

        assert_eq!(
            rendered
                .cell(0, 0)
                .expect("focused ScrollView background")
                .background
                .normalized(),
            "#171a25",
        );

        gallery_scroll
            .borrow_mut()
            .child_window_mut()
            .expect("gallery ScrollView child")
            .set_generated_scroll_view_focus_style(Style
            {
                background: Some(Color::new("#1e3a5f")),
                ..Default::default()
            });
        let rendered = render_generated_application(&mut app, &mut state, &options);
        assert_eq!(
            rendered
                .cell(0, 0)
                .expect("distinct focused ScrollView background")
                .background
                .normalized(),
            "#1e3a5f",
        );
    }

    #[test]
    fn reusable_generated_scroll_view_focus_underlays_alpha_descendant_background()
    {
        let parent_background = Color::new("#303545");
        let focus_background = Color::new("#ffffff14");
        let descendant_background = Color::new("#252a36cc");
        let mut scroll = GeneratedWindow::new_scroll_view("alpha focus scroll");
        scroll.set_generated_focusable(true);
        scroll.set_generated_window_style(Style
        {
            background: Some(parent_background.clone()),
            ..Default::default()
        });
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(parent_background.clone()),
            ..Default::default()
        });
        scroll.set_generated_scroll_view_focus_style(Style
        {
            background: Some(focus_background.clone()),
            ..Default::default()
        });
        let row = new_label("row", " ");
        row.borrow_mut().set_frame(Rect { row: 0, col: 0, width: 1, height: 1 });
        row.borrow_mut().set_style(Style
        {
            background: Some(descendant_background.clone()),
            ..Default::default()
        });
        scroll.add_child(row);

        let host: ElementRef = new_reusable_element("card", "AlphaFocusScroll").into();
        host.borrow_mut().set_child_window(scroll);
        host.borrow_mut().set_style(Style
        {
            background: Some(parent_background.clone()),
            ..Default::default()
        });
        host.borrow_mut().set_focus_style(Style
        {
            background: Some(focus_background.clone()),
            ..Default::default()
        });
        let mut window = GeneratedWindow::new("root");
        window.set_generated_window_style(Style
        {
            background: Some(parent_background.clone()),
            ..Default::default()
        });
        window.set_generated_layout(vec![LayoutItem
        {
            element_type: "uielementreusable".to_string(),
            cell_chars_width: 1,
            cell_chars_height: 2,
            cell_width: 1,
            cell_height: 2,
            cell_width_mode: "fixed".to_string(),
            cell_height_mode: "fixed".to_string(),
            width: 1,
            height: 2,
            width_mode: "fixed".to_string(),
            height_mode: "fixed".to_string(),
            chars_width: 1,
            chars_height: 2,
            content: "card".to_string(),
            cell_style: Style
            {
                background: Some(parent_background.clone()),
                ..Default::default()
            },
            ..Default::default()
        }]);
        window.add_element(host.clone());
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &window,
            &options,
            Size { width: 1, height: 2 },
        );
        let mut app = App
        {
            window,
            activated: false,
        };

        let unfocused = render_generated_application(&mut app, &mut state, &options);
        assert!(state.focus_element(&app.window, &host));
        let focused = render_generated_application(&mut app, &mut state, &options);
        let expected_unfocused = descendant_background.blend_over(&parent_background);
        let expected_focused =
            descendant_background.blend_over(&focus_background.blend_over(&parent_background));
        assert_eq!(
            unfocused
                .cell(0, 0)
                .expect("unfocused alpha descendant cell")
                .background
                .normalized(),
            expected_unfocused.normalized(),
        );
        assert_eq!(
            focused
                .cell(0, 0)
                .expect("focused alpha descendant cell")
                .background
                .normalized(),
            expected_focused.normalized(),
        );
        assert_eq!(
            focused
                .cell(1, 0)
                .expect("single focused structural cell")
                .background
                .normalized(),
            focus_background.blend_over(&parent_background).normalized(),
        );
    }

    #[test]
    fn transparent_scroll_focus_colors_gaps_without_coloring_content()
    {
        let mut buffer = TerminalBuffer::new(6, 2);
        buffer.fill(
            Rect { row: 0, col: 0, width: 6, height: 2 },
            ' ',
            &Color::new("transparent"),
            &Some(Color::new("#00000000")),
        );
        buffer.draw_text(
            1,
            1,
            "code",
            &Color::new("#ffffff"),
            &Color::new("#000000"),
            4,
        );

        apply_scroll_focus_background_gaps(
            &mut buffer,
            &Color::new("#ffffff14"),
            &Some(Color::new("#00000000")),
            [1, 1, 0, 1],
        );

        assert_eq!(
            buffer.cell(0, 2).map(|cell| cell.background.normalized()),
            Some("#141414".to_string()),
        );
        assert_eq!(
            buffer.cell(1, 2).map(|cell| cell.background.normalized()),
            Some("#000000".to_string()),
        );
    }

    #[test]
    fn focused_control_inside_scroll_row_keeps_its_control_focus_style()
    {
        let mut row_window = GeneratedWindow::new("row");
        row_window.set_generated_layout(vec![LayoutItem
        {
            element_type: "combobox".to_string(),
            cell_chars_width: 8,
            cell_chars_height: 1,
            cell_width: 8,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 8,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 8,
            chars_height: 1,
            content: "category".to_string(),
            ..Default::default()
        }]);
        let category = row_window.add_element(crate::new_combo_box(
            "category",
            vec!["Food".to_string(), "Home".to_string()],
        ));
        category.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#0b1220")),
            color: Some(Color::new("#e5e7eb")),
            ..Default::default()
        });
        category.borrow_mut().set_focus_style(Style
        {
            background: Some(Color::new("#25364f")),
            color: Some(Color::new("#ffffff")),
            ..Default::default()
        });
        let row = new_reusable_element("row", "Row");
        row.borrow_mut().set_child_window(row_window);

        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(Color::new("#172033")),
            ..Default::default()
        });
        scroll
            .generated_scroll_view()
            .expect("generated scroll view")
            .add_child(row);

        let mut parent = GeneratedWindow::new("parent");
        parent.set_generated_layout(vec![LayoutItem
        {
            element_type: "uielementreusable".to_string(),
            cell_chars_width: 10,
            cell_chars_height: 3,
            cell_width: 10,
            cell_height: 3,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 10,
            height: 3,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 10,
            chars_height: 3,
            content: "items".to_string(),
            ..Default::default()
        }]);
        let items = parent.add_element(new_reusable_element("items", "Items"));
        items.borrow_mut().set_child_window(scroll);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &parent,
            &options,
            Size { width: 10, height: 3 },
        );
        state.scope_edit_element = Some(items);
        assert!(state.focus_element(&parent, &category));
        state.edit_mode = true;
        let mut app = App { window: parent, activated: false };

        let rendered = render_generated_application(&mut app, &mut state, &options);
        let category_frame = state
            .focused_element(&app.window)
            .expect("focused category")
            .borrow()
            .frame;
        let cell = rendered
            .cell(category_frame.row, category_frame.col)
            .expect("focused category cell");

        assert_eq!(cell.text, "F");
        assert_eq!(cell.background.normalized(), "#25364f");
        assert_eq!(cell.foreground.normalized(), "#ffffff");
    }

    #[test]
    fn nested_scroll_combo_box_overlay_has_no_ancestor_local_ghost()
    {
        let mut row_window = GeneratedWindow::new("row");
        row_window.set_generated_layout(vec![LayoutItem
        {
            element_type: "combobox".to_string(),
            row: 1,
            col: 1,
            cell_chars_width: 8,
            cell_chars_height: 1,
            cell_width: 8,
            cell_height: 1,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 8,
            height: 1,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 8,
            chars_height: 1,
            content: "category".to_string(),
            ..Default::default()
        }]);
        let category = row_window.add_element(crate::new_combo_box(
            "category",
            vec!["Food".to_string(), "Home".to_string()],
        ));
        let row = new_reusable_element("row", "Row");
        row.borrow_mut().set_child_window(row_window);

        let scroll = GeneratedWindow::new_scroll_view("items");
        scroll
            .generated_scroll_view()
            .expect("generated scroll view")
            .add_child(row);

        let mut parent = GeneratedWindow::new("parent");
        parent.set_generated_layout(vec![LayoutItem
        {
            element_type: "uielementreusable".to_string(),
            row: 1,
            col: 2,
            cell_chars_width: 10,
            cell_chars_height: 3,
            cell_width: 10,
            cell_height: 3,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 10,
            height: 3,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 10,
            chars_height: 3,
            content: "items".to_string(),
            ..Default::default()
        }]);
        let items = parent.add_element(new_reusable_element("items", "Items"));
        items.borrow_mut().set_child_window(scroll);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &parent,
            &options,
            Size { width: 14, height: 5 },
        );
        state.scope_edit_element = Some(items);
        assert!(state.focus_element(&parent, &category));
        state.edit_mode = true;
        state.scope_dim_element = Some(category);
        let mut app = App { window: parent, activated: false };

        let rendered = render_generated_application(&mut app, &mut state, &options);

        assert_eq!(
            rendered.cell(0, 1).expect("ancestor-local cell").text,
            " ",
            "the generated ScrollView owns the descendant ComboBox overlay",
        );
        assert_eq!(
            rendered.cells().iter().filter(|cell| cell.text == "F").count(),
            2,
            "the closed value and selected option must each render exactly once",
        );
    }

    #[test]
    fn focused_button_in_multi_control_scroll_row_does_not_focus_neighbor_checkbox()
    {
        let make_row = |name: &str|
        {
            let mut row_window = GeneratedWindow::new(name);
            row_window.set_generated_layout(vec![
                LayoutItem
                {
                    element_type: "checkbox".to_string(),
                    cell_chars_width: 10,
                    cell_chars_height: 1,
                    cell_width: 10,
                    cell_height: 1,
                    cell_width_mode: "auto".to_string(),
                    cell_height_mode: "auto".to_string(),
                    width: 10,
                    height: 1,
                    width_mode: "auto".to_string(),
                    height_mode: "auto".to_string(),
                    chars_width: 10,
                    chars_height: 1,
                    content: "done".to_string(),
                    cell_style: Style
                    {
                        background: Some(Color::new("#172033")),
                        ..Default::default()
                    },
                    ..Default::default()
                },
                LayoutItem
                {
                    element_type: "button".to_string(),
                    row: 0,
                    col: 10,
                    cell_chars_width: 10,
                    cell_chars_height: 1,
                    cell_width: 10,
                    cell_height: 1,
                    cell_width_mode: "auto".to_string(),
                    cell_height_mode: "auto".to_string(),
                    width: 10,
                    height: 1,
                    width_mode: "auto".to_string(),
                    height_mode: "auto".to_string(),
                    chars_width: 10,
                    chars_height: 1,
                    content: "open_btn".to_string(),
                    cell_style: Style
                    {
                        background: Some(Color::new("#172033")),
                        ..Default::default()
                    },
                    ..Default::default()
                },
            ]);
            row_window.set_generated_window_style(Style
            {
                background: Some(Color::new("#162033")),
                ..Default::default()
            });
            let done = row_window.add_element(crate::new_checkbox("done", "Done", false));
            done.borrow_mut().set_style(Style
            {
                background: Some(Color::new("transparent")),
                color: Some(Color::new("#cbd5e1")),
                ..Default::default()
            });
            done.borrow_mut().set_focus_style(Style
            {
                background: Some(Color::new("#ffffff44")),
                color: Some(Color::new("#facc15")),
                ..Default::default()
            });
            done.borrow_mut().set_unchecked_style(Style
            {
                background: Some(Color::new("transparent")),
                color: Some(Color::new("#94a3b8")),
                ..Default::default()
            });
            let open = row_window.add_element(new_button("open_btn", "Open"));
            open.borrow_mut().set_style(Style
            {
                background: Some(Color::new("#334155")),
                color: Some(Color::new("#e2e8f0")),
                ..Default::default()
            });
            open.borrow_mut().set_focus_style(Style
            {
                background: Some(Color::new("#2563eb")),
                color: Some(Color::new("#ffffff")),
                ..Default::default()
            });
            (row_window, done, open)
        };

        let mut scroll = GeneratedWindow::new_scroll_view("items");
        scroll.set_generated_scroll_view_style(Style
        {
            background: Some(Color::new("#00000000")),
            gap: Some(1),
            ..Default::default()
        });
        scroll.set_generated_scroll_view_focus_style(Style
        {
            background: Some(Color::new("#ffffff14")),
            ..Default::default()
        });

        let (first_row, first_done, _) = make_row("first");
        scroll.add_named_child_window("board[0]", first_row);
        let (second_row, _, second_open) = make_row("second");
        scroll.add_named_child_window("board[1]", second_row);
        let (third_row, _, third_open) = make_row("third");
        scroll.add_named_child_window("board[2]", third_row);

        let mut parent = GeneratedWindow::new("parent");
        parent.set_generated_layout(vec![LayoutItem
        {
            element_type: "uielementreusable".to_string(),
            cell_chars_width: 20,
            cell_chars_height: 5,
            cell_width: 20,
            cell_height: 5,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 20,
            height: 5,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 20,
            chars_height: 5,
            content: "items".to_string(),
            cell_style: Style
            {
                background: Some(Color::new("#030712")),
                ..Default::default()
            },
            ..Default::default()
        }]);
        let items = parent.add_element(new_reusable_element("items", "Items"));
        items.borrow_mut().set_style(Style
        {
            background: Some(Color::new("#030712")),
            ..Default::default()
        });
        items.borrow_mut().set_focus_style(Style
        {
            background: Some(Color::new("#07111f")),
            ..Default::default()
        });
        items.borrow_mut().set_child_window(scroll);

        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &parent,
            &options,
            Size { width: 20, height: 5 },
        );
        state.scope_edit_element = Some(items.clone());
        assert!(state.focus_element(&parent, &second_open));
        state.edit_mode = true;
        let mut app = App { window: parent, activated: false };

        assert!(state.handle_key(&mut app, &options, "Escape"));
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("restored scope proxy"),
            &items,
        ));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("remembered descendant"),
            &second_open,
        ));

        let _ = render_generated_application(&mut app, &mut state, &options);
        assert!(state.handle_key(&mut app, &options, "Down"));
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("next offscreen descendant"),
            &third_open,
        ));
        assert!(state.handle_key(&mut app, &options, "Up"));
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("previous descendant"),
            &second_open,
        ));
        let rendered = render_generated_application(&mut app, &mut state, &options);
        let done_frame = first_done.borrow().frame;

        assert_eq!(
            rendered
                .cell(done_frame.row, done_frame.col)
                .expect("neighbor checkbox cell")
                .background
                .normalized(),
            "#172033",
        );

        state.edit_mode = false;
        let navigation_rendered =
            render_generated_application(&mut app, &mut state, &options);
        assert!(
            navigation_rendered.cells().iter().any(|cell|
                cell.background.normalized() == "#07111f"),
            "navigation scope gaps must use the reusable host focus background",
        );
    }

    #[test]
    fn span_label_fit_content_height_preserves_explicit_lines()
    {
        let label = crate::new_span_label("code", "one\ntwo\nthree");
        let item = LayoutItem {
            element_type: "spanlabel".to_string(),
            chars_height: 1,
            cell_chars_height: 1,
            ..Default::default()
        };
        assert_eq!(fit_content_height_for(Some(&label), &item, Some(1)), 3);
    }

    #[test]
    fn generated_content_size_uses_the_cpp_fit_content_second_pass()
    {
        let mut window = GeneratedWindow::new("fit");
        window.window_style.border_width_horizontal = Some(1);
        window.window_style.border_width_vertical = Some(1);
        window.add_element(crate::new_label("message", "abcdefgh"));
        window.add_element(crate::new_label("tail", "tail"));
        window.set_generated_layout(vec![
            LayoutItem
            {
                element_type: "label".to_string(),
                cell_chars_width: 4,
                cell_chars_height: 1,
                cell_width: 4,
                cell_height: FIT_CONTENT,
                cell_width_mode: "fixed".to_string(),
                cell_height_mode: "fit-content".to_string(),
                width: 4,
                height: FIT_CONTENT,
                width_mode: "fixed".to_string(),
                height_mode: "fit-content".to_string(),
                chars_width: 4,
                chars_height: 1,
                content: "message".to_string(),
                ..Default::default()
            },
            LayoutItem
            {
                element_type: "label".to_string(),
                cell_row: 2,
                cell_chars_width: 4,
                cell_chars_height: 1,
                cell_width: 4,
                cell_height: 1,
                cell_width_mode: "fixed".to_string(),
                cell_height_mode: "fixed".to_string(),
                width: 4,
                height: 1,
                width_mode: "fixed".to_string(),
                height_mode: "fixed".to_string(),
                chars_width: 4,
                chars_height: 1,
                content: "tail".to_string(),
                ..Default::default()
            },
        ]);

        assert_eq!(natural_content_size(&window), Size { width: 4, height: 3 });
        assert_eq!(
            generated_window_content_size(&window),
            Size { width: 4, height: 4 },
        );
        assert_eq!(
            generated_window_content_size_for_width(&window, 4),
            Size { width: 4, height: 4 },
        );
        assert_eq!(
            generated_window_resolved_content_size(
                &window,
                Size { width: 4, height: 3 },
            ),
            Size { width: 4, height: 4 },
        );
        assert_eq!(
            generated_window_reported_size(&window),
            Size { width: 6, height: 6 },
        );

        let mut background = TerminalBuffer::new(1, 1);
        background.draw_text(
            0,
            0,
            "x",
            &Color::new("#ffffff"),
            &Color::new("#808080"),
            1,
        );
        dim_generated_window_modal_background(&mut background);
        assert_eq!(
            background.cell(0, 0).expect("dimmed cell").foreground.normalized(),
            "#7f7f7f",
        );
    }
}

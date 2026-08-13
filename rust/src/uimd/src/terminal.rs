use crate::{
    ElementKind, GeneratedApplication, GeneratedWindowRuntimeOptions, Rect, RuntimeState, Size,
};
use std::cmp::{max, min};
use std::io::{self, Write};
#[cfg(not(unix))]
use std::io::Read;
use std::mem::MaybeUninit;
use std::os::fd::AsRawFd;
use std::process::{Command, Stdio};
use std::sync::{Mutex, OnceLock};
use std::time::{Duration, Instant};

const ANSI_ENTER: &str =
    "\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[>4;2m\x1b[?25l\x1b[2J";
const ANSI_LEAVE: &str =
    "\x1b[0m\x1b[2J\x1b[H\x1b[>4;0m\x1b[?25h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l\r\n";
const ANSI_EMBEDDED_ENTER: &str = "\x1b[?25l";
const ANSI_EMBEDDED_LEAVE: &str = "\x1b[0m\x1b[?25h";
const ANSI_CLEAR_SCREEN: &str = "\x1b[2J";
const BRACKETED_PASTE_START: &[u8] = b"\x1b[200~";
const BRACKETED_PASTE_END: &[u8] = b"\x1b[201~";
const ESCAPE_CONTINUATION_TIMEOUT: Duration = Duration::from_millis(50);
const TERMINAL_PIXEL_QUERY_TIMEOUT: Duration = Duration::from_millis(50);
const TERMINAL_CELL_PIXEL_QUERY: &str = "\x1b[16t";
const TERMINAL_CELL_PIXEL_RESPONSE_PREFIX: &str = "\x1b[6;";
const TERMINAL_TEXT_AREA_PIXEL_QUERY: &str = "\x1b[14t";
const TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX: &str = "\x1b[4;";
const TERMINAL_PIXEL_RESPONSE_MAX_BYTES: usize = 64;
const INPUT_POLL_INTERVAL: Duration = Duration::from_millis(10);
const ANIMATED_RENDER_INTERVAL: Duration = Duration::from_millis(70);
const MAX_COALESCED_MOUSE_WHEEL_DELTA: i32 = 12;
const INPUT_BUFFER_BYTES: usize = 256;
const INPUT_DRAIN_MAX_BYTES: usize = 16384;
const MOUSE_COORDINATE_BASE: i32 = 1;
const SIXEL_FALLBACK_WARNING: &str =
    "Sixel is not supported. Continue with fallback image blocks?";
const MOUSE_DRAG_FLAG: i32 = 32;

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub enum EventType
{
    #[default]
    Key,
    MousePress,
    MouseDrag,
    MouseRelease,
    MouseWheel,
    Paste,
    Resize,
    Focus,
    Blur,
    Open,
    Close,
    Save,
    ElementChanged,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct Event
{
    pub event_type: EventType,
    pub key: String,
    pub text: String,
    pub position: crate::Point,
    pub wheel_delta: i32,
    pub size: Size,
}

impl Event
{
    fn key(key: impl Into<String>) -> Self
    {
        Self { event_type: EventType::Key, key: key.into(), ..Default::default() }
    }

    fn paste(text: impl Into<String>) -> Self
    {
        Self { event_type: EventType::Paste, text: text.into(), ..Default::default() }
    }

    fn pointer(event_type: EventType, x: i32, y: i32) -> Self
    {
        Self
        {
            event_type,
            position: crate::Point { row: y, col: x },
            ..Default::default()
        }
    }

    fn wheel(x: i32, y: i32, delta: i32) -> Self
    {
        Self
        {
            event_type: EventType::MouseWheel,
            position: crate::Point { row: y, col: x },
            wheel_delta: delta,
            ..Default::default()
        }
    }
}

#[derive(Default)]
pub struct InputParser
{
    buffer: Vec<u8>,
}

impl InputParser
{
    pub fn feed(&mut self, bytes: &[u8]) -> Vec<Event>
    {
        self.buffer.extend_from_slice(bytes);
        let mut events = Vec::new();
        while self.parse_next(&mut events) {}
        events
    }

    fn flush_pending_escape(&mut self) -> Vec<Event>
    {
        if self.buffer.first() != Some(&0x1b)
        {
            return Vec::new();
        }
        self.buffer.remove(0);
        let mut events = vec![Event::key("Escape")];
        while self.parse_next(&mut events) {}
        events
    }

    fn has_pending_escape(&self) -> bool
    {
        self.buffer.first() == Some(&0x1b)
    }

    fn parse_next(&mut self, events: &mut Vec<Event>) -> bool
    {
        if self.buffer.is_empty()
        {
            return false;
        }
        if self.buffer.starts_with(BRACKETED_PASTE_START)
        {
            let Some(end) = find_bytes(
                &self.buffer[BRACKETED_PASTE_START.len()..],
                BRACKETED_PASTE_END,
            ) else
            {
                return false;
            };
            let payload_start = BRACKETED_PASTE_START.len();
            let payload_end = payload_start + end;
            let text = String::from_utf8_lossy(&self.buffer[payload_start..payload_end])
                .into_owned();
            self.buffer
                .drain(..payload_end + BRACKETED_PASTE_END.len());
            events.push(Event::paste(text));
            return true;
        }
        if self.buffer[0] == 0x1b
        {
            return self.parse_escape(events);
        }

        let first = self.buffer[0];
        let event = match first
        {
            3 => Event::key("Ctrl+C"),
            9 => Event::key("Tab"),
            10 | 13 => Event::key("Enter"),
            127 => Event::key("Backspace"),
            value if value < 0x80 =>
                Event::key(char::from(value).to_string()),
            _ =>
            {
                let expected = utf8_sequence_length(first);
                if self.buffer.len() < expected
                {
                    return false;
                }
                let text = String::from_utf8_lossy(&self.buffer[..expected]).into_owned();
                self.buffer.drain(..expected);
                events.push(Event::key(text));
                return true;
            }
        };
        self.buffer.remove(0);
        if !(event.event_type == EventType::Key && event.key.is_empty())
        {
            events.push(event);
        }
        true
    }

    fn parse_escape(&mut self, events: &mut Vec<Event>) -> bool
    {
        if self.buffer.len() == 1
        {
            return false;
        }
        if matches!(self.buffer.get(1), Some(b'\r' | b'\n'))
        {
            self.buffer.drain(..2);
            events.push(Event::key("Alt+Enter"));
            return true;
        }
        if self.buffer.starts_with(b"\x1bO")
        {
            if self.buffer.len() < 3
            {
                return false;
            }
            let key = simple_cursor_key(self.buffer[2]);
            if let Some(key) = key
            {
                self.buffer.drain(..3);
                events.push(Event::key(key));
                return true;
            }
            self.buffer.remove(0);
            events.push(Event::key("Escape"));
            return true;
        }
        if !self.buffer.starts_with(b"\x1b[")
        {
            self.buffer.remove(0);
            events.push(Event::key("Escape"));
            return true;
        }
        if self.buffer.starts_with(b"\x1b[<")
        {
            return self.parse_sgr_mouse(events);
        }
        let Some(final_index) = self
            .buffer
            .iter()
            .enumerate()
            .skip(2)
            .find_map(|(index, byte)| is_csi_final(*byte).then_some(index))
        else
        {
            return false;
        };
        let sequence = self.buffer[..=final_index].to_vec();
        if sequence.starts_with(b"\x1b[6;") || sequence.starts_with(b"\x1b[4;")
        {
            self.buffer.drain(..=final_index);
            return true;
        }
        let key = parse_csi_key(&sequence);
        if let Some(key) = key
        {
            self.buffer.drain(..=final_index);
            events.push(Event::key(key));
            return true;
        }
        self.buffer.remove(0);
        events.push(Event::key("Escape"));
        true
    }

    fn parse_sgr_mouse(&mut self, events: &mut Vec<Event>) -> bool
    {
        let Some(final_index) = self
            .buffer
            .iter()
            .enumerate()
            .skip(3)
            .find_map(|(index, byte)| matches!(*byte, b'M' | b'm').then_some(index))
        else
        {
            return false;
        };
        let suffix = self.buffer[final_index];
        let payload = String::from_utf8_lossy(&self.buffer[3..final_index]).into_owned();
        let fields: Vec<&str> = payload.split(';').collect();
        self.buffer.drain(..=final_index);
        if fields.len() != 3
        {
            return true;
        }
        let Some(values) = fields
            .iter()
            .map(|value| value.parse::<i32>().ok())
            .collect::<Option<Vec<_>>>()
        else
        {
            return true;
        };
        let button = values[0];
        let x = values[1] - MOUSE_COORDINATE_BASE;
        let y = values[2] - MOUSE_COORDINATE_BASE;
        if matches!(button, 64 | 65)
        {
            events.push(Event::wheel(
                x,
                y,
                if button == 64 { 1 } else { -1 },
            ));
        }
        else if suffix == b'm'
        {
            events.push(Event::pointer(EventType::MouseRelease, x, y));
        }
        else if button & MOUSE_DRAG_FLAG != 0
        {
            events.push(Event::pointer(EventType::MouseDrag, x, y));
        }
        else
        {
            events.push(Event::pointer(EventType::MousePress, x, y));
        }
        true
    }
}

fn coalesce_mouse_wheel_events(events: Vec<Event>) -> Vec<Event>
{
    let mut coalesced = Vec::with_capacity(events.len());
    let mut index = 0;
    while index < events.len()
    {
        let event = &events[index];
        if event.event_type != EventType::MouseWheel
        {
            coalesced.push(event.clone());
            index += 1;
            continue;
        }

        let mut merged = event.clone();
        let mut delta = 0;
        while index < events.len() && events[index].event_type == EventType::MouseWheel
        {
            merged.position = events[index].position;
            delta += events[index].wheel_delta;
            index += 1;
        }
        merged.wheel_delta = delta.clamp(
            -MAX_COALESCED_MOUSE_WHEEL_DELTA,
            MAX_COALESCED_MOUSE_WHEEL_DELTA,
        );
        if merged.wheel_delta != 0
        {
            coalesced.push(merged);
        }
    }
    coalesced
}

fn find_bytes(haystack: &[u8], needle: &[u8]) -> Option<usize>
{
    haystack
        .windows(needle.len())
        .position(|candidate| candidate == needle)
}

fn utf8_sequence_length(first: u8) -> usize
{
    if first & 0xe0 == 0xc0
    {
        2
    }
    else if first & 0xf0 == 0xe0
    {
        3
    }
    else if first & 0xf8 == 0xf0
    {
        4
    }
    else
    {
        1
    }
}

fn is_csi_final(byte: u8) -> bool
{
    (b'@'..=b'~').contains(&byte)
}

fn simple_cursor_key(final_byte: u8) -> Option<&'static str>
{
    match final_byte
    {
        b'A' => Some("Up"),
        b'B' => Some("Down"),
        b'C' => Some("Right"),
        b'D' => Some("Left"),
        b'H' => Some("Home"),
        b'F' => Some("End"),
        b'Z' => Some("Shift+Tab"),
        _ => None,
    }
}

fn modified_cursor_key(final_byte: u8, modifier: i32) -> Option<String>
{
    let key = simple_cursor_key(final_byte)?;
    Some(match modifier
    {
        2 => format!("Shift+{key}"),
        3 => format!("Alt+{key}"),
        4 => format!("Alt+Shift+{key}"),
        5 => format!("Ctrl+{key}"),
        6 => format!("Ctrl+Shift+{key}"),
        _ => key.to_string(),
    })
}

fn parse_csi_key(sequence: &[u8]) -> Option<String>
{
    let final_byte = *sequence.last()?;
    let payload = std::str::from_utf8(sequence.get(2..sequence.len() - 1)?).ok()?;
    if final_byte == b'u'
    {
        let (codepoint, modifier) = payload.split_once(';')?;
        let codepoint: i32 = codepoint.parse().ok()?;
        let modifier: i32 = modifier.parse().ok()?;
        if modifier == 1
        {
            return None;
        }
        if codepoint == 13
        {
            return Some("Alt+Enter".to_string());
        }
        return modified_codepoint_key(codepoint, modifier);
    }
    if final_byte == b'~' && payload.starts_with("27;")
    {
        if matches!(payload, "27;3;13" | "27;5;13")
        {
            return Some("Alt+Enter".to_string());
        }
        let mut fields = payload.split(';');
        if fields.next()? != "27"
        {
            return None;
        }
        let modifier: i32 = fields.next()?.parse().ok()?;
        let codepoint: i32 = fields.next()?.parse().ok()?;
        if fields.next().is_none()
            && (codepoint == 3
                || (matches!(codepoint, 67 | 99) && matches!(modifier, 5 | 6)))
        {
            return Some("Ctrl+C".to_string());
        }
        return None;
    }
    if payload.is_empty()
    {
        return simple_cursor_key(final_byte).map(str::to_string);
    }
    let modifier = payload
        .rsplit_once(';')
        .and_then(|(_, modifier)| modifier.parse().ok())?;
    modified_cursor_key(final_byte, modifier)
}

fn modified_codepoint_key(codepoint: i32, modifier: i32) -> Option<String>
{
    if codepoint == 3
        || (matches!(codepoint, 67 | 99) && matches!(modifier, 5 | 6))
    {
        return Some("Ctrl+C".to_string());
    }
    if matches!(codepoint, 67 | 99) && matches!(modifier, 9 | 10)
    {
        return Some("cmd_c".to_string());
    }
    if matches!(codepoint, 86 | 118) && matches!(modifier, 9 | 10)
    {
        return Some("cmd_v".to_string());
    }
    None
}

#[cfg(unix)]
static mut TERMINAL_SIGNAL_STATE_ACTIVE: libc::c_int = 0;
#[cfg(unix)]
static mut TERMINAL_SIGNAL_ORIGINAL: MaybeUninit<libc::termios> = MaybeUninit::uninit();
#[cfg(unix)]
static mut TERMINAL_SIGNAL_HAS_ORIGINAL: libc::c_int = 0;
#[cfg(unix)]
static mut TERMINAL_SIGNAL_INPUT_FD: libc::c_int = libc::STDIN_FILENO;
#[cfg(unix)]
static mut TERMINAL_SIGNAL_OUTPUT_FD: libc::c_int = libc::STDOUT_FILENO;
#[cfg(unix)]
static mut TERMINAL_SIGNAL_LEAVE: *const u8 = ANSI_LEAVE.as_ptr();
#[cfg(unix)]
static mut TERMINAL_SIGNAL_LEAVE_LEN: usize = ANSI_LEAVE.len();

#[cfg(unix)]
fn force_normal_output_flags(fd: i32)
{
    let mut current = MaybeUninit::<libc::termios>::uninit();
    if unsafe { libc::tcgetattr(fd, current.as_mut_ptr()) } != 0
    {
        return;
    }
    let mut current = unsafe { current.assume_init() };
    current.c_oflag |= libc::OPOST | libc::ONLCR;
    unsafe
    {
        libc::tcsetattr(fd, libc::TCSANOW, &current);
    }
}

#[cfg(unix)]
extern "C" fn restore_terminal_for_signal(signal: libc::c_int)
{
    if unsafe { TERMINAL_SIGNAL_STATE_ACTIVE != 0 }
    {
        unsafe
        {
            TERMINAL_SIGNAL_STATE_ACTIVE = 0;
            libc::write(
                TERMINAL_SIGNAL_OUTPUT_FD,
                TERMINAL_SIGNAL_LEAVE.cast(),
                TERMINAL_SIGNAL_LEAVE_LEN,
            );
            if TERMINAL_SIGNAL_HAS_ORIGINAL != 0
            {
                let mut restored = *std::ptr::addr_of!(TERMINAL_SIGNAL_ORIGINAL)
                    .cast::<libc::termios>();
                restored.c_oflag |= libc::OPOST | libc::ONLCR;
                libc::tcsetattr(
                    TERMINAL_SIGNAL_INPUT_FD,
                    libc::TCSANOW,
                    &restored,
                );
            }
        }
    }
    unsafe
    {
        let mut default_action: libc::sigaction = std::mem::zeroed();
        default_action.sa_sigaction = libc::SIG_DFL;
        libc::sigemptyset(&mut default_action.sa_mask);
        default_action.sa_flags = 0;
        libc::sigaction(signal, &default_action, std::ptr::null_mut());
        libc::raise(signal);
    }
}

#[cfg(unix)]
pub struct TerminalModeGuard
{
    input_fd: i32,
    output_fd: i32,
    original: libc::termios,
    previous_sigterm: libc::sigaction,
    previous_sighup: libc::sigaction,
    previous_sigint: libc::sigaction,
    leave_sequence: &'static str,
    mode_enabled: bool,
    restored: bool,
}

#[cfg(unix)]
impl TerminalModeGuard
{
    pub fn new(input_fd: i32, output_fd: i32) -> io::Result<Self>
    {
        Self::new_with_leave_sequence(input_fd, output_fd, ANSI_LEAVE)
    }

    fn new_with_leave_sequence(
        input_fd: i32,
        output_fd: i32,
        leave_sequence: &'static str,
    ) -> io::Result<Self>
    {
        force_normal_output_flags(input_fd);
        if output_fd != input_fd
        {
            force_normal_output_flags(output_fd);
        }
        let mut original = MaybeUninit::<libc::termios>::zeroed();
        let has_original = unsafe { libc::tcgetattr(input_fd, original.as_mut_ptr()) } == 0;
        let original = unsafe { original.assume_init() };
        let mode_enabled = if has_original
        {
            let mut raw = original;
            raw.c_lflag &= !(libc::ECHO | libc::ICANON | libc::IEXTEN | libc::ISIG);
            raw.c_iflag &=
                !(libc::BRKINT | libc::ICRNL | libc::INPCK | libc::ISTRIP | libc::IXON);
            raw.c_cflag |= libc::CS8;
            raw.c_oflag &= !libc::OPOST;
            raw.c_cc[libc::VMIN] = 0;
            raw.c_cc[libc::VTIME] = 1;
            (unsafe { libc::tcsetattr(input_fd, libc::TCSAFLUSH, &raw) }) == 0
        }
        else
        {
            false
        };
        unsafe
        {
            if has_original
            {
                std::ptr::write(
                    std::ptr::addr_of_mut!(TERMINAL_SIGNAL_ORIGINAL).cast::<libc::termios>(),
                    original,
                );
            }
            TERMINAL_SIGNAL_HAS_ORIGINAL = libc::c_int::from(has_original);
            TERMINAL_SIGNAL_INPUT_FD = input_fd;
            TERMINAL_SIGNAL_OUTPUT_FD = output_fd;
            TERMINAL_SIGNAL_LEAVE = leave_sequence.as_ptr();
            TERMINAL_SIGNAL_LEAVE_LEN = leave_sequence.len();
            TERMINAL_SIGNAL_STATE_ACTIVE = 1;
        }
        let mut action: libc::sigaction = unsafe { std::mem::zeroed() };
        action.sa_sigaction =
            restore_terminal_for_signal as *const () as libc::sighandler_t;
        unsafe
        {
            libc::sigemptyset(&mut action.sa_mask);
        }
        action.sa_flags = 0;
        let mut previous_sigterm: libc::sigaction = unsafe { std::mem::zeroed() };
        let mut previous_sighup: libc::sigaction = unsafe { std::mem::zeroed() };
        let mut previous_sigint: libc::sigaction = unsafe { std::mem::zeroed() };
        for (signal, previous) in [
            (libc::SIGTERM, &mut previous_sigterm),
            (libc::SIGHUP, &mut previous_sighup),
            (libc::SIGINT, &mut previous_sigint),
        ]
        {
            if unsafe { libc::sigaction(signal, &action, previous) } != 0
            {
                unsafe
                {
                    TERMINAL_SIGNAL_STATE_ACTIVE = 0;
                    TERMINAL_SIGNAL_HAS_ORIGINAL = 0;
                }
                if mode_enabled
                {
                    unsafe
                    {
                        libc::tcsetattr(input_fd, libc::TCSAFLUSH, &original);
                    }
                }
                return Err(io::Error::last_os_error());
            }
        }
        Ok(Self
        {
            input_fd,
            output_fd,
            original,
            previous_sigterm,
            previous_sighup,
            previous_sigint,
            leave_sequence,
            mode_enabled,
            restored: false,
        })
    }

    pub fn enabled(&self) -> bool
    {
        self.mode_enabled && !self.restored
    }

    pub fn restore(&mut self)
    {
        if self.restored
        {
            return;
        }
        self.restored = true;
        unsafe
        {
            TERMINAL_SIGNAL_STATE_ACTIVE = 0;
            TERMINAL_SIGNAL_HAS_ORIGINAL = 0;
            libc::sigaction(libc::SIGTERM, &self.previous_sigterm, std::ptr::null_mut());
            libc::sigaction(libc::SIGHUP, &self.previous_sighup, std::ptr::null_mut());
            libc::sigaction(libc::SIGINT, &self.previous_sigint, std::ptr::null_mut());
            if self.mode_enabled
            {
                let mut restored = self.original;
                restored.c_oflag |= libc::OPOST | libc::ONLCR;
                libc::tcsetattr(self.input_fd, libc::TCSAFLUSH, &restored);
            }
            let _ = libc::write(
                self.output_fd,
                self.leave_sequence.as_ptr().cast(),
                self.leave_sequence.len(),
            );
        }
    }
}

#[cfg(unix)]
impl Drop for TerminalModeGuard
{
    fn drop(&mut self)
    {
        self.restore();
    }
}

#[cfg(unix)]
fn terminal_cell_pixel_size_from_ioctl(output_fd: i32) -> Option<Size>
{
    let mut size = MaybeUninit::<libc::winsize>::zeroed();
    if unsafe
    {
        libc::ioctl(
            output_fd,
            libc::TIOCGWINSZ,
            size.as_mut_ptr(),
        )
    } != 0
    {
        return None;
    }
    let size = unsafe { size.assume_init() };
    if size.ws_col == 0 || size.ws_row == 0 || size.ws_xpixel == 0 || size.ws_ypixel == 0
    {
        return None;
    }
    Some(Size
    {
        width: max(
            1,
            (i32::from(size.ws_xpixel) + i32::from(size.ws_col) / 2)
                / i32::from(size.ws_col),
        ),
        height: max(
            1,
            (i32::from(size.ws_ypixel) + i32::from(size.ws_row) / 2)
                / i32::from(size.ws_row),
        ),
    })
}

#[cfg(unix)]
fn parse_terminal_i32_prefix(value: &str) -> Option<i32>
{
    let value = value.trim_start();
    let bytes = value.as_bytes();
    let mut end = usize::from(matches!(bytes.first(), Some(b'+' | b'-')));
    let digits_start = end;
    while bytes.get(end).is_some_and(u8::is_ascii_digit)
    {
        end += 1;
    }
    if end == digits_start
    {
        return None;
    }
    value[..end].parse().ok()
}

#[cfg(unix)]
fn parse_terminal_pixel_response(response: &str, prefix: &str) -> Option<Size>
{
    let start = response.find(prefix)? + prefix.len();
    let separator = response[start..].find(';')? + start;
    let end = response[separator + 1..].find('t')? + separator + 1;
    let height = parse_terminal_i32_prefix(&response[start..separator])?;
    let width = parse_terminal_i32_prefix(&response[separator + 1..end])?;
    if width <= 0 || height <= 0
    {
        return None;
    }
    Some(Size { width, height })
}

#[cfg(unix)]
fn query_terminal_pixel_report(
    input_fd: i32,
    output_fd: i32,
    query: &str,
    prefix: &str,
) -> Option<Size>
{
    write_fd(output_fd, query.as_bytes()).ok()?;
    let mut response = Vec::with_capacity(TERMINAL_PIXEL_RESPONSE_MAX_BYTES);
    while response.len() < TERMINAL_PIXEL_RESPONSE_MAX_BYTES
    {
        let mut poll = libc::pollfd
        {
            fd: input_fd,
            events: libc::POLLIN,
            revents: 0,
        };
        let timeout = TERMINAL_PIXEL_QUERY_TIMEOUT
            .as_millis()
            .clamp(1, i32::MAX as u128) as i32;
        if unsafe { libc::poll(&mut poll, 1, timeout) } <= 0
        {
            break;
        }
        let mut bytes = [0u8; TERMINAL_PIXEL_RESPONSE_MAX_BYTES];
        let available = min(bytes.len(), TERMINAL_PIXEL_RESPONSE_MAX_BYTES - response.len());
        let count = unsafe { libc::read(input_fd, bytes.as_mut_ptr().cast(), available) };
        if count <= 0
        {
            break;
        }
        response.extend_from_slice(&bytes[..count as usize]);
        let text = String::from_utf8_lossy(&response);
        if let Some(size) = parse_terminal_pixel_response(&text, prefix)
        {
            return Some(size);
        }
    }
    None
}

#[cfg(unix)]
fn detect_terminal_cell_pixel_size(input_fd: i32, output_fd: i32) -> Option<Size>
{
    if let Some(size) = terminal_cell_pixel_size_from_ioctl(output_fd)
    {
        return Some(size);
    }
    if let Some(size) = query_terminal_pixel_report(
        input_fd,
        output_fd,
        TERMINAL_CELL_PIXEL_QUERY,
        TERMINAL_CELL_PIXEL_RESPONSE_PREFIX,
    )
    {
        return Some(size);
    }
    let terminal = terminal_size_from_fd(output_fd)?;
    let text_area = query_terminal_pixel_report(
        input_fd,
        output_fd,
        TERMINAL_TEXT_AREA_PIXEL_QUERY,
        TERMINAL_TEXT_AREA_PIXEL_RESPONSE_PREFIX,
    )?;
    Some(Size
    {
        width: max(1, (text_area.width + terminal.width / 2) / terminal.width),
        height: max(1, (text_area.height + terminal.height / 2) / terminal.height),
    })
}

#[cfg(unix)]
fn terminal_size_from_fd(output_fd: i32) -> Option<Size>
{
    let mut size = MaybeUninit::<libc::winsize>::zeroed();
    if unsafe { libc::ioctl(output_fd, libc::TIOCGWINSZ, size.as_mut_ptr()) } == 0
    {
        let size = unsafe { size.assume_init() };
        if size.ws_col > 0 && size.ws_row > 0
        {
            return Some(Size
            {
                width: i32::from(size.ws_col),
                height: i32::from(size.ws_row),
            });
        }
    }
    None
}

#[cfg(not(unix))]
fn terminal_size() -> Size
{
    Size
    {
        width: crate::DEFAULT_VIEWPORT_WIDTH,
        height: crate::DEFAULT_VIEWPORT_HEIGHT,
    }
}

#[cfg(unix)]
fn write_fd(output_fd: i32, bytes: &[u8]) -> io::Result<()>
{
    let mut remaining = bytes;
    while !remaining.is_empty()
    {
        let written = unsafe
        {
            libc::write(output_fd, remaining.as_ptr().cast(), remaining.len())
        };
        if written < 0
        {
            let error = io::Error::last_os_error();
            if error.kind() == io::ErrorKind::Interrupted
            {
                continue;
            }
            return Err(error);
        }
        remaining = &remaining[written as usize..];
    }
    Ok(())
}

pub trait TerminalBackend
{
    fn size(&self) -> Option<Size>;

    fn cell_pixel_size(&mut self) -> Option<Size>
    {
        None
    }

    fn read_events(&mut self) -> io::Result<Vec<Event>>;
    fn write(&mut self, bytes: &[u8]) -> io::Result<()>;
}

#[cfg(unix)]
pub struct PosixTerminalBackend
{
    input_fd: i32,
    output_fd: i32,
    parser: InputParser,
    pending_escape_since: Option<Instant>,
}

#[cfg(unix)]
impl PosixTerminalBackend
{
    pub fn new(input_fd: i32, output_fd: i32) -> Self
    {
        Self
        {
            input_fd,
            output_fd,
            parser: InputParser::default(),
            pending_escape_since: None,
        }
    }
}

#[cfg(unix)]
impl TerminalBackend for PosixTerminalBackend
{
    fn size(&self) -> Option<Size>
    {
        terminal_size_from_fd(self.output_fd)
    }

    fn cell_pixel_size(&mut self) -> Option<Size>
    {
        detect_terminal_cell_pixel_size(self.input_fd, self.output_fd)
    }

    fn read_events(&mut self) -> io::Result<Vec<Event>>
    {
        let timeout_ms = i32::try_from(INPUT_POLL_INTERVAL.as_millis())
            .unwrap_or(i32::MAX);
        let mut poll = libc::pollfd
        {
            fd: self.input_fd,
            events: libc::POLLIN,
            revents: 0,
        };
        let ready = unsafe { libc::poll(&mut poll, 1, timeout_ms) };
        if ready < 0
        {
            let error = io::Error::last_os_error();
            if error.kind() != io::ErrorKind::Interrupted
            {
                return Err(error);
            }
        }
        let mut events = Vec::new();
        if ready > 0 && poll.revents & libc::POLLIN != 0
        {
            let mut bytes = [0u8; INPUT_BUFFER_BYTES];
            let mut drained = 0;
            loop
            {
                let available = min(bytes.len(), INPUT_DRAIN_MAX_BYTES - drained);
                if available == 0
                {
                    break;
                }
                let count = unsafe
                {
                    libc::read(
                        self.input_fd,
                        bytes.as_mut_ptr().cast(),
                        available,
                    )
                };
                if count < 0
                {
                    let error = io::Error::last_os_error();
                    if !matches!(
                        error.kind(),
                        io::ErrorKind::WouldBlock | io::ErrorKind::Interrupted
                    )
                    {
                        return Err(error);
                    }
                    break;
                }
                if count == 0
                {
                    break;
                }
                drained += count as usize;
                events.extend(self.parser.feed(&bytes[..count as usize]));
                let mut more = libc::pollfd
                {
                    fd: self.input_fd,
                    events: libc::POLLIN,
                    revents: 0,
                };
                if unsafe { libc::poll(&mut more, 1, 0) } <= 0
                {
                    break;
                }
            }
        }
        if self.parser.has_pending_escape()
        {
            let since = self.pending_escape_since.get_or_insert_with(Instant::now);
            if since.elapsed() >= ESCAPE_CONTINUATION_TIMEOUT
            {
                events.extend(self.parser.flush_pending_escape());
                self.pending_escape_since = None;
            }
        }
        else
        {
            self.pending_escape_since = None;
        }
        Ok(events)
    }

    fn write(&mut self, bytes: &[u8]) -> io::Result<()>
    {
        write_fd(self.output_fd, bytes)
    }
}

#[cfg(not(unix))]
fn read_terminal_events() -> io::Result<Vec<Event>>
{
    let mut byte = [0u8; 1];
    io::stdin().read_exact(&mut byte)?;
    Ok(vec![Event::key(char::from(byte[0]).to_string())])
}

fn type_terminal_text<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    text: &str,
)
{
    let focused = state.focused_element(app.active_window());
    if !state.edit_mode
        && focused.as_ref().is_some_and(|element|
        {
            matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
        })
    {
        state.handle_key(app, options, "Enter");
    }
    for ch in text.chars()
    {
        let key = if ch == '\n' || ch == '\r'
        {
            "Enter".to_string()
        }
        else
        {
            ch.to_string()
        };
        state.handle_key(app, options, &key);
    }
}

fn clipboard_storage() -> &'static Mutex<String>
{
    static CLIPBOARD: OnceLock<Mutex<String>> = OnceLock::new();
    CLIPBOARD.get_or_init(|| Mutex::new(String::new()))
}

fn run_clipboard_command(command: &str, args: &[&str], text: &str) -> bool
{
    let Ok(mut child) = Command::new(command)
        .args(args)
        .stdin(Stdio::piped())
        .stdout(Stdio::null())
        .stderr(Stdio::null())
        .spawn()
    else
    {
        return false;
    };
    child
        .stdin
        .take()
        .is_some_and(|mut input| input.write_all(text.as_bytes()).is_ok())
        && child.wait().is_ok_and(|status| status.success())
}

pub fn copy_text_to_clipboard(text: &str) -> bool
{
    if let Ok(mut clipboard) = clipboard_storage().lock()
    {
        clipboard.clear();
        clipboard.push_str(text);
    }
    if text.is_empty()
    {
        return false;
    }
    #[cfg(unix)]
    {
        use std::io::IsTerminal;
        if !io::stdin().is_terminal() || !io::stdout().is_terminal()
        {
            return false;
        }
        for (command, args) in [
            ("pbcopy", &[][..]),
            ("wl-copy", &[][..]),
            ("xclip", &["-selection", "clipboard"][..]),
            ("xsel", &["--clipboard", "--input"][..]),
        ]
        {
            if run_clipboard_command(command, args, text)
            {
                return true;
            }
        }
        #[cfg(target_os = "macos")]
        if Command::new("osascript")
            .args([
                "-e",
                "on run argv",
                "-e",
                "set the clipboard to item 1 of argv",
                "-e",
                "end run",
                "--",
                text,
            ])
            .stdout(Stdio::null())
            .stderr(Stdio::null())
            .status()
            .is_ok_and(|status| status.success())
        {
            return true;
        }
    }
    false
}

pub fn runtime_clipboard_text() -> String
{
    clipboard_storage()
        .lock()
        .map_or_else(|_| String::new(), |clipboard| clipboard.clone())
}

fn dispatch_terminal_event<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    event: Event,
    clipboard: &mut String,
)
{
    if state.has_pending_standard_escape()
    {
        return;
    }
    match event.event_type
    {
        EventType::Key if matches!(event.key.as_str(), "cmd_c" | "Cmd+C") =>
        {
            if let Some(element) = state.focused_element(app.active_window())
            {
                let selected = crate::mcp::selected_text(&element);
                if !selected.is_empty()
                {
                    clipboard.clone_from(&selected);
                    let _ = copy_text_to_clipboard(&selected);
                    app.window_mut()
                        .active_window_mut()
                        .show_notification("Copied to clipboard");
                }
            }
        }
        EventType::Key
            if matches!(event.key.as_str(), "cmd_v" | "Cmd+V" | "Ctrl+V") =>
        {
            *clipboard = runtime_clipboard_text();
            type_terminal_text(app, state, options, clipboard);
        }
        EventType::Key =>
        {
            if event.key == "Escape" && state.begin_standard_escape(app, options)
            {
                return;
            }
            state.handle_key(app, options, &event.key);
        }
        EventType::Paste =>
        {
            type_terminal_text(app, state, options, &event.text);
        }
        EventType::MousePress =>
        {
            crate::mcp::perform_mouse_press(
                app,
                state,
                options,
                state.viewport,
                event.position.col,
                event.position.row,
            );
        }
        EventType::MouseDrag =>
        {
            crate::mcp::perform_mouse_move(
                app,
                state,
                event.position.col,
                event.position.row,
            );
        }
        EventType::MouseRelease =>
        {
            if let Some(selected) =
                crate::mcp::perform_mouse_release(
                    app,
                    state,
                    options,
                    event.position.col,
                    event.position.row,
                )
            {
                clipboard.clone_from(&selected);
                let _ = copy_text_to_clipboard(&selected);
                app.window_mut()
                    .active_window_mut()
                    .show_notification("Copied to clipboard");
            }
        }
        EventType::MouseWheel =>
        {
            crate::mcp::perform_mouse_wheel(
                app,
                state,
                options,
                state.viewport,
                event.position.col,
                event.position.row,
                event.wheel_delta,
            );
        }
        EventType::Resize => state.viewport = event.size,
        EventType::Focus
        | EventType::Blur
        | EventType::Open
        | EventType::Close
        | EventType::Save
        | EventType::ElementChanged => {}
    }
}

fn style_has_animated_text_gradient(style: &crate::Style) -> bool
{
    style.text_color_gradient.is_some() || style.text_background_gradient.is_some()
}

fn element_has_animated_text_gradient(element: &crate::ElementRef) -> bool
{
    let value = element.borrow();
    if std::iter::once(&value.style)
        .chain(
            [
                &value.focus_style,
                &value.edit_style,
                &value.cursor_style,
                &value.selected_style,
                &value.active_style,
            ]
            .into_iter()
            .filter_map(Option::as_ref),
        )
        .any(style_has_animated_text_gradient)
    {
        return true;
    }
    if value
        .children
        .iter()
        .any(element_has_animated_text_gradient)
    {
        return true;
    }
    value
        .child_window()
        .is_some_and(window_has_animated_text_gradient)
}

fn window_has_animated_text_gradient(window: &crate::GeneratedWindow) -> bool
{
    style_has_animated_text_gradient(&window.window_style)
        || window
            .elements
            .iter()
            .any(element_has_animated_text_gradient)
}

fn element_needs_sixel_fallback_warning(element: &crate::ElementRef) -> bool
{
    let value = element.borrow();
    if value.kind() == ElementKind::Image
        && crate::image_mode_needs_sixel_fallback_warning(value.render_mode())
    {
        return true;
    }
    if value
        .children
        .iter()
        .any(element_needs_sixel_fallback_warning)
    {
        return true;
    }
    value
        .child_window()
        .is_some_and(window_needs_sixel_fallback_warning)
}

fn window_needs_sixel_fallback_warning(window: &crate::GeneratedWindow) -> bool
{
    window
        .elements
        .iter()
        .any(element_needs_sixel_fallback_warning)
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
struct ScrollRegionHint
{
    rect: Rect,
    delta: i32,
}

fn merge_scroll_region_hint(
    pending: &mut Option<ScrollRegionHint>,
    rect: Rect,
    delta: i32,
)
{
    if delta == 0
    {
        return;
    }
    if let Some(current) = pending.as_mut()
        && current.rect == rect
    {
        current.delta += delta;
        if current.delta == 0
        {
            *pending = None;
        }
        return;
    }
    *pending = Some(ScrollRegionHint { rect, delta });
}

fn consume_element_scroll_region_hint(
    element: &crate::ElementRef,
    pending: &mut Option<ScrollRegionHint>,
)
{
    let value = element.borrow();
    if value.kind() == ElementKind::ScrollView
    {
        merge_scroll_region_hint(
            pending,
            value.frame,
            value.consume_terminal_scroll_delta(),
        );
    }
    for child in &value.children
    {
        consume_element_scroll_region_hint(child, pending);
    }
    if let Some(window) = value.child_window()
    {
        consume_window_scroll_region_hints(window, value.frame, pending);
    }
}

fn consume_window_scroll_region_hints(
    window: &crate::GeneratedWindow,
    bounds: Rect,
    pending: &mut Option<ScrollRegionHint>,
)
{
    if window.is_generated_scroll_view()
    {
        merge_scroll_region_hint(
            pending,
            bounds,
            window.consume_terminal_scroll_delta(),
        );
    }
    for element in &window.elements
    {
        consume_element_scroll_region_hint(element, pending);
    }
}

pub fn run_interactive_terminal<A: GeneratedApplication>(
    app: &mut A,
    options: GeneratedWindowRuntimeOptions,
    mut mcp_config: crate::McpRuntimeConfig,
) -> i32
{
    let embedded_terminal = mcp_config.viewport.is_some();
    #[cfg(unix)]
    let input_fd = io::stdin().as_raw_fd();
    #[cfg(unix)]
    let output_fd = io::stdout().as_raw_fd();
    #[cfg(unix)]
    let _guard = match TerminalModeGuard::new_with_leave_sequence(
        input_fd,
        output_fd,
        if embedded_terminal { ANSI_EMBEDDED_LEAVE } else { ANSI_LEAVE },
    )
    {
        Ok(guard) => guard,
        Err(error) =>
        {
            eprintln!("uimd: error: {error}");
            return 1;
        }
    };
    #[cfg(unix)]
    let mut backend = PosixTerminalBackend::new(input_fd, output_fd);
    #[cfg(unix)]
    if let Some(size) = backend.cell_pixel_size()
    {
        crate::set_terminal_cell_pixel_size(size);
    }
    #[cfg(unix)]
    let enter_sequence = if embedded_terminal
    {
        ANSI_EMBEDDED_ENTER.to_string()
    }
    else
    {
        format!("\x1b]0;{} [rust]\x07{ANSI_ENTER}", app.window().title)
    };
    #[cfg(unix)]
    if let Err(error) = backend.write(enter_sequence.as_bytes())
    {
        eprintln!("uimd: error: {error}");
        return 1;
    }
    #[cfg(unix)]
    let viewport = mcp_config.viewport.map_or_else(
        ||
        {
            backend.size().unwrap_or(Size
            {
                width: crate::DEFAULT_VIEWPORT_WIDTH,
                height: crate::DEFAULT_VIEWPORT_HEIGHT,
            })
        },
        |viewport| Size { width: viewport.width, height: viewport.height },
    );
    #[cfg(not(unix))]
    let viewport = terminal_size();
    let mut sixel_fallback_dialog = None;
    if window_needs_sixel_fallback_warning(app.window())
    {
        let mut warning = crate::MessageBoxYesNo::new(
            "Warning",
            SIXEL_FALLBACK_WARNING,
        );
        app.window_mut().open_window(warning.take_window());
        sixel_fallback_dialog = Some(warning);
    }
    let mut state = RuntimeState::new(app.window(), &options, viewport);
    state.notify_initial_edit_started(app, &options);
    let mcp_server = if mcp_config.enabled
    {
        match crate::mcp::McpSocketServer::start(&mcp_config)
        {
            Ok(server) => Some(server),
            Err(error) =>
            {
                eprintln!("uimd: error: {error}");
                return 1;
            }
        }
    }
    else
    {
        None
    };
    let mut pending_mcp_responses = Vec::new();
    let mut clipboard = String::new();
    let mut dirty = true;
    let has_animated_text_gradient = window_has_animated_text_gradient(app.window());
    let mut last_animated_render_at = Instant::now();
    let mut presented = crate::TerminalBuffer::new(viewport.width, viewport.height);
    let mut pending_scroll_region: Option<ScrollRegionHint> = None;
    let mut previous_modal_depth = app.window().modal_depth();
    let mut clear_before_next_frame = false;
    loop
    {
        if let Some(server) = &mcp_server
        {
            loop
            {
                let request = match server.try_recv()
                {
                    Ok(request) => request,
                    Err(std::sync::mpsc::TryRecvError::Empty) => break,
                    Err(std::sync::mpsc::TryRecvError::Disconnected) => break,
                };
                let previous_viewport = mcp_config.viewport;
                let mut progress_write_failed = None;
                let response = crate::mcp::dispatch_json_rpc_value_with_progress(
                    app,
                    &mut state,
                    &options,
                    &mut mcp_config,
                    &request.request,
                    &mut |progress_app, progress_state, progress_options, progress_config|
                    {
                        let rendered = crate::render_generated_application(
                            progress_app,
                            progress_state,
                            progress_options,
                        );
                        presented.replace_current(&rendered);
                        let viewport_origin = progress_config.viewport.unwrap_or_default();
                        let scroll_output = pending_scroll_region
                            .filter(|hint|
                            {
                                progress_config.viewport.is_none()
                                    && hint.rect.col == 0
                                    && hint.rect.width >= presented.width
                            })
                            .map_or_else(String::new, |hint|
                            {
                                presented.render_scroll_region(
                                    0,
                                    hint.rect.row,
                                    hint.rect.height,
                                    hint.delta,
                                )
                            });
                        pending_scroll_region = None;
                        let frame = scroll_output
                            + &presented.render_diff(
                                viewport_origin.row,
                                viewport_origin.col,
                            );
                        #[cfg(unix)]
                        {
                            let write_result = if clear_before_next_frame
                            {
                                clear_before_next_frame = false;
                                backend.write(ANSI_CLEAR_SCREEN.as_bytes())
                            }
                            else
                            {
                                Ok(())
                            };
                            if let Err(error) =
                                write_result.and_then(|()| backend.write(frame.as_bytes()))
                            {
                                progress_write_failed = Some(error);
                            }
                        }
                        #[cfg(not(unix))]
                        {
                            let mut output = io::stdout().lock();
                            let write_result = if clear_before_next_frame
                            {
                                clear_before_next_frame = false;
                                output.write_all(ANSI_CLEAR_SCREEN.as_bytes())
                            }
                            else
                            {
                                Ok(())
                            };
                            if let Err(error) = write_result
                                .and_then(|()| output.write_all(frame.as_bytes()))
                                .and_then(|()| output.flush())
                            {
                                progress_write_failed = Some(error);
                            }
                        }
                    },
                );
                if let Some(error) = progress_write_failed
                {
                    eprintln!("uimd: error: {error}");
                    return 1;
                }
                crate::runtime::close_completed_modal(app, &mut state, &options);
                if mcp_config.viewport != previous_viewport
                {
                    if let Some(viewport) = mcp_config.viewport
                    {
                        state.viewport = Size
                        {
                            width: viewport.width,
                            height: viewport.height,
                        };
                        presented.resize(viewport.width, viewport.height);
                    }
                    pending_scroll_region = None;
                    clear_before_next_frame = mcp_config.viewport.is_none();
                    presented.request_full_redraw();
                }
                dirty = true;
                if mcp_config.wait_render
                {
                    pending_mcp_responses.push((request, response));
                }
                else
                {
                    request.respond(response);
                }
            }
        }
        if sixel_fallback_dialog
            .as_ref()
            .is_some_and(crate::MessageBoxYesNo::closed)
        {
            let continue_requested = sixel_fallback_dialog
                .take()
                .and_then(|dialog| dialog.result())
                == Some(crate::MessageBoxResult::Yes);
            if !continue_requested
            {
                return 0;
            }
            presented.request_full_redraw();
            dirty = true;
        }
        if state.complete_pending_standard_escape(app, &options)
        {
            crate::runtime::close_completed_modal(app, &mut state, &options);
            dirty = true;
        }
        if app.window_mut().active_window_mut().expire_notification()
        {
            dirty = true;
        }
        #[cfg(unix)]
        let next_viewport = mcp_config.viewport.map_or_else(
            || backend.size().unwrap_or(state.viewport),
            |viewport| Size { width: viewport.width, height: viewport.height },
        );
        #[cfg(not(unix))]
        let next_viewport = terminal_size();
        if next_viewport != state.viewport
        {
            #[cfg(unix)]
            if let Some(size) = backend.cell_pixel_size()
            {
                crate::set_terminal_cell_pixel_size(size);
            }
            state.viewport = next_viewport;
            presented.resize(next_viewport.width, next_viewport.height);
            pending_scroll_region = None;
            clear_before_next_frame = mcp_config.viewport.is_none();
            dirty = true;
        }
        let modal_depth = app.window().modal_depth();
        if modal_depth != previous_modal_depth
        {
            previous_modal_depth = modal_depth;
            presented.request_full_redraw();
            pending_scroll_region = None;
            clear_before_next_frame = mcp_config.viewport.is_none();
            dirty = true;
        }
        let now = Instant::now();
        if !mcp_config.controlled_render
            && has_animated_text_gradient
            && now.duration_since(last_animated_render_at) >= ANIMATED_RENDER_INTERVAL
        {
            dirty = true;
            last_animated_render_at = now;
        }
        if dirty
        {
            let rendered = crate::render_generated_application(app, &mut state, &options);
            presented.replace_current(&rendered);
            let viewport_origin = mcp_config.viewport.unwrap_or_default();
            let scroll_output = pending_scroll_region
                .filter(|hint|
                {
                    mcp_config.viewport.is_none()
                        && hint.rect.col == 0
                        && hint.rect.width >= presented.width
                })
                .map_or_else(String::new, |hint|
                {
                    presented.render_scroll_region(
                        0,
                        hint.rect.row,
                        hint.rect.height,
                        hint.delta,
                    )
            });
            pending_scroll_region = None;
            let frame = scroll_output
                + &presented.render_diff(viewport_origin.row, viewport_origin.col);
            #[cfg(unix)]
            let write_result = if clear_before_next_frame
            {
                clear_before_next_frame = false;
                backend.write(ANSI_CLEAR_SCREEN.as_bytes())
            }
            else
            {
                Ok(())
            };
            #[cfg(unix)]
            if let Err(error) = write_result
                .and_then(|()| backend.write(frame.as_bytes()))
            {
                eprintln!("uimd: error: {error}");
                return 1;
            }
            #[cfg(not(unix))]
            {
                let mut output = io::stdout().lock();
                let write_result = if clear_before_next_frame
                {
                    clear_before_next_frame = false;
                    output.write_all(ANSI_CLEAR_SCREEN.as_bytes())
                }
                else
                {
                    Ok(())
                };
                if let Err(error) = write_result
                    .and_then(|()| output.write_all(frame.as_bytes()))
                    .and_then(|()| output.flush())
                {
                    eprintln!("uimd: error: {error}");
                    return 1;
                }
            }
            dirty = false;
            for (request, response) in pending_mcp_responses.drain(..)
            {
                request.respond(response);
            }
        }
        if crate::runtime::generated_application_should_close(app, &options)
        {
            return 0;
        }
        #[cfg(unix)]
        let events = match backend.read_events()
        {
            Ok(events) => events,
            Err(error) =>
            {
                eprintln!("uimd: error: {error}");
                return 1;
            }
        };
        #[cfg(not(unix))]
        let events = match read_terminal_events()
        {
            Ok(events) => events,
            Err(error) =>
            {
                eprintln!("uimd: error: {error}");
                return 1;
            }
        };
        for mut event in coalesce_mouse_wheel_events(events)
        {
            if matches!(
                event.event_type,
                EventType::MousePress
                    | EventType::MouseDrag
                    | EventType::MouseRelease
                    | EventType::MouseWheel
            )
                && let Some(viewport) = mcp_config.viewport
            {
                event.position.row -= viewport.row;
                event.position.col -= viewport.col;
            }
            dispatch_terminal_event(
                app,
                &mut state,
                &options,
                event,
                &mut clipboard,
            );
            crate::runtime::close_completed_modal(app, &mut state, &options);
            let active = app.active_window();
            consume_window_scroll_region_hints(
                active,
                Rect
                {
                    row: 0,
                    col: 0,
                    width: state.viewport.width,
                    height: state.viewport.height,
                },
                &mut pending_scroll_region,
            );
            dirty = true;
        }
    }
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn parser_matches_reference_key_and_modifier_names()
    {
        let mut parser = InputParser::default();
        assert_eq!(
            parser.feed(b"\x1b[A\x1b[1;2B\x1b[1;3C\x1b[1;5D\x1bOA"),
            vec![
                Event::key("Up"),
                Event::key("Shift+Down"),
                Event::key("Alt+Right"),
                Event::key("Ctrl+Left"),
                Event::key("Up"),
            ],
        );
        assert_eq!(
            parser.feed(
                b"\x1b[13;3u\x1b[13;9u\x1b[99;9u\x1b[118;9u\
                  \x1b[27;3;13~\x1b[27;5;13~\x1b[27;5;99~",
            ),
            vec![
                Event::key("Alt+Enter"),
                Event::key("Alt+Enter"),
                Event::key("cmd_c"),
                Event::key("cmd_v"),
                Event::key("Alt+Enter"),
                Event::key("Alt+Enter"),
                Event::key("Ctrl+C"),
            ],
        );
    }

    #[test]
    fn parser_rejects_non_reference_key_extensions_and_preserves_raw_controls()
    {
        let mut parser = InputParser::default();
        assert_eq!(
            parser.feed(b"\x08\x01"),
            vec![Event::key("\x08"), Event::key("\x01")],
        );
        assert_eq!(
            parser.feed(b"\x1b[3~"),
            vec![
                Event::key("Escape"),
                Event::key("["),
                Event::key("3"),
                Event::key("~"),
            ],
        );
        assert_eq!(
            parser.feed(b"\x1bOX"),
            vec![Event::key("Escape"), Event::key("O"), Event::key("X")],
        );
        assert_eq!(
            parser.feed(b"\x1b[27;9;99~"),
            "\x1b[27;9;99~"
                .chars()
                .map(|value| Event::key(if value == '\x1b'
                {
                    "Escape".to_string()
                }
                else
                {
                    value.to_string()
                }))
                .collect::<Vec<_>>(),
        );
        assert_eq!(
            parser.feed(b"\x1b[3;1u"),
            "\x1b[3;1u"
                .chars()
                .map(|value| Event::key(if value == '\x1b'
                {
                    "Escape".to_string()
                }
                else
                {
                    value.to_string()
                }))
                .collect::<Vec<_>>(),
        );
        assert_eq!(
            parser.feed(b"\x1b[99;9;1u"),
            "\x1b[99;9;1u"
                .chars()
                .map(|value| Event::key(if value == '\x1b'
                {
                    "Escape".to_string()
                }
                else
                {
                    value.to_string()
                }))
                .collect::<Vec<_>>(),
        );
    }

    #[test]
    fn parser_preserves_split_sequences_and_emits_one_paste_event()
    {
        let mut parser = InputParser::default();
        assert!(parser.feed(b"\x1b[").is_empty());
        assert_eq!(
            parser.feed(b"A"),
            vec![Event::key("Up")],
        );
        assert!(parser.feed(b"\x1b[200~hello\n").is_empty());
        assert_eq!(
            parser.feed(b"world\x1b[201~"),
            vec![Event::paste("hello\nworld")],
        );
    }

    #[test]
    fn parser_decodes_sgr_mouse_and_consumes_terminal_pixel_reports()
    {
        let mut parser = InputParser::default();
        assert_eq!(
            parser.feed(
                b"\x1b[<0;5;7M\x1b[<32;6;8M\x1b[<0;6;8m\x1b[<64;5;7M\x1b[<68;5;7M",
            ),
            vec![
                Event::pointer(EventType::MousePress, 4, 6),
                Event::pointer(EventType::MouseDrag, 5, 7),
                Event::pointer(EventType::MouseRelease, 5, 7),
                Event::wheel(4, 6, 1),
                Event::pointer(EventType::MousePress, 4, 6),
            ],
        );
        assert!(parser.feed(b"\x1b[6;18;9t\x1b[4;900;1440t").is_empty());
        assert!(parser.feed(b"\x1b[<0;bad;7M").is_empty());
        assert!(parser.feed(b"\x1b[<0;5M").is_empty());
        assert!(parser.feed(b"\x1b[<0;5;7;9M").is_empty());
        assert_eq!(parser.feed(b"x"), vec![Event::key("x")]);
    }

    #[test]
    fn consecutive_mouse_wheel_bursts_match_cpp_bounded_coalescing()
    {
        let burst_count = usize::try_from(MAX_COALESCED_MOUSE_WHEEL_DELTA)
            .expect("positive wheel coalescing bound")
            + 8;
        let mut events = (0..burst_count)
            .map(|index| Event::wheel(index as i32, index as i32 + 1, 1))
            .collect::<Vec<_>>();
        events.push(Event::key("Down"));
        events.push(Event::wheel(30, 31, 1));
        events.push(Event::wheel(32, 33, -1));
        events.push(Event::pointer(EventType::MousePress, 34, 35));
        events.extend(
            (0..burst_count)
                .map(|index| Event::wheel(index as i32 + 40, index as i32 + 41, -1)),
        );

        assert_eq!(
            coalesce_mouse_wheel_events(events),
            vec![
                Event::wheel(
                    burst_count as i32 - 1,
                    burst_count as i32,
                    MAX_COALESCED_MOUSE_WHEEL_DELTA,
                ),
                Event::key("Down"),
                Event::pointer(EventType::MousePress, 34, 35),
                Event::wheel(
                    burst_count as i32 + 39,
                    burst_count as i32 + 40,
                    -MAX_COALESCED_MOUSE_WHEEL_DELTA,
                ),
            ],
        );
    }

    #[test]
    #[cfg(unix)]
    fn pixel_report_parser_matches_cpp_stoi_prefix_behavior()
    {
        assert_eq!(
            parse_terminal_pixel_response(
                "noise\x1b[6; 18rows;+9px;unusedt",
                TERMINAL_CELL_PIXEL_RESPONSE_PREFIX,
            ),
            Some(Size { width: 9, height: 18 }),
        );
        assert_eq!(
            parse_terminal_pixel_response(
                "\x1b[6;-18;9t",
                TERMINAL_CELL_PIXEL_RESPONSE_PREFIX,
            ),
            None,
        );
        assert_eq!(
            parse_terminal_pixel_response(
                "\x1b[6;height;9t",
                TERMINAL_CELL_PIXEL_RESPONSE_PREFIX,
            ),
            None,
        );
    }

    #[test]
    fn standalone_escape_is_flushed_only_after_continuation_wait()
    {
        let mut parser = InputParser::default();
        assert!(parser.feed(b"\x1b").is_empty());
        assert_eq!(
            parser.flush_pending_escape(),
            vec![Event::key("Escape")],
        );
    }

    #[cfg(unix)]
    #[test]
    fn non_tty_input_keeps_the_terminal_lifecycle_available_for_gui_mcp()
    {
        let mut input_pipe = [0; 2];
        let mut output_pipe = [0; 2];
        assert_eq!(unsafe { libc::pipe(input_pipe.as_mut_ptr()) }, 0);
        assert_eq!(unsafe { libc::pipe(output_pipe.as_mut_ptr()) }, 0);

        let guard = TerminalModeGuard::new_with_leave_sequence(
            input_pipe[0],
            output_pipe[1],
            ANSI_EMBEDDED_LEAVE,
        )
        .expect("non-TTY input must not reject the terminal lifecycle");
        assert!(!guard.enabled());
        drop(guard);
        unsafe
        {
            libc::close(output_pipe[1]);
        }

        let mut bytes = [0u8; 64];
        let count = unsafe
        {
            libc::read(
                output_pipe[0],
                bytes.as_mut_ptr().cast(),
                bytes.len(),
            )
        };
        assert_eq!(
            &bytes[..usize::try_from(count).expect("teardown byte count")],
            ANSI_EMBEDDED_LEAVE.as_bytes(),
        );
        unsafe
        {
            libc::close(input_pipe[0]);
            libc::close(input_pipe[1]);
            libc::close(output_pipe[0]);
        }
    }

    #[test]
    fn runtime_clipboard_keeps_the_reference_internal_copy_contract()
    {
        let text = "Rust internal clipboard";
        let _ = copy_text_to_clipboard(text);
        assert_eq!(runtime_clipboard_text(), text);
    }
}

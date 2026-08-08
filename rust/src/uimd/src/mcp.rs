use crate::{
    focusable_elements, ElementKind, ElementRef, GeneratedApplication,
    GeneratedWindowRuntimeOptions, Point, Rect, RuntimeState, Size,
};
use serde_json::{json, Map, Value};
use std::cmp::{max, min};
use std::collections::HashMap;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
#[cfg(test)]
use std::net::SocketAddr;
use std::net::{TcpListener, TcpStream};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{mpsc, Arc};
use std::thread::{self, JoinHandle};
use std::time::Duration;

const MCP_PROTOCOL_VERSION: &str = "2024-11-05";
const MCP_HTTP_ENDPOINT: &str = "/mcp";
const JSON_RPC_ERROR_CODE: i32 = -32000;
const DEFAULT_MCP_PORT: i32 = 8765;
const DEFAULT_GUI_MCP_ACTION_DELAY_MS: i32 = 500;
const DEFAULT_GUI_MCP_TYPE_DELAY_MS: i32 = 50;
const DEFAULT_HEADLESS_MCP_ACTION_DELAY_MS: i32 = 0;
const DEFAULT_HEADLESS_MCP_TYPE_DELAY_MS: i32 = 0;
const MCP_ACCEPT_POLL_INTERVAL: Duration = Duration::from_millis(10);

#[derive(Clone, Debug)]
pub struct McpRuntimeConfig
{
    pub enabled: bool,
    pub gui: bool,
    pub transport: String,
    pub host: String,
    pub port: i32,
    pub action_delay_ms: i32,
    pub type_delay_ms: i32,
    pub wait_render: bool,
    pub controlled_render: bool,
    pub viewport: Option<Rect>,
}

impl Default for McpRuntimeConfig
{
    fn default() -> Self
    {
        Self
        {
            enabled: false,
            gui: true,
            transport: "tcp".to_string(),
            host: "127.0.0.1".to_string(),
            port: DEFAULT_MCP_PORT,
            action_delay_ms: DEFAULT_GUI_MCP_ACTION_DELAY_MS,
            type_delay_ms: DEFAULT_GUI_MCP_TYPE_DELAY_MS,
            wait_render: false,
            controlled_render: false,
            viewport: None,
        }
    }
}

fn has_option(args: &[String], name: &str) -> bool
{
    args.iter().skip(1).any(|argument| argument == name)
}

fn option_value<'a>(args: &'a [String], name: &str) -> Option<&'a str>
{
    args.iter()
        .enumerate()
        .skip(1)
        .find_map(|(index, argument)|
        {
            (argument == name)
                .then(|| args.get(index + 1).map(String::as_str))
                .flatten()
        })
}

fn parse_cpp_i32(value: &str) -> i32
{
    let value = value.trim_start();
    let bytes = value.as_bytes();
    let mut end = usize::from(
        bytes
            .first()
            .is_some_and(|byte| matches!(*byte, b'+' | b'-')),
    );
    let digit_start = end;
    while bytes.get(end).is_some_and(u8::is_ascii_digit)
    {
        end += 1;
    }
    assert!(
        end > digit_start,
        "invalid integer argument: {value}",
    );
    value[..end]
        .parse()
        .unwrap_or_else(|_| panic!("integer argument out of range: {value}"))
}

fn parse_viewport_arg(args: &[String]) -> Option<Rect>
{
    if let Some(value) = option_value(args, "--viewport")
    {
        let values = value.split(',').map(parse_cpp_i32).collect::<Vec<_>>();
        assert!(
            values.len() == 4,
            "--viewport must have form row,col,width,height",
        );
        return Some(Rect
        {
            row: values[0].max(0),
            col: values[1].max(0),
            width: values[2].max(1),
            height: values[3].max(1),
        });
    }
    if ![
        "--viewport-row",
        "--viewport-col",
        "--viewport-width",
        "--viewport-height",
    ]
    .iter()
    .any(|name| has_option(args, name))
    {
        return None;
    }
    Some(Rect
    {
        row: parse_cpp_i32(option_value(args, "--viewport-row").unwrap_or("0")).max(0),
        col: parse_cpp_i32(option_value(args, "--viewport-col").unwrap_or("0")).max(0),
        width: parse_cpp_i32(option_value(args, "--viewport-width").unwrap_or("1")).max(1),
        height: parse_cpp_i32(option_value(args, "--viewport-height").unwrap_or("1")).max(1),
    })
}

impl McpRuntimeConfig
{
    pub fn parse(args: &[String]) -> Self
    {
        let enabled = has_option(args, "--mcp-server");
        let gui = !has_option(args, "--headless") || has_option(args, "--gui");
        let default_transport = if gui { "tcp" } else { "stdio" };
        let fast = has_option(args, "--mcp-fast");
        let default_action_delay = if fast
        {
            0
        }
        else if gui
        {
            DEFAULT_GUI_MCP_ACTION_DELAY_MS
        }
        else
        {
            DEFAULT_HEADLESS_MCP_ACTION_DELAY_MS
        };
        let default_type_delay = if fast
        {
            0
        }
        else if gui
        {
            DEFAULT_GUI_MCP_TYPE_DELAY_MS
        }
        else
        {
            DEFAULT_HEADLESS_MCP_TYPE_DELAY_MS
        };
        Self
        {
            enabled,
            gui,
            transport: option_value(args, "--mcp-transport")
                .unwrap_or(default_transport)
                .to_string(),
            host: option_value(args, "--mcp-host")
                .unwrap_or("127.0.0.1")
                .to_string(),
            port: parse_cpp_i32(option_value(args, "--mcp-port").unwrap_or("8765")),
            action_delay_ms: parse_cpp_i32(
                option_value(args, "--mcp-action-delay-ms")
                    .map_or_else(|| default_action_delay.to_string(), str::to_string)
                    .as_str(),
            ),
            type_delay_ms: parse_cpp_i32(
                option_value(args, "--mcp-type-delay-ms")
                    .map_or_else(|| default_type_delay.to_string(), str::to_string)
                    .as_str(),
            ),
            wait_render: has_option(args, "--mcp-wait-render"),
            controlled_render: has_option(args, "--mcp-controlled-render"),
            viewport: parse_viewport_arg(args),
        }
    }
}

pub(crate) struct McpSocketRequest
{
    pub request: Value,
    response: mpsc::Sender<Option<Value>>,
}

impl McpSocketRequest
{
    pub fn respond(self, response: Option<Value>)
    {
        let _ = self.response.send(response);
    }
}

pub(crate) struct McpSocketServer
{
    requests: mpsc::Receiver<McpSocketRequest>,
    running: Arc<AtomicBool>,
    thread: Option<JoinHandle<()>>,
    #[cfg(test)]
    address: SocketAddr,
}

impl McpSocketServer
{
    pub fn start(config: &McpRuntimeConfig) -> std::io::Result<Self>
    {
        if !matches!(config.transport.as_str(), "tcp" | "http")
        {
            return Err(std::io::Error::new(
                std::io::ErrorKind::InvalidInput,
                format!("unsupported Rust MCP socket transport: {}", config.transport),
            ));
        }
        let listener = TcpListener::bind((config.host.as_str(), config.port as u16))?;
        #[cfg(test)]
        let address = listener.local_addr()?;
        listener.set_nonblocking(true)?;
        let (request_sender, requests) = mpsc::channel();
        let running = Arc::new(AtomicBool::new(true));
        let thread_running = Arc::clone(&running);
        let transport = config.transport.clone();
        let thread = thread::spawn(move ||
        {
            while thread_running.load(Ordering::Acquire)
            {
                match listener.accept()
                {
                    Ok((stream, _)) =>
                    {
                        if let Err(error) = stream.set_nonblocking(false)
                        {
                            eprintln!(
                                "uimd: error: failed to restore blocking MCP client stream: {error}"
                            );
                            continue;
                        }
                        let request_sender = request_sender.clone();
                        let transport = transport.clone();
                        thread::spawn(move ||
                        {
                            if transport == "http"
                            {
                                serve_socket_http_stream(stream, request_sender);
                            }
                            else
                            {
                                serve_socket_tcp_stream(stream, request_sender);
                            }
                        });
                    }
                    Err(error) if error.kind() == std::io::ErrorKind::WouldBlock =>
                    {
                        thread::sleep(MCP_ACCEPT_POLL_INTERVAL);
                    }
                    Err(error) if error.kind() == std::io::ErrorKind::Interrupted => {}
                    Err(_) => break,
                }
            }
        });
        Ok(Self
        {
            requests,
            running,
            thread: Some(thread),
            #[cfg(test)]
            address,
        })
    }

    pub fn try_recv(&self) -> Result<McpSocketRequest, mpsc::TryRecvError>
    {
        self.requests.try_recv()
    }

    fn recv(&self) -> Result<McpSocketRequest, mpsc::RecvError>
    {
        self.requests.recv()
    }

    #[cfg(test)]
    fn recv_timeout(&self, timeout: Duration) -> Result<McpSocketRequest, mpsc::RecvTimeoutError>
    {
        self.requests.recv_timeout(timeout)
    }

    #[cfg(test)]
    fn local_addr(&self) -> SocketAddr
    {
        self.address
    }
}

impl Drop for McpSocketServer
{
    fn drop(&mut self)
    {
        self.running.store(false, Ordering::Release);
        if let Some(thread) = self.thread.take()
        {
            let _ = thread.join();
        }
    }
}

fn serve_socket_tcp_stream(
    stream: TcpStream,
    request_sender: mpsc::Sender<McpSocketRequest>,
)
{
    let Ok(read_stream) = stream.try_clone() else { return };
    let mut reader = BufReader::new(read_stream);
    let mut line = String::new();
    if reader.read_line(&mut line).ok().filter(|count| *count > 0).is_none()
    {
        return;
    }
    let parsed = match serde_json::from_str::<Value>(&line)
    {
        Ok(value) => value,
        Err(error) =>
        {
            let mut writer = BufWriter::new(stream);
            write_json_value(
                &mut writer,
                &json_rpc_error(Value::Null, -32700, &error.to_string()),
            );
            return;
        }
    };
    let (response_sender, response_receiver) = mpsc::channel();
    if request_sender
        .send(McpSocketRequest { request: parsed, response: response_sender })
        .is_err()
    {
        return;
    }
    let Ok(response) = response_receiver.recv() else { return };
    if let Some(response) = response
    {
        write_json_value(&mut BufWriter::new(stream), &response);
    }
}

fn serve_socket_http_stream(
    mut stream: TcpStream,
    request_sender: mpsc::Sender<McpSocketRequest>,
)
{
    let request = match read_http_request(&stream)
    {
        Ok(request) => request,
        Err(_) => return,
    };
    if let Some(response) = validate_http_request(&request)
    {
        let _ = stream.write_all(response.as_bytes());
        let _ = stream.flush();
        return;
    }
    let parsed = match serde_json::from_str::<Value>(&request.body)
    {
        Ok(value) => value,
        Err(error) =>
        {
            let body = json_rpc_error(Value::Null, -32700, &error.to_string()).to_string();
            let response = http_response(
                200,
                &body,
                &[
                    ("Content-Type", "application/json"),
                    ("MCP-Protocol-Version", MCP_PROTOCOL_VERSION),
                ],
            );
            let _ = stream.write_all(response.as_bytes());
            let _ = stream.flush();
            return;
        }
    };
    let (response_sender, response_receiver) = mpsc::channel();
    if request_sender
        .send(McpSocketRequest { request: parsed, response: response_sender })
        .is_err()
    {
        return;
    }
    let Ok(response) = response_receiver.recv() else { return };
    let response = response.map_or_else(
        || http_response(202, "", &[]),
        |response|
        {
            http_response(
                200,
                &response.to_string(),
                &[
                    ("Content-Type", "application/json"),
                    ("MCP-Protocol-Version", MCP_PROTOCOL_VERSION),
                ],
            )
        },
    );
    let _ = stream.write_all(response.as_bytes());
    let _ = stream.flush();
}

pub fn serve_mcp<A: GeneratedApplication>(
    app: &mut A,
    options: GeneratedWindowRuntimeOptions,
    mut config: McpRuntimeConfig,
) -> i32
{
    let viewport = config.viewport.map_or_else(
        || crate::runtime::generated_window_reported_size(app.window()),
        |viewport| Size { width: viewport.width, height: viewport.height },
    );
    let mut state = RuntimeState::new(app.window(), &options, viewport);
    state.notify_initial_edit_started(app, &options);
    if config.transport == "stdio"
    {
        let input = std::io::stdin();
        let output = std::io::stdout();
        serve_lines(app, &mut state, &options, &mut config, input.lock(), output.lock());
        return 0;
    }
    if !matches!(config.transport.as_str(), "tcp" | "http")
    {
        eprintln!("uimd: error: unsupported Rust MCP transport: {}", config.transport);
        return 1;
    }
    let server = match McpSocketServer::start(&config)
    {
        Ok(server) => server,
        Err(error) =>
        {
            eprintln!("uimd: error: {error}");
            return 1;
        }
    };
    loop
    {
        let request = match server.recv()
        {
            Ok(request) => request,
            Err(_) => return 1,
        };
        let response = dispatch_json_rpc_value(
            app,
            &mut state,
            &options,
            &mut config,
            &request.request,
        );
        request.respond(response);
        crate::runtime::close_completed_modal(app, &mut state, &options);
        if crate::runtime::generated_application_should_close(app, &options)
        {
            return 0;
        }
    }
}

#[derive(Clone, Debug, Default)]
struct HttpRequest
{
    method: String,
    path: String,
    headers: HashMap<String, String>,
    body: String,
}

fn read_http_request(stream: &TcpStream) -> std::io::Result<HttpRequest>
{
    let mut reader = BufReader::new(stream.try_clone()?);
    let mut request_line = String::new();
    reader.read_line(&mut request_line)?;
    let mut parts = request_line.split_whitespace();
    let mut request = HttpRequest
    {
        method: parts.next().unwrap_or_default().to_string(),
        path: parts.next().unwrap_or_default().to_string(),
        ..Default::default()
    };
    loop
    {
        let mut line = String::new();
        if reader.read_line(&mut line)? == 0 || line == "\r\n" || line == "\n"
        {
            break;
        }
        if let Some((name, value)) = line.split_once(':')
        {
            request
                .headers
                .insert(name.trim().to_ascii_lowercase(), value.trim().to_string());
        }
    }
    let content_length = request
        .headers
        .get("content-length")
        .and_then(|value| value.parse::<usize>().ok())
        .unwrap_or_default();
    let mut body = vec![0; content_length];
    reader.read_exact(&mut body)?;
    request.body = String::from_utf8_lossy(&body).into_owned();
    Ok(request)
}

#[cfg(test)]
fn handle_http_request<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    request: &HttpRequest,
) -> String
{
    if let Some(response) = validate_http_request(request)
    {
        return response;
    }
    let parsed = match serde_json::from_str::<Value>(&request.body)
    {
        Ok(value) => value,
        Err(error) =>
        {
            let body = json_rpc_error(Value::Null, -32700, &error.to_string()).to_string();
            return http_response(
                200,
                &body,
                &[
                    ("Content-Type", "application/json"),
                    ("MCP-Protocol-Version", MCP_PROTOCOL_VERSION),
                ],
            );
        }
    };
    let response = dispatch_json_rpc_value(app, state, options, config, &parsed);
    crate::runtime::close_completed_modal(app, state, options);
    let Some(response) = response else
    {
        return http_response(202, "", &[]);
    };
    http_response(
        200,
        &response.to_string(),
        &[
            ("Content-Type", "application/json"),
            ("MCP-Protocol-Version", MCP_PROTOCOL_VERSION),
        ],
    )
}

fn validate_http_request(request: &HttpRequest) -> Option<String>
{
    if request.path != MCP_HTTP_ENDPOINT
    {
        return Some(http_response(404, "", &[]));
    }
    if !mcp_http_origin_allowed(http_header(request, "origin"))
    {
        return Some(http_response(403, "", &[]));
    }
    if !mcp_http_protocol_allowed(http_header(request, "mcp-protocol-version"))
    {
        return Some(http_response(400, "", &[]));
    }
    if request.method != "POST"
    {
        return Some(http_response(405, "", &[]));
    }
    if !mcp_http_accept_allowed(http_header(request, "accept"))
    {
        return Some(http_response(406, "", &[]));
    }
    None
}

fn http_header<'a>(request: &'a HttpRequest, name: &str) -> &'a str
{
    request
        .headers
        .get(&name.to_ascii_lowercase())
        .map_or("", String::as_str)
}

fn mcp_http_accept_allowed(accept: &str) -> bool
{
    accept.is_empty()
        || accept.contains("*/*")
        || accept.contains("application/json")
        || accept.contains("text/event-stream")
}

fn mcp_http_protocol_allowed(protocol: &str) -> bool
{
    protocol.is_empty()
        || matches!(protocol, MCP_PROTOCOL_VERSION | "2025-03-26" | "2025-06-18")
}

fn mcp_http_origin_allowed(origin: &str) -> bool
{
    origin.is_empty()
        || origin == "null"
        || origin.starts_with("http://127.0.0.1")
        || origin.starts_with("http://localhost")
        || origin.starts_with("https://127.0.0.1")
        || origin.starts_with("https://localhost")
}

fn http_response(status: i32, body: &str, headers: &[(&str, &str)]) -> String
{
    let status_text = match status
    {
        200 => "OK",
        202 => "Accepted",
        400 => "Bad Request",
        403 => "Forbidden",
        404 => "Not Found",
        405 => "Method Not Allowed",
        406 => "Not Acceptable",
        _ => "Error",
    };
    let mut response = format!(
        "HTTP/1.1 {status} {status_text}\r\nConnection: close\r\nContent-Length: {}\r\n",
        body.len(),
    );
    for (name, value) in headers
    {
        response.push_str(name);
        response.push_str(": ");
        response.push_str(value);
        response.push_str("\r\n");
    }
    response.push_str("\r\n");
    response.push_str(body);
    response
}

fn serve_lines<A: GeneratedApplication, R: BufRead, W: Write>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    reader: R,
    mut writer: W,
)
{
    for line in reader.lines()
    {
        let Ok(line) = line else { return };
        if line.trim().is_empty() { continue; }
        let request: Value = match serde_json::from_str(&line)
        {
            Ok(value) => value,
            Err(error) =>
            {
                write_json_value(
                    &mut writer,
                    &json_rpc_error(Value::Null, -32700, &error.to_string()),
                );
                continue;
            }
        };
        if let Some(response) = dispatch_json_rpc_value(app, state, options, config, &request)
        {
            write_json_value(&mut writer, &response);
        }
        crate::runtime::close_completed_modal(app, state, options);
        if crate::runtime::generated_application_should_close(app, options) { return; }
    }
}

fn write_json_value(writer: &mut impl Write, response: &Value)
{
    let _ = writeln!(writer, "{response}");
    let _ = writer.flush();
}

fn json_rpc_error(id: Value, code: i32, message: &str) -> Value
{
    json!({"jsonrpc": "2.0", "id": id, "error": {"code": code, "message": message}})
}

pub(crate) fn dispatch_json_rpc_value<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    request: &Value,
) -> Option<Value>
{
    dispatch_json_rpc_value_with_progress(
        app,
        state,
        options,
        config,
        request,
        &mut |_, _, _, _| {},
    )
}

pub(crate) fn dispatch_json_rpc_value_with_progress<A, F>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    request: &Value,
    progress: &mut F,
) -> Option<Value>
where
    A: GeneratedApplication,
    F: FnMut(
        &mut A,
        &mut RuntimeState,
        &GeneratedWindowRuntimeOptions,
        &McpRuntimeConfig,
    ),
{
    if let Some(batch) = request.as_array()
    {
        if batch.is_empty()
        {
            return Some(json_rpc_error(
                Value::Null,
                JSON_RPC_ERROR_CODE,
                "Invalid JSON-RPC batch: empty array",
            ));
        }
        let responses = batch
            .iter()
            .filter_map(|request|
            {
                if !request.is_object()
                {
                    return Some(json_rpc_error(
                        Value::Null,
                        JSON_RPC_ERROR_CODE,
                        "JSON-RPC request must be an object",
                    ));
                }
                dispatch_json_rpc_object(
                    app,
                    state,
                    options,
                    config,
                    request,
                    progress,
                )
            })
            .collect::<Vec<_>>();
        return (!responses.is_empty()).then_some(Value::Array(responses));
    }
    if !request.is_object()
    {
        return Some(json_rpc_error(
            Value::Null,
            JSON_RPC_ERROR_CODE,
            "JSON-RPC request must be an object",
        ));
    }
    dispatch_json_rpc_object(app, state, options, config, request, progress)
}

fn dispatch_json_rpc_object<A, F>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    request: &Value,
    progress: &mut F,
) -> Option<Value>
where
    A: GeneratedApplication,
    F: FnMut(
        &mut A,
        &mut RuntimeState,
        &GeneratedWindowRuntimeOptions,
        &McpRuntimeConfig,
    ),
{
    let id = request.get("id").cloned();
    let method = request.get("method").and_then(Value::as_str).unwrap_or_default();
    let params = request
        .get("params")
        .and_then(Value::as_object)
        .cloned()
        .unwrap_or_default();
    let requested_tool = if method == "tools/call"
    {
        params.get("name").and_then(Value::as_str).unwrap_or_default()
    }
    else
    {
        method.trim_start_matches("ui.")
    };
    if mcp_action_has_delay(resolve_tool_name(requested_tool))
    {
        sleep_mcp_delay(config.action_delay_ms);
    }
    match handle_request_with_progress(
        app,
        state,
        options,
        config,
        method,
        &params,
        progress,
    )
    {
        Ok(result) => id.map(|id| json!({"jsonrpc": "2.0", "id": id, "result": result})),
        Err(message) => Some(json_rpc_error(
            id.unwrap_or(Value::Null),
            JSON_RPC_ERROR_CODE,
            &message,
        )),
    }
}

fn sleep_mcp_delay(milliseconds: i32)
{
    if milliseconds > 0
    {
        std::thread::sleep(Duration::from_millis(milliseconds as u64));
    }
}

fn mcp_action_has_delay(name: &str) -> bool
{
    matches!(
        name,
        "activate_element"
            | "check"
            | "clear_text"
            | "click_element"
            | "cut_selection"
            | "enter_edit_mode"
            | "exit_edit_mode"
            | "focus_element"
            | "mouse_click"
            | "mouse_drag"
            | "mouse_move"
            | "mouse_press"
            | "mouse_release"
            | "move_cursor"
            | "paste_text"
            | "press_key"
            | "replace_selection"
            | "scroll"
            | "select_all"
            | "select_option"
            | "select_text"
            | "set_checked"
            | "set_cursor"
            | "set_selection"
            | "set_text"
            | "set_value"
            | "set_viewport"
            | "type_text"
            | "uncheck"
    )
}

#[cfg(test)]
fn handle_request<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    method: &str,
    params: &Map<String, Value>,
) -> Result<Value, String>
{
    handle_request_with_progress(
        app,
        state,
        options,
        config,
        method,
        params,
        &mut |_, _, _, _| {},
    )
}

fn handle_request_with_progress<A, F>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    method: &str,
    params: &Map<String, Value>,
    progress: &mut F,
) -> Result<Value, String>
where
    A: GeneratedApplication,
    F: FnMut(
        &mut A,
        &mut RuntimeState,
        &GeneratedWindowRuntimeOptions,
        &McpRuntimeConfig,
    ),
{
    match method
    {
        "initialize" => Ok(json!({
            "protocolVersion": MCP_PROTOCOL_VERSION,
            "serverInfo": {"name": "uimd-rust", "version": env!("CARGO_PKG_VERSION")},
            "capabilities": {"tools": {}}
        })),
        "tools/list" =>
        {
            let tools: Vec<Value> = tool_names(app)
                .into_iter()
                .map(|name|
                {
                    let resolved = resolve_tool_name(&name);
                    if let Some(tool) = app
                        .active_window()
                        .mcp_app_tools
                        .iter()
                        .find(|tool| tool.name == resolved)
                    {
                        let mut definition = json!({
                            "name": name,
                            "description": tool.description,
                            "inputSchema": tool.input_schema,
                        });
                        if let Some(output_schema) = &tool.output_schema
                        {
                            definition["outputSchema"] = output_schema.clone();
                        }
                        definition
                    }
                    else
                    {
                        json!({
                            "name": name,
                            "description": "",
                            "inputSchema": tool_input_schema(resolved),
                        })
                    }
                })
                .collect();
            Ok(json!({"tools": tools}))
        }
        "tools/call" =>
        {
            let name = params.get("name").and_then(Value::as_str).unwrap_or_default();
            let arguments = params.get("arguments").and_then(Value::as_object).cloned().unwrap_or_default();
            let value =
                call_tool_with_progress(
                    app,
                    state,
                    options,
                    config,
                    name,
                    &arguments,
                    progress,
                )?;
            Ok(json!({"content": [{"type": "text", "text": serde_json::to_string(&value).unwrap_or_default()}]}))
        }
        _ => call_tool_with_progress(
            app,
            state,
            options,
            config,
            method.trim_start_matches("ui."),
            params,
            progress,
        ),
    }
}

fn tool_names<A: GeneratedApplication>(app: &A) -> Vec<String>
{
    let mut names = [
        "activate_element", "check", "clear_text", "click_element", "copy_selection",
        "cut_selection", "enter_edit_mode", "exit_edit_mode", "focus_element",
        "get_accessibility_snapshot", "get_cursor", "get_edit_mode", "get_element",
        "get_elements", "get_focused_element", "get_image_render_info", "get_options",
        "get_render_cell", "get_render_frame", "get_render_rect", "get_render_snapshot",
        "get_render_snapshot_compact", "get_schema", "get_selection", "get_source",
        "get_state", "get_text", "get_text_snapshot", "get_value", "get_viewport",
        "get_window", "mouse_click", "mouse_drag", "mouse_move", "mouse_press",
        "mouse_release", "move_cursor", "paste_text", "press_key", "repaint",
        "replace_selection", "scroll", "select_all", "select_option", "select_text",
        "set_checked", "set_cursor", "set_selection", "set_text", "set_value",
        "set_viewport", "type_text", "ui.activate", "ui.check", "ui.clear",
        "ui.click", "ui.copy_selection", "ui.cut_selection", "ui.fill", "ui.focus",
        "ui.get_element", "ui.get_elements", "ui.get_focused_element", "ui.get_options",
        "ui.get_schema", "ui.get_source", "ui.get_value", "ui.get_window", "ui.press",
        "ui.render_snapshot", "ui.select_option", "ui.select_text", "ui.set_checked",
        "ui.set_cursor", "ui.set_selection", "ui.set_value", "ui.snapshot",
        "ui.text_snapshot", "ui.uncheck", "uncheck",
    ]
    .into_iter()
    .map(str::to_string)
    .collect::<Vec<_>>();
    names.extend(
        app.active_window()
            .mcp_app_tools
            .iter()
            .map(|tool| tool.name.clone()),
    );
    names.extend(app.mcp_tool_names());
    names.sort();
    names.dedup();
    names
}

fn resolve_tool_name(name: &str) -> &str
{
    match name
    {
        "ui.snapshot" | "snapshot" => "get_accessibility_snapshot",
        "ui.render_snapshot" | "render_snapshot" => "get_render_snapshot",
        "ui.text_snapshot" | "text_snapshot" => "get_text_snapshot",
        "ui.click" | "click" => "click_element",
        "ui.press" | "press" => "press_key",
        "ui.fill" | "fill" => "set_text",
        "ui.clear" | "clear" => "clear_text",
        "ui.focus" | "focus" => "focus_element",
        "ui.activate" | "activate" => "activate_element",
        "ui.check" => "check",
        "ui.uncheck" => "uncheck",
        "ui.set_selection" => "set_selection",
        "ui.set_value" => "set_value",
        "ui.get_options" => "get_options",
        "ui.get_value" => "get_value",
        "ui.select_option" => "select_option",
        "ui.select_text" => "select_text",
        "ui.copy_selection" => "copy_selection",
        "ui.cut_selection" => "cut_selection",
        "ui.set_cursor" => "set_cursor",
        "ui.set_checked" => "set_checked",
        "ui.get_window" => "get_window",
        "ui.get_elements" => "get_elements",
        "ui.get_element" => "get_element",
        "ui.get_focused_element" => "get_focused_element",
        "ui.get_schema" => "get_schema",
        "ui.get_source" => "get_source",
        other => other.strip_prefix("ui.").unwrap_or(other),
    }
}

fn tool_input_schema(name: &str) -> Value
{
    let mut properties = Map::new();
    let mut required = Vec::new();
    let mut add_element_id = ||
    {
        properties.insert("element_id".to_string(), json!({"type": "string"}));
        required.push(Value::String("element_id".to_string()));
    };
    if matches!(
        name,
        "get_element" | "get_value" | "get_options" | "focus_element"
            | "activate_element" | "click_element" | "clear_text" | "select_all"
            | "copy_selection" | "cut_selection" | "get_text" | "get_selection"
            | "get_cursor" | "get_image_render_info" | "check" | "uncheck"
    )
    {
        add_element_id();
    }
    else if name == "set_value"
    {
        add_element_id();
        properties.insert("value".to_string(), json!({
            "oneOf": [
                {"type": "string"},
                {"type": "number"},
                {"type": "boolean"},
                {"type": "array", "items": {"type": "string"}}
            ]
        }));
        required.push(Value::String("value".to_string()));
    }
    else if matches!(name, "set_text" | "paste_text" | "type_text" | "replace_selection")
    {
        add_element_id();
        properties.insert("text".to_string(), json!({"type": "string"}));
        required.push(Value::String("text".to_string()));
    }
    else if name == "set_checked"
    {
        add_element_id();
        properties.insert("checked".to_string(), json!({"type": "boolean"}));
        required.push(Value::String("checked".to_string()));
    }
    else if name == "select_option"
    {
        add_element_id();
        properties.insert("value".to_string(), json!({"type": "string"}));
        required.push(Value::String("value".to_string()));
    }
    else if name == "set_selection"
    {
        add_element_id();
        properties.insert(
            "values".to_string(),
            json!({"type": "array", "items": {"type": "string"}}),
        );
        required.push(Value::String("values".to_string()));
    }
    else if name == "press_key"
    {
        properties.insert("key".to_string(), json!({"type": "string"}));
        required.push(Value::String("key".to_string()));
    }
    else if name == "move_cursor"
    {
        add_element_id();
        properties.insert("direction".to_string(), json!({"type": "string"}));
        properties.insert("count".to_string(), json!({"type": "integer", "minimum": 1}));
        required.push(Value::String("direction".to_string()));
    }
    else if name == "set_cursor"
    {
        add_element_id();
        properties.insert("offset".to_string(), json!({"type": "integer", "minimum": 0}));
        required.push(Value::String("offset".to_string()));
    }
    else if name == "select_text"
    {
        add_element_id();
        properties.insert("start".to_string(), json!({"type": "integer", "minimum": 0}));
        properties.insert("end".to_string(), json!({"type": "integer", "minimum": 0}));
        required.extend([
            Value::String("start".to_string()),
            Value::String("end".to_string()),
        ]);
    }
    else if name == "set_viewport"
    {
        for field in ["row", "col", "width", "height"]
        {
            properties.insert(field.to_string(), json!({"type": "integer"}));
            required.push(Value::String(field.to_string()));
        }
    }
    else if matches!(name, "get_render_snapshot" | "get_render_snapshot_compact")
    {
        properties.insert("snapshot_time_ms".to_string(), json!({"type": "integer"}));
        properties.insert(
            "render_scope".to_string(),
            json!({"type": "string", "enum": ["full_surface", "active_window"]}),
        );
    }
    else if name == "get_render_cell"
    {
        properties.insert("x".to_string(), json!({"type": "integer"}));
        properties.insert("y".to_string(), json!({"type": "integer"}));
        properties.insert("snapshot_time_ms".to_string(), json!({"type": "integer"}));
        properties.insert(
            "render_scope".to_string(),
            json!({"type": "string", "enum": ["full_surface", "active_window"]}),
        );
        required.extend([
            Value::String("x".to_string()),
            Value::String("y".to_string()),
        ]);
    }
    else if name == "repaint"
    {
        properties.insert("full".to_string(), json!({"type": "boolean"}));
    }
    else if matches!(name, "mouse_click" | "mouse_press" | "mouse_move" | "mouse_release")
    {
        properties.insert("x".to_string(), json!({"type": "integer"}));
        properties.insert("y".to_string(), json!({"type": "integer"}));
        required.extend([
            Value::String("x".to_string()),
            Value::String("y".to_string()),
        ]);
    }
    else if name == "mouse_drag"
    {
        for field in ["from_x", "from_y", "to_x", "to_y"]
        {
            properties.insert(field.to_string(), json!({"type": "integer"}));
            required.push(Value::String(field.to_string()));
        }
    }
    else if name == "scroll"
    {
        add_element_id();
        properties.insert("delta".to_string(), json!({"type": "integer"}));
        required.push(Value::String("delta".to_string()));
    }
    let mut schema = json!({"type": "object", "properties": properties});
    if !required.is_empty()
    {
        schema["required"] = Value::Array(required);
    }
    schema
}

#[cfg(test)]
fn call_tool<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    raw_name: &str,
    arguments: &Map<String, Value>,
) -> Result<Value, String>
{
    call_tool_with_progress(
        app,
        state,
        options,
        config,
        raw_name,
        arguments,
        &mut |_, _, _, _| {},
    )
}

fn window_snapshot<A: GeneratedApplication>(app: &A, state: &RuntimeState) -> Value
{
    let window = app.active_window();
    let size = crate::runtime::active_window_reported_size(window, state.viewport);
    json!({
        "title": window.title,
        "class": window.metadata.class_name,
        "backend": "rust",
        "mode": crate::runtime::generated_window_mode_name(window),
        "description": window.metadata.description,
        "width": size.width,
        "height": size.height,
        "mcp_enabled": window.metadata.enabled,
    })
}

fn state_snapshot<A: GeneratedApplication>(app: &A, state: &RuntimeState) -> Value
{
    let elements = Value::Array(
        exposed_elements(app.active_window())
            .iter()
            .map(|element| snapshot_element(app, state, element))
            .collect(),
    );
    let focused = focused_exposed_snapshot(app, state);
    json!({
        "window": window_snapshot(app, state),
        "focused_element": focused,
        "edit_mode": state.edit_mode,
        "elements": elements,
    })
}

fn call_tool_with_progress<A, F>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &mut McpRuntimeConfig,
    raw_name: &str,
    arguments: &Map<String, Value>,
    progress: &mut F,
) -> Result<Value, String>
where
    A: GeneratedApplication,
    F: FnMut(
        &mut A,
        &mut RuntimeState,
        &GeneratedWindowRuntimeOptions,
        &McpRuntimeConfig,
    ),
{
    state.sync_active_window(app.window_mut(), options);
    let name = resolve_tool_name(raw_name);
    match name
    {
        "get_window" => Ok(window_snapshot(app, state)),
        "get_elements" =>
        {
            render_for_mcp(app, state, options, config);
            Ok(Value::Array(
                exposed_elements(app.active_window())
                    .iter()
                    .map(|element| snapshot_element(app, state, element))
                    .collect(),
            ))
        }
        "get_element" =>
        {
            render_for_mcp(app, state, options, config);
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            Ok(snapshot_element(app, state, &element))
        }
        "get_value" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            Ok(json!({"value": element_value(&element)}))
        }
        "get_options" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            if !matches!(
                element.borrow().kind(),
                ElementKind::ComboBox | ElementKind::ListBox
            )
            {
                return Err(format!(
                    "Element has no selectable options: {}",
                    active_element_id(app, &element),
                ));
            }
            Ok(json!({"options": element.borrow().options()}))
        }
        "get_focused_element" =>
        {
            render_for_mcp(app, state, options, config);
            Ok(focused_exposed_snapshot(app, state))
        }
        "get_edit_mode" => Ok(json!({"edit_mode": state.edit_mode})),
        "get_accessibility_snapshot" | "get_state" =>
        {
            render_for_mcp(app, state, options, config);
            Ok(state_snapshot(app, state))
        }
        "get_schema" =>
        {
            render_for_mcp(app, state, options, config);
            let window = app.active_window();
            let size = crate::runtime::active_window_reported_size(window, state.viewport);
            let elements = exposed_elements(app.active_window())
                .iter()
                .map(|element|
                {
                    let mut snapshot = snapshot_element(app, state, element);
                    snapshot["capabilities"] = element_capabilities(&element.borrow());
                    snapshot
                })
                .collect::<Vec<_>>();
            Ok(json!({
                "window": {
                    "title": window.title,
                    "class": window.metadata.class_name,
                    "backend": "rust",
                    "mode": crate::runtime::generated_window_mode_name(window),
                    "description": window.metadata.description,
                    "width": size.width,
                    "height": size.height,
                    "mcp_enabled": window.metadata.enabled,
                },
                "elements": elements,
            }))
        }
        "get_source" => Ok(json!({
            "class": app.active_window().metadata.class_name,
            "mcp_enabled": app.active_window().metadata.enabled,
            "source_markdown": app.active_window().metadata.source
        })),
        "get_viewport" | "get_render_rect" => Ok(viewport_json(app, config)),
        "set_viewport" =>
        {
            let viewport = Rect
            {
                row: int_argument(arguments, "row").unwrap_or_default().max(0),
                col: int_argument(arguments, "col").unwrap_or_default().max(0),
                width: int_argument(arguments, "width").unwrap_or(1).max(1),
                height: int_argument(arguments, "height").unwrap_or(1).max(1),
            };
            config.viewport = Some(viewport);
            state.viewport = Size { width: viewport.width, height: viewport.height };
            Ok(viewport_json(app, config))
        }
        "get_render_frame" =>
        {
            let buffer = crate::with_render_time_override(
                int64_argument(arguments, "snapshot_time_ms"),
                || render_for_mcp(app, state, options, config),
            );
            Ok(json!({
                "width": buffer.width,
                "height": buffer.height,
                "cells": compact_cells(&buffer),
            }))
        }
        "get_render_snapshot_compact" =>
        {
            let buffer = crate::with_render_time_override(
                int64_argument(arguments, "snapshot_time_ms"),
                || render_for_mcp_scope(app, state, options, config, arguments),
            )?;
            Ok(json!({
                "format": "render-cells-v1",
                "cells": compact_cells(&buffer),
            }))
        }
        "get_render_snapshot" =>
        {
            let buffer = crate::with_render_time_override(
                int64_argument(arguments, "snapshot_time_ms"),
                || render_for_mcp_scope(app, state, options, config, arguments),
            )?;
            let lines = buffer.plain_text()
                .split('\n')
                .map(str::to_string)
                .collect::<Vec<_>>();
            Ok(json!({
                "ansi_lines": lines,
                "text_lines": lines,
                "cells": snapshot_cells(&buffer),
            }))
        }
        "get_render_cell" =>
        {
            let buffer = crate::with_render_time_override(
                int64_argument(arguments, "snapshot_time_ms"),
                || render_for_mcp_scope(app, state, options, config, arguments),
            )?;
            let x = int_argument(arguments, "x").unwrap_or_default();
            let y = int_argument(arguments, "y").unwrap_or_default();
            let cell = buffer.cell(y, x).ok_or_else(|| "render cell is outside viewport".to_string())?;
            Ok(json!({"x": x, "y": y, "char": cell.text, "foreground": cell_color(&cell.foreground), "background": cell_color(&cell.background), "attributes": []}))
        }
        "get_text_snapshot" =>
        {
            if let Some(viewport) = config.viewport
            {
                state.viewport = Size { width: viewport.width, height: viewport.height };
            }
            let buffer = crate::runtime::render_active_generated_window(
                app,
                state,
                options,
            );
            Ok(json!({"lines": buffer.plain_text().split('\n').collect::<Vec<_>>()}))
        }
        "repaint" =>
        {
            render_for_mcp(app, state, options, config);
            Ok(json!({"ok": true}))
        }
        "focus_element" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            mcp_focus_element(app, state, options, &element);
            render_for_mcp(app, state, options, config);
            Ok(snapshot_element(app, state, &element))
        }
        "enter_edit_mode" =>
        {
            if let Some(id) = arguments.get("element_id").and_then(Value::as_str)
            {
                let element = require_element(app, id)?;
                if state.edit_mode
                    && let Some(current) = state.focused_element(app.active_window())
                {
                    state.commit_element_edit(&current);
                }
                mcp_focus_element(app, state, options, &element);
                enter_edit_element(app, state, options, element);
            }
            else if let Some(element) = state.focused_element(app.active_window())
            {
                enter_edit_element(app, state, options, element);
            }
            Ok(json!({"edit_mode": state.edit_mode}))
        }
        "exit_edit_mode" =>
        {
            state.exit_current_edit(app, options, true);
            Ok(json!({"edit_mode": state.edit_mode}))
        }
        "activate_element" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            if !element.borrow().enabled
            {
                return Ok(snapshot_element(app, state, &element));
            }
            let element_kind = element.borrow().kind();
            mcp_focus_element_with_scroll_scope(app, state, options, &element);
            match element_kind
            {
                ElementKind::Button | ElementKind::Image =>
                {
                    state.edit_mode = false;
                    state.scope_dim_element = None;
                    crate::runtime::dispatch_element_button(
                        app,
                        state,
                        options,
                        &element,
                    );
                }
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput =>
                {
                    state.edit_mode = true;
                    crate::runtime::dispatch_edit_started(
                        app,
                        options,
                        &active_element_id(app, &element),
                    );
                }
                ElementKind::CheckBox =>
                {
                    element.borrow_mut().handle_key("Enter");
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &active_element_id(app, &element),
                        if element.borrow().checked() { "True" } else { "False" },
                    );
                    state.edit_mode = false;
                    state.scope_dim_element = None;
                }
                ElementKind::ComboBox | ElementKind::ListBox =>
                {
                    element.borrow_mut().handle_key("Enter");
                }
                ElementKind::Reusable | ElementKind::ViewHost =>
                {
                    let activated = crate::runtime::activate_reusable_control(
                        app,
                        state,
                        options,
                        &element,
                    );
                    if activated
                    {
                        state.edit_mode = false;
                        state.scope_dim_element = None;
                    }
                    else if mcp_is_editable_element(&element)
                    {
                        state.edit_mode = true;
                        crate::runtime::dispatch_edit_started(
                            app,
                            options,
                            &active_element_id(app, &element),
                        );
                    }
                }
                _ =>
                {
                    state.handle_key(app, options, "Enter");
                }
            }
            render_for_mcp(app, state, options, config);
            Ok(snapshot_element(app, state, &element))
        }
        "click_element" =>
        {
            render_for_mcp(app, state, options, config);
            crate::runtime::sync_window_descendant_frames(app.active_window());
            let element_id = string_argument(arguments, "element_id");
            let element = require_element(app, element_id)?;
            if !element.borrow().enabled
            {
                return Ok(snapshot_element(app, state, &element));
            }
            if crate::runtime::activate_reusable_control(
                app,
                state,
                options,
                &element,
            )
            {
                if let Some(active_element) =
                    app.active_window().find_element(element_id)
                {
                    return Ok(snapshot_element(app, state, &active_element));
                }
                return Ok(json!({"ok": true, "element_id": element_id}));
            }
            let frame = element.borrow().frame;
            perform_mouse_press_at_active_point(
                app,
                state,
                options,
                Point
                {
                    row: frame.row + frame.height / 2,
                    col: frame.col + frame.width / 2,
                },
            );
            let active_element = app.active_window().find_element(element_id);
            if let Some(active_element) = active_element
                && matches!(
                    active_element.borrow().kind(),
                    ElementKind::Button | ElementKind::CheckBox | ElementKind::Image
                )
            {
                mcp_focus_element_with_scroll_scope(
                    app,
                    state,
                    options,
                    &active_element,
                );
                return Ok(snapshot_element(app, state, &active_element));
            }
            if let Some(active_element) = app.active_window().find_element(element_id)
            {
                return Ok(snapshot_element(app, state, &active_element));
            }
            Ok(json!({"ok": true, "element_id": element_id}))
        }
        "press_key" =>
        {
            let key = string_argument(arguments, "key");
            if key == "cmd_c"
            {
                if let Some(element) = state.focused_element(app.active_window())
                {
                    let text = focused_copy_text(&element);
                    if !text.is_empty()
                    {
                        let _ = crate::copy_text_to_clipboard(&text);
                    }
                }
            }
            else if key == "cmd_v"
            {
                if let Some(element) = state.focused_element(app.active_window())
                {
                    let text = crate::runtime_clipboard_text();
                    if !state.edit_mode && mcp_is_editable_element(&element)
                    {
                        state.enter_element_edit(&element);
                        let element_name = active_element_id(app, &element);
                        crate::runtime::dispatch_edit_started(
                            app,
                            options,
                            &element_name,
                        );
                    }
                    if matches!(
                        element.borrow().kind(),
                        ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
                    )
                    {
                        let element_name = active_element_id(app, &element);
                        if matches!(
                            element.borrow().kind(),
                            ElementKind::TextInput | ElementKind::TextArea
                        )
                        {
                            element.borrow_mut().insert_text(&text);
                        }
                        else
                        {
                            for character in text.chars()
                            {
                                element.borrow_mut().handle_key(&character.to_string());
                            }
                        }
                        let value = text_change_value(&element);
                        crate::runtime::dispatch_text_changed(
                            app,
                            options,
                            &element_name,
                            &value,
                        );
                    }
                }
            }
            else if key == "Escape" && state.begin_standard_escape(app, options)
            {
                if config.gui && app.window().modal_depth() > 0
                {
                    progress(app, state, options, config);
                    std::thread::sleep(crate::runtime::DIALOG_BUTTON_CLOSE_DURATION);
                }
                state.complete_pending_standard_escape_now(app, options);
                crate::runtime::close_completed_modal(app, state, options);
            }
            else
            {
                state.handle_key(app, options, key);
            }
            Ok(state_snapshot(app, state))
        }
        "paste_text" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_name = active_element_id(app, &element);
            let element_kind = element.borrow().kind();
            if !matches!(
                element_kind,
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
            {
                return Err(format!("element is not editable text: {element_name}"));
            }
            prepare_mcp_text_element(app, state, options, &element);
            let text = text_argument(arguments, "text");
            match element_kind
            {
                ElementKind::TextInput | ElementKind::TextArea =>
                {
                    element.borrow_mut().insert_text(&text);
                }
                ElementKind::NumberInput =>
                {
                    let number = if text.is_empty()
                    {
                        0.0
                    }
                    else
                    {
                        crate::elements::parse_number_like_stod(&text)
                            .ok_or_else(|| format!("invalid number: {text}"))?
                    };
                    element.borrow_mut().set_number_value(number);
                }
                _ => unreachable!("editable element kind validated above"),
            }
            let value = if element_kind == ElementKind::NumberInput
            {
                element.borrow().number_display_value()
            }
            else
            {
                element.borrow().value().to_string()
            };
            crate::runtime::dispatch_text_changed(
                app,
                options,
                &element_name,
                &value,
            );
            progress(app, state, options, config);
            Ok(snapshot_element(app, state, &element))
        }
        "type_text" | "replace_selection" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_name = active_element_id(app, &element);
            if !matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
            {
                return Err(format!("element is not editable text: {element_name}"));
            }
            prepare_mcp_text_element(app, state, options, &element);
            if element.borrow().kind() == ElementKind::NumberInput
            {
                element.borrow_mut().begin_number_edit();
            }
            let text = text_argument(arguments, "text");
            for ch in text.chars()
            {
                sleep_mcp_delay(config.type_delay_ms);
                state.handle_key(app, options, &ch.to_string());
                progress(app, state, options, config);
            }
            Ok(snapshot_element(app, state, &element))
        }
        "set_text" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_kind = element.borrow().kind();
            if !matches!(
                element_kind,
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
            {
                let element_name = active_element_id(app, &element);
                return Err(format!("element is not editable text: {element_name}"));
            }
            prepare_mcp_text_element(app, state, options, &element);
            let text = text_argument(arguments, "text");
            let element_name = active_element_id(app, &element);
            match element_kind
            {
                ElementKind::TextInput | ElementKind::TextArea =>
                {
                    element.borrow_mut().set_value("");
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        "",
                    );
                    progress(app, state, options, config);
                    for ch in text.chars()
                    {
                        sleep_mcp_delay(config.type_delay_ms);
                        state.handle_key(app, options, &ch.to_string());
                        progress(app, state, options, config);
                    }
                    let mut value = element.borrow_mut();
                    value.set_value(&text);
                    let cursor = value.value().len();
                    value.select_range(cursor, cursor);
                    drop(value);
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        &text,
                    );
                    progress(app, state, options, config);
                }
                ElementKind::NumberInput =>
                {
                    element.borrow_mut().set_number_edit_text("");
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        "",
                    );
                    progress(app, state, options, config);
                    for ch in text.chars()
                    {
                        sleep_mcp_delay(config.type_delay_ms);
                        state.handle_key(app, options, &ch.to_string());
                        progress(app, state, options, config);
                    }
                    let parsed = if text.is_empty()
                    {
                        0.0
                    }
                    else
                    {
                        crate::elements::parse_number_like_stod(&text)
                            .ok_or_else(|| format!("invalid number: {text}"))?
                    };
                    let mut value = element.borrow_mut();
                    value.commit_number_edit();
                    value.set_number_value(parsed);
                    value.set_number_edit_text(&text);
                    drop(value);
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        &element.borrow().number_display_value(),
                    );
                    progress(app, state, options, config);
                }
                _ => return Err(format!("element is not editable text: {element_name}")),
            }
            Ok(snapshot_element(app, state, &element))
        }
        "clear_text" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_name = active_element_id(app, &element);
            let element_kind = element.borrow().kind();
            if !matches!(
                element_kind,
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
            {
                return Err(format!("element is not editable text: {element_name}"));
            }
            prepare_mcp_text_element(app, state, options, &element);
            match element_kind
            {
                ElementKind::TextInput | ElementKind::TextArea =>
                {
                    let end = element.borrow().value().len();
                    element.borrow_mut().select_range(0, end);
                    progress(app, state, options, config);
                    sleep_mcp_delay(config.action_delay_ms);
                    state.handle_key(app, options, "Backspace");
                    progress(app, state, options, config);
                }
                ElementKind::NumberInput =>
                {
                    element.borrow_mut().set_number_edit_text("");
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        "",
                    );
                    progress(app, state, options, config);
                }
                _ => return Err(format!("element is not editable text: {element_name}")),
            }
            Ok(snapshot_element(app, state, &element))
        }
        "set_value" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_name = active_element_id(app, &element);
            let element_kind = element.borrow().kind();
            let value = arguments
                .get("value")
                .ok_or_else(|| format!("Missing value for: {element_name}"))?;
            match element_kind
            {
                ElementKind::TextInput | ElementKind::TextArea =>
                {
                    let text = json_text_value(value);
                    let mut element_value = element.borrow_mut();
                    element_value.set_value(&text);
                    let cursor = element_value.value().len();
                    element_value.select_range(cursor, cursor);
                    drop(element_value);
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        &text,
                    );
                }
                ElementKind::NumberInput =>
                {
                    let text = json_text_value(value);
                    let number = if text.is_empty()
                    {
                        0.0
                    }
                    else
                    {
                        crate::elements::parse_number_like_stod(&text)
                            .ok_or_else(|| format!("invalid number: {text}"))?
                    };
                    element.borrow_mut().set_number_value(number);
                    crate::runtime::dispatch_text_changed(
                        app,
                        options,
                        &element_name,
                        &element.borrow().number_display_value(),
                    );
                }
                ElementKind::CheckBox =>
                {
                    let checked = value.as_bool().ok_or_else(||
                    {
                        format!("Checkbox value must be boolean: {element_name}")
                    })?;
                    let changed = element.borrow().checked() != checked;
                    element.borrow_mut().set_checked(checked);
                    if changed
                    {
                        crate::runtime::dispatch_text_changed(
                            app,
                            options,
                            &element_name,
                            if checked { "True" } else { "False" },
                        );
                    }
                }
                ElementKind::ComboBox =>
                {
                    set_option_value(app, options, &element, &json_text_value(value))?;
                }
                ElementKind::ListBox if value.is_array() =>
                {
                    set_list_box_selection(app, options, &element, value)?;
                }
                ElementKind::ListBox =>
                {
                    set_option_value(app, options, &element, &json_text_value(value))?;
                }
                _ =>
                {
                    return Err(format!(
                        "Element value cannot be set directly: {element_name}",
                    ));
                }
            }
            Ok(snapshot_element(app, state, &element))
        }
        "set_checked" | "check" | "uncheck" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            if element.borrow().kind() != ElementKind::CheckBox
            {
                return Err("element is not a checkbox".to_string());
            }
            let element_name = active_element_id(app, &element);
            let checked = if name == "check" { true } else if name == "uncheck" { false } else { arguments.get("checked").and_then(Value::as_bool).unwrap_or(false) };
            let changed = element.borrow().checked() != checked;
            element.borrow_mut().set_checked(checked);
            if changed
            {
                crate::runtime::dispatch_text_changed(
                    app,
                    options,
                    &element_name,
                    if checked { "True" } else { "False" },
                );
                if let Some(active_element) =
                    app.active_window().find_element(&element_name)
                {
                    return Ok(snapshot_element(app, state, &active_element));
                }
                return Ok(json!({"ok": true, "element_id": element_name}));
            }
            Ok(snapshot_element(app, state, &element))
        }
        "set_cursor" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let mut element_value = element.borrow_mut();
            if !matches!(
                element_value.kind(),
                ElementKind::TextInput | ElementKind::TextArea
            )
            {
                return Ok(json!({"cursor": Value::Null}));
            }
            let cursor = int_argument(arguments, "offset")
                .unwrap_or_default()
                .max(0) as usize;
            element_value.select_range(cursor, cursor);
            Ok(json!({"cursor": element_value.cursor()}))
        }
        "move_cursor" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            prepare_mcp_text_element(app, state, options, &element);
            let key = match string_argument(arguments, "direction")
            {
                "left" | "Left" => "Left",
                "right" | "Right" => "Right",
                "up" | "Up" => "Up",
                "down" | "Down" => "Down",
                "start" | "home" | "Home" => "Home",
                "end" | "End" => "End",
                direction => return Err(format!("unsupported cursor direction: {direction}")),
            };
            let count = int_argument(arguments, "count").unwrap_or(1).max(1);
            for _ in 0..count
            {
                element.borrow_mut().handle_key(key);
            }
            let value = element.borrow();
            Ok(json!({
                "cursor": if matches!(
                    value.kind(),
                    ElementKind::TextInput | ElementKind::TextArea
                )
                {
                    json!(value.cursor())
                }
                else
                {
                    Value::Null
                }
            }))
        }
        "select_text" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            if !matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea
            )
            {
                return Ok(json!({
                    "start": Value::Null,
                    "end": Value::Null,
                    "text": ""
                }));
            }
            prepare_edit_element(app, state, options, &element);
            let length = element.borrow().value().len();
            let requested_start = int_argument(arguments, "start").unwrap_or_default();
            let requested_end = int_argument(arguments, "end").unwrap_or_default();
            let start = requested_start.max(0) as usize;
            let end = requested_end.max(0) as usize;
            element
                .borrow_mut()
                .select_range(min(start, length), min(end, length));
            Ok(json!({
                "start": min(requested_start, requested_end),
                "end": max(requested_start, requested_end),
                "text": selected_text(&element)
            }))
        }
        "select_all" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            if matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea
            )
            {
                let length = element.borrow().value().len();
                element.borrow_mut().select_range(0, length);
            }
            Ok(selection_snapshot(&element))
        }
        "copy_selection" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let selected = if matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea
            )
            {
                selected_text(&element)
            }
            else
            {
                String::new()
            };
            let _ = crate::copy_text_to_clipboard(&selected);
            Ok(json!({"text": selected}))
        }
        "cut_selection" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let selected = if matches!(
                element.borrow().kind(),
                ElementKind::TextInput | ElementKind::TextArea
            )
            {
                selected_text(&element)
            }
            else
            {
                String::new()
            };
            if !selected.is_empty()
            {
                element.borrow_mut().handle_key("Backspace");
                let name = active_element_id(app, &element);
                let value = element.borrow().value().to_string();
                crate::runtime::dispatch_text_changed(app, options, &name, &value);
            }
            Ok(json!({"text": selected}))
        }
        "get_text" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let value = element.borrow();
            let text = if matches!(
                value.kind(),
                ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
            )
            {
                if value.kind() == ElementKind::NumberInput
                {
                    value.number_display_value()
                }
                else
                {
                    value.value().to_string()
                }
            }
            else
            {
                String::new()
            };
            Ok(json!({"text": text}))
        }
        "get_selection" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            Ok(selection_snapshot(&element))
        }
        "get_cursor" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let value = element.borrow();
            if matches!(value.kind(), ElementKind::TextInput | ElementKind::TextArea)
            {
                Ok(json!({"cursor": value.cursor()}))
            }
            else
            {
                Ok(json!({"cursor": Value::Null}))
            }
        }
        "select_option" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let value = string_argument(arguments, "value");
            set_option_value(app, options, &element, value)?;
            Ok(snapshot_element(app, state, &element))
        }
        "set_selection" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let values = arguments.get("values").unwrap_or(&Value::Null);
            set_list_box_selection(app, options, &element, values)?;
            Ok(snapshot_element(app, state, &element))
        }
        "scroll" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let delta = int_argument(arguments, "delta").unwrap_or_default();
            scroll_element(&element, delta)?;
            Ok(snapshot_element(app, state, &element))
        }
        "mouse_click" | "mouse_press" | "mouse_release" | "mouse_drag" | "mouse_move" =>
        {
            let viewport = config.viewport.unwrap_or(Rect
            {
                row: 0,
                col: 0,
                width: state.viewport.width,
                height: state.viewport.height,
            });
            if name == "mouse_drag"
            {
                let from_x =
                    int_argument(arguments, "from_x").unwrap_or_default() - viewport.col;
                let from_y =
                    int_argument(arguments, "from_y").unwrap_or_default() - viewport.row;
                let to_x =
                    int_argument(arguments, "to_x").unwrap_or_default() - viewport.col;
                let to_y =
                    int_argument(arguments, "to_y").unwrap_or_default() - viewport.row;
                perform_mouse_press(
                    app,
                    state,
                    options,
                    Size
                    {
                        width: viewport.width,
                        height: viewport.height,
                    },
                    from_x,
                    from_y,
                );
                perform_mouse_move(app, state, to_x, to_y);
                if let Some(selected) =
                    perform_mouse_release(app, state, options, to_x, to_y)
                {
                    let _ = crate::copy_text_to_clipboard(&selected);
                }
            }
            else
            {
                let x = int_argument(arguments, "x").unwrap_or_default() - viewport.col;
                let y = int_argument(arguments, "y").unwrap_or_default() - viewport.row;
                match name
                {
                    "mouse_click" =>
                    {
                        perform_mouse_press(
                            app,
                            state,
                            options,
                            Size
                            {
                                width: viewport.width,
                                height: viewport.height,
                            },
                            x,
                            y,
                        );
                        if let Some(selected) =
                            perform_mouse_release(app, state, options, x, y)
                        {
                            let _ = crate::copy_text_to_clipboard(&selected);
                        }
                    }
                    "mouse_press" =>
                    {
                        perform_mouse_press(
                            app,
                            state,
                            options,
                            Size
                            {
                                width: viewport.width,
                                height: viewport.height,
                            },
                            x,
                            y,
                        );
                    }
                    "mouse_move" => perform_mouse_move(app, state, x, y),
                    "mouse_release" =>
                    {
                        if let Some(selected) =
                            perform_mouse_release(app, state, options, x, y)
                        {
                            let _ = crate::copy_text_to_clipboard(&selected);
                        }
                    }
                    _ => {}
                }
            }
            Ok(state_snapshot(app, state))
        }
        "get_image_render_info" =>
        {
            let element = require_element(app, string_argument(arguments, "element_id"))?;
            let element_id = active_element_id(app, &element);
            if element.borrow().kind() != crate::ElementKind::Image
            {
                return Err(format!("element is not an image: {element_id}"));
            }
            let frame_restore = ElementFrameRestore::capture(app.active_window());
            render_for_mcp(app, state, options, config);
            let bounds = element.borrow().frame;
            drop(frame_restore);
            let focused = state
                .focused_element(app.active_window())
                .is_some_and(|focused| std::rc::Rc::ptr_eq(&focused, &element));
            let element = element.borrow();
            let info = crate::image_render_info(
                &element,
                crate::Size
                {
                    width: bounds.width.max(crate::MINIMUM_RENDERABLE_SIZE),
                    height: bounds.height.max(crate::MINIMUM_RENDERABLE_SIZE),
                },
                crate::ElementRenderState
                {
                    focused,
                    edit_mode: focused && state.edit_mode,
                    ..crate::ElementRenderState::default()
                },
            );
            let image_right = info.image_left + info.image_width;
            let image_bottom = info.image_top + info.image_height;
            let visible_right = info.visible_left + info.visible_width;
            let visible_bottom = info.visible_top + info.visible_height;
            Ok(json!({
                "id": element_id,
                "source": info.source,
                "fit": info.fit,
                "configured_render_mode": info.configured_render_mode,
                "resolved_render_mode": info.resolved_render_mode,
                "source_loaded": info.source_loaded,
                "source_width": info.source_width,
                "source_height": info.source_height,
                "element_width": info.element_width,
                "element_height": info.element_height,
                "cell_pixel_width": info.cell_pixel_width,
                "cell_pixel_height": info.cell_pixel_height,
                "image_left": info.image_left,
                "image_top": info.image_top,
                "image_width": info.image_width,
                "image_height": info.image_height,
                "image_right": image_right,
                "image_bottom": image_bottom,
                "visible_left": info.visible_left,
                "visible_top": info.visible_top,
                "visible_width": info.visible_width,
                "visible_height": info.visible_height,
                "visible_right": visible_right,
                "visible_bottom": visible_bottom,
                "raw_expected": info.raw_expected,
                "raw_present": info.raw_present,
                "sample_signature": info.sample_signature,
                "bounds": rect_json(bounds),
                "absolute_image_left": bounds.col + info.image_left,
                "absolute_image_top": bounds.row + info.image_top,
                "absolute_image_right": bounds.col + image_right,
                "absolute_image_bottom": bounds.row + image_bottom,
                "absolute_visible_left": bounds.col + info.visible_left,
                "absolute_visible_top": bounds.row + info.visible_top,
                "absolute_visible_right": bounds.col + visible_right,
                "absolute_visible_bottom": bounds.row + visible_bottom
            }))
        }
        _ =>
        {
            if let Some(result) = app.handle_mcp_tool(name, arguments)
            {
                return Ok(result);
            }
            Err(format!("unknown tool: {name}"))
        }
    }
}

struct ElementFrameRestore
{
    frames: Vec<(ElementRef, Rect)>,
}

impl ElementFrameRestore
{
    fn capture(window: &crate::GeneratedWindow) -> Self
    {
        fn append(element: &ElementRef, frames: &mut Vec<(ElementRef, Rect)>)
        {
            let (frame, children) =
            {
                let value = element.borrow();
                (value.frame, value.children().to_vec())
            };
            frames.push((element.clone(), frame));
            for child in children
            {
                append(&child, frames);
            }
            if let Some(child_window) = element.borrow().child_window()
            {
                for child in &child_window.elements
                {
                    append(child, frames);
                }
            }
        }

        let mut frames = Vec::new();
        for element in &window.elements
        {
            append(element, &mut frames);
        }
        Self { frames }
    }
}

impl Drop for ElementFrameRestore
{
    fn drop(&mut self)
    {
        for (element, frame) in &self.frames
        {
            element.borrow_mut().frame = *frame;
        }
    }
}

fn render_for_mcp<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &McpRuntimeConfig,
) -> crate::TerminalBuffer
{
    if let Some(viewport) = config.viewport
    {
        state.viewport = Size { width: viewport.width, height: viewport.height };
    }
    crate::render_generated_application(app, state, options)
}

fn render_for_mcp_scope<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    config: &McpRuntimeConfig,
    arguments: &Map<String, Value>,
) -> Result<crate::TerminalBuffer, String>
{
    if let Some(viewport) = config.viewport
    {
        state.viewport = Size { width: viewport.width, height: viewport.height };
    }
    match arguments
        .get("render_scope")
        .and_then(Value::as_str)
        .unwrap_or("full_surface")
    {
        "" | "full_surface" =>
            Ok(crate::render_generated_application(app, state, options)),
        "active_window" =>
            Ok(crate::runtime::render_active_generated_window(app, state, options)),
        scope => Err(format!("unknown render_scope: {scope}")),
    }
}

pub(crate) fn perform_mouse_press<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    viewport: Size,
    x: i32,
    y: i32,
)
{
    state.viewport = viewport;
    crate::render_generated_application(app, state, options);
    let point = crate::runtime::active_window_point(
        app.window(),
        Point { row: y, col: x },
    );
    perform_mouse_press_at_active_point(app, state, options, point);
}

fn perform_mouse_press_at_active_point<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    point: Point,
)
{
    state.mouse_selection_element = None;
    state.mouse_selection_anchor = 0;
    state.mouse_click_candidate = None;
    state.mouse_click_candidate_moved = false;
    state.scroll_selection = crate::runtime::ScrollSelection::default();
    if crate::runtime::dispatch_mouse_press_before_focused(
        app,
        options,
        point,
    )
    {
        return;
    }
    crate::runtime::clear_label_selections(app.active_window_mut());
    let x = point.col;
    let y = point.row;
    if activate_combo_box_option_at(app, state, options, x, y)
    {
        return;
    }

    let mouse_scroll_view = state
        .scope_edit_element
        .clone()
        .or_else(|| scroll_view_at_position(app.active_window(), x, y));
    let mouse_targets = mouse_target_elements(
        app.active_window(),
        mouse_scroll_view.as_ref(),
    );
    let target = mouse_target_element(&mouse_targets, x, y);
    let selection_target = target
        .is_none()
        .then(|| element_at(app, x, y))
        .flatten();
    if let Some(scroll_view) = selection_target.as_ref().and_then(|target|
    {
        (!crate::runtime::is_focusable_type(&target.borrow().kind()))
            .then(|| containing_selection_scroll_view(app.active_window(), target))
            .flatten()
    })
    {
        let point = clamped_point_in_frame(
            crate::Point { row: y, col: x },
            scroll_view.borrow().frame,
        );
        state.scroll_selection = crate::runtime::ScrollSelection
        {
            element: Some(scroll_view),
            anchor: point,
            current: point,
            active: true,
            changed: false,
        };
        return;
    }
    if let Some(label) = selection_target.as_ref().filter(|element|
    {
        let value = element.borrow();
        matches!(
            value.kind(),
            ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel
        )
            && value.style.user_select.as_deref() == Some("text")
    })
    {
        let frame = label.borrow().frame;
        let cursor = label.borrow().cursor_for_point(y - frame.row, x - frame.col);
        label.borrow_mut().select_range(cursor, cursor);
        state.mouse_selection_anchor = cursor;
        state.mouse_selection_element = Some(label.clone());
        return;
    }
    let current = state.focused_element(app.active_window());
    let changes_target = match (&current, &target)
    {
        (Some(current), Some(target)) => !std::rc::Rc::ptr_eq(current, target),
        (Some(_), None) => true,
        _ => false,
    };
    if state.edit_mode && changes_target
    {
        state.exit_current_edit(app, options, true);
    }
    let Some(element) = target else
    {
        state.focused_index = -1;
        return;
    };
    if !element.borrow().enabled
    {
        return;
    }
    let already_editing_target = state.edit_mode
        && current
            .as_ref()
            .is_some_and(|current| std::rc::Rc::ptr_eq(current, &element))
        && (state.scope_edit_element.is_none()
            || state
                .scope_dim_element
                .as_ref()
                .is_some_and(|editing| std::rc::Rc::ptr_eq(editing, &element)));
    focus_element(app, state, options, &element);
    if let Some(scroll_view) = mouse_scroll_view.as_ref().filter(|scroll_view|
    {
        !std::rc::Rc::ptr_eq(scroll_view, &element)
    })
    {
        state.scope_edit_element = Some(scroll_view.clone());
        state.edit_mode = true;
        state.remember_scope_descendant(scroll_view, &element);
    }
    let (kind, frame) =
    {
        let element_value = element.borrow();
        (element_value.kind(), element_value.frame)
    };
    if kind == ElementKind::ScrollView
    {
        let point = clamped_point_in_frame(
            crate::Point { row: y, col: x },
            frame,
        );
        state.scroll_selection = crate::runtime::ScrollSelection
        {
            element: Some(element),
            anchor: point,
            current: point,
            active: true,
            changed: false,
        };
        return;
    }
    if kind == ElementKind::Button
    {
        crate::runtime::dispatch_element_button(
            app,
            state,
            options,
            &element,
        );
        return;
    }
    if kind == ElementKind::Image
    {
        state.mouse_click_candidate = Some(element);
        state.mouse_click_candidate_moved = false;
        return;
    }
    if matches!(kind, ElementKind::Reusable | ElementKind::ViewHost)
    {
        let _ = crate::runtime::activate_reusable_control(
            app,
            state,
            options,
            &element,
        );
        return;
    }
    if kind == ElementKind::CheckBox
    {
        let before = crate::runtime::element_value(&element);
        element.borrow_mut().handle_key("Enter");
        crate::runtime::dispatch_change_if_needed(
            app,
            state,
            options,
            &element,
            before,
        );
        return;
    }
    if matches!(
        kind,
        ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput
    )
    {
        if !already_editing_target
        {
            enter_edit_element(app, state, options, element.clone());
        }
        else if state.scope_edit_element.is_some()
        {
            state.edit_mode = true;
            state.scope_dim_element = Some(element.clone());
        }
        let mut element_value = element.borrow_mut();
        if kind == ElementKind::NumberInput
        {
            let select_zero = element_value.number_value() == 0.0;
            element_value.set_number_edit_cursor(
                (x - frame.col).max(0) as usize,
                select_zero,
            );
            return;
        }
        let cursor = element_value.cursor_for_point(y - frame.row, x - frame.col);
        element_value.select_range(cursor, cursor);
        state.mouse_selection_anchor = element_value.cursor();
        drop(element_value);
        state.mouse_selection_element = Some(element);
        return;
    }
    if kind == ElementKind::ComboBox && already_editing_target
    {
        return;
    }
    if kind == ElementKind::ListBox
    {
        let local_row = y - frame.row;
        let option_index =
            element.borrow().list_scroll_offset() + local_row;
        let option_count = element.borrow().options().len() as i32;
        if option_index < 0 || option_index >= option_count
        {
            return;
        }
        let before = crate::runtime::element_value(&element);
        {
            let mut value = element.borrow_mut();
            if value.multiple()
            {
                let mut indices = value.selected_indices().to_vec();
                let option_index = option_index as usize;
                if let Some(index) =
                    indices.iter().position(|value| *value == option_index)
                {
                    indices.remove(index);
                }
                else
                {
                    indices.push(option_index);
                }
                value.set_selected_indices(indices);
            }
            else
            {
                value.set_selected_index(option_index);
            }
            value.set_active_index(option_index);
            value.hide_active_item();
        }
        enter_edit_element(app, state, options, element.clone());
        crate::runtime::dispatch_change_if_needed(
            app,
            state,
            options,
            &element,
            before,
        );
        return;
    }

    if state.scope_edit_element.is_some()
    {
        state.edit_mode = true;
    }
    if !already_editing_target
    {
        state.handle_key(app, options, "Enter");
    }
}

pub(crate) fn perform_mouse_move<A: GeneratedApplication>(
    app: &A,
    state: &mut RuntimeState,
    x: i32,
    y: i32,
)
{
    let point = crate::runtime::active_window_point(
        app.window(),
        Point { row: y, col: x },
    );
    let x = point.col;
    let y = point.row;
    if state.mouse_click_candidate.is_some()
    {
        state.mouse_click_candidate_moved = true;
        return;
    }
    if let Some(scroll_view) = state
        .scroll_selection
        .element
        .as_ref()
        .filter(|_| state.scroll_selection.active)
    {
        let current = clamped_point_in_frame(
            crate::Point { row: y, col: x },
            scroll_view.borrow().frame,
        );
        state.scroll_selection.changed = state.scroll_selection.changed
            || current.row != state.scroll_selection.anchor.row
            || current.col != state.scroll_selection.anchor.col;
        state.scroll_selection.current = current;
        return;
    }
    let Some(element) = state.mouse_selection_element.clone() else { return };
    let frame = element.borrow().frame;
    let mut local_row = y - frame.row;
    {
        let mut value = element.borrow_mut();
        if value.kind() == ElementKind::TextArea && frame.height > 1
        {
            if local_row < 0
            {
                value.scroll_text_rows(-1, frame.height, false);
                local_row = 0;
            }
            else if local_row >= frame.height
            {
                value.scroll_text_rows(1, frame.height, false);
                local_row = frame.height - 1;
            }
        }
    }
    let cursor = element.borrow().cursor_for_point(local_row, x - frame.col);
    element
        .borrow_mut()
        .select_range(state.mouse_selection_anchor, cursor);
}

pub(crate) fn perform_mouse_release<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    x: i32,
    y: i32,
) -> Option<String>
{
    let point = crate::runtime::active_window_point(
        app.window(),
        Point { row: y, col: x },
    );
    let x = point.col;
    let y = point.row;
    if let Some(candidate) = state.mouse_click_candidate.take()
    {
        let moved = std::mem::take(&mut state.mouse_click_candidate_moved);
        let contains = candidate
            .borrow()
            .frame
            .contains(crate::Point { row: y, col: x });
        if !moved
            && contains
            && app.active_window().element_id(&candidate).is_some()
        {
            crate::runtime::dispatch_element_button(
                app,
                state,
                options,
                &candidate,
            );
        }
        return None;
    }
    if state.scroll_selection.element.is_some() && state.scroll_selection.active
    {
        let rendered = crate::render_generated_application(app, state, options);
        let selected = crate::runtime::selected_scroll_text(
            &rendered,
            &state.scroll_selection,
        );
        if state.scroll_selection.changed
        {
            state.scroll_selection.active = false;
        }
        else
        {
            state.scroll_selection = crate::runtime::ScrollSelection::default();
        }
        return (!selected.is_empty()).then_some(selected);
    }
    let mut selection = None;
    if let Some(element) = state.mouse_selection_element.take()
    {
        let selected = selected_text(&element);
        if !selected.is_empty()
        {
            selection = Some(selected);
        }
    }
    state.mouse_selection_anchor = 0;
    selection
}

pub(crate) fn perform_mouse_wheel<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    viewport: Size,
    x: i32,
    y: i32,
    delta: i32,
) -> bool
{
    state.viewport = viewport;
    crate::render_generated_application(app, state, options);
    let point = crate::runtime::active_window_point(
        app.window(),
        Point { row: y, col: x },
    );
    if crate::runtime::dispatch_mouse_wheel_before_focused(
        app,
        options,
        point,
        delta,
    )
    {
        return true;
    }
    let Some(target) = element_at(app, point.col, point.row) else { return false };
    if scroll_element(&target, delta).is_ok()
    {
        return true;
    }
    if let Some(scope) = crate::containing_scroll_proxy(app.active_window(), &target)
        && scroll_element(&scope, delta).is_ok()
    {
        return true;
    }
    if let Some(parent) = containing_element_scroll_view(&target)
        && scroll_element(&parent, delta).is_ok()
    {
        return true;
    }
    let name = active_element_id(app, &target);
    crate::runtime::dispatch_mouse_wheel(app, options, &name, delta)
}

fn focus_element<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    target: &ElementRef,
)
{
    let previous = state.focused_element(app.active_window());
    let focusable = focusable_elements(app.active_window());
    if focusable.iter().any(|element| std::rc::Rc::ptr_eq(element, target))
    {
        state.scope_dim_element = None;
        state.scope_edit_element = None;
        state.focus_element(app.active_window(), target);
    }
    else if let Some(scope) = crate::containing_scroll_proxy(app.active_window(), target)
        .or_else(|| containing_element_scroll_view(target))
    {
        state.scope_dim_element = None;
        state.scope_edit_element = Some(scope.clone());
        state.edit_mode = false;
        state.focus_element(app.active_window(), target);
        state.remember_scope_descendant(&scope, target);
        crate::runtime::ensure_focused_visible_in_scroll(&scope, target);
    }
    else
    {
        state.scope_dim_element = None;
        state.scope_edit_element = None;
        state.focus_element(app.active_window(), target);
    }
    let focused = state.focused_element(app.active_window());
    if previous
        .as_ref()
        .zip(focused.as_ref())
        .is_none_or(|(previous, focused)| !std::rc::Rc::ptr_eq(previous, focused))
    {
        if let Some(previous) = previous
        {
            let previous_id = active_element_id(app, &previous);
            crate::runtime::dispatch_focus_changed(
                app,
                options,
                &previous_id,
                false,
            );
        }
        if let Some(focused) = focused
        {
            let focused_id = active_element_id(app, &focused);
            crate::runtime::dispatch_focus_changed(
                app,
                options,
                &focused_id,
                true,
            );
        }
    }
}

fn mcp_focus_element<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    target: &ElementRef,
)
{
    let previous = state.focused_element(app.active_window());
    if state.scope_edit_element.is_some() && state.edit_mode
    {
        if let Some(editing) = state.scope_dim_element.clone()
        {
            state.commit_element_edit(&editing);
            let editing_id = active_element_id(app, &editing);
            let value = text_change_value(&editing);
            crate::runtime::dispatch_text_changed(
                app,
                options,
                &editing_id,
                &value,
            );
        }
        else
        {
            state.leave_scope_navigation(app.active_window());
        }
        state.scope_dim_element = None;
    }

    let containing_scope = crate::containing_scroll_proxy(app.active_window(), target)
        .or_else(|| containing_element_scroll_view(target));
    state.scope_dim_element = None;
    state.scope_edit_element = containing_scope.clone();
    if containing_scope.is_some()
    {
        state.edit_mode = false;
    }
    let focused = if state.focus_element(app.active_window(), target)
    {
        if let Some(scope) = containing_scope
        {
            state.remember_scope_descendant(&scope, target);
            crate::runtime::ensure_focused_visible_in_scroll(&scope, target);
        }
        state.focused_element(app.active_window())
    }
    else
    {
        state.focused_index = -1;
        state.edit_mode = false;
        None
    };
    dispatch_mcp_focus_change(app, options, previous, focused);
}

fn mcp_focus_element_with_scroll_scope<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    target: &ElementRef,
)
{
    let containing_scope = crate::containing_scroll_proxy(app.active_window(), target)
        .or_else(|| containing_element_scroll_view(target));
    let Some(scope) = containing_scope else
    {
        mcp_focus_element(app, state, options, target);
        return;
    };
    state.scope_edit_element = Some(scope.clone());
    state.scope_dim_element = None;
    state.edit_mode = true;
    if !state.focus_element(app.active_window(), target)
    {
        state.focused_index = -1;
        return;
    }
    state.remember_scope_descendant(&scope, target);
    crate::runtime::ensure_focused_visible_in_scroll(&scope, target);
}

fn dispatch_mcp_focus_change<A: GeneratedApplication>(
    app: &mut A,
    options: &GeneratedWindowRuntimeOptions,
    previous: Option<ElementRef>,
    focused: Option<ElementRef>,
)
{
    if previous
        .as_ref()
        .zip(focused.as_ref())
        .is_some_and(|(previous, focused)| std::rc::Rc::ptr_eq(previous, focused))
    {
        return;
    }
    if let Some(previous) = previous
    {
        let previous_id = active_element_id(app, &previous);
        crate::runtime::dispatch_focus_changed(
            app,
            options,
            &previous_id,
            false,
        );
    }
    if let Some(focused) = focused
    {
        let focused_id = active_element_id(app, &focused);
        crate::runtime::dispatch_focus_changed(
            app,
            options,
            &focused_id,
            true,
        );
    }
}

fn enter_edit_element<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    element: ElementRef,
)
{
    if let Some(scope) = state.scope_edit_element.clone()
    {
        if std::rc::Rc::ptr_eq(&scope, &element)
        {
            state.edit_mode = false;
            state.handle_key(app, options, "Enter");
        }
        else
        {
            state.enter_element_edit(&element);
            state.scope_dim_element = Some(element);
            if let Some(editing) = state.scope_dim_element.as_ref()
            {
                let editing_id = active_element_id(app, editing);
                crate::runtime::dispatch_edit_started(
                    app,
                    options,
                    &editing_id,
                );
            }
        }
    }
    else
    {
        let is_generated_scroll_view = element
            .borrow()
            .child_window()
            .is_some_and(crate::GeneratedWindow::is_generated_scroll_view);
        if is_generated_scroll_view
        {
            state.edit_mode = false;
            state.handle_key(app, options, "Enter");
        }
        else
        {
            state.enter_element_edit(&element);
            let element_id = active_element_id(app, &element);
            crate::runtime::dispatch_edit_started(
                app,
                options,
                &element_id,
            );
        }
    }
}

fn prepare_edit_element<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    target: &ElementRef,
)
{
    mcp_focus_element(app, state, options, target);
    if state.scope_edit_element.is_some()
    {
        state.enter_element_edit(target);
        state.scope_dim_element = Some(target.clone());
        let target_id = active_element_id(app, target);
        crate::runtime::dispatch_edit_started(
            app,
            options,
            &target_id,
        );
    }
    else if !state.edit_mode
    {
        state.handle_key(app, options, "Enter");
    }
}

fn prepare_mcp_text_element<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    target: &ElementRef,
)
{
    mcp_focus_element(app, state, options, target);
    state.edit_mode = true;
    let target_id = active_element_id(app, target);
    crate::runtime::dispatch_edit_started(
        app,
        options,
        &target_id,
    );
}

fn require_element<A: GeneratedApplication>(app: &A, id: &str) -> Result<ElementRef, String>
{
    app.active_window()
        .find_element(id)
        .or_else(|| app.active_window().find_element(id.trim_start_matches("main.")))
        .ok_or_else(|| format!("unknown element: {id}"))
}

fn active_element_id<A: GeneratedApplication>(app: &A, element: &ElementRef) -> String
{
    app.active_window()
        .element_id(element)
        .unwrap_or_else(|| element.borrow().name.clone())
}

fn scroll_element(element: &ElementRef, delta: i32) -> Result<(), String>
{
    let mut value = element.borrow_mut();
    let frame = value.frame;
    if value
        .child_window()
        .is_some_and(crate::GeneratedWindow::is_generated_scroll_view)
    {
        if let Some(child) = value.child_window_mut()
        {
            child.scroll_lines(delta, Size
            {
                width: frame.width,
                height: frame.height,
            });
            child.consume_terminal_scroll_delta();
        }
        return Ok(());
    }

    if value.kind() == ElementKind::ScrollView
    {
        value.scroll_lines(
            delta,
            Size
            {
                width: frame.width,
                height: frame.height,
            },
        );
        value.consume_terminal_scroll_delta();
        return Ok(());
    }

    if !matches!(value.kind(), ElementKind::ListBox | ElementKind::TextArea)
    {
        return Err(format!("element is not scrollable: {}", value.name));
    }
    let steps = max(1, delta.abs());
    let scroll_delta = if delta < 0 { 1 } else { -1 };
    for _ in 0..steps
    {
        value.scroll_by(scroll_delta);
    }
    Ok(())
}

fn containing_element_scroll_view(element: &ElementRef) -> Option<ElementRef>
{
    let mut parent = element.borrow().parent();
    while let Some(candidate) = parent
    {
        if candidate.borrow().kind() == ElementKind::ScrollView
        {
            return Some(candidate);
        }
        parent = candidate.borrow().parent();
    }
    None
}

fn is_scroll_view_element(element: &ElementRef) -> bool
{
    let value = element.borrow();
    value.kind() == ElementKind::ScrollView
        || value
            .child_window()
            .is_some_and(crate::GeneratedWindow::is_generated_scroll_view)
}

fn containing_selection_scroll_view(
    window: &crate::GeneratedWindow,
    target: &ElementRef,
) -> Option<ElementRef>
{
    if is_scroll_view_element(target)
    {
        return Some(target.clone());
    }
    containing_element_scroll_view(target)
        .or_else(|| crate::runtime::containing_scroll_proxy(window, target))
}

fn clamped_point_in_frame(point: crate::Point, frame: Rect) -> crate::Point
{
    crate::Point
    {
        row: point.row.clamp(frame.row, frame.row + max(0, frame.height - 1)),
        col: point.col.clamp(frame.col, frame.col + max(0, frame.width - 1)),
    }
}

fn element_at<A: GeneratedApplication>(app: &A, x: i32, y: i32) -> Option<ElementRef>
{
    element_at_window(app.active_window(), x, y)
}

fn mouse_target_elements(
    window: &crate::GeneratedWindow,
    active_scroll_view: Option<&ElementRef>,
) -> Vec<ElementRef>
{
    let mut elements = Vec::new();
    for element in &window.elements
    {
        crate::runtime::sync_reusable_child_frames(element);
        let value = element.borrow();
        if matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
            && let Some(child) = value.child_window()
        {
            if child.is_generated_scroll_view()
            {
                if value.enabled && value.focusable
                {
                    elements.push(element.clone());
                }
                let active = active_scroll_view
                    .is_some_and(|active| std::rc::Rc::ptr_eq(active, element));
                let generated_scroll_view = child.generated_scroll_view();
                if active
                {
                    elements.extend(
                        mouse_target_elements(
                            child,
                            generated_scroll_view.as_deref(),
                        )
                            .into_iter()
                            .filter(|target|
                            {
                                generated_scroll_view.as_ref().is_none_or(
                                    |scroll_view|
                                    {
                                        !std::rc::Rc::ptr_eq(
                                            scroll_view,
                                            target,
                                        )
                                    },
                                )
                            }),
                    );
                }
                continue;
            }
            let child_targets =
                mouse_target_elements(child, active_scroll_view);
            if child_targets.is_empty()
                && value.focusable
                && child.generated_focusable()
                && value.kind() != ElementKind::ViewHost
            {
                elements.push(element.clone());
            }
            else
            {
                elements.extend(child_targets);
            }
            continue;
        }
        if value.kind() == ElementKind::ScrollView
        {
            elements.push(element.clone());
            let active = active_scroll_view
                .is_some_and(|active| std::rc::Rc::ptr_eq(active, element));
            if active
            {
                let frame = value.frame;
                let views = value.child_views(Size
                {
                    width: frame.width,
                    height: frame.height,
                });
                drop(value);
                for view in views
                {
                    let Some(child) = view.element else { continue };
                    child.borrow_mut().frame = Rect
                    {
                        row: frame.row + view.frame.row,
                        col: frame.col + view.frame.col,
                        width: view.frame.width,
                        height: view.frame.height,
                    };
                    crate::runtime::sync_reusable_child_frames(&child);
                    let child_value = child.borrow();
                    if let Some(window) = child_value.child_window()
                    {
                        elements.extend(mouse_target_elements(
                            window,
                            active_scroll_view,
                        ));
                    }
                    else if crate::runtime::is_focusable_type(
                        &child_value.kind(),
                    )
                    {
                        elements.push(child.clone());
                    }
                }
            }
            continue;
        }
        if crate::runtime::is_focusable_type(&value.kind())
        {
            elements.push(element.clone());
        }
    }
    elements
}

fn mouse_target_element(
    elements: &[ElementRef],
    x: i32,
    y: i32,
) -> Option<ElementRef>
{
    elements.iter().rev().find_map(|element|
    {
        let value = element.borrow();
        let mut frame = value.frame;
        if value.kind() == ElementKind::ComboBox
        {
            frame.height = 1;
        }
        frame
            .contains(crate::Point { row: y, col: x })
            .then(|| element.clone())
    })
}

fn scroll_view_at_position(
    window: &crate::GeneratedWindow,
    x: i32,
    y: i32,
) -> Option<ElementRef>
{
    for element in window.elements.iter().rev()
    {
        crate::runtime::sync_reusable_child_frames(element);
        let value = element.borrow();
        if matches!(value.kind(), ElementKind::Reusable | ElementKind::ViewHost)
            && let Some(child) = value.child_window()
        {
            if child.is_generated_scroll_view()
                && value.frame.contains(crate::Point { row: y, col: x })
            {
                return Some(element.clone());
            }
            if let Some(scroll_view) = scroll_view_at_position(child, x, y)
            {
                return Some(scroll_view);
            }
        }
        if value.kind() == ElementKind::ScrollView
            && value.frame.contains(crate::Point { row: y, col: x })
        {
            return Some(element.clone());
        }
    }
    None
}

fn element_at_window(window: &crate::GeneratedWindow, x: i32, y: i32) -> Option<ElementRef>
{
    for element in window.elements.iter().rev()
    {
        let value = element.borrow();
        if value.kind() == ElementKind::Reusable
            && value.frame.contains(crate::Point { row: y, col: x })
            && value.child_window().is_some_and(|child|
            {
                child.generated_focusable()
                    && focusable_elements(child).is_empty()
            })
        {
            return Some(element.clone());
        }
        if value.kind() == ElementKind::ScrollView
            && value.frame.contains(crate::Point { row: y, col: x })
        {
            for view in value
                .child_views(Size
                {
                    width: value.frame.width,
                    height: value.frame.height,
                })
                .into_iter()
                .rev()
            {
                let Some(child) = view.element else { continue };
                let rendered = Rect
                {
                    row: value.frame.row + view.frame.row,
                    col: value.frame.col + view.frame.col,
                    width: view.frame.width,
                    height: view.frame.height,
                };
                if !view.visible
                    || !rendered.contains(crate::Point { row: y, col: x })
                {
                    continue;
                }
                let child_value = child.borrow();
                if let Some(window) = child_value.child_window()
                    && let Some(target) = element_at_window(window, x, y)
                {
                    return Some(target);
                }
                return Some(child.clone());
            }
        }
        if let Some(child) = value.child_window()
        {
            let target = if child.is_generated_scroll_view()
            {
                element_at_scroll_window(child, value.frame, x, y)
            }
            else
            {
                element_at_window(child, x, y)
            };
            if let Some(target) = target
            {
                return Some(target);
            }
        }
        if value.frame.contains(crate::Point { row: y, col: x })
        {
            return Some(element.clone());
        }
    }
    None
}

fn element_at_scroll_window(
    window: &crate::GeneratedWindow,
    viewport: Rect,
    x: i32,
    y: i32,
) -> Option<ElementRef>
{
    if !viewport.contains(crate::Point { row: y, col: x })
    {
        return None;
    }
    let natural_skip = window.last_natural_skip();
    let actual_skip =
        natural_skip - window.view_offset().clamp(0, natural_skip);
    for row in window.scroll_view_children().iter().rev()
    {
        let row_frame = row.borrow().frame;
        let rendered = Rect
        {
            row: row_frame.row - actual_skip,
            col: row_frame.col - window.h_view_offset(),
            width: row_frame.width,
            height: row_frame.height,
        };
        if !rendered.contains(crate::Point { row: y, col: x })
        {
            continue;
        }
        let value = row.borrow();
        if let Some(child) = value.child_window()
            && let Some(target) = element_at_window(child, x, y)
        {
            return Some(target);
        }
        return Some(row.clone());
    }
    None
}

fn activate_combo_box_option_at<A: GeneratedApplication>(
    app: &mut A,
    state: &mut RuntimeState,
    options: &GeneratedWindowRuntimeOptions,
    x: i32,
    y: i32,
) -> bool
{
    if !state.edit_mode
    {
        return false;
    }
    let Some(element) = state.focused_element(app.active_window()) else { return false };
    let option_index = {
        let value = element.borrow();
        if value.kind() != ElementKind::ComboBox
            || x < value.frame.col
            || x >= value.frame.col + value.frame.width
            || y <= value.frame.row
            || y > value.frame.row + value.options().len() as i32
        {
            return false;
        }
        y - value.frame.row - 1
    };
    element.borrow_mut().set_selected_index(option_index);
    let element_name = active_element_id(app, &element);
    let selected = element.borrow().selected_values();
    state.commit_element_edit(&element);
    if state.scope_edit_element.is_some()
    {
        state.exit_scope_navigation(app.active_window());
    }
    crate::runtime::dispatch_selection_changed(
        app,
        options,
        &element_name,
        &selected,
    );
    true
}

fn snapshot_element<A: GeneratedApplication>(app: &A, state: &RuntimeState, element: &ElementRef) -> Value
{
    let element_id = active_element_id(app, element);
    let focused = state.focused_element(app.active_window()).is_some_and(|focused| std::rc::Rc::ptr_eq(&focused, element));
    let value = element.borrow();
    let description = element_metadata_for(app.active_window(), element)
        .map_or_else(String::new, |metadata| metadata.description);
    let mut result = json!({
        "id": element_id,
        "type": element_type(&value.kind()),
        "role": element_role(&value.kind()),
        "description": description,
        "value": element_value(element),
        "focused": focused,
        "enabled": value.enabled,
        "visible": true,
        "bounds": rect_json(value.frame),
        "style_state": if focused { if state.edit_mode { "edit" } else { "focus" } } else { "base" },
    });
    if matches!(value.kind(), ElementKind::ComboBox | ElementKind::ListBox)
    {
        result["options"] = json!(value.options());
    }
    if matches!(value.kind(), ElementKind::TextInput | ElementKind::TextArea)
    {
        result["cursor"] = json!(value.cursor());
        result["selection"] = selection_snapshot(element);
    }
    result
}

fn element_metadata_for(
    window: &crate::GeneratedWindow,
    target: &ElementRef,
) -> Option<crate::GeneratedElementMetadata>
{
    for element in &window.elements
    {
        if std::rc::Rc::ptr_eq(element, target)
        {
            return window
                .mcp_element_metadata(&element.borrow().name)
                .cloned();
        }
        if let Some(child) = element.borrow().child_window()
            && let Some(metadata) = element_metadata_for(child, target)
        {
            return Some(metadata);
        }
    }
    None
}

fn exposed_elements(window: &crate::GeneratedWindow) -> Vec<ElementRef>
{
    fn append(window: &crate::GeneratedWindow, elements: &mut Vec<ElementRef>)
    {
        for element in &window.elements
        {
            let (name, scroll_children) =
            {
                let value = element.borrow();
                (
                    value.name.clone(),
                    if value.kind() == ElementKind::ScrollView
                    {
                        value.children().to_vec()
                    }
                    else
                    {
                        Vec::new()
                    },
                )
            };
            let exposed = window
                .mcp_element_metadata(&name)
                .is_none_or(|metadata| metadata.expose);
            if exposed
            {
                elements.push(element.clone());
            }
            if let Some(child) = element.borrow().child_window()
            {
                append(child, elements);
            }
            for child in scroll_children
            {
                if let Some(child_window) = child.borrow().child_window()
                {
                    append(child_window, elements);
                }
            }
        }
    }

    let mut elements = Vec::new();
    append(window, &mut elements);
    elements
}

fn focused_exposed_snapshot<A: GeneratedApplication>(
    app: &A,
    state: &RuntimeState,
) -> Value
{
    let Some(focused) = state.focused_element(app.active_window()) else
    {
        return Value::Null;
    };
    if !exposed_elements(app.active_window())
        .iter()
        .any(|element| std::rc::Rc::ptr_eq(element, &focused))
    {
        return Value::Null;
    }
    snapshot_element(app, state, &focused)
}

fn element_capabilities(element: &crate::Element) -> Value
{
    let mut capabilities = vec!["ui.get_element"];
    if matches!(
        element.kind(),
        ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::Button
            | ElementKind::CheckBox
            | ElementKind::ComboBox
            | ElementKind::ListBox
            | ElementKind::Image
            | ElementKind::Label
            | ElementKind::SpanLabel
            | ElementKind::InfoLabel
    )
    {
        capabilities.push("ui.get_value");
    }
    match element.kind()
    {
        ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput =>
        {
            capabilities.extend(["ui.set_value", "ui.fill", "ui.clear", "ui.focus"]);
        }
        ElementKind::Button =>
        {
            capabilities.extend(["ui.activate", "ui.click", "ui.focus"]);
        }
        ElementKind::CheckBox =>
        {
            capabilities.extend([
                "ui.set_value",
                "ui.check",
                "ui.uncheck",
                "ui.set_checked",
                "ui.activate",
                "ui.click",
                "ui.focus",
            ]);
        }
        ElementKind::ComboBox =>
        {
            capabilities.extend([
                "ui.set_value",
                "ui.select_option",
                "ui.get_options",
                "ui.focus",
                "ui.press",
            ]);
        }
        ElementKind::ListBox =>
        {
            capabilities.extend([
                "ui.set_value",
                "ui.select_option",
                "ui.set_selection",
                "ui.get_options",
                "ui.focus",
                "ui.press",
            ]);
        }
        _ => {}
    }
    json!(capabilities)
}

fn selection_snapshot(element: &ElementRef) -> Value
{
    let value = element.borrow();
    if !matches!(
        value.kind(),
        ElementKind::TextInput | ElementKind::TextArea
    )
    {
        return json!({"start": Value::Null, "end": Value::Null, "text": ""});
    }
    let Some(anchor) = value.selection_anchor() else
    {
        return json!({"start": Value::Null, "end": Value::Null, "text": ""});
    };
    if anchor == value.cursor()
    {
        return json!({"start": Value::Null, "end": Value::Null, "text": ""});
    }
    json!({
        "start": min(anchor, value.cursor()),
        "end": max(anchor, value.cursor()),
        "text": value.selected_text(),
    })
}

fn element_type(kind: &ElementKind) -> &'static str
{
    match kind
    {
        ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel => "label",
        ElementKind::FrameBufferView => "element",
        ElementKind::MessageTable => "messagetable",
        ElementKind::Image => "image",
        ElementKind::TextInput => "textinput",
        ElementKind::TextArea => "textarea",
        ElementKind::NumberInput => "numberinput",
        ElementKind::Button => "button",
        ElementKind::CheckBox => "checkbox",
        ElementKind::ComboBox => "combobox",
        ElementKind::ListBox => "listbox",
        ElementKind::ScrollView => "scrollview",
        ElementKind::Reusable => "element",
        ElementKind::ViewHost => "viewhost",
    }
}

fn element_role(kind: &ElementKind) -> &'static str
{
    match kind
    {
        ElementKind::TextInput | ElementKind::TextArea | ElementKind::NumberInput => "input",
        ElementKind::Button => "action",
        ElementKind::CheckBox => "toggle",
        ElementKind::ComboBox | ElementKind::ListBox => "selection",
        _ => "text",
    }
}

fn element_value(element: &ElementRef) -> Value
{
    let value = element.borrow();
    match value.kind()
    {
        ElementKind::TextInput | ElementKind::TextArea =>
            Value::String(value.value().to_string()),
        ElementKind::CheckBox => Value::Bool(value.checked()),
        ElementKind::NumberInput => json!(value.number_value()),
        ElementKind::ComboBox => Value::String(value.selected_value()),
        ElementKind::ListBox => json!(value.selected_values()),
        ElementKind::Button => Value::String(value.title().to_string()),
        ElementKind::Image => Value::String(value.source().to_string()),
        ElementKind::Label | ElementKind::SpanLabel | ElementKind::InfoLabel =>
            Value::String(value.text().to_string()),
        ElementKind::MessageTable => Value::Null,
        ElementKind::FrameBufferView
            | ElementKind::ScrollView
            | ElementKind::Reusable
            | ElementKind::ViewHost => Value::Null,
    }
}

fn text_change_value(element: &ElementRef) -> String
{
    let element = element.borrow();
    match element.kind()
    {
        ElementKind::TextInput | ElementKind::TextArea =>
            element.value().to_string(),
        ElementKind::NumberInput => element.number_display_value(),
        ElementKind::CheckBox =>
        {
            if element.checked() { "True" } else { "False" }.to_string()
        }
        ElementKind::ComboBox | ElementKind::ListBox =>
            element.selected_values().into_iter().next().unwrap_or_default(),
        _ => String::new(),
    }
}

fn mcp_is_editable_element(element: &ElementRef) -> bool
{
    let element = element.borrow();
    if !element.enabled
    {
        return false;
    }
    if matches!(element.kind(), ElementKind::Reusable | ElementKind::ViewHost)
    {
        return element
            .child_window()
            .is_some_and(crate::GeneratedWindow::is_generated_scroll_view);
    }
    matches!(
        element.kind(),
        ElementKind::TextInput
            | ElementKind::TextArea
            | ElementKind::NumberInput
            | ElementKind::ComboBox
            | ElementKind::ListBox
            | ElementKind::ScrollView
    )
}

pub(crate) fn selected_text(element: &ElementRef) -> String
{
    element.borrow().selected_text()
}

fn focused_copy_text(element: &ElementRef) -> String
{
    let kind = element.borrow().kind();
    match kind
    {
        ElementKind::TextInput | ElementKind::TextArea =>
        {
            let selected = selected_text(element);
            if selected.is_empty()
            {
                element.borrow().value().to_string()
            }
            else
            {
                selected
            }
        }
        ElementKind::NumberInput =>
        {
            crate::NumberInputRef::from(element.clone()).display_value()
        }
        _ => String::new(),
    }
}

fn json_text_value(value: &Value) -> String
{
    match value
    {
        Value::String(value) => value.clone(),
        Value::Number(_) | Value::Bool(_) => value.to_string(),
        _ => String::new(),
    }
}

fn set_option_value<A: GeneratedApplication>(
    app: &mut A,
    options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
    value: &str,
) -> Result<(), String>
{
    let element_name = active_element_id(app, element);
    let element_kind = element.borrow().kind();
    if !matches!(element_kind, ElementKind::ComboBox | ElementKind::ListBox)
    {
        return Err(format!(
            "Element is not a supported selection control: {element_name}",
        ));
    }
    let next_index = element
        .borrow()
        .options()
        .iter()
        .position(|option| option == value)
        .ok_or_else(|| format!("Unknown option for {element_name}: {value}"))?;
    let previous_values = element.borrow().selected_values();
    if element_kind == ElementKind::ComboBox || !element.borrow().multiple()
    {
        element.borrow_mut().set_selected_index(next_index as i32);
    }
    else
    {
        element.borrow_mut().set_selected_items(&[value.to_string()]);
    }
    let selected = element.borrow().selected_values();
    if selected != previous_values
    {
        crate::runtime::dispatch_selection_changed(
            app,
            options,
            &element_name,
            &selected,
        );
    }
    Ok(())
}

fn set_list_box_selection<A: GeneratedApplication>(
    app: &mut A,
    options: &GeneratedWindowRuntimeOptions,
    element: &ElementRef,
    values: &Value,
) -> Result<(), String>
{
    let element_name = active_element_id(app, element);
    if element.borrow().kind() != ElementKind::ListBox
    {
        return Err(format!("Element is not a listbox: {element_name}"));
    }
    let values = values
        .as_array()
        .ok_or_else(||
        {
            format!("ListBox selection values must be an array: {element_name}")
        })?;
    if !element.borrow().multiple() && values.len() > 1
    {
        return Err(format!("ListBox is not multi-select: {element_name}"));
    }
    let requested = values
        .iter()
        .map(|value|
        {
            value
                .as_str()
                .map(str::to_string)
                .unwrap_or_else(|| value.to_string())
        })
        .collect::<Vec<_>>();
    let available = element.borrow().options().to_vec();
    for value in &requested
    {
        if !available.contains(value)
        {
            return Err(format!("Unknown option for {element_name}: {value}"));
        }
    }
    let selected = available
        .iter()
        .filter(|option| requested.contains(option))
        .cloned()
        .collect::<Vec<_>>();
    let previous_values = element.borrow().selected_values();
    element.borrow_mut().set_selected_items(&selected);
    if selected != previous_values
    {
        crate::runtime::dispatch_selection_changed(
            app,
            options,
            &element_name,
            &selected,
        );
    }
    Ok(())
}

fn viewport_json<A: GeneratedApplication>(app: &A, config: &McpRuntimeConfig) -> Value
{
    let viewport = config.viewport.unwrap_or_else(||
    {
        let size = crate::runtime::generated_window_content_size(app.active_window());
        Rect { row: 0, col: 0, width: size.width, height: size.height }
    });
    json!({"row": viewport.row, "col": viewport.col, "width": viewport.width, "height": viewport.height})
}

fn rect_json(rect: Rect) -> Value
{
    json!({"top": rect.row, "left": rect.col, "bottom": rect.row + rect.height, "right": rect.col + rect.width, "width": rect.width, "height": rect.height})
}

fn compact_cells(buffer: &crate::TerminalBuffer) -> Value
{
    Value::Array((0..buffer.height).map(|row|
    {
        Value::Array((0..buffer.width).map(|col|
        {
            let cell = buffer.cell(row, col).expect("valid buffer cell");
            json!([cell.text, cell_color(&cell.foreground), cell_color(&cell.background)])
        }).collect())
    }).collect())
}

fn snapshot_cells(buffer: &crate::TerminalBuffer) -> Value
{
    Value::Array((0..buffer.height).map(|row|
    {
        Value::Array((0..buffer.width).map(|col|
        {
            let cell = buffer.cell(row, col).expect("valid buffer cell");
            json!({"char": cell.text, "foreground": cell_color(&cell.foreground), "background": cell_color(&cell.background), "attributes": []})
        }).collect())
    }).collect())
}

fn cell_color(color: &Option<crate::Color>) -> Value
{
    let Some(color) = color else { return Value::Null };
    if color.empty() || color.is_transparent() || color.as_str().is_empty()
    {
        Value::Null
    }
    else
    {
        Value::String(color.normalized())
    }
}

fn string_argument<'a>(arguments: &'a Map<String, Value>, key: &str) -> &'a str
{
    arguments.get(key).and_then(Value::as_str).unwrap_or_default()
}

fn text_argument(arguments: &Map<String, Value>, key: &str) -> String
{
    match arguments.get(key)
    {
        Some(Value::String(value)) => value.clone(),
        Some(value @ (Value::Number(_) | Value::Bool(_))) => value.to_string(),
        _ => String::new(),
    }
}

fn int_argument(arguments: &Map<String, Value>, key: &str) -> Option<i32>
{
    arguments.get(key).and_then(|value|
        value.as_i64().map(|value| value as i32).or_else(|| value.as_str()?.parse().ok()))
}

fn int64_argument(arguments: &Map<String, Value>, key: &str) -> Option<i64>
{
    arguments.get(key).and_then(|value|
        value.as_i64().or_else(|| value.as_str()?.parse().ok()))
}

#[cfg(test)]
mod tests
{
    use super::*;

    struct TestApp
    {
        window: crate::GeneratedWindow,
    }

    impl GeneratedApplication for TestApp
    {
        fn window(&self) -> &crate::GeneratedWindow { &self.window }
        fn window_mut(&mut self) -> &mut crate::GeneratedWindow { &mut self.window }
    }

    struct ActiveWindowButtonApp
    {
        window: crate::GeneratedWindow,
        buttons: std::rc::Rc<std::cell::RefCell<Vec<String>>>,
    }

    impl GeneratedApplication for ActiveWindowButtonApp
    {
        fn window(&self) -> &crate::GeneratedWindow { &self.window }
        fn window_mut(&mut self) -> &mut crate::GeneratedWindow { &mut self.window }

        fn handle_active_window_button(&mut self, name: &str) -> bool
        {
            self.buttons.borrow_mut().push(name.to_string());
            true
        }
    }

    #[test]
    fn parses_compare_viewport()
    {
        let config = McpRuntimeConfig::parse(&[
            "app".into(),
            "--mcp-server".into(),
            "--headless".into(),
            "--viewport".into(),
            "0,0,90,35".into(),
        ]);
        assert!(config.enabled);
        assert!(!config.gui);
        assert_eq!(config.transport, "stdio");
        assert_eq!(config.action_delay_ms, DEFAULT_HEADLESS_MCP_ACTION_DELAY_MS);
        assert_eq!(config.type_delay_ms, DEFAULT_HEADLESS_MCP_TYPE_DELAY_MS);
        assert_eq!(
            config.viewport,
            Some(Rect { row: 0, col: 0, width: 90, height: 35 }),
        );
    }

    #[test]
    fn parses_absent_component_and_cpp_prefix_viewports()
    {
        let absent = McpRuntimeConfig::parse(&["app".into(), "--headless".into()]);
        assert_eq!(absent.viewport, None);

        let component = McpRuntimeConfig::parse(&[
            "app".into(),
            "--viewport-col".into(),
            " +7suffix".into(),
            "--viewport-height".into(),
            "-4".into(),
        ]);
        assert_eq!(
            component.viewport,
            Some(Rect { row: 0, col: 7, width: 1, height: 1 }),
        );

        let combined = McpRuntimeConfig::parse(&[
            "app".into(),
            "--viewport".into(),
            " -2x,+3tail,40cells,12rows".into(),
            "--viewport-width".into(),
            "99".into(),
        ]);
        assert_eq!(
            combined.viewport,
            Some(Rect { row: 0, col: 3, width: 40, height: 12 }),
        );
    }

    #[test]
    fn malformed_cpp_numeric_arguments_report_startup_failure()
    {
        for args in [
            vec!["app".into(), "--mcp-port".into(), "bad".into()],
            vec!["app".into(), "--viewport".into(), "0,0,90".into()],
            vec!["app".into(), "--viewport-width".into(), "x90".into()],
        ]
        {
            assert!(std::panic::catch_unwind(|| McpRuntimeConfig::parse(&args)).is_err());
        }
    }

    #[test]
    fn parses_gui_mcp_lifecycle_options_like_cpp()
    {
        let config = McpRuntimeConfig::parse(&[
            "app".into(),
            "--mcp-server".into(),
            "--mcp-transport".into(),
            "http".into(),
            "--mcp-port".into(),
            "9876".into(),
            "--mcp-action-delay-ms".into(),
            "7".into(),
            "--mcp-type-delay-ms".into(),
            "3".into(),
            "--mcp-wait-render".into(),
            "--mcp-controlled-render".into(),
        ]);
        assert!(config.enabled);
        assert!(config.gui);
        assert_eq!(config.transport, "http");
        assert_eq!(config.port, 9876);
        assert_eq!(config.action_delay_ms, 7);
        assert_eq!(config.type_delay_ms, 3);
        assert!(config.wait_render);
        assert!(config.controlled_render);
        assert_eq!(config.viewport, None);
    }

    #[test]
    fn gui_text_tools_publish_the_same_incremental_render_progress_as_cpp()
    {
        let mut window = crate::GeneratedWindow::new("root");
        window.add_element(crate::new_text_input("editor", "", 0));
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        let mut progress_values = Vec::new();
        let mut dispatch = |method: &str, text: &str|
        {
            progress_values.clear();
            let response = dispatch_json_rpc_value_with_progress(
                &mut app,
                &mut state,
                &options,
                &mut config,
                &json!({
                    "jsonrpc": "2.0",
                    "id": 1,
                    "method": method,
                    "params": {
                        "element_id": "editor",
                        "text": text,
                    },
                }),
                &mut |progress_app, _, _, _|
                {
                    progress_values.push(
                        progress_app
                            .active_window()
                            .find_element("editor")
                            .expect("text element")
                            .borrow()
                            .value()
                            .to_string(),
                    );
                },
            );
            assert!(response.is_some());
            progress_values.clone()
        };

        assert_eq!(dispatch("type_text", "ab"), ["a", "ab"]);
        assert_eq!(dispatch("paste_text", "cd"), ["abcd"]);
        assert_eq!(dispatch("set_text", "xy"), ["", "x", "xy", "xy"]);
    }

    #[test]
    fn paste_text_matches_cpp_single_operation_selection_and_number_contract()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let editor = window.add_element(crate::new_text_input("editor", "abcd", 4));
        let edit_started = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let changed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_edit_started: Some({
                let edit_started = edit_started.clone();
                std::rc::Rc::new(move |name|
                {
                    edit_started.borrow_mut().push(name.to_string());
                })
            }),
            on_text_changed: Some({
                let changed = changed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    changed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut app = TestApp { window };
        let mut config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        let selection_response = dispatch_json_rpc_value(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 1,
                "method": "select_text",
                "params": {"element_id": "editor", "start": 1, "end": 3},
            }),
        );
        assert!(selection_response.is_some());
        edit_started.borrow_mut().clear();
        changed.borrow_mut().clear();

        let mut progress_values = Vec::new();
        let response = dispatch_json_rpc_value_with_progress(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 2,
                "method": "paste_text",
                "params": {"element_id": "editor", "text": "XY"},
            }),
            &mut |progress_app, _, _, _|
            {
                progress_values.push(
                    progress_app
                        .active_window()
                        .find_element("editor")
                        .expect("text element")
                        .borrow()
                        .value()
                        .to_string(),
                );
            },
        );
        assert!(response.is_some());
        assert_eq!(editor.borrow().value(), "aXYd");
        assert_eq!(&*edit_started.borrow(), &["editor"]);
        assert_eq!(
            &*changed.borrow(),
            &[("editor".to_string(), "aXYd".to_string())],
        );
        assert_eq!(progress_values, ["aXYd"]);

        let mut number_window = crate::GeneratedWindow::new("root");
        let amount = number_window.add_element(crate::new_number_input_with_step(
            "amount",
            1.0,
            0.5,
        ));
        let number_edit_started =
            std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let number_changed =
            std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let number_options = GeneratedWindowRuntimeOptions
        {
            on_edit_started: Some({
                let number_edit_started = number_edit_started.clone();
                std::rc::Rc::new(move |name|
                {
                    number_edit_started.borrow_mut().push(name.to_string());
                })
            }),
            on_text_changed: Some({
                let number_changed = number_changed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    number_changed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut number_app = TestApp { window: number_window };
        let mut number_state = RuntimeState::new(
            &number_app.window,
            &number_options,
            Size { width: 30, height: 8 },
        );
        let mut number_progress = Vec::new();
        let number_response = dispatch_json_rpc_value_with_progress(
            &mut number_app,
            &mut number_state,
            &number_options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 3,
                "method": "paste_text",
                "params": {"element_id": "amount", "text": "12.5suffix"},
            }),
            &mut |progress_app, _, _, _|
            {
                number_progress.push(
                    progress_app
                        .active_window()
                        .find_element("amount")
                        .expect("number element")
                        .borrow()
                        .number_value(),
                );
            },
        );
        assert!(number_response.is_some());
        assert_eq!(amount.borrow().number_value(), 12.5);
        assert_eq!(&*number_edit_started.borrow(), &["amount"]);
        assert_eq!(
            &*number_changed.borrow(),
            &[("amount".to_string(), "12.5".to_string())],
        );
        assert_eq!(number_progress, [12.5]);

        number_edit_started.borrow_mut().clear();
        number_changed.borrow_mut().clear();
        let invalid_response = dispatch_json_rpc_value(
            &mut number_app,
            &mut number_state,
            &number_options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 4,
                "method": "paste_text",
                "params": {"element_id": "amount", "text": "suffix"},
            }),
        )
        .expect("invalid numeric paste response");
        assert!(invalid_response.get("error").is_some());
        assert_eq!(amount.borrow().number_value(), 12.5);
        assert_eq!(&*number_edit_started.borrow(), &["amount"]);
        assert!(number_changed.borrow().is_empty());
    }

    #[test]
    fn text_tools_accept_the_same_json_text_shapes_as_cpp()
    {
        for (value, expected) in [
            (json!("text"), "text"),
            (json!(34), "34"),
            (json!(true), "true"),
            (Value::Null, ""),
            (json!([1]), ""),
            (json!({"value": 1}), ""),
        ]
        {
            let arguments = json!({"text": value})
                .as_object()
                .expect("text arguments")
                .clone();
            assert_eq!(text_argument(&arguments, "text"), expected);
        }

        let mut window = crate::GeneratedWindow::new("root");
        let amount = window.add_element(crate::new_number_input("amount", 0.0));
        let changed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_text_changed: Some({
                let changed = changed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    changed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut app = TestApp { window };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let mut config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let response = dispatch_json_rpc_value(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 1,
                "method": "set_text",
                "params": {"element_id": "amount", "text": 34},
            }),
        );
        assert!(response.is_some());
        assert_eq!(amount.borrow().number_value(), 34.0);
        assert_eq!(
            changed.borrow().last(),
            Some(&("amount".to_string(), "34".to_string())),
        );
    }

    #[test]
    fn activate_and_click_keep_their_distinct_cpp_edit_entry_contracts()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let input = window.add_element(crate::new_text_input("editor", "abcdef", 0));
        input.borrow_mut().frame = Rect
        {
            row: 1,
            col: 2,
            width: 8,
            height: 1,
        };
        input.borrow_mut().set_cursor(0);
        let edit_started = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_edit_started: Some({
                let edit_started = edit_started.clone();
                std::rc::Rc::new(move |name|
                {
                    edit_started.borrow_mut().push(name.to_string());
                })
            }),
            ..Default::default()
        };
        let mut app = TestApp { window };
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            viewport: Some(Rect { row: 0, col: 0, width: 30, height: 8 }),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "activate_element",
            &Map::from_iter([("element_id".to_string(), json!("editor"))]),
        )
        .expect("activate text input");
        assert!(state.edit_mode);
        assert_eq!(input.borrow().cursor(), 0);
        assert_eq!(&*edit_started.borrow(), &["editor"]);

        state.exit_current_edit(&mut app, &options, true);
        input.borrow_mut().set_cursor(0);
        edit_started.borrow_mut().clear();
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "click_element",
            &Map::from_iter([("element_id".to_string(), json!("editor"))]),
        )
        .expect("click text input");
        assert!(state.edit_mode);
        assert_eq!(input.borrow().cursor(), 4);
        assert_eq!(&*edit_started.borrow(), &["editor"]);
    }

    #[test]
    fn click_element_refreshes_nested_scroll_row_frames_before_reading_the_centre()
    {
        let mut row = crate::GeneratedWindow::new("row");
        row.set_generated_layout(vec![crate::LayoutItem
        {
            element_type: "checkbox".to_string(),
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
            content: "done".to_string(),
            ..Default::default()
        }]);
        let done = row.add_element(crate::new_checkbox("done", "Done", false));

        let mut scroll = crate::GeneratedWindow::new_scroll_view("board");
        scroll.add_named_child_window("board[0]", row);
        let host = crate::new_reusable_element("board", "TaskList");
        host.borrow_mut().frame =
            Rect { row: 7, col: 27, width: 62, height: 28 };
        host.borrow_mut().set_child_window(scroll);

        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(host);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 90, height: 35 },
        );
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            viewport: Some(Rect { row: 0, col: 0, width: 90, height: 35 }),
            ..Default::default()
        };

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "click_element",
            &Map::from_iter([("element_id".to_string(), json!("board[0].done"))]),
        )
        .expect("click nested generated ScrollView checkbox");

        assert!(done.borrow().checked());
        assert_eq!(
            done.borrow().frame,
            Rect { row: 7, col: 27, width: 8, height: 1 },
        );
    }

    #[test]
    fn activate_button_after_text_tool_dispatches_before_render_like_cpp()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let editor = window.add_element(crate::new_text_input("editor", "", 0));
        let action = window.add_element(crate::new_button("action", "Action"));
        let activated = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let confirmed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_button: Some({
                let activated = activated.clone();
                std::rc::Rc::new(move |name|
                {
                    activated.borrow_mut().push(name.to_string());
                })
            }),
            on_text_confirmed: Some({
                let confirmed = confirmed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    confirmed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut app = TestApp { window };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_text",
            &Map::from_iter([
                ("element_id".to_string(), json!("editor")),
                ("text".to_string(), json!("updated")),
            ]),
        )
        .expect("set text");
        assert!(state.edit_mode);

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "activate_element",
            &Map::from_iter([("element_id".to_string(), json!("action"))]),
        )
        .expect("activate button");

        assert_eq!(editor.borrow().value(), "updated");
        assert_eq!(&*activated.borrow(), &["action"]);
        assert!(confirmed.borrow().is_empty());
        assert!(!state.edit_mode);
        assert!(std::rc::Rc::ptr_eq(
            &action,
            &state
                .focused_element(app.active_window())
                .expect("button remains focused"),
        ));
    }

    #[test]
    fn modal_button_uses_the_app_owned_frame_hook_before_dialog_behavior()
    {
        let mut root = crate::GeneratedWindow::new("root");
        let mut modal = crate::GeneratedWindow::new("modal");
        modal.add_element(crate::new_button("open_btn", "Open"));
        root.open_window(modal);
        let buttons = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let mut app = ActiveWindowButtonApp
        {
            window: root,
            buttons: buttons.clone(),
        };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "activate_element",
            &Map::from_iter([("element_id".to_string(), json!("open_btn"))]),
        )
        .expect("activate modal frame button");

        assert_eq!(&*buttons.borrow(), &["open_btn"]);
        assert_eq!(app.window.modal_depth(), 1);
    }

    #[test]
    fn mcp_focus_helpers_match_cpp_root_and_scroll_scope_cleanup_order()
    {
        let mut root_window = crate::GeneratedWindow::new("root");
        root_window.add_element(crate::new_text_input("first", "one", 0));
        root_window.add_element(crate::new_text_input("second", "two", 0));
        let confirmed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let root_options = GeneratedWindowRuntimeOptions
        {
            on_text_confirmed: Some({
                let confirmed = confirmed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    confirmed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut root_app = TestApp { window: root_window };
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut root_state = RuntimeState::new(
            &root_app.window,
            &root_options,
            Size { width: 30, height: 8 },
        );
        call_tool(
            &mut root_app,
            &mut root_state,
            &root_options,
            &mut config,
            "enter_edit_mode",
            &Map::from_iter([("element_id".to_string(), json!("first"))]),
        )
        .expect("enter first root editor");
        call_tool(
            &mut root_app,
            &mut root_state,
            &root_options,
            &mut config,
            "focus_element",
            &Map::from_iter([("element_id".to_string(), json!("second"))]),
        )
        .expect("retarget root focus");
        assert!(root_state.edit_mode);
        assert_eq!(
            root_state
                .focused_element(root_app.active_window())
                .expect("second focused")
                .borrow()
                .name,
            "second",
        );
        assert!(confirmed.borrow().is_empty());

        let mut scoped_window = crate::GeneratedWindow::new("root");
        let scroll = scoped_window.add_element(crate::new_scroll_view("items", 0));
        let first = crate::new_text_input("first", "one", 0);
        let second = crate::new_text_input("second", "two", 0);
        scroll.borrow_mut().add_child(first.clone());
        scroll.borrow_mut().add_child(second.clone());
        let outside = scoped_window.add_element(crate::new_button("outside", "Outside"));
        let changed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let scoped_options = GeneratedWindowRuntimeOptions
        {
            on_text_changed: Some({
                let changed = changed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    changed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut scoped_app = TestApp { window: scoped_window };
        let mut scoped_state = RuntimeState::new(
            &scoped_app.window,
            &scoped_options,
            Size { width: 30, height: 8 },
        );
        call_tool(
            &mut scoped_app,
            &mut scoped_state,
            &scoped_options,
            &mut config,
            "enter_edit_mode",
            &Map::from_iter([("element_id".to_string(), json!("items[0].first"))]),
        )
        .expect("enter first scoped editor");
        changed.borrow_mut().clear();
        call_tool(
            &mut scoped_app,
            &mut scoped_state,
            &scoped_options,
            &mut config,
            "activate_element",
            &Map::from_iter([("element_id".to_string(), json!("items[1].second"))]),
        )
        .expect("activate a different contained editor");
        assert!(scoped_state.edit_mode);
        assert!(scoped_state.scope_edit_element.is_some());
        assert!(scoped_state.scope_dim_element.is_none());
        assert!(changed.borrow().is_empty());

        call_tool(
            &mut scoped_app,
            &mut scoped_state,
            &scoped_options,
            &mut config,
            "enter_edit_mode",
            &Map::from_iter([("element_id".to_string(), json!("items[0].first"))]),
        )
        .expect("re-enter first scoped editor");
        changed.borrow_mut().clear();
        call_tool(
            &mut scoped_app,
            &mut scoped_state,
            &scoped_options,
            &mut config,
            "focus_element",
            &Map::from_iter([("element_id".to_string(), json!("outside"))]),
        )
        .expect("leave scoped editor");
        assert!(!scoped_state.edit_mode);
        assert!(scoped_state.scope_edit_element.is_none());
        assert!(scoped_state.scope_dim_element.is_none());
        assert_eq!(
            &*changed.borrow(),
            &[("items[0].first".to_string(), "one".to_string())],
        );
        assert!(std::rc::Rc::ptr_eq(
            &outside,
            &scoped_state
                .focused_element(scoped_app.active_window())
                .expect("outside button focused"),
        ));
    }

    #[test]
    fn checkbox_callbacks_and_active_modal_text_snapshot_match_cpp()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let root_label = window.add_element(crate::new_label(
            "root_label",
            "ROOT_BACKGROUND_MARKER",
        ));
        root_label.borrow_mut().frame = Rect
        {
            row: 0,
            col: 0,
            width: 24,
            height: 1,
        };
        let checkbox =
            window.add_element(crate::new_checkbox("enabled", "Enabled", false));

        let changed = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_text_changed: Some({
                let changed = changed.clone();
                std::rc::Rc::new(move |name, value|
                {
                    changed
                        .borrow_mut()
                        .push((name.to_string(), value.to_string()));
                })
            }),
            ..Default::default()
        };
        let mut app = TestApp { window };
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            viewport: Some(Rect { row: 0, col: 0, width: 30, height: 8 }),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_value",
            &Map::from_iter([
                ("element_id".to_string(), json!("enabled")),
                ("value".to_string(), json!(true)),
            ]),
        )
        .expect("set checkbox true");
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_checked",
            &Map::from_iter([
                ("element_id".to_string(), json!("enabled")),
                ("checked".to_string(), json!(false)),
            ]),
        )
        .expect("set checkbox false");
        assert_eq!(
            &*changed.borrow(),
            &[
                ("enabled".to_string(), "True".to_string()),
                ("enabled".to_string(), "False".to_string()),
            ],
        );
        assert!(!checkbox.borrow().checked());

        let mut modal =
            crate::MessageBoxOk::new("Modal", "MODAL_FOREGROUND_MARKER");
        app.window.open_window(modal.take_window());
        let snapshot = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_text_snapshot",
            &Map::new(),
        )
        .expect("active modal text snapshot");
        let text = snapshot["lines"]
            .as_array()
            .expect("snapshot lines")
            .iter()
            .filter_map(Value::as_str)
            .collect::<Vec<_>>()
            .join("\n");
        assert!(text.contains("MODAL_FOREGROUND_MARKER"));
        assert!(!text.contains("ROOT_BACKGROUND_MARKER"));
    }

    #[test]
    fn gui_standard_escape_renders_negative_button_before_closing_modal_like_cpp()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let mut dialog = crate::MessageBoxYesNo::new("Confirm", "Continue?");
        app.window.open_window(dialog.take_window());
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let mut progress_count = 0;
        let response = dispatch_json_rpc_value_with_progress(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &json!({
                "jsonrpc": "2.0",
                "id": 1,
                "method": "press_key",
                "params": {"key": "Escape"},
            }),
            &mut |progress_app, progress_state, _, _|
            {
                progress_count += 1;
                assert_eq!(progress_app.window().modal_depth(), 1);
                assert_eq!(
                    progress_state
                        .focused_element(progress_app.active_window())
                        .expect("semantic negative button")
                        .borrow()
                        .name,
                    "no_btn",
                );
            },
        );

        assert!(response.is_some());
        assert_eq!(progress_count, 1);
        assert_eq!(dialog.result(), Some(crate::MessageBoxResult::No));
        assert_eq!(app.window.modal_depth(), 0);
    }

    #[test]
    fn standard_tool_results_match_cpp_contract_shapes()
    {
        let mut window = crate::GeneratedWindow::new("root");
        window.title = "Contract".to_string();
        window.metadata.class_name = "ContractUI".to_string();
        window.metadata.description = "MCP contract".to_string();
        window.metadata.enabled = true;
        window.add_element(crate::new_button("action", "Action"));
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig
        {
            enabled: true,
            gui: false,
            action_delay_ms: 0,
            type_delay_ms: 0,
            viewport: Some(Rect { row: 0, col: 0, width: 30, height: 8 }),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let empty = Map::new();

        let state_result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("Tab"))]),
        )
        .expect("press_key result");
        assert_eq!(state_result["window"]["description"], "MCP contract");
        assert!(state_result["window"]["width"].as_i64().is_some_and(|value| value > 0));
        assert!(state_result["window"]["height"].as_i64().is_some_and(|value| value > 0));
        assert_eq!(state_result["window"]["mcp_enabled"], true);
        assert!(state_result["elements"].is_array());
        assert!(state_result.get("ok").is_none());

        let frame_result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_render_frame",
            &empty,
        )
        .expect("get_render_frame result");
        assert!(frame_result["width"].as_i64().is_some_and(|value| value > 0));
        assert!(frame_result["height"].as_i64().is_some_and(|value| value > 0));
        assert!(frame_result.get("format").is_none());

        let compact_result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_render_snapshot_compact",
            &empty,
        )
        .expect("compact snapshot result");
        assert_eq!(compact_result["format"], "render-cells-v1");
        assert!(compact_result.get("width").is_none());
        assert!(compact_result.get("height").is_none());

        let snapshot_result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_render_snapshot",
            &empty,
        )
        .expect("render snapshot result");
        assert_eq!(
            snapshot_result["ansi_lines"],
            snapshot_result["text_lines"],
        );

        let repaint_result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "repaint",
            &empty,
        )
        .expect("repaint result");
        assert_eq!(repaint_result, json!({"ok": true}));
    }

    #[test]
    fn element_snapshots_and_capabilities_match_cpp_conditional_contract()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let label = window.add_element(crate::new_label("label", "Text"));
        let info = window.add_element(crate::new_info_label("info", "Info"));
        let frame_buffer =
            window.add_element(crate::new_frame_buffer_view("frame"));
        let image = window.add_element(crate::new_image(
            "image",
            "sample.png",
            "Sample",
            "contain",
            "fallback",
            "left",
            "top",
        ));
        let scroll = window.add_element(crate::new_scroll_view("scroll", 0));
        let reusable =
            window.add_element(crate::new_reusable_element("row", "RowUI"));
        let host = window.add_element(crate::new_view_host("host"));
        let input = window.add_element(crate::new_text_input("input", "abc", 0));
        let combo = window.add_element(crate::new_combo_box(
            "combo",
            vec!["one".to_string(), "two".to_string()],
        ));
        for element in [
            &label,
            &info,
            &frame_buffer,
            &image,
            &scroll,
            &reusable,
            &host,
            &input,
            &combo,
        ]
        {
            element.borrow_mut().frame = Rect
            {
                row: 1,
                col: 2,
                width: 3,
                height: 4,
            };
        }
        let app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let bounds = json!({
            "top": 1,
            "left": 2,
            "bottom": 5,
            "right": 5,
            "width": 3,
            "height": 4,
        });

        assert_eq!(
            snapshot_element(&app, &state, &label),
            json!({
                "id": "label",
                "type": "label",
                "role": "text",
                "description": "",
                "focused": false,
                "visible": true,
                "enabled": true,
                "bounds": bounds,
                "style_state": "base",
                "value": "Text",
            }),
        );
        assert_eq!(snapshot_element(&app, &state, &info)["type"], "label");
        assert_eq!(snapshot_element(&app, &state, &frame_buffer)["type"], "element");
        assert!(snapshot_element(&app, &state, &frame_buffer)["value"].is_null());
        assert_eq!(snapshot_element(&app, &state, &image)["role"], "text");
        assert_eq!(snapshot_element(&app, &state, &scroll)["type"], "scrollview");
        assert_eq!(snapshot_element(&app, &state, &reusable)["type"], "element");
        assert_eq!(snapshot_element(&app, &state, &host)["type"], "viewhost");
        assert_eq!(
            snapshot_element(&app, &state, &input),
            json!({
                "id": "input",
                "type": "textinput",
                "role": "input",
                "description": "",
                "focused": false,
                "visible": true,
                "enabled": true,
                "bounds": bounds,
                "style_state": "base",
                "value": "abc",
                "cursor": 3,
                "selection": {"start": Value::Null, "end": Value::Null, "text": ""},
            }),
        );
        assert_eq!(
            snapshot_element(&app, &state, &combo)["options"],
            json!(["one", "two"]),
        );
        assert_eq!(
            element_capabilities(&frame_buffer.borrow()),
            json!(["ui.get_element"]),
        );
        assert_eq!(
            element_capabilities(&image.borrow()),
            json!(["ui.get_element", "ui.get_value"]),
        );
    }

    #[test]
    fn image_render_info_restores_all_layout_frames_like_cpp()
    {
        let mut window = crate::GeneratedWindow::new("root");
        window.set_generated_layout(vec![
            crate::LayoutItem
            {
                element_type: "image".to_string(),
                row: 0,
                col: 0,
                cell_row: 0,
                cell_col: 0,
                cell_chars_width: 4,
                cell_chars_height: 2,
                cell_width: 4,
                cell_height: 2,
                cell_width_mode: "fixed".to_string(),
                cell_height_mode: "fixed".to_string(),
                width: 4,
                height: 2,
                width_mode: "fixed".to_string(),
                height_mode: "fixed".to_string(),
                chars_width: 4,
                chars_height: 2,
                content: "preview".to_string(),
                ..Default::default()
            },
        ]);
        let image = window.add_element(crate::new_image(
            "preview",
            "",
            "",
            "contain",
            "fallback",
            "left",
            "top",
        ));
        let original = Rect
        {
            row: 7,
            col: 9,
            width: 11,
            height: 13,
        };
        image.borrow_mut().frame = original;
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig
        {
            viewport: Some(Rect { row: 0, col: 0, width: 20, height: 8 }),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 8 },
        );

        let info = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_image_render_info",
            &Map::from_iter([("element_id".to_string(), json!("preview"))]),
        )
        .expect("image render info");
        assert_eq!(
            info["bounds"],
            json!({
                "top": 3,
                "left": 8,
                "width": 4,
                "height": 2,
                "right": 12,
                "bottom": 5,
            }),
        );
        assert_eq!(image.borrow().frame, original);
    }

    #[test]
    fn value_selection_and_cursor_tools_match_cpp_contracts()
    {
        let mut window = crate::GeneratedWindow::new("root");
        window.add_element(crate::new_button("action", "Action"));
        let input = window.add_element(crate::new_text_input("name", "abc", 0));
        let checkbox =
            window.add_element(crate::new_checkbox("enabled", "Enabled", false));
        let combo = window.add_element(crate::new_combo_box(
            "color",
            vec!["red".to_string(), "green".to_string()],
        ));
        let single = window.add_element(crate::new_list_box(
            "single",
            vec!["one".to_string(), "two".to_string()],
            false,
        ));
        let multi = window.add_element(crate::new_list_box(
            "multi",
            vec!["one".to_string(), "two".to_string(), "three".to_string()],
            true,
        ));
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_options",
            &Map::from_iter([("element_id".to_string(), json!("action"))]),
        )
        .is_err());
        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_value",
            &Map::from_iter([
                ("element_id".to_string(), json!("action")),
                ("value".to_string(), json!("renamed")),
            ]),
        )
        .is_err());
        assert_eq!(app.window.find_element("action").unwrap().borrow().title(), "Action");

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_value",
            &Map::from_iter([
                ("element_id".to_string(), json!("name")),
                ("value".to_string(), json!(42)),
            ]),
        )
        .expect("set text input value");
        assert_eq!(input.borrow().value(), "42");
        assert_eq!(input.borrow().cursor(), 2);

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_value",
            &Map::from_iter([
                ("element_id".to_string(), json!("enabled")),
                ("value".to_string(), json!("true")),
            ]),
        )
        .is_err());

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "select_option",
            &Map::from_iter([
                ("element_id".to_string(), json!("color")),
                ("value".to_string(), json!("blue")),
            ]),
        )
        .is_err());
        assert_eq!(combo.borrow().selected_value(), "red");

        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "select_option",
            &Map::from_iter([
                ("element_id".to_string(), json!("color")),
                ("value".to_string(), json!("green")),
            ]),
        )
        .expect("select combo option");
        assert_eq!(combo.borrow().selected_value(), "green");

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_selection",
            &Map::from_iter([
                ("element_id".to_string(), json!("single")),
                ("values".to_string(), json!(["one", "two"])),
            ]),
        )
        .is_err());
        assert_eq!(single.borrow().selected_values(), ["one"]);

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_selection",
            &Map::from_iter([
                ("element_id".to_string(), json!("multi")),
                ("values".to_string(), json!(["missing"])),
            ]),
        )
        .is_err());
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_selection",
            &Map::from_iter([
                ("element_id".to_string(), json!("multi")),
                ("values".to_string(), json!(["three", "one"])),
            ]),
        )
        .expect("set multi selection");
        assert_eq!(multi.borrow().selected_values(), ["one", "three"]);

        assert!(!state.edit_mode);
        assert!(state.focused_element(app.active_window()).is_none());
        let cursor = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_cursor",
            &Map::from_iter([
                ("element_id".to_string(), json!("name")),
                ("offset".to_string(), json!(1)),
            ]),
        )
        .expect("set cursor");
        assert_eq!(cursor["cursor"], 1);
        assert!(!state.edit_mode);
        assert!(state.focused_element(app.active_window()).is_none());

        assert!(call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "clear_text",
            &Map::from_iter([("element_id".to_string(), json!("enabled"))]),
        )
        .is_err());
        assert!(!checkbox.borrow().checked());

        let non_text_cursor = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "move_cursor",
            &Map::from_iter([
                ("element_id".to_string(), json!("enabled")),
                ("direction".to_string(), json!("left")),
            ]),
        )
        .expect("move cursor on non-text control");
        assert_eq!(non_text_cursor, json!({"cursor": Value::Null}));
        assert!(!checkbox.borrow().checked());

        let selected = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "select_text",
            &Map::from_iter([
                ("element_id".to_string(), json!("name")),
                ("start".to_string(), json!(9)),
                ("end".to_string(), json!(-2)),
            ]),
        )
        .expect("select clamped text with raw result endpoints");
        assert_eq!(
            selected,
            json!({"start": -2, "end": 9, "text": "42"}),
        );
    }

    #[test]
    fn mcp_copy_and_paste_share_the_runtime_clipboard_like_cpp()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let input = window.add_element(crate::new_text_input("name", "alpha", 0));
        let number = window.add_element(crate::new_number_input_with_step("amount", 12.5, 0.5));
        let combo = window.add_element(crate::new_combo_box(
            "choice",
            vec!["one".to_string(), "two".to_string()],
        ));
        let proxy = crate::new_reusable_element("generated_scroll", "GeneratedScroll");
        proxy
            .borrow_mut()
            .set_child_window(crate::GeneratedWindow::new_scroll_view("rows"));
        window.add_element(proxy.clone());
        let mut app = TestApp { window };
        let edit_started = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let options = GeneratedWindowRuntimeOptions
        {
            on_edit_started: Some({
                let edit_started = edit_started.clone();
                std::rc::Rc::new(move |name|
                {
                    edit_started.borrow_mut().push(name.to_string());
                })
            }),
            ..Default::default()
        };
        let mut config = McpRuntimeConfig
        {
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );

        focus_element(&mut app, &mut state, &options, &input);
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_c"))]),
        )
        .expect("copy focused text");
        assert_eq!(crate::runtime_clipboard_text(), "alpha");

        input.borrow_mut().set_value("");
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_v"))]),
        )
        .expect("paste focused text");
        assert_eq!(input.borrow().value(), "alpha");

        input.borrow_mut().select_range(1, 4);
        let copied = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "copy_selection",
            &Map::from_iter([("element_id".to_string(), json!("name"))]),
        )
        .expect("copy selection");
        assert_eq!(copied, json!({"text": "lph"}));
        assert_eq!(crate::runtime_clipboard_text(), "lph");

        focus_element(&mut app, &mut state, &options, &number);
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_c"))]),
        )
        .expect("copy number");
        assert_eq!(crate::runtime_clipboard_text(), "12.5");

        let _ = crate::copy_text_to_clipboard("7.25");
        number.borrow_mut().set_number_value(0.0);
        state.edit_mode = false;
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_v"))]),
        )
        .expect("paste number");
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("Enter"))]),
        )
        .expect("commit pasted number");
        assert_eq!(number.borrow().number_value(), 7.25);

        state.edit_mode = false;
        focus_element(&mut app, &mut state, &options, &combo);
        edit_started.borrow_mut().clear();
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_v"))]),
        )
        .expect("paste key on ComboBox");
        assert!(state.edit_mode);
        assert_eq!(combo.borrow().selected_value(), "one");
        assert_eq!(&*edit_started.borrow(), &["choice"]);

        state.edit_mode = false;
        focus_element(&mut app, &mut state, &options, &proxy);
        edit_started.borrow_mut().clear();
        call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "press_key",
            &Map::from_iter([("key".to_string(), json!("cmd_v"))]),
        )
        .expect("paste key on generated ScrollView proxy");
        assert!(state.edit_mode);
        assert!(state.scope_edit_element.is_none());
        assert_eq!(&*edit_started.borrow(), &["generated_scroll"]);
    }

    #[test]
    fn gui_tcp_server_queues_concurrent_clients_for_ui_thread_dispatch()
    {
        let config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            port: 0,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let server = McpSocketServer::start(&config).expect("GUI MCP server");
        let address = server.local_addr();
        let clients = [11, 22].map(|id|
        {
            thread::spawn(move ||
            {
                let mut stream = TcpStream::connect(address).expect("connect GUI MCP");
                writeln!(
                    stream,
                    "{}",
                    json!({"jsonrpc": "2.0", "id": id, "method": "get_window"}),
                )
                .expect("write GUI MCP request");
                stream.flush().expect("flush GUI MCP request");
                let mut response = String::new();
                BufReader::new(stream)
                    .read_line(&mut response)
                    .expect("read GUI MCP response");
                serde_json::from_str::<Value>(&response).expect("GUI MCP JSON response")
            })
        });

        let mut window = crate::GeneratedWindow::new("root");
        window.title = "Concurrent GUI".to_string();
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut runtime_config = config;
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        for _ in 0..clients.len()
        {
            let request = server
                .recv_timeout(Duration::from_secs(2))
                .expect("queued GUI MCP request");
            let response = dispatch_json_rpc_value(
                &mut app,
                &mut state,
                &options,
                &mut runtime_config,
                &request.request,
            );
            request.respond(response);
        }
        let mut responses = clients.map(|client| client.join().expect("GUI MCP client"));
        responses.sort_by_key(|response| response["id"].as_i64().unwrap_or_default());
        assert_eq!(responses[0]["id"], 11);
        assert_eq!(responses[1]["id"], 22);
        assert_eq!(responses[0]["result"]["title"], "Concurrent GUI");
        assert_eq!(responses[1]["result"]["title"], "Concurrent GUI");
    }

    #[test]
    fn gui_tcp_server_keeps_accepted_stream_blocking_until_request_arrives()
    {
        let config = McpRuntimeConfig
        {
            enabled: true,
            gui: true,
            port: 0,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let server = McpSocketServer::start(&config).expect("GUI MCP server");
        let mut stream = TcpStream::connect(server.local_addr())
            .expect("connect before delayed GUI MCP request");
        thread::sleep(MCP_ACCEPT_POLL_INTERVAL * 10);
        writeln!(
            stream,
            "{}",
            json!({"jsonrpc": "2.0", "id": 31, "method": "get_window"}),
        )
        .expect("write delayed GUI MCP request");
        stream.flush().expect("flush delayed GUI MCP request");

        let request = server
            .recv_timeout(Duration::from_secs(2))
            .expect("accepted stream waited for delayed request bytes");
        let mut window = crate::GeneratedWindow::new("root");
        window.title = "Delayed GUI".to_string();
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut runtime_config = config;
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 30, height: 8 },
        );
        let response = dispatch_json_rpc_value(
            &mut app,
            &mut state,
            &options,
            &mut runtime_config,
            &request.request,
        );
        request.respond(response);

        let mut response = String::new();
        BufReader::new(stream)
            .read_line(&mut response)
            .expect("read delayed GUI MCP response");
        let response = serde_json::from_str::<Value>(&response)
            .expect("delayed GUI MCP JSON response");
        assert_eq!(response["id"], 31);
        assert_eq!(response["result"]["title"], "Delayed GUI");
    }

    #[test]
    fn headless_tcp_accepts_a_later_client_while_the_first_request_is_delayed()
    {
        let config = McpRuntimeConfig
        {
            enabled: true,
            gui: false,
            transport: "tcp".to_string(),
            port: 0,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let server = McpSocketServer::start(&config).expect("headless TCP MCP server");
        let address = server.local_addr();
        let mut delayed = TcpStream::connect(address)
            .expect("connect delayed headless TCP client");
        thread::sleep(MCP_ACCEPT_POLL_INTERVAL * 10);

        let later = thread::spawn(move ||
        {
            let mut stream = TcpStream::connect(address)
                .expect("connect later headless TCP client");
            writeln!(
                stream,
                "{}",
                json!({"jsonrpc": "2.0", "id": 42, "method": "get_window"}),
            )
            .expect("write later headless TCP request");
            stream.flush().expect("flush later headless TCP request");
            let mut response = String::new();
            BufReader::new(stream)
                .read_line(&mut response)
                .expect("read later headless TCP response");
            serde_json::from_str::<Value>(&response)
                .expect("later headless TCP JSON response")
        });

        let request = server
            .recv_timeout(Duration::from_secs(2))
            .expect("later request was accepted before delayed client sent bytes");
        assert_eq!(request.request["id"], 42);
        request.respond(Some(json!({
            "jsonrpc": "2.0",
            "id": 42,
            "result": {"title": "Later TCP"},
        })));
        assert_eq!(
            later.join().expect("later headless TCP client")["result"]["title"],
            "Later TCP",
        );

        writeln!(
            delayed,
            "{}",
            json!({"jsonrpc": "2.0", "id": 41, "method": "get_window"}),
        )
        .expect("write originally delayed headless TCP request");
        delayed.flush().expect("flush originally delayed TCP request");
        let request = server
            .recv_timeout(Duration::from_secs(2))
            .expect("originally delayed request remained live");
        assert_eq!(request.request["id"], 41);
        request.respond(Some(json!({
            "jsonrpc": "2.0",
            "id": 41,
            "result": {"title": "Delayed TCP"},
        })));
        let mut response = String::new();
        BufReader::new(delayed)
            .read_line(&mut response)
            .expect("read originally delayed TCP response");
        assert_eq!(
            serde_json::from_str::<Value>(&response)
                .expect("originally delayed TCP JSON response")["result"]["title"],
            "Delayed TCP",
        );
    }

    #[test]
    fn headless_http_accepts_a_later_client_while_the_first_request_is_delayed()
    {
        fn write_http_request(stream: &mut TcpStream, id: i32)
        {
            let body = json!({
                "jsonrpc": "2.0",
                "id": id,
                "method": "get_window",
            })
            .to_string();
            write!(
                stream,
                "POST /mcp HTTP/1.1\r\n\
                 Host: 127.0.0.1\r\n\
                 Accept: application/json\r\n\
                 Content-Type: application/json\r\n\
                 Content-Length: {}\r\n\r\n{}",
                body.len(),
                body,
            )
            .expect("write headless HTTP request");
            stream.flush().expect("flush headless HTTP request");
        }

        let config = McpRuntimeConfig
        {
            enabled: true,
            gui: false,
            transport: "http".to_string(),
            port: 0,
            action_delay_ms: 0,
            type_delay_ms: 0,
            ..Default::default()
        };
        let server = McpSocketServer::start(&config).expect("headless HTTP MCP server");
        let address = server.local_addr();
        let mut delayed = TcpStream::connect(address)
            .expect("connect delayed headless HTTP client");
        thread::sleep(MCP_ACCEPT_POLL_INTERVAL * 10);

        let later = thread::spawn(move ||
        {
            let mut stream = TcpStream::connect(address)
                .expect("connect later headless HTTP client");
            write_http_request(&mut stream, 52);
            let mut response = String::new();
            stream
                .read_to_string(&mut response)
                .expect("read later headless HTTP response");
            response
        });

        let request = server
            .recv_timeout(Duration::from_secs(2))
            .expect("later HTTP request was accepted before delayed client sent bytes");
        assert_eq!(request.request["id"], 52);
        request.respond(Some(json!({
            "jsonrpc": "2.0",
            "id": 52,
            "result": {"title": "Later HTTP"},
        })));
        let response = later.join().expect("later headless HTTP client");
        assert!(response.starts_with("HTTP/1.1 200 OK\r\n"));
        assert!(response.contains("\"title\":\"Later HTTP\""));

        write_http_request(&mut delayed, 51);
        let request = server
            .recv_timeout(Duration::from_secs(2))
            .expect("originally delayed HTTP request remained live");
        assert_eq!(request.request["id"], 51);
        request.respond(Some(json!({
            "jsonrpc": "2.0",
            "id": 51,
            "result": {"title": "Delayed HTTP"},
        })));
        let mut response = String::new();
        delayed
            .read_to_string(&mut response)
            .expect("read originally delayed HTTP response");
        assert!(response.starts_with("HTTP/1.1 200 OK\r\n"));
        assert!(response.contains("\"title\":\"Delayed HTTP\""));
    }

    #[test]
    fn tools_list_and_schema_expose_reference_metadata_and_schemas()
    {
        let mut window = crate::GeneratedWindow::new("root");
        window.set_mcp_metadata(true, "Root", "source", "Root window");
        let visible = window.add_element(crate::new_button("save", "Save"));
        let hidden = window.add_element(crate::new_label("secret", "Secret"));
        window.set_mcp_element_metadata(vec![
            crate::GeneratedElementMetadata::new("save", "Save the document", true),
            crate::GeneratedElementMetadata::new("secret", "Internal status", false),
        ]);
        window.set_mcp_app_tools(vec![crate::GeneratedAppToolMetadata::from_json(
            "query",
            "Query records",
            r#"{"type":"object","properties":{"id":{"type":"string"}},"required":["id"]}"#,
            r#"{"type":"object","properties":{"ok":{"type":"boolean"}}}"#,
        )]);
        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );

        focus_element(&mut app, &mut state, &options, &hidden);
        assert_eq!(
            call_tool(
                &mut app,
                &mut state,
                &options,
                &mut config,
                "get_focused_element",
                &Map::new(),
            )
            .expect("hidden focused element result"),
            Value::Null,
        );
        assert_eq!(
            call_tool(
                &mut app,
                &mut state,
                &options,
                &mut config,
                "get_state",
                &Map::new(),
            )
            .expect("state with hidden focused element")["focused_element"],
            Value::Null,
        );
        focus_element(&mut app, &mut state, &options, &visible);
        assert_eq!(
            call_tool(
                &mut app,
                &mut state,
                &options,
                &mut config,
                "get_focused_element",
                &Map::new(),
            )
            .expect("visible focused element result")["id"],
            "save",
        );

        let tools = handle_request(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "tools/list",
            &Map::new(),
        )
        .expect("tools list");
        let definitions = tools["tools"].as_array().expect("tool definitions");
        let cut = definitions
            .iter()
            .find(|tool| tool["name"] == "cut_selection")
            .expect("cut_selection");
        assert_eq!(cut["inputSchema"]["required"], json!(["element_id"]));
        let query = definitions
            .iter()
            .find(|tool| tool["name"] == "query")
            .expect("generated app tool");
        assert_eq!(query["description"], "Query records");
        assert_eq!(query["inputSchema"]["required"], json!(["id"]));
        assert_eq!(query["outputSchema"]["properties"]["ok"]["type"], "boolean");

        let reported_window = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_window",
            &Map::new(),
        )
        .expect("window");
        assert_eq!(reported_window["mode"], "normal");
        assert_eq!(reported_window["width"], 1);
        assert_eq!(reported_window["height"], 1);

        let schema = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_schema",
            &Map::new(),
        )
        .expect("schema");
        assert_eq!(schema["window"]["description"], "Root window");
        assert_eq!(schema["window"]["mode"], "normal");
        assert_eq!(schema["window"]["width"], 1);
        assert_eq!(schema["window"]["height"], 1);
        assert_eq!(schema["elements"].as_array().map(Vec::len), Some(1));
        assert_eq!(schema["elements"][0]["description"], "Save the document");
        assert!(schema["elements"][0]["capabilities"]
            .as_array()
            .is_some_and(|items| items.contains(&json!("ui.activate"))));
        assert!(std::rc::Rc::ptr_eq(
            &visible,
            &app.window.find_element("save").expect("visible element"),
        ));
        assert!(std::rc::Rc::ptr_eq(
            &hidden,
            &app.window.find_element("secret").expect("hidden element remains runtime-visible"),
        ));
    }

    #[test]
    fn schema_descends_scroll_view_reusable_rows_without_exposing_the_proxy()
    {
        let mut window = crate::GeneratedWindow::new("root");
        let scroll = window.add_element(crate::new_scroll_view("items", 0));
        let row = crate::new_reusable_element("row", "row");
        let mut row_window = crate::GeneratedWindow::new("row");
        row_window.add_element(crate::new_label("title", "Visible row"));
        row_window.set_mcp_element_metadata(vec![
            crate::GeneratedElementMetadata::new("title", "Row title", true),
        ]);
        row.borrow_mut().set_child_window(row_window);
        scroll.borrow_mut().add_child(row);

        let mut app = TestApp { window };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );

        let schema = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "get_schema",
            &Map::new(),
        )
        .expect("schema with a reusable ScrollView row");
        let ids = schema["elements"]
            .as_array()
            .expect("schema elements")
            .iter()
            .filter_map(|element| element["id"].as_str())
            .collect::<Vec<_>>();
        assert_eq!(ids, ["items", "items[0].title"]);
    }

    #[test]
    fn json_rpc_batches_suppress_successful_notifications()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );
        let request = json!([
            {"jsonrpc": "2.0", "id": 1, "method": "get_viewport"},
            {"jsonrpc": "2.0", "method": "get_viewport"}
        ]);

        let response = dispatch_json_rpc_value(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &request,
        )
        .expect("batch response");

        assert_eq!(response.as_array().map(Vec::len), Some(1));
        assert_eq!(response[0]["id"], 1);
        assert_eq!(response[0]["result"]["width"], 1);
        assert_eq!(response[0]["result"]["height"], 1);

        let viewport = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "set_viewport",
            &Map::from_iter([("width".to_string(), json!(90))]),
        )
        .expect("set viewport");
        assert_eq!(
            viewport,
            json!({"row": 0, "col": 0, "width": 90, "height": 1}),
        );
        assert_eq!(
            config.viewport,
            Some(Rect { row: 0, col: 0, width: 90, height: 1 }),
        );
    }

    #[test]
    fn http_transport_validates_origin_protocol_accept_and_notifications()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );
        let mut request = HttpRequest
        {
            method: "POST".to_string(),
            path: MCP_HTTP_ENDPOINT.to_string(),
            body: json!({"jsonrpc": "2.0", "id": 1, "method": "initialize"}).to_string(),
            ..Default::default()
        };

        let response = handle_http_request(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &request,
        );
        assert!(response.starts_with("HTTP/1.1 200 OK\r\n"));
        assert!(response.contains("MCP-Protocol-Version: 2024-11-05\r\n"));

        request.headers.insert(
            "origin".to_string(),
            "https://example.com".to_string(),
        );
        assert!(handle_http_request(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &request,
        )
        .starts_with("HTTP/1.1 403 Forbidden\r\n"));

        request.headers.clear();
        request.body = json!({"jsonrpc": "2.0", "method": "get_viewport"}).to_string();
        assert!(handle_http_request(
            &mut app,
            &mut state,
            &options,
            &mut config,
            &request,
        )
        .starts_with("HTTP/1.1 202 Accepted\r\n"));
    }

    #[test]
    fn nested_mouse_hit_testing_returns_the_deepest_element()
    {
        let mut child = crate::GeneratedWindow::new("child");
        let input = child.add_element(crate::new_number_input("amount", 0.0));
        input.borrow_mut().frame = Rect { row: 4, col: 7, width: 8, height: 1 };
        let host = crate::new_reusable_element("main", "child");
        host.borrow_mut().frame = Rect { row: 2, col: 3, width: 20, height: 8 };
        host.borrow_mut().set_child_window(child);
        let mut root = crate::GeneratedWindow::new("root");
        root.add_element(host);

        let hit = element_at_window(&root, 10, 4).expect("nested input hit");

        assert!(std::rc::Rc::ptr_eq(&hit, &input));
    }

    #[test]
    fn mouse_targets_and_reusable_activation_match_cpp_control_routing()
    {
        let activated = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let activated_for_callback = activated.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            on_button: Some(std::rc::Rc::new(move |name|
            {
                activated_for_callback.borrow_mut().push(name.to_string());
            })),
            ..Default::default()
        };
        let mut child = crate::GeneratedWindow::new("tile");
        child.set_generated_focusable(true);
        child.set_generated_control_activation("photo");
        let tile = crate::new_reusable_element("tile", "ImageButton");
        tile.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 3 };
        tile.borrow_mut().set_child_window(child);

        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let label = app.window.add_element(crate::new_label("caption", "Label"));
        label.borrow_mut().frame =
            Rect { row: 4, col: 0, width: 5, height: 1 };
        let image = app.window.add_element(crate::new_image(
            "preview",
            "",
            "",
            "contain",
            "fallback",
            "center",
            "middle",
        ));
        image.borrow_mut().frame =
            Rect { row: 5, col: 0, width: 5, height: 2 };
        app.window.add_element(tile.clone());

        let targets = mouse_target_elements(&app.window, None);
        assert_eq!(targets.len(), 1);
        assert!(std::rc::Rc::ptr_eq(&targets[0], &tile));

        let viewport = Size { width: 10, height: 8 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);
        perform_mouse_press(&mut app, &mut state, &options, viewport, 1, 1);

        assert_eq!(&*activated.borrow(), &["tile.photo"]);
        assert!(!state.edit_mode);
        assert!(state.mouse_click_candidate.is_none());
    }

    #[test]
    fn mouse_click_candidate_dispatches_only_on_unmoved_inside_release()
    {
        let activated = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let activated_for_callback = activated.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            on_button: Some(std::rc::Rc::new(move |name|
            {
                activated_for_callback.borrow_mut().push(name.to_string());
            })),
            ..Default::default()
        };
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let image = app.window.add_element(crate::new_image(
            "preview",
            "",
            "",
            "contain",
            "fallback",
            "center",
            "middle",
        ));
        image.borrow_mut().frame =
            Rect { row: 2, col: 3, width: 5, height: 2 };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 12, height: 6 },
        );

        state.mouse_click_candidate = Some(image.clone());
        assert_eq!(
            perform_mouse_release(&mut app, &mut state, &options, 4, 2),
            None,
        );
        assert_eq!(&*activated.borrow(), &["preview"]);

        state.mouse_click_candidate = Some(image.clone());
        perform_mouse_move(&app, &mut state, 4, 2);
        assert_eq!(
            perform_mouse_release(&mut app, &mut state, &options, 4, 2),
            None,
        );
        state.mouse_click_candidate = Some(image);
        assert_eq!(
            perform_mouse_release(&mut app, &mut state, &options, 0, 0),
            None,
        );
        assert_eq!(&*activated.borrow(), &["preview"]);
        assert!(state.mouse_click_candidate.is_none());
        assert!(!state.mouse_click_candidate_moved);
    }

    #[test]
    fn list_box_mouse_press_selects_row_and_hides_keyboard_overlay_like_cpp()
    {
        let changes = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let changes_for_callback = changes.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            on_selection_changed: Some(std::rc::Rc::new(move |name, values|
            {
                changes_for_callback
                    .borrow_mut()
                    .push((name.to_string(), values.to_vec()));
            })),
            ..Default::default()
        };
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let list = app.window.add_element(crate::new_list_box(
            "items",
            vec!["One".to_string(), "Two".to_string(), "Three".to_string()],
            false,
        ));
        list.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 8, height: 3 };
        list.borrow_mut().show_active_item();
        let viewport = Size { width: 8, height: 3 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);

        perform_mouse_press(&mut app, &mut state, &options, viewport, 1, 1);

        assert_eq!(list.borrow().selected_index(), 1);
        assert_eq!(list.borrow().active_index(), 1);
        assert!(!list.borrow().active_item_visible());
        assert!(state.edit_mode);
        assert_eq!(
            &*changes.borrow(),
            &[("items".to_string(), vec!["Two".to_string()])],
        );
    }

    #[test]
    fn mouse_press_enters_the_scroll_view_under_the_pointer_before_hit_testing()
    {
        let activated = std::rc::Rc::new(std::cell::RefCell::new(Vec::new()));
        let activated_for_callback = activated.clone();
        let options = GeneratedWindowRuntimeOptions
        {
            on_button: Some(std::rc::Rc::new(move |name|
            {
                activated_for_callback.borrow_mut().push(name.to_string());
            })),
            ..Default::default()
        };
        let scroll = crate::new_scroll_view("items", 0);
        scroll.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 10, height: 3 };
        let show = crate::new_button("show", "Show");
        show.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 1 };
        scroll.borrow_mut().add_child(show.clone());
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(scroll.clone());
        let viewport = Size { width: 10, height: 3 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);

        perform_mouse_press(&mut app, &mut state, &options, viewport, 1, 0);

        assert_eq!(&*activated.borrow(), &["show"]);
        assert!(state.edit_mode);
        assert!(state.scope_edit_element.as_ref().is_some_and(|scope|
        {
            std::rc::Rc::ptr_eq(scope, &scroll)
        }));
        assert!(std::rc::Rc::ptr_eq(
            &state
                .focused_element(&app.window)
                .expect("button focused in the pointer ScrollView"),
            &show,
        ));
    }

    #[test]
    fn mouse_targets_descend_into_the_generated_scroll_view_behind_its_host()
    {
        let mut row = crate::GeneratedWindow::new("row");
        let show = row.add_element(crate::new_button("show", "Show"));
        show.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 6, height: 1 };
        row.set_generated_layout(vec![crate::LayoutItem
        {
            element_type: "button".to_string(),
            cell_chars_width: 10,
            cell_chars_height: 1,
            cell_width: 10,
            cell_height: 1,
            width: 6,
            height: 1,
            chars_width: 6,
            chars_height: 1,
            content: "show".to_string(),
            ..Default::default()
        }]);
        let mut hidden_row = crate::GeneratedWindow::new("hidden row");
        let hidden = hidden_row.add_element(crate::new_button("hidden", "Hidden"));
        hidden.borrow_mut().frame =
            Rect { row: 8, col: 8, width: 6, height: 1 };
        hidden_row.set_generated_layout(vec![crate::LayoutItem
        {
            element_type: "button".to_string(),
            cell_chars_width: 10,
            cell_chars_height: 1,
            cell_width: 10,
            cell_height: 1,
            width: 6,
            height: 1,
            chars_width: 6,
            chars_height: 1,
            content: "hidden".to_string(),
            ..Default::default()
        }]);
        let mut scroll_window =
            crate::GeneratedWindow::new_scroll_view_with_gap("items", 3);
        scroll_window.scroll_view().borrow_mut().frame =
            Rect { row: 3, col: 30, width: 10, height: 3 };
        let row_host = scroll_window.add_named_child_window("items[0]", row);
        row_host.borrow_mut().frame =
            Rect { row: 0, col: 0, width: 10, height: 1 };
        let hidden_host =
            scroll_window.add_named_child_window("items[1]", hidden_row);
        hidden_host.borrow_mut().frame =
            Rect { row: 8, col: 8, width: 10, height: 1 };
        let scroll_host = crate::new_reusable_element("items", "Items");
        scroll_host.borrow_mut().frame =
            Rect { row: 3, col: 30, width: 10, height: 3 };
        scroll_host.borrow_mut().set_child_window(scroll_window);

        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(scroll_host.clone());
        crate::runtime::sync_reusable_child_frames(&scroll_host);
        assert_eq!(hidden_host.borrow().frame, Rect::default());
        let targets = mouse_target_elements(&app.window, Some(&scroll_host));
        let target = mouse_target_element(&targets, 31, 3)
            .expect("visible generated ScrollView button target");
        assert!(std::rc::Rc::ptr_eq(
            &target,
            &show,
        ));
        assert_eq!(
            show.borrow().frame,
            Rect { row: 3, col: 30, width: 6, height: 1 },
        );
        assert!(!std::rc::Rc::ptr_eq(&target, &hidden));
    }

    #[test]
    fn selectable_label_mouse_drag_returns_selected_text()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let label = app.window.add_element(crate::new_label("copy", "hello"));
        {
            let mut value = label.borrow_mut();
            value.frame = Rect { row: 0, col: 0, width: 5, height: 1 };
            value.style.user_select = Some("text".to_string());
        }
        let options = GeneratedWindowRuntimeOptions::default();
        let viewport = Size { width: 5, height: 1 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);

        perform_mouse_press(&mut app, &mut state, &options, viewport, 1, 0);
        perform_mouse_move(&app, &mut state, 4, 0);

        assert_eq!(
            perform_mouse_release(&mut app, &mut state, &options, 4, 0)
                .as_deref(),
            Some("ell"),
        );
    }

    #[test]
    fn number_input_mouse_press_uses_number_edit_cursor_without_text_selection()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let amount = app.window.add_element(crate::new_number_input("amount", 0.0));
        amount.borrow_mut().frame = Rect { row: 0, col: 0, width: 8, height: 1 };
        let options = GeneratedWindowRuntimeOptions::default();
        let viewport = Size { width: 8, height: 1 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);

        perform_mouse_press(&mut app, &mut state, &options, viewport, 1, 0);

        assert!(state.edit_mode);
        assert!(state.mouse_selection_element.is_none());
        assert!(state.handle_key(&mut app, &options, "7"));
        assert!(state.handle_key(&mut app, &options, "Enter"));
        assert_eq!(amount.borrow().number_value(), 7.0);
    }

    #[test]
    fn scroll_view_drag_uses_the_reference_cross_child_screen_selection()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let scroll = crate::new_scroll_view("items", 0);
        scroll.borrow_mut().frame = Rect { row: 0, col: 0, width: 3, height: 2 };
        for (name, text) in [("first", "one"), ("second", "two")]
        {
            let label = crate::new_label(name, text);
            {
                let mut value = label.borrow_mut();
                value.frame = Rect { row: 0, col: 0, width: 3, height: 1 };
                value.style.user_select = Some("text".to_string());
            }
            scroll.borrow_mut().add_child(label);
        }
        app.window.add_element(scroll);
        app.window.set_generated_layout(vec![crate::LayoutItem
        {
            element_type: "uiscrollview".to_string(),
            cell_chars_width: 3,
            cell_chars_height: 2,
            cell_width: 3,
            cell_height: 2,
            cell_width_mode: "auto".to_string(),
            cell_height_mode: "auto".to_string(),
            width: 3,
            height: 2,
            width_mode: "auto".to_string(),
            height_mode: "auto".to_string(),
            chars_width: 3,
            chars_height: 2,
            content: "items".to_string(),
            ..Default::default()
        }]);
        let options = GeneratedWindowRuntimeOptions::default();
        let viewport = Size { width: 3, height: 2 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);

        perform_mouse_press(&mut app, &mut state, &options, viewport, 0, 0);
        perform_mouse_move(&app, &mut state, 2, 1);

        assert!(state.scroll_selection.changed);
        assert_eq!(
            perform_mouse_release(&mut app, &mut state, &options, 2, 1)
                .as_deref(),
            Some("one\ntwo"),
        );
        assert!(!state.scroll_selection.active);
    }

    #[test]
    fn text_area_drag_outside_scrolls_before_extending_the_selection()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let area = app.window.add_element(crate::new_text_area(
            "description",
            "zero\none\ntwo\nthree",
        ));
        area.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 2 };
        let options = GeneratedWindowRuntimeOptions::default();
        let viewport = Size { width: 5, height: 2 };
        let mut state = RuntimeState::new(&app.window, &options, viewport);
        area.borrow_mut().set_cursor(0);
        state.mouse_selection_element = Some(area.clone());
        state.mouse_selection_anchor = 0;

        perform_mouse_move(&app, &mut state, 0, 2);

        assert_eq!(area.borrow().selected_text(), "zero\none\n");
        let rendered = area.borrow().render(5, 2, true, true);
        assert!(rendered.plain_text().starts_with("one"));
    }

    #[test]
    fn scroll_hit_testing_uses_rendered_absolute_row_frames_once()
    {
        let mut row_window = crate::GeneratedWindow::new("row");
        let paid = row_window.add_element(crate::new_checkbox("paid", "Done", false));
        paid.borrow_mut().frame = Rect { row: 14, col: 31, width: 9, height: 1 };
        let row = crate::new_reusable_element("item", "row");
        row.borrow_mut().frame = Rect { row: 14, col: 29, width: 57, height: 3 };
        row.borrow_mut().set_child_window(row_window);

        let scroll = crate::GeneratedWindow::new_scroll_view("items");
        scroll
            .generated_scroll_view()
            .expect("generated scroll view")
            .add_child(row);

        let hit = element_at_scroll_window(
            &scroll,
            Rect { row: 14, col: 28, width: 58, height: 18 },
            35,
            14,
        )
        .expect("visible checkbox hit");

        assert!(std::rc::Rc::ptr_eq(&hit, &paid));
    }

    #[test]
    fn scroll_routes_to_a_reusable_generated_scroll_view()
    {
        let mut scroll = crate::GeneratedWindow::new_scroll_view("items");
        for index in 0..3
        {
            let mut row = crate::GeneratedWindow::new(format!("row-{index}"));
            let label = row.add_element(crate::new_label("label", &format!("Row {index}")));
            label.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 1 };
            scroll.add_named_child_window(format!("item-{index}"), row);
        }
        let viewport = Size { width: 5, height: 1 };
        scroll.scroll_to_top_in(viewport);
        assert_eq!(scroll.view_offset(), 2);

        let host = crate::new_reusable_element("items", "TaskList");
        host.borrow_mut().frame = Rect { row: 0, col: 0, width: 5, height: 1 };
        host.borrow_mut().set_child_window(scroll);

        scroll_element(&host, -1).expect("scroll routed");

        assert_eq!(
            host.borrow()
                .child_window()
                .expect("scroll child")
                .view_offset(),
            1,
        );
    }

    #[test]
    fn focusing_a_generated_scroll_descendant_makes_it_visible()
    {
        let mut scroll = crate::GeneratedWindow::new_scroll_view("items");
        let mut target = None;
        for index in 0..6
        {
            let mut row = crate::GeneratedWindow::new(format!("row-{index}"));
            row.set_generated_layout(vec![crate::LayoutItem
            {
                element_type: "button".to_string(),
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
                content: "show".to_string(),
                ..Default::default()
            }]);
            let button = row.add_element(crate::new_button("show", "Show"));
            if index == 5
            {
                target = Some(button);
            }
            scroll.add_named_child_window(format!("item-{index}"), row);
        }
        let viewport = Size { width: 8, height: 2 };
        scroll.scroll_to_top_in(viewport);
        let initial_view_offset = scroll.view_offset();

        let host = crate::new_reusable_element("items", "ImageList");
        host.borrow_mut().frame =
            Rect { row: 0, col: 0, width: viewport.width, height: viewport.height };
        host.borrow_mut().set_child_window(scroll);
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(host.clone());
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(&app.window, &options, viewport);
        let target = target.expect("final row button");

        focus_element(&mut app, &mut state, &options, &target);

        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("focused descendant"),
            &target,
        ));
        assert!(state.scope_edit_element.as_ref().is_some_and(|scope|
        {
            std::rc::Rc::ptr_eq(scope, &host)
        }));
        assert!(
            host.borrow()
                .child_window()
                .expect("generated ScrollView")
                .view_offset()
                < initial_view_offset,
        );
    }

    #[test]
    fn snapshots_active_generated_scroll_descendant_after_focus_resolution()
    {
        let mut scroll = crate::GeneratedWindow::new_scroll_view("items");
        let mut row = crate::GeneratedWindow::new("row");
        row.set_generated_layout(vec![crate::LayoutItem
        {
            element_type: "checkbox".to_string(),
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
            content: "done".to_string(),
            ..Default::default()
        }]);
        let done = row.add_element(crate::new_checkbox("done", "Done", false));
        scroll.add_named_child_window("items[0]", row);

        let host = crate::new_reusable_element("items", "TaskList");
        host.borrow_mut().frame = Rect { row: 0, col: 0, width: 20, height: 4 };
        host.borrow_mut().set_child_window(scroll);
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(host);
        let options = GeneratedWindowRuntimeOptions::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 4 },
        );

        focus_element(&mut app, &mut state, &options, &done);
        let snapshot = snapshot_element(&app, &state, &done);

        assert_eq!(snapshot["focused"], true);
        assert_eq!(snapshot["id"], "items[0].done");
    }

    #[test]
    fn combo_box_mouse_selection_exits_its_scroll_scope()
    {
        let mut scroll = crate::GeneratedWindow::new_scroll_view("items");
        let category = scroll.add_element(crate::new_combo_box(
            "category",
            vec!["Food".to_string(), "Transport".to_string()],
        ));
        category.borrow_mut().frame = Rect { row: 1, col: 1, width: 10, height: 1 };
        let host = crate::new_reusable_element("items", "Expenses");
        host.borrow_mut().frame = Rect { row: 0, col: 0, width: 20, height: 10 };
        host.borrow_mut().set_child_window(scroll);

        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        app.window.add_element(host.clone());
        let options = GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "items".to_string(),
            ..Default::default()
        };
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 10 },
        );
        state.scope_edit_element = Some(host.clone().into());
        assert!(state.focus_element(&app.window, &category));
        state.scope_dim_element = Some(category.clone());
        state.edit_mode = true;

        assert!(activate_combo_box_option_at(
            &mut app,
            &mut state,
            &options,
            2,
            3,
        ));
        assert_eq!(category.borrow().selected_index(), 1);
        assert!(!state.edit_mode);
        assert!(state.scope_edit_element.is_none());
        assert!(state.scope_dim_element.is_none());
        assert!(std::rc::Rc::ptr_eq(
            &state.focused_element(&app.window).expect("restored scroll proxy focus"),
            &host,
        ));
    }

    #[test]
    fn set_value_dispatches_choice_selection_without_text_value_access()
    {
        let mut app = TestApp { window: crate::GeneratedWindow::new("root") };
        let status = app.window.add_element(crate::new_combo_box(
            "status",
            vec!["Any".to_string(), "Todo".to_string()],
        ));
        let options = GeneratedWindowRuntimeOptions::default();
        let mut config = McpRuntimeConfig::default();
        let mut state = RuntimeState::new(
            &app.window,
            &options,
            Size { width: 20, height: 5 },
        );
        let arguments = json!({
            "element_id": "status",
            "value": "Todo",
        })
        .as_object()
        .expect("object arguments")
        .clone();

        let result = call_tool(
            &mut app,
            &mut state,
            &options,
            &mut config,
            "ui.set_value",
            &arguments,
        )
        .expect("set value");

        assert_eq!(status.borrow().selected_value(), "Todo");
        assert_eq!(result["value"], "Todo");
    }
}

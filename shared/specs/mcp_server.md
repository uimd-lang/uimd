# MCP Server

The MCP server is the generated UI runtime control layer. It lets tests and
agents inspect a running terminal UI, drive it through the same interaction
model as a user, and compare rendered output across backends.

It is not an application-specific API. The server must stay generic and must
work through the active runtime window stack.

## Runtime Model

- A generated app runs normally unless started with `--mcp-server`.
- With `--mcp-server`, the app creates an MCP server beside the UI event loop.
- In GUI mode, the server runs in the background while the app continues to
  render and process terminal input.
- In headless mode, the app does not draw to the terminal and is driven only by
  MCP requests.
- MCP actions operate on the active top window. If a dialog or modal is open,
  tools address that top window, not a hidden parent window.

## Transports

Python and C++ support the same transport names:

- `tcp` for the project-internal visual tester and local debug clients
- `stdio` for local headless MCP clients that spawn the app as a subprocess
- `http` for Streamable HTTP request/response clients

Default transport selection:

- `--mcp-server` in GUI mode defaults to `tcp`
- `--mcp-server --headless` defaults to `stdio`
- `--mcp-server --mcp-transport http` starts the HTTP endpoint

`tcp` is maintained for the universal visual tester. New external MCP
integrations should prefer `stdio` for local subprocess use and Streamable HTTP
for network use.

The visual tester entrypoint defaults to the C++ tester in this repository.
Pass `--backend python` to run the packaged Python reference/fallback tester:

```bash
uimd mcp-test APP_PATH YAML_CONFIG
uimd-mcp-test APP_PATH YAML_CONFIG
uimd-mcp-test --backend python APP_PATH YAML_CONFIG
```

Common startup flags:

```bash
--mcp-server
--gui
--headless
--mcp-transport tcp
--mcp-host 127.0.0.1
--mcp-port 8765
--mcp-fast
--mcp-action-delay-ms 500
--mcp-type-delay-ms 50
--mcp-wait-render
--mcp-controlled-render
--viewport row,col,width,height
```

Examples:

```bash
python3 python/examples/formular/formular.py --mcp-server
python3 python/examples/formular/formular.py --mcp-server --headless
python3 python/examples/formular/formular.py --mcp-server --mcp-transport http
cpp/build/examples/formular/formular --mcp-server --headless
cpp/build/examples/formular/formular --mcp-server --mcp-transport http
```

`--mcp-fast` is a convenience shortcut for agent/test automation. It sets the
default MCP action delay and type delay to `0`. Explicit
`--mcp-action-delay-ms` and `--mcp-type-delay-ms` values have priority over
`--mcp-fast`.

## Protocol Shape

The server speaks JSON-RPC-style newline-delimited JSON.

Supported MCP-style methods:

- `initialize`
- `tools/list`
- `tools/call`

For internal and test tooling, direct method names are also accepted. A method
named `ui.get_window` maps to the same controller tool as `get_window`.

`tools/call` returns one text content item whose text is JSON containing the
tool result.

JSON-RPC batch requests are supported on all transports. A batch request is a
top-level JSON array of normal JSON-RPC requests:

```json
[
  {
    "jsonrpc": "2.0",
    "method": "tools/call",
    "params": {
      "name": "ui.fill",
      "arguments": {
        "element_id": "name_input",
        "text": "Ada Lovelace"
      }
    }
  },
  {
    "jsonrpc": "2.0",
    "id": 2,
    "method": "tools/call",
    "params": {
      "name": "ui.get_value",
      "arguments": {
        "element_id": "name_input"
      }
    }
  }
]
```

Batch items execute sequentially in array order so UI-mutating calls can be
followed by reads in the same batch. Responses are returned as a JSON array and
only include requests that have an `id`; notifications without `id` do not
produce response items. If every batch item is a notification, the server
returns no JSON-RPC body (`202 Accepted` over HTTP). Invalid batch payloads
return JSON-RPC error responses.

For HTTP, the MCP endpoint is `/mcp`. The initial implementation handles
JSON-RPC request/response calls through HTTP `POST` and returns
`application/json`. It accepts `Accept: application/json, text/event-stream`,
validates supported `MCP-Protocol-Version` values, and rejects non-local
`Origin` headers by default. SSE streaming is not implemented yet.

## Controller

The server itself only accepts requests and serializes responses. Runtime work
belongs to the controller.

The controller:

- reads the active window and element tree
- returns accessibility and render snapshots
- focuses and activates elements
- sends key input through runtime handlers
- edits text controls using runtime behavior
- resizes the app viewport
- waits or repaints after actions when configured

Tool names are backend contract names. Python and C++ must expose the same
public tools for shared MCP scripts and backend comparison.
There are currently no documented Python-only or C++-only MCP tools.

Legacy core tools:

- `get_window`
- `get_elements`
- `get_element`
- `get_value`
- `get_options`
- `get_schema`
- `get_source`
- `get_focused_element`
- `get_edit_mode`
- `get_state`
- `get_accessibility_snapshot`
- `get_render_snapshot`
- `get_render_snapshot_compact`
- `get_text_snapshot`
- `get_viewport`
- `get_render_rect`
- `set_viewport`
- `repaint`
- `focus_element`
- `enter_edit_mode`
- `exit_edit_mode`
- `activate_element`
- `click_element`
- `check`
- `uncheck`
- `press_key`
- `mouse_click`
- `mouse_drag`
- `scroll`
- `set_text`
- `set_value`
- `set_checked`
- `select_option`
- `set_selection`
- `paste_text`
- `type_text`
- `replace_selection`
- `clear_text`
- `move_cursor`
- `set_cursor`
- `select_text`
- `select_all`
- `copy_selection`
- `cut_selection`
- `get_text`
- `get_selection`
- `get_cursor`

Canonical UI tools use the `ui.*` namespace. Existing legacy names stay
available for compatibility.

Canonical agent-facing tools:

- `ui.get_window`
- `ui.get_elements`
- `ui.get_element`
- `ui.get_value`
- `ui.get_options`
- `ui.get_schema`
- `ui.get_source`
- `ui.get_focused_element`
- `ui.snapshot`
- `ui.render_snapshot`
- `ui.text_snapshot`
- `ui.focus`
- `ui.activate`
- `ui.click`
- `ui.press`
- `ui.set_value`
- `ui.fill`
- `ui.clear`
- `ui.select_option`
- `ui.set_selection`
- `ui.set_checked`
- `ui.check`
- `ui.uncheck`
- `ui.set_cursor`
- `ui.select_text`
- `ui.copy_selection`
- `ui.cut_selection`

`ui.get_schema` returns the active window, all elements, their current values,
selection options, and per-element capabilities. `ui.get_value(element_id)`
returns only the current value. `ui.get_options(element_id)` returns only the
available options for selection controls such as `ComboBox` and `ListBox`.
`ui.set_value(element_id, value)` sets a control directly in one command:
text and number inputs are replaced without per-character typing, checkboxes
accept booleans, and selection controls accept exact option values.
`ui.set_selection(element_id, values)` replaces a `ListBox` selection with exact
option values. Multi-value selection is accepted only for listboxes declared
with `multiple: true`; single-select listboxes accept one value.

The legacy pointer/scroll tools remain part of the shared Python/C++ contract:
`mouse_click(x, y)`, `mouse_drag(from_x, from_y, to_x, to_y)`, and
`scroll(element_id, delta)`. They are intentionally not mirrored as canonical
`ui.*` names yet. New agent-facing flows should prefer element-id tools such as
`ui.click`, `ui.select_option`, `ui.set_checked`, and `ui.set_value` whenever
those tools express the intended action.

Legacy tool names are compatibility aliases. New agents and external clients
should prefer the canonical `ui.*` names. Legacy names must not be removed or
renamed in the same migration; removal requires a separate explicit migration
after tests and clients have moved to canonical names.

`ui.get_source` is an explicit debug/provenance tool. It returns the embedded
source Markdown for the active generated window. Normal agent discovery should
use `ui.get_schema` so raw source Markdown is not sent in ordinary responses.

## Metadata

Generated UI files embed the original Markdown source when MCP is enabled.
MCP metadata is derived from the section-based Markdown UI format:

````markdown
# Form

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Main app window."
```

## Members

```yaml
input:
  type: textinput
  description: "Text input."
debug_label:
  type: label
  text: Debug
  expose: false
```
````

`Metadata.description` becomes the window description returned by
`ui.get_window`. Element descriptions come from member `description`, then
fallback to `title`, then `text`, then the member ID. Element roles are derived
from `type`; version 1 of the source format has no manual role override.

`expose: false` excludes an element from agent-facing element lists and schemas.
Direct element-id calls remain available for deterministic tests and runtime
introspection.

This metadata is used by accessibility snapshots and agent-facing inspection. It
must not change layout or runtime behavior.

## Viewport

Viewport support lets an external tester embed one or more app processes inside
bounded terminal rectangles.

The runtime must:

- render only inside the assigned viewport
- clip all output to that rectangle
- center normal windows inside the viewport
- expand fullscreen windows only inside the viewport
- translate mouse coordinates from terminal space to viewport-local space
- keep keyboard behavior unchanged

`set_viewport(row, col, width, height)` updates the runtime viewport, resizes
the active window as needed, marks the app dirty, and stabilizes rendering.

## Snapshots

`get_render_snapshot` returns:

- ANSI lines
- plain text lines
- expanded render cells

`get_render_snapshot_compact` returns compact cell arrays for backend parity.
Each cell is represented as:

```text
[character, foreground, background, attributes]
```

Backend parity compares the full assigned viewport, not only the natural window
content. Empty surrounding panel cells are part of the comparison.

## Tester Relationship

The C++ MCP tester is the default repository consumer of this server. It starts
each tested app as a separate process, assigns an MCP port, passes the viewport
flags, drives the app through MCP tools, and logs assertions. The Python
implementation remains available as `uimd.testing.mcp_tester` for
reference/fallback use. The repository keeps `tools/mcp_tester/mcp_tester.py`
as a launcher that defaults to C++ and accepts `--backend python`.

In compare mode, the tester drives multiple backends with the same YAML script
and compares snapshots from the same logical step.

## Architecture Rules

- Do not add app-specific MCP bridge callbacks.
- Do not special-case examples in the MCP server.
- Do not bypass the active window/modal stack.
- Do not let MCP metadata affect visual design.
- Keep Python and C++ tool names and result shapes compatible.
- Shared behavior belongs in runtime, generator, shared specs, or tester
  infrastructure, not in individual examples.

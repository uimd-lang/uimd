"""Tests for MCP-style UI control support."""

import json
import os
import re
import sys
import types
import unittest
from http import HTTPStatus

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from runtime import UIWindow
from runtime.application import UIApplication
from runtime.mcp import MCPController, MCPRuntimeConfig, MCPServer, parse_agent_metadata, parse_mcp_runtime_args


EXPECTED_MCP_TOOL_NAMES = {
    "activate_element",
    "check",
    "clear_text",
    "click_element",
    "copy_selection",
    "cut_selection",
    "enter_edit_mode",
    "exit_edit_mode",
    "focus_element",
    "get_accessibility_snapshot",
    "get_cursor",
    "get_edit_mode",
    "get_element",
    "get_elements",
    "get_focused_element",
    "get_options",
    "get_render_frame",
    "get_render_rect",
    "get_render_snapshot",
    "get_render_snapshot_compact",
    "get_schema",
    "get_selection",
    "get_source",
    "get_state",
    "get_text",
    "get_text_snapshot",
    "get_value",
    "get_viewport",
    "get_window",
    "mouse_click",
    "mouse_drag",
    "move_cursor",
    "paste_text",
    "press_key",
    "repaint",
    "replace_selection",
    "scroll",
    "select_all",
    "select_option",
    "select_text",
    "set_checked",
    "set_cursor",
    "set_selection",
    "set_text",
    "set_value",
    "set_viewport",
    "type_text",
    "ui.activate",
    "ui.check",
    "ui.clear",
    "ui.click",
    "ui.copy_selection",
    "ui.cut_selection",
    "ui.fill",
    "ui.focus",
    "ui.get_element",
    "ui.get_elements",
    "ui.get_focused_element",
    "ui.get_options",
    "ui.get_schema",
    "ui.get_source",
    "ui.get_value",
    "ui.get_window",
    "ui.press",
    "ui.render_snapshot",
    "ui.select_option",
    "ui.select_text",
    "ui.set_checked",
    "ui.set_cursor",
    "ui.set_selection",
    "ui.set_value",
    "ui.snapshot",
    "ui.text_snapshot",
    "ui.uncheck",
    "uncheck",
}


class TestMCPMetadata(unittest.TestCase):
    def test_parse_agent_metadata(self):
        source = """# Demo

## Metadata

```yaml
format: ui-md
format-version: 1
kind: window
description: "Demo window"
```

## Members

```yaml
name_input:
  type: textinput
  description: "Name field"
secret:
  type: label
  text: Hidden
  expose: false
```

## User Interface

```ui
+------------+
| name_input |
| secret     |
+------------+
```
"""
        metadata = parse_agent_metadata(source)
        self.assertEqual(metadata["window"]["description"], "Demo window")
        self.assertEqual(metadata["elements"]["name_input"]["role"], "input")
        self.assertEqual(metadata["elements"]["name_input"]["description"], "Name field")
        self.assertFalse(metadata["elements"]["secret"]["expose"])

    def test_parse_agent_metadata_description_fallback(self):
        source = """# Demo

## Metadata

```yaml
format: ui-md
format-version: 1
kind: window
description: "MCP described window"
```

## Members

```yaml
save_btn:
  type: button
  title: Save
```

## User Interface

```ui
+----------+
| save_btn |
+----------+
```
"""
        metadata = parse_agent_metadata(source)
        self.assertEqual(metadata["window"]["description"], "MCP described window")
        self.assertEqual(metadata["elements"]["save_btn"]["role"], "action")
        self.assertEqual(metadata["elements"]["save_btn"]["description"], "Save")


class TestMCPRuntimeArgs(unittest.TestCase):
    def test_gui_defaults_keep_existing_delays(self):
        config = parse_mcp_runtime_args(["--mcp-server"])
        self.assertTrue(config.gui)
        self.assertEqual(config.action_delay_ms, 500)
        self.assertEqual(config.type_delay_ms, 50)

    def test_headless_defaults_to_zero_delays(self):
        config = parse_mcp_runtime_args(["--mcp-server", "--headless"])
        self.assertFalse(config.gui)
        self.assertEqual(config.action_delay_ms, 0)
        self.assertEqual(config.type_delay_ms, 0)

    def test_mcp_fast_sets_delay_defaults_to_zero(self):
        config = parse_mcp_runtime_args(["--mcp-server", "--mcp-fast"])
        self.assertTrue(config.gui)
        self.assertEqual(config.action_delay_ms, 0)
        self.assertEqual(config.type_delay_ms, 0)

    def test_explicit_delays_override_mcp_fast(self):
        config = parse_mcp_runtime_args([
            "--mcp-server",
            "--mcp-fast",
            "--mcp-action-delay-ms",
            "25",
            "--mcp-type-delay-ms",
            "7",
        ])
        self.assertEqual(config.action_delay_ms, 25)
        self.assertEqual(config.type_delay_ms, 7)


DEMO_MCP_SOURCE = """# Demo

## Metadata

```yaml
format: ui-md
format-version: 1
kind: window
description: "Demo form"
```

## Members

```yaml
name_input:
  type: textinput
  value: Old
  description: "Name"
age_input:
  type: numberinput
  value: 7
  description: "Age"
ok_button:
  type: button
  title: OK
  description: "Confirm"
notify_check:
  type: checkbox
  title: Notify
  description: "Notification toggle"
country_combo:
  type: combobox
  options: ["Slovakia", "Czech Republic", "Poland"]
  selected_item: Slovakia
  description: "Country selector"
role_listbox:
  type: listbox
  options: ["Developer", "Designer", "Manager"]
  selected_items: ["Developer"]
  multiple: true
  description: "Role selector"
hidden_status:
  type: label
  text: Hidden
  expose: false
```
"""


def _demo_layout_entry(name, elem_type, row, height=1):
    return {
        "name": name,
        "type": elem_type,
        "row": row,
        "col": 0,
        "relative_row": row,
        "relative_col": 0,
        "cell_row": 0,
        "cell_col": 0,
        "cell_chars_width": 22,
        "cell_chars_height": 9,
        "cell_width": 22,
        "cell_height": 9,
        "cell_width_mode": "auto",
        "cell_height_mode": "auto",
        "chars_width": 20,
        "chars_height": height,
        "width": 20,
        "height": height,
        "margin_right": 2,
        "margin_bottom": max(0, 9 - row - height),
    }


class DemoUI(UIWindow):
    _mcp_enabled = True
    _mcp_source_md = DEMO_MCP_SOURCE
    _mcp_agent_metadata = {
        "window": {"description": "Demo form"},
        "elements": {
            "name_input": {"role": "input", "description": "Name", "expose": True},
            "age_input": {"role": "input", "description": "Age", "expose": True},
            "ok_button": {"role": "action", "description": "Confirm", "expose": True},
            "notify_check": {"role": "toggle", "description": "Notification toggle", "expose": True},
            "country_combo": {"role": "selection", "description": "Country selector", "expose": True},
            "role_listbox": {"role": "selection", "description": "Role selector", "expose": True},
            "hidden_status": {"role": "text", "description": "Hidden", "expose": False},
        },
    }
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Demo",
            layout=[
                _demo_layout_entry("name_input", "textinput", 0),
                _demo_layout_entry("age_input", "numberinput", 1),
                _demo_layout_entry("notify_check", "checkbox", 2),
                _demo_layout_entry("country_combo", "combobox", 3),
                _demo_layout_entry("role_listbox", "listbox", 4, height=3),
                _demo_layout_entry("hidden_status", "label", 7),
                _demo_layout_entry("ok_button", "button", 8),
            ],
            members={},
            style={},
        )
        self.name_input = self.create_element("name_input", "textinput", value="Old")
        self.age_input = self.create_element("age_input", "numberinput", value=7)
        self.ok_button = self.create_element("ok_button", "button", title="OK")
        self.notify_check = self.create_element("notify_check", "checkbox", title="Notify")
        self.country_combo = self.create_element(
            "country_combo",
            "combobox",
            options=["Slovakia", "Czech Republic", "Poland"],
            selected_item="Slovakia",
        )
        self.role_listbox = self.create_element(
            "role_listbox",
            "listbox",
            options=["Developer", "Designer", "Manager"],
            selected_items=["Developer"],
            multiple=True,
        )
        self.hidden_status = self.create_element("hidden_status", "label", text="Hidden")


class TestMCPController(unittest.TestCase):
    def setUp(self):
        self.app = UIApplication(width=20, height=4)
        self.window = DemoUI()
        self.app.open(self.window)
        self.controller = MCPController(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, action_delay_ms=0, type_delay_ms=0),
        )

    def test_inspection_uses_embedded_metadata(self):
        window = self.controller.tool_get_window()
        self.assertEqual(window["description"], "Demo form")

        element = self.controller.tool_get_element("name_input")
        self.assertEqual(element["role"], "input")
        self.assertEqual(element["description"], "Name")
        self.assertEqual(element["value"], "Old")

    def test_expose_false_hides_element_from_agent_lists(self):
        element_ids = {element["id"] for element in self.controller.tool_get_elements()}
        self.assertNotIn("hidden_status", element_ids)
        self.assertEqual(self.controller.tool_get_element("hidden_status")["description"], "Hidden")

    def test_set_text_replaces_existing_text(self):
        result = self.controller.tool_set_text("name_input", "New")
        self.assertEqual(result["value"], "New")
        self.assertEqual(self.window.name_input.value, "New")
        self.assertEqual(self.controller.tool_get_text("name_input"), {"text": "New"})

    def test_new_ui_aliases_call_existing_tools(self):
        self.assertEqual(
            self.controller.call_tool("ui.fill", {"element_id": "name_input", "text": "Alias"})["value"],
            "Alias",
        )
        self.assertEqual(
            self.controller.call_tool("ui.get_value", {"element_id": "name_input"}),
            {"value": "Alias"},
        )

    def test_call_tool_runs_under_app_render_lock(self):
        class RecordingLock:
            def __init__(self):
                self.active = False
                self.entered = 0
                self.exited = 0

            def __enter__(self):
                self.active = True
                self.entered += 1

            def __exit__(self, _exc_type, _exc, _tb):
                self.active = False
                self.exited += 1

        lock = RecordingLock()
        observed = []
        self.app._render_lock = lock

        def locked_tool():
            observed.append(lock.active)
            return {"ok": True}

        self.controller.tool_get_window = locked_tool

        self.assertEqual(self.controller.call_tool("get_window"), {"ok": True})
        self.assertEqual(observed, [True])
        self.assertEqual(lock.entered, 1)
        self.assertEqual(lock.exited, 1)

    def test_set_checked_is_idempotent(self):
        self.assertFalse(self.window.notify_check.value)

        result = self.controller.call_tool("ui.set_checked", {
            "element_id": "notify_check",
            "checked": True,
        })

        self.assertTrue(result["value"])
        self.assertTrue(self.window.notify_check.value)
        self.controller.call_tool("ui.check", {"element_id": "notify_check"})
        self.assertTrue(self.window.notify_check.value)
        self.controller.call_tool("ui.uncheck", {"element_id": "notify_check"})
        self.assertFalse(self.window.notify_check.value)

    def test_set_value_sets_controls_directly(self):
        self.assertEqual(
            self.controller.call_tool("ui.set_value", {
                "element_id": "name_input",
                "value": "Direct",
            })["value"],
            "Direct",
        )
        self.assertEqual(self.window.name_input.cursor_pos, len("Direct"))

        self.assertEqual(
            self.controller.call_tool("ui.set_value", {
                "element_id": "age_input",
                "value": 42,
            })["value"],
            42.0,
        )

        self.assertTrue(
            self.controller.call_tool("ui.set_value", {
                "element_id": "notify_check",
                "value": True,
            })["value"]
        )

        self.assertEqual(
            self.controller.call_tool("ui.set_value", {
                "element_id": "country_combo",
                "value": "Czech Republic",
            })["value"],
            "Czech Republic",
        )

        self.assertEqual(
            self.controller.call_tool("ui.set_value", {
                "element_id": "role_listbox",
                "value": ["Designer"],
            })["value"],
            ["Designer"],
        )

        self.assertEqual(
            self.controller.call_tool("ui.set_selection", {
                "element_id": "role_listbox",
                "values": ["Developer", "Manager"],
            })["value"],
            ["Developer", "Manager"],
        )

    def test_select_option_for_combobox_and_listbox(self):
        combo = self.controller.call_tool("ui.select_option", {
            "element_id": "country_combo",
            "value": "Poland",
        })
        self.assertEqual(combo["value"], "Poland")
        self.assertEqual(self.controller.call_tool("ui.get_value", {"element_id": "country_combo"}), {"value": "Poland"})
        self.assertEqual(
            self.controller.call_tool("ui.get_options", {"element_id": "country_combo"}),
            {"options": ["Slovakia", "Czech Republic", "Poland"]},
        )

        listbox = self.controller.call_tool("ui.select_option", {
            "element_id": "role_listbox",
            "value": "Manager",
        })
        self.assertEqual(listbox["value"], ["Manager"])
        self.assertEqual(self.controller.call_tool("ui.get_value", {"element_id": "role_listbox"}), {"value": ["Manager"]})

    def test_get_schema_returns_capabilities(self):
        schema = self.controller.call_tool("ui.get_schema")
        elements = {element["id"]: element for element in schema["elements"]}
        self.assertIn("ui.set_value", elements["name_input"]["capabilities"])
        self.assertIn("ui.fill", elements["name_input"]["capabilities"])
        self.assertIn("ui.set_checked", elements["notify_check"]["capabilities"])
        self.assertIn("ui.set_value", elements["country_combo"]["capabilities"])
        self.assertIn("ui.select_option", elements["country_combo"]["capabilities"])
        self.assertEqual(elements["role_listbox"]["options"], ["Developer", "Designer", "Manager"])

    def test_get_source_returns_explicit_provenance(self):
        source = self.controller.call_tool("ui.get_source")
        self.assertEqual(source["class"], self.window.__class__.__name__)
        self.assertTrue(source["mcp_enabled"])
        self.assertIn("role_listbox", source["source_markdown"])

    def test_tools_list_exposes_canonical_names(self):
        names = {tool["name"] for tool in self.controller.list_tools()}
        self.assertEqual(names, EXPECTED_MCP_TOOL_NAMES)

    def test_tool_inventory_has_input_schemas_for_action_arguments(self):
        tools = {tool["name"]: tool["inputSchema"] for tool in self.controller.list_tools()}
        self.assertEqual(tools["mouse_click"]["required"], ["x", "y"])
        self.assertEqual(tools["mouse_drag"]["required"], ["from_x", "from_y", "to_x", "to_y"])
        self.assertEqual(tools["scroll"]["required"], ["element_id", "delta"])
        self.assertEqual(tools["ui.fill"]["required"], ["element_id", "text"])

    def test_app_specific_tools_are_listed_and_dispatched(self):
        self.window._mcp_app_tools = {
            "query_projects": {
                "name": "query_projects",
                "description": "Return filtered project records.",
                "inputSchema": {
                    "type": "object",
                    "properties": {"name": {"type": "string"}},
                },
                "outputSchema": {
                    "type": "object",
                    "properties": {"projects": {"type": "array"}},
                },
            }
        }

        def query_projects(window, input=None):
            return {"projects": [{"id": "p1", "name": input.get("name", "")}]}

        self.window.query_projects = types.MethodType(query_projects, self.window)

        tools = {tool["name"]: tool for tool in self.controller.list_tools()}
        self.assertEqual(tools["query_projects"]["description"], "Return filtered project records.")
        self.assertIn("outputSchema", tools["query_projects"])
        self.assertEqual(
            self.controller.call_tool("query_projects", {"name": "Apollo"}),
            {"projects": [{"id": "p1", "name": "Apollo"}]},
        )

    def test_cpp_runtime_declares_matching_tool_inventory(self):
        source_path = os.path.join(
            os.path.dirname(__file__),
            "..",
            "..",
            "cpp",
            "src",
            "generated",
            "GeneratedWindowRuntime.cpp",
        )
        with open(source_path, encoding="utf-8") as f:
            source = f.read()
        match = re.search(
            r"mcpToolNames\(\).*?std::vector<std::string> names\{(?P<body>.*?)\};",
            source,
            flags=re.S,
        )
        self.assertIsNotNone(match)
        names = set(re.findall(r'"([^"]+)"', match.group("body")))
        self.assertEqual(names, EXPECTED_MCP_TOOL_NAMES)

    def test_select_all_and_paste_text(self):
        self.controller.tool_select_all("name_input")
        result = self.controller.tool_paste_text("name_input", "Pasted")
        self.assertEqual(result["value"], "Pasted")

    def test_cursor_selection_copy_cut_and_replace(self):
        self.controller.tool_set_cursor("name_input", 1)
        self.assertEqual(self.controller.tool_get_cursor("name_input"), {"cursor": 1})

        self.controller.tool_select_text("name_input", 1, 3)
        self.assertEqual(self.controller.tool_get_selection("name_input"), {
            "start": 1,
            "end": 3,
            "text": "ld",
        })
        self.assertEqual(self.controller.tool_copy_selection("name_input"), {"text": "ld"})
        self.assertEqual(self.controller.tool_cut_selection("name_input"), {"text": "ld"})
        self.assertEqual(self.window.name_input.value, "O")

        result = self.controller.tool_replace_selection("name_input", "K")
        self.assertEqual(result["value"], "OK")

    def test_text_snapshot_contains_rendered_content(self):
        snapshot = self.controller.tool_get_text_snapshot()
        self.assertTrue(any("Old" in line for line in snapshot["lines"]))

    def test_render_snapshot_contains_cells(self):
        snapshot = self.controller.tool_get_render_snapshot()
        self.assertIn("cells", snapshot)
        self.assertTrue(snapshot["cells"])
        self.assertIn("char", snapshot["cells"][0][0])
        self.assertIn("foreground", snapshot["cells"][0][0])
        self.assertIn("background", snapshot["cells"][0][0])
        self.assertIn("attributes", snapshot["cells"][0][0])

    def test_compact_render_snapshot_contains_array_cells(self):
        snapshot = self.controller.tool_get_render_snapshot_compact()
        self.assertEqual(snapshot["format"], "render-cells-v1")
        self.assertIn("cells", snapshot)
        self.assertTrue(snapshot["cells"])
        self.assertIsInstance(snapshot["cells"][0][0], list)
        self.assertEqual(len(snapshot["cells"][0][0]), 4)

    def test_render_snapshot_uses_full_viewport_size(self):
        self.controller.tool_set_viewport(2, 3, 40, 10)

        snapshot = self.controller.tool_get_render_snapshot()

        self.assertEqual(len(snapshot["cells"]), 10)
        self.assertTrue(all(len(row) == 40 for row in snapshot["cells"]))
        self.assertEqual(len(snapshot["text_lines"]), 10)
        self.assertTrue(all(len(line) == 40 for line in snapshot["text_lines"]))

    def test_jsonrpc_tools_call(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, action_delay_ms=0, type_delay_ms=0),
        )
        response = server.handle_json_line(
            '{"jsonrpc":"2.0","id":1,"method":"tools/call",'
            '"params":{"name":"get_text","arguments":{"element_id":"name_input"}}}'
        )
        self.assertEqual(response["id"], 1)
        self.assertIn('"Old"', response["result"]["content"][0]["text"])

    def test_jsonrpc_batch_runs_sequentially_and_omits_notifications(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, action_delay_ms=0, type_delay_ms=0),
        )
        response = server.handle_json_line(json.dumps([
            {
                "jsonrpc": "2.0",
                "method": "tools/call",
                "params": {
                    "name": "ui.fill",
                    "arguments": {"element_id": "name_input", "text": "Batch"},
                },
            },
            {
                "jsonrpc": "2.0",
                "id": 2,
                "method": "tools/call",
                "params": {
                    "name": "ui.get_value",
                    "arguments": {"element_id": "name_input"},
                },
            },
            "invalid",
        ]))

        self.assertEqual(len(response), 2)
        self.assertEqual(response[0]["id"], 2)
        self.assertIn('"Batch"', response[0]["result"]["content"][0]["text"])
        self.assertIsNone(response[1]["id"])
        self.assertIn("request must be an object", response[1]["error"]["message"])

    def test_jsonrpc_empty_batch_returns_error(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, action_delay_ms=0, type_delay_ms=0),
        )
        response = server.handle_json_line("[]")

        self.assertIsNone(response["id"])
        self.assertIn("empty array", response["error"]["message"])

    def test_http_post_tools_call(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, transport="http", action_delay_ms=0, type_delay_ms=0),
        )

        status, headers, payload = server.handle_http_post(
            "/mcp",
            {"Accept": "application/json, text/event-stream"},
            (
                b'{"jsonrpc":"2.0","id":1,"method":"tools/call",'
                b'"params":{"name":"get_text","arguments":{"element_id":"name_input"}}}'
            ),
        )

        self.assertEqual(status, HTTPStatus.OK)
        self.assertEqual(headers["Content-Type"], "application/json")
        response = json.loads(payload.decode("utf-8"))
        self.assertEqual(response["id"], 1)
        self.assertIn('"Old"', response["result"]["content"][0]["text"])

    def test_http_post_rejects_invalid_protocol_version(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, transport="http", action_delay_ms=0, type_delay_ms=0),
        )

        status, _headers, payload = server.handle_http_post(
            "/mcp",
            {"Accept": "application/json", "MCP-Protocol-Version": "invalid"},
            b'{"jsonrpc":"2.0","id":1,"method":"initialize"}',
        )

        self.assertEqual(status, HTTPStatus.BAD_REQUEST)
        self.assertEqual(payload, b"")

    def test_http_post_rejects_non_local_origin(self):
        server = MCPServer(
            self.app,
            MCPRuntimeConfig(enabled=True, gui=False, transport="http", action_delay_ms=0, type_delay_ms=0),
        )

        status, _headers, payload = server.handle_http_post(
            "/mcp",
            {"Accept": "application/json", "Origin": "https://example.com"},
            b'{"jsonrpc":"2.0","id":1,"method":"initialize"}',
        )

        self.assertEqual(status, HTTPStatus.FORBIDDEN)
        self.assertEqual(payload, b"")

    def test_viewport_tools_update_runtime_viewport(self):
        viewport = self.controller.tool_set_viewport(2, 3, 40, 10)
        self.assertEqual(viewport, {"row": 2, "col": 3, "width": 40, "height": 10})
        self.assertEqual(self.controller.tool_get_viewport(), viewport)

if __name__ == "__main__":
    unittest.main()

"""Tests for the universal MCP tester app configuration."""

import os
import re
import subprocess
import sys
import tempfile
import threading
import unittest
from unittest.mock import patch

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "src"))

from uimd.testing import mcp_tester_launcher
from uimd.testing.mcp_tester import (
    DEFAULT_COMMAND_TIMEOUT_SECONDS,
    DEFAULT_TYPE_DELAY_MS,
    CONTROLLED_ANIMATION_REPAINT_SECONDS,
    LIVE_BUFFER_REPAINT_INTERVAL_SECONDS,
    PAUSE_BUTTON_TITLE,
    PLAY_BUTTON_TITLE,
    TesterConfig,
    TEXT_TIMEOUT_MULTIPLIER,
    TestScript,
    McpTester,
    TargetApp,
    _compact_result,
    _assertion_matches,
    _format_target_value,
    _comparison_assertion_fields,
    _load_yaml_test_config,
    _load_yaml_test_scripts,
    _render_snapshot_mismatch,
    _run_tester_file_step,
    _select_snapshot_fields,
    parse_args,
)
from uimd.testing.log_panel import LogPanel
import uimd.runtime.elements as elements_module


ALL_EXAMPLE_SCRIPT_COUNT = 13
PARITY_COMPARE_APP_SIZE = "40x12"
PARITY_TIMEOUT_SECONDS = 60


PROJECT_ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), "..", ".."))
CPP_MCP_TESTER_BINARY = os.path.join(
    PROJECT_ROOT,
    "cpp",
    "build",
    "tools",
    "mcp_tester",
    "uimd_mcp_tester",
)
PARITY_TMP_ROOT = os.path.join(PROJECT_ROOT, "tests", "mcp", ".tmp")
LOG_TIMESTAMP_RE = re.compile(r"^(?:\x1b\[0m)?\d\d:\d\d:\d\d\s+")
PORT_RE = re.compile(r"port=\d+")
DOUBLE_QUOTED_ASSERT_RE = re.compile(r'== "([^"]*)"')
ELEMENT_ID_ARGUMENT_RE = re.compile(r'\{\s*"element_id"\s*:\s*"([^"]+)"\s*\}')
STABLE_PARITY_LOG_PREFIXES = (
    "loaded ",
    "SCRIPT ",
    "spawn ",
    "connected ",
    "viewports ",
    "calculator: get_window",
    "calculator: get_element",
    "ASSERT ",
    "SUMMARY",
    "RESULTS:",
    "PASS:",
    "RESULT:",
)


def _mcp_tester_parity_env():
    env = os.environ.copy()
    pythonpath = [
        os.path.join(PROJECT_ROOT, "python"),
        os.path.join(PROJECT_ROOT, "src"),
    ]
    if env.get("PYTHONPATH"):
        pythonpath.append(env["PYTHONPATH"])
    env["PYTHONPATH"] = os.pathsep.join(pythonpath)
    return env


def _stable_mcp_tester_log_lines(path):
    with open(path, encoding="utf-8") as handle:
        raw_lines = handle.read().splitlines()

    lines = []
    for line in raw_lines:
        normalized = LOG_TIMESTAMP_RE.sub("", line)
        normalized = normalized.replace(PROJECT_ROOT + os.sep, "")
        normalized = PORT_RE.sub("port=<port>", normalized)
        normalized = DOUBLE_QUOTED_ASSERT_RE.sub(r"== '\1'", normalized)
        normalized = ELEMENT_ID_ARGUMENT_RE.sub(r'{"element_id":"\1"}', normalized)
        if normalized.startswith(STABLE_PARITY_LOG_PREFIXES):
            lines.append(normalized)
    return lines


class TestMcpTesterConfig(unittest.TestCase):
    def _temporary_compare_example_roots(self):
        os.makedirs(PARITY_TMP_ROOT, exist_ok=True)
        temp_dir = tempfile.TemporaryDirectory(prefix="compare_examples_", dir=PARITY_TMP_ROOT)
        self.addCleanup(temp_dir.cleanup)
        python_root = os.path.join(temp_dir.name, "python", "examples")
        cpp_root = os.path.join(temp_dir.name, "cpp", "build", "examples")
        scripts = _load_yaml_test_scripts(
            PROJECT_ROOT,
            os.path.join(PROJECT_ROOT, "tests", "mcp", "all_examples.yaml"),
        )
        for script in scripts:
            name = next(iter(script.apps))
            python_app_dir = os.path.join(python_root, name)
            cpp_app_dir = os.path.join(cpp_root, name)
            os.makedirs(python_app_dir, exist_ok=True)
            os.makedirs(cpp_app_dir, exist_ok=True)
            with open(os.path.join(python_app_dir, f"{name}.py"), "w", encoding="utf-8") as handle:
                handle.write("# test fixture\n")
            with open(os.path.join(cpp_app_dir, name), "w", encoding="utf-8") as handle:
                handle.write("# test fixture\n")
        return python_root, cpp_root

    def test_config_argument_is_required(self):
        with self.assertRaises(SystemExit):
            parse_args([])

    def test_launcher_defaults_to_cpp_tester_binary(self):
        binary = os.path.join("/repo", "cpp", "build", "tools", "mcp_tester", "uimd_mcp_tester")
        with patch("uimd.testing.mcp_tester_launcher._project_root", return_value="/repo"), \
             patch("uimd.testing.mcp_tester_launcher.os.path.exists", return_value=True), \
             patch("uimd.testing.mcp_tester_launcher.subprocess.call", return_value=0) as run_binary:
            result = mcp_tester_launcher.main_argv(["tests/mcp/calculator.yaml"])

        self.assertEqual(result, 0)
        run_binary.assert_called_once_with([binary, "tests/mcp/calculator.yaml"], cwd="/repo")

    def test_launcher_can_select_python_reference_backend(self):
        with patch("uimd.testing.mcp_tester.main", return_value=5) as python_tester:
            result = mcp_tester_launcher.main_argv([
                "--backend",
                "python",
                "tests/mcp/calculator.yaml",
            ])

        self.assertEqual(result, 5)
        python_tester.assert_called_once_with(["tests/mcp/calculator.yaml"])

    def test_python_and_cpp_tester_backends_have_small_script_parity(self):
        self.maxDiff = None
        if not os.path.exists(CPP_MCP_TESTER_BINARY):
            self.skipTest("C++ MCP tester binary is not built")

        os.makedirs(PARITY_TMP_ROOT, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix="backend_parity_", dir=PARITY_TMP_ROOT) as temp_dir:
            yaml_path = os.path.join(temp_dir, "calculator_backend_parity.yaml")
            with open(yaml_path, "w", encoding="utf-8") as handle:
                handle.write(
                    "- name: calculator\n"
                    "  app: python/examples/calculator/calculator.py\n"
                    "  tests:\n"
                    "    - tool: get_window\n"
                    "      assert:\n"
                    "        title: Calculator\n"
                    "    - tool: get_element\n"
                    "      arguments:\n"
                    "        element_id: display\n"
                    "      assert:\n"
                    "        value: \"0\"\n"
                )

            python_result = self._run_mcp_tester_backend("python", yaml_path, temp_dir)
            cpp_result = self._run_mcp_tester_backend("cpp", yaml_path, temp_dir)

            self.assertEqual(python_result.returncode, 0, python_result.stderr)
            self.assertEqual(cpp_result.returncode, 0, cpp_result.stderr)

            python_lines = _stable_mcp_tester_log_lines(python_result.log_path)
            cpp_lines = _stable_mcp_tester_log_lines(cpp_result.log_path)

            self.assertEqual(python_lines, cpp_lines)
            self.assertIn("viewports calculator=0,0 40x12", python_lines)
            self.assertIn("ASSERT calculator.title == 'Calculator'", python_lines)
            self.assertIn("ASSERT calculator.value == '0'", python_lines)
            self.assertTrue(any("PASS 2 asserts passed, 0 failed, 0 step failures" in line for line in python_lines))
            self.assertIn("RESULT: 2 asserts passed, 0 failed, 0 step failures", python_lines)

    def _run_mcp_tester_backend(self, backend, yaml_path, temp_dir):
        log_path = os.path.join(temp_dir, f"{backend}.log")
        command = [
            sys.executable,
            "tools/mcp_tester/mcp_tester.py",
            "--backend",
            backend,
            yaml_path,
            "--mcp-fast",
            "--plain",
            "--exit-on-finish",
            "--log-file",
            log_path,
            "--compare-app-size",
            PARITY_COMPARE_APP_SIZE,
        ]
        result = subprocess.run(
            command,
            cwd=PROJECT_ROOT,
            env=_mcp_tester_parity_env(),
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=PARITY_TIMEOUT_SECONDS,
        )
        result.log_path = log_path
        return result

    def test_project_root_defaults_to_current_external_project(self):
        from uimd.testing.mcp_tester import _project_root

        previous = os.getcwd()
        with tempfile.TemporaryDirectory() as temp_dir:
            try:
                os.chdir(temp_dir)
                self.assertEqual(_project_root(), os.path.realpath(temp_dir))
            finally:
                os.chdir(previous)

    def test_yaml_config_defines_apps_and_steps(self):
        config = parse_args([
            "python/examples/calculator/calculator.py",
            "tests/mcp/calculator.yaml",
        ])
        self.assertEqual(
            config.apps,
            {"calculator": os.path.join(config.root, "python/examples/calculator/calculator.py")},
        )
        self.assertEqual(config.source_path, "tests/mcp/calculator.yaml")
        self.assertEqual(len(config.steps), 33)
        self.assertEqual(config.steps[0]["target"], "calculator")
        self.assertEqual(config.steps[0]["tool"], "activate_element")
        self.assertEqual(config.steps[-1]["assert"], {"value": "0.5"})
        self.assertEqual(config.setup_steps, [])
        self.assertEqual(config.cleanup_steps, [])
        self.assertEqual(config.env, {"calculator": {}})
        self.assertIsNone(config.log_path)
        self.assertFalse(config.exit_on_finish)

    def test_yaml_config_can_supply_app_paths_without_override(self):
        config = parse_args(["tests/mcp/calculator.yaml"])

        self.assertEqual(
            config.apps,
            {"calculator": os.path.join(config.root, "python/examples/calculator/calculator.py")},
        )
        self.assertEqual(config.source_path, "tests/mcp/calculator.yaml")

    def test_exit_on_finish_is_accepted_with_config(self):
        config = parse_args([
            "python/examples/calculator/calculator.py",
            "tests/mcp/calculator.yaml",
            "--exit-on-finish",
        ])
        self.assertTrue(config.exit_on_finish)

    def test_mcp_fast_zeroes_all_tester_delays(self):
        config = parse_args([
            "--mcp-fast",
            "tests/mcp/calculator.yaml",
        ])

        self.assertEqual(config.action_delay_ms, 0)
        self.assertEqual(config.type_delay_ms, 0)
        self.assertEqual(config.step_delay_seconds, 0)

    def test_mcp_fast_keeps_explicit_delay_overrides(self):
        config = parse_args([
            "--mcp-fast",
            "--mcp-action-delay-ms",
            "7",
            "--mcp-type-delay-ms",
            "11",
            "--step-delay-ms",
            "13",
            "tests/mcp/calculator.yaml",
        ])

        self.assertEqual(config.action_delay_ms, 7)
        self.assertEqual(config.type_delay_ms, 11)
        self.assertEqual(config.step_delay_seconds, 0.013)

    def test_positional_app_path_replaces_single_yaml_app_path(self):
        config = parse_args([
            "cpp/build/examples/calculator/calculator",
            "tests/mcp/calculator.yaml",
        ])

        self.assertEqual(
            config.apps,
            {"calculator": os.path.join(config.root, "cpp/build/examples/calculator/calculator")},
        )
        self.assertEqual(config.steps[0]["target"], "calculator")

    def test_named_app_path_replaces_single_yaml_app_path(self):
        config = parse_args([
            "--app",
            "cpp/build/examples/calculator/calculator",
            "tests/mcp/calculator.yaml",
        ])

        self.assertEqual(
            config.apps,
            {"calculator": os.path.join(config.root, "cpp/build/examples/calculator/calculator")},
        )

    def test_compare_cli_runs_single_yaml_against_two_apps(self):
        config = parse_args([
            "--compare",
            "python/examples/calculator/calculator.py",
            "cpp/build/examples/calculator/calculator",
            "tests/mcp/calculator.yaml",
        ])

        self.assertTrue(config.compare_mode)
        self.assertEqual(
            config.apps,
            {
                "python": os.path.join(config.root, "python/examples/calculator/calculator.py"),
                "cpp": os.path.join(config.root, "cpp/build/examples/calculator/calculator"),
            },
        )
        self.assertEqual(config.steps[0]["target"], "*")
        self.assertEqual(config.steps[0]["tool"], "activate_element")
        self.assertEqual(config.steps[-1]["target"], "*")
        self.assertEqual(config.steps[-1]["assert"], {"value": "0.5"})
        self.assertEqual(config.env, {"python": {}, "cpp": {}})

    def test_compare_cli_rejects_yaml_with_multiple_apps(self):
        root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
        script = TestScript(
            "unit.yaml",
            {"py": "python/examples/calculator/calculator.py", "cpp": "cpp/build/examples/calculator/calculator"},
            [{"target": "*", "tool": "get_state"}],
        )

        with self.assertRaises(ValueError):
            from uimd.testing.mcp_tester import _compare_app_paths

            _compare_app_paths(root, [script], [
                "python/examples/calculator/calculator.py",
                "cpp/build/examples/calculator/calculator",
            ])

    def test_app_override_rejects_include_config_with_multiple_scripts(self):
        with self.assertRaises(ValueError):
            parse_args([
                "cpp/build/examples/calculator/calculator",
                "tests/mcp/all_examples.yaml",
            ])

    def test_examples_root_replaces_each_included_script_app_path(self):
        config = parse_args([
            "python/examples",
            "tests/mcp/all_examples.yaml",
        ])

        self.assertEqual(len(config.scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(
            config.scripts[0].apps,
            {"calculator": os.path.join(config.root, "python/examples/calculator/calculator.py")},
        )

    def test_compare_examples_roots_replace_each_included_script_app_path(self):
        python_root, cpp_root = self._temporary_compare_example_roots()

        config = parse_args([
            "--compare",
            python_root,
            cpp_root,
            "tests/mcp/all_examples.yaml",
        ])

        self.assertTrue(config.compare_mode)
        self.assertEqual(len(config.scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(
            config.scripts[0].apps,
            {
                "python": os.path.join(python_root, "calculator", "calculator.py"),
                "cpp": os.path.join(cpp_root, "calculator", "calculator"),
            },
        )
        self.assertEqual(config.scripts[0].steps[0]["target"], "*")
        self.assertEqual(
            config.scripts[-1].apps,
            {
                "python": os.path.join(python_root, "expense_tracker", "expense_tracker.py"),
                "cpp": os.path.join(cpp_root, "expense_tracker", "expense_tracker"),
            },
        )

    def test_all_uses_known_examples_config(self):
        config = parse_args(["--all"])

        self.assertEqual(config.source_path, "tests/mcp/all_examples.yaml")
        self.assertEqual(len(config.scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(
            config.scripts[0].apps,
            {"calculator": os.path.join(config.root, "python/examples/calculator/calculator.py")},
        )

    def test_all_compare_uses_examples_roots(self):
        python_root, cpp_root = self._temporary_compare_example_roots()

        config = parse_args([
            "--all",
            "--compare",
            python_root,
            cpp_root,
        ])

        self.assertTrue(config.compare_mode)
        self.assertEqual(len(config.scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(
            config.scripts[0].apps,
            {
                "python": os.path.join(python_root, "calculator", "calculator.py"),
                "cpp": os.path.join(cpp_root, "calculator", "calculator"),
            },
        )

    def test_target_timeout_scales_with_text_length(self):
        target = TargetApp("calculator", "python/examples/calculator/calculator.py", os.getcwd())
        target.action_delay_ms = 120
        target.type_delay_ms = DEFAULT_TYPE_DELAY_MS

        short_timeout = target._request_timeout("tools/call", {
            "name": "get_window",
            "arguments": {},
        })
        long_timeout = target._request_timeout("tools/call", {
            "name": "set_text",
            "arguments": {"text": "x" * 200},
        })

        self.assertEqual(short_timeout, DEFAULT_COMMAND_TIMEOUT_SECONDS)
        self.assertEqual(long_timeout, DEFAULT_COMMAND_TIMEOUT_SECONDS)
        self.assertGreaterEqual(
            long_timeout,
            (200 * DEFAULT_TYPE_DELAY_MS / 1000.0) * TEXT_TIMEOUT_MULTIPLIER,
        )

    def test_target_timeout_can_be_overridden_by_step(self):
        target = TargetApp("calculator", "python/examples/calculator/calculator.py", os.getcwd())

        self.assertEqual(
            target._request_timeout("tools/call", {"name": "get_window"}, timeout_ms=120000),
            120.0,
        )
        self.assertIsNone(
            target._request_timeout("tools/call", {"name": "get_window"}, timeout_ms=0)
        )

    def test_parallel_target_calls_suppress_intermediate_output_for_plain_tools(self):
        class FakeTarget:
            def __init__(self, name):
                self.name = name
                self.forwarding = []

            def set_output_forwarding(self, enabled):
                self.forwarding.append(bool(enabled))

            def wait_output_idle(self):
                self.forwarding.append("idle")

            def call_tool(self, tool, arguments, timeout_ms=None):
                self.forwarding.append(f"{self.name}:{tool}:{self.forwarding[-1]}")
                return {"id": self.name, "value": arguments["value"]}

        script = TestScript("unit.yaml", {"a": "a.py", "b": "b.py"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=[],
            scripts=[script],
        )
        tester = McpTester(config)
        tester.targets = {"a": FakeTarget("a"), "b": FakeTarget("b")}

        arguments = {"a": {"value": "ok"}, "b": {"value": "ok"}}
        result = tester._call_targets(["a", "b"], "press_key", arguments, None)

        self.assertEqual(result, {"a": {"id": "a", "value": "ok"}, "b": {"id": "b", "value": "ok"}})
        self.assertEqual(tester.targets["a"].forwarding[0], False)
        self.assertIn("a:press_key:False", tester.targets["a"].forwarding)
        self.assertEqual(tester.targets["a"].forwarding[-2], "idle")
        self.assertEqual(tester.targets["a"].forwarding[-1], True)
        self.assertEqual(tester.targets["b"].forwarding[0], False)
        self.assertIn("b:press_key:False", tester.targets["b"].forwarding)
        self.assertEqual(tester.targets["b"].forwarding[-2], "idle")
        self.assertEqual(tester.targets["b"].forwarding[-1], True)

    def test_target_placeholder_expands_recursively(self):
        value = {
            "path": "tests/mcp/.tmp/{platform}/file.txt",
            "items": ["created-{target}.txt"],
        }

        self.assertEqual(
            _format_target_value(value, "cpp"),
            {
                "path": "tests/mcp/.tmp/cpp/file.txt",
                "items": ["created-cpp.txt"],
            },
        )

    def test_platform_specific_assert_fields_are_not_compared(self):
        assertion = {
            "value": "created-{platform}.txt",
            "class": "FileBrowser",
        }

        self.assertEqual(_comparison_assertion_fields(assertion), ["class"])

    def test_assertion_operators_support_contains_and_regex(self):
        self.assertEqual(_assertion_matches("Saved successfully", {"contains": "success"}), (True, "contains 'success'"))
        self.assertEqual(_assertion_matches("Contact #42 saved", {"regex": r"#[0-9]+"}), (True, "matches /#[0-9]+/"))
        self.assertEqual(_assertion_matches("42", "42"), (True, "== '42'"))

    def test_render_snapshot_mismatch_compares_cells(self):
        base = {
            "cells": [[
                {"char": "A", "foreground": "#ffffff", "background": "#000000", "attributes": []},
            ]],
        }
        same = {
            "cells": [[
                {"char": "A", "foreground": "#ffffff", "background": "#000000", "attributes": []},
            ]],
        }
        changed = {
            "cells": [[
                {"char": "B", "foreground": "#ffffff", "background": "#000000", "attributes": []},
            ]],
        }

        self.assertIsNone(_render_snapshot_mismatch(base, same))
        self.assertIn("row 0 col 0", _render_snapshot_mismatch(base, changed))

    def test_render_snapshot_mismatch_ignores_space_foreground(self):
        base = {
            "cells": [[
                {"char": " ", "foreground": "#ffffff", "background": "#000000", "attributes": []},
            ]],
        }
        same_visual = {
            "cells": [[
                {"char": " ", "foreground": None, "background": "#000000", "attributes": []},
            ]],
        }

        self.assertIsNone(_render_snapshot_mismatch(base, same_visual))

    def test_render_snapshot_mismatch_compares_compact_cells(self):
        base = {"format": "render-cells-v1", "cells": [[["A", "#ffffff", "#000000", []]]]}
        changed = {"format": "render-cells-v1", "cells": [[["A", "#ffffff", "#111111", []]]]}

        self.assertIsNone(_render_snapshot_mismatch(base, base))
        self.assertIn("bg='#000000'", _render_snapshot_mismatch(base, changed))

    def test_compare_mode_compares_render_snapshot_after_tool_step(self):
        class CompareTester(McpTester):
            def __init__(self, config):
                super().__init__(config)
                self.compared = []

            def _target_names(self, target_name):
                return ["python", "cpp"]

            def _call_targets(self, target_names, tool, arguments_by_target, timeout_ms):
                if tool == "activate_element":
                    return {target_name: {"ok": True} for target_name in target_names}
                if tool == "get_render_snapshot_compact":
                    self.compared.append(tuple(target_names))
                    return {
                        target_name: {
                            "format": "render-cells-v1",
                            "cells": [[["A", None, None, []]]],
                        }
                        for target_name in target_names
                    }
                raise AssertionError(tool)

            def _sync_protected_rects(self):
                pass

            def _repaint_targets(self):
                pass

        script = TestScript("unit.yaml", {"python": "py.py", "cpp": "cpp"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=[],
            compare_mode=True,
            scripts=[script],
        )
        tester = CompareTester(config)
        tester.targets = {"python": object(), "cpp": object()}

        tester._run_tool_step({"target": "*", "tool": "activate_element", "arguments": {"element_id": "n1"}})

        self.assertEqual(tester.compared, [("python", "cpp")])

    def test_yaml_loader_accepts_multiple_root_items(self):
        root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
        path = os.path.join(root, "tests", "mcp", "calculator.yaml")
        apps, steps, setup_steps, cleanup_steps, env = _load_yaml_test_config(root, path)
        self.assertEqual(list(apps), ["calculator"])
        self.assertEqual(steps[0]["target"], "calculator")
        self.assertEqual(setup_steps, [])
        self.assertEqual(cleanup_steps, [])
        self.assertEqual(env, {"calculator": {}})

    def test_yaml_loader_accepts_include_scripts(self):
        root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
        path = os.path.join(root, "tests", "mcp", "all_examples.yaml")

        scripts = _load_yaml_test_scripts(root, path)

        self.assertEqual(len(scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(scripts[0].name, "calculator")
        self.assertEqual(scripts[-1].name, "expense_tracker")

        config = parse_args(["python/examples", "tests/mcp/all_examples.yaml"])
        self.assertEqual(len(config.scripts), ALL_EXAMPLE_SCRIPT_COUNT)
        self.assertEqual(config.scripts[0].source_path, "tests/mcp/calculator.yaml")

    def test_tester_file_steps_create_assert_and_remove_files(self):
        root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
        test_dir = os.path.join("tests", "mcp", ".tmp", "unit_file_steps")
        test_file = os.path.join(test_dir, "sample.txt")
        _run_tester_file_step(root, {"tester": "remove", "path": test_dir})

        _run_tester_file_step(root, {"tester": "mkdir", "path": test_dir})
        _run_tester_file_step(root, {
            "tester": "write_file",
            "path": test_file,
            "lines": ["alpha", "beta"],
        })
        _run_tester_file_step(root, {
            "tester": "assert_file_content",
            "path": test_file,
            "lines": ["alpha", "beta"],
        })
        _run_tester_file_step(root, {
            "tester": "assert_file_contains",
            "path": test_file,
            "content": "alp",
        })
        _run_tester_file_step(root, {"tester": "remove", "path": test_dir})
        _run_tester_file_step(root, {"tester": "assert_missing", "path": test_dir})

    def test_compact_result_for_element_value(self):
        self.assertEqual(
            _compact_result({"id": "display", "value": "42", "type": "label"}),
            "display value='42'",
        )

    def test_select_snapshot_fields_keeps_requested_paths(self):
        snapshot = {
            "id": "display",
            "value": "42",
            "bounds": {"row": 1, "col": 2},
        }

        self.assertEqual(
            _select_snapshot_fields(snapshot, ["value", "bounds.row", "missing"]),
            {"value": "42", "bounds.row": 1, "missing": None},
        )

    def test_multiple_target_viewports_split_app_area(self):
        script = TestScript("unit.yaml", {"py": "py.py", "cpp": "cpp.py"}, [{"tool": "noop"}])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=script.steps,
            scripts=[script],
        )
        tester = McpTester(config)
        tester.app_viewport._cell_row = 2
        tester.app_viewport._cell_col = 3
        tester.app_viewport.row = 0
        tester.app_viewport.col = 0
        tester.app_viewport.width = 10
        tester.app_viewport.height = 4

        rects = tester._app_viewport_rects(["py", "cpp"])

        self.assertEqual(rects["py"], {"row": 2, "col": 3, "width": 5, "height": 4})
        self.assertEqual(rects["cpp"], {"row": 2, "col": 8, "width": 5, "height": 4})

    def test_compare_target_viewports_use_equal_dimensions(self):
        script = TestScript("unit.yaml", {"py": "py.py", "cpp": "cpp.py"}, [{"tool": "noop"}])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=script.steps,
            compare_mode=True,
            scripts=[script],
        )
        tester = McpTester(config)
        tester.app_viewport._cell_row = 2
        tester.app_viewport._cell_col = 3
        tester.app_viewport.row = 0
        tester.app_viewport.col = 0
        tester.app_viewport.width = 143
        tester.app_viewport.height = 37

        rects = tester._app_viewport_rects(["py", "cpp"])

        self.assertEqual(rects["py"], {"row": 2, "col": 3, "width": 71, "height": 37})
        self.assertEqual(rects["cpp"], {"row": 2, "col": 74, "width": 71, "height": 37})

    def test_single_target_explicit_app_size_uses_buffer_viewport(self):
        script = TestScript("unit.yaml", {"py": "py.py"}, [{"tool": "noop"}])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=script.steps,
            compare_viewport={"width": 10, "height": 20},
            scripts=[script],
        )
        tester = McpTester(config)
        tester.app_viewport._cell_row = 2
        tester.app_viewport._cell_col = 3
        tester.app_viewport.row = 0
        tester.app_viewport.col = 0
        tester.app_viewport.width = 143
        tester.app_viewport.height = 37

        rects = tester._app_viewport_rects(["py"])

        self.assertTrue(tester._is_buffer_mode())
        self.assertTrue(tester._requires_controlled_target_output(1))
        self.assertEqual(tester._buffer_visible_size(tester._app_viewport_rect(), 1), (10, 20))
        self.assertEqual(rects["py"], {"row": 2, "col": 3, "width": 10, "height": 20})

    def test_single_target_buffer_mode_repaints_during_text_tool(self):
        self.assertLess(LIVE_BUFFER_REPAINT_INTERVAL_SECONDS, CONTROLLED_ANIMATION_REPAINT_SECONDS)

        class FakeTarget:
            def __init__(self):
                self.forward_output = True
                self.started = threading.Event()
                self.release = threading.Event()
                self.render_frame_calls = 0

            def call_tool(self, name, arguments=None, timeout_ms=None):
                if name == "get_render_frame":
                    self.render_frame_calls += 1
                    if self.started.is_set():
                        self.release.set()
                    return {"cells": [[[" ", None, None]]]}
                self.started.set()
                self.release.wait(1)
                return {"value": arguments.get("text", "")}

            def set_output_forwarding(self, enabled):
                self.forward_output = bool(enabled)

            def wait_output_idle(self, *args, **kwargs):
                return None

            def mark_full_repaint_pending(self):
                return None

        script = TestScript("unit.yaml", {"py": "py.py"}, [{"tool": "noop"}])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=100,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=script.steps,
            compare_viewport={"width": 10, "height": 20},
            scripts=[script],
        )
        tester = McpTester(config)
        target = FakeTarget()
        tester.targets = {"py": target}
        tester._repaint_buffer_targets = lambda: None

        result = tester._call_targets(
            ["py"],
            "set_text",
            {"py": {"element_id": "message", "text": "abc"}},
            None,
        )

        self.assertEqual(result["py"], {"value": "abc"})
        self.assertGreaterEqual(target.render_frame_calls, 1)

    def test_multiple_target_viewports_tile_three_apps(self):
        script = TestScript("unit.yaml", {"a": "a.py", "b": "b.py", "c": "c.py"}, [{"tool": "noop"}])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=script.steps,
            scripts=[script],
        )
        tester = McpTester(config)
        tester.app_viewport._cell_row = 0
        tester.app_viewport._cell_col = 0
        tester.app_viewport.row = 1
        tester.app_viewport.col = 2
        tester.app_viewport.width = 11
        tester.app_viewport.height = 5

        rects = tester._app_viewport_rects(["a", "b", "c"])

        self.assertEqual(rects["a"], {"row": 1, "col": 2, "width": 6, "height": 3})
        self.assertEqual(rects["b"], {"row": 1, "col": 8, "width": 5, "height": 3})
        self.assertEqual(rects["c"], {"row": 4, "col": 2, "width": 6, "height": 2})

    def test_log_panel_uses_separate_colored_rows(self):
        panel = LogPanel()
        panel.resize(80, 10)
        panel.open()

        panel.append_line("15:00:00 calculator: activate_element")
        panel.append_line("15:00:00 ASSERT calculator.value == '0.5'")
        panel.append_line("15:00:00 FAIL step 3: expected")

        self.assertEqual([child.text for child in panel._children], [
            "15:00:00 calculator: activate_element",
            "15:00:00 ASSERT calculator.value == '0.5'",
            "15:00:00 FAIL step 3: expected",
        ])
        self.assertEqual(panel._children[0].style.color.hex, "#dddddd")
        self.assertEqual(panel._children[1].style.color.hex, "#15803d")
        self.assertEqual(panel._children[2].style.color.hex, "#b91c1c")
        self.assertEqual(panel._children[0].style.user_select, "text")
        self.assertEqual(panel._children[1].style.user_select, "text")
        self.assertEqual(panel._children[2].style.user_select, "text")
        self.assertEqual(panel._get_viewport_rect()["height"], 10)

    def test_log_panel_keeps_all_rows_and_copies_full_log(self):
        elements_module._TEXT_CLIPBOARD = ""
        panel = LogPanel()
        panel.resize(80, 10)
        panel.open()

        for index in range(505):
            panel.append_line(f"line {index}")

        self.assertEqual(len(panel._children), 505)
        self.assertEqual(panel.copy_all(), 505)
        self.assertTrue(elements_module._TEXT_CLIPBOARD.startswith("line 0\nline 1"))
        self.assertTrue(elements_module._TEXT_CLIPBOARD.endswith("line 504"))

    def test_log_panel_clear_selections_accepts_label_children(self):
        panel = LogPanel()
        panel.resize(80, 10)
        panel.open()
        panel.append_line("15:00:00 calculator: activate_element")

        panel._clear_selections()

        self.assertEqual(len(panel._children), 1)

    def test_failed_step_is_reported_in_result_even_without_failed_assert(self):
        script = TestScript("unit.yaml", {"missing": "missing.py"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=[],
            scripts=[script],
        )
        tester = McpTester(config)
        tester.open()
        tester.step_failures = 1
        tester.script_step_failures = 1
        tester.assertions_passed = 7
        tester.assertions_failed = 0
        tester.exit_code = 1
        tester._append_script_summary(False)
        tester._append_summary()

        lines = [child.text for child in tester.log_output._child_instance._children]
        self.assertTrue(any("FAIL 0 asserts passed, 0 failed, 1 step failures" in line for line in lines))
        self.assertTrue(any("RESULTS: unit.yaml [missing]: FAIL 0 asserts passed, 0 failed, 1 step failures" in line for line in lines))
        self.assertTrue(any("RESULT: 7 asserts passed, 0 failed, 1 step failures" in line for line in lines))

    def test_failed_script_continues_to_next_include(self):
        first = TestScript("first.yaml", {"first": "missing.py"}, [])
        second = TestScript("second.yaml", {"second": "missing.py"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=first.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="all.yaml",
            steps=[],
            scripts=[first, second],
        )
        tester = McpTester(config)
        tester.open()
        tester.exit_code = 1
        tester.step_failures = 1
        tester.script_step_failures = 1

        tester._finish_script(False)

        self.assertEqual(tester.current_script, second)
        self.assertFalse(tester.finished)
        self.assertEqual(len(tester.script_summaries), 1)
        self.assertEqual(tester.script_summaries[0]["status"], "FAIL")
        tester.close()

    def test_copy_log_button_copies_visible_log_history(self):
        elements_module._TEXT_CLIPBOARD = ""
        script = TestScript("unit.yaml", {"missing": "missing.py"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=[],
            scripts=[script],
        )
        tester = McpTester(config)
        tester.open()
        tester._append_log("alpha")
        tester._append_log("beta")

        tester.elementchanged(tester.copy_btn, None)

        self.assertIn("alpha", elements_module._TEXT_CLIPBOARD)
        self.assertIn("beta", elements_module._TEXT_CLIPBOARD)
        self.assertNotIn("copied", elements_module._TEXT_CLIPBOARD)
        tester.close()

    def test_pause_button_toggles_to_play_and_back(self):
        script = TestScript("unit.yaml", {"missing": "missing.py"}, [])
        config = TesterConfig(
            root=os.getcwd(),
            apps=script.apps,
            action_delay_ms=0,
            type_delay_ms=0,
            step_delay_seconds=0,
            source_path="unit.yaml",
            steps=[],
            scripts=[script],
        )
        tester = McpTester(config)
        tester.open()

        self.assertTrue(tester.running_script)
        self.assertFalse(tester.paused)
        self.assertEqual(tester.stop_btn.title, PAUSE_BUTTON_TITLE)

        tester.elementchanged(tester.stop_btn, None)

        self.assertFalse(tester.running_script)
        self.assertTrue(tester.paused)
        self.assertEqual(tester.stop_btn.title, PLAY_BUTTON_TITLE)

        tester.elementchanged(tester.stop_btn, None)

        self.assertTrue(tester.running_script)
        self.assertFalse(tester.paused)
        self.assertEqual(tester.stop_btn.title, PAUSE_BUTTON_TITLE)
        tester.close()


if __name__ == "__main__":
    unittest.main()

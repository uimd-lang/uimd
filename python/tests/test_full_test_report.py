"""Tests for the opt-in full-gate live report and complete log."""

from __future__ import annotations

from dataclasses import dataclass
from io import StringIO
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

from tools.full_test_report import FullTestReporter, PhaseOutputParser


TOOLS_DIR = Path(__file__).resolve().parents[2] / "tools"
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

import uimd_dev


@dataclass(frozen=True)
class _Phase:
    status: str


class FullTestReportTests(unittest.TestCase):
    def test_posix_wrapper_exposes_homebrew_libsixel_to_python_children(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            temp_path = Path(temp_dir)
            bin_dir = temp_path / "bin"
            library_prefix = temp_path / "libsixel"
            library_dir = library_prefix / "lib"
            bin_dir.mkdir()
            library_dir.mkdir(parents=True)
            brew = bin_dir / "brew"
            brew.write_text(
                "#!/bin/sh\n"
                "if [ \"$1\" = \"--prefix\" ] && [ \"$2\" = \"libsixel\" ]; then\n"
                f"    printf '%s\\n' '{library_prefix}'\n"
                "    exit 0\n"
                "fi\n"
                "exit 1\n",
                encoding="utf-8",
            )
            python = bin_dir / "python3"
            python.write_text(
                "#!/bin/sh\n"
                "printf '%s\\n' \"${UIMD_LIBSIXEL_DIR:-missing}\"\n",
                encoding="utf-8",
            )
            brew.chmod(0o755)
            python.chmod(0o755)
            env = os.environ.copy()
            env["PATH"] = f"{bin_dir}{os.pathsep}{env.get('PATH', '')}"
            env.pop("UIMD_LIBSIXEL_PATH", None)
            env.pop("UIMD_LIBSIXEL_DIR", None)

            result = subprocess.run(
                [str(TOOLS_DIR / "test_all.sh"), "--live-report"],
                cwd=TOOLS_DIR.parent,
                env=env,
                capture_output=True,
                text=True,
                check=False,
            )

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stdout.strip(), str(library_dir))

    def test_mcp_failure_is_emitted_immediately_once_and_raw_output_is_logged(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            log_path = Path(temp_dir) / "full.log"
            reporter = FullTestReporter(log_path, terminal=terminal)
            reporter.open()
            reporter.start_phase("MCP example compare", "mcp")
            reporter.write_raw_line(
                "12:00:00 SCRIPT 3/14 calculator: tests/mcp/calculator.yaml"
            )
            reporter.write_raw_line(
                "12:00:01 FAIL step 7: calculator.value expected == '3', got '2'"
            )

            immediate_output = terminal.getvalue()
            self.assertIn("[FAIL] MCP example compare", immediate_output)
            self.assertIn("calculator.value expected == '3', got '2'", immediate_output)

            reporter.write_raw_line(
                "12:00:01 SUMMARY calculator: FAIL 4 asserts passed, 0 failed, 1 step failures"
            )
            reporter.write_raw_line(
                "12:00:01 RESULT: 4 asserts passed, 0 failed, 1 step failures"
            )
            reporter.finish_phase("FAIL", 1.25, "exit 1")
            reporter.close()

            terminal_output = terminal.getvalue()
            self.assertEqual(terminal_output.count("calculator.value expected"), 1)
            self.assertIn("4/5 assertions/steps passed", terminal_output)
            self.assertIn("1 failed", terminal_output)
            log_output = log_path.read_text(encoding="utf-8")
            self.assertIn("SCRIPT 3/14 calculator", log_output)
            self.assertIn("FAIL step 7", log_output)
            self.assertIn("RESULT: 4 asserts passed", log_output)

    def test_mcp_phase_aggregates_multiple_regression_commands(self):
        parser = PhaseOutputParser("mcp")
        parser.start_command()
        parser.feed(
            "12:00:00 SUMMARY tests/mcp/first.yaml [first]: "
            "FAIL 7 asserts passed, 1 failed, 0 step failures"
        )
        parser.feed("12:00:00 RESULT: 7 asserts passed, 1 failed, 0 step failures")
        parser.finish_command()
        parser.start_command()
        parser.feed(
            "12:00:01 SUMMARY tests/mcp/second.yaml [second]: "
            "FAIL 5 asserts passed, 0 failed, 1 step failures"
        )
        parser.finish_command()

        counts = parser.counts("FAIL")
        self.assertEqual(counts.passed, 12)
        self.assertEqual(counts.total, 14)
        self.assertEqual(counts.failed, 2)
        self.assertTrue(counts.partial)

    def test_aborted_mcp_command_keeps_prior_failures_and_partial_counts(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            log_path = Path(temp_dir) / "full.log"
            reporter = FullTestReporter(log_path, terminal=terminal)
            reporter.open()
            reporter.start_phase("MCP example compare", "mcp", keep_going_commands=True)
            child = "\n".join(
                (
                    "print('SCRIPT 7/14 image_browser: tests/mcp/image_browser_compare.yaml')",
                    "print(\"FAIL step 1: snapshot mismatch for python: expected 'x', got 'y'\")",
                    "print('SUMMARY tests/mcp/image_browser_compare.yaml [image_browser]: "
                    "FAIL 2 asserts passed, 0 failed, 1 step failures')",
                    "print('SCRIPT 8/14 image_gallery: tests/mcp/image_gallery_compare.yaml')",
                    "print('SUMMARY tests/mcp/image_gallery_compare.yaml [image_gallery]: "
                    "PASS 3 asserts passed, 0 failed, 0 step failures')",
                    "print('SCRIPT 9/14 image_gallery: "
                    "tests/mcp/image_gallery_sixel_info_compare.yaml')",
                    "print('uimd_mcp_tester: cpp exited early: uimd: error: libsixel is required')",
                    "raise SystemExit(2)",
                )
            )

            reporter.run([sys.executable, "-c", child], cwd=Path(temp_dir))
            reporter.finish_phase("FAIL", 0.5, "exit 2")
            reporter.finish_gate([_Phase("FAIL")])
            reporter.close()

            output = terminal.getvalue()
            self.assertEqual(output.count("snapshot mismatch for python"), 2)
            self.assertEqual(output.count("libsixel is required"), 2)
            self.assertIn(
                "[FAIL] MCP example compare | image_gallery_sixel_info_compare | "
                "uimd_mcp_tester: cpp exited early: uimd: error: libsixel is required",
                output,
            )
            self.assertIn(
                "[FAIL] MCP example compare | 5/6 assertions/steps passed | "
                "1 failed | partial | exit 2",
                output,
            )
            raw_log = log_path.read_text(encoding="utf-8")
            self.assertIn("SCRIPT 7/14 image_browser", raw_log)
            self.assertIn("image_gallery_sixel_info_compare.yaml", raw_log)

    def test_pytest_structured_failure_preserves_assertion_and_strips_ansi(self):
        parser = PhaseOutputParser("pytest")

        failures = parser.feed(
            "\x1b[31mFUIMD_TEST_FAILURE\ttest_math.py::test_sum\t"
            "AssertionError: expected 3, got 2\x1b[0m\n"
        )
        parser.feed("================ 2 passed, 1 failed, 1 skipped in 0.12s ================")

        self.assertEqual(len(failures), 1)
        self.assertEqual(failures[0].label, "test_math.py::test_sum")
        self.assertEqual(failures[0].detail, "AssertionError: expected 3, got 2")
        self.assertEqual(parser.counts("FAIL").passed, 2)
        self.assertEqual(parser.counts("FAIL").total, 4)
        self.assertEqual(parser.counts("FAIL").failed, 1)
        self.assertEqual(parser.counts("FAIL").skipped, 1)

    def test_supported_suite_summaries_report_passed_over_total(self):
        cases = (
            ("ctest", "100% tests passed out of 26", (26, 26, 0, "tests")),
            ("ctest", "96% tests passed, 1 tests failed out of 26", (25, 26, 1, "tests")),
            ("go", "ok\tuimd/runtime\t0.123s", (1, 1, 0, "packages")),
            (
                "cargo",
                "test result: ok. 57 passed; 0 failed; 2 ignored; 0 measured; 0 filtered out",
                (57, 59, 0, "tests"),
            ),
            ("swift", "Executed 120 tests, with 2 failures in 1.000 seconds", (118, 120, 2, "tests")),
            ("smoke", "PASS Go direct terminal smoke: 10/10 checks passed", (10, 10, 0, "checks")),
        )
        for kind, line, expected in cases:
            with self.subTest(kind=kind):
                parser = PhaseOutputParser(kind)
                parser.feed(line)
                counts = parser.counts("FAIL" if expected[2] else "PASS")
                self.assertEqual(
                    (counts.passed, counts.total, counts.failed, counts.unit),
                    expected,
                )

    def test_supported_suites_emit_one_failure_event_as_the_test_finishes(self):
        cases = (
            ("ctest", "1/26 Test #1: ui_cpp_tests ........***Failed 0.01 sec", "ui_cpp_tests"),
            ("go", "--- FAIL: TestModalCleanup (0.01s)", "TestModalCleanup"),
            ("cargo", "test mcp::tests::modal_cleanup ... FAILED", "mcp::tests::modal_cleanup"),
            (
                "swift",
                "Test Case '-[UimdTests.RuntimeTests testModalCleanup]' failed (0.001 seconds)",
                "-[UimdTests.RuntimeTests testModalCleanup]",
            ),
            ("smoke", "FAIL image_browser direct terminal parity", "image_browser direct terminal parity"),
        )
        for kind, line, expected_label in cases:
            with self.subTest(kind=kind):
                failures = PhaseOutputParser(kind).feed(line)
                self.assertEqual(len(failures), 1)
                self.assertEqual(failures[0].label, expected_label)

    def test_smoke_error_emits_the_manifest_failure_reason_immediately(self):
        failures = PhaseOutputParser("smoke").feed(
            "ERROR Swift direct terminal smoke failed: parity build is stale because "
            "source inputs changed after rebuild (first changed input: rust/src/uimd/Cargo.lock)"
        )

        self.assertEqual(len(failures), 1)
        self.assertEqual(failures[0].label, "Swift direct terminal smoke")
        self.assertIn("rust/src/uimd/Cargo.lock", failures[0].detail)

    def test_aborted_smoke_reports_observed_checks_instead_of_a_generic_gate(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            reporter = FullTestReporter(Path(temp_dir) / "full.log", terminal=terminal)
            reporter.open()
            reporter.start_phase("Swift direct terminal smoke", "smoke")
            reporter.write_raw_line("PASS activity_feed terminal title")
            reporter.write_raw_line("PASS formular split arrow")
            reporter.write_raw_line(
                "ERROR Swift direct terminal smoke failed: screen text not found"
            )
            reporter.finish_phase("FAIL", 0.1, "exit 1")
            reporter.close()

            self.assertIn(
                "[FAIL] Swift direct terminal smoke | 2/3 checks passed | "
                "1 failed | partial | exit 1",
                terminal.getvalue(),
            )

    def test_failure_line_does_not_repeat_an_identical_phase_and_failure_label(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            reporter = FullTestReporter(Path(temp_dir) / "full.log", terminal=terminal)
            reporter.open()
            reporter.start_phase("Swift direct terminal smoke", "smoke")
            reporter.write_raw_line(
                "ERROR Swift direct terminal smoke failed: screen text not found"
            )
            reporter.finish_phase("FAIL", 0.1, "exit 1")
            reporter.finish_gate([_Phase("FAIL")])
            reporter.close()

            output = terminal.getvalue()
            self.assertNotIn(
                "Swift direct terminal smoke | Swift direct terminal smoke",
                output,
            )
            self.assertEqual(output.count("screen text not found"), 2)

    def test_failed_command_can_be_logged_and_deferred_for_keep_going(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            log_path = Path(temp_dir) / "full.log"
            reporter = FullTestReporter(log_path, terminal=terminal)
            reporter.open()
            reporter.start_phase("Synthetic gate", "generic", keep_going_commands=True)

            reporter.run(
                [
                    sys.executable,
                    "-c",
                    "print('complete child diagnostic', flush=True); raise SystemExit(3)",
                ],
                cwd=Path(temp_dir),
            )

            self.assertTrue(reporter.phase_has_failed_commands())
            error = reporter.first_failed_command()
            self.assertIsInstance(error, subprocess.CalledProcessError)
            self.assertEqual(error.returncode, 3)
            reporter.finish_phase("FAIL", 0.1, "exit 3")
            reporter.finish_gate([_Phase("FAIL")])
            reporter.close()

            self.assertIn("complete child diagnostic", log_path.read_text(encoding="utf-8"))
            self.assertEqual(terminal.getvalue().count("complete child diagnostic"), 2)
            self.assertIn("[FAIL] Synthetic gate", terminal.getvalue())
            self.assertIn("[SUMMARY] FULL TEST RECAP", terminal.getvalue())
            self.assertIn("[FAIL] FULL TEST", terminal.getvalue())

    def test_final_recap_repeats_every_phase_and_each_failure_reason(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            terminal = StringIO()
            reporter = FullTestReporter(Path(temp_dir) / "full.log", terminal=terminal)
            reporter.open()
            reporter.start_phase("Python tests", "pytest")
            reporter.write_raw_line("================ 3 passed in 0.12s ================")
            reporter.finish_phase("PASS", 0.12)
            reporter.start_phase("MCP example compare", "mcp")
            reporter.write_raw_line("SCRIPT 1/1 calculator: tests/mcp/calculator.yaml")
            reporter.write_raw_line("FAIL step 2: expected calculator.value == '3', got '2'")
            reporter.write_raw_line(
                "SUMMARY calculator: FAIL 1 asserts passed, 0 failed, 1 step failures"
            )
            reporter.write_raw_line("RESULT: 1 asserts passed, 0 failed, 1 step failures")
            reporter.finish_phase("FAIL", 0.25, "exit 1")

            reporter.finish_gate([_Phase("PASS"), _Phase("FAIL")])
            reporter.close()

            output = terminal.getvalue()
            recap = output[output.index("[SUMMARY] FULL TEST RECAP") :]
            self.assertIn("[PASS] Python tests | 3/3 tests passed", recap)
            self.assertIn(
                "[FAIL] MCP example compare | 1/2 assertions/steps passed | 1 failed",
                recap,
            )
            self.assertIn(
                "[FAIL] MCP example compare | calculator | step 2: expected calculator.value == '3', got '2'",
                recap,
            )
            self.assertIn("[FAIL] FULL TEST | 1/2 phases passed | 1 failed", recap)
            self.assertEqual(output.count("expected calculator.value == '3', got '2'"), 2)

    def test_nonzero_command_raises_when_keep_going_is_disabled(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            reporter = FullTestReporter(Path(temp_dir) / "full.log", terminal=StringIO())
            reporter.open()
            reporter.start_phase("Synthetic gate", "generic")
            with self.assertRaises(subprocess.CalledProcessError):
                reporter.run(
                    [sys.executable, "-c", "raise SystemExit(2)"],
                    cwd=Path(temp_dir),
                )
            reporter.finish_phase("FAIL", 0.1, "exit 2")
            reporter.close()

    def test_rust_runtime_build_precedes_example_and_regression_builds(self):
        example_dir = Path("rust/examples/example")
        regression_dir = Path("rust/regressions/uimd/parity/regression")

        with (
            patch.object(uimd_dev, "rust_example_app_dirs", return_value=[example_dir]),
            patch.object(uimd_dev, "rust_regression_app_dirs", return_value=[regression_dir]),
            patch.object(
                uimd_dev,
                "cargo_with_progress_command",
                side_effect=lambda *parts: ["cargo-progress", *parts],
            ),
            patch.object(uimd_dev, "run") as run_mock,
        ):
            uimd_dev.build_all_rust_examples()

        calls = run_mock.call_args_list
        self.assertEqual(len(calls), 3)
        self.assertEqual(
            calls[0].args[0],
            ["cargo-progress", "build", "--release", "--all-targets"],
        )
        self.assertEqual(calls[0].kwargs["cwd"], uimd_dev.ROOT / "rust/src/uimd")
        self.assertEqual(calls[1].kwargs["cwd"], example_dir)
        self.assertEqual(calls[2].kwargs["cwd"], regression_dir)


if __name__ == "__main__":
    unittest.main()

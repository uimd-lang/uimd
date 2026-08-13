#!/usr/bin/env python3
"""Live concise reporting and complete logging for the full test gate."""

from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
import re
import subprocess
import sys
from typing import Iterable, TextIO


DEFAULT_LOG_DIRECTORY = Path(".uimd/test-logs")
DEFAULT_LOG_NAME_PREFIX = "test-all"
REPORT_LOG_PREFIX = "[live-report]"
PYTEST_FAILURE_PREFIX = "UIMD_TEST_FAILURE"

ANSI_CSI_RE = re.compile(r"\x1b\[[0-?]*[ -/]*[@-~]")
ANSI_OSC_RE = re.compile(r"\x1b\][^\x07]*(?:\x07|\x1b\\)")
LOG_TIMESTAMP_RE = re.compile(r"^\d\d:\d\d:\d\d\s+")
WHITESPACE_RE = re.compile(r"\s+")

MCP_SCRIPT_RE = re.compile(r"^SCRIPT\s+\d+/\d+\s+(.+?):\s+(.+)$")
MCP_FAILURE_RE = re.compile(r"^FAIL\s+(step\s+\d+|cleanup):\s*(.+)$")
MCP_SCRIPT_SUMMARY_RE = re.compile(
    r"^SUMMARY\s+(.+?):\s+(PASS|FAIL)\s+"
    r"(\d+) asserts passed,\s+(\d+) failed,\s+(\d+) step failures$"
)
MCP_RESULT_RE = re.compile(
    r"^RESULT:\s+(\d+) asserts passed,\s+(\d+) failed,\s+(\d+) step failures$"
)
PYTEST_PROGRESS_FAILURE_RE = re.compile(r"^(\S+::\S+)\s+FAILED(?:\s|$)")
PYTEST_SUMMARY_TOKEN_RE = re.compile(
    r"(\d+)\s+(passed|failed|skipped|errors?|xfailed|xpassed|deselected)\b"
)
CTEST_SUMMARY_RE = re.compile(
    r"\d+% tests passed,\s+(\d+) tests failed out of\s+(\d+)",
    re.IGNORECASE,
)
CTEST_PASS_SUMMARY_RE = re.compile(
    r"100% tests passed out of\s+(\d+)",
    re.IGNORECASE,
)
CTEST_FAILURE_RE = re.compile(r"Test\s+#\d+:\s+(.+?)\s+\.{2,}\*{3}Failed")
GO_PACKAGE_RE = re.compile(r"^(ok|FAIL|\?)\s+(\S+)")
GO_TEST_FAILURE_RE = re.compile(r"^--- FAIL:\s+([^\s(]+)")
CARGO_RESULT_RE = re.compile(
    r"^test result:\s+(?:ok|FAILED)\.\s+"
    r"(\d+) passed;\s+(\d+) failed;\s+(\d+) ignored"
)
CARGO_TEST_FAILURE_RE = re.compile(r"^test\s+(.+?)\s+\.\.\.\s+FAILED$")
SWIFT_XCTEST_RESULT_RE = re.compile(
    r"Executed\s+(\d+) tests?,\s+with\s+(\d+) failures?",
    re.IGNORECASE,
)
SWIFT_TESTING_RESULT_RE = re.compile(r"Test run with\s+(\d+) tests? passed", re.IGNORECASE)
SWIFT_XCTEST_FAILURE_RE = re.compile(r"^Test Case '(.+)' failed")
SWIFT_TESTING_FAILURE_RE = re.compile(r"^[✘✗]\s+Test\s+(.+?)\s+failed")
SMOKE_RESULT_RE = re.compile(r"\b(\d+)/(\d+)\s+(?:checks|groups) passed\b", re.IGNORECASE)
SMOKE_PASS_RE = re.compile(r"^PASS\s+(.+)$", re.IGNORECASE)
SMOKE_FAILURE_RE = re.compile(r"^(?:ERROR\s+)?FAIL(?:ED)?\s+(.+)$", re.IGNORECASE)
SMOKE_ERROR_RE = re.compile(r"^ERROR\s+(.+?)\s+failed:\s*(.+)$", re.IGNORECASE)


@dataclass(frozen=True)
class FailureEvent:
    key: str
    label: str
    detail: str = ""


@dataclass(frozen=True)
class PhaseCounts:
    passed: int
    total: int
    failed: int = 0
    skipped: int = 0
    unit: str = "tests"
    partial: bool = False


def _plain_line(value: str) -> str:
    value = ANSI_OSC_RE.sub("", value)
    value = ANSI_CSI_RE.sub("", value)
    value = value.replace("\r", " ").replace("\n", " ")
    value = LOG_TIMESTAMP_RE.sub("", value.strip())
    return WHITESPACE_RE.sub(" ", value).strip()


def default_log_path(root: Path) -> Path:
    timestamp = datetime.now().strftime("%Y%m%d-%H%M%S-%f")
    return root / DEFAULT_LOG_DIRECTORY / f"{DEFAULT_LOG_NAME_PREFIX}-{timestamp}.log"


class PhaseOutputParser:
    """Incrementally extract counts and failure events from one test phase."""

    def __init__(self, kind: str):
        self.kind = kind
        self.pytest_counts: dict[str, int] | None = None
        self.ctest_counts: PhaseCounts | None = None
        self.go_passed_packages: set[str] = set()
        self.go_failed_packages: set[str] = set()
        self.go_skipped_packages: set[str] = set()
        self.cargo_passed = 0
        self.cargo_failed = 0
        self.cargo_ignored = 0
        self.cargo_results = 0
        self.swift_counts: PhaseCounts | None = None
        self.smoke_counts: PhaseCounts | None = None
        self.smoke_observed_passes: set[str] = set()
        self.mcp_passed = 0
        self.mcp_failed = 0
        self.mcp_count_records = 0
        self.mcp_partial_commands = 0
        self.mcp_pending_summary_passed = 0
        self.mcp_pending_summary_failed = 0
        self.mcp_pending_summary_seen = False
        self.mcp_pending_result_passed = 0
        self.mcp_pending_result_failed = 0
        self.mcp_pending_result_seen = False
        self.mcp_command_active = False
        self.mcp_current_script = "MCP script"
        self.mcp_current_source = ""
        self.mcp_current_failure_emitted = False
        self.mcp_script_summaries: set[str] = set()

    def _pending_mcp_counts(self) -> tuple[int, int, bool]:
        if self.mcp_pending_result_seen:
            return (
                self.mcp_pending_result_passed,
                self.mcp_pending_result_failed,
                True,
            )
        return (
            self.mcp_pending_summary_passed,
            self.mcp_pending_summary_failed,
            self.mcp_pending_summary_seen,
        )

    def _reset_pending_mcp_counts(self) -> None:
        self.mcp_pending_summary_passed = 0
        self.mcp_pending_summary_failed = 0
        self.mcp_pending_summary_seen = False
        self.mcp_pending_result_passed = 0
        self.mcp_pending_result_failed = 0
        self.mcp_pending_result_seen = False
        self.mcp_current_script = "MCP script"
        self.mcp_current_source = ""
        self.mcp_current_failure_emitted = False
        self.mcp_script_summaries.clear()

    def _commit_pending_mcp_counts(self) -> None:
        passed, failed, seen = self._pending_mcp_counts()
        if seen:
            self.mcp_passed += passed
            self.mcp_failed += failed
            self.mcp_count_records += 1
            if self.mcp_pending_summary_seen and not self.mcp_pending_result_seen:
                self.mcp_partial_commands += 1
        self._reset_pending_mcp_counts()

    def start_command(self) -> None:
        if self.kind != "mcp":
            return
        if self.mcp_command_active:
            raise RuntimeError("MCP report command is already active")
        self._commit_pending_mcp_counts()
        self.mcp_command_active = True

    def finish_command(self) -> None:
        if self.kind != "mcp":
            return
        self._commit_pending_mcp_counts()
        self.mcp_command_active = False

    def failure_context_label(self, fallback: str) -> str:
        if self.kind != "mcp":
            return fallback
        if self.mcp_current_source:
            return Path(self.mcp_current_source).stem
        if self.mcp_current_script != "MCP script":
            return self.mcp_current_script
        return fallback

    def feed(self, raw_line: str) -> list[FailureEvent]:
        structured_line = ANSI_OSC_RE.sub("", raw_line)
        structured_line = ANSI_CSI_RE.sub("", structured_line).strip()
        structured_line = LOG_TIMESTAMP_RE.sub("", structured_line)
        line = _plain_line(raw_line)
        if not line:
            return []
        failures: list[FailureEvent] = []

        if self.kind == "mcp":
            script_match = MCP_SCRIPT_RE.match(line)
            if script_match:
                self.mcp_current_script = script_match.group(1)
                self.mcp_current_source = script_match.group(2)
                self.mcp_current_failure_emitted = False

            failure_match = MCP_FAILURE_RE.match(line)
            if failure_match:
                self.mcp_current_failure_emitted = True
                failures.append(
                    FailureEvent(
                        (
                            f"mcp:{self.mcp_current_source or self.mcp_current_script}:"
                            f"{failure_match.group(1)}:{failure_match.group(2)}"
                        ),
                        self.mcp_current_script,
                        f"{failure_match.group(1)}: {failure_match.group(2)}",
                    )
                )

            summary_match = MCP_SCRIPT_SUMMARY_RE.match(line)
            if summary_match:
                name, status = summary_match.group(1), summary_match.group(2)
                if name not in self.mcp_script_summaries:
                    self.mcp_script_summaries.add(name)
                    passed = int(summary_match.group(3))
                    failed = int(summary_match.group(4)) + int(summary_match.group(5))
                    self.mcp_pending_summary_passed += passed
                    self.mcp_pending_summary_failed += failed
                    self.mcp_pending_summary_seen = True
                    if status == "FAIL" and not self.mcp_current_failure_emitted:
                        failures.append(
                            FailureEvent(
                                f"mcp-summary:{name}",
                                self.mcp_current_script,
                                (
                                    f"{summary_match.group(4)} failed assertions, "
                                    f"{summary_match.group(5)} failed steps"
                                ),
                            )
                        )

            result_match = MCP_RESULT_RE.match(line)
            if result_match:
                passed = int(result_match.group(1))
                failed = int(result_match.group(2)) + int(result_match.group(3))
                self.mcp_pending_result_passed += passed
                self.mcp_pending_result_failed += failed
                self.mcp_pending_result_seen = True
            return failures

        if self.kind == "pytest":
            failure_marker = f"{PYTEST_FAILURE_PREFIX}\t"
            failure_marker_index = structured_line.find(failure_marker)
            if failure_marker_index >= 0:
                parts = structured_line[failure_marker_index:].split("\t", 2)
                label = parts[1] if len(parts) > 1 else "pytest"
                detail = parts[2] if len(parts) > 2 else ""
                failures.append(FailureEvent(f"pytest:{label}", label, detail))
            else:
                progress_match = PYTEST_PROGRESS_FAILURE_RE.match(line)
                if progress_match:
                    label = progress_match.group(1)
                    failures.append(FailureEvent(f"pytest:{label}", label))
            tokens = PYTEST_SUMMARY_TOKEN_RE.findall(line)
            if tokens and (" in " in line or line.startswith("=")):
                counts: dict[str, int] = {}
                for count, label in tokens:
                    normalized = "error" if label in ("error", "errors") else label
                    counts[normalized] = counts.get(normalized, 0) + int(count)
                self.pytest_counts = counts
            return failures

        if self.kind == "ctest":
            failure_match = CTEST_FAILURE_RE.search(line)
            if failure_match:
                label = failure_match.group(1).strip()
                failures.append(FailureEvent(f"ctest:{label}", label))
            summary_match = CTEST_SUMMARY_RE.search(line)
            if summary_match:
                failed = int(summary_match.group(1))
                total = int(summary_match.group(2))
                self.ctest_counts = PhaseCounts(total - failed, total, failed)
            else:
                pass_match = CTEST_PASS_SUMMARY_RE.search(line)
                if pass_match:
                    total = int(pass_match.group(1))
                    self.ctest_counts = PhaseCounts(total, total)
            return failures

        if self.kind == "go":
            package_match = GO_PACKAGE_RE.match(line)
            if package_match:
                status, package = package_match.groups()
                if status == "ok":
                    self.go_passed_packages.add(package)
                elif status == "FAIL":
                    self.go_failed_packages.add(package)
                else:
                    self.go_skipped_packages.add(package)
            failure_match = GO_TEST_FAILURE_RE.match(line)
            if failure_match:
                label = failure_match.group(1)
                failures.append(FailureEvent(f"go:{label}", label))
            return failures

        if self.kind == "cargo":
            failure_match = CARGO_TEST_FAILURE_RE.match(line)
            if failure_match:
                label = failure_match.group(1)
                failures.append(FailureEvent(f"cargo:{label}", label))
            result_match = CARGO_RESULT_RE.match(line)
            if result_match:
                self.cargo_passed += int(result_match.group(1))
                self.cargo_failed += int(result_match.group(2))
                self.cargo_ignored += int(result_match.group(3))
                self.cargo_results += 1
            return failures

        if self.kind == "swift":
            failure_match = SWIFT_XCTEST_FAILURE_RE.match(line) or SWIFT_TESTING_FAILURE_RE.match(line)
            if failure_match:
                label = failure_match.group(1)
                failures.append(FailureEvent(f"swift:{label}", label))
            result_match = SWIFT_XCTEST_RESULT_RE.search(line)
            if result_match:
                total = int(result_match.group(1))
                failed = int(result_match.group(2))
                candidate = PhaseCounts(total - failed, total, failed)
                if self.swift_counts is None or candidate.total >= self.swift_counts.total:
                    self.swift_counts = candidate
            testing_match = SWIFT_TESTING_RESULT_RE.search(line)
            if testing_match:
                passed = int(testing_match.group(1))
                candidate = PhaseCounts(passed, passed)
                if self.swift_counts is None or candidate.total >= self.swift_counts.total:
                    self.swift_counts = candidate
            return failures

        if self.kind == "smoke":
            result_match = SMOKE_RESULT_RE.search(line)
            if result_match:
                passed = int(result_match.group(1))
                total = int(result_match.group(2))
                self.smoke_counts = PhaseCounts(passed, total, total - passed, unit="checks")
                return failures
            pass_match = SMOKE_PASS_RE.match(line)
            if pass_match:
                self.smoke_observed_passes.add(pass_match.group(1))
                return failures
            error_match = SMOKE_ERROR_RE.match(line)
            if error_match:
                label, detail = error_match.groups()
                failures.append(FailureEvent(f"smoke:{label}", label, detail))
                return failures
            failure_match = SMOKE_FAILURE_RE.match(line)
            if failure_match and not line.startswith("PASS"):
                label = failure_match.group(1)
                failures.append(FailureEvent(f"smoke:{label}", label))
            return failures

        return failures

    def counts(self, status: str) -> PhaseCounts:
        if self.kind == "mcp":
            pending_passed, pending_failed, pending_seen = self._pending_mcp_counts()
            if self.mcp_count_records or pending_seen:
                passed = self.mcp_passed + pending_passed
                failed = self.mcp_failed + pending_failed
                return PhaseCounts(
                    passed,
                    passed + failed,
                    failed,
                    unit="assertions/steps",
                    partial=(
                        self.mcp_partial_commands > 0
                        or (
                            self.mcp_pending_summary_seen
                            and not self.mcp_pending_result_seen
                        )
                    ),
                )
        if self.kind == "pytest" and self.pytest_counts is not None:
            passed = self.pytest_counts.get("passed", 0)
            failed = self.pytest_counts.get("failed", 0) + self.pytest_counts.get("error", 0)
            skipped = (
                self.pytest_counts.get("skipped", 0)
                + self.pytest_counts.get("xfailed", 0)
                + self.pytest_counts.get("deselected", 0)
            )
            total = passed + failed + skipped + self.pytest_counts.get("xpassed", 0)
            return PhaseCounts(passed, total, failed, skipped)
        if self.kind == "ctest" and self.ctest_counts is not None:
            return self.ctest_counts
        if self.kind == "go" and (
            self.go_passed_packages or self.go_failed_packages or self.go_skipped_packages
        ):
            passed = len(self.go_passed_packages)
            failed = len(self.go_failed_packages)
            skipped = len(self.go_skipped_packages)
            return PhaseCounts(passed, passed + failed + skipped, failed, skipped, unit="packages")
        if self.kind == "cargo" and self.cargo_results:
            total = self.cargo_passed + self.cargo_failed + self.cargo_ignored
            return PhaseCounts(
                self.cargo_passed,
                total,
                self.cargo_failed,
                self.cargo_ignored,
            )
        if self.kind == "swift" and self.swift_counts is not None:
            return self.swift_counts
        if self.kind == "smoke" and self.smoke_counts is not None:
            return self.smoke_counts
        if self.kind == "smoke" and self.smoke_observed_passes:
            passed = len(self.smoke_observed_passes)
            failed = 1 if status == "FAIL" else 0
            return PhaseCounts(
                passed,
                passed + failed,
                failed,
                unit="checks",
                partial=True,
            )
        if status == "PASS":
            return PhaseCounts(1, 1, unit="gate")
        return PhaseCounts(0, 1, failed=1, unit="gate")


@dataclass
class ActivePhase:
    name: str
    parser: PhaseOutputParser
    keep_going_commands: bool
    failed_commands: list[subprocess.CalledProcessError]
    emitted_failures: set[str]
    failures: list[FailureEvent]
    command_failure_start: int
    last_command_output: str


@dataclass(frozen=True)
class CompletedPhaseReport:
    name: str
    status: str
    counts: PhaseCounts | None
    seconds: float
    detail: str
    failures: tuple[FailureEvent, ...]


class FullTestReporter:
    """Write complete child output to one log and concise live terminal lines."""

    def __init__(self, log_path: Path, *, terminal: TextIO | None = None):
        self.log_path = log_path
        self.terminal = terminal if terminal is not None else sys.stdout
        self._log: TextIO | None = None
        self.active_phase: ActivePhase | None = None
        self.completed_phases: list[CompletedPhaseReport] = []

    def open(self) -> None:
        self.log_path.parent.mkdir(parents=True, exist_ok=True)
        self._log = self.log_path.open("x", encoding="utf-8")
        self._terminal_line(f"[LOG] Full output: {self.log_path}", include_in_log=True)

    def close(self) -> None:
        if self._log is not None:
            self._log.close()
            self._log = None

    def _write_log(self, value: str) -> None:
        if self._log is None:
            raise RuntimeError("full test reporter is not open")
        self._log.write(value)
        self._log.flush()

    def _terminal_line(self, value: str, *, include_in_log: bool = False) -> None:
        self.terminal.write(value + "\n")
        self.terminal.flush()
        if include_in_log:
            self._write_log(f"{REPORT_LOG_PREFIX} {value}\n")

    @staticmethod
    def _failure_line(phase_name: str, failure: FailureEvent, fallback_detail: str = "") -> str:
        detail_value = failure.detail or fallback_detail
        detail = f" | {detail_value}" if detail_value else ""
        if _plain_line(phase_name).casefold() == _plain_line(failure.label).casefold():
            return f"[FAIL] {phase_name}{detail}"
        return f"[FAIL] {phase_name} | {failure.label}{detail}"

    @staticmethod
    def _phase_line(phase: CompletedPhaseReport) -> str:
        if phase.status == "SKIP":
            parts = [f"[SKIP] {phase.name}"]
            if phase.detail:
                parts.append(phase.detail)
            parts.append(f"{phase.seconds:.1f}s")
            return " | ".join(parts)

        assert phase.counts is not None
        parts = [
            f"[{phase.status}] {phase.name}",
            f"{phase.counts.passed}/{phase.counts.total} {phase.counts.unit} passed",
        ]
        if phase.counts.failed:
            parts.append(f"{phase.counts.failed} failed")
        if phase.counts.skipped:
            parts.append(f"{phase.counts.skipped} skipped")
        if phase.counts.partial:
            parts.append("partial")
        if phase.detail:
            parts.append(phase.detail)
        parts.append(f"{phase.seconds:.1f}s")
        return " | ".join(parts)

    def _record_failure(self, failure: FailureEvent) -> None:
        phase = self.active_phase
        if phase is None or failure.key in phase.emitted_failures:
            return
        phase.emitted_failures.add(failure.key)
        phase.failures.append(failure)
        self._terminal_line(
            self._failure_line(phase.name, failure),
            include_in_log=True,
        )

    @staticmethod
    def _command_label(command: list[str]) -> str:
        for part in command[1:]:
            if part.endswith((".py", ".sh", ".ps1", ".cmd")):
                return Path(part).name
        return Path(command[0]).name

    def start_phase(self, name: str, kind: str, *, keep_going_commands: bool = False) -> None:
        if self.active_phase is not None:
            raise RuntimeError(f"test phase is already active: {self.active_phase.name}")
        self.active_phase = ActivePhase(
            name=name,
            parser=PhaseOutputParser(kind),
            keep_going_commands=keep_going_commands,
            failed_commands=[],
            emitted_failures=set(),
            failures=[],
            command_failure_start=0,
            last_command_output="",
        )
        self._write_log(f"\n==> PHASE: {name}\n")

    def write_raw_line(self, value: str) -> None:
        self.write_raw(value + "\n")

    def write_raw(self, value: str) -> None:
        self._write_log(value)
        phase = self.active_phase
        if phase is None:
            return
        for line in value.splitlines():
            plain_line = _plain_line(line)
            if plain_line and not plain_line.startswith("==>"):
                phase.last_command_output = plain_line
            for failure in phase.parser.feed(line):
                self._record_failure(failure)

    def run(
        self,
        command: Iterable[str | Path],
        *,
        cwd: Path,
        env: dict[str, str] | None = None,
    ) -> None:
        command_strings = [str(part) for part in command]
        if self.active_phase is not None:
            self.active_phase.command_failure_start = len(self.active_phase.failures)
            self.active_phase.last_command_output = ""
            self.active_phase.parser.start_command()
        self.write_raw_line(f"==> {' '.join(command_strings)}")
        process = subprocess.Popen(
            command_strings,
            cwd=cwd,
            env=env,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
            errors="replace",
        )
        assert process.stdout is not None
        try:
            for line in process.stdout:
                self.write_raw(line)
            return_code = process.wait()
        except BaseException:
            if process.poll() is None:
                process.terminate()
                process.wait()
            if self.active_phase is not None:
                self.active_phase.parser.finish_command()
            raise
        if return_code == 0:
            if self.active_phase is not None:
                self.active_phase.parser.finish_command()
            return
        if self.active_phase is not None:
            phase = self.active_phase
            detail = phase.last_command_output or f"command exited with {return_code}"
            new_failures = phase.failures[phase.command_failure_start :]
            detail_is_protocol = bool(
                MCP_SCRIPT_RE.match(detail)
                or MCP_FAILURE_RE.match(detail)
                or MCP_SCRIPT_SUMMARY_RE.match(detail)
                or MCP_RESULT_RE.match(detail)
            )
            detail_is_represented = any(
                failure.detail
                and (
                    _plain_line(failure.detail) in detail
                    or detail in _plain_line(failure.detail)
                )
                for failure in new_failures
            )
            if not new_failures or (
                phase.parser.kind == "mcp"
                and not detail_is_protocol
                and not detail_is_represented
            ):
                if detail_is_protocol:
                    detail = f"command exited with {return_code}"
                self._record_failure(
                    FailureEvent(
                        key=f"command:{len(phase.failed_commands)}:{detail}",
                        label=phase.parser.failure_context_label(
                            self._command_label(command_strings)
                        ),
                        detail=detail,
                    )
                )
            phase.parser.finish_command()
        error = subprocess.CalledProcessError(return_code, command_strings)
        if self.active_phase is not None:
            self.active_phase.failed_commands.append(error)
            if self.active_phase.keep_going_commands:
                return
        raise error

    def phase_has_failed_commands(self) -> bool:
        return bool(self.active_phase and self.active_phase.failed_commands)

    def first_failed_command(self) -> subprocess.CalledProcessError | None:
        if self.active_phase is None or not self.active_phase.failed_commands:
            return None
        return self.active_phase.failed_commands[0]

    def finish_phase(self, status: str, seconds: float, detail: str = "") -> None:
        if self.active_phase is None:
            raise RuntimeError("no full test phase is active")
        phase = self.active_phase
        if status == "SKIP":
            completed = CompletedPhaseReport(
                phase.name,
                status,
                None,
                seconds,
                detail,
                tuple(phase.failures),
            )
            self.completed_phases.append(completed)
            self._terminal_line(self._phase_line(completed), include_in_log=True)
            self.active_phase = None
            return
        counts = phase.parser.counts(status)
        completed = CompletedPhaseReport(
            phase.name,
            status,
            counts,
            seconds,
            detail,
            tuple(phase.failures),
        )
        self.completed_phases.append(completed)
        self._terminal_line(self._phase_line(completed), include_in_log=True)
        self.active_phase = None

    def skip_phase(self, name: str, detail: str) -> None:
        completed = CompletedPhaseReport(name, "SKIP", None, 0.0, detail, ())
        self.completed_phases.append(completed)
        self._terminal_line(self._phase_line(completed), include_in_log=True)

    def finish_gate(self, phases: Iterable[object]) -> None:
        phase_list = list(phases)
        passed = sum(getattr(phase, "status", "") == "PASS" for phase in phase_list)
        failed = sum(getattr(phase, "status", "") == "FAIL" for phase in phase_list)
        skipped = sum(getattr(phase, "status", "") == "SKIP" for phase in phase_list)
        status = "FAIL" if failed else "PASS"
        self._terminal_line("[SUMMARY] FULL TEST RECAP", include_in_log=True)
        for phase in self.completed_phases:
            self._terminal_line(self._phase_line(phase), include_in_log=True)
            if phase.status != "FAIL":
                continue
            if phase.failures:
                for failure in phase.failures:
                    self._terminal_line(
                        self._failure_line(phase.name, failure, phase.detail),
                        include_in_log=True,
                    )
            elif phase.detail:
                self._terminal_line(
                    f"[FAIL] {phase.name} | {phase.detail}",
                    include_in_log=True,
                )
        parts = [f"[{status}] FULL TEST", f"{passed}/{passed + failed} phases passed"]
        if failed:
            parts.append(f"{failed} failed")
        if skipped:
            parts.append(f"{skipped} skipped")
        parts.append(f"log: {self.log_path}")
        self._terminal_line(" | ".join(parts), include_in_log=True)

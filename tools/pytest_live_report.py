"""Pytest plugin that emits one parseable failure line as each test fails."""

from __future__ import annotations

import re

from tools.full_test_report import PYTEST_FAILURE_PREFIX


WHITESPACE_RE = re.compile(r"\s+")
_terminal_reporter = None


def _failure_message(report) -> str:
    longrepr = getattr(report, "longrepr", None)
    reprcrash = getattr(longrepr, "reprcrash", None)
    message = getattr(reprcrash, "message", None)
    if not message:
        lines = [line.strip() for line in str(longrepr).splitlines() if line.strip()]
        message = lines[-1] if lines else f"{getattr(report, 'when', 'test')} failed"
    return WHITESPACE_RE.sub(" ", str(message)).strip()


def _emit_failure(report, nodeid: str) -> None:
    line = f"{PYTEST_FAILURE_PREFIX}\t{nodeid}\t{_failure_message(report)}"
    if _terminal_reporter is not None:
        _terminal_reporter.ensure_newline()
        _terminal_reporter.write_line(line)
        return
    print(line, flush=True)


def pytest_configure(config) -> None:
    global _terminal_reporter
    _terminal_reporter = config.pluginmanager.get_plugin("terminalreporter")


def pytest_unconfigure(config) -> None:
    global _terminal_reporter
    _terminal_reporter = None


def pytest_runtest_logreport(report) -> None:
    if not report.failed:
        return
    _emit_failure(report, report.nodeid)


def pytest_collectreport(report) -> None:
    if not report.failed:
        return
    nodeid = getattr(report, "nodeid", None) or "pytest collection"
    _emit_failure(report, nodeid)

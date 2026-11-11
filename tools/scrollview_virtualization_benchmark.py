#!/usr/bin/env python3
"""Measure the canonical virtual ScrollView pipeline without terminal I/O."""

from __future__ import annotations

import argparse
import sys
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "src"))

from uimd.runtime.UIScrollView import UIScrollView  # noqa: E402
from uimd.runtime.application import _TerminalFrameDiff  # noqa: E402
from uimd.runtime.rendering import TerminalCell  # noqa: E402


DEFAULT_LOGICAL_COUNTS = (1_000, 10_000, 100_000, 1_000_000)
DEFAULT_SAMPLE_COUNT = 40
BENCHMARK_VIEWPORT_WIDTH = 90
BENCHMARK_VIEWPORT_HEIGHT = 35
BENCHMARK_GAP = 1
FIXED_ROW_HEIGHT = 3
VARIABLE_ROW_HEIGHT_RANGE = 5


def elapsed_ns(callback):
    started = time.perf_counter_ns()
    value = callback()
    return time.perf_counter_ns() - started, value


def milliseconds(value):
    return value / 1_000_000.0


def percentile_95(values):
    ordered = sorted(values)
    index = max(0, min(len(ordered) - 1, round((len(ordered) - 1) * 0.95)))
    return ordered[index]


class BenchmarkRow:
    """Small variable-height row used only by the development benchmark."""

    ELEMENT_TYPE = "benchmark-row"

    def __init__(self):
        self.parent = None
        self._app = None
        self._window_width = 1
        self._window_height = 1
        self._logical_index = 0

    def bind(self, logical_index, height):
        self._logical_index = int(logical_index)
        self._window_height = max(1, int(height))

    def get_natural_size(self):
        return self._window_width, self._window_height

    def resize(self, width, height):
        self._window_width = max(1, int(width))
        self._window_height = max(1, int(height))

    def open(self):
        return None

    def render_cells(self):
        width = self._window_width
        label = str(self._logical_index)[:width]
        first = [TerminalCell(character) for character in label]
        first.extend(TerminalCell(" ") for _column in range(width - len(first)))
        blank = [TerminalCell(" ") for _column in range(width)]
        return [first] + [list(blank) for _row in range(self._window_height - 1)]


class BenchmarkScrollView(UIScrollView):
    """Expose phase timings while preserving the production implementation."""

    def __init__(self):
        super().__init__()
        self._benchmark_viewport = {
            "row": 0,
            "col": 0,
            "width": BENCHMARK_VIEWPORT_WIDTH,
            "height": BENCHMARK_VIEWPORT_HEIGHT,
            "gap": BENCHMARK_GAP,
        }
        self._window_width = BENCHMARK_VIEWPORT_WIDTH
        self._window_height = BENCHMARK_VIEWPORT_HEIGHT
        self.last_refresh_ns = 0
        self.height_measure_ns = 0
        self.row_render_ns = 0

    def _get_viewport_rect(self):
        return dict(self._benchmark_viewport)

    def _base_element_grid(self):
        return {}

    def reset_phase_timers(self):
        self.last_refresh_ns = 0
        self.height_measure_ns = 0
        self.row_render_ns = 0

    def _refresh_virtual_materialization(self, viewport):
        started = time.perf_counter_ns()
        try:
            return super()._refresh_virtual_materialization(viewport)
        finally:
            self.last_refresh_ns = time.perf_counter_ns() - started

    def _measure_child_height(self, child, width):
        started = time.perf_counter_ns()
        try:
            return super()._measure_child_height(child, width)
        finally:
            self.height_measure_ns += time.perf_counter_ns() - started

    def _render_child_cells(self, child, width, clip=None):
        started = time.perf_counter_ns()
        try:
            return super()._render_child_cells(child, width, clip=clip)
        finally:
            self.row_render_ns += time.perf_counter_ns() - started


def benchmark_case(logical_count, variable_heights, sample_count):
    view = BenchmarkScrollView()
    materializer_ns = 0
    constructed_rows = 0

    def row_height(index):
        if not variable_heights:
            return FIXED_ROW_HEIGHT
        return 1 + (index % VARIABLE_ROW_HEIGHT_RANGE)

    def materialize(index, recycled):
        nonlocal materializer_ns, constructed_rows
        started = time.perf_counter_ns()
        row = recycled
        if row is None:
            row = BenchmarkRow()
            constructed_rows += 1
        row.bind(index, row_height(index))
        materializer_ns += time.perf_counter_ns() - started
        return row

    estimated_height = FIXED_ROW_HEIGHT if not variable_heights else 3
    configure_ns, _unused = elapsed_ns(
        lambda: view.set_virtual_data_source(
            logical_count,
            lambda index: index,
            materialize,
            estimated_height=estimated_height,
        )
    )
    viewport = view._clamped_viewport_rect()
    height_index_ns, _unused = elapsed_ns(lambda: view._ensure_virtual_height_index(viewport))

    view.reset_phase_timers()
    materializer_ns = 0
    initial_refresh_ns, _unused = elapsed_ns(lambda: view.child_view_entries(viewport))
    initial_measure_ns = view.height_measure_ns
    initial_materializer_ns = materializer_ns
    initial_diagnostics = view.virtual_diagnostics()

    view.scroll_to(logical_count // 2)
    view.render_cells()
    terminal_diff = _TerminalFrameDiff(
        lambda: (BENCHMARK_VIEWPORT_WIDTH, BENCHMARK_VIEWPORT_HEIGHT)
    )
    terminal_diff.render([(view.render_cells(), 0, 0)])

    layout_samples = []
    visible_overhead_samples = []
    render_samples = []
    row_render_samples = []
    terminal_diff_samples = []
    frame_samples = []
    event_samples = []

    for sample_index in range(sample_count):
        key = "ArrowDown" if sample_index % 2 == 0 else "ArrowUp"
        event_ns, _unused = elapsed_ns(lambda key=key: view.handle_key(key))
        event_samples.append(event_ns)

        view.reset_phase_timers()
        materializer_ns = 0
        refresh_ns, _unused = elapsed_ns(lambda: view.child_view_entries(viewport))
        layout_samples.append(refresh_ns)
        visible_overhead_samples.append(
            max(0, refresh_ns - view.height_measure_ns - materializer_ns)
        )

        view.reset_phase_timers()
        render_ns, rows = elapsed_ns(view.render_cells)
        render_samples.append(max(0, render_ns - view.last_refresh_ns))
        row_render_samples.append(view.row_render_ns)

        diff_ns, _frame = elapsed_ns(lambda rows=rows: terminal_diff.render([(rows, 0, 0)]))
        terminal_diff_samples.append(diff_ns)
        frame_samples.append(render_ns + diff_ns)

    final_diagnostics = view.virtual_diagnostics()
    return {
        "items": logical_count,
        "shape": "variable" if variable_heights else "fixed",
        "configure_ms": milliseconds(configure_ns),
        "height_index_ms": milliseconds(height_index_ns),
        "initial_refresh_ms": milliseconds(initial_refresh_ns),
        "initial_child_bind_ms": milliseconds(initial_materializer_ns),
        "initial_height_measure_ms": milliseconds(initial_measure_ns),
        "constructed_rows": constructed_rows,
        "materialized_rows": final_diagnostics["materialized_rows"],
        "initial_rows_considered": initial_diagnostics["last_rows_considered"],
        "layout_p95_ms": milliseconds(percentile_95(layout_samples)),
        "visible_overhead_p95_ms": milliseconds(percentile_95(visible_overhead_samples)),
        "render_p95_ms": milliseconds(percentile_95(render_samples)),
        "row_render_p95_ms": milliseconds(percentile_95(row_render_samples)),
        "terminal_diff_p95_ms": milliseconds(percentile_95(terminal_diff_samples)),
        "frame_p95_ms": milliseconds(percentile_95(frame_samples)),
        "event_p95_ms": milliseconds(percentile_95(event_samples)),
        "rows_considered": final_diagnostics["last_rows_considered"],
        "rows_measured": final_diagnostics["last_rows_measured"],
        "rows_rendered": final_diagnostics["last_rows_rendered"],
    }


def print_results(results, sample_count):
    print(
        "ScrollView virtualization reference benchmark "
        f"({sample_count} warm samples, {BENCHMARK_VIEWPORT_WIDTH}x{BENCHMARK_VIEWPORT_HEIGHT})"
    )
    print(
        "items shape    setup index first bind measure made live "
        "layout visible render rows diff frame event | considered/measured/rendered (p95 ms)"
    )
    for result in results:
        print(
            f"{result['items']:>7} {result['shape']:<8} "
            f"{result['configure_ms']:>5.3f} {result['height_index_ms']:>5.3f} "
            f"{result['initial_refresh_ms']:>5.3f} {result['initial_child_bind_ms']:>5.3f} "
            f"{result['initial_height_measure_ms']:>7.3f} "
            f"{result['constructed_rows']:>4} {result['materialized_rows']:>4} "
            f"{result['layout_p95_ms']:>6.3f} {result['visible_overhead_p95_ms']:>7.3f} "
            f"{result['render_p95_ms']:>6.3f} {result['row_render_p95_ms']:>5.3f} "
            f"{result['terminal_diff_p95_ms']:>5.3f} {result['frame_p95_ms']:>5.3f} "
            f"{result['event_p95_ms']:>5.3f} | "
            f"{result['rows_considered']}/{result['rows_measured']}/{result['rows_rendered']}"
        )


def parse_args(argv):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--samples",
        type=int,
        default=DEFAULT_SAMPLE_COUNT,
        help="warm samples per size and row-height shape",
    )
    parser.add_argument(
        "--counts",
        type=int,
        nargs="+",
        default=DEFAULT_LOGICAL_COUNTS,
        help="logical item counts to benchmark",
    )
    return parser.parse_args(argv)


def main(argv=None):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    if args.samples <= 0:
        raise SystemExit("--samples must be positive")
    if any(count <= 0 for count in args.counts):
        raise SystemExit("--counts values must be positive")
    results = [
        benchmark_case(count, variable_heights, args.samples)
        for count in args.counts
        for variable_heights in (False, True)
    ]
    print_results(results, args.samples)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

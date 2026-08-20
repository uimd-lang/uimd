#!/usr/bin/env python3
"""Direct PTY smoke tests for Java terminal runtime parity with C++."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import sys

try:
    from rust_direct_terminal_smoke import (
        DirectTerminalTarget,
        ROOT,
        run_calculator_parity,
        run_dialog_escape_flash,
        run_formular_input_cases,
        run_image_diff_and_quit,
        run_idle_text_gradient_animation,
        run_listbox_mouse_selection,
        run_mouse_selection,
        run_signal_teardown,
        run_title_ctrl_c_and_teardown,
    )
    from swift_direct_terminal_smoke import (
        run_image_browser_mouse_wheel_burst_case,
        run_image_browser_repeated_sixel_scroll_case,
        run_image_browser_sixel_cell_geometry_case,
    )
except ModuleNotFoundError:
    from tools.rust_direct_terminal_smoke import (
        DirectTerminalTarget,
        ROOT,
        run_calculator_parity,
        run_dialog_escape_flash,
        run_formular_input_cases,
        run_image_diff_and_quit,
        run_idle_text_gradient_animation,
        run_listbox_mouse_selection,
        run_mouse_selection,
        run_signal_teardown,
        run_title_ctrl_c_and_teardown,
    )
    from tools.swift_direct_terminal_smoke import (
        run_image_browser_mouse_wheel_burst_case,
        run_image_browser_repeated_sixel_scroll_case,
        run_image_browser_sixel_cell_geometry_case,
    )


JAVA_TARGET = DirectTerminalTarget("Java", "java")


def executable(examples_dir: Path, name: str) -> Path:
    return ROOT / examples_dir / name / "build" / "install" / name / "bin" / name


def check_binaries(cpp_build_dir: Path, java_examples_dir: Path) -> None:
    paths = [
        ROOT / cpp_build_dir / "examples/calculator/calculator",
        ROOT / cpp_build_dir / "examples/contacts_manager/contacts_manager",
        ROOT / cpp_build_dir / "examples/formular/formular",
        ROOT / cpp_build_dir / "examples/special_elements/special_elements",
        ROOT / cpp_build_dir / "examples/widget_gallery/widget_gallery",
        executable(java_examples_dir, "calculator"),
        executable(java_examples_dir, "contacts_manager"),
        executable(java_examples_dir, "formular"),
        executable(java_examples_dir, "image_browser"),
        executable(java_examples_dir, "image_gallery"),
        executable(java_examples_dir, "task_board"),
        executable(java_examples_dir, "special_elements"),
        executable(java_examples_dir, "widget_gallery"),
    ]
    sys.path.insert(0, str(ROOT / "src"))
    from uimd.testing.artifact_manifest import validate_artifact_paths

    validate_artifact_paths(ROOT, paths)
    missing = [path for path in paths if not path.exists()]
    if missing:
        details = "\n".join(f"  - {path.relative_to(ROOT)}" for path in missing)
        raise FileNotFoundError(
            f"missing Java direct-terminal smoke binary:\n{details}"
        )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cpp-build-dir", default="cpp/build", type=Path)
    parser.add_argument("--java-examples-dir", default="java/examples", type=Path)
    args = parser.parse_args()

    if os.name != "posix":
        print("SKIP Java direct terminal smoke: POSIX PTY is required", flush=True)
        return 0

    check_binaries(args.cpp_build_dir, args.java_examples_dir)
    cpp_calculator = [str(ROOT / args.cpp_build_dir / "examples/calculator/calculator")]
    cpp_contacts_manager = [
        str(ROOT / args.cpp_build_dir / "examples/contacts_manager/contacts_manager")
    ]
    cpp_formular = [str(ROOT / args.cpp_build_dir / "examples/formular/formular")]
    cpp_special_elements = [
        str(ROOT / args.cpp_build_dir / "examples/special_elements/special_elements")
    ]
    cpp_widget_gallery = [
        str(ROOT / args.cpp_build_dir / "examples/widget_gallery/widget_gallery")
    ]
    java_calculator = [str(executable(args.java_examples_dir, "calculator"))]
    java_contacts_manager = [
        str(executable(args.java_examples_dir, "contacts_manager"))
    ]
    java_formular = [str(executable(args.java_examples_dir, "formular"))]
    java_special_elements = [
        str(executable(args.java_examples_dir, "special_elements"))
    ]
    java_task_board = [str(executable(args.java_examples_dir, "task_board"))]
    java_widget_gallery = [str(executable(args.java_examples_dir, "widget_gallery"))]
    java_image_browser = [
        "env",
        "UIMD_FORCE_SIXEL=1",
        str(executable(args.java_examples_dir, "image_browser")),
    ]
    java_image_gallery = [
        "env",
        "UIMD_DISABLE_SIXEL=1",
        str(executable(args.java_examples_dir, "image_gallery")),
    ]

    run_title_ctrl_c_and_teardown(java_calculator, JAVA_TARGET)
    run_signal_teardown(java_calculator, JAVA_TARGET)
    run_calculator_parity(cpp_calculator, java_calculator, JAVA_TARGET)
    run_formular_input_cases(cpp_formular, java_formular, JAVA_TARGET)
    run_listbox_mouse_selection(
        cpp_contacts_manager,
        java_contacts_manager,
        cpp_formular,
        java_formular,
        JAVA_TARGET,
    )
    run_idle_text_gradient_animation(
        cpp_special_elements,
        java_special_elements,
        JAVA_TARGET,
    )
    run_mouse_selection(cpp_widget_gallery, java_widget_gallery, JAVA_TARGET)
    run_dialog_escape_flash(java_task_board, JAVA_TARGET)
    run_image_diff_and_quit(java_image_browser, java_image_gallery, JAVA_TARGET)
    run_image_browser_sixel_cell_geometry_case(
        "Java image_browser Sixel rows use physical terminal-cell geometry",
        "Java",
        "java/examples",
    )
    run_image_browser_repeated_sixel_scroll_case(
        "image_browser repeated Sixel scrolling stays bounded including Java",
    )
    run_image_browser_mouse_wheel_burst_case(
        "image_browser sustained mouse-wheel burst stays responsive including Java",
    )
    print("PASS Java direct terminal smoke: 12/12 groups passed", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())

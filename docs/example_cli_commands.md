# Example CLI Commands

Run commands from the repository root.

These commands assume a source checkout where UIMD is not installed yet. Build
the repo-local native launcher first with `cmake --build cpp/build --target
uimd`, then run the universal checkout entry point as `./uimd`. Legacy Python
compiler wrappers have been removed; use `./uimd` for all generation and run
commands.

## Full Rebuild

```bash
./tools/rebuild_all.sh
```
## Full Rebuild and Test
```bash
./tools/rebuild_all.sh
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
```

## Python Examples

Generate the UI source and run the Python app through the native UIMD launcher:

```bash
./uimd run python/examples/activity_feed/activity_feed.uimd
./uimd run python/examples/calculator/calculator.uimd
./uimd run python/examples/cells/cells.uimd
./uimd run python/examples/contacts_manager/contacts_manager.uimd
./uimd run python/examples/expense_tracker/expense_tracker.uimd
./uimd run python/examples/formular/formular.uimd
./uimd run python/examples/image_browser/image_browser.uimd
./uimd run python/examples/image_gallery/image_gallery.uimd
./uimd run python/examples/markdown_viewer/markdown_viewer.uimd
./uimd run python/examples/special_elements/special_elements.uimd
./uimd run python/examples/task_board/task_board.uimd
./uimd run python/examples/text_editor/text_editor.uimd
./uimd run python/examples/widget_gallery/widget_gallery.uimd
```

Run already-generated Python apps directly:

```bash
python3 python/examples/activity_feed/activity_feed.py
python3 python/examples/calculator/calculator.py
python3 python/examples/cells/cells.py
python3 python/examples/contacts_manager/contacts_manager.py
python3 python/examples/expense_tracker/expense_tracker.py
python3 python/examples/formular/formular.py
python3 python/examples/image_browser/image_browser.py
python3 python/examples/image_gallery/image_gallery.py
python3 python/examples/markdown_viewer/markdown_viewer.py
python3 python/examples/special_elements/special_elements.py
python3 python/examples/task_board/task_board.py
python3 python/examples/text_editor/text_editor.py
python3 python/examples/widget_gallery/widget_gallery.py
```

## C++ Examples

```bash
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target activity_feed && cpp/build/examples/activity_feed/activity_feed
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target calculator && cpp/build/examples/calculator/calculator
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target cells && cpp/build/examples/cells/cells
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target contacts_manager && cpp/build/examples/contacts_manager/contacts_manager
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target expense_tracker && cpp/build/examples/expense_tracker/expense_tracker
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target formular && cpp/build/examples/formular/formular
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target image_browser && cpp/build/examples/image_browser/image_browser
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target image_gallery && cpp/build/examples/image_gallery/image_gallery
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target markdown_viewer && cpp/build/examples/markdown_viewer/markdown_viewer
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target special_elements && cpp/build/examples/special_elements/special_elements
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target task_board && cpp/build/examples/task_board/task_board
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target text_editor && cpp/build/examples/text_editor/text_editor
./uimd generate cpp/examples --target cpp && cmake --build cpp/build --target widget_gallery && cpp/build/examples/widget_gallery/widget_gallery
```

## Source Regeneration

```bash
./uimd generate python/dialogs --target python
./uimd generate python/examples --target python
./uimd generate src/uimd/dialogs --target python
./uimd generate src/uimd/testing --target python
./uimd generate cpp/dialogs --target cpp
./uimd generate cpp/examples --target cpp
```

## Bulk Rebuild

```bash
./uimd generate python/dialogs --target python
./uimd generate python/examples --target python
./uimd generate src/uimd/dialogs --target python
./uimd generate src/uimd/testing --target python
./uimd generate cpp/dialogs --target cpp
./uimd generate cpp/examples --target cpp
cmake --build cpp/build
python3 -m compileall python src tests tools
```

## MCP Tester UI

```bash
./uimd generate src/uimd/testing --target python
cmake --build cpp/build --target uimd_mcp_tester
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp.py python/tests/test_mcp_tester.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py -k python_and_cpp_tester_backends_have_small_script_parity
```

`./uimd mcp-test` defaults to the C++ tester. Use `--backend python` only when
explicitly running the Python tester implementation.

## Python Unit Tests

```bash
PYTHONPATH=python:src python3 -m pytest python/tests
PYTHONPATH=python:src python3 -m pytest python/tests/test_activity_feed.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_application.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_calculator.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_dialogs.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_transports.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_style.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_text_editor.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_uiinstance.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_uiwindow.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_version_consistency.py
```

## C++ Unit Tests

```bash
cmake --build cpp/build --target ui_cpp_tests && cpp/build/ui_cpp_tests
ctest --test-dir cpp/build --output-on-failure
```

## Native CLI Smoke Tests

```bash
cmake --build cpp/build --target uimd
cmake --build cpp/build --target uimd_init
python3 tools/native_uimd_parity.py
python3 tools/native_uimd_parity.py --compile-examples
```

## Local SDK Release Artifact

```bash
python3 tools/package_sdk_release.py --build --output dist/sdk-release
UIMD_HOME=/tmp/uimd-home \
  cpp/build-release/tools/uimd/uimd sdk install 0.3.1 --release-root dist/sdk-release
UIMD_HOME=/tmp/uimd-home /tmp/uimd-home/bin/uimd doctor --json
UIMD_HOME=/tmp/uimd-home-from-script \
UIMD_RELEASE_BASE_URL=file://$PWD/dist/sdk-release \
  sh dist/sdk-release/install.sh --no-shell-config --json
```

## Python App MCP Tests

```bash
./uimd mcp-test python/examples tests/mcp/all_examples.yaml
./uimd mcp-test python/examples/activity_feed/activity_feed.py tests/mcp/activity_feed.yaml
./uimd mcp-test python/examples/calculator/calculator.py tests/mcp/calculator.yaml
./uimd mcp-test python/examples/cells/cells.py tests/mcp/cells.yaml
./uimd mcp-test python/examples/contacts_manager/contacts_manager.py tests/mcp/contacts_manager.yaml
./uimd mcp-test python/examples/expense_tracker/expense_tracker.py tests/mcp/expense_tracker_compare.yaml
./uimd mcp-test python/examples/formular/formular.py tests/mcp/formular.yaml
./uimd mcp-test python/examples/image_browser/image_browser.py tests/mcp/image_browser_compare.yaml
./uimd mcp-test python/examples/image_gallery/image_gallery.py tests/mcp/image_gallery_compare.yaml
./uimd mcp-test python/examples/markdown_viewer/markdown_viewer.py tests/mcp/markdown_viewer.yaml
./uimd mcp-test python/examples/special_elements/special_elements.py tests/mcp/special_elements.yaml
./uimd mcp-test python/examples/task_board/task_board.py tests/mcp/task_board_compare.yaml
./uimd mcp-test python/examples/text_editor/text_editor.py tests/mcp/text_editor.yaml
./uimd mcp-test python/examples/widget_gallery/widget_gallery.py tests/mcp/widget_gallery.yaml
```

## C++ App MCP Tests

```bash
./uimd mcp-test cpp/build/examples tests/mcp/all_examples.yaml
./uimd mcp-test cpp/build/examples/activity_feed/activity_feed tests/mcp/activity_feed.yaml
./uimd mcp-test cpp/build/examples/calculator/calculator tests/mcp/calculator.yaml
./uimd mcp-test cpp/build/examples/cells/cells tests/mcp/cells.yaml
./uimd mcp-test cpp/build/examples/contacts_manager/contacts_manager tests/mcp/contacts_manager.yaml
./uimd mcp-test cpp/build/examples/expense_tracker/expense_tracker tests/mcp/expense_tracker_compare.yaml
./uimd mcp-test cpp/build/examples/formular/formular tests/mcp/formular.yaml
./uimd mcp-test cpp/build/examples/image_browser/image_browser tests/mcp/image_browser_compare.yaml
./uimd mcp-test cpp/build/examples/image_gallery/image_gallery tests/mcp/image_gallery_compare.yaml
./uimd mcp-test cpp/build/examples/markdown_viewer/markdown_viewer tests/mcp/markdown_viewer.yaml
./uimd mcp-test cpp/build/examples/special_elements/special_elements tests/mcp/special_elements.yaml
./uimd mcp-test cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml
./uimd mcp-test cpp/build/examples/text_editor/text_editor tests/mcp/text_editor.yaml
./uimd mcp-test cpp/build/examples/widget_gallery/widget_gallery tests/mcp/widget_gallery.yaml
```

## Compare MCP Tests

```bash
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-action-delay-ms 1 --mcp-type-delay-ms 1 --step-delay-ms 1 --compare-app-size 90x35
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --all --compare python/examples cpp/build/examples --compare-app-size 90x35
./uimd mcp-test --compare python/examples/activity_feed/activity_feed.py cpp/build/examples/activity_feed/activity_feed tests/mcp/activity_feed.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/calculator/calculator.py cpp/build/examples/calculator/calculator tests/mcp/calculator.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/cells/cells.py cpp/build/examples/cells/cells tests/mcp/cells.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/contacts_manager/contacts_manager.py cpp/build/examples/contacts_manager/contacts_manager tests/mcp/contacts_manager.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/expense_tracker/expense_tracker.py cpp/build/examples/expense_tracker/expense_tracker tests/mcp/expense_tracker_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/formular/formular.py cpp/build/examples/formular/formular tests/mcp/formular.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/image_browser/image_browser.py cpp/build/examples/image_browser/image_browser tests/mcp/image_browser_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/image_gallery/image_gallery.py cpp/build/examples/image_gallery/image_gallery tests/mcp/image_gallery_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/markdown_viewer/markdown_viewer.py cpp/build/examples/markdown_viewer/markdown_viewer tests/mcp/markdown_viewer.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/special_elements/special_elements.py cpp/build/examples/special_elements/special_elements tests/mcp/special_elements.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/task_board/task_board.py cpp/build/examples/task_board/task_board tests/mcp/task_board_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/text_editor/text_editor.py cpp/build/examples/text_editor/text_editor tests/mcp/text_editor.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/widget_gallery/widget_gallery.py cpp/build/examples/widget_gallery/widget_gallery tests/mcp/widget_gallery.yaml --compare-app-size 90x35 --mcp-fast
```

## Project Scaffold Smoke Test

```bash
./uimd new hello --target python
./uimd run hello.uimd
./uimd generate hello.uimd --target cpp
cmake -S . -B build
cmake --build build
./build/hello
```

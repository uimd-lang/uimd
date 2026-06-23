# Example CLI Commands

Run commands from the repository root.

These commands assume a source checkout where UIMD is not installed yet. POSIX
shells use the checkout entry point as `./uimd`; Windows PowerShell uses
`.\uimd.ps1`, and Windows `cmd.exe` uses `.\uimd.cmd`; both locate the
repo-local native `uimd.exe`. Legacy Python compiler wrappers have been removed;
use the native UIMD launcher for all generation and run commands.

C++ build outputs are local artifacts under `cpp/build` or `cpp/build-windows`.
C# build outputs are local artifacts under `csharp/examples/*/bin`. They are
not committed to Git.

Python tests require `pytest` in the Python environment used by the `python` or
`python3` command. Install it once if `python -m pytest ...` reports
`No module named pytest`.

Windows over SSH / cmd.exe:

```bat
python -m pip install pytest
```

macOS/Linux:

```bash
python3 -m pip install pytest
```

For Windows over SSH or `cmd.exe`, use the `.cmd` commands. Do not run `.ps1`
scripts from that shell. `.ps1` scripts are only for an actual PowerShell
session.

On Windows interactive SSH, `cmd.exe`, or PowerShell sessions, `mcp-test
--compare` opens the visual tester UI. Target apps use Windows ConPTY capture
when that API is available and passes the built-in smoke test; otherwise they
fall back to headless MCP render frames so the side-by-side panes still work.
Non-interactive one-shot SSH commands fall back to console progress output
(`tester ready`, `SCRIPT`, `COMPARE`, `RESULT`).

If you run a command as a one-shot SSH remote command, for example
`ssh host ".\tools\test_all.cmd"`, SSH closes after that command
finishes. In an interactive SSH session, run the `.cmd` commands at the prompt.

## Full Rebuild

macOS/Linux (POSIX shell):

```bash
./tools/rebuild_all.sh
```

Windows over SSH / cmd.exe:

```bat
.\tools\rebuild_all.cmd
```

Windows PowerShell only:

```powershell
.\tools\rebuild_all.ps1
```

## Full Rebuild and Test

This runs the full local gate: regenerate/build all supported sources including
reported-bug regression corpora, build C# examples, compile Python sources, run
Python unit tests, run C++ `ctest`, run Python/C++ and C++/C# MCP example
compare tests with `--compare-app-size 90x35`, and run the UIMD regression
parity compare corpus when `tests/regressions/uimd/parity` exists.

macOS/Linux (POSIX shell):

```bash
./tools/test_all.sh
```

Equivalent explicit command sequence:

```bash
./tools/rebuild_all.sh
python3 -m pytest python/tests
ctest --test-dir cpp/build --output-on-failure
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --all --compare cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35
```

Windows over SSH / cmd.exe:

```bat
.\tools\test_all.cmd
```

Equivalent explicit command sequence:

```bat
.\tools\rebuild_all.cmd -Test
python -m pytest python\tests
.\uimd.cmd mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
```

Windows PowerShell only:

```powershell
.\tools\test_all.ps1
```

Equivalent explicit command sequence:

```powershell
.\tools\rebuild_all.ps1 -Test
python -m pytest python\tests
.\uimd.ps1 mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
```

## Windows Sixel Install

For Python/C++ image examples that require non-fallback Sixel rendering:

```bat
.\tools\install_sixel_windows.cmd
```

## Python Examples

Generate the UI source and run the Python app through the native UIMD launcher:

macOS/Linux (POSIX shell):

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

Windows over SSH / cmd.exe:

```bat
.\uimd.cmd run python\examples\activity_feed\activity_feed.uimd
.\uimd.cmd run python\examples\calculator\calculator.uimd
.\uimd.cmd run python\examples\cells\cells.uimd
.\uimd.cmd run python\examples\contacts_manager\contacts_manager.uimd
.\uimd.cmd run python\examples\expense_tracker\expense_tracker.uimd
.\uimd.cmd run python\examples\formular\formular.uimd
.\uimd.cmd run python\examples\image_browser\image_browser.uimd
.\uimd.cmd run python\examples\image_gallery\image_gallery.uimd
.\uimd.cmd run python\examples\markdown_viewer\markdown_viewer.uimd
.\uimd.cmd run python\examples\special_elements\special_elements.uimd
.\uimd.cmd run python\examples\task_board\task_board.uimd
.\uimd.cmd run python\examples\text_editor\text_editor.uimd
.\uimd.cmd run python\examples\widget_gallery\widget_gallery.uimd
```

Windows PowerShell only:

```powershell
.\uimd.ps1 run python\examples\activity_feed\activity_feed.uimd
.\uimd.ps1 run python\examples\calculator\calculator.uimd
.\uimd.ps1 run python\examples\cells\cells.uimd
.\uimd.ps1 run python\examples\contacts_manager\contacts_manager.uimd
.\uimd.ps1 run python\examples\expense_tracker\expense_tracker.uimd
.\uimd.ps1 run python\examples\formular\formular.uimd
.\uimd.ps1 run python\examples\image_browser\image_browser.uimd
.\uimd.ps1 run python\examples\image_gallery\image_gallery.uimd
.\uimd.ps1 run python\examples\markdown_viewer\markdown_viewer.uimd
.\uimd.ps1 run python\examples\special_elements\special_elements.uimd
.\uimd.ps1 run python\examples\task_board\task_board.uimd
.\uimd.ps1 run python\examples\text_editor\text_editor.uimd
.\uimd.ps1 run python\examples\widget_gallery\widget_gallery.uimd
```

Run already-generated Python apps directly:

macOS/Linux (POSIX shell):

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

Windows over SSH / cmd.exe:

```bat
python python\examples\activity_feed\activity_feed.py
python python\examples\calculator\calculator.py
python python\examples\cells\cells.py
python python\examples\contacts_manager\contacts_manager.py
python python\examples\expense_tracker\expense_tracker.py
python python\examples\formular\formular.py
python python\examples\image_browser\image_browser.py
python python\examples\image_gallery\image_gallery.py
python python\examples\markdown_viewer\markdown_viewer.py
python python\examples\special_elements\special_elements.py
python python\examples\task_board\task_board.py
python python\examples\text_editor\text_editor.py
python python\examples\widget_gallery\widget_gallery.py
```

## C++ Examples

Recommended cross-platform helper commands:

POSIX:

```bash
./tools/run_cpp_example.sh activity_feed
./tools/run_cpp_example.sh calculator
./tools/run_cpp_example.sh cells
./tools/run_cpp_example.sh contacts_manager
./tools/run_cpp_example.sh expense_tracker
./tools/run_cpp_example.sh formular
./tools/run_cpp_example.sh image_browser
./tools/run_cpp_example.sh image_gallery
./tools/run_cpp_example.sh markdown_viewer
./tools/run_cpp_example.sh special_elements
./tools/run_cpp_example.sh task_board
./tools/run_cpp_example.sh text_editor
./tools/run_cpp_example.sh widget_gallery
```

Windows over SSH / cmd.exe:

```bat
.\tools\run_cpp_example.cmd activity_feed
.\tools\run_cpp_example.cmd calculator
.\tools\run_cpp_example.cmd cells
.\tools\run_cpp_example.cmd contacts_manager
.\tools\run_cpp_example.cmd expense_tracker
.\tools\run_cpp_example.cmd formular
.\tools\run_cpp_example.cmd image_browser
.\tools\run_cpp_example.cmd image_gallery
.\tools\run_cpp_example.cmd markdown_viewer
.\tools\run_cpp_example.cmd special_elements
.\tools\run_cpp_example.cmd task_board
.\tools\run_cpp_example.cmd text_editor
.\tools\run_cpp_example.cmd widget_gallery
```

Run already-built Windows example executables directly:

```bat
.\cpp\build-windows\examples\activity_feed\Release\activity_feed.exe
.\cpp\build-windows\examples\calculator\Release\calculator.exe
.\cpp\build-windows\examples\cells\Release\cells.exe
.\cpp\build-windows\examples\contacts_manager\Release\contacts_manager.exe
.\cpp\build-windows\examples\expense_tracker\Release\expense_tracker.exe
.\cpp\build-windows\examples\formular\Release\formular.exe
.\cpp\build-windows\examples\image_browser\Release\image_browser.exe
.\cpp\build-windows\examples\image_gallery\Release\image_gallery.exe
.\cpp\build-windows\examples\markdown_viewer\Release\markdown_viewer.exe
.\cpp\build-windows\examples\special_elements\Release\special_elements.exe
.\cpp\build-windows\examples\task_board\Release\task_board.exe
.\cpp\build-windows\examples\text_editor\Release\text_editor.exe
.\cpp\build-windows\examples\widget_gallery\Release\widget_gallery.exe
```

Windows PowerShell:

```powershell
.\tools\run_cpp_example.ps1 activity_feed
.\tools\run_cpp_example.ps1 calculator
.\tools\run_cpp_example.ps1 cells
.\tools\run_cpp_example.ps1 contacts_manager
.\tools\run_cpp_example.ps1 expense_tracker
.\tools\run_cpp_example.ps1 formular
.\tools\run_cpp_example.ps1 image_browser
.\tools\run_cpp_example.ps1 image_gallery
.\tools\run_cpp_example.ps1 markdown_viewer
.\tools\run_cpp_example.ps1 special_elements
.\tools\run_cpp_example.ps1 task_board
.\tools\run_cpp_example.ps1 text_editor
.\tools\run_cpp_example.ps1 widget_gallery
```

Raw macOS/Linux POSIX shell form:

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

Raw Windows PowerShell form for one example:

```powershell
.\uimd.ps1 generate cpp\examples --target cpp
cmake --build cpp\build-windows --target activity_feed --config Release
.\cpp\build-windows\examples\activity_feed\Release\activity_feed.exe
```

## C# Examples

Recommended cross-platform helper commands:

POSIX:

```bash
python3 tools/uimd_dev.py run-csharp-example activity_feed
python3 tools/uimd_dev.py run-csharp-example calculator
python3 tools/uimd_dev.py run-csharp-example cells
python3 tools/uimd_dev.py run-csharp-example contacts_manager
python3 tools/uimd_dev.py run-csharp-example expense_tracker
python3 tools/uimd_dev.py run-csharp-example formular
python3 tools/uimd_dev.py run-csharp-example image_browser
python3 tools/uimd_dev.py run-csharp-example image_gallery
python3 tools/uimd_dev.py run-csharp-example markdown_viewer
python3 tools/uimd_dev.py run-csharp-example special_elements
python3 tools/uimd_dev.py run-csharp-example task_board
python3 tools/uimd_dev.py run-csharp-example text_editor
python3 tools/uimd_dev.py run-csharp-example widget_gallery
```

Raw macOS/Linux POSIX shell form:

```bash
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/activity_feed/activity_feed.csproj --configuration Debug && dotnet csharp/examples/activity_feed/bin/Debug/net10.0/activity_feed.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/calculator/calculator.csproj --configuration Debug && dotnet csharp/examples/calculator/bin/Debug/net10.0/calculator.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/cells/cells.csproj --configuration Debug && dotnet csharp/examples/cells/bin/Debug/net10.0/cells.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/contacts_manager/contacts_manager.csproj --configuration Debug && dotnet csharp/examples/contacts_manager/bin/Debug/net10.0/contacts_manager.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/expense_tracker/expense_tracker.csproj --configuration Debug && dotnet csharp/examples/expense_tracker/bin/Debug/net10.0/expense_tracker.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/formular/formular.csproj --configuration Debug && dotnet csharp/examples/formular/bin/Debug/net10.0/formular.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/image_browser/image_browser.csproj --configuration Debug && dotnet csharp/examples/image_browser/bin/Debug/net10.0/image_browser.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/image_gallery/image_gallery.csproj --configuration Debug && dotnet csharp/examples/image_gallery/bin/Debug/net10.0/image_gallery.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/markdown_viewer/markdown_viewer.csproj --configuration Debug && dotnet csharp/examples/markdown_viewer/bin/Debug/net10.0/markdown_viewer.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/special_elements/special_elements.csproj --configuration Debug && dotnet csharp/examples/special_elements/bin/Debug/net10.0/special_elements.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/task_board/task_board.csproj --configuration Debug && dotnet csharp/examples/task_board/bin/Debug/net10.0/task_board.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/text_editor/text_editor.csproj --configuration Debug && dotnet csharp/examples/text_editor/bin/Debug/net10.0/text_editor.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/widget_gallery/widget_gallery.csproj --configuration Debug && dotnet csharp/examples/widget_gallery/bin/Debug/net10.0/widget_gallery.dll
```

Raw Windows PowerShell form for one example:

```powershell
.\uimd.ps1 generate csharp\examples --target csharp
dotnet build csharp\examples\activity_feed\activity_feed.csproj --configuration Debug
dotnet csharp\examples\activity_feed\bin\Debug\net10.0\activity_feed.dll
```

## Source Regeneration

```bash
./uimd generate python/dialogs --target python
./uimd generate python/examples --target python
./uimd generate src/uimd/dialogs --target python
./uimd generate src/uimd/testing --target python
./uimd generate cpp/dialogs --target cpp
./uimd generate cpp/examples --target cpp
./uimd generate csharp/examples --target csharp
./uimd generate tests/regressions/uimd/parity/python --target python
./uimd generate tests/regressions/uimd/parity/cpp --target cpp
```

## Bulk Rebuild

POSIX raw form:

```bash
./uimd generate python/dialogs --target python
./uimd generate python/examples --target python
./uimd generate src/uimd/dialogs --target python
./uimd generate src/uimd/testing --target python
./uimd generate cpp/dialogs --target cpp
./uimd generate cpp/examples --target cpp
./uimd generate csharp/examples --target csharp
./uimd generate tests/regressions/uimd/parity/python --target python
./uimd generate tests/regressions/uimd/parity/cpp --target cpp
cmake -S cpp -B cpp/build
cmake --build cpp/build
for proj in csharp/examples/*/*.csproj; do dotnet build "$proj" --configuration Debug; done
python3 -m compileall python src tests tools
```

Windows raw form:

```powershell
.\uimd.ps1 generate python\dialogs --target python
.\uimd.ps1 generate python\examples --target python
.\uimd.ps1 generate src\uimd\dialogs --target python
.\uimd.ps1 generate src\uimd\testing --target python
.\uimd.ps1 generate cpp\dialogs --target cpp
.\uimd.ps1 generate cpp\examples --target cpp
.\uimd.ps1 generate csharp\examples --target csharp
.\uimd.ps1 generate tests\regressions\uimd\parity\python --target python
.\uimd.ps1 generate tests\regressions\uimd\parity\cpp --target cpp
cmake -S cpp -B cpp\build-windows -G "Visual Studio 17 2022" -A x64
cmake --build cpp\build-windows --config Release
Get-ChildItem csharp\examples -Filter *.csproj -Recurse | ForEach-Object { dotnet build $_.FullName --configuration Debug }
python -m compileall python src tests tools
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
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_configured_sixel_library_lookup_honors_directory_override
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_configured_sixel_library_lookup_overrides_ctypes_find_library
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_image_sixel_encoder_uses_libsixel_when_available
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_sixel_unavailable_excepthook_prints_actionable_error_without_traceback
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_transports.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_native_cli.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_style.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_text_editor.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_uiinstance.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_uiwindow.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_version_consistency.py
```

## C++ Unit Tests

POSIX:

```bash
cmake --build cpp/build --target ui_cpp_tests && cpp/build/ui_cpp_tests
ctest --test-dir cpp/build --output-on-failure
```

Windows PowerShell:

```powershell
cmake --build cpp\build-windows --target ui_cpp_tests --config Release
.\cpp\build-windows\Release\ui_cpp_tests.exe
ctest --test-dir cpp\build-windows -C Release --output-on-failure
```

## Native CLI Smoke Tests

```bash
cmake --build cpp/build --target uimd
cmake --build cpp/build --target uimd_init
cpp/build/tools/uimd/uimd issue-report python/examples/calculator/calculator.uimd --title "Issue report smoke" --output /tmp/uimd-issue-report.md
python3 tools/native_uimd_parity.py
python3 tools/native_uimd_parity.py --compile-examples
# Includes SDK store, self-update, target auto-install, and uimd-init smoke checks.
```

## Local SDK Release Artifact

```bash
python3 tools/package_sdk_release.py --build --output dist/sdk-release
UIMD_HOME=/tmp/uimd-home \
  cpp/build-release/tools/uimd/uimd sdk install 0.4.0 --release-root dist/sdk-release
UIMD_HOME=/tmp/uimd-home /tmp/uimd-home/bin/uimd doctor --json
UIMD_HOME=/tmp/uimd-home-from-script \
UIMD_RELEASE_BASE_URL=file://$PWD/dist/sdk-release \
  sh dist/sdk-release/install.sh --no-shell-config --json
UIMD_HOME=/tmp/uimd-home-from-script /tmp/uimd-home-from-script/bin/uimd doctor --json
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

Recommended cross-platform helper commands:

POSIX:

```bash
./tools/mcp_cpp_example.sh activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./tools/mcp_cpp_example.sh widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Windows over SSH / cmd.exe:

```bat
.\tools\mcp_cpp_example.cmd activity_feed tests\mcp\activity_feed.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd calculator tests\mcp\calculator.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd cells tests\mcp\cells.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd contacts_manager tests\mcp\contacts_manager.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd expense_tracker tests\mcp\expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd formular tests\mcp\formular.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd image_browser tests\mcp\image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd image_gallery tests\mcp\image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd markdown_viewer tests\mcp\markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd special_elements tests\mcp\special_elements.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd task_board tests\mcp\task_board_compare.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd text_editor tests\mcp\text_editor.yaml --mcp-fast --compare-app-size 90x35
.\tools\mcp_cpp_example.cmd widget_gallery tests\mcp\widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Windows PowerShell:

```powershell
.\tools\mcp_cpp_example.ps1 activity_feed tests\mcp\activity_feed.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 calculator tests\mcp\calculator.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 cells tests\mcp\cells.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 contacts_manager tests\mcp\contacts_manager.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 expense_tracker tests\mcp\expense_tracker_compare.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 formular tests\mcp\formular.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 image_browser tests\mcp\image_browser_compare.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 image_gallery tests\mcp\image_gallery_compare.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 markdown_viewer tests\mcp\markdown_viewer.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 special_elements tests\mcp\special_elements.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 task_board tests\mcp\task_board_compare.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 text_editor tests\mcp\text_editor.yaml -McpFast -CompareAppSize 90x35
.\tools\mcp_cpp_example.ps1 widget_gallery tests\mcp\widget_gallery.yaml -McpFast -CompareAppSize 90x35
```

Raw POSIX form:

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

Raw Windows PowerShell form for one example:

```powershell
.\uimd.ps1 mcp-test cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --mcp-fast --compare-app-size 90x35
```

## C# App MCP Tests

Recommended cross-platform helper commands:

POSIX:

```bash
python3 tools/uimd_dev.py mcp-csharp-example activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Raw POSIX form:

```bash
./uimd mcp-test --backend python --headless csharp/examples tests/mcp/all_examples.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/activity_feed/bin/Debug/net10.0/activity_feed.dll tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/calculator/bin/Debug/net10.0/calculator.dll tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/cells/bin/Debug/net10.0/cells.dll tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/contacts_manager/bin/Debug/net10.0/contacts_manager.dll tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/expense_tracker/bin/Debug/net10.0/expense_tracker.dll tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/formular/bin/Debug/net10.0/formular.dll tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/image_browser/bin/Debug/net10.0/image_browser.dll tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/image_gallery/bin/Debug/net10.0/image_gallery.dll tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/markdown_viewer/bin/Debug/net10.0/markdown_viewer.dll tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/special_elements/bin/Debug/net10.0/special_elements.dll tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/task_board/bin/Debug/net10.0/task_board.dll tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/text_editor/bin/Debug/net10.0/text_editor.dll tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/widget_gallery/bin/Debug/net10.0/widget_gallery.dll tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

## Compare MCP Tests

Recommended cross-platform helper commands:

POSIX:

```bash
./tools/mcp_compare_example.sh activity_feed tests/mcp/activity_feed.yaml --mcp-fast
./tools/mcp_compare_example.sh calculator tests/mcp/calculator.yaml --mcp-fast
./tools/mcp_compare_example.sh cells tests/mcp/cells.yaml --mcp-fast
./tools/mcp_compare_example.sh contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast
./tools/mcp_compare_example.sh expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast
./tools/mcp_compare_example.sh formular tests/mcp/formular.yaml --mcp-fast
./tools/mcp_compare_example.sh image_browser tests/mcp/image_browser_compare.yaml --mcp-fast
./tools/mcp_compare_example.sh image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast
./tools/mcp_compare_example.sh markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast
./tools/mcp_compare_example.sh special_elements tests/mcp/special_elements.yaml --mcp-fast
./tools/mcp_compare_example.sh task_board tests/mcp/task_board_compare.yaml --mcp-fast
./tools/mcp_compare_example.sh text_editor tests/mcp/text_editor.yaml --mcp-fast
./tools/mcp_compare_example.sh widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast
```

Windows over SSH / cmd.exe:

```bat
.\tools\mcp_compare_example.cmd activity_feed tests\mcp\activity_feed.yaml --mcp-fast
.\tools\mcp_compare_example.cmd calculator tests\mcp\calculator.yaml --mcp-fast
.\tools\mcp_compare_example.cmd cells tests\mcp\cells.yaml --mcp-fast
.\tools\mcp_compare_example.cmd contacts_manager tests\mcp\contacts_manager.yaml --mcp-fast
.\tools\mcp_compare_example.cmd expense_tracker tests\mcp\expense_tracker_compare.yaml --mcp-fast
.\tools\mcp_compare_example.cmd formular tests\mcp\formular.yaml --mcp-fast
.\tools\mcp_compare_example.cmd image_browser tests\mcp\image_browser_compare.yaml --mcp-fast
.\tools\mcp_compare_example.cmd image_gallery tests\mcp\image_gallery_compare.yaml --mcp-fast
.\tools\mcp_compare_example.cmd markdown_viewer tests\mcp\markdown_viewer.yaml --mcp-fast
.\tools\mcp_compare_example.cmd special_elements tests\mcp\special_elements.yaml --mcp-fast
.\tools\mcp_compare_example.cmd task_board tests\mcp\task_board_compare.yaml --mcp-fast
.\tools\mcp_compare_example.cmd text_editor tests\mcp\text_editor.yaml --mcp-fast
.\tools\mcp_compare_example.cmd widget_gallery tests\mcp\widget_gallery.yaml --mcp-fast
```

Windows PowerShell:

```powershell
.\tools\mcp_compare_example.ps1 activity_feed tests\mcp\activity_feed.yaml -McpFast
.\tools\mcp_compare_example.ps1 calculator tests\mcp\calculator.yaml -McpFast
.\tools\mcp_compare_example.ps1 cells tests\mcp\cells.yaml -McpFast
.\tools\mcp_compare_example.ps1 contacts_manager tests\mcp\contacts_manager.yaml -McpFast
.\tools\mcp_compare_example.ps1 expense_tracker tests\mcp\expense_tracker_compare.yaml -McpFast
.\tools\mcp_compare_example.ps1 formular tests\mcp\formular.yaml -McpFast
.\tools\mcp_compare_example.ps1 image_browser tests\mcp\image_browser_compare.yaml -McpFast
.\tools\mcp_compare_example.ps1 image_gallery tests\mcp\image_gallery_compare.yaml -McpFast
.\tools\mcp_compare_example.ps1 markdown_viewer tests\mcp\markdown_viewer.yaml -McpFast
.\tools\mcp_compare_example.ps1 special_elements tests\mcp\special_elements.yaml -McpFast
.\tools\mcp_compare_example.ps1 task_board tests\mcp\task_board_compare.yaml -McpFast
.\tools\mcp_compare_example.ps1 text_editor tests\mcp\text_editor.yaml -McpFast
.\tools\mcp_compare_example.ps1 widget_gallery tests\mcp\widget_gallery.yaml -McpFast
```

Raw POSIX form:

```bash
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-action-delay-ms 1 --mcp-type-delay-ms 1 --step-delay-ms 1 --compare-app-size 90x35
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --all --compare python/examples cpp/build/examples --compare-app-size 90x35
./uimd mcp-test --headless --compare python/examples csharp/examples tests/mcp/all_examples.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --all --compare cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python/stale_scrollview_focus/stale_scrollview_focus.py cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
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

Raw Windows PowerShell form:

```powershell
.\uimd.ps1 mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare python\examples csharp\examples tests\mcp\all_examples.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python\stale_scrollview_focus\stale_scrollview_focus.py cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --compare python\examples\activity_feed\activity_feed.py cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --compare-app-size 90x35 --mcp-fast
```

Raw Windows cmd.exe form:

```bat
.\uimd.cmd mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare python\examples csharp\examples tests\mcp\all_examples.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python\stale_scrollview_focus\stale_scrollview_focus.py cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --compare python\examples\activity_feed\activity_feed.py cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --compare-app-size 90x35 --mcp-fast
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

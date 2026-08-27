# Example CLI Commands

Run commands from the repository root.

These commands assume a source checkout where UIMD is not installed yet. POSIX
shells use the checkout entry point as `./uimd`; Windows PowerShell uses
`.\uimd.ps1`, and Windows `cmd.exe` uses `.\uimd.cmd`; both locate the
repo-local native `uimd.exe`. Legacy Python compiler wrappers have been removed;
use the native UIMD launcher for all generation and run commands.

C++ build outputs are local artifacts under `cpp/build` or `cpp/build-windows`.
C# build outputs are local artifacts under `csharp/examples/*/bin`. They are
not committed to Git. Java application distributions are local artifacts under
`java/examples/*/build/install` and are also not committed.

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

On POSIX, the helper runs the full local gate: regenerate/build all supported
sources including reported-bug regression corpora, build every compiled
example in the canonical `parity` profile (Release for C++, C#, Swift, and
Rust; Java 17 through the checked-in Gradle wrapper; and the standard
reproducible Go build), compile Python sources, run Python unit tests, run C++
`ctest`, run C#, Go, Java, and Rust runtime tests plus Java Checkstyle and Rust
Clippy, compile and exercise frozen previous-version applications against the
current runtime for every enabled language, run Swift runtime tests on POSIX,
run direct Swift, Go, Rust, and Java
terminal PTY smoke tests against C++, run the Rust and Java MCP transport
smokes, run Python/C++, C++/C#, C++/Swift, C++/Go, C++/Rust, and C++/Java MCP
example compare tests with
`--compare-app-size 90x35`, and run the UIMD regression parity compare corpus
for Python/C++, C++/Go, C++/Rust, and C++/Java when
`tests/regressions/uimd/parity` exists. Pass
`--no-swift` to the POSIX helper only when the local Swift toolchain is
intentionally unavailable, `--no-rust` only when the local Rust toolchain is
intentionally unavailable, or `--no-java` only when Java 17 is intentionally
unavailable.

Every successful rebuild writes `.uimd/build-manifest.json` with the exact
artifact paths plus SHA-256 hashes of the artifacts and all build inputs. The
MCP commands and direct-terminal/transport smoke tests below accept only those
recorded artifacts; missing, changed, or source-stale builds fail with an
instruction to rerun `./tools/rebuild_all.sh` instead of falling back to a
different Debug/Release output.

The current Windows wrappers predate Go automation. Their sections below list
the additional explicit Go generation, build, unit/static, and headless MCP
commands required for equivalent coverage. Native Go direct-terminal validation
is POSIX-only until the separately tracked Windows console/ConPTY adapter exists.

macOS/Linux (POSIX shell):

```bash
./tools/test_all.sh
```

For a concise live terminal report plus one complete log with fail-fast
execution, use:

```bash
./tools/test_all.sh --live-report
```

To continue with independent test phases after a test failure and collect all
failures in the same final recap, use:

```bash
./tools/test_all.sh --live-report --keep-going
```

`--live-report` writes every command and every unabridged stdout/stderr line to
a new `.uimd/test-logs/test-all-<timestamp>.log` file as the processes run. The
terminal shows one `passed/total` line when each phase completes and immediately
shows one single-line diagnostic for every failed pytest test, CTest test, Go/
Rust/Swift test, smoke check, or MCP script/example step. `--keep-going`
continues with independent test phases after a test failure so the final report
contains the entire gate; prerequisite configure/generate/build/manifest
failures remain fail-fast because later results would be invalid. Omit
`--keep-going` to retain fail-fast execution, or select a new explicit log path
with `--log-file <path>`. Without `--live-report`, the existing full terminal
output and behavior are unchanged. At the end, live reporting repeats one
concise line for every phase, repeats every failed test/assertion/step with its
one-line reason, prints the total passed/failed/skipped phase count, and points
to the complete log. If an MCP all-script command aborts before its final
`RESULT`, its phase line uses the completed per-script `SUMMARY` counts and the
active script's distinct abort reason is retained alongside any earlier
assertion or snapshot failures. Such summary-derived MCP counts are marked
`partial`. An aborted direct-terminal or transport smoke similarly reports its
observed `PASS` checks plus the failed attempt instead of falling back to a
generic `0/1 gate` whenever individual check output is available. On macOS the
POSIX wrapper also exposes an installed Homebrew `libsixel` library to Python
children when neither `UIMD_LIBSIXEL_PATH` nor `UIMD_LIBSIXEL_DIR` was supplied.

Equivalent explicit command sequence:

```bash
./tools/rebuild_all.sh
python3 tools/previous_version_compatibility.py
python3 -m pytest python/tests
ctest --test-dir cpp/build --output-on-failure
dotnet run --project csharp/tests/UimdRuntimeTests/UimdRuntimeTests.csproj --configuration Release
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test ./...
./java/gradlew -p java check
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml
python3 tools/cargo_with_progress.py clippy --manifest-path rust/src/uimd/Cargo.toml --all-targets -- -D warnings
swift test --package-path swift/src/Uimd
python3 tools/swift_direct_terminal_smoke.py --cpp-build-dir cpp/build
python3 tools/go_direct_terminal_smoke.py --cpp-build-dir cpp/build --go-examples-dir go/examples
python3 tools/rust_direct_terminal_smoke.py --cpp-build-dir cpp/build --rust-examples-dir rust/examples
python3 tools/rust_mcp_transport_smoke.py
python3 tools/java_direct_terminal_smoke.py --cpp-build-dir cpp/build --java-examples-dir java/examples
python3 tools/java_mcp_transport_smoke.py
./uimd mcp-test --all --compare python/examples cpp/build/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples csharp/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples go/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples java/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples rust/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll go/regressions/uimd/parity/source_separator_scroll/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus go/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll rust/regressions/uimd/parity/source_separator_scroll/target/release/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus rust/regressions/uimd/parity/stale_scrollview_focus/target/release/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll java/regressions/uimd/parity/source_separator_scroll/build/install/source_separator_scroll/bin/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus java/regressions/uimd/parity/stale_scrollview_focus/build/install/stale_scrollview_focus/bin/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

Windows over SSH / cmd.exe:

```bat
.\tools\test_all.cmd
.\tools\test_all.cmd --live-report
.\tools\test_all.cmd --live-report --keep-going
```

The Windows wrapper does not yet automate the Go-specific build, runtime, or
headless MCP gates. Use this complete explicit sequence to include them; native
Go direct-terminal validation remains POSIX-only. Rust generation is available
on Windows, but the Rust direct-terminal/runtime validation documented in this
file is POSIX-only until a Windows console/ConPTY adapter is implemented and
validated.

```bat
.\tools\rebuild_all.cmd -Test
.\uimd.cmd generate go\examples --target go
.\uimd.cmd generate go\regressions\uimd\parity --target go
powershell -NoProfile -Command "$env:GOCACHE=Join-Path $env:TEMP 'uimd-go-build-cache'; Get-ChildItem go\examples -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName ($_.Name + '.go'))) { Push-Location $_.FullName; go build -o ($_.Name + '.exe') .; Pop-Location } }; Get-ChildItem go\regressions\uimd\parity -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName ($_.Name + '.go'))) { Push-Location $_.FullName; go build -o ($_.Name + '.exe') .; Pop-Location } }"
go -C go\src\uimd test ./...
go -C go\src\uimd vet ./...
python -m pytest python\tests
.\uimd.cmd mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

Windows PowerShell only:

```powershell
.\tools\test_all.ps1
.\tools\test_all.ps1 -LiveReport
.\tools\test_all.ps1 -LiveReport -KeepGoing
```

The PowerShell wrapper has the same current Go automation boundary. Use this
complete explicit sequence for Windows generation, build, unit/static, and
headless MCP coverage:

```powershell
.\tools\rebuild_all.ps1 -Test
.\uimd.ps1 generate go\examples --target go
.\uimd.ps1 generate go\regressions\uimd\parity --target go
$env:GOCACHE = Join-Path $env:TEMP "uimd-go-build-cache"
Get-ChildItem go\examples -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "$($_.Name).go")) { Push-Location $_.FullName; go build -o "$($_.Name).exe" .; Pop-Location } }
Get-ChildItem go\regressions\uimd\parity -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "$($_.Name).go")) { Push-Location $_.FullName; go build -o "$($_.Name).exe" .; Pop-Location } }
go -C go\src\uimd test ./...
go -C go\src\uimd vet ./...
python -m pytest python\tests
.\uimd.ps1 mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
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
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/activity_feed/activity_feed.csproj --configuration Release && dotnet csharp/examples/activity_feed/bin/Release/net10.0/activity_feed.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/calculator/calculator.csproj --configuration Release && dotnet csharp/examples/calculator/bin/Release/net10.0/calculator.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/cells/cells.csproj --configuration Release && dotnet csharp/examples/cells/bin/Release/net10.0/cells.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/contacts_manager/contacts_manager.csproj --configuration Release && dotnet csharp/examples/contacts_manager/bin/Release/net10.0/contacts_manager.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/expense_tracker/expense_tracker.csproj --configuration Release && dotnet csharp/examples/expense_tracker/bin/Release/net10.0/expense_tracker.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/formular/formular.csproj --configuration Release && dotnet csharp/examples/formular/bin/Release/net10.0/formular.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/image_browser/image_browser.csproj --configuration Release && dotnet csharp/examples/image_browser/bin/Release/net10.0/image_browser.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/image_gallery/image_gallery.csproj --configuration Release && dotnet csharp/examples/image_gallery/bin/Release/net10.0/image_gallery.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/markdown_viewer/markdown_viewer.csproj --configuration Release && dotnet csharp/examples/markdown_viewer/bin/Release/net10.0/markdown_viewer.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/special_elements/special_elements.csproj --configuration Release && dotnet csharp/examples/special_elements/bin/Release/net10.0/special_elements.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/task_board/task_board.csproj --configuration Release && dotnet csharp/examples/task_board/bin/Release/net10.0/task_board.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/text_editor/text_editor.csproj --configuration Release && dotnet csharp/examples/text_editor/bin/Release/net10.0/text_editor.dll
./uimd generate csharp/examples --target csharp && dotnet build csharp/examples/widget_gallery/widget_gallery.csproj --configuration Release && dotnet csharp/examples/widget_gallery/bin/Release/net10.0/widget_gallery.dll
```

Raw Windows PowerShell form for one example:

```powershell
.\uimd.ps1 generate csharp\examples --target csharp
dotnet build csharp\examples\activity_feed\activity_feed.csproj --configuration Release
dotnet csharp\examples\activity_feed\bin\Release\net10.0\activity_feed.dll
```

## Go Examples

The Go direct-terminal runtime is currently implemented and validated on
macOS/Linux POSIX terminals. Every example below regenerates its generated Go
source before running.

Raw macOS/Linux POSIX shell form. Each command builds through `go -C`, then
runs the binary from the repository working directory. The calling shell's
current directory and the application's working directory therefore stay at
the directory from which the command was invoked:

```bash
./uimd generate go/examples/activity_feed --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/activity_feed build -o activity_feed . && ./go/examples/activity_feed/activity_feed
./uimd generate go/examples/calculator --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/calculator build -o calculator . && ./go/examples/calculator/calculator
./uimd generate go/examples/cells --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/cells build -o cells . && ./go/examples/cells/cells
./uimd generate go/examples/contacts_manager --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/contacts_manager build -o contacts_manager . && ./go/examples/contacts_manager/contacts_manager
./uimd generate go/examples/expense_tracker --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/expense_tracker build -o expense_tracker . && ./go/examples/expense_tracker/expense_tracker
./uimd generate go/examples/formular --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/formular build -o formular . && ./go/examples/formular/formular
./uimd generate go/examples/hello --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/hello build -o hello . && ./go/examples/hello/hello
./uimd generate go/examples/image_browser --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/image_browser build -o image_browser . && ./go/examples/image_browser/image_browser
./uimd generate go/examples/image_gallery --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/image_gallery build -o image_gallery . && ./go/examples/image_gallery/image_gallery
./uimd generate go/examples/markdown_viewer --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/markdown_viewer build -o markdown_viewer . && ./go/examples/markdown_viewer/markdown_viewer
./uimd generate go/examples/special_elements --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/special_elements build -o special_elements . && ./go/examples/special_elements/special_elements
./uimd generate go/examples/task_board --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/task_board build -o task_board . && ./go/examples/task_board/task_board
./uimd generate go/examples/text_editor --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/text_editor build -o text_editor . && ./go/examples/text_editor/text_editor
./uimd generate go/examples/widget_gallery --target go && GOCACHE=/tmp/uimd-go-cache go -C go/examples/widget_gallery build -o widget_gallery . && ./go/examples/widget_gallery/widget_gallery
```

Windows PowerShell generation and build form:

```powershell
.\uimd.ps1 generate go\examples --target go
$env:GOCACHE = Join-Path $env:TEMP "uimd-go-build-cache"
Get-ChildItem go\examples -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "$($_.Name).go")) { Push-Location $_.FullName; go build -o "$($_.Name).exe" .; Pop-Location } }
```

This Windows form validates generated source and compilation only. Do not use
it as an interactive direct-terminal run command until the native Go Windows
console/ConPTY adapter replaces the current POSIX `stty` terminal setup.

## Java Examples

Java uses the checked-in Gradle wrapper and an automatically discovered Java
17 JDK. The wrapper and generated distribution launcher share the SDK resolver,
so a normal JDK installation needs no per-shell `JAVA_HOME` export. Each
command regenerates from the canonical Python `.uimd` source, builds an
application distribution, and runs its launcher from the repository root.

```bash
./uimd generate python/examples/activity_feed --target java --output-dir java/examples/activity_feed && ./java/gradlew -p java/examples/activity_feed installDist --console=plain && ./java/examples/activity_feed/build/install/activity_feed/bin/activity_feed
./uimd generate python/examples/calculator --target java --output-dir java/examples/calculator && ./java/gradlew -p java/examples/calculator installDist --console=plain && ./java/examples/calculator/build/install/calculator/bin/calculator
./uimd generate python/examples/cells --target java --output-dir java/examples/cells && ./java/gradlew -p java/examples/cells installDist --console=plain && ./java/examples/cells/build/install/cells/bin/cells
./uimd generate python/examples/contacts_manager --target java --output-dir java/examples/contacts_manager && ./java/gradlew -p java/examples/contacts_manager installDist --console=plain && ./java/examples/contacts_manager/build/install/contacts_manager/bin/contacts_manager
./uimd generate python/examples/expense_tracker --target java --output-dir java/examples/expense_tracker && ./java/gradlew -p java/examples/expense_tracker installDist --console=plain && ./java/examples/expense_tracker/build/install/expense_tracker/bin/expense_tracker
./uimd generate python/examples/formular --target java --output-dir java/examples/formular && ./java/gradlew -p java/examples/formular installDist --console=plain && ./java/examples/formular/build/install/formular/bin/formular
./uimd generate python/examples/image_browser --target java --output-dir java/examples/image_browser && ./java/gradlew -p java/examples/image_browser installDist --console=plain && ./java/examples/image_browser/build/install/image_browser/bin/image_browser
./uimd generate python/examples/image_gallery --target java --output-dir java/examples/image_gallery && ./java/gradlew -p java/examples/image_gallery installDist --console=plain && ./java/examples/image_gallery/build/install/image_gallery/bin/image_gallery
./uimd generate python/examples/markdown_viewer --target java --output-dir java/examples/markdown_viewer && ./java/gradlew -p java/examples/markdown_viewer installDist --console=plain && ./java/examples/markdown_viewer/build/install/markdown_viewer/bin/markdown_viewer
./uimd generate python/examples/special_elements --target java --output-dir java/examples/special_elements && ./java/gradlew -p java/examples/special_elements installDist --console=plain && ./java/examples/special_elements/build/install/special_elements/bin/special_elements
./uimd generate python/examples/task_board --target java --output-dir java/examples/task_board && ./java/gradlew -p java/examples/task_board installDist --console=plain && ./java/examples/task_board/build/install/task_board/bin/task_board
./uimd generate python/examples/text_editor --target java --output-dir java/examples/text_editor && ./java/gradlew -p java/examples/text_editor installDist --console=plain && ./java/examples/text_editor/build/install/text_editor/bin/text_editor
./uimd generate python/examples/widget_gallery --target java --output-dir java/examples/widget_gallery && ./java/gradlew -p java/examples/widget_gallery installDist --console=plain && ./java/examples/widget_gallery/build/install/widget_gallery/bin/widget_gallery
```

Windows PowerShell generation and build form:

```powershell
.\uimd.ps1 generate python\examples --target java --output-dir java\examples
.\java\gradlew.bat -p java check
Get-ChildItem java\examples -Directory | ForEach-Object { .\java\gradlew.bat -p $_.FullName clean installDist --console=plain }
```

## Rust Toolchain Prerequisite

Rust commands require the official Rust toolchain. `cargo` is Rust's build,
test, and package-management program; it is not bundled with UIMD. Check the
toolchain before running any Rust command:

```bash
command -v cargo
cargo --version
rustc --version
```

On macOS/Linux, install the stable toolchain with the official `rustup`
installer when `cargo` is missing:

```bash
curl https://sh.rustup.rs -sSf | sh
source "$HOME/.cargo/env"
rustup default stable
rustup component add clippy
cargo --version
```

Open a new terminal instead of running `source` if preferred. The normal
installation places `cargo`, `rustc`, and `rustup` under `$HOME/.cargo/bin` and
adds that directory to `PATH`. Repository commands must not depend on an
agent-created toolchain under `/private/tmp`; such isolated toolchains are
temporary and require their own explicit `CARGO_HOME`, `RUSTUP_HOME`, and
executable path.

All repository Rust commands below use `tools/cargo_with_progress.py`. The
launcher resolves Cargo from `CARGO`, `PATH`, or `$HOME/.cargo/bin`, streams
Cargo output immediately, and prints a timestamped heartbeat after every ten
seconds without output. Therefore the documented commands work in an already
open shell immediately after installation, even before that shell reloads its
`PATH`. Use `--heartbeat-seconds N` before the Cargo subcommand to select a
different interval. For `run`, the launcher first builds while monitoring Cargo,
then starts the reported executable directly with the original terminal
attached. Interactive Rust applications therefore keep keyboard, mouse, raw
mode, and terminal teardown behavior instead of receiving a piped output stream.

```bash
python3 tools/cargo_with_progress.py --version
```

## Rust Examples

The Rust direct-terminal runtime is implemented and validated on macOS/Linux
POSIX terminals. Every command regenerates from the shared `.uimd` sources,
builds a release binary, and runs it from the repository working directory.

```bash
./uimd generate rust/examples/activity_feed --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/activity_feed/Cargo.toml
./uimd generate rust/examples/calculator --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/calculator/Cargo.toml
./uimd generate rust/examples/cells --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/cells/Cargo.toml
./uimd generate rust/examples/contacts_manager --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/contacts_manager/Cargo.toml
./uimd generate rust/examples/expense_tracker --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/expense_tracker/Cargo.toml
./uimd generate rust/examples/formular --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/formular/Cargo.toml
./uimd generate rust/examples/image_browser --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/image_browser/Cargo.toml
./uimd generate rust/examples/image_gallery --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/image_gallery/Cargo.toml
./uimd generate rust/examples/markdown_viewer --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/markdown_viewer/Cargo.toml
./uimd generate rust/examples/special_elements --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/special_elements/Cargo.toml
./uimd generate rust/examples/task_board --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/task_board/Cargo.toml
./uimd generate rust/examples/text_editor --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/text_editor/Cargo.toml
./uimd generate rust/examples/widget_gallery --target rust && python3 tools/cargo_with_progress.py run --release --manifest-path rust/examples/widget_gallery/Cargo.toml
```

## Swift Examples

macOS SwiftPM:

```bash
./uimd generate swift/examples/activity_feed --target swift && swift build -c release --package-path swift/examples/activity_feed && swift run -c release --package-path swift/examples/activity_feed activity_feed
./uimd generate swift/examples/calculator --target swift && swift build -c release --package-path swift/examples/calculator && swift run -c release --package-path swift/examples/calculator calculator
./uimd generate swift/examples/cells --target swift && swift build -c release --package-path swift/examples/cells && swift run -c release --package-path swift/examples/cells cells
./uimd generate swift/examples/contacts_manager --target swift && swift build -c release --package-path swift/examples/contacts_manager && swift run -c release --package-path swift/examples/contacts_manager contacts_manager
./uimd generate swift/examples/expense_tracker --target swift && swift build -c release --package-path swift/examples/expense_tracker && swift run -c release --package-path swift/examples/expense_tracker expense_tracker
./uimd generate swift/examples/formular --target swift && swift build -c release --package-path swift/examples/formular && swift run -c release --package-path swift/examples/formular formular
./uimd generate swift/examples/image_browser --target swift && swift build -c release --package-path swift/examples/image_browser && swift run -c release --package-path swift/examples/image_browser image_browser
./uimd generate swift/examples/image_gallery --target swift && swift build -c release --package-path swift/examples/image_gallery && swift run -c release --package-path swift/examples/image_gallery image_gallery
./uimd generate swift/examples/markdown_viewer --target swift && swift build -c release --package-path swift/examples/markdown_viewer && swift run -c release --package-path swift/examples/markdown_viewer markdown_viewer
./uimd generate swift/examples/special_elements --target swift && swift build -c release --package-path swift/examples/special_elements && swift run -c release --package-path swift/examples/special_elements special_elements
./uimd generate swift/examples/task_board --target swift && swift build -c release --package-path swift/examples/task_board && swift run -c release --package-path swift/examples/task_board task_board
./uimd generate swift/examples/text_editor --target swift && swift build -c release --package-path swift/examples/text_editor && swift run -c release --package-path swift/examples/text_editor text_editor
./uimd generate swift/examples/widget_gallery --target swift && swift build -c release --package-path swift/examples/widget_gallery && swift run -c release --package-path swift/examples/widget_gallery widget_gallery
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
./uimd generate go/examples --target go
./uimd generate go/regressions/uimd/parity --target go
./uimd generate python/examples --target java --output-dir java/examples
./uimd generate src/uimd/dialogs --target java --output-dir java/src/main/java/uimd --java-package uimd
./uimd generate tests/regressions/uimd/parity/python --target java --output-dir java/regressions/uimd/parity
./uimd generate rust/examples --target rust
./uimd generate rust/regressions/uimd/parity --target rust
./uimd generate swift/examples --target swift
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
./uimd generate go/examples --target go
./uimd generate go/regressions/uimd/parity --target go
./uimd generate python/examples --target java --output-dir java/examples
./uimd generate src/uimd/dialogs --target java --output-dir java/src/main/java/uimd --java-package uimd
./uimd generate tests/regressions/uimd/parity/python --target java --output-dir java/regressions/uimd/parity
./uimd generate rust/examples --target rust
./uimd generate rust/regressions/uimd/parity --target rust
./uimd generate swift/examples --target swift
./uimd generate tests/regressions/uimd/parity/python --target python
./uimd generate tests/regressions/uimd/parity/cpp --target cpp
cmake -S cpp -B cpp/build
cmake --build cpp/build
for proj in csharp/examples/*/*.csproj; do dotnet build "$proj" --configuration Release; done
for dir in go/examples/*; do if [ -f "$dir/$(basename "$dir").go" ]; then (cd "$dir" && GOCACHE=/tmp/uimd-go-cache go build -o "$(basename "$dir")" .); fi; done
for dir in go/regressions/uimd/parity/*; do if [ -f "$dir/$(basename "$dir").go" ]; then (cd "$dir" && GOCACHE=/tmp/uimd-go-cache go build -o "$(basename "$dir")" .); fi; done
./java/gradlew -p java assemble --console=plain
for dir in java/examples/* java/regressions/uimd/parity/*; do if [ -f "$dir/build.gradle" ]; then ./java/gradlew -p "$dir" clean installDist --console=plain; fi; done
for manifest in rust/examples/*/Cargo.toml rust/regressions/uimd/parity/*/Cargo.toml; do python3 tools/cargo_with_progress.py build --release --manifest-path "$manifest"; done
for package in swift/examples/*/Package.swift; do swift build -c release --package-path "$(dirname "$package")"; done
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
.\uimd.ps1 generate go\examples --target go
.\uimd.ps1 generate go\regressions\uimd\parity --target go
.\uimd.ps1 generate python\examples --target java --output-dir java\examples
.\uimd.ps1 generate src\uimd\dialogs --target java --output-dir java\src\main\java\uimd --java-package uimd
.\uimd.ps1 generate tests\regressions\uimd\parity\python --target java --output-dir java\regressions\uimd\parity
.\uimd.ps1 generate tests\regressions\uimd\parity\python --target python
.\uimd.ps1 generate tests\regressions\uimd\parity\cpp --target cpp
cmake -S cpp -B cpp\build-windows -G "Visual Studio 17 2022" -A x64
cmake --build cpp\build-windows --config Release
Get-ChildItem csharp\examples -Filter *.csproj -Recurse | ForEach-Object { dotnet build $_.FullName --configuration Release }
Get-ChildItem go\examples -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "$($_.Name).go")) { Push-Location $_.FullName; go build -o "$($_.Name).exe" .; Pop-Location } }
Get-ChildItem go\regressions\uimd\parity -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "$($_.Name).go")) { Push-Location $_.FullName; go build -o "$($_.Name).exe" .; Pop-Location } }
.\java\gradlew.bat -p java assemble --console=plain
Get-ChildItem java\examples, java\regressions\uimd\parity -Directory | ForEach-Object { if (Test-Path (Join-Path $_.FullName "build.gradle")) { .\java\gradlew.bat -p $_.FullName clean installDist --console=plain } }
python -m compileall python src tests tools
```

## MCP Tester UI

```bash
./uimd generate src/uimd/testing --target python
cmake --build cpp/build --target uimd_mcp_tester
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp.py python/tests/test_mcp_tester.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py -k python_and_cpp_tester_backends_have_small_script_parity
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py -k target_request_failure_diagnostics_include_exit_code_and_bounded_stderr
PYTHONPATH=python:src python3 -m pytest python/tests/test_mcp_tester.py -k parity_manifest_ignores_gradle_build_outputs
```

`./uimd mcp-test` defaults to the C++ tester. Use `--backend python` only when
explicitly running the Python tester implementation.

## Python Unit Tests

```bash
PYTHONPATH=python:src python3 -m pytest python/tests
PYTHONPATH=python:src python3 -m pytest python/tests/test_activity_feed.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_application.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_application.py::TestUIApplicationSizing::test_scrollview_scoped_confirm_keeps_same_focus_after_callback_changes_focusables
PYTHONPATH=python:src python3 -m pytest python/tests/test_application.py::TestUIApplicationSizing::test_scrollview_scoped_reusable_control_activates_with_enter_and_space
PYTHONPATH=python:src python3 -m pytest python/tests/test_calculator.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_dialogs.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_configured_sixel_library_lookup_honors_directory_override
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_configured_sixel_library_lookup_overrides_ctypes_find_library
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_image_sixel_encoder_uses_libsixel_when_available
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_image_sixel_mode_falls_back_for_apple_terminal
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_image_sixel_visible_row_resampling_stays_in_native_pillow
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_sixel_unavailable_excepthook_prints_actionable_error_without_traceback
PYTHONPATH=python:src python3 -m pytest python/tests/test_elements.py::TestImage::test_xterm_term_name_does_not_imply_sixel_support
PYTHONPATH=python:src python3 -m pytest python/tests/test_example_resource_parity.py
PYTHONPATH=python:src python3 -m pytest python/tests/test_full_test_report.py
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

## C# Runtime Tests

POSIX:

```bash
dotnet run --project csharp/tests/UimdRuntimeTests/UimdRuntimeTests.csproj --configuration Release
```

Windows PowerShell:

```powershell
dotnet run --project csharp\tests\UimdRuntimeTests\UimdRuntimeTests.csproj --configuration Release
```

## Java Runtime Tests And Static Checks

```bash
./java/gradlew -p java check
python3 -m pytest python/tests/test_java_toolchain.py
./java/gradlew -p java test --tests uimd.GeneratedDialogsTest
./java/gradlew -p java test --tests uimd.GeneratedWindowFocusTest
./java/gradlew -p java test --tests uimd.GeneratedWindowFocusTest.keyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements
./java/gradlew -p java test --tests uimd.GeneratedWindowFocusTest.scopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation
./java/gradlew -p java test --tests uimd.GeneratedWindowFocusTest.scrollViewScopedReusableControlActivatesWithEnterAndSpace
./java/gradlew -p java test --tests uimd.GeneratedWindowStackTest
./java/gradlew -p java test --tests uimd.ImageTest
UIMD_FORCE_SIXEL=1 ./java/gradlew -p java test --tests uimd.ImageTest.sixelModeWritesOneRawAnchorPerVisibleCellRow
./java/gradlew -p java test --tests uimd.McpControllerTest
./java/gradlew -p java test --tests uimd.terminal.InputParserTest
```

Windows PowerShell:

```powershell
.\java\gradlew.bat -p java check
.\java\gradlew.bat -p java test --tests uimd.GeneratedDialogsTest
.\java\gradlew.bat -p java test --tests uimd.GeneratedWindowFocusTest
.\java\gradlew.bat -p java test --tests uimd.GeneratedWindowFocusTest.keyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements
.\java\gradlew.bat -p java test --tests uimd.GeneratedWindowFocusTest.scopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation
.\java\gradlew.bat -p java test --tests uimd.GeneratedWindowFocusTest.scrollViewScopedReusableControlActivatesWithEnterAndSpace
.\java\gradlew.bat -p java test --tests uimd.GeneratedWindowStackTest
.\java\gradlew.bat -p java test --tests uimd.ImageTest
$env:UIMD_FORCE_SIXEL = "1"; .\java\gradlew.bat -p java test --tests uimd.ImageTest.sixelModeWritesOneRawAnchorPerVisibleCellRow
.\java\gradlew.bat -p java test --tests uimd.McpControllerTest
.\java\gradlew.bat -p java test --tests uimd.terminal.InputParserTest
```

## Swift Runtime Tests

```bash
swift test --package-path swift/src/Uimd
```

## Go Runtime Tests And Static Checks

```bash
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test ./...
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test -run '^TestCustomElementDoesNotRequireCommitMode$'
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test -run '^TestDirectTerminalInputReaderFramesSplitAndStandaloneEscape$'
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test -run '^TestKeyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements$'
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test -run '^TestScrollViewScopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation$'
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd test -run '^TestScrollViewScopedReusableControlActivatesWithEnterAndSpace$'
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go -C go/src/uimd vet ./...
```

Windows PowerShell:

```powershell
go -C go\src\uimd test ./...
go -C go\src\uimd test -run '^TestCustomElementDoesNotRequireCommitMode$'
go -C go\src\uimd test -run '^TestDirectTerminalInputReaderFramesSplitAndStandaloneEscape$'
go -C go\src\uimd test -run '^TestKeyboardFocusMovementNotifiesOnlyActualPreviousAndNextElements$'
go -C go\src\uimd test -run '^TestScrollViewScopedConfirmRetainsLiveInputAndRebasesFocusAfterMutation$'
go -C go\src\uimd test -run '^TestScrollViewScopedReusableControlActivatesWithEnterAndSpace$'
go -C go\src\uimd vet ./...
```

## Rust Runtime Tests And Static Checks

```bash
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::activate_button_after_text_tool_dispatches_before_render_like_cpp -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::click_element_refreshes_nested_scroll_row_frames_before_reading_the_centre -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::gui_tcp_server_keeps_accepted_stream_blocking_until_request_arrives -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::headless_http_accepts_a_later_client_while_the_first_request_is_delayed -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::headless_tcp_accepts_a_later_client_while_the_first_request_is_delayed -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml mcp::tests::modal_button_uses_the_app_owned_frame_hook_before_dialog_behavior -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::generated_scroll_view_is_the_single_canonical_element_and_child_owner -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::keyboard_focus_movement_notifies_only_actual_previous_and_next_elements -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::modal_close_reactivates_a_live_invoking_scroll_scope_like_cpp -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::nested_modal_opening_uses_one_flat_root_window_stack -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::nested_scroll_combo_box_overlay_has_no_ancestor_local_ghost -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::scoped_confirm_retains_live_input_and_rebases_focus_after_mutation -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml runtime::tests::scroll_view_scoped_reusable_control_activates_with_enter_and_space -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml terminal::tests::non_tty_input_keeps_the_terminal_lifecycle_available_for_gui_mcp -- --exact
python3 tools/cargo_with_progress.py test --manifest-path rust/src/uimd/Cargo.toml tests::public_runtime_version_is_the_cargo_package_version -- --exact
python3 tools/cargo_with_progress.py clippy --manifest-path rust/src/uimd/Cargo.toml --all-targets -- -D warnings
python3 -m pytest python/tests/test_cargo_with_progress.py
```

The repository uses Allman braces for new brace-language code. `cargo fmt
--check` is therefore not a gate while standard `rustfmt` rewrites those braces
to K&R style; do not apply that rewrite to generated or hand-written Rust UIMD
sources.

## Swift Direct Terminal Smoke Tests

```bash
python3 tools/swift_direct_terminal_smoke.py --cpp-build-dir cpp/build
```

## Go Direct Terminal Smoke Tests

macOS/Linux POSIX terminals only:

```bash
python3 tools/go_direct_terminal_smoke.py --cpp-build-dir cpp/build --go-examples-dir go/examples
```

The smoke covers raw keyboard input, root Escape staying inside the app,
MessageBox Escape exposing its negative-button flash before close, explicit
Quit, SGR mouse press/drag/release, Ctrl+C, title, alternate-screen setup,
full-frame writes, and terminal teardown. A native Windows direct-terminal
smoke remains blocked on the Windows console or ConPTY runtime adapter; Windows
headless MCP commands are documented below.

## Rust Direct Terminal And MCP Transport Smoke Tests

macOS/Linux POSIX terminals:

```bash
python3 tools/rust_direct_terminal_smoke.py --cpp-build-dir cpp/build --rust-examples-dir rust/examples
python3 tools/rust_mcp_transport_smoke.py
```

The direct-terminal smoke covers title and alternate-screen setup, normal and
signal teardown, direct key framing and modifiers, paste, mouse selection and
copy notification, standard-dialog Escape flash, bounded image redraw/Sixel
output, and explicit Quit. The MCP smoke covers stdio, TCP, HTTP,
batch/notification behavior, generated metadata/app-tool schemas, and clear
failure for unsupported transports.

## Java Direct Terminal And MCP Transport Smoke Tests

macOS/Linux POSIX terminals:

```bash
python3 tools/java_direct_terminal_smoke.py --cpp-build-dir cpp/build --java-examples-dir java/examples
python3 tools/java_mcp_transport_smoke.py
```

The direct-terminal gate additionally verifies Java Sixel row geometry,
bounded repeated Sixel scrolling, and the sustained 800-report mouse-wheel
burst against C++. The transport gate covers Java stdio, TCP, HTTP,
interactive MCP/terminal concurrency, repeated modal lifecycle, batches,
notifications, generated app tools, and unsupported-transport diagnostics.

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
./uimd mcp-test python/examples/image_gallery/image_gallery.py tests/mcp/image_gallery_sixel_info_compare.yaml
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
./tools/mcp_cpp_example.sh image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
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
.\tools\mcp_cpp_example.cmd image_gallery tests\mcp\image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
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
.\tools\mcp_cpp_example.ps1 image_gallery tests\mcp\image_gallery_sixel_info_compare.yaml -McpFast -CompareAppSize 90x35
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
./uimd mcp-test cpp/build/examples/image_gallery/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml
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
python3 tools/uimd_dev.py mcp-csharp-example image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
python3 tools/uimd_dev.py mcp-csharp-example widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Raw POSIX form:

```bash
./uimd mcp-test --backend python --headless csharp/examples tests/mcp/all_examples.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/activity_feed/bin/Release/net10.0/activity_feed.dll tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/calculator/bin/Release/net10.0/calculator.dll tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/cells/bin/Release/net10.0/cells.dll tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/contacts_manager/bin/Release/net10.0/contacts_manager.dll tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/expense_tracker/bin/Release/net10.0/expense_tracker.dll tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/formular/bin/Release/net10.0/formular.dll tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/image_browser/bin/Release/net10.0/image_browser.dll tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/image_gallery/bin/Release/net10.0/image_gallery.dll tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/image_gallery/bin/Release/net10.0/image_gallery.dll tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/markdown_viewer/bin/Release/net10.0/markdown_viewer.dll tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/special_elements/bin/Release/net10.0/special_elements.dll tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/task_board/bin/Release/net10.0/task_board.dll tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/text_editor/bin/Release/net10.0/text_editor.dll tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless csharp/examples/widget_gallery/bin/Release/net10.0/widget_gallery.dll tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

## C++/Swift MCP Compare Tests

Raw macOS POSIX all-example form:

```bash
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35
```

Raw macOS POSIX per-app form:

```bash
./uimd mcp-test --headless --compare cpp/build/examples/activity_feed/activity_feed swift/examples/activity_feed/.build/release/activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/calculator/calculator swift/examples/calculator/.build/release/calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/cells/cells swift/examples/cells/.build/release/cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/contacts_manager/contacts_manager swift/examples/contacts_manager/.build/release/contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/expense_tracker/expense_tracker swift/examples/expense_tracker/.build/release/expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/formular/formular swift/examples/formular/.build/release/formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/image_browser/image_browser swift/examples/image_browser/.build/release/image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/image_gallery/image_gallery swift/examples/image_gallery/.build/release/image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/image_gallery/image_gallery swift/examples/image_gallery/.build/release/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/markdown_viewer/markdown_viewer swift/examples/markdown_viewer/.build/release/markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/special_elements/special_elements swift/examples/special_elements/.build/release/special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/task_board/task_board swift/examples/task_board/.build/release/task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/text_editor/text_editor swift/examples/text_editor/.build/release/text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --headless --compare cpp/build/examples/widget_gallery/widget_gallery swift/examples/widget_gallery/.build/release/widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

## C++/Go MCP Compare Tests

Build the Go example and regression binaries with the commands in the
`Bulk Rebuild` section before running these comparisons.

Raw macOS/Linux POSIX all-example form:

```bash
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples go/examples --mcp-fast --compare-app-size 90x35
```

Raw macOS/Linux POSIX per-app form:

```bash
./uimd mcp-test --backend python --headless --compare cpp/build/examples/activity_feed/activity_feed go/examples/activity_feed/activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/calculator/calculator go/examples/calculator/calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/cells/cells go/examples/cells/cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/contacts_manager/contacts_manager go/examples/contacts_manager/contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/expense_tracker/expense_tracker go/examples/expense_tracker/expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/formular/formular go/examples/formular/formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_browser/image_browser go/examples/image_browser/image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery go/examples/image_gallery/image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery go/examples/image_gallery/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/markdown_viewer/markdown_viewer go/examples/markdown_viewer/markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/special_elements/special_elements go/examples/special_elements/special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/task_board/task_board go/examples/task_board/task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/text_editor/text_editor go/examples/text_editor/text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/widget_gallery/widget_gallery go/examples/widget_gallery/widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Raw macOS/Linux POSIX regression form:

```bash
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll go/regressions/uimd/parity/source_separator_scroll/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus go/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

Windows PowerShell headless all-example form:

```powershell
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
```

Windows PowerShell headless per-app form:

```powershell
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\activity_feed\Release\activity_feed.exe go\examples\activity_feed\activity_feed.exe tests\mcp\activity_feed.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\calculator\Release\calculator.exe go\examples\calculator\calculator.exe tests\mcp\calculator.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\cells\Release\cells.exe go\examples\cells\cells.exe tests\mcp\cells.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\contacts_manager\Release\contacts_manager.exe go\examples\contacts_manager\contacts_manager.exe tests\mcp\contacts_manager.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\expense_tracker\Release\expense_tracker.exe go\examples\expense_tracker\expense_tracker.exe tests\mcp\expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\formular\Release\formular.exe go\examples\formular\formular.exe tests\mcp\formular.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\image_browser\Release\image_browser.exe go\examples\image_browser\image_browser.exe tests\mcp\image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\image_gallery\Release\image_gallery.exe go\examples\image_gallery\image_gallery.exe tests\mcp\image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\image_gallery\Release\image_gallery.exe go\examples\image_gallery\image_gallery.exe tests\mcp\image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\markdown_viewer\Release\markdown_viewer.exe go\examples\markdown_viewer\markdown_viewer.exe tests\mcp\markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\special_elements\Release\special_elements.exe go\examples\special_elements\special_elements.exe tests\mcp\special_elements.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\task_board\Release\task_board.exe go\examples\task_board\task_board.exe tests\mcp\task_board_compare.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\text_editor\Release\text_editor.exe go\examples\text_editor\text_editor.exe tests\mcp\text_editor.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\examples\widget_gallery\Release\widget_gallery.exe go\examples\widget_gallery\widget_gallery.exe tests\mcp\widget_gallery.yaml --mcp-fast --compare-app-size 90x35
```

Windows PowerShell headless regression form:

```powershell
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

Windows cmd.exe headless all-example and regression form:

```bat
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

## C++/Rust MCP Compare Tests

Build the Rust examples and regressions in release mode before running these
POSIX comparisons.

```bash
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples rust/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/activity_feed/activity_feed rust/examples/activity_feed/target/release/activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/calculator/calculator rust/examples/calculator/target/release/calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/cells/cells rust/examples/cells/target/release/cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/contacts_manager/contacts_manager rust/examples/contacts_manager/target/release/contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/expense_tracker/expense_tracker rust/examples/expense_tracker/target/release/expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/formular/formular rust/examples/formular/target/release/formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_browser/image_browser rust/examples/image_browser/target/release/image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery rust/examples/image_gallery/target/release/image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery rust/examples/image_gallery/target/release/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/markdown_viewer/markdown_viewer rust/examples/markdown_viewer/target/release/markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/special_elements/special_elements rust/examples/special_elements/target/release/special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/task_board/task_board rust/examples/task_board/target/release/task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/text_editor/text_editor rust/examples/text_editor/target/release/text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/widget_gallery/widget_gallery rust/examples/widget_gallery/target/release/widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll rust/regressions/uimd/parity/source_separator_scroll/target/release/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus rust/regressions/uimd/parity/stale_scrollview_focus/target/release/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
```

## C++/Java MCP Compare Tests

Build the Java examples and regressions with `installDist` before running these
comparisons.

```bash
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples java/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/activity_feed/activity_feed java/examples/activity_feed/build/install/activity_feed/bin/activity_feed tests/mcp/activity_feed.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/calculator/calculator java/examples/calculator/build/install/calculator/bin/calculator tests/mcp/calculator.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/cells/cells java/examples/cells/build/install/cells/bin/cells tests/mcp/cells.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/contacts_manager/contacts_manager java/examples/contacts_manager/build/install/contacts_manager/bin/contacts_manager tests/mcp/contacts_manager.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/expense_tracker/expense_tracker java/examples/expense_tracker/build/install/expense_tracker/bin/expense_tracker tests/mcp/expense_tracker_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/formular/formular java/examples/formular/build/install/formular/bin/formular tests/mcp/formular.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_browser/image_browser java/examples/image_browser/build/install/image_browser/bin/image_browser tests/mcp/image_browser_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery java/examples/image_gallery/build/install/image_gallery/bin/image_gallery tests/mcp/image_gallery_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/image_gallery/image_gallery java/examples/image_gallery/build/install/image_gallery/bin/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/markdown_viewer/markdown_viewer java/examples/markdown_viewer/build/install/markdown_viewer/bin/markdown_viewer tests/mcp/markdown_viewer.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/special_elements/special_elements java/examples/special_elements/build/install/special_elements/bin/special_elements tests/mcp/special_elements.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/task_board/task_board java/examples/task_board/build/install/task_board/bin/task_board tests/mcp/task_board_compare.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/text_editor/text_editor java/examples/text_editor/build/install/text_editor/bin/text_editor tests/mcp/text_editor.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/examples/widget_gallery/widget_gallery java/examples/widget_gallery/build/install/widget_gallery/bin/widget_gallery tests/mcp/widget_gallery.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll java/regressions/uimd/parity/source_separator_scroll/build/install/source_separator_scroll/bin/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus java/regressions/uimd/parity/stale_scrollview_focus/build/install/stale_scrollview_focus/bin/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --mcp-fast --compare-app-size 90x35
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
./tools/mcp_compare_example.sh image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --mcp-fast
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
.\tools\mcp_compare_example.cmd image_gallery tests\mcp\image_gallery_sixel_info_compare.yaml --mcp-fast
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
.\tools\mcp_compare_example.ps1 image_gallery tests\mcp\image_gallery_sixel_info_compare.yaml -McpFast
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
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples swift/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --backend python --headless --all --compare cpp/build/examples go/examples --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python cpp/build/regressions/uimd/parity tests/regressions/uimd/parity/all.yaml --mcp-fast --compare-app-size 90x35
./uimd mcp-test --compare tests/regressions/uimd/parity/python/stale_scrollview_focus/stale_scrollview_focus.py cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll go/regressions/uimd/parity/source_separator_scroll/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus go/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/source_separator_scroll/source_separator_scroll java/regressions/uimd/parity/source_separator_scroll/build/install/source_separator_scroll/bin/source_separator_scroll tests/regressions/uimd/parity/source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --backend python --headless --compare cpp/build/regressions/uimd/parity/stale_scrollview_focus/stale_scrollview_focus java/regressions/uimd/parity/stale_scrollview_focus/build/install/stale_scrollview_focus/bin/stale_scrollview_focus tests/regressions/uimd/parity/stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/activity_feed/activity_feed.py cpp/build/examples/activity_feed/activity_feed tests/mcp/activity_feed.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/calculator/calculator.py cpp/build/examples/calculator/calculator tests/mcp/calculator.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/cells/cells.py cpp/build/examples/cells/cells tests/mcp/cells.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/contacts_manager/contacts_manager.py cpp/build/examples/contacts_manager/contacts_manager tests/mcp/contacts_manager.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/expense_tracker/expense_tracker.py cpp/build/examples/expense_tracker/expense_tracker tests/mcp/expense_tracker_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/formular/formular.py cpp/build/examples/formular/formular tests/mcp/formular.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/image_browser/image_browser.py cpp/build/examples/image_browser/image_browser tests/mcp/image_browser_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/image_gallery/image_gallery.py cpp/build/examples/image_gallery/image_gallery tests/mcp/image_gallery_compare.yaml --compare-app-size 90x35 --mcp-fast
./uimd mcp-test --compare python/examples/image_gallery/image_gallery.py cpp/build/examples/image_gallery/image_gallery tests/mcp/image_gallery_sixel_info_compare.yaml --compare-app-size 90x35 --mcp-fast
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
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --backend python --headless --all --compare cpp\build-windows\examples java\examples --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.ps1 mcp-test --compare tests\regressions\uimd\parity\python\stale_scrollview_focus\stale_scrollview_focus.py cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe java\regressions\uimd\parity\source_separator_scroll\build\install\source_separator_scroll\bin\source_separator_scroll.bat tests\regressions\uimd\parity\source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe java\regressions\uimd\parity\stale_scrollview_focus\build\install\stale_scrollview_focus\bin\stale_scrollview_focus.bat tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.ps1 mcp-test --compare python\examples\activity_feed\activity_feed.py cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --compare-app-size 90x35 --mcp-fast
```

Raw Windows cmd.exe form:

```bat
.\uimd.cmd mcp-test --all --compare python\examples cpp\build-windows\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --compare python\examples csharp\examples tests\mcp\all_examples.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples csharp\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples go\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --backend python --headless --all --compare cpp\build-windows\examples java\examples --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python cpp\build-windows\regressions\uimd\parity tests\regressions\uimd\parity\all.yaml --mcp-fast --compare-app-size 90x35
.\uimd.cmd mcp-test --compare tests\regressions\uimd\parity\python\stale_scrollview_focus\stale_scrollview_focus.py cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe go\regressions\uimd\parity\source_separator_scroll\source_separator_scroll.exe tests\regressions\uimd\parity\source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe go\regressions\uimd\parity\stale_scrollview_focus\stale_scrollview_focus.exe tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\source_separator_scroll\Release\source_separator_scroll.exe java\regressions\uimd\parity\source_separator_scroll\build\install\source_separator_scroll\bin\source_separator_scroll.bat tests\regressions\uimd\parity\source_separator_scroll.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --backend python --headless --compare cpp\build-windows\regressions\uimd\parity\stale_scrollview_focus\Release\stale_scrollview_focus.exe java\regressions\uimd\parity\stale_scrollview_focus\build\install\stale_scrollview_focus\bin\stale_scrollview_focus.bat tests\regressions\uimd\parity\stale_scrollview_focus.yaml --compare-app-size 90x35 --mcp-fast
.\uimd.cmd mcp-test --compare python\examples\activity_feed\activity_feed.py cpp\build-windows\examples\activity_feed\Release\activity_feed.exe tests\mcp\activity_feed.yaml --compare-app-size 90x35 --mcp-fast
```

## Project Scaffold Smoke Test

Python and C++ source-checkout scaffold smoke:

```bash
./uimd new hello --target python
./uimd run hello.uimd
./uimd generate hello.uimd --target cpp
cmake -S . -B build
cmake --build build
./build/hello
```

Go source-checkout scaffold build smoke on macOS/Linux:

```bash
repo_root="$PWD"
work_dir="$(mktemp -d)"
cd "$work_dir"
"$repo_root/uimd" new hello --target go
"$repo_root/uimd" generate hello.uimd --target go
env GOCACHE="${TMPDIR:-/tmp}/uimd-go-build-cache" go build .
```

Installed-SDK Go scaffold lookup and external-project compilation are also
covered by:

```bash
python3 tools/native_uimd_parity.py --compile-examples
```

Rust source-checkout scaffold build smoke on macOS/Linux:

```bash
repo_root="$PWD"
work_dir="$(mktemp -d)"
cd "$work_dir"
"$repo_root/uimd" new hello --target rust
"$repo_root/uimd" generate hello.uimd --target rust
python3 "$repo_root/tools/cargo_with_progress.py" build --release
```

Installed-SDK Rust target lookup and external-project compilation are also
covered by `python3 tools/native_uimd_parity.py --compile-examples`.

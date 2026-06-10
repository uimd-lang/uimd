# UIMD

User Interface Markdown.

Define app interfaces in Markdown. Generate UI and agent access.

UIMD is a Markdown-based interface definition format and toolchain. It
generates application UI source files from `.uimd` documents and embeds
metadata that can be exposed through MCP for agents, scripts, and other
programs.

The current runtime targets terminal applications. The format and repository
layout are prepared for additional backends such as desktop, web, mobile, and
more native runtimes.

## Install

From PyPI, once published:

```bash
python3 -m pip install uimd
```

From GitHub:

```bash
python3 -m pip install git+https://github.com/uimd-lang/uimd.git
```

For isolated project development:

```bash
mkdir hello-ui
cd hello-ui
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install uimd
```

On Windows PowerShell:

```powershell
mkdir hello-ui
cd hello-ui
py -m venv .venv
.venv\Scripts\Activate.ps1
py -m pip install uimd
```

Apps that use non-fallback `Image` rendering need Sixel support. Python apps
use the `libsixel-python` package; C++ apps dynamically load the system
`libsixel` library and search common install locations such as Homebrew
automatically. See `docs/installation.md` for details.

## Hello World

```bash
./uimd new hello
./uimd run hello.uimd
```

This creates:

```text
hello.uimd
hello.py
hello_ui.py
```

During development:

```bash
./uimd generate hello.uimd
python3 hello.py
```

## CLI

```bash
./uimd new hello
./uimd generate hello.uimd
./uimd run hello.uimd
./uimd generate hello.uimd --target cpp
./uimd mcp-test hello.py tests/mcp/hello.yaml
./uimd doctor
```

`uimd` is the native SDK-facing tool. In a source checkout, use the repo wrapper
`./uimd`; installed SDKs provide a native `uimd` binary on `PATH`. The Python
package provides the Python runtime and tester helpers, not the compiler CLI.

The visual MCP tester is also installed for projects that use UIMD:

```bash
./uimd mcp-test hello.py tests/mcp/hello.yaml
uimd-mcp-test hello.py tests/mcp/hello.yaml
```

The tested app must be a generated UIMD app or compatible executable that
accepts the MCP runtime flags used by UIMD.

## Format

`.uimd` is the canonical file extension. Existing `.md` UI files with
`format: ui-md` remain supported for compatibility.

````md
# Hello

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
description: "Hello world UI."
```

## Members

```yaml
title:
  type: label
  text: "Hello from UIMD"
  description: "Application title."
```

## User Interface

```ui
+-----------------------------+
| title...................... |
+-----------------------------+
```
````

## C++

Generate C++ UI source files and bootstrap a single-file C++ app when needed:

```bash
./uimd generate hello.uimd --target cpp
cmake -S . -B build
cmake --build build
```

For single-file generation, missing `hello.cpp` and `CMakeLists.txt` files are
created once and are not overwritten later. The generated CMake project can use
a local sibling checkout at `../uimd/cpp` during development.

The C++ runtime is available through CMake targets:

```cmake
target_link_libraries(my_app PRIVATE uimd::runtime)
```

The repository currently keeps the C++ backend under `cpp/`.

## Repository

Official source:

```text
https://github.com/uimd-lang/uimd
```

Main directories:

```text
src/uimd/       Python runtime package, dialogs, testing helpers, specs, and themes
cpp/            C++ runtime, generator support, examples, and tests
shared/         Cross-backend themes, IR notes, and specs
docs/           User and maintainer documentation
tests/          Cross-project and MCP scenarios
tools/          Development helpers
```

## Development

Run Python tests:

```bash
python3 -m pytest python/tests
```

Build and test C++:

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

This repository currently uses SVN for local development. Do not assume Git
workflow inside this checkout.

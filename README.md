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

UIMD is currently an alpha project. The only validated development platform
today is macOS on Intel (`x86_64`). macOS Apple Silicon support is next; Windows
x64 support is planned after that.

There is no public PyPI package, Homebrew package, GitHub Release, or packaged
SDK installer yet. Do not use `pip install uimd`, `pip install uimd-sdk`,
`brew install uimd-sdk`, or release download commands until the first public
release exists.

Current alpha setup is a source checkout build:

```bash
git clone https://github.com/uimd-lang/uimd.git
cd uimd
git switch sdk-work
cmake -S cpp -B cpp/build
cmake --build cpp/build --target uimd
./uimd doctor
```

The `./uimd` script is the source-checkout entry point. It expects the native
SDK-facing binary at `cpp/build/tools/uimd/uimd`; build that target before
running `./uimd`.

To build the local SDK bootstrapper as well:

```bash
cmake --build cpp/build --target uimd_init
```

The local SDK Store work exists for development and testing, but it is not a
public installer flow yet. To exercise it from a source checkout:

```bash
UIMD_HOME=/tmp/uimd-home \
UIMD_INIT_UIMD_BINARY=$PWD/cpp/build/tools/uimd/uimd \
  cpp/build/tools/uimd_init/uimd-init --no-shell-config --json
/tmp/uimd-home/bin/uimd doctor --json
```

This creates a local SDK Store under `UIMD_HOME` and installs the development
launcher plus a versioned SDK binary. Network downloads, release asset
verification, target payload downloads, and package-manager recipes are still
future release work.

Apps that use non-fallback `Image` rendering need Sixel support. Python apps
use the `libsixel-python` package; C++ apps dynamically load the system
`libsixel` library and search common install locations such as Homebrew
automatically. See `docs/installation.md` for details.

## Hello World

After building the native source-checkout CLI:

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

## SDK Model

The intended SDK model is:

```text
package manager installs uimd-init
uimd-init creates or repairs ~/.uimd/
~/.uimd/bin/uimd delegates to ~/.uimd/sdk/<version>/bin/uimd
project commands select SDK versions from root .uimd sdk-version metadata
```

The implementation currently supports the local/offline foundation:

```bash
./uimd sdk home
./uimd sdk install 0.x.y --release-root /path/to/releases
./uimd sdk install-target cpp
./uimd sdk use 0.x.y
./uimd sdk list --json
./uimd sdk update --release-root /path/to/releases --json
./uimd sdk prune --json
./uimd doctor --json
UIMD_HOME=/tmp/uimd-home ./uimd self uninstall --json
```

These commands are available from the native binary, but public release assets
and package-manager installation are not available yet. See `docs/sdk-store.md`
for the current implementation status.

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
`./uimd`. Packaged SDKs will later provide a native `uimd` binary on `PATH`.
The future Python package will provide the Python runtime and tester helpers,
not the compiler CLI.

The visual MCP tester is available through the source-checkout CLI:

```bash
./uimd mcp-test hello.py tests/mcp/hello.yaml
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

The repository currently keeps the C++ backend under `cpp/`. The generated
CMake GitHub `FetchContent` fallback depends on a future public release tag;
until then, use a local source checkout or installed CMake package.

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

This GitHub repository uses `main` as the stable public snapshot branch and
`sdk-work` for active SDK and publishing work. Alpha development should happen
on `sdk-work`.

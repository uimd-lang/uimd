# Installation

UIMD currently installs as a Python package and provides the `uimd` console
command. The repository also builds a native `uimd` binary as the SDK-facing
tool surface; public package-manager bootstrap is being split so end-user SDK
installs use the native binary while `pip install uimd` remains the Python
runtime package.

## Recommended Project Install

macOS/Linux:

```bash
mkdir hello-ui
cd hello-ui
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install uimd
```

Windows PowerShell:

```powershell
mkdir hello-ui
cd hello-ui
py -m venv .venv
.venv\Scripts\Activate.ps1
py -m pip install uimd
```

Supported development platforms are macOS, Linux, and Windows. The current
runtime targets terminal applications, so terminal color, keyboard, mouse, and
PTY behavior can vary by terminal emulator.

## GitHub Install

```bash
python3 -m pip install git+https://github.com/uimd-lang/uimd.git
```

## Global CLI Install

Use `pipx` when you want the `uimd` command available globally but isolated
from system Python packages:

```bash
pipx install uimd
```

## Native Binary From Source

The native SDK-facing command builds through the C++ project:

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build --target uimd
cmake --build cpp/build --target uimd_init
cpp/build/tools/uimd/uimd doctor --json
```

Native generation and tool commands are smoke-checked with:

```bash
python3 tools/native_uimd_parity.py
```

## C++ Projects

C++ projects still use the `uimd` CLI to generate source from `.uimd` files:

```bash
uimd generate hello.uimd --target cpp
```

In a source checkout, use `./uimd generate hello.uimd --target cpp`. In an
installed SDK, `uimd` is the native SDK-facing binary on `PATH`.

That does not make the final C++ executable depend on the Python runtime. The
generated C++ code links against the UIMD C++ runtime through CMake. The
generated `CMakeLists.txt` first tries an installed CMake package, then a local
sibling checkout, then the official GitHub release tag through `FetchContent`.

## Image And Sixel Support

Most UIMD apps do not need image/Sixel dependencies. Apps that use
non-fallback `Image` rendering need `libsixel`.

For Python apps with non-fallback images, install the Python binding into the
same Python environment that runs the app:

```bash
python3 -m pip install libsixel-python
```

For C++ apps with non-fallback images, install the system `libsixel` library.
On macOS with Homebrew:

```bash
brew install libsixel
```

The C++ runtime loads `libsixel` dynamically when an app actually needs
non-fallback image rendering. It first tries normal system loader paths and
then common install locations, including Homebrew, MacPorts, Linux library
directories, Windows install directories, and `lib` directories near the
executable. A normal Homebrew install should work without any extra terminal
variables.

If a prebuilt app must run with `libsixel` in a non-standard location, use one
of these optional fallback variables:

```bash
export UIMD_LIBSIXEL_PATH=/path/to/libsixel.1.dylib
export UIMD_LIBSIXEL_DIR=/path/to/libsixel/lib
```

Use `UIMD_LIBSIXEL_PATH` for an exact library file and `UIMD_LIBSIXEL_DIR` for
a directory containing the library. These are not required for standard
installations.

## Installed Files

The Python runtime is installed into the active Python environment, usually
under a path like:

```text
.../site-packages/uimd/runtime/
```

For `pip install uimd`, the `uimd` command is a Python console script generated
by pip. On macOS/Linux it is installed into the environment `bin/` directory,
for example:

```text
.venv/bin/uimd
~/.local/bin/uimd
```

On Windows it is installed into the environment `Scripts\` directory, for
example:

```text
.venv\Scripts\uimd.exe
```

For source builds, the native command is produced at:

```text
cpp/build/tools/uimd/uimd
```

## pip, pip3, and python3 -m pip

Prefer:

```bash
python3 -m pip install uimd
```

This installs into the package environment of the same `python3` interpreter
you use to run your app. `pip`, `pip3`, and `python3 -m pip` can point to
different Python installations on the same machine.

Inspect the installed location:

```bash
python3 -m pip show uimd
```

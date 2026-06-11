# Installation

UIMD has two install surfaces:

- The SDK installer provides the native `uimd` compiler/launcher and installs
  into the SDK Store.
- The future `pip install uimd` package is only the Python runtime package; it
  does not bootstrap or repair the SDK Store.

## SDK Install

Use the versioned GitHub Release URL after the matching release assets are
published and smoke-tested. Do not use `releases/latest/download/install.sh`
as the primary command until the prerelease/latest policy is verified.

macOS/Linux:

```bash
curl -fsSL https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.sh | sh
~/.uimd/bin/uimd doctor
```

Windows PowerShell:

```powershell
Invoke-WebRequest -UseBasicParsing https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.ps1 -OutFile install.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File .\install.ps1
& "$env:LOCALAPPDATA\uimd\bin\uimd.exe" doctor
```

The default install location is:

```text
macOS/Linux: ~/.uimd
Windows:     %LOCALAPPDATA%\uimd
Override:    UIMD_HOME
```

The installer does not modify shell startup files by default. For immediate
use, call the launcher directly:

```bash
~/.uimd/bin/uimd run hello.uimd
```

For human PATH setup, opt in explicitly:

```bash
curl -fsSL https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.sh | sh -s -- --modify-shell
```

Then open a new shell or source the updated shell profile, for example:

```bash
source ~/.zshrc
uimd doctor
```

For agents and CI, keep startup files untouched:

```bash
curl -fsSL https://github.com/uimd-lang/uimd/releases/download/v0.4.0/install.sh | sh -s -- --no-shell-config
~/.uimd/bin/uimd doctor --json
```

After PATH setup, update an installed SDK launcher through the normal `uimd`
command:

```bash
uimd self update
```

When an installed project command needs a missing target such as `cpp`, the
launcher auto-installs it from release assets unless offline mode is enabled.
`UIMD_RELEASE_ROOT` and `UIMD_RELEASE_BASE_URL` are development/CI overrides,
not the normal user path.

The current packaged SDK flow is validated first on macOS Intel (`x86_64`).
The release tooling also emits macOS Apple Silicon, Linux, and Windows
platform labels plus a Windows `install.ps1`, but those release assets still
need platform validation before being advertised as supported public installs.

## Python Runtime Package

`pip install uimd` is reserved for the Python runtime package. It is for users
running Python UIMD applications that report missing UIMD Python libraries; it
is not the compiler/SDK installer.

Package-manager bootstrap packages such as `uimd-sdk` for Homebrew, PyPI,
winget, or apt are not published yet. Their future job is to install
`uimd-init`; `uimd-init` then creates or repairs the SDK Store.

To remove a per-user SDK Store, run `uimd self uninstall` before uninstalling
any future package-manager shim. It removes UIMD-owned PATH marker blocks from
supported shell profiles and deletes the SDK Store after safety checks.

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

The SDK installer writes the native launcher and versioned SDK under the SDK
Store:

```text
~/.uimd/bin/uimd
~/.uimd/current
~/.uimd/sdk/0.4.0/bin/uimd
~/.uimd/sdk/0.4.0/targets/python/
~/.uimd/sdk/0.4.0/targets/cpp/
```

On Windows the SDK Store lives under:

```text
%LOCALAPPDATA%\uimd
```

The Python runtime package is installed into the active Python environment,
usually under a path like:

```text
.../site-packages/uimd/runtime/
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

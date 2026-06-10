# CLI

UIMD installs one command:

```bash
uimd
```

It follows the common `tool action arguments` model used by tools such as
`dotnet`, `git`, `cargo`, and `docker`.

## Commands

```bash
uimd new hello
```

Create a new app in the current directory.

```bash
uimd generate hello.uimd
```

Generate Python UI source files from a `.uimd` file.

```bash
uimd generate hello.uimd --target cpp
```

Generate C++ UI source files. For a single `.uimd` source, this also creates missing `hello.cpp` and `CMakeLists.txt` bootstrap files without overwriting existing ones.

```bash
uimd run hello.uimd
```

Generate Python UI source files and run the matching `hello.py` app.

```bash
uimd doctor
```

Inspect the native tool, source checkout, and local SDK Store. Use
`uimd doctor --json` for agent-readable diagnostics.

```bash
uimd sdk home
uimd sdk install 0.x.y
uimd sdk install 0.x.y --release-root /path/to/releases
uimd sdk install-target cpp
uimd sdk use 0.x.y
uimd sdk list --json
uimd sdk remove 0.x.y
uimd sdk prune --json
uimd sdk update --release-root /path/to/releases --json
```

Manage local SDK Store entries under `UIMD_HOME` or the platform default store.
`uimd sdk install` is currently a local/offline copy install; pass
`--from /path/to/uimd` to install a specific local binary into
`sdk/<version>/bin/uimd`, or pass `--release-root /path/to/releases` to install
from a local manifest fixture with SHA-256 verification. `uimd sdk
install-target` currently creates the local target directory for an installed
SDK version; network release downloads are a later packaging step. `uimd sdk
prune` keeps the newest two patch versions per minor series and preserves the
current SDK version. `uimd sdk update` selects the newest available patch in
the current minor series from already installed SDKs and, with `--release-root`,
can install that patch from a local manifest fixture.

```bash
uimd --require-sdk-version generate hello.uimd
uimd --offline generate hello.uimd
```

When running through an installed SDK Store launcher, project commands read
`sdk-version` from the root `.uimd` Metadata and delegate to the highest
installed compatible SDK version. `--require-sdk-version` fails if the source
does not declare `sdk-version`; `--offline` fails fast when the required SDK is
not already installed.

Set `UIMD_SDK_PATH=/path/to/uimd` or point it at a local SDK/source checkout to
bypass SDK Store version resolution for project commands during development.
The launcher still handles `sdk`, `self`, and `doctor` locally.

```bash
uimd-init --no-shell-config --json
uimd-init --modify-shell --json
```

Bootstrap or repair the local SDK Store. `--no-shell-config` leaves shell
startup files untouched for agents and CI. `--modify-shell` idempotently adds
the SDK launcher directory to the active user's shell profile and reports the
profile path plus whether it changed in JSON.

```bash
uimd self uninstall --json
```

Remove the local SDK Store under `UIMD_HOME` or the platform default store.
Package-manager uninstall and shell profile cleanup remain separate user steps.

## Native Launcher

`uimd` is the native SDK-facing command. In a source checkout, `./uimd`
executes the repo-local native binary from `cpp/build/tools/uimd/uimd`.
Installed SDK Stores place the real launcher at `UIMD_HOME/bin/uimd`; that
launcher handles `sdk` and `doctor` locally and delegates other commands to the
selected `sdk/<version>/bin/uimd`.

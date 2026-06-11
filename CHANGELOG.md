# Changelog

## 0.4.0 - Unreleased

- Add a cross-platform `Image` element with Sixel rendering and an ANSI
  character-based fallback in Python and C++.
- Add `.uimd` image properties: source, alt/fallback text, fit mode,
  render mode, and alignment.
- Integrate libsixel for Sixel output with in-house encoder fallback, add the
  `lodepng` decoder for C++, and add `Pillow` as a Python runtime dependency.
- Add TerminalBuffer raw-cell support so image payloads survive diff rendering.
- Add the `image_gallery` example for Python and C++ with sample image assets.
- Add the local SDK Store/Launcher alpha foundation: `uimd-init`, local
  `uimd sdk install/use/list/remove/prune/update`, `doctor --json`, local
  release-root manifest installs, and self-uninstall.
- Add local macOS Intel SDK release artifact packaging for maintainer/dev
  validation, including manifest payloads, `uimd-init`, `install.sh`, tarball
  output, and checksums.
- Add the minimal macOS Intel GitHub Release install path: `install.sh`
  downloads and verifies `uimd-init`, and `uimd-init` downloads, verifies,
  extracts, and installs the matching SDK tarball. Published release assets and
  package-manager installers are still future work.
- Harden the SDK installer MVP with cross-platform release labels, generated
  Windows `install.ps1`, platform-aware `uimd-init` download/extract helpers,
  and repair detection for missing Python SDK targets.
- Add default `uimd self update` from release assets, with local release-root
  override support for CI, and launcher-side auto-install for missing installed
  SDK targets before project command delegation.
- Add default `uimd sdk update` from release assets, launcher-side auto-install
  for missing project SDK versions, and removal of UIMD-owned shell profile
  PATH blocks during `uimd self uninstall`.
- Add minisign signature hardening for release downloads: package
  `checksums.txt.minisig`, embed the public release key in bootstrap scripts
  and native tools, and verify signatures before SHA-256 asset checks.

## 0.2.0 - Unreleased

- Establish UIMD naming: User Interface Markdown, `.uimd`, and `uimd`.
- Add packaging and distribution work toward a public Python package.
- Prepare C++ CMake targets for public `uimd::` consumption.
- Add the C++ MCP tester as the default tester backend and keep the Python
  tester available as an explicit backend.
- Improve Python/C++ runtime parity for modal dialogs, browse dialogs,
  scroll views, edit controls, selection styles, terminal rendering, and MCP
  snapshots.
- Add compare snapshot diagnostics, shared rendering helpers, shorter MCP
  compare scripts, isolated test temporary directories, and broader example
  parity coverage.

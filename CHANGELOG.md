# Changelog

## 0.3.0 - Unreleased

- Add a cross-platform `Image` element with Sixel rendering and an ANSI
  character-based fallback in Python and C++.
- Add `.uimd` image properties: source, alt/fallback text, fit mode,
  render mode, and alignment.
- Integrate libsixel for Sixel output with in-house encoder fallback, add the
  `lodepng` decoder for C++, and add `Pillow` as a Python runtime dependency.
- Add TerminalBuffer raw-cell support so image payloads survive diff rendering.
- Add the `image_gallery` example for Python and C++ with sample image assets.

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

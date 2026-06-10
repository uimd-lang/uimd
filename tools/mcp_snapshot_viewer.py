#!/usr/bin/env python3
"""View saved MCP render snapshot compare bundles."""

import argparse
import json
import os
import sys


RESET = "\x1b[0m"


def main(argv=None):
    parser = argparse.ArgumentParser(description="View an MCP snapshot compare bundle")
    parser.add_argument("snapshot", help="Path to a JSON snapshot bundle")
    parser.add_argument("--plain", action="store_true", help="Render without ANSI colors")
    parser.add_argument("--cell", metavar="ROW,COL", help="Print cell details for every target")
    args = parser.parse_args(argv)

    path = os.path.abspath(args.snapshot)
    with open(path, encoding="utf-8") as handle:
        bundle = json.load(handle)

    targets = bundle.get("targets") or []
    if not targets:
        raise SystemExit("snapshot bundle has no targets")

    print(f"file: {os.path.relpath(path)}")
    print(f"created_at: {bundle.get('created_at', '')}")
    print(f"source: {bundle.get('source_path', '')}")
    print(f"step: {bundle.get('step_index', '')}")
    print(f"mismatch: {bundle.get('mismatch', '')}")

    if args.cell:
        row, col = _parse_cell(args.cell)
        print()
        _print_cell_details(targets, row, col)
        return 0

    print()
    _print_side_by_side(targets, plain=args.plain, mismatch=bundle.get("mismatch", ""))
    return 0


def _parse_cell(value):
    parts = str(value).split(",", 1)
    if len(parts) != 2:
        raise SystemExit("--cell must have ROW,COL format")
    return int(parts[0]), int(parts[1])


def _print_cell_details(targets, row, col):
    for target in targets:
        cells = _snapshot_cells(target.get("snapshot", {}))
        cell = {}
        if 0 <= row < len(cells) and 0 <= col < len(cells[row]):
            cell = cells[row][col]
        print(f"{target.get('name', '')}: {json.dumps(cell, ensure_ascii=False, sort_keys=True)}")


def _print_side_by_side(targets, plain=False, mismatch=""):
    panels = []
    for target in targets:
        cells = _snapshot_cells(target.get("snapshot", {}))
        panels.append({
            "name": str(target.get("name", "")),
            "cells": cells,
            "width": max((len(row) for row in cells), default=0),
            "height": len(cells),
        })

    mismatch_pos = _parse_mismatch_position(mismatch)
    header_parts = []
    for panel in panels:
        header_parts.append(panel["name"].center(panel["width"]))
    print("  ".join(header_parts))

    max_height = max((panel["height"] for panel in panels), default=0)
    for row in range(max_height):
        parts = []
        for panel in panels:
            cells = panel["cells"][row] if row < panel["height"] else []
            parts.append(_render_row(cells, panel["width"], plain, row, mismatch_pos))
        suffix = "" if plain else RESET
        print("  ".join(parts) + suffix)


def _parse_mismatch_position(message):
    tokens = str(message).split()
    try:
        row_index = tokens.index("row")
        col_index = tokens.index("col")
        return int(tokens[row_index + 1]), int(tokens[col_index + 1])
    except (ValueError, IndexError):
        return None


def _render_row(cells, width, plain, row, mismatch_pos):
    rendered = []
    for col in range(width):
        cell = cells[col] if col < len(cells) else _blank_cell()
        char = str(cell.get("char", " ") or " ")[:1]
        if mismatch_pos == (row, col):
            char = "!"
        if plain:
            rendered.append(char)
        else:
            rendered.append(_ansi_cell(cell, char))
    return "".join(rendered)


def _ansi_cell(cell, char):
    codes = []
    fg = cell.get("foreground")
    bg = cell.get("background")
    if fg:
        codes.append(_ansi_color(fg, foreground=True))
    if bg:
        codes.append(_ansi_color(bg, foreground=False))
    attrs = cell.get("attributes") or []
    if "bold" in attrs:
        codes.append("1")
    if "underline" in attrs:
        codes.append("4")
    if not codes:
        return RESET + char
    return "\x1b[" + ";".join(codes) + "m" + char


def _ansi_color(value, foreground):
    value = str(value)
    if value.startswith("#") and len(value) >= 7:
        red = int(value[1:3], 16)
        green = int(value[3:5], 16)
        blue = int(value[5:7], 16)
        prefix = "38" if foreground else "48"
        return f"{prefix};2;{red};{green};{blue}"
    return "39" if foreground else "49"


def _snapshot_cells(snapshot):
    cells = snapshot.get("cells")
    if cells is not None:
        return [[_snapshot_cell(cell) for cell in row] for row in cells]
    text_lines = snapshot.get("text_lines")
    if text_lines is not None:
        return [
            [
                {
                    "char": char,
                    "foreground": None,
                    "background": None,
                    "attributes": [],
                }
                for char in str(line)
            ]
            for line in text_lines
        ]
    return []


def _snapshot_cell(cell):
    if isinstance(cell, dict):
        return {
            "char": cell.get("char", " "),
            "foreground": cell.get("foreground"),
            "background": cell.get("background"),
            "attributes": cell.get("attributes", []),
        }
    if isinstance(cell, list):
        return {
            "char": cell[0] if len(cell) > 0 else " ",
            "foreground": cell[1] if len(cell) > 1 else None,
            "background": cell[2] if len(cell) > 2 else None,
            "attributes": cell[3] if len(cell) > 3 else [],
        }
    return _blank_cell(char=str(cell))


def _blank_cell(char=" "):
    return {
        "char": char,
        "foreground": None,
        "background": None,
        "attributes": [],
    }


if __name__ == "__main__":
    sys.exit(main())

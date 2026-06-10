"""Compatibility facade for legacy ``runtime`` imports.

The canonical Python runtime implementation lives in ``src/uimd/runtime``.
This package exists only so older examples and generated files using
``from runtime ...`` keep working without carrying a second runtime copy.
"""

from pathlib import Path
import importlib
import sys


_PROJECT_ROOT = Path(__file__).resolve().parents[2]
_SRC_DIR = _PROJECT_ROOT / "src"
if _SRC_DIR.is_dir():
    src_path = str(_SRC_DIR)
    if src_path not in sys.path:
        sys.path.insert(0, src_path)

_CANONICAL_PACKAGE = "uimd.runtime"
_SUBMODULES = (
    "application",
    "UIBase",
    "uiwindow",
    "UIControl",
    "UIScrollView",
    "UIPopup",
    "elements",
    "image",
    "mcp",
    "rendering",
    "style",
    "uielement",
    "uiinstance",
)


def _alias_module(module_name):
    module = importlib.import_module(f"{_CANONICAL_PACKAGE}.{module_name}")
    sys.modules[f"{__name__}.{module_name}"] = module
    return module


for _module_name in _SUBMODULES:
    _alias_module(_module_name)

from uimd.runtime import (  # noqa: E402,F401
    UIApplication,
    UIBase,
    LayoutCell,
    ResolvedCell,
    LayoutNode,
    LeafNode,
    GroupNode,
    UIWindow,
    UIControl,
    UIScrollView,
    UIPopup,
    Label,
    FrameBufferView,
    TextInput,
    TextArea,
    NumberInput,
    Button,
    CheckBox,
    ComboBox,
    ListBox,
    UIElementReusable,
    ViewHost,
    Image,
    Style,
    Color,
)

__all__ = [
    "UIApplication",
    "UIBase",
    "LayoutCell",
    "ResolvedCell",
    "LayoutNode",
    "LeafNode",
    "GroupNode",
    "UIWindow",
    "UIControl",
    "UIScrollView",
    "UIPopup",
    "Label",
    "FrameBufferView",
    "TextInput",
    "TextArea",
    "NumberInput",
    "Button",
    "CheckBox",
    "ComboBox",
    "ListBox",
    "UIElementReusable",
    "ViewHost",
    "Image",
    "Style",
    "Color",
]

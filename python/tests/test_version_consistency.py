"""Version consistency checks for package and C++ release surfaces."""

import importlib.util
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "src"))

from uimd import __version__


def _read(relative_path):
    return (ROOT / relative_path).read_text(encoding="utf-8")


def _load_set_version_module():
    spec = importlib.util.spec_from_file_location("set_version", ROOT / "tools" / "set_version.py")
    assert spec is not None
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def test_release_version_surfaces_are_consistent():
    pyproject = _read("pyproject.toml")
    pyproject_version = re.search(r'^version\s*=\s*"([^"]+)"', pyproject, re.MULTILINE)
    assert pyproject_version is not None
    assert pyproject_version.group(1) == __version__

    cmake = _read("cpp/CMakeLists.txt")
    assert f"project(ui_cpp_runtime VERSION {__version__} LANGUAGES CXX)" in cmake
    assert 'target_compile_definitions(ui_cpp_runtime PRIVATE UIMD_VERSION="${PROJECT_VERSION}")' in cmake
    assert 'target_compile_definitions(ui_cpp_tests PRIVATE UIMD_EXPECTED_VERSION="${PROJECT_VERSION}")' in cmake
    assert "VERSION ${PROJECT_VERSION}" in cmake

    cpp_version = _read("cpp/src/core/Version.cpp")
    assert "return UIMD_VERSION;" in cpp_version
    assert 'return "0.1.0";' not in cpp_version

    cpp_runtime_test = _read("cpp/tests/test_runtime_skeleton.cpp")
    assert "runtimeVersion() == std::string_view(UIMD_EXPECTED_VERSION)" in cpp_runtime_test

    native_generator = _read("cpp/tools/uimd/NativeCppGenerator.cpp")
    assert '"        GIT_TAG v" + std::string{UIMD_VERSION} + "\\n"' in native_generator

    native_cli = _read("cpp/tools/uimd/main.cpp")
    assert '"@VERSION@", runtimeVersion()' in native_cli

    cpp_cmake_docs = _read("docs/cpp-cmake.md")
    assert f"GIT_TAG v{__version__}" in cpp_cmake_docs

    changelog = _read("CHANGELOG.md")
    assert re.search(rf"^## {re.escape(__version__)}(?:\s|-)", changelog, re.MULTILINE)


def test_set_version_check_accepts_current_version():
    set_version = _load_set_version_module()
    assert set_version.main([__version__, "--check"]) == 0

"""Version consistency checks for package, native, and Java release surfaces."""

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

    java_build = _read("java/build.gradle")
    assert f'version = "{__version__}"' in java_build

    java_version = _read("java/src/main/java/uimd/Version.java")
    assert f'private static final String RUNTIME_VERSION = "{__version__}";' in java_version
    assert "public static String runtimeVersion()" in java_version

    native_java_generator = _read("cpp/tools/uimd/NativeJavaGenerator.cpp")
    assert '"org.uimd:uimd:" UIMD_VERSION' in native_java_generator
    for java_project in sorted((ROOT / "java").glob("**/build.gradle")):
        if java_project == ROOT / "java" / "build.gradle":
            continue
        project_build = java_project.read_text(encoding="utf-8")
        assert f'implementation "org.uimd:uimd:{__version__}"' in project_build

    cpp_cmake_docs = _read("docs/cpp-cmake.md")
    assert f"GIT_TAG v{__version__}" in cpp_cmake_docs

    changelog = _read("CHANGELOG.md")
    assert re.search(rf"^## {re.escape(__version__)}(?:\s|-)", changelog, re.MULTILINE)


def test_set_version_check_accepts_current_version():
    set_version = _load_set_version_module()
    assert set_version.main([__version__, "--check"]) == 0


def test_set_version_plan_covers_every_java_version_surface():
    set_version = _load_set_version_module()
    planned_version = "9.8.7"
    replacements = {
        replacement.path: replacement.replacement
        for replacement in set_version._replacement_plan(planned_version)
    }

    assert replacements["java/build.gradle"] == f'version = "{planned_version}"'
    assert replacements["java/src/main/java/uimd/Version.java"].endswith(
        f'RUNTIME_VERSION = "{planned_version}";'
    )
    generated_projects = {
        path.relative_to(ROOT).as_posix()
        for path in (ROOT / "java").glob("**/build.gradle")
        if path != ROOT / "java" / "build.gradle"
    }
    assert generated_projects
    for project in generated_projects:
        assert replacements[project] == f'implementation "org.uimd:uimd:{planned_version}"'

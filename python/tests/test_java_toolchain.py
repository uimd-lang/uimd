"""Java 17 resolver integration checks for repository development helpers."""

import os
from pathlib import Path
import subprocess

import pytest

from tools import java_toolchain


def test_java_build_env_uses_the_sdk_shipped_resolver(monkeypatch, tmp_path: Path):
    java_home = tmp_path / "jdk-17"
    java_binary = java_home / "bin" / ("java.exe" if os.name == "nt" else "java")
    java_binary.parent.mkdir(parents=True)
    java_binary.write_text("test java", encoding="utf-8")

    completed = subprocess.CompletedProcess(
        ["uimd-java", "--java-home"],
        0,
        stdout=f"{java_home}\n",
        stderr="",
    )
    monkeypatch.setattr(java_toolchain, "java_resolver_command", lambda: ["uimd-java"])
    monkeypatch.setattr(java_toolchain.subprocess, "run", lambda *args, **kwargs: completed)
    monkeypatch.setenv("PATH", "existing-path")
    monkeypatch.setenv("GRADLE_USER_HOME", str(tmp_path / "gradle-cache"))

    resolved = java_toolchain.java_build_env()

    assert resolved["JAVA_HOME"] == str(java_home)
    assert resolved["PATH"].split(os.pathsep) == [str(java_home / "bin"), "existing-path"]
    assert resolved["GRADLE_USER_HOME"] == str(tmp_path / "gradle-cache")


def test_java_resolver_failure_preserves_actionable_diagnostic(monkeypatch):
    completed = subprocess.CompletedProcess(
        ["uimd-java", "--java-home"],
        1,
        stdout="",
        stderr="uimd-java: Java 17 JDK was not found.\n",
    )
    monkeypatch.setattr(java_toolchain, "java_resolver_command", lambda: ["uimd-java"])
    monkeypatch.setattr(java_toolchain.subprocess, "run", lambda *args, **kwargs: completed)

    with pytest.raises(FileNotFoundError, match="Java 17 JDK was not found"):
        java_toolchain.resolve_java_home()


def test_gradle_and_distribution_launchers_share_the_sdk_resolver():
    root = Path(__file__).resolve().parents[2]
    gradlew = (root / "java/gradlew").read_text(encoding="utf-8")
    gradlew_bat = (root / "java/gradlew.bat").read_text(encoding="utf-8")
    launcher_plugin = (root / "java/uimd-java-launchers.gradle").read_text(
        encoding="utf-8"
    )
    generator = (root / "cpp/tools/uimd/NativeJavaGenerator.cpp").read_text(
        encoding="utf-8"
    )

    assert 'sh "$APP_HOME/uimd-java" --java-home' in gradlew
    assert 'call "%APP_HOME%uimd-java.bat" --set-env' in gradlew_bat
    assert '$APP_HOME/bin/uimd-java' in launcher_plugin
    assert "%APP_HOME%\\\\bin\\\\uimd-java.bat" in launcher_plugin
    assert "uimd-java-launchers.gradle" in generator
    assert "gradle.ext.uimdRuntimeRoot" in generator

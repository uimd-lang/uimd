#!/usr/bin/env python3
"""Use the SDK-shipped Java 17 resolver from repository development helpers."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_GRADLE_CACHE_DIR_NAME = "uimd-gradle-cache"


def java_resolver_command() -> list[str]:
    """Return the platform command for the canonical SDK Java resolver."""
    if os.name == "nt":
        resolver = ROOT / "java/uimd-java.bat"
        if not resolver.is_file():
            raise FileNotFoundError(f"UIMD Java resolver is missing: {resolver}")
        return [os.environ.get("COMSPEC", "cmd.exe"), "/c", str(resolver)]
    resolver = ROOT / "java/uimd-java"
    if not resolver.is_file():
        raise FileNotFoundError(f"UIMD Java resolver is missing: {resolver}")
    return ["sh", str(resolver)]


def resolve_java_home() -> Path:
    """Resolve and validate the Java 17 JDK through the shipped SDK adapter."""
    probe = subprocess.run(
        [*java_resolver_command(), "--java-home"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        check=False,
    )
    if probe.returncode != 0:
        detail = probe.stderr.strip() or "Java 17 JDK was not found"
        raise FileNotFoundError(detail)
    java_home = Path(probe.stdout.strip())
    java_name = "java.exe" if os.name == "nt" else "java"
    if not (java_home / "bin" / java_name).is_file():
        raise FileNotFoundError(
            f"UIMD Java resolver returned an invalid JDK home: {java_home}"
        )
    return java_home


def java_build_env() -> dict[str, str]:
    """Return the minimal environment needed to build or launch Java targets."""
    java_home = resolve_java_home()
    result = {
        "GRADLE_USER_HOME": os.environ.get("GRADLE_USER_HOME")
        or str(Path(tempfile.gettempdir()) / DEFAULT_GRADLE_CACHE_DIR_NAME),
        "JAVA_HOME": str(java_home),
        "PATH": f"{java_home / 'bin'}{os.pathsep}{os.environ.get('PATH', '')}",
    }
    return result

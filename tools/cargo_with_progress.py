#!/usr/bin/env python3
"""Run Cargo with streamed output and a heartbeat during silent work."""

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import queue
import shlex
import shutil
import subprocess
import sys
import threading
import time
from typing import TextIO


DEFAULT_HEARTBEAT_SECONDS = 10.0
STREAM_END = object()
OUTPUT_MESSAGE = object()
ARTIFACT_MESSAGE = object()
JSON_MESSAGE_FORMAT = "json-render-diagnostics"


def resolve_cargo(explicit: str | None = None) -> str:
    candidates = [
        explicit,
        os.environ.get("CARGO"),
        shutil.which("cargo"),
        str(Path.home() / ".cargo" / "bin" / "cargo"),
    ]
    for candidate in candidates:
        if not candidate:
            continue
        resolved = shutil.which(candidate)
        if resolved:
            return resolved
        path = Path(candidate).expanduser()
        if path.is_file() and os.access(path, os.X_OK):
            return str(path)
    raise FileNotFoundError(
        "cargo was not found; install the official Rust toolchain from "
        "https://www.rust-lang.org/tools/install"
    )


def _read_output(stream: TextIO, messages: queue.Queue[object]) -> None:
    try:
        for line in iter(stream.readline, ""):
            messages.put((OUTPUT_MESSAGE, line))
    finally:
        messages.put((STREAM_END, None))


def _read_cargo_json_output(
    stream: TextIO,
    messages: queue.Queue[object],
) -> None:
    try:
        for line in iter(stream.readline, ""):
            try:
                message = json.loads(line)
            except json.JSONDecodeError:
                messages.put((OUTPUT_MESSAGE, line))
                continue
            if not isinstance(message, dict):
                continue
            if message.get("reason") == "compiler-artifact":
                messages.put((ARTIFACT_MESSAGE, message))
                continue
            if message.get("reason") == "compiler-message":
                diagnostic = message.get("message")
                if isinstance(diagnostic, dict):
                    rendered = diagnostic.get("rendered")
                    if isinstance(rendered, str) and rendered:
                        messages.put((OUTPUT_MESSAGE, rendered))
    finally:
        messages.put((STREAM_END, None))


def _stop_process(process: subprocess.Popen[str]) -> None:
    if process.poll() is None:
        process.terminate()
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait()


def _run_cargo_command(
    command: list[str],
    *,
    heartbeat_seconds: float,
    operation: str,
    parse_json_messages: bool = False,
) -> tuple[int, list[dict[str, object]]]:
    started_at = time.monotonic()
    print(f"==> starting: {shlex.join(command)}", flush=True)
    if parse_json_messages:
        process = subprocess.Popen(
            command,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
        )
        assert process.stdout is not None
        assert process.stderr is not None
        streams = [
            (_read_cargo_json_output, process.stdout),
            (_read_output, process.stderr),
        ]
    else:
        process = subprocess.Popen(
            command,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
        )
        assert process.stdout is not None
        streams = [(_read_output, process.stdout)]

    messages: queue.Queue[object] = queue.Queue()
    for reader_target, stream in streams:
        threading.Thread(
            target=reader_target,
            args=(stream, messages),
            daemon=True,
        ).start()

    last_output_at = started_at
    open_streams = len(streams)
    artifacts: list[dict[str, object]] = []
    try:
        while open_streams:
            try:
                queued = messages.get(timeout=heartbeat_seconds)
            except queue.Empty:
                now = time.monotonic()
                total = int(now - started_at)
                silent = int(now - last_output_at)
                print(
                    f"==> cargo is still running "
                    f"(elapsed {total}s, no output for {silent}s)",
                    flush=True,
                )
                continue
            message_type, payload = queued
            if message_type is STREAM_END:
                open_streams -= 1
                continue
            if message_type is ARTIFACT_MESSAGE:
                if isinstance(payload, dict):
                    artifacts.append(payload)
                continue
            print(str(payload), end="", flush=True)
            last_output_at = time.monotonic()
    except KeyboardInterrupt:
        _stop_process(process)
        print(f"==> {operation} interrupted", file=sys.stderr, flush=True)
        return 130, artifacts

    return_code = process.wait()
    elapsed = int(time.monotonic() - started_at)
    print(
        f"==> {operation} finished with exit code {return_code} after {elapsed}s",
        flush=True,
    )
    return return_code, artifacts


def _split_run_arguments(cargo_args: list[str]) -> tuple[list[str], list[str]]:
    try:
        separator = cargo_args.index("--")
    except ValueError:
        return cargo_args, []
    return cargo_args[:separator], cargo_args[separator + 1 :]


def _prepare_build_arguments(run_args: list[str]) -> list[str]:
    build_args: list[str] = []
    index = 0
    while index < len(run_args):
        argument = run_args[index]
        if argument == "--message-format":
            if index + 1 >= len(run_args):
                raise ValueError("--message-format requires a value")
            index += 2
            continue
        if argument.startswith("--message-format="):
            index += 1
            continue
        build_args.append(argument)
        index += 1
    build_args.append(f"--message-format={JSON_MESSAGE_FORMAT}")
    return build_args


def _option_value(
    arguments: list[str],
    long_name: str,
    short_name: str | None = None,
) -> str | None:
    for index, argument in enumerate(arguments):
        if argument == long_name and index + 1 < len(arguments):
            return arguments[index + 1]
        if argument.startswith(f"{long_name}="):
            return argument.split("=", 1)[1]
        if short_name is None:
            continue
        if argument == short_name and index + 1 < len(arguments):
            return arguments[index + 1]
        if argument.startswith(short_name) and argument != short_name:
            return argument[len(short_name) :]
    return None


def _select_application_artifact(
    artifacts: list[dict[str, object]],
    run_args: list[str],
) -> str:
    requested_binary = _option_value(run_args, "--bin")
    requested_example = _option_value(run_args, "--example")
    candidates: list[tuple[str, str]] = []
    for artifact in artifacts:
        executable = artifact.get("executable")
        target = artifact.get("target")
        if not isinstance(executable, str) or not isinstance(target, dict):
            continue
        name = target.get("name")
        kinds = target.get("kind")
        if not isinstance(name, str) or not isinstance(kinds, list):
            continue
        if requested_binary is not None:
            if requested_binary != name or "bin" not in kinds:
                continue
        elif requested_example is not None:
            if requested_example != name or "example" not in kinds:
                continue
        elif "bin" not in kinds:
            continue
        candidate = (name, executable)
        if candidate not in candidates:
            candidates.append(candidate)

    if not candidates:
        raise RuntimeError(
            "Cargo completed the build but did not report an executable "
            "application artifact"
        )
    if len(candidates) > 1:
        names = ", ".join(name for name, _ in candidates)
        raise RuntimeError(
            "Cargo reported multiple runnable binaries "
            f"({names}); select one with --bin"
        )
    return candidates[0][1]


def _run_interactive_application(
    cargo_command: str,
    cargo_args: list[str],
    heartbeat_seconds: float,
) -> int:
    run_args, application_args = _split_run_arguments(cargo_args[1:])
    try:
        build_args = _prepare_build_arguments(run_args)
    except ValueError as error:
        print(f"error: {error}", file=sys.stderr)
        return 2
    build_command = [cargo_command, "build", *build_args]
    build_code, artifacts = _run_cargo_command(
        build_command,
        heartbeat_seconds=heartbeat_seconds,
        operation="cargo build",
        parse_json_messages=True,
    )
    if build_code != 0:
        return build_code
    try:
        executable = _select_application_artifact(artifacts, run_args)
    except RuntimeError as error:
        print(f"error: {error}", file=sys.stderr)
        return 2

    command = [executable, *application_args]
    started_at = time.monotonic()
    print(f"==> launching interactive: {shlex.join(command)}", flush=True)
    try:
        completed = subprocess.run(command, check=False)
    except KeyboardInterrupt:
        print("==> application interrupted", file=sys.stderr, flush=True)
        return 130
    except OSError as error:
        print(f"error: could not launch {executable}: {error}", file=sys.stderr)
        return 126

    return_code = completed.returncode
    if return_code < 0:
        return_code = 128 - return_code
    elapsed = int(time.monotonic() - started_at)
    print(
        f"==> application finished with exit code {return_code} after {elapsed}s",
        flush=True,
    )
    return return_code


def run_cargo(
    cargo_args: list[str],
    *,
    cargo: str | None = None,
    heartbeat_seconds: float = DEFAULT_HEARTBEAT_SECONDS,
) -> int:
    if not cargo_args:
        print("error: no Cargo arguments were provided", file=sys.stderr)
        return 2
    if heartbeat_seconds <= 0:
        print("error: --heartbeat-seconds must be greater than zero", file=sys.stderr)
        return 2
    try:
        cargo_command = resolve_cargo(cargo)
    except FileNotFoundError as error:
        print(f"error: {error}", file=sys.stderr)
        return 127

    if cargo_args[0] == "run":
        return _run_interactive_application(
            cargo_command,
            cargo_args,
            heartbeat_seconds,
        )
    return _run_cargo_command(
        [cargo_command, *cargo_args],
        heartbeat_seconds=heartbeat_seconds,
        operation="cargo",
    )[0]


def parse_args(argv: list[str]) -> tuple[argparse.Namespace, list[str]]:
    parser = argparse.ArgumentParser(
        description=(
            "Run Cargo while streaming its output and printing a heartbeat "
            "during silent work."
        )
    )
    parser.add_argument(
        "--heartbeat-seconds",
        type=float,
        default=DEFAULT_HEARTBEAT_SECONDS,
        help=f"seconds of silence before each heartbeat (default: {DEFAULT_HEARTBEAT_SECONDS:g})",
    )
    parser.add_argument(
        "--cargo",
        help="explicit Cargo executable; defaults to CARGO, PATH, or ~/.cargo/bin/cargo",
    )
    return parser.parse_known_args(argv)


def main(argv: list[str] | None = None) -> int:
    options, cargo_args = parse_args(sys.argv[1:] if argv is None else argv)
    return run_cargo(
        cargo_args,
        cargo=options.cargo,
        heartbeat_seconds=options.heartbeat_seconds,
    )


if __name__ == "__main__":
    raise SystemExit(main())

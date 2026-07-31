import os
from pathlib import Path
import select
import subprocess
import sys
import tempfile
import time

import pytest

if os.name == "posix":
    import pty


PROJECT_ROOT = Path(__file__).resolve().parents[2]
LAUNCHER = PROJECT_ROOT / "tools" / "cargo_with_progress.py"


def _fake_cargo(directory: Path) -> Path:
    executable = directory / "cargo"
    executable.write_text(
        "\n".join(
            [
                "#!/usr/bin/env python3",
                "import sys",
                "import time",
                "print('fake cargo started', flush=True)",
                "time.sleep(float(sys.argv[1]))",
                "print('fake cargo finished', flush=True)",
                "raise SystemExit(int(sys.argv[2]))",
                "",
            ]
        ),
        encoding="utf-8",
    )
    executable.chmod(0o755)
    return executable


def test_cargo_progress_launcher_streams_heartbeats_and_preserves_exit_code():
    with tempfile.TemporaryDirectory(prefix="uimd-cargo-progress-") as temp:
        fake_cargo = _fake_cargo(Path(temp))
        result = subprocess.run(
            [
                sys.executable,
                str(LAUNCHER),
                "--cargo",
                str(fake_cargo),
                "--heartbeat-seconds",
                "0.05",
                "0.16",
                "7",
            ],
            cwd=PROJECT_ROOT,
            text=True,
            capture_output=True,
            check=False,
            env=os.environ.copy(),
        )

    assert result.returncode == 7
    assert "==> starting:" in result.stdout
    assert "fake cargo started" in result.stdout
    assert "==> cargo is still running" in result.stdout
    assert "fake cargo finished" in result.stdout
    assert "==> cargo finished with exit code 7" in result.stdout


def _fake_interactive_cargo(directory: Path) -> tuple[Path, Path]:
    application = directory / "fake_app"
    application.write_text(
        "\n".join(
            [
                "#!/usr/bin/env python3",
                "import os",
                "import sys",
                "import time",
                "print(",
                "    f'fake app tty stdin={os.isatty(sys.stdin.fileno())} '",
                "    f'stdout={os.isatty(sys.stdout.fileno())}',",
                "    flush=True,",
                ")",
                "time.sleep(0.1)",
                "print('fake app finished', flush=True)",
                "",
            ]
        ),
        encoding="utf-8",
    )
    application.chmod(0o755)

    executable = directory / "cargo"
    executable.write_text(
        "\n".join(
            [
                "#!/usr/bin/env python3",
                "import json",
                "import os",
                "import subprocess",
                "import sys",
                "application = os.environ['FAKE_CARGO_APP']",
                "subcommand = sys.argv[1]",
                "print('fake build progress', file=sys.stderr, flush=True)",
                "if subcommand == 'run':",
                "    raise SystemExit(subprocess.run([application], check=False).returncode)",
                "if subcommand != 'build':",
                "    raise SystemExit(f'unexpected fake Cargo subcommand: {subcommand}')",
                "print(json.dumps({",
                "    'reason': 'compiler-artifact',",
                "    'package_id': 'path+file:///fake#fake_app@0.1.0',",
                "    'manifest_path': '/fake/Cargo.toml',",
                "    'target': {",
                "        'kind': ['bin'],",
                "        'name': 'fake_app',",
                "    },",
                "    'executable': application,",
                "}), flush=True)",
                "print(json.dumps({'reason': 'build-finished', 'success': True}), flush=True)",
                "",
            ]
        ),
        encoding="utf-8",
    )
    executable.chmod(0o755)
    return executable, application


@pytest.mark.skipif(os.name != "posix", reason="POSIX PTY is required")
def test_cargo_run_builds_with_progress_then_launches_application_on_real_tty():
    with tempfile.TemporaryDirectory(prefix="uimd-cargo-run-tty-") as temp:
        fake_cargo, application = _fake_interactive_cargo(Path(temp))
        environment = os.environ.copy()
        environment["FAKE_CARGO_APP"] = str(application)
        master, slave = pty.openpty()
        process = subprocess.Popen(
            [
                sys.executable,
                str(LAUNCHER),
                "--cargo",
                str(fake_cargo),
                "run",
                "--release",
                "--manifest-path",
                str(Path(temp) / "Cargo.toml"),
            ],
            cwd=PROJECT_ROOT,
            stdin=slave,
            stdout=slave,
            stderr=slave,
            env=environment,
            close_fds=True,
        )
        os.close(slave)
        output = bytearray()
        try:
            deadline = time.monotonic() + 5.0
            while process.poll() is None and time.monotonic() < deadline:
                readable, _, _ = select.select([master], [], [], 0.05)
                if readable:
                    try:
                        chunk = os.read(master, 4096)
                    except OSError:
                        break
                    if not chunk:
                        break
                    output.extend(chunk)
            process.wait(timeout=1.0)
            while True:
                readable, _, _ = select.select([master], [], [], 0.05)
                if not readable:
                    break
                try:
                    chunk = os.read(master, 4096)
                except OSError:
                    break
                if not chunk:
                    break
                output.extend(chunk)
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()
            os.close(master)

    rendered = output.decode("utf-8", errors="replace")
    assert process.returncode == 0
    assert "fake build progress" in rendered
    assert "fake app tty stdin=True stdout=True" in rendered
    assert "fake app finished" in rendered

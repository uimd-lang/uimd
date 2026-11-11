#!/usr/bin/env python3
"""Copy the canonical offline Agent Chat fixture into all example ports."""

import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SCENARIO = ROOT / "python/examples/agent_chat/scenario.json"
DATA = json.loads(SCENARIO.read_text())
MESSAGES = DATA["messages"]
assert len(MESSAGES) == 100


def quoted(value):
    return json.dumps(value, ensure_ascii=False)


def entry(message):
    role = {"user": "You", "assistant": "Agent", "tool": "Tool"}[message["role"]]
    return (role, message["kind"], message["text"], message["title"])


def fragments(message):
    if message["kind"] not in {"text", "code"} or len(message["text"]) < 70:
        return [(False, entry(message), message["duration_ms"])]
    value = message["text"]
    # Slice by Unicode character; all ports receive the same completed fragment.
    cuts = (max(1, len(value) // 3), max(2, len(value) * 2 // 3), len(value))
    delays = (message["duration_ms"] * 3 // 10, message["duration_ms"] * 3 // 10)
    delays += (message["duration_ms"] - sum(delays),)
    return [
        (index != 0, (*entry(message)[:2], value[:cut], message["title"]), delays[index])
        for index, cut in enumerate(cuts)
    ]


STEPS = [fragment for message in MESSAGES[1:] for fragment in fragments(message)]
assert sum(not replace for replace, _, _ in STEPS) == 99


def replace_region(path, start, end, replacement):
    content = path.read_text()
    first = content.index(start)
    last = content.index(end, first)
    path.write_text(content[:first] + replacement + content[last:])


def python_code():
    lines = ["# Generated from scenario.json by tools/generate_agent_chat_replay.py."]
    lines.append("INITIAL_ENTRY = " + repr(entry(MESSAGES[0])))
    lines.append("SCRIPTED_STEPS = (")
    for replace, e, _ in STEPS:
        lines.append(f"    ({replace!r}, {e[0]!r}, {e[1]!r}, {e[2]!r}, {e[3]!r}),")
    lines.append(")")
    lines.append("SCRIPTED_DURATIONS = (" + ", ".join(str(ms / 1000) for _, _, ms in STEPS) + ")")
    return "\n".join(lines) + "\n\n"


def cpp_code():
    lines = ["// Generated from python/examples/agent_chat/scenario.json."]
    lines.append("const TranscriptEntry kInitialEntry{" + ", ".join(map(quoted, entry(MESSAGES[0]))) + "};")
    lines.append(f"const std::array<ScriptedStep, {len(STEPS)}> kScriptedSteps{{{{")
    for replace, e, _ in STEPS:
        lines.append("    {" + ("true" if replace else "false") + ", {" + ", ".join(map(quoted, e)) + "}},")
    lines.append("}};")
    lines.append(f"const std::array<double, {len(STEPS)}> kScriptedDurations{{{{")
    lines.extend("    " + str(ms / 1000) + "," for _, _, ms in STEPS)
    lines.append("}};")
    return "\n".join(lines) + "\n\n"


def csharp_code():
    lines = ["    // Generated from python/examples/agent_chat/scenario.json."]
    e = entry(MESSAGES[0])
    lines.append("    private static readonly TranscriptEntry InitialEntry = new(" + ", ".join(map(quoted, e)) + ");")
    lines.append("    private static readonly ScriptedStep[] ScriptedSteps =")
    lines.append("    {")
    for replace, e, _ in STEPS:
        lines.append("        new(" + ("true" if replace else "false") + ", new(" + ", ".join(map(quoted, e)) + ")),")
    lines.append("    };")
    lines.append("    private static readonly double[] ScriptedDurations = {" + ", ".join(str(ms / 1000) for _, _, ms in STEPS) + "};")
    return "\n".join(lines) + "\n\n"


def go_code():
    e = entry(MESSAGES[0])
    lines = ["// Generated from python/examples/agent_chat/scenario.json."]
    lines.append("var initialEntry = transcriptview.Entry{Role: " + quoted(e[0]) + ", Phase: " + quoted(e[1]) + ", Body: " + quoted(e[2]) + ", Detail: " + quoted(e[3]) + "}")
    lines.append("var scriptedSteps = []scriptedStep{")
    for replace, e, _ in STEPS:
        fields = ", ".join(f"{name}: {quoted(value)}" for name, value in zip(("Role", "Phase", "Body", "Detail"), e))
        lines.append("    {" + ("true" if replace else "false") + ", transcriptview.Entry{" + fields + "}},")
    lines.append("}")
    lines.append("var scriptedDurations = []time.Duration{" + ", ".join(f"{ms} * time.Millisecond" for _, _, ms in STEPS) + "}")
    return "\n".join(lines) + "\n\n"


def rust_code():
    lines = ["// Generated from python/examples/agent_chat/scenario.json."]
    def fields(e):
        return ", ".join(f"{name}: {quoted(value)}" for name, value in zip(("role", "phase", "body", "detail"), e))
    lines.append("const INITIAL_ENTRY: TranscriptEntry = TranscriptEntry { " + fields(entry(MESSAGES[0])) + " };")
    lines.append(f"const SCRIPTED_STEPS: [ScriptedStep; {len(STEPS)}] = [")
    for replace, e, _ in STEPS:
        lines.append("    ScriptedStep { replace_last: " + ("true" if replace else "false") + ", entry: TranscriptEntry { " + fields(e) + " } },")
    lines.append("];")
    lines.append(f"const SCRIPTED_DURATIONS: [f64; {len(STEPS)}] = [" + ", ".join(str(ms / 1000) for _, _, ms in STEPS) + "];")
    return "\n".join(lines) + "\n\n"


def swift_code():
    lines = ["// Generated from python/examples/agent_chat/scenario.json."]
    def constructor(e):
        return "TranscriptEntry(" + ", ".join(f"{name}: {quoted(value)}" for name, value in zip(("role", "phase", "body", "detail"), e)) + ")"
    lines.append("private let kInitialEntry = " + constructor(entry(MESSAGES[0])))
    lines.append("private let kScriptedSteps = [")
    for replace, e, _ in STEPS:
        lines.append("    ScriptedStep(replaceLast: " + ("true" if replace else "false") + ", entry: " + constructor(e) + "),")
    lines.append("]")
    lines.append("private let kScriptedDurations: [TimeInterval] = [" + ", ".join(str(ms / 1000) for _, _, ms in STEPS) + "]")
    return "\n".join(lines) + "\n\n"


def java_code():
    lines = ["    // Generated from python/examples/agent_chat/scenario.json."]
    lines.append("    private static final TranscriptEntry INITIAL_ENTRY = new TranscriptEntry(" + ", ".join(map(quoted, entry(MESSAGES[0]))) + ");")
    lines.append("    private static final List<ScriptedStep> SCRIPTED_STEPS = List.of(")
    for index, (replace, e, _) in enumerate(STEPS):
        suffix = "));" if index + 1 == len(STEPS) else "),"
        lines.append("        new ScriptedStep(" + ("true" if replace else "false") + ", new TranscriptEntry(" + ", ".join(map(quoted, e)) + ")" + suffix)
    lines.append("    private static final double[] SCRIPTED_DURATIONS = {" + ", ".join(str(ms / 1000) for _, _, ms in STEPS) + "};")
    return "\n".join(lines) + "\n\n"


def main():
    replace_region(ROOT / "python/examples/agent_chat/agent_chat.py", "# Generated from scenario.json by tools/generate_agent_chat_replay.py.", "STRESS_ENTRY_COUNT =", python_code())
    for port in ("cpp", "csharp", "go", "rust", "swift", "java"):
        path = {
            "cpp": ROOT / "cpp/examples/agent_chat/agent_chat.cpp",
            "csharp": ROOT / "csharp/examples/agent_chat/agent_chat.cs",
            "go": ROOT / "go/examples/agent_chat/agent_chat.go",
            "rust": ROOT / "rust/examples/agent_chat/agent_chat.rs",
            "swift": ROOT / "swift/examples/agent_chat/agent_chat.swift",
            "java": ROOT / "java/examples/agent_chat/AgentChat.java",
        }[port]
        end = {
            "cpp": "constexpr std::size_t kStressEntryCount",
            "csharp": "    private readonly Transcript transcriptPanel",
            "go": "const (\n\tstressEntryCount",
            "rust": "const STRESS_ENTRY_COUNT",
            "swift": "private let kStressEntryCount",
            "java": "    private final Transcript transcriptPanel",
        }[port]
        generated = {
            "cpp": cpp_code,
            "csharp": csharp_code,
            "go": go_code,
            "rust": rust_code,
            "swift": swift_code,
            "java": java_code,
        }[port]
        prefix = "    " if port in ("csharp", "java") else ""
        replace_region(path, prefix + "// Generated from python/examples/agent_chat/scenario.json.", end, generated())


if __name__ == "__main__":
    main()

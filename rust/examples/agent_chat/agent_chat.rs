mod agent_chat_ui;
#[path = "chat_entry/chat_entry_ui.rs"]
mod chat_entry_ui;

use agent_chat_ui::*;
use chat_entry_ui::ChatEntryUI;
use std::time::Instant;

#[derive(Clone, Copy)]
struct TranscriptEntry {
    role: &'static str,
    phase: &'static str,
    body: &'static str,
    detail: &'static str,
}

#[derive(Clone, Copy)]
struct ScriptedStep {
    replace_last: bool,
    entry: TranscriptEntry,
}

const INITIAL_ENTRY: TranscriptEntry = TranscriptEntry {
    role: "You",
    phase: "request",
    body: "Audit the release pipeline and propose a safe fix.\nConstraints: deterministic output, no network, preserve compatibility.",
    detail: "simulated user message",
};

const SCRIPTED_STEPS: [ScriptedStep; 11] = [
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "plan",
            body: "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate",
            detail: "simulated reasoning summary",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Tool",
            phase: "search_code",
            body: "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches",
            detail: "SIMULATED TOOL · no command executed",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "table",
            body: "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |",
            detail: "deterministic comparison table",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "code · python",
            body: "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```",
            detail: "simulated code block",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "stream · 1/3",
            body: "I found the shared invariant. The manifest must reject empty digests",
            detail: "simulated streaming response",
        },
    },
    ScriptedStep {
        replace_last: true,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "stream · 2/3",
            body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.",
            detail: "simulated streaming response",
        },
    },
    ScriptedStep {
        replace_last: true,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "stream · complete",
            body: "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.",
            detail: "simulated streaming response",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Tool",
            phase: "patch",
            body: "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")",
            detail: "SIMULATED PATCH · repository unchanged",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Tool",
            phase: "test failure",
            body: "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed",
            detail: "SIMULATED ERROR · no test executed",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "structured data",
            body: "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}",
            detail: "simulated YAML result",
        },
    },
    ScriptedStep {
        replace_last: false,
        entry: TranscriptEntry {
            role: "Agent",
            phase: "final",
            body: "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small",
            detail: "simulated final answer",
        },
    },
];

const STRESS_ENTRY_COUNT: usize = 5_556;
const STRESS_TARGET_ROWS: usize = 50_000;
const SIMULATION_MESSAGE_TARGET: usize = 100;
const SCRIPTED_MESSAGE_COUNT: usize = 10;
const SIMULATION_EVENT_COUNT: usize =
    SCRIPTED_STEPS.len() + SIMULATION_MESSAGE_TARGET - SCRIPTED_MESSAGE_COUNT;
const SIMULATION_DURATION_SECONDS: f64 = 180.0;
const SIMULATION_STEP_INTERVAL_SECONDS: f64 =
    SIMULATION_DURATION_SECONDS / (SIMULATION_EVENT_COUNT - 1) as f64;

struct AgentChatApp {
    next_step: usize,
    simulation_running: bool,
    next_step_at: Option<f64>,
    clock_origin: Instant,
}

impl AgentChatApp {
    fn new(ui: &mut AgentChatUI) -> Self {
        let mut app = Self {
            next_step: 0,
            simulation_running: false,
            next_step_at: None,
            clock_origin: Instant::now(),
        };
        app.reset_simulation(ui);
        app
    }

    fn with_transcript(ui: &AgentChatUI, operation: impl FnOnce(&mut uimd::GeneratedWindow)) {
        let mut reusable = ui.transcript.borrow_mut();
        let transcript = reusable
            .child_window_mut()
            .expect("generated transcript child must be initialized");
        operation(transcript);
    }

    fn set_entry_window(window: &mut uimd::GeneratedWindow, entry: &TranscriptEntry) {
        for (name, value) in [
            ("role", entry.role),
            ("phase", entry.phase),
            ("body", entry.body),
            ("detail", entry.detail),
        ] {
            window
                .find_direct_element(name)
                .expect("generated chat entry member must exist")
                .borrow_mut()
                .set_text(value);
        }
    }

    fn append_entry(ui: &AgentChatUI, entry: &TranscriptEntry) {
        let mut view = ChatEntryUI::new();
        Self::set_entry_window(&mut view.base, entry);
        Self::with_transcript(ui, |transcript| {
            let index = transcript.scroll_view().children().len();
            transcript.add_named_child_window(format!("transcript.entries[{index}]"), view.base);
        });
    }

    fn append_owned_entry(ui: &AgentChatUI, entry: &OwnedTranscriptEntry) {
        let mut view = ChatEntryUI::new();
        set_owned_entry_window(&mut view.base, entry);
        Self::with_transcript(ui, |transcript| {
            let index = transcript.scroll_view().children().len();
            transcript.add_named_child_window(format!("transcript.entries[{index}]"), view.base);
        });
    }

    fn replace_last_entry(ui: &AgentChatUI, entry: &TranscriptEntry) {
        Self::with_transcript(ui, |transcript| {
            let children = transcript.scroll_view().children();
            let Some(last) = children.last() else {
                let mut view = ChatEntryUI::new();
                Self::set_entry_window(&mut view.base, entry);
                transcript.add_named_child_window("transcript.entries[0]", view.base);
                return;
            };
            let mut reusable = last.borrow_mut();
            let window = reusable
                .child_window_mut()
                .expect("transcript entry child must be initialized");
            Self::set_entry_window(window, entry);
        });
    }

    fn entry_count(ui: &AgentChatUI) -> usize {
        let mut count = 0;
        Self::with_transcript(ui, |transcript| {
            count = transcript.scroll_view().children().len();
        });
        count
    }

    fn message_count_text(count: usize) -> String {
        format!(
            "{count} {}",
            if count == 1 { "message" } else { "messages" }
        )
    }

    fn set_status(ui: &AgentChatUI, prefix: &str) {
        ui.status.borrow_mut().set_text(format!(
            "{prefix} · {}",
            Self::message_count_text(Self::entry_count(ui)),
        ));
    }

    fn apply_step(ui: &AgentChatUI, step: &ScriptedStep) {
        if step.replace_last {
            Self::replace_last_entry(ui, &step.entry);
        } else {
            Self::append_entry(ui, &step.entry);
        }
    }

    fn start_simulation(&mut self, ui: &mut AgentChatUI) {
        self.start_simulation_at(ui, self.clock_origin.elapsed().as_secs_f64());
    }

    fn start_simulation_at(&mut self, ui: &mut AgentChatUI, now: f64) {
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.reset_simulation(ui);
        }
        if self.next_step == 0 {
            self.apply_next_timeline_event(ui);
        }
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.finish_simulation(ui);
            return;
        }
        self.simulation_running = true;
        self.next_step_at = Some(now + SIMULATION_STEP_INTERVAL_SECONDS);
        Self::set_status(ui, "Running");
    }

    fn pause_simulation(&mut self, ui: &AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        Self::set_status(ui, "Paused");
    }

    fn next_step(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        if self.next_step >= SIMULATION_EVENT_COUNT {
            Self::set_status(ui, "Scenario complete");
            return;
        }
        self.apply_next_timeline_event(ui);
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
        let prefix = if self.next_step == SIMULATION_EVENT_COUNT {
            "Scenario complete"
        } else {
            "Step applied"
        };
        Self::set_status(ui, prefix);
    }

    fn reset_simulation(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        Self::with_transcript(ui, |transcript| {
            transcript.clear_children();
            let _ = transcript.scroll_to_top();
        });
        Self::append_entry(ui, &INITIAL_ENTRY);
        self.next_step = 0;
        Self::set_status(ui, "Ready");
    }

    fn load_stress_transcript(&mut self, ui: &mut AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        Self::with_transcript(ui, |transcript| {
            transcript.clear_children();
        });
        for index in 0..STRESS_ENTRY_COUNT {
            let entry = stress_entry(index);
            Self::append_owned_entry(ui, &entry);
        }
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_top();
        });
        self.next_step = SIMULATION_EVENT_COUNT;
        ui.status.borrow_mut().set_text(format!(
            "Stress loaded · {} · ~{STRESS_TARGET_ROWS} rows",
            Self::message_count_text(Self::entry_count(ui)),
        ));
    }

    fn apply_next_timeline_event(&mut self, ui: &AgentChatUI) {
        if self.next_step < SCRIPTED_STEPS.len() {
            Self::apply_step(ui, &SCRIPTED_STEPS[self.next_step]);
        } else {
            let entry = stress_entry(self.next_step - SCRIPTED_STEPS.len());
            Self::append_owned_entry(ui, &entry);
        }
        self.next_step += 1;
    }

    fn finish_simulation(&mut self, ui: &AgentChatUI) {
        self.simulation_running = false;
        self.next_step_at = None;
        Self::set_status(ui, "Scenario complete");
    }

    fn tick_at(&mut self, ui: &AgentChatUI, now: f64) -> bool {
        if !self.simulation_running {
            return false;
        }
        let mut applied = false;
        while self.next_step_at.is_some_and(|deadline| now >= deadline)
            && self.next_step < SIMULATION_EVENT_COUNT
        {
            self.apply_next_timeline_event(ui);
            self.next_step_at = self
                .next_step_at
                .map(|deadline| deadline + SIMULATION_STEP_INTERVAL_SECONDS);
            applied = true;
        }
        if !applied {
            return false;
        }
        Self::with_transcript(ui, |transcript| {
            let _ = transcript.scroll_to_bottom();
        });
        if self.next_step >= SIMULATION_EVENT_COUNT {
            self.finish_simulation(ui);
        } else {
            Self::set_status(ui, "Running");
        }
        true
    }
}

impl AgentChatUIEvents for AgentChatApp {
    fn on_quit_button_click(&mut self, ui: &mut AgentChatUI) {
        ui.base.request_close();
    }

    fn on_start_button_click(&mut self, ui: &mut AgentChatUI) {
        self.start_simulation(ui);
    }

    fn on_pause_button_click(&mut self, ui: &mut AgentChatUI) {
        self.pause_simulation(ui);
    }

    fn on_next_step_button_click(&mut self, ui: &mut AgentChatUI) {
        self.next_step(ui);
    }

    fn on_reset_button_click(&mut self, ui: &mut AgentChatUI) {
        self.reset_simulation(ui);
    }

    fn on_load_stress_button_click(&mut self, ui: &mut AgentChatUI) {
        self.load_stress_transcript(ui);
    }

    fn tick(&mut self, ui: &mut AgentChatUI) -> bool {
        self.tick_at(ui, self.clock_origin.elapsed().as_secs_f64())
    }
}

struct OwnedTranscriptEntry {
    role: &'static str,
    phase: &'static str,
    body: String,
    detail: &'static str,
}

fn stress_entry(index: usize) -> OwnedTranscriptEntry {
    let detail = "SIMULATED STRESS DATA · no external action";
    let line = (index % 997) + 1;
    match index % 8 {
        0 => OwnedTranscriptEntry {
            role: "You",
            phase: "request",
            body: format!(
                "Investigate deterministic case {index}.\n- preserve API\n- compare ports"
            ),
            detail,
        },
        1 => OwnedTranscriptEntry {
            role: "Agent",
            phase: "plan",
            body: format!("## Plan {index}\n1. inspect\n2. reproduce\n3. repair\n4. validate"),
            detail,
        },
        2 => OwnedTranscriptEntry {
            role: "Tool",
            phase: "search_code",
            body: format!(
                "$ search_code case_{index}\nsrc/module.py:{line}: match\nresult: simulated"
            ),
            detail,
        },
        3 => OwnedTranscriptEntry {
            role: "Agent",
            phase: "code",
            body: format!("```python\ndef case_{index}():\n    return {{\"ok\": True}}\n```"),
            detail,
        },
        4 => OwnedTranscriptEntry {
            role: "Tool",
            phase: "diff",
            body: format!("--- before_{index}\n+++ after_{index}\n@@ -1 +1 @@\n-old\n+new"),
            detail,
        },
        5 => OwnedTranscriptEntry {
            role: "Agent",
            phase: "table",
            body: format!("| item | value |\n|------|-------|\n| id | {index} |\n| state | pass |"),
            detail,
        },
        6 => OwnedTranscriptEntry {
            role: "Tool",
            phase: "warning",
            body: format!(
                "WARNING simulated retry {index}\ntrace: worker -> parser -> validator\nno external action"
            ),
            detail,
        },
        _ => OwnedTranscriptEntry {
            role: "Agent",
            phase: "result",
            body: format!("{{\"id\": {index}, \"status\": \"ok\"}}\nUnicode: čau · 東京 · 🙂"),
            detail,
        },
    }
}

fn set_owned_entry_window(window: &mut uimd::GeneratedWindow, entry: &OwnedTranscriptEntry) {
    for (name, value) in [
        ("role", entry.role),
        ("phase", entry.phase),
        ("body", entry.body.as_str()),
        ("detail", entry.detail),
    ] {
        window
            .find_direct_element(name)
            .expect("generated chat entry member must exist")
            .borrow_mut()
            .set_text(value);
    }
}

fn logic_test() {
    let mut ui = AgentChatUI::new();
    let mut app = AgentChatApp::new(&mut ui);
    assert_eq!(AgentChatApp::entry_count(&ui), 1);
    assert_eq!(ui.status.borrow().text(), "Ready · 1 message");
    app.next_step(&mut ui);
    app.start_simulation_at(&mut ui, 0.0);
    assert_eq!(AgentChatApp::entry_count(&ui), 2);
    assert_eq!(ui.status.borrow().text(), "Running · 2 messages");
    app.pause_simulation(&ui);
    assert!(!app.tick_at(&ui, SIMULATION_DURATION_SECONDS));
    assert_eq!(AgentChatApp::entry_count(&ui), 2);
    app.next_step(&mut ui);
    app.start_simulation_at(&mut ui, SIMULATION_DURATION_SECONDS);
    assert!(app.tick_at(&ui, SIMULATION_DURATION_SECONDS * 2.0));
    assert_eq!(AgentChatApp::entry_count(&ui), SIMULATION_MESSAGE_TARGET);
    assert_eq!(
        ui.status.borrow().text(),
        "Scenario complete · 100 messages"
    );
}

fn main() {
    if std::env::args().nth(1).as_deref() == Some("--logic-test") {
        logic_test();
        return;
    }
    let code = uimd::run_generated_app_main(|| {
        let mut ui = AgentChatUI::new();
        let mut app = AgentChatApp::new(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
#![allow(clippy::too_many_lines, dead_code, unused_variables)]

use serde_json::{Map, Value};
#[path = "transcript/transcript_ui.rs"]
mod uimd_dependency_transcript;

pub struct AgentChatUI
{
    pub base: uimd::GeneratedWindow,
    pub title: uimd::LabelRef,
    pub simulation_badge: uimd::LabelRef,
    pub quit_button: uimd::ButtonRef,
    pub start_button: uimd::ButtonRef,
    pub pause_button: uimd::ButtonRef,
    pub next_step_button: uimd::ButtonRef,
    pub reset_button: uimd::ButtonRef,
    pub load_stress_button: uimd::ButtonRef,
    pub transcript: uimd::ReusableElementRef,
    pub status: uimd::LabelRef,
}

impl AgentChatUI
{
    pub fn new() -> Self
    {
        let mut base = uimd::GeneratedWindow::new("Agent Chat Simulation");
        base.set_generated_layout(vec![
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 1,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 65, margin_bottom: 0,
            content: "start_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 10, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "".to_string(), cell_width: 78, cell_height: 1,
            cell_width_mode: "auto".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 76, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "status".to_string(),
            cell_style: uimd::Style::default(),
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 1,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: uimd::EXPANDED, height: 1,
            width_mode: "expanded".to_string(), height_mode: "auto".to_string(),
            chars_width: 34, chars_height: 1,
            margin_right: 43, margin_bottom: 0,
            content: "title".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "transcript".to_string(),
            row: 0, col: 1,
            cell_row: 4, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 5,
            cell_name: "content".to_string(), cell_width: 0, cell_height: 0,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "expanded".to_string(),
            width: uimd::EXPANDED, height: uimd::EXPANDED,
            width_mode: "expanded".to_string(), height_mode: "expanded".to_string(),
            chars_width: 76, chars_height: 5,
            margin_right: 1, margin_bottom: 0,
            content: "transcript".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 14,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 52, margin_bottom: 0,
            content: "pause_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 27,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 16, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 16, chars_height: 1,
            margin_right: 35, margin_bottom: 0,
            content: "next_step_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "label".to_string(),
            row: 0, col: 36,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 22, margin_bottom: 0,
            content: "simulation_badge".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 44,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 12, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 12, chars_height: 1,
            margin_right: 22, margin_bottom: 0,
            content: "reset_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 57,
            cell_row: 2, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "actions".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "load_stress_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#6d28d9")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
        uimd::LayoutItem {
            element_type: "button".to_string(),
            row: 0, col: 57,
            cell_row: 0, cell_col: 0,
            cell_chars_width: 78, cell_chars_height: 1,
            cell_name: "header".to_string(), cell_width: 0, cell_height: 1,
            cell_width_mode: "expanded".to_string(), cell_height_mode: "auto".to_string(),
            width: 20, height: 1,
            width_mode: "auto".to_string(), height_mode: "auto".to_string(),
            chars_width: 20, chars_height: 1,
            margin_right: 1, margin_bottom: 0,
            content: "quit_button".to_string(),
            cell_style: uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    },
            element_style: uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    },
        },
    ]);
        base.set_generated_kind("window");
        base.set_generated_focusable(false);
        base.set_generated_window_style(uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        border_color: Some(uimd::Color::new("transparent")),
        border_width_horizontal: Some(0),
        border_width_vertical: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("dialog_header", uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("window_header", uimd::Style {
        background: Some(uimd::Color::new("#1d314c")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("header", uimd::Style {
        background: Some(uimd::Color::new("#111827")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_generated_named_style("title", uimd::Style {
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("simulation_badge", uimd::Style {
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        base.set_generated_named_style("actions", uimd::Style {
        background: Some(uimd::Color::new("#172033")),
        padding_bottom: Some(1),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(1),
        ..Default::default()
    });
        base.set_generated_named_style("load_stress_button", uimd::Style {
        background: Some(uimd::Color::new("#6d28d9")),
        ..Default::default()
    });
        base.set_generated_named_style("quit_button", uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        ..Default::default()
    });
        base.set_generated_named_style("content", uimd::Style {
        background: Some(uimd::Color::new("#030712")),
        ..Default::default()
    });
        base.set_generated_named_style("transcript", uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    });
        base.set_generated_named_style("status", uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        base.set_mcp_metadata(true, "AgentChat", "# Agent Chat Simulation\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Deterministic simulated agent chat for rich transcript and large ScrollView testing.\"\ntags: [example, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - transcript/transcript\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Agent Chat Simulation\"\n  description: \"Application title.\"\n  expose: false\n\nsimulation_badge:\n  type: label\n  text: \"SIMULATED · OFFLINE\"\n  description: \"Clarifies that the transcript performs no real agent or tool work.\"\n  expose: false\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the Agent Chat Simulation example.\"\n\nstart_button:\n  type: button\n  title: Start\n  description: \"Run one hundred deterministic simulated messages over three minutes.\"\n\npause_button:\n  type: button\n  title: Pause\n  description: \"Pause the timed simulation without discarding progress.\"\n\nnext_step_button:\n  type: button\n  title: \"Next step\"\n  description: \"Apply one deterministic scripted chat step.\"\n\nreset_button:\n  type: button\n  title: Reset\n  description: \"Reset the transcript to its initial message.\"\n\nload_stress_button:\n  type: button\n  title: \"Load 50k\"\n  description: \"Load an approximately fifty-thousand-row deterministic stress transcript.\"\n\ntranscript:\n  type: transcript\n  cpp-class: \"agent_chat_example::Transcript\"\n  cpp-header: transcript/transcript.hpp\n  description: \"Scrollable deterministic simulated agent transcript.\"\n\nstatus:\n  type: label\n  text: \"Ready · 1 message\"\n  description: \"Shows deterministic simulation progress and transcript size.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\n@simulation_badge:\n  color: \"#c4b5fd\"\n  text-align: right\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@load_stress_button:\n  background: \"#6d28d9\"\n  focus-background: \"#7c3aed\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@content:\n  background: \"#030712\"\n@transcript:\n  background: \"#00000000\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n  padding: 0, 1, 0, 1\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**......................... simulation_badge.... quit_button......... |\n+-actions-**-------------------------------------------------------------------+\n| start_button pause_button next_step_button reset_button load_stress_button.. |\n+-content-**-------------------------------------------------------------------+\n| transcript..**.............................................................. |\n| ............................................................................ |\n* *........................................................................... |\n* *........................................................................... |\n| ............................................................................ |\n+------------------------------------------------------------------------------+\n| status..**.................................................................. |\n+------------------------------------------------------------------------------+\n```\n", "Deterministic simulated agent chat for rich transcript and large ScrollView testing.");
        base.set_mcp_element_metadata(vec![uimd::GeneratedElementMetadata::new("title", "Application title.", false), uimd::GeneratedElementMetadata::new("simulation_badge", "Clarifies that the transcript performs no real agent or tool work.", false), uimd::GeneratedElementMetadata::new("quit_button", "Close the Agent Chat Simulation example.", true), uimd::GeneratedElementMetadata::new("start_button", "Run one hundred deterministic simulated messages over three minutes.", true), uimd::GeneratedElementMetadata::new("pause_button", "Pause the timed simulation without discarding progress.", true), uimd::GeneratedElementMetadata::new("next_step_button", "Apply one deterministic scripted chat step.", true), uimd::GeneratedElementMetadata::new("reset_button", "Reset the transcript to its initial message.", true), uimd::GeneratedElementMetadata::new("load_stress_button", "Load an approximately fifty-thousand-row deterministic stress transcript.", true), uimd::GeneratedElementMetadata::new("transcript", "Scrollable deterministic simulated agent transcript.", true), uimd::GeneratedElementMetadata::new("status", "Shows deterministic simulation progress and transcript size.", true)]);
        base.set_mcp_app_tools(vec![]);
        let title = uimd::new_label("title", "Agent Chat Simulation");
        base.add_element(title.clone());
        title.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#f8fafc")),
        text_align: Some("center".to_string()),
        ..Default::default()
    });
        title.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let simulation_badge = uimd::new_label("simulation_badge", "SIMULATED · OFFLINE");
        base.add_element(simulation_badge.clone());
        simulation_badge.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("transparent")),
        color: Some(uimd::Color::new("#c4b5fd")),
        text_align: Some("right".to_string()),
        ..Default::default()
    });
        simulation_badge.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        let quit_button = uimd::new_button("quit_button", "Quit");
        base.add_element(quit_button.clone());
        quit_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#7f1d1d")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        quit_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#dc2626")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        quit_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let start_button = uimd::new_button("start_button", "Start");
        base.add_element(start_button.clone());
        start_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        start_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        start_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let pause_button = uimd::new_button("pause_button", "Pause");
        base.add_element(pause_button.clone());
        pause_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        pause_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        pause_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let next_step_button = uimd::new_button("next_step_button", "Next step");
        base.add_element(next_step_button.clone());
        next_step_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        next_step_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        next_step_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let reset_button = uimd::new_button("reset_button", "Reset");
        base.add_element(reset_button.clone());
        reset_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#334155")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        reset_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#2563eb")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        reset_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let load_stress_button = uimd::new_button("load_stress_button", "Load 50k");
        base.add_element(load_stress_button.clone());
        load_stress_button.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#6d28d9")),
        color: Some(uimd::Color::new("#e2e8f0")),
        ..Default::default()
    });
        load_stress_button.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#7c3aed")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        load_stress_button.borrow_mut().set_disabled_style(uimd::Style {
        background: Some(uimd::Color::new("#1f2937")),
        color: Some(uimd::Color::new("#64748b")),
        ..Default::default()
    });
        let transcript = uimd::new_reusable_element("transcript", "transcript");
        base.add_element(transcript.clone());
        transcript.borrow_mut().set_child_window(uimd_dependency_transcript::TranscriptUI::new().base);
        transcript.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#00000000")),
        ..Default::default()
    });
        transcript.borrow_mut().set_focus_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff14")),
        ..Default::default()
    });
        transcript.borrow_mut().set_edit_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff18")),
        ..Default::default()
    });
        transcript.borrow_mut().set_selected_style(uimd::Style {
        background: Some(uimd::Color::new("#ffffff26")),
        color: Some(uimd::Color::new("#ffffff")),
        ..Default::default()
    });
        let status = uimd::new_label("status", "Ready · 1 message");
        base.add_element(status.clone());
        status.borrow_mut().set_style(uimd::Style {
        background: Some(uimd::Color::new("#0f172a")),
        color: Some(uimd::Color::new("#93c5fd")),
        padding_bottom: Some(0),
        padding_left: Some(1),
        padding_right: Some(1),
        padding_top: Some(0),
        ..Default::default()
    });
        status.borrow_mut().set_cursor_style(uimd::Style {
        background: Some(uimd::Color::new("#b8d7ff")),
        color: Some(uimd::Color::new("#000000")),
        ..Default::default()
    });
        Self
        {
            base,
            title,
            simulation_badge,
            quit_button,
            start_button,
            pause_button,
            next_step_button,
            reset_button,
            load_stress_button,
            transcript,
            status,
        }
    }

    pub fn run<H: AgentChatUIEvents>(&mut self, handler: &mut H, args: &[String]) -> i32
    {
        let options = self.base.runtime_options();
        let mut runtime = AgentChatUIRuntime { ui: self, handler };
        uimd::run_generated_window(&mut runtime, options, args)
    }
}

impl Default for AgentChatUI
{
    fn default() -> Self { Self::new() }
}

pub trait AgentChatUIEvents
{
    fn on_quit_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn on_start_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn on_pause_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn on_next_step_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn on_reset_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn on_load_stress_button_click(&mut self, _ui: &mut AgentChatUI) {}
    fn handle_dynamic_button(&mut self, _ui: &mut AgentChatUI, _name: &str) -> bool { false }
    fn handle_dynamic_text_changed(&mut self, _ui: &mut AgentChatUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut AgentChatUI, _name: &str, _value: &str) -> bool { false }
    fn handle_dynamic_selection_changed(&mut self, _ui: &mut AgentChatUI, _name: &str, _value: &[String]) -> bool { false }
    fn handle_active_window_button(&mut self, _ui: &mut AgentChatUI, _name: &str) -> bool { false }
    fn on_focus_changed(&mut self, _ui: &mut AgentChatUI, _name: &str, _focused: bool) {}
    fn on_preview_key(&mut self, _ui: &mut AgentChatUI, _event: &uimd::KeyEvent) -> bool { false }
    #[deprecated(since = "0.5.4", note = "use on_preview_key; removal in UIMD 0.7.0")]
    fn handle_key_before_focused(&mut self, _ui: &mut AgentChatUI, _key: &str, _name: &str, _edit_mode: bool) -> bool { false }
    fn handle_key(&mut self, _ui: &mut AgentChatUI, _key: &str) -> bool { false }
    fn tick(&mut self, _ui: &mut AgentChatUI) -> bool { false }
    fn on_window_closed(&mut self, _ui: &mut AgentChatUI, _window: uimd::GeneratedWindow) {}
    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }
    fn handle_mcp_tool(&mut self, _ui: &mut AgentChatUI, _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }
}

struct AgentChatUIRuntime<'a, H: AgentChatUIEvents>
{
    ui: &'a mut AgentChatUI,
    handler: &'a mut H,
}

impl<H: AgentChatUIEvents> uimd::GeneratedApplication for AgentChatUIRuntime<'_, H>
{
    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }
    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }
    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }

    fn handle_generated_button(&mut self, name: &str) -> bool
    {
        if name == "quit_button" { self.handler.on_quit_button_click(self.ui); return true; }
        if name == "start_button" { self.handler.on_start_button_click(self.ui); return true; }
        if name == "pause_button" { self.handler.on_pause_button_click(self.ui); return true; }
        if name == "next_step_button" { self.handler.on_next_step_button_click(self.ui); return true; }
        if name == "reset_button" { self.handler.on_reset_button_click(self.ui); return true; }
        if name == "load_stress_button" { self.handler.on_load_stress_button_click(self.ui); return true; }
        self.handler.handle_dynamic_button(self.ui, name)
}

    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_changed(self.ui, name, value)
}

    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool
    {
        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)
}

    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool
    {
        self.handler.handle_dynamic_selection_changed(self.ui, name, value)
    }

    fn handle_focus_changed(&mut self, name: &str, focused: bool) -> bool
    {
        self.handler.on_focus_changed(self.ui, name, focused);
        true
    }

    fn handle_preview_key(&mut self, event: &uimd::KeyEvent) -> bool
    {
        self.handler.on_preview_key(self.ui, event)
    }

    #[allow(deprecated)]
    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool
    {
        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)
    }

    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }
    fn tick(&mut self) -> bool { self.handler.tick(self.ui) }
    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }
    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }
    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }
}

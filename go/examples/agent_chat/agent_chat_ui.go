// Auto-generated UI code for agent_chat - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type AgentChatUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Title *uimd.Label
    SimulationBadge *uimd.Label
    QuitButton *uimd.Button
    StartButton *uimd.Button
    PauseButton *uimd.Button
    NextStepButton *uimd.Button
    ResetButton *uimd.Button
    LoadStressButton *uimd.Button
    Transcript *uimd.ReusableElement
    Status *uimd.Label
}

func NewAgentChatUI() *AgentChatUI {
    ui := &AgentChatUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Agent Chat Simulation")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 65, MarginBottom: 0,
            Content: "start_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 10, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "", CellWidth: 78, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 76, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#93c5fd"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 34, CharsHeight: 1,
            MarginRight: 43, MarginBottom: 0,
            Content: "title",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 4, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 5,
            CellName: "content", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 76, CharsHeight: 5,
            MarginRight: 1, MarginBottom: 0,
            Content: "transcript",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#030712"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#00000000"),
    },
        },
        {
            Row: 0, Col: 14,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 52, MarginBottom: 0,
            Content: "pause_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 27,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 16, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 16, CharsHeight: 1,
            MarginRight: 35, MarginBottom: 0,
            Content: "next_step_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 36,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 22, MarginBottom: 0,
            Content: "simulation_badge",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#c4b5fd"),
        TextAlign: "right",
    },
        },
        {
            Row: 0, Col: 44,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 12, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 12, CharsHeight: 1,
            MarginRight: 22, MarginBottom: 0,
            Content: "reset_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 57,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "actions", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "load_stress_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#172033"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#6d28d9"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 0, Col: 57,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 78, CellCharsHeight: 1,
            CellName: "header", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 20, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 20, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "quit_button",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#030712"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "AgentChat", "# Agent Chat Simulation\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: draft\ndescription: \"Deterministic simulated agent chat for rich transcript and large ScrollView testing.\"\ntags: [example, simulation, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - transcript/transcript\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Agent Chat Simulation\"\n  description: \"Application title.\"\n  expose: false\n\nsimulation_badge:\n  type: label\n  text: \"SIMULATED · OFFLINE\"\n  description: \"Clarifies that the transcript performs no real agent or tool work.\"\n  expose: false\n\nquit_button:\n  type: button\n  title: Quit\n  description: \"Close the Agent Chat Simulation example.\"\n\nstart_button:\n  type: button\n  title: Start\n  description: \"Run one hundred deterministic simulated messages over three minutes.\"\n\npause_button:\n  type: button\n  title: Pause\n  description: \"Pause the timed simulation without discarding progress.\"\n\nnext_step_button:\n  type: button\n  title: \"Next step\"\n  description: \"Apply one deterministic scripted chat step.\"\n\nreset_button:\n  type: button\n  title: Reset\n  description: \"Reset the transcript to its initial message.\"\n\nload_stress_button:\n  type: button\n  title: \"Load 50k\"\n  description: \"Load an approximately fifty-thousand-row deterministic stress transcript.\"\n\ntranscript:\n  type: transcript\n  cpp-class: \"agent_chat_example::Transcript\"\n  cpp-header: transcript/transcript.hpp\n  description: \"Scrollable deterministic simulated agent transcript.\"\n\nstatus:\n  type: label\n  text: \"Ready · 1 message\"\n  description: \"Shows deterministic simulation progress and transcript size.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n  background: \"#030712\"\n@header:\n  background: \"#111827\"\n  padding: 0, 1, 0, 1\n@title:\n  color: \"#f8fafc\"\n  text-align: center\n@simulation_badge:\n  color: \"#c4b5fd\"\n  text-align: right\n@actions:\n  background: \"#172033\"\n  padding: 1, 1, 1, 1\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@load_stress_button:\n  background: \"#6d28d9\"\n  focus-background: \"#7c3aed\"\n@quit_button:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@content:\n  background: \"#030712\"\n@transcript:\n  background: \"#00000000\"\n@status:\n  background: \"#0f172a\"\n  color: \"#93c5fd\"\n  padding: 0, 1, 0, 1\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------+\n| title..**......................... simulation_badge.... quit_button......... |\n+-actions-**-------------------------------------------------------------------+\n| start_button pause_button next_step_button reset_button load_stress_button.. |\n+-content-**-------------------------------------------------------------------+\n| transcript..**.............................................................. |\n| ............................................................................ |\n* *........................................................................... |\n* *........................................................................... |\n| ............................................................................ |\n+------------------------------------------------------------------------------+\n| status..**.................................................................. |\n+------------------------------------------------------------------------------+\n```\n", "Deterministic simulated agent chat for rich transcript and large ScrollView testing.")
    ui.Title = uimd.NewLabel("title", "Agent Chat Simulation")
    ui.AddElement(ui.Title)
    ui.Title.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#f8fafc"),
        TextAlign: "center",
    })
    ui.Title.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.SimulationBadge = uimd.NewLabel("simulation_badge", "SIMULATED · OFFLINE")
    ui.AddElement(ui.SimulationBadge)
    ui.SimulationBadge.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#c4b5fd"),
        TextAlign: "right",
    })
    ui.SimulationBadge.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.QuitButton = uimd.NewButton("quit_button", "Quit")
    ui.AddElement(ui.QuitButton)
    ui.QuitButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.QuitButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.QuitButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.StartButton = uimd.NewButton("start_button", "Start")
    ui.AddElement(ui.StartButton)
    ui.StartButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.StartButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.StartButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.PauseButton = uimd.NewButton("pause_button", "Pause")
    ui.AddElement(ui.PauseButton)
    ui.PauseButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.PauseButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.PauseButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.NextStepButton = uimd.NewButton("next_step_button", "Next step")
    ui.AddElement(ui.NextStepButton)
    ui.NextStepButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.NextStepButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.NextStepButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ResetButton = uimd.NewButton("reset_button", "Reset")
    ui.AddElement(ui.ResetButton)
    ui.ResetButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ResetButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ResetButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.LoadStressButton = uimd.NewButton("load_stress_button", "Load 50k")
    ui.AddElement(ui.LoadStressButton)
    ui.LoadStressButton.SetStyle(uimd.Style{
        Background: uimd.NewColor("#6d28d9"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.LoadStressButton.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#7c3aed"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.LoadStressButton.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Transcript = uimd.NewReusableElement("transcript", "transcript")
    ui.AddElement(ui.Transcript)
    ui.Transcript.SetStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
    })
    ui.Transcript.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Transcript.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Transcript.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Status = uimd.NewLabel("status", "Ready · 1 message")
    ui.AddElement(ui.Status)
    ui.Status.SetStyle(uimd.Style{
        Background: uimd.NewColor("#0f172a"),
        Color: uimd.NewColor("#93c5fd"),
        PaddingBottom: uimd.Int(0),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(0),
    })
    ui.Status.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *AgentChatUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *AgentChatUI) HandleGeneratedButton(name string) bool {
    if name == "quit_button" {
        if handler, ok := ui.eventHandler.(interface{ OnQuitButtonClick() }); ok {
            handler.OnQuitButtonClick()
            return true
        }
        return false
    }
    if name == "start_button" {
        if handler, ok := ui.eventHandler.(interface{ OnStartButtonClick() }); ok {
            handler.OnStartButtonClick()
            return true
        }
        return false
    }
    if name == "pause_button" {
        if handler, ok := ui.eventHandler.(interface{ OnPauseButtonClick() }); ok {
            handler.OnPauseButtonClick()
            return true
        }
        return false
    }
    if name == "next_step_button" {
        if handler, ok := ui.eventHandler.(interface{ OnNextStepButtonClick() }); ok {
            handler.OnNextStepButtonClick()
            return true
        }
        return false
    }
    if name == "reset_button" {
        if handler, ok := ui.eventHandler.(interface{ OnResetButtonClick() }); ok {
            handler.OnResetButtonClick()
            return true
        }
        return false
    }
    if name == "load_stress_button" {
        if handler, ok := ui.eventHandler.(interface{ OnLoadStressButtonClick() }); ok {
            handler.OnLoadStressButtonClick()
            return true
        }
        return false
    }
    return false
}

func (ui *AgentChatUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *AgentChatUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *AgentChatUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}

func (ui *AgentChatUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *AgentChatUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}

#include "agent_chat_ui.hpp"
#include "transcript/transcript.hpp"
#include "transcript/transcript_ui.hpp"

#include <memory>
#include <array>
#include <charconv>
#include <initializer_list>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

namespace
{

ui::AxisDimension dimension(const char* mode, int value)
{
    const std::string text(mode);
    if (text == "fixed")
    {
        return ui::AxisDimension::fixed(value);
    }
    if (text == "expanded")
    {
        return ui::AxisDimension::expanded();
    }
    if (text == "fit-content")
    {
        return ui::AxisDimension::fitContent();
    }
    return ui::AxisDimension{};
}

ui::Style makeStyle(std::initializer_list<std::pair<const char*, const char*>> props)
{
    ui::Style style;
    auto parseInt = [](std::string_view text, int fallback = 0)
    {
        int value = fallback;
        const auto* begin = text.data();
        const auto* end = text.data() + text.size();
        (void)std::from_chars(begin, end, value);
        return value;
    };
    auto setPadding = [&](std::string_view value)
    {
        std::array<int, 4> values{0, 0, 0, 0};
        std::size_t start = 0;
        int count = 0;
        while (start <= value.size() && count < 4)
        {
            const std::size_t comma = value.find(',', start);
            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;
            while (start < end && value[start] == ' ')
            {
                ++start;
            }
            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));
            if (comma == std::string_view::npos)
            {
                break;
            }
            start = comma + 1;
        }
        if (count == 1)
        {
            style.padding = values[0];
            style.paddingTop = values[0];
            style.paddingRight = values[0];
            style.paddingBottom = values[0];
            style.paddingLeft = values[0];
        }
        else if (count == 2)
        {
            style.paddingTop = values[0];
            style.paddingRight = values[1];
            style.paddingBottom = values[0];
            style.paddingLeft = values[1];
        }
        else if (count >= 4)
        {
            style.paddingTop = values[0];
            style.paddingRight = values[1];
            style.paddingBottom = values[2];
            style.paddingLeft = values[3];
        }
    };
    auto setMargin = [&](std::string_view value)
    {
        std::array<int, 4> values{0, 0, 0, 0};
        std::size_t start = 0;
        int count = 0;
        while (start <= value.size() && count < 4)
        {
            const std::size_t comma = value.find(',', start);
            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;
            while (start < end && value[start] == ' ')
            {
                ++start;
            }
            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));
            if (comma == std::string_view::npos)
            {
                break;
            }
            start = comma + 1;
        }
        if (count == 1)
        {
            style.margin = values[0];
            style.marginTop = values[0];
            style.marginRight = values[0];
            style.marginBottom = values[0];
            style.marginLeft = values[0];
        }
        else if (count == 2)
        {
            style.marginTop = values[0];
            style.marginRight = values[1];
            style.marginBottom = values[0];
            style.marginLeft = values[1];
        }
        else if (count >= 4)
        {
            style.marginTop = values[0];
            style.marginRight = values[1];
            style.marginBottom = values[2];
            style.marginLeft = values[3];
        }
    };
    for (const auto& [key, value] : props)
    {
        const std::string prop(key);
        if (prop == "color")
        {
            style.color = ui::Color(value);
        }
        else if (prop == "background")
        {
            style.background = ui::Color(value);
        }
        else if (prop == "background-texture")
        {
            style.backgroundTexture = value;
        }
        else if (prop == "background-texture-color")
        {
            style.backgroundTextureColor = ui::Color(value);
        }
        else if (prop == "scope-dim-background")
        {
            style.scopeDimBackground = ui::Color(value);
        }
        else if (prop == "border-color")
        {
            style.borderColor = ui::Color(value);
        }
        else if (prop == "border-width")
        {
            style.borderWidthHorizontal = parseInt(value);
            style.borderWidthVertical = parseInt(value);
        }
        else if (prop == "border-width-horizontal")
        {
            style.borderWidthHorizontal = parseInt(value);
        }
        else if (prop == "border-width-vertical")
        {
            style.borderWidthVertical = parseInt(value);
        }
        else if (prop == "padding")
        {
            setPadding(value);
        }
        else if (prop == "margin")
        {
            setMargin(value);
        }
        else if (prop == "gap")
        {
            style.gap = parseInt(value);
        }
        else if (prop == "text-align")
        {
            style.textAlign = value;
        }
        else if (prop == "user-select")
        {
            style.userSelect = value;
        }
        else if (prop == "scroll-x")
        {
            style.scrollX = (std::string_view(value) == "true" || std::string_view(value) == "1" || std::string_view(value) == "yes");
        }
        else if (prop == "scroll-y")
        {
            style.scrollY = (std::string_view(value) == "true" || std::string_view(value) == "1" || std::string_view(value) == "yes");
        }
    }
    return style;
}

[[maybe_unused]] ui::TextGradient makeTextGradient(int intervalMs, int step, int segmentSize, std::initializer_list<const char*> colors)
{
    ui::TextGradient gradient;
    gradient.intervalMs = intervalMs;
    gradient.step = step;
    gradient.segmentSize = segmentSize;
    for (const char* color : colors)
    {
        gradient.colors.emplace_back(color);
    }
    return gradient;
}

std::vector<ui::GeneratedLayoutEntry> buildLayout()
{
    return {
        ui::GeneratedLayoutEntry{.name = "start_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 1, 12, 1}, .sourceCell = ui::Rect{2, 0, 78, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 65, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "status", .type = "label", .cellName = "", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{10, 0, 78, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("auto", 78), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{76, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 1, .marginBottom = 0, .elementStyle = makeStyle({{"background", "#0f172a"}, {"color", "#93c5fd"}, {"padding", "0, 1, 0, 1"}})},
        ui::GeneratedLayoutEntry{.name = "title", .type = "label", .cellName = "header", .relative = ui::Rect{0, 1, 536870911, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("expanded", 536870911), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{34, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 43, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"text-align", "center"}})},
        ui::GeneratedLayoutEntry{.name = "transcript", .type = "transcript", .cellName = "content", .relative = ui::Rect{0, 1, 536870911, 536870911}, .sourceCell = ui::Rect{4, 0, 78, 5}, .width = dimension("expanded", 536870911), .height = dimension("expanded", 536870911), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("expanded", 0), .charsSize = ui::Size{76, 5}, .cellCharsSize = ui::Size{78, 5}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#030712"}}), .elementStyle = makeStyle({{"background", "#00000000"}})},
        ui::GeneratedLayoutEntry{.name = "pause_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 14, 12, 1}, .sourceCell = ui::Rect{2, 0, 78, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 52, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "next_step_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 27, 16, 1}, .sourceCell = ui::Rect{2, 0, 78, 1}, .width = dimension("auto", 16), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{16, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 35, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "simulation_badge", .type = "label", .cellName = "header", .relative = ui::Rect{0, 36, 20, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 22, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "transparent"}, {"color", "#c4b5fd"}, {"text-align", "right"}})},
        ui::GeneratedLayoutEntry{.name = "reset_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 44, 12, 1}, .sourceCell = ui::Rect{2, 0, 78, 1}, .width = dimension("auto", 12), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{12, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 22, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "load_stress_button", .type = "button", .cellName = "actions", .relative = ui::Rect{0, 57, 20, 1}, .sourceCell = ui::Rect{2, 0, 78, 1}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#172033"}, {"padding", "1, 1, 1, 1"}}), .elementStyle = makeStyle({{"background", "#6d28d9"}, {"color", "#e2e8f0"}})},
        ui::GeneratedLayoutEntry{.name = "quit_button", .type = "button", .cellName = "header", .relative = ui::Rect{0, 57, 20, 1}, .sourceCell = ui::Rect{0, 0, 78, 1}, .width = dimension("auto", 20), .height = dimension("auto", 1), .cellWidth = dimension("expanded", 0), .cellHeight = dimension("auto", 1), .charsSize = ui::Size{20, 1}, .cellCharsSize = ui::Size{78, 1}, .marginRight = 1, .marginBottom = 0, .cellStyle = makeStyle({{"background", "#111827"}, {"padding", "0, 1, 0, 1"}}), .elementStyle = makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}})},
    };
}

}  // namespace

AgentChatUI::AgentChatUI() : ui::GeneratedWindowBase("Agent Chat Simulation")
{
    setGeneratedLayout(buildLayout());
    setGeneratedFocusable(false);
    setGeneratedKind("window");
    setMcpMetadata(true, "AgentChat", R"UI_MCP_MD(# Agent Chat Simulation

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Deterministic simulated agent chat for rich transcript and large ScrollView testing."
tags: [example, simulation, scrollview]
```

## Definition

```yaml
uses:
  - transcript/transcript
```

## Members

```yaml
title:
  type: label
  text: "Agent Chat Simulation"
  description: "Application title."
  expose: false

simulation_badge:
  type: label
  text: "SIMULATED · OFFLINE"
  description: "Clarifies that the transcript performs no real agent or tool work."
  expose: false

quit_button:
  type: button
  title: Quit
  description: "Close the Agent Chat Simulation example."

start_button:
  type: button
  title: Start
  description: "Run one hundred deterministic simulated messages over three minutes."

pause_button:
  type: button
  title: Pause
  description: "Pause the timed simulation without discarding progress."

next_step_button:
  type: button
  title: "Next step"
  description: "Apply one deterministic scripted chat step."

reset_button:
  type: button
  title: Reset
  description: "Reset the transcript to its initial message."

load_stress_button:
  type: button
  title: "Load 50k"
  description: "Load an approximately fifty-thousand-row deterministic stress transcript."

transcript:
  type: transcript
  cpp-class: "agent_chat_example::Transcript"
  cpp-header: transcript/transcript.hpp
  description: "Scrollable deterministic simulated agent transcript."

status:
  type: label
  text: "Ready · 1 message"
  description: "Shows deterministic simulation progress and transcript size."
```

## Style

```yaml
include: dark
this:
  border-width: 0
  background: "#030712"
@header:
  background: "#111827"
  padding: 0, 1, 0, 1
@title:
  color: "#f8fafc"
  text-align: center
@simulation_badge:
  color: "#c4b5fd"
  text-align: right
@actions:
  background: "#172033"
  padding: 1, 1, 1, 1
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@load_stress_button:
  background: "#6d28d9"
  focus-background: "#7c3aed"
@quit_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@content:
  background: "#030712"
@transcript:
  background: "#00000000"
@status:
  background: "#0f172a"
  color: "#93c5fd"
  padding: 0, 1, 0, 1
```

## User Interface

```ui
+-header-**--------------------------------------------------------------------+
| title..**......................... simulation_badge.... quit_button......... |
+-actions-**-------------------------------------------------------------------+
| start_button pause_button next_step_button reset_button load_stress_button.. |
+-content-**-------------------------------------------------------------------+
| transcript..**.............................................................. |
| ............................................................................ |
* *........................................................................... |
* *........................................................................... |
| ............................................................................ |
+------------------------------------------------------------------------------+
| status..**.................................................................. |
+------------------------------------------------------------------------------+
```
)UI_MCP_MD", "Deterministic simulated agent chat for rich transcript and large ScrollView testing.", std::vector<ui::GeneratedElementMetadata>{ui::GeneratedElementMetadata{"title", "Application title.", false}, ui::GeneratedElementMetadata{"simulation_badge", "Clarifies that the transcript performs no real agent or tool work.", false}, ui::GeneratedElementMetadata{"quit_button", "Close the Agent Chat Simulation example.", true}, ui::GeneratedElementMetadata{"start_button", "Run one hundred deterministic simulated messages over three minutes.", true}, ui::GeneratedElementMetadata{"pause_button", "Pause the timed simulation without discarding progress.", true}, ui::GeneratedElementMetadata{"next_step_button", "Apply one deterministic scripted chat step.", true}, ui::GeneratedElementMetadata{"reset_button", "Reset the transcript to its initial message.", true}, ui::GeneratedElementMetadata{"load_stress_button", "Load an approximately fifty-thousand-row deterministic stress transcript.", true}, ui::GeneratedElementMetadata{"transcript", "Scrollable deterministic simulated agent transcript.", true}, ui::GeneratedElementMetadata{"status", "Shows deterministic simulation progress and transcript size.", true}});
    setMcpAppTools(std::vector<ui::GeneratedAppToolMetadata>{});
    setGeneratedWindowStyle(makeStyle({{"background", "#030712"}, {"border-color", "transparent"}, {"border-width", "0"}}));
    title = &static_cast<ui::Label&>(addElement<ui::Label>("title", "Agent Chat Simulation"));
    title->setStyle(makeStyle({{"background", "transparent"}, {"color", "#f8fafc"}, {"text-align", "center"}}));
    title->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    simulation_badge = &static_cast<ui::Label&>(addElement<ui::Label>("simulation_badge", "SIMULATED · OFFLINE"));
    simulation_badge->setStyle(makeStyle({{"background", "transparent"}, {"color", "#c4b5fd"}, {"text-align", "right"}}));
    simulation_badge->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
    quit_button = &static_cast<ui::Button&>(addElement<ui::Button>("quit_button", "Quit"));
    quit_button->setStyle(makeStyle({{"background", "#7f1d1d"}, {"color", "#e2e8f0"}}));
    quit_button->setFocusStyle(makeStyle({{"background", "#dc2626"}, {"color", "#ffffff"}}));
    quit_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    start_button = &static_cast<ui::Button&>(addElement<ui::Button>("start_button", "Start"));
    start_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    start_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    start_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    pause_button = &static_cast<ui::Button&>(addElement<ui::Button>("pause_button", "Pause"));
    pause_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    pause_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    pause_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    next_step_button = &static_cast<ui::Button&>(addElement<ui::Button>("next_step_button", "Next step"));
    next_step_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    next_step_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    next_step_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    reset_button = &static_cast<ui::Button&>(addElement<ui::Button>("reset_button", "Reset"));
    reset_button->setStyle(makeStyle({{"background", "#334155"}, {"color", "#e2e8f0"}}));
    reset_button->setFocusStyle(makeStyle({{"background", "#2563eb"}, {"color", "#ffffff"}}));
    reset_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    load_stress_button = &static_cast<ui::Button&>(addElement<ui::Button>("load_stress_button", "Load 50k"));
    load_stress_button->setStyle(makeStyle({{"background", "#6d28d9"}, {"color", "#e2e8f0"}}));
    load_stress_button->setFocusStyle(makeStyle({{"background", "#7c3aed"}, {"color", "#ffffff"}}));
    load_stress_button->setDisabledStyle(makeStyle({{"background", "#1f2937"}, {"color", "#64748b"}}));
    transcript = &static_cast<ui::ReusableElement&>(addElement<ui::ReusableElement>("transcript"));
    transcript->setChild(std::make_unique<agent_chat_example::Transcript>());
    transcript->setStyle(makeStyle({{"background", "#00000000"}}));
    transcript->setFocusStyle(makeStyle({{"background", "#ffffff14"}}));
    transcript->setEditStyle(makeStyle({{"background", "#ffffff18"}}));
    transcript->setSelectedStyle(makeStyle({{"background", "#ffffff26"}, {"color", "#ffffff"}}));
    status = &static_cast<ui::Label&>(addElement<ui::Label>("status", "Ready · 1 message"));
    status->setStyle(makeStyle({{"background", "#0f172a"}, {"color", "#93c5fd"}, {"padding", "0, 1, 0, 1"}}));
    status->setCursorStyle(makeStyle({{"background", "#b8d7ff"}, {"color", "#000000"}}));
}

void AgentChatUI::onQuitButtonClick()
{
}

void AgentChatUI::onStartButtonClick()
{
}

void AgentChatUI::onPauseButtonClick()
{
}

void AgentChatUI::onNextStepButtonClick()
{
}

void AgentChatUI::onResetButtonClick()
{
}

void AgentChatUI::onLoadStressButtonClick()
{
}

bool AgentChatUI::shouldClose() const
{
    return false;
}

bool AgentChatUI::handleGeneratedButton(std::string_view name)
{
    (void)name;
    if (name == "quit_button")
    {
        onQuitButtonClick();
        return true;
    }
    else if (name == "start_button")
    {
        onStartButtonClick();
        return true;
    }
    else if (name == "pause_button")
    {
        onPauseButtonClick();
        return true;
    }
    else if (name == "next_step_button")
    {
        onNextStepButtonClick();
        return true;
    }
    else if (name == "reset_button")
    {
        onResetButtonClick();
        return true;
    }
    else if (name == "load_stress_button")
    {
        onLoadStressButtonClick();
        return true;
    }
    return false;
}

bool AgentChatUI::handleGeneratedTextChanged(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool AgentChatUI::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)
{
    (void)name;
    (void)value;
    return false;
}

bool AgentChatUI::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)
{
    (void)name;
    (void)value;
    return false;
}

ui::GeneratedWindowRuntimeOptions AgentChatUI::runtimeOptions()
{
    ui::GeneratedWindowRuntimeOptions options;
    options.onButton = [this](std::string_view name)
    {
        if (name == "quit_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "start_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "pause_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "next_step_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "reset_button")
        {
            (void)handleGeneratedButton(name);
        }
        else if (name == "load_stress_button")
        {
            (void)handleGeneratedButton(name);
        }
    };

    options.shouldClose = [this]
    {
        return shouldClose();
    };

    return options;
}

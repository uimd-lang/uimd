#include "agent_chat_ui.hpp"
#include "transcript/transcript.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

using agent_chat_example::TranscriptEntry;

struct ScriptedStep
{
    bool replaceLast;
    TranscriptEntry entry;
};

const TranscriptEntry kInitialEntry{
    "You",
    "request",
    "Audit the release pipeline and propose a safe fix.\n"
    "Constraints: deterministic output, no network, preserve compatibility.",
    "simulated user message",
};

const std::array<ScriptedStep, 11> kScriptedSteps{{
    {false, {"Agent", "plan", "## Plan\n1. Inspect the failing path\n2. Compare all language ports\n3. Add focused tests\n4. Run the parity gate", "simulated reasoning summary"}},
    {false, {"Tool", "search_code", "$ search_code query=release_manifest\nsrc/release/manifest.py:42 validate_manifest\ncpp/tools/release.cpp:88 validateManifest\nresult: 2 matches", "SIMULATED TOOL · no command executed"}},
    {false, {"Agent", "table", "| Port   | Owner                  | State |\n|--------|------------------------|-------|\n| Python | validate_manifest      | ref   |\n| C++    | validateManifest       | drift |", "deterministic comparison table"}},
    {false, {"Agent", "code · python", "```python\ndef validate(entry):\n    if not entry.digest:\n        raise ValueError(\"missing digest\")\n    return entry\n```", "simulated code block"}},
    {false, {"Agent", "stream · 1/3", "I found the shared invariant. The manifest must reject empty digests", "simulated streaming response"}},
    {true, {"Agent", "stream · 2/3", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.", "simulated streaming response"}},
    {true, {"Agent", "stream · complete", "I found the shared invariant. The manifest must reject empty digests\nbefore platform-specific packaging starts, while preserving old inputs.\nNext I will apply the same transition in each affected port.", "simulated streaming response"}},
    {false, {"Tool", "patch", "--- a/src/release/manifest.py\n+++ b/src/release/manifest.py\n@@ -41,2 +41,4 @@\n+ if not entry.digest:\n+     raise ValueError(\"missing digest\")", "SIMULATED PATCH · repository unchanged"}},
    {false, {"Tool", "test failure", "ERROR test_manifest_rejects_empty_digest\nAssertionError: expected 'missing digest', got 'invalid manifest'\n  at tests/test_manifest.py:117\nstatus: 1 failed, 23 passed", "SIMULATED ERROR · no test executed"}},
    {false, {"Agent", "structured data", "result:\n  status: fixed\n  ports: [python, cpp, csharp, go, rust, swift, java]\n  compatibility: preserved\n  evidence: {focused: 24, compare: 7}", "simulated YAML result"}},
    {false, {"Agent", "final", "### Done ✓\n- [x] Focused regression\n- [x] 1:1 parity audit\n- [x] Backward compatibility\nUnicode: ľščťžýáíé · 東京 · 🙂\nOption: keep patch small", "simulated final answer"}},
}};

constexpr std::size_t kStressEntryCount = 5'556;
constexpr std::size_t kStressTargetRows = 50'000;
constexpr std::size_t kSimulationMessageTarget = 100;
constexpr std::size_t kScriptedMessageCount = 10;
constexpr std::size_t kSimulationEventCount =
    kScriptedSteps.size() + kSimulationMessageTarget - kScriptedMessageCount;
constexpr auto kSimulationDuration = std::chrono::seconds{180};
constexpr auto kSimulationStepInterval =
    std::chrono::duration<double>{kSimulationDuration} /
    static_cast<double>(kSimulationEventCount - 1);

[[nodiscard]] TranscriptEntry stressEntry(std::size_t index)
{
    const std::string number = std::to_string(index);
    const std::string line = std::to_string((index % 997) + 1);
    switch (index % 8)
    {
    case 0:
        return {"You", "request", "Investigate deterministic case " + number + ".\n- preserve API\n- compare ports", "SIMULATED STRESS DATA · no external action"};
    case 1:
        return {"Agent", "plan", "## Plan " + number + "\n1. inspect\n2. reproduce\n3. repair\n4. validate", "SIMULATED STRESS DATA · no external action"};
    case 2:
        return {"Tool", "search_code", "$ search_code case_" + number + "\nsrc/module.py:" + line + ": match\nresult: simulated", "SIMULATED STRESS DATA · no external action"};
    case 3:
        return {"Agent", "code", "```python\ndef case_" + number + "():\n    return {\"ok\": True}\n```", "SIMULATED STRESS DATA · no external action"};
    case 4:
        return {"Tool", "diff", "--- before_" + number + "\n+++ after_" + number + "\n@@ -1 +1 @@\n-old\n+new", "SIMULATED STRESS DATA · no external action"};
    case 5:
        return {"Agent", "table", "| item | value |\n|------|-------|\n| id | " + number + " |\n| state | pass |", "SIMULATED STRESS DATA · no external action"};
    case 6:
        return {"Tool", "warning", "WARNING simulated retry " + number + "\ntrace: worker -> parser -> validator\nno external action", "SIMULATED STRESS DATA · no external action"};
    default:
        return {"Agent", "result", "{\"id\": " + number + ", \"status\": \"ok\"}\nUnicode: čau · 東京 · 🙂", "SIMULATED STRESS DATA · no external action"};
    }
}

[[nodiscard]] std::string messageCountText(std::size_t count)
{
    return std::to_string(count) + (count == 1 ? " message" : " messages");
}

class AgentChatApp final : public AgentChatUI
{
public:
    using Clock = std::chrono::steady_clock;
    using Now = std::function<Clock::time_point()>;

    explicit AgentChatApp(Now now = [] { return Clock::now(); })
        : now_(std::move(now))
    {
        transcript_ = dynamic_cast<agent_chat_example::Transcript*>(transcript->child());
        assert(transcript_ != nullptr);
        resetSimulation();
    }

    void startSimulation()
    {
        if (nextStep_ >= kSimulationEventCount)
        {
            resetSimulation();
        }
        if (nextStep_ == 0)
        {
            applyNextTimelineEvent();
        }
        (void)transcript_->scrollToBottom();
        if (nextStep_ >= kSimulationEventCount)
        {
            finishSimulation();
            return;
        }
        simulationRunning_ = true;
        nextStepAt_ = now_() + std::chrono::duration_cast<Clock::duration>(kSimulationStepInterval);
        setStatus("Running");
    }

    void pauseSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        setStatus("Paused");
    }

    void nextStep()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        if (nextStep_ >= kSimulationEventCount)
        {
            setStatus("Scenario complete");
            return;
        }
        applyNextTimelineEvent();
        (void)transcript_->scrollToBottom();
        setStatus(nextStep_ == kSimulationEventCount ? "Scenario complete" : "Step applied");
    }

    void resetSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        transcript_->clearEntries();
        transcript_->appendEntry(kInitialEntry);
        (void)transcript_->scrollToTop();
        nextStep_ = 0;
        setStatus("Ready");
    }

    void loadStressTranscript()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        transcript_->clearEntries();
        for (std::size_t index = 0; index < kStressEntryCount; ++index)
        {
            transcript_->appendEntry(stressEntry(index));
        }
        (void)transcript_->scrollToTop();
        nextStep_ = kSimulationEventCount;
        status->setText(
            "Stress loaded · " + messageCountText(transcript_->entryCount()) + " · ~" +
            std::to_string(kStressTargetRows) + " rows");
    }

    [[nodiscard]] std::size_t entryCount() const
    {
        return transcript_->entryCount();
    }

    bool tick() override
    {
        if (!simulationRunning_ || !nextStepAt_.has_value())
        {
            return false;
        }
        const Clock::time_point now = now_();
        bool applied = false;
        while (now >= *nextStepAt_ && nextStep_ < kSimulationEventCount)
        {
            applyNextTimelineEvent();
            *nextStepAt_ += std::chrono::duration_cast<Clock::duration>(kSimulationStepInterval);
            applied = true;
        }
        if (!applied)
        {
            return false;
        }
        (void)transcript_->scrollToBottom();
        if (nextStep_ >= kSimulationEventCount)
        {
            finishSimulation();
        }
        else
        {
            setStatus("Running");
        }
        return true;
    }

protected:
    void onStartButtonClick() override
    {
        startSimulation();
    }

    void onPauseButtonClick() override
    {
        pauseSimulation();
    }

    void onNextStepButtonClick() override
    {
        nextStep();
    }

    void onResetButtonClick() override
    {
        resetSimulation();
    }

    void onLoadStressButtonClick() override
    {
        loadStressTranscript();
    }

    void onQuitButtonClick() override
    {
        quitRequested_ = true;
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    void applyStep(const ScriptedStep& step)
    {
        if (step.replaceLast)
        {
            transcript_->replaceLastEntry(step.entry);
        }
        else
        {
            transcript_->appendEntry(step.entry);
        }
    }

    void applyNextTimelineEvent()
    {
        if (nextStep_ < kScriptedSteps.size())
        {
            applyStep(kScriptedSteps[nextStep_]);
        }
        else
        {
            transcript_->appendEntry(stressEntry(nextStep_ - kScriptedSteps.size()));
        }
        ++nextStep_;
    }

    void finishSimulation()
    {
        simulationRunning_ = false;
        nextStepAt_.reset();
        setStatus("Scenario complete");
    }

    void setStatus(std::string_view prefix)
    {
        status->setText(std::string{prefix} + " · " + messageCountText(transcript_->entryCount()));
    }

    agent_chat_example::Transcript* transcript_ = nullptr;
    Now now_;
    std::size_t nextStep_ = 0;
    bool simulationRunning_ = false;
    std::optional<Clock::time_point> nextStepAt_;
    bool quitRequested_ = false;
};

void smoke()
{
    AgentChatUI window;
    assert(window.start_button != nullptr);
    assert(window.pause_button != nullptr);
    assert(window.next_step_button != nullptr);
    assert(window.reset_button != nullptr);
    assert(window.load_stress_button != nullptr);
    assert(window.transcript != nullptr);
    assert(window.status != nullptr);
}

void logicTest()
{
    constexpr ui::Size kViewport{90, 35};
    constexpr int kScrollUpLines = 10;
    AgentChatApp::Clock::time_point now{};
    AgentChatApp app([&now] { return now; });
    assert(app.entryCount() == 1);
    assert(app.status->text() == "Ready · 1 message");
    app.nextStep();
    assert(app.entryCount() == 2);
    app.startSimulation();
    assert(app.entryCount() == 2);
    assert(app.status->text() == "Running · 2 messages");
    app.pauseSimulation();
    now += kSimulationDuration;
    assert(!app.tick());
    assert(app.entryCount() == 2);
    app.nextStep();
    assert(app.entryCount() == 3);
    app.startSimulation();
    now += kSimulationDuration;
    assert(app.tick());
    assert(app.entryCount() == kSimulationMessageTarget);
    assert(app.status->text() == "Scenario complete · 100 messages");
    (void)ui::renderGeneratedWindowContent(app, kViewport);
    auto* transcript = dynamic_cast<agent_chat_example::Transcript*>(app.transcript->child());
    assert(transcript != nullptr);
    const ui::Rect transcriptFrame = transcript->scrollView().frame();
    assert(transcriptFrame.width > 0);
    assert(transcriptFrame.height > 0);
    assert(transcript->viewOffset() == 0);
    assert(transcript->scrollView().scrollLines(
        kScrollUpLines,
        ui::Size{transcriptFrame.width, transcriptFrame.height}));
    assert(transcript->viewOffset() > 0);
    app.pauseSimulation();
    assert(app.status->text() == "Paused · 100 messages");
    app.resetSimulation();
    assert(app.entryCount() == 1);
    app.loadStressTranscript();
    assert(app.entryCount() == kStressEntryCount);
    const ui::RenderedContent stressFrame = ui::renderGeneratedWindowContent(app, kViewport);
    assert(transcript->viewOffset() > 0);
    assert(transcript->scrollView().handleWheel(
        -1,
        ui::Size{transcriptFrame.width, transcriptFrame.height}));
    const std::vector<std::string> stressRows = ui::renderedText(stressFrame);
    assert(std::any_of(stressRows.begin(), stressRows.end(), [](const std::string& row)
    {
        return row.find("request") != std::string::npos;
    }));
}

}

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view{argv[1]} == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view{argv[1]} == "--logic-test")
    {
        logicTest();
        return 0;
    }

    AgentChatApp app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

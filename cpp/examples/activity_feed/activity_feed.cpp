#include "activity_feed_ui.hpp"

#include "activity_feed_panel/activity_feed_panel.hpp"
#include "settings/settings.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace
{

constexpr std::string_view kDefaultActivityType = "Info";

struct SampleActivity
{
    std::string_view type;
    std::string_view message;
};

constexpr std::array<SampleActivity, 6> kSampleActivities{{
    {"Info", "Workspace opened"},
    {"Task", "Report export queued"},
    {"Deploy", "Version 1.4 deployed"},
    {"Warning", "Review queue is almost full"},
    {"Note", "Follow-up scheduled"},
    {"Task", "Background sync finished"},
}};

constexpr std::size_t kInitialSampleCount = 2;
constexpr int kHeaderLayoutTestWidth = 112;
constexpr int kHeaderLayoutTestHeight = 38;

[[nodiscard]] std::string trim(std::string_view text)
{
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])))
    {
        ++begin;
    }
    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }
    return std::string{text.substr(begin, end - begin)};
}

[[nodiscard]] std::string currentTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif
    std::ostringstream out;
    out << std::put_time(&localTime, "%H:%M");
    return out.str();
}

[[nodiscard]] std::string selectedComboValue(const ui::ComboBox& comboBox, std::string_view fallback)
{
    const auto& options = comboBox.options();
    const int index = comboBox.selectedIndex();
    if (index < 0 || index >= static_cast<int>(options.size()))
    {
        return std::string{fallback};
    }
    return options[static_cast<std::size_t>(index)];
}

void selectComboValue(ui::ComboBox& comboBox, std::string_view value)
{
    const auto& options = comboBox.options();
    for (std::size_t index = 0; index < options.size(); ++index)
    {
        if (options[index] == value)
        {
            comboBox.setSelectedIndex(static_cast<int>(index));
            return;
        }
    }
    comboBox.setSelectedIndex(0);
}

[[nodiscard]] std::string renderedScrollText(const ui::ScrollView& scrollView, ui::Size size)
{
    std::ostringstream text;
    for (const std::string& row : ui::renderedText(scrollView.render(size)))
    {
        text << row << '\n';
    }
    return text.str();
}

class ActivityFeedApp : public ActivityFeedUI
{
public:
    ActivityFeedApp()
    {
        auto panel = std::make_unique<activity_feed_example::ActivityFeedPanel>();
        feed_ = panel.get();
        feed->setChild(std::move(panel));
    }

    void open()
    {
        selectComboValue(*activity_type, defaultType_);
        const ui::Size contentSize = ui::generatedWindowContentSize(*this);
        (void)ui::renderGeneratedWindowContent(*this, contentSize);
        feed_->setAutoScroll(autoScroll_);
        if (feed_->activityCount() == 0)
        {
            seedInitial();
        }
    }

    void button(std::string_view name)
    {
        if (name == "add_button")
        {
            addCurrentActivity();
        }
        else if (name == "seed_button")
        {
            seedActivities();
        }
        else if (name == "clear_button")
        {
            clearActivities();
        }
        else if (name == "settings_button")
        {
            openSettings();
        }
        else if (name == "quit_button")
        {
            quitRequested_ = true;
        }
    }

    void addCurrentActivity()
    {
        const std::string activityMessage = trim(message->value());
        if (activityMessage.empty())
        {
            status->setText("Enter an activity message.");
            return;
        }
        appendActivity(selectedComboValue(*activity_type, defaultType_), activityMessage);
        message->setValue("");
        message->setCursor(0);
        updateCountStatus();
    }

    void appendActivity(std::string eventType, std::string messageText)
    {
        feed_->appendActivity(currentTimestamp(), std::move(eventType), std::move(messageText), showTimestamps_);
    }

    void seedActivities()
    {
        for (const SampleActivity& activity : kSampleActivities)
        {
            appendActivity(std::string{activity.type}, std::string{activity.message});
        }
        updateCountStatus();
    }

    void clearActivities()
    {
        feed_->clearActivities();
        status->setText("Feed cleared");
    }

    [[nodiscard]] std::size_t activityCount() const
    {
        return feed_->activityCount();
    }

    [[nodiscard]] bool hasModalOverlay() const
    {
        return !modalStack_.empty();
    }

    [[nodiscard]] ui::GeneratedWindowStack& modalStack()
    {
        return modalStack_;
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = ActivityFeedUI::runtimeOptions();
        options.windowStack = &modalStack_;
        return options;
    }

protected:
    void onAddButtonClick() override
    {
        addCurrentActivity();
    }

    void onSeedButtonClick() override
    {
        seedActivities();
    }

    void onClearButtonClick() override
    {
        clearActivities();
    }

    void onSettingsButtonClick() override
    {
        openSettings();
    }

    void onQuitButtonClick() override
    {
        quitRequested_ = true;
    }

    void onMessageSubmit(std::string_view value) override
    {
        message->setValue(std::string{value});
    }

    bool shouldClose() const override
    {
        return quitRequested_;
    }

private:
    void openSettings()
    {
        settingsDialog_.configure(activity_feed_example::SettingsResult{
            .autoScroll = autoScroll_,
            .showTimestamps = showTimestamps_,
            .defaultType = defaultType_,
        });
        settingsDialog_.open();
        ui::GeneratedWindowFrameOptions frame;
        frame.onButton = [&](std::string_view name)
        {
            if (name == "save_button")
            {
                applySettings(settingsDialog_.result());
            }
            else
            {
                status->setText("Settings canceled");
            }
            closeSettings();
        };
        frame.onKeyBeforeFocusedElement = [&](std::string_view keyName, std::string_view, bool editMode)
        {
            if (keyName == "Escape" && !editMode)
            {
                status->setText("Settings canceled");
                closeSettings();
                return true;
            }
            return false;
        };
        modalStack_.push(settingsDialog_.window(), std::move(frame));
    }

    void closeSettings()
    {
        modalStack_.remove(settingsDialog_.window());
        settingsDialog_.close();
    }

    void applySettings(const activity_feed_example::SettingsResult& settings)
    {
        autoScroll_ = settings.autoScroll;
        showTimestamps_ = settings.showTimestamps;
        defaultType_ = settings.defaultType.empty() ? std::string{kDefaultActivityType} : settings.defaultType;
        selectComboValue(*activity_type, defaultType_);
        feed_->setAutoScroll(autoScroll_);
        status->setText("Settings saved");
    }

    void seedInitial()
    {
        const std::size_t count = std::min(kInitialSampleCount, kSampleActivities.size());
        for (std::size_t index = 0; index < count; ++index)
        {
            appendActivity(std::string{kSampleActivities[index].type}, std::string{kSampleActivities[index].message});
        }
        updateCountStatus();
    }

    void updateCountStatus()
    {
        status->setText(std::to_string(feed_->activityCount()) + " activities");
    }

    activity_feed_example::ActivityFeedPanel* feed_ = nullptr;
    activity_feed_example::SettingsDialog settingsDialog_;
    ui::GeneratedWindowStack modalStack_;
    bool autoScroll_ = true;
    bool showTimestamps_ = true;
    bool quitRequested_ = false;
    std::string defaultType_{kDefaultActivityType};
};

void smoke()
{
    ActivityFeedUI window;
    assert(static_cast<const ui::Window&>(window).title() == "Activity Feed");
    assert(window.header_title != nullptr);
    assert(window.settings_button != nullptr);
    assert(window.quit_button != nullptr);
    assert(window.feed != nullptr);
    assert(window.activity_type != nullptr);
    assert(window.message != nullptr);
    assert(window.add_button != nullptr);
    assert(window.seed_button != nullptr);
    assert(window.clear_button != nullptr);
    assert(window.status != nullptr);

    ui::TerminalBuffer buffer{kHeaderLayoutTestWidth, kHeaderLayoutTestHeight};
    ui::renderGeneratedWindow(window, buffer);
    assert(window.header_title->frame().col + window.header_title->frame().width <=
           window.settings_button->frame().col);
    assert(window.settings_button->frame().col + window.settings_button->frame().width <
           window.quit_button->frame().col);
}

void logicTest()
{
    ActivityFeedApp app;
    ActivityFeedUI& window = app;
    app.open();

    assert(app.activityCount() == kInitialSampleCount);
    assert(window.feed->child() != nullptr);
    auto* feedScrollView = window.feed->child()->generatedScrollView();
    assert(feedScrollView != nullptr);
    std::string feedText = renderedScrollText(*feedScrollView, ui::Size{80, 20});
    assert(feedText.find("Workspace opened") != std::string::npos);

    window.message->setValue("Manual event");
    app.button("add_button");
    assert(window.message->value().empty());
    assert(window.status->text() == "3 activities");
    feedText = renderedScrollText(*feedScrollView, ui::Size{80, 20});
    assert(feedText.find("Manual event") != std::string::npos);

    app.seedActivities();
    assert(app.activityCount() == kInitialSampleCount + 1 + kSampleActivities.size());
    feedText = renderedScrollText(*feedScrollView, ui::Size{80, 6});
    assert(feedText.find("Background sync finished") != std::string::npos);

    app.clearActivities();
    assert(app.activityCount() == 0);
    assert(window.status->text() == "Feed cleared");

    app.button("settings_button");
    assert(app.hasModalOverlay());
    app.button("quit_button");
    const ui::GeneratedWindowRuntimeOptions options = app.runtimeOptions();
    assert(options.shouldClose && options.shouldClose());
}

}  // namespace

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        logicTest();
        return 0;
    }

    ActivityFeedApp app;
    app.open();
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

#include "widget_gallery_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

namespace
{

constexpr std::size_t kExpectedLayoutEntries = 18;
constexpr int kResetCount = 3;
constexpr std::string_view kDefaultName = "Ada Lovelace";

std::string summaryText(const WidgetGalleryUI& window)
{
    const auto& themes = window.theme_combo->options();
    const auto& modes = window.mode_list->options();
    const std::string theme = themes.empty() ? "" : themes[static_cast<std::size_t>(window.theme_combo->selectedIndex())];
    const std::string mode = modes.empty() ? "" : modes[static_cast<std::size_t>(window.mode_list->selectedIndex())];
    return "values:\n  name: " + window.name_input->value() +
           "\n  count: " + window.count_input->displayValue() +
           "\n  theme: " + theme +
           "\n  mode: " + mode +
           "\n  enabled: " + std::string(window.enabled_check->checked() ? "True" : "False");
}

void refreshSummary(WidgetGalleryUI& window)
{
    window.spans->setSpans({
        ui::LabelSpan{.text = "SpanLabel", .foreground = "#facc15"},
        ui::LabelSpan{.text = " can mix "},
        ui::LabelSpan{.text = "foreground", .foreground = "#38bdf8"},
        ui::LabelSpan{.text = " and "},
        ui::LabelSpan{.text = "background", .foreground = "#111827", .background = "#f97316"},
        ui::LabelSpan{.text = " colors."},
    });
    window.summary->setText(summaryText(window));
}

void resetValues(WidgetGalleryUI& window)
{
    window.name_input->setValue(std::string(kDefaultName));
    window.count_input->setValue(kResetCount);
    window.enabled_check->setChecked(true);
    window.theme_combo->setSelectedIndex(0);
    window.mode_list->setSelectedIndex(0);
    refreshSummary(window);
}


class WidgetGallery : public WidgetGalleryUI
{
public:
    WidgetGallery()
    {
        refreshSummary(*this);
    }

protected:
    void onResetBtnClick() override
    {
        resetValues(*this);
    }

    void onApplyBtnClick() override
    {
        refreshSummary(*this);
    }

    void onCloseBtnClick() override
    {
        finished_ = true;
    }

    void onNameInputChange(std::string_view) override
    {
    }

    void onCountInputChange(std::string_view) override
    {
    }

    void onEnabledCheckChange(std::string_view) override
    {
    }

    void onThemeComboChange(std::string_view) override
    {
    }

    void onModeListSelectionChange(const std::vector<std::string>&) override
    {
    }

    bool shouldClose() const override
    {
        return finished_;
    }

private:
    bool finished_ = false;
};

void smoke()
{
    WidgetGalleryUI window;
    assert(window.title() == "Widget Gallery");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.header != nullptr);
    assert(window.name_input != nullptr);
    assert(window.count_input != nullptr);
    assert(window.enabled_check != nullptr);
    assert(window.theme_combo != nullptr);
    assert(window.mode_list != nullptr);
    assert(window.summary != nullptr);
    refreshSummary(window);
    assert(window.name_input->value() == kDefaultName);
    assert(window.enabled_check->checked());
    assert(window.theme_combo->selectedIndex() == 0);
    assert(window.mode_list->selectedIndex() == 0);
}

void logicTest()
{
    WidgetGalleryUI window;
    window.name_input->setValue("Grace Hopper");
    window.count_input->setValue(7);
    window.enabled_check->setChecked(false);
    window.theme_combo->setSelectedIndex(1);
    window.mode_list->setSelectedIndex(3);
    refreshSummary(window);
    assert(window.summary->text() == "values:\n  name: Grace Hopper\n  count: 7\n  theme: Light\n  mode: Publish\n  enabled: False");

    resetValues(window);
    assert(window.spans->text() == "SpanLabel can mix foreground and background colors.");
    assert(window.summary->text() == "values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark\n  mode: Preview\n  enabled: True");
}

}  // namespace

int main(int argc, char** argv)
{
    WidgetGallery app;
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

    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

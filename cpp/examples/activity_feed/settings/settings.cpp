#include "settings.hpp"

#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"

#include <cstddef>
#include <utility>

namespace activity_feed_example {

void SettingsDialog::configure(const SettingsResult& settings) {
    auto_scroll->setChecked(settings.autoScroll);
    show_timestamps->setChecked(settings.showTimestamps);
    selectDefaultType(settings.defaultType);
}

void SettingsDialog::open() {
    open_ = true;
}

void SettingsDialog::close() {
    open_ = false;
}

SettingsResult SettingsDialog::result() const {
    return SettingsResult{
        .autoScroll = auto_scroll->checked(),
        .showTimestamps = show_timestamps->checked(),
        .defaultType = selectedDefaultType(),
    };
}

ui::GeneratedWindowBase& SettingsDialog::window() {
    return *this;
}

void SettingsDialog::setOnCancel(std::function<void()> onCancel) {
    onCancel_ = std::move(onCancel);
}

bool SettingsDialog::onPreviewKey(const ui::KeyEvent& event) {
    if (event.key != "Escape" || event.editMode) {
        return false;
    }
    if (onCancel_) {
        onCancel_();
    }
    return true;
}

void SettingsDialog::selectDefaultType(const std::string& value) {
    const auto& options = default_type->options();
    for (std::size_t index = 0; index < options.size(); ++index) {
        if (options[index] == value) {
            default_type->setSelectedIndex(static_cast<int>(index));
            return;
        }
    }
    default_type->setSelectedIndex(0);
}

std::string SettingsDialog::selectedDefaultType() const {
    const auto& options = default_type->options();
    const int index = default_type->selectedIndex();
    if (index < 0 || index >= static_cast<int>(options.size())) {
        return "Info";
    }
    return options[static_cast<std::size_t>(index)];
}

}  // namespace activity_feed_example

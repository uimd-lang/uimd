#include "settings.hpp"

#include "ui/elements/CheckBox.hpp"
#include "ui/elements/ComboBox.hpp"

#include <cstddef>

namespace activity_feed_example {

void SettingsDialog::configure(const SettingsResult& settings) {
    ui_.auto_scroll->setChecked(settings.autoScroll);
    ui_.show_timestamps->setChecked(settings.showTimestamps);
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
        .autoScroll = ui_.auto_scroll->checked(),
        .showTimestamps = ui_.show_timestamps->checked(),
        .defaultType = selectedDefaultType(),
    };
}

ui::GeneratedWindowBase& SettingsDialog::window() {
    return ui_;
}

void SettingsDialog::selectDefaultType(const std::string& value) {
    const auto& options = ui_.default_type->options();
    for (std::size_t index = 0; index < options.size(); ++index) {
        if (options[index] == value) {
            ui_.default_type->setSelectedIndex(static_cast<int>(index));
            return;
        }
    }
    ui_.default_type->setSelectedIndex(0);
}

std::string SettingsDialog::selectedDefaultType() const {
    const auto& options = ui_.default_type->options();
    const int index = ui_.default_type->selectedIndex();
    if (index < 0 || index >= static_cast<int>(options.size())) {
        return "Info";
    }
    return options[static_cast<std::size_t>(index)];
}

}  // namespace activity_feed_example

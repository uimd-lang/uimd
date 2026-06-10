#pragma once

#include "settings_ui.hpp"
#include "ui/generated/GeneratedWindowBase.hpp"

#include <string>

namespace activity_feed_example {

struct SettingsResult {
    bool autoScroll = true;
    bool showTimestamps = true;
    std::string defaultType = "Info";
};

class SettingsDialog {
public:
    void configure(const SettingsResult& settings);
    void open();
    void close();
    [[nodiscard]] SettingsResult result() const;
    [[nodiscard]] bool open() const {
        return open_;
    }
    ui::GeneratedWindowBase& window();

private:
    void selectDefaultType(const std::string& value);
    [[nodiscard]] std::string selectedDefaultType() const;

    SettingsUI ui_;
    bool open_ = false;
};

}  // namespace activity_feed_example

#pragma once

#include "settings_ui.hpp"
#include "ui/generated/GeneratedWindowBase.hpp"

#include <functional>
#include <string>

namespace activity_feed_example {

struct SettingsResult {
    bool autoScroll = true;
    bool showTimestamps = true;
    std::string defaultType = "Info";
};

class SettingsDialog : public SettingsUI {
public:
    void configure(const SettingsResult& settings);
    void open();
    void close();
    [[nodiscard]] SettingsResult result() const;
    [[nodiscard]] bool open() const {
        return open_;
    }
    ui::GeneratedWindowBase& window();
    void setOnCancel(std::function<void()> onCancel);

private:
    bool onPreviewKey(const ui::KeyEvent& event) override;
    void selectDefaultType(const std::string& value);
    [[nodiscard]] std::string selectedDefaultType() const;

    std::function<void()> onCancel_;
    bool open_ = false;
};

}  // namespace activity_feed_example

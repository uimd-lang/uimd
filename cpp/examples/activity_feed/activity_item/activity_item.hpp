#pragma once

#include "ui/core/Style.hpp"
#include "ui/elements/Render.hpp"

#include <string>

namespace activity_feed_example {

struct ActivityItemData {
    std::string timestamp;
    std::string eventType;
    std::string message;
    bool showTimestamp = true;
};

[[nodiscard]] ui::RenderedContent renderActivityItem(const ActivityItemData& item, int width);

}  // namespace activity_feed_example

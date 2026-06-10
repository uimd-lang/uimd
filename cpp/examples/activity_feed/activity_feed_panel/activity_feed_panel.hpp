#pragma once

#include "../activity_item/activity_item.hpp"
#include "activity_feed_panel_ui.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace activity_feed_example {

class ActivityFeedPanel : public ActivityFeedPanelUI {
public:
    ActivityFeedPanel();

    void clearActivities();
    void appendActivity(std::string timestamp, std::string eventType, std::string message, bool showTimestamp);
    void setAutoScroll(bool enabled);
    [[nodiscard]] std::size_t activityCount() const;
    [[nodiscard]] int viewOffset() const;

private:
    [[nodiscard]] int feedWidth() const;
    [[nodiscard]] std::vector<ui::RenderedContent> renderActivities(int width) const;

    std::vector<ActivityItemData> activities_;
};

}  // namespace activity_feed_example

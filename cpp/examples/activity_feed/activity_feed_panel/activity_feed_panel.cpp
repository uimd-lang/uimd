#include "activity_feed_panel.hpp"

#include <algorithm>
#include <utility>

namespace activity_feed_example {

ActivityFeedPanel::ActivityFeedPanel() {
    setDynamicChildrenRenderer([this](int width) {
        return renderActivities(width);
    });
}

void ActivityFeedPanel::clearActivities() {
    activities_.clear();
    clearChildren();
    invalidateDynamicChildren();
}

void ActivityFeedPanel::appendActivity(std::string timestamp,
                                       std::string eventType,
                                       std::string message,
                                       bool showTimestamp) {
    activities_.push_back(ActivityItemData{
        .timestamp = std::move(timestamp),
        .eventType = std::move(eventType),
        .message = std::move(message),
        .showTimestamp = showTimestamp,
    });
    invalidateDynamicChildren();
}

void ActivityFeedPanel::setAutoScroll(bool enabled) {
    scrollView().setAutoScroll(enabled);
}

std::size_t ActivityFeedPanel::activityCount() const {
    return activities_.size();
}

int ActivityFeedPanel::viewOffset() const {
    return scrollView().viewOffset();
}

int ActivityFeedPanel::feedWidth() const {
    return std::max(1, frame().width);
}

std::vector<ui::RenderedContent> ActivityFeedPanel::renderActivities(int width) const {
    std::vector<ui::RenderedContent> rendered;
    rendered.reserve(activities_.size());
    for (const ActivityItemData& activity : activities_) {
        rendered.push_back(renderActivityItem(activity, std::max(1, width)));
    }
    return rendered;
}

}  // namespace activity_feed_example

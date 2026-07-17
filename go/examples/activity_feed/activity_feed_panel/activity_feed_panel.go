package activity_feed_panel

import (
	activityitem "uimd_examples/activity_feed/activity_item"

	uimd "uimd"
)

type ActivityFeedPanel struct {
	*ActivityFeedPanelUI
	Panel             *uimd.ScrollView
	activities        []activityitem.ActivityItemData
	autoScrollEnabled bool
}

func NewActivityFeedPanel() *ActivityFeedPanel {
	panel := &ActivityFeedPanel{ActivityFeedPanelUI: NewActivityFeedPanelUI()}
	panel.Panel = uimd.NewScrollView("panel", 1)
	panel.AddElement(panel.Panel)
	return panel
}

func (panel *ActivityFeedPanel) ClearActivities() {
	panel.activities = nil
	panel.Panel.ClearChildren()
}

func (panel *ActivityFeedPanel) AppendActivity(timestamp string, eventType string, message string, showTimestamp bool) {
	panel.activities = append(panel.activities, activityitem.ActivityItemData{
		Timestamp:     timestamp,
		EventType:     eventType,
		Message:       message,
		ShowTimestamp: showTimestamp,
	})
	panel.rebuildChildren()
	if panel.autoScrollEnabled {
		panel.Panel.ScrollToBottom()
	}
}

func (panel *ActivityFeedPanel) SetAutoScroll(enabled bool) {
	panel.autoScrollEnabled = enabled
	panel.Panel.SetAutoScroll(enabled)
}

func (panel *ActivityFeedPanel) ActivityCount() int {
	return len(panel.activities)
}

func (panel *ActivityFeedPanel) ViewOffset() int {
	return panel.Panel.ViewOffset
}

func (panel *ActivityFeedPanel) rebuildChildren() {
	panel.Panel.ClearChildren()
	for index, activity := range panel.activities {
		element := activityitem.RenderActivityItem(activity, 1)
		element.SetFrame(uimd.Rect{
			Width:  1,
			Height: panel.activityHeight(activity, 1),
		})
		panel.Panel.AddChild(element)
		_ = index
	}
}

func (panel *ActivityFeedPanel) activityHeight(activity activityitem.ActivityItemData, width int) int {
	_ = activity
	_ = width
	return activityitem.ActivityItemDefaultHeight
}

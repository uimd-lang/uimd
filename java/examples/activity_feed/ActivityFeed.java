import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.function.BiConsumer;

import uimd.Color;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.GeneratedWindowRuntime;
import uimd.Size;
import uimd.TerminalCell;

public final class ActivityFeed extends ActivityFeedUI
{
    private static final String DEFAULT_ACTIVITY_TYPE = "Info";
    private static final String TIMESTAMP_ENV = "UIMD_ACTIVITY_FEED_TIMESTAMP";
    private static final int INITIAL_SAMPLE_COUNT = 2;
    private static final DateTimeFormatter TIMESTAMP_FORMATTER =
        DateTimeFormatter.ofPattern("HH:mm", Locale.ROOT);
    private static final List<SampleActivity> SAMPLE_ACTIVITIES = List.of(
        new SampleActivity("Info", "Workspace opened"),
        new SampleActivity("Task", "Report export queued"),
        new SampleActivity("Deploy", "Version 1.4 deployed"),
        new SampleActivity("Warning", "Review queue is almost full"),
        new SampleActivity("Note", "Follow-up scheduled"),
        new SampleActivity("Task", "Background sync finished"));

    private final ActivityFeedPanel feedPanel;
    private boolean autoScroll = true;
    private boolean showTimestamps = true;
    private boolean closeRequested;
    private String defaultType = DEFAULT_ACTIVITY_TYPE;

    public ActivityFeed()
    {
        feedPanel = new ActivityFeedPanel();
        feed.setChild(feedPanel);
    }

    public void open()
    {
        selectActivityType(defaultType);
        GeneratedWindowRuntime.renderGeneratedWindowContent(
            this,
            GeneratedWindowRuntime.generatedWindowContentSize(this),
            -1);
        feedPanel.setAutoScroll(autoScroll);
        if (feedPanel.activityCount() == 0)
        {
            seedInitial();
        }
    }

    @Override
    protected void onAddButtonClick()
    {
        addCurrentActivity();
    }

    @Override
    protected void onSeedButtonClick()
    {
        seedActivities();
    }

    @Override
    protected void onClearButtonClick()
    {
        clearActivities();
    }

    @Override
    protected void onSettingsButtonClick()
    {
        openSettings();
    }

    @Override
    protected void onQuitButtonClick()
    {
        closeRequested = true;
    }

    @Override
    protected void onMessageSubmit(String value)
    {
        message.setValue(value);
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    private void addCurrentActivity()
    {
        String activityMessage = message.value().trim();
        if (activityMessage.isEmpty())
        {
            status.setText("Enter an activity message.");
            return;
        }
        appendActivity(activity_type.selectedText(), activityMessage);
        message.setValue("");
        message.setCursor(0);
        updateCountStatus();
    }

    private void appendActivity(String eventType, String activityMessage)
    {
        feedPanel.appendActivity(
            currentTimestamp(),
            eventType == null || eventType.isEmpty() ? defaultType : eventType,
            activityMessage,
            showTimestamps);
    }

    private void seedActivities()
    {
        for (SampleActivity activity : SAMPLE_ACTIVITIES)
        {
            appendActivity(activity.type(), activity.message());
        }
        updateCountStatus();
    }

    private void clearActivities()
    {
        feedPanel.clearActivities();
        status.setText("Feed cleared");
    }

    private void openSettings()
    {
        SettingsDialog dialog = new SettingsDialog();
        dialog.configure(new SettingsResult(autoScroll, showTimestamps, defaultType));
        dialog.setOnClosed(this::applySettingsResult);
        openWindow(dialog);
    }

    private void applySettingsResult(SettingsResult settings, boolean saved)
    {
        if (!saved)
        {
            status.setText("Settings canceled");
            return;
        }
        autoScroll = settings.autoScroll();
        showTimestamps = settings.showTimestamps();
        defaultType = settings.defaultType() == null || settings.defaultType().isEmpty()
            ? DEFAULT_ACTIVITY_TYPE
            : settings.defaultType();
        selectActivityType(defaultType);
        feedPanel.setAutoScroll(autoScroll);
        status.setText("Settings saved");
    }

    private void seedInitial()
    {
        int count = Math.min(INITIAL_SAMPLE_COUNT, SAMPLE_ACTIVITIES.size());
        for (int index = 0; index < count; ++index)
        {
            SampleActivity activity = SAMPLE_ACTIVITIES.get(index);
            appendActivity(activity.type(), activity.message());
        }
        updateCountStatus();
    }

    private void updateCountStatus()
    {
        status.setText(feedPanel.activityCount() + " activities");
    }

    private void selectActivityType(String value)
    {
        int index = activity_type.options().indexOf(value);
        activity_type.setSelectedIndex(index >= 0 ? index : 0);
    }

    private static String currentTimestamp()
    {
        String fixedTimestamp = System.getenv(TIMESTAMP_ENV);
        if (fixedTimestamp != null && !fixedTimestamp.isEmpty())
        {
            return fixedTimestamp;
        }
        return LocalTime.now().format(TIMESTAMP_FORMATTER);
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            ActivityFeed application = new ActivityFeed();
            application.open();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private record SampleActivity(String type, String message)
    {
    }

    private record SettingsResult(boolean autoScroll, boolean showTimestamps, String defaultType)
    {
    }

    private static final class SettingsDialog extends SettingsUI
    {
        private BiConsumer<SettingsResult, Boolean> onClosed;

        void configure(SettingsResult settings)
        {
            auto_scroll.setChecked(settings.autoScroll());
            show_timestamps.setChecked(settings.showTimestamps());
            int index = default_type.options().indexOf(settings.defaultType());
            default_type.setSelectedIndex(index >= 0 ? index : 0);
        }

        void setOnClosed(BiConsumer<SettingsResult, Boolean> value)
        {
            onClosed = value;
        }

        @Override
        protected void onSaveButtonClick()
        {
            notifyClosed(true);
        }

        @Override
        protected void onCancelButtonClick()
        {
            notifyClosed(false);
        }

        private void notifyClosed(boolean saved)
        {
            if (onClosed != null)
            {
                onClosed.accept(result(), saved);
            }
            closeWindow();
        }

        private SettingsResult result()
        {
            return new SettingsResult(
                auto_scroll.checked(),
                show_timestamps.checked(),
                default_type.selectedText());
        }
    }

    private record ActivityItemData(
        String timestamp,
        String eventType,
        String message,
        boolean showTimestamp)
    {
    }

    private static final class ActivityFeedPanel extends ActivityFeedPanelUI
    {
        private final List<ActivityItemData> activities = new ArrayList<>();
        private boolean autoScrollEnabled;

        ActivityFeedPanel()
        {
            setDynamicChildrenRenderer(this::renderActivities);
        }

        void clearActivities()
        {
            activities.clear();
            clearChildren();
            invalidateDynamicChildren();
        }

        void appendActivity(
            String timestamp,
            String eventType,
            String activityMessage,
            boolean showTimestamp)
        {
            activities.add(new ActivityItemData(
                timestamp,
                eventType,
                activityMessage,
                showTimestamp));
            invalidateDynamicChildren();
            if (autoScrollEnabled)
            {
                scrollToBottom();
            }
        }

        @Override
        public void setAutoScroll(boolean enabled)
        {
            autoScrollEnabled = enabled;
            scrollView().setAutoScroll(enabled);
            if (autoScrollEnabled)
            {
                scrollToBottom();
            }
        }

        int activityCount()
        {
            return activities.size();
        }

        private List<List<List<TerminalCell>>> renderActivities(int width)
        {
            List<List<List<TerminalCell>>> rendered = new ArrayList<>();
            for (ActivityItemData activity : activities)
            {
                rendered.add(renderActivityItem(activity, Math.max(1, width)));
            }
            return rendered;
        }

        private static List<List<TerminalCell>> renderActivityItem(
            ActivityItemData item,
            int width)
        {
            ActivityItemUI view = new ActivityItemUI();
            view.timestamp.setText(item.showTimestamp() ? item.timestamp() : "");
            view.event_type.setText(item.eventType());
            view.message.setText(item.message());

            int sourceHeight = generatedSourceHeight(view);
            GeneratedWindowRuntime.renderGeneratedWindowContent(
                view,
                new Size(Math.max(1, width), sourceHeight),
                -1);
            int messageRows = wrappedLineCount(
                item.message(),
                Math.max(1, view.message.frame().width()));
            List<List<TerminalCell>> rendered =
                GeneratedWindowRuntime.renderGeneratedWindowContent(
                    view,
                    new Size(Math.max(1, width), compactContentHeight(view, messageRows)),
                    -1);
            applyLeftMarker(rendered, markerColor(item.eventType()));
            return rendered;
        }

        private static int generatedSourceHeight(GeneratedWindowBase view)
        {
            int height = 0;
            for (GeneratedLayoutEntry entry : view.generatedLayout())
            {
                height = Math.max(
                    height,
                    entry.sourceCell().row() + entry.sourceCell().height());
            }
            return Math.max(1, height);
        }

        private static int compactContentHeight(GeneratedWindowBase view, int messageRows)
        {
            int height = 1;
            for (GeneratedLayoutEntry entry : view.generatedLayout())
            {
                if (entry.name().isEmpty())
                {
                    continue;
                }
                int entryHeight = "message".equals(entry.name())
                    ? Math.max(1, messageRows)
                    : Math.max(1, entry.charsSize().height());
                height = Math.max(height, entry.relative().row() + entryHeight);
            }
            return height;
        }

        private static int wrappedLineCount(String text, int width)
        {
            int safeWidth = Math.max(1, width);
            int count = 0;
            for (String line : text.split("\\n", -1))
            {
                count += Math.max(1, (line.length() + safeWidth - 1) / safeWidth);
            }
            return Math.max(1, count);
        }

        private static Color markerColor(String eventType)
        {
            return switch (eventType)
            {
                case "Task" -> new Color("#a78bfa");
                case "Warning" -> new Color("#facc15");
                case "Deploy" -> new Color("#22c55e");
                case "Note" -> new Color("#fb7185");
                default -> new Color("#38bdf8");
            };
        }

        private static void applyLeftMarker(
            List<List<TerminalCell>> rendered,
            Color color)
        {
            for (List<TerminalCell> row : rendered)
            {
                if (row.isEmpty())
                {
                    continue;
                }
                TerminalCell cell = row.get(0);
                cell.setText("▐");
                cell.setForeground(color);
            }
        }
    }
}

import java.util.List;

import uimd.GeneratedWindowRuntime;
import uimd.LabelSpan;

public final class WidgetGallery extends WidgetGalleryUI
{
    private static final String DEFAULT_NAME = "Ada Lovelace";
    private static final double DEFAULT_COUNT = 3.0;
    private static final int EXPECTED_LAYOUT_ENTRIES = 18;

    private boolean closeRequested;

    public WidgetGallery()
    {
        refreshSummary();
    }

    @Override
    protected void onResetBtnClick()
    {
        resetValues();
    }

    @Override
    protected void onApplyBtnClick()
    {
        refreshSummary();
    }

    @Override
    protected void onCloseBtnClick()
    {
        closeRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    private void resetValues()
    {
        name_input.setValue(DEFAULT_NAME);
        count_input.setValue(DEFAULT_COUNT);
        enabled_check.setChecked(true);
        theme_combo.setSelectedIndex(0);
        mode_list.setSelectedIndex(0);
        refreshSummary();
    }

    private void refreshSummary()
    {
        spans.setSpans(List.of(
            new LabelSpan("SpanLabel", "#facc15", ""),
            new LabelSpan(" can mix "),
            new LabelSpan("foreground", "#38bdf8", ""),
            new LabelSpan(" and "),
            new LabelSpan("background", "#111827", "#f97316"),
            new LabelSpan(" colors.")));
        summary.setText("values:"
            + "\n  name: " + name_input.value()
            + "\n  count: " + count_input.displayText()
            + "\n  theme: " + theme_combo.selectedText()
            + "\n  mode: " + selectedMode()
            + "\n  enabled: " + (enabled_check.checked() ? "True" : "False"));
    }

    private String selectedMode()
    {
        List<String> selected = mode_list.selectedValues();
        return selected.isEmpty() ? "" : selected.get(0);
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return 0;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return 0;
        }

        WidgetGallery application = new WidgetGallery();
        return GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
    }

    private static void smoke()
    {
        WidgetGallery window = new WidgetGallery();
        require("Widget Gallery".equals(window.title()), "unexpected title");
        require(window.generatedLayout().size() == EXPECTED_LAYOUT_ENTRIES, "unexpected layout size");
        require(DEFAULT_NAME.equals(window.name_input.value()), "unexpected default name");
        require(window.enabled_check.checked(), "feature should start enabled");
        require(window.theme_combo.selectedIndex() == 0, "unexpected default theme");
        require(window.mode_list.selectedIndex() == 0, "unexpected default mode");
    }

    private static void logicTest()
    {
        WidgetGallery window = new WidgetGallery();
        window.name_input.setValue("Grace Hopper");
        window.count_input.setValue(7.0);
        window.enabled_check.setChecked(false);
        window.theme_combo.setSelectedIndex(1);
        window.mode_list.setSelectedIndex(3);
        window.refreshSummary();
        require(("values:\n  name: Grace Hopper\n  count: 7\n  theme: Light"
            + "\n  mode: Publish\n  enabled: False").equals(window.summary.text()),
            "updated summary mismatch");

        window.resetValues();
        require("SpanLabel can mix foreground and background colors.".equals(window.spans.text()),
            "span text mismatch");
        require(("values:\n  name: Ada Lovelace\n  count: 3\n  theme: Dark"
            + "\n  mode: Preview\n  enabled: True").equals(window.summary.text()),
            "reset summary mismatch");
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }
}

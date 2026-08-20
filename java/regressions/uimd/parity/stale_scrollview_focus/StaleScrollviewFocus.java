import java.util.List;
import java.util.function.BiConsumer;
import java.util.function.Consumer;

import uimd.GeneratedScrollViewBase;
import uimd.GeneratedWindowBase;
import uimd.GeneratedWindowRuntime;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.ScrollViewPosition;
import uimd.Size;

public final class StaleScrollviewFocus extends StaleScrollviewFocusUI
{
    private static final int MINIMUM_RENDER_HEIGHT = 1;
    private static final int FIELD_COUNT = 40;
    private static final int PRIMARY_FIELD_INDEX = 0;
    private static final int BODY_FIELD_INDEX = 1;
    private static final int DISPLAY_INDEX_OFFSET = 1;
    private static final int FIELD_NUMBER_PADDING_THRESHOLD = 10;

    private static final List<ReproItem> ITEMS = List.of(
        new ReproItem(
            "Alpha Apartments",
            "First row, visible without scrolling",
            "Alpha item was opened from the top of the ScrollView.\n"
                + "The edit page should not keep any stale list focus background."),
        new ReproItem(
            "Bravo Offices",
            "Second row, used by the automated repro",
            "Bravo item was opened by pressing Enter on a row button.\n"
                + "The old ScrollView page must be gone before this page renders."),
        new ReproItem(
            "Charlie Lofts",
            "Third row, keeps the source list taller",
            "Charlie item keeps the source page closer to the reported list shape.\n"
                + "The opened edit page still owns the new nested ScrollView."),
        new ReproItem(
            "Delta Studios",
            "Fourth row, manual repro filler",
            "Delta item is extra manual test data for scroll and focus traversal.\n"
                + "Activating Open should replace the whole page cleanly."),
        new ReproItem(
            "Echo Retail",
            "Fifth row, enough rows to browse",
            "Echo item makes the source ScrollView visibly list-like.\n"
                + "The edit form below contains many alternating controls."));

    private boolean quitRequested;

    public StaleScrollviewFocus()
    {
        showListPage();
    }

    @Override
    protected void onTabListClick()
    {
        showListPage();
    }

    @Override
    protected void onTabOtherClick()
    {
        hint.setText("Other tab intentionally leaves the list unchanged.");
    }

    @Override
    protected void onQuitBtnClick()
    {
        quitRequested = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return quitRequested;
    }

    private void showListPage()
    {
        page.setView(new ListPage(this::showEditPage));
        hint.setText("Focus list, press Enter, choose Open.");
    }

    private void showEditPage(ReproItem item)
    {
        page.setView(new EditPage(item, this::showListPage));
        hint.setText("Edit page opened from ScrollView row.");
    }

    private static String fieldNumber(int index)
    {
        int displayIndex = index + DISPLAY_INDEX_OFFSET;
        String value = Integer.toString(displayIndex);
        return displayIndex < FIELD_NUMBER_PADDING_THRESHOLD ? "0" + value : value;
    }

    private static String firstLine(String value)
    {
        int lineBreak = value.indexOf('\n');
        return lineBreak < 0 ? value : value.substring(0, lineBreak);
    }

    private static String fieldValue(ReproItem item, int index)
    {
        if (index == PRIMARY_FIELD_INDEX)
        {
            return item.name() + " primary field";
        }
        if (index == BODY_FIELD_INDEX)
        {
            return firstLine(item.body());
        }
        return item.name() + " detail field " + fieldNumber(index);
    }

    private static void addScrollChild(
        GeneratedScrollViewBase owner,
        String name,
        GeneratedWindowBase child)
    {
        child.setMode("expand_width");
        int height = Math.max(
            MINIMUM_RENDER_HEIGHT,
            GeneratedWindowRuntime.generatedWindowContentSize(child).height());
        ReusableElement reusable = new ReusableElement(name, child);
        reusable.setFrame(new Rect(0, 0, 0, height));
        owner.addChild(reusable);
    }

    private record ReproItem(String name, String summary, String body)
    {
    }

    private static final class ItemRow extends ItemRowUI
    {
        private final ReproItem item;
        private final Consumer<ReproItem> openRequested;

        ItemRow(ReproItem item, Consumer<ReproItem> openRequested)
        {
            this.item = item;
            this.openRequested = openRequested;
            name.setText(item.name());
            summary.setText(item.summary());
        }

        @Override
        protected void onOpenBtnClick()
        {
            openRequested.accept(item);
        }
    }

    private static final class ListPage extends ListPageUI
    {
        ListPage(Consumer<ReproItem> openRequested)
        {
            clearChildren();
            for (ReproItem item : ITEMS)
            {
                addScrollChild(this, "row", new ItemRow(item, openRequested));
            }
            if (scrollView().frame().width() > 0 && scrollView().frame().height() > 0)
            {
                scrollToTop();
            }
        }
    }

    private static final class EditFieldRow extends EditFieldRowUI
    {
        private final int index;
        private final BiConsumer<Integer, String> actionRequested;

        EditFieldRow(
            int index,
            String value,
            BiConsumer<Integer, String> actionRequested)
        {
            this.index = index;
            this.actionRequested = actionRequested;
            field_input.setValue(value);
            field_btn.setTitle("Action " + fieldNumber(index));
        }

        @Override
        protected void onFieldBtnClick()
        {
            actionRequested.accept(index, field_input.value());
        }
    }

    private static final class EditFieldsScroll extends EditFieldsScrollUI
    {
        EditFieldsScroll(ReproItem item, BiConsumer<Integer, String> actionRequested)
        {
            clearChildren();
            for (int index = 0; index < FIELD_COUNT; ++index)
            {
                addScrollChild(
                    this,
                    "row",
                    new EditFieldRow(index, fieldValue(item, index), actionRequested));
            }
            if (scrollView().frame().width() > 0 && scrollView().frame().height() > 0)
            {
                scrollToTop();
            }
        }
    }

    private static final class EditPage extends EditPageUI
    {
        private final Runnable backRequested;

        EditPage(ReproItem item, Runnable backRequested)
        {
            this.backRequested = backRequested;
            edit_title.setText("Edit " + item.name());
            name_input.setValue(item.name());
            EditFieldsScroll fieldScroll = new EditFieldsScroll(item, this::handleFieldAction);
            fields.setView(fieldScroll);
            Size fieldsSize = GeneratedWindowRuntime.generatedWindowContentSize(fieldScroll);
            if (fieldsSize.width() > 0 && fieldsSize.height() > 0)
            {
                fieldScroll.restoreScrollPosition(new ScrollViewPosition(
                    0,
                    fieldScroll.scrollView().contentHeight(fieldsSize.width()),
                    false));
            }
        }

        @Override
        protected void onBackBtnClick()
        {
            backRequested.run();
        }

        @Override
        protected void onSaveBtnClick()
        {
            status.setText("Saved in memory");
        }

        private void handleFieldAction(Integer index, String value)
        {
            status.setText("Action " + fieldNumber(index) + ": " + value);
        }
    }

    public static void main(String[] arguments)
    {
        System.exit(GeneratedWindowRuntime.runGeneratedWindow(
            new StaleScrollviewFocus(),
            arguments));
    }
}

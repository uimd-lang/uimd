import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;

import uimd.Button;
import uimd.ComboBox;
import uimd.GeneratedWindowBase;
import uimd.GeneratedWindowRuntime;
import uimd.MessageBoxYesNo;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;

public final class ExpenseTracker extends ExpenseTrackerUI
{
    private static final List<String> DEFAULT_CATEGORIES =
        List.of("Food", "Transport", "Home", "Health", "Other");
    private static final String DEFAULT_CURRENCY = "EUR";
    private static final double DEFAULT_MONTHLY_LIMIT = 500.0;

    private final List<String> categories = new ArrayList<>(DEFAULT_CATEGORIES);
    private final List<ExpenseItem> expenses = seedExpenses();
    private String currency = DEFAULT_CURRENCY;
    private boolean showPaidExpenses = true;
    private double monthlyLimit = DEFAULT_MONTHLY_LIMIT;
    private String pendingDeleteId = "";
    private boolean quitRequested;
    private int nextExpenseNumber = expenses.size() + 1;

    public ExpenseTracker()
    {
        showExpenses();
    }

    @Override
    protected void onExpensesBtnClick()
    {
        showExpenses();
    }

    @Override
    protected void onCategoriesBtnClick()
    {
        showCategories();
    }

    @Override
    protected void onSettingsBtnClick()
    {
        showSettings();
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

    private List<ExpenseItem> expenseRows()
    {
        if (showPaidExpenses)
        {
            return new ArrayList<>(expenses);
        }
        return expenses.stream().filter(expense -> !expense.paid()).toList();
    }

    private void addExpense(String item, double amount, String category)
    {
        String normalizedItem = item == null ? "" : item.trim();
        if (normalizedItem.isEmpty())
        {
            normalizedItem = "Untitled expense";
        }
        String normalizedCategory = categories.contains(category) ? category : categories.get(0);
        expenses.add(0, new ExpenseItem(
            "e-" + nextExpenseNumber++,
            normalizedItem,
            amount,
            normalizedCategory,
            false));
    }

    private void updateExpense(String id, String category, boolean paid)
    {
        for (int index = 0; index < expenses.size(); ++index)
        {
            ExpenseItem expense = expenses.get(index);
            if (expense.id().equals(id))
            {
                expenses.set(index, expense.withCategoryAndPaid(category, paid));
                return;
            }
        }
    }

    private void confirmDeleteExpense(String id)
    {
        pendingDeleteId = id;
        String item = expenses.stream()
            .filter(expense -> expense.id().equals(id))
            .map(ExpenseItem::item)
            .findFirst()
            .orElse("expense");
        openWindow(new MessageBoxYesNo(
            "Delete Expense",
            "Delete " + item + "?",
            result -> deleteConfirmed("yes".equals(result))));
    }

    private String addCategory(String name)
    {
        String normalized = name == null ? "" : name.trim();
        if (normalized.isEmpty())
        {
            return "";
        }
        if (!categories.contains(normalized))
        {
            categories.add(normalized);
        }
        return normalized;
    }

    private void removeCategory(String name)
    {
        if (name == null || name.isEmpty() || categories.size() <= 1 || !categories.remove(name))
        {
            return;
        }
        String fallback = categories.get(0);
        for (int index = 0; index < expenses.size(); ++index)
        {
            ExpenseItem expense = expenses.get(index);
            if (expense.category().equals(name))
            {
                expenses.set(index, expense.withCategoryAndPaid(fallback, expense.paid()));
            }
        }
    }

    private void resetDemoData()
    {
        categories.clear();
        categories.addAll(DEFAULT_CATEGORIES);
        expenses.clear();
        expenses.addAll(seedExpenses());
        currency = DEFAULT_CURRENCY;
        showPaidExpenses = true;
        monthlyLimit = DEFAULT_MONTHLY_LIMIT;
        nextExpenseNumber = expenses.size() + 1;
        refreshCurrentView();
    }

    private void showExpenses()
    {
        selectNavButton(expenses_btn);
        main.setChild(new ExpensesView(this));
        setFocus(expenses_btn);
    }

    private void showCategories()
    {
        selectNavButton(categories_btn);
        main.setChild(new CategoriesView(this));
        setFocus(categories_btn);
    }

    private void showSettings()
    {
        selectNavButton(settings_btn);
        main.setChild(new SettingsView(this));
        setFocus(settings_btn);
    }

    private void deleteConfirmed(boolean confirmed)
    {
        String id = pendingDeleteId;
        pendingDeleteId = "";
        if (!confirmed)
        {
            return;
        }
        expenses.removeIf(expense -> expense.id().equals(id));
        refreshCurrentView();
    }

    private void refreshCurrentView()
    {
        GeneratedWindowBase current = main.child();
        if (current instanceof ExpensesView expensesView)
        {
            expensesView.refresh();
        }
        else if (current instanceof CategoriesView categoriesView)
        {
            categoriesView.refresh();
        }
        else if (current instanceof SettingsView settingsView)
        {
            settingsView.refresh();
        }
        selectCurrentNavButton();
    }

    private void selectCurrentNavButton()
    {
        if (main.child() instanceof CategoriesView)
        {
            selectNavButton(categories_btn);
        }
        else if (main.child() instanceof SettingsView)
        {
            selectNavButton(settings_btn);
        }
        else
        {
            selectNavButton(expenses_btn);
        }
    }

    private void selectNavButton(Button target)
    {
        Style normalStyle = normalNavButtonStyle(target);
        Style activeStyle = activeNavButtonStyle(target);
        for (Button button : List.of(expenses_btn, categories_btn, settings_btn))
        {
            button.setStyle(normalStyle.copy());
        }
        target.setStyle(activeStyle);
    }

    private static Style activeNavButtonStyle(Button button)
    {
        Style style = button.style().copy();
        style.merge(button.focusStyle());
        return style;
    }

    private Style normalNavButtonStyle(Button target)
    {
        if (expenses_btn != target && !(main.child() instanceof ExpensesView))
        {
            return expenses_btn.style().copy();
        }
        if (categories_btn != target && !(main.child() instanceof CategoriesView))
        {
            return categories_btn.style().copy();
        }
        if (settings_btn != target && !(main.child() instanceof SettingsView))
        {
            return settings_btn.style().copy();
        }
        return target.style().copy();
    }

    private static List<ExpenseItem> seedExpenses()
    {
        return new ArrayList<>(List.of(
            new ExpenseItem("e-1", "Groceries", 42.80, "Food", true),
            new ExpenseItem("e-2", "Bus ticket", 2.50, "Transport", true),
            new ExpenseItem("e-3", "Light bulbs", 8.90, "Home", false),
            new ExpenseItem("e-4", "Pharmacy", 14.20, "Health", true),
            new ExpenseItem("e-5", "Notebook", 5.40, "Other", false),
            new ExpenseItem("e-6", "Internet bill", 29.99, "Home", true),
            new ExpenseItem("e-7", "Team lunch", 18.50, "Food", false),
            new ExpenseItem("e-8", "Train pass", 24.00, "Transport", true),
            new ExpenseItem("e-9", "Vitamins", 11.75, "Health", false),
            new ExpenseItem("e-10", "Desk lamp", 21.30, "Home", false),
            new ExpenseItem("e-11", "Cloud storage", 9.99, "Other", true),
            new ExpenseItem("e-12", "Replacement keyboard", 44.00, "Other", false)));
    }

    private static int measuredLayoutHeight(GeneratedWindowBase window)
    {
        Size natural = GeneratedWindowRuntime.generatedWindowContentSize(window);
        Size measured = GeneratedWindowRuntime.generatedWindowContentSizeForWidth(
            window,
            natural.width());
        return Math.max(1, measured.height());
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            ExpenseTracker application = new ExpenseTracker();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private record ExpenseItem(String id, String item, double amount, String category, boolean paid)
    {
        ExpenseItem withCategoryAndPaid(String nextCategory, boolean nextPaid)
        {
            return new ExpenseItem(id, item, amount, nextCategory, nextPaid);
        }
    }

    @FunctionalInterface
    private interface RowChanged
    {
        void accept(String id, String category, boolean paid);
    }

    private static final class ExpensesList extends ExpensesListUI
    {
        private final List<ExpenseItem> rows = new ArrayList<>();
        private Consumer<String> deleteRequested;
        private RowChanged rowChanged;

        void setDeleteRequested(Consumer<String> value)
        {
            deleteRequested = value;
        }

        void setRowChanged(RowChanged value)
        {
            rowChanged = value;
        }

        void setRows(List<ExpenseItem> nextRows, List<String> nextCategories)
        {
            rows.clear();
            rows.addAll(nextRows);
            clearChildren();
            for (int index = 0; index < rows.size(); ++index)
            {
                ExpenseRow row = new ExpenseRow(
                    rows.get(index),
                    nextCategories,
                    deleteRequested,
                    rowChanged);
                row.setMode("expand_width");
                ReusableElement reusable = new ReusableElement("expenses[" + index + "]", row);
                reusable.setFrame(new Rect(0, 0, 0, measuredLayoutHeight(row)));
                addChild(reusable);
            }
            scrollToTop();
        }
    }

    private static final class ExpenseRow extends ExpenseRowUI
    {
        private final ExpenseItem record;
        private final List<String> categories;
        private final Consumer<String> onDelete;
        private final RowChanged onChange;

        ExpenseRow(
            ExpenseItem record,
            List<String> categories,
            Consumer<String> onDelete,
            RowChanged onChange)
        {
            this.record = record;
            this.categories = categories;
            this.onDelete = onDelete;
            this.onChange = onChange;
            syncFromRecord();
        }

        @Override
        protected void onPaidChange(String value)
        {
            if (onChange != null)
            {
                onChange.accept(record.id(), category.selectedText(), paid.checked());
            }
        }

        @Override
        protected void onCategoryChange(String value)
        {
            if (onChange != null)
            {
                onChange.accept(record.id(), category.selectedText(), paid.checked());
            }
        }

        @Override
        protected void onDeleteBtnClick()
        {
            if (onDelete != null)
            {
                onDelete.accept(record.id());
            }
        }

        private void syncFromRecord()
        {
            item.setText(record.item());
            amount.setText(String.format(Locale.ROOT, "%.2f", record.amount()));
            paid.setChecked(record.paid());
            category.setOptions(categories);
            int categoryIndex = category.options().indexOf(record.category());
            if (!category.options().isEmpty())
            {
                category.setSelectedIndex(categoryIndex >= 0 ? categoryIndex : 0);
            }
        }
    }

    private static final class ExpensesView extends ExpensesViewUI
    {
        private final ExpenseTracker shell;
        private final ExpensesList expensesList = new ExpensesList();

        ExpensesView(ExpenseTracker shell)
        {
            this.shell = shell;
            expensesList.setDeleteRequested(shell::confirmDeleteExpense);
            expensesList.setRowChanged(shell::updateExpense);
            expenses.setChild(expensesList);
            refresh();
        }

        void refresh()
        {
            currency_label.setText(shell.currency);
            category_input.setOptions(shell.categories);
            selectComboValue(category_input, category_input.selectedText());
            expensesList.setRows(shell.expenseRows(), shell.categories);
        }

        @Override
        protected void onAddExpenseBtnClick()
        {
            shell.addExpense(item_input.value(), amount_input.value(), category_input.selectedText());
            item_input.setValue("");
            amount_input.setValue(0.0);
            refresh();
        }
    }

    private static final class CategoriesView extends CategoriesViewUI
    {
        private final ExpenseTracker shell;

        CategoriesView(ExpenseTracker shell)
        {
            this.shell = shell;
            refresh();
        }

        void refresh()
        {
            categories.setOptions(shell.categories);
            if (!categories.options().isEmpty())
            {
                categories.setSelectedIndex(0);
            }
        }

        @Override
        protected void onAddCategoryBtnClick()
        {
            String added = shell.addCategory(category_name.value());
            category_name.setValue("");
            refresh();
            if (!added.isEmpty())
            {
                categories.setSelectedValues(List.of(added));
            }
        }

        @Override
        protected void onRemoveCategoryBtnClick()
        {
            String selected = categories.selectedValues().stream().findFirst().orElse("");
            if (!selected.isEmpty())
            {
                shell.removeCategory(selected);
            }
            refresh();
        }
    }

    private static final class SettingsView extends SettingsViewUI
    {
        private final ExpenseTracker shell;

        SettingsView(ExpenseTracker shell)
        {
            this.shell = shell;
            refresh();
        }

        void refresh()
        {
            selectComboValue(currency, shell.currency);
            show_paid.setChecked(shell.showPaidExpenses);
            monthly_limit.setValue(shell.monthlyLimit);
        }

        @Override
        protected void onResetBtnClick()
        {
            shell.resetDemoData();
        }

        @Override
        protected void onCurrencyChange(String value)
        {
            shell.currency = value;
        }

        @Override
        protected void onShowPaidChange(String value)
        {
            shell.showPaidExpenses = show_paid.checked();
        }

        @Override
        protected void onMonthlyLimitChange(String value)
        {
            shell.monthlyLimit = monthly_limit.value();
        }

        @Override
        protected void onMonthlyLimitSubmit(String value)
        {
            shell.monthlyLimit = monthly_limit.value();
        }
    }

    private static void selectComboValue(ComboBox comboBox, String value)
    {
        int index = comboBox.options().indexOf(value);
        comboBox.setSelectedIndex(index >= 0 ? index : 0);
    }
}

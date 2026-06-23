using Uimd;

public sealed record ExpenseItem(string Id, string Item, double Amount, string Category, bool Paid);

public sealed class ExpensesList : ExpensesListUI
{
    private readonly List<ExpenseItem> rows = new();

    public Action<string>? DeleteRequested { get; set; }
    public Action<string, string, bool>? RowChanged { get; set; }

    public void SetRows(IEnumerable<ExpenseItem> nextRows, IReadOnlyList<string> categories)
    {
        rows.Clear();
        rows.AddRange(nextRows);
        ScrollView().ClearChildren();
        for (int index = 0; index < rows.Count; ++index)
        {
            ExpenseItem expense = rows[index];
            ExpenseRow row = new(expense, categories, DeleteRequested, RowChanged);
            ReusableElement reusable = new("expenses[" + index + "]");
            reusable.SetChild(row);
            row.Mode = "expand_width";
            reusable.Frame = new Rect(0, 0, 0, GeneratedWindowRuntime.GeneratedWindowContentSize(row).Height);
            AddChild(reusable);
        }
    }
}

public sealed class ExpenseRow : ExpenseRowUI
{
    private readonly ExpenseItem record;
    private readonly IReadOnlyList<string> categories;
    private readonly Action<string>? onDelete;
    private readonly Action<string, string, bool>? onChange;

    public ExpenseRow(
        ExpenseItem record,
        IReadOnlyList<string> categories,
        Action<string>? onDelete,
        Action<string, string, bool>? onChange)
    {
        this.record = record;
        this.categories = categories;
        this.onDelete = onDelete;
        this.onChange = onChange;
        SyncFromRecord();
    }

    protected override void onPaidChange(string value)
    {
        onChange?.Invoke(record.Id, category.SelectedText, paid.Checked);
    }

    protected override void onCategoryChange(string value)
    {
        onChange?.Invoke(record.Id, category.SelectedText, paid.Checked);
    }

    protected override void onDeleteBtnClick()
    {
        onDelete?.Invoke(record.Id);
    }

    private void SyncFromRecord()
    {
        item.SetText(record.Item);
        amount.SetText(record.Amount.ToString("0.00", System.Globalization.CultureInfo.InvariantCulture));
        paid.SetChecked(record.Paid);
        category.SetOptions(categories);
        int categoryIndex = category.Options.IndexOf(record.Category);
        if (category.Options.Count > 0)
        {
            category.SetSelectedIndex(categoryIndex >= 0 ? categoryIndex : 0);
        }
    }
}

public sealed class ExpensesView : ExpensesViewUI
{
    private readonly ExpenseTracker shell;
    private readonly ExpensesList expensesList;

    public ExpensesView(ExpenseTracker shell)
    {
        this.shell = shell;
        expensesList = new ExpensesList();
        expensesList.DeleteRequested = shell.ConfirmDeleteExpense;
        expensesList.RowChanged = shell.UpdateExpense;
        expenses.SetChild(expensesList);
        Refresh();
    }

    public void Refresh()
    {
        currency_label.SetText(shell.Currency);
        category_input.SetOptions(shell.Categories);
        SelectComboValue(category_input, category_input.SelectedText);
        expensesList.SetRows(shell.ExpenseRows(), shell.Categories);
    }

    protected override void onAddExpenseBtnClick()
    {
        shell.AddExpense(item_input.Value, amount_input.Value, category_input.SelectedText);
        item_input.SetValue("");
        amount_input.SetValue(0);
        Refresh();
    }

    private static void SelectComboValue(ComboBox combo, string value)
    {
        int index = combo.Options.IndexOf(value);
        combo.SetSelectedIndex(index >= 0 ? index : 0);
    }
}

public sealed class CategoriesView : CategoriesViewUI
{
    private readonly ExpenseTracker shell;

    public CategoriesView(ExpenseTracker shell)
    {
        this.shell = shell;
        Refresh();
    }

    public void Refresh()
    {
        categories.SetOptions(shell.Categories);
        if (categories.Options.Count > 0)
        {
            categories.SetSelectedIndex(0);
        }
    }

    protected override void onAddCategoryBtnClick()
    {
        string added = shell.AddCategory(category_name.Value);
        category_name.SetValue("");
        Refresh();
        if (!string.IsNullOrEmpty(added))
        {
            categories.SetSelectedValues(new[] { added });
        }
    }

    protected override void onRemoveCategoryBtnClick()
    {
        string selected = categories.SelectedValues.FirstOrDefault() ?? "";
        if (!string.IsNullOrEmpty(selected))
        {
            shell.RemoveCategory(selected);
        }
        Refresh();
    }
}

public sealed class SettingsView : SettingsViewUI
{
    private readonly ExpenseTracker shell;

    public SettingsView(ExpenseTracker shell)
    {
        this.shell = shell;
        Refresh();
    }

    public void Refresh()
    {
        SelectComboValue(currency, shell.Currency);
        show_paid.SetChecked(shell.ShowPaidExpenses);
        monthly_limit.SetValue(shell.MonthlyLimit);
    }

    protected override void onResetBtnClick()
    {
        shell.ResetDemoData();
    }

    protected override void onCurrencyChange(string value)
    {
        shell.Currency = value;
    }

    protected override void onShowPaidChange(string value)
    {
        shell.ShowPaidExpenses = show_paid.Checked;
    }

    protected override void onMonthlyLimitChange(string value)
    {
        shell.MonthlyLimit = monthly_limit.Value;
    }

    protected override void onMonthlyLimitSubmit(string value)
    {
        shell.MonthlyLimit = monthly_limit.Value;
    }

    private static void SelectComboValue(ComboBox combo, string value)
    {
        int index = combo.Options.IndexOf(value);
        combo.SetSelectedIndex(index >= 0 ? index : 0);
    }
}

public sealed class ExpenseTracker : ExpenseTrackerUI
{
    private static readonly string[] DefaultCategories = { "Food", "Transport", "Home", "Health", "Other" };
    private readonly List<string> categories = new(DefaultCategories);
    private readonly List<ExpenseItem> expenses = SeedExpenses();
    private string pendingDeleteId = "";
    private bool quitRequested;
    private int nextExpenseNumber = 13;

    public ExpenseTracker()
    {
        ShowExpenses();
    }

    public string Currency { get; set; } = "EUR";
    public bool ShowPaidExpenses { get; set; } = true;
    public double MonthlyLimit { get; set; } = 500.0;
    public IReadOnlyList<string> Categories => categories;

    protected override void onExpensesBtnClick()
    {
        ShowExpenses();
    }

    protected override void onCategoriesBtnClick()
    {
        ShowCategories();
    }

    protected override void onSettingsBtnClick()
    {
        ShowSettings();
    }

    protected override void onQuitBtnClick()
    {
        quitRequested = true;
    }

    protected override bool shouldClose()
    {
        return quitRequested;
    }

    public IEnumerable<ExpenseItem> ExpenseRows()
    {
        return expenses.Where(expense => ShowPaidExpenses || !expense.Paid).ToList();
    }

    public void AddExpense(string item, double amount, string category)
    {
        item = item.Trim();
        if (string.IsNullOrEmpty(item))
        {
            item = "Untitled expense";
        }
        if (!categories.Contains(category))
        {
            category = categories[0];
        }
        expenses.Insert(0, new ExpenseItem("e-" + nextExpenseNumber++, item, amount, category, false));
    }

    public void UpdateExpense(string id, string category, bool paid)
    {
        int index = expenses.FindIndex(expense => expense.Id == id);
        if (index >= 0)
        {
            expenses[index] = expenses[index] with { Category = category, Paid = paid };
        }
    }

    public void ConfirmDeleteExpense(string id)
    {
        pendingDeleteId = id;
        string item = expenses.FirstOrDefault(expense => expense.Id == id)?.Item ?? "expense";
        MessageBoxYesNo dialog = new("Delete Expense", "Delete " + item + "?");
        dialog.OnClose = DeleteConfirmed;
        OpenWindow(dialog);
    }

    public string AddCategory(string name)
    {
        name = name.Trim();
        if (string.IsNullOrEmpty(name))
        {
            return "";
        }
        if (!categories.Contains(name))
        {
            categories.Add(name);
        }
        return name;
    }

    public void RemoveCategory(string name)
    {
        if (string.IsNullOrEmpty(name) || categories.Count <= 1)
        {
            return;
        }
        if (!categories.Remove(name))
        {
            return;
        }
        string fallback = categories[0];
        for (int index = 0; index < expenses.Count; ++index)
        {
            if (expenses[index].Category == name)
            {
                expenses[index] = expenses[index] with { Category = fallback };
            }
        }
    }

    public void ResetDemoData()
    {
        categories.Clear();
        categories.AddRange(DefaultCategories);
        expenses.Clear();
        expenses.AddRange(SeedExpenses());
        Currency = "EUR";
        ShowPaidExpenses = true;
        MonthlyLimit = 500.0;
        nextExpenseNumber = 13;
        RefreshCurrentView();
    }

    private void ShowExpenses()
    {
        SelectNavButton(expenses_btn);
        main.SetChild(new ExpensesView(this));
        SetFocus(expenses_btn);
    }

    private void ShowCategories()
    {
        SelectNavButton(categories_btn);
        main.SetChild(new CategoriesView(this));
        SetFocus(categories_btn);
    }

    private void ShowSettings()
    {
        SelectNavButton(settings_btn);
        main.SetChild(new SettingsView(this));
        SetFocus(settings_btn);
    }

    private void DeleteConfirmed(bool confirmed)
    {
        string id = pendingDeleteId;
        pendingDeleteId = "";
        if (!confirmed)
        {
            return;
        }
        expenses.RemoveAll(expense => expense.Id == id);
        RefreshCurrentView();
    }

    private void RefreshCurrentView()
    {
        if (main.Child is ExpensesView expensesView)
        {
            expensesView.Refresh();
        }
        else if (main.Child is CategoriesView categoriesView)
        {
            categoriesView.Refresh();
        }
        else if (main.Child is SettingsView settingsView)
        {
            settingsView.Refresh();
        }
        SelectCurrentNavButton();
    }

    private void SelectCurrentNavButton()
    {
        if (main.Child is CategoriesView)
        {
            SelectNavButton(categories_btn);
        }
        else if (main.Child is SettingsView)
        {
            SelectNavButton(settings_btn);
        }
        else
        {
            SelectNavButton(expenses_btn);
        }
    }

    private void SelectNavButton(Button target)
    {
        Style normalStyle = NormalNavButtonStyle(target);
        Style activeStyle = ActiveNavButtonStyle(target);
        foreach (Button button in new[] { expenses_btn, categories_btn, settings_btn })
        {
            button.SetStyle(normalStyle.Clone());
        }
        target.SetStyle(activeStyle);
    }

    private Style ActiveNavButtonStyle(Button button)
    {
        Style style = button.Style.Clone();
        if (button.FocusStyle is not null)
        {
            style.Merge(button.FocusStyle);
        }
        return style;
    }

    private Style NormalNavButtonStyle(Button target)
    {
        foreach ((Button button, Type viewType) in new[]
        {
            (expenses_btn, typeof(ExpensesView)),
            (categories_btn, typeof(CategoriesView)),
            (settings_btn, typeof(SettingsView)),
        })
        {
            if (button != target && (main.Child is null || main.Child.GetType() != viewType))
            {
                return button.Style.Clone();
            }
        }
        return target.Style.Clone();
    }

    private static List<ExpenseItem> SeedExpenses()
    {
        return new List<ExpenseItem>
        {
            new("e-1", "Groceries", 42.80, "Food", true),
            new("e-2", "Bus ticket", 2.50, "Transport", true),
            new("e-3", "Light bulbs", 8.90, "Home", false),
            new("e-4", "Pharmacy", 14.20, "Health", true),
            new("e-5", "Notebook", 5.40, "Other", false),
            new("e-6", "Internet bill", 29.99, "Home", true),
            new("e-7", "Team lunch", 18.50, "Food", false),
            new("e-8", "Train pass", 24.00, "Transport", true),
            new("e-9", "Vitamins", 11.75, "Health", false),
            new("e-10", "Desk lamp", 21.30, "Home", false),
            new("e-11", "Cloud storage", 9.99, "Other", true),
            new("e-12", "Replacement keyboard", 44.00, "Other", false),
        };
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            ExpenseTracker app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}

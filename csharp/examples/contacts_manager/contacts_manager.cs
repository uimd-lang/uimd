using Uimd;

public sealed record Contact(
    string Id,
    string FirstName,
    string LastName,
    string Email,
    string Phone,
    string Company,
    string City,
    string Category,
    bool Active,
    string Notes);

public sealed class ContactForm : ContactFormUI
{
    private string contactId = "";

    public Action<Contact?>? OnClosed { get; set; }

    public ContactForm(Contact? contact = null)
    {
        if (contact is not null)
        {
            contactId = contact.Id;
            first_name.SetValue(contact.FirstName);
            last_name.SetValue(contact.LastName);
            email.SetValue(contact.Email);
            phone.SetValue(contact.Phone);
            company.SetValue(contact.Company);
            city.SetValue(contact.City);
            SelectCategory(contact.Category);
            active.SetChecked(contact.Active);
            notes.SetValue(contact.Notes);
        }
    }

    protected override void onSaveBtnClick()
    {
        if (string.IsNullOrWhiteSpace(first_name.Value) ||
            string.IsNullOrWhiteSpace(last_name.Value) ||
            string.IsNullOrWhiteSpace(email.Value))
        {
            message.SetText("Required fields are missing.");
            return;
        }
        OnClosed?.Invoke(new Contact(
            contactId,
            first_name.Value,
            last_name.Value,
            email.Value,
            phone.Value,
            company.Value,
            city.Value,
            category.SelectedText,
            active.Checked,
            notes.Value));
        CloseWindow();
    }

    protected override void onCancelBtnClick()
    {
        OnClosed?.Invoke(null);
        CloseWindow();
    }

    private void SelectCategory(string value)
    {
        int index = category.Options.IndexOf(value);
        category.SetSelectedIndex(index >= 0 ? index : 0);
    }
}

public sealed class ContactsManager : ContactsManagerUI
{
    private readonly List<Contact> contactRecords = new();
    private bool dirty;
    private bool finished;
    private string deleteContactId = "";

    public ContactsManager()
    {
        Reload();
    }

    protected override void onContactsSelectionChange(List<string> value)
    {
        RefreshDetail();
    }

    protected override void onAddBtnClick()
    {
        OpenForm(null);
    }

    protected override void onEditBtnClick()
    {
        Contact? contact = SelectedContact();
        if (contact is null)
        {
            SetStatus("Select a contact to edit.");
            return;
        }
        OpenForm(contact);
    }

    protected override void onDeleteBtnClick()
    {
        Contact? contact = SelectedContact();
        if (contact is null)
        {
            SetStatus("Select a contact to delete.");
            return;
        }
        deleteContactId = contact.Id;
        MessageBoxYesNo dialog = new("Delete Contact", "Delete " + RowLabel(contact) + "?");
        dialog.OnClose = DeleteConfirmed;
        OpenWindow(dialog);
    }

    protected override void onSaveBtnClick()
    {
        Save();
    }

    protected override void onReloadBtnClick()
    {
        Reload();
    }

    protected override void onCloseBtnClick()
    {
        finished = true;
    }

    protected override bool shouldClose()
    {
        return finished;
    }

    private void Reload()
    {
        contactRecords.Clear();
        contactRecords.AddRange(LoadContacts());
        dirty = false;
        RefreshList(SelectedId());
        SetStatus("Loaded " + contactRecords.Count + " contacts.");
    }

    private void Save()
    {
        SaveContacts(contactRecords);
        dirty = false;
        SetStatus("Saved " + contactRecords.Count + " contacts.");
    }

    private void OpenForm(Contact? contact)
    {
        ContactForm form = new(contact);
        form.OnClosed = OnFormClosed;
        OpenWindow(form);
    }

    private void OnFormClosed(Contact? result)
    {
        if (result is null)
        {
            SetStatus("Edit cancelled.");
            return;
        }
        if (string.IsNullOrEmpty(result.Id))
        {
            string id = NextId().ToString();
            Contact contact = result with { Id = id };
            contactRecords.Add(contact);
            SetStatus("Contact added.");
            dirty = true;
            RefreshList(id);
            return;
        }
        for (int index = 0; index < contactRecords.Count; ++index)
        {
            if (contactRecords[index].Id == result.Id)
            {
                contactRecords[index] = result;
                SetStatus("Contact updated.");
                dirty = true;
                RefreshList(result.Id);
                return;
            }
        }
        dirty = true;
        RefreshList(result.Id);
    }

    private void DeleteConfirmed(bool yes)
    {
        if (!yes)
        {
            SetStatus("Delete cancelled.");
            return;
        }
        int before = contactRecords.Count;
        contactRecords.RemoveAll(contact => contact.Id == deleteContactId);
        deleteContactId = "";
        if (contactRecords.Count == before)
        {
            SetStatus("Contact no longer exists.");
            return;
        }
        dirty = true;
        RefreshList("");
        SetStatus("Contact deleted.");
    }

    private void RefreshList(string selectedContactId)
    {
        List<string> labels = contactRecords.Select(RowLabel).ToList();
        header.SetText("Contacts Manager - " + labels.Count + " records");
        SetContactsList(labels, selectedContactId);
        RefreshDetail();
    }

    private void SetContactsList(List<string> labels, string selectedContactId)
    {
        contacts.SetOptions(labels);
        if (labels.Count == 0)
        {
            contacts.SetSelectedIndex(-1);
            return;
        }
        int selectedIndex = 0;
        if (!string.IsNullOrEmpty(selectedContactId))
        {
            int index = contactRecords.FindIndex(contact => contact.Id == selectedContactId);
            selectedIndex = index >= 0 ? index : 0;
        }
        contacts.SetSelectedIndex(selectedIndex);
    }

    private void RefreshDetail()
    {
        Contact? contact = SelectedContact();
        detail.SetText(contact is null ? "No contact selected." : DetailText(contact));
    }

    private Contact? SelectedContact()
    {
        if (contactRecords.Count == 0)
        {
            return null;
        }
        int index = Math.Clamp(contacts.SelectedIndex, 0, contactRecords.Count - 1);
        return contactRecords[index];
    }

    private string SelectedId()
    {
        return SelectedContact()?.Id ?? "";
    }

    private int NextId()
    {
        int result = 1;
        foreach (Contact contact in contactRecords)
        {
            if (int.TryParse(contact.Id, out int id))
            {
                result = Math.Max(result, id + 1);
            }
        }
        return result;
    }

    private void SetStatus(string text)
    {
        status.SetText(text + (dirty ? " *" : ""));
    }

    private static string RowLabel(Contact contact)
    {
        return (contact.FirstName + " " + contact.LastName).Trim();
    }

    private static string DetailText(Contact contact)
    {
        return contact.FirstName + " " + contact.LastName +
            "\nEmail: " + contact.Email +
            "\nPhone: " + contact.Phone +
            "\nCompany: " + contact.Company +
            "\nCity: " + contact.City +
            "\nCategory: " + contact.Category + " (" + (contact.Active ? "active" : "inactive") + ")" +
            "\n\n" + contact.Notes;
    }

    private static string DataPath()
    {
        string? path = Environment.GetEnvironmentVariable("UI_CONTACTS_MANAGER_DATA_PATH");
        return string.IsNullOrEmpty(path) ? Path.Combine(AppContext.BaseDirectory, "data", "contacts.csv") : path;
    }

    private static List<Contact> LoadContacts()
    {
        List<Contact> result = new();
        string path = DataPath();
        if (!File.Exists(path))
        {
            return result;
        }
        foreach (string line in File.ReadLines(path).Skip(1))
        {
            string[] cells = line.Split(',');
            if (cells.Length < 10)
            {
                continue;
            }
            result.Add(new Contact(cells[0], cells[1], cells[2], cells[3], cells[4], cells[5], cells[6], cells[7], cells[8] is "1" or "true", cells[9]));
        }
        return result;
    }

    private static void SaveContacts(List<Contact> contacts)
    {
        string path = DataPath();
        Directory.CreateDirectory(Path.GetDirectoryName(path) ?? ".");
        List<string> lines = new()
        {
            "id,first_name,last_name,email,phone,company,city,category,active,notes",
        };
        lines.AddRange(contacts.Select(contact =>
            string.Join(",", contact.Id, contact.FirstName, contact.LastName, contact.Email, contact.Phone,
                contact.Company, contact.City, contact.Category, contact.Active ? "true" : "false", contact.Notes)));
        File.WriteAllLines(path, lines);
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            ContactsManager app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}

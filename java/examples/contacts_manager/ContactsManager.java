import java.io.IOException;
import java.io.UncheckedIOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;

import uimd.GeneratedWindowRuntime;
import uimd.MessageBoxYesNo;

public final class ContactsManager extends ContactsManagerUI
{
    private static final String DATA_PATH_ENV = "UI_CONTACTS_MANAGER_DATA_PATH";
    private static final String CSV_HEADER =
        "id,first_name,last_name,email,phone,company,city,category,active,notes";

    private final List<Contact> contactRecords = new ArrayList<>();
    private boolean dirty;
    private boolean finished;
    private String deleteContactId = "";

    public ContactsManager()
    {
        reload();
    }

    @Override
    protected void onContactsSelectionChange(List<String> value)
    {
        refreshDetail();
    }

    @Override
    protected void onAddBtnClick()
    {
        openForm(null);
    }

    @Override
    protected void onEditBtnClick()
    {
        Contact contact = selectedContact();
        if (contact == null)
        {
            setStatus("Select a contact to edit.");
            return;
        }
        openForm(contact);
    }

    @Override
    protected void onDeleteBtnClick()
    {
        Contact contact = selectedContact();
        if (contact == null)
        {
            setStatus("Select a contact to delete.");
            return;
        }
        deleteContactId = contact.id();
        openWindow(new MessageBoxYesNo(
            "Delete Contact",
            "Delete " + rowLabel(contact) + "?",
            result -> deleteConfirmed("yes".equals(result))));
    }

    @Override
    protected void onSaveBtnClick()
    {
        save();
    }

    @Override
    protected void onReloadBtnClick()
    {
        reload();
    }

    @Override
    protected void onCloseBtnClick()
    {
        finished = true;
    }

    @Override
    protected boolean shouldClose()
    {
        return finished;
    }

    private void reload()
    {
        contactRecords.clear();
        contactRecords.addAll(loadContacts());
        dirty = false;
        refreshList(selectedId());
        setStatus("Loaded " + contactRecords.size() + " contacts.");
    }

    private void save()
    {
        saveContacts(contactRecords);
        dirty = false;
        setStatus("Saved " + contactRecords.size() + " contacts.");
    }

    private void openForm(Contact contact)
    {
        ContactForm form = new ContactForm(contact);
        form.setOnClosed(this::onFormClosed);
        openWindow(form);
    }

    private void onFormClosed(Contact result)
    {
        if (result == null)
        {
            setStatus("Edit cancelled.");
            return;
        }
        if (result.id().isEmpty())
        {
            String id = Integer.toString(nextId());
            contactRecords.add(result.withId(id));
            setStatus("Contact added.");
            dirty = true;
            refreshList(id);
            return;
        }
        for (int index = 0; index < contactRecords.size(); ++index)
        {
            if (contactRecords.get(index).id().equals(result.id()))
            {
                contactRecords.set(index, result);
                setStatus("Contact updated.");
                dirty = true;
                refreshList(result.id());
                return;
            }
        }
        dirty = true;
        refreshList(result.id());
    }

    private void deleteConfirmed(boolean yes)
    {
        if (!yes)
        {
            setStatus("Delete cancelled.");
            return;
        }
        int before = contactRecords.size();
        contactRecords.removeIf(contact -> contact.id().equals(deleteContactId));
        deleteContactId = "";
        if (contactRecords.size() == before)
        {
            setStatus("Contact no longer exists.");
            return;
        }
        dirty = true;
        refreshList("");
        setStatus("Contact deleted.");
    }

    private void refreshList(String selectedContactId)
    {
        List<String> labels = contactRecords.stream().map(ContactsManager::rowLabel).toList();
        header.setText("Contacts Manager - " + labels.size() + " records");
        setContactsList(labels, selectedContactId);
        refreshDetail();
    }

    private void setContactsList(List<String> labels, String selectedContactId)
    {
        contacts.setOptions(labels);
        if (labels.isEmpty())
        {
            contacts.setSelectedIndex(-1);
            return;
        }
        int selectedIndex = 0;
        if (selectedContactId != null && !selectedContactId.isEmpty())
        {
            for (int index = 0; index < contactRecords.size(); ++index)
            {
                if (contactRecords.get(index).id().equals(selectedContactId))
                {
                    selectedIndex = index;
                    break;
                }
            }
        }
        contacts.setSelectedIndex(selectedIndex);
    }

    private void refreshDetail()
    {
        Contact contact = selectedContact();
        detail.setText(contact == null ? "No contact selected." : detailText(contact));
    }

    private Contact selectedContact()
    {
        if (contactRecords.isEmpty())
        {
            return null;
        }
        int index = Math.max(0, Math.min(contacts.selectedIndex(), contactRecords.size() - 1));
        return contactRecords.get(index);
    }

    private String selectedId()
    {
        Contact contact = selectedContact();
        return contact == null ? "" : contact.id();
    }

    private int nextId()
    {
        int result = 1;
        for (Contact contact : contactRecords)
        {
            try
            {
                result = Math.max(result, Integer.parseInt(contact.id()) + 1);
            }
            catch (NumberFormatException ignored)
            {
                // Non-numeric identifiers do not participate in numeric allocation.
            }
        }
        return result;
    }

    private void setStatus(String text)
    {
        status.setText(text + (dirty ? " *" : ""));
    }

    private static String rowLabel(Contact contact)
    {
        return (contact.firstName() + " " + contact.lastName()).trim();
    }

    private static String detailText(Contact contact)
    {
        return contact.firstName() + " " + contact.lastName()
            + "\nEmail: " + contact.email()
            + "\nPhone: " + contact.phone()
            + "\nCompany: " + contact.company()
            + "\nCity: " + contact.city()
            + "\nCategory: " + contact.category()
            + " (" + (contact.active() ? "active" : "inactive") + ")"
            + "\n\n" + contact.notes();
    }

    private static Path dataPath()
    {
        String configured = System.getenv(DATA_PATH_ENV);
        if (configured != null && !configured.isEmpty())
        {
            return Path.of(configured);
        }
        return Path.of("python", "examples", "contacts_manager", "data", "contacts.csv");
    }

    private static List<Contact> loadContacts()
    {
        Path path = dataPath();
        if (!Files.exists(path))
        {
            return List.of();
        }
        try
        {
            List<Contact> result = new ArrayList<>();
            List<String> lines = Files.readAllLines(path, StandardCharsets.UTF_8);
            for (int index = 1; index < lines.size(); ++index)
            {
                List<String> cells = splitCsvLine(lines.get(index));
                if (cells.size() < 10)
                {
                    continue;
                }
                result.add(new Contact(
                    cells.get(0),
                    cells.get(1),
                    cells.get(2),
                    cells.get(3),
                    cells.get(4),
                    cells.get(5),
                    cells.get(6),
                    cells.get(7),
                    toBoolean(cells.get(8)),
                    cells.get(9)));
            }
            return result;
        }
        catch (IOException error)
        {
            throw new UncheckedIOException(error);
        }
    }

    private static void saveContacts(List<Contact> records)
    {
        Path path = dataPath();
        try
        {
            Path parent = path.getParent();
            if (parent != null)
            {
                Files.createDirectories(parent);
            }
            List<String> lines = new ArrayList<>();
            lines.add(CSV_HEADER);
            for (Contact contact : records)
            {
                lines.add(String.join(",",
                    csvCell(contact.id()),
                    csvCell(contact.firstName()),
                    csvCell(contact.lastName()),
                    csvCell(contact.email()),
                    csvCell(contact.phone()),
                    csvCell(contact.company()),
                    csvCell(contact.city()),
                    csvCell(contact.category()),
                    contact.active() ? "true" : "false",
                    csvCell(contact.notes())));
            }
            Files.write(path, lines, StandardCharsets.UTF_8);
        }
        catch (IOException error)
        {
            throw new UncheckedIOException(error);
        }
    }

    private static List<String> splitCsvLine(String line)
    {
        List<String> cells = new ArrayList<>();
        StringBuilder cell = new StringBuilder();
        boolean quoted = false;
        for (int index = 0; index < line.length(); ++index)
        {
            char value = line.charAt(index);
            if (value == '"')
            {
                if (quoted && index + 1 < line.length() && line.charAt(index + 1) == '"')
                {
                    cell.append(value);
                    ++index;
                }
                else
                {
                    quoted = !quoted;
                }
            }
            else if (value == ',' && !quoted)
            {
                cells.add(cell.toString());
                cell.setLength(0);
            }
            else
            {
                cell.append(value);
            }
        }
        cells.add(cell.toString());
        return cells;
    }

    private static String csvCell(String value)
    {
        if (value.indexOf(',') < 0 && value.indexOf('"') < 0 && value.indexOf('\n') < 0)
        {
            return value;
        }
        return '"' + value.replace("\"", "\"\"") + '"';
    }

    private static boolean toBoolean(String value)
    {
        return switch (value.strip().toLowerCase(Locale.ROOT))
        {
            case "1", "true", "yes", "y" -> true;
            default -> false;
        };
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            ContactsManager application = new ContactsManager();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }

    private record Contact(
        String id,
        String firstName,
        String lastName,
        String email,
        String phone,
        String company,
        String city,
        String category,
        boolean active,
        String notes)
    {
        Contact withId(String value)
        {
            return new Contact(
                value,
                firstName,
                lastName,
                email,
                phone,
                company,
                city,
                category,
                active,
                notes);
        }
    }

    private static final class ContactForm extends ContactFormUI
    {
        private String contactId = "";
        private Consumer<Contact> onClosed;

        ContactForm(Contact contact)
        {
            if (contact == null)
            {
                return;
            }
            contactId = contact.id();
            first_name.setValue(contact.firstName());
            last_name.setValue(contact.lastName());
            email.setValue(contact.email());
            phone.setValue(contact.phone());
            company.setValue(contact.company());
            city.setValue(contact.city());
            selectCategory(contact.category());
            active.setChecked(contact.active());
            notes.setValue(contact.notes());
        }

        void setOnClosed(Consumer<Contact> value)
        {
            onClosed = value;
        }

        @Override
        protected void onSaveBtnClick()
        {
            if (first_name.value().isBlank()
                || last_name.value().isBlank()
                || email.value().isBlank())
            {
                message.setText("Required fields are missing.");
                return;
            }
            if (onClosed != null)
            {
                onClosed.accept(new Contact(
                    contactId,
                    first_name.value(),
                    last_name.value(),
                    email.value(),
                    phone.value(),
                    company.value(),
                    city.value(),
                    category.selectedText(),
                    active.checked(),
                    notes.value()));
            }
            closeWindow();
        }

        @Override
        protected void onCancelBtnClick()
        {
            if (onClosed != null)
            {
                onClosed.accept(null);
            }
            closeWindow();
        }

        private void selectCategory(String value)
        {
            int index = category.options().indexOf(value);
            category.setSelectedIndex(index >= 0 ? index : 0);
        }
    }
}

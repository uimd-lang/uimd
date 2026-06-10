#include "contacts_manager_ui.hpp"
#include "contact_form/contact_form.hpp"

#include "../../dialogs/message_box.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

constexpr std::size_t kExpectedLayoutEntries = 11;
constexpr std::size_t kExpectedFormLayoutEntries = 21;
constexpr std::string_view kDataPathEnv = "UI_CONTACTS_MANAGER_DATA_PATH";
constexpr std::array<std::string_view, 10> kCsvColumns = {
    "id", "first_name", "last_name", "email", "phone", "company", "city", "category", "active", "notes",
};

struct Contact
{
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string company;
    std::string city;
    std::string category;
    bool active = true;
    std::string notes;
};

std::filesystem::path dataPath()
{
    const std::string key{kDataPathEnv};
    const char* value = std::getenv(key.c_str());
    if (value != nullptr && !std::string_view(value).empty())
    {
        return std::filesystem::path(value);
    }
    return std::filesystem::path(__FILE__).parent_path() / "data" / "contacts.csv";
}

std::vector<std::string> splitCsvLine(std::string_view line)
{
    std::vector<std::string> cells;
    std::string cell;
    const auto pushCell = [&]()
    {
        if (!cell.empty() && cell.back() == '\r')
        {
            cell.pop_back();
        }
        cells.push_back(cell);
        cell.clear();
    };
    bool quoted = false;
    for (char ch : line)
    {
        if (ch == '"')
        {
            quoted = !quoted;
        }
        else if (ch == ',' && !quoted)
        {
            pushCell();
        }
        else
        {
            cell.push_back(ch);
        }
    }
    pushCell();
    return cells;
}

std::vector<Contact> loadContacts()
{
    std::ifstream input(dataPath());
    std::vector<Contact> contacts;
    std::string line;
    bool header = true;
    while (std::getline(input, line))
    {
        if (header)
        {
            header = false;
            continue;
        }
        const std::vector<std::string> row = splitCsvLine(line);
        if (row.size() < 10)
        {
            continue;
        }
        contacts.push_back(Contact{
            .id = row[0],
            .firstName = row[1],
            .lastName = row[2],
            .email = row[3],
            .phone = row[4],
            .company = row[5],
            .city = row[6],
            .category = row[7],
            .active = row[8] == "1" || row[8] == "true",
            .notes = row[9],
        });
    }
    return contacts;
}

void saveContacts(const std::vector<Contact>& contacts)
{
    std::ofstream output(dataPath());
    for (std::size_t index = 0; index < kCsvColumns.size(); ++index)
    {
        if (index > 0)
        {
            output << ',';
        }
        output << kCsvColumns[index];
    }
    output << '\n';
    for (const Contact& contact : contacts)
    {
        output << contact.id << ','
               << contact.firstName << ','
               << contact.lastName << ','
               << contact.email << ','
               << contact.phone << ','
               << contact.company << ','
               << contact.city << ','
               << contact.category << ','
               << (contact.active ? "true" : "false") << ','
               << contact.notes << '\n';
    }
}

Contact fromRecord(const contacts_manager_example::ContactRecord& record)
{
    const auto get = [&](const std::string& key)
    {
        const auto it = record.find(key);
        return it == record.end() ? std::string{} : it->second;
    };
    const std::string active = get("active");
    return Contact{
        .id = get("id"),
        .firstName = get("first_name"),
        .lastName = get("last_name"),
        .email = get("email"),
        .phone = get("phone"),
        .company = get("company"),
        .city = get("city"),
        .category = get("category"),
        .active = active == "1" || active == "true" || active == "yes" || active == "y",
        .notes = get("notes"),
    };
}

contacts_manager_example::ContactRecord toRecord(const Contact& contact)
{
    return {
        {"id", contact.id},
        {"first_name", contact.firstName},
        {"last_name", contact.lastName},
        {"email", contact.email},
        {"phone", contact.phone},
        {"company", contact.company},
        {"city", contact.city},
        {"category", contact.category},
        {"active", contact.active ? "true" : "false"},
        {"notes", contact.notes},
    };
}

std::string rowLabel(const Contact& contact)
{
    std::string label = contact.firstName + " " + contact.lastName;
    const auto first = std::find_if_not(label.begin(), label.end(), [](unsigned char ch)
    {
        return std::isspace(ch);
    });
    const auto last = std::find_if_not(label.rbegin(), label.rend(), [](unsigned char ch)
    {
        return std::isspace(ch);
    }).base();
    return first >= last ? std::string{} : std::string(first, last);
}

std::string detailText(const Contact& contact)
{
    return contact.firstName + " " + contact.lastName +
           "\nEmail: " + contact.email +
           "\nPhone: " + contact.phone +
           "\nCompany: " + contact.company +
           "\nCity: " + contact.city +
           "\nCategory: " + contact.category + " (" + std::string(contact.active ? "active" : "inactive") + ")" +
           "\n\n" + contact.notes;
}

class ContactsManagerApp : public ContactsManagerUI
{
public:
    ContactsManagerApp()
    {
        reload();
    }

    [[nodiscard]] ContactsManagerUI& window()
    {
        return *this;
    }

    [[nodiscard]] ui::GeneratedWindowStack& modalStack()
    {
        return modalStack_;
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = ContactsManagerUI::runtimeOptions();
        options.windowStack = &modalStack_;
        return options;
    }

protected:
    void onContactsSelectionChange(const std::vector<std::string>&) override
    {
        refreshDetail();
    }
    void onReloadBtnClick() override
    {
        reload();
    }
    void onAddBtnClick() override
    {
        addContact();
    }
    void onEditBtnClick() override
    {
        editSelected();
    }
    void onSaveBtnClick() override
    {
        save();
    }
    void onDeleteBtnClick() override
    {
        confirmDeleteSelected();
    }
    void onCloseBtnClick() override
    {
        finished_ = true;
    }

    bool shouldClose() const override
    {
        return finished_;
    }

public:
    void reload()
    {
        contacts_ = loadContacts();
        dirty_ = false;
        refreshList(selectedId());
        setStatus("Loaded " + std::to_string(contacts_.size()) + " contacts.");
    }

    void save()
    {
        saveContacts(contacts_);
        dirty_ = false;
        setStatus("Saved " + std::to_string(contacts_.size()) + " contacts.");
    }

    void refreshDetail()
    {
        const Contact* contact = selectedContact();
        if (contact == nullptr)
        {
            detail->setText("No contact selected.");
            return;
        }
        detail->setText(detailText(*contact));
    }

    void addContact()
    {
        openForm(nullptr);
    }

    void editSelected()
    {
        const Contact* contact = selectedContact();
        if (contact == nullptr)
        {
            setStatus("Select a contact to edit.");
            return;
        }
        openForm(contact);
    }

    void confirmDeleteSelected()
    {
        const Contact* contact = selectedContact();
        if (contact == nullptr)
        {
            setStatus("Select a contact to delete.");
            return;
        }
        deleteContactId_ = contact->id;
        deleteDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNo>(
            "Delete Contact",
            "Delete " + rowLabel(*contact) + "?");
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNo";
        frame.initialFocusName = "no_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                closeDeleteDialog("yes");
            }
            else if (name == "no_btn")
            {
                closeDeleteDialog("no");
            }
        };
        frame.onKey = [this](std::string_view key)
        {
            if (key == "Escape")
            {
                closeDeleteDialog("no");
                return true;
            }
            return false;
        };
        modalStack_.push(*deleteDialog_, std::move(frame));
    }

    void onFormClosed(contacts_manager_example::ContactRecord result)
    {
        if (result.empty())
        {
            setStatus("Edit cancelled.");
            return;
        }
        if (result["id"].empty())
        {
            result["id"] = std::to_string(nextId());
            contacts_.push_back(fromRecord(result));
            setStatus("Contact added.");
            dirty_ = true;
            refreshList(result["id"]);
            return;
        }
        for (Contact& contact : contacts_)
        {
            if (contact.id == result["id"])
            {
                contact = fromRecord(result);
                setStatus("Contact updated.");
                dirty_ = true;
                refreshList(result["id"]);
                return;
            }
        }
        setStatus("Contact no longer exists.");
    }

    void addContactForTest(std::string firstName, std::string lastName, std::string email)
    {
        contacts_manager_example::ContactForm form({}, [this](contacts_manager_example::ContactRecord result)
        {
            onFormClosed(std::move(result));
        });
        form.first_name->setValue(std::move(firstName));
        form.last_name->setValue(std::move(lastName));
        form.email->setValue(std::move(email));
        (void)form.submit();
    }

    void editSelectedForTest(std::string email)
    {
        const Contact* contact = selectedContact();
        if (contact == nullptr)
        {
            setStatus("Select a contact to edit.");
            return;
        }
        contacts_manager_example::ContactForm form(toRecord(*contact), [this](contacts_manager_example::ContactRecord result)
        {
            onFormClosed(std::move(result));
        });
        form.email->setValue(std::move(email));
        (void)form.submit();
    }

    void deleteConfirmed(std::string contactId, std::string result)
    {
        if (result != "yes")
        {
            setStatus("Delete cancelled.");
            return;
        }
        const std::size_t before = contacts_.size();
        contacts_.erase(std::remove_if(contacts_.begin(), contacts_.end(), [&](const Contact& contact)
        {
            return contact.id == contactId;
        }), contacts_.end());
        if (contacts_.size() == before)
        {
            setStatus("Contact no longer exists.");
            return;
        }
        dirty_ = true;
        refreshList("");
        setStatus("Contact deleted.");
    }

private:
    void openForm(const Contact* contact)
    {
        form_ = std::make_unique<contacts_manager_example::ContactForm>(contact == nullptr
            ? contacts_manager_example::ContactRecord{}
            : toRecord(*contact));
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "ContactForm";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "save_btn")
            {
                submitForm();
            }
            else if (name == "cancel_btn")
            {
                cancelForm();
            }
        };
        modalStack_.push(*form_, std::move(frame));
    }

    void submitForm()
    {
        if (form_ == nullptr)
        {
            return;
        }
        if (!form_->submit())
        {
            return;
        }
        contacts_manager_example::ContactRecord result = form_->result();
        modalStack_.remove(*form_);
        form_.reset();
        onFormClosed(std::move(result));
    }

    void cancelForm()
    {
        if (form_ == nullptr)
        {
            return;
        }
        form_->cancel();
        modalStack_.remove(*form_);
        form_.reset();
        onFormClosed({});
    }

    void closeDeleteDialog(std::string result)
    {
        if (deleteDialog_ != nullptr)
        {
            modalStack_.remove(*deleteDialog_);
        }
        deleteDialog_.reset();
        const std::string contactId = std::move(deleteContactId_);
        deleteContactId_.clear();
        deleteConfirmed(contactId, std::move(result));
    }

    void refreshList(const std::string& selectedContactId)
    {
        std::vector<std::string> labels;
        labels.reserve(contacts_.size());
        for (const Contact& item : contacts_)
        {
            labels.push_back(rowLabel(item));
        }
        header->setText("Contacts Manager - " + std::to_string(labels.size()) + " records");
        contacts->setOptions(std::move(labels));
        int selectedIndex = 0;
        if (!selectedContactId.empty())
        {
            for (std::size_t index = 0; index < contacts_.size(); ++index)
            {
                if (contacts_[index].id == selectedContactId)
                {
                    selectedIndex = static_cast<int>(index);
                    break;
                }
            }
        }
        contacts->setSelectedIndex(selectedIndex);
        refreshDetail();
    }

    [[nodiscard]] const Contact* selectedContact() const
    {
        if (contacts_.empty())
        {
            return nullptr;
        }
        const int index = std::max(0, std::min(contacts->selectedIndex(), static_cast<int>(contacts_.size()) - 1));
        return &contacts_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] std::string selectedId() const
    {
        const Contact* contact = selectedContact();
        return contact == nullptr ? std::string{} : contact->id;
    }

    [[nodiscard]] int nextId() const
    {
        int result = 1;
        for (const Contact& contact : contacts_)
        {
            result = std::max(result, std::stoi(contact.id) + 1);
        }
        return result;
    }

    void setStatus(std::string text)
    {
        status->setText(text + (dirty_ ? " *" : ""));
    }

    ui::GeneratedWindowStack modalStack_;
    std::vector<Contact> contacts_;
    std::unique_ptr<contacts_manager_example::ContactForm> form_;
    std::unique_ptr<ui::dialogs::MessageBoxYesNo> deleteDialog_;
    std::string deleteContactId_;
    bool finished_ = false;
    bool dirty_ = false;
};

void smoke()
{
    ContactsManagerApp app;
    ContactsManagerUI& window = app.window();
    contacts_manager_example::ContactForm form;
    assert(window.title() == "Contacts Manager");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(form.generatedLayout().size() == kExpectedFormLayoutEntries);
    assert(window.contacts != nullptr);
    assert(window.detail != nullptr);
    assert(window.add_btn != nullptr);
    assert(window.close_btn != nullptr);
    assert(!window.contacts->options().empty());
    assert(window.header->text().find("records") != std::string::npos);
    assert(window.detail->text().find('\r') == std::string::npos);
    const ui::Size contentSize = ui::generatedWindowContentSize(window);
    (void)ui::renderGeneratedWindowContent(window, contentSize);
    assert(window.contacts->frame().height == static_cast<int>(window.contacts->options().size()));
    const ui::Size widerContentSize{contentSize.width + window.close_btn->frame().width, contentSize.height};
    (void)ui::renderGeneratedWindowContent(window, widerContentSize);
    assert(window.close_btn->frame().col + window.close_btn->frame().width == widerContentSize.width);
}

void logicTest()
{
    ContactsManagerApp app;
    ContactsManagerUI& window = app.window();
    const std::size_t initialSize = window.contacts->options().size();
    assert(initialSize > 0);
    window.contacts->setSelectedIndex(0);
    app.refreshDetail();
    assert(window.detail->text().find("Email:") != std::string::npos);
    app.addContactForTest("New", "Contact", "new@example.com");
    assert(window.contacts->options().size() == initialSize + 1);
    assert(window.status->text() == "Contact added.");
    app.editSelectedForTest("updated@example.com");
    assert(window.status->text() == "Contact updated. *");
    const std::size_t editedSize = window.contacts->options().size();
    app.deleteConfirmed(window.contacts->options().empty() ? "" : std::to_string(999999), "no");
    assert(window.contacts->options().size() == editedSize);
}

}  // namespace

int main(int argc, char** argv)
{
    ContactsManagerApp app;
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        logicTest();
        return 0;
    }

    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

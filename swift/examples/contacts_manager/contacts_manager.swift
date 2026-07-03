import Foundation
import Uimd

private let kExpectedLayoutEntries = 11
private let kExpectedFormLayoutEntries = 21
private let kCategoryDefault = "Client"
private let kDataPathEnv = "UI_CONTACTS_MANAGER_DATA_PATH"
private let kCsvColumns = [
    "id",
    "first_name",
    "last_name",
    "email",
    "phone",
    "company",
    "city",
    "category",
    "active",
    "notes",
]

private struct Contact
{
    var id = ""
    var firstName = ""
    var lastName = ""
    var email = ""
    var phone = ""
    var company = ""
    var city = ""
    var category = ""
    var active = true
    var notes = ""
}

private typealias ContactRecord = [String: String]

private func envString(_ name: String) -> String?
{
    let value = ProcessInfo.processInfo.environment[name] ?? ""
    return value.isEmpty ? nil : value
}

private func dataPath() -> String
{
    if let path = envString(kDataPathEnv)
    {
        return path
    }
    return URL(fileURLWithPath: #filePath)
        .deletingLastPathComponent()
        .appendingPathComponent("data")
        .appendingPathComponent("contacts.csv")
        .standardizedFileURL
        .path
}

private func splitCsvLine(_ line: String) -> [String]
{
    var cells: [String] = []
    var cell = ""
    var quoted = false
    for ch in line
    {
        if ch == "\""
        {
            quoted.toggle()
        }
        else if ch == "," && !quoted
        {
            cells.append(cell.trimmingCharacters(in: CharacterSet(charactersIn: "\r")))
            cell = ""
        }
        else
        {
            cell.append(ch)
        }
    }
    cells.append(cell.trimmingCharacters(in: CharacterSet(charactersIn: "\r")))
    return cells
}

private func toBool(_ value: String) -> Bool
{
    let normalized = value.trimmingCharacters(in: .whitespacesAndNewlines).lowercased()
    return normalized == "1" || normalized == "true" || normalized == "yes" || normalized == "y"
}

private func loadContacts() -> [Contact]
{
    guard let text = try? String(contentsOfFile: dataPath(), encoding: .utf8) else
    {
        return []
    }
    var contacts: [Contact] = []
    for (index, line) in text.components(separatedBy: "\n").enumerated()
    {
        if index == 0 || line.isEmpty
        {
            continue
        }
        let row = splitCsvLine(line)
        if row.count < kCsvColumns.count
        {
            continue
        }
        contacts.append(Contact(
            id: row[0],
            firstName: row[1],
            lastName: row[2],
            email: row[3],
            phone: row[4],
            company: row[5],
            city: row[6],
            category: row[7],
            active: toBool(row[8]),
            notes: row[9]
        ))
    }
    return contacts
}

private func saveContacts(_ contacts: [Contact])
{
    let path = dataPath()
    let parent = URL(fileURLWithPath: path).deletingLastPathComponent().path
    try? FileManager.default.createDirectory(atPath: parent, withIntermediateDirectories: true)
    var lines = [kCsvColumns.joined(separator: ",")]
    for contact in contacts
    {
        lines.append([
            contact.id,
            contact.firstName,
            contact.lastName,
            contact.email,
            contact.phone,
            contact.company,
            contact.city,
            contact.category,
            contact.active ? "true" : "false",
            contact.notes,
        ].joined(separator: ","))
    }
    try? (lines.joined(separator: "\n") + "\n").write(toFile: path, atomically: true, encoding: .utf8)
}

private func contactFromRecord(_ record: ContactRecord) -> Contact
{
    Contact(
        id: record["id"] ?? "",
        firstName: record["first_name"] ?? "",
        lastName: record["last_name"] ?? "",
        email: record["email"] ?? "",
        phone: record["phone"] ?? "",
        company: record["company"] ?? "",
        city: record["city"] ?? "",
        category: record["category"] ?? "",
        active: toBool(record["active"] ?? "true"),
        notes: record["notes"] ?? ""
    )
}

private func recordFromContact(_ contact: Contact) -> ContactRecord
{
    [
        "id": contact.id,
        "first_name": contact.firstName,
        "last_name": contact.lastName,
        "email": contact.email,
        "phone": contact.phone,
        "company": contact.company,
        "city": contact.city,
        "category": contact.category,
        "active": contact.active ? "true" : "false",
        "notes": contact.notes,
    ]
}

private func rowLabel(_ contact: Contact) -> String
{
    (contact.firstName + " " + contact.lastName).trimmingCharacters(in: .whitespacesAndNewlines)
}

private func detailText(_ contact: Contact) -> String
{
    contact.firstName + " " + contact.lastName
        + "\nEmail: " + contact.email
        + "\nPhone: " + contact.phone
        + "\nCompany: " + contact.company
        + "\nCity: " + contact.city
        + "\nCategory: " + contact.category + " (" + (contact.active ? "active" : "inactive") + ")"
        + "\n\n" + contact.notes
}

private final class MessageBoxYesNo: MessageBoxYesNoUI
{
    init(_ header: String, _ text: String)
    {
        super.init()
        dialog_header.setText(header)
        message.setText(text)
    }
}

private final class ContactForm: ContactFormUI
{
    private var contact: ContactRecord
    private(set) var result: ContactRecord = [:]

    init(_ contact: ContactRecord = [:])
    {
        self.contact = contact
        super.init()
        loadContact(contact)
    }

    override func onSaveBtnClick()
    {
        _ = submit()
    }

    override func onCancelBtnClick()
    {
        cancel()
    }

    func submit() -> Bool
    {
        if let error = validationError()
        {
            message.setText(error)
            return false
        }
        var updated = contact
        updated["first_name"] = first_name.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["last_name"] = last_name.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["email"] = email.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["phone"] = phone.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["company"] = company.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["city"] = city.value.trimmingCharacters(in: .whitespacesAndNewlines)
        updated["category"] = selectedCategory()
        updated["active"] = active.checked ? "true" : "false"
        updated["notes"] = notes.value.trimmingCharacters(in: .whitespacesAndNewlines)
        result = updated
        return true
    }

    func cancel()
    {
        result = [:]
    }

    private func loadContact(_ contact: ContactRecord)
    {
        first_name.setValue(contact["first_name"] ?? "")
        last_name.setValue(contact["last_name"] ?? "")
        email.setValue(contact["email"] ?? "")
        phone.setValue(contact["phone"] ?? "")
        company.setValue(contact["company"] ?? "")
        city.setValue(contact["city"] ?? "")
        category.selectValue(contact["category"] ?? kCategoryDefault)
        active.setChecked(toBool(contact["active"] ?? "true"))
        notes.setValue(contact["notes"] ?? "")
    }

    private func selectedCategory() -> String
    {
        category.selectedIndex >= 0 && category.selectedIndex < category.options.count
            ? category.options[category.selectedIndex]
            : kCategoryDefault
    }

    private func validationError() -> String?
    {
        if first_name.value.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty
        {
            return "First name is required."
        }
        if last_name.value.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty
        {
            return "Last name is required."
        }
        let value = email.value.trimmingCharacters(in: .whitespacesAndNewlines)
        if value.isEmpty
        {
            return "Email is required."
        }
        guard let at = value.firstIndex(of: "@"), at != value.startIndex else
        {
            return "Email must look like name@example.com."
        }
        let domain = value[value.index(after: at)...]
        if !domain.contains(".")
        {
            return "Email must look like name@example.com."
        }
        return nil
    }
}

public final class ContactsManagerApp: ContactsManagerUI
{
    private let modalStack = GeneratedWindowStack()
    private var contactsModel: [Contact] = []
    private var form: ContactForm?
    private var deleteDialog: MessageBoxYesNo?
    private var deleteContactId = ""
    private var finished = false
    private var dirty = false

    public override init()
    {
        super.init()
        reload()
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.windowStack = modalStack
        return options
    }

    public override func shouldClose() -> Bool
    {
        finished
    }

    override public func onContactsSelectionChange(_ value: [String])
    {
        _ = value
        refreshDetail()
    }

    override public func onAddBtnClick()
    {
        addContact()
    }

    override public func onEditBtnClick()
    {
        editSelected()
    }

    override public func onDeleteBtnClick()
    {
        confirmDeleteSelected()
    }

    override public func onSaveBtnClick()
    {
        save()
    }

    override public func onReloadBtnClick()
    {
        reload()
    }

    override public func onCloseBtnClick()
    {
        finished = true
    }

    func reload()
    {
        contactsModel = loadContacts()
        dirty = false
        refreshList(selectedId())
        setStatus("Loaded " + String(contactsModel.count) + " contacts.")
    }

    func save()
    {
        saveContacts(contactsModel)
        dirty = false
        setStatus("Saved " + String(contactsModel.count) + " contacts.")
    }

    func refreshDetail()
    {
        guard let contact = selectedContact() else
        {
            detail.setText("No contact selected.")
            return
        }
        detail.setText(detailText(contact))
    }

    func addContact()
    {
        openForm(nil)
    }

    func editSelected()
    {
        guard let contact = selectedContact() else
        {
            setStatus("Select a contact to edit.")
            return
        }
        openForm(contact)
    }

    func confirmDeleteSelected()
    {
        guard let contact = selectedContact() else
        {
            setStatus("Select a contact to delete.")
            return
        }
        deleteContactId = contact.id
        deleteDialog = MessageBoxYesNo("Delete Contact", "Delete " + rowLabel(contact) + "?")
        guard let deleteDialog else
        {
            return
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "MessageBoxYesNo"
        frame.initialFocusName = "no_btn"
        frame.onButton = { [weak self] name in
            if name == "yes_btn"
            {
                self?.closeDeleteDialog("yes")
                return true
            }
            if name == "no_btn"
            {
                self?.closeDeleteDialog("no")
                return true
            }
            return false
        }
        frame.onKey = { [weak self] key in
            if key == "Escape"
            {
                self?.closeDeleteDialog("no")
                return true
            }
            return false
        }
        modalStack.push(deleteDialog, frame)
    }

    private func onFormClosed(_ result: ContactRecord)
    {
        if result.isEmpty
        {
            setStatus("Edit cancelled.")
            return
        }
        if (result["id"] ?? "").isEmpty
        {
            var updated = result
            updated["id"] = String(nextId())
            contactsModel.append(contactFromRecord(updated))
            setStatus("Contact added.")
            dirty = true
            refreshList(updated["id"] ?? "")
            return
        }
        for index in contactsModel.indices
        {
            if contactsModel[index].id == result["id"]
            {
                contactsModel[index] = contactFromRecord(result)
                setStatus("Contact updated.")
                dirty = true
                refreshList(result["id"] ?? "")
                return
            }
        }
        setStatus("Contact no longer exists.")
    }

    func addContactForTest(firstName: String, lastName: String, email: String)
    {
        let form = ContactForm()
        form.first_name.setValue(firstName)
        form.last_name.setValue(lastName)
        form.email.setValue(email)
        if form.submit()
        {
            onFormClosed(form.result)
        }
    }

    func editSelectedForTest(email: String)
    {
        guard let contact = selectedContact() else
        {
            setStatus("Select a contact to edit.")
            return
        }
        let form = ContactForm(recordFromContact(contact))
        form.email.setValue(email)
        if form.submit()
        {
            onFormClosed(form.result)
        }
    }

    func deleteConfirmed(contactId: String, result: String)
    {
        if result != "yes"
        {
            setStatus("Delete cancelled.")
            return
        }
        let before = contactsModel.count
        contactsModel.removeAll { $0.id == contactId }
        if contactsModel.count == before
        {
            setStatus("Contact no longer exists.")
            return
        }
        dirty = true
        refreshList("")
        setStatus("Contact deleted.")
    }

    private func openForm(_ contact: Contact?)
    {
        form = ContactForm(contact.map(recordFromContact) ?? [:])
        guard let form else
        {
            return
        }
        var frame = GeneratedWindowFrameOptions()
        frame.className = "ContactForm"
        frame.onButton = { [weak self] name in
            if name == "save_btn"
            {
                self?.submitForm()
                return true
            }
            if name == "cancel_btn"
            {
                self?.cancelForm()
                return true
            }
            return false
        }
        modalStack.push(form, frame)
    }

    private func submitForm()
    {
        guard let form else
        {
            return
        }
        if !form.submit()
        {
            return
        }
        let result = form.result
        modalStack.remove(form)
        self.form = nil
        onFormClosed(result)
    }

    private func cancelForm()
    {
        guard let form else
        {
            return
        }
        form.cancel()
        modalStack.remove(form)
        self.form = nil
        onFormClosed([:])
    }

    private func closeDeleteDialog(_ result: String)
    {
        if let deleteDialog
        {
            modalStack.remove(deleteDialog)
        }
        deleteDialog = nil
        let contactId = deleteContactId
        deleteContactId = ""
        deleteConfirmed(contactId: contactId, result: result)
    }

    private func refreshList(_ selectedContactId: String)
    {
        let labels = contactsModel.map(rowLabel)
        header.setText("Contacts Manager - " + String(labels.count) + " records")
        contacts.options = labels
        if labels.isEmpty
        {
            contacts.selectedIndex = -1
            contacts.selectedValues = []
            refreshDetail()
            return
        }
        var selectedIndex = 0
        if !selectedContactId.isEmpty,
           let index = contactsModel.firstIndex(where: { $0.id == selectedContactId })
        {
            selectedIndex = index
        }
        contacts.selectedIndex = selectedIndex
        contacts.selectedValues = [labels[selectedIndex]]
        refreshDetail()
    }

    private func selectedContact() -> Contact?
    {
        if contactsModel.isEmpty
        {
            return nil
        }
        let index = min(max(0, contacts.selectedIndex), contactsModel.count - 1)
        return contactsModel[index]
    }

    private func selectedId() -> String
    {
        selectedContact()?.id ?? ""
    }

    private func nextId() -> Int
    {
        contactsModel.reduce(1) { result, contact in
            max(result, (Int(contact.id) ?? 0) + 1)
        }
    }

    private func setStatus(_ text: String)
    {
        status.setText(text + (dirty ? " *" : ""))
    }
}

private func smoke()
{
    let app = ContactsManagerApp()
    let form = ContactForm()
    precondition(app.windowTitle == "Contacts Manager")
    precondition(app.layout.count == kExpectedLayoutEntries)
    precondition(form.layout.count == kExpectedFormLayoutEntries)
    precondition(app.contacts != nil)
    precondition(app.detail != nil)
    precondition(app.add_btn != nil)
    precondition(app.close_btn != nil)
    precondition(!app.contacts.options.isEmpty)
    precondition(app.header.text.contains("records"))
    precondition(!app.detail.text.contains("\r"))
    let contentSize = app.generatedContentSize()
    _ = app.renderContent(size: contentSize, focusedName: nil, editMode: false)
    precondition(app.contacts.frame.height == app.contacts.options.count)
    let widerContentSize = Size(width: contentSize.width + app.close_btn.frame.width, height: contentSize.height)
    _ = app.renderContent(size: widerContentSize, focusedName: nil, editMode: false)
    precondition(app.close_btn.frame.col + app.close_btn.frame.width == widerContentSize.width)
}

private func logicTest()
{
    let app = ContactsManagerApp()
    let initialSize = app.contacts.options.count
    precondition(initialSize > 0)
    app.contacts.selectedIndex = 0
    app.refreshDetail()
    precondition(app.detail.text.contains("Email:"))
    app.addContactForTest(firstName: "New", lastName: "Contact", email: "new@example.com")
    precondition(app.contacts.options.count == initialSize + 1)
    precondition(app.status.text == "Contact added.")
    app.editSelectedForTest(email: "updated@example.com")
    precondition(app.status.text == "Contact updated. *")
    let editedSize = app.contacts.options.count
    app.deleteConfirmed(contactId: "999999", result: "no")
    precondition(app.contacts.options.count == editedSize)
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            if CommandLine.arguments.contains("--smoke")
            {
                smoke()
                return 0
            }
            if CommandLine.arguments.contains("--logic-test")
            {
                logicTest()
                return 0
            }
            let app = ContactsManagerApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}

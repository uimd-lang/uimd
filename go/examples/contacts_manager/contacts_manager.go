package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	contactform "uimd_examples/contacts_manager/contact_form"

	uimd "uimd"
)

const contactsDataPathEnv = "UI_CONTACTS_MANAGER_DATA_PATH"

var contactCsvColumns = []string{
	"id", "first_name", "last_name", "email", "phone", "company", "city", "category", "active", "notes",
}

type Contact struct {
	ID        string
	FirstName string
	LastName  string
	Email     string
	Phone     string
	Company   string
	City      string
	Category  string
	Active    bool
	Notes     string
}

type ContactsManagerApp struct {
	*ContactsManagerUI
	stack           uimd.GeneratedWindowStack
	contacts        []Contact
	form            *contactform.ContactForm
	deleteDialog    *uimd.MessageBoxYesNo
	deleteContactID string
	finished        bool
	dirty           bool
}

func NewContactsManagerApp() *ContactsManagerApp {
	app := &ContactsManagerApp{ContactsManagerUI: NewContactsManagerUI()}
	app.SetEventHandler(app)
	app.Reload()
	return app
}

func (app *ContactsManagerApp) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.ContactsManagerUI.RuntimeOptions()
	options.WindowStack = &app.stack
	options.ShouldClose = app.ShouldClose
	return options
}

func (app *ContactsManagerApp) ShouldClose() bool {
	return app.finished || app.GeneratedWindowBase.ShouldClose()
}

func (app *ContactsManagerApp) OnContactsSelectionChange([]string) {
	app.RefreshDetail()
}

func (app *ContactsManagerApp) OnReloadBtnClick() {
	app.Reload()
}

func (app *ContactsManagerApp) OnAddBtnClick() {
	app.AddContact()
}

func (app *ContactsManagerApp) OnEditBtnClick() {
	app.EditSelected()
}

func (app *ContactsManagerApp) OnSaveBtnClick() {
	app.Save()
}

func (app *ContactsManagerApp) OnDeleteBtnClick() {
	app.ConfirmDeleteSelected()
}

func (app *ContactsManagerApp) OnCloseBtnClick() {
	app.finished = true
	app.RequestClose()
}

func (app *ContactsManagerApp) Reload() {
	app.contacts = loadContacts()
	app.dirty = false
	app.refreshList(app.selectedID())
	app.setStatus(fmt.Sprintf("Loaded %d contacts.", len(app.contacts)))
}

func (app *ContactsManagerApp) Save() {
	saveContacts(app.contacts)
	app.dirty = false
	app.setStatus(fmt.Sprintf("Saved %d contacts.", len(app.contacts)))
}

func (app *ContactsManagerApp) RefreshDetail() {
	contact := app.selectedContact()
	if contact == nil {
		app.Detail.SetText("No contact selected.")
		return
	}
	app.Detail.SetText(detailText(*contact))
}

func (app *ContactsManagerApp) AddContact() {
	app.openForm(nil)
}

func (app *ContactsManagerApp) EditSelected() {
	contact := app.selectedContact()
	if contact == nil {
		app.setStatus("Select a contact to edit.")
		return
	}
	app.openForm(contact)
}

func (app *ContactsManagerApp) ConfirmDeleteSelected() {
	contact := app.selectedContact()
	if contact == nil {
		app.setStatus("Select a contact to delete.")
		return
	}
	app.deleteContactID = contact.ID
	app.deleteDialog = uimd.NewMessageBoxYesNo("Delete Contact", "Delete "+rowLabel(*contact)+"?")
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName:        "MessageBoxYesNo",
		InitialFocusName: "no_btn",
		OnButton: func(name string) {
			switch name {
			case "yes_btn":
				app.closeDeleteDialog("yes")
			case "no_btn":
				app.closeDeleteDialog("no")
			}
		},
		OnKey: func(key string) bool {
			if key == "Escape" {
				app.closeDeleteDialog("no")
				return true
			}
			return false
		},
	}
	app.stack.Push(app.deleteDialog, frame)
}

func (app *ContactsManagerApp) OnFormClosed(result contactform.ContactRecord) {
	if len(result) == 0 {
		app.setStatus("Edit cancelled.")
		return
	}
	if result["id"] == "" {
		result["id"] = strconv.Itoa(app.nextID())
		app.contacts = append(app.contacts, contactFromRecord(result))
		app.setStatus("Contact added.")
		app.dirty = true
		app.refreshList(result["id"])
		return
	}
	for index := range app.contacts {
		if app.contacts[index].ID == result["id"] {
			app.contacts[index] = contactFromRecord(result)
			app.setStatus("Contact updated.")
			app.dirty = true
			app.refreshList(result["id"])
			return
		}
	}
	app.setStatus("Contact no longer exists.")
}

func (app *ContactsManagerApp) AddContactForTest(firstName string, lastName string, email string) {
	form := contactform.NewContactForm(contactform.ContactRecord{}, func(result contactform.ContactRecord) {
		app.OnFormClosed(result)
	})
	form.FirstName.SetValue(firstName)
	form.LastName.SetValue(lastName)
	form.Email.SetValue(email)
	form.Submit()
}

func (app *ContactsManagerApp) EditSelectedForTest(email string) {
	contact := app.selectedContact()
	if contact == nil {
		app.setStatus("Select a contact to edit.")
		return
	}
	form := contactform.NewContactForm(recordFromContact(*contact), func(result contactform.ContactRecord) {
		app.OnFormClosed(result)
	})
	form.Email.SetValue(email)
	form.Submit()
}

func (app *ContactsManagerApp) DeleteConfirmed(contactID string, result string) {
	if result != "yes" {
		app.setStatus("Delete cancelled.")
		return
	}
	before := len(app.contacts)
	filtered := app.contacts[:0]
	for _, contact := range app.contacts {
		if contact.ID != contactID {
			filtered = append(filtered, contact)
		}
	}
	app.contacts = filtered
	if len(app.contacts) == before {
		app.setStatus("Contact no longer exists.")
		return
	}
	app.dirty = true
	app.refreshList("")
	app.setStatus("Contact deleted.")
}

func (app *ContactsManagerApp) openForm(contact *Contact) {
	record := contactform.ContactRecord{}
	if contact != nil {
		record = recordFromContact(*contact)
	}
	app.form = contactform.NewContactForm(record, nil)
	frame := uimd.GeneratedWindowFrameOptions{
		ClassName: "ContactForm",
		OnButton: func(name string) {
			switch name {
			case "save_btn":
				app.submitForm()
			case "cancel_btn":
				app.cancelForm()
			}
		},
	}
	app.stack.Push(app.form.GeneratedWindowBase, frame)
}

func (app *ContactsManagerApp) submitForm() {
	if app.form == nil {
		return
	}
	if !app.form.Submit() {
		return
	}
	result := app.form.Result()
	app.stack.Remove(app.form.GeneratedWindowBase)
	app.form = nil
	app.OnFormClosed(result)
}

func (app *ContactsManagerApp) cancelForm() {
	if app.form == nil {
		return
	}
	app.form.Cancel()
	app.stack.Remove(app.form.GeneratedWindowBase)
	app.form = nil
	app.OnFormClosed(contactform.ContactRecord{})
}

func (app *ContactsManagerApp) closeDeleteDialog(result string) {
	if app.deleteDialog != nil {
		app.stack.Remove(app.deleteDialog)
	}
	app.deleteDialog = nil
	contactID := app.deleteContactID
	app.deleteContactID = ""
	app.DeleteConfirmed(contactID, result)
}

func (app *ContactsManagerApp) refreshList(selectedContactID string) {
	labels := make([]string, 0, len(app.contacts))
	for _, contact := range app.contacts {
		labels = append(labels, rowLabel(contact))
	}
	app.Header.SetText(fmt.Sprintf("Contacts Manager - %d records", len(labels)))
	app.Contacts.SetOptions(labels)
	selectedIndex := 0
	if selectedContactID != "" {
		for index, contact := range app.contacts {
			if contact.ID == selectedContactID {
				selectedIndex = index
				break
			}
		}
	}
	app.Contacts.SetSelectedIndex(selectedIndex)
	app.RefreshDetail()
}

func (app *ContactsManagerApp) selectedContact() *Contact {
	if len(app.contacts) == 0 {
		return nil
	}
	index := app.Contacts.SelectedIndex
	if index < 0 {
		index = 0
	}
	if index >= len(app.contacts) {
		index = len(app.contacts) - 1
	}
	return &app.contacts[index]
}

func (app *ContactsManagerApp) selectedID() string {
	contact := app.selectedContact()
	if contact == nil {
		return ""
	}
	return contact.ID
}

func (app *ContactsManagerApp) nextID() int {
	result := 1
	for _, contact := range app.contacts {
		value, err := strconv.Atoi(contact.ID)
		if err == nil && value >= result {
			result = value + 1
		}
	}
	return result
}

func (app *ContactsManagerApp) setStatus(text string) {
	if app.dirty {
		text += " *"
	}
	app.Status.SetText(text)
}

func dataPath() string {
	if value := os.Getenv(contactsDataPathEnv); value != "" {
		return value
	}
	return filepath.Join("data", "contacts.csv")
}

func loadContacts() []Contact {
	file, err := os.Open(dataPath())
	if err != nil {
		return []Contact{}
	}
	defer file.Close()
	reader := csv.NewReader(file)
	records, err := reader.ReadAll()
	if err != nil || len(records) <= 1 {
		return []Contact{}
	}
	contacts := make([]Contact, 0, len(records)-1)
	for _, row := range records[1:] {
		if len(row) < len(contactCsvColumns) {
			continue
		}
		contacts = append(contacts, Contact{
			ID:        row[0],
			FirstName: row[1],
			LastName:  row[2],
			Email:     row[3],
			Phone:     row[4],
			Company:   row[5],
			City:      row[6],
			Category:  row[7],
			Active:    contactformBool(row[8]),
			Notes:     row[9],
		})
	}
	return contacts
}

func saveContacts(contacts []Contact) {
	_ = os.MkdirAll(filepath.Dir(dataPath()), 0o755)
	file, err := os.Create(dataPath())
	if err != nil {
		return
	}
	defer file.Close()
	writer := csv.NewWriter(file)
	_ = writer.Write(contactCsvColumns)
	for _, contact := range contacts {
		active := "false"
		if contact.Active {
			active = "true"
		}
		_ = writer.Write([]string{
			contact.ID, contact.FirstName, contact.LastName, contact.Email, contact.Phone,
			contact.Company, contact.City, contact.Category, active, contact.Notes,
		})
	}
	writer.Flush()
}

func contactFromRecord(record contactform.ContactRecord) Contact {
	return Contact{
		ID:        record["id"],
		FirstName: record["first_name"],
		LastName:  record["last_name"],
		Email:     record["email"],
		Phone:     record["phone"],
		Company:   record["company"],
		City:      record["city"],
		Category:  record["category"],
		Active:    contactformBool(record["active"]),
		Notes:     record["notes"],
	}
}

func recordFromContact(contact Contact) contactform.ContactRecord {
	active := "false"
	if contact.Active {
		active = "true"
	}
	return contactform.ContactRecord{
		"id":         contact.ID,
		"first_name": contact.FirstName,
		"last_name":  contact.LastName,
		"email":      contact.Email,
		"phone":      contact.Phone,
		"company":    contact.Company,
		"city":       contact.City,
		"category":   contact.Category,
		"active":     active,
		"notes":      contact.Notes,
	}
}

func rowLabel(contact Contact) string {
	return strings.TrimSpace(contact.FirstName + " " + contact.LastName)
}

func detailText(contact Contact) string {
	active := "inactive"
	if contact.Active {
		active = "active"
	}
	return contact.FirstName + " " + contact.LastName +
		"\nEmail: " + contact.Email +
		"\nPhone: " + contact.Phone +
		"\nCompany: " + contact.Company +
		"\nCity: " + contact.City +
		"\nCategory: " + contact.Category + " (" + active + ")" +
		"\n\n" + contact.Notes
}

func contactformBool(value string) bool {
	switch strings.ToLower(value) {
	case "1", "true", "yes", "y":
		return true
	default:
		return false
	}
}

func smoke() {
	app := NewContactsManagerApp()
	if app.Title() != "Contacts Manager" {
		panic("unexpected title")
	}
	if app.Contacts == nil || app.Detail == nil || app.AddBtn == nil || app.CloseBtn == nil {
		panic("missing contacts manager elements")
	}
	if len(app.Contacts.Options) == 0 {
		panic("contacts did not load")
	}
}

func logicTest() {
	app := NewContactsManagerApp()
	initialSize := len(app.Contacts.Options)
	if initialSize == 0 {
		panic("contacts did not load")
	}
	app.Contacts.SetSelectedIndex(0)
	app.RefreshDetail()
	if !strings.Contains(app.Detail.Text, "Email:") {
		panic("detail missing email")
	}
	app.AddContactForTest("New", "Contact", "new@example.com")
	if len(app.Contacts.Options) != initialSize+1 {
		panic("contact not added")
	}
	app.EditSelectedForTest("updated@example.com")
	if app.Status.Text != "Contact updated. *" {
		panic("contact not updated")
	}
	editedSize := len(app.Contacts.Options)
	app.DeleteConfirmed("999999", "no")
	if len(app.Contacts.Options) != editedSize {
		panic("delete cancel changed contacts")
	}
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		if len(os.Args) > 1 {
			switch os.Args[1] {
			case "--smoke":
				smoke()
				return 0
			case "--logic-test":
				logicTest()
				return 0
			}
		}
		app := NewContactsManagerApp()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}

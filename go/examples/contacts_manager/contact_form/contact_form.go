package contact_form

import (
	"strings"
)

const contactFormDefaultCategory = "Client"

type ContactRecord map[string]string

type ContactForm struct {
	*ContactFormUI
	contact ContactRecord
	result  ContactRecord
	onClose func(ContactRecord)
}

func NewContactForm(contact ContactRecord, onClose func(ContactRecord)) *ContactForm {
	form := &ContactForm{
		ContactFormUI: NewContactFormUI(),
		contact:       copyContactRecord(contact),
		onClose:       onClose,
	}
	form.SetEventHandler(form)
	form.LoadContact(contact)
	return form
}

func (form *ContactForm) OnSaveBtnClick() {
	form.Submit()
}

func (form *ContactForm) OnCancelBtnClick() {
	form.Cancel()
}

func (form *ContactForm) LoadContact(contact ContactRecord) {
	form.FirstName.SetValue(contactValue(contact, "first_name"))
	form.LastName.SetValue(contactValue(contact, "last_name"))
	form.Email.SetValue(contactValue(contact, "email"))
	form.Phone.SetValue(contactValue(contact, "phone"))
	form.Company.SetValue(contactValue(contact, "company"))
	form.City.SetValue(contactValue(contact, "city"))
	categoryValue := contactValue(contact, "category")
	if categoryValue == "" {
		categoryValue = contactFormDefaultCategory
	}
	form.Category.SetSelectedIndex(indexOfString(form.Category.Options, categoryValue))
	activeValue := contactValue(contact, "active")
	if activeValue == "" {
		activeValue = "true"
	}
	form.Active.SetChecked(contactBool(activeValue))
	form.Notes.SetValue(contactValue(contact, "notes"))
}

func (form *ContactForm) Submit() bool {
	if message := form.validationError(); message != "" {
		form.Message.SetText(message)
		return false
	}
	result := copyContactRecord(form.contact)
	result["first_name"] = strings.TrimSpace(form.FirstName.Value)
	result["last_name"] = strings.TrimSpace(form.LastName.Value)
	result["email"] = strings.TrimSpace(form.Email.Value)
	result["phone"] = strings.TrimSpace(form.Phone.Value)
	result["company"] = strings.TrimSpace(form.Company.Value)
	result["city"] = strings.TrimSpace(form.City.Value)
	result["category"] = contactFormDefaultCategory
	if len(form.Category.Options) > 0 && form.Category.SelectedIndex >= 0 && form.Category.SelectedIndex < len(form.Category.Options) {
		result["category"] = form.Category.Options[form.Category.SelectedIndex]
	}
	result["active"] = "false"
	if form.Active.Checked {
		result["active"] = "true"
	}
	result["notes"] = strings.TrimSpace(form.Notes.Value)
	form.result = result
	if form.onClose != nil {
		form.onClose(result)
	}
	return true
}

func (form *ContactForm) Cancel() {
	form.result = ContactRecord{}
	if form.onClose != nil {
		form.onClose(ContactRecord{})
	}
}

func (form *ContactForm) Result() ContactRecord {
	return copyContactRecord(form.result)
}

func (form *ContactForm) validationError() string {
	if strings.TrimSpace(form.FirstName.Value) == "" {
		return "First name is required."
	}
	if strings.TrimSpace(form.LastName.Value) == "" {
		return "Last name is required."
	}
	email := strings.TrimSpace(form.Email.Value)
	if email == "" {
		return "Email is required."
	}
	at := strings.Index(email, "@")
	if at <= 0 || !strings.Contains(email[at+1:], ".") {
		return "Email must look like name@example.com."
	}
	return ""
}

func contactValue(contact ContactRecord, key string) string {
	if contact == nil {
		return ""
	}
	return contact[key]
}

func copyContactRecord(contact ContactRecord) ContactRecord {
	result := ContactRecord{}
	for key, value := range contact {
		result[key] = value
	}
	return result
}

func indexOfString(values []string, needle string) int {
	for index, value := range values {
		if value == needle {
			return index
		}
	}
	return 0
}

func contactBool(value string) bool {
	switch strings.ToLower(value) {
	case "1", "true", "yes", "y":
		return true
	default:
		return false
	}
}

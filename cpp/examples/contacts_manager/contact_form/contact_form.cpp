#include "contact_form.hpp"

#include <algorithm>
#include <cctype>
#include <string_view>
#include <utility>

namespace contacts_manager_example
{

namespace
{

constexpr std::string_view kCategoryDefault = "Client";

[[nodiscard]] std::string getOrEmpty(const ContactRecord& contact, const std::string& key)
{
    const auto it = contact.find(key);
    return it == contact.end() ? "" : it->second;
}

[[nodiscard]] std::string trim(std::string text)
{
    const auto begin = std::find_if_not(text.begin(), text.end(), [](unsigned char ch)
    {
        return std::isspace(ch);
    });
    const auto end = std::find_if_not(text.rbegin(), text.rend(), [](unsigned char ch)
    {
        return std::isspace(ch);
    }).base();
    if (begin >= end)
    {
        return {};
    }
    return std::string(begin, end);
}

}  // namespace

ContactForm::ContactForm(ContactRecord contact, ContactFormCallback onClose)
    : contact_(std::move(contact)), onClose_(std::move(onClose))
{
    loadContact(contact_);
}


void ContactForm::onSaveBtnClick()
{
    (void)submit();
}

void ContactForm::onCancelBtnClick()
{
    cancel();
}

void ContactForm::loadContact(const ContactRecord& contact)
{
    first_name->setValue(getOrEmpty(contact, "first_name"));
    last_name->setValue(getOrEmpty(contact, "last_name"));
    email->setValue(getOrEmpty(contact, "email"));
    phone->setValue(getOrEmpty(contact, "phone"));
    company->setValue(getOrEmpty(contact, "company"));
    city->setValue(getOrEmpty(contact, "city"));
    const std::string categoryValue = getOrEmpty(contact, "category").empty() ? std::string(kCategoryDefault) : getOrEmpty(contact, "category");
    const auto& options = category->options();
    const auto found = std::find(options.begin(), options.end(), categoryValue);
    category->setSelectedIndex(found == options.end() ? 0 : static_cast<int>(std::distance(options.begin(), found)));
    const std::string activeValue = getOrEmpty(contact, "active");
    active->setChecked(toBool(activeValue.empty() ? "true" : activeValue));
    notes->setValue(getOrEmpty(contact, "notes"));
}

bool ContactForm::submit()
{
    const std::string error = validationError();
    if (!error.empty())
    {
        message->setText(error);
        return false;
    }

    result_ = contact_;
    result_["first_name"] = trim(first_name->value());
    result_["last_name"] = trim(last_name->value());
    result_["email"] = trim(email->value());
    result_["phone"] = trim(phone->value());
    result_["company"] = trim(company->value());
    result_["city"] = trim(city->value());
    const auto& options = category->options();
    result_["category"] = options.empty() ? std::string(kCategoryDefault) : options[static_cast<std::size_t>(category->selectedIndex())];
    result_["active"] = active->checked() ? "true" : "false";
    result_["notes"] = trim(notes->value());

    if (onClose_)
    {
        onClose_(result_);
    }
    return true;
}

void ContactForm::cancel()
{
    result_.clear();
    if (onClose_)
    {
        onClose_({});
    }
}

std::string ContactForm::validationError() const
{
    if (trim(first_name->value()).empty())
    {
        return "First name is required.";
    }
    if (trim(last_name->value()).empty())
    {
        return "Last name is required.";
    }
    const std::string emailValue = trim(email->value());
    if (emailValue.empty())
    {
        return "Email is required.";
    }
    const std::size_t at = emailValue.find('@');
    if (at == std::string::npos || at == 0 || emailValue.find('.', at + 1) == std::string::npos)
    {
        return "Email must look like name@example.com.";
    }
    return {};
}

bool ContactForm::toBool(const std::string& value)
{
    std::string lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });
    return lower == "1" || lower == "true" || lower == "yes" || lower == "y";
}

}  // namespace contacts_manager_example

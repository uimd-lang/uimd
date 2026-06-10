#pragma once

#include "contact_form_ui.hpp"

#include <functional>
#include <string>
#include <unordered_map>

namespace contacts_manager_example
{

using ContactRecord = std::unordered_map<std::string, std::string>;
using ContactFormCallback = std::function<void(ContactRecord)>;

class ContactForm : public ContactFormUI
{
public:
    explicit ContactForm(ContactRecord contact = {}, ContactFormCallback onClose = {});

    void loadContact(const ContactRecord& contact);
    bool submit();
    void cancel();

    [[nodiscard]] const ContactRecord& result() const
    {
        return result_;
    }

protected:
    void onSaveBtnClick() override;
    void onCancelBtnClick() override;

private:
    [[nodiscard]] std::string validationError() const;
    [[nodiscard]] static bool toBool(const std::string& value);

    ContactRecord contact_;
    ContactRecord result_;
    ContactFormCallback onClose_;
};

}  // namespace contacts_manager_example

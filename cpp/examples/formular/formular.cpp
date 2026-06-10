#include "formular_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace
{

std::string formYaml(const FormularUI& window, std::string_view action);

void smoke()
{
    constexpr std::size_t kExpectedLayoutEntries = 17;
    constexpr std::size_t kExpectedCountries = 5;
    constexpr std::size_t kExpectedRoles = 7;

    FormularUI window;

    assert(window.title() == "FormApp");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.name_input != nullptr);
    assert(window.email_input != nullptr);
    assert(window.age_input != nullptr);
    assert(window.country_combo != nullptr);
    assert(window.country_combo->options().size() == kExpectedCountries);
    assert(window.role_listbox != nullptr);
    assert(window.role_listbox->selectedIndex() < static_cast<int>(kExpectedRoles));
    assert(window.save_btn != nullptr);
    assert(window.save_btn->title() == "Save");
    assert(window.cancel_btn != nullptr);
    assert(window.cancel_btn->title() == "Cancel");
}

void logicTest()
{
    FormularUI window;
    window.name_input->setValue("Marek");
    window.email_input->setValue("marek@example.com");
    window.age_input->setValue(30.0);
    window.description_input->setValue("hello");
    window.country_combo->setSelectedIndex(2);
    window.role_listbox->setSelectedIndex(3);
    (void)window.notify_check->handleKey("Enter");
    (void)window.terms_check->handleKey("Enter");

    const std::string expected =
        "action: save\n"
        "form:\n"
        "  name: Marek\n"
        "  email: marek@example.com\n"
        "  age: 30\n"
        "  description: hello\n"
        "  country: Poland\n"
        "  roles:\n"
        "  - QA\n"
        "  email_notifications: true\n"
        "  accepted_terms: true\n";
    assert(formYaml(window, "save") == expected);
}

std::string yamlScalar(const std::string& value)
{
    return value.empty() ? "\"\"" : value;
}

std::string yamlBool(bool value)
{
    return value ? "true" : "false";
}

std::string selectedComboText(const FormularUI& window)
{
    const auto& options = window.country_combo->options();
    if (options.empty())
    {
        return "";
    }
    return options[static_cast<std::size_t>(window.country_combo->selectedIndex())];
}

std::string selectedRoleText(const FormularUI& window)
{
    const auto& options = window.role_listbox->options();
    if (options.empty())
    {
        return "";
    }
    return options[static_cast<std::size_t>(window.role_listbox->selectedIndex())];
}

std::string formYaml(const FormularUI& window, std::string_view action)
{
    std::ostringstream out;
    out << "action: " << action << '\n';
    out << "form:\n";
    out << "  name: " << yamlScalar(window.name_input->value()) << '\n';
    out << "  email: " << yamlScalar(window.email_input->value()) << '\n';
    out << "  age: " << window.age_input->value() << '\n';
    out << "  description: " << yamlScalar(window.description_input->value()) << '\n';
    out << "  country: " << yamlScalar(selectedComboText(window)) << '\n';
    out << "  roles:\n";
    out << "  - " << yamlScalar(selectedRoleText(window)) << '\n';
    out << "  email_notifications: " << yamlBool(window.notify_check->checked()) << '\n';
    out << "  accepted_terms: " << yamlBool(window.terms_check->checked()) << '\n';
    return out.str();
}

void printTerminalLines(std::string_view text)
{
    std::cout << "\r\n";
    for (char ch : text)
    {
        if (ch == '\n')
        {
            std::cout << "\r\n";
        }
        else
        {
            std::cout << ch;
        }
    }
    std::cout.flush();
}

class FormApp : public FormularUI
{
public:
    [[nodiscard]] const std::string& action() const
    {
        return action_;
    }

protected:
    void onSaveBtnClick() override
    {
        action_ = "save";
        closeRequested_ = true;
    }

    void onCancelBtnClick() override
    {
        action_ = "cancel";
        closeRequested_ = true;
    }

    bool shouldClose() const override
    {
        return closeRequested_;
    }

private:
    std::string action_;
    bool closeRequested_ = false;
};

}  // namespace

int main(int argc, char** argv)
{
    FormApp app;
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

    const int result = ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
    if (!app.action().empty())
    {
        printTerminalLines(formYaml(app, app.action()));
    }
    return result;
}

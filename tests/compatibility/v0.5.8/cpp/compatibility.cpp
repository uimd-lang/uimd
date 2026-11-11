// Application logic for compatibility.
#include "compatibility_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <string>

class Compatibility : public CompatibilityUI
{
protected:
    // Override generated event hooks here.

};

int main(int argc, char** argv)
{
    return ui::runGeneratedAppMain([&]() -> int
    {
        Compatibility app;
        return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
    });
}

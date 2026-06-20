#include "source_separator_row_ui.hpp"
#include "source_separator_scroll_ui.hpp"

#include "ui/elements/ReusableElement.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <memory>

namespace
{

class SourceSeparatorScrollApp : public SourceSeparatorScrollUI
{
public:
    SourceSeparatorScrollApp()
    {
        scrollView().clearChildren();
        auto row = std::make_unique<SourceSeparatorRowUI>();
        const ui::Size rowSize = ui::generatedWindowContentSize(*row);
        auto reusable = std::make_unique<ui::ReusableElement>("row", std::move(row));
        ui::Rect rowFrame;
        rowFrame.width = rowSize.width;
        rowFrame.height = rowSize.height;
        reusable->setFrame(rowFrame);
        scrollView().addChild(
            std::move(reusable)
        );
    }
};

}  // namespace

int main(int argc, char** argv)
{
    SourceSeparatorScrollApp app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

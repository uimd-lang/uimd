#include "special_elements_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace {

void smoke() {
    constexpr std::size_t kExpectedLayoutEntries = 22;

    SpecialElementsUI window;
    assert(window.title() == "Special Elements Demo");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.header_label != nullptr);
    assert(window.info_processing != nullptr);
    assert(window.info_loading != nullptr);
    assert(window.info_complete != nullptr);
    assert(window.info_warning != nullptr);
    assert(window.info_multiline != nullptr);
    assert(window.status_label != nullptr);
    assert(window.normal_info != nullptr);
    assert(window.speed_label != nullptr);
    assert(window.raw_label != nullptr);
    assert(window.raw_button != nullptr);
    assert(window.raw_combo != nullptr);
    assert(window.raw_list != nullptr);
    assert(window.raw_input != nullptr);
    assert(window.raw_area != nullptr);
}

void logicTest() {
    SpecialElementsUI window;
    const ui::RenderedContent rendered = window.info_multiline->render(ui::Size{40, 4});
    const std::vector<std::string> rows = ui::renderedText(rendered);
    assert(rows[0].find("Line one") != std::string::npos);
    assert(rows[1].find("Line two") != std::string::npos);
    assert(rows[2].find("Line three") != std::string::npos);

    const ui::RenderedContent rawLabel = window.raw_label->render(ui::Size{64, 1});
    const std::string rawLabelText = ui::renderedText(rawLabel)[0];
    assert(rawLabelText.find('\t') == std::string::npos);
    assert(rawLabelText.find('\x1b') == std::string::npos);
    assert(rawLabelText.find('\a') == std::string::npos);

    const ui::RenderedContent rawInput = window.raw_input->render(ui::Size{32, 1}, ui::ElementRenderState{.focused = true, .editMode = true});
    const std::string rawInputText = ui::renderedText(rawInput)[0];
    assert(rawInputText.find('\t') == std::string::npos);
    assert(rawInputText.find('\x1b') == std::string::npos);

    const ui::RenderedContent rawArea = window.raw_area->render(ui::Size{32, 4}, ui::ElementRenderState{.focused = true, .editMode = true});
    for (const std::string& row : ui::renderedText(rawArea)) {
        assert(row.find('\t') == std::string::npos);
        assert(row.find('\x1b') == std::string::npos);
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc > 1 && std::string_view(argv[1]) == "--smoke") {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test") {
        logicTest();
        return 0;
    }

    SpecialElementsUI window;
    return ui::runGeneratedWindow(window, {}, argc, argv);
}

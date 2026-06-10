#include "cells_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string_view>

namespace {

const ui::GeneratedLayoutEntry* findEntry(const CellsUI& window, std::string_view name) {
    const auto& layout = window.generatedLayout();
    const auto it = std::find_if(layout.begin(), layout.end(), [name](const ui::GeneratedLayoutEntry& entry) {
        return entry.name == name;
    });
    return it == layout.end() ? nullptr : &*it;
}

}  // namespace

void smoke() {
    constexpr std::size_t kExpectedLayoutEntries = 31;

    CellsUI window;

    assert(window.title() == "Cells");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.a1 != nullptr);
    assert(window.f3 != nullptr);

    const ui::GeneratedLayoutEntry* a1 = findEntry(window, "a1");
    const ui::GeneratedLayoutEntry* b1 = findEntry(window, "b1");
    const ui::GeneratedLayoutEntry* d1 = findEntry(window, "d1");
    const ui::GeneratedLayoutEntry* f1 = findEntry(window, "f1");
    assert(a1 != nullptr);
    assert(b1 != nullptr);
    assert(d1 != nullptr);
    assert(f1 != nullptr);
    assert(a1->cellName == "cell1");
    assert(b1->cellName == "cell2");
    assert(d1->cellName == "cl3");
    assert(f1->cellName == "cl6");
}

void logicTest() {
    CellsUI window;
    ui::TerminalBuffer buffer{80, 24};
    ui::renderGeneratedWindow(window, buffer);

    assert(window.a1->frame().width > 0);
    assert(window.f3->frame().height > 0);
    assert(window.b1->frame().row == window.a1->frame().row);
    assert(window.b1->frame().col > window.a1->frame().col);
    assert(window.d6->frame().row > window.d1->frame().row);
}

int main(int argc, char** argv) {
    CellsUI window;
    if (argc > 1 && std::string_view(argv[1]) == "--smoke") {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test") {
        logicTest();
        return 0;
    }

    return ui::runGeneratedWindow(window, {}, argc, argv);
}

#include "file_browser.hpp"
#include "message_box.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

void testMessageBoxes() {
    std::string callbackResult;
    ui::dialogs::MessageBoxOk ok("Header", "Body", [&](std::string result) {
        callbackResult = result;
    });
    assert(ok.dialog_header->text() == "Header");
    assert(ok.message->text() == "Body");
    assert(!ok.closed());

    ui::dialogs::MessageBoxYesNoCancel yesNoCancel("Question", "Continue?");
    assert(yesNoCancel.dialog_header->text() == "Question");
    assert(yesNoCancel.message->text() == "Continue?");
    const ui::RenderedContent focusedCancel = ui::renderGeneratedWindowContent(yesNoCancel, ui::Size{42, 10}, 2, false);
    const std::vector<std::string> lines = ui::renderedText(focusedCancel);
    assert(std::any_of(lines.begin(), lines.end(), [](const std::string& line) {
        return line.find('[') != std::string::npos && line.find("Cancel") != std::string::npos;
    }));

    ui::dialogs::MessageBoxYesNo longWord("Question", "Continue?\n" + std::string(100, 'x'));
    const ui::Size longWordSize = ui::generatedWindowContentSize(longWord);
    const ui::RenderedContent longWordRendered = ui::renderGeneratedWindowContent(longWord, longWordSize, 1, false);
    assert(!longWordRendered.empty());
}

void testFileBrowser() {
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "ui_cpp_dialog_test";
    std::filesystem::create_directories(root);
    std::filesystem::create_directories(root / "subdir");
    {
        std::ofstream(root / "sample.txt") << "sample";
    }

    ui::dialogs::FileBrowser browser(root, root, "open");
    assert(browser.path_label->text().find("ui_cpp_dialog_test") != std::string::npos);
    assert(!browser.entries->options().empty());
    browser.close((root / "sample.txt").string());
    assert(browser.closed());
    assert(browser.result().find("sample.txt") != std::string::npos);

    std::filesystem::remove(root / "new.txt");
    ui::dialogs::FileBrowser saveBrowser(root, root, "save", {}, "new.txt");
    assert(saveBrowser.dialog_header->text() == "Save As");
    assert(saveBrowser.open_btn->title() == "Save");
    assert(saveBrowser.filename->value() == "new.txt");
    assert(saveBrowser.filename->cursor() == static_cast<int>(saveBrowser.filename->value().size()));
    assert(saveBrowser.acceptFilename());
    assert(saveBrowser.result() == (std::filesystem::weakly_canonical(root) / "new.txt").string());

    {
        std::ofstream(root / "photo.png") << "png";
        std::string picked;
        ui::dialogs::FileBrowser filtered(root, root, "open", [&](std::string path) {
            picked = std::move(path);
        }, "", "png|jpg");
        const auto& disabled = filtered.entries->disabledValues();
        assert(std::find(disabled.begin(), disabled.end(), "sample.txt") != disabled.end());
        const auto& options = filtered.entries->options();
        const auto txt = std::find(options.begin(), options.end(), "sample.txt");
        assert(txt != options.end());
        filtered.selectEntry(static_cast<int>(txt - options.begin()));
        assert(!filtered.open_btn->enabled());
        assert(!filtered.acceptCurrent());
        assert(picked.empty());

        const auto png = std::find(options.begin(), options.end(), "photo.png");
        assert(png != options.end());
        filtered.selectEntry(static_cast<int>(png - options.begin()));
        assert(filtered.open_btn->enabled());
        assert(filtered.acceptCurrent());
        assert(picked == (std::filesystem::weakly_canonical(root) / "photo.png").string());
    }
}

void testFileBrowserDoubleClickDirectory() {
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "ui_cpp_dialog_double_click_test";
    std::filesystem::create_directories(root / "subdir");
    {
        std::ofstream(root / "open_me.txt") << "open";
    }

    ui::dialogs::FileBrowser browser(root, root, "open");
    const ui::Size size = ui::generatedWindowContentSize(browser);
    (void)ui::renderGeneratedWindowContent(browser, size, -1, true);

    const auto& options = browser.entries->options();
    const auto subdir = std::find(options.begin(), options.end(), "subdir/");
    assert(subdir != options.end());
    const int subdirIndex = static_cast<int>(std::distance(options.begin(), subdir));
    const ui::Point subdirPoint{
        browser.entries->frame().row + subdirIndex - browser.entries->scrollOffset(),
        browser.entries->frame().col,
    };
    assert(!browser.handleEntryMousePress(subdirPoint));
    assert(browser.handleEntryMousePress(subdirPoint));
    assert(browser.currentDir() == std::filesystem::weakly_canonical(root / "subdir"));

    (void)ui::renderGeneratedWindowContent(browser, size, -1, true);
    const ui::Point parentPoint{browser.entries->frame().row, browser.entries->frame().col};
    assert(!browser.handleEntryMousePress(parentPoint));
    assert(browser.handleEntryMousePress(parentPoint));
    assert(browser.currentDir() == std::filesystem::weakly_canonical(root));

    (void)ui::renderGeneratedWindowContent(browser, size, -1, true);
    const auto& rootOptions = browser.entries->options();
    const auto file = std::find(rootOptions.begin(), rootOptions.end(), "open_me.txt");
    assert(file != rootOptions.end());
    const int fileIndex = static_cast<int>(std::distance(rootOptions.begin(), file));
    const ui::Point filePoint{
        browser.entries->frame().row + fileIndex - browser.entries->scrollOffset(),
        browser.entries->frame().col,
    };
    assert(!browser.handleEntryMousePress(filePoint));
    assert(browser.handleEntryMousePress(filePoint));
    assert(browser.closed());
    assert(browser.result() == (std::filesystem::weakly_canonical(root) / "open_me.txt").string());
}

}  // namespace

void run_dialog_tests() {
    testMessageBoxes();
    testFileBrowser();
    testFileBrowserDoubleClickDirectory();
}

#include "text_editor_ui.hpp"

#include "../../dialogs/file_browser.hpp"
#include "../../dialogs/message_box.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

constexpr std::size_t kExpectedLayoutEntries = 10;
constexpr std::string_view kNotesDirEnv = "UI_TEXT_EDITOR_NOTES_DIR";
constexpr std::string_view kDefaultNoteEnv = "UI_TEXT_EDITOR_DEFAULT_NOTE";
constexpr std::string_view kBrowserRootDirEnv = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR";
constexpr std::string_view kDefaultBrowserDirEnv = "UI_TEXT_EDITOR_BROWSER_DIR";
constexpr std::string_view kDefaultFileName = "welcome.txt";
constexpr std::string_view kUntitledName = "untitled.txt";
std::optional<std::filesystem::path> envPath(std::string_view name)
{
    const std::string key{name};
    const char* value = std::getenv(key.c_str());
    if (value == nullptr || std::string_view(value).empty())
    {
        return std::nullopt;
    }
    return std::filesystem::path(value);
}

std::optional<std::string> envString(std::string_view name)
{
    const std::string key{name};
    const char* value = std::getenv(key.c_str());
    if (value == nullptr || std::string_view(value).empty())
    {
        return std::nullopt;
    }
    return std::string(value);
}

std::filesystem::path notesDir()
{
    if (const auto path = envPath(kNotesDirEnv); path.has_value())
    {
        return *path;
    }
    return std::filesystem::path(__FILE__).parent_path() / "notes";
}

std::string defaultNoteName()
{
    return envString(kDefaultNoteEnv).value_or(std::string(kDefaultFileName));
}

std::filesystem::path userHomeDir()
{
    const char* home = std::getenv("HOME");
    return home == nullptr || std::string_view(home).empty()
        ? std::filesystem::current_path()
        : std::filesystem::path(home);
}

std::filesystem::path browserRootDir()
{
    if (const auto path = envPath(kBrowserRootDirEnv); path.has_value())
    {
        return *path;
    }
    return userHomeDir();
}

std::filesystem::path defaultBrowserDir()
{
    if (const auto path = envPath(kDefaultBrowserDirEnv); path.has_value())
    {
        return *path;
    }
    const std::filesystem::path desktop = userHomeDir() / "Desktop";
    return std::filesystem::is_directory(desktop) ? desktop : userHomeDir();
}

bool pathStartsWith(const std::filesystem::path& path, const std::filesystem::path& root)
{
    const std::string absolutePath = std::filesystem::absolute(path).string();
    const std::string absoluteRoot = std::filesystem::absolute(root).string();
    return absolutePath.rfind(absoluteRoot, 0) == 0;
}

std::string readTextFile(const std::filesystem::path& path)
{
    std::ifstream input(path);
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void writeTextFile(const std::filesystem::path& path, std::string_view text)
{
    std::ofstream output(path);
    output << text;
}

std::string displayFileName(const std::filesystem::path& path)
{
    const std::string name = path.filename().string();
    return name.empty() ? path.string() : name;
}

class TextEditorApp : public TextEditorUI
{
public:
    TextEditorApp()
    {
        loadPath(notesDir() / defaultNoteName());
    }

    [[nodiscard]] TextEditorUI& window()
    {
        return *this;
    }

    [[nodiscard]] ui::GeneratedWindowStack& modalStack()
    {
        return modalStack_;
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = TextEditorUI::runtimeOptions();
        options.windowStack = &modalStack_;
        return options;
    }

    void newFile()
    {
        currentPath_.reset();
        filename->setText(std::string(kUntitledName));
        setEditorText("");
        loadedText_.clear();
        refreshStatus("New file");
    }

    void loadPath(std::filesystem::path path)
    {
        path = std::filesystem::absolute(std::move(path));
        const std::string text = readTextFile(path);
        currentPath_ = path;
        filename->setText(displayFileName(path));
        setEditorText(text);
        loadedText_ = text;
        refreshStatus("Loaded");
    }

    void saveOrSaveAs()
    {
        if (!currentPath_.has_value())
        {
            saveAsBrowser();
            return;
        }
        savePath(*currentPath_);
    }

    void openBrowser()
    {
        browser_ = std::make_unique<ui::dialogs::FileBrowser>(
            browserRootDir(),
            defaultBrowserDir(),
            "open",
            [this](std::string path)
            {
                onOpenSelected(path);
            });
        pushBrowserFrame();
    }

    void saveAsBrowser()
    {
        std::filesystem::path start = currentPath_.has_value() ? currentPath_->parent_path() : notesDir();
        if (!pathStartsWith(start, browserRootDir()))
        {
            start = defaultBrowserDir();
        }
        const std::string initialFilename = currentPath_.has_value()
            ? currentPath_->filename().string()
            : std::string(kUntitledName);
        browser_ = std::make_unique<ui::dialogs::FileBrowser>(
            browserRootDir(),
            start,
            "save",
            [this](std::string path)
            {
                onSaveSelected(path);
            },
            initialFilename);
        pushBrowserFrame();
    }

    void savePath(std::filesystem::path path)
    {
        path = std::filesystem::absolute(std::move(path));
        writeTextFile(path, editor->value());
        currentPath_ = path;
        filename->setText(displayFileName(path));
        loadedText_ = editor->value();
        refreshStatus("Saved");
    }

    void markModified()
    {
        refreshStatus("Modified");
    }

    [[nodiscard]] bool modified() const
    {
        return editor->value() != loadedText_;
    }

    void withSavedChanges(const std::function<void()>& action, std::string simulatedChoice = {})
    {
        afterSaveAction_ = nullptr;
        if (!modified())
        {
            action();
            return;
        }
        pendingAction_ = action;
        if (simulatedChoice.empty())
        {
            unsavedDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNoCancel>(
                "Unsaved Changes",
                "Save current file before continuing?");
            pushUnsavedFrame();
            return;
        }
        onUnsavedChoice(std::move(simulatedChoice));
    }

    void quit()
    {
        finished_ = true;
    }

    [[nodiscard]] bool shouldClose() const override
    {
        return finished_;
    }

    [[nodiscard]] bool hasModalOverlay() const
    {
        return browser_ != nullptr || unsavedDialog_ != nullptr || overwriteDialog_ != nullptr;
    }

    void onOpenSelected(const std::filesystem::path& path)
    {
        if (!path.empty())
        {
            loadPath(path);
        }
    }

    void onSaveSelected(const std::filesystem::path& path)
    {
        auto action = std::move(afterSaveAction_);
        afterSaveAction_ = nullptr;
        if (!path.empty())
        {
            savePath(path);
            if (action)
            {
                action();
            }
        }
        else if (action)
        {
            refreshStatus("Cancelled");
        }
    }

protected:
    void onNewBtnClick() override
    {
        withSavedChanges([this]
        {
            newFile();
        });
    }

    void onOpenBtnClick() override
    {
        withSavedChanges([this]
        {
            openBrowser();
        });
    }

    void onSaveBtnClick() override
    {
        saveOrSaveAs();
    }

    void onSaveAsBtnClick() override
    {
        saveAsBrowser();
    }

    void onQuitBtnClick() override
    {
        withSavedChanges([this]
        {
            quit();
        });
    }

    void onEditorChange(std::string_view) override
    {
        markModified();
    }

private:
    void acceptBrowserCurrent()
    {
        (void)browser_->acceptCurrent();
        closeBrowserIfClosed();
    }

    void acceptBrowserFilename()
    {
        acceptBrowserOpenButton();
    }

    void acceptBrowserOpenButton()
    {
        if (browser_->open_btn->title() == "Save")
        {
            const std::string name = browser_->filename->value();
            if (name.empty())
            {
                return;
            }
            const std::filesystem::path path = browser_->currentDir() / name;
            if (std::filesystem::is_regular_file(path))
            {
                overwritePath_ = path;
                overwriteDialog_ = std::make_unique<ui::dialogs::MessageBoxYesNo>(
                    "Overwrite File",
                    "File already exists:\n" + path.string() + "\n\nOverwrite it?");
                pushOverwriteFrame();
                return;
            }
            closeBrowser(path.string());
            return;
        }
        const std::string name = browser_->filename->value();
        if (!name.empty())
        {
            const std::filesystem::path path = browser_->currentDir() / name;
            if (std::filesystem::is_regular_file(path))
            {
                closeBrowser(path.string());
                return;
            }
        }
        acceptBrowserCurrent();
    }

    void closeBrowserIfClosed()
    {
        if (browser_ != nullptr && browser_->closed())
        {
            modalStack_.remove(*browser_);
            browser_.reset();
        }
    }

    void closeBrowser(std::string path)
    {
        if (browser_ != nullptr)
        {
            browser_->close(std::move(path));
            modalStack_.remove(*browser_);
            browser_.reset();
        }
    }

    void closeUnsaved(const std::string& choice)
    {
        if (unsavedDialog_ != nullptr)
        {
            modalStack_.remove(*unsavedDialog_);
        }
        unsavedDialog_.reset();
        onUnsavedChoice(choice);
    }

    void closeOverwrite(const std::string& choice)
    {
        if (overwriteDialog_ != nullptr)
        {
            modalStack_.remove(*overwriteDialog_);
        }
        overwriteDialog_.reset();
        if (choice == "yes")
        {
            closeBrowser(overwritePath_.string());
        }
        overwritePath_.clear();
    }

    void pushBrowserFrame()
    {
        if (browser_ == nullptr)
        {
            return;
        }
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "FileBrowser";
        frame.initialFocusName = "entries";
        frame.startInEditMode = true;
        frame.onButton = [this](std::string_view name)
        {
            if (name == "open_btn")
            {
                acceptBrowserOpenButton();
            }
            else if (name == "close_btn")
            {
                closeBrowser("");
            }
        };
        frame.onTextChanged = [this](std::string_view name, std::string_view)
        {
            if (browser_ != nullptr && name == "entries")
            {
                browser_->selectEntry(browser_->entries->selectedIndex());
            }
        };
        frame.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>&)
        {
            if (browser_ != nullptr && name == "entries")
            {
                browser_->selectEntry(browser_->entries->selectedIndex());
            }
        };
        frame.onTextConfirmed = frame.onTextChanged;
        frame.onKeyBeforeFocusedElement = [this](std::string_view key, std::string_view name, bool editMode)
        {
            if (browser_ == nullptr)
            {
                return false;
            }
            if (key == "Escape")
            {
                closeBrowser("");
                return true;
            }
            if (name == "entries" && editMode && key == "Enter")
            {
                acceptBrowserCurrent();
                return true;
            }
            return false;
        };
        modalStack_.push(*browser_, std::move(frame));
    }

    void pushUnsavedFrame()
    {
        if (unsavedDialog_ == nullptr)
        {
            return;
        }
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNoCancel";
        frame.initialFocusName = "cancel_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                closeUnsaved("yes");
            }
            else if (name == "no_btn")
            {
                closeUnsaved("no");
            }
            else if (name == "cancel_btn")
            {
                closeUnsaved("cancel");
            }
        };
        frame.onKey = [this](std::string_view key)
        {
            if (key == "Escape")
            {
                closeUnsaved("cancel");
                return true;
            }
            return false;
        };
        modalStack_.push(*unsavedDialog_, std::move(frame));
    }

    void pushOverwriteFrame()
    {
        if (overwriteDialog_ == nullptr)
        {
            return;
        }
        ui::GeneratedWindowFrameOptions frame;
        frame.className = "MessageBoxYesNo";
        frame.initialFocusName = "no_btn";
        frame.onButton = [this](std::string_view name)
        {
            if (name == "yes_btn")
            {
                closeOverwrite("yes");
            }
            else if (name == "no_btn")
            {
                closeOverwrite("no");
            }
        };
        frame.onKey = [this](std::string_view key)
        {
            if (key == "Escape")
            {
                closeOverwrite("no");
                return true;
            }
            return false;
        };
        modalStack_.push(*overwriteDialog_, std::move(frame));
    }

    void onUnsavedChoice(std::string choice)
    {
        auto action = std::move(pendingAction_);
        pendingAction_ = nullptr;
        if (choice == "cancel" || !action)
        {
            refreshStatus("Cancelled");
            return;
        }
        if (choice == "yes")
        {
            if (!currentPath_.has_value())
            {
                afterSaveAction_ = action;
                saveAsBrowser();
                return;
            }
            saveOrSaveAs();
            action();
            return;
        }
        if (choice == "no")
        {
            action();
        }
    }

    void setEditorText(std::string text)
    {
        editor->setValue(std::move(text));
        editor->selectRange(0, 0);
    }

    void refreshStatus(std::string prefix)
    {
        const std::string& text = editor->value();
        const std::string modifiedMarker = modified() ? "*" : "";
        const int lineCount = text.empty() ? 0 : static_cast<int>(std::count(text.begin(), text.end(), '\n')) + 1;
        status->setText(prefix + ": " + filename->text() + modifiedMarker + " - " +
                                std::to_string(lineCount) + " lines, " + std::to_string(text.size()) + " chars");
    }

    ui::GeneratedWindowStack modalStack_;
    std::optional<std::filesystem::path> currentPath_;
    std::string loadedText_;
    std::function<void()> pendingAction_;
    std::function<void()> afterSaveAction_;
    std::unique_ptr<ui::dialogs::MessageBoxYesNoCancel> unsavedDialog_;
    std::unique_ptr<ui::dialogs::MessageBoxYesNo> overwriteDialog_;
    std::unique_ptr<ui::dialogs::FileBrowser> browser_;
    std::filesystem::path overwritePath_;
    bool finished_ = false;
};

void smoke()
{
    TextEditorApp app;
    TextEditorUI& window = app.window();
    assert(window.title() == "Text Editor");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.window_header != nullptr);
    assert(window.filename != nullptr);
    assert(window.editor != nullptr);
    assert(window.new_btn != nullptr);
    assert(window.open_btn != nullptr);
    assert(window.save_btn != nullptr);
    assert(window.save_as_btn != nullptr);
    assert(window.quit_btn != nullptr);
    assert(window.status != nullptr);
    assert(window.filename->text().find(std::string(kDefaultFileName)) != std::string::npos);
}

void logicTest()
{
    {
        TextEditorApp modalApp;
        assert(!modalApp.hasModalOverlay());
        modalApp.openBrowser();
        assert(modalApp.hasModalOverlay());
    }

    TextEditorApp app;
    TextEditorUI& window = app.window();
    assert(!window.editor->value().empty());
    app.newFile();
    assert(window.filename->text() == kUntitledName);
    assert(window.editor->value().empty());
    window.editor->setValue("draft");
    app.markModified();
    assert(app.modified());
    assert(window.status->text() == "Modified: untitled.txt* - 1 lines, 5 chars");
    bool actionCalled = false;
    app.withSavedChanges([&]
    {
        actionCalled = true;
    }, "no");
    assert(actionCalled);

    const std::filesystem::path savedPath = std::filesystem::temp_directory_path() / "ui_cpp_text_editor_logic.txt";
    window.editor->setValue("saved draft");
    app.onSaveSelected(savedPath);
    assert(readTextFile(savedPath) == "saved draft");
    assert(!app.modified());
    std::filesystem::remove(savedPath);

    app.withSavedChanges([&]
    {
        app.quit();
    });
    assert(app.shouldClose());
}

}  // namespace

int main(int argc, char** argv)
{
    TextEditorApp app;
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

    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}

#pragma once

#include "file_browser_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <chrono>

namespace ui::dialogs {

using FileBrowserCallback = std::function<void(std::string)>;

class FileBrowser : public FileBrowserUI {
public:
    FileBrowser(std::filesystem::path rootDir, std::filesystem::path startPath = {}, std::string mode = "open",
                FileBrowserCallback onClose = {}, std::string initialFilename = {},
                std::string extensionFilter = {});

    [[nodiscard]] const std::filesystem::path& currentDir() const {
        return currentDir_;
    }

    [[nodiscard]] const std::string& result() const {
        return result_;
    }

    [[nodiscard]] bool closed() const {
        return closed_;
    }

    void refreshEntries();
    bool acceptCurrent();
    bool acceptFilename();
    void selectEntry(int index);
    [[nodiscard]] bool selectedEntryIsDirectory() const;
    [[nodiscard]] bool entryIndexIsDirectory(int index) const;
    [[nodiscard]] bool handleEntryMousePress(Point point);
    void moveFilenameCursorToEnd();
    void close(std::string path);
    [[nodiscard]] GeneratedWindowFrameOptions stackFrameOptions();
    int run();

private:
    [[nodiscard]] std::filesystem::path clampDir(std::filesystem::path path) const;
    [[nodiscard]] std::filesystem::path selectedPath() const;
    [[nodiscard]] bool pathMatchesFilter(const std::filesystem::path& path) const;
    [[nodiscard]] std::filesystem::path selectedPathForOpenState() const;
    void confirmOverwrite(const std::filesystem::path& path);
    void previewSelected();
    void updateOpenEnabled();
    [[nodiscard]] GeneratedWindowRuntimeOptions runtimeOptions();

    std::filesystem::path rootDir_;
    std::filesystem::path currentDir_;
    std::string mode_;
    FileBrowserCallback onClose_;
    std::string initialFilename_;
    std::string extensionFilter_;
    std::string result_;
    int lastClickedEntry_ = -1;
    std::chrono::steady_clock::time_point lastEntryClickTime_{};
    bool closed_ = false;
};

}  // namespace ui::dialogs

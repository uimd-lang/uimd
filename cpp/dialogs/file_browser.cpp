#include "file_browser.hpp"

#include "message_box.hpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <regex>
#include <sstream>
#include <system_error>
#include <utility>
#include <vector>

namespace ui::dialogs {

namespace {

constexpr auto kFileBrowserDoubleClickInterval = std::chrono::milliseconds(400);

std::string displayName(const std::filesystem::directory_entry& entry) {
    const std::string name = entry.path().filename().string();
    return entry.is_directory() ? name + "/" : name;
}

std::string trim(std::string text) {
    const auto notSpace = [](unsigned char ch) {
        return !std::isspace(ch);
    };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), notSpace));
    text.erase(std::find_if(text.rbegin(), text.rend(), notSpace).base(), text.end());
    return text;
}

}  // namespace

FileBrowser::FileBrowser(std::filesystem::path rootDir, std::filesystem::path startPath, std::string mode,
                         FileBrowserCallback onClose, std::string initialFilename, std::string extensionFilter)
    : rootDir_(std::filesystem::weakly_canonical(std::move(rootDir))),
      mode_(std::move(mode)),
      onClose_(std::move(onClose)),
      extensionFilter_(trim(std::move(extensionFilter))) {
    std::filesystem::path requestedStart = startPath.empty() ? rootDir_ : std::move(startPath);
    if (std::filesystem::is_regular_file(requestedStart)) {
        currentDir_ = clampDir(requestedStart.parent_path());
        if (initialFilename.empty()) {
            initialFilename = requestedStart.filename().string();
        }
    } else {
        currentDir_ = clampDir(std::move(requestedStart));
    }
    initialFilename_ = std::move(initialFilename);
    dialog_header->setText(mode_ == "save" ? "Save As" : "Open File");
    open_btn->setTitle(mode_ == "save" ? "Save" : "Open");
    if (mode_ == "save") {
        filename->setValue(initialFilename_);
        moveFilenameCursorToEnd();
    }
    refreshEntries();
}

void FileBrowser::refreshEntries() {
    std::vector<std::string> rows{".."};
    std::vector<std::string> disabledRows;

    std::vector<std::filesystem::directory_entry> dirs;
    std::vector<std::filesystem::directory_entry> files;
    std::error_code error;
    for (const auto& entry : std::filesystem::directory_iterator(currentDir_, error)) {
        if (entry.is_directory()) {
            dirs.push_back(entry);
        } else if (entry.is_regular_file()) {
            files.push_back(entry);
        }
    }
    const auto byName = [](const auto& lhs, const auto& rhs) {
        return lhs.path().filename().string() < rhs.path().filename().string();
    };
    std::sort(dirs.begin(), dirs.end(), byName);
    std::sort(files.begin(), files.end(), byName);
    for (const auto& dir : dirs) {
        rows.push_back(displayName(dir));
    }
    for (const auto& file : files) {
        const std::string name = displayName(file);
        rows.push_back(name);
        if (!pathMatchesFilter(file.path())) {
            disabledRows.push_back(name);
        }
    }

    path_label->setText(currentDir_.string());
    entries->setOptions(std::move(rows));
    entries->setDisabledValues(std::move(disabledRows));
    const auto& options = entries->options();
    auto selected = std::find(options.begin(), options.end(), initialFilename_);
    entries->setSelectedIndex(selected == options.end() ? 0 : static_cast<int>(selected - options.begin()));
    lastClickedEntry_ = -1;
    previewSelected();
}

bool FileBrowser::acceptCurrent() {
    const std::filesystem::path path = selectedPath();
    if (path.empty()) {
        return false;
    }
    if (std::filesystem::is_directory(path)) {
        currentDir_ = clampDir(path);
        refreshEntries();
        return true;
    }
    const std::string selectedName = path.filename().string();
    const std::string name = trim(filename->value().empty() ? selectedName : filename->value());
    if (name.empty()) {
        return false;
    }
    const std::filesystem::path namedPath = currentDir_ / name;
    if (mode_ == "open" && std::filesystem::is_regular_file(namedPath) && clampDir(namedPath.parent_path()) == currentDir_) {
        if (!pathMatchesFilter(namedPath)) {
            return false;
        }
        close(namedPath.string());
        return true;
    }
    if (mode_ == "save") {
        return acceptFilename();
    }
    return false;
}

bool FileBrowser::acceptFilename() {
    const std::string name = trim(filename->value());
    if (name.empty()) {
        return false;
    }
    const std::filesystem::path path = currentDir_ / name;
    if (std::filesystem::is_directory(path) || clampDir(path.parent_path()) != currentDir_) {
        return false;
    }
    if (std::filesystem::is_regular_file(path)) {
        confirmOverwrite(path);
        return true;
    }
    close(path.string());
    return true;
}

void FileBrowser::selectEntry(int index) {
    entries->setSelectedIndex(index);
    previewSelected();
}

bool FileBrowser::selectedEntryIsDirectory() const {
    return entryIndexIsDirectory(entries->selectedIndex());
}

bool FileBrowser::entryIndexIsDirectory(int index) const {
    const auto& options = entries->options();
    if (index < 0 || index >= static_cast<int>(options.size())) {
        return false;
    }
    const std::string& selected = options[static_cast<std::size_t>(index)];
    return selected == ".." || selected.ends_with("/");
}

bool FileBrowser::handleEntryMousePress(Point point) {
    const Rect frame = entries->frame();
    if (!frame.contains(point)) {
        return false;
    }
    const int index = entries->scrollOffset() + point.row - frame.row;
    const auto now = std::chrono::steady_clock::now();
    const bool doubleClick = index == lastClickedEntry_ && now - lastEntryClickTime_ <= kFileBrowserDoubleClickInterval;
    selectEntry(index);
    lastClickedEntry_ = index;
    lastEntryClickTime_ = now;
    if (doubleClick && (entryIndexIsDirectory(index) || mode_ == "open")) {
        const bool accepted = acceptCurrent();
        lastClickedEntry_ = -1;
        return accepted;
    }
    return false;
}

void FileBrowser::moveFilenameCursorToEnd() {
    const int end = static_cast<int>(filename->value().size());
    filename->selectRange(end, end);
}

void FileBrowser::close(std::string path) {
    result_ = std::move(path);
    closed_ = true;
    if (onClose_) {
        onClose_(result_);
    }
}

int FileBrowser::run() {
    return runGeneratedWindow(*this, runtimeOptions());
}

GeneratedWindowFrameOptions FileBrowser::stackFrameOptions() {
    GeneratedWindowRuntimeOptions runtime = runtimeOptions();
    GeneratedWindowFrameOptions frame;
    frame.className = "FileBrowser";
    frame.initialFocusName = std::move(runtime.initialFocusName);
    frame.startInEditMode = runtime.startInEditMode;
    frame.onButton = std::move(runtime.onButton);
    frame.onKeyBeforeFocusedElement = std::move(runtime.onKeyBeforeFocusedElement);
    frame.onKeyBeforeFocused = std::move(runtime.onKeyBeforeFocused);
    frame.onKey = std::move(runtime.onKey);
    frame.onMousePressBeforeFocused = std::move(runtime.onMousePressBeforeFocused);
    frame.onTextChanged = std::move(runtime.onTextChanged);
    frame.onTextConfirmed = std::move(runtime.onTextConfirmed);
    frame.onSelectionChanged = std::move(runtime.onSelectionChanged);
    frame.onEditStarted = std::move(runtime.onEditStarted);
    frame.onFocusChanged = std::move(runtime.onFocusChanged);
    frame.shouldClose = std::move(runtime.shouldClose);
    return frame;
}

std::filesystem::path FileBrowser::clampDir(std::filesystem::path path) const {
    std::error_code error;
    std::filesystem::path canonical = std::filesystem::weakly_canonical(path, error);
    if (error || canonical.empty()) {
        canonical = rootDir_;
    }
    const std::string root = rootDir_.string();
    const std::string candidate = canonical.string();
    if (candidate.rfind(root, 0) != 0) {
        return rootDir_;
    }
    return canonical;
}

void FileBrowser::confirmOverwrite(const std::filesystem::path& path) {
    MessageBoxYesNo dialog(
        "Overwrite File",
        "File already exists:\n" + path.string() + "\n\nOverwrite it?");
    (void)dialog.run();
    if (dialog.result() == "yes") {
        close(path.string());
    }
}

void FileBrowser::previewSelected() {
    const auto& options = entries->options();
    if (options.empty()) {
        return;
    }
    const std::string& selected = options[static_cast<std::size_t>(entries->selectedIndex())];
    if (selected != "..") {
        filename->setValue(selected.ends_with("/") ? selected.substr(0, selected.size() - 1) : selected);
    } else if (mode_ == "open") {
        filename->setValue("");
    }
    moveFilenameCursorToEnd();
    updateOpenEnabled();
}

std::filesystem::path FileBrowser::selectedPath() const {
    const auto& options = entries->options();
    if (options.empty()) {
        return {};
    }
    const std::string& selected = options[static_cast<std::size_t>(entries->selectedIndex())];
    if (selected == "..") {
        return currentDir_.parent_path();
    }
    if (selected.ends_with("/")) {
        return currentDir_ / selected.substr(0, selected.size() - 1);
    }
    return currentDir_ / selected;
}

bool FileBrowser::pathMatchesFilter(const std::filesystem::path& path) const {
    if (extensionFilter_.empty()) {
        return true;
    }
    std::string ext = path.extension().string();
    if (!ext.empty() && ext.front() == '.') {
        ext.erase(ext.begin());
    }
    if (ext.empty()) {
        return false;
    }
    try {
        return std::regex_match(ext, std::regex(extensionFilter_, std::regex_constants::icase));
    } catch (const std::regex_error&) {
        std::istringstream stream(extensionFilter_);
        std::string part;
        while (std::getline(stream, part, '|')) {
            part = trim(std::move(part));
            if (!part.empty() && part.front() == '.') {
                part.erase(part.begin());
            }
            std::transform(part.begin(), part.end(), part.begin(), [](unsigned char ch) {
                return static_cast<char>(std::tolower(ch));
            });
            std::string lowered = ext;
            std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char ch) {
                return static_cast<char>(std::tolower(ch));
            });
            if (lowered == part) {
                return true;
            }
        }
    }
    return false;
}

std::filesystem::path FileBrowser::selectedPathForOpenState() const {
    const std::string name = trim(filename->value());
    if (!name.empty()) {
        return currentDir_ / name;
    }
    const auto& options = entries->options();
    if (options.empty()) {
        return {};
    }
    const std::string& selected = options[static_cast<std::size_t>(entries->selectedIndex())];
    if (selected.empty()) {
        return {};
    }
    if (selected == "..") {
        return clampDir(currentDir_.parent_path());
    }
    if (selected.ends_with("/")) {
        return currentDir_ / selected.substr(0, selected.size() - 1);
    }
    return currentDir_ / selected;
}

void FileBrowser::updateOpenEnabled() {
    if (mode_ == "save") {
        open_btn->setEnabled(!trim(filename->value()).empty());
        return;
    }
    const std::filesystem::path path = selectedPathForOpenState();
    if (path.empty()) {
        open_btn->setEnabled(false);
        return;
    }
    if (std::filesystem::is_directory(path)) {
        open_btn->setEnabled(true);
        return;
    }
    open_btn->setEnabled(std::filesystem::is_regular_file(path) && pathMatchesFilter(path));
}

GeneratedWindowRuntimeOptions FileBrowser::runtimeOptions() {
    GeneratedWindowRuntimeOptions options;
    options.initialFocusName = "entries";
    options.startInEditMode = true;
    options.onButton = [this](std::string_view name) {
        if (name == "open_btn") {
            if (mode_ == "save") {
                (void)acceptFilename();
            } else {
                (void)acceptCurrent();
            }
        } else if (name == "close_btn") {
            close("");
        }
    };
    options.onTextChanged = [this](std::string_view name, std::string_view) {
        if (name == "entries") {
            previewSelected();
        } else if (name == "filename") {
            updateOpenEnabled();
        }
    };
    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>&) {
        if (name == "entries") {
            previewSelected();
        }
    };
    options.onTextConfirmed = [this](std::string_view name, std::string_view) {
        if (name == "entries") {
            previewSelected();
        }
    };
    options.onKeyBeforeFocusedElement = [this](std::string_view key, std::string_view name, bool editMode) {
        if (name == "entries" && editMode && key == "Enter") {
            if (selectedEntryIsDirectory()) {
                (void)acceptCurrent();
            }
            return true;
        }
        return false;
    };
    options.onEditStarted = [this](std::string_view name) {
        if (name == "filename") {
            moveFilenameCursorToEnd();
        }
    };
    options.onMousePressBeforeFocused = [this](Point point) {
        return handleEntryMousePress(point);
    };
    options.onKey = [this](std::string_view key) {
        if (key == "Escape") {
            close("");
            return true;
        }
        return false;
    };
    options.shouldClose = [this] {
        return closed_;
    };
    return options;
}

}  // namespace ui::dialogs

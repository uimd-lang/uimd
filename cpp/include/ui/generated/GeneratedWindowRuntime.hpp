#pragma once

#include "ui/generated/GeneratedWindowBase.hpp"
#include "ui/elements/Render.hpp"
#include "ui/terminal/TerminalBuffer.hpp"

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ui {

struct GeneratedWindowFrameOptions {
    std::string className;
    std::string initialFocusName;
    bool startInEditMode = false;
    bool dimBackground = true;
    std::function<void(std::string_view)> onButton;
    std::function<bool(std::string_view, std::string_view, bool)> onKeyBeforeFocusedElement;
    std::function<bool(std::string_view)> onKeyBeforeFocused;
    std::function<bool(std::string_view)> onKey;
    std::function<bool(Point)> onMousePressBeforeFocused;
    std::function<bool(Point, int)> onMouseWheelBeforeFocused;
    std::function<void(std::string_view, std::string_view)> onTextChanged;
    std::function<void(std::string_view, std::string_view)> onTextConfirmed;
    std::function<void(std::string_view, std::vector<std::string>)> onSelectionChanged;
    std::function<void(std::string_view)> onEditStarted;
    std::function<void(std::string_view, bool)> onFocusChanged;
    std::function<bool()> shouldClose;
};

struct EditSnapshot {
    Element* element = nullptr;
    std::string textValue;
    int textCursor = 0;
    double numberValue = 0.0;
    int selectedIndex = 0;
};

struct GeneratedWindowStackFrame {
    GeneratedWindowBase* window = nullptr;
    GeneratedWindowFrameOptions options;
    Rect bounds{};
    int focusedIndex = -1;
    bool editMode = false;
    Element* activeScrollView = nullptr;
    Element* activeScrollViewEditElement = nullptr;
    std::optional<EditSnapshot> editSnapshot;
    std::unordered_map<Element*, Element*> scrollViewLastDescendant;
};

class GeneratedWindowStack {
public:
    GeneratedWindowStackFrame& push(GeneratedWindowBase& window, GeneratedWindowFrameOptions options = {});
    void pop();
    void remove(GeneratedWindowBase& window);
    void clear();

    [[nodiscard]] bool empty() const {
        return frames_.empty();
    }

    [[nodiscard]] GeneratedWindowStackFrame* top();
    [[nodiscard]] const GeneratedWindowStackFrame* top() const;

    [[nodiscard]] std::vector<GeneratedWindowStackFrame>& frames() {
        return frames_;
    }

    [[nodiscard]] const std::vector<GeneratedWindowStackFrame>& frames() const {
        return frames_;
    }

private:
    std::vector<GeneratedWindowStackFrame> frames_;
};

struct GeneratedWindowRuntimeOptions {
    std::string footer;
    std::string initialFocusName;
    bool startInEditMode = false;
    bool keepEditModeAfterConfirm = false;
    bool keepEditModeAfterEscape = false;
    GeneratedWindowStack* windowStack = nullptr;
    std::function<void(std::string_view)> onButton;
    std::function<bool(std::string_view, std::string_view, bool)> onKeyBeforeFocusedElement;
    std::function<bool(std::string_view)> onKeyBeforeFocused;
    std::function<bool(std::string_view)> onKey;
    std::function<bool(Point)> onMousePressBeforeFocused;
    std::function<bool(Point, int)> onMouseWheelBeforeFocused;
    std::function<bool(std::string_view, int)> onMouseWheel;
    std::function<void(std::string_view, std::string_view)> onTextChanged;
    std::function<void(std::string_view, std::string_view)> onTextConfirmed;
    std::function<void(std::string_view, std::vector<std::string>)> onSelectionChanged;
    std::function<void(std::string_view)> onEditStarted;
    std::function<void(std::string_view, bool)> onFocusChanged;
    std::function<void(TerminalBuffer&)> onRenderOverlay;
    std::function<bool()> shouldDimOverlayBackground;
    std::function<bool()> shouldClose;
};

void renderGeneratedWindow(GeneratedWindowBase& window, TerminalBuffer& buffer, int focusedIndex = -1, bool editMode = false,
                           std::string_view footer = {}, Element* activeScrollView = nullptr,
                           Element* activeScrollViewEditElement = nullptr);
[[nodiscard]] Size generatedWindowContentSize(const GeneratedWindowBase& window);
[[nodiscard]] Size generatedWindowContentSizeForWidth(const GeneratedWindowBase& window, int width);
[[nodiscard]] Size generatedWindowResolvedContentSize(const GeneratedWindowBase& window, Size size);
[[nodiscard]] RenderedContent renderGeneratedWindowContent(GeneratedWindowBase& window, Size size,
                                                           int focusedIndex = -1, bool editMode = false,
                                                           Element* activeScrollView = nullptr,
                                                           Element* activeScrollViewEditElement = nullptr,
                                                           const Style* windowStyleOverride = nullptr,
                                                           std::optional<Color> activeScrollViewFocusBackground = std::nullopt,
                                                           std::optional<int> clipTop = std::nullopt,
                                                           std::optional<int> clipBottom = std::nullopt,
                                                           bool applyActiveScrollViewDim = true);
void dimGeneratedWindowModalBackground(TerminalBuffer& buffer);
int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options = {});
int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options, int argc, char** argv);

}  // namespace ui

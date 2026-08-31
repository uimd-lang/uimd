#pragma once

#include "ui/generated/GeneratedWindowBase.hpp"
#include "ui/elements/Render.hpp"
#include "ui/terminal/TerminalBuffer.hpp"

#include <cstdint>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ui {

template <typename Signature>
class DeprecatedKeyCallback;

template <typename Result, typename... Args>
class DeprecatedKeyCallback<Result(Args...)> {
public:
    DeprecatedKeyCallback() = default;
    DeprecatedKeyCallback(const DeprecatedKeyCallback&) = default;
    DeprecatedKeyCallback(DeprecatedKeyCallback&&) noexcept = default;
    DeprecatedKeyCallback& operator=(const DeprecatedKeyCallback&) = default;
    DeprecatedKeyCallback& operator=(DeprecatedKeyCallback&&) noexcept = default;

    template <typename Callback>
        requires (!std::is_same_v<std::remove_cvref_t<Callback>, DeprecatedKeyCallback>)
    [[deprecated("Use GeneratedWindowBase::onPreviewKey; removal in UIMD 0.7.0")]]
    DeprecatedKeyCallback(Callback&& callback)
        : callback_(std::forward<Callback>(callback)) {
    }

    template <typename Callback>
        requires (!std::is_same_v<std::remove_cvref_t<Callback>, DeprecatedKeyCallback>)
    [[deprecated("Use GeneratedWindowBase::onPreviewKey; removal in UIMD 0.7.0")]]
    DeprecatedKeyCallback& operator=(Callback&& callback) {
        callback_ = std::forward<Callback>(callback);
        return *this;
    }

    [[nodiscard]] explicit operator bool() const noexcept {
        return static_cast<bool>(callback_);
    }

    Result operator()(Args... args) const {
        return callback_(std::forward<Args>(args)...);
    }

private:
    std::function<Result(Args...)> callback_;
};

struct GeneratedWindowFrameOptions {
    std::string className;
    std::string initialFocusName;
    bool startInEditMode = false;
    bool keepEditModeAfterConfirm = false;
    bool keepEditModeAfterEscape = false;
    bool dimBackground = true;
    std::function<void(std::string_view)> onButton;
    DeprecatedKeyCallback<bool(std::string_view, std::string_view, bool)> onKeyBeforeFocusedElement;
    DeprecatedKeyCallback<bool(std::string_view)> onKeyBeforeFocused;
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
    std::optional<int> textSelectionAnchor;
    double numberValue = 0.0;
    int selectedIndex = 0;
};

struct RememberedScrollViewDescendant {
    Element* element = nullptr;
    std::uint64_t scrollViewIdentity = 0;
    std::uint64_t elementIdentity = 0;
};

using ScrollViewLastDescendantMap = std::unordered_map<Element*, RememberedScrollViewDescendant>;

struct GeneratedWindowStackFrame {
    GeneratedWindowBase* window = nullptr;
    GeneratedWindowFrameOptions options;
    Rect bounds{};
    int focusedIndex = -1;
    bool editMode = false;
    Element* activeScrollView = nullptr;
    Element* activeScrollViewEditElement = nullptr;
    bool suppressActiveScrollViewScopeVisuals = false;
    std::optional<EditSnapshot> editSnapshot;
    ScrollViewLastDescendantMap scrollViewLastDescendant;
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
    DeprecatedKeyCallback<bool(std::string_view, std::string_view, bool)> onKeyBeforeFocusedElement;
    DeprecatedKeyCallback<bool(std::string_view)> onKeyBeforeFocused;
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
                           Element* activeScrollViewEditElement = nullptr,
                           bool suppressActiveScrollViewScopeVisuals = false);
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
                                                           bool applyActiveScrollViewDim = true,
                                                           bool forceFullscreenLayout = false,
                                                           bool useHostViewportForRootScrollViewIndicators = false,
                                                           bool suppressActiveScrollViewScopeVisuals = false);
void dimGeneratedWindowModalBackground(TerminalBuffer& buffer);
int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options = {});
int runGeneratedWindow(GeneratedWindowBase& window, GeneratedWindowRuntimeOptions options, int argc, char** argv);
int reportGeneratedAppUnhandledException(const std::exception& exc);
int reportGeneratedAppUnhandledException();

template <typename MainBody>
int runGeneratedAppMain(MainBody&& body)
{
    try
    {
        return std::forward<MainBody>(body)();
    }
    catch (const std::exception& exc)
    {
        return reportGeneratedAppUnhandledException(exc);
    }
    catch (...)
    {
        return reportGeneratedAppUnhandledException();
    }
}

}  // namespace ui

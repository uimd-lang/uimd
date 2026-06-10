#pragma once

#include "ui/app/Window.hpp"
#include "ui/core/Layout.hpp"
#include "ui/core/Style.hpp"
#include "ui/elements/ScrollView.hpp"

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace ui {

struct GeneratedLayoutEntry {
    std::string name;
    std::string type;
    std::string cellName;
    Rect relative;
    Rect sourceCell;
    AxisDimension width;
    AxisDimension height;
    AxisDimension cellWidth;
    AxisDimension cellHeight;
    Size charsSize;
    Size cellCharsSize;
    int marginRight = 0;
    int marginBottom = 0;
    Style cellStyle;
    Style elementStyle;
};

struct GeneratedElementMetadata {
    std::string name;
    std::string description;
    bool expose = true;
};

struct GeneratedAppToolMetadata {
    std::string name;
    std::string description;
    std::string inputSchemaJson;
    std::string outputSchemaJson;
};

class GeneratedWindowBase : public Window {
public:
    using Window::Window;

    [[nodiscard]] const std::vector<GeneratedLayoutEntry>& generatedLayout() const {
        return generatedLayout_;
    }

    [[nodiscard]] const Style& generatedWindowStyle() const {
        return windowStyle_;
    }

    [[nodiscard]] bool generatedFocusable() const {
        return generatedFocusable_;
    }

    [[nodiscard]] const std::string& generatedKind() const {
        return generatedKind_;
    }

    [[nodiscard]] bool mcpEnabled() const {
        return mcpEnabled_;
    }

    [[nodiscard]] const std::string& mcpSourceMarkdown() const {
        return mcpSourceMarkdown_;
    }

    [[nodiscard]] const std::string& mcpClassName() const {
        return mcpClassName_;
    }

    [[nodiscard]] const std::string& mcpWindowDescription() const {
        return mcpWindowDescription_;
    }

    [[nodiscard]] const GeneratedElementMetadata* mcpElementMetadata(std::string_view name) const {
        for (const GeneratedElementMetadata& metadata : mcpElementMetadata_) {
            if (metadata.name == name) {
                return &metadata;
            }
        }
        return nullptr;
    }

    [[nodiscard]] const std::vector<GeneratedAppToolMetadata>& mcpAppTools() const {
        return mcpAppTools_;
    }

    virtual std::string callAppTool(std::string_view name, std::string_view inputJson) {
        (void)inputJson;
        throw std::runtime_error("App MCP tool is not implemented: " + std::string(name));
    }

    virtual bool handleGeneratedButton(std::string_view name) {
        (void)name;
        return false;
    }

    virtual bool handleGeneratedTextChanged(std::string_view name, std::string_view value) {
        (void)name;
        (void)value;
        return false;
    }

    virtual bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) {
        (void)name;
        (void)value;
        return false;
    }

    virtual bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) {
        (void)name;
        (void)value;
        return false;
    }

    virtual bool activateGeneratedControl() {
        return false;
    }

    [[nodiscard]] virtual ScrollView* generatedScrollView() {
        return nullptr;
    }

    [[nodiscard]] virtual const ScrollView* generatedScrollView() const {
        return nullptr;
    }

protected:
    void setGeneratedLayout(std::vector<GeneratedLayoutEntry> layout) {
        generatedLayout_ = std::move(layout);
    }

    void setGeneratedWindowStyle(Style style) {
        windowStyle_ = std::move(style);
    }

    void setGeneratedFocusable(bool focusable) {
        generatedFocusable_ = focusable;
    }

    void setGeneratedKind(std::string kind) {
        generatedKind_ = std::move(kind);
    }

    void setMcpMetadata(bool enabled, std::string sourceMarkdown) {
        mcpEnabled_ = enabled;
        mcpSourceMarkdown_ = std::move(sourceMarkdown);
    }

    void setMcpMetadata(bool enabled, std::string className, std::string sourceMarkdown) {
        mcpEnabled_ = enabled;
        mcpClassName_ = std::move(className);
        mcpSourceMarkdown_ = std::move(sourceMarkdown);
    }

    void setMcpMetadata(
        bool enabled,
        std::string className,
        std::string sourceMarkdown,
        std::string windowDescription,
        std::vector<GeneratedElementMetadata> elementMetadata
    ) {
        mcpEnabled_ = enabled;
        mcpClassName_ = std::move(className);
        mcpSourceMarkdown_ = std::move(sourceMarkdown);
        mcpWindowDescription_ = std::move(windowDescription);
        mcpElementMetadata_ = std::move(elementMetadata);
    }

    void setMcpAppTools(std::vector<GeneratedAppToolMetadata> appTools) {
        mcpAppTools_ = std::move(appTools);
    }

private:
    std::vector<GeneratedLayoutEntry> generatedLayout_;
    Style windowStyle_;
    bool generatedFocusable_ = false;
    std::string generatedKind_ = "window";
    bool mcpEnabled_ = true;
    std::string mcpClassName_;
    std::string mcpSourceMarkdown_;
    std::string mcpWindowDescription_;
    std::vector<GeneratedElementMetadata> mcpElementMetadata_;
    std::vector<GeneratedAppToolMetadata> mcpAppTools_;
};

class GeneratedScrollViewBase : public GeneratedWindowBase {
public:
    using GeneratedWindowBase::GeneratedWindowBase;

    [[nodiscard]] ScrollView* generatedScrollView() override {
        return generatedScrollView_;
    }

    [[nodiscard]] const ScrollView* generatedScrollView() const override {
        return generatedScrollView_;
    }

    [[nodiscard]] ScrollView& scrollView() {
        if (generatedScrollView_ == nullptr) {
            throw std::runtime_error("generated scroll view has not been initialized");
        }
        return *generatedScrollView_;
    }

    [[nodiscard]] const ScrollView& scrollView() const {
        if (generatedScrollView_ == nullptr) {
            throw std::runtime_error("generated scroll view has not been initialized");
        }
        return *generatedScrollView_;
    }

    [[nodiscard]] const Rect& frame() const {
        return scrollView().frame();
    }

    void setAutoScroll(bool autoScroll) {
        scrollView().setAutoScroll(autoScroll);
    }

    void clearChildren() {
        scrollView().clearChildren();
    }

    Element& addChild(std::unique_ptr<Element> child) {
        return scrollView().addChild(std::move(child));
    }

    template <typename T, typename... Args>
    T& addChild(Args&&... args) {
        return scrollView().addChild<T>(std::forward<Args>(args)...);
    }

    void setDynamicChildrenRenderer(std::function<std::vector<RenderedContent>(int)> renderer) {
        scrollView().setDynamicChildrenRenderer(std::move(renderer));
    }

    void invalidateDynamicChildren() const {
        scrollView().invalidateDynamicChildren();
    }

    [[nodiscard]] int viewOffset() const {
        return scrollView().viewOffset();
    }

    [[nodiscard]] int scrollOffset() const {
        return scrollView().scrollOffset();
    }

    [[nodiscard]] ScrollViewPosition scrollPosition() const {
        return scrollView().scrollPosition();
    }

    void restoreScrollPosition(ScrollViewPosition position) {
        scrollView().restoreScrollPosition(position);
    }

    [[nodiscard]] int contentHeight() const {
        return scrollView().contentHeight();
    }

    [[nodiscard]] int contentHeight(int width) const {
        return scrollView().contentHeight(width);
    }

    [[nodiscard]] bool handleKey(std::string_view key) {
        const Rect scrollFrame = scrollView().frame();
        return scrollView().handleKey(key, Size{scrollFrame.width, scrollFrame.height});
    }

    [[nodiscard]] bool scrollToBottom() {
        const Rect scrollFrame = scrollView().frame();
        return scrollView().scrollToBottom(Size{scrollFrame.width, scrollFrame.height});
    }

    [[nodiscard]] bool scrollToTop() {
        const Rect scrollFrame = scrollView().frame();
        return scrollView().scrollToTop(Size{scrollFrame.width, scrollFrame.height});
    }

    [[nodiscard]] bool scrollToTop(Size viewport) {
        return scrollView().scrollToTop(viewport);
    }

    [[nodiscard]] bool scrollToBottom(Size viewport) {
        return scrollView().scrollToBottom(viewport);
    }

protected:
    void setGeneratedScrollView(ScrollView& scrollView) {
        generatedScrollView_ = &scrollView;
    }

private:
    ScrollView* generatedScrollView_ = nullptr;
};

}  // namespace ui

#pragma once

#include <memory>
#include <optional>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "ui/core/Rect.hpp"
#include "ui/core/Style.hpp"

namespace ui {

inline constexpr std::string_view kCommitModeStandard = "standard";
inline constexpr std::string_view kCommitModeLeave = "leave";

class Element {
public:
    explicit Element(std::string name);
    virtual ~Element() = default;

    Element(const Element&) = delete;
    Element& operator=(const Element&) = delete;
    Element(Element&&) = delete;
    Element& operator=(Element&&) = delete;

    [[nodiscard]] const std::string& name() const {
        return name_;
    }

    [[nodiscard]] std::uint64_t identity() const {
        return identity_;
    }

    void setCommitMode(std::string mode) {
        commitMode_ = std::move(mode);
    }

    [[nodiscard]] const std::string& commitMode() const {
        return commitMode_;
    }

    void setFocusable(bool focusable) {
        focusable_ = focusable;
    }

    [[nodiscard]] bool focusable() const {
        return focusable_;
    }

    void setEnabled(bool enabled) {
        enabled_ = enabled;
    }

    [[nodiscard]] bool enabled() const {
        return enabled_;
    }

    [[nodiscard]] Element* parent() const {
        return parent_;
    }

    // Background composited behind the element currently being rendered (set by
    // the window runtime). Used to resolve transparent image letterbox fills.
    [[nodiscard]] static std::optional<Color> renderingParentBackground();

    [[nodiscard]] const Rect& frame() const {
        return frame_;
    }

    void setFrame(Rect frame) {
        frame_ = frame;
    }

    [[nodiscard]] const Style& style() const {
        return style_;
    }

    void setStyle(Style style) {
        style_ = std::move(style);
    }

    [[nodiscard]] Style effectiveStyle(bool focused = false, bool editMode = false) const;

    void setFocusStyle(Style style) {
        focusStyle_ = std::move(style);
    }

    void setEditStyle(Style style) {
        editStyle_ = std::move(style);
    }

    void setCursorStyle(Style style) {
        cursorStyle_ = std::move(style);
    }

    void setSelectedStyle(Style style) {
        selectedStyle_ = std::move(style);
    }

    void setCheckedStyle(Style style) {
        checkedStyle_ = std::move(style);
    }

    void setUncheckedStyle(Style style) {
        uncheckedStyle_ = std::move(style);
    }

    void setDisabledStyle(Style style) {
        disabledStyle_ = std::move(style);
    }

    [[nodiscard]] const std::optional<Style>& cursorStyle() const {
        return cursorStyle_;
    }

    [[nodiscard]] const std::optional<Style>& focusStyle() const {
        return focusStyle_;
    }

    [[nodiscard]] const std::optional<Style>& editStyle() const {
        return editStyle_;
    }

    [[nodiscard]] const std::optional<Style>& selectedStyle() const {
        return selectedStyle_;
    }

    [[nodiscard]] const std::optional<Style>& checkedStyle() const {
        return checkedStyle_;
    }

    [[nodiscard]] const std::optional<Style>& uncheckedStyle() const {
        return uncheckedStyle_;
    }

    [[nodiscard]] const std::optional<Style>& disabledStyle() const {
        return disabledStyle_;
    }

    template <typename T, typename... Args>
    T& addChild(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        addChild(std::move(child));
        return ref;
    }

    Element& addChild(std::unique_ptr<Element> child);
    void clearChildElements();

    [[nodiscard]] const std::vector<std::unique_ptr<Element>>& children() const {
        return children_;
    }

private:
    void setParent(Element* parent) {
        parent_ = parent;
    }

    std::string name_;
    std::uint64_t identity_ = 0;
    std::string commitMode_ = std::string(kCommitModeStandard);
    bool focusable_ = true;
    bool enabled_ = true;
    Element* parent_ = nullptr;  // Non-owning; owned by the parent's children_ vector.
    Rect frame_;
    Style style_;
    std::optional<Style> focusStyle_;
    std::optional<Style> editStyle_;
    std::optional<Style> cursorStyle_;
    std::optional<Style> selectedStyle_;
    std::optional<Style> checkedStyle_;
    std::optional<Style> uncheckedStyle_;
    std::optional<Style> disabledStyle_;
    std::vector<std::unique_ptr<Element>> children_;
};

class ScopedElementParentBackground {
public:
    explicit ScopedElementParentBackground(std::optional<Color> background);
    ~ScopedElementParentBackground();

    ScopedElementParentBackground(const ScopedElementParentBackground&) = delete;
    ScopedElementParentBackground& operator=(const ScopedElementParentBackground&) = delete;

private:
    std::optional<Color> previous_;
};

}  // namespace ui

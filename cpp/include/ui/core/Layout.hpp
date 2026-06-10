#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ui/core/Rect.hpp"

namespace ui {

inline constexpr int kDefaultCellGap = 1;
inline constexpr int kDefaultCellPadding = 0;

enum class DimensionMode {
    Auto,
    Fixed,
    Expanded,
    FitContent,
};

enum class LayoutDirection {
    Row,
    Column,
};

struct AxisDimension {
    DimensionMode mode = DimensionMode::Auto;
    int value = 0;

    [[nodiscard]] static AxisDimension fixed(int value) {
        return AxisDimension{DimensionMode::Fixed, value};
    }

    [[nodiscard]] static AxisDimension expanded() {
        return AxisDimension{DimensionMode::Expanded, 0};
    }

    [[nodiscard]] static AxisDimension fitContent() {
        return AxisDimension{DimensionMode::FitContent, 0};
    }
};

struct LayoutElement {
    std::string name;
    Rect relative;
    AxisDimension width;
    AxisDimension height;
    Size fitContentSize;
};

struct LayoutCell {
    std::string name;
    AxisDimension width;
    AxisDimension height;
    Size sourceSize;
    Size fitContentSize;
    int padding = kDefaultCellPadding;
    int gap = kDefaultCellGap;
    LayoutDirection direction = LayoutDirection::Column;
    std::vector<LayoutElement> elements;
    std::vector<LayoutCell> children;
};

struct ResolvedElement {
    std::string name;
    Rect rect;
};

struct ResolvedCell {
    std::string name;
    Rect rect;
    std::vector<ResolvedElement> elements;
    std::vector<ResolvedCell> children;
};

struct FlatResolvedCell {
    std::string name;
    Rect rect;
};

struct FlatResolvedElement {
    std::string cellName;
    std::string name;
    Rect rect;
};

[[nodiscard]] const ResolvedCell* findCellByName(const ResolvedCell& root, std::string_view name);
[[nodiscard]] const ResolvedElement* findElementByName(const ResolvedCell& root, std::string_view name);
[[nodiscard]] std::vector<FlatResolvedCell> flattenCells(const ResolvedCell& root);
[[nodiscard]] std::vector<FlatResolvedElement> flattenElements(const ResolvedCell& root);

class LayoutResolver {
public:
    [[nodiscard]] ResolvedCell resolve(const LayoutCell& root, Rect bounds) const;

private:
    [[nodiscard]] ResolvedCell resolveCell(const LayoutCell& cell, Rect bounds) const;
    [[nodiscard]] Size measureCell(const LayoutCell& cell) const;
    [[nodiscard]] Size measureChildren(const LayoutCell& cell) const;
    [[nodiscard]] int resolveAxis(const AxisDimension& dimension, int fallback, int fitContent) const;
    [[nodiscard]] std::vector<int> resolveChildAxisSizes(const LayoutCell& cell, int available) const;
    [[nodiscard]] ResolvedElement resolveElement(const LayoutElement& element, Rect contentRect) const;
};

}  // namespace ui

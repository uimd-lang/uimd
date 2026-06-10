#include "ui/core/Layout.hpp"

#include <algorithm>
#include <numeric>

namespace ui {

namespace {

[[nodiscard]] int clampNonNegative(int value) {
    return std::max(0, value);
}

[[nodiscard]] int childAxisFixedSize(const LayoutCell& child, bool horizontal, const LayoutResolver& resolver) {
    const Size measured = resolver.resolve(child, Rect{0, 0, 0, 0}).rect.size();
    const AxisDimension& dimension = horizontal ? child.width : child.height;
    const int source = horizontal ? child.sourceSize.width : child.sourceSize.height;
    const int fit = horizontal ? measured.width : measured.height;
    switch (dimension.mode) {
        case DimensionMode::Fixed:
            return clampNonNegative(dimension.value);
        case DimensionMode::FitContent:
            return fit;
        case DimensionMode::Auto:
            return source > 0 ? source : fit;
        case DimensionMode::Expanded:
            return 0;
    }
    return 0;
}

[[nodiscard]] int childCrossSize(const LayoutCell& child, bool horizontal, int available, const LayoutResolver& resolver) {
    const Size measured = resolver.resolve(child, Rect{0, 0, 0, 0}).rect.size();
    const AxisDimension& dimension = horizontal ? child.height : child.width;
    const int source = horizontal ? child.sourceSize.height : child.sourceSize.width;
    const int fit = horizontal ? measured.height : measured.width;
    switch (dimension.mode) {
        case DimensionMode::Fixed:
            return clampNonNegative(dimension.value);
        case DimensionMode::Expanded:
            return clampNonNegative(available);
        case DimensionMode::FitContent:
            return fit;
        case DimensionMode::Auto:
            return source > 0 ? source : fit;
    }
    return 0;
}

[[nodiscard]] Rect inset(Rect rect, int padding) {
    const int safePadding = clampNonNegative(padding);
    return Rect{
        rect.row + safePadding,
        rect.col + safePadding,
        clampNonNegative(rect.width - (safePadding * 2)),
        clampNonNegative(rect.height - (safePadding * 2)),
    };
}

}  // namespace

const ResolvedCell* findCellByName(const ResolvedCell& root, std::string_view name) {
    if (root.name == name) {
        return &root;
    }
    for (const ResolvedCell& child : root.children) {
        if (const ResolvedCell* found = findCellByName(child, name)) {
            return found;
        }
    }
    return nullptr;
}

const ResolvedElement* findElementByName(const ResolvedCell& root, std::string_view name) {
    for (const ResolvedElement& element : root.elements) {
        if (element.name == name) {
            return &element;
        }
    }
    for (const ResolvedCell& child : root.children) {
        if (const ResolvedElement* found = findElementByName(child, name)) {
            return found;
        }
    }
    return nullptr;
}

std::vector<FlatResolvedCell> flattenCells(const ResolvedCell& root) {
    std::vector<FlatResolvedCell> cells;
    cells.push_back(FlatResolvedCell{.name = root.name, .rect = root.rect});
    for (const ResolvedCell& child : root.children) {
        std::vector<FlatResolvedCell> childCells = flattenCells(child);
        cells.insert(cells.end(), childCells.begin(), childCells.end());
    }
    return cells;
}

std::vector<FlatResolvedElement> flattenElements(const ResolvedCell& root) {
    std::vector<FlatResolvedElement> elements;
    for (const ResolvedElement& element : root.elements) {
        elements.push_back(FlatResolvedElement{
            .cellName = root.name,
            .name = element.name,
            .rect = element.rect,
        });
    }
    for (const ResolvedCell& child : root.children) {
        std::vector<FlatResolvedElement> childElements = flattenElements(child);
        elements.insert(elements.end(), childElements.begin(), childElements.end());
    }
    return elements;
}

ResolvedCell LayoutResolver::resolve(const LayoutCell& root, Rect bounds) const {
    Size measured = measureCell(root);
    const int width = bounds.width > 0 ? bounds.width : resolveAxis(root.width, root.sourceSize.width, measured.width);
    const int height = bounds.height > 0 ? bounds.height : resolveAxis(root.height, root.sourceSize.height, measured.height);
    return resolveCell(root, Rect{bounds.row, bounds.col, width, height});
}

ResolvedCell LayoutResolver::resolveCell(const LayoutCell& cell, Rect bounds) const {
    ResolvedCell resolved{.name = cell.name, .rect = bounds};
    const Rect contentRect = inset(bounds, cell.padding);

    for (const LayoutElement& element : cell.elements) {
        resolved.elements.push_back(resolveElement(element, contentRect));
    }

    if (cell.children.empty()) {
        return resolved;
    }

    const bool horizontal = cell.direction == LayoutDirection::Row;
    const int availableMain = horizontal ? contentRect.width : contentRect.height;
    const int availableCross = horizontal ? contentRect.height : contentRect.width;
    const std::vector<int> mainSizes = resolveChildAxisSizes(cell, availableMain);

    int cursor = horizontal ? contentRect.col : contentRect.row;
    for (std::size_t index = 0; index < cell.children.size(); ++index) {
        const LayoutCell& child = cell.children[index];
        const int mainSize = mainSizes[index];
        const int crossSize = childCrossSize(child, horizontal, availableCross, *this);
        const Rect childRect = horizontal
            ? Rect{contentRect.row, cursor, mainSize, crossSize}
            : Rect{cursor, contentRect.col, crossSize, mainSize};
        resolved.children.push_back(resolveCell(child, childRect));
        cursor += mainSize + cell.gap;
    }

    return resolved;
}

Size LayoutResolver::measureCell(const LayoutCell& cell) const {
    const Size childSize = measureChildren(cell);
    int contentWidth = std::max(cell.fitContentSize.width, childSize.width);
    int contentHeight = std::max(cell.fitContentSize.height, childSize.height);

    for (const LayoutElement& element : cell.elements) {
        const int elementWidth = resolveAxis(element.width, element.relative.width, element.fitContentSize.width);
        const int elementHeight = resolveAxis(element.height, element.relative.height, element.fitContentSize.height);
        contentWidth = std::max(contentWidth, element.relative.col + elementWidth);
        contentHeight = std::max(contentHeight, element.relative.row + elementHeight);
    }

    const int padding = clampNonNegative(cell.padding);
    return Size{contentWidth + (padding * 2), contentHeight + (padding * 2)};
}

Size LayoutResolver::measureChildren(const LayoutCell& cell) const {
    if (cell.children.empty()) {
        return Size{};
    }

    const bool horizontal = cell.direction == LayoutDirection::Row;
    int main = 0;
    int cross = 0;
    for (const LayoutCell& child : cell.children) {
        const Size childSize = measureCell(child);
        main += horizontal ? childSize.width : childSize.height;
        cross = std::max(cross, horizontal ? childSize.height : childSize.width);
    }

    main += clampNonNegative(cell.gap) * static_cast<int>(cell.children.size() - 1);
    return horizontal ? Size{main, cross} : Size{cross, main};
}

int LayoutResolver::resolveAxis(const AxisDimension& dimension, int fallback, int fitContent) const {
    switch (dimension.mode) {
        case DimensionMode::Fixed:
            return clampNonNegative(dimension.value);
        case DimensionMode::FitContent:
            return clampNonNegative(fitContent);
        case DimensionMode::Expanded:
            return clampNonNegative(fallback);
        case DimensionMode::Auto:
            return clampNonNegative(fallback > 0 ? fallback : fitContent);
    }
    return 0;
}

std::vector<int> LayoutResolver::resolveChildAxisSizes(const LayoutCell& cell, int available) const {
    const bool horizontal = cell.direction == LayoutDirection::Row;
    std::vector<int> sizes(cell.children.size(), 0);
    int expandedCount = 0;
    int fixedTotal = 0;

    for (std::size_t index = 0; index < cell.children.size(); ++index) {
        const LayoutCell& child = cell.children[index];
        const AxisDimension& dimension = horizontal ? child.width : child.height;
        if (dimension.mode == DimensionMode::Expanded) {
            ++expandedCount;
            continue;
        }
        sizes[index] = childAxisFixedSize(child, horizontal, *this);
        fixedTotal += sizes[index];
    }

    const int gaps = clampNonNegative(cell.gap) * static_cast<int>(cell.children.empty() ? 0 : cell.children.size() - 1);
    int remaining = clampNonNegative(available - fixedTotal - gaps);
    const int expandedShare = expandedCount > 0 ? remaining / expandedCount : 0;
    for (std::size_t index = 0; index < cell.children.size(); ++index) {
        const LayoutCell& child = cell.children[index];
        const AxisDimension& dimension = horizontal ? child.width : child.height;
        if (dimension.mode == DimensionMode::Expanded) {
            sizes[index] = expandedShare;
            remaining -= expandedShare;
            --expandedCount;
            if (expandedCount == 0) {
                sizes[index] += remaining;
            }
        }
    }

    return sizes;
}

ResolvedElement LayoutResolver::resolveElement(const LayoutElement& element, Rect contentRect) const {
    const int width = resolveAxis(element.width, element.relative.width, element.fitContentSize.width);
    const int height = resolveAxis(element.height, element.relative.height, element.fitContentSize.height);
    return ResolvedElement{
        .name = element.name,
        .rect = Rect{
            contentRect.row + element.relative.row,
            contentRect.col + element.relative.col,
            width,
            height,
        },
    };
}

}  // namespace ui

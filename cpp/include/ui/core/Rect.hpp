#pragma once

namespace ui {

struct Point {
    int row = 0;
    int col = 0;
};

struct Size {
    int width = 0;
    int height = 0;
};

struct Rect {
    int row = 0;
    int col = 0;
    int width = 0;
    int height = 0;

    [[nodiscard]] bool contains(Point point) const {
        return point.row >= row && point.row < row + height &&
               point.col >= col && point.col < col + width;
    }

    [[nodiscard]] bool empty() const {
        return width <= 0 || height <= 0;
    }

    [[nodiscard]] Point origin() const {
        return Point{row, col};
    }

    [[nodiscard]] Size size() const {
        return Size{width, height};
    }
};

}  // namespace ui

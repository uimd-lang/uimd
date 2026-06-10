#include "NativeGrid.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <set>

namespace uimd::tool
{
namespace
{

struct ExtractedContent
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int charsWidth = 0;
    int charsHeight = 0;
    int marginRight = 0;
    int marginBottom = 0;
    std::string nameOrText;
    bool hasWidthMarkers = false;
    bool hasHeightMarkers = false;
    std::string widthMode = "auto";
    std::string heightMode = "auto";
};

struct ExtractedCell
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int charsWidth = 0;
    int charsHeight = 0;
    std::string cellName;
    std::string widthMode = "auto";
    std::string heightMode = "auto";
    std::vector<ExtractedContent> elements;

    [[nodiscard]] int innerCharsWidth() const
    {
        return std::max(0, charsWidth - 1);
    }

    [[nodiscard]] int innerCharsHeight() const
    {
        return std::max(0, charsHeight - 1);
    }
};

struct LayoutCellGroup
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int charsWidth = 0;
    int charsHeight = 0;
    std::vector<std::unique_ptr<LayoutCellGroup>> tables;
    ExtractedCell* cell = nullptr;
};

std::vector<std::string> normalizeGrid(const std::string& uiText)
{
    std::string stripped = uiText;
    while (!stripped.empty() && stripped.front() == '\n')
    {
        stripped.erase(stripped.begin());
    }
    while (!stripped.empty() && stripped.back() == '\n')
    {
        stripped.pop_back();
    }
    if (stripped.find_first_not_of(" \r\n\t") == std::string::npos)
    {
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    for (char ch : stripped)
    {
        if (ch == '\n')
        {
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            lines.push_back(line);
            line.clear();
        }
        else
        {
            line += ch;
        }
    }
    if (!line.empty() || (!stripped.empty() && stripped.back() == '\n'))
    {
        lines.push_back(line);
    }

    std::size_t width = 0;
    for (const std::string& current : lines)
    {
        width = std::max(width, current.size());
    }
    for (std::string& current : lines)
    {
        current.resize(width, ' ');
    }
    return lines;
}

bool isCellNameStart(char ch)
{
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

bool isCellNameChar(char ch)
{
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

std::pair<std::string, int> readCellName(const std::string& line, int startX)
{
    while (startX < static_cast<int>(line.size()) && line[static_cast<std::size_t>(startX)] == '-')
    {
        ++startX;
    }
    if (startX >= static_cast<int>(line.size()) || !isCellNameStart(line[static_cast<std::size_t>(startX)]))
    {
        return {"", 0};
    }
    int endX = startX + 1;
    while (endX < static_cast<int>(line.size()) && isCellNameChar(line[static_cast<std::size_t>(endX)]))
    {
        ++endX;
    }
    return {line.substr(static_cast<std::size_t>(startX), static_cast<std::size_t>(endX - startX)), endX};
}

int readVerticalNumber(const std::vector<std::string>& grid, int x, int startY)
{
    std::string digits;
    for (int y = startY; y < static_cast<int>(grid.size()); ++y)
    {
        char ch = grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
        if (!std::isdigit(static_cast<unsigned char>(ch)))
        {
            break;
        }
        digits += ch;
    }
    return digits.empty() ? 0 : std::stoi(digits);
}

int readHorizontalNumber(const std::vector<std::string>& grid, int y, int startX)
{
    std::string digits;
    const std::string& line = grid[static_cast<std::size_t>(y)];
    for (int x = startX; x < static_cast<int>(line.size()); ++x)
    {
        char ch = line[static_cast<std::size_t>(x)];
        if (!std::isdigit(static_cast<unsigned char>(ch)))
        {
            break;
        }
        digits += ch;
    }
    return digits.empty() ? 0 : std::stoi(digits);
}

std::vector<ExtractedCell> findRawCells(const std::vector<std::string>& grid)
{
    const int gridHeight = static_cast<int>(grid.size());
    const int gridWidth = grid.empty() ? 0 : static_cast<int>(grid.front().size());
    std::vector<ExtractedCell> cells;

    for (int y = 0; y < gridHeight - 1; ++y)
    {
        for (int x = 0; x < gridWidth - 1; ++x)
        {
            if (grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)] != '+')
            {
                continue;
            }
            if (grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(x + 1)] != '-' ||
                grid[static_cast<std::size_t>(y + 1)][static_cast<std::size_t>(x)] != '|')
            {
                continue;
            }

            int cellWidth = 0;
            int cellHeight = 0;
            std::string widthMode = "auto";
            std::string heightMode = "auto";
            int charsWidth = 0;
            int charsHeight = 0;
            auto [cellName, cellNameEnd] = readCellName(grid[static_cast<std::size_t>(y)], x + 2);

            for (int offsetY = y + 1; offsetY < gridHeight; ++offsetY)
            {
                char ch = grid[static_cast<std::size_t>(offsetY)][static_cast<std::size_t>(x)];
                char right = x + 1 < gridWidth ? grid[static_cast<std::size_t>(offsetY)][static_cast<std::size_t>(x + 1)] : '\0';
                char bottom = offsetY + 1 < gridHeight ? grid[static_cast<std::size_t>(offsetY + 1)][static_cast<std::size_t>(x)] : '\0';
                char prev = offsetY - 1 >= 0 ? grid[static_cast<std::size_t>(offsetY - 1)][static_cast<std::size_t>(x)] : '\0';
                if (ch == '+' && right == '-')
                {
                    charsHeight = offsetY - y;
                    break;
                }
                if (std::isdigit(static_cast<unsigned char>(ch)) && !std::isdigit(static_cast<unsigned char>(prev)))
                {
                    cellHeight = readVerticalNumber(grid, x, offsetY);
                    heightMode = "fixed";
                }
                else if (ch == '*' && bottom == '*')
                {
                    cellHeight = GRID_EXPANDED;
                    heightMode = "expanded";
                }
                else if (ch == '#' && bottom == '#')
                {
                    cellHeight = GRID_FIT_CONTENT;
                    heightMode = "fit-content";
                }
            }

            for (int offsetX = x + 1; offsetX < gridWidth; ++offsetX)
            {
                char ch = grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(offsetX)];
                char bottom = y + 1 < gridHeight ? grid[static_cast<std::size_t>(y + 1)][static_cast<std::size_t>(offsetX)] : '\0';
                char right = offsetX + 1 < gridWidth ? grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(offsetX + 1)] : '\0';
                char prev = offsetX - 1 >= 0 ? grid[static_cast<std::size_t>(y)][static_cast<std::size_t>(offsetX - 1)] : '\0';
                if (ch == '+' && bottom == '|')
                {
                    charsWidth = offsetX - x;
                    break;
                }
                if (cellNameEnd != 0 && x + 2 <= offsetX && offsetX < cellNameEnd)
                {
                    continue;
                }
                if (std::isdigit(static_cast<unsigned char>(ch)) && !std::isdigit(static_cast<unsigned char>(prev)))
                {
                    cellWidth = readHorizontalNumber(grid, y, offsetX);
                    widthMode = "fixed";
                }
                else if (ch == '*' && right == '*')
                {
                    cellWidth = GRID_EXPANDED;
                    widthMode = "expanded";
                }
                else if (ch == '#' && right == '#')
                {
                    cellWidth = GRID_FIT_CONTENT;
                    widthMode = "fit-content";
                }
            }

            if (charsWidth <= 1 || charsHeight <= 1)
            {
                continue;
            }
            cells.push_back(ExtractedCell{x, y, cellWidth, cellHeight, charsWidth, charsHeight, cellName, widthMode, heightMode, {}});
        }
    }
    return cells;
}

bool isElementStart(const std::vector<std::string>& grid, int row, int col, int left)
{
    char ch = grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
    if (std::string{" .*|+-#"}.find(ch) != std::string::npos)
    {
        return false;
    }
    char prev = col > left ? grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(col - 1)] : ' ';
    if (ch == '"')
    {
        return std::string{" .*"}.find(prev) != std::string::npos;
    }
    return std::string{" .*\""}.find(prev) != std::string::npos;
}

bool isTokenChar(char ch)
{
    return std::isalnum(static_cast<unsigned char>(ch)) || std::string{"_-:/"}.find(ch) != std::string::npos;
}

std::pair<std::string, int> readToken(const std::string& line, int start, int right)
{
    if (line[static_cast<std::size_t>(start)] == '"')
    {
        int end = start + 1;
        while (end < right && line[static_cast<std::size_t>(end)] != '"')
        {
            ++end;
        }
        if (end < right && line[static_cast<std::size_t>(end)] == '"')
        {
            ++end;
        }
        return {line.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(end - start)), end};
    }
    int end = start;
    while (end < right && isTokenChar(line[static_cast<std::size_t>(end)]))
    {
        ++end;
    }
    return {line.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(end - start)), end};
}

std::vector<ExtractedContent> extractElements(const std::vector<std::string>& grid, const ExtractedCell& rawCell)
{
    const int top = rawCell.y + 1;
    const int left = rawCell.x + 1;
    const int bottom = rawCell.y + rawCell.charsHeight;
    const int right = rawCell.x + rawCell.charsWidth;
    std::vector<ExtractedContent> elements;

    for (int row = top; row < bottom; ++row)
    {
        int col = left;
        while (col < right)
        {
            if (!isElementStart(grid, row, col, left))
            {
                ++col;
                continue;
            }
            auto [token, tokenEnd] = readToken(grid[static_cast<std::size_t>(row)], col, right);
            if (token.empty())
            {
                ++col;
                continue;
            }

            ExtractedContent element;
            element.x = col - left;
            element.y = row - top;
            element.charsWidth = std::max(1, tokenEnd - col);
            element.charsHeight = 1;
            element.nameOrText = token;

            int markerEnd = tokenEnd;
            std::string widthRun;
            while (markerEnd < right && std::string{".*#"}.find(grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(markerEnd)]) != std::string::npos)
            {
                widthRun += grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(markerEnd)];
                ++markerEnd;
            }
            if (!widthRun.empty())
            {
                element.hasWidthMarkers = true;
                element.charsWidth = std::max(element.charsWidth, markerEnd - col);
                if (widthRun.find("##") != std::string::npos)
                {
                    element.width = GRID_FIT_CONTENT;
                    element.widthMode = "fit-content";
                }
                else if (widthRun.find('*') != std::string::npos)
                {
                    element.width = GRID_EXPANDED;
                    element.widthMode = "expanded";
                }
            }

            std::string heightRun;
            int markerRow = row + 1;
            while (markerRow < bottom && std::string{".*#"}.find(grid[static_cast<std::size_t>(markerRow)][static_cast<std::size_t>(col)]) != std::string::npos)
            {
                heightRun += grid[static_cast<std::size_t>(markerRow)][static_cast<std::size_t>(col)];
                ++markerRow;
            }
            if (!heightRun.empty())
            {
                element.hasHeightMarkers = true;
                element.charsHeight = 1 + static_cast<int>(heightRun.size());
                if (heightRun.find("##") != std::string::npos)
                {
                    element.height = GRID_FIT_CONTENT;
                    element.heightMode = "fit-content";
                }
                else if (heightRun.find('*') != std::string::npos)
                {
                    element.height = GRID_EXPANDED;
                    element.heightMode = "expanded";
                }
            }

            element.marginRight = std::max(0, rawCell.innerCharsWidth() - element.x - element.charsWidth);
            element.marginBottom = std::max(0, rawCell.innerCharsHeight() - element.y - element.charsHeight);
            elements.push_back(element);
            col = std::max(markerEnd, tokenEnd);
        }
    }
    return elements;
}

std::set<const ExtractedCell*> resultExpandedWidthCells(const std::vector<ExtractedCell>& rawCells)
{
    std::set<std::pair<int, int>> expandedColumns;
    for (const ExtractedCell& cell : rawCells)
    {
        if (cell.width == GRID_EXPANDED)
        {
            expandedColumns.insert({cell.x, cell.charsWidth});
        }
    }

    std::set<const ExtractedCell*> result;
    for (const ExtractedCell& cell : rawCells)
    {
        if (cell.width == GRID_EXPANDED || expandedColumns.find({cell.x, cell.charsWidth}) != expandedColumns.end())
        {
            result.insert(&cell);
        }
    }
    return result;
}

void applyRowLastElementDefaults(ExtractedCell& rawCell, bool resultWidthExpanded)
{
    if (resultWidthExpanded)
    {
        return;
    }
    std::vector<int> rows;
    for (const ExtractedContent& element : rawCell.elements)
    {
        if (std::find(rows.begin(), rows.end(), element.y) == rows.end())
        {
            rows.push_back(element.y);
        }
    }
    for (int row : rows)
    {
        auto last = std::max_element(rawCell.elements.begin(), rawCell.elements.end(), [row](const ExtractedContent& left, const ExtractedContent& right) {
            if (left.y != row)
            {
                return true;
            }
            if (right.y != row)
            {
                return false;
            }
            return left.x < right.x;
        });
        if (last != rawCell.elements.end() && last->y == row && last->width == 0)
        {
            last->width = GRID_EXPANDED;
            last->widthMode = "expanded";
        }
    }
}

void applySingleElementDefaults(ExtractedCell& rawCell)
{
    if (rawCell.elements.size() != 1)
    {
        return;
    }
    ExtractedContent& element = rawCell.elements.front();
    if (element.width == 0)
    {
        element.width = element.hasWidthMarkers ? element.charsWidth : (rawCell.width != 0 ? rawCell.width : rawCell.innerCharsWidth());
    }
    if (element.height == 0)
    {
        element.height = element.charsHeight;
    }
}

void applyElementDefaults(std::vector<ExtractedCell>& rawCells)
{
    const std::set<const ExtractedCell*> expandedCells = resultExpandedWidthCells(rawCells);
    for (ExtractedCell& rawCell : rawCells)
    {
        applyRowLastElementDefaults(rawCell, expandedCells.find(&rawCell) != expandedCells.end());
        applySingleElementDefaults(rawCell);
    }
}

std::unique_ptr<LayoutCellGroup> makeGroup(ExtractedCell* cell)
{
    auto group = std::make_unique<LayoutCellGroup>();
    group->x = cell->x;
    group->y = cell->y;
    group->width = cell->width;
    group->height = cell->height;
    group->charsWidth = cell->charsWidth;
    group->charsHeight = cell->charsHeight;
    group->cell = cell;
    return group;
}

std::unique_ptr<LayoutCellGroup> buildTableTree(std::vector<ExtractedCell>& rawCells)
{
    std::vector<std::unique_ptr<LayoutCellGroup>> tables;
    for (ExtractedCell& cell : rawCells)
    {
        tables.push_back(makeGroup(&cell));
    }

    while (tables.size() > 1)
    {
        bool merged = false;
        for (std::size_t i = 0; i < tables.size() && !merged; ++i)
        {
            for (std::size_t j = i + 1; j < tables.size(); ++j)
            {
                LayoutCellGroup& left = *tables[i];
                LayoutCellGroup& right = *tables[j];
                if (left.x + left.charsWidth == right.x && left.y == right.y && left.charsHeight == right.charsHeight)
                {
                    left.charsWidth += right.charsWidth;
                    left.tables.push_back(std::move(tables[j]));
                    tables.erase(tables.begin() + static_cast<long>(j));
                    merged = true;
                    break;
                }
                if (left.y + left.charsHeight == right.y && left.x == right.x && left.charsWidth == right.charsWidth)
                {
                    left.charsHeight += right.charsHeight;
                    left.tables.push_back(std::move(tables[j]));
                    tables.erase(tables.begin() + static_cast<long>(j));
                    merged = true;
                    break;
                }
            }
        }
        if (!merged)
        {
            break;
        }
    }

    if (tables.size() == 1)
    {
        return std::move(tables.front());
    }

    auto root = std::make_unique<LayoutCellGroup>();
    root->x = tables.front()->x;
    root->y = tables.front()->y;
    int maxX = tables.front()->x + tables.front()->charsWidth;
    int maxY = tables.front()->y + tables.front()->charsHeight;
    for (auto& table : tables)
    {
        root->x = std::min(root->x, table->x);
        root->y = std::min(root->y, table->y);
        maxX = std::max(maxX, table->x + table->charsWidth);
        maxY = std::max(maxY, table->y + table->charsHeight);
        root->tables.push_back(std::move(table));
    }
    root->charsWidth = maxX - root->x;
    root->charsHeight = maxY - root->y;
    return root;
}

void fillEmptyWidthAndHeight(LayoutCellGroup& table)
{
    if (table.cell != nullptr)
    {
        if (table.cell->width == 0)
        {
            table.cell->width = table.cell->innerCharsWidth();
        }
        if (table.cell->height == 0)
        {
            table.cell->height = table.cell->innerCharsHeight();
        }
    }
    for (auto& child : table.tables)
    {
        fillEmptyWidthAndHeight(*child);
    }
}

std::vector<ExtractedCell*> collectRootCells(const LayoutCellGroup& root, std::vector<ExtractedCell>& rawCells)
{
    (void)root;
    std::vector<ExtractedCell*> result;
    for (ExtractedCell& cell : rawCells)
    {
        result.push_back(&cell);
    }
    return result;
}

NativeLayoutItem buildEmptyLayoutCell(const ExtractedCell& rawCell)
{
    NativeLayoutItem cell;
    cell.cellRow = rawCell.y;
    cell.cellCol = rawCell.x;
    cell.cellCharsWidth = rawCell.innerCharsWidth();
    cell.cellCharsHeight = rawCell.innerCharsHeight();
    cell.cellName = rawCell.cellName;
    cell.cellWidth = rawCell.width == GRID_EXPANDED ? 0 : rawCell.width;
    cell.cellHeight = rawCell.height == GRID_EXPANDED ? 0 : rawCell.height;
    cell.cellWidthMode = rawCell.widthMode;
    cell.cellHeightMode = rawCell.heightMode;
    cell.width = rawCell.width;
    cell.height = rawCell.height;
    cell.charsWidth = rawCell.innerCharsWidth();
    cell.charsHeight = rawCell.innerCharsHeight();
    if (cell.width == 0)
    {
        cell.width = cell.charsWidth;
    }
    if (cell.height == 0)
    {
        cell.height = cell.charsHeight;
    }
    return cell;
}

std::vector<NativeLayoutItem> rawCellToLayoutCells(const ExtractedCell& rawCell)
{
    if (rawCell.elements.empty())
    {
        return {buildEmptyLayoutCell(rawCell)};
    }
    std::vector<NativeLayoutItem> result;
    const bool multiElement = rawCell.elements.size() > 1;
    for (const ExtractedContent& element : rawCell.elements)
    {
        NativeLayoutItem cell;
        cell.row = element.y;
        cell.col = element.x;
        cell.cellRow = rawCell.y;
        cell.cellCol = rawCell.x;
        cell.cellCharsWidth = rawCell.innerCharsWidth();
        cell.cellCharsHeight = rawCell.innerCharsHeight();
        cell.cellName = rawCell.cellName;
        cell.cellWidth = rawCell.width == GRID_EXPANDED ? 0 : rawCell.width;
        cell.cellHeight = rawCell.height == GRID_EXPANDED ? 0 : rawCell.height;
        cell.cellWidthMode = rawCell.widthMode;
        cell.cellHeightMode = rawCell.heightMode;
        cell.width = element.width;
        cell.height = element.height;
        cell.widthMode = element.widthMode;
        cell.heightMode = element.heightMode;
        cell.charsWidth = element.charsWidth;
        cell.charsHeight = element.charsHeight;
        cell.marginRight = element.marginRight;
        cell.marginBottom = element.marginBottom;
        cell.content = element.nameOrText;
        if (cell.width == 0)
        {
            cell.width = (!multiElement && rawCell.width == GRID_EXPANDED) ? GRID_EXPANDED : cell.charsWidth;
        }
        if (cell.height == 0)
        {
            cell.height = (!multiElement && rawCell.height == GRID_EXPANDED) ? GRID_EXPANDED : cell.charsHeight;
        }
        result.push_back(cell);
    }
    return result;
}

}  // namespace

std::vector<std::string> extractNamedCells(const std::string& uiText)
{
    const std::vector<std::string> grid = normalizeGrid(uiText);
    std::vector<std::string> names;
    for (const ExtractedCell& cell : findRawCells(grid))
    {
        if (!cell.cellName.empty())
        {
            names.push_back(cell.cellName);
        }
    }
    return names;
}

std::vector<NativeLayoutItem> parseGrid(const std::string& uiText)
{
    const std::vector<std::string> grid = normalizeGrid(uiText);
    if (grid.empty())
    {
        return {};
    }
    std::vector<ExtractedCell> rawCells = findRawCells(grid);
    if (rawCells.empty())
    {
        return {};
    }
    for (ExtractedCell& rawCell : rawCells)
    {
        rawCell.elements = extractElements(grid, rawCell);
    }
    std::unique_ptr<LayoutCellGroup> root = buildTableTree(rawCells);
    fillEmptyWidthAndHeight(*root);
    applyElementDefaults(rawCells);

    std::vector<ExtractedCell*> rootCells = collectRootCells(*root, rawCells);
    std::sort(rootCells.begin(), rootCells.end(), [](const ExtractedCell* left, const ExtractedCell* right) {
        if (left->y != right->y)
        {
            return left->y < right->y;
        }
        return left->x < right->x;
    });

    std::vector<NativeLayoutItem> layout;
    for (const ExtractedCell* rawCell : rootCells)
    {
        std::vector<NativeLayoutItem> extracted = rawCellToLayoutCells(*rawCell);
        layout.insert(layout.end(), extracted.begin(), extracted.end());
    }

    std::sort(layout.begin(), layout.end(), [](const NativeLayoutItem& left, const NativeLayoutItem& right) {
        if (left.row != right.row)
        {
            return left.row < right.row;
        }
        if (left.col != right.col)
        {
            return left.col < right.col;
        }
        return left.content < right.content;
    });
    return layout;
}

}  // namespace uimd::tool

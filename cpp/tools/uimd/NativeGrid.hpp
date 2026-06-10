#pragma once

#include <string>
#include <vector>

namespace uimd::tool
{

constexpr int GRID_EXPANDED = 0x1FFFFFFF;
constexpr int GRID_FIT_CONTENT = 0x1FFFFFFE;

struct NativeLayoutItem
{
    int row = 0;
    int col = 0;
    int cellRow = 0;
    int cellCol = 0;
    int cellCharsWidth = 0;
    int cellCharsHeight = 0;
    std::string cellName;
    int cellWidth = 0;
    int cellHeight = 0;
    std::string cellWidthMode = "auto";
    std::string cellHeightMode = "auto";
    int width = 0;
    int height = 0;
    std::string widthMode = "auto";
    std::string heightMode = "auto";
    int charsWidth = 0;
    int charsHeight = 0;
    int marginRight = 0;
    int marginBottom = 0;
    std::string content;
};

std::vector<NativeLayoutItem> parseGrid(const std::string& uiText);
std::vector<std::string> extractNamedCells(const std::string& uiText);

}  // namespace uimd::tool

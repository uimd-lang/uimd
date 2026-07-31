#pragma once

#include "NativeModel.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace uimd::tool
{

struct CompilerMember
{
    std::string name;
    std::string type = "label";
    std::string description;
    std::string text;
    std::string title;
    std::string value;
    std::string source;
    std::string customSource;
    std::string alt;
    std::string fit = "contain";
    std::string renderMode = "auto";
    std::string align = "center";
    std::string verticalAlign = "middle";
    std::string commitMode;
    bool hasCommitMode = false;
    bool expose = true;
    bool checked = false;
    bool multiple = false;
    int maxLength = 0;
    double numberValue = 0.0;
    double numberStep = 1.0;
    std::vector<std::string> options;
    std::vector<std::string> selectedValues;
    int selectedItemIndex = -1;
    int selectedItemsIndex = -1;
};

struct CompilerDocument
{
    NativeDocument document;
    YamlMap style;
    YamlMap mcpMetadata;
    YamlMap mcpTools;
    std::string extends = "uiwindow";
    std::string direction;
    bool focusable = false;
    std::string kind = "window";
    std::string description;
    std::vector<std::string> dependencies;
    std::vector<CompilerMember> members;
};

CompilerDocument parseCompilerDocument(const std::filesystem::path& sourcePath);

std::string compilerJsonCompact(const YamlValue& value);

bool compilerIsBuiltinType(const std::string& elementType);

YamlMap compilerCellStyle(const YamlMap& style, const std::string& cellName);

YamlMap compilerLayoutElementStyle(
    const YamlMap& style,
    const std::string& elementType,
    const std::string& name,
    const std::string& cellName
);

YamlMap compilerWindowStyle(const YamlMap& style);

std::vector<std::pair<std::string, YamlMap>> compilerStyleStatesForElement(
    const YamlMap& style,
    const std::string& elementType,
    const std::string& name,
    const std::string& cellName,
    const YamlMap* member
);

std::vector<std::pair<std::string, YamlMap>> compilerScrollviewExtensionStyleStates(
    const YamlMap& style
);

YamlMap compilerScrollviewExtensionDescendantFocusStyle(const YamlMap& style);

int compilerScrollviewExtensionGap(const YamlMap& style);

std::vector<std::filesystem::path> compilerDependencyPaths(
    const std::filesystem::path& sourcePath,
    const CompilerDocument& model
);

}  // namespace uimd::tool

#include "NativeSwiftGenerator.hpp"

#include "NativeModel.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#ifndef UIMD_NATIVE_SOURCE_ROOT
#define UIMD_NATIVE_SOURCE_ROOT ""
#endif

namespace uimd::tool
{
namespace
{

constexpr const char* DEFAULT_EXTENDS = "uiwindow";
constexpr const char* GENERATED_SCROLLVIEW_ELEMENT_NAME = "__scrollview";

const std::set<std::string> BUILTIN_TYPES{
    "label",
    "spanlabel",
    "infolabel",
    "framebufferview",
    "messagetable",
    "image",
    "textinput",
    "textarea",
    "numberinput",
    "button",
    "checkbox",
    "combobox",
    "listbox",
    "uiscrollview",
    "viewhost",
};

std::string pathString(const std::filesystem::path& path)
{
    return path.lexically_normal().string();
}

std::filesystem::path absolutePath(const std::filesystem::path& path)
{
    return std::filesystem::absolute(path).lexically_normal();
}

std::string trim(const std::string& value)
{
    std::size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
    {
        ++start;
    }
    std::size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
    {
        --end;
    }
    return value.substr(start, end - start);
}

std::string lower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool endsWith(const std::string& value, const std::string& suffix)
{
    return value.size() >= suffix.size() && value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

const YamlValue* yamlGet(const YamlMap& map, const std::string& key)
{
    return map.get(key);
}

const YamlMap* valueAsMap(const YamlValue* value)
{
    if (value == nullptr || !std::holds_alternative<YamlMap>(value->value))
    {
        return nullptr;
    }
    return &std::get<YamlMap>(value->value);
}

const YamlList* valueAsList(const YamlValue* value)
{
    if (value == nullptr || !std::holds_alternative<YamlList>(value->value))
    {
        return nullptr;
    }
    return &std::get<YamlList>(value->value);
}

YamlValue makeString(const std::string& value)
{
    return YamlValue{value};
}

YamlValue makeMap(YamlMap value)
{
    return YamlValue{std::move(value)};
}

YamlValue makeList(YamlList value)
{
    return YamlValue{std::move(value)};
}

std::string valueAsString(const YamlValue& value, const std::string& fallback = "")
{
    if (std::holds_alternative<std::string>(value.value))
    {
        return std::get<std::string>(value.value);
    }
    if (std::holds_alternative<long long>(value.value))
    {
        return std::to_string(std::get<long long>(value.value));
    }
    if (std::holds_alternative<double>(value.value))
    {
        std::ostringstream output;
        output << std::setprecision(15) << std::get<double>(value.value);
        return output.str();
    }
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? "true" : "false";
    }
    return fallback;
}

std::string yamlString(const YamlMap& map, const std::string& key, const std::string& fallback = "")
{
    const YamlValue* value = yamlGet(map, key);
    return value == nullptr ? fallback : valueAsString(*value, fallback);
}

bool truthy(const YamlValue* value, bool fallback = false)
{
    if (value == nullptr)
    {
        return fallback;
    }
    if (std::holds_alternative<bool>(value->value))
    {
        return std::get<bool>(value->value);
    }
    if (std::holds_alternative<long long>(value->value))
    {
        return std::get<long long>(value->value) != 0;
    }
    if (std::holds_alternative<double>(value->value))
    {
        return std::get<double>(value->value) != 0.0;
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        const std::string normalized = lower(trim(std::get<std::string>(value->value)));
        return !(normalized.empty() || normalized == "0" || normalized == "false" || normalized == "no" || normalized == "off");
    }
    if (std::holds_alternative<YamlList>(value->value))
    {
        return !std::get<YamlList>(value->value).empty();
    }
    return !std::get<YamlMap>(value->value).empty();
}

int valueAsInt(const YamlValue* value, int fallback = 0)
{
    if (value == nullptr)
    {
        return fallback;
    }
    if (std::holds_alternative<long long>(value->value))
    {
        return static_cast<int>(std::get<long long>(value->value));
    }
    if (std::holds_alternative<double>(value->value))
    {
        return static_cast<int>(std::get<double>(value->value));
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        try
        {
            return std::stoi(std::get<std::string>(value->value));
        }
        catch (const std::exception&)
        {
            return fallback;
        }
    }
    return fallback;
}

double valueAsDouble(const YamlValue* value, double fallback = 0.0)
{
    if (value == nullptr)
    {
        return fallback;
    }
    if (std::holds_alternative<long long>(value->value))
    {
        return static_cast<double>(std::get<long long>(value->value));
    }
    if (std::holds_alternative<double>(value->value))
    {
        return std::get<double>(value->value);
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        try
        {
            return std::stod(std::get<std::string>(value->value));
        }
        catch (const std::exception&)
        {
            return fallback;
        }
    }
    return fallback;
}

std::string swiftString(const std::string& value)
{
    std::string result = "\"";
    for (unsigned char ch : value)
    {
        if (ch == '\\')
        {
            result += "\\\\";
        }
        else if (ch == '"')
        {
            result += "\\\"";
        }
        else if (ch == '\n')
        {
            result += "\\n";
        }
        else if (ch == '\r')
        {
            result += "\\r";
        }
        else if (ch == '\t')
        {
            result += "\\t";
        }
        else if (ch < 0x20)
        {
            std::ostringstream escape;
            escape << "\\u{" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch) << "}";
            result += escape.str();
        }
        else
        {
            result += static_cast<char>(ch);
        }
    }
    result += "\"";
    return result;
}

std::string swiftStringValue(const YamlValue* value, const std::string& fallback = "")
{
    return swiftString(value == nullptr ? fallback : valueAsString(*value, fallback));
}

std::string className(std::string baseName)
{
    std::string result;
    std::string current;
    auto flush = [&]() {
        if (current.empty())
        {
            return;
        }
        current[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(current[0])));
        for (std::size_t index = 1; index < current.size(); ++index)
        {
            current[index] = static_cast<char>(std::tolower(static_cast<unsigned char>(current[index])));
        }
        result += current;
        current.clear();
    };
    for (char ch : baseName)
    {
        if (ch == '_' || ch == '-' || std::isspace(static_cast<unsigned char>(ch)))
        {
            flush();
        }
        else
        {
            current += ch;
        }
    }
    flush();
    return result + "UI";
}

std::string appClassName(const std::string& classNameValue)
{
    if (endsWith(classNameValue, "UI"))
    {
        return classNameValue.substr(0, classNameValue.size() - 2) + "App";
    }
    return classNameValue + "App";
}

std::string typeFor(const YamlMap& member)
{
    return lower(yamlString(member, "type", "label"));
}

bool isBuiltinType(const std::string& elemType)
{
    return BUILTIN_TYPES.find(elemType) != BUILTIN_TYPES.end();
}

std::string swiftElementType(const std::string& elemType)
{
    if (elemType == "label")
    {
        return "Label";
    }
    if (elemType == "spanlabel")
    {
        return "SpanLabel";
    }
    if (elemType == "infolabel")
    {
        return "InfoLabel";
    }
    if (elemType == "framebufferview")
    {
        return "FrameBufferView";
    }
    if (elemType == "messagetable")
    {
        return "MessageTable";
    }
    if (elemType == "image")
    {
        return "Image";
    }
    if (elemType == "button")
    {
        return "Button";
    }
    if (elemType == "checkbox")
    {
        return "CheckBox";
    }
    if (elemType == "textinput")
    {
        return "TextInput";
    }
    if (elemType == "textarea")
    {
        return "TextArea";
    }
    if (elemType == "numberinput")
    {
        return "NumberInput";
    }
    if (elemType == "combobox")
    {
        return "ComboBox";
    }
    if (elemType == "listbox")
    {
        return "ListBox";
    }
    if (elemType == "uiscrollview")
    {
        return "ScrollView";
    }
    if (elemType == "viewhost")
    {
        return "ViewHost";
    }
    return "ReusableElement";
}

std::vector<std::string> listValue(const YamlValue* value)
{
    std::vector<std::string> result;
    const YamlList* list = valueAsList(value);
    if (list == nullptr)
    {
        return result;
    }
    for (const YamlValue& item : *list)
    {
        result.push_back(valueAsString(item));
    }
    return result;
}

std::string listCode(const std::vector<std::string>& values)
{
    std::string result = "[";
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += swiftString(values[index]);
    }
    result += "]";
    return result;
}

int selectedIndex(const YamlMap& member, const std::string& key)
{
    const std::vector<std::string> options = listValue(yamlGet(member, "options"));
    if (options.empty())
    {
        return -1;
    }
    const YamlValue* selectedValue = yamlGet(member, key);
    if (selectedValue == nullptr)
    {
        return -1;
    }
    std::string selected;
    bool hasSelected = false;
    const YamlList* selectedList = valueAsList(selectedValue);
    if (selectedList != nullptr)
    {
        if (!selectedList->empty())
        {
            selected = valueAsString(selectedList->front());
            hasSelected = true;
        }
    }
    else
    {
        selected = valueAsString(*selectedValue);
        hasSelected = true;
    }
    if (!hasSelected)
    {
        return -1;
    }
    for (std::size_t index = 0; index < options.size(); ++index)
    {
        if (options[index] == selected)
        {
            return static_cast<int>(index);
        }
    }
    return -1;
}

std::vector<std::string> selectedValues(const YamlMap& member, const std::string& key)
{
    const std::vector<std::string> options = listValue(yamlGet(member, "options"));
    const std::vector<std::string> selected = listValue(yamlGet(member, key));
    std::vector<std::string> result;
    for (const std::string& option : options)
    {
        if (std::find(selected.begin(), selected.end(), option) != selected.end())
        {
            result.push_back(option);
        }
    }
    return result;
}

std::string doubleCode(double value)
{
    std::ostringstream output;
    output << std::setprecision(15) << value;
    std::string result = output.str();
    if (result.find('.') == std::string::npos && result.find('e') == std::string::npos && result.find('E') == std::string::npos)
    {
        result += ".0";
    }
    return result;
}

std::string ctorArgs(const std::string& name, const YamlMap& member)
{
    const std::string elemType = typeFor(member);
    if (elemType == "label" || elemType == "spanlabel" || elemType == "infolabel" || elemType == "messagetable")
    {
        return swiftString(name) + ", " + swiftString(yamlString(member, "text", name));
    }
    if (elemType == "image")
    {
        return swiftString(name) + ", source: " + swiftStringValue(yamlGet(member, "source")) +
            ", alt: " + swiftStringValue(yamlGet(member, "alt")) +
            ", fit: " + swiftString(yamlString(member, "fit", "contain")) +
            ", renderMode: " + swiftString(yamlString(member, "render_mode", "auto")) +
            ", align: " + swiftString(yamlString(member, "align", "center")) +
            ", verticalAlign: " + swiftString(yamlString(member, "valign", "middle"));
    }
    if (elemType == "button")
    {
        return swiftString(name) + ", " + swiftString(yamlString(member, "title", name));
    }
    if (elemType == "checkbox")
    {
        const YamlValue* checkedValue = yamlGet(member, "checked");
        if (checkedValue == nullptr)
        {
            checkedValue = yamlGet(member, "value");
        }
        return swiftString(name) + ", " + swiftString(yamlString(member, "title", name)) + ", " + (truthy(checkedValue, false) ? "true" : "false");
    }
    if (elemType == "textinput" || elemType == "textarea")
    {
        const int maxLength = valueAsInt(yamlGet(member, "maxlength"), 0);
        return swiftString(name) + ", " + swiftStringValue(yamlGet(member, "value")) + ", maxLength: " + std::to_string(maxLength);
    }
    if (elemType == "numberinput")
    {
        const YamlValue* stepValue = yamlGet(member, "step_size") != nullptr ? yamlGet(member, "step_size") : yamlGet(member, "step");
        return swiftString(name) + ", " + doubleCode(valueAsDouble(yamlGet(member, "value"), 0.0)) +
            ", step: " + doubleCode(valueAsDouble(stepValue, 1.0));
    }
    if (elemType == "combobox")
    {
        return swiftString(name) + ", " + listCode(listValue(yamlGet(member, "options"))) +
            ", selectedIndex: " + std::to_string(selectedIndex(member, "selected_item"));
    }
    if (elemType == "listbox")
    {
        return swiftString(name) + ", " + listCode(listValue(yamlGet(member, "options"))) +
            ", selectedValues: " + listCode(selectedValues(member, "selected_items"));
    }
    return swiftString(name);
}

std::string dimensionCode(const std::string& mode, int value)
{
    if (mode == "fixed")
    {
        return ".fixed(" + std::to_string(value) + ")";
    }
    if (mode == "expanded")
    {
        return ".expanded";
    }
    if (mode == "fit-content")
    {
        return ".fitContent";
    }
    return ".auto(" + std::to_string(value) + ")";
}

std::string rectCode(int row, int col, int width, int height)
{
    return "Rect(row: " + std::to_string(row) + ", col: " + std::to_string(col) +
        ", width: " + std::to_string(width) + ", height: " + std::to_string(height) + ")";
}

std::string sizeCode(int width, int height)
{
    return "Size(width: " + std::to_string(width) + ", height: " + std::to_string(height) + ")";
}

YamlMap cellStyle(const YamlMap& style, const std::string& cellName);
YamlMap layoutElementStyle(const YamlMap& style, const std::string& elemType, const std::string& name, const std::string& cellName);
std::string styleCode(const YamlMap& style);

YamlMap scrollviewExtensionLayoutCellStyle(const YamlMap& input)
{
    YamlMap result;
    for (const auto& [key, value] : input)
    {
        if (key != "padding")
        {
            result[key] = value;
        }
    }
    return result;
}

std::string layoutEntry(const NativeLayoutItem& cell, const YamlMap& members, const YamlMap& style, bool scrollviewExtension)
{
    std::string name = cell.content;
    std::string widthMode = cell.widthMode;
    int widthValue = cell.width;
    std::string heightMode = cell.heightMode;
    int heightValue = cell.height;
    const YamlMap* member = name.empty() ? nullptr : valueAsMap(yamlGet(members, name));
    std::string elemType = member == nullptr ? (name.empty() ? "" : "label") : typeFor(*member);
    if (scrollviewExtension && name.empty() && (cell.cellName == "panel" || cell.cellName == "viewport"))
    {
        name = GENERATED_SCROLLVIEW_ELEMENT_NAME;
        elemType = "uiscrollview";
        widthMode = "expanded";
        widthValue = 0;
        heightMode = "expanded";
        heightValue = 0;
    }
    YamlMap cellStyleMap = cellStyle(style, cell.cellName);
    if (scrollviewExtension && name == GENERATED_SCROLLVIEW_ELEMENT_NAME)
    {
        cellStyleMap = scrollviewExtensionLayoutCellStyle(cellStyleMap);
    }
    const YamlMap elementStyle = layoutElementStyle(style, elemType, name, cell.cellName);
    return "            GeneratedLayoutEntry(\n"
        "                name: " + swiftString(name) + ",\n"
        "                type: " + swiftString(elemType) + ",\n"
        "                cellName: " + swiftString(cell.cellName) + ",\n"
        "                relative: " + rectCode(cell.row, cell.col, cell.width, cell.height) + ",\n"
        "                sourceCell: " + rectCode(cell.cellRow, cell.cellCol, cell.cellCharsWidth, cell.cellCharsHeight) + ",\n"
        "                width: " + dimensionCode(widthMode, widthValue) + ",\n"
        "                height: " + dimensionCode(heightMode, heightValue) + ",\n"
        "                cellWidth: " + dimensionCode(cell.cellWidthMode, cell.cellWidth) + ",\n"
        "                cellHeight: " + dimensionCode(cell.cellHeightMode, cell.cellHeight) + ",\n"
        "                charsSize: " + sizeCode(cell.charsWidth, cell.charsHeight) + ",\n"
        "                cellCharsSize: " + sizeCode(cell.cellCharsWidth, cell.cellCharsHeight) + ",\n"
        "                marginRight: " + std::to_string(cell.marginRight) + ",\n"
        "                marginBottom: " + std::to_string(cell.marginBottom) + ",\n"
        "                cellStyle: " + styleCode(cellStyleMap) + ",\n"
        "                elementStyle: " + styleCode(elementStyle) + "\n"
        "            ),";
}

std::string identifierFragment(const std::string& name)
{
    std::vector<std::string> parts;
    std::string current;
    for (unsigned char ch : name)
    {
        if (std::isalnum(ch))
        {
            current += static_cast<char>(ch);
        }
        else if (!current.empty())
        {
            parts.push_back(current);
            current.clear();
        }
    }
    if (!current.empty())
    {
        parts.push_back(current);
    }
    std::string result;
    for (std::string part : parts)
    {
        part[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(part[0])));
        for (std::size_t index = 1; index < part.size(); ++index)
        {
            part[index] = static_cast<char>(std::tolower(static_cast<unsigned char>(part[index])));
        }
        result += part;
    }
    return result.empty() ? "Element" : result;
}

std::string eventMethodName(const std::string& name, const std::string& suffix)
{
    return "on" + identifierFragment(name) + suffix;
}

struct EventSpec
{
    std::string name;
    std::string channel;
    std::string methodName;
    std::string argType;
};

std::vector<EventSpec> eventSpecsForMember(const std::string& name, const YamlMap& member)
{
    const std::string elemType = typeFor(member);
    if (elemType == "button" || elemType == "image")
    {
        return {{name, "button", eventMethodName(name, "Click"), ""}};
    }
    if (elemType == "checkbox")
    {
        return {{name, "text", eventMethodName(name, "Change"), "String"}};
    }
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return {
            {name, "text", eventMethodName(name, "Change"), "String"},
            {name, "confirmed", eventMethodName(name, "Submit"), "String"},
        };
    }
    if (elemType == "combobox")
    {
        return {{name, "selection_text", eventMethodName(name, "Change"), "String"}};
    }
    if (elemType == "listbox")
    {
        return {{name, "selection", eventMethodName(name, "SelectionChange"), "[String]"}};
    }
    return {};
}

std::vector<EventSpec> eventSpecs(const YamlMap& members)
{
    std::vector<EventSpec> specs;
    std::set<std::pair<std::string, std::string>> seen;
    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        for (const EventSpec& spec : eventSpecsForMember(name, *member))
        {
            const auto key = std::make_pair(spec.methodName, spec.argType);
            if (seen.insert(key).second)
            {
                specs.push_back(spec);
            }
        }
    }
    return specs;
}

std::vector<std::string> dependenciesFromDefinition(const YamlMap& definition)
{
    const YamlValue* uses = yamlGet(definition, "uses");
    if (uses == nullptr)
    {
        uses = yamlGet(definition, "dependencies");
    }
    std::vector<std::string> result;
    if (uses == nullptr)
    {
        return result;
    }
    if (std::holds_alternative<std::string>(uses->value))
    {
        result.push_back(std::get<std::string>(uses->value));
    }
    else if (std::holds_alternative<YamlList>(uses->value))
    {
        for (const YamlValue& item : std::get<YamlList>(uses->value))
        {
            const std::string value = valueAsString(item);
            if (!value.empty())
            {
                result.push_back(value);
            }
        }
    }
    return result;
}

std::string customSourceName(const YamlMap& member, const std::string& elemType)
{
    return yamlString(member, "source", elemType);
}

std::vector<std::string> dependencyNames(const YamlMap& members, const std::vector<std::string>& explicitDependencies)
{
    std::vector<std::string> names;
    std::set<std::string> seen;
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string elemType = typeFor(*member);
        if (isBuiltinType(elemType))
        {
            continue;
        }
        const std::string sourceName = customSourceName(*member, elemType);
        if (seen.insert(sourceName).second)
        {
            names.push_back(sourceName);
        }
    }
    for (const std::string& dependency : explicitDependencies)
    {
        std::filesystem::path dependencyPath{dependency};
        dependencyPath.replace_extension();
        const std::string sourceName = dependencyPath.string();
        if (seen.insert(sourceName).second)
        {
            names.push_back(sourceName);
        }
    }
    return names;
}

std::filesystem::path findDependencyFile(
    const std::filesystem::path& mdPath,
    const std::string& sourceName,
    const std::string& suffix
)
{
    const std::filesystem::path root = absolutePath(mdPath).parent_path();
    const std::filesystem::path parent = root.parent_path();
    std::vector<std::filesystem::path> candidates{
        root / (sourceName + suffix),
        root / sourceName / (std::filesystem::path(sourceName).filename().string() + suffix),
        parent / (sourceName + suffix),
        parent / sourceName / (std::filesystem::path(sourceName).filename().string() + suffix),
    };
    for (const std::filesystem::path& candidate : candidates)
    {
        if (std::filesystem::exists(candidate))
        {
            return absolutePath(candidate);
        }
    }
    for (const std::filesystem::path& base : {root, parent})
    {
        if (!std::filesystem::is_directory(base))
        {
            continue;
        }
        std::vector<std::filesystem::path> matches;
        for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(base))
        {
            if (entry.is_regular_file() && entry.path().filename() == std::filesystem::path(sourceName).filename().string() + suffix)
            {
                matches.push_back(entry.path());
            }
        }
        std::sort(matches.begin(), matches.end());
        if (!matches.empty())
        {
            return absolutePath(matches.front());
        }
    }
    return {};
}

std::filesystem::path findDependencySourceFile(const std::filesystem::path& mdPath, const std::string& sourceName)
{
    std::filesystem::path result = findDependencyFile(mdPath, sourceName, ".uimd");
    if (!result.empty())
    {
        return result;
    }
    return findDependencyFile(mdPath, sourceName, ".md");
}

bool isLegacyUimdFile(const std::filesystem::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        return false;
    }
    std::string head(4096, '\0');
    input.read(head.data(), static_cast<std::streamsize>(head.size()));
    head.resize(static_cast<std::size_t>(input.gcount()));
    return head.find("format: ui-md") != std::string::npos || head.find("format: uimd") != std::string::npos;
}

std::vector<std::filesystem::path> discoverUiSourceFiles(const std::filesystem::path& sourceDir)
{
    std::vector<std::filesystem::path> candidates;
    if (!std::filesystem::is_directory(sourceDir))
    {
        return candidates;
    }
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(sourceDir))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }
        const std::filesystem::path path = entry.path();
        if (path.extension() == ".uimd" || (path.extension() == ".md" && isLegacyUimdFile(path)))
        {
            candidates.push_back(path);
        }
    }
    std::sort(candidates.begin(), candidates.end());
    return candidates;
}

std::vector<std::filesystem::path> sourceFiles(const std::filesystem::path& sourcePath)
{
    if (std::filesystem::is_regular_file(sourcePath))
    {
        return {sourcePath};
    }
    return discoverUiSourceFiles(sourcePath);
}

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeSwiftGenerateOptions& options)
{
    if (options.hasOutputDir)
    {
        return absolutePath(options.outputDir);
    }
    return absolutePath(sourceFile).parent_path();
}

std::vector<std::filesystem::path> dedupePaths(const std::vector<std::filesystem::path>& paths)
{
    std::set<std::filesystem::path> seen;
    std::vector<std::filesystem::path> result;
    for (const std::filesystem::path& path : paths)
    {
        const std::filesystem::path normalized = absolutePath(path);
        if (seen.insert(normalized).second)
        {
            result.push_back(path);
        }
    }
    return result;
}

std::filesystem::path projectRootFor(const std::filesystem::path& sourcePath)
{
    const char* overrideRoot = std::getenv("UIMD_SOURCE_ROOT");
    if (overrideRoot != nullptr && *overrideRoot != '\0')
    {
        return absolutePath(overrideRoot);
    }
    const std::filesystem::path configuredRoot{UIMD_NATIVE_SOURCE_ROOT};
    if (!configuredRoot.empty() && std::filesystem::is_regular_file(configuredRoot / "shared" / "themes" / "dark.uimd"))
    {
        return absolutePath(configuredRoot);
    }
    std::filesystem::path current = absolutePath(sourcePath);
    if (std::filesystem::is_regular_file(current))
    {
        current = current.parent_path();
    }
    while (!current.empty() && current != current.parent_path())
    {
        if (std::filesystem::is_regular_file(current / "shared" / "themes" / "dark.uimd"))
        {
            return current;
        }
        current = current.parent_path();
    }
    return {};
}

std::string packageReferencePathFor(const std::filesystem::path& packagePath)
{
    const std::filesystem::path root = projectRootFor(packagePath);
    const std::filesystem::path runtimePackage = root.empty()
        ? std::filesystem::path{"swift"} / "src" / "Uimd"
        : root / "swift" / "src" / "Uimd";
    std::error_code error;
    std::filesystem::path relativePath = std::filesystem::relative(runtimePackage, packagePath.parent_path(), error);
    if (!error && !relativePath.empty())
    {
        return relativePath.generic_string();
    }
    return runtimePackage.generic_string();
}

YamlMap flattenStyleMapping(const YamlMap& styleTree);
YamlMap loadIncludedStyle(const std::string& name, const std::filesystem::path& sourcePath);

void flattenStyleTree(const YamlMap& tree, YamlMap& flattened, const std::string& prefix = "")
{
    const std::set<std::string> nestedProps{"text-color-gradient", "text-background-gradient"};
    for (const auto& [key, value] : tree)
    {
        const std::string selector = prefix.empty() ? key : prefix + " " + key;
        const YamlMap* childMap = valueAsMap(&value);
        if (!prefix.empty() && nestedProps.find(key) != nestedProps.end() && childMap != nullptr)
        {
            YamlValue& slot = flattened[prefix];
            if (!std::holds_alternative<YamlMap>(slot.value))
            {
                slot = makeMap(YamlMap{});
            }
            std::get<YamlMap>(slot.value)[key] = value;
        }
        else if (childMap != nullptr)
        {
            YamlMap props;
            for (const auto& [childKey, childValue] : *childMap)
            {
                if (!std::holds_alternative<YamlMap>(childValue.value))
                {
                    props[childKey] = childValue;
                }
            }
            if (!props.empty())
            {
                flattened[selector] = makeMap(props);
            }
            for (const auto& [childKey, childValue] : *childMap)
            {
                if (std::holds_alternative<YamlMap>(childValue.value))
                {
                    YamlMap nested;
                    nested[childKey] = childValue;
                    flattenStyleTree(nested, flattened, selector);
                }
            }
        }
        else
        {
            YamlValue& slot = flattened[prefix];
            if (!std::holds_alternative<YamlMap>(slot.value))
            {
                slot = makeMap(YamlMap{});
            }
            std::get<YamlMap>(slot.value)[key] = value;
        }
    }
}

YamlMap flattenStyleMapping(const YamlMap& styleTree)
{
    YamlMap flattened;
    flattenStyleTree(styleTree, flattened);
    return flattened;
}

YamlMap mergeStyles(const YamlMap& base, const YamlMap& override)
{
    YamlMap result;
    std::set<std::string> baseSelectors;
    for (const auto& [selector, value] : base)
    {
        baseSelectors.insert(selector);
        const YamlMap* baseProps = valueAsMap(&value);
        const YamlMap* overrideProps = valueAsMap(yamlGet(override, selector));
        if (baseProps != nullptr && overrideProps != nullptr)
        {
            YamlMap merged = *baseProps;
            for (const auto& [key, propValue] : *overrideProps)
            {
                merged[key] = propValue;
            }
            result[selector] = makeMap(merged);
        }
        else if (overrideProps != nullptr)
        {
            result[selector] = makeMap(*overrideProps);
        }
        else if (baseProps != nullptr)
        {
            result[selector] = makeMap(*baseProps);
        }
    }
    for (const auto& [selector, value] : override)
    {
        if (baseSelectors.find(selector) != baseSelectors.end())
        {
            continue;
        }
        const YamlMap* props = valueAsMap(&value);
        result[selector] = props == nullptr ? value : makeMap(*props);
    }
    return result;
}

YamlMap styleForDocument(const NativeDocument& document, const std::filesystem::path& sourcePath, bool isTheme)
{
    if (!document.style.empty())
    {
        YamlMap styleTree;
        std::string includeName;
        for (const auto& [key, value] : document.style)
        {
            if (key == "include")
            {
                includeName = valueAsString(value);
            }
            else
            {
                styleTree[key] = value;
            }
        }
        YamlMap local = flattenStyleMapping(styleTree);
        if (!includeName.empty())
        {
            return mergeStyles(loadIncludedStyle(includeName, sourcePath), local);
        }
        if (!isTheme)
        {
            YamlMap darkStyle = loadIncludedStyle("dark", sourcePath);
            if (!darkStyle.empty())
            {
                return mergeStyles(darkStyle, local);
            }
        }
        return local;
    }
    if (!isTheme)
    {
        return loadIncludedStyle("dark", sourcePath);
    }
    return {};
}

YamlMap loadIncludedStyle(const std::string& name, const std::filesystem::path& sourcePath)
{
    if (name.empty())
    {
        return {};
    }
    std::vector<std::string> candidateNames;
    if (endsWith(name, ".uimd") || endsWith(name, ".md"))
    {
        candidateNames.push_back(name);
    }
    else
    {
        candidateNames.push_back(name + ".uimd");
        candidateNames.push_back(name + ".md");
    }

    std::vector<std::filesystem::path> searchDirs;
    if (!sourcePath.empty())
    {
        searchDirs.push_back(absolutePath(sourcePath).parent_path());
    }
    const std::filesystem::path root = projectRootFor(sourcePath);
    if (!root.empty())
    {
        searchDirs.push_back(root / "shared" / "themes");
        searchDirs.push_back(root / "src" / "shared" / "themes");
        searchDirs.push_back(root / "src" / "uimd" / "themes");
    }
    const char* sdkPythonTarget = std::getenv("UIMD_SDK_PYTHON_TARGET");
    if (sdkPythonTarget != nullptr && *sdkPythonTarget != '\0')
    {
        searchDirs.push_back(std::filesystem::path{sdkPythonTarget} / "uimd" / "themes");
    }

    for (const std::filesystem::path& directory : searchDirs)
    {
        for (const std::string& candidateName : candidateNames)
        {
            const std::filesystem::path candidate = directory / candidateName;
            if (std::filesystem::exists(candidate))
            {
                NativeDocument theme = parseDocumentFile(pathString(candidate));
                return styleForDocument(theme, candidate, true);
            }
        }
    }
    return {};
}

bool looksLikeJsonSchema(const YamlMap& value)
{
    return value.contains("type") || value.contains("oneOf") || value.contains("anyOf") || value.contains("allOf") || value.contains("$ref");
}

YamlMap schemaForTypeName(const std::string& typeName)
{
    const std::string normalized = lower(trim(typeName));
    std::string mapped;
    if (normalized == "str" || normalized == "string")
    {
        mapped = "string";
    }
    else if (normalized == "int" || normalized == "integer")
    {
        mapped = "integer";
    }
    else if (normalized == "float" || normalized == "number")
    {
        mapped = "number";
    }
    else if (normalized == "bool" || normalized == "boolean")
    {
        mapped = "boolean";
    }
    else if (normalized == "object")
    {
        mapped = "object";
    }
    else if (normalized == "array")
    {
        mapped = "array";
    }
    if (!mapped.empty())
    {
        YamlMap result;
        result["type"] = makeString(mapped);
        return result;
    }
    if (normalized == "any")
    {
        return {};
    }
    YamlMap result;
    result["type"] = makeString("string");
    result["description"] = makeString(typeName);
    return result;
}

YamlMap schemaFromShortForm(const YamlValue* value, bool topLevelObject = false)
{
    if (value == nullptr || std::holds_alternative<std::nullptr_t>(value->value))
    {
        if (topLevelObject)
        {
            YamlMap result;
            result["type"] = makeString("object");
            result["properties"] = makeMap(YamlMap{});
            return result;
        }
        return {};
    }
    if (std::holds_alternative<YamlMap>(value->value))
    {
        const YamlMap& input = std::get<YamlMap>(value->value);
        if (looksLikeJsonSchema(input))
        {
            return input;
        }
        YamlMap properties;
        YamlList required;
        for (const auto& [key, item] : input)
        {
            if (key == "required")
            {
                const YamlList* requiredList = valueAsList(&item);
                if (requiredList != nullptr)
                {
                    for (const YamlValue& entry : *requiredList)
                    {
                        required.push_back(makeString(valueAsString(entry)));
                    }
                }
                continue;
            }
            properties[key] = makeMap(schemaFromShortForm(&item));
        }
        YamlMap schema;
        schema["type"] = makeString("object");
        schema["properties"] = makeMap(properties);
        if (!required.empty())
        {
            schema["required"] = makeList(required);
        }
        return schema;
    }
    if (std::holds_alternative<YamlList>(value->value))
    {
        const YamlList& list = std::get<YamlList>(value->value);
        YamlMap schema;
        schema["type"] = makeString("array");
        if (!list.empty())
        {
            schema["items"] = makeMap(schemaFromShortForm(&list.front()));
        }
        return schema;
    }
    return schemaForTypeName(valueAsString(*value));
}

YamlMap buildMcpTools(const YamlMap& tools)
{
    YamlMap result;
    for (const auto& [name, value] : tools)
    {
        const YamlMap* props = valueAsMap(&value);
        if (props == nullptr)
        {
            continue;
        }
        YamlMap definition;
        definition["name"] = makeString(name);
        definition["description"] = makeString(yamlString(*props, "description", ""));
        definition["inputSchema"] = makeMap(schemaFromShortForm(yamlGet(*props, "input"), true));
        if (yamlGet(*props, "output") != nullptr)
        {
            definition["outputSchema"] = makeMap(schemaFromShortForm(yamlGet(*props, "output"), true));
        }
        result[name] = makeMap(definition);
    }
    return result;
}

std::string jsonEscape(const std::string& value)
{
    std::string result = "\"";
    for (unsigned char ch : value)
    {
        if (ch == '\\')
        {
            result += "\\\\";
        }
        else if (ch == '"')
        {
            result += "\\\"";
        }
        else if (ch == '\n')
        {
            result += "\\n";
        }
        else if (ch == '\t')
        {
            result += "\\t";
        }
        else if (ch == '\r')
        {
            result += "\\r";
        }
        else if (ch < 0x20)
        {
            std::ostringstream escape;
            escape << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(ch);
            result += escape.str();
        }
        else
        {
            result += static_cast<char>(ch);
        }
    }
    result += "\"";
    return result;
}

std::string jsonCompact(const YamlValue& value);

std::string jsonCompact(const YamlMap& map)
{
    std::string result = "{";
    bool first = true;
    for (const auto& [key, item] : map)
    {
        if (!first)
        {
            result += ",";
        }
        first = false;
        result += jsonEscape(key) + ":" + jsonCompact(item);
    }
    result += "}";
    return result;
}

std::string jsonCompact(const YamlList& list)
{
    std::string result = "[";
    bool first = true;
    for (const YamlValue& item : list)
    {
        if (!first)
        {
            result += ",";
        }
        first = false;
        result += jsonCompact(item);
    }
    result += "]";
    return result;
}

std::string jsonCompact(const YamlValue& value)
{
    if (std::holds_alternative<std::nullptr_t>(value.value))
    {
        return "null";
    }
    if (std::holds_alternative<std::string>(value.value))
    {
        return jsonEscape(std::get<std::string>(value.value));
    }
    if (std::holds_alternative<long long>(value.value))
    {
        return std::to_string(std::get<long long>(value.value));
    }
    if (std::holds_alternative<double>(value.value))
    {
        std::ostringstream output;
        output << std::setprecision(15) << std::get<double>(value.value);
        return output.str();
    }
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? "true" : "false";
    }
    if (std::holds_alternative<YamlList>(value.value))
    {
        return jsonCompact(std::get<YamlList>(value.value));
    }
    return jsonCompact(std::get<YamlMap>(value.value));
}

std::string styleStateName(const std::string& key)
{
    for (const std::string& prefix : {"focus-", "edit-", "cursor-", "selected-", "active-", "checked-", "unchecked-", "disabled-", "error-"})
    {
        if (key.rfind(prefix, 0) == 0)
        {
            return prefix.substr(0, prefix.size() - 1);
        }
    }
    return {};
}

std::string stylePropName(const std::string& key)
{
    const std::string state = styleStateName(key);
    return state.empty() ? key : key.substr(state.size() + 1);
}

YamlMap selectorMatches(const YamlMap& style, const std::string& selector)
{
    const YamlMap* props = valueAsMap(yamlGet(style, selector));
    return props == nullptr ? YamlMap{} : *props;
}

YamlMap styleForElement(const YamlMap& style, const std::string& elemType, const std::string& name, const std::string& cellName = "")
{
    YamlMap merged;
    std::vector<std::string> selectors{elemType};
    if (!cellName.empty())
    {
        selectors.push_back(cellName + " " + elemType);
    }
    selectors.push_back("@" + name);
    if (!cellName.empty())
    {
        selectors.push_back(cellName + " @" + name);
    }
    for (const std::string& selector : selectors)
    {
        YamlMap props = selectorMatches(style, selector);
        for (const auto& [key, value] : props)
        {
            merged[key] = value;
        }
    }
    return merged;
}

YamlMap cellStyle(const YamlMap& style, const std::string& cellName)
{
    YamlMap merged;
    std::vector<std::string> selectors{"cell"};
    if (!cellName.empty())
    {
        selectors.push_back(cellName);
        selectors.push_back("@" + cellName);
    }
    for (const std::string& selector : selectors)
    {
        YamlMap props = selectorMatches(style, selector);
        for (const auto& [key, value] : props)
        {
            if (styleStateName(key).empty())
            {
                merged[key] = value;
            }
        }
    }
    return merged;
}

YamlMap layoutElementStyle(const YamlMap& style, const std::string& elemType, const std::string& name, const std::string& cellName)
{
    if (elemType.empty())
    {
        return {};
    }
    YamlMap result;
    YamlMap merged = styleForElement(style, elemType, name, cellName);
    for (const auto& [key, value] : merged)
    {
        if (styleStateName(key).empty())
        {
            result[key] = value;
        }
    }
    return result;
}

YamlMap styleWindow(const YamlMap& style)
{
    YamlMap result;
    const YamlMap* props = valueAsMap(yamlGet(style, "this"));
    if (props == nullptr)
    {
        return result;
    }
    const std::set<std::string> allowed{
        "color",
        "background",
        "background-texture",
        "background-texture-color",
        "scope-dim-background",
        "border-color",
        "border-width",
        "border-width-horizontal",
        "border-width-vertical",
        "padding",
        "margin",
        "gap",
    };
    for (const auto& [key, value] : *props)
    {
        if (allowed.find(key) != allowed.end())
        {
            result[key] = value;
        }
    }
    return result;
}

YamlMap inlineMemberStyle(const YamlMap* member)
{
    YamlMap result;
    if (member == nullptr)
    {
        return result;
    }
    const std::set<std::string> allowed{"color", "background", "background-texture", "background-texture-color"};
    for (const auto& [key, value] : *member)
    {
        if (allowed.find(key) != allowed.end())
        {
            result[key] = value;
        }
    }
    return result;
}

std::vector<std::pair<std::string, YamlMap>> styleStatesForElement(
    const YamlMap& style,
    const std::string& elemType,
    const std::string& name,
    const std::string& cellName = "",
    const YamlMap* member = nullptr
)
{
    YamlMap merged = styleForElement(style, elemType, name, cellName);
    YamlMap inlineStyle = inlineMemberStyle(member);
    for (const auto& [key, value] : inlineStyle)
    {
        merged[key] = value;
    }

    std::vector<std::pair<std::string, YamlMap>> states{
        {"setStyle", {}},
        {"setFocusStyle", {}},
        {"setEditStyle", {}},
        {"setCursorStyle", {}},
        {"setSelectedStyle", {}},
        {"setActiveStyle", {}},
        {"setCheckedStyle", {}},
        {"setUncheckedStyle", {}},
        {"setDisabledStyle", {}},
    };
    auto stateForSetter = [&states](const std::string& setter) -> YamlMap& {
        for (auto& [currentSetter, map] : states)
        {
            if (currentSetter == setter)
            {
                return map;
            }
        }
        return states.front().second;
    };
    for (const auto& [key, value] : merged)
    {
        const std::string state = styleStateName(key);
        if (state == "error")
        {
            continue;
        }
        std::string setter = "setStyle";
        if (state == "focus")
        {
            setter = "setFocusStyle";
        }
        else if (state == "edit")
        {
            setter = "setEditStyle";
        }
        else if (state == "cursor")
        {
            setter = "setCursorStyle";
        }
        else if (state == "selected")
        {
            setter = "setSelectedStyle";
        }
        else if (state == "active")
        {
            setter = "setActiveStyle";
        }
        else if (state == "checked")
        {
            setter = "setCheckedStyle";
        }
        else if (state == "unchecked")
        {
            setter = "setUncheckedStyle";
        }
        else if (state == "disabled")
        {
            setter = "setDisabledStyle";
        }
        stateForSetter(setter)[stylePropName(key)] = value;
    }
    return states;
}

std::vector<std::pair<std::string, YamlMap>> mergeScrollviewDependencyStateStyles(
    const std::vector<std::pair<std::string, YamlMap>>& styleStates,
    const std::map<std::string, YamlMap>& dependencyStates
)
{
    std::vector<std::pair<std::string, YamlMap>> mergedStates;
    std::set<std::string> seen;
    for (const auto& [setter, stateStyle] : styleStates)
    {
        YamlMap merged = stateStyle;
        const auto it = dependencyStates.find(setter);
        if (it != dependencyStates.end())
        {
            for (const auto& [key, value] : it->second)
            {
                merged[key] = value;
            }
            seen.insert(setter);
        }
        mergedStates.push_back({setter, merged});
    }
    for (const auto& [setter, stateStyle] : dependencyStates)
    {
        if (seen.find(setter) == seen.end())
        {
            mergedStates.push_back({setter, stateStyle});
        }
    }
    return mergedStates;
}

std::vector<std::pair<std::string, YamlMap>> mergeScrollviewDependencyStyleStates(
    const std::vector<std::pair<std::string, YamlMap>>& styleStates,
    const YamlMap& panelStyle
)
{
    YamlMap dependencyStyle;
    for (const auto& [key, value] : panelStyle)
    {
        if (styleStateName(key).empty())
        {
            dependencyStyle[key] = value;
        }
    }
    if (dependencyStyle.empty())
    {
        return styleStates;
    }

    std::vector<std::pair<std::string, YamlMap>> mergedStates;
    bool hasBase = false;
    for (const auto& [setter, stateStyle] : styleStates)
    {
        if (setter != "setStyle")
        {
            mergedStates.push_back({setter, stateStyle});
            continue;
        }
        YamlMap merged = dependencyStyle;
        for (const auto& [key, value] : stateStyle)
        {
            merged[key] = value;
        }
        mergedStates.push_back({setter, merged});
        hasBase = true;
    }
    if (!hasBase)
    {
        mergedStates.insert(mergedStates.begin(), {"setStyle", dependencyStyle});
    }
    return mergedStates;
}

std::string scrollviewExtensionPanelCellName(const YamlMap& style)
{
    if (yamlGet(style, "@viewport") != nullptr || yamlGet(style, "viewport") != nullptr)
    {
        return "viewport";
    }
    return "panel";
}

YamlMap scrollviewExtensionPanelStyle(const YamlMap& style)
{
    for (const std::string& selector : {"@viewport", "viewport", "@panel", "panel"})
    {
        const YamlMap* panelStyle = valueAsMap(yamlGet(style, selector));
        if (panelStyle != nullptr && !panelStyle->empty())
        {
            return *panelStyle;
        }
    }
    return {};
}

int scrollviewExtensionGap(const YamlMap& style)
{
    YamlMap panelStyle = scrollviewExtensionPanelStyle(style);
    return valueAsInt(yamlGet(panelStyle, "gap"), 0);
}

std::vector<std::pair<std::string, YamlMap>> scrollviewExtensionStyleStates(const YamlMap& style)
{
    const std::string panelCellName = scrollviewExtensionPanelCellName(style);
    std::vector<std::pair<std::string, YamlMap>> states = styleStatesForElement(
        style,
        "uiscrollview",
        GENERATED_SCROLLVIEW_ELEMENT_NAME,
        panelCellName,
        nullptr
    );
    states = mergeScrollviewDependencyStyleStates(states, scrollviewExtensionPanelStyle(style));

    std::map<std::string, YamlMap> thisStateStyles;
    for (const auto& [setter, stateStyle] : styleStatesForElement(style, "this", "", "", nullptr))
    {
        if (setter != "setStyle" && !stateStyle.empty())
        {
            thisStateStyles[setter] = stateStyle;
        }
    }
    if (!thisStateStyles.empty())
    {
        states = mergeScrollviewDependencyStateStyles(states, thisStateStyles);
    }
    return states;
}

YamlMap scrollviewExtensionDescendantFocusStyle(const YamlMap& style)
{
    for (const auto& [setter, stateStyle] : styleStatesForElement(style, "this", "", "", nullptr))
    {
        if (setter == "setFocusStyle" && !stateStyle.empty())
        {
            return stateStyle;
        }
    }
    return {};
}

std::vector<int> styleIntValues(const YamlValue& value)
{
    std::vector<int> result;
    if (std::holds_alternative<YamlList>(value.value))
    {
        for (const YamlValue& item : std::get<YamlList>(value.value))
        {
            result.push_back(valueAsInt(&item, 0));
        }
        return result;
    }
    std::string text = valueAsString(value);
    std::replace(text.begin(), text.end(), ',', ' ');
    std::istringstream input(text);
    std::string token;
    while (input >> token)
    {
        try
        {
            result.push_back(std::stoi(token));
        }
        catch (const std::exception&)
        {
            return {};
        }
    }
    return result;
}

void assignBoxValues(std::map<std::string, std::string>& assignments, const std::string& prefix, const YamlValue& value)
{
    const std::vector<int> values = styleIntValues(value);
    if (values.size() == 1)
    {
        assignments[prefix] = std::to_string(values[0]);
    }
    else if (values.size() == 2)
    {
        assignments[prefix + "Top"] = std::to_string(values[0]);
        assignments[prefix + "Right"] = std::to_string(values[1]);
        assignments[prefix + "Bottom"] = std::to_string(values[0]);
        assignments[prefix + "Left"] = std::to_string(values[1]);
    }
    else if (values.size() >= 4)
    {
        assignments[prefix + "Top"] = std::to_string(values[0]);
        assignments[prefix + "Right"] = std::to_string(values[1]);
        assignments[prefix + "Bottom"] = std::to_string(values[2]);
        assignments[prefix + "Left"] = std::to_string(values[3]);
    }
}

std::string textGradientCode(const YamlMap& gradient)
{
    std::vector<std::string> colors;
    const YamlValue* colorsValue = yamlGet(gradient, "colors");
    if (colorsValue != nullptr && std::holds_alternative<std::string>(colorsValue->value))
    {
        colors.push_back(std::get<std::string>(colorsValue->value));
    }
    else
    {
        colors = listValue(colorsValue);
    }
    const int interval = valueAsInt(yamlGet(gradient, "interval"), 70);
    const int step = valueAsInt(yamlGet(gradient, "step"), 1);
    const int segmentSize = valueAsInt(yamlGet(gradient, "segment-size") != nullptr ? yamlGet(gradient, "segment-size") : yamlGet(gradient, "segment_size"), 1);

    std::string colorArgs;
    for (std::size_t index = 0; index < colors.size(); ++index)
    {
        if (index != 0)
        {
            colorArgs += ", ";
        }
        colorArgs += "Color(" + swiftString(colors[index]) + ")";
    }
    return "TextGradient(intervalMs: " + std::to_string(interval) +
        ", step: " + std::to_string(step) +
        ", segmentSize: " + std::to_string(segmentSize) +
        ", colors: [" + colorArgs + "])";
}

std::string styleCode(const YamlMap& style)
{
    std::map<std::string, std::string> assignments;
    const YamlMap* colorGradient = nullptr;
    const YamlMap* backgroundGradient = nullptr;
    const std::set<std::string> accepted{
        "color",
        "background",
        "background-texture",
        "background-texture-color",
        "scope-dim-background",
        "border-color",
        "border-width",
        "border-width-horizontal",
        "border-width-vertical",
        "padding",
        "margin",
        "gap",
        "text-align",
        "user-select",
        "scroll-x",
        "scroll-y",
    };
    for (const auto& [key, value] : style)
    {
        if (key == "text-color-gradient")
        {
            colorGradient = valueAsMap(&value);
            continue;
        }
        if (key == "text-background-gradient")
        {
            backgroundGradient = valueAsMap(&value);
            continue;
        }
        if (accepted.find(key) == accepted.end())
        {
            continue;
        }
        const std::string text = valueAsString(value);
        if (key == "color")
        {
            assignments["color"] = "Color(" + swiftString(text) + ")";
        }
        else if (key == "background")
        {
            assignments["background"] = "Color(" + swiftString(text) + ")";
        }
        else if (key == "background-texture")
        {
            assignments["backgroundTexture"] = swiftString(text);
        }
        else if (key == "background-texture-color")
        {
            assignments["backgroundTextureColor"] = "Color(" + swiftString(text) + ")";
        }
        else if (key == "scope-dim-background")
        {
            assignments["scopeDimBackground"] = "Color(" + swiftString(text) + ")";
        }
        else if (key == "border-color")
        {
            assignments["borderColor"] = "Color(" + swiftString(text) + ")";
        }
        else if (key == "border-width")
        {
            assignments["borderWidthHorizontal"] = text;
            assignments["borderWidthVertical"] = text;
        }
        else if (key == "border-width-horizontal")
        {
            assignments["borderWidthHorizontal"] = text;
        }
        else if (key == "border-width-vertical")
        {
            assignments["borderWidthVertical"] = text;
        }
        else if (key == "padding")
        {
            assignBoxValues(assignments, "padding", value);
        }
        else if (key == "margin")
        {
            assignBoxValues(assignments, "margin", value);
        }
        else if (key == "gap")
        {
            assignments["gap"] = text;
        }
        else if (key == "text-align")
        {
            assignments["textAlign"] = swiftString(text);
        }
        else if (key == "user-select")
        {
            assignments["userSelect"] = swiftString(text);
        }
        else if (key == "scroll-x")
        {
            assignments["scrollX"] = truthy(&value, false) ? "true" : "false";
        }
        else if (key == "scroll-y")
        {
            assignments["scrollY"] = truthy(&value, false) ? "true" : "false";
        }
    }
    if (colorGradient != nullptr)
    {
        assignments["textColorGradient"] = textGradientCode(*colorGradient);
    }
    if (backgroundGradient != nullptr)
    {
        assignments["textBackgroundGradient"] = textGradientCode(*backgroundGradient);
    }
    if (assignments.empty())
    {
        return "Style()";
    }
    std::string result = "{\n";
    result += "                    var style = Style()\n";
    for (const auto& [property, expression] : assignments)
    {
        result += "                    style." + property + " = " + expression + "\n";
    }
    result += "                    return style\n";
    result += "                }()";
    return result;
}

std::string cellNameForElement(const std::vector<NativeLayoutItem>& cells, const std::string& name)
{
    for (const NativeLayoutItem& cell : cells)
    {
        if (cell.content == name)
        {
            return cell.cellName;
        }
    }
    return {};
}

struct CompilerDocument
{
    NativeDocument document;
    YamlMap style;
    YamlMap mcpTools;
    std::string extends = DEFAULT_EXTENDS;
    std::string kind = "window";
    bool focusable = false;
    std::vector<std::string> dependencies;
};

CompilerDocument parseCompilerDocument(const std::filesystem::path& sourcePath)
{
    CompilerDocument result;
    result.document = parseDocumentFile(pathString(sourcePath));
    result.kind = lower(trim(yamlString(result.document.metadata, "kind", "window")));
    const bool isTheme = result.kind == "theme";
    result.extends = lower(trim(yamlString(result.document.definition, "extends", DEFAULT_EXTENDS)));
    result.focusable = truthy(yamlGet(result.document.definition, "focusable"), false);
    result.dependencies = dependenciesFromDefinition(result.document.definition);
    result.style = styleForDocument(result.document, sourcePath, isTheme);
    result.mcpTools = buildMcpTools(result.document.tools);
    return result;
}

bool isScrollviewExtension(const CompilerDocument& model)
{
    return lower(trim(model.extends)) == "uiscrollview";
}

std::vector<std::filesystem::path> dependencyPaths(const std::filesystem::path& mdPath, const CompilerDocument& model)
{
    std::vector<std::filesystem::path> paths;
    for (const std::string& sourceName : dependencyNames(model.document.members, model.dependencies))
    {
        const std::filesystem::path dependency = findDependencySourceFile(mdPath, sourceName);
        if (!dependency.empty())
        {
            paths.push_back(dependency);
        }
    }
    return paths;
}

std::string windowTitle(const NativeDocument& document, const std::string& baseName)
{
    return document.title.empty() ? baseName : document.title;
}

std::string generateHookDeclarations(const YamlMap& members)
{
    std::string result;
    for (const EventSpec& spec : eventSpecs(members))
    {
        if (spec.argType.empty())
        {
            result += "    open func " + spec.methodName + "() { }\n";
        }
        else
        {
            result += "    open func " + spec.methodName + "(_ value: " + spec.argType + ") { }\n";
        }
    }
    return result;
}

std::string generateDispatch(
    const std::string& methodName,
    const std::string& signature,
    const std::vector<EventSpec>& specs,
    const std::string& callSuffix
)
{
    std::vector<std::string> lines{
        "    open override func " + methodName + "(" + signature + ") -> Bool",
        "    {",
    };
    for (std::size_t index = 0; index < specs.size(); ++index)
    {
        const std::string keyword = index == 0 ? "if" : "else if";
        lines.push_back("        " + keyword + " name == " + swiftString(specs[index].name));
        lines.push_back("        {");
        lines.push_back("            " + specs[index].methodName + callSuffix);
        lines.push_back("            return true");
        lines.push_back("        }");
    }
    lines.push_back("        return false");
    lines.push_back("    }");
    lines.push_back("");

    std::string result;
    for (const std::string& line : lines)
    {
        result += line + "\n";
    }
    return result;
}

std::string mcpAppToolsCode(const YamlMap& mcpTools)
{
    std::vector<std::string> names;
    for (const auto& [name, value] : mcpTools)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    std::vector<std::string> entries;
    for (const std::string& name : names)
    {
        const YamlMap* tool = valueAsMap(yamlGet(mcpTools, name));
        if (tool == nullptr)
        {
            continue;
        }
        const YamlValue* inputSchema = yamlGet(*tool, "inputSchema");
        const YamlValue* outputSchema = yamlGet(*tool, "outputSchema");
        const std::string inputJson = inputSchema == nullptr
            ? "{\"type\":\"object\",\"properties\":{}}"
            : jsonCompact(*inputSchema);
        const std::string outputJson = outputSchema == nullptr ? "" : jsonCompact(*outputSchema);
        entries.push_back(
            "            GeneratedAppToolMetadata(name: " + swiftString(name) +
            ", description: " + swiftString(yamlString(*tool, "description", "")) +
            ", inputSchemaJson: " + swiftString(inputJson) +
            ", outputSchemaJson: " + swiftString(outputJson) + "),"
        );
    }
    if (entries.empty())
    {
        return "[]";
    }
    std::string result = "[\n";
    for (const std::string& entry : entries)
    {
        result += entry + "\n";
    }
    result += "        ]";
    return result;
}

std::string generateSource(const std::string& baseName, const std::string& classNameValue, const CompilerDocument& model, bool mcpEnabled)
{
    const NativeDocument& document = model.document;
    const YamlMap& members = document.members;
    const std::string title = windowTitle(document, baseName);
    const bool scrollviewExtension = isScrollviewExtension(model);
    const int scrollviewGap = scrollviewExtensionGap(model.style);
    const std::string baseClass = scrollviewExtension ? "GeneratedScrollViewBase" : "GeneratedWindowBase";
    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
        "import Foundation",
        "import Uimd",
        "",
        "open class " + classNameValue + ": " + baseClass,
        "{",
    };

    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        lines.push_back("    public var " + name + ": " + swiftElementType(typeFor(*member)) + "!");
    }

    lines.push_back("");
    lines.push_back("    public init()");
    lines.push_back("    {");
    lines.push_back("        super.init(" + swiftString(title) + ")");
    lines.push_back("        setGeneratedLayout(Self.buildLayout())");
    lines.push_back("        setGeneratedKind(" + swiftString(model.kind) + ")");
    lines.push_back("        setGeneratedFocusable(" + std::string{model.focusable ? "true" : "false"} + ")");
    lines.push_back("        setMcpMetadata(enabled: " + std::string{mcpEnabled ? "true" : "false"} +
        ", className: " + swiftString(classNameValue.substr(0, classNameValue.size() >= 2 ? classNameValue.size() - 2 : classNameValue.size())) +
        ", source: " + swiftString(mcpEnabled ? document.sourceText : std::string{}) +
        ", description: " + swiftString(yamlString(document.metadata, "description", "")) + ")");
    lines.push_back("        setMcpAppTools(" + mcpAppToolsCode(model.mcpTools) + ")");
    YamlMap windowStyle = styleWindow(model.style);
    if (!windowStyle.empty())
    {
        lines.push_back("        setGeneratedWindowStyle(" + styleCode(windowStyle) + ")");
    }
    for (const auto& [setter, stateStyle] : styleStatesForElement(model.style, "this", "", "", nullptr))
    {
        if (setter == "setFocusStyle" && !stateStyle.empty())
        {
            lines.push_back("        setGeneratedWindowFocusStyle(" + styleCode(stateStyle) + ")");
        }
    }
    if (scrollviewExtension)
    {
        lines.push_back("        setGeneratedScrollView(addElement(ScrollView(" + swiftString(GENERATED_SCROLLVIEW_ELEMENT_NAME) + ", " + std::to_string(scrollviewGap) + ")))");
        for (const auto& [setter, stateStyle] : scrollviewExtensionStyleStates(model.style))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        scrollView()." + setter + "(" + styleCode(stateStyle) + ")");
            }
        }
        YamlMap descendantFocusStyle = scrollviewExtensionDescendantFocusStyle(model.style);
        if (!descendantFocusStyle.empty())
        {
            lines.push_back("        scrollView().setDescendantFocusStyle(" + styleCode(descendantFocusStyle) + ")");
        }
    }

    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string elemType = typeFor(*member);
        lines.push_back("        " + name + " = addElement(" + swiftElementType(elemType) + "(" + ctorArgs(name, *member) + "))");
        const YamlValue* commitMode = yamlGet(*member, "commit-mode");
        if (commitMode == nullptr)
        {
            commitMode = yamlGet(*member, "commit_mode");
        }
        if (commitMode != nullptr)
        {
            lines.push_back("        " + name + ".commitMode = " + swiftString(valueAsString(*commitMode)));
        }
        if (elemType == "listbox" && truthy(yamlGet(*member, "multiple"), false))
        {
            lines.push_back("        " + name + ".multiple = true");
        }
        const std::string cellName = cellNameForElement(document.layout, name);
        std::string styleElemType;
        if (elemType == "textarea")
        {
            styleElemType = "textinput";
        }
        else if (BUILTIN_TYPES.find(elemType) == BUILTIN_TYPES.end() || elemType == "uiscrollview")
        {
            styleElemType = "uielement";
        }
        else
        {
            styleElemType = elemType;
        }
        for (const auto& [setter, stateStyle] : styleStatesForElement(model.style, styleElemType, name, cellName, member))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        " + name + "." + setter + "(" + styleCode(stateStyle) + ")");
            }
        }
    }
    lines.push_back("    }");
    lines.push_back("");

    lines.push_back("    private static func buildLayout() -> [GeneratedLayoutEntry]");
    lines.push_back("    {");
    lines.push_back("        return [");
    for (const NativeLayoutItem& cell : document.layout)
    {
        lines.push_back(layoutEntry(cell, members, model.style, scrollviewExtension));
    }
    lines.push_back("        ]");
    lines.push_back("    }");
    lines.push_back("");

    lines.push_back(generateHookDeclarations(members));

    std::vector<EventSpec> buttonSpecs;
    std::vector<EventSpec> textSpecs;
    std::vector<EventSpec> confirmedSpecs;
    std::vector<EventSpec> selectionTextSpecs;
    std::vector<EventSpec> selectionSpecs;
    for (const EventSpec& spec : eventSpecs(members))
    {
        if (spec.channel == "button")
        {
            buttonSpecs.push_back(spec);
        }
        else if (spec.channel == "text")
        {
            textSpecs.push_back(spec);
        }
        else if (spec.channel == "confirmed")
        {
            confirmedSpecs.push_back(spec);
        }
        else if (spec.channel == "selection_text")
        {
            selectionTextSpecs.push_back(spec);
        }
        else if (spec.channel == "selection")
        {
            selectionSpecs.push_back(spec);
        }
    }

    lines.push_back(generateDispatch("handleGeneratedButton", "_ name: String", buttonSpecs, "()"));
    lines.push_back(generateDispatch("handleGeneratedTextChanged", "_ name: String, value: String", textSpecs, "(value)"));
    lines.push_back(generateDispatch("handleGeneratedTextConfirmed", "_ name: String, value: String", confirmedSpecs, "(value)"));

    lines.push_back("    open override func handleGeneratedSelectionChanged(_ name: String, value: [String]) -> Bool");
    lines.push_back("    {");
    std::size_t branchIndex = 0;
    for (const EventSpec& spec : selectionTextSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("        " + keyword + " name == " + swiftString(spec.name));
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value.isEmpty ? \"\" : value[0])");
        lines.push_back("            return true");
        lines.push_back("        }");
        ++branchIndex;
    }
    for (const EventSpec& spec : selectionSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("        " + keyword + " name == " + swiftString(spec.name));
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value)");
        lines.push_back("            return true");
        lines.push_back("        }");
        ++branchIndex;
    }
    lines.push_back("        return false");
    lines.push_back("    }");
    lines.push_back("}");
    lines.push_back("");

    std::string result;
    for (std::size_t index = 0; index < lines.size(); ++index)
    {
        if (index != 0)
        {
            result += "\n";
        }
        result += lines[index];
    }
    return result;
}

std::string generateAppStub(const std::string& classNameValue)
{
    const std::string appName = appClassName(classNameValue);
    return "import Foundation\n"
        "import Uimd\n"
        "\n"
        "public final class " + appName + ": " + classNameValue + "\n"
        "{\n"
        "    // Override generated event hooks here.\n"
        "}\n"
        "\n"
        "@main\n"
        "enum Program\n"
        "{\n"
        "    static func main()\n"
        "    {\n"
        "        let code = GeneratedWindowRuntime.runGeneratedAppMain\n"
        "        {\n"
        "            let app = " + appName + "()\n"
        "            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)\n"
        "        }\n"
        "        Foundation.exit(Int32(code))\n"
        "    }\n"
        "}\n";
}

std::filesystem::path generatedSourcePath(const std::filesystem::path& sourcePath, const std::filesystem::path& outputDir)
{
    return outputDir / (sourcePath.stem().string() + "_ui.swift");
}

void writeTextFile(const std::filesystem::path& path, const std::string& content)
{
    std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    if (!output)
    {
        throw std::runtime_error("cannot write " + pathString(path));
    }
    output << content;
}

std::vector<std::filesystem::path> compileSwiftFile(
    const std::filesystem::path& sourcePath,
    const std::filesystem::path& outputDir,
    bool shouldGenerateAppStub,
    bool mcpEnabled,
    std::set<std::filesystem::path>& visited
)
{
    const std::filesystem::path absoluteSource = absolutePath(sourcePath);
    if (visited.find(absoluteSource) != visited.end())
    {
        return {};
    }
    visited.insert(absoluteSource);
    std::filesystem::create_directories(outputDir);

    CompilerDocument model = parseCompilerDocument(absoluteSource);
    std::vector<std::filesystem::path> generated;
    for (const std::filesystem::path& dependency : dependencyPaths(absoluteSource, model))
    {
        std::vector<std::filesystem::path> dependencyGenerated = compileSwiftFile(
            dependency,
            outputDir,
            false,
            mcpEnabled,
            visited
        );
        generated.insert(generated.end(), dependencyGenerated.begin(), dependencyGenerated.end());
    }

    const std::string baseName = absoluteSource.stem().string();
    const std::string classNameValue = className(baseName);
    const std::filesystem::path sourceOutputPath = generatedSourcePath(absoluteSource, outputDir);
    writeTextFile(sourceOutputPath, generateSource(baseName, classNameValue, model, mcpEnabled));
    generated.push_back(sourceOutputPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (baseName + ".swift");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, generateAppStub(classNameValue));
            generated.push_back(appPath);
        }
    }
    return generated;
}

}  // namespace

std::string swiftPackageManifest(
    const std::string& project,
    const std::string& localRuntimeReference,
    const std::string& sourceFileName,
    const std::vector<std::string>& targetSources,
    const std::vector<std::string>& targetExcludes
)
{
    const std::string excludedSource = sourceFileName.empty() ? project + ".uimd" : sourceFileName;
    std::vector<std::string> excludes = targetExcludes;
    if (std::find(excludes.begin(), excludes.end(), excludedSource) == excludes.end())
    {
        excludes.push_back(excludedSource);
    }
    std::sort(excludes.begin(), excludes.end());
    std::vector<std::string> sources = targetSources;
    if (sources.empty())
    {
        sources.push_back(project + ".swift");
        sources.push_back(project + "_ui.swift");
    }
    std::string sourcesCode;
    for (std::size_t index = 0; index < sources.size(); ++index)
    {
        if (index != 0)
        {
            sourcesCode += ", ";
        }
        sourcesCode += swiftString(sources[index]);
    }
    std::string excludesCode;
    for (std::size_t index = 0; index < excludes.size(); ++index)
    {
        if (index != 0)
        {
            excludesCode += ", ";
        }
        excludesCode += swiftString(excludes[index]);
    }
    return "// swift-tools-version: 6.0\n"
        "import Foundation\n"
        "import PackageDescription\n"
        "\n"
        "let localUimdPackagePath = " + swiftString(localRuntimeReference) + "\n"
        "\n"
        "func installedUimdPackagePath() -> String?\n"
        "{\n"
        "    let environment = ProcessInfo.processInfo.environment\n"
        "    let home = environment[\"UIMD_HOME\"] ?? environment[\"HOME\"].map { $0 + \"/.uimd\" }\n"
        "    guard let sdkHome = home else { return nil }\n"
        "    let currentFile = sdkHome + \"/current\"\n"
        "    guard let version = try? String(contentsOfFile: currentFile, encoding: .utf8).trimmingCharacters(in: .whitespacesAndNewlines), !version.isEmpty else { return nil }\n"
        "    let packagePath = sdkHome + \"/sdk/\" + version + \"/targets/swift\"\n"
        "    return FileManager.default.fileExists(atPath: packagePath + \"/Package.swift\") ? packagePath : nil\n"
        "}\n"
        "\n"
        "let uimdPackagePath = FileManager.default.fileExists(atPath: localUimdPackagePath + \"/Package.swift\")\n"
        "    ? localUimdPackagePath\n"
        "    : (installedUimdPackagePath() ?? localUimdPackagePath)\n"
        "\n"
        "let package = Package(\n"
        "    name: " + swiftString(project) + ",\n"
        "    platforms: [.macOS(.v13)],\n"
        "    products: [\n"
        "        .executable(name: " + swiftString(project) + ", targets: [" + swiftString(project) + "]),\n"
        "    ],\n"
        "    dependencies: [\n"
        "        .package(path: uimdPackagePath),\n"
        "    ],\n"
        "    targets: [\n"
        "        .executableTarget(\n"
        "            name: " + swiftString(project) + ",\n"
        "            dependencies: [.product(name: \"Uimd\", package: \"Uimd\")],\n"
        "            path: \".\",\n"
        "            exclude: [" + excludesCode + "],\n"
        "            sources: [" + sourcesCode + "]\n"
        "        ),\n"
        "    ]\n"
        ")\n";
}

std::vector<std::filesystem::path> generateSwiftSources(
    const std::filesystem::path& sourcePath,
    const NativeSwiftGenerateOptions& options
)
{
    std::vector<std::filesystem::path> generated;
    const std::vector<std::filesystem::path> files = sourceFiles(sourcePath);
    const bool autoBootstrap = files.size() == 1;
    for (const std::filesystem::path& sourceFile : files)
    {
        std::set<std::filesystem::path> visited;
        const std::filesystem::path outputDir = outputDirFor(sourceFile, options);
        const bool bootstrap = options.generateAppStub || autoBootstrap;
        std::vector<std::filesystem::path> result = compileSwiftFile(
            sourceFile,
            outputDir,
            bootstrap,
            options.mcpEnabled,
            visited
        );
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path packagePath = outputDir / "Package.swift";
            std::vector<std::string> targetSources{sourceFile.stem().string() + ".swift"};
            std::set<std::string> sourceNames{targetSources.front()};
            for (const std::filesystem::path& generatedPath : result)
            {
                if (generatedPath.parent_path() == outputDir && generatedPath.extension() == ".swift")
                {
                    const std::string fileName = generatedPath.filename().string();
                    if (fileName == "Package.swift")
                    {
                        continue;
                    }
                    if (sourceNames.insert(fileName).second)
                    {
                        targetSources.push_back(fileName);
                    }
                }
            }
            for (const auto& entry : std::filesystem::directory_iterator(outputDir))
            {
                if (!entry.is_regular_file() || entry.path().extension() != ".swift")
                {
                    continue;
                }
                const std::string fileName = entry.path().filename().string();
                if (fileName == "Package.swift")
                {
                    continue;
                }
                if (sourceNames.insert(fileName).second)
                {
                    targetSources.push_back(fileName);
                }
            }
            std::sort(targetSources.begin() + 1, targetSources.end());
            std::set<std::string> excludeNames{sourceFile.filename().string()};
            for (const auto& entry : std::filesystem::directory_iterator(outputDir))
            {
                const std::string fileName = entry.path().filename().string();
                if (fileName.empty() || fileName.front() == '.')
                {
                    continue;
                }
                if (fileName == "Package.swift" || fileName == "Package.resolved")
                {
                    continue;
                }
                if (entry.is_regular_file() && entry.path().extension() == ".swift")
                {
                    continue;
                }
                excludeNames.insert(fileName);
            }
            std::vector<std::string> targetExcludes(excludeNames.begin(), excludeNames.end());
            writeTextFile(packagePath, swiftPackageManifest(
                sourceFile.stem().string(),
                packageReferencePathFor(packagePath),
                sourceFile.filename().string(),
                targetSources,
                targetExcludes
            ));
            generated.push_back(packagePath);
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

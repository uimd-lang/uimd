#include "NativePythonGenerator.hpp"

#include "NativeModel.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
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
constexpr const char* DIMENSION_AUTO = "auto";

const std::set<std::string> BUILTIN_ELEMENT_TYPES{
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
    "uielement",
    "viewhost",
};

std::string readTextHead(const std::filesystem::path& path, std::size_t count)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        return {};
    }
    std::string result(count, '\0');
    input.read(result.data(), static_cast<std::streamsize>(result.size()));
    result.resize(static_cast<std::size_t>(input.gcount()));
    return result;
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

std::string pathString(const std::filesystem::path& path)
{
    return path.lexically_normal().string();
}

std::filesystem::path absolutePath(const std::filesystem::path& path)
{
    return std::filesystem::absolute(path).lexically_normal();
}

const YamlValue* yamlGet(const YamlMap& map, const std::string& key)
{
    return map.get(key);
}

YamlValue makeString(const std::string& value)
{
    return YamlValue{value};
}

YamlValue makeBool(bool value)
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

std::string yamlString(const YamlMap& map, const std::string& key, const std::string& fallback = "")
{
    const YamlValue* value = yamlGet(map, key);
    if (value == nullptr)
    {
        return fallback;
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        return std::get<std::string>(value->value);
    }
    if (std::holds_alternative<long long>(value->value))
    {
        return std::to_string(std::get<long long>(value->value));
    }
    if (std::holds_alternative<double>(value->value))
    {
        std::ostringstream output;
        output << std::setprecision(15) << std::get<double>(value->value);
        return output.str();
    }
    if (std::holds_alternative<bool>(value->value))
    {
        return std::get<bool>(value->value) ? "True" : "False";
    }
    return fallback;
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
        return std::get<bool>(value.value) ? "True" : "False";
    }
    return fallback;
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
    return true;
}

bool pythonTruthy(const YamlValue* value)
{
    if (value == nullptr || std::holds_alternative<std::nullptr_t>(value->value))
    {
        return false;
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
        return !std::get<std::string>(value->value).empty();
    }
    if (std::holds_alternative<YamlList>(value->value))
    {
        return !std::get<YamlList>(value->value).empty();
    }
    return !std::get<YamlMap>(value->value).empty();
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

std::string pythonStringRepr(const std::string& value)
{
    std::ostringstream output;
    output << "'";
    for (unsigned char ch : value)
    {
        if (ch == '\\')
        {
            output << "\\\\";
        }
        else if (ch == '\'')
        {
            output << "\\'";
        }
        else if (ch == '\n')
        {
            output << "\\n";
        }
        else if (ch == '\t')
        {
            output << "\\t";
        }
        else if (ch == '\r')
        {
            output << "\\r";
        }
        else if (ch < 0x20 || ch == 0x7F)
        {
            output << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch)
                   << std::dec << std::setfill(' ');
        }
        else
        {
            output << static_cast<char>(ch);
        }
    }
    output << "'";
    return output.str();
}

std::string pythonFloatRepr(double value)
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

std::string pythonRepr(const YamlValue& value);

std::string pythonListRepr(const YamlList& list)
{
    std::string result = "[";
    for (std::size_t index = 0; index < list.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += pythonRepr(list[index]);
    }
    result += "]";
    return result;
}

std::string pythonMapRepr(const YamlMap& map)
{
    std::string result = "{";
    bool first = true;
    for (const auto& [key, value] : map)
    {
        if (!first)
        {
            result += ", ";
        }
        first = false;
        result += pythonStringRepr(key);
        result += ": ";
        result += pythonRepr(value);
    }
    result += "}";
    return result;
}

std::string pythonRepr(const YamlValue& value)
{
    if (std::holds_alternative<std::nullptr_t>(value.value))
    {
        return "None";
    }
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? "True" : "False";
    }
    if (std::holds_alternative<long long>(value.value))
    {
        return std::to_string(std::get<long long>(value.value));
    }
    if (std::holds_alternative<double>(value.value))
    {
        return pythonFloatRepr(std::get<double>(value.value));
    }
    if (std::holds_alternative<std::string>(value.value))
    {
        return pythonStringRepr(std::get<std::string>(value.value));
    }
    if (std::holds_alternative<YamlList>(value.value))
    {
        return pythonListRepr(std::get<YamlList>(value.value));
    }
    return pythonMapRepr(std::get<YamlMap>(value.value));
}

std::string pythonLiteralString(const std::string& value)
{
    return pythonStringRepr(value);
}

std::string titleToClassName(std::string title)
{
    std::replace(title.begin(), title.end(), '-', ' ');
    std::replace(title.begin(), title.end(), '_', ' ');
    std::istringstream input(title);
    std::string word;
    std::string result;
    while (input >> word)
    {
        if (word.empty())
        {
            continue;
        }
        word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        for (std::size_t index = 1; index < word.size(); ++index)
        {
            word[index] = static_cast<char>(std::tolower(static_cast<unsigned char>(word[index])));
        }
        result += word;
    }
    return result + "UI";
}

std::string typeToClassName(std::string typeName)
{
    std::replace(typeName.begin(), typeName.end(), '-', ' ');
    std::replace(typeName.begin(), typeName.end(), '_', ' ');
    std::istringstream input(typeName);
    std::string word;
    std::string result;
    while (input >> word)
    {
        if (word.empty())
        {
            continue;
        }
        word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
        result += word;
    }
    return result;
}

std::string snakeIdentifier(const std::string& name)
{
    std::vector<std::string> parts;
    std::string current;
    for (unsigned char ch : name)
    {
        if (std::isalnum(ch))
        {
            current += static_cast<char>(std::tolower(ch));
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
    std::string identifier;
    for (std::size_t index = 0; index < parts.size(); ++index)
    {
        if (index != 0)
        {
            identifier += "_";
        }
        identifier += parts[index];
    }
    if (identifier.empty())
    {
        identifier = "element";
    }
    if (std::isdigit(static_cast<unsigned char>(identifier.front())))
    {
        identifier = "element_" + identifier;
    }
    return identifier;
}

std::string pythonEventMethodName(const std::string& name, const std::string& suffix)
{
    return "on_" + snakeIdentifier(name) + "_" + suffix;
}

std::string generatedAttrName(const std::string& name)
{
    return name == "title" ? "title_element" : name;
}

bool isBuiltinElementType(const std::string& elemType)
{
    return BUILTIN_ELEMENT_TYPES.find(elemType) != BUILTIN_ELEMENT_TYPES.end();
}

std::string memberType(const YamlMap& props, const std::string& fallback = "label")
{
    return lower(trim(yamlString(props, "type", fallback)));
}

std::string customSourceName(const YamlMap& props, const std::string& elemType)
{
    return yamlString(props, "source", elemType);
}

std::string imageRenderMode(const YamlMap& member)
{
    const YamlValue* renderMode = yamlGet(member, "render_mode");
    if (renderMode == nullptr)
    {
        renderMode = yamlGet(member, "render-mode");
    }
    return lower(trim(renderMode == nullptr ? std::string{"auto"} : valueAsString(*renderMode, "auto")));
}

bool memberRequiresSixel(const YamlMap& member)
{
    return memberType(member) == "image" && imageRenderMode(member) != "fallback";
}

bool membersRequireSixel(const YamlMap& members)
{
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* member = valueAsMap(&value);
        if (member != nullptr && memberRequiresSixel(*member))
        {
            return true;
        }
    }
    return false;
}

struct LayoutToken
{
    std::string raw;
    std::string prefix;
    std::string name;
    std::string text;
    bool quoted = false;
};

LayoutToken parseLayoutToken(const std::string& content)
{
    LayoutToken token;
    std::string stripped = trim(content);
    if (stripped.empty())
    {
        return token;
    }
    std::size_t index = 0;
    if (stripped[index] == '3' || stripped[index] == '0')
    {
        token.prefix = stripped.substr(index, 1);
        ++index;
    }
    if (index < stripped.size() && stripped[index] == '"')
    {
        std::size_t end = index + 1;
        while (end < stripped.size() && stripped[end] != '"')
        {
            ++end;
        }
        if (end >= stripped.size())
        {
            return {};
        }
        token.raw = stripped.substr(0, end + 1);
        token.text = stripped.substr(index + 1, end - index - 1);
        token.quoted = true;
        return token;
    }
    std::size_t end = index;
    while (end < stripped.size() && !std::isspace(static_cast<unsigned char>(stripped[end])))
    {
        ++end;
    }
    token.raw = stripped.substr(0, end);
    token.text = stripped.substr(index, end - index);
    std::size_t nameStart = 0;
    while (nameStart < token.text.size() && std::isdigit(static_cast<unsigned char>(token.text[nameStart])))
    {
        ++nameStart;
    }
    token.name = token.text.substr(nameStart);
    token.text = token.name;
    return token;
}

std::string elementTypeFromToken(const LayoutToken& token)
{
    if (token.quoted)
    {
        return "label";
    }
    if (token.prefix == "3")
    {
        return "textinput";
    }
    if (token.prefix == "0")
    {
        return "numberinput";
    }
    return "label";
}

bool hasMember(const YamlMap& members, const std::string& name)
{
    return members.find(name) != members.end();
}

const YamlMap* memberProps(const YamlMap& members, const std::string& name)
{
    return valueAsMap(yamlGet(members, name));
}

bool dimensionIsNone(int value, const std::string& mode = "")
{
    return value == GRID_EXPANDED || value == GRID_FIT_CONTENT || mode == "expanded" || mode == "fit-content";
}

std::string formatDictListValue(const std::string& value)
{
    return value;
}

std::string formatDictList(const std::vector<std::vector<std::pair<std::string, std::string>>>& items, int indent)
{
    if (items.empty())
    {
        return "[]";
    }
    const std::string prefix(static_cast<std::size_t>(indent), ' ');
    const std::string innerPrefix(static_cast<std::size_t>(indent + 4), ' ');
    std::string result = "[\n";
    for (std::size_t itemIndex = 0; itemIndex < items.size(); ++itemIndex)
    {
        if (itemIndex != 0)
        {
            result += ",\n";
        }
        result += prefix + "{\n";
        const auto& item = items[itemIndex];
        for (std::size_t keyIndex = 0; keyIndex < item.size(); ++keyIndex)
        {
            result += innerPrefix + pythonLiteralString(item[keyIndex].first) + ": " + formatDictListValue(item[keyIndex].second);
            if (keyIndex + 1 < item.size())
            {
                result += ",";
            }
            result += "\n";
        }
        result += prefix + "}";
    }
    result += "\n]";
    return result;
}

std::string formatLayout(const std::vector<NativeLayoutItem>& cells, const YamlMap& members)
{
    if (cells.empty())
    {
        return "[]";
    }

    std::vector<std::vector<std::pair<std::string, std::string>>> layoutItems;
    for (std::size_t index = 0; index < cells.size(); ++index)
    {
        const NativeLayoutItem& cell = cells[index];
        LayoutToken token = parseLayoutToken(cell.content);
        std::string elemType = elementTypeFromToken(token);
        std::string layoutName;
        if (!cell.content.empty())
        {
            if (!token.name.empty())
            {
                layoutName = token.name;
            }
            else if (token.quoted)
            {
                layoutName = "_label_" + std::to_string(index);
            }
            else
            {
                layoutName = "_cell_" + std::to_string(index);
            }
        }
        else
        {
            layoutName = "_cell_" + std::to_string(index);
        }
        const YamlMap* layoutMember = memberProps(members, layoutName);
        if (layoutMember != nullptr)
        {
            elemType = yamlString(*layoutMember, "type", elemType);
        }

        std::vector<std::pair<std::string, std::string>> item;
        item.push_back({"row", std::to_string(cell.row)});
        item.push_back({"col", std::to_string(cell.col)});
        if (cell.row != 0)
        {
            item.push_back({"relative_row", std::to_string(cell.row)});
        }
        if (cell.col != 0)
        {
            item.push_back({"relative_col", std::to_string(cell.col)});
        }
        item.push_back({"cell_row", std::to_string(cell.cellRow)});
        item.push_back({"cell_col", std::to_string(cell.cellCol)});
        item.push_back({"cell_chars_width", std::to_string(cell.cellCharsWidth)});
        item.push_back({"cell_chars_height", std::to_string(cell.cellCharsHeight)});
        if (!cell.cellName.empty())
        {
            item.push_back({"cell_name", pythonLiteralString(cell.cellName)});
        }
        if (!dimensionIsNone(cell.cellWidth, cell.cellWidthMode))
        {
            item.push_back({"cell_width", std::to_string(cell.cellWidth)});
        }
        if (!dimensionIsNone(cell.cellHeight, cell.cellHeightMode))
        {
            item.push_back({"cell_height", std::to_string(cell.cellHeight)});
        }
        item.push_back({"cell_width_mode", pythonLiteralString(cell.cellWidthMode)});
        item.push_back({"cell_height_mode", pythonLiteralString(cell.cellHeightMode)});
        if (cell.widthMode != DIMENSION_AUTO)
        {
            item.push_back({"width_mode", pythonLiteralString(cell.widthMode)});
        }
        if (cell.heightMode != DIMENSION_AUTO)
        {
            item.push_back({"height_mode", pythonLiteralString(cell.heightMode)});
        }
        item.push_back({"chars_width", std::to_string(cell.charsWidth)});
        item.push_back({"chars_height", std::to_string(cell.charsHeight)});
        if (cell.marginRight != 0)
        {
            item.push_back({"margin_right", std::to_string(cell.marginRight)});
        }
        if (cell.marginBottom != 0)
        {
            item.push_back({"margin_bottom", std::to_string(cell.marginBottom)});
        }
        if (!dimensionIsNone(cell.width, cell.widthMode))
        {
            item.push_back({"width", std::to_string(cell.width)});
        }
        if (!dimensionIsNone(cell.height, cell.heightMode))
        {
            item.push_back({"height", std::to_string(cell.height)});
        }

        item.push_back({"type", pythonLiteralString(elemType)});
        item.push_back({"name", pythonLiteralString(layoutName)});
        if (!cell.content.empty() && elementTypeFromToken(token) == "label" && token.quoted)
        {
            item.push_back({"text", pythonLiteralString(token.text)});
        }
        layoutItems.push_back(item);
    }
    return formatDictList(layoutItems, 4);
}

std::string formatMembersCode(const YamlMap& members)
{
    if (members.empty())
    {
        return "{}";
    }
    std::string result = "{\n";
    for (const auto& [name, value] : members)
    {
        const YamlMap* props = valueAsMap(&value);
        result += "    \"" + name + "\": {\n";
        if (props != nullptr)
        {
            for (const auto& [key, propValue] : *props)
            {
                result += "        \"" + key + "\": " + pythonRepr(propValue) + ",\n";
            }
        }
        result += "    },\n";
    }
    result += "}";
    return result;
}

std::string formatStyleCode(const YamlMap& style)
{
    if (style.empty())
    {
        return "{}";
    }
    std::string result = "{\n";
    for (const auto& [name, value] : style)
    {
        const YamlMap* props = valueAsMap(&value);
        result += "    \"" + name + "\": {\n";
        if (props != nullptr)
        {
            for (const auto& [key, propValue] : *props)
            {
                result += "        \"" + key + "\": " + pythonRepr(propValue) + ",\n";
            }
        }
        result += "    },\n";
    }
    result += "}";
    return result;
}

struct EventSpec
{
    std::string channel;
    std::string methodName;
    std::string argName;
};

std::vector<EventSpec> eventSpecsForMember(const std::string& name, const YamlMap& member)
{
    const std::string elemType = memberType(member);
    if (elemType == "button")
    {
        return {{"element", pythonEventMethodName(name, "click"), ""}};
    }
    if (elemType == "checkbox")
    {
        return {{"element", pythonEventMethodName(name, "change"), "value"}};
    }
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return {
            {"element", pythonEventMethodName(name, "change"), "value"},
            {"confirmed", pythonEventMethodName(name, "submit"), "value"},
        };
    }
    if (elemType == "combobox")
    {
        return {{"selection", pythonEventMethodName(name, "change"), "value"}};
    }
    if (elemType == "listbox")
    {
        return {{"selection", pythonEventMethodName(name, "selection_change"), "value"}};
    }
    return {};
}

std::string generateEventHookCode(const YamlMap& members)
{
    std::string result;
    std::set<std::string> seen;
    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        for (const EventSpec& spec : eventSpecsForMember(name, *member))
        {
            if (seen.find(spec.methodName) != seen.end())
            {
                continue;
            }
            seen.insert(spec.methodName);
            result += "    def " + spec.methodName + "(self";
            if (!spec.argName.empty())
            {
                result += ", " + spec.argName;
            }
            result += "):\n        pass\n\n";
        }
    }
    if (!result.empty())
    {
        result.pop_back();
    }
    return result;
}

std::string generateEventDispatchCode(const YamlMap& members)
{
    std::vector<std::pair<std::string, EventSpec>> elementSpecs;
    std::vector<std::pair<std::string, EventSpec>> confirmedSpecs;
    std::vector<std::pair<std::string, EventSpec>> selectionSpecs;
    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        for (const EventSpec& spec : eventSpecsForMember(name, *member))
        {
            if (spec.channel == "element")
            {
                elementSpecs.push_back({name, spec});
            }
            else if (spec.channel == "confirmed")
            {
                confirmedSpecs.push_back({name, spec});
            }
            else if (spec.channel == "selection")
            {
                selectionSpecs.push_back({name, spec});
            }
        }
    }

    auto appendSpecs = [](std::string& result, const std::vector<std::pair<std::string, EventSpec>>& specs) {
        if (specs.empty())
        {
            result += "        pass\n";
            return;
        }
        for (std::size_t index = 0; index < specs.size(); ++index)
        {
            const auto& [name, spec] = specs[index];
            result += std::string(index == 0 ? "        if" : "        elif") + " element is self." + generatedAttrName(name) + ":\n";
            result += "            self." + spec.methodName + "(";
            if (!spec.argName.empty())
            {
                result += spec.argName;
            }
            result += ")\n";
        }
    };

    std::string result = "    def _dispatch_element_changed(self, element, value):\n";
    appendSpecs(result, elementSpecs);
    result += "        super()._dispatch_element_changed(element, value)\n\n";
    result += "    def _dispatch_confirmed(self, element, value):\n";
    appendSpecs(result, confirmedSpecs);
    result += "        super()._dispatch_confirmed(element, value)\n\n";
    result += "    def _dispatch_selection_changed(self, element, value):\n";
    appendSpecs(result, selectionSpecs);
    result += "        super()._dispatch_selection_changed(element, value)\n";
    return result;
}

std::string generateAppToolHookCode(const YamlMap& mcpTools)
{
    std::vector<std::string> names;
    names.reserve(mcpTools.size());
    for (const auto& [name, value] : mcpTools)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    std::string result;
    for (const std::string& name : names)
    {
        result += "    def " + name + "(self, input=None):\n";
        result += "        raise NotImplementedError(" + pythonLiteralString("App tool not implemented: " + name) + ")\n\n";
    }
    if (!result.empty())
    {
        result.pop_back();
    }
    return result;
}

std::filesystem::path nearestPackageRoot(std::filesystem::path path)
{
    path = absolutePath(path);
    while (!path.empty() && path != path.parent_path())
    {
        if (path.filename() == "uimd" && std::filesystem::is_regular_file(path / "__init__.py"))
        {
            return path;
        }
        path = path.parent_path();
    }
    return {};
}

std::string packageImportModule(
    const std::string& sourceName,
    const std::filesystem::path& depDir,
    const std::filesystem::path& outputDir
)
{
    if (depDir.empty() || outputDir.empty())
    {
        return {};
    }
    const std::filesystem::path packageRoot = nearestPackageRoot(outputDir);
    if (packageRoot.empty())
    {
        return {};
    }
    const std::filesystem::path packageParent = packageRoot.parent_path();
    const std::filesystem::path moduleFile = absolutePath(depDir) / std::filesystem::path(sourceName).filename();
    std::filesystem::path rel = std::filesystem::relative(moduleFile, packageParent);
    if (rel.empty())
    {
        return {};
    }
    const std::string relString = rel.generic_string();
    if (relString.rfind("..", 0) == 0)
    {
        return {};
    }
    std::filesystem::path noExt = rel;
    noExt.replace_extension();
    std::string module = noExt.generic_string();
    std::replace(module.begin(), module.end(), '/', '.');
    return module;
}

std::string generateCustomImports(
    const YamlMap& members,
    const std::string& baseName,
    const std::vector<std::pair<std::string, std::filesystem::path>>& dependencyDirs,
    const std::filesystem::path& outputDir
)
{
    std::vector<std::string> pathLines;
    std::vector<std::string> importLines;
    std::set<std::string> seenPaths;
    std::set<std::pair<std::string, std::string>> seenMembers;

    auto depDirFor = [&dependencyDirs](const std::string& name) -> std::filesystem::path {
        for (const auto& [depName, depDir] : dependencyDirs)
        {
            if (depName == name)
            {
                return depDir;
            }
        }
        return {};
    };

    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* props = valueAsMap(&value);
        if (props == nullptr)
        {
            continue;
        }
        const std::string elemType = yamlString(*props, "type", "label");
        if (isBuiltinElementType(elemType))
        {
            continue;
        }
        const std::string sourceName = customSourceName(*props, elemType);
        if (sourceName == baseName)
        {
            continue;
        }
        const std::string className = yamlString(*props, "class", typeToClassName(sourceName));
        const auto key = std::make_pair(sourceName, className);
        if (seenMembers.find(key) != seenMembers.end())
        {
            continue;
        }
        seenMembers.insert(key);

        const std::filesystem::path depDir = depDirFor(sourceName);
        const std::string packageModule = packageImportModule(sourceName, depDir, outputDir);
        if (!packageModule.empty())
        {
            importLines.push_back("from " + packageModule + " import " + className);
            continue;
        }
        if (!depDir.empty() && absolutePath(depDir) != absolutePath(outputDir))
        {
            const std::filesystem::path rel = std::filesystem::relative(absolutePath(depDir), absolutePath(outputDir));
            const std::string pathLine = "sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), " +
                pythonLiteralString(rel.generic_string()) + "))";
            if (seenPaths.insert(pathLine).second)
            {
                pathLines.push_back(pathLine);
            }
        }
        importLines.push_back("from " + sourceName + " import " + className);
    }

    std::set<std::string> memberSources;
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* props = valueAsMap(&value);
        if (props != nullptr)
        {
            const std::string elemType = yamlString(*props, "type", "label");
            memberSources.insert(customSourceName(*props, elemType));
        }
    }
    for (const auto& [depName, depDir] : dependencyDirs)
    {
        if (memberSources.find(depName) != memberSources.end())
        {
            continue;
        }
        if (!depDir.empty() && absolutePath(depDir) != absolutePath(outputDir))
        {
            const std::filesystem::path rel = std::filesystem::relative(absolutePath(depDir), absolutePath(outputDir));
            const std::string pathLine = "sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), " +
                pythonLiteralString(rel.generic_string()) + "))";
            if (seenPaths.insert(pathLine).second)
            {
                pathLines.push_back(pathLine);
            }
        }
    }

    if (pathLines.empty() && importLines.empty())
    {
        return {};
    }
    std::vector<std::string> lines;
    if (!pathLines.empty())
    {
        lines.push_back("import sys");
        lines.push_back("import os");
        lines.insert(lines.end(), pathLines.begin(), pathLines.end());
    }
    lines.insert(lines.end(), importLines.begin(), importLines.end());

    std::string result = "\n";
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

std::vector<std::string> sortedMemberNames(const YamlMap& members)
{
    std::vector<std::string> names;
    names.reserve(members.size());
    for (const auto& [name, value] : members)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::string propValueForArgument(const YamlMap& props, const std::string& key, const std::string& fallback = "")
{
    const YamlValue* value = yamlGet(props, key);
    if (value == nullptr)
    {
        return fallback;
    }
    return pythonRepr(*value);
}

std::string generateInitCode(const YamlMap& members, const std::vector<NativeLayoutItem>& cells)
{
    std::vector<std::pair<std::string, const NativeLayoutItem*>> cellMap;
    for (const NativeLayoutItem& cell : cells)
    {
        LayoutToken token = parseLayoutToken(cell.content);
        if (!token.name.empty())
        {
            cellMap.push_back({token.name, &cell});
        }
    }
    auto findCell = [&cellMap](const std::string& name) -> const NativeLayoutItem* {
        for (const auto& [cellName, cell] : cellMap)
        {
            if (cellName == name)
            {
                return cell;
            }
        }
        return nullptr;
    };

    std::vector<std::string> lines;
    for (const std::string& name : sortedMemberNames(members))
    {
        const YamlMap* props = memberProps(members, name);
        if (props == nullptr)
        {
            continue;
        }
        const std::string elemType = yamlString(*props, "type", "label");
        std::string runtimeType = elemType;
        std::vector<std::string> args;

        if (elemType == "label" || elemType == "spanlabel" || elemType == "infolabel" || elemType == "messagetable")
        {
            std::string text = yamlString(*props, "text", "");
            const bool hasExplicitText = yamlGet(*props, "text") != nullptr;
            if (!hasExplicitText && text.empty())
            {
                const NativeLayoutItem* cell = findCell(name);
                if (cell != nullptr && !cell->content.empty())
                {
                    text = parseLayoutToken(cell->content).text;
                }
            }
            if (!hasExplicitText && text.empty())
            {
                std::string titled = name;
                bool capitalizeNext = true;
                for (char& ch : titled)
                {
                    if (ch == '_' || ch == '-')
                    {
                        ch = ' ';
                        capitalizeNext = true;
                    }
                    else if (capitalizeNext)
                    {
                        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
                        capitalizeNext = false;
                    }
                }
                text = titled + " area";
            }
            args.push_back("text=" + pythonLiteralString(text));
            if (yamlGet(*props, "color") != nullptr)
            {
                args.push_back("color=" + propValueForArgument(*props, "color"));
            }
            if (yamlGet(*props, "background") != nullptr)
            {
                args.push_back("background=" + propValueForArgument(*props, "background"));
            }
        }
        else if (elemType == "framebufferview")
        {
        }
        else if (elemType == "image")
        {
            args.push_back("source=" + propValueForArgument(*props, "source", "''"));
            if (yamlGet(*props, "alt") != nullptr)
            {
                args.push_back("alt=" + propValueForArgument(*props, "alt"));
            }
            if (yamlGet(*props, "fit") != nullptr)
            {
                args.push_back("fit=" + propValueForArgument(*props, "fit"));
            }
            if (yamlGet(*props, "render_mode") != nullptr)
            {
                args.push_back("render_mode=" + propValueForArgument(*props, "render_mode"));
            }
            else if (yamlGet(*props, "render-mode") != nullptr)
            {
                args.push_back("render_mode=" + propValueForArgument(*props, "render-mode"));
            }
            if (yamlGet(*props, "align") != nullptr)
            {
                args.push_back("align=" + propValueForArgument(*props, "align"));
            }
            if (yamlGet(*props, "valign") != nullptr)
            {
                args.push_back("valign=" + propValueForArgument(*props, "valign"));
            }
            else if (yamlGet(*props, "vertical_align") != nullptr)
            {
                args.push_back("valign=" + propValueForArgument(*props, "vertical_align"));
            }
            else if (yamlGet(*props, "vertical-align") != nullptr)
            {
                args.push_back("valign=" + propValueForArgument(*props, "vertical-align"));
            }
        }
        else if (elemType == "textinput" || elemType == "textarea")
        {
            args.push_back("value=" + propValueForArgument(*props, "value", "''"));
            if (yamlGet(*props, "maxlength") != nullptr)
            {
                args.push_back("maxlength=" + propValueForArgument(*props, "maxlength"));
            }
            if (yamlGet(*props, "format") != nullptr)
            {
                args.push_back("format_regex=" + propValueForArgument(*props, "format"));
            }
        }
        else if (elemType == "numberinput")
        {
            args.push_back("value=" + propValueForArgument(*props, "value", "0"));
            if (yamlGet(*props, "step_size") != nullptr)
            {
                args.push_back("step_size=" + propValueForArgument(*props, "step_size"));
            }
            if (yamlGet(*props, "min_value") != nullptr)
            {
                args.push_back("min_value=" + propValueForArgument(*props, "min_value"));
            }
            if (yamlGet(*props, "max_value") != nullptr)
            {
                args.push_back("max_value=" + propValueForArgument(*props, "max_value"));
            }
        }
        else if (elemType == "uielement")
        {
            std::string text = yamlString(*props, "text", "");
            if (text.empty())
            {
                const NativeLayoutItem* cell = findCell(name);
                if (cell != nullptr && !cell->content.empty())
                {
                    const std::string tokenText = parseLayoutToken(cell->content).text;
                    if (!tokenText.empty())
                    {
                        text = typeToClassName(tokenText) + " area";
                    }
                }
            }
            args.push_back("text=" + pythonLiteralString(text));
        }
        else if (elemType == "viewhost")
        {
            const std::string text = yamlString(*props, "text", "");
            if (!text.empty())
            {
                args.push_back("text=" + pythonLiteralString(text));
            }
        }
        else if (elemType == "button")
        {
            args.push_back("title=" + pythonLiteralString(yamlString(*props, "title", name)));
        }
        else if (elemType == "checkbox")
        {
            args.push_back("value=" + propValueForArgument(*props, "value", "False"));
            const std::string title = yamlString(*props, "title", "");
            if (!title.empty())
            {
                args.push_back("title=" + pythonLiteralString(title));
            }
        }
        else if (elemType == "combobox")
        {
            if (yamlGet(*props, "options") != nullptr)
            {
                const YamlList* options = valueAsList(yamlGet(*props, "options"));
                if (options != nullptr && !options->empty())
                {
                    args.push_back("options=" + pythonListRepr(*options));
                }
            }
            const std::string selectedItem = yamlString(*props, "selected_item", "");
            if (!selectedItem.empty())
            {
                args.push_back("selected_item=" + pythonLiteralString(selectedItem));
            }
        }
        else if (elemType == "listbox")
        {
            const YamlList* options = valueAsList(yamlGet(*props, "options"));
            if (options != nullptr && !options->empty())
            {
                args.push_back("options=" + pythonListRepr(*options));
            }
            const YamlList* selectedItems = valueAsList(yamlGet(*props, "selected_items"));
            if (selectedItems != nullptr && !selectedItems->empty())
            {
                args.push_back("selected_items=" + pythonListRepr(*selectedItems));
            }
            const YamlValue* multiple = yamlGet(*props, "multiple");
            if (pythonTruthy(multiple))
            {
                args.push_back(std::string{"multiple="} + (truthy(multiple, false) ? "True" : "False"));
            }
        }
        else if (!isBuiltinElementType(elemType))
        {
            runtimeType = "uielement";
            const std::string sourceName = customSourceName(*props, elemType);
            const std::string className = yamlString(*props, "class", typeToClassName(sourceName));
            args.push_back("layout_class=" + className);
            const std::string text = yamlString(*props, "text", "");
            if (!text.empty())
            {
                args.push_back("text=" + pythonLiteralString(text));
            }
        }

        std::string joinedArgs;
        for (std::size_t index = 0; index < args.size(); ++index)
        {
            if (index != 0)
            {
                joinedArgs += ", ";
            }
            joinedArgs += args[index];
        }
        lines.push_back("        self." + generatedAttrName(name) + " = self.create_element(\"" + name + "\", \"" + runtimeType + "\", " + joinedArgs + ")");
        const YamlValue* commitMode = yamlGet(*props, "commit-mode");
        if (commitMode == nullptr)
        {
            commitMode = yamlGet(*props, "commit_mode");
        }
        if (commitMode != nullptr)
        {
            lines.push_back("        self." + generatedAttrName(name) + ".commit_mode = " + pythonLiteralString(valueAsString(*commitMode)));
        }
    }

    for (std::size_t index = 0; index < cells.size(); ++index)
    {
        const NativeLayoutItem& cell = cells[index];
        LayoutToken token = parseLayoutToken(cell.content);
        if (!token.name.empty() && hasMember(members, token.name))
        {
            continue;
        }
        const std::string elemType = elementTypeFromToken(token);
        std::vector<std::string> args;
        const std::string text = token.quoted ? token.text : token.name;
        if (elemType == "label" || elemType == "spanlabel" || elemType == "infolabel" || elemType == "messagetable")
        {
            args.push_back("text=" + pythonLiteralString(text));
        }
        else if (elemType == "textinput")
        {
            args.push_back("value=''");
        }
        else if (elemType == "numberinput")
        {
            args.push_back("value=0");
        }

        std::string elemName;
        if (!token.name.empty())
        {
            elemName = token.name;
        }
        else if (token.quoted)
        {
            elemName = "_label_" + std::to_string(index);
        }
        else
        {
            elemName = "_cell_" + std::to_string(index);
        }

        std::string joinedArgs;
        for (std::size_t argIndex = 0; argIndex < args.size(); ++argIndex)
        {
            if (argIndex != 0)
            {
                joinedArgs += ", ";
            }
            joinedArgs += args[argIndex];
        }
        lines.push_back("        self." + generatedAttrName(elemName) + " = self.create_element(\"" + elemName + "\", \"" + elemType + "\", " + joinedArgs + ")");
    }

    if (lines.empty())
    {
        return "        pass";
    }
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

bool isLegacyUimdFile(const std::filesystem::path& path)
{
    const std::string head = readTextHead(path, 4096);
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
        const std::string extension = path.extension().string();
        if (extension == ".uimd" || (extension == ".md" && isLegacyUimdFile(path)))
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

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeGenerateOptions& options)
{
    if (options.hasOutputDir)
    {
        return absolutePath(options.outputDir);
    }
    return absolutePath(sourceFile).parent_path();
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
        if (std::filesystem::is_regular_file(current / "src" / "uimd" / "cli" / "main.py") ||
            std::filesystem::is_regular_file(current / "shared" / "themes" / "dark.uimd"))
        {
            return current;
        }
        current = current.parent_path();
    }
    return {};
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
            flattened[prefix];
            if (!std::holds_alternative<YamlMap>(flattened[prefix].value))
            {
                flattened[prefix] = makeMap(YamlMap{});
            }
            std::get<YamlMap>(flattened[prefix].value)[key] = value;
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

std::string roleForType(const std::string& typeName)
{
    const std::string elemType = lower(typeName);
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return "input";
    }
    if (elemType == "button")
    {
        return "action";
    }
    if (elemType == "checkbox")
    {
        return "toggle";
    }
    if (elemType == "combobox" || elemType == "listbox")
    {
        return "selection";
    }
    return "text";
}

std::string descriptionForMember(const std::string& name, const YamlMap& props)
{
    for (const std::string& key : {"description", "title", "text"})
    {
        const YamlValue* value = yamlGet(props, key);
        if (value != nullptr)
        {
            const std::string text = valueAsString(*value);
            if (!text.empty())
            {
                return text;
            }
        }
    }
    return name;
}

YamlMap buildMcpMetadata(const YamlMap& metadata, const YamlMap& members)
{
    YamlMap window;
    window["description"] = makeString(yamlString(metadata, "description", ""));

    YamlMap elements;
    for (const auto& [name, value] : members)
    {
        const YamlMap* props = valueAsMap(&value);
        if (props == nullptr)
        {
            continue;
        }
        YamlMap element;
        element["role"] = makeString(roleForType(yamlString(*props, "type", "")));
        element["description"] = makeString(descriptionForMember(name, *props));
        element["expose"] = makeBool(truthy(yamlGet(*props, "expose"), true));
        elements[name] = makeMap(element);
    }

    YamlMap result;
    result["window"] = makeMap(window);
    result["elements"] = makeMap(elements);
    return result;
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

std::vector<std::string> customDependencyNames(const YamlMap& members, const std::vector<std::string>& explicitDependencies)
{
    std::vector<std::string> dependencies;
    std::set<std::string> seen;
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* props = valueAsMap(&value);
        if (props == nullptr)
        {
            continue;
        }
        const std::string elemType = yamlString(*props, "type", "label");
        if (isBuiltinElementType(elemType))
        {
            continue;
        }
        const std::string sourceName = customSourceName(*props, elemType);
        if (seen.insert(sourceName).second)
        {
            dependencies.push_back(sourceName);
        }
    }
    for (const std::string& dependency : explicitDependencies)
    {
        const std::string sourceName = std::filesystem::path(dependency).replace_extension().string();
        if (seen.insert(sourceName).second)
        {
            dependencies.push_back(sourceName);
        }
    }
    return dependencies;
}

std::filesystem::path resolveDependencyPath(const std::filesystem::path& parentPath, const std::string& dependencyName)
{
    const std::filesystem::path dependency{dependencyName};
    std::filesystem::path depStemPath = dependency;
    const std::string depStem = depStemPath.replace_extension().string();

    if (dependency.is_absolute())
    {
        return std::filesystem::is_regular_file(dependency) ? absolutePath(dependency) : std::filesystem::path{};
    }

    const std::filesystem::path parentDir = absolutePath(parentPath).parent_path();
    const std::filesystem::path grandparentDir = parentDir.parent_path();
    std::vector<std::filesystem::path> dependencyNames;
    if (endsWith(dependencyName, ".uimd") || endsWith(dependencyName, ".md"))
    {
        dependencyNames.push_back(dependencyName);
    }
    else
    {
        dependencyNames.push_back(dependencyName + ".uimd");
        dependencyNames.push_back(dependencyName + ".md");
    }

    std::vector<std::filesystem::path> candidates;
    for (const std::filesystem::path& dependencyFile : dependencyNames)
    {
        candidates.push_back(parentDir / dependencyFile);
        candidates.push_back(parentDir / depStem / dependencyFile);
        candidates.push_back(grandparentDir / dependencyFile);
        candidates.push_back(grandparentDir / depStem / dependencyFile);
    }
    for (const std::filesystem::path& candidate : candidates)
    {
        if (std::filesystem::is_regular_file(candidate))
        {
            return absolutePath(candidate);
        }
    }

    std::vector<std::filesystem::path> matches;
    for (const std::filesystem::path& dependencyFile : dependencyNames)
    {
        if (std::filesystem::is_directory(parentDir))
        {
            for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(parentDir))
            {
                if (!entry.is_regular_file())
                {
                    continue;
                }
                const std::filesystem::path path = entry.path();
                if (path.filename() == dependencyFile.filename() && path.parent_path().filename() == std::filesystem::path(depStem).filename())
                {
                    matches.push_back(path);
                }
            }
            std::sort(matches.begin(), matches.end());
            if (!matches.empty())
            {
                return absolutePath(matches.front());
            }
            for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(parentDir))
            {
                if (entry.is_regular_file() && entry.path().filename() == dependencyFile.filename())
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
    }
    return {};
}

struct CompilerDocument
{
    NativeDocument document;
    YamlMap style;
    YamlMap mcpMetadata;
    YamlMap mcpTools;
    std::string extends = DEFAULT_EXTENDS;
    std::string direction;
    bool focusable = false;
    std::string kind = "window";
    std::vector<std::string> dependencies;
};

CompilerDocument parseCompilerDocument(const std::filesystem::path& sourcePath)
{
    CompilerDocument result;
    result.document = parseDocumentFile(pathString(sourcePath));
    result.kind = lower(trim(yamlString(result.document.metadata, "kind", "window")));
    const bool isTheme = result.kind == "theme";
    result.extends = lower(trim(yamlString(result.document.definition, "extends", DEFAULT_EXTENDS)));
    const std::string direction = yamlString(result.document.definition, "direction", "");
    result.direction = direction.empty() ? "" : lower(trim(direction));
    result.focusable = truthy(yamlGet(result.document.definition, "focusable"), false);
    result.dependencies = dependenciesFromDefinition(result.document.definition);
    result.style = styleForDocument(result.document, sourcePath, isTheme);
    result.mcpMetadata = buildMcpMetadata(result.document.metadata, result.document.members);
    result.mcpTools = buildMcpTools(result.document.tools);
    return result;
}

std::string generatePythonFileContent(
    const CompilerDocument& model,
    const std::filesystem::path& outputPath,
    const std::vector<std::pair<std::string, std::filesystem::path>>& dependencyDirs,
    bool mcpEnabled
)
{
    std::string baseName = outputPath.stem().string();
    if (endsWith(baseName, "_ui"))
    {
        baseName = baseName.substr(0, baseName.size() - 3);
    }
    const std::string className = titleToClassName(baseName);

    std::string baseClass = "UIWindow";
    if (model.extends == "uielement")
    {
        baseClass = "UIElement";
    }
    else if (model.extends == "uicontrol")
    {
        baseClass = "UIControl";
    }
    else if (model.extends == "uiscrollview")
    {
        baseClass = "UIScrollView";
    }
    else if (model.extends == "uipopup")
    {
        baseClass = "UIPopup";
    }

    const bool requiresSixel = membersRequireSixel(model.document.members);
    const std::string sixelImport = requiresSixel ? "from uimd.runtime.image import require_sixel_for_image_rendering\n" : "";
    const std::string sixelStartupCheck = requiresSixel ? "        require_sixel_for_image_rendering()\n" : "";
    const std::string customImports = generateCustomImports(model.document.members, baseName, dependencyDirs, outputPath.parent_path());

    std::string directionLine;
    const std::set<std::string> validDirections{"auto", "up", "down", "left", "right"};
    if (model.extends == "uipopup" && validDirections.find(model.direction) != validDirections.end())
    {
        directionLine = "    _preferred_direction = \"" + model.direction + "\"\n\n";
    }

    std::ostringstream output;
    output << "\"\"\"Auto-generated UI code for " << outputPath.filename().string() << " - DO NOT EDIT MANUALLY.\"\"\"\n\n";
    output << "from uimd.runtime import " << baseClass << "\n";
    output << sixelImport << customImports << "\n\n\n";
    output << "class " << className << "(" << baseClass << "):\n";
    output << "    \"\"\"Generated UI window from markdown definition.\"\"\"\n\n";
    output << "    _mcp_enabled = " << (mcpEnabled ? "True" : "False") << "\n";
    output << "    _mcp_source_md = " << pythonLiteralString(mcpEnabled ? model.document.sourceText : "") << "\n";
    output << "    _mcp_agent_metadata = " << pythonMapRepr(model.mcpMetadata) << "\n";
    output << "    _mcp_app_tools = " << pythonMapRepr(model.mcpTools) << "\n\n";
    output << directionLine;
    output << "    def __init__(self):\n";
    output << "        super().__init__(\n";
    output << "            title=\"" << model.document.title << "\",\n";
    output << "            layout=COMPILED_LAYOUT,\n";
    output << "            members=COMPILED_MEMBERS,\n";
    output << "            style=COMPILED_STYLE\n";
    output << "        )\n";
    output << sixelStartupCheck;
    output << "        self.focusable = " << (model.focusable ? "True" : "False") << "\n";
    output << "        self._metadata_kind = " << pythonLiteralString(lower(trim(model.kind.empty() ? std::string{"window"} : model.kind))) << "\n";
    output << "        if self._metadata_kind == \"dialog\":\n";
    output << "            self.mode = \"dialog\"\n\n";
    output << generateInitCode(model.document.members, model.document.layout) << "\n";

    const std::string eventHookCode = generateEventHookCode(model.document.members);
    const std::string appToolHookCode = generateAppToolHookCode(model.mcpTools);
    const std::string eventDispatchCode = generateEventDispatchCode(model.document.members);
    output << eventHookCode << "\n";
    output << appToolHookCode << "\n";
    output << eventDispatchCode << "\n\n";
    output << "COMPILED_MEMBERS = " << formatMembersCode(model.document.members) << "\n\n";
    output << "COMPILED_STYLE = " << formatStyleCode(model.style) << "\n\n";
    output << "COMPILED_LAYOUT = " << formatLayout(model.document.layout, model.document.members) << "\n";
    return output.str();
}

std::filesystem::path generatedPathFor(const std::filesystem::path& sourcePath, const std::filesystem::path& outputDir)
{
    return outputDir / (sourcePath.stem().string() + "_ui.py");
}

std::vector<std::filesystem::path> compilePythonFile(
    const std::filesystem::path& sourcePath,
    const std::filesystem::path& outputDir,
    bool compileDependencies,
    std::set<std::filesystem::path>& visited,
    bool mcpEnabled
)
{
    const std::filesystem::path absoluteSource = absolutePath(sourcePath);
    if (visited.find(absoluteSource) != visited.end())
    {
        if (compileDependencies)
        {
            return {};
        }
        return {generatedPathFor(absoluteSource, outputDir)};
    }
    visited.insert(absoluteSource);

    CompilerDocument model = parseCompilerDocument(absoluteSource);
    const bool inPlace = absolutePath(outputDir) == absoluteSource.parent_path();

    std::vector<std::pair<std::string, std::filesystem::path>> dependencyDirs;
    for (const std::string& dependencyName : customDependencyNames(model.document.members, model.dependencies))
    {
        const std::filesystem::path dependencyPath = resolveDependencyPath(absoluteSource, dependencyName);
        if (dependencyPath.empty())
        {
            throw std::runtime_error("Custom UI dependency '" + dependencyName + "' not found for " + pathString(absoluteSource));
        }
        const std::filesystem::path depOutputDir = inPlace ? dependencyPath.parent_path() : outputDir;
        dependencyDirs.push_back({dependencyName, depOutputDir});
    }

    std::vector<std::filesystem::path> generated;
    if (compileDependencies)
    {
        for (const auto& [dependencyName, depOutputDir] : dependencyDirs)
        {
            const std::filesystem::path dependencyPath = resolveDependencyPath(absoluteSource, dependencyName);
            std::vector<std::filesystem::path> dependencyGenerated = compilePythonFile(
                dependencyPath,
                depOutputDir,
                true,
                visited,
                mcpEnabled
            );
            generated.insert(generated.end(), dependencyGenerated.begin(), dependencyGenerated.end());
        }
    }

    const std::filesystem::path outputPath = generatedPathFor(absoluteSource, outputDir);
    std::filesystem::create_directories(outputPath.parent_path());
    const std::string content = generatePythonFileContent(model, outputPath, dependencyDirs, mcpEnabled);
    std::ofstream output(outputPath, std::ios::binary);
    if (!output)
    {
        throw std::runtime_error("cannot write " + pathString(outputPath));
    }
    output << content;

    if (compileDependencies)
    {
        generated.push_back(outputPath);
        return generated;
    }
    return {outputPath};
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

}  // namespace

std::vector<std::filesystem::path> generatePythonSources(
    const std::filesystem::path& sourcePath,
    const NativeGenerateOptions& options
)
{
    std::vector<std::filesystem::path> generated;
    for (const std::filesystem::path& sourceFile : sourceFiles(sourcePath))
    {
        std::set<std::filesystem::path> visited;
        const std::filesystem::path outputDir = outputDirFor(sourceFile, options);
        std::vector<std::filesystem::path> result = compilePythonFile(
            sourceFile,
            outputDir,
            options.compileDependencies,
            visited,
            options.mcpEnabled
        );
        generated.insert(generated.end(), result.begin(), result.end());
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

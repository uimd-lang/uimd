#include "NativeCppGenerator.hpp"

#include "NativeModel.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#ifndef UIMD_NATIVE_SOURCE_ROOT
#define UIMD_NATIVE_SOURCE_ROOT ""
#endif

#ifndef UIMD_VERSION
#define UIMD_VERSION "0.0.0"
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
    if (value == nullptr)
    {
        return fallback;
    }
    return valueAsString(*value, fallback);
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

std::string cppString(const std::string& value)
{
    std::string result = "\"";
    for (char ch : value)
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
        else
        {
            result += ch;
        }
    }
    result += "\"";
    return result;
}

std::string cppStringValue(const YamlValue* value, const std::string& fallback = "")
{
    return cppString(value == nullptr ? fallback : valueAsString(*value, fallback));
}

std::string cppRawString(const std::string& value)
{
    std::string delimiter = "UI_MCP_MD";
    while (value.find(")" + delimiter + "\"") != std::string::npos)
    {
        delimiter += "_";
    }
    return "R\"" + delimiter + "(" + value + ")" + delimiter + "\"";
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

std::string mcpClassName(std::string baseName)
{
    std::string result = className(std::move(baseName));
    if (endsWith(result, "UI"))
    {
        result.resize(result.size() - 2);
    }
    return result;
}

std::string typeFor(const YamlMap& member)
{
    return lower(yamlString(member, "type", "label"));
}

std::string descriptionForMember(const std::string& name, const YamlMap& member)
{
    for (const std::string& key : {"description", "title", "text"})
    {
        const YamlValue* value = yamlGet(member, key);
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

bool isBuiltinType(const std::string& elemType, const std::set<std::string>& scrollviewTypes = {})
{
    (void)scrollviewTypes;
    return BUILTIN_TYPES.find(elemType) != BUILTIN_TYPES.end();
}

std::string customSourceName(const YamlMap& member, const std::string& elemType)
{
    return yamlString(member, "source", elemType);
}

std::string customClassName(const YamlMap& member, const std::string& elemType)
{
    return yamlString(member, "class", className(customSourceName(member, elemType)));
}

std::string cppCustomClassName(const YamlMap& member)
{
    const YamlValue* value = yamlGet(member, "cpp-class");
    if (value == nullptr)
    {
        value = yamlGet(member, "cpp_class");
    }
    return value == nullptr ? std::string{} : valueAsString(*value);
}

std::string cppCustomHeader(const YamlMap& member)
{
    const YamlValue* value = yamlGet(member, "cpp-header");
    if (value == nullptr)
    {
        value = yamlGet(member, "cpp_header");
    }
    return value == nullptr ? std::string{} : valueAsString(*value);
}

std::string cppElementType(const std::string& elemType, const std::set<std::string>& scrollviewTypes = {})
{
    (void)scrollviewTypes;
    if (elemType == "label" || elemType == "spanlabel")
    {
        return "ui::Label";
    }
    if (elemType == "infolabel")
    {
        return "ui::InfoLabel";
    }
    if (elemType == "framebufferview")
    {
        return "ui::FrameBufferView";
    }
    if (elemType == "messagetable")
    {
        return "ui::MessageTable";
    }
    if (elemType == "image")
    {
        return "ui::Image";
    }
    if (elemType == "button")
    {
        return "ui::Button";
    }
    if (elemType == "checkbox")
    {
        return "ui::CheckBox";
    }
    if (elemType == "textinput")
    {
        return "ui::TextInput";
    }
    if (elemType == "textarea")
    {
        return "ui::TextArea";
    }
    if (elemType == "numberinput")
    {
        return "ui::NumberInput";
    }
    if (elemType == "combobox")
    {
        return "ui::ComboBox";
    }
    if (elemType == "listbox")
    {
        return "ui::ListBox";
    }
    if (elemType == "uiscrollview")
    {
        return "ui::ScrollView";
    }
    if (elemType == "viewhost")
    {
        return "ui::ViewHost";
    }
    return "ui::ReusableElement";
}

std::string imageRenderMode(const YamlMap& member)
{
    const YamlValue* value = yamlGet(member, "render_mode");
    if (value == nullptr)
    {
        value = yamlGet(member, "render-mode");
    }
    return lower(trim(value == nullptr ? std::string{"auto"} : valueAsString(*value, "auto")));
}

bool memberRequiresSixel(const YamlMap& member)
{
    return typeFor(member) == "image" && imageRenderMode(member) != "fallback";
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

std::string addElementCppType(const YamlMap& member, const std::string& elemType, const std::set<std::string>& scrollviewTypes = {})
{
    (void)member;
    return cppElementType(elemType, scrollviewTypes);
}

std::string customChildClassName(const YamlMap& member, const std::string& elemType)
{
    const std::string cppName = cppCustomClassName(member);
    return cppName.empty() ? customClassName(member, elemType) : cppName;
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
            result += "\\u00";
            const char* hex = "0123456789abcdef";
            result += hex[(ch >> 4U) & 0x0F];
            result += hex[ch & 0x0F];
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

std::string jsonCompactMap(const YamlMap& map)
{
    std::string result = "{";
    bool first = true;
    for (const auto& [key, value] : map)
    {
        if (!first)
        {
            result += ",";
        }
        first = false;
        result += jsonEscape(key);
        result += ":";
        result += jsonCompact(value);
    }
    result += "}";
    return result;
}

std::string jsonCompactList(const YamlList& list)
{
    std::string result = "[";
    for (std::size_t index = 0; index < list.size(); ++index)
    {
        if (index != 0)
        {
            result += ",";
        }
        result += jsonCompact(list[index]);
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
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? "true" : "false";
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
    if (std::holds_alternative<std::string>(value.value))
    {
        return jsonEscape(std::get<std::string>(value.value));
    }
    if (std::holds_alternative<YamlList>(value.value))
    {
        return jsonCompactList(std::get<YamlList>(value.value));
    }
    return jsonCompactMap(std::get<YamlMap>(value.value));
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

std::vector<std::string> dependencyNames(
    const YamlMap& members,
    const std::vector<std::string>& explicitDependencies,
    const std::set<std::string>& scrollviewTypes = {}
)
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
        if (isBuiltinType(elemType, scrollviewTypes))
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
        root / sourceName / (sourceName + suffix),
        parent / (sourceName + suffix),
        parent / sourceName / (sourceName + suffix),
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
            if (entry.is_regular_file() && entry.path().filename() == sourceName + suffix)
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

bool isScrollviewExtension(const CompilerDocument& model)
{
    return lower(trim(model.extends)) == "uiscrollview";
}

std::set<std::string> scrollviewDependencyTypes(const std::filesystem::path& mdPath, const CompilerDocument& model)
{
    std::set<std::string> types;
    for (const std::string& sourceName : dependencyNames(model.document.members, model.dependencies, {}))
    {
        const std::filesystem::path dependency = findDependencySourceFile(mdPath, sourceName);
        if (dependency.empty())
        {
            continue;
        }
        CompilerDocument dependencyModel = parseCompilerDocument(dependency);
        if (lower(dependencyModel.extends) == "uiscrollview")
        {
            types.insert(std::filesystem::path(sourceName).filename().string());
        }
    }
    return types;
}

std::vector<std::filesystem::path> dependencyPaths(const std::filesystem::path& mdPath, const CompilerDocument& model)
{
    std::vector<std::filesystem::path> paths;
    const std::set<std::string> scrollviewTypes = scrollviewDependencyTypes(mdPath, model);
    for (const std::string& sourceName : dependencyNames(model.document.members, model.dependencies, scrollviewTypes))
    {
        const std::filesystem::path dependency = findDependencySourceFile(mdPath, sourceName);
        if (!dependency.empty())
        {
            paths.push_back(dependency);
        }
    }
    return paths;
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

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeCppGenerateOptions& options)
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

std::string ctorArgs(
    const std::string& name,
    const YamlMap& member,
    const std::map<std::string, int>& scrollviewGaps = {},
    bool customScrollviewClass = false
)
{
    (void)customScrollviewClass;
    const std::string elemType = typeFor(member);
    if (elemType == "label" || elemType == "spanlabel" || elemType == "infolabel" || elemType == "messagetable")
    {
        return cppString(name) + ", " + cppString(yamlString(member, "text", name));
    }
    if (elemType == "framebufferview")
    {
        return cppString(name);
    }
    if (elemType == "image")
    {
        const YamlValue* renderMode = yamlGet(member, "render_mode");
        if (renderMode == nullptr)
        {
            renderMode = yamlGet(member, "render-mode");
        }
        const YamlValue* verticalAlign = yamlGet(member, "valign");
        if (verticalAlign == nullptr)
        {
            verticalAlign = yamlGet(member, "vertical_align");
        }
        if (verticalAlign == nullptr)
        {
            verticalAlign = yamlGet(member, "vertical-align");
        }
        return cppString(name) + ", " +
            cppStringValue(yamlGet(member, "source")) + ", " +
            cppStringValue(yamlGet(member, "alt")) + ", " +
            cppStringValue(yamlGet(member, "fit"), "contain") + ", " +
            cppStringValue(renderMode, "auto") + ", " +
            cppStringValue(yamlGet(member, "align"), "center") + ", " +
            cppStringValue(verticalAlign, "middle");
    }
    if (elemType == "button")
    {
        return cppString(name) + ", " + cppString(yamlString(member, "title", name));
    }
    if (elemType == "checkbox")
    {
        const YamlValue* checkedValue = yamlGet(member, "checked");
        if (checkedValue == nullptr)
        {
            checkedValue = yamlGet(member, "value");
        }
        const std::string checked = truthy(checkedValue, false) ? "true" : "false";
        return cppString(name) + ", " + cppString(yamlString(member, "title", name)) + ", " + checked;
    }
    if (elemType == "textinput" || elemType == "textarea")
    {
        const int maxLength = valueAsInt(yamlGet(member, "maxlength"), 0);
        return cppString(name) + ", " + cppStringValue(yamlGet(member, "value")) + ", " + std::to_string(maxLength);
    }
    if (elemType == "numberinput")
    {
        return cppString(name) + ", " +
            doubleCode(valueAsDouble(yamlGet(member, "value"), 0.0)) + ", " +
            doubleCode(valueAsDouble(yamlGet(member, "step_size") != nullptr ? yamlGet(member, "step_size") : yamlGet(member, "step"), 1.0));
    }
    if (elemType == "combobox" || elemType == "listbox")
    {
        const std::vector<std::string> options = listValue(yamlGet(member, "options"));
        std::string optionList;
        for (std::size_t index = 0; index < options.size(); ++index)
        {
            if (index != 0)
            {
                optionList += ", ";
            }
            optionList += cppString(options[index]);
        }
        return cppString(name) + ", std::vector<std::string>{" + optionList + "}";
    }
    if (elemType == "uiscrollview")
    {
        int gap = 0;
        const auto it = scrollviewGaps.find(elemType);
        if (it != scrollviewGaps.end())
        {
            gap = it->second;
        }
        return cppString(name) + ", " + std::to_string(gap);
    }
    return cppString(name);
}

std::string mcpElementMetadataCode(const YamlMap& members)
{
    std::vector<std::string> entries;
    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string expose = truthy(yamlGet(*member, "expose"), true) ? "true" : "false";
        entries.push_back(
            "ui::GeneratedElementMetadata{" + cppString(name) + ", " +
            cppString(descriptionForMember(name, *member)) + ", " + expose + "}"
        );
    }
    std::string result = "std::vector<ui::GeneratedElementMetadata>{";
    for (std::size_t index = 0; index < entries.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += entries[index];
    }
    result += "}";
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
            "ui::GeneratedAppToolMetadata{" + cppString(name) + ", " +
            cppString(yamlString(*tool, "description", "")) + ", " +
            cppRawString(inputJson) + ", " + cppRawString(outputJson) + "}"
        );
    }
    std::string result = "std::vector<ui::GeneratedAppToolMetadata>{";
    for (std::size_t index = 0; index < entries.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += entries[index];
    }
    result += "}";
    return result;
}

std::string dependencyHeaderInclude(const std::filesystem::path& mdPath, const std::filesystem::path& outputDir, const std::string& sourceName)
{
    std::filesystem::path dependency = findDependencySourceFile(mdPath, sourceName);
    std::filesystem::path header;
    if (!dependency.empty())
    {
        header = dependency.parent_path() / (sourceName + "_ui.hpp");
    }
    else
    {
        header = findDependencyFile(mdPath, sourceName + "_ui", ".hpp");
    }
    if (header.empty())
    {
        header = outputDir / (sourceName + "_ui.hpp");
    }
    return std::filesystem::relative(header, outputDir).generic_string();
}

std::vector<std::string> customSourceIncludes(
    const std::filesystem::path& mdPath,
    const std::filesystem::path& outputDir,
    const std::string& baseName,
    const YamlMap& members,
    const std::set<std::string>& scrollviewTypes
)
{
    std::vector<std::string> includes;
    std::set<std::string> seen;
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string customHeader = cppCustomHeader(*member);
        if (!customHeader.empty() && seen.insert(customHeader).second)
        {
            includes.push_back(customHeader);
        }
        const std::string elemType = typeFor(*member);
        if (isBuiltinType(elemType, scrollviewTypes))
        {
            continue;
        }
        const std::string sourceName = customSourceName(*member, elemType);
        if (sourceName == baseName)
        {
            continue;
        }
        const std::string include = dependencyHeaderInclude(mdPath, outputDir, sourceName);
        if (seen.insert(include).second)
        {
            includes.push_back(include);
        }
    }
    return includes;
}

std::string cppIdentifierFragment(const std::string& name)
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
        if (part.empty())
        {
            continue;
        }
        part[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(part[0])));
        for (std::size_t index = 1; index < part.size(); ++index)
        {
            part[index] = static_cast<char>(std::tolower(static_cast<unsigned char>(part[index])));
        }
        result += part;
    }
    return result.empty() ? "Element" : result;
}

std::string cppEventMethodName(const std::string& name, const std::string& suffix)
{
    return "on" + cppIdentifierFragment(name) + suffix;
}

std::string cppToolMethodName(const std::string& name)
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
    if (parts.empty())
    {
        return "appTool";
    }
    std::string result = lower(parts.front());
    for (std::size_t index = 1; index < parts.size(); ++index)
    {
        std::string part = parts[index];
        part[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(part[0])));
        for (std::size_t charIndex = 1; charIndex < part.size(); ++charIndex)
        {
            part[charIndex] = static_cast<char>(std::tolower(static_cast<unsigned char>(part[charIndex])));
        }
        result += part;
    }
    return result;
}

struct EventSpec
{
    std::string name;
    std::string channel;
    std::string suffix;
    std::string methodName;
    std::string args;
};

std::vector<EventSpec> eventSpecsForMember(const std::string& name, const YamlMap& member)
{
    const std::string elemType = typeFor(member);
    if (elemType == "button" || elemType == "image")
    {
        return {{name, "button", "Click", cppEventMethodName(name, "Click"), ""}};
    }
    if (elemType == "checkbox")
    {
        return {{name, "text", "Change", cppEventMethodName(name, "Change"), "std::string_view value"}};
    }
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return {
            {name, "text", "Change", cppEventMethodName(name, "Change"), "std::string_view value"},
            {name, "confirmed", "Submit", cppEventMethodName(name, "Submit"), "std::string_view value"},
        };
    }
    if (elemType == "combobox")
    {
        return {{name, "selection_text", "Change", cppEventMethodName(name, "Change"), "std::string_view value"}};
    }
    if (elemType == "listbox")
    {
        return {{name, "selection", "SelectionChange", cppEventMethodName(name, "SelectionChange"), "const std::vector<std::string>& value"}};
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
            const auto key = std::make_pair(spec.methodName, spec.args);
            if (seen.insert(key).second)
            {
                specs.push_back(spec);
            }
        }
    }
    return specs;
}

std::string generateCppHookDeclarations(const YamlMap& members)
{
    std::vector<std::string> lines;
    for (const EventSpec& spec : eventSpecs(members))
    {
        lines.push_back(spec.args.empty()
            ? "    virtual void " + spec.methodName + "();"
            : "    virtual void " + spec.methodName + "(" + spec.args + ");");
    }
    lines.push_back("    virtual bool shouldClose() const;");
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

std::string generateCppToolDeclarations(const YamlMap& mcpTools)
{
    return mcpTools.empty() ? std::string{} : "    std::string callAppTool(std::string_view name, std::string_view inputJson) override;";
}

std::string generateCppToolHookDeclarations(const YamlMap& mcpTools)
{
    std::vector<std::string> names;
    for (const auto& [name, value] : mcpTools)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    std::string result;
    for (std::size_t index = 0; index < names.size(); ++index)
    {
        if (index != 0)
        {
            result += "\n";
        }
        result += "    virtual std::string " + cppToolMethodName(names[index]) + "(std::string_view inputJson);";
    }
    return result;
}

std::string styleStateName(const std::string& key)
{
    for (const std::string& prefix : {"focus-", "edit-", "cursor-", "selected-", "checked-", "unchecked-", "disabled-", "error-"})
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

std::string textGradientCode(const YamlMap& gradient)
{
    std::vector<std::string> colors;
    const YamlValue* colorsValue = yamlGet(gradient, "colors");
    if (std::holds_alternative<std::string>(colorsValue == nullptr ? YamlValue{}.value : colorsValue->value))
    {
        if (colorsValue != nullptr)
        {
            colors.push_back(std::get<std::string>(colorsValue->value));
        }
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
        colorArgs += cppString(colors[index]);
    }
    return "makeTextGradient(" + std::to_string(interval) + ", " + std::to_string(step) + ", " + std::to_string(segmentSize) + ", {" + colorArgs + "})";
}

std::string styleCode(const YamlMap& style)
{
    std::vector<std::string> pairs;
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
        if (accepted.find(key) != accepted.end())
        {
            pairs.push_back("{" + cppString(key) + ", " + cppString(valueAsString(value)) + "}");
        }
        else if (key == "text-color-gradient")
        {
            colorGradient = valueAsMap(&value);
        }
        else if (key == "text-background-gradient")
        {
            backgroundGradient = valueAsMap(&value);
        }
    }
    std::string joinedPairs;
    for (std::size_t index = 0; index < pairs.size(); ++index)
    {
        if (index != 0)
        {
            joinedPairs += ", ";
        }
        joinedPairs += pairs[index];
    }
    const std::string base = "makeStyle({" + joinedPairs + "})";
    std::vector<std::string> assignments;
    if (colorGradient != nullptr)
    {
        assignments.push_back("style.textColorGradient = " + textGradientCode(*colorGradient) + ";");
    }
    if (backgroundGradient != nullptr)
    {
        assignments.push_back("style.textBackgroundGradient = " + textGradientCode(*backgroundGradient) + ";");
    }
    if (assignments.empty())
    {
        return base;
    }
    std::string assignmentLines;
    for (const std::string& assignment : assignments)
    {
        assignmentLines += "        " + assignment + "\n";
    }
    return "[]\n"
        "    {\n"
        "        ui::Style style = " + base + ";\n" +
        assignmentLines +
        "        return style;\n"
        "    }()";
}

struct LayoutEntryData
{
    std::string name;
    std::string type;
    std::string cellName;
    int relativeRow = 0;
    int relativeCol = 0;
    int relativeWidth = 0;
    int relativeHeight = 0;
    int sourceRow = 0;
    int sourceCol = 0;
    int sourceWidth = 0;
    int sourceHeight = 0;
    std::string widthMode = "auto";
    int widthValue = 0;
    std::string heightMode = "auto";
    int heightValue = 0;
    std::string cellWidthMode = "auto";
    int cellWidthValue = 0;
    std::string cellHeightMode = "auto";
    int cellHeightValue = 0;
    int charsWidth = 0;
    int charsHeight = 0;
    int cellCharsWidth = 0;
    int cellCharsHeight = 0;
    int marginRight = 0;
    int marginBottom = 0;
};

LayoutEntryData layoutEntryData(const NativeLayoutItem& cell, const YamlMap& members)
{
    const std::string name = cell.content;
    const YamlMap* member = name.empty() ? nullptr : valueAsMap(yamlGet(members, name));
    const std::string elemType = member == nullptr ? (name.empty() ? "" : "label") : typeFor(*member);
    return LayoutEntryData{
        name,
        elemType,
        cell.cellName,
        cell.row,
        cell.col,
        cell.width,
        cell.height,
        cell.cellRow,
        cell.cellCol,
        cell.cellCharsWidth,
        cell.cellCharsHeight,
        cell.widthMode,
        cell.width,
        cell.heightMode,
        cell.height,
        cell.cellWidthMode,
        cell.cellWidth,
        cell.cellHeightMode,
        cell.cellHeight,
        cell.charsWidth,
        cell.charsHeight,
        cell.cellCharsWidth,
        cell.cellCharsHeight,
        cell.marginRight,
        cell.marginBottom,
    };
}

std::string rectCode(int row, int col, int width, int height)
{
    return "ui::Rect{" + std::to_string(row) + ", " + std::to_string(col) + ", " + std::to_string(width) + ", " + std::to_string(height) + "}";
}

std::string dimensionCode(const std::string& mode, int value)
{
    return "dimension(" + cppString(mode) + ", " + std::to_string(value) + ")";
}

std::string sizeCode(int width, int height)
{
    return "ui::Size{" + std::to_string(width) + ", " + std::to_string(height) + "}";
}

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
    LayoutEntryData data = layoutEntryData(cell, members);
    if (scrollviewExtension && data.name.empty() && (data.cellName == "panel" || data.cellName == "viewport"))
    {
        data.name = GENERATED_SCROLLVIEW_ELEMENT_NAME;
        data.type = "uiscrollview";
        data.widthMode = "expanded";
        data.widthValue = 0;
        data.heightMode = "expanded";
        data.heightValue = 0;
    }
    YamlMap cellStyleMap = cellStyle(style, data.cellName);
    if (scrollviewExtension && data.name == GENERATED_SCROLLVIEW_ELEMENT_NAME)
    {
        cellStyleMap = scrollviewExtensionLayoutCellStyle(cellStyleMap);
    }
    const std::string cellStyleCode = cellStyleMap.empty() ? "" : ", .cellStyle = " + styleCode(cellStyleMap);
    YamlMap elementStyle = layoutElementStyle(style, data.type, data.name, data.cellName);
    const std::string elementStyleCode = elementStyle.empty() ? "" : ", .elementStyle = " + styleCode(elementStyle);
    return "        ui::GeneratedLayoutEntry{"
        ".name = " + cppString(data.name) + ", "
        ".type = " + cppString(data.type) + ", "
        ".cellName = " + cppString(data.cellName) + ", "
        ".relative = " + rectCode(data.relativeRow, data.relativeCol, data.relativeWidth, data.relativeHeight) + ", "
        ".sourceCell = " + rectCode(data.sourceRow, data.sourceCol, data.sourceWidth, data.sourceHeight) + ", "
        ".width = " + dimensionCode(data.widthMode, data.widthValue) + ", "
        ".height = " + dimensionCode(data.heightMode, data.heightValue) + ", "
        ".cellWidth = " + dimensionCode(data.cellWidthMode, data.cellWidthValue) + ", "
        ".cellHeight = " + dimensionCode(data.cellHeightMode, data.cellHeightValue) + ", "
        ".charsSize = " + sizeCode(data.charsWidth, data.charsHeight) + ", "
        ".cellCharsSize = " + sizeCode(data.cellCharsWidth, data.cellCharsHeight) + ", "
        ".marginRight = " + std::to_string(data.marginRight) + ", "
        ".marginBottom = " + std::to_string(data.marginBottom) +
        cellStyleCode + elementStyleCode + "},";
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

YamlValue embeddedScrollviewPanelStyleValue(const std::string& key, const YamlValue& value)
{
    (void)key;
    return value;
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
            dependencyStyle[key] = embeddedScrollviewPanelStyleValue(key, value);
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

std::vector<std::string> generateCppHookDefinitions(const std::string& classNameValue, const YamlMap& members)
{
    std::vector<std::string> lines;
    for (const EventSpec& spec : eventSpecs(members))
    {
        lines.push_back("void " + classNameValue + "::" + spec.methodName + "(" + spec.args + ")");
        lines.push_back("{");
        if (!spec.args.empty())
        {
            std::string argName = spec.args.substr(spec.args.find_last_of(' ') + 1);
            while (!argName.empty() && argName.front() == '&')
            {
                argName.erase(argName.begin());
            }
            lines.push_back("    (void)" + argName + ";");
        }
        lines.push_back("}");
        lines.push_back("");
    }
    lines.push_back("bool " + classNameValue + "::shouldClose() const");
    lines.push_back("{");
    lines.push_back("    return false;");
    lines.push_back("}");
    lines.push_back("");
    return lines;
}

std::vector<std::string> generateCppToolDefinitions(const std::string& classNameValue, const YamlMap& mcpTools)
{
    std::vector<std::string> lines;
    if (mcpTools.empty())
    {
        return lines;
    }

    std::vector<std::string> names;
    for (const auto& [name, value] : mcpTools)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    for (const std::string& name : names)
    {
        const std::string methodName = cppToolMethodName(name);
        lines.push_back("std::string " + classNameValue + "::" + methodName + "(std::string_view inputJson)");
        lines.push_back("{");
        lines.push_back("    (void)inputJson;");
        lines.push_back("    throw std::runtime_error(" + cppString("App tool not implemented: " + name) + ");");
        lines.push_back("}");
        lines.push_back("");
    }

    lines.push_back("std::string " + classNameValue + "::callAppTool(std::string_view name, std::string_view inputJson)");
    lines.push_back("{");
    for (std::size_t index = 0; index < names.size(); ++index)
    {
        const std::string keyword = index == 0 ? "if" : "else if";
        lines.push_back("    " + keyword + " (name == " + cppString(names[index]) + ")");
        lines.push_back("    {");
        lines.push_back("        return " + cppToolMethodName(names[index]) + "(inputJson);");
        lines.push_back("    }");
    }
    lines.push_back("    return ui::GeneratedWindowBase::callAppTool(name, inputJson);");
    lines.push_back("}");
    lines.push_back("");
    return lines;
}

void appendIfBranches(
    std::vector<std::string>& lines,
    const std::vector<std::pair<std::string, std::string>>& specs,
    const std::string& callLine,
    int initialIndex = 0
)
{
    int branchIndex = initialIndex;
    for (const auto& [name, method] : specs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("    " + keyword + " (name == " + cppString(name) + ")");
        lines.push_back("    {");
        std::string line = callLine;
        std::size_t position = line.find("@METHOD@");
        if (position != std::string::npos)
        {
            line.replace(position, std::string{"@METHOD@"}.size(), method);
        }
        lines.push_back(line);
        lines.push_back("        return true;");
        lines.push_back("    }");
        ++branchIndex;
    }
}

void appendCppEventDispatch(std::vector<std::string>& lines, const std::string& classNameValue, const YamlMap& members)
{
    std::vector<std::pair<std::string, std::string>> buttonSpecs;
    std::vector<std::pair<std::string, std::string>> textSpecs;
    std::vector<std::pair<std::string, std::string>> confirmedSpecs;
    std::vector<std::pair<std::string, std::string>> selectionTextSpecs;
    std::vector<std::pair<std::string, std::string>> selectionSpecs;
    for (const EventSpec& spec : eventSpecs(members))
    {
        if (spec.channel == "button")
        {
            buttonSpecs.push_back({spec.name, spec.methodName});
        }
        else if (spec.channel == "text")
        {
            textSpecs.push_back({spec.name, spec.methodName});
        }
        else if (spec.channel == "confirmed")
        {
            confirmedSpecs.push_back({spec.name, spec.methodName});
        }
        else if (spec.channel == "selection_text")
        {
            selectionTextSpecs.push_back({spec.name, spec.methodName});
        }
        else if (spec.channel == "selection")
        {
            selectionSpecs.push_back({spec.name, spec.methodName});
        }
    }

    lines.push_back("bool " + classNameValue + "::handleGeneratedButton(std::string_view name)");
    lines.push_back("{");
    appendIfBranches(lines, buttonSpecs, "        @METHOD@();");
    lines.push_back("    return false;");
    lines.push_back("}");
    lines.push_back("");

    lines.push_back("bool " + classNameValue + "::handleGeneratedTextChanged(std::string_view name, std::string_view value)");
    lines.push_back("{");
    appendIfBranches(lines, textSpecs, "        @METHOD@(value);");
    lines.push_back("    return false;");
    lines.push_back("}");
    lines.push_back("");

    lines.push_back("bool " + classNameValue + "::handleGeneratedTextConfirmed(std::string_view name, std::string_view value)");
    lines.push_back("{");
    appendIfBranches(lines, confirmedSpecs, "        @METHOD@(value);");
    lines.push_back("    return false;");
    lines.push_back("}");
    lines.push_back("");

    lines.push_back("bool " + classNameValue + "::handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value)");
    lines.push_back("{");
    int branchIndex = 0;
    for (const auto& [name, method] : selectionTextSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("    " + keyword + " (name == " + cppString(name) + ")");
        lines.push_back("    {");
        lines.push_back("        " + method + "(value.empty() ? std::string_view{} : std::string_view{value.front()});");
        lines.push_back("        return true;");
        lines.push_back("    }");
        ++branchIndex;
    }
    for (const auto& [name, method] : selectionSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("    " + keyword + " (name == " + cppString(name) + ")");
        lines.push_back("    {");
        lines.push_back("        " + method + "(value);");
        lines.push_back("        return true;");
        lines.push_back("    }");
        ++branchIndex;
    }
    lines.push_back("    return false;");
    lines.push_back("}");
    lines.push_back("");

    lines.push_back("ui::GeneratedWindowRuntimeOptions " + classNameValue + "::runtimeOptions()");
    lines.push_back("{");
    lines.push_back("    ui::GeneratedWindowRuntimeOptions options;");

    auto appendRuntimeOptions = [&](const std::string& optionName, const std::string& signature, const std::vector<std::pair<std::string, std::string>>& specs, const std::string& dispatchLine) {
        if (specs.empty())
        {
            return;
        }
        lines.push_back("    options." + optionName + " = [this]" + signature);
        lines.push_back("    {");
        for (std::size_t index = 0; index < specs.size(); ++index)
        {
            const std::string keyword = index == 0 ? "if" : "else if";
            lines.push_back("        " + keyword + " (name == " + cppString(specs[index].first) + ")");
            lines.push_back("        {");
            lines.push_back(dispatchLine);
            lines.push_back("        }");
        }
        lines.push_back("    };");
        lines.push_back("");
    };

    appendRuntimeOptions("onButton", "(std::string_view name)", buttonSpecs, "            (void)handleGeneratedButton(name);");
    appendRuntimeOptions("onTextChanged", "(std::string_view name, std::string_view value)", textSpecs, "            (void)handleGeneratedTextChanged(name, value);");
    appendRuntimeOptions("onTextConfirmed", "(std::string_view name, std::string_view value)", confirmedSpecs, "            (void)handleGeneratedTextConfirmed(name, value);");

    if (!selectionTextSpecs.empty() || !selectionSpecs.empty())
    {
        lines.push_back("    options.onSelectionChanged = [this](std::string_view name, const std::vector<std::string>& value)");
        lines.push_back("    {");
        branchIndex = 0;
        for (const auto& [name, method] : selectionTextSpecs)
        {
            (void)method;
            const std::string keyword = branchIndex == 0 ? "if" : "else if";
            lines.push_back("        " + keyword + " (name == " + cppString(name) + ")");
            lines.push_back("        {");
            lines.push_back("            (void)handleGeneratedSelectionChanged(name, value);");
            lines.push_back("        }");
            ++branchIndex;
        }
        for (const auto& [name, method] : selectionSpecs)
        {
            (void)method;
            const std::string keyword = branchIndex == 0 ? "if" : "else if";
            lines.push_back("        " + keyword + " (name == " + cppString(name) + ")");
            lines.push_back("        {");
            lines.push_back("            (void)handleGeneratedSelectionChanged(name, value);");
            lines.push_back("        }");
            ++branchIndex;
        }
        lines.push_back("    };");
        lines.push_back("");
    }

    lines.push_back("    options.shouldClose = [this]");
    lines.push_back("    {");
    lines.push_back("        return shouldClose();");
    lines.push_back("    };");
    lines.push_back("");
    lines.push_back("    return options;");
    lines.push_back("}");
    lines.push_back("");
}

std::string joinLines(const std::vector<std::string>& lines)
{
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

std::string generateHeader(const std::string& baseName, const std::string& classNameValue, const CompilerDocument& model, const std::filesystem::path& mdPath)
{
    const YamlMap& members = model.document.members;
    const YamlMap& mcpTools = model.mcpTools;
    const std::set<std::string> scrollviewTypes = scrollviewDependencyTypes(mdPath, model);
    const std::string baseClass = isScrollviewExtension(model) ? "ui::GeneratedScrollViewBase" : "ui::GeneratedWindowBase";

    std::vector<std::string> memberLines;
    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string elemType = typeFor(*member);
        memberLines.push_back("    " + addElementCppType(*member, elemType, scrollviewTypes) + "* " + name + " = nullptr;");
    }

    bool hasImage = false;
    std::vector<std::string> customIncludes;
    std::set<std::string> seenCustomIncludes;
    for (const auto& [name, value] : members)
    {
        (void)name;
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        if (typeFor(*member) == "image")
        {
            hasImage = true;
        }
        const std::string customHeader = cppCustomHeader(*member);
        if (!customHeader.empty() && seenCustomIncludes.insert(customHeader).second)
        {
            customIncludes.push_back("#include \"" + customHeader + "\"");
        }
    }

    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
        "#pragma once",
        "",
        "#include \"ui/generated/GeneratedWindowBase.hpp\"",
        "#include \"ui/generated/GeneratedWindowRuntime.hpp\"",
        "#include \"ui/elements/Button.hpp\"",
        "#include \"ui/elements/CheckBox.hpp\"",
        "#include \"ui/elements/ComboBox.hpp\"",
        "#include \"ui/elements/FrameBufferView.hpp\"",
    };
    if (hasImage)
    {
        lines.push_back("#include \"ui/elements/Image.hpp\"");
    }
    lines.insert(lines.end(), {
        "#include \"ui/elements/InfoLabel.hpp\"",
        "#include \"ui/elements/Label.hpp\"",
        "#include \"ui/elements/ListBox.hpp\"",
        "#include \"ui/elements/MessageTable.hpp\"",
        "#include \"ui/elements/NumberInput.hpp\"",
        "#include \"ui/elements/ReusableElement.hpp\"",
        "#include \"ui/elements/ScrollView.hpp\"",
        "#include \"ui/elements/TextArea.hpp\"",
        "#include \"ui/elements/TextInput.hpp\"",
    });
    lines.insert(lines.end(), customIncludes.begin(), customIncludes.end());
    lines.push_back("");
    lines.push_back("");
    lines.insert(lines.end(), {
        "#include <string>",
        "#include <string_view>",
        "#include <vector>",
        "",
        "class " + classNameValue + " : public " + baseClass,
        "{",
        "public:",
        "    " + classNameValue + "();",
        "",
        "    virtual ui::GeneratedWindowRuntimeOptions runtimeOptions();",
        "    bool handleGeneratedButton(std::string_view name) override;",
        "    bool handleGeneratedTextChanged(std::string_view name, std::string_view value) override;",
        "    bool handleGeneratedTextConfirmed(std::string_view name, std::string_view value) override;",
        "    bool handleGeneratedSelectionChanged(std::string_view name, const std::vector<std::string>& value) override;",
    });
    const std::string toolDeclarations = generateCppToolDeclarations(mcpTools);
    lines.push_back("");
    if (!toolDeclarations.empty())
    {
        lines.push_back(toolDeclarations);
    }
    lines.push_back("");
    lines.insert(lines.end(), memberLines.begin(), memberLines.end());
    lines.push_back("");
    if (memberLines.empty())
    {
        lines.push_back("");
    }
    lines.push_back("protected:");
    lines.push_back(generateCppHookDeclarations(members));
    const std::string toolHookDeclarations = generateCppToolHookDeclarations(mcpTools);
    lines.push_back("");
    if (!toolHookDeclarations.empty())
    {
        lines.push_back(toolHookDeclarations);
    }
    lines.push_back("};");
    lines.push_back("");
    return joinLines(lines);
}

std::vector<std::string> sourcePrelude(const std::string& baseName)
{
    return {
        "#include \"" + baseName + "_ui.hpp\"",
    };
}

void appendGeneratedHelpers(std::vector<std::string>& lines)
{
    lines.insert(lines.end(), {
        "",
        "#include <memory>",
        "#include <array>",
        "#include <charconv>",
        "#include <initializer_list>",
        "#include <string>",
        "#include <string_view>",
        "#include <stdexcept>",
        "#include <vector>",
        "",
        "namespace",
        "{",
        "",
        "ui::AxisDimension dimension(const char* mode, int value)",
        "{",
        "    const std::string text(mode);",
        "    if (text == \"fixed\")",
        "    {",
        "        return ui::AxisDimension::fixed(value);",
        "    }",
        "    if (text == \"expanded\")",
        "    {",
        "        return ui::AxisDimension::expanded();",
        "    }",
        "    if (text == \"fit-content\")",
        "    {",
        "        return ui::AxisDimension::fitContent();",
        "    }",
        "    return ui::AxisDimension{};",
        "}",
        "",
        "ui::Style makeStyle(std::initializer_list<std::pair<const char*, const char*>> props)",
        "{",
        "    ui::Style style;",
        "    auto parseInt = [](std::string_view text, int fallback = 0)",
        "    {",
        "        int value = fallback;",
        "        const auto* begin = text.data();",
        "        const auto* end = text.data() + text.size();",
        "        (void)std::from_chars(begin, end, value);",
        "        return value;",
        "    };",
        "    auto setPadding = [&](std::string_view value)",
        "    {",
        "        std::array<int, 4> values{0, 0, 0, 0};",
        "        std::size_t start = 0;",
        "        int count = 0;",
        "        while (start <= value.size() && count < 4)",
        "        {",
        "            const std::size_t comma = value.find(',', start);",
        "            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;",
        "            while (start < end && value[start] == ' ')",
        "            {",
        "                ++start;",
        "            }",
        "            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));",
        "            if (comma == std::string_view::npos)",
        "            {",
        "                break;",
        "            }",
        "            start = comma + 1;",
        "        }",
        "        if (count == 1)",
        "        {",
        "            style.padding = values[0];",
        "            style.paddingTop = values[0];",
        "            style.paddingRight = values[0];",
        "            style.paddingBottom = values[0];",
        "            style.paddingLeft = values[0];",
        "        }",
        "        else if (count == 2)",
        "        {",
        "            style.paddingTop = values[0];",
        "            style.paddingRight = values[1];",
        "            style.paddingBottom = values[0];",
        "            style.paddingLeft = values[1];",
        "        }",
        "        else if (count >= 4)",
        "        {",
        "            style.paddingTop = values[0];",
        "            style.paddingRight = values[1];",
        "            style.paddingBottom = values[2];",
        "            style.paddingLeft = values[3];",
        "        }",
        "    };",
        "    auto setMargin = [&](std::string_view value)",
        "    {",
        "        std::array<int, 4> values{0, 0, 0, 0};",
        "        std::size_t start = 0;",
        "        int count = 0;",
        "        while (start <= value.size() && count < 4)",
        "        {",
        "            const std::size_t comma = value.find(',', start);",
        "            const std::size_t end = comma == std::string_view::npos ? value.size() : comma;",
        "            while (start < end && value[start] == ' ')",
        "            {",
        "                ++start;",
        "            }",
        "            values[static_cast<std::size_t>(count++)] = parseInt(value.substr(start, end - start));",
        "            if (comma == std::string_view::npos)",
        "            {",
        "                break;",
        "            }",
        "            start = comma + 1;",
        "        }",
        "        if (count == 1)",
        "        {",
        "            style.margin = values[0];",
        "            style.marginTop = values[0];",
        "            style.marginRight = values[0];",
        "            style.marginBottom = values[0];",
        "            style.marginLeft = values[0];",
        "        }",
        "        else if (count == 2)",
        "        {",
        "            style.marginTop = values[0];",
        "            style.marginRight = values[1];",
        "            style.marginBottom = values[0];",
        "            style.marginLeft = values[1];",
        "        }",
        "        else if (count >= 4)",
        "        {",
        "            style.marginTop = values[0];",
        "            style.marginRight = values[1];",
        "            style.marginBottom = values[2];",
        "            style.marginLeft = values[3];",
        "        }",
        "    };",
        "    for (const auto& [key, value] : props)",
        "    {",
        "        const std::string prop(key);",
        "        if (prop == \"color\")",
        "        {",
        "            style.color = ui::Color(value);",
        "        }",
        "        else if (prop == \"background\")",
        "        {",
        "            style.background = ui::Color(value);",
        "        }",
        "        else if (prop == \"background-texture\")",
        "        {",
        "            style.backgroundTexture = value;",
        "        }",
        "        else if (prop == \"background-texture-color\")",
        "        {",
        "            style.backgroundTextureColor = ui::Color(value);",
        "        }",
        "        else if (prop == \"scope-dim-background\")",
        "        {",
        "            style.scopeDimBackground = ui::Color(value);",
        "        }",
        "        else if (prop == \"border-color\")",
        "        {",
        "            style.borderColor = ui::Color(value);",
        "        }",
        "        else if (prop == \"border-width\")",
        "        {",
        "            style.borderWidthHorizontal = parseInt(value);",
        "            style.borderWidthVertical = parseInt(value);",
        "        }",
        "        else if (prop == \"border-width-horizontal\")",
        "        {",
        "            style.borderWidthHorizontal = parseInt(value);",
        "        }",
        "        else if (prop == \"border-width-vertical\")",
        "        {",
        "            style.borderWidthVertical = parseInt(value);",
        "        }",
        "        else if (prop == \"padding\")",
        "        {",
        "            setPadding(value);",
        "        }",
        "        else if (prop == \"margin\")",
        "        {",
        "            setMargin(value);",
        "        }",
        "        else if (prop == \"gap\")",
        "        {",
        "            style.gap = parseInt(value);",
        "        }",
        "        else if (prop == \"text-align\")",
        "        {",
        "            style.textAlign = value;",
        "        }",
        "        else if (prop == \"user-select\")",
        "        {",
        "            style.userSelect = value;",
        "        }",
        "        else if (prop == \"scroll-x\")",
        "        {",
        "            style.scrollX = (std::string_view(value) == \"true\" || std::string_view(value) == \"1\" || std::string_view(value) == \"yes\");",
        "        }",
        "        else if (prop == \"scroll-y\")",
        "        {",
        "            style.scrollY = (std::string_view(value) == \"true\" || std::string_view(value) == \"1\" || std::string_view(value) == \"yes\");",
        "        }",
        "    }",
        "    return style;",
        "}",
        "",
        "ui::TextGradient makeTextGradient(int intervalMs, int step, int segmentSize, std::initializer_list<const char*> colors)",
        "{",
        "    ui::TextGradient gradient;",
        "    gradient.intervalMs = intervalMs;",
        "    gradient.step = step;",
        "    gradient.segmentSize = segmentSize;",
        "    for (const char* color : colors)",
        "    {",
        "        gradient.colors.emplace_back(color);",
        "    }",
        "    return gradient;",
        "}",
        "",
        "std::vector<ui::GeneratedLayoutEntry> buildLayout()",
        "{",
        "    return {",
    });
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

std::string windowDescription(const CompilerDocument& model)
{
    std::string description = yamlString(model.document.metadata, "description", "");
    if (!description.empty())
    {
        return description;
    }
    const YamlMap* window = valueAsMap(yamlGet(model.mcpMetadata, "window"));
    return window == nullptr ? std::string{} : yamlString(*window, "description", "");
}

std::string generateSource(
    const std::string& baseName,
    const std::string& classNameValue,
    const CompilerDocument& model,
    const std::filesystem::path& mdPath,
    const std::filesystem::path& outputDir,
    bool mcpEnabled
)
{
    const YamlMap& members = model.document.members;
    const YamlMap& mcpTools = model.mcpTools;
    const YamlMap& style = model.style;
    const bool scrollviewExtension = isScrollviewExtension(model);
    const std::set<std::string> scrollviewTypes = scrollviewDependencyTypes(mdPath, model);
    const std::map<std::string, int> scrollviewGaps{{"uiscrollview", scrollviewExtensionGap(style)}};

    std::vector<std::string> lines = sourcePrelude(baseName);
    for (const std::string& includePath : customSourceIncludes(mdPath, outputDir, baseName, members, scrollviewTypes))
    {
        lines.push_back("#include \"" + includePath + "\"");
    }
    appendGeneratedHelpers(lines);

    for (const NativeLayoutItem& cell : model.document.layout)
    {
        lines.push_back(layoutEntry(cell, members, style, scrollviewExtension));
    }

    const std::string title = model.document.title.empty() ? baseName : model.document.title;
    const std::string baseClass = scrollviewExtension ? "ui::GeneratedScrollViewBase" : "ui::GeneratedWindowBase";
    lines.insert(lines.end(), {
        "    };",
        "}",
        "",
        "}  // namespace",
        "",
        classNameValue + "::" + classNameValue + "() : " + baseClass + "(" + cppString(title) + ")",
        "{",
        "    setGeneratedLayout(buildLayout());",
        std::string{"    setGeneratedFocusable("} + (model.focusable ? "true" : "false") + ");",
        "    setGeneratedKind(" + cppString(lower(trim(model.kind.empty() ? std::string{"window"} : model.kind))) + ");",
        std::string{"    setMcpMetadata("} + (mcpEnabled ? "true" : "false") + ", " + cppString(mcpClassName(baseName)) + ", " +
            cppRawString(mcpEnabled ? model.document.sourceText : std::string{}) + ", " + cppString(windowDescription(model)) + ", " +
            mcpElementMetadataCode(members) + ");",
        "    setMcpAppTools(" + mcpAppToolsCode(mcpTools) + ");",
    });
    if (membersRequireSixel(members))
    {
        lines.push_back("    ui::requireSixelForImageRendering();");
    }
    YamlMap windowStyle = styleWindow(style);
    if (!windowStyle.empty())
    {
        lines.push_back("    setGeneratedWindowStyle(" + styleCode(windowStyle) + ");");
    }

    if (scrollviewExtension)
    {
        lines.push_back(
            "    setGeneratedScrollView(static_cast<ui::ScrollView&>(addElement<ui::ScrollView>(" +
            cppString(GENERATED_SCROLLVIEW_ELEMENT_NAME) + ", " +
            std::to_string(scrollviewExtensionGap(style)) + ")));"
        );
        for (const auto& [setter, stateStyle] : scrollviewExtensionStyleStates(style))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("    scrollView()." + setter + "(" + styleCode(stateStyle) + ");");
            }
        }
        YamlMap descendantFocusStyle = scrollviewExtensionDescendantFocusStyle(style);
        if (!descendantFocusStyle.empty())
        {
            lines.push_back("    scrollView().setDescendantFocusStyle(" + styleCode(descendantFocusStyle) + ");");
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
        const std::string addElementType = addElementCppType(*member, elemType, scrollviewTypes);
        const std::string cppType = addElementType;
        lines.push_back(
            "    " + name + " = &static_cast<" + cppType + "&>("
            "addElement<" + addElementType + ">(" + ctorArgs(name, *member, scrollviewGaps) + "));"
        );
        const YamlValue* commitMode = yamlGet(*member, "commit-mode");
        if (commitMode == nullptr)
        {
            commitMode = yamlGet(*member, "commit_mode");
        }
        if (commitMode != nullptr && truthy(commitMode, false))
        {
            lines.push_back("    " + name + "->setCommitMode(" + cppString(valueAsString(*commitMode)) + ");");
        }
        if (elemType == "combobox")
        {
            const int index = selectedIndex(*member, "selected_item");
            if (index >= 0)
            {
                lines.push_back("    " + name + "->setSelectedIndex(" + std::to_string(index) + ");");
            }
        }
        if (elemType == "listbox")
        {
            if (truthy(yamlGet(*member, "multiple"), false))
            {
                lines.push_back("    " + name + "->setMultiple(true);");
            }
            std::vector<std::string> values = selectedValues(*member, "selected_items");
            if (values.size() > 1)
            {
                std::string selectedList;
                for (std::size_t index = 0; index < values.size(); ++index)
                {
                    if (index != 0)
                    {
                        selectedList += ", ";
                    }
                    selectedList += cppString(values[index]);
                }
                lines.push_back("    " + name + "->setSelectedValues(std::vector<std::string>{" + selectedList + "});");
            }
            else
            {
                const int index = selectedIndex(*member, "selected_items");
                if (index >= 0)
                {
                    lines.push_back("    " + name + "->setSelectedIndex(" + std::to_string(index) + ");");
                }
            }
        }
        if (!isBuiltinType(elemType, scrollviewTypes))
        {
            lines.push_back("    " + name + "->setChild(std::make_unique<" + customChildClassName(*member, elemType) + ">());");
        }

        const std::string cellName = cellNameForElement(model.document.layout, name);
        std::string styleElemType;
        if (elemType == "textarea")
        {
            styleElemType = "textinput";
        }
        else if (!isBuiltinType(elemType, scrollviewTypes) || scrollviewTypes.find(elemType) != scrollviewTypes.end())
        {
            styleElemType = "uielement";
        }
        else
        {
            styleElemType = elemType;
        }
        for (const auto& [setter, stateStyle] : styleStatesForElement(style, styleElemType, name, cellName, member))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("    " + name + "->" + setter + "(" + styleCode(stateStyle) + ");");
            }
        }
    }

    lines.push_back("}");
    lines.push_back("");
    const std::vector<std::string> hookDefinitions = generateCppHookDefinitions(classNameValue, members);
    lines.insert(lines.end(), hookDefinitions.begin(), hookDefinitions.end());
    const std::vector<std::string> toolDefinitions = generateCppToolDefinitions(classNameValue, mcpTools);
    lines.insert(lines.end(), toolDefinitions.begin(), toolDefinitions.end());
    appendCppEventDispatch(lines, classNameValue, members);
    return joinLines(lines);
}

std::string memberType(const YamlMap& members, const std::string& name)
{
    const YamlMap* member = valueAsMap(yamlGet(members, name));
    return member == nullptr ? std::string{} : lower(yamlString(*member, "type", ""));
}

std::string generateAppStubHandlers(const CompilerDocument& model)
{
    const YamlMap& members = model.document.members;
    if (
        (memberType(members, "name") == "textinput" || memberType(members, "name") == "textarea") &&
        memberType(members, "hello_button") == "button" &&
        memberType(members, "quit_button") == "button" &&
        (memberType(members, "headline") == "label" || memberType(members, "headline") == "spanlabel" || memberType(members, "headline") == "infolabel")
    )
    {
        return R"CPP(    void onHelloButtonClick() override
    {
        std::string value = name->value();
        if (value.empty())
        {
            value = "world";
        }
        headline->setText("Hello, " + value + "!");
    }

    void onQuitButtonClick() override
    {
        closeRequested_ = true;
    }

    bool shouldClose() const override
    {
        return closeRequested_;
    }

private:
    bool closeRequested_ = false;
)CPP";
    }
    if (
        (memberType(members, "name") == "textinput" || memberType(members, "name") == "textarea") &&
        memberType(members, "greet") == "button" &&
        (memberType(members, "output") == "label" || memberType(members, "output") == "spanlabel" || memberType(members, "output") == "infolabel")
    )
    {
        return R"CPP(    void onGreetClick() override
    {
        std::string value = name->value();
        if (value.empty())
        {
            value = "world";
        }
        output->setText("Hello, " + value + "!");
    }
)CPP";
    }
    return "    // Override generated event hooks here.\n";
}

std::string generateAppStub(const std::string& baseName, const std::string& classNameValue, const CompilerDocument& model)
{
    std::string appClassName = classNameValue;
    if (endsWith(appClassName, "UI"))
    {
        appClassName.resize(appClassName.size() - 2);
    }
    else
    {
        appClassName += "App";
    }
    return "// Application logic for " + baseName + ".\n"
        "#include \"" + baseName + "_ui.hpp\"\n"
        "\n"
        "#include \"ui/generated/GeneratedWindowRuntime.hpp\"\n"
        "\n"
        "#include <string>\n"
        "\n"
        "class " + appClassName + " : public " + classNameValue + "\n"
        "{\n"
        "protected:\n" +
        generateAppStubHandlers(model) +
        "\n"
        "};\n"
        "\n"
        "int main(int argc, char** argv)\n"
        "{\n"
        "    " + appClassName + " app;\n"
        "    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);\n"
        "}\n";
}

std::string cppCMake(const std::string& project)
{
    return "cmake_minimum_required(VERSION 3.20)\n"
        "\n"
        "project(" + project + " LANGUAGES CXX)\n"
        "\n"
        "find_package(uimd CONFIG QUIET)\n"
        "\n"
        "if(NOT TARGET uimd::runtime)\n"
        "    set(_uimd_local_cpp \"${CMAKE_CURRENT_LIST_DIR}/../uimd/cpp\")\n"
        "    if(EXISTS \"${_uimd_local_cpp}/CMakeLists.txt\")\n"
        "        set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL \"\" FORCE)\n"
        "        set(UIMD_BUILD_TESTS OFF CACHE BOOL \"\" FORCE)\n"
        "        set(UIMD_INSTALL OFF CACHE BOOL \"\" FORCE)\n"
        "        add_subdirectory(\"${_uimd_local_cpp}\" \"${CMAKE_BINARY_DIR}/_deps/uimd-build\")\n"
        "    endif()\n"
        "endif()\n"
        "\n"
        "if(NOT TARGET uimd::runtime)\n"
        "    include(FetchContent)\n"
        "\n"
        "    set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL \"\" FORCE)\n"
        "    set(UIMD_BUILD_TESTS OFF CACHE BOOL \"\" FORCE)\n"
        "    set(UIMD_INSTALL OFF CACHE BOOL \"\" FORCE)\n"
        "\n"
        "    FetchContent_Declare(\n"
        "        uimd\n"
        "        GIT_REPOSITORY https://github.com/uimd-lang/uimd.git\n"
        "        GIT_TAG v" + std::string{UIMD_VERSION} + "\n"
        "    )\n"
        "    FetchContent_MakeAvailable(uimd)\n"
        "endif()\n"
        "\n"
        "if(NOT TARGET uimd::runtime)\n"
        "    message(FATAL_ERROR \"UIMD C++ runtime was not found. Install UIMD, set CMAKE_PREFIX_PATH, keep a sibling ../uimd checkout, or use a released Git tag.\")\n"
        "endif()\n"
        "\n"
        "add_executable(" + project + "\n"
        "    " + project + ".cpp\n"
        "    " + project + "_ui.cpp\n"
        ")\n"
        "\n"
        "target_compile_features(" + project + " PRIVATE cxx_std_20)\n"
        "target_link_libraries(" + project + " PRIVATE uimd::runtime)\n";
}

std::filesystem::path generatedHeaderPath(const std::filesystem::path& sourcePath, const std::filesystem::path& outputDir)
{
    return outputDir / (sourcePath.stem().string() + "_ui.hpp");
}

std::filesystem::path generatedSourcePath(const std::filesystem::path& sourcePath, const std::filesystem::path& outputDir)
{
    return outputDir / (sourcePath.stem().string() + "_ui.cpp");
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

std::vector<std::filesystem::path> compileCppFile(
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
        std::vector<std::filesystem::path> dependencyGenerated = compileCppFile(
            dependency,
            dependency.parent_path(),
            false,
            mcpEnabled,
            visited
        );
        generated.insert(generated.end(), dependencyGenerated.begin(), dependencyGenerated.end());
    }

    const std::string baseName = absoluteSource.stem().string();
    const std::string classNameValue = className(baseName);
    const std::filesystem::path headerPath = generatedHeaderPath(absoluteSource, outputDir);
    const std::filesystem::path sourceOutputPath = generatedSourcePath(absoluteSource, outputDir);
    writeTextFile(headerPath, generateHeader(baseName, classNameValue, model, absoluteSource));
    writeTextFile(sourceOutputPath, generateSource(baseName, classNameValue, model, absoluteSource, outputDir, mcpEnabled));
    generated.push_back(headerPath);
    generated.push_back(sourceOutputPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (baseName + ".cpp");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, generateAppStub(baseName, classNameValue, model));
            generated.push_back(appPath);
        }
    }
    return generated;
}

}  // namespace

std::vector<std::filesystem::path> generateCppSources(
    const std::filesystem::path& sourcePath,
    const NativeCppGenerateOptions& options
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
        std::vector<std::filesystem::path> result = compileCppFile(
            sourceFile,
            outputDir,
            bootstrap,
            options.mcpEnabled,
            visited
        );
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path cmakePath = outputDir / "CMakeLists.txt";
            if (!std::filesystem::exists(cmakePath))
            {
                writeTextFile(cmakePath, cppCMake(sourceFile.stem().string()));
                generated.push_back(cmakePath);
            }
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

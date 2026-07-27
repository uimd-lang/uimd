#include "NativeCSharpGenerator.hpp"

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

std::string csString(const std::string& value)
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
        else if (ch == '\r')
        {
            result += "\\r";
        }
        else if (ch == '\t')
        {
            result += "\\t";
        }
        else
        {
            result += ch;
        }
    }
    result += "\"";
    return result;
}

std::string csStringValue(const YamlValue* value, const std::string& fallback = "")
{
    return csString(value == nullptr ? fallback : valueAsString(*value, fallback));
}

std::string csRawString(const std::string& value)
{
    std::string delimiter = "\"\"\"";
    if (value.empty())
    {
        return "\"\"";
    }
    return delimiter + "\n" + value + "\n" + delimiter;
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

bool isBuiltinType(const std::string& elemType)
{
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

std::string csElementType(const std::string& elemType)
{
    if (elemType == "label" || elemType == "spanlabel")
    {
        return "Label";
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

std::string roleForType(const std::string& typeName)
{
    const std::string elemType = lower(typeName);
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return "input";
    }
    if (elemType == "button" || elemType == "image")
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
    if (std::holds_alternative<YamlMap>(value.value))
    {
        return jsonCompactMap(std::get<YamlMap>(value.value));
    }
    return "null";
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

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeCSharpGenerateOptions& options)
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

std::string listCode(const std::vector<std::string>& values)
{
    if (values.empty())
    {
        return "new string[] {}";
    }
    std::string result = "new[] {";
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += csString(values[index]);
    }
    result += "}";
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

std::string projectReferencePathFor(const std::filesystem::path& projectPath)
{
    const std::filesystem::path root = projectRootFor(projectPath);
    const std::filesystem::path runtimeProject = root.empty()
        ? std::filesystem::path{"csharp"} / "src" / "Uimd" / "Uimd.csproj"
        : root / "csharp" / "src" / "Uimd" / "Uimd.csproj";
    std::error_code error;
    std::filesystem::path relativePath = std::filesystem::relative(runtimeProject, projectPath.parent_path(), error);
    if (!error && !relativePath.empty())
    {
        return relativePath.generic_string();
    }
    return runtimeProject.generic_string();
}

std::string ctorArgs(const std::string& name, const YamlMap& member, int scrollviewGap)
{
    const std::string elemType = typeFor(member);
    if (elemType == "label" || elemType == "spanlabel" || elemType == "infolabel" || elemType == "messagetable")
    {
        return csString(name) + ", " + csString(yamlString(member, "text", name));
    }
    if (elemType == "framebufferview")
    {
        return csString(name);
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
        return csString(name) + ", " +
            csStringValue(yamlGet(member, "source")) + ", " +
            csStringValue(yamlGet(member, "alt")) + ", " +
            csStringValue(yamlGet(member, "fit"), "contain") + ", " +
            csStringValue(renderMode, "auto") + ", " +
            csStringValue(yamlGet(member, "align"), "center") + ", " +
            csStringValue(verticalAlign, "middle");
    }
    if (elemType == "button")
    {
        return csString(name) + ", " + csString(yamlString(member, "title", name));
    }
    if (elemType == "checkbox")
    {
        const YamlValue* checkedValue = yamlGet(member, "checked");
        if (checkedValue == nullptr)
        {
            checkedValue = yamlGet(member, "value");
        }
        return csString(name) + ", " + csString(yamlString(member, "title", name)) + ", " + (truthy(checkedValue, false) ? "true" : "false");
    }
    if (elemType == "textinput" || elemType == "textarea")
    {
        const int maxLength = valueAsInt(yamlGet(member, "maxlength"), 0);
        return csString(name) + ", " + csStringValue(yamlGet(member, "value")) + ", " + std::to_string(maxLength);
    }
    if (elemType == "numberinput")
    {
        return csString(name) + ", " +
            doubleCode(valueAsDouble(yamlGet(member, "value"), 0.0)) + ", " +
            doubleCode(valueAsDouble(yamlGet(member, "step_size") != nullptr ? yamlGet(member, "step_size") : yamlGet(member, "step"), 1.0));
    }
    if (elemType == "combobox" || elemType == "listbox")
    {
        return csString(name) + ", " + listCode(listValue(yamlGet(member, "options")));
    }
    if (elemType == "uiscrollview")
    {
        return csString(name) + ", " + std::to_string(scrollviewGap);
    }
    return csString(name);
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
        {"SetStyle", {}},
        {"SetFocusStyle", {}},
        {"SetEditStyle", {}},
        {"SetCursorStyle", {}},
        {"SetSelectedStyle", {}},
        {"SetActiveStyle", {}},
        {"SetCheckedStyle", {}},
        {"SetUncheckedStyle", {}},
        {"SetDisabledStyle", {}},
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
        std::string setter = "SetStyle";
        if (state == "focus")
        {
            setter = "SetFocusStyle";
        }
        else if (state == "edit")
        {
            setter = "SetEditStyle";
        }
        else if (state == "cursor")
        {
            setter = "SetCursorStyle";
        }
        else if (state == "selected")
        {
            setter = "SetSelectedStyle";
        }
        else if (state == "active")
        {
            setter = "SetActiveStyle";
        }
        else if (state == "checked")
        {
            setter = "SetCheckedStyle";
        }
        else if (state == "unchecked")
        {
            setter = "SetUncheckedStyle";
        }
        else if (state == "disabled")
        {
            setter = "SetDisabledStyle";
        }
        stateForSetter(setter)[stylePropName(key)] = value;
    }
    return states;
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
            assignments["Color"] = "new Color(" + csString(text) + ")";
        }
        else if (key == "background")
        {
            assignments["Background"] = "new Color(" + csString(text) + ")";
        }
        else if (key == "background-texture")
        {
            assignments["BackgroundTexture"] = csString(text);
        }
        else if (key == "background-texture-color")
        {
            assignments["BackgroundTextureColor"] = "new Color(" + csString(text) + ")";
        }
        else if (key == "scope-dim-background")
        {
            assignments["ScopeDimBackground"] = "new Color(" + csString(text) + ")";
        }
        else if (key == "border-color")
        {
            assignments["BorderColor"] = "new Color(" + csString(text) + ")";
        }
        else if (key == "border-width")
        {
            assignments["BorderWidthHorizontal"] = text;
            assignments["BorderWidthVertical"] = text;
        }
        else if (key == "border-width-horizontal")
        {
            assignments["BorderWidthHorizontal"] = text;
        }
        else if (key == "border-width-vertical")
        {
            assignments["BorderWidthVertical"] = text;
        }
        else if (key == "padding")
        {
            assignBoxValues(assignments, "Padding", value);
        }
        else if (key == "margin")
        {
            assignBoxValues(assignments, "Margin", value);
        }
        else if (key == "gap")
        {
            assignments["Gap"] = text;
        }
        else if (key == "text-align")
        {
            assignments["TextAlign"] = csString(text);
        }
        else if (key == "user-select")
        {
            assignments["UserSelect"] = csString(text);
        }
        else if (key == "scroll-x")
        {
            assignments["ScrollX"] = truthy(&value, false) ? "true" : "false";
        }
        else if (key == "scroll-y")
        {
            assignments["ScrollY"] = truthy(&value, false) ? "true" : "false";
        }
    }
    auto textGradientCode = [](const YamlMap& gradient) -> std::string
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
        std::string result = "new TextGradient\n    {\n"
            "        IntervalMs = " + std::to_string(interval) + ",\n"
            "        Step = " + std::to_string(step) + ",\n"
            "        SegmentSize = " + std::to_string(segmentSize) + ",\n"
            "        Colors = {";
        for (std::size_t index = 0; index < colors.size(); ++index)
        {
            if (index != 0)
            {
                result += ", ";
            }
            result += "new Color(" + csString(colors[index]) + ")";
        }
        result += "}\n    }";
        return result;
    };
    if (colorGradient != nullptr)
    {
        assignments["TextColorGradient"] = textGradientCode(*colorGradient);
    }
    if (backgroundGradient != nullptr)
    {
        assignments["TextBackgroundGradient"] = textGradientCode(*backgroundGradient);
    }
    if (assignments.empty())
    {
        return "new Style()";
    }
    std::string result = "new Style\n    {\n";
    std::size_t index = 0;
    for (const auto& [property, expression] : assignments)
    {
        result += "        " + property + " = " + expression + (index + 1 == assignments.size() ? "\n" : ",\n");
        ++index;
    }
    result += "    }";
    return result;
}

std::string dimensionCode(const std::string& mode, int value)
{
    if (mode == "fixed")
    {
        return "AxisDimension.Fixed(" + std::to_string(value) + ")";
    }
    if (mode == "expanded")
    {
        return "AxisDimension.Expanded()";
    }
    if (mode == "fit-content")
    {
        return "AxisDimension.FitContent()";
    }
    return "AxisDimension.Auto(" + std::to_string(value) + ")";
}

std::string rectCode(int row, int col, int width, int height)
{
    return "new Rect(" + std::to_string(row) + ", " + std::to_string(col) + ", " + std::to_string(width) + ", " + std::to_string(height) + ")";
}

std::string sizeCode(int width, int height)
{
    return "new Size(" + std::to_string(width) + ", " + std::to_string(height) + ")";
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
    YamlMap elementStyle = layoutElementStyle(style, data.type, data.name, data.cellName);
    return "            new GeneratedLayoutEntry\n"
        "            {\n"
        "                Name = " + csString(data.name) + ",\n"
        "                Type = " + csString(data.type) + ",\n"
        "                CellName = " + csString(data.cellName) + ",\n"
        "                Relative = " + rectCode(data.relativeRow, data.relativeCol, data.relativeWidth, data.relativeHeight) + ",\n"
        "                SourceCell = " + rectCode(data.sourceRow, data.sourceCol, data.sourceWidth, data.sourceHeight) + ",\n"
        "                Width = " + dimensionCode(data.widthMode, data.widthValue) + ",\n"
        "                Height = " + dimensionCode(data.heightMode, data.heightValue) + ",\n"
        "                CellWidth = " + dimensionCode(data.cellWidthMode, data.cellWidthValue) + ",\n"
        "                CellHeight = " + dimensionCode(data.cellHeightMode, data.cellHeightValue) + ",\n"
        "                CharsSize = " + sizeCode(data.charsWidth, data.charsHeight) + ",\n"
        "                CellCharsSize = " + sizeCode(data.cellCharsWidth, data.cellCharsHeight) + ",\n"
        "                MarginRight = " + std::to_string(data.marginRight) + ",\n"
        "                MarginBottom = " + std::to_string(data.marginBottom) + ",\n"
        "                CellStyle = " + styleCode(cellStyleMap) + ",\n"
        "                ElementStyle = " + styleCode(elementStyle) + ",\n"
        "            },";
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
        if (setter != "SetStyle")
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
        mergedStates.insert(mergedStates.begin(), {"SetStyle", dependencyStyle});
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
        if (setter != "SetStyle" && !stateStyle.empty())
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
        return {{name, "text", eventMethodName(name, "Change"), "string value"}};
    }
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return {
            {name, "text", eventMethodName(name, "Change"), "string value"},
            {name, "confirmed", eventMethodName(name, "Submit"), "string value"},
        };
    }
    if (elemType == "combobox")
    {
        return {{name, "selection_text", eventMethodName(name, "Change"), "string value"}};
    }
    if (elemType == "listbox")
    {
        return {{name, "selection", eventMethodName(name, "SelectionChange"), "List<string> value"}};
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

std::string generateHookDeclarations(const YamlMap& members)
{
    std::vector<std::string> lines;
    for (const EventSpec& spec : eventSpecs(members))
    {
        lines.push_back(spec.argType.empty()
            ? "    protected virtual void " + spec.methodName + "() { }"
            : "    protected virtual void " + spec.methodName + "(" + spec.argType + ") { }");
    }
    lines.push_back("    protected override bool shouldClose() { return false; }");
    std::string result;
    for (const std::string& line : lines)
    {
        result += line + "\n";
    }
    return result;
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
        entries.push_back("            new GeneratedElementMetadata { Name = " + csString(name) +
            ", Description = " + csString(descriptionForMember(name, *member)) +
            ", Expose = " + (truthy(yamlGet(*member, "expose"), true) ? "true" : "false") + " },");
    }
    if (entries.empty())
    {
        return "Array.Empty<GeneratedElementMetadata>()";
    }
    std::string result = "new[]\n        {\n";
    for (const std::string& entry : entries)
    {
        result += entry + "\n";
    }
    result += "        }";
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
        entries.push_back("            new GeneratedAppToolMetadata { Name = " + csString(name) +
            ", Description = " + csString(yamlString(*tool, "description", "")) +
            ", InputSchemaJson = " + csRawString(inputJson) +
            ", OutputSchemaJson = " + csRawString(outputJson) + " },");
    }
    if (entries.empty())
    {
        return "Array.Empty<GeneratedAppToolMetadata>()";
    }
    std::string result = "new[]\n        {\n";
    for (const std::string& entry : entries)
    {
        result += entry + "\n";
    }
    result += "        }";
    return result;
}

std::string windowDescription(const CompilerDocument& model)
{
    return yamlString(model.document.metadata, "description", "");
}

std::string generateSource(
    const std::string& baseName,
    const std::string& classNameValue,
    const CompilerDocument& model,
    bool mcpEnabled
)
{
    const YamlMap& members = model.document.members;
    const YamlMap& style = model.style;
    const bool scrollviewExtension = isScrollviewExtension(model);
    const int scrollviewGap = scrollviewExtensionGap(style);
    const std::string baseClass = scrollviewExtension ? "GeneratedScrollViewBase" : "GeneratedWindowBase";
    const std::string title = model.document.title.empty() ? baseName : model.document.title;

    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
        "using Uimd;",
        "",
        "public class " + classNameValue + " : " + baseClass,
        "{",
    };

    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string elemType = typeFor(*member);
        lines.push_back("    public " + csElementType(elemType) + " " + name + " = null!;");
    }

    lines.push_back("");
    lines.push_back("    public " + classNameValue + "() : base(" + csString(title) + ")");
    lines.push_back("    {");
    lines.push_back("        SetGeneratedLayout(BuildLayout());");
    lines.push_back(std::string{"        SetGeneratedFocusable("} + (model.focusable ? "true" : "false") + ");");
    lines.push_back("        SetGeneratedKind(" + csString(lower(trim(model.kind.empty() ? std::string{"window"} : model.kind))) + ");");
    lines.push_back("        SetMcpMetadata(" + std::string{mcpEnabled ? "true" : "false"} + ", " +
        csString(mcpClassName(baseName)) + ", " + csRawString(mcpEnabled ? model.document.sourceText : std::string{}) +
        ", " + csString(windowDescription(model)) + ", " + mcpElementMetadataCode(members) + ");");
    lines.push_back("        SetMcpAppTools(" + mcpAppToolsCode(model.mcpTools) + ");");
    YamlMap windowStyle = styleWindow(style);
    if (!windowStyle.empty())
    {
        lines.push_back("        SetGeneratedWindowStyle(" + styleCode(windowStyle) + ");");
    }

    if (scrollviewExtension)
    {
        lines.push_back("        SetGeneratedScrollView(AddElement(new ScrollView(" + csString(GENERATED_SCROLLVIEW_ELEMENT_NAME) + ", " + std::to_string(scrollviewGap) + ")));");
        for (const auto& [setter, stateStyle] : scrollviewExtensionStyleStates(style))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        ScrollView()." + setter + "(" + styleCode(stateStyle) + ");");
            }
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
        const std::string csType = csElementType(elemType);
        lines.push_back("        " + name + " = AddElement(new " + csType + "(" + ctorArgs(name, *member, scrollviewGap) + "));");
        const YamlValue* commitMode = yamlGet(*member, "commit-mode");
        if (commitMode == nullptr)
        {
            commitMode = yamlGet(*member, "commit_mode");
        }
        if (commitMode != nullptr)
        {
            lines.push_back("        " + name + ".SetCommitMode(" + csString(valueAsString(*commitMode)) + ");");
        }
        if (elemType == "combobox")
        {
            const int index = selectedIndex(*member, "selected_item");
            if (index >= 0)
            {
                lines.push_back("        " + name + ".SetSelectedIndex(" + std::to_string(index) + ");");
            }
        }
        if (elemType == "listbox")
        {
            if (truthy(yamlGet(*member, "multiple"), false))
            {
                lines.push_back("        " + name + ".SetMultiple(true);");
            }
            std::vector<std::string> values = selectedValues(*member, "selected_items");
            if (values.size() > 1)
            {
                lines.push_back("        " + name + ".SetSelectedValues(" + listCode(values) + ");");
            }
            else
            {
                const int index = selectedIndex(*member, "selected_items");
                if (index >= 0)
                {
                    lines.push_back("        " + name + ".SetSelectedIndex(" + std::to_string(index) + ");");
                }
            }
        }
        if (!isBuiltinType(elemType))
        {
            lines.push_back("        " + name + ".SetChild(new " + customClassName(*member, elemType) + "());");
        }

        const std::string cellName = cellNameForElement(model.document.layout, name);
        std::string styleElemType = elemType == "textarea" ? "textinput" : (!isBuiltinType(elemType) ? "uielement" : elemType);
        for (const auto& [setter, stateStyle] : styleStatesForElement(style, styleElemType, name, cellName, member))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        " + name + "." + setter + "(" + styleCode(stateStyle) + ");");
            }
        }
    }
    lines.push_back("    }");
    lines.push_back("");

    lines.push_back("    private static List<GeneratedLayoutEntry> BuildLayout()");
    lines.push_back("    {");
    lines.push_back("        return new List<GeneratedLayoutEntry>");
    lines.push_back("        {");
    for (const NativeLayoutItem& cell : model.document.layout)
    {
        lines.push_back(layoutEntry(cell, members, style, scrollviewExtension));
    }
    lines.push_back("        };");
    lines.push_back("    }");
    lines.push_back("");

    lines.push_back(generateHookDeclarations(members));

    auto appendDispatch = [&](const std::string& methodName, const std::string& signature, const std::vector<EventSpec>& specs, const std::string& callSuffix) {
        lines.push_back("    public override bool " + methodName + "(" + signature + ")");
        lines.push_back("    {");
        if (!specs.empty())
        {
            for (std::size_t index = 0; index < specs.size(); ++index)
            {
                const std::string keyword = index == 0 ? "if" : "else if";
                lines.push_back("        " + keyword + " (name == " + csString(specs[index].name) + ")");
                lines.push_back("        {");
                lines.push_back("            " + specs[index].methodName + callSuffix + ";");
                lines.push_back("            return true;");
                lines.push_back("        }");
            }
        }
        lines.push_back("        return false;");
        lines.push_back("    }");
        lines.push_back("");
    };

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

    appendDispatch("HandleGeneratedButton", "string name", buttonSpecs, "()");
    appendDispatch("HandleGeneratedTextChanged", "string name, string value", textSpecs, "(value)");
    appendDispatch("HandleGeneratedTextConfirmed", "string name, string value", confirmedSpecs, "(value)");

    lines.push_back("    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)");
    lines.push_back("    {");
    std::size_t branchIndex = 0;
    for (const EventSpec& spec : selectionTextSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("        " + keyword + " (name == " + csString(spec.name) + ")");
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value.Count == 0 ? \"\" : value[0]);");
        lines.push_back("            return true;");
        lines.push_back("        }");
        ++branchIndex;
    }
    for (const EventSpec& spec : selectionSpecs)
    {
        const std::string keyword = branchIndex == 0 ? "if" : "else if";
        lines.push_back("        " + keyword + " (name == " + csString(spec.name) + ")");
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value);");
        lines.push_back("            return true;");
        lines.push_back("        }");
        ++branchIndex;
    }
    lines.push_back("        return false;");
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

std::string generateAppStub(const std::string& baseName, const std::string& classNameValue)
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
    return "using Uimd;\n"
        "\n"
        "public class " + appClassName + " : " + classNameValue + "\n"
        "{\n"
        "    // Override generated event hooks here.\n"
        "}\n"
        "\n"
        "public static class Program\n"
        "{\n"
        "    public static int Main(string[] args)\n"
        "    {\n"
        "        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>\n"
        "        {\n"
        "            " + appClassName + " app = new();\n"
        "            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);\n"
        "        });\n"
        "    }\n"
        "}\n";
}

std::string csproj(const std::string& project, const std::filesystem::path& projectPath)
{
    const std::string runtimeReference = projectReferencePathFor(projectPath);
    return "<Project Sdk=\"Microsoft.NET.Sdk\">\n"
        "  <PropertyGroup>\n"
        "    <OutputType>Exe</OutputType>\n"
        "    <TargetFramework>net10.0</TargetFramework>\n"
        "    <ImplicitUsings>enable</ImplicitUsings>\n"
        "    <Nullable>enable</Nullable>\n"
        "    <LangVersion>preview</LangVersion>\n"
        "    <AssemblyName>" + project + "</AssemblyName>\n"
        "  </PropertyGroup>\n"
        + csharpRuntimeReferenceProperties(runtimeReference) +
        "</Project>\n";
}

std::filesystem::path generatedSourcePath(const std::filesystem::path& sourcePath, const std::filesystem::path& outputDir)
{
    return outputDir / (sourcePath.stem().string() + "_ui.cs");
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

std::vector<std::filesystem::path> compileCSharpFile(
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
        std::vector<std::filesystem::path> dependencyGenerated = compileCSharpFile(
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
    const std::filesystem::path sourceOutputPath = generatedSourcePath(absoluteSource, outputDir);
    writeTextFile(sourceOutputPath, generateSource(baseName, classNameValue, model, mcpEnabled));
    generated.push_back(sourceOutputPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (baseName + ".cs");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, generateAppStub(baseName, classNameValue));
            generated.push_back(appPath);
        }
    }
    return generated;
}

}  // namespace

namespace
{

std::string csharpXmlEscape(const std::string& text)
{
    std::string escaped;
    for (char ch : text)
    {
        if (ch == '&')
        {
            escaped += "&amp;";
        }
        else if (ch == '"')
        {
            escaped += "&quot;";
        }
        else if (ch == '<')
        {
            escaped += "&lt;";
        }
        else if (ch == '>')
        {
            escaped += "&gt;";
        }
        else
        {
            escaped += ch;
        }
    }
    return escaped;
}

}  // namespace

std::string csharpRuntimeReferenceProperties(const std::string& localRuntimeReference)
{
    return
        "  <PropertyGroup>\n"
        "    <UimdLocalCSharpProject>" + csharpXmlEscape(localRuntimeReference) + "</UimdLocalCSharpProject>\n"
        "    <UimdSdkHome Condition=\"'$(UimdSdkHome)' == '' and '$(UIMD_HOME)' != ''\">$(UIMD_HOME)</UimdSdkHome>\n"
        "    <UimdSdkHome Condition=\"'$(UimdSdkHome)' == '' and '$(LOCALAPPDATA)' != ''\">$([System.IO.Path]::Combine('$(LOCALAPPDATA)', 'uimd'))</UimdSdkHome>\n"
        "    <UimdSdkHome Condition=\"'$(UimdSdkHome)' == '' and '$(HOME)' != ''\">$([System.IO.Path]::Combine('$(HOME)', '.uimd'))</UimdSdkHome>\n"
        "    <UimdCurrentFile Condition=\"'$(UimdSdkHome)' != ''\">$([System.IO.Path]::Combine('$(UimdSdkHome)', 'current'))</UimdCurrentFile>\n"
        "    <UimdCurrentVersion Condition=\"'$(UimdCurrentVersion)' == '' and Exists('$(UimdCurrentFile)')\">$([System.IO.File]::ReadAllText('$(UimdCurrentFile)').Trim())</UimdCurrentVersion>\n"
        "    <UimdInstalledCSharpProject Condition=\"'$(UimdCurrentVersion)' != ''\">$([System.IO.Path]::Combine('$(UimdSdkHome)', 'sdk', '$(UimdCurrentVersion)', 'targets', 'csharp', 'Uimd.csproj'))</UimdInstalledCSharpProject>\n"
        "  </PropertyGroup>\n"
        "  <ItemGroup Condition=\"Exists('$(UimdLocalCSharpProject)')\">\n"
        "    <ProjectReference Include=\"$(UimdLocalCSharpProject)\" />\n"
        "  </ItemGroup>\n"
        "  <ItemGroup Condition=\"!Exists('$(UimdLocalCSharpProject)') and Exists('$(UimdInstalledCSharpProject)')\">\n"
        "    <ProjectReference Include=\"$(UimdInstalledCSharpProject)\" />\n"
        "  </ItemGroup>\n"
        "  <Target Name=\"EnsureUimdCSharpRuntime\" BeforeTargets=\"ResolveReferences\">\n"
        "    <Error Condition=\"!Exists('$(UimdLocalCSharpProject)') and !Exists('$(UimdInstalledCSharpProject)')\" Text=\"UIMD C# runtime was not found. Install UIMD, set UIMD_HOME, keep a sibling ../uimd checkout, or set UimdLocalCSharpProject.\" />\n"
        "  </Target>\n";
}

std::vector<std::filesystem::path> generateCSharpSources(
    const std::filesystem::path& sourcePath,
    const NativeCSharpGenerateOptions& options
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
        std::vector<std::filesystem::path> result = compileCSharpFile(
            sourceFile,
            outputDir,
            bootstrap,
            options.mcpEnabled,
            visited
        );
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path projectPath = outputDir / (sourceFile.stem().string() + ".csproj");
            if (!std::filesystem::exists(projectPath))
            {
                writeTextFile(projectPath, csproj(sourceFile.stem().string(), projectPath));
                generated.push_back(projectPath);
            }
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

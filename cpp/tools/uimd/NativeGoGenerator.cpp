#include "NativeGoGenerator.hpp"

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
constexpr int textGradientDefaultIntervalMs = 70;
constexpr int textGradientDefaultStep = 1;
constexpr int textGradientDefaultSegmentSize = 1;

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
    const YamlValue* value = map.get(key);
    return value == nullptr ? fallback : valueAsString(*value, fallback);
}

YamlValue makeMap(YamlMap value)
{
    return YamlValue{std::move(value)};
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
    return std::holds_alternative<YamlMap>(value->value) && !std::get<YamlMap>(value->value).empty();
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

std::string goString(const std::string& value)
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
            escape << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
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

std::string goPackageName(const std::filesystem::path& directory)
{
    std::string raw = lower(directory.filename().string());
    std::string result;
    for (unsigned char ch : raw)
    {
        if (std::isalnum(ch) || ch == '_')
        {
            result.push_back(static_cast<char>(ch));
        }
        else
        {
            result.push_back('_');
        }
    }
    if (result.empty() || std::isdigit(static_cast<unsigned char>(result.front())))
    {
        result = "pkg_" + result;
    }
    return result;
}

std::string goIdentifier(std::string value)
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
    for (char ch : value)
    {
        if (ch == '_' || ch == '-' || std::isspace(static_cast<unsigned char>(ch)))
        {
            flush();
        }
        else if (std::isalnum(static_cast<unsigned char>(ch)))
        {
            current += ch;
        }
        else
        {
            flush();
        }
    }
    flush();
    if (result.empty() || std::isdigit(static_cast<unsigned char>(result.front())))
    {
        result = "Uimd" + result;
    }
    return result;
}

std::string className(const std::string& baseName)
{
    return goIdentifier(baseName) + "UI";
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

std::string goElementType(const std::string& elemType)
{
    if (elemType == "spanlabel")
    {
        return "*uimd.SpanLabel";
    }
    if (elemType == "infolabel")
    {
        return "*uimd.InfoLabel";
    }
    if (elemType == "framebufferview")
    {
        return "*uimd.FrameBufferView";
    }
    if (elemType == "messagetable")
    {
        return "*uimd.MessageTable";
    }
    if (elemType == "image")
    {
        return "*uimd.Image";
    }
    if (elemType == "button")
    {
        return "*uimd.Button";
    }
    if (elemType == "checkbox")
    {
        return "*uimd.CheckBox";
    }
    if (elemType == "textinput")
    {
        return "*uimd.TextInput";
    }
    if (elemType == "textarea")
    {
        return "*uimd.TextArea";
    }
    if (elemType == "numberinput")
    {
        return "*uimd.NumberInput";
    }
    if (elemType == "combobox")
    {
        return "*uimd.ComboBox";
    }
    if (elemType == "listbox")
    {
        return "*uimd.ListBox";
    }
    if (elemType == "uiscrollview")
    {
        return "*uimd.ScrollView";
    }
    if (elemType == "viewhost")
    {
        return "*uimd.ViewHost";
    }
    if (elemType == "label")
    {
        return "*uimd.Label";
    }
    return "*uimd.ReusableElement";
}

bool isBuiltinType(const std::string& elemType)
{
    return BUILTIN_TYPES.find(elemType) != BUILTIN_TYPES.end();
}

std::string listCode(const YamlValue* value)
{
    const YamlList* list = valueAsList(value);
    if (list == nullptr)
    {
        return "nil";
    }
    std::string result = "[]string{";
    for (std::size_t index = 0; index < list->size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += goString(valueAsString((*list)[index]));
    }
    result += "}";
    return result;
}

std::string listCode(const std::vector<std::string>& values)
{
    std::string result = "[]string{";
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += goString(values[index]);
    }
    result += "}";
    return result;
}

std::vector<std::string> stringList(const YamlValue* value)
{
    std::vector<std::string> result;
    const YamlList* list = valueAsList(value);
    if (list == nullptr)
    {
        if (value != nullptr)
        {
            const std::string single = valueAsString(*value);
            if (!single.empty())
            {
                result.push_back(single);
            }
        }
        return result;
    }
    for (const YamlValue& item : *list)
    {
        result.push_back(valueAsString(item));
    }
    return result;
}

std::vector<std::string> selectedValues(const YamlMap& member, const std::string& key)
{
    return stringList(member.get(key));
}

int selectedIndex(const YamlMap& member, const std::string& key)
{
    const std::vector<std::string> options = stringList(member.get("options"));
    const std::vector<std::string> values = selectedValues(member, key);
    if (values.empty())
    {
        return -1;
    }
    const std::string selected = values.front();
    for (std::size_t index = 0; index < options.size(); ++index)
    {
        if (options[index] == selected)
        {
            return static_cast<int>(index);
        }
    }
    return -1;
}

std::string ctorCode(const std::string& name, const YamlMap& member)
{
    const std::string elemType = typeFor(member);
    if (elemType == "spanlabel")
    {
        return "uimd.NewSpanLabel(" + goString(name) + ", " + goString(yamlString(member, "text", name)) + ")";
    }
    if (elemType == "infolabel")
    {
        return "uimd.NewInfoLabel(" + goString(name) + ", " + goString(yamlString(member, "text", name)) + ")";
    }
    if (elemType == "framebufferview")
    {
        return "uimd.NewFrameBufferView(" + goString(name) + ")";
    }
    if (elemType == "messagetable")
    {
        return "uimd.NewMessageTable(" + goString(name) + ")";
    }
    if (elemType == "image")
    {
        return "uimd.NewImage(" + goString(name) +
            ", " + goString(yamlString(member, "source")) +
            ", " + goString(yamlString(member, "alt")) +
            ", " + goString(yamlString(member, "fit", "contain")) +
            ", " + goString(yamlString(member, "render_mode", "auto")) +
            ", " + goString(yamlString(member, "align", "center")) +
            ", " + goString(yamlString(member, "valign", "middle")) + ")";
    }
    if (elemType == "button")
    {
        return "uimd.NewButton(" + goString(name) + ", " + goString(yamlString(member, "title", name)) + ")";
    }
    if (elemType == "checkbox")
    {
        const YamlValue* checked = member.get("checked");
        if (checked == nullptr)
        {
            checked = member.get("value");
        }
        return "uimd.NewCheckBox(" + goString(name) + ", " + goString(yamlString(member, "title", name)) +
            ", " + std::string{truthy(checked, false) ? "true" : "false"} + ")";
    }
    if (elemType == "textinput")
    {
        return "uimd.NewTextInput(" + goString(name) + ", " + goString(yamlString(member, "value")) +
            ", " + std::to_string(valueAsInt(member.get("max-length"), 0)) + ")";
    }
    if (elemType == "textarea")
    {
        return "uimd.NewTextArea(" + goString(name) + ", " + goString(yamlString(member, "value")) + ")";
    }
    if (elemType == "numberinput")
    {
        return "uimd.NewNumberInput(" + goString(name) + ", " + std::to_string(valueAsDouble(member.get("value"), 0.0)) + ")";
    }
    if (elemType == "combobox")
    {
        return "uimd.NewComboBox(" + goString(name) + ", " + listCode(member.get("options")) + ")";
    }
    if (elemType == "listbox")
    {
        return "uimd.NewListBox(" + goString(name) + ", " + listCode(member.get("options")) +
            ", " + std::string{(truthy(member.get("multiple"), false) || truthy(member.get("multi"), false)) ? "true" : "false"} + ")";
    }
    if (elemType == "uiscrollview")
    {
        return "uimd.NewScrollView(" + goString(name) + ", " + std::to_string(valueAsInt(member.get("gap"), 0)) + ")";
    }
    if (elemType == "viewhost")
    {
        return "uimd.NewViewHost(" + goString(name) + ")";
    }
    if (elemType == "label")
    {
        return "uimd.NewLabel(" + goString(name) + ", " + goString(yamlString(member, "text", name)) + ")";
    }
    return "uimd.NewReusableElement(" + goString(name) + ", " + goString(elemType) + ")";
}

std::string windowTitle(const NativeDocument& document, const std::string& baseName)
{
    return document.title.empty() ? baseName : document.title;
}

std::string extendsKind(const NativeDocument& document)
{
    const std::string extends = lower(trim(yamlString(document.definition, "extends", DEFAULT_EXTENDS)));
    return extends.empty() ? DEFAULT_EXTENDS : extends;
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

std::string layoutItemType(const NativeLayoutItem& item, const YamlMap& members)
{
    const YamlValue* memberValue = members.get(item.content);
    const YamlMap* member = valueAsMap(memberValue);
    if (member != nullptr)
    {
        return typeFor(*member);
    }
    if (!item.content.empty())
    {
        return "label";
    }
    return "";
}

YamlMap cellStyle(const YamlMap& style, const std::string& cellName);
YamlMap layoutElementStyle(const YamlMap& style, const std::string& elemType, const std::string& name, const std::string& cellName);
YamlMap styleWindow(const YamlMap& style);
std::string goStyleCode(const YamlMap& style);
std::vector<std::pair<std::string, YamlMap>> styleStatesForElement(
    const YamlMap& style,
    const std::string& elemType,
    const std::string& name,
    const std::string& cellName,
    const YamlMap* member
);
std::vector<std::pair<std::string, YamlMap>> scrollviewExtensionStyleStates(const YamlMap& style);
YamlMap scrollviewExtensionDescendantFocusStyle(const YamlMap& style);

std::string generateLayout(const NativeDocument& document, const YamlMap& style)
{
    std::string result = "[]uimd.LayoutItem{\n";
    auto layoutInt = [](int value) {
        if (value == GRID_EXPANDED)
        {
            return std::string{"uimd.Expanded"};
        }
        if (value == GRID_FIT_CONTENT)
        {
            return std::string{"uimd.FitContent"};
        }
        return std::to_string(value);
    };
    for (const NativeLayoutItem& item : document.layout)
    {
        const std::string itemType = layoutItemType(item, document.members);
        const YamlMap itemCellStyle = cellStyle(style, item.cellName);
        const YamlMap itemElementStyle = layoutElementStyle(style, itemType, item.content, item.cellName);
        result += "        {\n";
        result += "            Row: " + layoutInt(item.row) + ", Col: " + layoutInt(item.col) + ",\n";
        result += "            CellRow: " + layoutInt(item.cellRow) + ", CellCol: " + layoutInt(item.cellCol) + ",\n";
        result += "            CellCharsWidth: " + layoutInt(item.cellCharsWidth) + ", CellCharsHeight: " + layoutInt(item.cellCharsHeight) + ",\n";
        result += "            CellName: " + goString(item.cellName) + ", CellWidth: " + layoutInt(item.cellWidth) + ", CellHeight: " + layoutInt(item.cellHeight) + ",\n";
        result += "            CellWidthMode: " + goString(item.cellWidthMode) + ", CellHeightMode: " + goString(item.cellHeightMode) + ",\n";
        result += "            Width: " + layoutInt(item.width) + ", Height: " + layoutInt(item.height) + ",\n";
        result += "            WidthMode: " + goString(item.widthMode) + ", HeightMode: " + goString(item.heightMode) + ",\n";
        result += "            CharsWidth: " + layoutInt(item.charsWidth) + ", CharsHeight: " + layoutInt(item.charsHeight) + ",\n";
        result += "            MarginRight: " + layoutInt(item.marginRight) + ", MarginBottom: " + layoutInt(item.marginBottom) + ",\n";
        result += "            Content: " + goString(item.content) + ",\n";
        result += "            CellStyle: " + goStyleCode(itemCellStyle) + ",\n";
        result += "            ElementStyle: " + goStyleCode(itemElementStyle) + ",\n";
        result += "        },\n";
    }
    result += "    }";
    return result;
}

struct EventSpec
{
    std::string name;
    std::string channel;
    std::string methodName;
};

std::string eventMethodName(const std::string& name, const std::string& suffix)
{
    return "On" + goIdentifier(name) + suffix;
}

std::vector<EventSpec> eventSpecsForMember(const std::string& name, const YamlMap& member)
{
    const std::string elemType = typeFor(member);
    if (elemType == "button" || elemType == "image")
    {
        return {{name, "button", eventMethodName(name, "Click")}};
    }
    if (elemType == "checkbox")
    {
        return {{name, "text", eventMethodName(name, "Change")}};
    }
    if (elemType == "textinput" || elemType == "textarea" || elemType == "numberinput")
    {
        return {
            {name, "text", eventMethodName(name, "Change")},
            {name, "confirmed", eventMethodName(name, "Submit")},
        };
    }
    if (elemType == "combobox")
    {
        return {{name, "selection_text", eventMethodName(name, "Change")}};
    }
    if (elemType == "listbox")
    {
        return {{name, "selection", eventMethodName(name, "SelectionChange")}};
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
            if (seen.insert({spec.methodName, spec.channel}).second)
            {
                specs.push_back(spec);
            }
        }
    }
    return specs;
}

std::vector<EventSpec> specsForChannel(const YamlMap& members, const std::string& channel)
{
    std::vector<EventSpec> result;
    for (const EventSpec& spec : eventSpecs(members))
    {
        if (spec.channel == channel)
        {
            result.push_back(spec);
        }
    }
    return result;
}

std::string generateSource(
    const std::string& baseName,
    const std::string& classNameValue,
    const NativeDocument& document,
    const YamlMap& style,
    bool mcpEnabled,
    const std::string& packageName)
{
    const YamlMap& members = document.members;
    const std::string title = windowTitle(document, baseName);
    const std::string baseConstructor = lower(extendsKind(document)) == "uiscrollview" ? "uimd.NewGeneratedScrollViewBase" : "uimd.NewGeneratedWindowBase";

    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
        "package " + packageName,
        "",
        "import uimd \"uimd\"",
        "",
        "type " + classNameValue + " struct {",
        "    *uimd.GeneratedWindowBase",
        "    eventHandler any",
    };

    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        lines.push_back("    " + goIdentifier(name) + " " + goElementType(typeFor(*member)));
    }

    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func New" + classNameValue + "() *" + classNameValue + " {");
    lines.push_back("    ui := &" + classNameValue + "{GeneratedWindowBase: " + baseConstructor + "(" + goString(title) + ")}");
    lines.push_back("    ui.SetGeneratedLayout(" + generateLayout(document, style) + ")");
    lines.push_back("    ui.SetGeneratedKind(" + goString(yamlString(document.metadata, "kind", "window")) + ")");
    lines.push_back("    ui.SetGeneratedFocusable(" + std::string{truthy(document.definition.get("focusable"), false) ? "true" : "false"} + ")");
    const YamlMap windowStyle = styleWindow(style);
    if (!windowStyle.empty())
    {
        lines.push_back("    ui.SetGeneratedWindowStyle(" + goStyleCode(windowStyle) + ")");
    }
    if (lower(extendsKind(document)) == "uiscrollview")
    {
        for (const auto& [setter, stateStyle] : scrollviewExtensionStyleStates(style))
        {
            if (stateStyle.empty())
            {
                continue;
            }
            if (setter == "SetStyle")
            {
                lines.push_back("    ui.SetGeneratedScrollViewStyle(" + goStyleCode(stateStyle) + ")");
            }
            else if (setter == "SetFocusStyle")
            {
                lines.push_back("    ui.SetGeneratedScrollViewFocusStyle(" + goStyleCode(stateStyle) + ")");
            }
            else if (setter == "SetEditStyle")
            {
                lines.push_back("    ui.SetGeneratedScrollViewEditStyle(" + goStyleCode(stateStyle) + ")");
            }
            else if (setter == "SetSelectedStyle")
            {
                lines.push_back("    ui.SetGeneratedScrollViewSelectedStyle(" + goStyleCode(stateStyle) + ")");
            }
        }
        const YamlMap descendantFocusStyle = scrollviewExtensionDescendantFocusStyle(style);
        if (!descendantFocusStyle.empty())
        {
            lines.push_back("    ui.SetGeneratedScrollViewDescendantFocusStyle(" + goStyleCode(descendantFocusStyle) + ")");
        }
    }
    lines.push_back("    ui.SetMCPMetadata(" + std::string{mcpEnabled ? "true" : "false"} +
        ", " + goString(classNameValue.substr(0, classNameValue.size() >= 2 ? classNameValue.size() - 2 : classNameValue.size())) +
        ", " + goString(mcpEnabled ? document.sourceText : std::string{}) +
        ", " + goString(yamlString(document.metadata, "description")) + ")");

    for (const auto& [name, value] : members)
    {
        const YamlMap* member = valueAsMap(&value);
        if (member == nullptr)
        {
            continue;
        }
        const std::string field = goIdentifier(name);
        lines.push_back("    ui." + field + " = " + ctorCode(name, *member));
        lines.push_back("    ui.AddElement(ui." + field + ")");
        const std::string elemType = typeFor(*member);
        if (elemType == "combobox")
        {
            const int index = selectedIndex(*member, "selected_item");
            if (index >= 0)
            {
                lines.push_back("    ui." + field + ".SetSelectedIndex(" + std::to_string(index) + ")");
            }
        }
        if (elemType == "listbox")
        {
            const std::vector<std::string> values = selectedValues(*member, "selected_items");
            if (values.size() > 1)
            {
                lines.push_back("    ui." + field + ".SetSelectedItems(" + listCode(values) + ")");
            }
            else
            {
                const int index = selectedIndex(*member, "selected_items");
                if (index >= 0)
                {
                    lines.push_back("    ui." + field + ".SetSelectedIndex(" + std::to_string(index) + ")");
                }
            }
        }
        const std::string cellName = cellNameForElement(document.layout, name);
        std::string styleElemType;
        if (elemType == "textarea")
        {
            styleElemType = "textinput";
        }
        else if (!isBuiltinType(elemType) || elemType == "uiscrollview")
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
                lines.push_back("    ui." + field + "." + setter + "(" + goStyleCode(stateStyle) + ")");
            }
        }
    }

    lines.push_back("    return ui");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func (ui *" + classNameValue + ") SetEventHandler(handler any) {");
    lines.push_back("    ui.eventHandler = handler");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func (ui *" + classNameValue + ") HandleGeneratedButton(name string) bool {");
    for (const EventSpec& spec : specsForChannel(members, "button"))
    {
        lines.push_back("    if name == " + goString(spec.name) + " {");
        lines.push_back("        if handler, ok := ui.eventHandler.(interface{ " + spec.methodName + "() }); ok {");
        lines.push_back("            handler." + spec.methodName + "()");
        lines.push_back("            return true");
        lines.push_back("        }");
        lines.push_back("        return false");
        lines.push_back("    }");
    }
    lines.push_back("    return false");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func (ui *" + classNameValue + ") HandleGeneratedTextChanged(name string, value string) bool {");
    for (const EventSpec& spec : specsForChannel(members, "text"))
    {
        lines.push_back("    if name == " + goString(spec.name) + " {");
        lines.push_back("        if handler, ok := ui.eventHandler.(interface{ " + spec.methodName + "(string) }); ok {");
        lines.push_back("            handler." + spec.methodName + "(value)");
        lines.push_back("            return true");
        lines.push_back("        }");
        lines.push_back("        return false");
        lines.push_back("    }");
    }
    lines.push_back("    return false");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func (ui *" + classNameValue + ") HandleGeneratedTextConfirmed(name string, value string) bool {");
    for (const EventSpec& spec : specsForChannel(members, "confirmed"))
    {
        lines.push_back("    if name == " + goString(spec.name) + " {");
        lines.push_back("        if handler, ok := ui.eventHandler.(interface{ " + spec.methodName + "(string) }); ok {");
        lines.push_back("            handler." + spec.methodName + "(value)");
        lines.push_back("            return true");
        lines.push_back("        }");
        lines.push_back("        return false");
        lines.push_back("    }");
    }
    lines.push_back("    return false");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("func (ui *" + classNameValue + ") HandleGeneratedSelectionChanged(name string, value []string) bool {");
    for (const EventSpec& spec : specsForChannel(members, "selection_text"))
    {
        lines.push_back("    if name == " + goString(spec.name) + " {");
        lines.push_back("        if handler, ok := ui.eventHandler.(interface{ " + spec.methodName + "(string) }); ok {");
        lines.push_back("            selected := \"\"");
        lines.push_back("            if len(value) > 0 {");
        lines.push_back("                selected = value[0]");
        lines.push_back("            }");
        lines.push_back("            handler." + spec.methodName + "(selected)");
        lines.push_back("            return true");
        lines.push_back("        }");
        lines.push_back("        return false");
        lines.push_back("    }");
    }
    for (const EventSpec& spec : specsForChannel(members, "selection"))
    {
        lines.push_back("    if name == " + goString(spec.name) + " {");
        lines.push_back("        if handler, ok := ui.eventHandler.(interface{ " + spec.methodName + "([]string) }); ok {");
        lines.push_back("            handler." + spec.methodName + "(value)");
        lines.push_back("            return true");
        lines.push_back("        }");
        lines.push_back("        return false");
        lines.push_back("    }");
    }
    lines.push_back("    return false");
    lines.push_back("}");

    std::string result;
    for (std::size_t index = 0; index < lines.size(); ++index)
    {
        if (index != 0)
        {
            result += "\n";
        }
        result += lines[index];
    }
    result += "\n";
    return result;
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

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeGoGenerateOptions& options)
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
        else if (!prefix.empty())
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
        const YamlMap* overrideProps = valueAsMap(override.get(selector));
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
            YamlMap dark = loadIncludedStyle("dark", sourcePath);
            if (!dark.empty())
            {
                return mergeStyles(dark, local);
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
    const YamlMap* props = valueAsMap(style.get(selector));
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
    const YamlMap* props = valueAsMap(style.get("this"));
    if (props == nullptr)
    {
        return {};
    }
    return *props;
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
    const std::string& cellName,
    const YamlMap* member
)
{
    YamlMap merged = styleForElement(style, elemType, name, cellName);
    for (const auto& [key, value] : inlineMemberStyle(member))
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
    if (style.get("@viewport") != nullptr || style.get("viewport") != nullptr)
    {
        return "viewport";
    }
    return "panel";
}

YamlMap scrollviewExtensionPanelStyle(const YamlMap& style)
{
    for (const std::string& selector : {"@viewport", "viewport", "@panel", "panel"})
    {
        const YamlMap* panelStyle = valueAsMap(style.get(selector));
        if (panelStyle != nullptr && !panelStyle->empty())
        {
            return *panelStyle;
        }
    }
    return {};
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

YamlMap scrollviewExtensionDescendantFocusStyle(const YamlMap& style)
{
    for (const auto& [setter, stateStyle] : styleStatesForElement(style, "this", "", "", nullptr))
    {
        if (setter == "SetFocusStyle" && !stateStyle.empty())
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
        assignments[prefix] = "uimd.Int(" + std::to_string(values[0]) + ")";
    }
    else if (values.size() == 2)
    {
        assignments[prefix + "Top"] = "uimd.Int(" + std::to_string(values[0]) + ")";
        assignments[prefix + "Right"] = "uimd.Int(" + std::to_string(values[1]) + ")";
        assignments[prefix + "Bottom"] = "uimd.Int(" + std::to_string(values[0]) + ")";
        assignments[prefix + "Left"] = "uimd.Int(" + std::to_string(values[1]) + ")";
    }
    else if (values.size() >= 4)
    {
        assignments[prefix + "Top"] = "uimd.Int(" + std::to_string(values[0]) + ")";
        assignments[prefix + "Right"] = "uimd.Int(" + std::to_string(values[1]) + ")";
        assignments[prefix + "Bottom"] = "uimd.Int(" + std::to_string(values[2]) + ")";
        assignments[prefix + "Left"] = "uimd.Int(" + std::to_string(values[3]) + ")";
    }
}

std::string textGradientCode(const YamlMap& gradient)
{
    const std::vector<std::string> colors = stringList(gradient.get("colors"));
    const int interval = valueAsInt(gradient.get("interval"), textGradientDefaultIntervalMs);
    const int step = valueAsInt(gradient.get("step"), textGradientDefaultStep);
    const int segmentSize = valueAsInt(
        gradient.get("segment-size") != nullptr ? gradient.get("segment-size") : gradient.get("segment_size"),
        textGradientDefaultSegmentSize);
    std::string colorCode;
    for (std::size_t index = 0; index < colors.size(); ++index)
    {
        if (index != 0)
        {
            colorCode += ", ";
        }
        colorCode += "uimd.NewColor(" + goString(colors[index]) + ")";
    }
    return "&uimd.TextGradient{IntervalMs: " + std::to_string(interval) +
        ", Step: " + std::to_string(step) +
        ", SegmentSize: " + std::to_string(segmentSize) +
        ", Colors: []uimd.Color{" + colorCode + "}}";
}

std::string goStyleCode(const YamlMap& style)
{
    std::map<std::string, std::string> assignments;
    for (const auto& [key, value] : style)
    {
        const std::string text = valueAsString(value);
        if (key == "color")
        {
            assignments["Color"] = "uimd.NewColor(" + goString(text) + ")";
        }
        else if (key == "background")
        {
            assignments["Background"] = "uimd.NewColor(" + goString(text) + ")";
        }
        else if (key == "background-texture")
        {
            assignments["BackgroundTexture"] = goString(text);
        }
        else if (key == "background-texture-color")
        {
            assignments["BackgroundTextureColor"] = "uimd.NewColor(" + goString(text) + ")";
        }
        else if (key == "scope-dim-background")
        {
            assignments["ScopeDimBackground"] = "uimd.NewColor(" + goString(text) + ")";
        }
        else if (key == "border-color")
        {
            assignments["BorderColor"] = "uimd.NewColor(" + goString(text) + ")";
        }
        else if (key == "border-width")
        {
            const std::string code = "uimd.Int(" + std::to_string(valueAsInt(&value, 0)) + ")";
            assignments["BorderWidthHorizontal"] = code;
            assignments["BorderWidthVertical"] = code;
        }
        else if (key == "border-width-horizontal")
        {
            assignments["BorderWidthHorizontal"] = "uimd.Int(" + std::to_string(valueAsInt(&value, 0)) + ")";
        }
        else if (key == "border-width-vertical")
        {
            assignments["BorderWidthVertical"] = "uimd.Int(" + std::to_string(valueAsInt(&value, 0)) + ")";
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
            assignments["Gap"] = "uimd.Int(" + std::to_string(valueAsInt(&value, 0)) + ")";
        }
        else if (key == "text-align")
        {
            assignments["TextAlign"] = goString(text);
        }
        else if (key == "user-select")
        {
            assignments["UserSelect"] = goString(text);
        }
        else if (key == "scroll-x")
        {
            assignments["ScrollX"] = std::string("uimd.Bool(") + (truthy(&value, false) ? "true" : "false") + ")";
        }
        else if (key == "scroll-y")
        {
            assignments["ScrollY"] = std::string("uimd.Bool(") + (truthy(&value, false) ? "true" : "false") + ")";
        }
        else if (key == "text-color-gradient")
        {
            const YamlMap* gradient = valueAsMap(&value);
            if (gradient != nullptr)
            {
                assignments["TextColorGradient"] = textGradientCode(*gradient);
            }
        }
        else if (key == "text-background-gradient")
        {
            const YamlMap* gradient = valueAsMap(&value);
            if (gradient != nullptr)
            {
                assignments["TextBackgroundGradient"] = textGradientCode(*gradient);
            }
        }
    }
    if (assignments.empty())
    {
        return "uimd.NewStyle()";
    }
    std::string result = "uimd.Style{\n";
    for (const auto& [field, code] : assignments)
    {
        result += "        " + field + ": " + code + ",\n";
    }
    result += "    }";
    return result;
}

std::string moduleReferencePathFor(const std::filesystem::path& modulePath)
{
    const char* sdkGoTarget = std::getenv("UIMD_SDK_GO_TARGET");
    if (sdkGoTarget != nullptr && *sdkGoTarget != '\0')
    {
        const std::filesystem::path installedTarget = absolutePath(sdkGoTarget);
        if (std::filesystem::is_regular_file(installedTarget / "go.mod"))
        {
            return installedTarget.generic_string();
        }
    }

    const std::filesystem::path root = projectRootFor(modulePath);
    const std::filesystem::path runtimePackage = root.empty()
        ? std::filesystem::path{"go"} / "src" / "uimd"
        : root / "go" / "src" / "uimd";
    std::error_code error;
    std::filesystem::path relativePath = std::filesystem::relative(runtimePackage, modulePath.parent_path(), error);
    if (!error && !relativePath.empty())
    {
        return relativePath.generic_string();
    }
    return runtimePackage.generic_string();
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

std::vector<std::filesystem::path> compileGoFile(
    const std::filesystem::path& sourcePath,
    const std::filesystem::path& sourceRoot,
    bool sourceRootHasDirectSource,
    const std::filesystem::path& outputDir,
    bool shouldGenerateAppStub,
    bool mcpEnabled
)
{
    const std::filesystem::path absoluteSource = absolutePath(sourcePath);
    std::filesystem::create_directories(outputDir);

    NativeDocument document = parseDocumentFile(pathString(absoluteSource));
    const YamlMap style = styleForDocument(document, absoluteSource, false);
    const std::string baseName = absoluteSource.stem().string();
    const std::string classNameValue = className(baseName);
    const std::filesystem::path sourceOutputPath = outputDir / (baseName + "_ui.go");
    const std::filesystem::path sourceParent = absoluteSource.parent_path();
    const std::filesystem::path absoluteSourceRoot = absolutePath(sourceRoot);
    const bool isDirectRootSource = sourceParent == absoluteSourceRoot;
    const bool isDirectExampleSource = !sourceRootHasDirectSource && sourceParent.parent_path() == absoluteSourceRoot;
    const std::string packageName = (isDirectRootSource || isDirectExampleSource) ? "main" : goPackageName(sourceParent);

    std::vector<std::filesystem::path> generated;
    writeTextFile(sourceOutputPath, generateSource(baseName, classNameValue, document, style, mcpEnabled, packageName));
    generated.push_back(sourceOutputPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (baseName + ".go");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, goAppTemplate(classNameValue));
            generated.push_back(appPath);
        }
    }
    return generated;
}

}  // namespace

std::string goModuleFile(const std::string& project, const std::string& localRuntimeReference)
{
    const bool quoteReference = std::any_of(
        localRuntimeReference.begin(),
        localRuntimeReference.end(),
        [](unsigned char ch)
        {
            return std::isspace(ch) != 0 || ch == '"' || ch == '\\';
        }
    );
    const std::string runtimeReference = quoteReference ? goString(localRuntimeReference) : localRuntimeReference;
    return "module " + project + "\n"
        "\n"
        "go 1.26\n"
        "\n"
        "require uimd v0.0.0\n"
        "\n"
        "replace uimd => " + runtimeReference + "\n";
}

std::string goAppTemplate(const std::string& classNameValue)
{
    const std::string appName = appClassName(classNameValue);
    return "package main\n"
        "\n"
        "import (\n"
        "    \"os\"\n"
        "    \"strings\"\n"
        "\n"
        "    uimd \"uimd\"\n"
        ")\n"
        "\n"
        "type " + appName + " struct {\n"
        "    *" + classNameValue + "\n"
        "}\n"
        "\n"
        "func New" + appName + "() *" + appName + " {\n"
        "    app := &" + appName + "{\n"
        "        " + classNameValue + ": New" + classNameValue + "(),\n"
        "    }\n"
        "    app.SetEventHandler(app)\n"
        "    return app\n"
        "}\n"
        "\n"
        "func (app *" + appName + ") OnHelloButtonClick() {\n"
        "    value := strings.TrimSpace(app.Name.Value)\n"
        "    if value == \"\" {\n"
        "        value = \"world\"\n"
        "    }\n"
        "    app.Headline.SetText(\"Hello, \" + value + \"!\")\n"
        "}\n"
        "\n"
        "func (app *" + appName + ") OnQuitButtonClick() {\n"
        "    app.RequestClose()\n"
        "}\n"
        "\n"
        "func main() {\n"
        "    code := uimd.RunGeneratedAppMain(func() int {\n"
        "        app := New" + appName + "()\n"
        "        return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)\n"
        "    })\n"
        "    os.Exit(code)\n"
        "}\n";
}

std::vector<std::filesystem::path> generateGoSources(
    const std::filesystem::path& sourcePath,
    const NativeGoGenerateOptions& options
)
{
    std::vector<std::filesystem::path> generated;
    const std::vector<std::filesystem::path> files = sourceFiles(sourcePath);
    const bool autoBootstrap = files.size() == 1;
    const std::filesystem::path sourceRoot = std::filesystem::is_regular_file(sourcePath)
        ? absolutePath(sourcePath).parent_path()
        : absolutePath(sourcePath);
    bool sourceRootHasDirectSource = false;
    for (const std::filesystem::path& sourceFile : files)
    {
        if (absolutePath(sourceFile).parent_path() == sourceRoot)
        {
            sourceRootHasDirectSource = true;
            break;
        }
    }
    for (const std::filesystem::path& sourceFile : files)
    {
        const std::filesystem::path outputDir = outputDirFor(sourceFile, options);
        const bool bootstrap = options.generateAppStub || autoBootstrap;
        std::vector<std::filesystem::path> result = compileGoFile(
            sourceFile,
            sourceRoot,
            sourceRootHasDirectSource,
            outputDir,
            bootstrap,
            options.mcpEnabled
        );
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path modulePath = outputDir / "go.mod";
            writeTextFile(modulePath, goModuleFile(sourceFile.stem().string(), moduleReferencePathFor(modulePath)));
            generated.push_back(modulePath);
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

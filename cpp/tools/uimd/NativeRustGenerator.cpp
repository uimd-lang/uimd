#include "NativeRustGenerator.hpp"
#include "NativeCompilerModel.hpp"

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

constexpr int textGradientDefaultIntervalMs = 70;
constexpr int textGradientDefaultStep = 1;
constexpr int textGradientDefaultSegmentSize = 1;

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

std::string rustString(const std::string& value)
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

std::string rustModuleName(const std::filesystem::path& directory)
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

std::string rustIdentifier(std::string value)
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

std::string rustFieldIdentifier(std::string value)
{
    std::string result;
    bool separator = false;
    for (unsigned char ch : value)
    {
        if (std::isalnum(ch))
        {
            if (separator && !result.empty())
            {
                result.push_back('_');
            }
            result.push_back(static_cast<char>(std::tolower(ch)));
            separator = false;
        }
        else
        {
            separator = true;
        }
    }
    if (result.empty() || std::isdigit(static_cast<unsigned char>(result.front())))
    {
        result = "uimd_" + result;
    }
    static const std::set<std::string> keywords{
        "as", "break", "const", "continue", "crate", "else", "enum", "extern",
        "false", "fn", "for", "if", "impl", "in", "let", "loop", "match",
        "mod", "move", "mut", "pub", "ref", "return", "self", "Self", "static",
        "struct", "super", "trait", "true", "type", "unsafe", "use", "where",
        "while", "async", "await", "dyn",
    };
    if (keywords.find(result) != keywords.end())
    {
        result += "_element";
    }
    return result;
}

std::string rustTypeModuleName(std::string value)
{
    if (value.size() >= 2 && value.ends_with("UI"))
    {
        value.resize(value.size() - 2);
    }
    std::string result;
    for (std::size_t index = 0; index < value.size(); ++index)
    {
        const unsigned char ch = static_cast<unsigned char>(value[index]);
        if (std::isupper(ch) && index != 0)
        {
            const unsigned char previous = static_cast<unsigned char>(value[index - 1]);
            if (std::islower(previous) || std::isdigit(previous))
            {
                result.push_back('_');
            }
        }
        result.push_back(static_cast<char>(std::tolower(ch)));
    }
    return rustFieldIdentifier(result);
}

std::string className(const std::string& baseName)
{
    return rustIdentifier(baseName) + "UI";
}

std::string appClassName(const std::string& classNameValue)
{
    if (endsWith(classNameValue, "UI"))
    {
        return classNameValue.substr(0, classNameValue.size() - 2) + "App";
    }
    return classNameValue + "App";
}

std::string mcpElementMetadataCode(const std::vector<CompilerMember>& members)
{
    std::string result = "vec![";
    bool first = true;
    for (const CompilerMember& member : members)
    {
        if (!first)
        {
            result += ", ";
        }
        first = false;
        result += "uimd::GeneratedElementMetadata::new("
            + rustString(member.name) + ", "
            + rustString(member.description) + ", "
            + (member.expose ? "true" : "false") + ")";
    }
    return result + "]";
}

std::string mcpAppToolsCode(const YamlMap& tools)
{
    std::vector<std::string> names;
    for (const auto& [name, value] : tools)
    {
        (void)value;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    std::string result = "vec![";
    bool first = true;
    for (const std::string& name : names)
    {
        const YamlMap* tool = valueAsMap(tools.get(name));
        if (tool == nullptr)
        {
            continue;
        }
        if (!first)
        {
            result += ", ";
        }
        first = false;
        const YamlValue* input = tool->get("inputSchema");
        const YamlValue* output = tool->get("outputSchema");
        const std::string inputSchema = input == nullptr
            ? "{\"type\":\"object\",\"properties\":{}}"
            : compilerJsonCompact(*input);
        const std::string outputSchema = output == nullptr
            ? std::string{}
            : compilerJsonCompact(*output);
        result += "uimd::GeneratedAppToolMetadata::from_json("
            + rustString(name) + ", "
            + rustString(yamlString(*tool, "description")) + ", "
            + rustString(inputSchema) + ", "
            + rustString(outputSchema) + ")";
    }
    return result + "]";
}

bool rustMembersRequireSixel(const std::vector<CompilerMember>& members)
{
    for (const CompilerMember& member : members)
    {
        if (member.type == "image" && lower(trim(member.renderMode)) != "fallback")
        {
            return true;
        }
    }
    return false;
}

std::string rustElementType(const std::string& elemType)
{
    if (elemType == "label") return "uimd::LabelRef";
    if (elemType == "spanlabel") return "uimd::SpanLabelRef";
    if (elemType == "infolabel") return "uimd::InfoLabelRef";
    if (elemType == "framebufferview") return "uimd::FrameBufferViewRef";
    if (elemType == "messagetable") return "uimd::MessageTableRef";
    if (elemType == "image") return "uimd::ImageRef";
    if (elemType == "button") return "uimd::ButtonRef";
    if (elemType == "checkbox") return "uimd::CheckBoxRef";
    if (elemType == "textinput") return "uimd::TextInputRef";
    if (elemType == "textarea") return "uimd::TextAreaRef";
    if (elemType == "numberinput") return "uimd::NumberInputRef";
    if (elemType == "combobox") return "uimd::ComboBoxRef";
    if (elemType == "listbox") return "uimd::ListBoxRef";
    if (elemType == "uiscrollview") return "uimd::ScrollViewRef";
    if (elemType == "viewhost") return "uimd::ViewHostRef";
    if (!compilerIsBuiltinType(elemType)) return "uimd::ReusableElementRef";
    return "uimd::ElementRef";
}

std::string listCode(const std::vector<std::string>& values)
{
    std::string result = "vec![";
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += rustString(values[index]) + ".to_string()";
    }
    result += "]";
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

std::string ctorCode(const CompilerMember& member, int scrollviewGap)
{
    const std::string& name = member.name;
    const std::string& elemType = member.type;
    if (elemType == "spanlabel")
    {
        return "uimd::new_span_label(" + rustString(name) + ", " + rustString(member.text) + ")";
    }
    if (elemType == "infolabel")
    {
        return "uimd::new_info_label(" + rustString(name) + ", " + rustString(member.text) + ")";
    }
    if (elemType == "framebufferview")
    {
        return "uimd::new_frame_buffer_view(" + rustString(name) + ")";
    }
    if (elemType == "messagetable")
    {
        return "uimd::new_message_table(" + rustString(name) + ")";
    }
    if (elemType == "image")
    {
        return "uimd::new_image(" + rustString(name) +
            ", " + rustString(member.source) +
            ", " + rustString(member.alt) +
            ", " + rustString(member.fit) +
            ", " + rustString(member.renderMode) +
            ", " + rustString(member.align) +
            ", " + rustString(member.verticalAlign) + ")";
    }
    if (elemType == "button")
    {
        return "uimd::new_button(" + rustString(name) + ", " + rustString(member.title) + ")";
    }
    if (elemType == "checkbox")
    {
        return "uimd::new_checkbox(" + rustString(name) + ", " + rustString(member.title) +
            ", " + std::string{member.checked ? "true" : "false"} + ")";
    }
    if (elemType == "textinput")
    {
        return "uimd::new_text_input(" + rustString(name) + ", " + rustString(member.value) +
            ", " + std::to_string(member.maxLength) + ")";
    }
    if (elemType == "textarea")
    {
        return "uimd::new_text_area_with_max_length(" + rustString(name) + ", " +
            rustString(member.value) + ", " +
            std::to_string(member.maxLength) + ")";
    }
    if (elemType == "numberinput")
    {
        return "uimd::new_number_input_with_step(" + rustString(name) + ", " +
            std::to_string(member.numberValue) + ", " +
            std::to_string(member.numberStep) + ")";
    }
    if (elemType == "combobox")
    {
        return "uimd::new_combo_box(" + rustString(name) + ", " + listCode(member.options) + ")";
    }
    if (elemType == "listbox")
    {
        return "uimd::new_list_box(" + rustString(name) + ", " + listCode(member.options) +
            ", " + std::string{member.multiple ? "true" : "false"} + ")";
    }
    if (elemType == "uiscrollview")
    {
        return "uimd::new_scroll_view(" + rustString(name) + ", " + std::to_string(scrollviewGap) + ")";
    }
    if (elemType == "viewhost")
    {
        return "uimd::new_view_host(" + rustString(name) + ")";
    }
    if (elemType == "label")
    {
        return "uimd::new_label(" + rustString(name) + ", " + rustString(member.text) + ")";
    }
    return "uimd::new_reusable_element(" + rustString(name) + ", " + rustString(elemType) + ")";
}

std::string windowTitle(const NativeDocument& document, const std::string& baseName)
{
    return document.title.empty() ? baseName : document.title;
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

std::string layoutItemType(
    const NativeLayoutItem& item,
    const std::vector<CompilerMember>& members
)
{
    const auto member = std::find_if(
        members.begin(),
        members.end(),
        [&](const CompilerMember& candidate) { return candidate.name == item.content; }
    );
    if (member != members.end())
    {
        return member->type;
    }
    if (!item.content.empty())
    {
        return "label";
    }
    return "";
}

std::string rustStyleCode(const YamlMap& style);

std::string generateLayout(const CompilerDocument& model)
{
    const NativeDocument& document = model.document;
    const YamlMap& style = model.style;
    std::string result = "vec![\n";
    auto layoutInt = [](int value) {
        if (value == GRID_EXPANDED)
        {
            return std::string{"uimd::EXPANDED"};
        }
        if (value == GRID_FIT_CONTENT)
        {
            return std::string{"uimd::FIT_CONTENT"};
        }
        return std::to_string(value);
    };
    for (const NativeLayoutItem& item : document.layout)
    {
        const std::string itemType = layoutItemType(item, model.members);
        const YamlMap itemCellStyle = compilerCellStyle(style, item.cellName);
        const YamlMap itemElementStyle =
            compilerLayoutElementStyle(style, itemType, item.content, item.cellName);
        result += "        uimd::LayoutItem {\n";
        result += "            element_type: " + rustString(itemType) + ".to_string(),\n";
        result += "            row: " + layoutInt(item.row) + ", col: " + layoutInt(item.col) + ",\n";
        result += "            cell_row: " + layoutInt(item.cellRow) + ", cell_col: " + layoutInt(item.cellCol) + ",\n";
        result += "            cell_chars_width: " + layoutInt(item.cellCharsWidth) + ", cell_chars_height: " + layoutInt(item.cellCharsHeight) + ",\n";
        result += "            cell_name: " + rustString(item.cellName) + ".to_string(), cell_width: " + layoutInt(item.cellWidth) + ", cell_height: " + layoutInt(item.cellHeight) + ",\n";
        result += "            cell_width_mode: " + rustString(item.cellWidthMode) + ".to_string(), cell_height_mode: " + rustString(item.cellHeightMode) + ".to_string(),\n";
        result += "            width: " + layoutInt(item.width) + ", height: " + layoutInt(item.height) + ",\n";
        result += "            width_mode: " + rustString(item.widthMode) + ".to_string(), height_mode: " + rustString(item.heightMode) + ".to_string(),\n";
        result += "            chars_width: " + layoutInt(item.charsWidth) + ", chars_height: " + layoutInt(item.charsHeight) + ",\n";
        result += "            margin_right: " + layoutInt(item.marginRight) + ", margin_bottom: " + layoutInt(item.marginBottom) + ",\n";
        result += "            content: " + rustString(item.content) + ".to_string(),\n";
        result += "            cell_style: " + rustStyleCode(itemCellStyle) + ",\n";
        result += "            element_style: " + rustStyleCode(itemElementStyle) + ",\n";
        result += "        },\n";
    }
    result += "    ]";
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
    return "on_" + rustFieldIdentifier(name) + "_" + lower(suffix);
}

std::vector<EventSpec> eventSpecsForMember(const CompilerMember& member)
{
    const std::string& name = member.name;
    const std::string& elemType = member.type;
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

std::vector<EventSpec> eventSpecs(const std::vector<CompilerMember>& members)
{
    std::vector<EventSpec> specs;
    std::set<std::pair<std::string, std::string>> seen;
    for (const CompilerMember& member : members)
    {
        for (const EventSpec& spec : eventSpecsForMember(member))
        {
            if (seen.insert({spec.methodName, spec.channel}).second)
            {
                specs.push_back(spec);
            }
        }
    }
    return specs;
}

std::vector<EventSpec> specsForChannel(
    const std::vector<CompilerMember>& members,
    const std::string& channel
)
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

struct RustDependencySpec
{
    std::string sourceName;
    std::string moduleName;
    std::string className;
    std::string relativePath;
};

std::string generateSource(
    const std::string& baseName,
    const std::string& classNameValue,
    const CompilerDocument& model,
    bool mcpEnabled,
    const std::string& moduleName,
    const std::vector<RustDependencySpec>& dependencies)
{
    (void)moduleName;
    const NativeDocument& document = model.document;
    const YamlMap& style = model.style;
    const std::vector<CompilerMember>& members = model.members;
    const std::string title = windowTitle(document, baseName);
    const bool scrollviewExtension = model.extends == "uiscrollview";
    const std::string baseConstructor = scrollviewExtension
        ? "uimd::GeneratedWindow::new_scroll_view_with_gap"
        : "uimd::GeneratedWindow::new";
    const int scrollviewGap = compilerScrollviewExtensionGap(style);
    const std::string eventTrait = classNameValue + "Events";
    const std::string runtimeAdapter = classNameValue + "Runtime";

    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
        "#![allow(clippy::too_many_lines, dead_code, unused_variables)]",
        "",
        "use serde_json::{Map, Value};",
    };
    for (const RustDependencySpec& dependency : dependencies)
    {
        lines.push_back("#[path = " + rustString(dependency.relativePath) + "]");
        lines.push_back("mod " + dependency.moduleName + ";");
    }
    lines.push_back("");
    lines.push_back("pub struct " + classNameValue);
    lines.push_back("{");
    lines.push_back("    pub base: uimd::GeneratedWindow,");

    for (const CompilerMember& member : members)
    {
        lines.push_back(
            "    pub " + rustFieldIdentifier(member.name) + ": " +
            rustElementType(member.type) + ","
        );
    }

    lines.push_back("}");
    lines.push_back("");
    lines.push_back("impl " + classNameValue);
    lines.push_back("{");
    lines.push_back("    pub fn new() -> Self");
    lines.push_back("    {");
    lines.push_back(
        "        let mut base = " + baseConstructor + "(" + rustString(title) +
        (scrollviewExtension ? ", " + std::to_string(scrollviewGap) : std::string{}) +
        ");"
    );
    lines.push_back("        base.set_generated_layout(" + generateLayout(model) + ");");
    lines.push_back("        base.set_generated_kind(" + rustString(model.kind) + ");");
    lines.push_back("        base.set_generated_focusable(" + std::string{model.focusable ? "true" : "false"} + ");");
    const YamlMap windowStyle = compilerWindowStyle(style);
    if (!windowStyle.empty())
    {
        lines.push_back("        base.set_generated_window_style(" + rustStyleCode(windowStyle) + ");");
    }
    if (model.extends == "uiscrollview")
    {
        for (const auto& [setter, stateStyle] :
             compilerScrollviewExtensionStyleStates(style))
        {
            if (stateStyle.empty())
            {
                continue;
            }
            if (setter == "setStyle")
            {
                lines.push_back("        base.set_generated_scroll_view_style(" + rustStyleCode(stateStyle) + ");");
            }
            else if (setter == "setFocusStyle")
            {
                lines.push_back("        base.set_generated_scroll_view_focus_style(" + rustStyleCode(stateStyle) + ");");
            }
            else if (setter == "setEditStyle")
            {
                lines.push_back("        base.set_generated_scroll_view_edit_style(" + rustStyleCode(stateStyle) + ");");
            }
            else if (setter == "setSelectedStyle")
            {
                lines.push_back("        base.set_generated_scroll_view_selected_style(" + rustStyleCode(stateStyle) + ");");
            }
        }
        const YamlMap descendantFocusStyle =
            compilerScrollviewExtensionDescendantFocusStyle(style);
        if (!descendantFocusStyle.empty())
        {
            lines.push_back("        base.set_generated_scroll_view_descendant_focus_style(" + rustStyleCode(descendantFocusStyle) + ");");
        }
    }
    for (const auto& [selector, value] : style)
    {
        const YamlMap* namedStyle = valueAsMap(&value);
        if (namedStyle == nullptr || selector.size() < 2 || selector.front() != '@')
        {
            continue;
        }
        lines.push_back(
            "        base.set_generated_named_style(" +
            rustString(selector.substr(1)) +
            ", " +
            rustStyleCode(*namedStyle) +
            ");"
        );
    }
    lines.push_back("        base.set_mcp_metadata(" + std::string{mcpEnabled ? "true" : "false"} +
        ", " + rustString(classNameValue.substr(0, classNameValue.size() >= 2 ? classNameValue.size() - 2 : classNameValue.size())) +
        ", " + rustString(mcpEnabled ? document.sourceText : std::string{}) +
        ", " + rustString(model.description) + ");");
    lines.push_back("        base.set_mcp_element_metadata(" + mcpElementMetadataCode(members) + ");");
    lines.push_back("        base.set_mcp_app_tools(" + mcpAppToolsCode(model.mcpTools) + ");");
    if (rustMembersRequireSixel(members))
    {
        lines.push_back("        uimd::require_sixel_for_image_rendering();");
    }

    for (const CompilerMember& member : members)
    {
        const std::string& name = member.name;
        const YamlMap* rawMember = valueAsMap(document.members.get(name));
        const std::string field = rustFieldIdentifier(name);
        lines.push_back("        let " + field + " = " + ctorCode(member, scrollviewGap) + ";");
        if (member.hasCommitMode)
        {
            lines.push_back(
                "        " + field + ".borrow_mut().set_commit_mode(" +
                rustString(member.commitMode) + ");"
            );
        }
        lines.push_back("        base.add_element(" + field + ".clone());");
        const std::string& elemType = member.type;
        if (!compilerIsBuiltinType(elemType))
        {
            const std::string sourceName =
                std::filesystem::path(member.customSource).stem().string();
            const auto dependency = std::find_if(
                dependencies.begin(),
                dependencies.end(),
                [&](const RustDependencySpec& candidate)
                {
                    return candidate.sourceName == sourceName;
                }
            );
            if (dependency != dependencies.end())
            {
                lines.push_back(
                    "        " + field + ".borrow_mut().set_child_window(" +
                    dependency->moduleName + "::" + dependency->className +
                    "::new().base);"
                );
            }
        }
        if (elemType == "combobox")
        {
            if (member.selectedItemIndex >= 0)
            {
                lines.push_back(
                    "        " + field + ".borrow_mut().set_selected_index(" +
                    std::to_string(member.selectedItemIndex) + ");"
                );
            }
        }
        if (elemType == "listbox")
        {
            if (member.selectedValues.size() > 1)
            {
                lines.push_back(
                    "        " + field + ".borrow_mut().set_selected_items(&" +
                    listCode(member.selectedValues) + ");"
                );
            }
            else if (member.selectedItemsIndex >= 0)
            {
                lines.push_back(
                    "        " + field + ".borrow_mut().set_selected_index(" +
                    std::to_string(member.selectedItemsIndex) + ");"
                );
            }
        }
        const std::string cellName = cellNameForElement(document.layout, name);
        std::string styleElemType;
        if (elemType == "textarea")
        {
            styleElemType = "textinput";
        }
        else if (!compilerIsBuiltinType(elemType) || elemType == "uiscrollview")
        {
            styleElemType = "uielement";
        }
        else
        {
            styleElemType = elemType;
        }
        for (const auto& [setter, stateStyle] :
             compilerStyleStatesForElement(
                 style,
                 styleElemType,
                 name,
                 cellName,
                 rawMember
             ))
        {
            if (!stateStyle.empty())
            {
                std::string method = lower(setter);
                std::string snakeMethod;
                for (std::size_t index = 0; index < setter.size(); ++index)
                {
                    const unsigned char ch = static_cast<unsigned char>(setter[index]);
                    if (std::isupper(ch) && index != 0)
                    {
                        snakeMethod += "_";
                    }
                    snakeMethod += static_cast<char>(std::tolower(ch));
                }
                lines.push_back("        " + field + ".borrow_mut()." + snakeMethod + "(" + rustStyleCode(stateStyle) + ");");
            }
        }
    }
    lines.push_back("        Self");
    lines.push_back("        {");
    lines.push_back("            base,");
    for (const CompilerMember& member : members)
    {
        lines.push_back("            " + rustFieldIdentifier(member.name) + ",");
    }
    lines.push_back("        }");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    pub fn run<H: " + eventTrait + ">(&mut self, handler: &mut H, args: &[String]) -> i32");
    lines.push_back("    {");
    lines.push_back("        let options = self.base.runtime_options();");
    lines.push_back("        let mut runtime = " + runtimeAdapter + " { ui: self, handler };");
    lines.push_back("        uimd::run_generated_window(&mut runtime, options, args)");
    lines.push_back("    }");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("impl Default for " + classNameValue);
    lines.push_back("{");
    lines.push_back("    fn default() -> Self { Self::new() }");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("pub trait " + eventTrait);
    lines.push_back("{");
    for (const EventSpec& spec : specsForChannel(members, "button"))
    {
        lines.push_back("    fn " + spec.methodName + "(&mut self, _ui: &mut " + classNameValue + ") {}");
    }
    for (const EventSpec& spec : specsForChannel(members, "text"))
    {
        lines.push_back("    fn " + spec.methodName + "(&mut self, _ui: &mut " + classNameValue + ", _value: &str) {}");
    }
    for (const EventSpec& spec : specsForChannel(members, "confirmed"))
    {
        lines.push_back("    fn " + spec.methodName + "(&mut self, _ui: &mut " + classNameValue + ", _value: &str) {}");
    }
    for (const EventSpec& spec : specsForChannel(members, "selection_text"))
    {
        lines.push_back("    fn " + spec.methodName + "(&mut self, _ui: &mut " + classNameValue + ", _value: &str) {}");
    }
    for (const EventSpec& spec : specsForChannel(members, "selection"))
    {
        lines.push_back("    fn " + spec.methodName + "(&mut self, _ui: &mut " + classNameValue + ", _value: &[String]) {}");
    }
    lines.push_back("    fn handle_dynamic_button(&mut self, _ui: &mut " + classNameValue + ", _name: &str) -> bool { false }");
    lines.push_back("    fn handle_dynamic_text_changed(&mut self, _ui: &mut " + classNameValue + ", _name: &str, _value: &str) -> bool { false }");
    lines.push_back("    fn handle_dynamic_text_confirmed(&mut self, _ui: &mut " + classNameValue + ", _name: &str, _value: &str) -> bool { false }");
    lines.push_back("    fn handle_dynamic_selection_changed(&mut self, _ui: &mut " + classNameValue + ", _name: &str, _value: &[String]) -> bool { false }");
    lines.push_back("    fn handle_active_window_button(&mut self, _ui: &mut " + classNameValue + ", _name: &str) -> bool { false }");
    lines.push_back("    fn on_focus_changed(&mut self, _ui: &mut " + classNameValue + ", _name: &str, _focused: bool) {}");
    lines.push_back("    fn handle_key_before_focused(&mut self, _ui: &mut " + classNameValue + ", _key: &str, _name: &str, _edit_mode: bool) -> bool { false }");
    lines.push_back("    fn handle_key(&mut self, _ui: &mut " + classNameValue + ", _key: &str) -> bool { false }");
    lines.push_back("    fn on_window_closed(&mut self, _ui: &mut " + classNameValue + ", _window: uimd::GeneratedWindow) {}");
    lines.push_back("    fn mcp_tool_names(&self) -> Vec<String> { Vec::new() }");
    lines.push_back("    fn handle_mcp_tool(&mut self, _ui: &mut " + classNameValue + ", _name: &str, _arguments: &Map<String, Value>) -> Option<Value> { None }");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("struct " + runtimeAdapter + "<'a, H: " + eventTrait + ">");
    lines.push_back("{");
    lines.push_back("    ui: &'a mut " + classNameValue + ",");
    lines.push_back("    handler: &'a mut H,");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("impl<H: " + eventTrait + "> uimd::GeneratedApplication for " + runtimeAdapter + "<'_, H>");
    lines.push_back("{");
    lines.push_back("    fn window(&self) -> &uimd::GeneratedWindow { &self.ui.base }");
    lines.push_back("    fn window_mut(&mut self) -> &mut uimd::GeneratedWindow { &mut self.ui.base }");
    lines.push_back("    fn handle_active_window_button(&mut self, name: &str) -> bool { self.handler.handle_active_window_button(self.ui, name) }");
    lines.push_back("");
    lines.push_back("    fn handle_generated_button(&mut self, name: &str) -> bool");
    lines.push_back("    {");
    for (const EventSpec& spec : specsForChannel(members, "button"))
    {
        lines.push_back("        if name == " + rustString(spec.name) + " { self.handler." + spec.methodName + "(self.ui); return true; }");
    }
    lines.push_back("        self.handler.handle_dynamic_button(self.ui, name)");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("    fn handle_generated_text_changed(&mut self, name: &str, value: &str) -> bool");
    lines.push_back("    {");
    for (const EventSpec& spec : specsForChannel(members, "text"))
    {
        lines.push_back("        if name == " + rustString(spec.name) + " { self.handler." + spec.methodName + "(self.ui, value); return true; }");
    }
    lines.push_back("        self.handler.handle_dynamic_text_changed(self.ui, name, value)");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("    fn handle_generated_text_confirmed(&mut self, name: &str, value: &str) -> bool");
    lines.push_back("    {");
    for (const EventSpec& spec : specsForChannel(members, "confirmed"))
    {
        lines.push_back("        if name == " + rustString(spec.name) + " { self.handler." + spec.methodName + "(self.ui, value); return true; }");
    }
    lines.push_back("        self.handler.handle_dynamic_text_confirmed(self.ui, name, value)");
    lines.push_back("}");
    lines.push_back("");
    lines.push_back("    fn handle_generated_selection_changed(&mut self, name: &str, value: &[String]) -> bool");
    lines.push_back("    {");
    for (const EventSpec& spec : specsForChannel(members, "selection_text"))
    {
        lines.push_back("        if name == " + rustString(spec.name) + " { self.handler." + spec.methodName + "(self.ui, value.first().map(String::as_str).unwrap_or_default()); return true; }");
    }
    for (const EventSpec& spec : specsForChannel(members, "selection"))
    {
        lines.push_back("        if name == " + rustString(spec.name) + " { self.handler." + spec.methodName + "(self.ui, value); return true; }");
    }
    lines.push_back("        self.handler.handle_dynamic_selection_changed(self.ui, name, value)");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    fn handle_focus_changed(&mut self, name: &str, focused: bool) -> bool");
    lines.push_back("    {");
    lines.push_back("        self.handler.on_focus_changed(self.ui, name, focused);");
    lines.push_back("        true");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    fn handle_key_before_focused(&mut self, key: &str, name: &str, edit_mode: bool) -> bool");
    lines.push_back("    {");
    lines.push_back("        self.handler.handle_key_before_focused(self.ui, key, name, edit_mode)");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    fn handle_key(&mut self, key: &str) -> bool { self.handler.handle_key(self.ui, key) }");
    lines.push_back("    fn handle_generated_window_closed(&mut self, window: uimd::GeneratedWindow) { self.handler.on_window_closed(self.ui, window); }");
    lines.push_back("    fn mcp_tool_names(&self) -> Vec<String> { self.handler.mcp_tool_names() }");
    lines.push_back("    fn handle_mcp_tool(&mut self, name: &str, arguments: &Map<String, Value>) -> Option<Value> { self.handler.handle_mcp_tool(self.ui, name, arguments) }");
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

std::filesystem::path outputDirFor(const std::filesystem::path& sourceFile, const NativeRustGenerateOptions& options)
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
        assignments[prefix] = "Some(" + std::to_string(values[0]) + ")";
    }
    else if (values.size() == 2)
    {
        assignments[prefix + "_top"] = "Some(" + std::to_string(values[0]) + ")";
        assignments[prefix + "_right"] = "Some(" + std::to_string(values[1]) + ")";
        assignments[prefix + "_bottom"] = "Some(" + std::to_string(values[0]) + ")";
        assignments[prefix + "_left"] = "Some(" + std::to_string(values[1]) + ")";
    }
    else if (values.size() >= 4)
    {
        assignments[prefix + "_top"] = "Some(" + std::to_string(values[0]) + ")";
        assignments[prefix + "_right"] = "Some(" + std::to_string(values[1]) + ")";
        assignments[prefix + "_bottom"] = "Some(" + std::to_string(values[2]) + ")";
        assignments[prefix + "_left"] = "Some(" + std::to_string(values[3]) + ")";
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
        colorCode += "uimd::Color::new(" + rustString(colors[index]) + ")";
    }
    return "Some(uimd::TextGradient { interval_ms: " + std::to_string(interval) +
        ", step: " + std::to_string(step) +
        ", segment_size: " + std::to_string(segmentSize) +
        ", colors: vec![" + colorCode + "] })";
}

std::string rustStyleCode(const YamlMap& style)
{
    std::map<std::string, std::string> assignments;
    for (const auto& [key, value] : style)
    {
        const std::string text = valueAsString(value);
        if (key == "color")
        {
            assignments["color"] = "Some(uimd::Color::new(" + rustString(text) + "))";
        }
        else if (key == "background")
        {
            assignments["background"] = "Some(uimd::Color::new(" + rustString(text) + "))";
        }
        else if (key == "background-texture")
        {
            assignments["background_texture"] = "Some(" + rustString(text) + ".to_string())";
        }
        else if (key == "background-texture-color")
        {
            assignments["background_texture_color"] = "Some(uimd::Color::new(" + rustString(text) + "))";
        }
        else if (key == "scope-dim-background")
        {
            assignments["scope_dim_background"] = "Some(uimd::Color::new(" + rustString(text) + "))";
        }
        else if (key == "border-color")
        {
            assignments["border_color"] = "Some(uimd::Color::new(" + rustString(text) + "))";
        }
        else if (key == "border-width")
        {
            const std::string code = "Some(" + std::to_string(valueAsInt(&value, 0)) + ")";
            assignments["border_width_horizontal"] = code;
            assignments["border_width_vertical"] = code;
        }
        else if (key == "border-width-horizontal")
        {
            assignments["border_width_horizontal"] = "Some(" + std::to_string(valueAsInt(&value, 0)) + ")";
        }
        else if (key == "border-width-vertical")
        {
            assignments["border_width_vertical"] = "Some(" + std::to_string(valueAsInt(&value, 0)) + ")";
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
            assignments["gap"] = "Some(" + std::to_string(valueAsInt(&value, 0)) + ")";
        }
        else if (key == "text-align")
        {
            assignments["text_align"] = "Some(" + rustString(text) + ".to_string())";
        }
        else if (key == "user-select")
        {
            assignments["user_select"] = "Some(" + rustString(text) + ".to_string())";
        }
        else if (key == "scroll-x")
        {
            assignments["scroll_x"] = std::string("Some(") + (truthy(&value, false) ? "true" : "false") + ")";
        }
        else if (key == "scroll-y")
        {
            assignments["scroll_y"] = std::string("Some(") + (truthy(&value, false) ? "true" : "false") + ")";
        }
        else if (key == "text-color-gradient")
        {
            const YamlMap* gradient = valueAsMap(&value);
            if (gradient != nullptr)
            {
                assignments["text_color_gradient"] = textGradientCode(*gradient);
            }
        }
        else if (key == "text-background-gradient")
        {
            const YamlMap* gradient = valueAsMap(&value);
            if (gradient != nullptr)
            {
                assignments["text_background_gradient"] = textGradientCode(*gradient);
            }
        }
    }
    if (assignments.empty())
    {
        return "uimd::Style::default()";
    }
    std::string result = "uimd::Style {\n";
    for (const auto& [field, code] : assignments)
    {
        result += "        " + field + ": " + code + ",\n";
    }
    result += "        ..Default::default()\n"
        "    }";
    return result;
}

std::string crateReferencePathFor(const std::filesystem::path& modulePath)
{
    const char* sdkRustTarget = std::getenv("UIMD_SDK_RUST_TARGET");
    if (sdkRustTarget != nullptr && *sdkRustTarget != '\0')
    {
        const std::filesystem::path installedTarget = absolutePath(sdkRustTarget);
        if (std::filesystem::is_regular_file(installedTarget / "Cargo.toml"))
        {
            return installedTarget.generic_string();
        }
    }

    const std::filesystem::path root = projectRootFor(modulePath);
    const std::filesystem::path runtimePackage = root.empty()
        ? std::filesystem::path{"rust"} / "src" / "uimd"
        : root / "rust" / "src" / "uimd";
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

std::vector<std::filesystem::path> compileRustFile(
    const std::filesystem::path& sourcePath,
    const std::filesystem::path& sourceRoot,
    bool sourceRootHasDirectSource,
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

    const CompilerDocument model = parseCompilerDocument(absoluteSource);
    const std::vector<std::filesystem::path> dependencyPaths =
        compilerDependencyPaths(absoluteSource, model);
    std::vector<std::filesystem::path> generated;
    for (const std::filesystem::path& dependency : dependencyPaths)
    {
        std::vector<std::filesystem::path> dependencyGenerated = compileRustFile(
            dependency,
            sourceRoot,
            sourceRootHasDirectSource,
            dependency.parent_path(),
            false,
            mcpEnabled,
            visited
        );
        generated.insert(generated.end(), dependencyGenerated.begin(), dependencyGenerated.end());
    }
    const std::string baseName = absoluteSource.stem().string();
    const std::string classNameValue = className(baseName);
    const std::filesystem::path sourceOutputPath = outputDir / (baseName + "_ui.rs");
    const std::filesystem::path sourceParent = absoluteSource.parent_path();
    const std::filesystem::path absoluteSourceRoot = absolutePath(sourceRoot);
    const bool isDirectRootSource = sourceParent == absoluteSourceRoot;
    const bool isDirectExampleSource = !sourceRootHasDirectSource && sourceParent.parent_path() == absoluteSourceRoot;
    const std::string packageName = (isDirectRootSource || isDirectExampleSource) ? "main" : rustModuleName(sourceParent);
    std::vector<RustDependencySpec> dependencies;
    std::set<std::filesystem::path> emittedDependencySources;
    for (const std::filesystem::path& dependency : dependencyPaths)
    {
        const std::filesystem::path normalizedDependency = absolutePath(dependency);
        if (!emittedDependencySources.insert(normalizedDependency).second)
        {
            continue;
        }
        const std::string dependencyBaseName = dependency.stem().string();
        const std::filesystem::path dependencyOutput =
            dependency.parent_path() / (dependencyBaseName + "_ui.rs");
        std::error_code relativeError;
        std::filesystem::path relativePath =
            std::filesystem::relative(dependencyOutput, sourceOutputPath.parent_path(), relativeError);
        if (relativeError)
        {
            relativePath = dependencyOutput.lexically_normal();
        }
        dependencies.push_back(RustDependencySpec{
            .sourceName = dependencyBaseName,
            .moduleName = "uimd_dependency_" + rustTypeModuleName(className(dependencyBaseName)),
            .className = className(dependencyBaseName),
            .relativePath = relativePath.generic_string(),
        });
    }

    writeTextFile(
        sourceOutputPath,
        generateSource(
            baseName,
            classNameValue,
            model,
            mcpEnabled,
            packageName,
            dependencies
        )
    );
    generated.push_back(sourceOutputPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (baseName + ".rs");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, rustAppTemplate(classNameValue));
            generated.push_back(appPath);
        }
    }
    return generated;
}

}  // namespace

std::string rustCargoManifest(const std::string& project, const std::string& localRuntimeReference)
{
    return "[package]\n"
        "name = " + rustString(rustModuleName(project)) + "\n"
        "version = \"0.1.0\"\n"
        "edition = \"2024\"\n"
        "\n"
        "[[bin]]\n"
        "name = " + rustString(rustModuleName(project)) + "\n"
        "path = " + rustString(project + ".rs") + "\n"
        "\n"
        "[dependencies]\n"
        "serde_json = \"1\"\n"
        "uimd = { path = " + rustString(localRuntimeReference) + " }\n";
}

std::string rustAppTemplate(const std::string& classNameValue)
{
    const std::string appName = appClassName(classNameValue);
    const std::string moduleName = rustTypeModuleName(classNameValue) + "_ui";
    return "mod " + moduleName + ";\n"
        "\n"
        "use " + moduleName + "::*;\n"
        "\n"
        "#[derive(Default)]\n"
        "struct " + appName + ";\n"
        "\n"
        "impl " + classNameValue + "Events for " + appName + " {}\n"
        "\n"
        "fn main()\n"
        "{\n"
        "    let code = uimd::run_generated_app_main(||\n"
        "    {\n"
        "        let mut ui = " + classNameValue + "::new();\n"
        "        let mut app = " + appName + ";\n"
        "        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())\n"
        "    });\n"
        "    std::process::exit(code);\n"
        "}\n";
}

std::vector<std::filesystem::path> generateRustSources(
    const std::filesystem::path& sourcePath,
    const NativeRustGenerateOptions& options
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
        std::set<std::filesystem::path> visited;
        const std::filesystem::path outputDir = outputDirFor(sourceFile, options);
        const bool bootstrap = options.generateAppStub || autoBootstrap;
        std::vector<std::filesystem::path> result = compileRustFile(
            sourceFile,
            sourceRoot,
            sourceRootHasDirectSource,
            outputDir,
            bootstrap,
            options.mcpEnabled,
            visited
        );
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path modulePath = outputDir / "Cargo.toml";
            writeTextFile(modulePath, rustCargoManifest(sourceFile.stem().string(), crateReferencePathFor(modulePath)));
            generated.push_back(modulePath);
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

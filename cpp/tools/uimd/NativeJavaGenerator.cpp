#include "NativeJavaGenerator.hpp"

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
#include <vector>

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

constexpr const char* GENERATED_SCROLLVIEW_ELEMENT_NAME = "__scrollview";

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
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool endsWith(const std::string& value, const std::string& suffix)
{
    return value.size() >= suffix.size()
        && value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

const YamlMap* valueAsMap(const YamlValue* value)
{
    if (value == nullptr || !std::holds_alternative<YamlMap>(value->value))
    {
        return nullptr;
    }
    return &std::get<YamlMap>(value->value);
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

int valueAsInt(const YamlValue& value, int fallback = 0)
{
    if (std::holds_alternative<long long>(value.value))
    {
        return static_cast<int>(std::get<long long>(value.value));
    }
    if (std::holds_alternative<double>(value.value))
    {
        return static_cast<int>(std::get<double>(value.value));
    }
    try
    {
        return std::stoi(valueAsString(value));
    }
    catch (const std::exception&)
    {
        return fallback;
    }
}

int valueAsInt(const YamlValue* value, int fallback)
{
    return value == nullptr ? fallback : valueAsInt(*value, fallback);
}

std::vector<std::string> stringList(const YamlValue* value)
{
    std::vector<std::string> result;
    if (value == nullptr)
    {
        return result;
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        result.push_back(std::get<std::string>(value->value));
        return result;
    }
    if (!std::holds_alternative<YamlList>(value->value))
    {
        return result;
    }
    for (const YamlValue& item : std::get<YamlList>(value->value))
    {
        result.push_back(valueAsString(item));
    }
    return result;
}

bool truthy(const YamlValue& value)
{
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value);
    }
    if (std::holds_alternative<long long>(value.value))
    {
        return std::get<long long>(value.value) != 0;
    }
    if (std::holds_alternative<double>(value.value))
    {
        return std::get<double>(value.value) != 0.0;
    }
    const std::string normalized = lower(trim(valueAsString(value)));
    return normalized == "true" || normalized == "1" || normalized == "yes" || normalized == "on";
}

std::string javaString(const std::string& value)
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

std::string typeIdentifier(std::string value)
{
    std::string result;
    std::string current;
    auto flush = [&]()
    {
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
    for (unsigned char ch : value)
    {
        if (std::isalnum(ch))
        {
            current += static_cast<char>(ch);
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
    return typeIdentifier(baseName) + "UI";
}

std::string appClassName(const std::string& classNameValue)
{
    if (endsWith(classNameValue, "UI"))
    {
        return classNameValue.substr(0, classNameValue.size() - 2);
    }
    return classNameValue + "App";
}

std::string fieldIdentifier(std::string value)
{
    std::string result;
    for (unsigned char ch : value)
    {
        if (std::isalnum(ch) || ch == '_')
        {
            result += static_cast<char>(ch);
        }
        else if (result.empty() || result.back() != '_')
        {
            result += '_';
        }
    }
    if (result.empty() || std::isdigit(static_cast<unsigned char>(result.front())))
    {
        result = "uimd_" + result;
    }
    static const std::set<std::string> keywords{
        "abstract", "assert", "boolean", "break", "byte", "case", "catch", "char",
        "class", "const", "continue", "default", "do", "double", "else", "enum",
        "extends", "final", "finally", "float", "for", "goto", "if", "implements",
        "import", "instanceof", "int", "interface", "long", "native", "new", "package",
        "private", "protected", "public", "return", "short", "static", "strictfp", "super",
        "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void",
        "volatile", "while", "true", "false", "null", "record", "sealed", "permits", "yield",
    };
    if (keywords.contains(result))
    {
        result += "_element";
    }
    return result;
}

std::string elementType(const std::string& type)
{
    if (type == "label") return "Label";
    if (type == "spanlabel") return "SpanLabel";
    if (type == "infolabel") return "InfoLabel";
    if (type == "framebufferview") return "FrameBufferView";
    if (type == "messagetable") return "MessageTable";
    if (type == "image") return "Image";
    if (type == "button") return "Button";
    if (type == "checkbox") return "CheckBox";
    if (type == "textinput") return "TextInput";
    if (type == "textarea") return "TextArea";
    if (type == "numberinput") return "NumberInput";
    if (type == "combobox") return "ComboBox";
    if (type == "listbox") return "ListBox";
    if (type == "uiscrollview") return "ScrollView";
    if (type == "viewhost") return "ViewHost";
    if (!compilerIsBuiltinType(type)) return "ReusableElement";
    return "Element";
}

std::string javaStringList(const std::vector<std::string>& values)
{
    std::string result = "List.of(";
    for (std::size_t index = 0; index < values.size(); ++index)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += javaString(values[index]);
    }
    result += ")";
    return result;
}

std::string numberCode(double value)
{
    std::ostringstream output;
    output << std::setprecision(15) << value;
    std::string result = output.str();
    if (result.find('.') == std::string::npos
        && result.find('e') == std::string::npos
        && result.find('E') == std::string::npos)
    {
        result += ".0";
    }
    return result;
}

std::string constructorCode(const CompilerMember& member, int scrollviewGap)
{
    const std::string name = javaString(member.name);
    if (member.type == "spanlabel")
    {
        return "new SpanLabel(" + name + ", " + javaString(member.text) + ")";
    }
    if (member.type == "infolabel")
    {
        return "new InfoLabel(" + name + ", " + javaString(member.text) + ")";
    }
    if (member.type == "framebufferview")
    {
        return "new FrameBufferView(" + name + ")";
    }
    if (member.type == "messagetable")
    {
        return "new MessageTable(" + name + ", " + javaString(member.text) + ")";
    }
    if (member.type == "image")
    {
        return "new Image(" + name + ", "
            + javaString(member.source) + ", "
            + javaString(member.alt) + ", "
            + javaString(member.fit) + ", "
            + javaString(member.renderMode) + ", "
            + javaString(member.align) + ", "
            + javaString(member.verticalAlign) + ")";
    }
    if (member.type == "button")
    {
        return "new Button(" + name + ", " + javaString(member.title) + ")";
    }
    if (member.type == "checkbox")
    {
        return "new CheckBox(" + name + ", " + javaString(member.title) + ", "
            + (member.checked ? "true" : "false") + ")";
    }
    if (member.type == "textinput")
    {
        return "new TextInput(" + name + ", " + javaString(member.value) + ", "
            + std::to_string(member.maxLength) + ")";
    }
    if (member.type == "textarea")
    {
        return "new TextArea(" + name + ", " + javaString(member.value) + ", "
            + std::to_string(member.maxLength) + ")";
    }
    if (member.type == "numberinput")
    {
        return "new NumberInput(" + name + ", " + numberCode(member.numberValue) + ", "
            + numberCode(member.numberStep) + ")";
    }
    if (member.type == "combobox")
    {
        return "new ComboBox(" + name + ", " + javaStringList(member.options) + ")";
    }
    if (member.type == "listbox")
    {
        return "new ListBox(" + name + ", " + javaStringList(member.options) + ")";
    }
    if (member.type == "uiscrollview")
    {
        return "new ScrollView(" + name + ", " + std::to_string(scrollviewGap) + ")";
    }
    if (member.type == "viewhost")
    {
        return "new ViewHost(" + name + ")";
    }
    if (member.type == "label")
    {
        return "new Label(" + name + ", " + javaString(member.text) + ")";
    }
    return "new ReusableElement(" + name + ", " + javaString(member.type) + ")";
}

std::vector<int> styleIntegers(const YamlValue& value)
{
    std::vector<int> result;
    if (std::holds_alternative<YamlList>(value.value))
    {
        for (const YamlValue& item : std::get<YamlList>(value.value))
        {
            result.push_back(valueAsInt(item));
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

void assignBox(
    std::map<std::string, std::string>& properties,
    const std::string& prefix,
    const YamlValue& value)
{
    const std::vector<int> values = styleIntegers(value);
    if (values.size() == 1)
    {
        properties[prefix] = std::to_string(values[0]);
    }
    else if (values.size() == 2)
    {
        properties[prefix + "-top"] = std::to_string(values[0]);
        properties[prefix + "-right"] = std::to_string(values[1]);
        properties[prefix + "-bottom"] = std::to_string(values[0]);
        properties[prefix + "-left"] = std::to_string(values[1]);
    }
    else if (values.size() >= 4)
    {
        properties[prefix + "-top"] = std::to_string(values[0]);
        properties[prefix + "-right"] = std::to_string(values[1]);
        properties[prefix + "-bottom"] = std::to_string(values[2]);
        properties[prefix + "-left"] = std::to_string(values[3]);
    }
}

std::string textGradientCode(const YamlMap& gradient)
{
    const std::vector<std::string> colors = stringList(gradient.get("colors"));
    const int interval = valueAsInt(gradient.get("interval"), 70);
    const int step = valueAsInt(gradient.get("step"), 1);
    const YamlValue* segmentValue = gradient.get("segment-size") != nullptr
        ? gradient.get("segment-size")
        : gradient.get("segment_size");
    const int segmentSize = valueAsInt(segmentValue, 1);
    std::string result = "makeTextGradient("
        + std::to_string(interval) + ", "
        + std::to_string(step) + ", "
        + std::to_string(segmentSize);
    for (const std::string& color : colors)
    {
        result += ", " + javaString(color);
    }
    result += ")";
    return result;
}

std::string styleCode(const YamlMap& style)
{
    std::map<std::string, std::string> properties;
    const YamlMap* colorGradient = nullptr;
    const YamlMap* backgroundGradient = nullptr;
    const std::set<std::string> directProperties{
        "color", "background", "background-texture", "background-texture-color",
        "scope-dim-background", "border-color", "border-width-horizontal",
        "border-width-vertical", "gap", "text-align", "user-select", "scroll-x", "scroll-y",
    };
    for (const auto& [key, value] : style)
    {
        if (key == "text-color-gradient")
        {
            colorGradient = valueAsMap(&value);
        }
        else if (key == "text-background-gradient")
        {
            backgroundGradient = valueAsMap(&value);
        }
        else if (directProperties.contains(key))
        {
            properties[key] = (key == "scroll-x" || key == "scroll-y")
                ? (truthy(value) ? "true" : "false")
                : valueAsString(value);
        }
        else if (key == "border-width")
        {
            properties["border-width-horizontal"] = valueAsString(value);
            properties["border-width-vertical"] = valueAsString(value);
        }
        else if (key == "padding")
        {
            assignBox(properties, "padding", value);
        }
        else if (key == "margin")
        {
            assignBox(properties, "margin", value);
        }
    }
    std::string result = properties.empty() ? "new Style()" : "Style.fromProperties(";
    std::size_t index = 0;
    for (const auto& [key, value] : properties)
    {
        if (index != 0)
        {
            result += ", ";
        }
        result += javaString(key) + ", " + javaString(value);
        ++index;
    }
    if (!properties.empty())
    {
        result += ")";
    }
    if (colorGradient != nullptr || backgroundGradient != nullptr)
    {
        result = "styleWithGradients("
            + result + ", "
            + (colorGradient == nullptr ? "null" : textGradientCode(*colorGradient)) + ", "
            + (backgroundGradient == nullptr ? "null" : textGradientCode(*backgroundGradient)) + ")";
    }
    return result;
}

std::string dimensionCode(const std::string& mode, int value)
{
    if (mode == "fixed")
    {
        return "AxisDimension.fixed(" + std::to_string(value) + ")";
    }
    if (mode == "expanded")
    {
        return "AxisDimension.expanded()";
    }
    if (mode == "fit-content")
    {
        return "AxisDimension.fitContent()";
    }
    return "AxisDimension.auto(" + std::to_string(value) + ")";
}

std::string layoutType(const NativeLayoutItem& item, const std::vector<CompilerMember>& members)
{
    const auto found = std::find_if(members.begin(), members.end(), [&](const CompilerMember& member)
    {
        return member.name == item.content;
    });
    if (found != members.end())
    {
        return found->type;
    }
    return item.content.empty() ? "" : "label";
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

std::string layoutEntry(
    const NativeLayoutItem& item,
    const CompilerDocument& model,
    bool scrollviewExtension)
{
    std::string itemName = item.content;
    std::string itemType = layoutType(item, model.members);
    std::string widthMode = item.widthMode;
    std::string heightMode = item.heightMode;
    int widthValue = item.width;
    int heightValue = item.height;
    if (scrollviewExtension
        && itemName.empty()
        && (item.cellName == "panel" || item.cellName == "viewport"))
    {
        itemName = GENERATED_SCROLLVIEW_ELEMENT_NAME;
        itemType = "uiscrollview";
        widthMode = "expanded";
        heightMode = "expanded";
        widthValue = 0;
        heightValue = 0;
    }
    YamlMap cellStyle = compilerCellStyle(model.style, item.cellName);
    if (scrollviewExtension && itemName == GENERATED_SCROLLVIEW_ELEMENT_NAME)
    {
        cellStyle = scrollviewExtensionLayoutCellStyle(cellStyle);
    }
    const YamlMap elementStyle = compilerLayoutElementStyle(
        model.style,
        itemType,
        itemName,
        item.cellName);
    return "            new GeneratedLayoutEntry()\n"
        "                .setName(" + javaString(itemName) + ")\n"
        "                .setType(" + javaString(itemType) + ")\n"
        "                .setCellName(" + javaString(item.cellName) + ")\n"
        "                .setRelative(new Rect(" + std::to_string(item.row) + ", "
            + std::to_string(item.col) + ", " + std::to_string(item.width) + ", "
            + std::to_string(item.height) + "))\n"
        "                .setSourceCell(new Rect(" + std::to_string(item.cellRow) + ", "
            + std::to_string(item.cellCol) + ", " + std::to_string(item.cellCharsWidth) + ", "
            + std::to_string(item.cellCharsHeight) + "))\n"
        "                .setWidth(" + dimensionCode(widthMode, widthValue) + ")\n"
        "                .setHeight(" + dimensionCode(heightMode, heightValue) + ")\n"
        "                .setCellWidth(" + dimensionCode(item.cellWidthMode, item.cellWidth) + ")\n"
        "                .setCellHeight(" + dimensionCode(item.cellHeightMode, item.cellHeight) + ")\n"
        "                .setCharsSize(new Size(" + std::to_string(item.charsWidth) + ", "
            + std::to_string(item.charsHeight) + "))\n"
        "                .setCellCharsSize(new Size(" + std::to_string(item.cellCharsWidth) + ", "
            + std::to_string(item.cellCharsHeight) + "))\n"
        "                .setMarginRight(" + std::to_string(item.marginRight) + ")\n"
        "                .setMarginBottom(" + std::to_string(item.marginBottom) + ")\n"
        "                .setCellStyle(" + styleCode(cellStyle) + ")\n"
        "                .setElementStyle(" + styleCode(elementStyle) + ")";
}

std::string cellNameFor(const CompilerDocument& model, const std::string& memberName)
{
    for (const NativeLayoutItem& item : model.document.layout)
    {
        if (item.content == memberName)
        {
            return item.cellName;
        }
    }
    return {};
}

struct EventSpec
{
    std::string name;
    std::string channel;
    std::string methodName;
};

std::string eventMethodName(const std::string& name, const std::string& suffix)
{
    return "on" + typeIdentifier(name) + suffix;
}

std::vector<EventSpec> eventSpecs(const std::vector<CompilerMember>& members)
{
    std::vector<EventSpec> result;
    for (const CompilerMember& member : members)
    {
        if (member.type == "button" || member.type == "image")
        {
            result.push_back({member.name, "button", eventMethodName(member.name, "Click")});
        }
        else if (member.type == "checkbox")
        {
            result.push_back({member.name, "text", eventMethodName(member.name, "Change")});
        }
        else if (member.type == "textinput" || member.type == "textarea" || member.type == "numberinput")
        {
            result.push_back({member.name, "text", eventMethodName(member.name, "Change")});
            result.push_back({member.name, "confirmed", eventMethodName(member.name, "Submit")});
        }
        else if (member.type == "combobox")
        {
            result.push_back({member.name, "selection-text", eventMethodName(member.name, "Change")});
        }
        else if (member.type == "listbox")
        {
            result.push_back({member.name, "selection", eventMethodName(member.name, "SelectionChange")});
            result.push_back({member.name, "listbox-activate", eventMethodName(member.name, "ItemActivate")});
        }
    }
    return result;
}

std::vector<EventSpec> specsForChannel(
    const std::vector<CompilerMember>& members,
    const std::string& channel)
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

std::string mcpElementMetadataCode(const std::vector<CompilerMember>& members)
{
    if (members.empty())
    {
        return "List.of()";
    }
    std::string result = "List.of(\n";
    for (std::size_t index = 0; index < members.size(); ++index)
    {
        const CompilerMember& member = members[index];
        result += "            new GeneratedElementMetadata("
            + javaString(member.name) + ", "
            + javaString(member.description) + ", "
            + (member.expose ? "true" : "false") + ")";
        result += index + 1 == members.size() ? ")" : ",\n";
    }
    return result;
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
    if (names.empty())
    {
        return "List.of()";
    }
    std::string result = "List.of(\n";
    std::size_t emitted = 0;
    for (const std::string& name : names)
    {
        const YamlMap* tool = valueAsMap(tools.get(name));
        if (tool == nullptr)
        {
            continue;
        }
        const YamlValue* description = tool->get("description");
        const YamlValue* input = tool->get("inputSchema");
        const YamlValue* output = tool->get("outputSchema");
        if (emitted != 0)
        {
            result += ",\n";
        }
        result += "            new GeneratedAppToolMetadata("
            + javaString(name) + ", "
            + javaString(description == nullptr ? "" : valueAsString(*description)) + ", "
            + javaString(input == nullptr ? "{\"type\":\"object\",\"properties\":{}}" : compilerJsonCompact(*input)) + ", "
            + javaString(output == nullptr ? "" : compilerJsonCompact(*output)) + ")";
        ++emitted;
    }
    result += ")";
    return result;
}

void appendDispatch(
    std::vector<std::string>& lines,
    const std::string& methodName,
    const std::string& arguments,
    const std::vector<EventSpec>& specs,
    const std::string& callArguments)
{
    lines.push_back("    @Override");
    lines.push_back("    public boolean " + methodName + "(" + arguments + ")");
    lines.push_back("    {");
    for (const EventSpec& spec : specs)
    {
        lines.push_back("        if (" + javaString(spec.name) + ".equals(name))");
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(" + callArguments + ");");
        lines.push_back("            return true;");
        lines.push_back("        }");
    }
    lines.push_back("        return false;");
    lines.push_back("    }");
    lines.push_back("");
}

std::string generateSource(
    const std::string& baseName,
    const std::string& classNameValue,
    const CompilerDocument& model,
    bool mcpEnabled,
    const std::string& packageName)
{
    const bool scrollviewExtension = lower(trim(model.extends)) == "uiscrollview";
    const int scrollviewGap = compilerScrollviewExtensionGap(model.style);
    const std::string baseClass = scrollviewExtension ? "GeneratedScrollViewBase" : "GeneratedWindowBase";
    const std::string title = model.document.title.empty() ? baseName : model.document.title;
    std::set<std::string> imports{
        "AxisDimension",
        "Color",
        "GeneratedAppToolMetadata",
        "GeneratedElementMetadata",
        "GeneratedLayoutEntry",
        "GeneratedWindowBase",
        "Rect",
        "Size",
        "Style",
        "TextGradient",
    };
    if (scrollviewExtension)
    {
        imports.insert("GeneratedScrollViewBase");
        imports.insert("ScrollView");
    }
    for (const CompilerMember& member : model.members)
    {
        imports.insert(elementType(member.type));
    }
    std::vector<std::string> lines{
        "// Auto-generated UI code for " + baseName + " - DO NOT EDIT MANUALLY.",
    };
    if (!packageName.empty())
    {
        lines.push_back("package " + packageName + ";");
        lines.push_back("");
    }
    lines.push_back("import java.util.List;");
    lines.push_back("");
    for (const std::string& importName : imports)
    {
        lines.push_back("import uimd." + importName + ";");
    }
    lines.push_back("");
    lines.push_back("public class " + classNameValue + " extends " + baseClass);
    lines.push_back("{");

    for (const CompilerMember& member : model.members)
    {
        lines.push_back("    public " + elementType(member.type) + " "
            + fieldIdentifier(member.name) + ";");
    }
    lines.push_back("");
    lines.push_back("    public " + classNameValue + "()");
    lines.push_back("    {");
    lines.push_back("        super(" + javaString(title) + ");");
    lines.push_back("        setGeneratedLayout(buildLayout());");
    lines.push_back("        setGeneratedFocusable(" + std::string{model.focusable ? "true" : "false"} + ");");
    lines.push_back("        setGeneratedKind(" + javaString(lower(trim(model.kind))) + ");");
    lines.push_back("        setMcpMetadata(");
    lines.push_back("            " + std::string{mcpEnabled ? "true" : "false"} + ",");
    lines.push_back("            " + javaString(typeIdentifier(baseName)) + ",");
    lines.push_back("            " + javaString(mcpEnabled ? model.document.sourceText : std::string{}) + ",");
    lines.push_back("            " + javaString(model.description) + ",");
    lines.push_back("            " + mcpElementMetadataCode(model.members) + ");");
    lines.push_back("        setMcpAppTools(" + mcpAppToolsCode(model.mcpTools) + ");");
    const YamlMap windowStyle = compilerWindowStyle(model.style);
    if (!windowStyle.empty())
    {
        lines.push_back("        setGeneratedWindowStyle(" + styleCode(windowStyle) + ");");
    }
    if (scrollviewExtension)
    {
        lines.push_back("        setGeneratedScrollView(addElement(new ScrollView("
            + javaString(GENERATED_SCROLLVIEW_ELEMENT_NAME) + ", "
            + std::to_string(scrollviewGap) + ")));");
        for (const auto& [setter, stateStyle] : compilerScrollviewExtensionStyleStates(model.style))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        scrollView()." + setter + "(" + styleCode(stateStyle) + ");");
            }
        }
        const YamlMap descendantFocusStyle =
            compilerScrollviewExtensionDescendantFocusStyle(model.style);
        if (!descendantFocusStyle.empty())
        {
            lines.push_back("        scrollView().setDescendantFocusStyle("
                + styleCode(descendantFocusStyle) + ");");
        }
    }

    for (const CompilerMember& member : model.members)
    {
        const std::string field = fieldIdentifier(member.name);
        lines.push_back("        " + field + " = addElement(" + constructorCode(member, scrollviewGap) + ");");
        if (!compilerIsBuiltinType(member.type))
        {
            lines.push_back("        " + field + ".setChild(new "
                + typeIdentifier(member.type) + "UI());");
        }
        if (member.hasCommitMode)
        {
            lines.push_back("        " + field + ".setCommitMode(" + javaString(member.commitMode) + ");");
        }
        if (member.type == "listbox" && member.multiple)
        {
            lines.push_back("        " + field + ".setMultiple(true);");
        }
        if (member.type == "combobox" && member.selectedItemIndex >= 0)
        {
            lines.push_back(
                "        " + field + ".setSelectedIndex("
                + std::to_string(member.selectedItemIndex) + ");");
        }
        if (member.type == "listbox")
        {
            if (member.multiple && !member.selectedValues.empty())
            {
                lines.push_back(
                    "        " + field + ".setSelectedValues("
                    + javaStringList(member.selectedValues) + ");");
            }
            else if (member.selectedItemsIndex >= 0)
            {
                lines.push_back(
                    "        " + field + ".setSelectedIndex("
                    + std::to_string(member.selectedItemsIndex) + ");");
            }
        }
        const YamlMap* rawMember = valueAsMap(model.document.members.get(member.name));
        const std::string styleType = member.type == "textarea"
            ? "textinput"
            : (compilerIsBuiltinType(member.type) ? member.type : "uielement");
        for (const auto& [setter, stateStyle] : compilerStyleStatesForElement(
                 model.style,
                 styleType,
                 member.name,
                 cellNameFor(model, member.name),
                 rawMember))
        {
            if (!stateStyle.empty())
            {
                lines.push_back("        " + field + "." + setter + "(" + styleCode(stateStyle) + ");");
            }
        }
    }
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    private static TextGradient makeTextGradient(");
    lines.push_back("        int intervalMs,");
    lines.push_back("        int step,");
    lines.push_back("        int segmentSize,");
    lines.push_back("        String... colors)");
    lines.push_back("    {");
    lines.push_back("        TextGradient gradient = new TextGradient();");
    lines.push_back("        gradient.setIntervalMs(intervalMs);");
    lines.push_back("        gradient.setStep(step);");
    lines.push_back("        gradient.setSegmentSize(segmentSize);");
    lines.push_back("        for (String color : colors)");
    lines.push_back("        {");
    lines.push_back("            gradient.colors().add(new Color(color));");
    lines.push_back("        }");
    lines.push_back("        return gradient;");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    private static Style styleWithGradients(");
    lines.push_back("        Style style,");
    lines.push_back("        TextGradient colorGradient,");
    lines.push_back("        TextGradient backgroundGradient)");
    lines.push_back("    {");
    lines.push_back("        style.setTextColorGradient(colorGradient);");
    lines.push_back("        style.setTextBackgroundGradient(backgroundGradient);");
    lines.push_back("        return style;");
    lines.push_back("    }");
    lines.push_back("");
    lines.push_back("    private static List<GeneratedLayoutEntry> buildLayout()");
    lines.push_back("    {");
    lines.push_back("        return List.of(");
    for (std::size_t index = 0; index < model.document.layout.size(); ++index)
    {
        std::string entry = layoutEntry(
            model.document.layout[index],
            model,
            scrollviewExtension);
        if (index + 1 != model.document.layout.size())
        {
            entry += ",";
        }
        lines.push_back(entry);
    }
    lines.push_back("        );");
    lines.push_back("    }");
    lines.push_back("");

    for (const EventSpec& spec : eventSpecs(model.members))
    {
        if (spec.channel == "listbox-activate")
        {
            lines.push_back("    protected boolean " + spec.methodName + "(int index, String value)");
            lines.push_back("    {");
            lines.push_back("        return false;");
            lines.push_back("    }");
            lines.push_back("");
            continue;
        }
        const bool hasValue = spec.channel != "button";
        const std::string argument = spec.channel == "selection"
            ? "List<String> value"
            : (hasValue ? "String value" : "");
        lines.push_back("    protected void " + spec.methodName + "(" + argument + ")");
        lines.push_back("    {");
        lines.push_back("    }");
        lines.push_back("");
    }
    appendDispatch(
        lines,
        "handleGeneratedButton",
        "String name",
        specsForChannel(model.members, "button"),
        "");
    appendDispatch(
        lines,
        "handleGeneratedTextChanged",
        "String name, String value",
        specsForChannel(model.members, "text"),
        "value");
    appendDispatch(
        lines,
        "handleGeneratedTextConfirmed",
        "String name, String value",
        specsForChannel(model.members, "confirmed"),
        "value");

    lines.push_back("    @Override");
    lines.push_back("    public boolean handleGeneratedSelectionChanged(String name, List<String> value)");
    lines.push_back("    {");
    for (const EventSpec& spec : specsForChannel(model.members, "selection-text"))
    {
        lines.push_back("        if (" + javaString(spec.name) + ".equals(name))");
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value.isEmpty() ? \"\" : value.get(0));");
        lines.push_back("            return true;");
        lines.push_back("        }");
    }
    for (const EventSpec& spec : specsForChannel(model.members, "selection"))
    {
        lines.push_back("        if (" + javaString(spec.name) + ".equals(name))");
        lines.push_back("        {");
        lines.push_back("            " + spec.methodName + "(value);");
        lines.push_back("            return true;");
        lines.push_back("        }");
    }
    lines.push_back("        return false;");
    lines.push_back("    }");
    const std::vector<EventSpec> listboxActivateSpecs =
        specsForChannel(model.members, "listbox-activate");
    if (!listboxActivateSpecs.empty())
    {
        lines.push_back("");
        lines.push_back("    @Override");
        lines.push_back("    public boolean handleGeneratedListBoxItemActivate(");
        lines.push_back("        String name,");
        lines.push_back("        String elementId,");
        lines.push_back("        int index,");
        lines.push_back("        String value)");
        lines.push_back("    {");
        for (const EventSpec& spec : listboxActivateSpecs)
        {
            lines.push_back("        if (" + javaString(spec.name) + ".equals(name))");
            lines.push_back("        {");
            lines.push_back("            return " + spec.methodName + "(index, value);");
            lines.push_back("        }");
        }
        lines.push_back("        return false;");
        lines.push_back("    }");
    }
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
    return head.find("format: ui-md") != std::string::npos
        || head.find("format: uimd") != std::string::npos;
}

std::vector<std::filesystem::path> sourceFiles(const std::filesystem::path& sourcePath)
{
    if (std::filesystem::is_regular_file(sourcePath))
    {
        return {sourcePath};
    }
    std::vector<std::filesystem::path> result;
    for (const std::filesystem::directory_entry& entry :
         std::filesystem::recursive_directory_iterator(sourcePath))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }
        const std::filesystem::path path = entry.path();
        if (path.extension() == ".uimd" || (path.extension() == ".md" && isLegacyUimdFile(path)))
        {
            result.push_back(path);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

bool pathIsWithin(
    const std::filesystem::path& path,
    const std::filesystem::path& root)
{
    std::error_code error;
    const std::filesystem::path relative = std::filesystem::relative(
        absolutePath(path),
        absolutePath(root),
        error);
    if (error || relative.empty())
    {
        return absolutePath(path) == absolutePath(root);
    }
    const auto first = relative.begin();
    return first == relative.end() || *first != "..";
}

std::filesystem::path mappedOutputDir(
    const std::filesystem::path& sourceFile,
    const std::filesystem::path& sourceRoot,
    const std::filesystem::path& outputRoot,
    bool writeInPlace)
{
    if (writeInPlace)
    {
        return absolutePath(sourceFile).parent_path();
    }
    std::error_code error;
    const std::filesystem::path relative = std::filesystem::relative(
        absolutePath(sourceFile).parent_path(),
        sourceRoot,
        error);
    if (error || (!relative.empty() && relative.begin() != relative.end() && *relative.begin() == ".."))
    {
        return outputRoot;
    }
    return (outputRoot / relative).lexically_normal();
}

std::filesystem::path projectRootFor(const std::filesystem::path& sourcePath)
{
    const char* overrideRoot = std::getenv("UIMD_SOURCE_ROOT");
    if (overrideRoot != nullptr && *overrideRoot != '\0')
    {
        return absolutePath(overrideRoot);
    }
    const std::filesystem::path configuredRoot{UIMD_NATIVE_SOURCE_ROOT};
    if (!configuredRoot.empty()
        && std::filesystem::is_regular_file(configuredRoot / "shared" / "themes" / "dark.uimd"))
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

std::string runtimeReferenceFor(const std::filesystem::path& settingsPath)
{
    const char* sdkTarget = std::getenv("UIMD_SDK_JAVA_TARGET");
    if (sdkTarget != nullptr && *sdkTarget != '\0')
    {
        const std::filesystem::path installed = absolutePath(sdkTarget);
        if (std::filesystem::is_regular_file(installed / "settings.gradle"))
        {
            return installed.generic_string();
        }
    }
    const std::filesystem::path root = projectRootFor(settingsPath);
    const std::filesystem::path runtime = root.empty()
        ? std::filesystem::path{"java"}
        : root / "java";
    std::error_code error;
    const std::filesystem::path relative = std::filesystem::relative(
        runtime,
        settingsPath.parent_path(),
        error);
    return !error && !relative.empty() ? relative.generic_string() : runtime.generic_string();
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

std::vector<std::filesystem::path> compileJavaFile(
    const std::filesystem::path& sourcePath,
    const std::filesystem::path& outputDir,
    const std::filesystem::path& mappingSourceRoot,
    const std::filesystem::path& mappingOutputRoot,
    bool writeInPlace,
    bool shouldGenerateAppStub,
    bool mcpEnabled,
    const std::string& packageName,
    std::set<std::filesystem::path>& visited)
{
    const std::filesystem::path absoluteSource = absolutePath(sourcePath);
    if (!visited.insert(absoluteSource).second)
    {
        return {};
    }
    std::filesystem::create_directories(outputDir);
    const CompilerDocument model = parseCompilerDocument(absoluteSource);
    std::vector<std::filesystem::path> generated;
    for (const std::filesystem::path& dependency : compilerDependencyPaths(absoluteSource, model))
    {
        std::vector<std::filesystem::path> dependencyGenerated = compileJavaFile(
            dependency,
            mappedOutputDir(
                dependency,
                mappingSourceRoot,
                mappingOutputRoot,
                writeInPlace),
            mappingSourceRoot,
            mappingOutputRoot,
            writeInPlace,
            false,
            mcpEnabled,
            packageName,
            visited);
        generated.insert(generated.end(), dependencyGenerated.begin(), dependencyGenerated.end());
    }

    const std::string baseName = absoluteSource.stem().string();
    const std::string classNameValue = className(baseName);
    const std::filesystem::path generatedPath = outputDir / (classNameValue + ".java");
    writeTextFile(
        generatedPath,
        generateSource(baseName, classNameValue, model, mcpEnabled, packageName));
    generated.push_back(generatedPath);

    if (shouldGenerateAppStub)
    {
        const std::filesystem::path appPath = outputDir / (appClassName(classNameValue) + ".java");
        if (!std::filesystem::exists(appPath))
        {
            writeTextFile(appPath, javaAppTemplate(classNameValue, packageName));
            generated.push_back(appPath);
        }
    }
    return generated;
}

std::vector<std::filesystem::path> dedupePaths(const std::vector<std::filesystem::path>& paths)
{
    std::set<std::filesystem::path> seen;
    std::vector<std::filesystem::path> result;
    for (const std::filesystem::path& path : paths)
    {
        if (seen.insert(absolutePath(path)).second)
        {
            result.push_back(path);
        }
    }
    return result;
}

}  // namespace

std::string javaBuildFile(const std::string& project)
{
    return "plugins\n"
        "{\n"
        "    id \"application\"\n"
        "}\n"
        "\n"
        "apply from: new File(gradle.ext.uimdRuntimeRoot, \"uimd-java-launchers.gradle\")\n"
        "\n"
        "repositories\n"
        "{\n"
        "    mavenCentral()\n"
        "}\n"
        "\n"
        "java\n"
        "{\n"
        "    toolchain\n"
        "    {\n"
        "        languageVersion = JavaLanguageVersion.of(17)\n"
        "    }\n"
        "}\n"
        "\n"
        "dependencies\n"
        "{\n"
        "    implementation \"org.uimd:uimd:" UIMD_VERSION "\"\n"
        "}\n"
        "\n"
        "application\n"
        "{\n"
        "    mainClass = " + javaString(typeIdentifier(project)) + "\n"
        "}\n"
        "\n"
        "sourceSets\n"
        "{\n"
        "    main\n"
        "    {\n"
        "        java\n"
        "        {\n"
        "            srcDirs = [\".\"]\n"
        "            exclude \"build/**\"\n"
        "        }\n"
        "    }\n"
        "}\n"
        "\n"
        "tasks.withType(JavaCompile).configureEach\n"
        "{\n"
        "    options.encoding = \"UTF-8\"\n"
        "    options.compilerArgs.addAll([\"-Xlint:all\", \"-Werror\"])\n"
        "}\n";
}

std::string javaSettingsFile(
    const std::string& project,
    const std::string& localRuntimeReference)
{
    return "rootProject.name = " + javaString(project) + "\n"
        "\n"
        "def installedRuntime = System.getenv(\"UIMD_SDK_JAVA_TARGET\")\n"
        "def localRuntime = file(" + javaString(localRuntimeReference) + ")\n"
        "def runtimeRoot = installedRuntime ? file(installedRuntime) : localRuntime\n"
        "if (!new File(runtimeRoot, \"settings.gradle\").isFile())\n"
        "{\n"
        "    throw new GradleException(\"UIMD Java runtime was not found. Set UIMD_SDK_JAVA_TARGET or keep the source runtime available.\")\n"
        "}\n"
        "gradle.ext.uimdRuntimeRoot = runtimeRoot.canonicalFile\n"
        "includeBuild(runtimeRoot)\n";
}

std::string javaAppTemplate(
    const std::string& classNameValue,
    const std::string& packageName)
{
    const std::string appName = appClassName(classNameValue);
    const std::string packageDeclaration = packageName.empty()
        ? std::string{}
        : "package " + packageName + ";\n\n";
    return packageDeclaration
        + "import uimd.GeneratedWindowRuntime;\n"
        "\n"
        "public class " + appName + " extends " + classNameValue + "\n"
        "{\n"
        "    public static void main(String[] arguments)\n"
        "    {\n"
        "        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->\n"
        "        {\n"
        "            " + appName + " application = new " + appName + "();\n"
        "            return GeneratedWindowRuntime.runGeneratedWindow(\n"
        "                application,\n"
        "                application.runtimeOptions(),\n"
        "                arguments);\n"
        "        });\n"
        "        System.exit(exitCode);\n"
        "    }\n"
        "}\n";
}

std::vector<std::filesystem::path> generateJavaSources(
    const std::filesystem::path& sourcePath,
    const NativeJavaGenerateOptions& options)
{
    std::vector<std::filesystem::path> generated;
    const std::vector<std::filesystem::path> files = sourceFiles(sourcePath);
    const bool autoBootstrap = files.size() == 1;
    const std::filesystem::path absoluteSource = absolutePath(sourcePath);
    std::filesystem::path mappingSourceRoot = std::filesystem::is_directory(absoluteSource)
        ? absoluteSource
        : absoluteSource.parent_path();
    std::filesystem::path mappingOutputRoot = options.hasOutputDir
        ? absolutePath(options.outputDir)
        : mappingSourceRoot;
    const std::filesystem::path projectRoot = projectRootFor(absoluteSource);
    if (options.hasOutputDir && !projectRoot.empty())
    {
        const std::filesystem::path pythonRoot = projectRoot / "python";
        const std::filesystem::path javaRoot = projectRoot / "java";
        if (pathIsWithin(absoluteSource, pythonRoot) && pathIsWithin(mappingOutputRoot, javaRoot))
        {
            mappingSourceRoot = pythonRoot;
            mappingOutputRoot = javaRoot;
        }
    }
    for (const std::filesystem::path& sourceFile : files)
    {
        std::set<std::filesystem::path> visited;
        const std::filesystem::path outputDir = mappedOutputDir(
            sourceFile,
            mappingSourceRoot,
            mappingOutputRoot,
            !options.hasOutputDir);
        const bool bootstrap = options.generateAppStub || autoBootstrap;
        std::vector<std::filesystem::path> result = compileJavaFile(
            sourceFile,
            outputDir,
            mappingSourceRoot,
            mappingOutputRoot,
            !options.hasOutputDir,
            bootstrap,
            options.mcpEnabled,
            options.packageName,
            visited);
        generated.insert(generated.end(), result.begin(), result.end());
        if (bootstrap)
        {
            const std::filesystem::path buildPath = outputDir / "build.gradle";
            const std::filesystem::path settingsPath = outputDir / "settings.gradle";
            if (!std::filesystem::exists(buildPath))
            {
                writeTextFile(buildPath, javaBuildFile(sourceFile.stem().string()));
                generated.push_back(buildPath);
            }
            if (!std::filesystem::exists(settingsPath))
            {
                writeTextFile(
                    settingsPath,
                    javaSettingsFile(sourceFile.stem().string(), runtimeReferenceFor(settingsPath)));
                generated.push_back(settingsPath);
            }
        }
    }
    return dedupePaths(generated);
}

}  // namespace uimd::tool

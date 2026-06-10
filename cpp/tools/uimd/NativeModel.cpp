#include "NativeModel.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace uimd::tool
{
namespace
{

constexpr const char* DEFAULT_EXTENDS = "uiwindow";

std::string readTextFile(const std::string& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        throw std::runtime_error("cannot read file: " + path);
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
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

bool startsWith(const std::string& value, const std::string& prefix)
{
    return value.rfind(prefix, 0) == 0;
}

std::string utf8FromCodepoint(unsigned int codepoint)
{
    std::string result;
    if (codepoint <= 0x7F)
    {
        result += static_cast<char>(codepoint);
    }
    else if (codepoint <= 0x7FF)
    {
        result += static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0xFFFF)
    {
        result += static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else
    {
        result += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
        result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    return result;
}

int hexDigitValue(char ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return ch - '0';
    }
    if ('a' <= ch && ch <= 'f')
    {
        return 10 + ch - 'a';
    }
    if ('A' <= ch && ch <= 'F')
    {
        return 10 + ch - 'A';
    }
    return -1;
}

bool parseHexCodepoint(const std::string& value, std::size_t start, std::size_t count, unsigned int& codepoint)
{
    if (start + count > value.size())
    {
        return false;
    }
    unsigned int parsed = 0;
    for (std::size_t index = 0; index < count; ++index)
    {
        const int digit = hexDigitValue(value[start + index]);
        if (digit < 0)
        {
            return false;
        }
        parsed = (parsed << 4U) | static_cast<unsigned int>(digit);
    }
    codepoint = parsed;
    return true;
}

std::vector<std::string> splitLines(const std::string& text)
{
    std::vector<std::string> lines;
    std::string current;
    std::istringstream input(text);
    while (std::getline(input, current))
    {
        if (!current.empty() && current.back() == '\r')
        {
            current.pop_back();
        }
        lines.push_back(current);
    }
    return lines;
}

int indentOf(const std::string& line)
{
    int indent = 0;
    while (indent < static_cast<int>(line.size()) && line[static_cast<std::size_t>(indent)] == ' ')
    {
        ++indent;
    }
    return indent;
}

bool ignoredYamlLine(const std::string& line)
{
    const std::string stripped = trim(line);
    return stripped.empty() || startsWith(stripped, "#");
}

std::size_t skipEmptyYamlLines(const std::vector<std::string>& lines, std::size_t index)
{
    while (index < lines.size() && ignoredYamlLine(lines[index]))
    {
        ++index;
    }
    return index;
}

std::string stripQuotedKey(std::string key)
{
    key = trim(key);
    if (key.size() >= 2 && ((key.front() == '"' && key.back() == '"') || (key.front() == '\'' && key.back() == '\'')))
    {
        return key.substr(1, key.size() - 2);
    }
    return key;
}

std::string unquoteString(const std::string& value)
{
    if (value.size() < 2)
    {
        return value;
    }
    const char quote = value.front();
    if (!((quote == '"' || quote == '\'') && value.back() == quote))
    {
        return value;
    }
    std::string result;
    for (std::size_t index = 1; index + 1 < value.size(); ++index)
    {
        char ch = value[index];
        if (quote == '"' && ch == '\\' && index + 1 < value.size() - 1)
        {
            char next = value[++index];
            if (next == 'n')
            {
                result += '\n';
            }
            else if (next == 't')
            {
                result += '\t';
            }
            else if (next == 'r')
            {
                result += '\r';
            }
            else if (next == 'a')
            {
                result += '\a';
            }
            else if (next == 'b')
            {
                result += '\b';
            }
            else if (next == 'f')
            {
                result += '\f';
            }
            else if (next == 'v')
            {
                result += '\v';
            }
            else if (next == 'x')
            {
                unsigned int codepoint = 0;
                if (parseHexCodepoint(value, index + 1, 2, codepoint))
                {
                    result += static_cast<char>(codepoint);
                    index += 2;
                }
                else
                {
                    result += next;
                }
            }
            else if (next == 'u')
            {
                unsigned int codepoint = 0;
                if (parseHexCodepoint(value, index + 1, 4, codepoint))
                {
                    result += utf8FromCodepoint(codepoint);
                    index += 4;
                }
                else
                {
                    result += next;
                }
            }
            else if (next == 'U')
            {
                unsigned int codepoint = 0;
                if (parseHexCodepoint(value, index + 1, 8, codepoint))
                {
                    result += utf8FromCodepoint(codepoint);
                    index += 8;
                }
                else
                {
                    result += next;
                }
            }
            else
            {
                result += next;
            }
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

YamlValue parseYamlScalar(std::string value);

YamlValue parseInlineList(const std::string& value)
{
    YamlList list;
    std::string inner = trim(value.substr(1, value.size() - 2));
    if (inner.empty())
    {
        return YamlValue{list};
    }

    std::string item;
    char quote = '\0';
    int bracketDepth = 0;
    for (char ch : inner)
    {
        if (quote != '\0')
        {
            item += ch;
            if (ch == quote)
            {
                quote = '\0';
            }
            continue;
        }
        if (ch == '"' || ch == '\'')
        {
            quote = ch;
            item += ch;
            continue;
        }
        if (ch == '[' || ch == '{')
        {
            ++bracketDepth;
            item += ch;
            continue;
        }
        if (ch == ']' || ch == '}')
        {
            --bracketDepth;
            item += ch;
            continue;
        }
        if (ch == ',' && bracketDepth == 0)
        {
            const std::string stripped = trim(item);
            if (!stripped.empty())
            {
                list.push_back(parseYamlScalar(stripped));
            }
            item.clear();
            continue;
        }
        item += ch;
    }

    const std::string stripped = trim(item);
    if (!stripped.empty())
    {
        list.push_back(parseYamlScalar(stripped));
    }
    return YamlValue{list};
}

bool fullMatch(const std::string& value, const std::regex& pattern)
{
    return std::regex_match(value, pattern);
}

YamlValue parseYamlScalar(std::string value)
{
    value = trim(value);
    if (!value.empty() && value.back() == ';')
    {
        value.pop_back();
        value = trim(value);
    }
    if (value == "{}")
    {
        return YamlValue{YamlMap{}};
    }
    if (value == "[]")
    {
        return YamlValue{YamlList{}};
    }

    const std::string lowered = lower(value);
    if (lowered == "true" || lowered == "yes" || lowered == "on")
    {
        return YamlValue{true};
    }
    if (lowered == "false" || lowered == "no" || lowered == "off")
    {
        return YamlValue{false};
    }
    if (lowered == "null" || lowered == "none")
    {
        return YamlValue{nullptr};
    }
    if (value.size() >= 2 && value.front() == '[' && value.back() == ']')
    {
        return parseInlineList(value);
    }
    if (value.size() >= 2 && ((value.front() == '"' && value.back() == '"') || (value.front() == '\'' && value.back() == '\'')))
    {
        return YamlValue{unquoteString(value)};
    }

    static const std::regex intPattern{R"([-+]?\d+)"};
    static const std::regex floatPattern{R"([-+]?\d+\.\d+)"};
    if (fullMatch(value, intPattern))
    {
        return YamlValue{std::stoll(value)};
    }
    if (fullMatch(value, floatPattern))
    {
        return YamlValue{std::stod(value)};
    }
    return YamlValue{value};
}

std::pair<YamlValue, std::size_t> parseYamlNode(
    const std::vector<std::string>& lines,
    std::size_t index,
    int indent
);

std::pair<YamlValue, std::size_t> parseYamlDict(
    const std::vector<std::string>& lines,
    std::size_t index,
    int indent
)
{
    YamlMap result;
    while (index < lines.size())
    {
        if (ignoredYamlLine(lines[index]))
        {
            ++index;
            continue;
        }
        const int currentIndent = indentOf(lines[index]);
        if (currentIndent < indent)
        {
            break;
        }
        if (currentIndent > indent)
        {
            throw std::runtime_error("invalid indentation in YAML block");
        }

        const std::string stripped = trim(lines[index]);
        if (startsWith(stripped, "-"))
        {
            break;
        }
        const std::size_t separator = stripped.find(':');
        if (separator == std::string::npos)
        {
            throw std::runtime_error("invalid mapping entry in YAML block: " + stripped);
        }
        const std::string key = stripQuotedKey(stripped.substr(0, separator));
        if (result.find(key) != result.end())
        {
            throw std::runtime_error("duplicate key in YAML block: " + key);
        }
        std::string value = trim(stripped.substr(separator + 1));
        ++index;
        if (!value.empty())
        {
            result[key] = parseYamlScalar(value);
            continue;
        }

        const std::size_t nextIndex = skipEmptyYamlLines(lines, index);
        if (nextIndex >= lines.size() || indentOf(lines[nextIndex]) <= currentIndent)
        {
            result[key] = YamlValue{YamlMap{}};
            index = nextIndex;
            continue;
        }
        auto [child, childEnd] = parseYamlNode(lines, nextIndex, indentOf(lines[nextIndex]));
        result[key] = child;
        index = childEnd;
    }
    return {YamlValue{result}, index};
}

bool looksLikeInlineMapping(const std::string& value)
{
    if (value.find(':') == std::string::npos)
    {
        return false;
    }
    if (!value.empty() && (value.front() == '"' || value.front() == '\'' || value.front() == '[' || value.front() == '{'))
    {
        return false;
    }
    const std::string key = trim(value.substr(0, value.find(':')));
    static const std::regex keyPattern{R"([A-Za-z_][A-Za-z0-9_-]*)"};
    return !key.empty() && std::regex_match(key, keyPattern);
}

std::pair<YamlValue, std::size_t> parseYamlList(
    const std::vector<std::string>& lines,
    std::size_t index,
    int indent
)
{
    YamlList result;
    while (index < lines.size())
    {
        if (ignoredYamlLine(lines[index]))
        {
            ++index;
            continue;
        }
        const int currentIndent = indentOf(lines[index]);
        if (currentIndent < indent)
        {
            break;
        }
        if (currentIndent > indent)
        {
            throw std::runtime_error("invalid list indentation in YAML block");
        }

        std::string stripped = trim(lines[index]);
        if (!startsWith(stripped, "-"))
        {
            break;
        }
        std::string value = trim(stripped.substr(1));
        ++index;
        if (!value.empty())
        {
            if (looksLikeInlineMapping(value))
            {
                const std::size_t separator = value.find(':');
                YamlMap item;
                item[stripQuotedKey(value.substr(0, separator))] = parseYamlScalar(trim(value.substr(separator + 1)));
                const std::size_t nextIndex = skipEmptyYamlLines(lines, index);
                if (nextIndex < lines.size() && indentOf(lines[nextIndex]) > currentIndent)
                {
                    auto [child, childEnd] = parseYamlNode(lines, nextIndex, indentOf(lines[nextIndex]));
                    if (!std::holds_alternative<YamlMap>(child.value))
                    {
                        throw std::runtime_error("inline list mapping must continue with a mapping");
                    }
                    for (const auto& [key, childValue] : std::get<YamlMap>(child.value))
                    {
                        if (item.find(key) != item.end())
                        {
                            throw std::runtime_error("duplicate key in YAML block: " + key);
                        }
                        item[key] = childValue;
                    }
                    index = childEnd;
                }
                else
                {
                    index = nextIndex;
                }
                result.push_back(YamlValue{item});
                continue;
            }
            result.push_back(parseYamlScalar(value));
            continue;
        }

        const std::size_t nextIndex = skipEmptyYamlLines(lines, index);
        if (nextIndex >= lines.size() || indentOf(lines[nextIndex]) <= currentIndent)
        {
            result.push_back(YamlValue{nullptr});
            index = nextIndex;
            continue;
        }
        auto [child, childEnd] = parseYamlNode(lines, nextIndex, indentOf(lines[nextIndex]));
        result.push_back(child);
        index = childEnd;
    }
    return {YamlValue{result}, index};
}

std::pair<YamlValue, std::size_t> parseYamlNode(
    const std::vector<std::string>& lines,
    std::size_t index,
    int indent
)
{
    const std::string stripped = trim(lines[index]);
    if (startsWith(stripped, "-"))
    {
        return parseYamlList(lines, index, indent);
    }
    return parseYamlDict(lines, index, indent);
}

YamlMap parseYamlMapping(const std::string& content)
{
    const std::vector<std::string> lines = splitLines(content);
    for (const std::string& line : lines)
    {
        if (line.find('\t') != std::string::npos)
        {
            throw std::runtime_error("YAML block cannot contain tabs");
        }
    }
    std::size_t index = skipEmptyYamlLines(lines, 0);
    if (index >= lines.size())
    {
        return {};
    }
    auto [node, endIndex] = parseYamlNode(lines, index, indentOf(lines[index]));
    const std::size_t trailing = skipEmptyYamlLines(lines, endIndex);
    if (trailing < lines.size())
    {
        throw std::runtime_error("invalid trailing YAML content");
    }
    if (!std::holds_alternative<YamlMap>(node.value))
    {
        throw std::runtime_error("YAML block must contain a mapping");
    }
    return std::get<YamlMap>(node.value);
}

std::string extractH1(const std::string& content)
{
    const std::vector<std::string> lines = splitLines(content);
    for (const std::string& line : lines)
    {
        if (startsWith(line, "# ") && !startsWith(line, "## "))
        {
            return trim(line.substr(2));
        }
    }
    return {};
}

std::map<std::string, std::string> extractH2Sections(const std::string& content)
{
    static const std::map<std::string, std::string> allowed{
        {"metadata", "Metadata"},
        {"definition", "Definition"},
        {"members", "Members"},
        {"tools", "Tools"},
        {"style", "Style"},
        {"user interface", "User Interface"},
    };

    std::map<std::string, std::string> sections;
    std::regex headingPattern{R"(^##\s+(.+?)\s*$)", std::regex::multiline};
    std::vector<std::pair<std::smatch, std::size_t>> matches;
    for (std::sregex_iterator it(content.begin(), content.end(), headingPattern), end; it != end; ++it)
    {
        matches.push_back({*it, static_cast<std::size_t>(it->position())});
    }

    for (std::size_t index = 0; index < matches.size(); ++index)
    {
        const std::string rawName = trim(matches[index].first[1].str());
        const auto allowedIt = allowed.find(lower(rawName));
        if (allowedIt == allowed.end())
        {
            throw std::runtime_error("unknown Markdown UI section: " + rawName);
        }
        const std::string canonical = allowedIt->second;
        if (sections.find(canonical) != sections.end())
        {
            throw std::runtime_error("duplicate Markdown UI section: " + canonical);
        }
        const std::size_t start = matches[index].second + static_cast<std::size_t>(matches[index].first.length());
        const std::size_t end = index + 1 < matches.size() ? matches[index + 1].second : content.size();
        sections[canonical] = content.substr(start, end - start);
    }
    return sections;
}

std::string firstFencedBlock(const std::string& section, const std::vector<std::string>& languages)
{
    std::string languagePattern;
    for (std::size_t index = 0; index < languages.size(); ++index)
    {
        if (index != 0)
        {
            languagePattern += "|";
        }
        languagePattern += languages[index];
    }
    std::regex pattern{"```(?:" + languagePattern + R"()\s*\n([\s\S]*?)\n```)", std::regex::icase};
    std::smatch match;
    if (std::regex_search(section, match, pattern))
    {
        return match[1].str();
    }
    return {};
}

YamlMap optionalYamlSection(const std::map<std::string, std::string>& sections, const std::string& name)
{
    const auto it = sections.find(name);
    if (it == sections.end())
    {
        return {};
    }
    const std::string block = firstFencedBlock(it->second, {"yaml", "yml"});
    if (block.empty())
    {
        throw std::runtime_error("## " + name + " must contain a fenced yaml block");
    }
    return parseYamlMapping(block);
}

YamlMap requiredYamlSection(const std::map<std::string, std::string>& sections, const std::string& name)
{
    if (sections.find(name) == sections.end())
    {
        throw std::runtime_error("missing required ## " + name + " section");
    }
    return optionalYamlSection(sections, name);
}

std::string yamlString(const YamlMap& map, const std::string& key, const std::string& fallback = "")
{
    const auto it = map.find(key);
    if (it == map.end())
    {
        return fallback;
    }
    if (std::holds_alternative<std::string>(it->second.value))
    {
        return std::get<std::string>(it->second.value);
    }
    if (std::holds_alternative<long long>(it->second.value))
    {
        return std::to_string(std::get<long long>(it->second.value));
    }
    return fallback;
}

void appendJsonString(std::ostringstream& output, const std::string& value)
{
    output << '"';
    for (char ch : value)
    {
        if (ch == '\\')
        {
            output << "\\\\";
        }
        else if (ch == '"')
        {
            output << "\\\"";
        }
        else if (ch == '\n')
        {
            output << "\\n";
        }
        else
        {
            output << ch;
        }
    }
    output << '"';
}

void appendJsonValue(std::ostringstream& output, const YamlValue& value);

void appendJsonList(std::ostringstream& output, const YamlList& list)
{
    output << '[';
    for (std::size_t index = 0; index < list.size(); ++index)
    {
        if (index != 0)
        {
            output << ',';
        }
        appendJsonValue(output, list[index]);
    }
    output << ']';
}

void appendJsonMap(std::ostringstream& output, const YamlMap& map)
{
    output << '{';
    bool first = true;
    for (const auto& [key, value] : map)
    {
        if (!first)
        {
            output << ',';
        }
        first = false;
        appendJsonString(output, key);
        output << ':';
        appendJsonValue(output, value);
    }
    output << '}';
}

void appendJsonValue(std::ostringstream& output, const YamlValue& value)
{
    if (std::holds_alternative<std::nullptr_t>(value.value))
    {
        output << "null";
    }
    else if (std::holds_alternative<bool>(value.value))
    {
        output << (std::get<bool>(value.value) ? "true" : "false");
    }
    else if (std::holds_alternative<long long>(value.value))
    {
        output << std::get<long long>(value.value);
    }
    else if (std::holds_alternative<double>(value.value))
    {
        output << std::get<double>(value.value);
    }
    else if (std::holds_alternative<std::string>(value.value))
    {
        appendJsonString(output, std::get<std::string>(value.value));
    }
    else if (std::holds_alternative<YamlList>(value.value))
    {
        appendJsonList(output, std::get<YamlList>(value.value));
    }
    else
    {
        appendJsonMap(output, std::get<YamlMap>(value.value));
    }
}

void appendMapKeys(std::ostringstream& output, const YamlMap& map)
{
    std::vector<std::string> keys;
    keys.reserve(map.size());
    for (const auto& [key, value] : map)
    {
        (void)value;
        keys.push_back(key);
    }
    std::sort(keys.begin(), keys.end());

    output << '[';
    for (std::size_t index = 0; index < keys.size(); ++index)
    {
        if (index != 0)
        {
            output << ',';
        }
        appendJsonString(output, keys[index]);
    }
    output << ']';
}

}  // namespace

bool YamlValue::isMap() const
{
    return std::holds_alternative<YamlMap>(value);
}

const YamlMap& YamlValue::asMap() const
{
    return std::get<YamlMap>(value);
}

bool YamlMap::empty() const
{
    return entries.empty();
}

std::size_t YamlMap::size() const
{
    return entries.size();
}

YamlMap::iterator YamlMap::begin()
{
    return entries.begin();
}

YamlMap::iterator YamlMap::end()
{
    return entries.end();
}

YamlMap::const_iterator YamlMap::begin() const
{
    return entries.begin();
}

YamlMap::const_iterator YamlMap::end() const
{
    return entries.end();
}

YamlMap::iterator YamlMap::find(const std::string& key)
{
    return std::find_if(entries.begin(), entries.end(), [&key](const Entry& entry) {
        return entry.first == key;
    });
}

YamlMap::const_iterator YamlMap::find(const std::string& key) const
{
    return std::find_if(entries.begin(), entries.end(), [&key](const Entry& entry) {
        return entry.first == key;
    });
}

bool YamlMap::contains(const std::string& key) const
{
    return find(key) != end();
}

YamlValue& YamlMap::operator[](const std::string& key)
{
    auto it = find(key);
    if (it != end())
    {
        return it->second;
    }
    entries.push_back({key, YamlValue{}});
    return entries.back().second;
}

const YamlValue* YamlMap::get(const std::string& key) const
{
    auto it = find(key);
    if (it == end())
    {
        return nullptr;
    }
    return &it->second;
}

NativeDocument parseDocumentFile(const std::string& path)
{
    const std::string content = readTextFile(path);
    if (startsWith(trim(content), "---"))
    {
        throw std::runtime_error("YAML frontmatter UI files are no longer supported");
    }

    NativeDocument document;
    document.sourceText = content;
    document.title = extractH1(content);
    if (document.title.empty())
    {
        throw std::runtime_error("missing required H1 title");
    }

    const std::map<std::string, std::string> sections = extractH2Sections(content);
    document.metadata = requiredYamlSection(sections, "Metadata");
    document.definition = optionalYamlSection(sections, "Definition");
    document.members = optionalYamlSection(sections, "Members");
    document.tools = optionalYamlSection(sections, "Tools");
    document.style = optionalYamlSection(sections, "Style");

    const auto uiIt = sections.find("User Interface");
    if (uiIt != sections.end())
    {
        document.uiText = firstFencedBlock(uiIt->second, {"ui"});
    }

    const std::string kind = lower(yamlString(document.metadata, "kind", "window"));
    if (kind != "theme" && document.uiText.empty())
    {
        throw std::runtime_error("missing required ## User Interface section");
    }
    document.layout = parseGrid(document.uiText);
    return document;
}

std::string inspectDocumentJson(const NativeDocument& document)
{
    std::ostringstream output;
    const std::string kind = lower(yamlString(document.metadata, "kind", "window"));
    const std::string extends = lower(yamlString(document.definition, "extends", DEFAULT_EXTENDS));

    output << '{';
    output << "\"definition\":";
    appendJsonMap(output, document.definition);
    output << ",\"extends\":";
    appendJsonString(output, extends);
    output << ",\"kind\":";
    appendJsonString(output, kind);
    output << ",\"layout_item_count\":";
    output << document.layout.size();
    output << ",\"layout_member_names\":";
    output << '[';
    for (std::size_t index = 0; index < document.layout.size(); ++index)
    {
        if (index != 0)
        {
            output << ',';
        }
        appendJsonString(output, document.layout[index].content);
    }
    output << ']';
    output << ",\"member_names\":";
    appendMapKeys(output, document.members);
    output << ",\"metadata\":";
    appendJsonMap(output, document.metadata);
    output << ",\"style_selectors\":";
    appendMapKeys(output, document.style);
    output << ",\"title\":";
    appendJsonString(output, document.title);
    output << ",\"tool_names\":";
    appendMapKeys(output, document.tools);
    output << ",\"ui_text_length\":";
    output << document.uiText.size();
    output << '}';
    return output.str();
}

}  // namespace uimd::tool

#include "IssueReport.hpp"

#include "NativeModel.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace uimd::tool
{
namespace
{

constexpr int PREFIX_ALIAS_SEARCH_LIMIT = 10000;
constexpr int ALPHABETIC_ALIAS_SEARCH_LIMIT = 100000;
constexpr std::uint64_t ALPHABET_SIZE = 26U;
constexpr std::size_t MIN_MARKDOWN_FENCE_LENGTH = 4;

struct NameReplacement
{
    std::string original;
    std::string replacement;
    std::string type;
    bool anonymized = false;
};

struct LinePart
{
    std::string content;
    std::string newline;
};

std::string readTextFile(const std::filesystem::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        throw std::runtime_error("cannot read file: " + path.string());
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

int indentation(const std::string& line)
{
    int count = 0;
    while (count < static_cast<int>(line.size()) && line[static_cast<std::size_t>(count)] == ' ')
    {
        ++count;
    }
    return count;
}

std::string yamlScalar(const YamlValue* value)
{
    if (value == nullptr)
    {
        return {};
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
        output << std::get<double>(value->value);
        return output.str();
    }
    if (std::holds_alternative<bool>(value->value))
    {
        return std::get<bool>(value->value) ? "true" : "false";
    }
    return {};
}

std::string memberType(const YamlValue& member)
{
    if (!std::holds_alternative<YamlMap>(member.value))
    {
        return {};
    }
    const YamlMap& map = std::get<YamlMap>(member.value);
    return lower(yamlScalar(map.get("type")));
}

std::string friendlyPrefixForType(const std::string& type)
{
    if (type == "textinput")
    {
        return "input";
    }
    if (type == "numberinput")
    {
        return "number";
    }
    if (type == "textarea")
    {
        return "text";
    }
    if (type == "checkbox")
    {
        return "check";
    }
    if (type == "combobox")
    {
        return "combo";
    }
    if (type == "listbox")
    {
        return "list";
    }
    if (type == "infolabel")
    {
        return "info";
    }
    if (type.empty())
    {
        return "item";
    }
    std::string prefix;
    for (char ch : type)
    {
        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')
        {
            prefix.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
    }
    return prefix.empty() ? "item" : prefix;
}

std::string shortPrefixForType(const std::string& type)
{
    const std::string friendly = friendlyPrefixForType(type);
    return friendly.empty() ? "i" : friendly.substr(0, 1);
}

std::string numberedCandidate(const std::string& prefix, int ordinal, std::size_t width)
{
    if (prefix.empty() || width <= prefix.size())
    {
        return {};
    }
    const std::string number = std::to_string(ordinal);
    const std::size_t numberWidth = width - prefix.size();
    if (number.size() > numberWidth)
    {
        return {};
    }
    return prefix + std::string(numberWidth - number.size(), '0') + number;
}

std::string alphabeticCandidate(int ordinal, std::size_t width)
{
    if (width == 0)
    {
        return {};
    }

    std::uint64_t capacity = 1;
    for (std::size_t index = 0; index < width; ++index)
    {
        capacity *= ALPHABET_SIZE;
    }
    if (static_cast<std::uint64_t>(ordinal) > capacity)
    {
        return {};
    }

    std::uint64_t value = static_cast<std::uint64_t>(ordinal - 1);
    std::string result(width, 'a');
    for (std::size_t index = 0; index < width; ++index)
    {
        const std::size_t target = width - index - 1;
        result[target] = static_cast<char>('a' + (value % ALPHABET_SIZE));
        value /= ALPHABET_SIZE;
    }
    return result;
}

std::string uniqueReplacement(
    const std::string& original,
    const std::string& type,
    int ordinal,
    std::set<std::string>& used
)
{
    const std::size_t width = original.size();
    if (width == 0)
    {
        return original;
    }

    const std::vector<std::string> prefixes{
        friendlyPrefixForType(type),
        shortPrefixForType(type),
    };
    for (const std::string& prefix : prefixes)
    {
        for (int offset = 0; offset < PREFIX_ALIAS_SEARCH_LIMIT; ++offset)
        {
            const std::string candidate = numberedCandidate(prefix, ordinal + offset, width);
            if (!candidate.empty() && used.find(candidate) == used.end())
            {
                used.insert(candidate);
                return candidate;
            }
        }
    }

    for (int offset = 0; offset < ALPHABETIC_ALIAS_SEARCH_LIMIT; ++offset)
    {
        const std::string candidate = alphabeticCandidate(ordinal + offset, width);
        if (!candidate.empty() && used.find(candidate) == used.end())
        {
            used.insert(candidate);
            return candidate;
        }
    }

    used.insert(original);
    return original;
}

std::vector<NameReplacement> buildNameReplacements(const NativeDocument& document, bool anonymizeNames)
{
    std::vector<NameReplacement> replacements;
    replacements.reserve(document.members.size());
    std::set<std::string> used;
    for (const auto& [name, value] : document.members.entries)
    {
        (void)value;
        used.insert(name);
    }

    int ordinal = 1;
    for (const auto& [name, value] : document.members.entries)
    {
        NameReplacement replacement;
        replacement.original = name;
        replacement.type = memberType(value);
        if (anonymizeNames)
        {
            replacement.replacement = uniqueReplacement(name, replacement.type, ordinal, used);
            replacement.anonymized = replacement.replacement != replacement.original;
        }
        else
        {
            replacement.replacement = name;
        }
        replacements.push_back(replacement);
        ++ordinal;
    }

    std::sort(replacements.begin(), replacements.end(), [](const NameReplacement& left, const NameReplacement& right) {
        if (left.original.size() != right.original.size())
        {
            return left.original.size() > right.original.size();
        }
        return left.original < right.original;
    });
    return replacements;
}

bool isIdentifierChar(char ch)
{
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

bool replacementBoundary(const std::string& line, std::size_t start, std::size_t length)
{
    const bool leftOk = start == 0 || !isIdentifierChar(line[start - 1]);
    const std::size_t end = start + length;
    const bool rightOk = end >= line.size() || !isIdentifierChar(line[end]);
    return leftOk && rightOk;
}

std::string replaceNamesOutsideUi(std::string line, const std::vector<NameReplacement>& replacements)
{
    for (const NameReplacement& replacement : replacements)
    {
        if (!replacement.anonymized)
        {
            continue;
        }
        std::size_t position = 0;
        while ((position = line.find(replacement.original, position)) != std::string::npos)
        {
            if (!replacementBoundary(line, position, replacement.original.size()))
            {
                position += replacement.original.size();
                continue;
            }
            line.replace(position, replacement.original.size(), replacement.replacement);
            position += replacement.replacement.size();
        }
    }
    return line;
}

std::string replaceNamesInUi(std::string line, const std::vector<NameReplacement>& replacements)
{
    std::size_t position = 0;
    bool quoted = false;
    while (position < line.size())
    {
        if (line[position] == '"')
        {
            quoted = !quoted;
            ++position;
            continue;
        }
        if (quoted)
        {
            ++position;
            continue;
        }

        bool replaced = false;
        for (const NameReplacement& replacement : replacements)
        {
            if (!replacement.anonymized)
            {
                continue;
            }
            if (replacement.original.size() != replacement.replacement.size())
            {
                continue;
            }
            if (position + replacement.original.size() > line.size())
            {
                continue;
            }
            if (line.compare(position, replacement.original.size(), replacement.original) != 0)
            {
                continue;
            }
            if (!replacementBoundary(line, position, replacement.original.size()))
            {
                continue;
            }
            line.replace(position, replacement.original.size(), replacement.replacement);
            position += replacement.replacement.size();
            replaced = true;
            break;
        }
        if (!replaced)
        {
            ++position;
        }
    }
    return line;
}

std::vector<LinePart> splitLinesPreservingEndings(const std::string& text)
{
    std::vector<LinePart> lines;
    std::size_t start = 0;
    while (start < text.size())
    {
        const std::size_t newline = text.find('\n', start);
        if (newline == std::string::npos)
        {
            lines.push_back(LinePart{text.substr(start), ""});
            break;
        }
        lines.push_back(LinePart{text.substr(start, newline - start), "\n"});
        start = newline + 1;
    }
    if (text.empty())
    {
        return {};
    }
    return lines;
}

bool isFenceStart(const std::string& line, const std::string& language)
{
    const std::string stripped = lower(trim(line));
    return stripped == "```" + language || startsWith(stripped, "```" + language + " ");
}

bool isFenceEnd(const std::string& line)
{
    return trim(line) == "```";
}

bool isDescriptionKey(const std::string& line, int& indent, std::string& value)
{
    indent = indentation(line);
    const std::string stripped = trim(line);
    if (!startsWith(stripped, "description:"))
    {
        return false;
    }
    value = trim(stripped.substr(std::string{"description:"}.size()));
    return true;
}

bool isBlockScalarValue(const std::string& value)
{
    return startsWith(value, "|") || startsWith(value, ">");
}

std::string sanitizedSourceText(
    const NativeDocument& document,
    const std::vector<NameReplacement>& replacements,
    const IssueReportOptions& options
)
{
    std::ostringstream output;
    bool inUiFence = false;
    int skippedDescriptionIndent = -1;

    for (const LinePart& part : splitLinesPreservingEndings(document.sourceText))
    {
        std::string line = part.content;
        const std::string stripped = trim(line);

        if (inUiFence)
        {
            if (isFenceEnd(line))
            {
                inUiFence = false;
                output << line << part.newline;
                continue;
            }
            output << replaceNamesInUi(line, replacements) << part.newline;
            continue;
        }

        if (isFenceStart(line, "ui"))
        {
            inUiFence = true;
            output << line << part.newline;
            continue;
        }

        if (!options.keepDescriptions && skippedDescriptionIndent >= 0)
        {
            const bool blank = stripped.empty();
            if (blank || indentation(line) > skippedDescriptionIndent)
            {
                continue;
            }
            skippedDescriptionIndent = -1;
        }

        if (!options.keepDescriptions)
        {
            int descriptionIndent = 0;
            std::string descriptionValue;
            if (isDescriptionKey(line, descriptionIndent, descriptionValue))
            {
                if (isBlockScalarValue(descriptionValue))
                {
                    skippedDescriptionIndent = descriptionIndent;
                }
                continue;
            }
        }

        output << replaceNamesOutsideUi(line, replacements) << part.newline;
    }

    return output.str();
}

bool layoutGeometryMatches(const NativeDocument& left, const NativeDocument& right)
{
    if (left.layout.size() != right.layout.size())
    {
        return false;
    }

    std::vector<NativeLayoutItem> leftLayout = left.layout;
    std::vector<NativeLayoutItem> rightLayout = right.layout;
    auto geometryLess = [](const NativeLayoutItem& a, const NativeLayoutItem& b) {
        if (a.row != b.row)
        {
            return a.row < b.row;
        }
        if (a.col != b.col)
        {
            return a.col < b.col;
        }
        if (a.cellRow != b.cellRow)
        {
            return a.cellRow < b.cellRow;
        }
        if (a.cellCol != b.cellCol)
        {
            return a.cellCol < b.cellCol;
        }
        if (a.cellCharsWidth != b.cellCharsWidth)
        {
            return a.cellCharsWidth < b.cellCharsWidth;
        }
        if (a.cellCharsHeight != b.cellCharsHeight)
        {
            return a.cellCharsHeight < b.cellCharsHeight;
        }
        if (a.cellWidth != b.cellWidth)
        {
            return a.cellWidth < b.cellWidth;
        }
        if (a.cellHeight != b.cellHeight)
        {
            return a.cellHeight < b.cellHeight;
        }
        if (a.cellWidthMode != b.cellWidthMode)
        {
            return a.cellWidthMode < b.cellWidthMode;
        }
        if (a.cellHeightMode != b.cellHeightMode)
        {
            return a.cellHeightMode < b.cellHeightMode;
        }
        if (a.width != b.width)
        {
            return a.width < b.width;
        }
        if (a.height != b.height)
        {
            return a.height < b.height;
        }
        if (a.widthMode != b.widthMode)
        {
            return a.widthMode < b.widthMode;
        }
        if (a.heightMode != b.heightMode)
        {
            return a.heightMode < b.heightMode;
        }
        if (a.charsWidth != b.charsWidth)
        {
            return a.charsWidth < b.charsWidth;
        }
        if (a.charsHeight != b.charsHeight)
        {
            return a.charsHeight < b.charsHeight;
        }
        if (a.marginRight != b.marginRight)
        {
            return a.marginRight < b.marginRight;
        }
        return a.marginBottom < b.marginBottom;
    };
    std::sort(leftLayout.begin(), leftLayout.end(), geometryLess);
    std::sort(rightLayout.begin(), rightLayout.end(), geometryLess);

    for (std::size_t index = 0; index < leftLayout.size(); ++index)
    {
        const NativeLayoutItem& a = leftLayout[index];
        const NativeLayoutItem& b = rightLayout[index];
        if (a.row != b.row || a.col != b.col ||
            a.cellRow != b.cellRow || a.cellCol != b.cellCol ||
            a.cellCharsWidth != b.cellCharsWidth || a.cellCharsHeight != b.cellCharsHeight ||
            a.cellWidth != b.cellWidth || a.cellHeight != b.cellHeight ||
            a.cellWidthMode != b.cellWidthMode || a.cellHeightMode != b.cellHeightMode ||
            a.width != b.width || a.height != b.height ||
            a.widthMode != b.widthMode || a.heightMode != b.heightMode ||
            a.charsWidth != b.charsWidth || a.charsHeight != b.charsHeight ||
            a.marginRight != b.marginRight || a.marginBottom != b.marginBottom)
        {
            return false;
        }
    }
    return true;
}

std::string markdownFenceFor(const std::string& content)
{
    std::size_t longest = 0;
    std::size_t run = 0;
    for (char ch : content)
    {
        if (ch == '`')
        {
            ++run;
            longest = std::max(longest, run);
        }
        else
        {
            run = 0;
        }
    }
    return std::string(std::max<std::size_t>(MIN_MARKDOWN_FENCE_LENGTH, longest + 1), '`');
}

std::string displayPath(const std::filesystem::path& path)
{
    std::error_code error;
    const std::filesystem::path relative = std::filesystem::relative(path, std::filesystem::current_path(), error);
    if (!error && !relative.empty())
    {
        return relative.string();
    }
    return path.string();
}

std::string markdownListValue(const std::string& value, const std::string& fallback)
{
    return value.empty() ? fallback : value;
}

}  // namespace

std::string generateIssueReport(const std::filesystem::path& sourcePath, const IssueReportOptions& options)
{
    const std::string rawSource = readTextFile(sourcePath);
    const NativeDocument document = parseDocumentText(rawSource, sourcePath.string());
    const std::vector<NameReplacement> replacements = buildNameReplacements(document, options.anonymizeNames);
    const std::string sanitizedSource = sanitizedSourceText(document, replacements, options);
    const NativeDocument sanitizedDocument = parseDocumentText(sanitizedSource, sourcePath.string() + " (sanitized)");
    if (!layoutGeometryMatches(document, sanitizedDocument))
    {
        throw std::runtime_error("sanitized UIMD source changed layout geometry");
    }

    int anonymizedCount = 0;
    for (const NameReplacement& replacement : replacements)
    {
        if (replacement.anonymized)
        {
            ++anonymizedCount;
        }
    }

    const std::string title = markdownListValue(options.title, "Describe the UIMD issue.");
    const std::string fence = markdownFenceFor(sanitizedSource);
    std::ostringstream report;
    report
        << "## Summary\n\n"
        << title << "\n\n"
        << "## Source\n\n"
        << "- File: `" << displayPath(sourcePath) << "`\n"
        << "- UIMD title: `" << document.title << "`\n"
        << "- Kind: `" << markdownListValue(options.kind, "bug") << "`\n"
        << "- Targets: `" << markdownListValue(options.targets, "unknown") << "`\n\n"
        << "## Expected\n\n"
        << "Describe the expected behavior.\n\n"
        << "## Actual\n\n"
        << "Describe the actual behavior. Include the first mismatch, error message, or screenshot context.\n\n"
        << "## Reproduction\n\n"
        << "```bash\n"
        << "# Add the exact command used to reproduce the problem.\n"
        << "```\n\n"
        << "## Sanitized UIMD Source\n\n"
        << fence << "uimd\n"
        << sanitizedSource;
    if (sanitizedSource.empty() || sanitizedSource.back() != '\n')
    {
        report << "\n";
    }
    report
        << fence << "\n\n"
        << "## Sanitization\n\n"
        << "- element names anonymized: " << (options.anonymizeNames ? "yes" : "no") << "\n"
        << "- anonymized element count: " << anonymizedCount << "\n"
        << "- descriptions removed: " << (options.keepDescriptions ? "no" : "yes") << "\n"
        << "- colors changed: no\n"
        << "- sanitized source parses: yes\n"
        << "- layout geometry preserved: yes\n";

    return report.str();
}

}  // namespace uimd::tool

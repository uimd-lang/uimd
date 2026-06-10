#include "mcp_tester_ui.hpp"
#include "log_panel_ui.hpp"
#include "ui/elements/FrameBufferView.hpp"
#include "ui/elements/Render.hpp"
#include "ui/generated/GeneratedWindowRuntime.hpp"
#include "ui/terminal/Clipboard.hpp"
#include "ui/terminal/TerminalBuffer.hpp"
#include "ui/terminal/TerminalBackend.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <csignal>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <optional>
#include <poll.h>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <variant>
#include <vector>

#if defined(__APPLE__)
#include <util.h>
#else
#include <pty.h>
#endif

namespace
{

constexpr const char* kJsonRpcVersion = "2.0";
constexpr const char* kDefaultHost = "127.0.0.1";
constexpr int kDefaultActionDelayMs = 120;
constexpr int kDefaultTypeDelayMs = 100;
constexpr int kDefaultStepDelayMs = 450;
constexpr int kConnectTimeoutMs = 5000;
constexpr int kRequestTimeoutMs = 60000;
constexpr int kRequestRetryCount = 3;
constexpr int kRequestRetryDelayMs = 50;
constexpr int kLiveBufferRepaintFps = 30;
constexpr int kLiveBufferRepaintIntervalMs = 1000 / kLiveBufferRepaintFps;
constexpr int kProcessStopTimeoutMs = 2000;
constexpr int kFallbackViewportWidth = 100;
constexpr int kFallbackViewportHeight = 40;
constexpr int kInitialViewportWidth = 1;
constexpr int kInitialViewportHeight = 1;
constexpr int kMinimumLogRows = 1;
constexpr const char* kAllExamplesConfig = "tests/mcp/all_examples.yaml";
constexpr const char* kDefaultSnapshotDir = "tests/mcp/snapshots";
constexpr const char* kSnapshotCompareTool = "get_render_snapshot";
constexpr const char* kSnapshotCompactCompareTool = "get_render_snapshot_compact";
constexpr const char* kPauseButtonTitle = "Pause";
constexpr const char* kPlayButtonTitle = "Play";
constexpr const char* kInitialFocusName = "pause";
constexpr const char* kBufferCornerFocusColor = "#facc15";
constexpr const char* kBufferCornerEditColor = "#22c55e";
constexpr char kResultSeparatorChar = '-';
constexpr double kBufferScopeDimAlpha = 0.4;

namespace fs = std::filesystem;

std::mutex gOutputMutex;

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

bool startsWith(const std::string& value, const std::string& prefix)
{
    return value.rfind(prefix, 0) == 0;
}

bool endsWith(const std::string& value, const std::string& suffix)
{
    return value.size() >= suffix.size() && value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string basenameWithoutExtension(const std::string& path)
{
    std::size_t slash = path.find_last_of("/\\");
    std::string base = slash == std::string::npos ? path : path.substr(slash + 1);
    std::size_t dot = base.find_last_of('.');
    if (dot != std::string::npos)
    {
        base = base.substr(0, dot);
    }
    return base.empty() ? "app" : base;
}

std::string pathString(const fs::path& path)
{
    return path.lexically_normal().string();
}

std::string absolutePath(const std::string& root, const std::string& value)
{
    fs::path path(value);
    if (path.is_absolute())
    {
        return pathString(path);
    }
    return pathString(fs::absolute(fs::path(root) / path));
}

std::string resolveConfigPath(const std::string& root, const std::string& configPath, const std::string& value)
{
    fs::path path(value);
    if (path.is_absolute())
    {
        return pathString(path);
    }
    fs::path relativeToConfig = fs::path(configPath).parent_path() / path;
    if (fs::exists(relativeToConfig))
    {
        return pathString(relativeToConfig);
    }
    return pathString(fs::path(root) / path);
}

std::string relativePath(const std::string& root, const std::string& value)
{
    std::error_code error;
    fs::path relative = fs::relative(fs::path(value), fs::path(root), error);
    return error ? value : pathString(relative);
}

std::string safeTestPath(const std::string& root, const std::string& value)
{
    fs::path rootPath = fs::weakly_canonical(fs::absolute(root));
    fs::path target = fs::weakly_canonical(fs::absolute(fs::path(root) / value));
    std::string rootText = pathString(rootPath);
    std::string targetText = pathString(target);
    if (targetText != rootText && !startsWith(targetText, rootText + "/"))
    {
        throw std::runtime_error("refusing path outside project root: " + value);
    }
    return targetText;
}

std::string findProjectRoot(const char* executablePath)
{
    std::vector<fs::path> starts;
    if (executablePath != nullptr && std::string(executablePath).find('/') != std::string::npos)
    {
        fs::path executable(executablePath);
        if (executable.is_relative())
        {
            executable = fs::absolute(executable);
        }
        starts.push_back(executable.parent_path());
    }
    starts.push_back(fs::current_path());

    for (fs::path start : starts)
    {
        std::error_code error;
        start = fs::weakly_canonical(start, error);
        if (error)
        {
            start = fs::absolute(start);
        }
        for (fs::path current = start; !current.empty(); current = current.parent_path())
        {
            if (fs::exists(current / "src/uimd/testing/mcp_tester.py") &&
                fs::exists(current / "tests/mcp/all_examples.yaml"))
            {
                return pathString(current);
            }
            if (current == current.parent_path())
            {
                break;
            }
        }
    }
    return pathString(fs::current_path());
}

std::string appPathFromExamplesRoot(const std::string& examplesRoot, const std::string& name)
{
    std::vector<fs::path> candidates = {
        fs::path(examplesRoot) / name / (name + ".py"),
        fs::path(examplesRoot) / name / name,
        fs::path(examplesRoot) / (name + ".py"),
        fs::path(examplesRoot) / name,
    };
    for (const auto& candidate : candidates)
    {
        if (fs::exists(candidate))
        {
            return pathString(candidate);
        }
    }
    throw std::runtime_error("cannot resolve app '" + name + "' under examples root: " + examplesRoot);
}

std::string replaceAll(std::string value, const std::string& from, const std::string& to)
{
    std::size_t position = 0;
    while ((position = value.find(from, position)) != std::string::npos)
    {
        value.replace(position, from.size(), to);
        position += to.size();
    }
    return value;
}

std::string readFile(const std::string& path)
{
    std::ifstream input(path);
    if (!input)
    {
        throw std::runtime_error("cannot read file: " + path);
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

class Json
{
public:
    using Array = std::vector<Json>;
    using Object = std::map<std::string, Json>;
    using Value = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

    Json() : value_(nullptr) {}
    Json(std::nullptr_t) : value_(nullptr) {}
    Json(bool value) : value_(value) {}
    Json(int value) : value_(static_cast<double>(value)) {}
    Json(double value) : value_(value) {}
    Json(std::string value) : value_(std::move(value)) {}
    Json(const char* value) : value_(std::string(value)) {}
    Json(Array value) : value_(std::move(value)) {}
    Json(Object value) : value_(std::move(value)) {}

    bool isNull() const { return std::holds_alternative<std::nullptr_t>(value_); }
    bool isBool() const { return std::holds_alternative<bool>(value_); }
    bool isNumber() const { return std::holds_alternative<double>(value_); }
    bool isString() const { return std::holds_alternative<std::string>(value_); }
    bool isArray() const { return std::holds_alternative<Array>(value_); }
    bool isObject() const { return std::holds_alternative<Object>(value_); }

    bool boolValue(bool fallback = false) const
    {
        return isBool() ? std::get<bool>(value_) : fallback;
    }

    double numberValue(double fallback = 0.0) const
    {
        return isNumber() ? std::get<double>(value_) : fallback;
    }

    std::string stringValue(const std::string& fallback = {}) const
    {
        if (isString())
        {
            return std::get<std::string>(value_);
        }
        if (isBool())
        {
            return std::get<bool>(value_) ? "true" : "false";
        }
        if (isNumber())
        {
            std::ostringstream out;
            out << std::setprecision(15) << std::get<double>(value_);
            std::string text = out.str();
            if (text.find('.') != std::string::npos)
            {
                while (!text.empty() && text.back() == '0')
                {
                    text.pop_back();
                }
                if (!text.empty() && text.back() == '.')
                {
                    text.pop_back();
                }
            }
            return text;
        }
        return fallback;
    }

    const Array& array() const
    {
        static const Array empty;
        return isArray() ? std::get<Array>(value_) : empty;
    }

    const Object& object() const
    {
        static const Object empty;
        return isObject() ? std::get<Object>(value_) : empty;
    }

    Array& array()
    {
        if (!isArray())
        {
            value_ = Array{};
        }
        return std::get<Array>(value_);
    }

    Object& object()
    {
        if (!isObject())
        {
            value_ = Object{};
        }
        return std::get<Object>(value_);
    }

    const Json* get(const std::string& key) const
    {
        if (!isObject())
        {
            return nullptr;
        }
        const auto& obj = object();
        auto it = obj.find(key);
        return it == obj.end() ? nullptr : &it->second;
    }

    Json& operator[](const std::string& key)
    {
        return object()[key];
    }

    std::string dump() const
    {
        if (isNull())
        {
            return "null";
        }
        if (isBool())
        {
            return boolValue() ? "true" : "false";
        }
        if (isNumber())
        {
            return stringValue("0");
        }
        if (isString())
        {
            return "\"" + escape(std::get<std::string>(value_)) + "\"";
        }
        if (isArray())
        {
            std::string result = "[";
            const auto& values = array();
            for (std::size_t index = 0; index < values.size(); ++index)
            {
                if (index > 0)
                {
                    result += ",";
                }
                result += values[index].dump();
            }
            result += "]";
            return result;
        }
        std::string result = "{";
        bool first = true;
        for (const auto& [key, value] : object())
        {
            if (!first)
            {
                result += ",";
            }
            first = false;
            result += "\"" + escape(key) + "\":" + value.dump();
        }
        result += "}";
        return result;
    }

    static std::string escape(const std::string& text)
    {
        std::string result;
        for (char ch : text)
        {
            switch (ch)
            {
                case '\\': result += "\\\\"; break;
                case '"': result += "\\\""; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default:
                    result += ch;
                    break;
            }
        }
        return result;
    }

private:
    Value value_;
};

class JsonParser
{
public:
    explicit JsonParser(std::string text) : text_(std::move(text)) {}

    Json parse()
    {
        Json value = parseValue();
        skipWhitespace();
        if (position_ != text_.size())
        {
            throw std::runtime_error("trailing JSON content");
        }
        return value;
    }

private:
    static void appendUtf8(std::string& result, unsigned int codepoint)
    {
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
    }

    unsigned int parseHexCodepoint()
    {
        if (position_ + 4 > text_.size())
        {
            throw std::runtime_error("invalid JSON unicode escape");
        }
        unsigned int value = 0;
        for (int index = 0; index < 4; ++index)
        {
            char ch = text_[position_++];
            value <<= 4;
            if (ch >= '0' && ch <= '9')
            {
                value += static_cast<unsigned int>(ch - '0');
            }
            else if (ch >= 'a' && ch <= 'f')
            {
                value += static_cast<unsigned int>(ch - 'a' + 10);
            }
            else if (ch >= 'A' && ch <= 'F')
            {
                value += static_cast<unsigned int>(ch - 'A' + 10);
            }
            else
            {
                throw std::runtime_error("invalid JSON unicode escape");
            }
        }
        return value;
    }

    Json parseValue()
    {
        skipWhitespace();
        if (position_ >= text_.size())
        {
            throw std::runtime_error("unexpected end of JSON");
        }
        char ch = text_[position_];
        if (ch == '"') return Json(parseString());
        if (ch == '{') return parseObject();
        if (ch == '[') return parseArray();
        if (startsWithAt("true")) { position_ += 4; return Json(true); }
        if (startsWithAt("false")) { position_ += 5; return Json(false); }
        if (startsWithAt("null")) { position_ += 4; return Json(nullptr); }
        return parseNumber();
    }

    Json parseObject()
    {
        expect('{');
        Json::Object result;
        skipWhitespace();
        if (peek('}'))
        {
            expect('}');
            return result;
        }
        while (true)
        {
            std::string key = parseString();
            skipWhitespace();
            expect(':');
            result.emplace(std::move(key), parseValue());
            skipWhitespace();
            if (peek('}'))
            {
                expect('}');
                break;
            }
            expect(',');
        }
        return result;
    }

    Json parseArray()
    {
        expect('[');
        Json::Array result;
        skipWhitespace();
        if (peek(']'))
        {
            expect(']');
            return result;
        }
        while (true)
        {
            result.push_back(parseValue());
            skipWhitespace();
            if (peek(']'))
            {
                expect(']');
                break;
            }
            expect(',');
        }
        return result;
    }

    Json parseNumber()
    {
        std::size_t start = position_;
        if (peek('-'))
        {
            ++position_;
        }
        while (position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_])))
        {
            ++position_;
        }
        if (peek('.'))
        {
            ++position_;
            while (position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_])))
            {
                ++position_;
            }
        }
        if (position_ < text_.size() && (text_[position_] == 'e' || text_[position_] == 'E'))
        {
            ++position_;
            if (position_ < text_.size() && (text_[position_] == '+' || text_[position_] == '-'))
            {
                ++position_;
            }
            while (position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_])))
            {
                ++position_;
            }
        }
        return Json(std::stod(text_.substr(start, position_ - start)));
    }

    std::string parseString()
    {
        expect('"');
        std::string result;
        while (position_ < text_.size())
        {
            char ch = text_[position_++];
            if (ch == '"')
            {
                return result;
            }
            if (ch == '\\')
            {
                if (position_ >= text_.size())
                {
                    throw std::runtime_error("invalid JSON escape");
                }
                char esc = text_[position_++];
                switch (esc)
                {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    case 'u':
                    {
                        unsigned int codepoint = parseHexCodepoint();
                        if (codepoint >= 0xD800 && codepoint <= 0xDBFF &&
                            position_ + 6 <= text_.size() && text_[position_] == '\\' && text_[position_ + 1] == 'u')
                        {
                            position_ += 2;
                            unsigned int low = parseHexCodepoint();
                            if (low >= 0xDC00 && low <= 0xDFFF)
                            {
                                codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                            }
                        }
                        appendUtf8(result, codepoint);
                        break;
                    }
                    default:
                        result += esc;
                        break;
                }
            }
            else
            {
                result += ch;
            }
        }
        throw std::runtime_error("unterminated JSON string");
    }

    void skipWhitespace()
    {
        while (position_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[position_])))
        {
            ++position_;
        }
    }

    bool startsWithAt(const std::string& value) const
    {
        return text_.compare(position_, value.size(), value) == 0;
    }

    bool peek(char ch) const
    {
        return position_ < text_.size() && text_[position_] == ch;
    }

    void expect(char ch)
    {
        skipWhitespace();
        if (position_ >= text_.size() || text_[position_] != ch)
        {
            throw std::runtime_error(std::string("expected JSON char: ") + ch);
        }
        ++position_;
    }

    std::string text_;
    std::size_t position_ = 0;
};

Json parseJson(const std::string& text)
{
    return JsonParser(text).parse();
}

struct YamlLine
{
    int indent = 0;
    std::string text;
    int lineNumber = 0;
};

std::vector<YamlLine> yamlLines(const std::string& content)
{
    std::vector<YamlLine> result;
    std::istringstream input(content);
    std::string raw;
    int line = 1;
    while (std::getline(input, raw))
    {
        if (!raw.empty() && raw.back() == '\r')
        {
            raw.pop_back();
        }
        std::string stripped = trim(raw);
        if (stripped.empty() || stripped.front() == '#')
        {
            ++line;
            continue;
        }
        int indent = 0;
        while (indent < static_cast<int>(raw.size()) && raw[static_cast<std::size_t>(indent)] == ' ')
        {
            ++indent;
        }
        result.push_back({indent, stripped, line});
        ++line;
    }
    return result;
}

bool looksLikeYamlPair(const std::string& value)
{
    auto pos = value.find(':');
    return pos != std::string::npos && !trim(value.substr(0, pos)).empty();
}

std::pair<std::string, std::string> parseYamlPair(const std::string& value, int line)
{
    auto pos = value.find(':');
    if (pos == std::string::npos)
    {
        throw std::runtime_error("expected YAML pair on line " + std::to_string(line));
    }
    std::string key = trim(value.substr(0, pos));
    if ((startsWith(key, "\"") && key.back() == '"') || (startsWith(key, "'") && key.back() == '\''))
    {
        key = key.substr(1, key.size() - 2);
    }
    return {key, trim(value.substr(pos + 1))};
}

Json parseYamlScalar(const std::string& value)
{
    std::string text = trim(value);
    if (text.empty())
    {
        return Json("");
    }
    if (text.front() == '"' && text.back() == '"')
    {
        return parseJson(text);
    }
    if (text.front() == '\'' && text.back() == '\'')
    {
        return Json(text.substr(1, text.size() - 2));
    }
    if (text == "null" || text == "Null" || text == "~")
    {
        return Json(nullptr);
    }
    if (text == "true" || text == "True")
    {
        return Json(true);
    }
    if (text == "false" || text == "False")
    {
        return Json(false);
    }
    if (text.front() == '[' && text.back() == ']')
    {
        Json::Array values;
        std::string inner = text.substr(1, text.size() - 2);
        std::stringstream stream(inner);
        std::string item;
        while (std::getline(stream, item, ','))
        {
            values.push_back(parseYamlScalar(trim(item)));
        }
        return Json(values);
    }
    char* end = nullptr;
    double number = std::strtod(text.c_str(), &end);
    if (end != text.c_str() && *end == '\0')
    {
        return Json(number);
    }
    return Json(text);
}

Json parseYamlBlock(const std::vector<YamlLine>& lines, std::size_t& index, int indent);

Json parseYamlList(const std::vector<YamlLine>& lines, std::size_t& index, int indent)
{
    Json::Array result;
    while (index < lines.size())
    {
        const auto& line = lines[index];
        if (line.indent < indent || line.indent != indent || !startsWith(line.text, "- "))
        {
            break;
        }
        std::string itemText = trim(line.text.substr(2));
        ++index;
        if (itemText.empty())
        {
            if (index < lines.size() && lines[index].indent > indent)
            {
                result.push_back(parseYamlBlock(lines, index, lines[index].indent));
            }
            else
            {
                result.push_back(Json(nullptr));
            }
            continue;
        }
        if (looksLikeYamlPair(itemText))
        {
            Json::Object item;
            auto [key, value] = parseYamlPair(itemText, line.lineNumber);
            if (value.empty())
            {
                if (index < lines.size() && lines[index].indent > indent)
                {
                    item[key] = parseYamlBlock(lines, index, lines[index].indent);
                }
                else
                {
                    item[key] = Json::Object{};
                }
            }
            else
            {
                item[key] = parseYamlScalar(value);
            }
            if (index < lines.size() && lines[index].indent > indent)
            {
                Json extra = parseYamlBlock(lines, index, lines[index].indent);
                for (const auto& [extraKey, extraValue] : extra.object())
                {
                    item[extraKey] = extraValue;
                }
            }
            result.push_back(item);
        }
        else
        {
            result.push_back(parseYamlScalar(itemText));
        }
    }
    return result;
}

Json parseYamlMap(const std::vector<YamlLine>& lines, std::size_t& index, int indent)
{
    Json::Object result;
    while (index < lines.size())
    {
        const auto& line = lines[index];
        if (line.indent < indent || line.indent != indent || startsWith(line.text, "- "))
        {
            break;
        }
        auto [key, value] = parseYamlPair(line.text, line.lineNumber);
        ++index;
        if (value.empty())
        {
            if (index < lines.size() && lines[index].indent > indent)
            {
                result[key] = parseYamlBlock(lines, index, lines[index].indent);
            }
            else
            {
                result[key] = Json::Object{};
            }
        }
        else
        {
            result[key] = parseYamlScalar(value);
        }
    }
    return result;
}

Json parseYamlBlock(const std::vector<YamlLine>& lines, std::size_t& index, int indent)
{
    if (index >= lines.size())
    {
        return Json(nullptr);
    }
    if (startsWith(lines[index].text, "- "))
    {
        return parseYamlList(lines, index, indent);
    }
    return parseYamlMap(lines, index, indent);
}

Json parseYaml(const std::string& content)
{
    auto lines = yamlLines(content);
    if (lines.empty())
    {
        return Json::Array{};
    }
    std::size_t index = 0;
    return parseYamlBlock(lines, index, lines.front().indent);
}

const Json* jsonPath(const Json& value, const std::string& path)
{
    const Json* current = &value;
    std::size_t start = 0;
    while (current != nullptr)
    {
        std::size_t dot = path.find('.', start);
        std::string part = path.substr(start, dot == std::string::npos ? std::string::npos : dot - start);
        current = current->get(part);
        if (dot == std::string::npos)
        {
            break;
        }
        start = dot + 1;
    }
    return current;
}

Json formatTargetValue(const Json& value, const std::string& targetName)
{
    if (value.isString())
    {
        std::string text = replaceAll(value.stringValue(), "{platform}", targetName);
        text = replaceAll(text, "{target}", targetName);
        return Json(text);
    }
    if (value.isArray())
    {
        Json::Array result;
        for (const Json& item : value.array())
        {
            result.push_back(formatTargetValue(item, targetName));
        }
        return Json(result);
    }
    if (value.isObject())
    {
        Json::Object result;
        for (const auto& [key, item] : value.object())
        {
            std::string formattedKey = replaceAll(key, "{platform}", targetName);
            formattedKey = replaceAll(formattedKey, "{target}", targetName);
            result[formattedKey] = formatTargetValue(item, targetName);
        }
        return Json(result);
    }
    return value;
}

bool hasTargetPlaceholder(const Json& value)
{
    if (value.isString())
    {
        std::string text = value.stringValue();
        return text.find("{platform}") != std::string::npos || text.find("{target}") != std::string::npos;
    }
    if (value.isArray())
    {
        for (const Json& item : value.array())
        {
            if (hasTargetPlaceholder(item))
            {
                return true;
            }
        }
    }
    if (value.isObject())
    {
        for (const auto& [key, item] : value.object())
        {
            if (key.find("{platform}") != std::string::npos || key.find("{target}") != std::string::npos ||
                hasTargetPlaceholder(item))
            {
                return true;
            }
        }
    }
    return false;
}

bool jsonEqual(const Json& actual, const Json& expected)
{
    if (expected.isNull())
    {
        return actual.isNull();
    }
    if (expected.isBool())
    {
        return actual.isBool() ? actual.boolValue() == expected.boolValue() : actual.stringValue() == expected.stringValue();
    }
    if (expected.isNumber())
    {
        return actual.stringValue() == expected.stringValue();
    }
    if (expected.isString())
    {
        return actual.stringValue() == expected.stringValue();
    }
    if (expected.isArray())
    {
        if (!actual.isArray() || actual.array().size() != expected.array().size())
        {
            return false;
        }
        for (std::size_t index = 0; index < expected.array().size(); ++index)
        {
            if (!jsonEqual(actual.array()[index], expected.array()[index]))
            {
                return false;
            }
        }
        return true;
    }
    if (expected.isObject())
    {
        if (!actual.isObject() || actual.object().size() != expected.object().size())
        {
            return false;
        }
        for (const auto& [key, item] : expected.object())
        {
            const Json* actualItem = actual.get(key);
            if (actualItem == nullptr || !jsonEqual(*actualItem, item))
            {
                return false;
            }
        }
        return true;
    }
    return actual.dump() == expected.dump();
}

bool containsValue(const Json& actual, const Json& expected)
{
    if (actual.isString())
    {
        return actual.stringValue().find(expected.stringValue()) != std::string::npos;
    }
    if (actual.isArray())
    {
        for (const Json& item : actual.array())
        {
            if (jsonEqual(item, expected))
            {
                return true;
            }
        }
    }
    if (actual.isObject())
    {
        for (const auto& [key, item] : actual.object())
        {
            if (key == expected.stringValue() || jsonEqual(item, expected))
            {
                return true;
            }
        }
    }
    return actual.dump().find(expected.stringValue()) != std::string::npos;
}

bool assertionMatches(const Json& actual, const Json& expected, std::string& label)
{
    if (expected.isObject())
    {
        int opCount = 0;
        std::string op;
        for (const std::string& name : {"contains", "regex", "equals"})
        {
            if (expected.get(name) != nullptr)
            {
                ++opCount;
                op = name;
            }
        }
        if (opCount > 1)
        {
            throw std::runtime_error("assertion has multiple operators");
        }
        if (opCount == 1)
        {
            const Json& opValue = *expected.get(op);
            if (op == "contains")
            {
                label = "contains " + opValue.dump();
                return containsValue(actual, opValue);
            }
            if (op == "regex")
            {
                label = "matches /" + opValue.stringValue() + "/";
                return std::regex_search(actual.stringValue(), std::regex(opValue.stringValue()));
            }
            label = "== " + opValue.dump();
            return jsonEqual(actual, opValue);
        }
    }
    label = "== " + expected.dump();
    return jsonEqual(actual, expected);
}

int findFreePort()
{
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        throw std::runtime_error("socket failed");
    }
    sockaddr_in addr{};
#if defined(__APPLE__)
    addr.sin_len = sizeof(addr);
#endif
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;
    if (::bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::string message = "bind failed: ";
        message += std::strerror(errno);
        ::close(sock);
        throw std::runtime_error(message);
    }
    socklen_t len = sizeof(addr);
    if (::getsockname(sock, reinterpret_cast<sockaddr*>(&addr), &len) != 0)
    {
        ::close(sock);
        throw std::runtime_error("getsockname failed");
    }
    int port = ntohs(addr.sin_port);
    ::close(sock);
    return port;
}

void setPtySize(int fd, int width, int height)
{
    winsize size{};
    size.ws_col = static_cast<unsigned short>(std::max(1, width));
    size.ws_row = static_cast<unsigned short>(std::max(1, height));
    ioctl(fd, TIOCSWINSZ, &size);
}

struct Viewport
{
    int row = 0;
    int col = 0;
    int width = kInitialViewportWidth;
    int height = kInitialViewportHeight;
};

struct TerminalSize
{
    int width = kInitialViewportWidth;
    int height = kInitialViewportHeight;
};

TerminalSize terminalSize()
{
    winsize size{};
    if (::ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0 && size.ws_row > 0)
    {
        return {static_cast<int>(size.ws_col), static_cast<int>(size.ws_row)};
    }
    return {};
}

struct TesterConfig
{
    std::string root = ".";
    std::string appPath;
    std::string yamlPath;
    bool all = false;
    bool compare = false;
    std::string compareA;
    std::string compareB;
    bool mcpFast = false;
    int actionDelayMs = kDefaultActionDelayMs;
    int typeDelayMs = kDefaultTypeDelayMs;
    int stepDelayMs = kDefaultStepDelayMs;
    bool exitOnFinish = false;
    bool plain = false;
    std::optional<Viewport> compareAppSize;
    std::string logFile;
    std::string snapshotDir = kDefaultSnapshotDir;
};

struct Step
{
    Json node;
};

struct Script
{
    std::string sourcePath;
    std::string name;
    std::map<std::string, std::string> apps;
    std::map<std::string, std::map<std::string, std::string>> env;
    std::vector<Step> setupSteps;
    std::vector<Step> steps;
    std::vector<Step> cleanupSteps;
};

struct ScriptSummary
{
    std::string name;
    std::string status;
    int assertionsPassed = 0;
    int assertionsFailed = 0;
    int stepFailures = 0;
};

class TargetApp
{
public:
    TargetApp(std::string name, std::string path, std::string root, std::map<std::string, std::string> env)
        : name_(std::move(name)), path_(std::move(path)), root_(std::move(root)), env_(std::move(env))
    {
    }

    ~TargetApp()
    {
        stop();
    }

    const std::string& name() const { return name_; }
    int port() const { return port_; }
    const Viewport& viewport() const { return viewport_; }

    void start(const Viewport& viewport, int actionDelayMs, int typeDelayMs, bool controlledRender, bool forwardOutput)
    {
        viewport_ = viewport;
        forwardOutput_ = forwardOutput;
        exited_ = false;
        port_ = findFreePort();
        int master = -1;
        int slave = -1;
        if (openpty(&master, &slave, nullptr, nullptr, nullptr) != 0)
        {
            throw std::runtime_error("openpty failed");
        }
        int stderrPipe[2] = {-1, -1};
        if (pipe(stderrPipe) != 0)
        {
            ::close(master);
            ::close(slave);
            throw std::runtime_error("stderr pipe failed");
        }
        setPtySize(master, viewport.width, viewport.height);
        setPtySize(slave, viewport.width, viewport.height);

        pid_ = fork();
        if (pid_ < 0)
        {
            ::close(master);
            ::close(slave);
            ::close(stderrPipe[0]);
            ::close(stderrPipe[1]);
            throw std::runtime_error("fork failed");
        }
        if (pid_ == 0)
        {
            ::setsid();
            // Match the Python tester: target apps must not consume tester UI input.
            int nullInput = ::open("/dev/null", O_RDONLY);
            if (nullInput >= 0)
            {
                ::dup2(nullInput, STDIN_FILENO);
                if (nullInput != STDIN_FILENO)
                {
                    ::close(nullInput);
                }
            }
            ::dup2(slave, STDOUT_FILENO);
            ::dup2(stderrPipe[1], STDERR_FILENO);
            ::close(master);
            ::close(slave);
            ::close(stderrPipe[0]);
            ::close(stderrPipe[1]);
            ::chdir(root_.c_str());
            for (const auto& [key, value] : env_)
            {
                ::setenv(key.c_str(), value.c_str(), 1);
            }

            std::vector<std::string> args;
            if (endsWith(path_, ".py"))
            {
                args.push_back("python3");
                args.push_back(path_);
            }
            else
            {
                args.push_back(path_);
            }
            args.push_back("--mcp-server");
            args.push_back("--gui");
            args.push_back("--mcp-transport");
            args.push_back("tcp");
            args.push_back("--mcp-host");
            args.push_back(kDefaultHost);
            args.push_back("--mcp-port");
            args.push_back(std::to_string(port_));
            args.push_back("--mcp-action-delay-ms");
            args.push_back(std::to_string(actionDelayMs));
            args.push_back("--mcp-type-delay-ms");
            args.push_back(std::to_string(typeDelayMs));
            args.push_back("--viewport");
            args.push_back(std::to_string(viewport.row) + "," + std::to_string(viewport.col) + "," +
                           std::to_string(viewport.width) + "," + std::to_string(viewport.height));
            args.push_back("--mcp-wait-render");
            if (controlledRender)
            {
                args.push_back("--mcp-controlled-render");
            }

            std::vector<char*> argv;
            for (auto& arg : args)
            {
                argv.push_back(arg.data());
            }
            argv.push_back(nullptr);
            execvp(argv.front(), argv.data());
            _exit(127);
        }

        ::close(slave);
        ::close(stderrPipe[1]);
        masterFd_ = master;
        stderrFd_ = stderrPipe[0];
        outputThread_ = std::thread([this]() { drainOutput(); });
        stderrThread_ = std::thread([this]() { drainStderr(); });
    }

    void waitUntilReady()
    {
        auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(kConnectTimeoutMs);
        while (std::chrono::steady_clock::now() < deadline)
        {
            if (processExited())
            {
                throw std::runtime_error(name_ + " exited early: " + capturedStderr());
            }
            try
            {
                callTool("get_window", Json::Object{});
                return;
            }
            catch (const std::exception&)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
        throw std::runtime_error(name_ + " MCP server did not start");
    }

    Json callTool(const std::string& tool, const Json& arguments)
    {
        int id = ++requestId_;
        Json::Object params;
        params["name"] = tool;
        params["arguments"] = arguments;
        Json::Object request;
        request["jsonrpc"] = kJsonRpcVersion;
        request["id"] = id;
        request["method"] = "tools/call";
        request["params"] = params;
        Json response = sendRequest(Json(request).dump());
        if (const Json* error = response.get("error"))
        {
            throw std::runtime_error(error->dump());
        }
        const Json* result = response.get("result");
        if (result == nullptr)
        {
            return Json::Object{};
        }
        const Json* content = result->get("content");
        if (content != nullptr && content->isArray() && !content->array().empty())
        {
            const Json* text = content->array().front().get("text");
            if (text != nullptr && text->isString() && !text->stringValue().empty())
            {
                return parseJson(text->stringValue());
            }
        }
        return *result;
    }

    bool hasExited()
    {
        return processExited();
    }

    void stop()
    {
        if (pid_ > 0)
        {
            kill(pid_, SIGTERM);
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(kProcessStopTimeoutMs);
            int status = 0;
            while (std::chrono::steady_clock::now() < deadline)
            {
                pid_t result = waitpid(pid_, &status, WNOHANG);
                if (result == pid_)
                {
                    exited_ = true;
                    pid_ = -1;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            if (pid_ > 0)
            {
                kill(pid_, SIGKILL);
                waitpid(pid_, &status, 0);
                exited_ = true;
                pid_ = -1;
            }
        }
        if (masterFd_ >= 0)
        {
            ::close(masterFd_);
            masterFd_ = -1;
        }
        if (outputThread_.joinable())
        {
            outputThread_.join();
        }
        if (stderrThread_.joinable())
        {
            stderrThread_.join();
        }
        if (stderrFd_ >= 0)
        {
            ::close(stderrFd_);
            stderrFd_ = -1;
        }
    }

private:
    Json sendRequest(const std::string& payload)
    {
        std::string lastError;
        for (int attempt = 0; attempt <= kRequestRetryCount; ++attempt)
        {
            try
            {
                return sendRequestOnce(payload);
            }
            catch (const std::exception& exc)
            {
                lastError = exc.what();
                if (processExited())
                {
                    std::string stderrText = capturedStderr();
                    throw std::runtime_error(name_ + " exited: " + (stderrText.empty() ? lastError : stderrText));
                }
                if (attempt >= kRequestRetryCount)
                {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(kRequestRetryDelayMs));
            }
        }
        throw std::runtime_error(lastError.empty() ? "MCP request failed" : lastError);
    }

    Json sendRequestOnce(const std::string& payload)
    {
        int sock = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            throw std::runtime_error("socket failed");
        }
        sockaddr_in addr{};
#if defined(__APPLE__)
        addr.sin_len = sizeof(addr);
#endif
        addr.sin_family = AF_INET;
        addr.sin_port = htons(static_cast<uint16_t>(port_));
        inet_pton(AF_INET, kDefaultHost, &addr.sin_addr);
        if (::connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
        {
            ::close(sock);
            throw std::runtime_error("connect failed");
        }
        timeval timeout{};
        timeout.tv_sec = kRequestTimeoutMs / 1000;
        timeout.tv_usec = (kRequestTimeoutMs % 1000) * 1000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        std::string line = payload + "\n";
        ::send(sock, line.data(), line.size(), 0);
        std::string response;
        char ch = 0;
        while (true)
        {
            ssize_t read = ::recv(sock, &ch, 1, 0);
            if (read <= 0)
            {
                break;
            }
            if (ch == '\n')
            {
                break;
            }
            response += ch;
        }
        ::close(sock);
        if (response.empty())
        {
            throw std::runtime_error("empty MCP response");
        }
        return parseJson(response);
    }

    void drainOutput()
    {
        char buffer[4096];
        while (masterFd_ >= 0)
        {
            ssize_t read = ::read(masterFd_, buffer, sizeof(buffer));
            if (read <= 0)
            {
                return;
            }
            if (forwardOutput_)
            {
                std::lock_guard lock(gOutputMutex);
                std::cout.write(buffer, read);
                std::cout.flush();
            }
        }
    }

    void drainStderr()
    {
        char buffer[4096];
        while (stderrFd_ >= 0)
        {
            ssize_t read = ::read(stderrFd_, buffer, sizeof(buffer));
            if (read <= 0)
            {
                return;
            }
            std::lock_guard lock(stderrMutex_);
            stderrText_.append(buffer, static_cast<std::size_t>(read));
        }
    }

    bool processExited()
    {
        if (exited_)
        {
            return true;
        }
        if (pid_ <= 0)
        {
            return false;
        }
        int status = 0;
        pid_t result = waitpid(pid_, &status, WNOHANG);
        if (result == pid_)
        {
            exited_ = true;
            pid_ = -1;
            return true;
        }
        return false;
    }

    std::string capturedStderr() const
    {
        std::lock_guard lock(stderrMutex_);
        return trim(stderrText_);
    }

    std::string name_;
    std::string path_;
    std::string root_;
    std::map<std::string, std::string> env_;
    Viewport viewport_;
    int port_ = 0;
    pid_t pid_ = -1;
    int masterFd_ = -1;
    int stderrFd_ = -1;
    bool forwardOutput_ = false;
    bool exited_ = false;
    std::atomic<int> requestId_{0};
    std::thread outputThread_;
    std::thread stderrThread_;
    mutable std::mutex stderrMutex_;
    std::string stderrText_;
};

struct FrameCell
{
    std::string text = " ";
    std::string fg;
    std::string bg;
};

std::vector<std::vector<FrameCell>> frameCells(const Json& frame)
{
    std::vector<std::vector<FrameCell>> result;
    const Json* cells = frame.get("cells");
    if (cells == nullptr || !cells->isArray())
    {
        return result;
    }
    for (const Json& row : cells->array())
    {
        std::vector<FrameCell> outputRow;
        for (const Json& cell : row.array())
        {
            FrameCell out;
            if (cell.isObject())
            {
                if (const Json* value = cell.get("char")) out.text = value->stringValue(" ");
                if (const Json* value = cell.get("foreground")) out.fg = value->stringValue();
                if (const Json* value = cell.get("background")) out.bg = value->stringValue();
            }
            else
            {
                const auto& items = cell.array();
                if (!items.empty()) out.text = items[0].stringValue(" ");
                if (items.size() > 1) out.fg = items[1].stringValue();
                if (items.size() > 2) out.bg = items[2].stringValue();
            }
            outputRow.push_back(out);
        }
        result.push_back(outputRow);
    }
    return result;
}

std::string ansiColor(const std::string& color, const std::string& prefix)
{
    if (color.size() == 7 && color.front() == '#')
    {
        int r = std::stoi(color.substr(1, 2), nullptr, 16);
        int g = std::stoi(color.substr(3, 2), nullptr, 16);
        int b = std::stoi(color.substr(5, 2), nullptr, 16);
        return prefix + ";2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b);
    }
    return {};
}

std::string cellAnsi(const FrameCell& cell)
{
    std::vector<std::string> codes;
    std::string fg = ansiColor(cell.fg, "38");
    std::string bg = ansiColor(cell.bg, "48");
    if (!fg.empty()) codes.push_back(fg);
    if (!bg.empty()) codes.push_back(bg);
    std::string result;
    if (!codes.empty())
    {
        result += "\x1b[";
        for (std::size_t index = 0; index < codes.size(); ++index)
        {
            if (index > 0) result += ";";
            result += codes[index];
        }
        result += "m";
    }
    result += cell.text.empty() ? " " : cell.text;
    return result;
}

std::string centeredText(const std::string& text, int width)
{
    width = std::max(kInitialViewportWidth, width);
    std::string clipped = text.size() > static_cast<std::size_t>(width)
        ? text.substr(0, static_cast<std::size_t>(width))
        : text;
    int left = std::max(0, (width - static_cast<int>(clipped.size())) / 2);
    int right = std::max(0, width - left - static_cast<int>(clipped.size()));
    return std::string(static_cast<std::size_t>(left), ' ') + clipped + std::string(static_cast<std::size_t>(right), ' ');
}

std::string currentTimeStamp()
{
    std::time_t now = std::time(nullptr);
    std::tm local{};
    localtime_r(&now, &local);
    std::ostringstream out;
    out << std::put_time(&local, "%H:%M:%S");
    return out.str();
}

double currentEpochMillis()
{
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

struct LogLine
{
    std::string text;
    std::string kind;
};

std::vector<std::vector<FrameCell>> renderSnapshotCells(const Json& snapshot)
{
    if (const Json* cells = snapshot.get("cells"))
    {
        Json::Object frame;
        frame["cells"] = *cells;
        return frameCells(Json(frame));
    }
    if (const Json* textLines = snapshot.get("text_lines"))
    {
        std::vector<std::vector<FrameCell>> result;
        for (const Json& line : textLines->array())
        {
            std::vector<FrameCell> row;
            std::string text = line.stringValue();
            for (char ch : text)
            {
                FrameCell cell;
                cell.text = std::string(1, ch);
                row.push_back(cell);
            }
            result.push_back(row);
        }
        return result;
    }
    throw std::runtime_error("render snapshot must contain cells");
}

std::string normalizedCellKey(FrameCell cell)
{
    if (cell.text == " ")
    {
        cell.fg.clear();
    }
    return cell.text + "|" + cell.fg + "|" + cell.bg;
}

std::string formatCell(const FrameCell& cell)
{
    return "char=" + cell.text + " fg=" + cell.fg + " bg=" + cell.bg;
}

std::optional<std::string> renderSnapshotMismatch(const Json& expected, const Json& actual)
{
    auto expectedCells = renderSnapshotCells(expected);
    auto actualCells = renderSnapshotCells(actual);
    if (actualCells.size() != expectedCells.size())
    {
        return "height expected " + std::to_string(expectedCells.size()) + ", got " + std::to_string(actualCells.size());
    }
    for (std::size_t row = 0; row < expectedCells.size(); ++row)
    {
        if (actualCells[row].size() != expectedCells[row].size())
        {
            return "row " + std::to_string(row) + " width expected " + std::to_string(expectedCells[row].size()) +
                   ", got " + std::to_string(actualCells[row].size());
        }
        for (std::size_t col = 0; col < expectedCells[row].size(); ++col)
        {
            if (normalizedCellKey(actualCells[row][col]) != normalizedCellKey(expectedCells[row][col]))
            {
                return "row " + std::to_string(row) + " col " + std::to_string(col) +
                       " expected " + formatCell(expectedCells[row][col]) + ", got " + formatCell(actualCells[row][col]);
            }
        }
    }
    return std::nullopt;
}

bool isTesterStep(const Json::Object& node)
{
    return node.find("tester") != node.end() || node.find("action") != node.end();
}

std::string testerAction(const Json::Object& node)
{
    auto tester = node.find("tester");
    if (tester != node.end())
    {
        return tester->second.stringValue();
    }
    auto action = node.find("action");
    return action == node.end() ? std::string{} : action->second.stringValue();
}

std::string fileStepContent(const Json::Object& node)
{
    auto lines = node.find("lines");
    if (lines != node.end())
    {
        std::string text;
        for (const Json& line : lines->second.array())
        {
            text += line.stringValue();
            text += "\n";
        }
        return text;
    }
    auto content = node.find("content");
    if (content == node.end())
    {
        content = node.find("text");
    }
    return content == node.end() ? std::string{} : content->second.stringValue();
}

Json runTesterFileStep(const std::string& root, const Json& step)
{
    Json::Object node = step.object();
    std::string action = testerAction(node);
    if (action.empty())
    {
        throw std::runtime_error("tester step is missing action");
    }
    auto pathIt = node.find("path");
    std::string path = pathIt == node.end() ? std::string{} : safeTestPath(root, pathIt->second.stringValue());
    if (action == "mkdir")
    {
        fs::create_directories(path);
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    if (action == "write_file" || action == "append_file")
    {
        fs::create_directories(fs::path(path).parent_path());
        std::ofstream output(path, action == "append_file" ? std::ios::app : std::ios::trunc);
        output << fileStepContent(node);
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    if (action == "remove")
    {
        std::error_code error;
        fs::remove_all(path, error);
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    if (action == "assert_exists")
    {
        if (!fs::exists(path))
        {
            throw std::runtime_error("path does not exist: " + relativePath(root, path));
        }
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    if (action == "assert_missing")
    {
        if (fs::exists(path))
        {
            throw std::runtime_error("path exists: " + relativePath(root, path));
        }
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    if (action == "assert_file_content" || action == "assert_file_contains")
    {
        std::string actual = readFile(path);
        std::string expected = fileStepContent(node);
        bool ok = action == "assert_file_content" ? actual == expected : actual.find(expected) != std::string::npos;
        if (!ok)
        {
            throw std::runtime_error(action == "assert_file_content"
                ? "file content mismatch: " + relativePath(root, path)
                : "file does not contain expected text: " + relativePath(root, path));
        }
        return Json::Object{{"ok", true}, {"path", relativePath(root, path)}};
    }
    throw std::runtime_error("unknown tester action: " + action);
}

class Tester
{
public:
    explicit Tester(TesterConfig config) : config_(std::move(config))
    {
        openRepaintStatsLog();
        shell_.log_output->setAutoScroll(true);
        shell_.log_output->setDynamicChildrenRenderer([this](int width)
        {
            return renderLogChildren(width);
        });
    }

    ~Tester()
    {
        stopBufferRepaintThread();
        requestStop();
        finishTerminalUi();
    }

    int run()
    {
        if (!config_.logFile.empty())
        {
            fs::path parent = fs::path(config_.logFile).parent_path();
            if (!parent.empty())
            {
                fs::create_directories(parent);
            }
            logFile_.open(config_.logFile);
        }
        if (!config_.snapshotDir.empty())
        {
            fs::create_directories(config_.snapshotDir);
        }
        std::vector<Script> scripts = loadScripts();
        beginTerminalUi(scripts.empty() ? kInitialViewportWidth : static_cast<int>(scripts.front().apps.size()));
        log("loaded " + config_.yamlPath + ": " + std::to_string(scripts.size()) + " script(s)");
        if (terminalUiStarted_)
        {
            return runInteractive(scripts);
        }
        return runScripts(scripts);
    }

private:
    int runScripts(const std::vector<Script>& scripts)
    {
        scriptSummaries_.clear();
        summaryDone_ = false;
        assertionsPassed_ = 0;
        assertionsFailed_ = 0;
        stepFailures_ = 0;
        int exitCode = 0;
        for (std::size_t index = 0; index < scripts.size(); ++index)
        {
            currentScript_ = &scripts[index];
            resetScriptCounters();
            renderTerminalShell(static_cast<int>(currentScript_->apps.size()));
            log("SCRIPT " + std::to_string(index + 1) + "/" + std::to_string(scripts.size()) + " " +
                currentScript_->name + ": " + currentScript_->sourcePath);
            int code = runScript(*currentScript_);
            appendScriptSummary(code == 0);
            if (code != 0)
            {
                exitCode = code;
            }
            if (index + 1 < scripts.size() || code != 0)
            {
                stopTargets();
            }
            if (stopRequested_.load())
            {
                exitCode = 1;
                break;
            }
        }
        if (stopRequested_.load())
        {
            appendSummary(1);
            return 1;
        }
        appendSummary(exitCode);
        return exitCode;
    }

    int runInteractive(const std::vector<Script>& scripts)
    {
        int code = 0;
        while (true)
        {
            restartRequested_.store(false);
            quitRequested_.store(false);
            stopRequested_.store(false);
            paused_.store(false);
            finished_.store(false);
            nextStepRequested_.store(false);
            workerRunning_.store(true);
            updatePauseButton();
            std::thread worker([&]()
            {
                code = runScripts(scripts);
                finished_.store(true);
                workerRunning_.store(false);
                pauseCondition_.notify_all();
            });
            interactiveEventLoop();
            requestStop();
            if (worker.joinable())
            {
                worker.join();
            }
            stopTargets();
            if (!restartRequested_.load())
            {
                return code;
            }
            {
                std::lock_guard lock(logLinesMutex_);
                logLines_.clear();
            }
            log("restart");
        }
    }

    int runScript(const Script& script)
    {
        try
        {
            runTesterSteps(script.setupSteps, "setup", script);
        }
        catch (const std::exception& exc)
        {
            log(std::string("FAIL setup: ") + exc.what());
            return 1;
        }

        const int targetCount = static_cast<int>(script.apps.size());
        bool controlled = terminalUiStarted_ || config_.compareAppSize.has_value() || targetCount > 1;
        int index = 0;
        targetOrder_.clear();
        comparePanV_ = 0;
        comparePanH_ = 0;
        bufferPanelFocused_ = false;
        bufferPanelEdit_ = false;
        bufferPreFocus_.clear();
        {
            std::lock_guard lock(cachedFramesMutex_);
            cachedFrames_.clear();
        }
        stopBufferRepaintThread();
        for (const auto& [name, path] : script.apps)
        {
            Viewport viewport = viewportForIndex(index++, targetCount);
            auto envIt = script.env.find(name);
            std::map<std::string, std::string> env = envIt == script.env.end() ? std::map<std::string, std::string>{} : envIt->second;
            if (!env.empty())
            {
                Json::Object envObject;
                for (const auto& [key, value] : env)
                {
                    envObject[key] = value;
                }
                Json formatted = formatTargetValue(Json(envObject), name);
                env = normalizeEnv(&formatted);
            }
            auto target = std::make_unique<TargetApp>(name, path, config_.root, env);
            log("spawn " + name + ": " + path);
            const bool forwardOutput = terminalUiStarted_ && !controlled;
            target->start(viewport, config_.actionDelayMs, config_.typeDelayMs, controlled, forwardOutput);
            target->waitUntilReady();
            log("connected " + name + " port=" + std::to_string(target->port()));
            targetOrder_.push_back(name);
            targets_[name] = std::move(target);
        }
        log("viewports " + viewportSummary(script));
        paintAllFrames();
        if (usesControlledBufferDisplay())
        {
            startBufferRepaintThread();
        }

        for (std::size_t stepIndex = 0; stepIndex < script.steps.size(); ++stepIndex)
        {
            if (!waitForStepPermission())
            {
                return 1;
            }
            stepIndex_ = static_cast<int>(stepIndex + 1);
            try
            {
                runStep(script.steps[stepIndex], stepIndex_);
            }
            catch (const std::exception& exc)
            {
                ++scriptStepFailures_;
                ++stepFailures_;
                log("FAIL step " + std::to_string(stepIndex + 1) + ": " + exc.what());
                try
                {
                    runTesterSteps(script.cleanupSteps, "cleanup", script);
                }
                catch (const std::exception& cleanupExc)
                {
                    log(std::string("FAIL cleanup: ") + cleanupExc.what());
                }
                return 1;
            }
            waitForStepDelay();
        }
        try
        {
            runTesterSteps(script.cleanupSteps, "cleanup", script);
        }
        catch (const std::exception& exc)
        {
            log(std::string("FAIL cleanup: ") + exc.what());
            return 1;
        }
        return 0;
    }

    void stopTargets()
    {
        stopBufferRepaintThread();
        for (auto& [name, target] : targets_)
        {
            (void)name;
            target->stop();
        }
        targets_.clear();
        targetOrder_.clear();
        {
            std::lock_guard lock(cachedFramesMutex_);
            cachedFrames_.clear();
        }
    }

    void requestStop()
    {
        stopRequested_.store(true);
        paused_.store(false);
        pauseCondition_.notify_all();
    }

    bool waitForStepPermission()
    {
        std::unique_lock lock(pauseMutex_);
        pauseCondition_.wait(lock, [&]()
        {
            return stopRequested_.load() || !paused_.load() || nextStepRequested_.load();
        });
        if (stopRequested_.load())
        {
            return false;
        }
        if (nextStepRequested_.load())
        {
            nextStepRequested_.store(false);
        }
        return true;
    }

    void waitForStepDelay()
    {
        if (config_.stepDelayMs <= 0)
        {
            return;
        }
        std::unique_lock lock(pauseMutex_);
        pauseCondition_.wait_for(lock, std::chrono::milliseconds(config_.stepDelayMs), [&]()
        {
            return stopRequested_.load() || paused_.load() || nextStepRequested_.load();
        });
    }

    std::vector<Script> loadScripts()
    {
        std::vector<std::string> seen;
        std::vector<Script> scripts = loadYamlTestScripts(absolutePath(config_.root, config_.yamlPath), seen);
        if (config_.compare)
        {
            scripts = compareAppPaths(scripts);
        }
        else if (!config_.appPath.empty())
        {
            scripts = overrideAppPaths(scripts, config_.appPath);
        }
        return scripts;
    }

    std::vector<Script> loadYamlTestScripts(const std::string& path, std::vector<std::string>& seen)
    {
        std::string absolute = absolutePath(config_.root, path);
        if (std::find(seen.begin(), seen.end(), absolute) != seen.end())
        {
            throw std::runtime_error("recursive MCP tester include: " + relativePath(config_.root, absolute));
        }
        seen.push_back(absolute);
        Json root = parseYaml(readFile(absolute));
        if (!root.isArray())
        {
            throw std::runtime_error("MCP tester YAML root must be an array");
        }

        std::vector<Script> scripts;
        Json::Array appItems;
        for (const Json& item : root.array())
        {
            const Json::Object& node = item.object();
            const Json* include = firstPresent(node, {"include", "includes"});
            if (include != nullptr)
            {
                for (const Json& includePath : includePaths(*include))
                {
                    std::string resolved = resolveConfigPath(config_.root, absolute, includePath.stringValue());
                    std::vector<Script> included = loadYamlTestScripts(resolved, seen);
                    scripts.insert(scripts.end(), included.begin(), included.end());
                }
                if (node.size() == 1)
                {
                    continue;
                }
            }
            appItems.push_back(item);
        }

        if (!appItems.empty())
        {
            scripts.push_back(scriptFromYamlItems(absolute, appItems));
        }
        if (scripts.empty())
        {
            throw std::runtime_error("MCP tester YAML must contain at least one app or include");
        }
        seen.pop_back();
        return scripts;
    }

    Script scriptFromYamlItems(const std::string& path, const Json::Array& items)
    {
        Script script;
        script.sourcePath = relativePath(config_.root, path);
        for (const Json& item : items)
        {
            const Json::Object& node = item.object();
            const Json* app = firstPresent(node, {"app", "path", "command"});
            if (app == nullptr)
            {
                throw std::runtime_error("YAML app item is missing app/path");
            }
            std::string appPath = resolveConfigPath(config_.root, path, app->stringValue());
            std::string name = node.count("name") ? node.at("name").stringValue() : basenameWithoutExtension(appPath);
            if (script.name.empty())
            {
                script.name = name;
            }
            if (script.apps.count(name))
            {
                throw std::runtime_error("duplicate app name in YAML: " + name);
            }
            script.apps[name] = appPath;
            script.env[name] = normalizeEnv(firstPresent(node, {"env", "environment"}));

            const Json* setup = firstPresent(node, {"setup", "before"});
            if (setup != nullptr)
            {
                appendSteps(script.setupSteps, *setup);
            }
            const Json* cleanup = firstPresent(node, {"cleanup", "after"});
            if (cleanup != nullptr)
            {
                std::vector<Step> cleanupItems;
                appendSteps(cleanupItems, *cleanup);
                cleanupItems.insert(cleanupItems.end(), script.cleanupSteps.begin(), script.cleanupSteps.end());
                script.cleanupSteps = cleanupItems;
            }

            const Json* tests = firstPresent(node, {"tests", "steps"});
            if (tests == nullptr || !tests->isArray())
            {
                throw std::runtime_error("YAML app item " + name + " must contain tests array");
            }
            for (const Json& step : tests->array())
            {
                Json normalized = step;
                normalized["target"] = name;
                script.steps.push_back({normalized});
            }
        }
        if (script.steps.empty())
        {
            throw std::runtime_error("MCP tester YAML must contain at least one test step");
        }
        return script;
    }

    const Json* firstPresent(const Json::Object& node, std::initializer_list<const char*> keys) const
    {
        for (const char* key : keys)
        {
            auto it = node.find(key);
            if (it != node.end())
            {
                return &it->second;
            }
        }
        return nullptr;
    }

    Json::Array includePaths(const Json& include) const
    {
        if (include.isArray())
        {
            return include.array();
        }
        return Json::Array{include};
    }

    void appendSteps(std::vector<Step>& steps, const Json& value) const
    {
        if (!value.isArray())
        {
            throw std::runtime_error("tester setup/cleanup must be an array");
        }
        for (const Json& step : value.array())
        {
            steps.push_back({step});
        }
    }

    std::map<std::string, std::string> normalizeEnv(const Json* value) const
    {
        std::map<std::string, std::string> env;
        if (value == nullptr)
        {
            return env;
        }
        if (!value->isObject())
        {
            throw std::runtime_error("YAML env must be a mapping");
        }
        for (const auto& [key, item] : value->object())
        {
            env[key] = item.stringValue();
        }
        return env;
    }

    std::vector<Script> overrideAppPaths(const std::vector<Script>& scripts, const std::string& appPath)
    {
        std::string resolved = resolveConfigPath(config_.root, absolutePath(config_.root, scripts.front().sourcePath), appPath);
        if (scripts.size() == 1 && scripts.front().apps.size() == 1 && !fs::is_directory(resolved))
        {
            std::vector<Script> result = scripts;
            result.front().apps.begin()->second = resolved;
            return result;
        }
        if (!fs::is_directory(resolved))
        {
            throw std::runtime_error("multiple-script app override requires an examples root directory");
        }
        std::vector<Script> result = scripts;
        for (Script& script : result)
        {
            if (script.apps.size() != 1)
            {
                throw std::runtime_error("examples root override requires every script to contain exactly one app");
            }
            std::string name = script.apps.begin()->first;
            script.apps.begin()->second = appPathFromExamplesRoot(resolved, name);
        }
        return result;
    }

    std::vector<Script> compareAppPaths(const std::vector<Script>& scripts)
    {
        std::vector<std::string> resolved = {
            resolveConfigPath(config_.root, absolutePath(config_.root, scripts.front().sourcePath), config_.compareA),
            resolveConfigPath(config_.root, absolutePath(config_.root, scripts.front().sourcePath), config_.compareB),
        };
        std::vector<std::string> names = compareTargetNames(resolved);
        std::vector<Script> result;
        for (const Script& script : scripts)
        {
            result.push_back(compareScriptAppPaths(script, resolved, names, scripts.size() > 1));
        }
        return result;
    }

    Script compareScriptAppPaths(
        const Script& script,
        const std::vector<std::string>& resolved,
        const std::vector<std::string>& names,
        bool requireExamplesRoots)
    {
        if (script.apps.size() != 1)
        {
            throw std::runtime_error("--compare requires each YAML test script to contain exactly one app");
        }
        std::string originalName = script.apps.begin()->first;
        Script result = script;
        result.apps.clear();
        result.env.clear();
        for (std::size_t index = 0; index < resolved.size(); ++index)
        {
            if (fs::is_directory(resolved[index]))
            {
                result.apps[names[index]] = appPathFromExamplesRoot(resolved[index], originalName);
            }
            else if (requireExamplesRoots)
            {
                throw std::runtime_error("--compare with include aggregate requires examples root directories");
            }
            else
            {
                result.apps[names[index]] = resolved[index];
            }
            auto envIt = script.env.find(originalName);
            if (envIt != script.env.end())
            {
                Json::Object envObject;
                for (const auto& [key, value] : envIt->second)
                {
                    envObject[key] = value;
                }
                Json formatted = formatTargetValue(Json(envObject), names[index]);
                result.env[names[index]] = normalizeEnv(&formatted);
            }
        }
        result.steps.clear();
        for (const Step& step : script.steps)
        {
            result.steps.push_back({compareStep(step.node, originalName)});
        }
        return result;
    }

    Json compareStep(const Json& step, const std::string& originalName) const
    {
        Json normalized = step;
        Json::Object& node = normalized.object();
        if (isTesterStep(node) || node.find("compare") != node.end() || node.find("tool") == node.end())
        {
            return normalized;
        }
        auto targetIt = node.find("target");
        std::string target = targetIt == node.end() ? std::string{} : targetIt->second.stringValue();
        if (!target.empty() && target != originalName && target != "*")
        {
            throw std::runtime_error("--compare cannot map YAML target '" + target + "'");
        }
        node["target"] = "*";
        return normalized;
    }

    std::vector<std::string> compareTargetNames(const std::vector<std::string>& paths) const
    {
        std::vector<std::string> names;
        for (std::size_t index = 0; index < paths.size(); ++index)
        {
            names.push_back(compareTargetName(paths[index], index));
        }
        if (names.size() >= 2 && names[0] == names[1])
        {
            names[0] = "app_a";
            names[1] = "app_b";
        }
        return names;
    }

    std::string compareTargetName(const std::string& path, std::size_t index) const
    {
        std::string normalized = pathString(fs::path(path));
        if (normalized.find("/python/") != std::string::npos || endsWith(normalized, ".py"))
        {
            return "python";
        }
        if (normalized.find("/cpp/") != std::string::npos)
        {
            return "cpp";
        }
        std::string base = basenameWithoutExtension(normalized);
        return base.empty() ? "app_" + std::to_string(index + 1) : base;
    }

    Viewport viewportForIndex(int index) const
    {
        return viewportForIndex(index, static_cast<int>(targetOrder_.size()));
    }

    Viewport viewportForIndex(int index, int targetCount) const
    {
        Viewport viewport;
        const ui::Rect bounds = appViewportBounds();
        if (config_.compareAppSize.has_value())
        {
            viewport.width = config_.compareAppSize->width;
            viewport.height = config_.compareAppSize->height;
            viewport.row = bounds.row;
            viewport.col = bounds.col + index * viewport.width;
            return viewport;
        }
        if (targetCount <= 1)
        {
            viewport.row = bounds.row;
            viewport.col = bounds.col;
            viewport.width = std::max(kInitialViewportWidth, bounds.width);
            viewport.height = std::max(kInitialViewportHeight, bounds.height);
            return viewport;
        }
        if (config_.compare)
        {
            viewport.width = std::max(kInitialViewportWidth, bounds.width / std::max(kInitialViewportWidth, targetCount));
            viewport.height = std::max(kInitialViewportHeight, bounds.height);
            viewport.row = bounds.row;
            viewport.col = bounds.col + index * viewport.width;
            return viewport;
        }
        const int cols = std::min({targetCount, std::max(kInitialViewportWidth, bounds.width),
                                   std::max(kInitialViewportWidth, static_cast<int>(std::ceil(std::sqrt(targetCount))))});
        const int rows = std::max(kInitialViewportHeight, static_cast<int>(std::ceil(static_cast<double>(targetCount) / cols)));
        const int rowIndex = index / cols;
        const int colIndex = index % cols;
        const int baseWidth = std::max(kInitialViewportWidth, bounds.width / cols);
        const int extraWidth = std::max(0, bounds.width % cols);
        const int baseHeight = std::max(kInitialViewportHeight, bounds.height / rows);
        const int extraHeight = std::max(0, bounds.height % rows);
        viewport.width = baseWidth + (colIndex < extraWidth ? 1 : 0);
        viewport.height = baseHeight + (rowIndex < extraHeight ? 1 : 0);
        viewport.row = bounds.row + rowIndex * baseHeight + std::min(rowIndex, extraHeight);
        viewport.col = bounds.col + colIndex * baseWidth + std::min(colIndex, extraWidth);
        return viewport;
    }

    ui::Rect appViewportBounds() const
    {
        if (shell_.app_viewport != nullptr && !shell_.app_viewport->frame().empty())
        {
            return shell_.app_viewport->frame();
        }
        return ui::Rect{
            0,
            0,
            config_.compareAppSize.has_value() ? config_.compareAppSize->width : kFallbackViewportWidth,
            config_.compareAppSize.has_value() ? config_.compareAppSize->height : kFallbackViewportHeight,
        };
    }

    std::string viewportSummary(const Script& script) const
    {
        std::string result;
        int index = 0;
        for (const auto& [name, _path] : script.apps)
        {
            if (!result.empty()) result += ", ";
            Viewport viewport = viewportForIndex(index++, static_cast<int>(script.apps.size()));
            result += name + "=" + std::to_string(viewport.row) + "," + std::to_string(viewport.col) +
                      " " + std::to_string(viewport.width) + "x" + std::to_string(viewport.height);
        }
        return result;
    }

    void runStep(const Step& step, int stepIndex)
    {
        const Json::Object& node = step.node.object();
        if (isTesterStep(node))
        {
            runTesterSteps({step}, "tester", *currentScript_);
            return;
        }
        if (node.find("compare") != node.end())
        {
            runCompareStep(step);
            return;
        }
        auto toolIt = node.find("tool");
        if (toolIt == node.end())
        {
            return;
        }
        std::string targetName = node.count("target") ? node.at("target").stringValue() : defaultTargetName();
        std::string tool = toolIt->second.stringValue();
        Json arguments = node.count("arguments") ? node.at("arguments") : Json::Object{};
        Json assertion = node.count("assert") ? node.at("assert") : Json(nullptr);
        std::optional<int> timeoutMs;
        if (node.count("timeout_ms"))
        {
            timeoutMs = static_cast<int>(node.at("timeout_ms").numberValue());
        }

        std::vector<std::string> names = targetNames(targetName);
        std::map<std::string, Json> results;
        std::vector<std::thread> threads;
        std::vector<std::shared_ptr<std::atomic<bool>>> doneFlags;
        std::mutex resultsMutex;
        std::optional<std::string> error;

        for (const std::string& name : names)
        {
            Json targetArguments = targetToolArguments(tool, arguments, name);
            log(name + ": " + tool + " " + targetArguments.dump());
            auto done = std::make_shared<std::atomic<bool>>(false);
            doneFlags.push_back(done);
            threads.emplace_back([&, name, done, targetArguments]()
            {
                try
                {
                    Json result = targets_.at(name)->callTool(tool, targetArguments);
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    results[name] = result;
                }
                catch (const std::exception& exc)
                {
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    error = name + ": " + exc.what();
                }
                done->store(true);
            });
        }
        joinWithLiveRepaint(threads, doneFlags, tool);
        if (error.has_value())
        {
            throw std::runtime_error(*error);
        }
        for (const auto& [name, result] : results)
        {
            log(name + ": -> " + compactResult(result));
            if (!assertion.isNull())
            {
                assertResult(name, result, formatTargetValue(assertion, name));
            }
        }
        if (config_.compare)
        {
            compareToolResults(tool, results, assertion);
        }
        if (handleExitedTargetsAfterStep(names, stepIndex))
        {
            return;
        }
        if (toolNeedsRepaint(tool))
        {
            paintAllFrames();
        }
        if (handleExitedTargetsAfterStep(names, stepIndex))
        {
            return;
        }
        if (config_.compare)
        {
            try
            {
                compareRenderSnapshots(names);
            }
            catch (const std::exception&)
            {
                if (handleExitedTargetsAfterStep(names, stepIndex))
                {
                    return;
                }
                throw;
            }
        }
        (void)stepIndex;
        (void)timeoutMs;
    }

    std::vector<std::string> targetNames(const std::string& targetName) const
    {
        if (targetName == "*")
        {
            std::vector<std::string> names;
            for (const auto& [name, _target] : targets_)
            {
                names.push_back(name);
            }
            return names;
        }
        if (targets_.count(targetName))
        {
            return {targetName};
        }
        throw std::runtime_error("unknown target: " + targetName);
    }

    Json targetToolArguments(const std::string& tool, const Json& arguments, const std::string& targetName) const
    {
        Json targetArguments = formatTargetValue(arguments, targetName);
        if (!config_.compare || (tool != "mouse_click" && tool != "mouse_drag"))
        {
            return targetArguments;
        }
        auto targetIt = targets_.find(targetName);
        if (targetIt == targets_.end() || !targetArguments.isObject())
        {
            return targetArguments;
        }
        const Viewport& viewport = targetIt->second->viewport();
        Json::Object adjusted = targetArguments.object();
        if (tool == "mouse_click")
        {
            if (auto it = adjusted.find("x"); it != adjusted.end())
            {
                it->second = static_cast<int>(it->second.numberValue()) + viewport.col;
            }
            if (auto it = adjusted.find("y"); it != adjusted.end())
            {
                it->second = static_cast<int>(it->second.numberValue()) + viewport.row;
            }
            return Json(adjusted);
        }
        for (const std::string& key : {"from_x", "to_x"})
        {
            if (auto it = adjusted.find(key); it != adjusted.end())
            {
                it->second = static_cast<int>(it->second.numberValue()) + viewport.col;
            }
        }
        for (const std::string& key : {"from_y", "to_y"})
        {
            if (auto it = adjusted.find(key); it != adjusted.end())
            {
                it->second = static_cast<int>(it->second.numberValue()) + viewport.row;
            }
        }
        return Json(adjusted);
    }

    std::vector<std::string> exitedTargetNames(const std::vector<std::string>& names)
    {
        std::vector<std::string> exited;
        for (const std::string& name : names)
        {
            auto target = targets_.find(name);
            if (target != targets_.end() && target->second->hasExited())
            {
                exited.push_back(name);
            }
        }
        return exited;
    }

    bool handleExitedTargetsAfterStep(const std::vector<std::string>& names, int stepIndex)
    {
        std::vector<std::string> exited = exitedTargetNames(names);
        if (exited.empty())
        {
            return false;
        }
        if (currentScript_ != nullptr && static_cast<std::size_t>(stepIndex) >= currentScript_->steps.size())
        {
            log("skip post-step snapshot after target exit: " + joinNames(exited));
            return true;
        }
        throw std::runtime_error("target exited before script completed: " + joinNames(exited));
    }

    std::string defaultTargetName() const
    {
        if (targets_.empty())
        {
            return {};
        }
        return targets_.begin()->first;
    }

    void assertResult(const std::string& targetName, const Json& result, const Json& assertion)
    {
        for (const auto& [field, expected] : assertion.object())
        {
            const Json* actual = jsonPath(result, field);
            std::string label;
            if (actual == nullptr || !assertionMatches(*actual, expected, label))
            {
                ++scriptAssertionsFailed_;
                ++assertionsFailed_;
                throw std::runtime_error(targetName + "." + field + " expected " + label +
                                         ", got " + (actual ? actual->dump() : "<missing>"));
            }
            ++scriptAssertionsPassed_;
            ++assertionsPassed_;
            log("ASSERT " + targetName + "." + field + " " + label);
        }
    }

    void resetScriptCounters()
    {
        scriptAssertionsPassed_ = 0;
        scriptAssertionsFailed_ = 0;
        scriptStepFailures_ = 0;
    }

    std::string scriptSummaryLine(const ScriptSummary& summary, const std::string& prefix) const
    {
        return prefix + " " + summary.name + ": " + summary.status + " " +
               std::to_string(summary.assertionsPassed) + " asserts passed, " +
               std::to_string(summary.assertionsFailed) + " failed, " +
               std::to_string(summary.stepFailures) + " step failures";
    }

    std::string resultSeparator(const std::string& line) const
    {
        int width = static_cast<int>(line.size());
        if (shell_.log_output != nullptr && shell_.log_output->frame().width > 0)
        {
            width = std::max(width, shell_.log_output->frame().width);
        }
        return std::string(static_cast<std::size_t>(width), kResultSeparatorChar);
    }

    void appendScriptSummary(bool success)
    {
        ScriptSummary summary;
        summary.name = currentScript_ == nullptr ? "script" : currentScript_->sourcePath + " [" + currentScript_->name + "]";
        summary.status = success ? "PASS" : "FAIL";
        summary.assertionsPassed = scriptAssertionsPassed_;
        summary.assertionsFailed = scriptAssertionsFailed_;
        summary.stepFailures = scriptStepFailures_;
        scriptSummaries_.push_back(summary);

        const std::string line = scriptSummaryLine(summary, "SUMMARY");
        log(resultSeparator(line));
        log(line);
        log("");
    }

    void appendSummary(int exitCode)
    {
        if (summaryDone_)
        {
            return;
        }
        summaryDone_ = true;
        if (!scriptSummaries_.empty())
        {
            log("SUMMARY: all app scripts");
            for (const ScriptSummary& summary : scriptSummaries_)
            {
                log(scriptSummaryLine(summary, "SUMMARY"));
            }
            log("RESULTS: all app scripts");
            for (const ScriptSummary& summary : scriptSummaries_)
            {
                log(scriptSummaryLine(summary, "RESULTS:"));
            }
        }
        if (exitCode == 0)
        {
            log("PASS: all MCP scripts completed");
        }
        const std::string result = "RESULT: " + std::to_string(assertionsPassed_) + " asserts passed, " +
                                   std::to_string(assertionsFailed_) + " failed, " +
                                   std::to_string(stepFailures_) + " step failures";
        const std::string separator = resultSeparator(result);
        log(separator);
        log(result);
        log(separator);
    }

    void runTesterSteps(const std::vector<Step>& steps, const std::string& label, const Script& script)
    {
        for (const Step& step : expandTesterSteps(steps, script))
        {
            const Json::Object& node = step.node.object();
            std::string action = testerAction(node);
            std::string suffix;
            auto path = node.find("path");
            if (path != node.end())
            {
                suffix = " " + path->second.stringValue();
            }
            log(label + ": " + action + suffix);
            Json result = runTesterFileStep(config_.root, step.node);
            log(label + ": -> " + compactResult(result));
        }
    }

    std::vector<Step> expandTesterSteps(const std::vector<Step>& steps, const Script& script) const
    {
        std::vector<Step> expanded;
        for (const Step& step : steps)
        {
            if (!hasTargetPlaceholder(step.node))
            {
                expanded.push_back(step);
                continue;
            }
            for (const auto& [name, _path] : script.apps)
            {
                expanded.push_back({formatTargetValue(step.node, name)});
            }
        }
        return expanded;
    }

    void runCompareStep(const Step& step)
    {
        const Json::Object& node = step.node.object();
        const Json* compare = node.at("compare").isArray() ? &node.at("compare") : nullptr;
        if (compare == nullptr || compare->array().size() < 2)
        {
            throw std::runtime_error("compare step requires at least two target names");
        }
        std::string snapshotTool = node.count("snapshot") ? node.at("snapshot").stringValue() : kSnapshotCompareTool;
        Json arguments = node.count("arguments") ? node.at("arguments") : Json::Object{};
        if (arguments.get("snapshot_time_ms") == nullptr)
        {
            arguments["snapshot_time_ms"] = currentEpochMillis();
        }

        std::vector<std::string> names;
        std::vector<Json> snapshots;
        for (const Json& target : compare->array())
        {
            std::string name = target.stringValue();
            names.push_back(name);
            log(name + ": " + snapshotTool + " " + arguments.dump());
            snapshots.push_back(targets_.at(name)->callTool(snapshotTool, arguments));
        }
        compareSnapshots(snapshotTool, names, snapshots);
        log("COMPARE " + snapshotTool + ": " + joinNames(names));
    }

    void compareToolResults(const std::string& tool, const std::map<std::string, Json>& results, const Json& assertion)
    {
        if (results.size() < 2)
        {
            return;
        }
        std::vector<std::string> fields;
        if (!assertion.isNull())
        {
            for (const auto& [field, expected] : assertion.object())
            {
                if (!hasTargetPlaceholder(expected))
                {
                    fields.push_back(field);
                }
            }
            if (fields.empty())
            {
                return;
            }
        }
        else if (!compareObservationTool(tool))
        {
            return;
        }

        auto first = results.begin();
        if ((tool == kSnapshotCompareTool || tool == kSnapshotCompactCompareTool) && fields.empty())
        {
            std::vector<std::string> names;
            std::vector<Json> snapshots;
            for (const auto& [name, result] : results)
            {
                names.push_back(name);
                snapshots.push_back(result);
            }
            compareSnapshots(tool, names, snapshots);
            log("COMPARE " + tool + ": " + joinNames(names) + " cells");
            return;
        }

        Json firstSelected = selectFields(first->second, fields);
        for (auto it = std::next(results.begin()); it != results.end(); ++it)
        {
            Json current = selectFields(it->second, fields);
            if (!jsonEqual(current, firstSelected))
            {
                throw std::runtime_error("compare mismatch for " + it->first + " on " + tool);
            }
        }
        std::vector<std::string> names;
        for (const auto& [name, _result] : results)
        {
            names.push_back(name);
        }
        log("COMPARE " + tool + ": " + joinNames(names));
    }

    void compareRenderSnapshots(const std::vector<std::string>& targetNames)
    {
        if (targetNames.size() < 2)
        {
            return;
        }
        std::map<std::string, Json> results;
        const double snapshotTimeMs = currentEpochMillis();
        for (const std::string& name : targetNames)
        {
            results[name] = targets_.at(name)->callTool(
                kSnapshotCompactCompareTool,
                Json::Object{{"snapshot_time_ms", snapshotTimeMs}});
        }
        std::vector<std::string> names;
        std::vector<Json> snapshots;
        for (const auto& [name, snapshot] : results)
        {
            names.push_back(name);
            snapshots.push_back(snapshot);
        }
        compareSnapshots(kSnapshotCompactCompareTool, names, snapshots);
        log("COMPARE " + std::string(kSnapshotCompactCompareTool) + ": " + joinNames(names));
    }

    void compareSnapshots(const std::string& tool, const std::vector<std::string>& names, const std::vector<Json>& snapshots)
    {
        if (snapshots.size() < 2)
        {
            return;
        }
        for (std::size_t index = 1; index < snapshots.size(); ++index)
        {
            std::optional<std::string> mismatch = renderSnapshotMismatch(snapshots.front(), snapshots[index]);
            if (mismatch.has_value())
            {
                std::string bundle = writeSnapshotBundle(tool, names, snapshots, *mismatch);
                if (!bundle.empty())
                {
                    log("snapshot dump: " + relativePath(config_.root, bundle));
                    log("viewer: python3 tools/mcp_snapshot_viewer.py " + relativePath(config_.root, bundle));
                }
                throw std::runtime_error("snapshot mismatch for " + names[index] + ": " + *mismatch);
            }
        }
    }

    Json selectFields(const Json& value, const std::vector<std::string>& fields) const
    {
        if (fields.empty())
        {
            return value;
        }
        Json::Object result;
        for (const std::string& field : fields)
        {
            const Json* fieldValue = jsonPath(value, field);
            result[field] = fieldValue == nullptr ? Json(nullptr) : *fieldValue;
        }
        return Json(result);
    }

    static bool compareObservationTool(const std::string& tool)
    {
        return tool == "get_element" || tool == "get_state" || tool == "get_window" ||
               tool == "get_render_snapshot" || tool == "get_render_snapshot_compact" ||
               tool == "get_render_rect";
    }

    std::string writeSnapshotBundle(
        const std::string& tool,
        const std::vector<std::string>& names,
        const std::vector<Json>& snapshots,
        const std::string& mismatch)
    {
        if (config_.snapshotDir.empty() || currentScript_ == nullptr)
        {
            return {};
        }
        std::time_t now = std::time(nullptr);
        std::tm localTime{};
        localtime_r(&now, &localTime);
        std::ostringstream timestamp;
        timestamp << std::put_time(&localTime, "%Y%m%d-%H%M%S");
        std::string scriptName = basenameWithoutExtension(currentScript_->sourcePath);
        scriptName = std::regex_replace(scriptName, std::regex("[^A-Za-z0-9_.-]+"), "_");
        std::string path = pathString(fs::path(config_.snapshotDir) /
            (timestamp.str() + "-step-" + std::to_string(stepIndex_) + "-" + scriptName + ".json"));

        Json::Array targets;
        for (std::size_t index = 0; index < names.size(); ++index)
        {
            targets.push_back(Json::Object{{"name", names[index]}, {"snapshot", snapshots[index]}});
        }
        Json::Object payload;
        payload["created_at"] = timestamp.str();
        payload["source_path"] = currentScript_->sourcePath;
        payload["step_index"] = stepIndex_;
        payload["tool"] = tool;
        payload["targets"] = targets;
        payload["mismatch"] = mismatch;
        payload["viewer"] = "python3 tools/mcp_snapshot_viewer.py " + relativePath(config_.root, path);

        fs::create_directories(fs::path(path).parent_path());
        std::ofstream output(path);
        output << Json(payload).dump() << "\n";
        return path;
    }

    static std::string joinNames(const std::vector<std::string>& names)
    {
        std::string result;
        for (const std::string& name : names)
        {
            if (!result.empty())
            {
                result += ", ";
            }
            result += name;
        }
        return result;
    }

    void joinWithLiveRepaint(
        std::vector<std::thread>& threads,
        const std::vector<std::shared_ptr<std::atomic<bool>>>& doneFlags,
        const std::string& tool)
    {
        if (!usesControlledBufferDisplay() || !liveOutputTool(tool))
        {
            for (auto& thread : threads)
            {
                thread.join();
            }
            return;
        }
        while (true)
        {
            bool alive = false;
            for (const auto& done : doneFlags)
            {
                if (!done->load())
                {
                    alive = true;
                    break;
                }
            }
            paintAllFrames();
            if (!alive)
            {
                for (auto& thread : threads)
                {
                    if (thread.joinable())
                    {
                        thread.join();
                    }
                }
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(kLiveBufferRepaintIntervalMs));
        }
    }

    static bool liveOutputTool(const std::string& tool)
    {
        return tool == "set_text" || tool == "type_text" || tool == "replace_selection" ||
               tool == "activate_element" || tool == "click_element";
    }

    static bool toolNeedsRepaint(const std::string& tool)
    {
        return !startsWith(tool, "get_");
    }

    bool usesTerminalUi() const
    {
        return stdoutIsTty_ && !config_.plain;
    }

    bool usesControlledBufferDisplay() const
    {
        return terminalUiStarted_ && !targetOrder_.empty();
    }

    bool usesInteractiveBufferPanel() const
    {
        return usesControlledBufferDisplay();
    }

    void beginTerminalUi(int targetCount)
    {
        if (!usesTerminalUi() || terminalUiStarted_)
        {
            return;
        }
        terminalUiStarted_ = true;
        terminalTargetCount_ = std::max(kInitialViewportWidth, targetCount);
        std::cout << "\x1b[?1049h\x1b[?1000h\x1b[?1002h\x1b[?1006h\x1b[?2004h\x1b[?7l\x1b[?25l\x1b[H\x1b[2J";
        renderTerminalShell(terminalTargetCount_);
        std::cout.flush();
    }

    void finishTerminalUi()
    {
        if (!terminalUiStarted_)
        {
            return;
        }
        std::cout << "\x1b[0m\x1b[>4;0m\x1b[?25h\x1b[?7h\x1b[?2004l\x1b[?1006l\x1b[?1002l\x1b[?1000l\x1b[?1049l";
        std::cout.flush();
        terminalUiStarted_ = false;
    }

    TerminalSize terminalSizeForUi() const
    {
        return terminalSize();
    }

    void openRepaintStatsLog()
    {
        const char* path = std::getenv("UIMD_MCP_TESTER_REPAINT_LOG");
        if (path == nullptr || std::string(path).empty())
        {
            return;
        }
        fs::path logPath(path);
        fs::path parent = logPath.parent_path();
        if (!parent.empty())
        {
            fs::create_directories(parent);
        }
        repaintStatsFile_.open(logPath, std::ios::app);
    }

    void writeRepaintStats(
        bool repaintBuffer,
        bool refreshLogChildren,
        const ui::TerminalBufferRenderStats& stats,
        std::size_t outputBytes
    )
    {
        if (!repaintStatsFile_.is_open())
        {
            return;
        }
        ++renderShellCalls_;
        repaintStatsFile_
            << currentTimeStamp()
            << " renderTerminalShell"
            << " call=" << renderShellCalls_
            << " repaint_buffer=" << (repaintBuffer ? 1 : 0)
            << " refresh_log_children=" << (refreshLogChildren ? 1 : 0)
            << " full_redraw=" << (stats.fullRedraw ? 1 : 0)
            << " changed_cells=" << stats.changedCells
            << " changed_runs=" << stats.changedRuns
            << " diff_bytes=" << stats.outputBytes
            << " output_bytes=" << outputBytes
            << " log_dynamic_refreshes=" << logDynamicRefreshes_
            << " frame_syncs=" << frameBufferSyncs_
            << "\n";
        repaintStatsFile_.flush();
    }

    void renderTerminalShell(int targetCount, bool repaintBuffer = true, bool refreshLogChildren = true)
    {
        if (!terminalUiStarted_)
        {
            return;
        }
        terminalTargetCount_ = std::max(kInitialViewportWidth, targetCount);
        TerminalSize size = terminalSizeForUi();
        std::lock_guard lock(terminalMutex_);
        shellBuffer_.resize(size.width, size.height);
        shellBuffer_.beginRenderStats();
        updatePauseButton();
        shell_.app_header->setText(appHeaderText());
        if (repaintBuffer)
        {
            syncFrameBufferViewFromCache();
        }
        if (refreshLogChildren)
        {
            ++logDynamicRefreshes_;
            shell_.log_output->invalidateDynamicChildren();
        }
        ui::renderGeneratedWindow(shell_, shellBuffer_, shellFocusIndex(), logPanelEdit_);
        std::string diff = renderShellDiff();
        writeRepaintStats(repaintBuffer, refreshLogChildren, shellBuffer_.renderStats(), diff.size());
        std::cout << "\x1b[0m" << diff;
        std::cout.flush();
    }

    std::string renderShellDiff()
    {
        std::string scrollOutput;
        if (pendingLogScrollDelta_ != 0 && shell_.log_output != nullptr)
        {
            const ui::Rect rect = shell_.log_output->frame();
            if (rect.col == 0 && rect.width >= shellBuffer_.width())
            {
                scrollOutput = shellBuffer_.renderScrollRegion(0, rect.row, rect.height, pendingLogScrollDelta_);
            }
            pendingLogScrollDelta_ = 0;
        }
        if (!shouldProtectBufferDisplay())
        {
            return scrollOutput + shellBuffer_.renderDiff();
        }

        const ui::Rect protectedRect = protectedBufferDisplayRect();
        if (protectedRect.empty())
        {
            return scrollOutput + shellBuffer_.renderDiff();
        }

        std::string output = std::move(scrollOutput);
        output += shellBuffer_.renderDiffRegion(
            0,
            0,
            0,
            0,
            protectedRect.row,
            shellBuffer_.width()
        );
        output += shellBuffer_.renderDiffRegion(
            0,
            0,
            protectedRect.row,
            0,
            protectedRect.height,
            protectedRect.col
        );
        output += shellBuffer_.renderDiffRegion(
            0,
            0,
            protectedRect.row,
            protectedRect.col + protectedRect.width,
            protectedRect.height,
            shellBuffer_.width() - protectedRect.col - protectedRect.width
        );
        output += shellBuffer_.renderDiffRegion(
            0,
            0,
            protectedRect.row + protectedRect.height,
            0,
            shellBuffer_.height() - protectedRect.row - protectedRect.height,
            shellBuffer_.width()
        );
        return output;
    }

    bool shouldProtectBufferDisplay() const
    {
        if (shell_.app_viewport != nullptr)
        {
            return false;
        }
        if (!targetOrder_.empty())
        {
            return true;
        }
        std::lock_guard lock(cachedFramesMutex_);
        return !cachedFrames_.empty();
    }

    ui::Rect protectedBufferDisplayRect() const
    {
        const ui::Rect bounds = appViewportBounds();
        const int count = std::max(kInitialViewportWidth, static_cast<int>(targetOrder_.size()));
        const int width = std::min(bounds.width, bufferVisibleWidth(bounds, count) * count);
        const int height = std::min(bounds.height, bufferVisibleHeight(bounds));
        return ui::Rect{bounds.row, bounds.col, width, height};
    }

    void renderLogPanel(bool refreshLogChildren = true)
    {
        if (!terminalUiStarted_)
        {
            return;
        }
        renderTerminalShell(terminalTargetCount_, false, refreshLogChildren);
    }

    void renderShellFocusOnly()
    {
        renderTerminalShell(terminalTargetCount_, false, false);
    }

    void renderShellBufferOnly()
    {
        renderTerminalShell(terminalTargetCount_, true, false);
    }

    std::string logKindForLine(const std::string& line) const
    {
        if (line.find("FAIL") != std::string::npos)
        {
            return "log_assert_fail";
        }
        if (line.find("ASSERT") != std::string::npos || line.find("PASS") != std::string::npos)
        {
            return "log_assert_ok";
        }
        return "log_normal";
    }

    std::vector<ui::RenderedContent> renderLogChildren(int width) const
    {
        std::vector<ui::RenderedContent> result;
        const int lineWidth = std::max(kInitialViewportWidth, width);
        std::vector<LogLine> lines;
        {
            std::lock_guard lock(logLinesMutex_);
            lines = logLines_;
        }
        for (const LogLine& entry : lines)
        {
            ui::Style style;
            if (std::optional<ui::Color> color = logColor(entry.kind))
            {
                style.color = *color;
            }
            if (std::optional<ui::Color> background = logBackground(entry.kind))
            {
                style.background = *background;
            }
            result.push_back(ui::renderPlainText(entry.text, lineWidth, kMinimumLogRows, style));
        }
        return result;
    }

    std::optional<ui::Color> logColor(const std::string& kind) const
    {
        return styleColor(logPanelStyleSource_.mcpSourceMarkdown(), "@" + kind, "color");
    }

    std::optional<ui::Color> logBackground(const std::string& kind) const
    {
        return styleColor(logPanelStyleSource_.mcpSourceMarkdown(), "@" + kind, "background");
    }

    std::optional<ui::Color> styleColor(const std::string& source, const std::string& selector, const std::string& property) const
    {
        std::optional<std::string> value = styleProperty(source, selector, property);
        if (!value.has_value())
        {
            return std::nullopt;
        }
        return ui::Color(*value);
    }

    std::optional<std::string> styleProperty(const std::string& source, const std::string& selector, const std::string& property) const
    {
        std::istringstream input(source);
        std::string line;
        bool inStyleBlock = false;
        bool inSelector = false;
        while (std::getline(input, line))
        {
            const std::string stripped = trim(line);
            if (stripped == "```yaml")
            {
                inStyleBlock = true;
                inSelector = false;
                continue;
            }
            if (inStyleBlock && stripped == "```")
            {
                inStyleBlock = false;
                inSelector = false;
                continue;
            }
            if (!inStyleBlock)
            {
                continue;
            }
            if (stripped == selector + ":")
            {
                inSelector = true;
                continue;
            }
            if (!stripped.empty() && stripped.back() == ':' && !startsWith(stripped, " "))
            {
                inSelector = false;
            }
            if (!inSelector || !startsWith(stripped, property + ":"))
            {
                continue;
            }
            std::string value = trim(stripped.substr(property.size() + 1));
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
            {
                value = value.substr(1, value.size() - 2);
            }
            return value;
        }
        return std::nullopt;
    }

    int shellFocusIndex() const
    {
        if (bufferPanelFocused_ || bufferPanelEdit_)
        {
            return -1;
        }
        int focusIndex = 0;
        for (const auto& element : shell_.elements())
        {
            if (element == nullptr || !isFocusableElement(*element))
            {
                continue;
            }
            if (element->name() == focusedName_)
            {
                return focusIndex;
            }
            ++focusIndex;
        }
        return -1;
    }

    bool isFocusableElement(const ui::Element& element) const
    {
        return dynamic_cast<const ui::Button*>(&element) != nullptr ||
               dynamic_cast<const ui::CheckBox*>(&element) != nullptr ||
               dynamic_cast<const ui::TextInput*>(&element) != nullptr ||
               dynamic_cast<const ui::NumberInput*>(&element) != nullptr ||
               dynamic_cast<const ui::ComboBox*>(&element) != nullptr ||
               dynamic_cast<const ui::ListBox*>(&element) != nullptr ||
               dynamic_cast<const ui::ScrollView*>(&element) != nullptr;
    }

    std::string appHeaderText() const
    {
        const ui::Rect header = appViewportBounds();
        if (header.width <= 0)
        {
            return {};
        }
        std::string text(static_cast<std::size_t>(header.width), ' ');
        if (currentScript_ == nullptr)
        {
            return text;
        }
        int index = 0;
        for (const auto& [name, _path] : currentScript_->apps)
        {
            const Viewport viewport = viewportForIndex(index++);
            const int start = viewport.col - header.col;
            const std::string label = centeredText(name, viewport.width);
            for (int col = 0; col < viewport.width && col < static_cast<int>(label.size()); ++col)
            {
                const int headerCol = start + col;
                if (headerCol >= 0 && headerCol < header.width)
                {
                    text[static_cast<std::size_t>(headerCol)] = label[static_cast<std::size_t>(col)];
                }
            }
        }
        return text;
    }

    void interactiveEventLoop()
    {
        ui::TerminalModeGuard mode(STDIN_FILENO, STDOUT_FILENO);
        ui::PosixTerminalBackend backend(STDIN_FILENO, STDOUT_FILENO);
        renderTerminalShell(terminalTargetCount_);
        while (!quitRequested_.load())
        {
            const std::vector<ui::Event> events = backend.readEvents();
            for (std::size_t index = 0; index < events.size(); ++index)
            {
                if (tryHandleCoalescedLogWheel(events, index))
                {
                    continue;
                }
                handleInteractiveEvent(events[index]);
            }
            if (finished_.load() && config_.exitOnFinish)
            {
                break;
            }
            if (restartRequested_.load())
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(kRequestRetryDelayMs));
        }
    }

    bool tryHandleCoalescedLogWheel(const std::vector<ui::Event>& events, std::size_t& index)
    {
        if (shell_.log_output == nullptr || index >= events.size())
        {
            return false;
        }
        const ui::Event& first = events[index];
        if (first.type != ui::EventType::MouseWheel || !shell_.log_output->frame().contains(first.position))
        {
            return false;
        }
        if (bufferPanelFocused_ || bufferPanelEdit_)
        {
            exitBufferPanelToPreFocus(false);
        }
        const ui::Size viewport = shell_.log_output->frame().size();
        int lineDelta = 0;
        while (index < events.size())
        {
            const ui::Event& event = events[index];
            if (event.type != ui::EventType::MouseWheel || !shell_.log_output->frame().contains(event.position))
            {
                --index;
                break;
            }
            const int lines = shell_.log_output->wheelScrollLines(viewport);
            lineDelta += event.wheelDelta > 0 ? lines : -lines;
            ++index;
        }
        if (index == events.size())
        {
            --index;
        }
        if (lineDelta != 0 && shell_.log_output->scrollLines(lineDelta, viewport))
        {
            pendingLogScrollDelta_ += shell_.log_output->consumeTerminalScrollDelta();
            renderLogPanel(false);
        }
        return true;
    }

    void handleInteractiveEvent(const ui::Event& event)
    {
        if (event.type == ui::EventType::MousePress)
        {
            if (pointInBufferDisplay(event.position))
            {
                setBufferPanelState(true, true);
                return;
            }
            if (bufferPanelFocused_ || bufferPanelEdit_)
            {
                setBufferPanelState(false, false);
                restorePreFocus();
            }
            if (focusElementAt(event.position))
            {
                if (isToolbarFocus(focusedName_))
                {
                    activateFocusedElement();
                }
            }
            return;
        }
        if (event.type == ui::EventType::MouseWheel)
        {
            if (shell_.log_output != nullptr && shell_.log_output->frame().contains(event.position))
            {
                if (bufferPanelFocused_ || bufferPanelEdit_)
                {
                    exitBufferPanelToPreFocus(false);
                }
                if (shell_.log_output->handleWheel(event.wheelDelta, shell_.log_output->frame().size()))
                {
                    pendingLogScrollDelta_ += shell_.log_output->consumeTerminalScrollDelta();
                }
                renderLogPanel(false);
            }
            return;
        }
        if (event.type != ui::EventType::Key)
        {
            return;
        }
        handleInteractiveKey(event.key);
    }

    void handleInteractiveKey(const std::string& key)
    {
        if (key == "Ctrl+C")
        {
            quitRequested_.store(true);
            requestStop();
            return;
        }
        if (usesInteractiveBufferPanel() && bufferPanelEdit_)
        {
            if (isKey(key, "Escape") || isKey(key, "Enter"))
            {
                setBufferPanelState(true, false);
                return;
            }
            if (isPanKey(key))
            {
                if (bufferPanelPan(key))
                {
                    repaintBufferTargetsFromCache();
                    fetchFramesFromMcp();
                    repaintBufferTargetsFromCache();
                }
                return;
            }
        }
        if (usesInteractiveBufferPanel() && bufferPanelFocused_)
        {
            if (isKey(key, "Enter"))
            {
                setBufferPanelState(true, true);
                return;
            }
            if (isKey(key, "Escape"))
            {
                exitBufferPanelToPreFocus(false);
                renderShellBufferOnly();
                return;
            }
            exitBufferPanelToPreFocus(false);
            routeBaseKey(key);
            renderShellBufferOnly();
            return;
        }

        if (usesInteractiveBufferPanel() && isKey(key, "Tab") && focusedName_ == "quit")
        {
            setBufferPanelState(true, false);
            return;
        }
        if (usesInteractiveBufferPanel() && isKey(key, "Shift+Tab") && focusedName_ == "log_output")
        {
            setBufferPanelState(true, false);
            return;
        }
        if (usesInteractiveBufferPanel() && isKey(key, "Down") && isToolbarFocus(focusedName_))
        {
            setBufferPanelState(true, false);
            return;
        }
        if (usesInteractiveBufferPanel() && isKey(key, "Up") && focusedName_ == "log_output" && !logPanelEdit_)
        {
            setBufferPanelState(true, false);
            return;
        }

        routeBaseKey(key);
    }

    void routeBaseKey(const std::string& key)
    {
        if (focusedName_ == "log_output" && logPanelEdit_)
        {
            handleLogPanelKey(key);
            return;
        }
        if (isKey(key, "Tab"))
        {
            moveFocus(1);
            return;
        }
        if (isKey(key, "Shift+Tab"))
        {
            moveFocus(-1);
            return;
        }
        if (isArrowKey(key))
        {
            moveFocusSpatial(key);
            return;
        }
        if (focusedName_ == "log_output")
        {
            handleLogPanelKey(key);
            return;
        }
        if (isKey(key, "Enter") || key == " ")
        {
            activateFocusedElement();
        }
    }

    void activateFocusedElement()
    {
        if (focusedName_ == "run")
        {
            restartRequested_.store(true);
            requestStop();
        }
        else if (focusedName_ == "next")
        {
            nextStepRequested_.store(true);
            pauseCondition_.notify_all();
        }
        else if (focusedName_ == "pause")
        {
            togglePause();
        }
        else if (focusedName_ == "copy")
        {
            copyLog();
        }
        else if (focusedName_ == "quit")
        {
            quitRequested_.store(true);
            requestStop();
        }
    }

    void togglePause()
    {
        if (finished_.load())
        {
            return;
        }
        if (!paused_.load())
        {
            paused_.store(true);
            nextStepRequested_.store(false);
            updatePauseButton();
            renderShellFocusOnly();
            log("paused");
            return;
        }
        paused_.store(false);
        nextStepRequested_.store(false);
        updatePauseButton();
        renderShellFocusOnly();
        log("resumed");
        pauseCondition_.notify_all();
    }

    void updatePauseButton()
    {
        if (shell_.pause != nullptr)
        {
            shell_.pause->setTitle(paused_.load() ? kPlayButtonTitle : kPauseButtonTitle);
        }
    }

    void copyLog()
    {
        std::string text;
        std::size_t count = 0;
        {
            std::lock_guard lock(logLinesMutex_);
            count = logLines_.size();
            for (const LogLine& line : logLines_)
            {
                text += line.text;
                text += "\n";
            }
        }
        (void)ui::copyTextToClipboard(text);
        log("copied " + std::to_string(count) + " log lines");
    }

    void handleLogPanelKey(const std::string& key)
    {
        if (isKey(key, "Enter"))
        {
            logPanelEdit_ = true;
            if (shell_.log_output != nullptr)
            {
                shell_.log_output->setAutoScroll(false);
            }
            renderShellBufferOnly();
            return;
        }
        if (isKey(key, "Escape"))
        {
            logPanelEdit_ = false;
            renderShellBufferOnly();
            return;
        }
        if (isPanKey(key) && shell_.log_output != nullptr)
        {
            if (shell_.log_output->handleKey(key, shell_.log_output->frame().size()))
            {
                pendingLogScrollDelta_ += shell_.log_output->consumeTerminalScrollDelta();
            }
            renderLogPanel(false);
        }
    }

    void moveFocus(int delta)
    {
        const std::vector<std::string> order = focusOrder();
        auto it = std::find(order.begin(), order.end(), focusedName_);
        int index = it == order.end() ? -1 : static_cast<int>(std::distance(order.begin(), it));
        index = (index + delta + static_cast<int>(order.size())) % static_cast<int>(order.size());
        focusedName_ = order[static_cast<std::size_t>(index)];
        logPanelEdit_ = false;
        renderShellFocusOnly();
    }

    void moveFocusSpatial(const std::string& key)
    {
        const std::vector<FocusCandidate> candidates = focusCandidates();
        if (candidates.empty())
        {
            return;
        }
        auto current = std::find_if(candidates.begin(), candidates.end(), [&](const FocusCandidate& candidate)
        {
            return candidate.name == focusedName_;
        });
        if (current == candidates.end())
        {
            focusedName_ = candidates.front().name;
            logPanelEdit_ = false;
            renderShellFocusOnly();
            return;
        }

        int bestIndex = -1;
        std::tuple<int, int, int, int> bestScore{};
        for (int index = 0; index < static_cast<int>(candidates.size()); ++index)
        {
            if (candidates[static_cast<std::size_t>(index)].name == focusedName_)
            {
                continue;
            }
            std::optional<std::tuple<int, int, int, int>> score =
                directionalFocusScore(current->rect, candidates[static_cast<std::size_t>(index)].rect, key);
            if (!score.has_value())
            {
                continue;
            }
            if (bestIndex < 0 || *score < bestScore)
            {
                bestIndex = index;
                bestScore = *score;
            }
        }
        if (bestIndex >= 0)
        {
            focusedName_ = candidates[static_cast<std::size_t>(bestIndex)].name;
            logPanelEdit_ = false;
            renderShellFocusOnly();
        }
    }

    std::vector<std::string> focusOrder() const
    {
        return {"run", "next", "pause", "copy", "quit", "log_output"};
    }

    struct FocusCandidate
    {
        std::string name;
        ui::Rect rect;
    };

    std::vector<FocusCandidate> focusCandidates() const
    {
        std::vector<FocusCandidate> result;
        for (const std::string& name : focusOrder())
        {
            ui::Element* element = mutableElementByName(name);
            if (element == nullptr)
            {
                continue;
            }
            ui::Rect rect = element->frame();
            if (rect.empty())
            {
                continue;
            }
            result.push_back(FocusCandidate{name, rect});
        }
        return result;
    }

    static int rectCenterRow(const ui::Rect& rect)
    {
        return rect.row * 2 + rect.height;
    }

    static int rectCenterCol(const ui::Rect& rect)
    {
        return rect.col * 2 + rect.width;
    }

    static bool axisOverlaps(int startA, int endA, int startB, int endB)
    {
        return std::max(startA, startB) < std::min(endA, endB);
    }

    static int axisGap(int startA, int endA, int startB, int endB)
    {
        if (endA <= startB)
        {
            return startB - endA;
        }
        if (endB <= startA)
        {
            return startA - endB;
        }
        return 0;
    }

    static std::optional<int> bandScanRank(int currentStart, int currentEnd, int candidateStart, int candidateEnd)
    {
        if (axisOverlaps(currentStart, currentEnd, candidateStart, candidateEnd))
        {
            return 0;
        }
        const int maxDistance =
            std::max({
                std::abs(candidateStart - currentStart),
                std::abs(candidateEnd - currentEnd),
                std::abs(candidateStart - currentEnd),
                std::abs(candidateEnd - currentStart),
            }) +
            std::max(1, currentEnd - currentStart) +
            std::max(1, candidateEnd - candidateStart);
        int rank = 1;
        for (int distance = 1; distance <= maxDistance; ++distance)
        {
            const int offsets[] = {distance, -distance};
            for (int offset : offsets)
            {
                if (axisOverlaps(currentStart + offset, currentEnd + offset, candidateStart, candidateEnd))
                {
                    return rank;
                }
                ++rank;
            }
        }
        return std::nullopt;
    }

    static std::optional<std::tuple<int, int, int, int>> directionalFocusScore(
        const ui::Rect& current,
        const ui::Rect& candidate,
        const std::string& direction)
    {
        int primaryGap = 0;
        int perpendicularGap = 0;
        int edgeDelta = 0;
        std::optional<int> bandRank;
        if (isKey(direction, "Right"))
        {
            if (rectCenterCol(candidate) <= rectCenterCol(current))
            {
                return std::nullopt;
            }
            primaryGap = std::max(0, candidate.col - (current.col + current.width));
            bandRank = bandScanRank(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            perpendicularGap = axisGap(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            edgeDelta = std::abs(candidate.row - current.row);
        }
        else if (isKey(direction, "Left"))
        {
            if (rectCenterCol(candidate) >= rectCenterCol(current))
            {
                return std::nullopt;
            }
            primaryGap = std::max(0, current.col - (candidate.col + candidate.width));
            bandRank = bandScanRank(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            perpendicularGap = axisGap(current.row, current.row + current.height, candidate.row, candidate.row + candidate.height);
            edgeDelta = std::abs(candidate.row - current.row);
        }
        else if (isKey(direction, "Down"))
        {
            if (rectCenterRow(candidate) <= rectCenterRow(current))
            {
                return std::nullopt;
            }
            primaryGap = std::max(0, candidate.row - (current.row + current.height));
            bandRank = bandScanRank(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            perpendicularGap = axisGap(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            edgeDelta = std::abs(candidate.col - current.col);
        }
        else if (isKey(direction, "Up"))
        {
            if (rectCenterRow(candidate) >= rectCenterRow(current))
            {
                return std::nullopt;
            }
            primaryGap = std::max(0, current.row - (candidate.row + candidate.height));
            bandRank = bandScanRank(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            perpendicularGap = axisGap(current.col, current.col + current.width, candidate.col, candidate.col + candidate.width);
            edgeDelta = std::abs(candidate.col - current.col);
        }
        else
        {
            return std::nullopt;
        }
        if (!bandRank.has_value())
        {
            return std::nullopt;
        }
        return std::tuple<int, int, int, int>{*bandRank, primaryGap, perpendicularGap, edgeDelta};
    }

    bool isToolbarFocus(const std::string& name) const
    {
        return name == "run" || name == "next" || name == "pause" || name == "copy" || name == "quit";
    }

    bool isArrowKey(const std::string& key) const
    {
        return isKey(key, "Up") || isKey(key, "Down") || isKey(key, "Left") || isKey(key, "Right");
    }

    bool isPanKey(const std::string& key) const
    {
        return isArrowKey(key);
    }

    static bool isKey(const std::string& key, const std::string& expected)
    {
        if (key == expected)
        {
            return true;
        }
        if (expected == "Up")
        {
            return key == "ArrowUp";
        }
        if (expected == "Down")
        {
            return key == "ArrowDown";
        }
        if (expected == "Left")
        {
            return key == "ArrowLeft";
        }
        if (expected == "Right")
        {
            return key == "ArrowRight";
        }
        return false;
    }

    void setBufferPanelState(bool focused, bool edit)
    {
        const bool changed = bufferPanelFocused_ != focused || bufferPanelEdit_ != edit;
        if (focused && !bufferPanelFocused_ && !bufferPanelEdit_)
        {
            bufferPreFocus_ = focusedName_;
        }
        bufferPanelFocused_ = focused;
        bufferPanelEdit_ = edit;
        if (changed)
        {
            renderShellBufferOnly();
        }
    }

    void restorePreFocus()
    {
        if (!bufferPreFocus_.empty())
        {
            focusedName_ = bufferPreFocus_;
            bufferPreFocus_.clear();
            renderShellBufferOnly();
        }
    }

    void leaveBufferPanelToPreFocus()
    {
        std::string focus = bufferPreFocus_.empty() ? kInitialFocusName : bufferPreFocus_;
        leaveBufferPanelTo(focus);
    }

    void exitBufferPanelToPreFocus(bool render)
    {
        const std::string focus = bufferPreFocus_.empty() ? kInitialFocusName : bufferPreFocus_;
        bufferPanelFocused_ = false;
        bufferPanelEdit_ = false;
        bufferPreFocus_.clear();
        focusedName_ = focus;
        logPanelEdit_ = false;
        if (render)
        {
            renderShellBufferOnly();
        }
    }

    void leaveBufferPanelTo(const std::string& focus)
    {
        bufferPanelFocused_ = false;
        bufferPanelEdit_ = false;
        bufferPreFocus_.clear();
        focusedName_ = focus.empty() ? kInitialFocusName : focus;
        logPanelEdit_ = false;
        renderShellBufferOnly();
    }

    bool pointInBufferDisplay(ui::Point point) const
    {
        if (!usesInteractiveBufferPanel() || targets_.empty())
        {
            return false;
        }
        const ui::Rect bounds = appViewportBounds();
        const int count = std::max(kInitialViewportWidth, static_cast<int>(targetOrder_.size()));
        const int visibleWidth = bufferVisibleWidth(bounds, count);
        const int visibleHeight = bufferVisibleHeight(bounds);
        return point.row >= bounds.row && point.row < bounds.row + visibleHeight &&
               point.col >= bounds.col && point.col < bounds.col + visibleWidth * count;
    }

    bool focusElementAt(ui::Point point)
    {
        for (const std::string& name : focusOrder())
        {
            ui::Element* element = mutableElementByName(name);
            if (element != nullptr && element->frame().contains(point))
            {
                focusedName_ = name;
                logPanelEdit_ = false;
                renderShellFocusOnly();
                return true;
            }
        }
        return false;
    }

    ui::Element* mutableElementByName(const std::string& name) const
    {
        for (const auto& element : shell_.elements())
        {
            if (element != nullptr && element->name() == name)
            {
                return element.get();
            }
        }
        return nullptr;
    }

    int bufferVisibleWidth(const ui::Rect& bounds, int count) const
    {
        const int available = std::max(kInitialViewportWidth, bounds.width / std::max(kInitialViewportWidth, count));
        if (!config_.compareAppSize.has_value())
        {
            return available;
        }
        return std::max(kInitialViewportWidth, std::min(available, config_.compareAppSize->width));
    }

    int bufferVisibleHeight(const ui::Rect& bounds) const
    {
        if (!config_.compareAppSize.has_value())
        {
            return std::max(kInitialViewportHeight, bounds.height);
        }
        return std::max(kInitialViewportHeight, std::min(std::max(kInitialViewportHeight, bounds.height), config_.compareAppSize->height));
    }

    bool bufferPanelPan(const std::string& key)
    {
        const ui::Rect bounds = appViewportBounds();
        const int count = std::max(kInitialViewportWidth, static_cast<int>(targetOrder_.size()));
        const int visibleHeight = bufferVisibleHeight(bounds);
        const int visibleWidth = bufferVisibleWidth(bounds, count);
        const int targetHeight = config_.compareAppSize.has_value() ? config_.compareAppSize->height : visibleHeight;
        const int targetWidth = config_.compareAppSize.has_value() ? config_.compareAppSize->width : visibleWidth;
        const int maxV = std::max(0, targetHeight - visibleHeight);
        const int maxH = std::max(0, targetWidth - visibleWidth);
        const int previousV = comparePanV_;
        const int previousH = comparePanH_;
        if (isKey(key, "Up"))
        {
            comparePanV_ = std::max(0, comparePanV_ - 1);
        }
        else if (isKey(key, "Down"))
        {
            comparePanV_ = std::min(maxV, comparePanV_ + 1);
        }
        else if (isKey(key, "Left"))
        {
            comparePanH_ = std::max(0, comparePanH_ - 1);
        }
        else if (isKey(key, "Right"))
        {
            comparePanH_ = std::min(maxH, comparePanH_ + 1);
        }
        return previousV != comparePanV_ || previousH != comparePanH_;
    }

    void fetchFramesFromMcp()
    {
        std::vector<std::thread> threads;
        std::map<std::string, Json> frames;
        std::mutex framesMutex;
        for (const std::string& name : targetOrder_)
        {
            auto target = targets_.find(name);
            if (target == targets_.end())
            {
                continue;
            }
            threads.emplace_back([&, name]()
            {
                try
                {
                    Json frame = targets_.at(name)->callTool("get_render_frame", Json::Object{});
                    std::lock_guard lock(framesMutex);
                    frames[name] = std::move(frame);
                }
                catch (const std::exception&)
                {
                }
            });
        }
        for (auto& thread : threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        if (!frames.empty())
        {
            std::lock_guard lock(cachedFramesMutex_);
            for (auto& [name, frame] : frames)
            {
                cachedFrames_[name] = std::move(frame);
            }
        }
    }

    void requestBufferRepaint()
    {
        bufferRepaintRequested_.store(true);
        bufferRepaintCondition_.notify_all();
    }

    void startBufferRepaintThread()
    {
        stopBufferRepaintThread();
        bufferRepaintStop_.store(false);
        bufferRepaintRequested_.store(false);
        bufferRepaintThread_ = std::thread([this]()
        {
            bufferRepaintWorker();
        });
    }

    void stopBufferRepaintThread()
    {
        bufferRepaintStop_.store(true);
        bufferRepaintCondition_.notify_all();
        if (bufferRepaintThread_.joinable())
        {
            bufferRepaintThread_.join();
        }
        bufferRepaintStop_.store(false);
        bufferRepaintRequested_.store(false);
    }

    void bufferRepaintWorker()
    {
        while (!bufferRepaintStop_.load())
        {
            std::unique_lock lock(bufferRepaintMutex_);
            bufferRepaintCondition_.wait(lock, [&]()
            {
                return bufferRepaintStop_.load() || bufferRepaintRequested_.load();
            });
            bufferRepaintRequested_.store(false);
            lock.unlock();
            if (bufferRepaintStop_.load())
            {
                break;
            }
            if (!usesControlledBufferDisplay() || targets_.empty())
            {
                continue;
            }
            fetchFramesFromMcp();
            repaintBufferTargetsFromCache();
        }
    }

    std::map<std::string, ui::FrameBufferRegion> frameBufferRegions() const
    {
        std::map<std::string, ui::FrameBufferRegion> regions;
        const ui::Rect bounds = appViewportBounds();
        const int count = std::max(kInitialViewportWidth, static_cast<int>(targetOrder_.size()));
        const int visibleWidth = bufferVisibleWidth(bounds, count);
        const int visibleHeight = bufferVisibleHeight(bounds);
        for (int index = 0; index < static_cast<int>(targetOrder_.size()); ++index)
        {
            regions[targetOrder_[static_cast<std::size_t>(index)]] = ui::FrameBufferRegion{
                .row = 0,
                .col = index * visibleWidth,
                .width = visibleWidth,
                .height = visibleHeight,
            };
        }
        return regions;
    }

    static std::vector<std::vector<ui::FrameBufferCell>> frameBufferCells(const Json& frame)
    {
        std::vector<std::vector<ui::FrameBufferCell>> result;
        for (const std::vector<FrameCell>& row : frameCells(frame))
        {
            std::vector<ui::FrameBufferCell> outputRow;
            outputRow.reserve(row.size());
            for (const FrameCell& cell : row)
            {
                outputRow.push_back(ui::FrameBufferCell{
                    .text = cell.text,
                    .foreground = cell.fg,
                    .background = cell.bg,
                });
            }
            result.push_back(std::move(outputRow));
        }
        return result;
    }

    void syncFrameBufferViewFromCache()
    {
        if (shell_.app_viewport == nullptr)
        {
            return;
        }
        ++frameBufferSyncs_;
        std::map<std::string, Json> cached;
        {
            std::lock_guard cacheLock(cachedFramesMutex_);
            cached = cachedFrames_;
        }
        std::map<std::string, std::vector<std::vector<ui::FrameBufferCell>>> frames;
        for (const std::string& name : targetOrder_)
        {
            auto frame = cached.find(name);
            if (frame != cached.end())
            {
                frames[name] = frameBufferCells(frame->second);
            }
        }
        shell_.app_viewport->setFrames(std::move(frames), targetOrder_, frameBufferRegions());
        shell_.app_viewport->setPan(comparePanV_, comparePanH_);
        shell_.app_viewport->setDimmed(logPanelEdit_);
        shell_.app_viewport->setInteractionState(bufferPanelFocused_, bufferPanelEdit_);
    }

    void repaintBufferTargetsFromCache()
    {
        if (!usesControlledBufferDisplay())
        {
            return;
        }
        renderShellBufferOnly();
    }

    void clearBufferGutters(const ui::Rect& bounds, int count, int visibleWidth, int visibleHeight)
    {
        const int displayWidth = visibleWidth * count;
        const std::string fill = ansiFill(std::max(0, bounds.width - displayWidth), appViewportBackground());
        if (!fill.empty())
        {
            for (int row = 0; row < visibleHeight; ++row)
            {
                std::cout << "\x1b[" << (bounds.row + row + 1) << ";" << (bounds.col + displayWidth + 1) << "H" << fill;
            }
        }
        const std::string rowFill = ansiFill(bounds.width, appViewportBackground());
        for (int row = visibleHeight; row < bounds.height; ++row)
        {
            std::cout << "\x1b[" << (bounds.row + row + 1) << ";" << (bounds.col + 1) << "H" << rowFill;
        }
    }

    std::string ansiFill(int width, const std::string& background) const
    {
        if (width <= 0)
        {
            return {};
        }
        FrameCell cell;
        cell.bg = background;
        cell.text = " ";
        std::string chunk = cellAnsi(cell);
        std::string result;
        for (int index = 0; index < width; ++index)
        {
            result += chunk;
        }
        result += "\x1b[0m";
        return result;
    }

    std::string appViewportBackground() const
    {
        if (shell_.app_viewport != nullptr && shell_.app_viewport->style().background.has_value())
        {
            return shell_.app_viewport->style().background->toString();
        }
        return {};
    }

    void drawBufferCorners()
    {
        if (!usesInteractiveBufferPanel() || !usesControlledBufferDisplay())
        {
            return;
        }
        const char* color = nullptr;
        if (bufferPanelEdit_)
        {
            color = kBufferCornerEditColor;
        }
        else if (bufferPanelFocused_)
        {
            color = kBufferCornerFocusColor;
        }
        if (color == nullptr)
        {
            return;
        }
        const ui::Rect bounds = appViewportBounds();
        const int count = std::max(kInitialViewportWidth, static_cast<int>(targetOrder_.size()));
        const int visibleWidth = bufferVisibleWidth(bounds, count);
        const int visibleHeight = bufferVisibleHeight(bounds);
        if (visibleWidth < 2 || visibleHeight < 2)
        {
            return;
        }
        std::lock_guard terminalLock(terminalMutex_);
        std::ostringstream output;
        for (int index = 0; index < static_cast<int>(targetOrder_.size()); ++index)
        {
            const int startCol = bounds.col + index * visibleWidth;
            const int endCol = startCol + visibleWidth - 1;
            const int startRow = bounds.row;
            const int endRow = bounds.row + visibleHeight - 1;
            output << "\x1b[" << (startRow + 1) << ";" << (startCol + 1) << "H"
                   << cornerAnsi("┌", color, frameBackgroundAt(index, 0, 0))
                   << cornerAnsi("─", color, frameBackgroundAt(index, 0, 1));
            output << "\x1b[" << (startRow + 1) << ";" << endCol << "H"
                   << cornerAnsi("─", color, frameBackgroundAt(index, 0, visibleWidth - 2))
                   << cornerAnsi("┐", color, frameBackgroundAt(index, 0, visibleWidth - 1));
            output << "\x1b[" << (endRow + 1) << ";" << (startCol + 1) << "H"
                   << cornerAnsi("└", color, frameBackgroundAt(index, visibleHeight - 1, 0))
                   << cornerAnsi("─", color, frameBackgroundAt(index, visibleHeight - 1, 1));
            output << "\x1b[" << (endRow + 1) << ";" << endCol << "H"
                   << cornerAnsi("─", color, frameBackgroundAt(index, visibleHeight - 1, visibleWidth - 2))
                   << cornerAnsi("┘", color, frameBackgroundAt(index, visibleHeight - 1, visibleWidth - 1));
        }
        std::cout << output.str() << "\x1b[0m";
        std::cout.flush();
    }

    std::string frameBackgroundAt(int targetIndex, int rowOffset, int colOffset) const
    {
        if (targetIndex < 0 || targetIndex >= static_cast<int>(targetOrder_.size()))
        {
            return {};
        }
        std::lock_guard lock(cachedFramesMutex_);
        auto frame = cachedFrames_.find(targetOrder_[static_cast<std::size_t>(targetIndex)]);
        if (frame == cachedFrames_.end())
        {
            return {};
        }
        auto cells = frameCells(frame->second);
        const int row = comparePanV_ + rowOffset;
        const int col = comparePanH_ + colOffset;
        if (row >= 0 && row < static_cast<int>(cells.size()) &&
            col >= 0 && col < static_cast<int>(cells[static_cast<std::size_t>(row)].size()))
        {
            return cells[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)].bg;
        }
        return {};
    }

    std::string cornerAnsi(const std::string& text, const std::string& fg, const std::string& bg) const
    {
        FrameCell cell;
        cell.text = text;
        cell.fg = fg;
        cell.bg = bg;
        return cellAnsi(cell);
    }

    static std::string dimHexColor(const std::string& color)
    {
        if (color.size() != 7 || color.front() != '#')
        {
            return color;
        }
        try
        {
            int red = static_cast<int>(std::round((1.0 - kBufferScopeDimAlpha) * std::stoi(color.substr(1, 2), nullptr, 16)));
            int green = static_cast<int>(std::round((1.0 - kBufferScopeDimAlpha) * std::stoi(color.substr(3, 2), nullptr, 16)));
            int blue = static_cast<int>(std::round((1.0 - kBufferScopeDimAlpha) * std::stoi(color.substr(5, 2), nullptr, 16)));
            std::ostringstream out;
            out << "#" << std::hex << std::setfill('0') << std::setw(2) << red
                << std::setw(2) << green << std::setw(2) << blue;
            return out.str();
        }
        catch (const std::exception&)
        {
            return color;
        }
    }

    void paintAllFrames()
    {
        if (!usesControlledBufferDisplay() || !stdoutIsTty_)
        {
            return;
        }
        fetchFramesFromMcp();
        repaintBufferTargetsFromCache();
        renderLogPanel();
        std::cout.flush();
    }

    void paintFrame(int index, const Json& frame)
    {
        Viewport viewport = viewportForIndex(index);
        auto cells = frameCells(frame);
        for (int row = 0; row < viewport.height; ++row)
        {
            std::cout << "\x1b[" << (viewport.row + row + 1) << ";" << (viewport.col + 1) << "H";
            if (row < static_cast<int>(cells.size()))
            {
                for (int col = 0; col < viewport.width; ++col)
                {
                    if (col < static_cast<int>(cells[static_cast<std::size_t>(row)].size()))
                    {
                        std::cout << cellAnsi(cells[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)]);
                    }
                    else
                    {
                        std::cout << ' ';
                    }
                }
            }
            else
            {
                std::cout << std::string(static_cast<std::size_t>(viewport.width), ' ');
            }
            std::cout << "\x1b[0m";
        }
    }

    static std::string compactResult(const Json& value)
    {
        if (const Json* field = value.get("value"))
        {
            return "value='" + field->stringValue() + "'";
        }
        if (const Json* field = value.get("title"))
        {
            return "title='" + field->stringValue() + "'";
        }
        return value.dump().substr(0, 160);
    }

    void log(const std::string& line)
    {
        std::string entry = currentTimeStamp() + " " + line;
        if (logFile_.is_open())
        {
            logFile_ << entry << "\n";
        }
        if (!terminalUiStarted_)
        {
            std::cout << "\x1b[0m" << entry << "\n";
            return;
        }

        std::istringstream input(entry);
        std::string part;
        bool emitted = false;
        {
            std::lock_guard lock(logLinesMutex_);
            while (std::getline(input, part))
            {
                logLines_.push_back({part, logKindForLine(part)});
                emitted = true;
            }
            if (!emitted)
            {
                logLines_.push_back({{}, "log_normal"});
            }
        }
        renderLogPanel();
    }

    TesterConfig config_;
    McpTesterUI shell_;
    LogPanelUI logPanelStyleSource_;
    ui::TerminalBuffer shellBuffer_{kInitialViewportWidth, kInitialViewportHeight};
    std::ofstream repaintStatsFile_;
    std::size_t renderShellCalls_ = 0;
    std::size_t logDynamicRefreshes_ = 0;
    std::size_t frameBufferSyncs_ = 0;
    std::map<std::string, std::unique_ptr<TargetApp>> targets_;
    std::vector<std::string> targetOrder_;
    std::ofstream logFile_;
    const Script* currentScript_ = nullptr;
    int stepIndex_ = 0;
    std::vector<ScriptSummary> scriptSummaries_;
    bool summaryDone_ = false;
    int assertionsPassed_ = 0;
    int assertionsFailed_ = 0;
    int stepFailures_ = 0;
    int scriptAssertionsPassed_ = 0;
    int scriptAssertionsFailed_ = 0;
    int scriptStepFailures_ = 0;
    bool stdoutIsTty_ = ::isatty(STDOUT_FILENO);
    bool terminalUiStarted_ = false;
    int terminalTargetCount_ = kInitialViewportWidth;
    std::vector<LogLine> logLines_;
    std::string focusedName_ = kInitialFocusName;
    bool logPanelEdit_ = false;
    bool bufferPanelFocused_ = false;
    bool bufferPanelEdit_ = false;
    int pendingLogScrollDelta_ = 0;
    std::string bufferPreFocus_;
    int comparePanV_ = 0;
    int comparePanH_ = 0;
    std::map<std::string, Json> cachedFrames_;
    mutable std::mutex cachedFramesMutex_;
    mutable std::mutex logLinesMutex_;
    std::recursive_mutex terminalMutex_;
    std::atomic<bool> stopRequested_{false};
    std::atomic<bool> quitRequested_{false};
    std::atomic<bool> restartRequested_{false};
    std::atomic<bool> paused_{false};
    std::atomic<bool> finished_{false};
    std::atomic<bool> workerRunning_{false};
    std::atomic<bool> nextStepRequested_{false};
    std::mutex pauseMutex_;
    std::condition_variable pauseCondition_;
    std::thread bufferRepaintThread_;
    std::atomic<bool> bufferRepaintStop_{false};
    std::atomic<bool> bufferRepaintRequested_{false};
    std::mutex bufferRepaintMutex_;
    std::condition_variable bufferRepaintCondition_;
};

void printHelp(std::ostream& out, const char* program)
{
    out << "usage: " << (program == nullptr ? "uimd_mcp_tester" : program)
        << " [-h] [--all] [--app APP] [--compare APP_A APP_B]\n"
        << "                        [--mcp-fast] [--mcp-action-delay-ms MCP_ACTION_DELAY_MS]\n"
        << "                        [--mcp-type-delay-ms MCP_TYPE_DELAY_MS]\n"
        << "                        [--step-delay-ms STEP_DELAY_MS] [--exit-on-finish]\n"
        << "                        [--log-file LOG_FILE] [--snapshot-dir SNAPSHOT_DIR]\n"
        << "                        [--compare-viewport WIDTHxHEIGHT] [--plain]\n"
        << "                        [path ...]\n\n"
        << "Universal visual MCP tester\n\n"
        << "positional arguments:\n"
        << "  path                  APP_PATH_OR_ROOT YAML config\n\n"
        << "options:\n"
        << "  -h, --help            show this help message and exit\n"
        << "  --all                 Run all known example MCP scripts\n"
        << "  --app APP             App path or examples root directory\n"
        << "  --compare APP_A APP_B Run the same YAML test against two app processes at the same time\n"
        << "  --mcp-fast            Run with zero MCP action, typing, and step delays.\n"
        << "  --mcp-action-delay-ms MCP_ACTION_DELAY_MS\n"
        << "  --mcp-type-delay-ms MCP_TYPE_DELAY_MS\n"
        << "  --step-delay-ms STEP_DELAY_MS\n"
        << "  --exit-on-finish\n"
        << "  --log-file LOG_FILE\n"
        << "  --snapshot-dir SNAPSHOT_DIR\n"
        << "                        Directory where compare snapshot mismatch bundles are written.\n"
        << "  --compare-viewport, --compare-app-size WIDTHxHEIGHT\n"
        << "                        Fixed per-app viewport size; omit or use auto to use the tester app area.\n"
        << "  --plain, --headless   Run without the interactive tester window.\n";
}

bool hasHelpArg(int argc, char** argv)
{
    for (int index = 1; index < argc; ++index)
    {
        const std::string arg = argv[index];
        if (arg == "-h" || arg == "--help")
        {
            return true;
        }
    }
    return false;
}

TesterConfig parseArgs(int argc, char** argv)
{
    TesterConfig config;
    std::vector<std::string> paths;
    for (int index = 1; index < argc; ++index)
    {
        std::string arg = argv[index];
        auto requireValue = [&](const std::string& option)
        {
            if (index + 1 >= argc)
            {
                throw std::runtime_error(option + " requires a value");
            }
            return std::string(argv[++index]);
        };
        if (arg == "-h" || arg == "--help")
        {
            continue;
        }
        else if (arg == "--compare")
        {
            config.compare = true;
            config.compareA = requireValue(arg);
            config.compareB = requireValue(arg);
        }
        else if (arg == "--all")
        {
            config.all = true;
        }
        else if (arg == "--app")
        {
            config.appPath = requireValue(arg);
        }
        else if (arg == "--mcp-fast")
        {
            config.mcpFast = true;
            config.actionDelayMs = 0;
            config.typeDelayMs = 0;
            config.stepDelayMs = 0;
        }
        else if (arg == "--mcp-action-delay-ms")
        {
            config.actionDelayMs = std::stoi(requireValue(arg));
        }
        else if (arg == "--mcp-type-delay-ms")
        {
            config.typeDelayMs = std::stoi(requireValue(arg));
        }
        else if (arg == "--step-delay-ms")
        {
            config.stepDelayMs = std::stoi(requireValue(arg));
        }
        else if (arg == "--exit-on-finish")
        {
            config.exitOnFinish = true;
        }
        else if (arg == "--plain" || arg == "--headless")
        {
            config.plain = true;
        }
        else if (arg == "--log-file")
        {
            config.logFile = requireValue(arg);
        }
        else if (arg == "--snapshot-dir")
        {
            config.snapshotDir = requireValue(arg);
        }
        else if (arg == "--compare-app-size" || arg == "--compare-viewport")
        {
            std::string value = requireValue(arg);
            std::string normalized = trim(value);
            std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char ch)
            {
                return static_cast<char>(std::tolower(ch));
            });
            if (normalized.empty() || normalized == "auto")
            {
                config.compareAppSize.reset();
                continue;
            }
            auto x = value.find('x');
            if (x == std::string::npos)
            {
                throw std::runtime_error(arg + " expects WIDTHxHEIGHT");
            }
            if (!std::regex_match(value, std::regex(R"(\s*[0-9]+x[0-9]+\s*)")))
            {
                throw std::runtime_error(arg + " expects WIDTHxHEIGHT");
            }
            Viewport viewport;
            viewport.width = std::stoi(value.substr(0, x));
            viewport.height = std::stoi(value.substr(x + 1));
            if (viewport.width <= 0 || viewport.height <= 0)
            {
                throw std::runtime_error(arg + " dimensions must be positive");
            }
            config.compareAppSize = viewport;
        }
        else if (startsWith(arg, "--"))
        {
            throw std::runtime_error("unsupported option: " + arg);
        }
        else
        {
            paths.push_back(arg);
        }
    }
    config.root = findProjectRoot(argc > 0 ? argv[0] : nullptr);
    if (config.all)
    {
        if (config.compare && !config.appPath.empty())
        {
            throw std::runtime_error("use either --compare or --app, not both");
        }
        if (!paths.empty())
        {
            throw std::runtime_error("--all does not accept a YAML config path");
        }
        config.yamlPath = kAllExamplesConfig;
    }
    else if (config.compare)
    {
        if (paths.size() != 1)
        {
            throw std::runtime_error("--compare expects APP_A APP_B YAML config");
        }
        config.yamlPath = paths[0];
    }
    else if (paths.size() == 2)
    {
        if (!config.appPath.empty())
        {
            throw std::runtime_error("use either --app or positional APP_PATH, not both");
        }
        config.appPath = paths[0];
        config.yamlPath = paths[1];
    }
    else if (paths.size() == 1)
    {
        config.yamlPath = paths[0];
    }
    else
    {
        throw std::runtime_error("expected APP YAML or YAML");
    }
    if (config.compare && !config.appPath.empty())
    {
        throw std::runtime_error("use either --compare or --app, not both");
    }
    if (!config.logFile.empty())
    {
        config.logFile = safeTestPath(config.root, config.logFile);
    }
    if (!config.snapshotDir.empty())
    {
        config.snapshotDir = safeTestPath(config.root, config.snapshotDir);
    }
    return config;
}

} // namespace

int main(int argc, char** argv)
{
    try
    {
        if (hasHelpArg(argc, argv))
        {
            printHelp(std::cout, argc > 0 ? argv[0] : "uimd_mcp_tester");
            return 0;
        }
        TesterConfig config = parseArgs(argc, argv);
        return Tester(std::move(config)).run();
    }
    catch (const std::exception& exc)
    {
        std::cerr << "uimd_mcp_tester: " << exc.what() << "\n";
        return 2;
    }
}

#pragma once

#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "NativeGrid.hpp"

namespace uimd::tool
{

struct YamlValue;

using YamlList = std::vector<YamlValue>;

struct YamlMap
{
    using Entry = std::pair<std::string, YamlValue>;
    using Container = std::vector<Entry>;
    using iterator = Container::iterator;
    using const_iterator = Container::const_iterator;

    Container entries;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] iterator begin();
    [[nodiscard]] iterator end();
    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator end() const;
    [[nodiscard]] iterator find(const std::string& key);
    [[nodiscard]] const_iterator find(const std::string& key) const;
    [[nodiscard]] bool contains(const std::string& key) const;
    YamlValue& operator[](const std::string& key);
    [[nodiscard]] const YamlValue* get(const std::string& key) const;
};

struct YamlValue
{
    using Storage = std::variant<std::nullptr_t, bool, long long, double, std::string, YamlList, YamlMap>;

    Storage value = nullptr;

    [[nodiscard]] bool isMap() const;
    [[nodiscard]] const YamlMap& asMap() const;
};

struct NativeDocument
{
    std::string title;
    YamlMap metadata;
    YamlMap definition;
    YamlMap members;
    YamlMap tools;
    YamlMap style;
    std::string uiText;
    std::string sourceText;
    std::vector<NativeLayoutItem> layout;
};

NativeDocument parseDocumentFile(const std::string& path);
std::string inspectDocumentJson(const NativeDocument& document);

}  // namespace uimd::tool

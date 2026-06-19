#pragma once

#include <filesystem>
#include <string>

namespace uimd::tool
{

struct IssueReportOptions
{
    std::string title;
    std::string kind = "bug";
    std::string targets = "python,cpp";
    bool anonymizeNames = true;
    bool keepDescriptions = false;
};

std::string generateIssueReport(const std::filesystem::path& sourcePath, const IssueReportOptions& options);

}  // namespace uimd::tool

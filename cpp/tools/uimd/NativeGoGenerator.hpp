#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace uimd::tool
{

struct NativeGoGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generateGoSources(
    const std::filesystem::path& sourcePath,
    const NativeGoGenerateOptions& options
);

std::string goModuleFile(const std::string& project, const std::string& localRuntimeReference);
std::string goAppTemplate(const std::string& classNameValue);

}  // namespace uimd::tool

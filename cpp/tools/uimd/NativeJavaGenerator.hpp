#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace uimd::tool
{

struct NativeJavaGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
    std::string packageName;
};

std::vector<std::filesystem::path> generateJavaSources(
    const std::filesystem::path& sourcePath,
    const NativeJavaGenerateOptions& options
);

std::string javaBuildFile(const std::string& project);
std::string javaSettingsFile(const std::string& project, const std::string& localRuntimeReference);
std::string javaAppTemplate(
    const std::string& classNameValue,
    const std::string& packageName = {});

}  // namespace uimd::tool

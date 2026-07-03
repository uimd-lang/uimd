#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace uimd::tool
{

struct NativeSwiftGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generateSwiftSources(
    const std::filesystem::path& sourcePath,
    const NativeSwiftGenerateOptions& options
);

std::string swiftPackageManifest(
    const std::string& project,
    const std::string& localRuntimeReference,
    const std::string& sourceFileName = "",
    const std::vector<std::string>& targetSources = {},
    const std::vector<std::string>& targetExcludes = {}
);

}  // namespace uimd::tool

#pragma once

#include <filesystem>
#include <vector>

namespace uimd::tool
{

struct NativeCppGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generateCppSources(
    const std::filesystem::path& sourcePath,
    const NativeCppGenerateOptions& options
);

}  // namespace uimd::tool

#pragma once

#include <filesystem>
#include <vector>

namespace uimd::tool
{

struct NativeGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool compileDependencies = true;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generatePythonSources(
    const std::filesystem::path& sourcePath,
    const NativeGenerateOptions& options
);

}  // namespace uimd::tool

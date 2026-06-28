#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace uimd::tool
{

struct NativeCSharpGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generateCSharpSources(
    const std::filesystem::path& sourcePath,
    const NativeCSharpGenerateOptions& options
);

std::string csharpRuntimeReferenceProperties(const std::string& localRuntimeReference);

}  // namespace uimd::tool

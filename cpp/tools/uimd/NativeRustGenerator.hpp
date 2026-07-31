#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace uimd::tool
{

struct NativeRustGenerateOptions
{
    std::filesystem::path outputDir;
    bool hasOutputDir = false;
    bool generateAppStub = false;
    bool mcpEnabled = true;
};

std::vector<std::filesystem::path> generateRustSources(
    const std::filesystem::path& sourcePath,
    const NativeRustGenerateOptions& options
);

std::string rustCargoManifest(const std::string& project, const std::string& localRuntimeReference);
std::string rustAppTemplate(const std::string& classNameValue);

}  // namespace uimd::tool

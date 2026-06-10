#include "NativeModel.hpp"
#include "NativeCppGenerator.hpp"
#include "NativePythonGenerator.hpp"

// Native UIMD command entry point.
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#else
#include <process.h>
#endif

#ifndef UIMD_VERSION
#define UIMD_VERSION "0.0.0"
#endif

#ifndef UIMD_NATIVE_SOURCE_ROOT
#define UIMD_NATIVE_SOURCE_ROOT ""
#endif

namespace
{

constexpr int EXIT_OK = 0;
constexpr int EXIT_USAGE = 2;
constexpr int EXIT_ERROR = 1;
constexpr std::size_t SDK_PRUNE_PATCH_RETENTION = 2;

const std::filesystem::path SOURCE_CHECKOUT_MARKER{"cpp/tools/uimd/main.cpp"};
const std::filesystem::path SDK_CURRENT_FILE{"current"};
const std::filesystem::path SDK_ROOT_DIR{"sdk"};
const std::filesystem::path SDK_BIN_DIR{"bin"};
const std::filesystem::path SDK_TARGETS_DIR{"targets"};
const std::filesystem::path SDK_PYTHON_TARGET_DIR{"python"};
const std::filesystem::path SDK_EXAMPLES_DIR{"examples"};
const std::string SDK_VERSION_METADATA_KEY = "sdk-version";
const std::string REQUIRE_SDK_VERSION_ENV = "UIMD_REQUIRE_SDK_VERSION";
const std::string NO_AUTO_INSTALL_ENV = "UIMD_NO_AUTO_INSTALL";
const std::string RELEASE_ROOT_ENV = "UIMD_RELEASE_ROOT";
const std::string SDK_PATH_ENV = "UIMD_SDK_PATH";
const std::string RELEASE_MANIFEST_PREFIX = "uimd-sdk-";
const std::string RELEASE_MANIFEST_SUFFIX = ".manifest";
const std::vector<std::string> SUPPORTED_SDK_TARGETS{"python", "cpp"};

struct GlobalOptions
{
    bool requireSdkVersion = false;
    bool offline = false;
};

struct SdkResolution
{
    std::string version;
    std::string requiredVersion;
    std::filesystem::path sourcePath;
    bool sourceFound = false;
    bool metadataFound = false;
    bool usedLegacyFallback = false;
    bool crossedMinorSeries = false;
};

struct ReleaseManifestFile
{
    std::filesystem::path destination;
    std::string checksum;
    std::filesystem::path source;
};

struct ReleaseManifest
{
    std::string version;
    std::filesystem::path manifestPath;
    std::vector<ReleaseManifestFile> files;
};

std::string runtimeVersion()
{
    return UIMD_VERSION;
}

std::string envValue(const char* name)
{
    const char* value = std::getenv(name);
    return value == nullptr ? std::string{} : std::string{value};
}

char pathSeparator();
std::string jsonEscape(const std::string& text);

bool hasSourceCheckoutMarker(const std::filesystem::path& root)
{
    return std::filesystem::is_regular_file(root / SOURCE_CHECKOUT_MARKER);
}

std::filesystem::path sourceRoot()
{
    const std::string overrideRoot = envValue("UIMD_SOURCE_ROOT");
    if (!overrideRoot.empty() && hasSourceCheckoutMarker(overrideRoot))
    {
        return std::filesystem::absolute(overrideRoot);
    }

    const std::filesystem::path configured{UIMD_NATIVE_SOURCE_ROOT};
    if (!configured.empty() && hasSourceCheckoutMarker(configured))
    {
        return std::filesystem::absolute(configured);
    }

    std::filesystem::path current = std::filesystem::current_path();
    while (true)
    {
        if (hasSourceCheckoutMarker(current))
        {
            return current;
        }
        if (!current.has_parent_path() || current.parent_path() == current)
        {
            break;
        }
        current = current.parent_path();
    }

    return {};
}

std::string pathString(const std::filesystem::path& path)
{
    return path.lexically_normal().string();
}

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

std::string lower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool envEnabled(const char* name)
{
    const std::string value = lower(trim(envValue(name)));
    return !value.empty() && value != "0" && value != "false" && value != "no" && value != "off";
}

std::string yamlStringValue(const uimd::tool::YamlMap& map, const std::string& key, const std::string& fallback = "")
{
    const uimd::tool::YamlValue* value = map.get(key);
    if (value == nullptr)
    {
        return fallback;
    }
    if (std::holds_alternative<std::string>(value->value))
    {
        return std::get<std::string>(value->value);
    }
    if (std::holds_alternative<long long>(value->value))
    {
        return std::to_string(std::get<long long>(value->value));
    }
    return fallback;
}

std::filesystem::path sdkHome()
{
    const std::string overrideHome = envValue("UIMD_HOME");
    if (!overrideHome.empty())
    {
        return std::filesystem::path{overrideHome};
    }
#ifdef _WIN32
    const std::string localAppData = envValue("LOCALAPPDATA");
    if (!localAppData.empty())
    {
        return std::filesystem::path{localAppData} / "uimd";
    }
    const std::string userProfile = envValue("USERPROFILE");
    if (!userProfile.empty())
    {
        return std::filesystem::path{userProfile} / ".uimd";
    }
#else
    const std::string home = envValue("HOME");
    if (!home.empty())
    {
        return std::filesystem::path{home} / ".uimd";
    }
#endif
    return std::filesystem::path{".uimd"};
}

std::filesystem::path uimdExecutableName()
{
#ifdef _WIN32
    return std::filesystem::path{"uimd.exe"};
#else
    return std::filesystem::path{"uimd"};
#endif
}

std::filesystem::path launcherPath(const std::filesystem::path& home)
{
    return home / SDK_BIN_DIR / uimdExecutableName();
}

std::filesystem::path sdkVersionRoot(const std::filesystem::path& home, const std::string& version)
{
    return home / SDK_ROOT_DIR / version;
}

std::filesystem::path sdkVersionBinary(const std::filesystem::path& home, const std::string& version)
{
    return sdkVersionRoot(home, version) / SDK_BIN_DIR / uimdExecutableName();
}

std::filesystem::path sdkTargetRoot(const std::filesystem::path& home, const std::string& version, const std::string& target)
{
    return sdkVersionRoot(home, version) / SDK_TARGETS_DIR / target;
}

bool isVersionNameSafe(const std::string& version)
{
    if (version.empty() || version == "." || version == "..")
    {
        return false;
    }
    for (char ch : version)
    {
        const unsigned char uch = static_cast<unsigned char>(ch);
        if (!std::isalnum(uch) && ch != '.' && ch != '-' && ch != '_' && ch != '+')
        {
            return false;
        }
    }
    return true;
}

bool isSdkTargetNameSafe(const std::string& target)
{
    return !target.empty() && std::all_of(target.begin(), target.end(), [](unsigned char ch) {
        return std::isalnum(ch) || ch == '-' || ch == '_';
    });
}

bool isSupportedSdkTarget(const std::string& target)
{
    return std::find(SUPPORTED_SDK_TARGETS.begin(), SUPPORTED_SDK_TARGETS.end(), target) != SUPPORTED_SDK_TARGETS.end();
}

bool numericTokenLess(const std::string& left, const std::string& right)
{
    std::size_t leftStart = left.find_first_not_of('0');
    std::size_t rightStart = right.find_first_not_of('0');
    const std::string leftTrimmed = leftStart == std::string::npos ? std::string{"0"} : left.substr(leftStart);
    const std::string rightTrimmed = rightStart == std::string::npos ? std::string{"0"} : right.substr(rightStart);
    if (leftTrimmed.size() != rightTrimmed.size())
    {
        return leftTrimmed.size() < rightTrimmed.size();
    }
    if (leftTrimmed != rightTrimmed)
    {
        return leftTrimmed < rightTrimmed;
    }
    return left.size() < right.size();
}

bool tokenIsNumeric(const std::string& token)
{
    return !token.empty() && std::all_of(token.begin(), token.end(), [](unsigned char ch) {
        return std::isdigit(ch);
    });
}

std::vector<std::string> versionTokens(const std::string& version)
{
    std::vector<std::string> tokens;
    std::string current;
    for (char ch : version)
    {
        const unsigned char uch = static_cast<unsigned char>(ch);
        if (std::isalnum(uch))
        {
            current += ch;
        }
        else if (!current.empty())
        {
            tokens.push_back(current);
            current.clear();
        }
    }
    if (!current.empty())
    {
        tokens.push_back(current);
    }
    return tokens;
}

bool versionLess(const std::string& left, const std::string& right)
{
    const std::vector<std::string> leftTokens = versionTokens(left);
    const std::vector<std::string> rightTokens = versionTokens(right);
    const std::size_t common = std::min(leftTokens.size(), rightTokens.size());
    for (std::size_t index = 0; index < common; ++index)
    {
        const bool leftNumeric = tokenIsNumeric(leftTokens[index]);
        const bool rightNumeric = tokenIsNumeric(rightTokens[index]);
        if (leftNumeric && rightNumeric)
        {
            if (numericTokenLess(leftTokens[index], rightTokens[index]))
            {
                return true;
            }
            if (numericTokenLess(rightTokens[index], leftTokens[index]))
            {
                return false;
            }
        }
        else if (leftTokens[index] != rightTokens[index])
        {
            return leftTokens[index] < rightTokens[index];
        }
    }
    if (leftTokens.size() != rightTokens.size())
    {
        return leftTokens.size() < rightTokens.size();
    }
    return left < right;
}

std::filesystem::path normalizedAbsolutePath(const std::filesystem::path& path)
{
    std::error_code error;
    std::filesystem::path normalized = std::filesystem::weakly_canonical(path, error);
    if (!error)
    {
        return normalized;
    }
    return std::filesystem::absolute(path).lexically_normal();
}

bool samePath(const std::filesystem::path& left, const std::filesystem::path& right)
{
    return normalizedAbsolutePath(left) == normalizedAbsolutePath(right);
}

std::filesystem::path findExecutableInPath(const std::string& executable)
{
    const std::string pathValue = envValue("PATH");
    std::size_t start = 0;
    while (start <= pathValue.size())
    {
        const std::size_t end = pathValue.find(pathSeparator(), start);
        const std::string directory = pathValue.substr(start, end == std::string::npos ? std::string::npos : end - start);
        if (!directory.empty())
        {
            const std::filesystem::path candidate = std::filesystem::path{directory} / executable;
            if (std::filesystem::is_regular_file(candidate))
            {
                return normalizedAbsolutePath(candidate);
            }
        }
        if (end == std::string::npos)
        {
            break;
        }
        start = end + 1;
    }
    return {};
}

std::filesystem::path currentExecutablePath(const char* argv0)
{
    if (argv0 == nullptr || std::string{argv0}.empty())
    {
        return {};
    }
    const std::filesystem::path raw{argv0};
    if (raw.is_absolute() || raw.has_parent_path())
    {
        return normalizedAbsolutePath(raw);
    }
    const std::filesystem::path fromPath = findExecutableInPath(raw.string());
    if (!fromPath.empty())
    {
        return fromPath;
    }
    return normalizedAbsolutePath(raw);
}

std::filesystem::path sdkOverrideBinary()
{
    const std::string overridePath = envValue(SDK_PATH_ENV.c_str());
    if (overridePath.empty())
    {
        return {};
    }

    const std::filesystem::path configured{overridePath};
    if (std::filesystem::is_regular_file(configured))
    {
        return normalizedAbsolutePath(configured);
    }
    if (std::filesystem::is_directory(configured))
    {
        const std::vector<std::filesystem::path> candidates{
            configured / SDK_BIN_DIR / uimdExecutableName(),
            configured / "cpp" / "build" / "tools" / "uimd" / uimdExecutableName(),
            configured / uimdExecutableName(),
        };
        for (const std::filesystem::path& candidate : candidates)
        {
            if (std::filesystem::is_regular_file(candidate))
            {
                return normalizedAbsolutePath(candidate);
            }
        }
    }
    return normalizedAbsolutePath(configured);
}

bool copyExecutableFile(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    std::error_code error;
    std::filesystem::create_directories(destination.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination.parent_path()) << ": " << error.message() << "\n";
        return false;
    }
    if (std::filesystem::is_regular_file(destination) && samePath(source, destination))
    {
        return true;
    }
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing, error);
    if (error)
    {
        std::cerr << "error: cannot copy " << pathString(source) << " to " << pathString(destination) << ": " << error.message() << "\n";
        return false;
    }
#ifndef _WIN32
    std::filesystem::permissions(
        destination,
        std::filesystem::perms::owner_exec | std::filesystem::perms::group_exec | std::filesystem::perms::others_exec,
        std::filesystem::perm_options::add,
        error
    );
    if (error)
    {
        std::cerr << "error: cannot make " << pathString(destination) << " executable: " << error.message() << "\n";
        return false;
    }
#endif
    return true;
}

bool copyRegularFile(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    std::error_code error;
    std::filesystem::create_directories(destination.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination.parent_path()) << ": " << error.message() << "\n";
        return false;
    }
    if (std::filesystem::is_regular_file(destination) && samePath(source, destination))
    {
        return true;
    }
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing, error);
    if (error)
    {
        std::cerr << "error: cannot copy " << pathString(source) << " to " << pathString(destination) << ": " << error.message() << "\n";
        return false;
    }
    return true;
}

bool isSdkExecutableDestination(const std::filesystem::path& destination)
{
    return destination == (SDK_BIN_DIR / uimdExecutableName());
}

bool safeRelativePath(const std::filesystem::path& path)
{
    if (path.empty() || path.is_absolute())
    {
        return false;
    }
    for (const std::filesystem::path& part : path)
    {
        const std::string text = part.string();
        if (text.empty() || text == "." || text == "..")
        {
            return false;
        }
    }
    return true;
}

std::vector<unsigned char> readBinaryFile(const std::filesystem::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        throw std::runtime_error("cannot read " + pathString(path));
    }
    return std::vector<unsigned char>(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>()
    );
}

std::uint32_t rotateRight(std::uint32_t value, std::uint32_t bits)
{
    return (value >> bits) | (value << (32U - bits));
}

std::string sha256Hex(const std::vector<unsigned char>& input)
{
    static constexpr std::array<std::uint32_t, 64> ROUND_CONSTANTS{
        0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U,
        0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
        0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
        0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
        0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU,
        0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
        0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U,
        0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
        0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
        0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
        0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U,
        0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
        0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U,
        0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
        0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
        0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U,
    };

    std::vector<unsigned char> message = input;
    const std::uint64_t bitLength = static_cast<std::uint64_t>(message.size()) * 8U;
    message.push_back(0x80U);
    while ((message.size() % 64U) != 56U)
    {
        message.push_back(0U);
    }
    for (int shift = 56; shift >= 0; shift -= 8)
    {
        message.push_back(static_cast<unsigned char>((bitLength >> static_cast<unsigned>(shift)) & 0xffU));
    }

    std::array<std::uint32_t, 8> hash{
        0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
        0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U,
    };

    for (std::size_t offset = 0; offset < message.size(); offset += 64U)
    {
        std::array<std::uint32_t, 64> words{};
        for (std::size_t index = 0; index < 16U; ++index)
        {
            const std::size_t pos = offset + index * 4U;
            words[index] =
                (static_cast<std::uint32_t>(message[pos]) << 24U) |
                (static_cast<std::uint32_t>(message[pos + 1U]) << 16U) |
                (static_cast<std::uint32_t>(message[pos + 2U]) << 8U) |
                static_cast<std::uint32_t>(message[pos + 3U]);
        }
        for (std::size_t index = 16U; index < words.size(); ++index)
        {
            const std::uint32_t s0 = rotateRight(words[index - 15U], 7U) ^ rotateRight(words[index - 15U], 18U) ^ (words[index - 15U] >> 3U);
            const std::uint32_t s1 = rotateRight(words[index - 2U], 17U) ^ rotateRight(words[index - 2U], 19U) ^ (words[index - 2U] >> 10U);
            words[index] = words[index - 16U] + s0 + words[index - 7U] + s1;
        }

        std::uint32_t a = hash[0];
        std::uint32_t b = hash[1];
        std::uint32_t c = hash[2];
        std::uint32_t d = hash[3];
        std::uint32_t e = hash[4];
        std::uint32_t f = hash[5];
        std::uint32_t g = hash[6];
        std::uint32_t h = hash[7];

        for (std::size_t index = 0; index < words.size(); ++index)
        {
            const std::uint32_t s1 = rotateRight(e, 6U) ^ rotateRight(e, 11U) ^ rotateRight(e, 25U);
            const std::uint32_t ch = (e & f) ^ ((~e) & g);
            const std::uint32_t temp1 = h + s1 + ch + ROUND_CONSTANTS[index] + words[index];
            const std::uint32_t s0 = rotateRight(a, 2U) ^ rotateRight(a, 13U) ^ rotateRight(a, 22U);
            const std::uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            const std::uint32_t temp2 = s0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    }

    std::ostringstream output;
    output << std::hex << std::setfill('0');
    for (std::uint32_t value : hash)
    {
        output << std::setw(8) << value;
    }
    return output.str();
}

std::string sha256File(const std::filesystem::path& path)
{
    return sha256Hex(readBinaryFile(path));
}

bool looksLikeSha256(const std::string& value)
{
    return value.size() == 64U && std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isxdigit(ch);
    });
}

std::vector<std::string> splitWhitespace(const std::string& line)
{
    std::istringstream input(line);
    std::vector<std::string> parts;
    std::string part;
    while (input >> part)
    {
        parts.push_back(part);
    }
    return parts;
}

std::filesystem::path localReleaseManifestPath(const std::filesystem::path& releaseRoot, const std::string& version)
{
    const std::filesystem::path versionManifest = releaseRoot / version / "manifest.txt";
    if (std::filesystem::is_regular_file(versionManifest))
    {
        return versionManifest;
    }
    return releaseRoot / (RELEASE_MANIFEST_PREFIX + version + RELEASE_MANIFEST_SUFFIX);
}

bool localReleaseManifestExists(const std::filesystem::path& releaseRoot, const std::string& version)
{
    return std::filesystem::is_regular_file(localReleaseManifestPath(releaseRoot, version));
}

std::vector<std::string> localReleaseVersions(const std::filesystem::path& releaseRoot)
{
    std::vector<std::string> versions;
    if (!std::filesystem::is_directory(releaseRoot))
    {
        return versions;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(releaseRoot))
    {
        std::string version;
        if (entry.is_directory())
        {
            version = entry.path().filename().string();
            if (!std::filesystem::is_regular_file(entry.path() / "manifest.txt"))
            {
                version.clear();
            }
        }
        else if (entry.is_regular_file())
        {
            const std::string name = entry.path().filename().string();
            if (name.starts_with(RELEASE_MANIFEST_PREFIX) && name.ends_with(RELEASE_MANIFEST_SUFFIX))
            {
                version = name.substr(
                    RELEASE_MANIFEST_PREFIX.size(),
                    name.size() - RELEASE_MANIFEST_PREFIX.size() - RELEASE_MANIFEST_SUFFIX.size()
                );
            }
        }

        if (isVersionNameSafe(version) && std::find(versions.begin(), versions.end(), version) == versions.end())
        {
            versions.push_back(version);
        }
    }
    std::sort(versions.begin(), versions.end(), versionLess);
    return versions;
}

ReleaseManifest readReleaseManifest(const std::filesystem::path& releaseRoot, const std::string& version)
{
    ReleaseManifest manifest;
    manifest.version = version;
    manifest.manifestPath = localReleaseManifestPath(releaseRoot, version);

    std::ifstream input(manifest.manifestPath);
    if (!input)
    {
        throw std::runtime_error("release manifest not found: " + pathString(manifest.manifestPath));
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line))
    {
        ++lineNumber;
        line = trim(line);
        if (line.empty() || line.front() == '#')
        {
            continue;
        }
        const std::vector<std::string> parts = splitWhitespace(line);
        if (parts.empty())
        {
            continue;
        }
        if (parts[0] == "version")
        {
            if (parts.size() != 2U)
            {
                throw std::runtime_error("invalid version entry in release manifest line " + std::to_string(lineNumber));
            }
            if (parts[1] != version)
            {
                throw std::runtime_error("release manifest version " + parts[1] + " does not match requested " + version);
            }
        }
        else if (parts[0] == "file")
        {
            if (parts.size() != 4U)
            {
                throw std::runtime_error("invalid file entry in release manifest line " + std::to_string(lineNumber));
            }
            ReleaseManifestFile file;
            file.destination = std::filesystem::path{parts[1]};
            file.checksum = lower(parts[2]);
            file.source = std::filesystem::path{parts[3]};
            if (!safeRelativePath(file.destination) || !safeRelativePath(file.source))
            {
                throw std::runtime_error("release manifest file paths must be safe relative paths on line " + std::to_string(lineNumber));
            }
            if (!looksLikeSha256(file.checksum))
            {
                throw std::runtime_error("release manifest checksum must be SHA-256 hex on line " + std::to_string(lineNumber));
            }
            manifest.files.push_back(file);
        }
        else
        {
            throw std::runtime_error("unknown release manifest entry on line " + std::to_string(lineNumber) + ": " + parts[0]);
        }
    }

    if (manifest.files.empty())
    {
        throw std::runtime_error("release manifest has no file entries: " + pathString(manifest.manifestPath));
    }
    return manifest;
}

bool installReleaseManifest(const std::filesystem::path& home, const std::filesystem::path& releaseRoot, const std::string& version)
{
    ReleaseManifest manifest;
    try
    {
        manifest = readReleaseManifest(releaseRoot, version);
    }
    catch (const std::exception& exc)
    {
        std::cerr << "error: " << exc.what() << "\n";
        return false;
    }

    const std::filesystem::path manifestDirectory = manifest.manifestPath.parent_path();
    const std::filesystem::path destinationRoot = sdkVersionRoot(home, version);
    for (const ReleaseManifestFile& file : manifest.files)
    {
        const std::filesystem::path source = manifestDirectory / file.source;
        if (!std::filesystem::is_regular_file(source))
        {
            std::cerr << "error: release file is missing: " << pathString(source) << "\n";
            return false;
        }
        std::string actual;
        try
        {
            actual = sha256File(source);
        }
        catch (const std::exception& exc)
        {
            std::cerr << "error: " << exc.what() << "\n";
            return false;
        }
        if (actual != file.checksum)
        {
            std::cerr
                << "error: checksum mismatch for " << pathString(source)
                << ": expected " << file.checksum
                << ", got " << actual << "\n";
            return false;
        }
    }

    for (const ReleaseManifestFile& file : manifest.files)
    {
        const std::filesystem::path source = manifestDirectory / file.source;
        const std::filesystem::path destination = destinationRoot / file.destination;
        const bool copied = isSdkExecutableDestination(file.destination)
            ? copyExecutableFile(source, destination)
            : copyRegularFile(source, destination);
        if (!copied)
        {
            return false;
        }
    }
    return true;
}

bool ensureSdkStoreDirectories(const std::filesystem::path& home)
{
    std::error_code error;
    std::filesystem::create_directories(home / SDK_BIN_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(home / SDK_BIN_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(home / SDK_ROOT_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(home / SDK_ROOT_DIR) << ": " << error.message() << "\n";
        return false;
    }
    return true;
}

bool ensureLauncher(const std::filesystem::path& home, const std::filesystem::path& executable)
{
    if (!ensureSdkStoreDirectories(home))
    {
        return false;
    }
    const std::filesystem::path destination = launcherPath(home);
    if (std::filesystem::is_regular_file(destination))
    {
        return true;
    }
    if (executable.empty() || !std::filesystem::is_regular_file(executable))
    {
        std::cerr << "error: cannot install launcher because current executable path is unavailable\n";
        return false;
    }
    return copyExecutableFile(executable, destination);
}

bool ensureSdkVersionDirectories(const std::filesystem::path& home, const std::string& version)
{
    std::error_code error;
    const std::filesystem::path versionRoot = sdkVersionRoot(home, version);
    std::filesystem::create_directories(versionRoot / SDK_BIN_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(versionRoot / SDK_BIN_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(versionRoot / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(versionRoot / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(versionRoot / SDK_EXAMPLES_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(versionRoot / SDK_EXAMPLES_DIR) << ": " << error.message() << "\n";
        return false;
    }
    return true;
}

std::vector<std::string> installedSdkVersions(const std::filesystem::path& home)
{
    std::vector<std::string> versions;
    const std::filesystem::path sdkRoot = home / SDK_ROOT_DIR;
    if (!std::filesystem::is_directory(sdkRoot))
    {
        return versions;
    }
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(sdkRoot))
    {
        const std::string version = entry.path().filename().string();
        if (entry.is_directory() && isVersionNameSafe(version) && std::filesystem::is_regular_file(sdkVersionBinary(home, version)))
        {
            versions.push_back(version);
        }
    }
    std::sort(versions.begin(), versions.end(), versionLess);
    return versions;
}

std::vector<std::string> installedSdkTargets(const std::filesystem::path& home, const std::string& version)
{
    std::vector<std::string> targets;
    const std::filesystem::path targetsRoot = sdkVersionRoot(home, version) / SDK_TARGETS_DIR;
    if (!std::filesystem::is_directory(targetsRoot))
    {
        return targets;
    }
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(targetsRoot))
    {
        const std::string target = entry.path().filename().string();
        if (entry.is_directory() && isSdkTargetNameSafe(target))
        {
            targets.push_back(target);
        }
    }
    std::sort(targets.begin(), targets.end());
    return targets;
}

std::string sdkTargetsJson(const std::filesystem::path& home, const std::vector<std::string>& versions)
{
    std::string json = "{";
    for (std::size_t versionIndex = 0; versionIndex < versions.size(); ++versionIndex)
    {
        if (versionIndex != 0)
        {
            json += ",";
        }
        const std::string& version = versions[versionIndex];
        json += "\"" + jsonEscape(version) + "\":[";
        const std::vector<std::string> targets = installedSdkTargets(home, version);
        for (std::size_t targetIndex = 0; targetIndex < targets.size(); ++targetIndex)
        {
            if (targetIndex != 0)
            {
                json += ",";
            }
            json += "\"" + jsonEscape(targets[targetIndex]) + "\"";
        }
        json += "]";
    }
    json += "}";
    return json;
}

std::string latestInstalledSdkVersion(const std::filesystem::path& home)
{
    const std::vector<std::string> versions = installedSdkVersions(home);
    if (versions.empty())
    {
        return {};
    }
    return versions.back();
}

std::string selectedSdkVersion(const std::filesystem::path& home)
{
    const std::filesystem::path currentPath = home / SDK_CURRENT_FILE;
    if (std::filesystem::is_regular_file(currentPath))
    {
        std::ifstream input(currentPath);
        std::string version;
        std::getline(input, version);
        version = trim(version);
        if (isVersionNameSafe(version) && std::filesystem::is_regular_file(sdkVersionBinary(home, version)))
        {
            return version;
        }
    }

    const std::vector<std::string> versions = installedSdkVersions(home);
    if (versions.empty())
    {
        return {};
    }
    return versions.back();
}

std::string sdkMinorSeriesKey(const std::string& version)
{
    const std::vector<std::string> tokens = versionTokens(version);
    if (tokens.size() < 2)
    {
        return version;
    }
    return tokens[0] + "." + tokens[1];
}

std::vector<std::string> prunableSdkVersions(const std::filesystem::path& home, const std::string& currentVersion)
{
    const std::vector<std::string> versions = installedSdkVersions(home);
    std::vector<std::string> seriesKeys;
    for (const std::string& version : versions)
    {
        const std::string key = sdkMinorSeriesKey(version);
        if (std::find(seriesKeys.begin(), seriesKeys.end(), key) == seriesKeys.end())
        {
            seriesKeys.push_back(key);
        }
    }

    std::vector<std::string> prunable;
    for (const std::string& key : seriesKeys)
    {
        std::vector<std::string> seriesVersions;
        for (const std::string& version : versions)
        {
            if (sdkMinorSeriesKey(version) == key)
            {
                seriesVersions.push_back(version);
            }
        }

        std::size_t retained = 0;
        for (std::size_t index = seriesVersions.size(); index > 0; --index)
        {
            const std::string& version = seriesVersions[index - 1];
            if (retained < SDK_PRUNE_PATCH_RETENTION)
            {
                ++retained;
                continue;
            }
            if (version == currentVersion)
            {
                continue;
            }
            prunable.push_back(version);
        }
    }
    std::sort(prunable.begin(), prunable.end(), versionLess);
    return prunable;
}

bool safeSdkVersionRootForRemoval(const std::filesystem::path& home, const std::string& version)
{
    if (!isVersionNameSafe(version))
    {
        return false;
    }
    const std::filesystem::path sdkRoot = (home / SDK_ROOT_DIR).lexically_normal();
    const std::filesystem::path versionRoot = sdkVersionRoot(home, version).lexically_normal();
    return versionRoot.parent_path() == sdkRoot && versionRoot.filename().string() == version;
}

bool safeSdkHomeForRemoval(const std::filesystem::path& home)
{
    if (home.empty())
    {
        return false;
    }
    const std::filesystem::path normalized = normalizedAbsolutePath(home);
    if (normalized.empty() || normalized == normalized.root_path())
    {
        return false;
    }
    return std::filesystem::is_directory(normalized / SDK_ROOT_DIR) &&
           std::filesystem::is_directory(normalized / SDK_BIN_DIR) &&
           std::filesystem::is_regular_file(launcherPath(normalized));
}

bool versionGreaterOrEqual(const std::string& version, const std::string& required)
{
    return !versionLess(version, required);
}

bool sameMinorSeries(const std::string& left, const std::string& right)
{
    const std::vector<std::string> leftTokens = versionTokens(left);
    const std::vector<std::string> rightTokens = versionTokens(right);
    if (leftTokens.size() < 2 || rightTokens.size() < 2)
    {
        return left == right;
    }
    return leftTokens[0] == rightTokens[0] && leftTokens[1] == rightTokens[1];
}

std::string latestSdkUpdateCandidate(
    const std::filesystem::path& home,
    const std::filesystem::path& releaseRoot,
    const std::string& currentVersion
)
{
    std::vector<std::string> versions = installedSdkVersions(home);
    if (!releaseRoot.empty())
    {
        for (const std::string& version : localReleaseVersions(releaseRoot))
        {
            if (std::find(versions.begin(), versions.end(), version) == versions.end())
            {
                versions.push_back(version);
            }
        }
        std::sort(versions.begin(), versions.end(), versionLess);
    }

    std::string candidate = currentVersion;
    for (const std::string& version : versions)
    {
        if (sameMinorSeries(version, currentVersion) && versionGreaterOrEqual(version, candidate))
        {
            candidate = version;
        }
    }
    return candidate;
}

std::string highestInstalledSdkAtLeast(const std::filesystem::path& home, const std::string& required)
{
    std::string result;
    for (const std::string& version : installedSdkVersions(home))
    {
        if (versionGreaterOrEqual(version, required))
        {
            result = version;
        }
    }
    return result;
}

bool writeSelectedSdkVersion(const std::filesystem::path& home, const std::string& version)
{
    if (!ensureSdkStoreDirectories(home))
    {
        return false;
    }
    std::ofstream output(home / SDK_CURRENT_FILE, std::ios::binary);
    if (!output)
    {
        std::cerr << "error: cannot write " << pathString(home / SDK_CURRENT_FILE) << "\n";
        return false;
    }
    output << version << "\n";
    return true;
}

bool clearSelectedSdkVersion(const std::filesystem::path& home)
{
    std::error_code error;
    std::filesystem::remove(home / SDK_CURRENT_FILE, error);
    if (error)
    {
        std::cerr << "error: cannot remove " << pathString(home / SDK_CURRENT_FILE) << ": " << error.message() << "\n";
        return false;
    }
    return true;
}

bool commandOptionConsumesNext(const std::string& command, const std::string& option)
{
    if (command == "generate")
    {
        return option == "--target" || option == "--output-dir";
    }
    return false;
}

bool supportedSdkSourceExtension(const std::filesystem::path& path)
{
    const std::string extension = lower(path.extension().string());
    return extension == ".uimd" || extension == ".md";
}

std::vector<std::filesystem::path> directUimdSources(const std::filesystem::path& directory)
{
    std::vector<std::filesystem::path> sources;
    if (!std::filesystem::is_directory(directory))
    {
        return sources;
    }
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && supportedSdkSourceExtension(entry.path()))
        {
            sources.push_back(entry.path());
        }
    }
    std::sort(sources.begin(), sources.end());
    return sources;
}

std::filesystem::path commandSourceArgument(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        return {};
    }
    const std::string& command = args.front();
    if (command != "generate" && command != "run" && command != "inspect")
    {
        return {};
    }

    bool skipNext = false;
    for (std::size_t index = 1; index < args.size(); ++index)
    {
        const std::string& arg = args[index];
        if (arg == "--")
        {
            break;
        }
        if (skipNext)
        {
            skipNext = false;
            continue;
        }
        if (commandOptionConsumesNext(command, arg))
        {
            skipNext = true;
            continue;
        }
        if (arg.rfind("--", 0) == 0)
        {
            continue;
        }
        return std::filesystem::path{arg};
    }
    return {};
}

std::string sdkVersionMetadata(const std::filesystem::path& source)
{
    const uimd::tool::NativeDocument document = uimd::tool::parseDocumentFile(pathString(source));
    return trim(yamlStringValue(document.metadata, SDK_VERSION_METADATA_KEY, ""));
}

std::filesystem::path sdkSourceForCommand(const std::vector<std::string>& args)
{
    const std::filesystem::path sourceArgument = commandSourceArgument(args);
    if (sourceArgument.empty())
    {
        return {};
    }

    std::error_code error;
    const std::filesystem::path sourcePath = std::filesystem::weakly_canonical(sourceArgument, error);
    const std::filesystem::path normalized = error ? std::filesystem::absolute(sourceArgument).lexically_normal() : sourcePath;
    if (std::filesystem::is_regular_file(normalized) && supportedSdkSourceExtension(normalized))
    {
        return normalized;
    }

    const std::vector<std::filesystem::path> directSources = directUimdSources(normalized);
    for (const std::filesystem::path& candidate : directSources)
    {
        try
        {
            if (!sdkVersionMetadata(candidate).empty())
            {
                return candidate;
            }
        }
        catch (const std::exception&)
        {
        }
    }
    if (directSources.size() == 1)
    {
        return directSources.front();
    }
    return {};
}

bool resolveSdkForLaunch(
    const std::filesystem::path& home,
    const std::vector<std::string>& args,
    const GlobalOptions& options,
    SdkResolution& resolution,
    std::string& error
)
{
    resolution.sourcePath = sdkSourceForCommand(args);
    resolution.sourceFound = !resolution.sourcePath.empty();

    if (resolution.sourceFound)
    {
        try
        {
            resolution.requiredVersion = sdkVersionMetadata(resolution.sourcePath);
        }
        catch (const std::exception& exc)
        {
            error = "error: cannot read SDK version metadata from " + pathString(resolution.sourcePath) + ": " + exc.what();
            return false;
        }
        resolution.metadataFound = !resolution.requiredVersion.empty();

        if (!resolution.metadataFound)
        {
            if (options.requireSdkVersion)
            {
                error = "error: root .uimd has no UIMD SDK version; add sdk-version to Metadata for reproducible builds";
                return false;
            }
            resolution.version = latestInstalledSdkVersion(home);
            resolution.usedLegacyFallback = !resolution.version.empty();
            if (resolution.version.empty())
            {
                error = "error: root .uimd has no UIMD SDK version and no SDK versions are installed; add sdk-version to Metadata or run `uimd sdk install " + runtimeVersion() + "`";
                return false;
            }
            return true;
        }

        if (!isVersionNameSafe(resolution.requiredVersion))
        {
            error = "error: root .uimd sdk-version is not a safe version name: " + resolution.requiredVersion;
            return false;
        }

        resolution.version = highestInstalledSdkAtLeast(home, resolution.requiredVersion);
        if (resolution.version.empty())
        {
            if (options.offline)
            {
                error = "error: required UIMD SDK " + resolution.requiredVersion + " is not installed and offline mode is enabled; run `uimd sdk install " + resolution.requiredVersion + "` before retrying";
            }
            else
            {
                error = "error: required UIMD SDK " + resolution.requiredVersion + " is not installed; release downloads are not implemented in this build, run `uimd sdk install " + resolution.requiredVersion + "` first";
            }
            return false;
        }
        resolution.crossedMinorSeries = !sameMinorSeries(resolution.requiredVersion, resolution.version);
        return true;
    }

    resolution.version = selectedSdkVersion(home);
    if (resolution.version.empty())
    {
        error = "error: no SDK version is installed; run `uimd sdk install " + runtimeVersion() + "` first";
        return false;
    }
    return true;
}

void printSdkResolutionWarnings(const SdkResolution& resolution)
{
    if (resolution.usedLegacyFallback)
    {
        std::cerr
            << "warning: root .uimd has no UIMD SDK version; using installed SDK "
            << resolution.version
            << "; add sdk-version to Metadata for reproducible builds\n";
    }
    if (resolution.crossedMinorSeries)
    {
        std::cerr
            << "warning: root .uimd requires UIMD SDK "
            << resolution.requiredVersion
            << " but selected installed SDK "
            << resolution.version
            << " from a different minor series; update sdk-version to silence this warning\n";
    }
}

void setEnvironment(const std::string& name, const std::string& value)
{
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}

char pathSeparator()
{
#ifdef _WIN32
    return ';';
#else
    return ':';
#endif
}

std::string projectName(std::string raw)
{
    std::string name;
    bool previousSeparator = false;
    for (char ch : raw)
    {
        const unsigned char uch = static_cast<unsigned char>(ch);
        if (std::isalnum(uch) || ch == '_' || ch == '-')
        {
            name += static_cast<char>(std::tolower(uch));
            previousSeparator = false;
        }
        else if (!previousSeparator)
        {
            name += '_';
            previousSeparator = true;
        }
    }

    while (!name.empty() && (name.front() == '_' || name.front() == '-'))
    {
        name.erase(name.begin());
    }
    while (!name.empty() && (name.back() == '_' || name.back() == '-'))
    {
        name.pop_back();
    }

    if (name.empty())
    {
        return {};
    }

    const unsigned char first = static_cast<unsigned char>(name.front());
    if (!std::isalpha(first) && name.front() != '_')
    {
        name = "app_" + name;
    }
    return name;
}

std::string className(const std::string& name)
{
    std::string result;
    bool capitalizeNext = true;
    for (char ch : name)
    {
        const unsigned char uch = static_cast<unsigned char>(ch);
        if (ch == '_' || ch == '-' || std::isspace(uch))
        {
            capitalizeNext = true;
            continue;
        }
        if (capitalizeNext)
        {
            result += static_cast<char>(std::toupper(uch));
            capitalizeNext = false;
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

void replaceAll(std::string& text, std::string_view needle, std::string_view replacement)
{
    std::string::size_type position = 0;
    while ((position = text.find(needle, position)) != std::string::npos)
    {
        text.replace(position, needle.size(), replacement);
        position += replacement.size();
    }
}

using TemplateValue = std::pair<std::string, std::string>;
using GeneratedFile = std::pair<std::filesystem::path, std::string>;

std::string applyTemplate(std::string text, const std::vector<TemplateValue>& values)
{
    for (const auto& [key, value] : values)
    {
        replaceAll(text, key, value);
    }
    return text;
}

bool writeText(const std::filesystem::path& path, const std::string& content)
{
    std::ofstream output(path, std::ios::binary);
    if (!output)
    {
        std::cerr << "error: cannot write " << pathString(path) << "\n";
        return false;
    }
    output << content;
    return true;
}

std::string uimdSourceTemplate()
{
    return R"UIMD(# @CLASS@

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: draft
description: "Hello world UIMD application."
```

## Members

```yaml
headline:
  type: label
  text: "Hello from UIMD"
  description: "Application title."

name:
  type: textinput
  value: ""
  placeholder: "Your name"
  description: "Name to greet."

hello_button:
  type: button
  title: "Say Hello"
  description: "Create the greeting."

quit_button:
  type: button
  title: "Quit"
  description: "Close the application."
```

## Style

```yaml
include: dark
@title:
  background: "#1f2937"
@headline:
  color: "#ffffff"
@quit_button:
  background: "#7f1d1d"
  focus-background: "#dc2626"
```

## User Interface

```ui
+-title-----------------------------------+
| headline............................... |
+-----------------------------------------+
|                                         |
|  "Name"      name....................   |
|                                         |
|  hello_button.......  quit_button.....  |
|                                         |
+-----------------------------------------+
```
)UIMD";
}

std::string pythonAppTemplate()
{
    return R"UIMD("""Application logic for @PROJECT@.uimd."""

import sys

from uimd.runtime import UIApplication
from @PROJECT@_ui import @CLASS@UI


class @CLASS@(@CLASS@UI):
    def on_hello_button_click(self):
        name = self.name.value.strip() or "world"
        self.headline.text = f"Hello, {name}!"

    def on_quit_button_click(self):
        if self._app is not None:
            self._app._running = False


def main():
    app = UIApplication()
    app.open(@CLASS@())
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
)UIMD";
}

std::string cppAppTemplate()
{
    return R"UIMD(// Application logic for @PROJECT@.
#include "@PROJECT@_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <string>

class @CLASS@ : public @CLASS@UI
{
protected:
    void onHelloButtonClick() override
    {
        std::string value = name->value();
        if (value.empty())
        {
            value = "world";
        }
        headline->setText("Hello, " + value + "!");
    }

    void onQuitButtonClick() override
    {
        closeRequested_ = true;
    }

    bool shouldClose() const override
    {
        return closeRequested_;
    }

private:
    bool closeRequested_ = false;
};

int main(int argc, char** argv)
{
    @CLASS@ app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
)UIMD";
}

std::string cppCMakeTemplate()
{
    return R"UIMD(cmake_minimum_required(VERSION 3.20)

project(@PROJECT@ LANGUAGES CXX)

find_package(uimd CONFIG QUIET)

if(NOT TARGET uimd::runtime)
    set(_uimd_local_cpp "${CMAKE_CURRENT_LIST_DIR}/../uimd/cpp")
    if(EXISTS "${_uimd_local_cpp}/CMakeLists.txt")
        set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(UIMD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(UIMD_INSTALL OFF CACHE BOOL "" FORCE)
        add_subdirectory("${_uimd_local_cpp}" "${CMAKE_BINARY_DIR}/_deps/uimd-build")
    endif()
endif()

if(NOT TARGET uimd::runtime)
    include(FetchContent)

    set(UIMD_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(UIMD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(UIMD_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        uimd
        GIT_REPOSITORY https://github.com/uimd-lang/uimd.git
        GIT_TAG v@VERSION@
    )
    FetchContent_MakeAvailable(uimd)
endif()

if(NOT TARGET uimd::runtime)
    message(FATAL_ERROR "UIMD C++ runtime was not found. Install UIMD, set CMAKE_PREFIX_PATH, keep a sibling ../uimd checkout, or use a released Git tag.")
endif()

add_executable(@PROJECT@
    @PROJECT@.cpp
    @PROJECT@_ui.cpp
)

target_compile_features(@PROJECT@ PRIVATE cxx_std_20)
target_link_libraries(@PROJECT@ PRIVATE uimd::runtime)
)UIMD";
}

std::string pythonExecutable()
{
    const std::string pythonOverride = envValue("UIMD_PYTHON");
    return pythonOverride.empty() ? "python3" : pythonOverride;
}

void configurePythonRuntimePath()
{
    const std::filesystem::path root = sourceRoot();
    if (root.empty())
    {
        return;
    }

    std::ostringstream pythonPath;
    pythonPath << pathString(root / "src") << pathSeparator()
               << pathString(root / "python") << pathSeparator()
               << pathString(root);
    const std::string existingPythonPath = envValue("PYTHONPATH");
    if (!existingPythonPath.empty())
    {
        pythonPath << pathSeparator() << existingPythonPath;
    }
    setEnvironment("PYTHONPATH", pythonPath.str());
}

int runProcess(std::vector<std::string> command, const std::filesystem::path& workingDirectory = {})
{
    if (command.empty())
    {
        std::cerr << "error: process command is empty\n";
        return EXIT_ERROR;
    }

    std::vector<char*> argv;
    argv.reserve(command.size() + 1);
    for (std::string& arg : command)
    {
        argv.push_back(arg.data());
    }
    argv.push_back(nullptr);

#ifdef _WIN32
    const std::filesystem::path previousDirectory = std::filesystem::current_path();
    if (!workingDirectory.empty())
    {
        std::filesystem::current_path(workingDirectory);
    }
    const intptr_t status = _spawnvp(_P_WAIT, command.front().c_str(), argv.data());
    if (!workingDirectory.empty())
    {
        std::filesystem::current_path(previousDirectory);
    }
    if (status == -1)
    {
        std::cerr << "error: failed to execute command: " << command.front() << "\n";
        return EXIT_ERROR;
    }
    return static_cast<int>(status);
#else
    const pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "error: failed to fork process\n";
        return EXIT_ERROR;
    }
    if (pid == 0)
    {
        if (!workingDirectory.empty() && chdir(workingDirectory.c_str()) != 0)
        {
            _exit(127);
        }
        execvp(command.front().c_str(), argv.data());
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0)
    {
        std::cerr << "error: failed to wait for process\n";
        return EXIT_ERROR;
    }
    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status))
    {
        return 128 + WTERMSIG(status);
    }
    return EXIT_ERROR;
#endif
}

int execReplacing(std::vector<std::string> command)
{
    if (command.empty())
    {
        std::cerr << "error: process command is empty\n";
        return EXIT_ERROR;
    }

    std::vector<char*> argv;
    argv.reserve(command.size() + 1);
    for (std::string& arg : command)
    {
        argv.push_back(arg.data());
    }
    argv.push_back(nullptr);

#ifdef _WIN32
    _execvp(command.front().c_str(), argv.data());
#else
    execvp(command.front().c_str(), argv.data());
#endif
    std::cerr << "error: failed to execute command: " << command.front() << "\n";
    return EXIT_ERROR;
}

bool commandHandledByLauncher(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        return true;
    }
    const std::string& command = args.front();
    return command == "sdk" || command == "self" || command == "doctor" || command == "--help" || command == "-h" || command == "--version";
}

GlobalOptions extractGlobalOptions(std::vector<std::string>& args)
{
    GlobalOptions options;
    options.requireSdkVersion = envEnabled(REQUIRE_SDK_VERSION_ENV.c_str());
    options.offline = envEnabled(NO_AUTO_INSTALL_ENV.c_str());

    std::vector<std::string> filtered;
    filtered.reserve(args.size());
    bool passthrough = false;
    for (const std::string& arg : args)
    {
        if (passthrough)
        {
            filtered.push_back(arg);
            continue;
        }
        if (arg == "--")
        {
            passthrough = true;
            filtered.push_back(arg);
            continue;
        }
        if (arg == "--require-sdk-version")
        {
            options.requireSdkVersion = true;
            continue;
        }
        if (arg == "--offline")
        {
            options.offline = true;
            continue;
        }
        filtered.push_back(arg);
    }
    args = std::move(filtered);
    return options;
}

int delegateToSelectedSdk(
    const std::filesystem::path& home,
    const std::filesystem::path& launcher,
    const std::vector<std::string>& args,
    const GlobalOptions& options
)
{
    const std::filesystem::path overrideBinary = sdkOverrideBinary();
    if (!overrideBinary.empty())
    {
        if (!std::filesystem::is_regular_file(overrideBinary))
        {
            std::cerr << "error: " << SDK_PATH_ENV << " does not point to an executable SDK binary: " << pathString(overrideBinary) << "\n";
            return EXIT_ERROR;
        }
        if (samePath(overrideBinary, launcher))
        {
            std::cerr << "error: " << SDK_PATH_ENV << " points to the launcher and would recurse: " << pathString(overrideBinary) << "\n";
            return EXIT_ERROR;
        }
        std::vector<std::string> command{pathString(overrideBinary)};
        command.insert(command.end(), args.begin(), args.end());
        return execReplacing(std::move(command));
    }

    SdkResolution resolution;
    std::string error;
    if (!resolveSdkForLaunch(home, args, options, resolution, error))
    {
        std::cerr << error << "\n";
        return EXIT_ERROR;
    }
    printSdkResolutionWarnings(resolution);

    const std::filesystem::path binary = sdkVersionBinary(home, resolution.version);
    if (!std::filesystem::is_regular_file(binary))
    {
        std::cerr << "error: selected SDK binary is missing: " << pathString(binary) << "\n";
        return EXIT_ERROR;
    }
    if (samePath(binary, launcher))
    {
        std::cerr << "error: selected SDK binary points to the launcher and would recurse: " << pathString(binary) << "\n";
        return EXIT_ERROR;
    }

    std::vector<std::string> command{pathString(binary)};
    command.insert(command.end(), args.begin(), args.end());
    return execReplacing(std::move(command));
}

int printHelp(const char* program)
{
    std::cout
        << "usage: " << program << " [--version] <command> [args]\n"
        << "\n"
        << "User Interface Markdown tools.\n"
        << "\n"
        << "commands:\n"
        << "  new       Create a new UIMD app\n"
        << "  generate  Generate UI source files\n"
        << "  inspect   Inspect a UIMD source model\n"
        << "  run       Generate and run a Python UIMD app\n"
        << "  mcp-test  Run MCP scripts against an app\n"
        << "  sdk       Inspect and manage UIMD SDK installs\n"
        << "  self      Manage this UIMD SDK launcher install\n"
        << "  doctor    Inspect the UIMD installation\n";
    return EXIT_USAGE;
}

std::string jsonEscape(const std::string& text)
{
    std::string escaped;
    for (char ch : text)
    {
        if (ch == '\\')
        {
            escaped += "\\\\";
        }
        else if (ch == '"')
        {
            escaped += "\\\"";
        }
        else if (ch == '\n')
        {
            escaped += "\\n";
        }
        else
        {
            escaped += ch;
        }
    }
    return escaped;
}

int runDoctor(const std::vector<std::string>& args)
{
    bool json = false;
    for (const std::string& arg : args)
    {
        if (arg == "--json")
        {
            json = true;
        }
        else
        {
            std::cerr << "error: unknown doctor option: " << arg << "\n";
            return EXIT_USAGE;
        }
    }

    const std::filesystem::path root = sourceRoot();
    const bool sourceCheckoutAvailable = !root.empty();
    const std::filesystem::path sdkHomePath = sdkHome();
    const std::vector<std::string> sdkVersions = installedSdkVersions(sdkHomePath);
    const std::string currentVersion = selectedSdkVersion(sdkHomePath);
    const std::filesystem::path sdkLauncherPath = launcherPath(sdkHomePath);
    const std::filesystem::path currentBinaryPath = currentVersion.empty() ? std::filesystem::path{} : sdkVersionBinary(sdkHomePath, currentVersion);
    const bool launcherExists = std::filesystem::is_regular_file(sdkLauncherPath);
    const bool currentBinaryExists = !currentBinaryPath.empty() && std::filesystem::is_regular_file(currentBinaryPath);
    const bool sdkUsable = launcherExists && currentBinaryExists;
    const bool ok = sourceCheckoutAvailable || sdkUsable;
    const std::vector<std::string> currentTargets = currentVersion.empty() ? std::vector<std::string>{} : installedSdkTargets(sdkHomePath, currentVersion);

    if (json)
    {
        std::string versionJson = "[";
        for (std::size_t index = 0; index < sdkVersions.size(); ++index)
        {
            if (index != 0)
            {
                versionJson += ",";
            }
            versionJson += "\"" + jsonEscape(sdkVersions[index]) + "\"";
        }
        versionJson += "]";
        std::string targetJson = "[";
        for (std::size_t index = 0; index < currentTargets.size(); ++index)
        {
            if (index != 0)
            {
                targetJson += ",";
            }
            targetJson += "\"" + jsonEscape(currentTargets[index]) + "\"";
        }
        targetJson += "]";
        std::cout
            << "{\"native\":{\"binary\":true,\"source_root\":\""
            << jsonEscape(sourceCheckoutAvailable ? pathString(root) : "")
            << "\"},\"source_checkout\":{\"available\":"
            << (sourceCheckoutAvailable ? "true" : "false")
            << "},\"sdk\":{\"home\":\""
            << jsonEscape(pathString(sdkHomePath))
            << "\",\"launcher\":\""
            << jsonEscape(pathString(sdkLauncherPath))
            << "\",\"launcher_exists\":"
            << (launcherExists ? "true" : "false")
            << ",\"current_version\":\""
            << jsonEscape(currentVersion)
            << "\",\"current_binary\":\""
            << jsonEscape(currentBinaryPath.empty() ? std::string{} : pathString(currentBinaryPath))
            << "\",\"current_binary_exists\":"
            << (currentBinaryExists ? "true" : "false")
            << ",\"current_targets\":"
            << targetJson
            << ",\"versions\":"
            << versionJson
            << ",\"status\":\""
            << (sdkUsable ? "ok" : "incomplete")
            << "\"},\"status\":\""
            << (ok ? "ok" : "problems found")
            << "\",\"version\":\""
            << jsonEscape(runtimeVersion())
            << "\"}\n";
        return ok ? EXIT_OK : EXIT_ERROR;
    }

    std::cout << "UIMD Doctor\n\n";
    std::cout << "Native:\n";
    std::cout << "  binary: yes\n";
    std::cout << "  version: " << runtimeVersion() << "\n";
    std::cout << "  source root: " << (sourceCheckoutAvailable ? pathString(root) : "not found") << "\n";
    std::cout << "  source checkout: " << (sourceCheckoutAvailable ? "yes" : "no") << "\n\n";
    std::cout << "SDK:\n";
    std::cout << "  home: " << pathString(sdkHomePath) << "\n";
    std::cout << "  launcher: " << pathString(sdkLauncherPath) << (launcherExists ? "" : " (missing)") << "\n";
    std::cout << "  current version: " << (currentVersion.empty() ? "none" : currentVersion) << "\n";
    if (!currentBinaryPath.empty())
    {
        std::cout << "  current binary: " << pathString(currentBinaryPath) << (currentBinaryExists ? "" : " (missing)") << "\n";
        std::cout << "  current targets: ";
        if (currentTargets.empty())
        {
            std::cout << "none\n";
        }
        else
        {
            for (std::size_t index = 0; index < currentTargets.size(); ++index)
            {
                if (index != 0)
                {
                    std::cout << ", ";
                }
                std::cout << currentTargets[index];
            }
            std::cout << "\n";
        }
    }
    if (sdkVersions.empty())
    {
        std::cout << "  installed versions: none\n\n";
    }
    else
    {
        std::cout << "  installed versions: ";
        for (std::size_t index = 0; index < sdkVersions.size(); ++index)
        {
            if (index != 0)
            {
                std::cout << ", ";
            }
            std::cout << sdkVersions[index];
        }
        std::cout << "\n\n";
    }
    std::cout << "Status: " << (ok ? "ok" : "problems found") << "\n";
    return ok ? EXIT_OK : EXIT_ERROR;
}

int runSdk(const std::vector<std::string>& args, const std::filesystem::path& executablePath)
{
    if (args.empty())
    {
        std::cerr << "error: sdk command is required\n";
        return EXIT_USAGE;
    }

    const std::string command = args.front();
    const std::filesystem::path home = sdkHome();

    if (command == "install")
    {
        std::string version;
        std::filesystem::path source = executablePath;
        std::filesystem::path releaseRoot;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            const std::string& arg = args[index];
            if (arg == "--from")
            {
                if (index + 1 >= args.size())
                {
                    std::cerr << "error: --from requires a path\n";
                    return EXIT_USAGE;
                }
                source = args[++index];
            }
            else if (arg == "--release-root")
            {
                if (index + 1 >= args.size())
                {
                    std::cerr << "error: --release-root requires a path\n";
                    return EXIT_USAGE;
                }
                releaseRoot = args[++index];
            }
            else if (version.empty())
            {
                version = arg;
            }
            else
            {
                std::cerr << "error: unexpected sdk install argument: " << arg << "\n";
                return EXIT_USAGE;
            }
        }

        if (!isVersionNameSafe(version))
        {
            std::cerr << "error: SDK version must be a safe version name\n";
            return EXIT_USAGE;
        }
        if (releaseRoot.empty())
        {
            const std::string releaseRootEnv = envValue(RELEASE_ROOT_ENV.c_str());
            if (!releaseRootEnv.empty())
            {
                releaseRoot = releaseRootEnv;
            }
        }
        if (releaseRoot.empty() && (source.empty() || !std::filesystem::is_regular_file(source)))
        {
            std::cerr << "error: SDK install source is not a file: " << pathString(source) << "\n";
            return EXIT_USAGE;
        }
        if (!ensureLauncher(home, executablePath))
        {
            return EXIT_ERROR;
        }
        const bool hadSelectedVersion = !selectedSdkVersion(home).empty();
        if (!ensureSdkVersionDirectories(home, version))
        {
            return EXIT_ERROR;
        }
        const bool installed = !releaseRoot.empty()
            ? installReleaseManifest(home, releaseRoot, version)
            : copyExecutableFile(source, sdkVersionBinary(home, version));
        if (!installed)
        {
            return EXIT_ERROR;
        }
        if (!std::filesystem::is_regular_file(sdkVersionBinary(home, version)))
        {
            std::cerr << "error: release manifest did not install " << pathString(SDK_BIN_DIR / uimdExecutableName()) << "\n";
            return EXIT_ERROR;
        }
        if (!hadSelectedVersion && !writeSelectedSdkVersion(home, version))
        {
            return EXIT_ERROR;
        }
        std::cout << "Installed UIMD SDK " << version << " at " << pathString(sdkVersionRoot(home, version)) << "\n";
        return EXIT_OK;
    }

    if (command == "install-target")
    {
        std::string target;
        std::string version;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            const std::string& arg = args[index];
            if (arg == "--version")
            {
                if (index + 1 >= args.size())
                {
                    std::cerr << "error: --version requires a value\n";
                    return EXIT_USAGE;
                }
                version = args[++index];
            }
            else if (target.empty())
            {
                target = lower(arg);
            }
            else
            {
                std::cerr << "error: unexpected sdk install-target argument: " << arg << "\n";
                return EXIT_USAGE;
            }
        }
        if (!isSdkTargetNameSafe(target) || !isSupportedSdkTarget(target))
        {
            std::cerr << "error: supported SDK targets are: python, cpp\n";
            return EXIT_USAGE;
        }
        if (version.empty())
        {
            version = selectedSdkVersion(home);
        }
        if (!isVersionNameSafe(version))
        {
            std::cerr << "error: SDK version must be a safe version name\n";
            return EXIT_USAGE;
        }
        if (!std::filesystem::is_regular_file(sdkVersionBinary(home, version)))
        {
            std::cerr << "error: SDK version is not installed: " << version << "\n";
            return EXIT_ERROR;
        }
        std::error_code error;
        const std::filesystem::path targetRoot = sdkTargetRoot(home, version, target);
        std::filesystem::create_directories(targetRoot, error);
        if (error)
        {
            std::cerr << "error: cannot create " << pathString(targetRoot) << ": " << error.message() << "\n";
            return EXIT_ERROR;
        }
        std::cout << "Installed UIMD SDK target " << target << " for " << version << " at " << pathString(targetRoot) << "\n";
        return EXIT_OK;
    }

    if (command == "update")
    {
        bool json = false;
        std::filesystem::path releaseRoot;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            const std::string& arg = args[index];
            if (arg == "--json")
            {
                json = true;
            }
            else if (arg == "--release-root")
            {
                if (index + 1 >= args.size())
                {
                    std::cerr << "error: --release-root requires a path\n";
                    return EXIT_USAGE;
                }
                releaseRoot = args[++index];
            }
            else
            {
                std::cerr << "error: unknown sdk update option: " << arg << "\n";
                return EXIT_USAGE;
            }
        }
        if (releaseRoot.empty())
        {
            const std::string releaseRootEnv = envValue(RELEASE_ROOT_ENV.c_str());
            if (!releaseRootEnv.empty())
            {
                releaseRoot = releaseRootEnv;
            }
        }
        if (!releaseRoot.empty() && !std::filesystem::is_directory(releaseRoot))
        {
            std::cerr << "error: release root is not a directory: " << pathString(releaseRoot) << "\n";
            return EXIT_USAGE;
        }

        const std::string currentVersion = selectedSdkVersion(home);
        if (currentVersion.empty())
        {
            std::cerr << "error: no SDK version is selected; run `uimd sdk install " << runtimeVersion() << "` first\n";
            return EXIT_ERROR;
        }

        const std::string targetVersion = latestSdkUpdateCandidate(home, releaseRoot, currentVersion);
        if (targetVersion.empty() || targetVersion == currentVersion)
        {
            if (json)
            {
                std::cout
                    << "{\"current\":\"" << jsonEscape(currentVersion)
                    << "\",\"selected\":\"" << jsonEscape(currentVersion)
                    << "\",\"updated\":false,\"installed\":false,\"status\":\"ok\"}\n";
            }
            else
            {
                std::cout << "UIMD SDK " << currentVersion << " is already the latest available patch.\n";
            }
            return EXIT_OK;
        }

        const bool alreadyInstalled = std::filesystem::is_regular_file(sdkVersionBinary(home, targetVersion));
        bool installedFromRelease = false;
        if (!alreadyInstalled)
        {
            if (releaseRoot.empty() || !localReleaseManifestExists(releaseRoot, targetVersion))
            {
                std::cerr
                    << "error: UIMD SDK " << targetVersion
                    << " is not installed; release downloads are not implemented in this build, pass --release-root or install it first\n";
                return EXIT_ERROR;
            }
            if (!ensureLauncher(home, executablePath) || !ensureSdkVersionDirectories(home, targetVersion))
            {
                return EXIT_ERROR;
            }
            if (!installReleaseManifest(home, releaseRoot, targetVersion))
            {
                return EXIT_ERROR;
            }
            if (!std::filesystem::is_regular_file(sdkVersionBinary(home, targetVersion)))
            {
                std::cerr << "error: release manifest did not install " << pathString(SDK_BIN_DIR / uimdExecutableName()) << "\n";
                return EXIT_ERROR;
            }
            installedFromRelease = true;
        }
        if (!writeSelectedSdkVersion(home, targetVersion))
        {
            return EXIT_ERROR;
        }

        if (json)
        {
            std::cout
                << "{\"current\":\"" << jsonEscape(currentVersion)
                << "\",\"selected\":\"" << jsonEscape(targetVersion)
                << "\",\"updated\":true,\"installed\":" << (installedFromRelease ? "true" : "false")
                << ",\"status\":\"ok\"}\n";
            return EXIT_OK;
        }

        std::cout << "Updated UIMD SDK from " << currentVersion << " to " << targetVersion << "\n";
        return EXIT_OK;
    }

    if (command == "use")
    {
        if (args.size() != 2)
        {
            std::cerr << "error: sdk use requires exactly one version\n";
            return EXIT_USAGE;
        }
        const std::string version = args[1];
        if (!isVersionNameSafe(version))
        {
            std::cerr << "error: SDK version must be a safe version name\n";
            return EXIT_USAGE;
        }
        if (!std::filesystem::is_regular_file(sdkVersionBinary(home, version)))
        {
            std::cerr << "error: SDK version is not installed: " << version << "\n";
            return EXIT_ERROR;
        }
        if (!writeSelectedSdkVersion(home, version))
        {
            return EXIT_ERROR;
        }
        std::cout << "Using UIMD SDK " << version << "\n";
        return EXIT_OK;
    }

    if (command == "remove")
    {
        if (args.size() != 2)
        {
            std::cerr << "error: sdk remove requires exactly one version\n";
            return EXIT_USAGE;
        }
        const std::string version = args[1];
        if (!isVersionNameSafe(version))
        {
            std::cerr << "error: SDK version must be a safe version name\n";
            return EXIT_USAGE;
        }
        const std::filesystem::path versionRoot = sdkVersionRoot(home, version);
        if (!std::filesystem::exists(versionRoot))
        {
            std::cerr << "error: SDK version is not installed: " << version << "\n";
            return EXIT_ERROR;
        }
        std::error_code error;
        std::filesystem::remove_all(versionRoot, error);
        if (error)
        {
            std::cerr << "error: cannot remove " << pathString(versionRoot) << ": " << error.message() << "\n";
            return EXIT_ERROR;
        }
        std::ifstream currentInput(home / SDK_CURRENT_FILE);
        std::string currentVersion;
        if (currentInput)
        {
            std::getline(currentInput, currentVersion);
            currentVersion = trim(currentVersion);
        }
        if (currentVersion == version && !clearSelectedSdkVersion(home))
        {
            return EXIT_ERROR;
        }
        std::cout << "Removed UIMD SDK " << version << "\n";
        return EXIT_OK;
    }

    if (command == "prune")
    {
        bool json = false;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (args[index] == "--json")
            {
                json = true;
            }
            else
            {
                std::cerr << "error: unknown sdk prune option: " << args[index] << "\n";
                return EXIT_USAGE;
            }
        }

        const std::string currentVersion = selectedSdkVersion(home);
        const std::vector<std::string> prunable = prunableSdkVersions(home, currentVersion);
        std::vector<std::string> removed;
        for (const std::string& version : prunable)
        {
            if (!safeSdkVersionRootForRemoval(home, version))
            {
                std::cerr << "error: refusing to prune unsafe SDK version path: " << version << "\n";
                return EXIT_ERROR;
            }
            const std::filesystem::path versionRoot = sdkVersionRoot(home, version);
            std::error_code error;
            std::filesystem::remove_all(versionRoot, error);
            if (error)
            {
                std::cerr << "error: cannot remove " << pathString(versionRoot) << ": " << error.message() << "\n";
                return EXIT_ERROR;
            }
            removed.push_back(version);
        }

        if (json)
        {
            std::cout
                << "{\"sdk_home\":\"" << jsonEscape(pathString(home))
                << "\",\"current\":\"" << jsonEscape(currentVersion)
                << "\",\"removed\":[";
            for (std::size_t index = 0; index < removed.size(); ++index)
            {
                if (index != 0)
                {
                    std::cout << ",";
                }
                std::cout << "\"" << jsonEscape(removed[index]) << "\"";
            }
            std::cout << "]}\n";
            return EXIT_OK;
        }

        if (removed.empty())
        {
            std::cout << "No SDK versions pruned.\n";
            return EXIT_OK;
        }
        std::cout << "Pruned UIMD SDK versions:";
        for (const std::string& version : removed)
        {
            std::cout << " " << version;
        }
        std::cout << "\n";
        return EXIT_OK;
    }

    if (command == "home")
    {
        if (args.size() != 1)
        {
            std::cerr << "error: unexpected sdk home argument\n";
            return EXIT_USAGE;
        }
        std::cout << pathString(home) << "\n";
        return EXIT_OK;
    }

    if (command == "list")
    {
        bool json = false;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (args[index] == "--json")
            {
                json = true;
            }
            else
            {
                std::cerr << "error: unknown sdk list option: " << args[index] << "\n";
                return EXIT_USAGE;
            }
        }

        const std::vector<std::string> versions = installedSdkVersions(home);
        const std::string currentVersion = selectedSdkVersion(home);
        if (json)
        {
            std::cout << "{\"sdk_home\":\"" << jsonEscape(pathString(home)) << "\",\"current\":\"" << jsonEscape(currentVersion) << "\",\"versions\":[";
            for (std::size_t index = 0; index < versions.size(); ++index)
            {
                if (index != 0)
                {
                    std::cout << ",";
                }
                std::cout << "\"" << jsonEscape(versions[index]) << "\"";
            }
            std::cout << "],\"targets\":" << sdkTargetsJson(home, versions) << "}\n";
            return EXIT_OK;
        }

        if (versions.empty())
        {
            std::cout << "No SDK versions installed.\n";
            return EXIT_OK;
        }
        for (const std::string& version : versions)
        {
            const std::vector<std::string> targets = installedSdkTargets(home, version);
            std::cout << (version == currentVersion ? "* " : "  ") << version;
            if (!targets.empty())
            {
                std::cout << " [";
                for (std::size_t index = 0; index < targets.size(); ++index)
                {
                    if (index != 0)
                    {
                        std::cout << ", ";
                    }
                    std::cout << targets[index];
                }
                std::cout << "]";
            }
            std::cout << "\n";
        }
        return EXIT_OK;
    }

    std::cerr << "error: unknown sdk command: " << command << "\n";
    return EXIT_USAGE;
}

int runSelf(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cerr << "error: self command is required\n";
        return EXIT_USAGE;
    }

    const std::string command = args.front();
    if (command == "uninstall")
    {
        bool json = false;
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (args[index] == "--json")
            {
                json = true;
            }
            else
            {
                std::cerr << "error: unknown self uninstall option: " << args[index] << "\n";
                return EXIT_USAGE;
            }
        }

        const std::filesystem::path home = sdkHome();
        const std::filesystem::path normalizedHome = normalizedAbsolutePath(home);
        if (!std::filesystem::exists(normalizedHome))
        {
            if (json)
            {
                std::cout
                    << "{\"home\":\"" << jsonEscape(pathString(normalizedHome))
                    << "\",\"removed\":false,\"status\":\"absent\"}\n";
            }
            else
            {
                std::cout << "UIMD SDK Store is already absent at " << pathString(normalizedHome) << "\n";
            }
            return EXIT_OK;
        }
        if (!safeSdkHomeForRemoval(normalizedHome))
        {
            std::cerr << "error: refusing to uninstall unsafe or incomplete SDK Store: " << pathString(normalizedHome) << "\n";
            return EXIT_ERROR;
        }

        std::error_code error;
        std::filesystem::remove_all(normalizedHome, error);
        if (error)
        {
            std::cerr << "error: cannot remove " << pathString(normalizedHome) << ": " << error.message() << "\n";
            return EXIT_ERROR;
        }
        if (json)
        {
            std::cout
                << "{\"home\":\"" << jsonEscape(pathString(normalizedHome))
                << "\",\"removed\":true,\"status\":\"ok\"}\n";
            return EXIT_OK;
        }

        std::cout << "Removed UIMD SDK Store at " << pathString(normalizedHome) << "\n";
        std::cout << "Uninstall the uimd-sdk package manager package separately if needed.\n";
        return EXIT_OK;
    }

    std::cerr << "error: unknown self command: " << command << "\n";
    return EXIT_USAGE;
}

int runInspect(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cerr << "error: inspect path is required\n";
        return EXIT_USAGE;
    }

    bool json = false;
    std::string path;
    for (const std::string& arg : args)
    {
        if (arg == "--json")
        {
            json = true;
        }
        else if (path.empty())
        {
            path = arg;
        }
        else
        {
            std::cerr << "error: unexpected inspect argument: " << arg << "\n";
            return EXIT_USAGE;
        }
    }
    if (path.empty())
    {
        std::cerr << "error: inspect path is required\n";
        return EXIT_USAGE;
    }

    try
    {
        const uimd::tool::NativeDocument document = uimd::tool::parseDocumentFile(path);
        if (json)
        {
            std::cout << uimd::tool::inspectDocumentJson(document) << "\n";
        }
        else
        {
            std::cout << "Title: " << document.title << "\n";
            std::cout << "Members: " << document.members.size() << "\n";
            std::cout << "Tools: " << document.tools.size() << "\n";
            std::cout << "UI text bytes: " << document.uiText.size() << "\n";
        }
    }
    catch (const std::exception& exc)
    {
        std::cerr << "error: " << exc.what() << "\n";
        return EXIT_ERROR;
    }
    return EXIT_OK;
}

int runNew(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cerr << "error: application name is required\n";
        return EXIT_USAGE;
    }

    std::string target = "python";
    bool force = false;
    std::string rawName;

    for (std::size_t index = 0; index < args.size(); ++index)
    {
        const std::string& arg = args[index];
        if (arg == "--target")
        {
            if (index + 1 >= args.size())
            {
                std::cerr << "error: --target requires a value\n";
                return EXIT_USAGE;
            }
            target = args[++index];
        }
        else if (arg == "--force")
        {
            force = true;
        }
        else if (rawName.empty())
        {
            rawName = arg;
        }
        else
        {
            std::cerr << "error: unexpected argument: " << arg << "\n";
            return EXIT_USAGE;
        }
    }

    const std::string project = projectName(rawName);
    if (project.empty())
    {
        std::cerr << "error: application name cannot be empty\n";
        return EXIT_ERROR;
    }
    if (target != "python" && target != "cpp")
    {
        std::cerr << "error: --target must be python or cpp\n";
        return EXIT_USAGE;
    }

    const std::string klass = className(project);
    const std::vector<TemplateValue> values{
        {"@PROJECT@", project},
        {"@CLASS@", klass},
        {"@VERSION@", runtimeVersion()},
    };

    std::vector<GeneratedFile> files;
    files.emplace_back(project + ".uimd", applyTemplate(uimdSourceTemplate(), values));
    if (target == "cpp")
    {
        files.emplace_back(project + ".cpp", applyTemplate(cppAppTemplate(), values));
        files.emplace_back("CMakeLists.txt", applyTemplate(cppCMakeTemplate(), values));
    }
    else
    {
        files.emplace_back(project + ".py", applyTemplate(pythonAppTemplate(), values));
    }

    std::vector<std::filesystem::path> existing;
    for (const auto& [path, content] : files)
    {
        (void)content;
        if (std::filesystem::exists(path))
        {
            existing.push_back(path);
        }
    }

    if (!existing.empty() && !force)
    {
        std::cerr << "error: refusing to overwrite existing file(s):\n";
        for (const std::filesystem::path& path : existing)
        {
            std::cerr << "  - " << pathString(path) << "\n";
        }
        std::cerr << "pass --force to overwrite\n";
        return EXIT_ERROR;
    }

    for (const auto& [path, content] : files)
    {
        if (!writeText(path, content))
        {
            return EXIT_ERROR;
        }
        std::cout << "created " << pathString(path) << "\n";
    }
    return EXIT_OK;
}

int runGenerate(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cerr << "error: generate path is required\n";
        return EXIT_USAGE;
    }

    std::string target = "python";
    std::string path;
    uimd::tool::NativeGenerateOptions options;
    bool generateAppStub = false;

    for (std::size_t index = 0; index < args.size(); ++index)
    {
        const std::string& arg = args[index];
        if (arg == "--target")
        {
            if (index + 1 >= args.size())
            {
                std::cerr << "error: --target requires a value\n";
                return EXIT_USAGE;
            }
            target = args[++index];
        }
        else if (arg == "--output-dir")
        {
            if (index + 1 >= args.size())
            {
                std::cerr << "error: --output-dir requires a value\n";
                return EXIT_USAGE;
            }
            options.outputDir = args[++index];
            options.hasOutputDir = true;
        }
        else if (arg == "--no-deps")
        {
            options.compileDependencies = false;
        }
        else if (arg == "--mcp")
        {
            options.mcpEnabled = true;
        }
        else if (arg == "--no-mcp")
        {
            options.mcpEnabled = false;
        }
        else if (arg == "--app-stub")
        {
            generateAppStub = true;
        }
        else if (path.empty())
        {
            path = arg;
        }
        else
        {
            std::cerr << "error: unexpected generate argument: " << arg << "\n";
            return EXIT_USAGE;
        }
    }

    if (path.empty())
    {
        std::cerr << "error: generate path is required\n";
        return EXIT_USAGE;
    }
    if (target != "python" && target != "cpp")
    {
        std::cerr << "error: --target must be python or cpp\n";
        return EXIT_USAGE;
    }

    const std::filesystem::path sourcePath{path};
    if (!std::filesystem::exists(sourcePath))
    {
        std::cerr << "error: path does not exist: " << path << "\n";
        return EXIT_USAGE;
    }

    try
    {
        std::vector<std::filesystem::path> generated;
        if (target == "cpp")
        {
            uimd::tool::NativeCppGenerateOptions cppOptions;
            cppOptions.outputDir = options.outputDir;
            cppOptions.hasOutputDir = options.hasOutputDir;
            cppOptions.generateAppStub = generateAppStub;
            cppOptions.mcpEnabled = options.mcpEnabled;
            generated = uimd::tool::generateCppSources(sourcePath, cppOptions);
        }
        else
        {
            generated = uimd::tool::generatePythonSources(sourcePath, options);
        }
        if (generated.empty())
        {
            std::cout << "No UIMD source files generated.\n";
            return EXIT_OK;
        }
        std::cout << "Generated " << generated.size() << " file(s):\n";
        for (const std::filesystem::path& generatedPath : generated)
        {
            std::cout << "  - " << pathString(generatedPath) << "\n";
        }
    }
    catch (const std::exception& exc)
    {
        std::cerr << "error: " << exc.what() << "\n";
        return EXIT_ERROR;
    }

    return EXIT_OK;
}

int runRun(const std::vector<std::string>& args)
{
    bool compileDependencies = true;
    bool mcpEnabled = true;
    std::string path;
    std::vector<std::string> appArgs;
    bool passthrough = false;

    for (const std::string& arg : args)
    {
        if (passthrough)
        {
            appArgs.push_back(arg);
            continue;
        }
        if (arg == "--")
        {
            passthrough = true;
        }
        else if (arg == "--no-deps")
        {
            compileDependencies = false;
        }
        else if (arg == "--mcp")
        {
            mcpEnabled = true;
        }
        else if (arg == "--no-mcp")
        {
            mcpEnabled = false;
        }
        else if (path.empty())
        {
            path = arg;
        }
        else if (arg.rfind("--", 0) == 0)
        {
            std::cerr << "error: unexpected run argument: " << arg << "\n";
            return EXIT_USAGE;
        }
        else
        {
            appArgs.push_back(arg);
        }
    }

    if (path.empty())
    {
        std::cerr << "error: run path is required\n";
        return EXIT_USAGE;
    }

    const std::filesystem::path sourcePath{path};
    if (!std::filesystem::is_regular_file(sourcePath))
    {
        std::cerr << "error: not a file: " << path << "\n";
        return EXIT_USAGE;
    }

    try
    {
        uimd::tool::NativeGenerateOptions options;
        options.compileDependencies = compileDependencies;
        options.mcpEnabled = mcpEnabled;
        (void)uimd::tool::generatePythonSources(sourcePath, options);
    }
    catch (const std::exception& exc)
    {
        std::cerr << "error: " << exc.what() << "\n";
        return EXIT_ERROR;
    }

    std::filesystem::path appPath = sourcePath;
    appPath.replace_extension(".py");
    if (!std::filesystem::exists(appPath))
    {
        std::cerr << "error: app file not found after generation: " << pathString(appPath) << "\n";
        std::cerr << "hint: create " << appPath.filename().string() << " or run `uimd new " << sourcePath.stem().string() << "` first\n";
        return EXIT_ERROR;
    }

    configurePythonRuntimePath();
    std::vector<std::string> command{pythonExecutable(), pathString(appPath)};
    command.insert(command.end(), appArgs.begin(), appArgs.end());
    return runProcess(std::move(command));
}

constexpr const char* MCP_BACKEND_CPP = "cpp";
constexpr const char* MCP_BACKEND_PYTHON = "python";
constexpr const char* MCP_BACKEND_ENV = "UIMD_MCP_TESTER_BACKEND";
constexpr int MCP_CPP_TESTER_MISSING_EXIT_CODE = 127;
const std::filesystem::path MCP_CPP_TESTER_RELATIVE_PATH{"cpp/build/tools/mcp_tester/uimd_mcp_tester"};
const std::string MCP_CPP_BUILD_COMMAND = "cmake --build cpp/build --target uimd_mcp_tester";
const std::vector<std::filesystem::path> MCP_PROJECT_MARKERS{
    std::filesystem::path{"tests/mcp/all_examples.yaml"},
    std::filesystem::path{"cpp/tools/mcp_tester/main.cpp"},
};

bool isMcpProjectRoot(const std::filesystem::path& path)
{
    for (const std::filesystem::path& marker : MCP_PROJECT_MARKERS)
    {
        if (!std::filesystem::exists(path / marker))
        {
            return false;
        }
    }
    return true;
}

std::filesystem::path mcpProjectRoot()
{
    std::vector<std::filesystem::path> starts{std::filesystem::current_path()};
    const std::filesystem::path root = sourceRoot();
    if (!root.empty())
    {
        starts.push_back(root);
    }

    for (std::filesystem::path start : starts)
    {
        std::filesystem::path current = std::filesystem::absolute(start).lexically_normal();
        while (true)
        {
            if (isMcpProjectRoot(current))
            {
                return current;
            }
            if (!current.has_parent_path() || current.parent_path() == current)
            {
                break;
            }
            current = current.parent_path();
        }
    }
    return std::filesystem::current_path();
}

std::string normalizeMcpBackend(const std::string& value)
{
    std::string backend = lower(trim(value.empty() ? std::string{MCP_BACKEND_CPP} : value));
    if (backend == "c++" || backend == "uimd_mcp_tester")
    {
        return MCP_BACKEND_CPP;
    }
    if (backend == "py" || backend == "python3" || backend == "reference")
    {
        return MCP_BACKEND_PYTHON;
    }
    if (backend != MCP_BACKEND_CPP && backend != MCP_BACKEND_PYTHON)
    {
        throw std::runtime_error("unsupported MCP tester backend: " + value);
    }
    return backend;
}

std::pair<std::string, std::vector<std::string>> extractMcpBackend(const std::vector<std::string>& args)
{
    std::string backend = envValue(MCP_BACKEND_ENV);
    if (backend.empty())
    {
        backend = MCP_BACKEND_CPP;
    }

    std::vector<std::string> forwarded;
    for (std::size_t index = 0; index < args.size(); ++index)
    {
        const std::string& arg = args[index];
        if (arg == "--backend")
        {
            ++index;
            if (index >= args.size())
            {
                throw std::runtime_error("--backend requires cpp or python");
            }
            backend = args[index];
        }
        else if (arg.rfind("--backend=", 0) == 0)
        {
            backend = arg.substr(std::string{"--backend="}.size());
        }
        else if (arg == "--cpp")
        {
            backend = MCP_BACKEND_CPP;
        }
        else if (arg == "--python")
        {
            backend = MCP_BACKEND_PYTHON;
        }
        else
        {
            forwarded.push_back(arg);
        }
    }
    return {normalizeMcpBackend(backend), forwarded};
}

void printMcpBackendHelp()
{
    std::cout
        << "\nBackend selection:\n"
        << "  --backend cpp|python  Choose tester backend; default is cpp.\n"
        << "  --cpp                 Alias for --backend cpp.\n"
        << "  --python              Alias for --backend python.\n"
        << "  " << MCP_BACKEND_ENV << "=cpp|python  Environment fallback for backend selection.\n"
        << std::flush;
}

bool wantsMcpHelp(const std::vector<std::string>& args)
{
    return std::find(args.begin(), args.end(), "-h") != args.end() ||
        std::find(args.begin(), args.end(), "--help") != args.end();
}

int runCppMcpTester(const std::vector<std::string>& args)
{
    const std::filesystem::path root = mcpProjectRoot();
    const std::filesystem::path binary = root / MCP_CPP_TESTER_RELATIVE_PATH;
    if (!std::filesystem::exists(binary))
    {
        std::cerr
            << "C++ MCP tester binary not found: " << pathString(binary) << "\n"
            << "Build it with: " << MCP_CPP_BUILD_COMMAND << "\n";
        return MCP_CPP_TESTER_MISSING_EXIT_CODE;
    }

    configurePythonRuntimePath();
    std::vector<std::string> command{pathString(binary)};
    command.insert(command.end(), args.begin(), args.end());
    return runProcess(std::move(command), root);
}

int runPythonMcpTester(const std::vector<std::string>& args)
{
    configurePythonRuntimePath();
    std::vector<std::string> command{pythonExecutable(), "-m", "uimd.testing.mcp_tester"};
    command.insert(command.end(), args.begin(), args.end());
    return runProcess(std::move(command));
}

int runMcpTest(const std::vector<std::string>& args)
{
    std::string backend;
    std::vector<std::string> forwarded;
    try
    {
        std::tie(backend, forwarded) = extractMcpBackend(args);
    }
    catch (const std::exception& exc)
    {
        std::cerr << "mcp_tester: " << exc.what() << "\n";
        return EXIT_USAGE;
    }

    if (wantsMcpHelp(args))
    {
        printMcpBackendHelp();
    }
    if (backend == MCP_BACKEND_PYTHON)
    {
        return runPythonMcpTester(forwarded);
    }
    return runCppMcpTester(forwarded);
}

}  // namespace

int main(int argc, char** argv)
{
    std::vector<std::string> args;
    for (int index = 1; index < argc; ++index)
    {
        args.emplace_back(argv[index]);
    }

    const GlobalOptions globalOptions = extractGlobalOptions(args);
    const std::filesystem::path executable = currentExecutablePath(argc > 0 ? argv[0] : nullptr);
    const std::filesystem::path home = sdkHome();
    const std::filesystem::path launcher = launcherPath(home);
    if (!args.empty() &&
        !executable.empty() &&
        std::filesystem::is_regular_file(launcher) &&
        samePath(executable, launcher) &&
        !commandHandledByLauncher(args))
    {
        return delegateToSelectedSdk(home, launcher, args, globalOptions);
    }

    if (args.empty())
    {
        return printHelp(argc > 0 ? argv[0] : "uimd");
    }
    if (args.front() == "--version")
    {
        std::cout << "UIMD " << runtimeVersion() << "\n";
        return EXIT_OK;
    }
    if (args.front() == "--help" || args.front() == "-h")
    {
        return printHelp(argc > 0 ? argv[0] : "uimd");
    }

    const std::string command = args.front();
    args.erase(args.begin());

    if (command == "doctor")
    {
        return runDoctor(args);
    }
    if (command == "new")
    {
        return runNew(args);
    }
    if (command == "sdk")
    {
        return runSdk(args, executable);
    }
    if (command == "self")
    {
        return runSelf(args);
    }
    if (command == "inspect")
    {
        return runInspect(args);
    }
    if (command == "generate")
    {
        return runGenerate(args);
    }
    if (command == "run")
    {
        return runRun(args);
    }
    if (command == "mcp-test")
    {
        return runMcpTest(args);
    }

    std::cerr << "error: unknown command: " << command << "\n";
    return printHelp(argc > 0 ? argv[0] : "uimd");
}

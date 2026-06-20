#include <cstdlib>
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#ifndef UIMD_VERSION
#define UIMD_VERSION "0.0.0"
#endif

namespace
{

constexpr int EXIT_OK = 0;
constexpr int EXIT_ERROR = 1;
constexpr int EXIT_USAGE = 2;

const std::filesystem::path SDK_CURRENT_FILE{"current"};
const std::filesystem::path SDK_ROOT_DIR{"sdk"};
const std::filesystem::path SDK_BIN_DIR{"bin"};
const std::filesystem::path SDK_TARGETS_DIR{"targets"};
const std::filesystem::path SDK_PYTHON_TARGET_DIR{"python"};
const std::filesystem::path SDK_EXAMPLES_DIR{"examples"};
const std::string SHELL_CONFIG_MARKER{"# UIMD SDK"};
const std::string RELEASE_BASE_URL_ENV{"UIMD_RELEASE_BASE_URL"};
const std::string RELEASE_PUBLIC_KEY_ENV{"UIMD_RELEASE_PUBLIC_KEY"};
const std::string ALLOW_UNSIGNED_RELEASE_ENV{"UIMD_ALLOW_UNSIGNED_RELEASE"};
const std::string RELEASE_CHECKSUMS_FILE{"checksums.txt"};
const std::string RELEASE_SIGNATURE_FILE{"checksums.txt.minisig"};
const std::string RELEASE_PUBLIC_KEY{"RWR71aDOUx1vHQeAYhBjmL71qWnPzCp3kXGe2HLHPORARHbM2Al77AsD"};

struct ReleaseManifestFile
{
    std::filesystem::path destination;
    std::string checksum;
    std::filesystem::path source;
};

struct ShellConfigResult
{
    bool ok{true};
    bool changed{false};
    std::filesystem::path profile;
    std::string state{"unchanged"};
};

std::string envValue(const char* name)
{
    const char* value = std::getenv(name);
    return value == nullptr ? std::string{} : std::string{value};
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

std::filesystem::path sdkVersionRoot(const std::filesystem::path& home)
{
    return home / SDK_ROOT_DIR / UIMD_VERSION;
}

std::filesystem::path sdkVersionBinary(const std::filesystem::path& home)
{
    return sdkVersionRoot(home) / SDK_BIN_DIR / uimdExecutableName();
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

bool looksLikeSha256(const std::string& value)
{
    return value.size() == 64U && std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isxdigit(ch);
    });
}

bool safeRelativePath(const std::filesystem::path& path)
{
    if (path.empty() || path.is_absolute())
    {
        return false;
    }
    for (const std::filesystem::path& part : path)
    {
        if (part == "..")
        {
            return false;
        }
    }
    return true;
}

std::string releasePlatform()
{
#if defined(__APPLE__) && (defined(__x86_64__) || defined(_M_X64))
    return "macos-x86_64";
#elif defined(__APPLE__) && (defined(__aarch64__) || defined(__arm64__) || defined(_M_ARM64))
    return "macos-arm64";
#elif defined(__linux__) && (defined(__x86_64__) || defined(_M_X64))
    return "linux-x86_64";
#elif defined(__linux__) && (defined(__aarch64__) || defined(_M_ARM64))
    return "linux-arm64";
#elif defined(_WIN32) && defined(_M_ARM64)
    return "windows-arm64";
#elif defined(_WIN32) && (defined(_M_X64) || defined(__x86_64__))
    return "windows-x86_64";
#else
    return {};
#endif
}

std::string releaseAssetName()
{
    const std::string platform = releasePlatform();
    if (platform.empty())
    {
        return {};
    }
    return "uimd-sdk-" + std::string{UIMD_VERSION} + "-" + platform + ".tar.gz";
}

std::string releaseDirectoryName()
{
    const std::string platform = releasePlatform();
    if (platform.empty())
    {
        return {};
    }
    return "uimd-sdk-" + std::string{UIMD_VERSION} + "-" + platform;
}

std::string releaseBaseUrl()
{
    std::string base = envValue(RELEASE_BASE_URL_ENV.c_str());
    if (base.empty())
    {
        base = "https://github.com/uimd-lang/uimd/releases/download/v" + std::string{UIMD_VERSION};
    }
    while (!base.empty() && base.back() == '/')
    {
        base.pop_back();
    }
    return base;
}

std::string releaseAssetUrl(const std::string& asset)
{
    return releaseBaseUrl() + "/" + asset;
}

std::filesystem::path userHome()
{
#ifdef _WIN32
    const std::string userProfile = envValue("USERPROFILE");
    if (!userProfile.empty())
    {
        return std::filesystem::path{userProfile};
    }
    const std::string home = envValue("HOME");
    if (!home.empty())
    {
        return std::filesystem::path{home};
    }
#else
    const std::string home = envValue("HOME");
    if (!home.empty())
    {
        return std::filesystem::path{home};
    }
#endif
    return {};
}

std::string activeShellName()
{
    const std::string shell = envValue("SHELL");
    if (shell.empty())
    {
        return {};
    }
    return std::filesystem::path{shell}.filename().string();
}

std::filesystem::path shellProfilePath()
{
    const std::filesystem::path home = userHome();
    if (home.empty())
    {
        return {};
    }
#ifdef _WIN32
    return home / "Documents" / "PowerShell" / "Microsoft.PowerShell_profile.ps1";
#else
    const std::string shell = activeShellName();
    if (shell == "zsh")
    {
        return home / ".zshrc";
    }
    if (shell == "bash")
    {
        return home / ".bashrc";
    }
    if (shell == "fish")
    {
        return home / ".config" / "fish" / "config.fish";
    }
    return home / ".profile";
#endif
}

#ifdef _WIN32
std::string powershellSingleQuote(const std::string& value)
{
    std::string quoted{"'"};
    for (char ch : value)
    {
        if (ch == '\'')
        {
            quoted += "''";
        }
        else
        {
            quoted += ch;
        }
    }
    quoted += "'";
    return quoted;
}
#else
std::string posixSingleQuote(const std::string& value)
{
    std::string quoted{"'"};
    for (char ch : value)
    {
        if (ch == '\'')
        {
            quoted += "'\\''";
        }
        else
        {
            quoted += ch;
        }
    }
    quoted += "'";
    return quoted;
}
#endif

std::string shellPathCommand(const std::filesystem::path& launcherDirectory)
{
    const std::string launcher = launcherDirectory.lexically_normal().string();
#ifdef _WIN32
    return "$env:Path = " + powershellSingleQuote(launcher + ";") + " + $env:Path";
#else
    if (activeShellName() == "fish")
    {
        return "set -gx PATH " + posixSingleQuote(launcher) + " $PATH";
    }
    return "export PATH=" + posixSingleQuote(launcher) + ":\"$PATH\"";
#endif
}

bool runShellCommand(const std::string& command)
{
    return std::system(command.c_str()) == 0;
}

[[maybe_unused]] std::string readShellCommand(const std::string& command)
{
#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif
    std::array<char, 256> buffer{};
    std::string output;
    if (pipe == nullptr)
    {
        return {};
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr)
    {
        output += buffer.data();
    }
#ifdef _WIN32
    const int status = _pclose(pipe);
#else
    const int status = pclose(pipe);
#endif
    if (status != 0)
    {
        return {};
    }
    return output;
}

bool downloadFile(const std::string& url, const std::filesystem::path& destination)
{
    std::error_code error;
    std::filesystem::create_directories(destination.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination.parent_path()) << ": " << error.message() << "\n";
        return false;
    }
#ifdef _WIN32
    const std::string command =
        "powershell -NoProfile -ExecutionPolicy Bypass -Command "
        "\"$ProgressPreference = 'SilentlyContinue'; "
        "Invoke-WebRequest -UseBasicParsing -Uri " + powershellSingleQuote(url) +
        " -OutFile " + powershellSingleQuote(pathString(destination)) + "\"";
#else
    const std::string command = "curl -fsSL --retry 3 -o " +
        posixSingleQuote(pathString(destination)) + " " + posixSingleQuote(url);
#endif
    if (!runShellCommand(command))
    {
        std::cerr << "error: failed to download " << url << "\n";
        return false;
    }
    return std::filesystem::is_regular_file(destination);
}

std::string releasePublicKey()
{
    const std::string overrideKey = trim(envValue(RELEASE_PUBLIC_KEY_ENV.c_str()));
    return overrideKey.empty() ? RELEASE_PUBLIC_KEY : overrideKey;
}

bool allowUnsignedRelease()
{
    return trim(envValue(ALLOW_UNSIGNED_RELEASE_ENV.c_str())) == "1";
}

bool verifyChecksumsSignature(const std::filesystem::path& checksumsPath, const std::filesystem::path& signaturePath)
{
    if (allowUnsignedRelease())
    {
        return true;
    }
    const std::string publicKey = releasePublicKey();
    if (publicKey.empty())
    {
        std::cerr << "error: release public key is not configured\n";
        return false;
    }
    if (!std::filesystem::is_regular_file(checksumsPath))
    {
        std::cerr << "error: checksums file is missing: " << pathString(checksumsPath) << "\n";
        return false;
    }
    if (!std::filesystem::is_regular_file(signaturePath))
    {
        std::cerr << "error: checksums signature is missing: " << pathString(signaturePath) << "\n";
        return false;
    }
#ifdef _WIN32
    const std::string command =
        "powershell -NoProfile -ExecutionPolicy Bypass -Command "
        "\"minisign -Vq -P " + powershellSingleQuote(publicKey) +
        " -m " + powershellSingleQuote(pathString(checksumsPath)) +
        " -x " + powershellSingleQuote(pathString(signaturePath)) + "\"";
#else
    const std::string command =
        "minisign -Vq -P " + posixSingleQuote(publicKey) +
        " -m " + posixSingleQuote(pathString(checksumsPath)) +
        " -x " + posixSingleQuote(pathString(signaturePath));
#endif
    if (!runShellCommand(command))
    {
        std::cerr << "error: failed to verify " << RELEASE_CHECKSUMS_FILE
                  << " signature; install minisign or check release integrity\n";
        return false;
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
            const std::uint32_t s0 = rotateRight(words[index - 15U], 7U) ^
                rotateRight(words[index - 15U], 18U) ^ (words[index - 15U] >> 3U);
            const std::uint32_t s1 = rotateRight(words[index - 2U], 17U) ^
                rotateRight(words[index - 2U], 19U) ^ (words[index - 2U] >> 10U);
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

bool extractTarball(const std::filesystem::path& archive, const std::filesystem::path& destination)
{
    std::error_code error;
    std::filesystem::remove_all(destination, error);
    error.clear();
    std::filesystem::create_directories(destination, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination) << ": " << error.message() << "\n";
        return false;
    }
#ifdef _WIN32
    const std::string command =
        "powershell -NoProfile -ExecutionPolicy Bypass -Command "
        "\"tar -xzf " + powershellSingleQuote(pathString(archive)) +
        " -C " + powershellSingleQuote(pathString(destination)) + "\"";
#else
    const std::string command = "LC_ALL=C tar -xzf " + posixSingleQuote(pathString(archive)) +
        " -C " + posixSingleQuote(pathString(destination));
#endif
    if (!runShellCommand(command))
    {
        std::cerr << "error: failed to extract " << pathString(archive) << "\n";
        return false;
    }
    return true;
}

bool readTextFile(const std::filesystem::path& path, std::string& text)
{
    if (!std::filesystem::exists(path))
    {
        text.clear();
        return true;
    }
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        return false;
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    text = buffer.str();
    return true;
}

ShellConfigResult configureShellProfile(const std::filesystem::path& home)
{
    ShellConfigResult result;
    result.state = "modify";
    result.profile = shellProfilePath();
    if (result.profile.empty())
    {
        std::cerr << "error: cannot determine shell profile path; set HOME or USERPROFILE\n";
        result.ok = false;
        result.state = "error";
        return result;
    }

    const std::filesystem::path launcherDirectory = home / SDK_BIN_DIR;
    const std::string pathCommand = shellPathCommand(launcherDirectory);
    std::string existing;
    if (!readTextFile(result.profile, existing))
    {
        std::cerr << "error: cannot read shell profile " << result.profile << "\n";
        result.ok = false;
        result.state = "error";
        return result;
    }
    if (existing.find(pathCommand) != std::string::npos)
    {
        result.state = "already-configured";
        return result;
    }

    std::error_code error;
    std::filesystem::create_directories(result.profile.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << result.profile.parent_path() << ": " << error.message() << "\n";
        result.ok = false;
        result.state = "error";
        return result;
    }

    std::ofstream output(result.profile, std::ios::binary | std::ios::app);
    if (!output)
    {
        std::cerr << "error: cannot write shell profile " << result.profile << "\n";
        result.ok = false;
        result.state = "error";
        return result;
    }
    if (!existing.empty() && existing.back() != '\n')
    {
        output << "\n";
    }
    output << SHELL_CONFIG_MARKER << "\n" << pathCommand << "\n";
    result.changed = true;
    result.state = "modified";
    return result;
}

std::string jsonEscape(const std::string& value)
{
    std::string escaped;
    for (char ch : value)
    {
        if (ch == '\\')
        {
            escaped += "\\\\";
        }
        else if (ch == '"')
        {
            escaped += "\\\"";
        }
        else
        {
            escaped += ch;
        }
    }
    return escaped;
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
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing, error);
    if (error)
    {
        std::cerr << "error: cannot copy " << pathString(source) << " to " << pathString(destination) << ": " << error.message() << "\n";
        return false;
    }
    return true;
}

bool copyExecutableFile(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    if (!copyRegularFile(source, destination))
    {
        return false;
    }
    std::error_code error;
#ifndef _WIN32
    std::filesystem::permissions(
        destination,
        std::filesystem::perms::owner_exec | std::filesystem::perms::group_exec | std::filesystem::perms::others_exec,
        std::filesystem::perm_options::add,
        error
    );
#endif
    return true;
}

bool copyBinary(const std::filesystem::path& destination)
{
    const std::string source = envValue("UIMD_INIT_UIMD_BINARY");
    if (source.empty())
    {
        return std::filesystem::is_regular_file(destination);
    }

    std::error_code error;
    if (std::filesystem::is_regular_file(destination))
    {
        const bool sameFile = std::filesystem::equivalent(source, destination, error);
        if (!error && sameFile)
        {
            return true;
        }
        error.clear();
    }
    return copyExecutableFile(source, destination);
}

bool writeCurrentVersion(const std::filesystem::path& home)
{
    std::ofstream output(home / SDK_CURRENT_FILE, std::ios::binary);
    if (!output)
    {
        std::cerr << "error: cannot write " << (home / SDK_CURRENT_FILE) << "\n";
        return false;
    }
    output << UIMD_VERSION << "\n";
    return true;
}

std::string checksumForAsset(const std::filesystem::path& checksumsPath, const std::string& assetName)
{
    std::ifstream input(checksumsPath);
    if (!input)
    {
        return {};
    }
    std::string line;
    while (std::getline(input, line))
    {
        const std::vector<std::string> parts = splitWhitespace(line);
        if (parts.size() >= 2U && parts[1] == assetName && looksLikeSha256(parts[0]))
        {
            return lower(parts[0]);
        }
    }
    return {};
}

bool verifyAssetChecksum(const std::filesystem::path& checksumsPath, const std::filesystem::path& assetPath)
{
    const std::string assetName = assetPath.filename().string();
    const std::string expected = checksumForAsset(checksumsPath, assetName);
    if (expected.empty())
    {
        std::cerr << "error: missing checksum for " << assetName << " in " << pathString(checksumsPath) << "\n";
        return false;
    }
    const std::string actual = sha256File(assetPath);
    if (actual.empty())
    {
        std::cerr << "error: cannot compute SHA-256 for " << pathString(assetPath) << "\n";
        return false;
    }
    if (actual != expected)
    {
        std::cerr
            << "error: checksum mismatch for " << assetName
            << ": expected " << expected
            << ", got " << actual << "\n";
        return false;
    }
    return true;
}

std::vector<ReleaseManifestFile> readReleaseManifest(const std::filesystem::path& manifestPath)
{
    std::ifstream input(manifestPath);
    if (!input)
    {
        throw std::runtime_error("release manifest not found: " + pathString(manifestPath));
    }

    bool versionSeen = false;
    std::vector<ReleaseManifestFile> files;
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
            if (parts.size() != 2U || parts[1] != UIMD_VERSION)
            {
                throw std::runtime_error("invalid release manifest version on line " + std::to_string(lineNumber));
            }
            versionSeen = true;
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
            files.push_back(file);
        }
        else
        {
            throw std::runtime_error("unknown release manifest entry on line " + std::to_string(lineNumber) + ": " + parts[0]);
        }
    }

    if (!versionSeen)
    {
        throw std::runtime_error("release manifest has no version entry: " + pathString(manifestPath));
    }
    if (files.empty())
    {
        throw std::runtime_error("release manifest has no file entries: " + pathString(manifestPath));
    }
    return files;
}

bool installExtractedRelease(const std::filesystem::path& home, const std::filesystem::path& releaseRoot)
{
    std::vector<ReleaseManifestFile> files;
    const std::filesystem::path manifestPath = releaseRoot / "manifest.txt";
    try
    {
        files = readReleaseManifest(manifestPath);
    }
    catch (const std::exception& exc)
    {
        std::cerr << "error: " << exc.what() << "\n";
        return false;
    }

    for (const ReleaseManifestFile& file : files)
    {
        const std::filesystem::path source = releaseRoot / file.source;
        if (!std::filesystem::is_regular_file(source))
        {
            std::cerr << "error: release file is missing: " << pathString(source) << "\n";
            return false;
        }
        const std::string actual = sha256File(source);
        if (actual.empty())
        {
            std::cerr << "error: cannot compute SHA-256 for " << pathString(source) << "\n";
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

    const std::filesystem::path destinationRoot = sdkVersionRoot(home);
    for (const ReleaseManifestFile& file : files)
    {
        const std::filesystem::path source = releaseRoot / file.source;
        const std::filesystem::path destination = destinationRoot / file.destination;
        const bool executable = file.destination == (SDK_BIN_DIR / uimdExecutableName());
        const bool copied = executable
            ? copyExecutableFile(source, destination)
            : copyRegularFile(source, destination);
        if (!copied)
        {
            return false;
        }
    }

    return copyExecutableFile(sdkVersionBinary(home), launcherPath(home)) && writeCurrentVersion(home);
}

bool installReleaseDownload(const std::filesystem::path& home)
{
    const std::string platform = releasePlatform();
    if (platform.empty())
    {
        std::cerr << "error: unsupported GitHub Release SDK download platform\n";
        return false;
    }

    const std::string asset = releaseAssetName();
    const std::filesystem::path workRoot = home / "tmp" / ("uimd-init-" + std::string{UIMD_VERSION});
    const std::filesystem::path checksumsPath = workRoot / RELEASE_CHECKSUMS_FILE;
    const std::filesystem::path signaturePath = workRoot / RELEASE_SIGNATURE_FILE;
    const std::filesystem::path archivePath = workRoot / asset;
    const std::filesystem::path extractRoot = workRoot / "extract";

    std::error_code error;
    std::filesystem::remove_all(workRoot, error);
    error.clear();
    std::filesystem::create_directories(workRoot, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(workRoot) << ": " << error.message() << "\n";
        return false;
    }

    const bool requireSignature = !allowUnsignedRelease();
    if (!downloadFile(releaseAssetUrl(RELEASE_CHECKSUMS_FILE), checksumsPath) ||
        (requireSignature && !downloadFile(releaseAssetUrl(RELEASE_SIGNATURE_FILE), signaturePath)) ||
        !verifyChecksumsSignature(checksumsPath, signaturePath) ||
        !downloadFile(releaseAssetUrl(asset), archivePath) ||
        !verifyAssetChecksum(checksumsPath, archivePath) ||
        !extractTarball(archivePath, extractRoot))
    {
        return false;
    }

    const std::filesystem::path releaseRoot = extractRoot / releaseDirectoryName();
    if (!std::filesystem::is_regular_file(releaseRoot / "manifest.txt"))
    {
        std::cerr << "error: extracted SDK release is missing manifest: " << pathString(releaseRoot / "manifest.txt") << "\n";
        return false;
    }

    const bool installed = installExtractedRelease(home, releaseRoot);
    if (installed)
    {
        std::filesystem::remove_all(workRoot, error);
    }
    return installed;
}

bool ensureStore(const std::filesystem::path& home)
{
    std::error_code error;
    std::filesystem::create_directories(home / SDK_BIN_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << (home / SDK_BIN_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(home / SDK_ROOT_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << (home / SDK_ROOT_DIR) << ": " << error.message() << "\n";
        return false;
    }

    if (envValue("UIMD_INIT_UIMD_BINARY").empty())
    {
        return installReleaseDownload(home);
    }

    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_BIN_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(sdkVersionRoot(home) / SDK_BIN_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(sdkVersionRoot(home) / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_EXAMPLES_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(sdkVersionRoot(home) / SDK_EXAMPLES_DIR) << ": " << error.message() << "\n";
        return false;
    }
    return copyBinary(launcherPath(home)) &&
        copyBinary(sdkVersionBinary(home)) &&
        writeCurrentVersion(home);
}

bool storeValid(const std::filesystem::path& home)
{
    std::ifstream current(home / SDK_CURRENT_FILE);
    std::string currentVersion;
    if (current)
    {
        std::getline(current, currentVersion);
    }
    return std::filesystem::is_directory(home / SDK_BIN_DIR) &&
           std::filesystem::is_directory(home / SDK_ROOT_DIR) &&
           std::filesystem::is_regular_file(launcherPath(home)) &&
           std::filesystem::is_regular_file(sdkVersionBinary(home)) &&
           std::filesystem::is_directory(sdkVersionRoot(home) / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR) &&
           currentVersion == UIMD_VERSION;
}

int printHelp(const char* program)
{
    std::cout
        << "usage: " << (program == nullptr ? "uimd-init" : program)
        << " [--check] [--json] [--no-shell-config|--modify-shell]\n";
    return EXIT_USAGE;
}

}  // namespace

int main(int argc, char** argv)
{
    bool checkOnly = false;
    bool json = false;
    bool noShellConfig = false;
    bool modifyShell = false;

    for (int index = 1; index < argc; ++index)
    {
        const std::string arg = argv[index];
        if (arg == "--check")
        {
            checkOnly = true;
        }
        else if (arg == "--json")
        {
            json = true;
        }
        else if (arg == "--no-shell-config")
        {
            noShellConfig = true;
        }
        else if (arg == "--modify-shell")
        {
            modifyShell = true;
        }
        else if (arg == "--help" || arg == "-h")
        {
            return printHelp(argc > 0 ? argv[0] : "uimd-init");
        }
        else
        {
            std::cerr << "error: unknown argument: " << arg << "\n";
            return printHelp(argc > 0 ? argv[0] : "uimd-init");
        }
    }

    if (noShellConfig && modifyShell)
    {
        std::cerr << "error: --no-shell-config and --modify-shell are mutually exclusive\n";
        return EXIT_USAGE;
    }

    const std::filesystem::path home = sdkHome();
    bool valid = storeValid(home);
    bool changed = false;
    if (!checkOnly && !valid)
    {
        changed = ensureStore(home);
        valid = storeValid(home);
    }

    ShellConfigResult shellConfig;
    if (noShellConfig)
    {
        shellConfig.state = "skip";
    }
    else if (modifyShell)
    {
        if (checkOnly)
        {
            shellConfig.state = "check";
            shellConfig.profile = shellProfilePath();
        }
        else if (valid)
        {
            shellConfig = configureShellProfile(home);
        }
        else
        {
            shellConfig.state = "blocked";
        }
    }

    const bool success = valid && shellConfig.ok;
    const bool anyChanged = changed || shellConfig.changed;

    if (json)
    {
        std::cout
            << "{\"changed\":" << (anyChanged ? "true" : "false")
            << ",\"home\":\"" << jsonEscape(home.lexically_normal().string())
            << "\",\"launcher\":\"" << jsonEscape(launcherPath(home).lexically_normal().string())
            << "\",\"current_version\":\"" << jsonEscape(UIMD_VERSION)
            << "\",\"current_binary\":\"" << jsonEscape(sdkVersionBinary(home).lexically_normal().string())
            << "\",\"shell_config\":\""
            << (modifyShell ? "modify" : (noShellConfig ? "skip" : "unchanged"))
            << "\",\"shell_status\":\"" << jsonEscape(shellConfig.state)
            << "\",\"shell_changed\":" << (shellConfig.changed ? "true" : "false")
            << ",\"shell_profile\":\"" << jsonEscape(shellConfig.profile.lexically_normal().string())
            << "\",\"status\":\"" << (success ? "ok" : (valid ? "shell-error" : "incomplete"))
            << "\",\"version\":\"" << jsonEscape(UIMD_VERSION)
            << "\"}\n";
    }
    else
    {
        std::cout << "UIMD init\n";
        std::cout << "  home: " << home.lexically_normal().string() << "\n";
        std::cout << "  launcher: " << launcherPath(home).lexically_normal().string() << "\n";
        std::cout << "  current version: " << UIMD_VERSION << "\n";
        std::cout << "  current binary: " << sdkVersionBinary(home).lexically_normal().string() << "\n";
        std::cout << "  shell config: " << shellConfig.state << "\n";
        if (!shellConfig.profile.empty())
        {
            std::cout << "  shell profile: " << shellConfig.profile.lexically_normal().string() << "\n";
        }
        std::cout << "Status: " << (success ? "ok" : (valid ? "shell-error" : "incomplete")) << "\n";
        if (success)
        {
            std::cout << "\nNext steps:\n";
            std::cout << "  " << launcherPath(home).lexically_normal().string() << " doctor\n";
            if (modifyShell && shellConfig.changed)
            {
                std::cout << "  Open a new shell to use `uimd` from PATH.\n";
            }
            else if (modifyShell && shellConfig.state == "already-configured")
            {
                std::cout << "  `uimd` is already configured on PATH for new shells.\n";
            }
            else
            {
                std::cout << "  Use the launcher path above, or rerun with --modify-shell to add it to PATH.\n";
            }
        }
    }

    return success ? EXIT_OK : EXIT_ERROR;
}

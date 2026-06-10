#include <cstdlib>
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
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
const std::string RELEASE_CHECKSUMS_FILE{"checksums.txt"};

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
#if defined(__APPLE__) && defined(__x86_64__)
    return "macos-x86_64";
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
#ifdef _WIN32
    (void)command;
    return false;
#else
    return std::system(command.c_str()) == 0;
#endif
}

std::string readShellCommand(const std::string& command)
{
#ifdef _WIN32
    (void)command;
    return {};
#else
    std::array<char, 256> buffer{};
    std::string output;
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe == nullptr)
    {
        return {};
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr)
    {
        output += buffer.data();
    }
    const int status = pclose(pipe);
    if (status != 0)
    {
        return {};
    }
    return output;
#endif
}

bool downloadFile(const std::string& url, const std::filesystem::path& destination)
{
#ifdef _WIN32
    (void)url;
    (void)destination;
    std::cerr << "error: GitHub Release downloads are not implemented for Windows yet\n";
    return false;
#else
    std::error_code error;
    std::filesystem::create_directories(destination.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination.parent_path()) << ": " << error.message() << "\n";
        return false;
    }
    const std::string command = "curl -fsSL --retry 3 -o " +
        posixSingleQuote(pathString(destination)) + " " + posixSingleQuote(url);
    if (!runShellCommand(command))
    {
        std::cerr << "error: failed to download " << url << "\n";
        return false;
    }
    return std::filesystem::is_regular_file(destination);
#endif
}

std::string sha256File(const std::filesystem::path& path)
{
#ifdef _WIN32
    (void)path;
    return {};
#else
    const std::string output = readShellCommand("LC_ALL=C shasum -a 256 " + posixSingleQuote(pathString(path)));
    const std::vector<std::string> parts = splitWhitespace(output);
    if (parts.empty())
    {
        return {};
    }
    return lower(parts.front());
#endif
}

bool extractTarball(const std::filesystem::path& archive, const std::filesystem::path& destination)
{
#ifdef _WIN32
    (void)archive;
    (void)destination;
    std::cerr << "error: SDK archive extraction is not implemented for Windows yet\n";
    return false;
#else
    std::error_code error;
    std::filesystem::remove_all(destination, error);
    error.clear();
    std::filesystem::create_directories(destination, error);
    if (error)
    {
        std::cerr << "error: cannot create " << pathString(destination) << ": " << error.message() << "\n";
        return false;
    }
    const std::string command = "LC_ALL=C tar -xzf " + posixSingleQuote(pathString(archive)) +
        " -C " + posixSingleQuote(pathString(destination));
    if (!runShellCommand(command))
    {
        std::cerr << "error: failed to extract " << pathString(archive) << "\n";
        return false;
    }
    return true;
#endif
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
        std::cerr << "error: GitHub Release SDK downloads currently support only macOS Intel (x86_64)\n";
        return false;
    }

    const std::string asset = releaseAssetName();
    const std::filesystem::path workRoot = home / "tmp" / ("uimd-init-" + std::string{UIMD_VERSION});
    const std::filesystem::path checksumsPath = workRoot / RELEASE_CHECKSUMS_FILE;
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

    if (!downloadFile(releaseAssetUrl(RELEASE_CHECKSUMS_FILE), checksumsPath) ||
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
    }

    return success ? EXIT_OK : EXIT_ERROR;
}

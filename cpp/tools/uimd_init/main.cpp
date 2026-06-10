#include <cstdlib>
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

bool copyBinary(const std::filesystem::path& destination)
{
    const std::string source = envValue("UIMD_INIT_UIMD_BINARY");
    if (source.empty())
    {
        return std::filesystem::is_regular_file(destination);
    }

    std::error_code error;
    std::filesystem::create_directories(destination.parent_path(), error);
    if (error)
    {
        std::cerr << "error: cannot create " << destination.parent_path() << ": " << error.message() << "\n";
        return false;
    }
    if (std::filesystem::is_regular_file(destination))
    {
        const bool sameFile = std::filesystem::equivalent(source, destination, error);
        if (!error && sameFile)
        {
            return true;
        }
        error.clear();
    }
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing, error);
    if (error)
    {
        std::cerr << "error: cannot copy launcher: " << error.message() << "\n";
        return false;
    }
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
    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_BIN_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << (sdkVersionRoot(home) / SDK_BIN_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << (sdkVersionRoot(home) / SDK_TARGETS_DIR / SDK_PYTHON_TARGET_DIR) << ": " << error.message() << "\n";
        return false;
    }
    std::filesystem::create_directories(sdkVersionRoot(home) / SDK_EXAMPLES_DIR, error);
    if (error)
    {
        std::cerr << "error: cannot create " << (sdkVersionRoot(home) / SDK_EXAMPLES_DIR) << ": " << error.message() << "\n";
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

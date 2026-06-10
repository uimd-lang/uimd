#include "ui/elements/Image.hpp"

#include "lodepng.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <dlfcn.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace ui
{

namespace
{

constexpr int kImageCellPixelWidth = 8;
constexpr int kImageCellPixelHeight = 16;
constexpr int kFallbackVerticalSamplesPerCell = 2;
constexpr int kSixelBitsPerGlyph = 6;
constexpr int kSixelColorComponentScale = 100;
constexpr int kSixelColorLevels = 6;
constexpr int kSixelMaxColors = 256;
constexpr int kSixelFalseStatusMask = 0x1000;
constexpr int kSixelPixelFormatRgb888 = 0x03;
constexpr int kSixelLargeAuto = 0x0;
constexpr int kSixelRepAuto = 0x0;
constexpr int kSixelQualityHigh = 0x1;
constexpr int kExecutablePathInitialBufferSize = 4096;
constexpr int kTestFallbackCheckerTilePixels = 4;
constexpr int kTestFallbackCheckerLightAlpha = 160;
constexpr int kTestFallbackCheckerDarkAlpha = 0;
constexpr int kTestFallbackColorQuantum = 32;
constexpr char kFallbackUpperHalfBlock[] = "\xE2\x96\x80";
constexpr char kFallbackFullBlock[] = "\xE2\x96\x88";
constexpr char kMissingImagePlaceholder[] = "image";
constexpr char kSixelUnavailableMessage[] =
    "libsixel is required for non-fallback Image rendering. "
    "Install libsixel, or use render_mode: fallback.";
thread_local int gForceFallbackRenderingDepth = 0;
thread_local int gForceCellBackgroundRenderingDepth = 0;
std::optional<std::pair<int, int>> gTerminalCellPxOverride;

struct sixel_output_t;
struct sixel_dither_t;
using SixelStatus = int;
using SixelWriteFunction = int (*)(char*, int, void*);
using SixelOutputNewFunction = SixelStatus (*)(sixel_output_t**, SixelWriteFunction, void*, void*);
using SixelDitherNewFunction = SixelStatus (*)(sixel_dither_t**, int, void*);
using SixelDitherInitializeFunction = SixelStatus (*)(sixel_dither_t*, unsigned char*, int, int, int, int, int, int);
using SixelEncodeFunction = SixelStatus (*)(unsigned char*, int, int, int, sixel_dither_t*, sixel_output_t*);
using SixelOutputUnrefFunction = void (*)(sixel_output_t*);
using SixelDitherUnrefFunction = void (*)(sixel_dither_t*);

struct SixelApi
{
    SixelOutputNewFunction outputNew = nullptr;
    SixelDitherNewFunction ditherNew = nullptr;
    SixelDitherInitializeFunction ditherInitialize = nullptr;
    SixelEncodeFunction encode = nullptr;
    SixelOutputUnrefFunction outputUnref = nullptr;
    SixelDitherUnrefFunction ditherUnref = nullptr;
};

struct Rgb
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    friend bool operator<(const Rgb& lhs, const Rgb& rhs)
    {
        return std::tie(lhs.red, lhs.green, lhs.blue) < std::tie(rhs.red, rhs.green, rhs.blue);
    }

    friend bool operator==(const Rgb&, const Rgb&) = default;
};

constexpr Rgb kTestFallbackCheckerRgb{255, 255, 255};

struct Raster
{
    unsigned width = 0;
    unsigned height = 0;
    std::vector<Rgb> pixels;
    std::vector<unsigned char> alpha;
};

struct ImageRenderCacheKey
{
    std::string source;
    int width = 0;
    int height = 0;
    std::string fit;
    std::string align;
    std::string verticalAlign;
    Rgb background;
    int sourceHeight = 0;
    int cropTop = 0;

    friend bool operator<(const ImageRenderCacheKey& lhs, const ImageRenderCacheKey& rhs)
    {
        return std::tie(lhs.source, lhs.width, lhs.height, lhs.fit, lhs.align, lhs.verticalAlign,
                        lhs.background, lhs.sourceHeight, lhs.cropTop) <
               std::tie(rhs.source, rhs.width, rhs.height, rhs.fit, rhs.align, rhs.verticalAlign,
                        rhs.background, rhs.sourceHeight, rhs.cropTop);
    }
};

[[nodiscard]] std::string normalizedMode(std::string text, std::string fallback)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });
    return text.empty() ? fallback : text;
}

[[nodiscard]] bool truthyEnv(const char* name)
{
    const char* value = std::getenv(name);
    if (value == nullptr)
    {
        return false;
    }
    const std::string text = normalizedMode(value, "");
    return text == "1" || text == "true" || text == "yes" || text == "on";
}

[[nodiscard]] bool terminalSupportsSixel()
{
    if (truthyEnv("UIMD_FORCE_SIXEL"))
    {
        return true;
    }
    if (truthyEnv("UIMD_DISABLE_SIXEL"))
    {
        return false;
    }
    const std::string term = normalizedMode(std::getenv("TERM") == nullptr ? "" : std::getenv("TERM"), "");
    if (term.find("sixel") != std::string::npos)
    {
        return true;
    }
    return false;
}

[[nodiscard]] bool sixelDisabled()
{
    return truthyEnv("UIMD_DISABLE_SIXEL");
}

[[nodiscard]] bool deterministicImageFallbackEnabled()
{
    return truthyEnv("UIMD_DETERMINISTIC_IMAGE_FALLBACK");
}

[[nodiscard]] bool sixelStatusSucceeded(SixelStatus status)
{
    return (status & kSixelFalseStatusMask) == 0;
}

#if defined(_WIN32)
using DynamicLibraryHandle = HMODULE;
#elif defined(__unix__) || defined(__APPLE__)
using DynamicLibraryHandle = void*;
#else
using DynamicLibraryHandle = void*;
#endif

[[nodiscard]] std::vector<std::string> sixelLibraryNames()
{
#if defined(_WIN32)
    return {"libsixel.dll", "sixel.dll"};
#elif defined(__APPLE__)
    return {"libsixel.1.dylib", "libsixel.dylib"};
#else
    return {"libsixel.so.1", "libsixel.so"};
#endif
}

void appendSearchDirectory(std::vector<std::filesystem::path>& directories, std::filesystem::path directory)
{
    if (directory.empty())
    {
        return;
    }
    directory = directory.lexically_normal();
    const std::string text = directory.string();
    const auto found = std::find_if(directories.begin(), directories.end(), [&text](const std::filesystem::path& existing)
    {
        return existing.string() == text;
    });
    if (found == directories.end())
    {
        directories.push_back(std::move(directory));
    }
}

void appendEnvironmentSearchDirectories(std::vector<std::filesystem::path>& directories, const char* variable, char delimiter)
{
    const char* value = std::getenv(variable);
    if (value == nullptr || *value == '\0')
    {
        return;
    }
    std::stringstream stream(value);
    std::string item;
    while (std::getline(stream, item, delimiter))
    {
        appendSearchDirectory(directories, item);
    }
}

[[nodiscard]] std::filesystem::path executablePath()
{
#if defined(_WIN32)
    std::vector<char> buffer(kExecutablePathInitialBufferSize);
    const DWORD written = GetModuleFileNameA(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (written == 0 || written >= buffer.size())
    {
        return {};
    }
    return std::filesystem::path(std::string(buffer.data(), written));
#elif defined(__APPLE__)
    std::vector<char> buffer(kExecutablePathInitialBufferSize);
    uint32_t size = static_cast<uint32_t>(buffer.size());
    if (_NSGetExecutablePath(buffer.data(), &size) != 0)
    {
        buffer.resize(size);
        if (_NSGetExecutablePath(buffer.data(), &size) != 0)
        {
            return {};
        }
    }
    return std::filesystem::path(buffer.data());
#elif defined(__linux__)
    std::vector<char> buffer(kExecutablePathInitialBufferSize);
    const ssize_t written = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
    if (written <= 0)
    {
        return {};
    }
    return std::filesystem::path(std::string(buffer.data(), static_cast<std::size_t>(written)));
#else
    return {};
#endif
}

[[nodiscard]] std::vector<std::filesystem::path> sixelSearchDirectories()
{
    std::vector<std::filesystem::path> directories;

#if defined(_WIN32)
    appendEnvironmentSearchDirectories(directories, "PATH", ';');
    appendEnvironmentSearchDirectories(directories, "LIB", ';');
#else
    appendEnvironmentSearchDirectories(directories, "LD_LIBRARY_PATH", ':');
#if defined(__APPLE__)
    appendEnvironmentSearchDirectories(directories, "DYLD_LIBRARY_PATH", ':');
    appendEnvironmentSearchDirectories(directories, "DYLD_FALLBACK_LIBRARY_PATH", ':');
#endif
#endif

    if (const char* prefix = std::getenv("HOMEBREW_PREFIX"))
    {
        appendSearchDirectory(directories, std::filesystem::path(prefix) / "lib");
        appendSearchDirectory(directories, std::filesystem::path(prefix) / "opt" / "libsixel" / "lib");
    }
    if (const char* prefix = std::getenv("MACPORTS_PREFIX"))
    {
        appendSearchDirectory(directories, std::filesystem::path(prefix) / "lib");
    }

    const std::filesystem::path exe = executablePath();
    if (!exe.empty())
    {
        const std::filesystem::path dir = exe.parent_path();
        appendSearchDirectory(directories, dir);
        appendSearchDirectory(directories, dir / "lib");
        appendSearchDirectory(directories, dir / ".." / "lib");
        appendSearchDirectory(directories, dir / ".." / ".." / "lib");
    }

#if defined(__APPLE__)
    appendSearchDirectory(directories, "/opt/homebrew/opt/libsixel/lib");
    appendSearchDirectory(directories, "/opt/homebrew/lib");
    appendSearchDirectory(directories, "/usr/local/opt/libsixel/lib");
    appendSearchDirectory(directories, "/usr/local/lib");
    appendSearchDirectory(directories, "/opt/local/lib");
#elif defined(_WIN32)
    appendSearchDirectory(directories, "C:/Program Files/libsixel/bin");
    appendSearchDirectory(directories, "C:/Program Files/libsixel/lib");
    appendSearchDirectory(directories, "C:/Program Files (x86)/libsixel/bin");
    appendSearchDirectory(directories, "C:/Program Files (x86)/libsixel/lib");
#else
    appendSearchDirectory(directories, "/usr/local/lib");
    appendSearchDirectory(directories, "/usr/lib");
    appendSearchDirectory(directories, "/usr/lib64");
    appendSearchDirectory(directories, "/lib");
    appendSearchDirectory(directories, "/lib64");
    appendSearchDirectory(directories, "/usr/lib/x86_64-linux-gnu");
    appendSearchDirectory(directories, "/usr/lib/aarch64-linux-gnu");
    appendSearchDirectory(directories, "/usr/lib/arm-linux-gnueabihf");
#endif

    return directories;
}

#if defined(_WIN32)

[[nodiscard]] DynamicLibraryHandle openDynamicLibrary(const std::filesystem::path& path)
{
    return LoadLibraryA(path.string().c_str());
}

[[nodiscard]] void* loadSymbol(DynamicLibraryHandle handle, const char* name)
{
    return reinterpret_cast<void*>(GetProcAddress(handle, name));
}
#elif defined(__unix__) || defined(__APPLE__)

[[nodiscard]] DynamicLibraryHandle openDynamicLibrary(const std::filesystem::path& path)
{
    return dlopen(path.string().c_str(), RTLD_LAZY | RTLD_LOCAL);
}

[[nodiscard]] void* loadSymbol(DynamicLibraryHandle handle, const char* name)
{
    return dlsym(handle, name);
}
#else

[[nodiscard]] DynamicLibraryHandle openDynamicLibrary(const std::filesystem::path&)
{
    return nullptr;
}

[[nodiscard]] void* loadSymbol(DynamicLibraryHandle, const char*)
{
    return nullptr;
}
#endif

[[nodiscard]] char libraryPathDelimiter()
{
#if defined(_WIN32)
    return ';';
#else
    return ':';
#endif
}

[[nodiscard]] DynamicLibraryHandle openConfiguredSixelLibrary(const std::vector<std::string>& names)
{
    if (const char* path = std::getenv("UIMD_LIBSIXEL_PATH"))
    {
        if (DynamicLibraryHandle handle = openDynamicLibrary(path))
        {
            return handle;
        }
    }

    std::vector<std::filesystem::path> directories;
    appendEnvironmentSearchDirectories(directories, "UIMD_LIBSIXEL_DIR", libraryPathDelimiter());
    for (const std::filesystem::path& directory : directories)
    {
        for (const std::string& name : names)
        {
            if (DynamicLibraryHandle handle = openDynamicLibrary(directory / name))
            {
                return handle;
            }
        }
    }

    return nullptr;
}

[[nodiscard]] DynamicLibraryHandle openSixelLibrary()
{
    const std::vector<std::string> names = sixelLibraryNames();
    for (const std::string& name : names)
    {
        if (DynamicLibraryHandle handle = openDynamicLibrary(name))
        {
            return handle;
        }
    }
    for (const std::filesystem::path& directory : sixelSearchDirectories())
    {
        for (const std::string& name : names)
        {
            if (DynamicLibraryHandle handle = openDynamicLibrary(directory / name))
            {
                return handle;
            }
        }
    }
    return openConfiguredSixelLibrary(names);
}

[[nodiscard]] SixelApi* loadSixelApi()
{
    static SixelApi loadedApi;
    static SixelApi* cachedApi = []
    {
        DynamicLibraryHandle handle = openSixelLibrary();
        if (handle == nullptr)
        {
            return static_cast<SixelApi*>(nullptr);
        }

        loadedApi.outputNew = reinterpret_cast<SixelOutputNewFunction>(loadSymbol(handle, "sixel_output_new"));
        loadedApi.ditherNew = reinterpret_cast<SixelDitherNewFunction>(loadSymbol(handle, "sixel_dither_new"));
        loadedApi.ditherInitialize = reinterpret_cast<SixelDitherInitializeFunction>(loadSymbol(handle, "sixel_dither_initialize"));
        loadedApi.encode = reinterpret_cast<SixelEncodeFunction>(loadSymbol(handle, "sixel_encode"));
        loadedApi.outputUnref = reinterpret_cast<SixelOutputUnrefFunction>(loadSymbol(handle, "sixel_output_unref"));
        loadedApi.ditherUnref = reinterpret_cast<SixelDitherUnrefFunction>(loadSymbol(handle, "sixel_dither_unref"));
        const bool available = loadedApi.outputNew != nullptr && loadedApi.ditherNew != nullptr &&
                               loadedApi.ditherInitialize != nullptr && loadedApi.encode != nullptr &&
                               loadedApi.outputUnref != nullptr && loadedApi.ditherUnref != nullptr;
        return available ? &loadedApi : nullptr;
    }();
    return cachedApi;
}

[[nodiscard]] const Raster& loadRaster(const std::string& path)
{
    static std::map<std::string, Raster> cache;
    const auto cached = cache.find(path);
    if (cached != cache.end())
    {
        return cached->second;
    }

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* decoded = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (decoded == nullptr || width <= 0 || height <= 0)
    {
        if (decoded != nullptr)
        {
            stbi_image_free(decoded);
        }
        return cache.try_emplace(path, Raster{}).first->second;
    }

    Raster raster;
    raster.width = static_cast<unsigned>(width);
    raster.height = static_cast<unsigned>(height);
    const std::size_t pixelCount = static_cast<std::size_t>(raster.width) * static_cast<std::size_t>(raster.height);
    raster.pixels.reserve(pixelCount);
    raster.alpha.reserve(pixelCount);
    for (std::size_t offset = 0; offset + 3 < pixelCount * 4; offset += 4)
    {
        raster.pixels.push_back(Rgb{decoded[offset], decoded[offset + 1], decoded[offset + 2]});
        raster.alpha.push_back(decoded[offset + 3]);
    }
    stbi_image_free(decoded);
    return cache.emplace(path, std::move(raster)).first->second;
}

[[nodiscard]] unsigned char pixelAlphaAt(const Raster& raster, int x, int y)
{
    if (raster.alpha.empty())
    {
        return 255;
    }
    x = std::max(0, std::min(x, static_cast<int>(raster.width) - 1));
    y = std::max(0, std::min(y, static_cast<int>(raster.height) - 1));
    return raster.alpha[static_cast<std::size_t>(y) * raster.width + static_cast<std::size_t>(x)];
}

[[nodiscard]] Rgb blendWithBackground(Rgb pixel, unsigned char alpha, Rgb background)
{
    if (alpha == 255)
    {
        return pixel;
    }
    if (alpha == 0)
    {
        return background;
    }
    const auto blend = [](unsigned char src, unsigned char dst, unsigned char a) -> unsigned char
    {
        return static_cast<unsigned char>((static_cast<unsigned>(src) * a + static_cast<unsigned>(dst) * (255U - a)) / 255U);
    };
    return {blend(pixel.red, background.red, alpha),
            blend(pixel.green, background.green, alpha),
            blend(pixel.blue, background.blue, alpha)};
}

[[nodiscard]] Rgb pixelAt(const Raster& raster, int x, int y)
{
    if (raster.width == 0 || raster.height == 0 || raster.pixels.empty())
    {
        return {};
    }
    x = std::max(0, std::min(x, static_cast<int>(raster.width) - 1));
    y = std::max(0, std::min(y, static_cast<int>(raster.height) - 1));
    return raster.pixels[static_cast<std::size_t>(y) * raster.width + static_cast<std::size_t>(x)];
}

[[nodiscard]] double alignmentOffset(double outer, double inner, const std::string& value,
                                     const std::string& startValue, const std::string& endValue)
{
    const double span = std::max(0.0, outer - inner);
    if (value == startValue)
    {
        return 0.0;
    }
    if (value == endValue)
    {
        return span;
    }
    return span / 2.0;
}

[[nodiscard]] Raster resizeRaster(const Raster& source, int targetWidth, int targetHeight, const std::string& fit,
                                  const std::string& align, const std::string& verticalAlign, Rgb background = {})
{
    targetWidth = std::max(1, targetWidth);
    targetHeight = std::max(1, targetHeight);
    if (source.width == 0 || source.height == 0)
    {
        return {};
    }

    Raster result;
    result.width = static_cast<unsigned>(targetWidth);
    result.height = static_cast<unsigned>(targetHeight);
    const std::size_t pixelCount = static_cast<std::size_t>(targetWidth) * static_cast<std::size_t>(targetHeight);
    result.pixels.assign(pixelCount, background);
    result.alpha.assign(pixelCount, 255);

    const bool stretch = fit == "stretch";
    const double scale = stretch ? 0.0 : (
        fit == "cover"
            ? std::max(static_cast<double>(targetWidth) / source.width, static_cast<double>(targetHeight) / source.height)
            : std::min(static_cast<double>(targetWidth) / source.width, static_cast<double>(targetHeight) / source.height)
    );
    const double drawnWidth = stretch ? targetWidth : std::max(1.0, source.width * scale);
    const double drawnHeight = stretch ? targetHeight : std::max(1.0, source.height * scale);
    const double xOffset = stretch ? 0.0 : alignmentOffset(targetWidth, drawnWidth, align, "left", "right");
    const double yOffset = stretch ? 0.0 : alignmentOffset(targetHeight, drawnHeight, verticalAlign, "top", "bottom");

    for (int y = 0; y < targetHeight; ++y)
    {
        for (int x = 0; x < targetWidth; ++x)
        {
            const double sourceX = stretch ? (static_cast<double>(x) * source.width / targetWidth) : ((x - xOffset) / scale);
            const double sourceY = stretch ? (static_cast<double>(y) * source.height / targetHeight) : ((y - yOffset) / scale);
            if (sourceX < 0.0 || sourceY < 0.0 ||
                sourceX >= static_cast<double>(source.width) || sourceY >= static_cast<double>(source.height))
            {
                continue;
            }
            const int sx = static_cast<int>(std::floor(sourceX));
            const int sy = static_cast<int>(std::floor(sourceY));
            const std::size_t dstIdx = static_cast<std::size_t>(y) * targetWidth + static_cast<std::size_t>(x);
            result.pixels[dstIdx] = blendWithBackground(pixelAt(source, sx, sy), pixelAlphaAt(source, sx, sy), background);
            result.alpha[dstIdx] = 255;
        }
    }
    return result;
}

[[nodiscard]] Color colorFromRgb(Rgb color)
{
    return Color::rgb(color.red, color.green, color.blue);
}

[[nodiscard]] Raster cropRasterRows(const Raster& source, int top, int height)
{
    top = std::max(0, top);
    height = std::max(1, height);
    if (source.width == 0 || source.height == 0 || (top <= 0 && height >= static_cast<int>(source.height)))
    {
        return source;
    }
    const int bottom = std::min(static_cast<int>(source.height), top + height);
    if (bottom <= top)
    {
        return {};
    }
    Raster cropped;
    cropped.width = source.width;
    cropped.height = static_cast<unsigned>(bottom - top);
    const std::size_t rowWidth = static_cast<std::size_t>(source.width);
    cropped.pixels.reserve(rowWidth * static_cast<std::size_t>(cropped.height));
    cropped.alpha.reserve(rowWidth * static_cast<std::size_t>(cropped.height));
    for (int row = top; row < bottom; ++row)
    {
        const std::size_t offset = static_cast<std::size_t>(row) * rowWidth;
        cropped.pixels.insert(cropped.pixels.end(), source.pixels.begin() + static_cast<std::ptrdiff_t>(offset),
                              source.pixels.begin() + static_cast<std::ptrdiff_t>(offset + rowWidth));
        cropped.alpha.insert(cropped.alpha.end(), source.alpha.begin() + static_cast<std::ptrdiff_t>(offset),
                             source.alpha.begin() + static_cast<std::ptrdiff_t>(offset + rowWidth));
    }
    return cropped;
}

[[nodiscard]] RenderedContent blankContent(int width, int height, const Style& style)
{
    RenderedContent content;
    content.reserve(static_cast<std::size_t>(height));
    for (int row = 0; row < height; ++row)
    {
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(width));
        for (int col = 0; col < width; ++col)
        {
            renderedRow.push_back(TerminalCell{.text = " ", .foreground = style.color, .background = style.background});
        }
        content.push_back(std::move(renderedRow));
    }
    return content;
}

[[nodiscard]] RenderedContent placeholderContent(int width, int height, const Style& style, const std::string& text)
{
    RenderedContent content = blankContent(width, height, style);
    if (content.empty())
    {
        return content;
    }
    const int row = height / 2;
    const int startCol = std::max(0, (width - static_cast<int>(text.size())) / 2);
    for (int index = 0; index < static_cast<int>(text.size()) && startCol + index < width; ++index)
    {
        content[static_cast<std::size_t>(row)][static_cast<std::size_t>(startCol + index)] =
            TerminalCell{.text = text.substr(static_cast<std::size_t>(index), 1),
                         .foreground = style.color,
                         .background = style.background};
    }
    return content;
}

[[nodiscard]] RenderedContent fallbackContent(const Raster& raster, int width, int height)
{
    if (raster.width == 0 || raster.height == 0)
    {
        return {};
    }
    RenderedContent content;
    content.reserve(static_cast<std::size_t>(height));
    for (int row = 0; row < height; ++row)
    {
        RenderedRow renderedRow;
        renderedRow.reserve(static_cast<std::size_t>(width));
        const int topY = row * kFallbackVerticalSamplesPerCell;
        const int bottomY = std::min(static_cast<int>(raster.height) - 1, topY + 1);
        for (int col = 0; col < width; ++col)
        {
            renderedRow.push_back(TerminalCell{
                .text = kFallbackUpperHalfBlock,
                .foreground = colorFromRgb(pixelAt(raster, col, topY)),
                .background = colorFromRgb(pixelAt(raster, col, bottomY)),
            });
        }
        content.push_back(std::move(renderedRow));
    }
    return content;
}

[[maybe_unused]] [[nodiscard]] unsigned char quantizeChannel(unsigned char value)
{
    const int index = static_cast<int>(std::lround(value * (kSixelColorLevels - 1) / 255.0));
    return static_cast<unsigned char>(std::max(0, std::min(255, index * 255 / (kSixelColorLevels - 1))));
}

[[maybe_unused]] [[nodiscard]] Raster quantizeRaster(Raster raster)
{
    for (Rgb& color : raster.pixels)
    {
        color.red = quantizeChannel(color.red);
        color.green = quantizeChannel(color.green);
        color.blue = quantizeChannel(color.blue);
    }
    return raster;
}

[[maybe_unused]] [[nodiscard]] int sixelComponent(unsigned char value)
{
    return std::max(0, std::min(kSixelColorComponentScale,
                                static_cast<int>(std::lround(value * kSixelColorComponentScale / 255.0))));
}

[[maybe_unused]] void appendSixelRun(std::string& output, char ch, int count)
{
    if (count <= 0)
    {
        return;
    }
    if (count >= 4)
    {
        output += "!" + std::to_string(count) + ch;
    }
    else
    {
        output.append(static_cast<std::size_t>(count), ch);
    }
}

[[nodiscard]] std::pair<int, int> terminalCellPx()
{
    if (gTerminalCellPxOverride.has_value())
    {
        return *gTerminalCellPxOverride;
    }
    static std::pair<int, int> cached{0, 0};
    static bool queried = false;
    if (queried)
    {
        return cached;
    }
    queried = true;
#if defined(__unix__) || defined(__APPLE__)
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 &&
        ws.ws_row > 0 && ws.ws_col > 0 && ws.ws_xpixel > 0 && ws.ws_ypixel > 0)
    {
        cached = {ws.ws_xpixel / ws.ws_col, ws.ws_ypixel / ws.ws_row};
    }
#endif
    return cached;
}

struct CellRegion
{
    int cols = 0;
    int rows = 0;
    int colOffset = 0;
    int rowOffset = 0;
};

// For "contain", the sixel is generated only over the cells the image actually
// occupies; the surrounding letterbox cells stay transparent so the window
// background shows through. "cover"/"stretch" fill the whole element.
[[nodiscard]] CellRegion imageCellRegion(int width, int height, unsigned srcWidth, unsigned srcHeight,
                                         const std::string& fit, const std::string& align,
                                         const std::string& verticalAlign)
{
    if (fit != "contain" || srcWidth == 0 || srcHeight == 0)
    {
        return {width, height, 0, 0};
    }
    const auto [cellW, cellH] = terminalCellPx();
    const int pxW = cellW > 0 ? cellW : kImageCellPixelWidth;
    const int pxH = cellH > 0 ? cellH : kImageCellPixelHeight;
    const double canvasWidth = static_cast<double>(width) * pxW;
    const double canvasHeight = static_cast<double>(height) * pxH;
    const double scale = std::min(canvasWidth / srcWidth, canvasHeight / srcHeight);
    const int cols = std::clamp(static_cast<int>(std::lround(srcWidth * scale / pxW)), 1, width);
    const int rows = std::clamp(static_cast<int>(std::lround(srcHeight * scale / pxH)), 1, height);
    const int colOffset = static_cast<int>(alignmentOffset(width, cols, align, "left", "right"));
    const int rowOffset = static_cast<int>(alignmentOffset(height, rows, verticalAlign, "top", "bottom"));
    return {cols, rows, colOffset, rowOffset};
}

[[nodiscard]] RenderedContent fallbackImageContent(const Raster& sourceRaster, int width, int height,
                                                   const std::string& fit, const std::string& align,
                                                   const std::string& verticalAlign, const Style& style,
                                                   Rgb background)
{
    RenderedContent content = blankContent(width, height, style);
    const CellRegion region = imageCellRegion(width, height, sourceRaster.width, sourceRaster.height,
                                              fit, align, verticalAlign);
    const std::string regionFit = fit == "contain" ? std::string{"cover"} : fit;
    RenderedContent image = fallbackContent(
        resizeRaster(sourceRaster, region.cols, region.rows * kFallbackVerticalSamplesPerCell,
                     regionFit, align, verticalAlign, background),
        region.cols,
        region.rows);
    if (image.empty())
    {
        return image;
    }
    for (int row = 0; row < region.rows; ++row)
    {
        const int targetRow = region.rowOffset + row;
        if (targetRow < 0 || targetRow >= height)
        {
            continue;
        }
        for (int col = 0; col < region.cols; ++col)
        {
            const int targetCol = region.colOffset + col;
            if (targetCol < 0 || targetCol >= width)
            {
                continue;
            }
            content[static_cast<std::size_t>(targetRow)][static_cast<std::size_t>(targetCol)] =
                image[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)];
        }
    }
    return content;
}

[[nodiscard]] RenderedContent cellBackgroundImageContent(const Raster& sourceRaster, int width, int height,
                                                         const std::string& fit, const std::string& align,
                                                         const std::string& verticalAlign, const Style& style,
                                                         Rgb background)
{
    RenderedContent content = blankContent(width, height, style);
    const CellRegion region = imageCellRegion(width, height, sourceRaster.width, sourceRaster.height,
                                              fit, align, verticalAlign);
    const std::string regionFit = fit == "contain" ? std::string{"cover"} : fit;
    const Raster image = resizeRaster(sourceRaster, region.cols, region.rows, regionFit, align, verticalAlign,
                                      background);
    if (image.width == 0 || image.height == 0)
    {
        return content;
    }
    for (int row = 0; row < region.rows; ++row)
    {
        const int targetRow = region.rowOffset + row;
        if (targetRow < 0 || targetRow >= height)
        {
            continue;
        }
        for (int col = 0; col < region.cols; ++col)
        {
            const int targetCol = region.colOffset + col;
            if (targetCol < 0 || targetCol >= width)
            {
                continue;
            }
            const Color color = colorFromRgb(pixelAt(image, col, row));
            content[static_cast<std::size_t>(targetRow)][static_cast<std::size_t>(targetCol)] =
                TerminalCell{.text = kFallbackFullBlock, .foreground = color, .background = color};
        }
    }
    return content;
}

[[nodiscard]] Rgb quantizeTestFallbackColor(Rgb color)
{
    const auto quantize = [](unsigned char channel) -> unsigned char
    {
        return static_cast<unsigned char>((static_cast<int>(channel) / kTestFallbackColorQuantum) * kTestFallbackColorQuantum);
    };
    return {quantize(color.red), quantize(color.green), quantize(color.blue)};
}

[[nodiscard]] bool testFallbackCheckerLightSquare(int x, int y)
{
    return ((x / kTestFallbackCheckerTilePixels) + (y / kTestFallbackCheckerTilePixels)) % 2 == 0;
}

[[nodiscard]] Rgb testFallbackCheckerTint(Rgb color, int x, int y)
{
    const unsigned char alpha = testFallbackCheckerLightSquare(x, y)
        ? static_cast<unsigned char>(kTestFallbackCheckerLightAlpha)
        : static_cast<unsigned char>(kTestFallbackCheckerDarkAlpha);
    return blendWithBackground(kTestFallbackCheckerRgb, alpha, color);
}

[[nodiscard]] Rgb testFallbackSampleColor(const Raster& source, int x, int y, int targetWidth, int targetHeight,
                                          const std::string& fit, const std::string& align,
                                          const std::string& verticalAlign, Rgb background, bool checker)
{
    targetWidth = std::max(1, targetWidth);
    targetHeight = std::max(1, targetHeight);
    x = std::clamp(x, 0, targetWidth - 1);
    y = std::clamp(y, 0, targetHeight - 1);
    const Rgb base = checker ? testFallbackCheckerTint(background, x, y) : background;
    if (source.width == 0 || source.height == 0 || source.pixels.empty())
    {
        return quantizeTestFallbackColor(base);
    }

    int sourceX = 0;
    int sourceY = 0;
    if (fit == "stretch")
    {
        sourceX = static_cast<int>(static_cast<double>(x) * source.width / targetWidth);
        sourceY = static_cast<int>(static_cast<double>(y) * source.height / targetHeight);
    }
    else
    {
        const double scale = fit == "cover"
            ? std::max(static_cast<double>(targetWidth) / source.width, static_cast<double>(targetHeight) / source.height)
            : std::min(static_cast<double>(targetWidth) / source.width, static_cast<double>(targetHeight) / source.height);
        const double drawnWidth = source.width * scale;
        const double drawnHeight = source.height * scale;
        const double xOffset = alignmentOffset(targetWidth, drawnWidth, align, "left", "right");
        const double yOffset = alignmentOffset(targetHeight, drawnHeight, verticalAlign, "top", "bottom");
        const double sourceXFloat = (x - xOffset) / scale;
        const double sourceYFloat = (y - yOffset) / scale;
        if (sourceXFloat < 0.0 || sourceYFloat < 0.0 ||
            sourceXFloat >= static_cast<double>(source.width) ||
            sourceYFloat >= static_cast<double>(source.height))
        {
            return quantizeTestFallbackColor(base);
        }
        sourceX = static_cast<int>(sourceXFloat);
        sourceY = static_cast<int>(sourceYFloat);
    }

    sourceX = std::clamp(sourceX, 0, static_cast<int>(source.width) - 1);
    sourceY = std::clamp(sourceY, 0, static_cast<int>(source.height) - 1);
    Rgb pixel = blendWithBackground(pixelAt(source, sourceX, sourceY), pixelAlphaAt(source, sourceX, sourceY), base);
    if (checker)
    {
        pixel = testFallbackCheckerTint(pixel, x, y);
    }
    return quantizeTestFallbackColor(pixel);
}

[[nodiscard]] RenderedContent deterministicImageContent(const std::string& source, const Raster& sourceRaster,
                                                        int width, int height, const std::string& fit,
                                                        const std::string& align, const std::string& verticalAlign,
                                                        const Style& style, Rgb background, bool checker)
{
    (void)source;
    RenderedContent content = blankContent(width, height, style);
    const CellRegion region = imageCellRegion(width, height, sourceRaster.width, sourceRaster.height,
                                              fit, align, verticalAlign);
    const std::string regionFit = fit == "contain" ? std::string{"cover"} : fit;
    const int sampleHeight = region.rows * kFallbackVerticalSamplesPerCell;
    for (int row = region.rowOffset; row < region.rowOffset + region.rows; ++row)
    {
        for (int col = region.colOffset; col < region.colOffset + region.cols; ++col)
        {
            const int sampleCol = col - region.colOffset;
            const int sampleTop = (row - region.rowOffset) * kFallbackVerticalSamplesPerCell;
            const Rgb top = testFallbackSampleColor(
                sourceRaster, sampleCol, sampleTop, region.cols, sampleHeight, regionFit, align, verticalAlign,
                background, checker);
            const Rgb bottom = testFallbackSampleColor(
                sourceRaster, sampleCol, sampleTop + 1, region.cols, sampleHeight, regionFit, align, verticalAlign,
                background, checker);
            content[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)] = TerminalCell{
                .text = kFallbackUpperHalfBlock,
                .foreground = colorFromRgb(top),
                .background = colorFromRgb(bottom),
            };
        }
    }
    return content;
}

[[maybe_unused]] [[nodiscard]] std::string sixelPayload(const Raster& raster)
{
    if (raster.width == 0 || raster.height == 0)
    {
        return {};
    }
    std::map<Rgb, int> colors;
    for (std::size_t i = 0; i < raster.pixels.size(); ++i)
    {
        if (!raster.alpha.empty() && raster.alpha[i] == 0)
        {
            continue;
        }
        const Rgb& color = raster.pixels[i];
        if (colors.find(color) == colors.end())
        {
            colors[color] = static_cast<int>(colors.size());
        }
    }

    std::string output = "\x1bPq";
    for (const auto& [color, index] : colors)
    {
        output += "#" + std::to_string(index) + ";2;" +
                  std::to_string(sixelComponent(color.red)) + ";" +
                  std::to_string(sixelComponent(color.green)) + ";" +
                  std::to_string(sixelComponent(color.blue));
    }
    for (unsigned y = 0; y < raster.height; y += kSixelBitsPerGlyph)
    {
        for (const auto& [color, index] : colors)
        {
            std::string run;
            bool hasPixels = false;
            char previous = '\0';
            int count = 0;
            for (unsigned x = 0; x < raster.width; ++x)
            {
                int bits = 0;
                for (int bit = 0; bit < kSixelBitsPerGlyph; ++bit)
                {
                    const unsigned py = y + static_cast<unsigned>(bit);
                    if (py < raster.height &&
                        (raster.alpha.empty() || raster.alpha[py * raster.width + x] > 0) &&
                        pixelAt(raster, static_cast<int>(x), static_cast<int>(py)) == color)
                    {
                        bits |= 1 << bit;
                    }
                }
                if (bits != 0)
                {
                    hasPixels = true;
                }
                const char ch = static_cast<char>(63 + bits);
                if (ch == previous)
                {
                    ++count;
                }
                else
                {
                    appendSixelRun(run, previous, count);
                    previous = ch;
                    count = 1;
                }
            }
            if (!hasPixels)
            {
                continue;
            }
            appendSixelRun(run, previous, count);
            output += "#" + std::to_string(index);
            output += run;
            output += "$";
        }
        output += "-";
    }
    output += "\x1b\\";
    return output;
}

int libsixelWrite(char* data, int size, void* priv)
{
    auto* out = static_cast<std::string*>(priv);
    out->append(data, static_cast<std::size_t>(size));
    return size;
}

// Optimized, higher-quality encoder via libsixel (adaptive palette + dithering),
// replacing the naive in-house O(colors x pixels) loop.
[[nodiscard]] std::string libsixelEncode(const Raster& raster)
{
    if (raster.width == 0 || raster.height == 0)
    {
        return {};
    }
    SixelApi* api = loadSixelApi();
    if (api == nullptr)
    {
        return {};
    }
    std::vector<unsigned char> rgb;
    rgb.reserve(raster.pixels.size() * 3);
    for (const Rgb& pixel : raster.pixels)
    {
        rgb.push_back(pixel.red);
        rgb.push_back(pixel.green);
        rgb.push_back(pixel.blue);
    }
    std::string out;
    sixel_output_t* output = nullptr;
    sixel_dither_t* dither = nullptr;
    if (!sixelStatusSucceeded(api->outputNew(&output, libsixelWrite, &out, nullptr)))
    {
        return {};
    }
    if (!sixelStatusSucceeded(api->ditherNew(&dither, kSixelMaxColors, nullptr)))
    {
        api->outputUnref(output);
        return {};
    }
    const int width = static_cast<int>(raster.width);
    const int height = static_cast<int>(raster.height);
    const SixelStatus status = api->ditherInitialize(
        dither, rgb.data(), width, height, kSixelPixelFormatRgb888,
        kSixelLargeAuto, kSixelRepAuto, kSixelQualityHigh);
    if (sixelStatusSucceeded(status))
    {
        api->encode(rgb.data(), width, height, 3, dither, output);
    }
    api->ditherUnref(dither);
    api->outputUnref(output);
    return out;
}

[[nodiscard]] std::string cachedSixelPayload(const std::string& source, const Raster& sourceRaster,
                                             int width, int height, const std::string& fit,
                                             const std::string& align, const std::string& verticalAlign,
                                             Rgb background, int sourceHeight = 0, int cropTop = 0)
{
    static std::map<ImageRenderCacheKey, std::string> cache;
    sourceHeight = sourceHeight > 0 ? sourceHeight : height;
    cropTop = std::max(0, cropTop);
    const ImageRenderCacheKey key{source, width, height, fit, align, verticalAlign, background, sourceHeight, cropTop};
    const auto cached = cache.find(key);
    if (cached != cache.end())
    {
        return cached->second;
    }
    const auto [cellW, cellH] = terminalCellPx();
    const int pixelW = cellW > 0 ? cellW : kImageCellPixelWidth;
    const int pixelH = cellH > 0 ? cellH : kImageCellPixelHeight;
    const Raster fitted = resizeRaster(sourceRaster, width * pixelW, sourceHeight * pixelH, fit, align, verticalAlign, background);
    const Raster payloadRaster = cropRasterRows(fitted, cropTop * pixelH, height * pixelH);
    std::string raw = libsixelEncode(payloadRaster);
    if (raw.empty())
    {
        raw = sixelPayload(quantizeRaster(payloadRaster));
    }
    cache[key] = raw;
    return raw;
}

}  // namespace

ScopedImageFallbackRendering::ScopedImageFallbackRendering()
{
    ++gForceFallbackRenderingDepth;
}

ScopedImageFallbackRendering::~ScopedImageFallbackRendering()
{
    gForceFallbackRenderingDepth = std::max(0, gForceFallbackRenderingDepth - 1);
}

ScopedImageCellBackgroundRendering::ScopedImageCellBackgroundRendering()
{
    ++gForceCellBackgroundRenderingDepth;
}

ScopedImageCellBackgroundRendering::~ScopedImageCellBackgroundRendering()
{
    gForceCellBackgroundRenderingDepth = std::max(0, gForceCellBackgroundRenderingDepth - 1);
}

void requireSixelForImageRendering()
{
    if (deterministicImageFallbackEnabled() || sixelDisabled())
    {
        return;
    }
    if (loadSixelApi() == nullptr)
    {
        throw std::runtime_error(kSixelUnavailableMessage);
    }
}

void setImageTerminalCellPixels(Size size)
{
    if (size.width <= 0 || size.height <= 0)
    {
        return;
    }
    gTerminalCellPxOverride = {size.width, size.height};
}

Image::Image(std::string name, std::string source, std::string alt, std::string fit, std::string renderMode,
             std::string align, std::string verticalAlign)
    : Element(std::move(name)),
      source_(std::move(source)),
      alt_(std::move(alt)),
      fit_(normalizedMode(std::move(fit), kDefaultImageFit)),
      renderMode_(normalizedMode(std::move(renderMode), kDefaultImageRenderMode)),
      align_(normalizedMode(std::move(align), kDefaultImageAlign)),
      verticalAlign_(normalizedMode(std::move(verticalAlign), kDefaultImageVerticalAlign))
{
}

void Image::setSource(std::string source)
{
    source_ = std::move(source);
}

void Image::setAlt(std::string alt)
{
    alt_ = std::move(alt);
}

void Image::setFit(std::string fit)
{
    fit_ = normalizedMode(std::move(fit), kDefaultImageFit);
}

void Image::setRenderMode(std::string renderMode)
{
    renderMode_ = normalizedMode(std::move(renderMode), kDefaultImageRenderMode);
}

void Image::setAlign(std::string align)
{
    align_ = normalizedMode(std::move(align), kDefaultImageAlign);
}

void Image::setVerticalAlign(std::string verticalAlign)
{
    verticalAlign_ = normalizedMode(std::move(verticalAlign), kDefaultImageVerticalAlign);
}

RenderedContent Image::render(Size size, ElementRenderState state) const
{
    const int width = std::max(1, size.width);
    const int height = std::max(1, size.height);
    const Style style = effectiveStyle(state.focused, state.editMode);
    const bool forceFallback = gForceFallbackRenderingDepth > 0;
    const bool forceCellBackground = gForceCellBackgroundRenderingDepth > 0;
    const bool sixel = !forceFallback && !forceCellBackground && (
        (renderMode_ == "sixel" && !sixelDisabled()) ||
        (renderMode_ == "auto" && terminalSupportsSixel()));
    if (sixel)
    {
        requireSixelForImageRendering();
    }
    const Raster& sourceRaster = loadRaster(source_);
    if (sourceRaster.width == 0 || sourceRaster.height == 0)
    {
        const std::string text = alt_.empty() ? std::filesystem::path(source_).filename().string() : alt_;
        return placeholderContent(width, height, style, text.empty() ? std::string{kMissingImagePlaceholder} : text);
    }

    Rgb background;
    bool backgroundOpaque = false;
    if (style.background.has_value() && style.background->rgba().has_value())
    {
        const auto& rgba = *style.background->rgba();
        background = {rgba.red, rgba.green, rgba.blue};
        backgroundOpaque = rgba.alpha == 255;
    }

    // Fallback letterbox fill: when the image background is transparent, match the
    // Sixel path (which leaves the surrounding cells transparent and shows the
    // panel behind) by filling with the background composited behind this element.
    Rgb fallbackBackground = background;
    if (!backgroundOpaque)
    {
        const std::optional<Color> parentBackground = Element::renderingParentBackground();
        if (parentBackground.has_value() && parentBackground->rgba().has_value())
        {
            const auto& rgba = *parentBackground->rgba();
            fallbackBackground = {rgba.red, rgba.green, rgba.blue};
        }
    }

    if (forceCellBackground)
    {
        return cellBackgroundImageContent(sourceRaster, width, height, fit_, align_, verticalAlign_,
                                          style, fallbackBackground);
    }

    if (forceFallback)
    {
        return fallbackImageContent(sourceRaster, width, height, fit_, align_, verticalAlign_,
                                    style, fallbackBackground);
    }

    if (deterministicImageFallbackEnabled())
    {
        return deterministicImageContent(source_, sourceRaster, width, height, fit_, align_, verticalAlign_,
                                         style, fallbackBackground, renderMode_ != "fallback");
    }

    if (sixel)
    {
        const CellRegion region = imageCellRegion(width, height, sourceRaster.width, sourceRaster.height,
                                                  fit_, align_, verticalAlign_);
        int visibleTop = region.rowOffset;
        int visibleBottom = region.rowOffset + region.rows;
        if (state.clipTop.has_value() || state.clipBottom.has_value())
        {
            visibleTop = std::max(visibleTop, state.clipTop.value_or(0));
            visibleBottom = std::min(visibleBottom, state.clipBottom.value_or(height));
        }
        const int visibleRows = std::max(0, visibleBottom - visibleTop);
        // Inside its own cell region the image fills the area; letterbox lives in
        // the surrounding transparent cells, not baked into the sixel.
        const std::string regionFit = (fit_ == "contain") ? std::string("cover") : fit_;
        const std::string raw = visibleRows > 0
            ? cachedSixelPayload(source_, sourceRaster, region.cols, visibleRows,
                                 regionFit, align_, verticalAlign_, background,
                                 region.rows, visibleTop - region.rowOffset)
            : std::string{};
        if (!raw.empty())
        {
            RenderedContent content = blankContent(width, height, style);
            for (int r = visibleTop; r < visibleBottom; ++r)
            {
                for (int c = region.colOffset; c < region.colOffset + region.cols; ++c)
                {
                    content[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)].rawSkip = true;
                }
            }
            TerminalCell& anchor = content[static_cast<std::size_t>(visibleTop)]
                                          [static_cast<std::size_t>(region.colOffset)];
            anchor.raw = raw;
            anchor.rawWidth = region.cols;
            anchor.rawHeight = visibleRows;
            anchor.rawSkip = false;
            return content;
        }
    }

    RenderedContent fallback = fallbackImageContent(sourceRaster, width, height, fit_, align_, verticalAlign_,
                                                   style, fallbackBackground);
    if (!fallback.empty())
    {
        return fallback;
    }
    return placeholderContent(width, height, style, alt_.empty() ? std::string{kMissingImagePlaceholder} : alt_);
}

}  // namespace ui

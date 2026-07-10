#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui
{

inline constexpr const char* kDefaultImageFit = "contain";
inline constexpr const char* kDefaultImageRenderMode = "auto";
inline constexpr const char* kDefaultImageAlign = "center";
inline constexpr const char* kDefaultImageVerticalAlign = "middle";

struct ImageRenderInfo
{
    std::string source;
    std::string fit;
    std::string configuredRenderMode;
    std::string resolvedRenderMode;
    bool sourceLoaded = false;
    int sourceWidth = 0;
    int sourceHeight = 0;
    int elementWidth = 0;
    int elementHeight = 0;
    int cellPixelWidth = 0;
    int cellPixelHeight = 0;
    int imageLeft = 0;
    int imageTop = 0;
    int imageWidth = 0;
    int imageHeight = 0;
    int visibleLeft = 0;
    int visibleTop = 0;
    int visibleWidth = 0;
    int visibleHeight = 0;
    bool rawExpected = false;
    bool rawPresent = false;
    std::vector<std::string> sampleSignature;
};

class Image : public Element
{
public:
    Image(std::string name, std::string source = {}, std::string alt = {},
          std::string fit = kDefaultImageFit, std::string renderMode = kDefaultImageRenderMode,
          std::string align = kDefaultImageAlign, std::string verticalAlign = kDefaultImageVerticalAlign);

    [[nodiscard]] const std::string& source() const
    {
        return source_;
    }

    [[nodiscard]] const std::string& alt() const
    {
        return alt_;
    }

    [[nodiscard]] const std::string& fit() const
    {
        return fit_;
    }

    [[nodiscard]] const std::string& renderMode() const
    {
        return renderMode_;
    }

    [[nodiscard]] const std::string& align() const
    {
        return align_;
    }

    [[nodiscard]] const std::string& verticalAlign() const
    {
        return verticalAlign_;
    }

    void setSource(std::string source);
    void setAlt(std::string alt);
    void setFit(std::string fit);
    void setRenderMode(std::string renderMode);
    void setAlign(std::string align);
    void setVerticalAlign(std::string verticalAlign);

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;
    [[nodiscard]] ImageRenderInfo renderInfo(Size size, ElementRenderState state = {}) const;

private:
    std::string source_;
    std::string alt_;
    std::string fit_;
    std::string renderMode_;
    std::string align_;
    std::string verticalAlign_;
};

class ScopedImageFallbackRendering
{
public:
    ScopedImageFallbackRendering();
    ~ScopedImageFallbackRendering();

    ScopedImageFallbackRendering(const ScopedImageFallbackRendering&) = delete;
    ScopedImageFallbackRendering& operator=(const ScopedImageFallbackRendering&) = delete;
};

class ScopedImageCellBackgroundRendering
{
public:
    ScopedImageCellBackgroundRendering();
    ~ScopedImageCellBackgroundRendering();

    ScopedImageCellBackgroundRendering(const ScopedImageCellBackgroundRendering&) = delete;
    ScopedImageCellBackgroundRendering& operator=(const ScopedImageCellBackgroundRendering&) = delete;
};

void requireSixelForImageRendering();
void setImageTerminalCellPixels(Size size);
[[nodiscard]] bool imageModeNeedsSixelFallbackWarning(std::string_view renderMode);

}  // namespace ui

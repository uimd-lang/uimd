#pragma once

#include <map>
#include <string>
#include <vector>

#include "ui/core/Element.hpp"
#include "ui/elements/Render.hpp"

namespace ui {

struct FrameBufferCell {
    std::string text = " ";
    std::string foreground;
    std::string background;
};

struct FrameBufferRegion {
    int row = 0;
    int col = 0;
    int width = 1;
    int height = 1;
};

class FrameBufferView : public Element {
public:
    explicit FrameBufferView(std::string name);

    void clearFrames();
    void setFrames(std::map<std::string, std::vector<std::vector<FrameBufferCell>>> frames,
                   std::vector<std::string> targetOrder,
                   std::map<std::string, FrameBufferRegion> regions = {});
    void setPan(int row, int col);
    void setDimmed(bool dimmed);
    void setInteractionState(bool focused, bool edit);

    [[nodiscard]] RenderedContent render(Size size, ElementRenderState state = {}) const;

private:
    [[nodiscard]] std::map<std::string, FrameBufferRegion> defaultRegions(Size size) const;
    void overlayCorners(RenderedContent& content, const std::map<std::string, FrameBufferRegion>& regions,
                        const Style& style) const;

    std::map<std::string, std::vector<std::vector<FrameBufferCell>>> frames_;
    std::vector<std::string> targetOrder_;
    std::map<std::string, FrameBufferRegion> regions_;
    int panRow_ = 0;
    int panCol_ = 0;
    bool dimmed_ = false;
    bool panelFocused_ = false;
    bool panelEdit_ = false;
};

}  // namespace ui

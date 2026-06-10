#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "ui/terminal/TerminalBuffer.hpp"

namespace ui {

struct ElementRenderState {
    bool focused = false;
    bool editMode = false;
    bool passiveFocus = false;
    std::optional<int> clipTop;
    std::optional<int> clipBottom;
};

using RenderedRow = std::vector<TerminalCell>;
using RenderedContent = std::vector<RenderedRow>;

class ScopedRenderTimeOverride {
public:
    explicit ScopedRenderTimeOverride(std::optional<std::int64_t> timeMs);
    ~ScopedRenderTimeOverride();

    ScopedRenderTimeOverride(const ScopedRenderTimeOverride&) = delete;
    ScopedRenderTimeOverride& operator=(const ScopedRenderTimeOverride&) = delete;

private:
    std::optional<std::int64_t> previous_;
};

[[nodiscard]] RenderedContent renderPlainText(const std::string& text, int width, int height, const Style& style);
[[nodiscard]] std::vector<std::string> renderedText(const RenderedContent& rendered);

}  // namespace ui

#pragma once

#include <string>
#include <string_view>

namespace ui {

bool copyTextToClipboard(std::string_view text);
[[nodiscard]] std::string runtimeClipboardText();

}  // namespace ui

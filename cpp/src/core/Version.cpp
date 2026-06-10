#include "ui/core/Version.hpp"

#ifndef UIMD_VERSION
#error "UIMD_VERSION must be defined by the build system"
#endif

namespace ui {

std::string_view runtimeVersion()
{
    return UIMD_VERSION;
}

}  // namespace ui

#include"slpch.h"

#include "SoLin/Core/Window.h"

#ifdef SL_PLATFORM_WINDOWS
    #include"Platform/Windows/WindowsWndow.h"
#endif

namespace SoLin {
    std::unique_ptr<Window> Window::Create(const WindowProps& props) {
        #ifdef SL_PLATFORM_WINDOWS
               return std::make_unique<WindowsWindow>(props);
        #elif defined SL_PLATFORM_MACOS
                return std::make_unique<MacosWindow>(props);
        #elif defined SL_PLATFORM_LINUX
                return std::make_unique<LinuxWindow>(props);
        #else
                NUT_CORE_ASSERT(false, "Unknown platform!");
                return nullptr;
        #endif
    }
}

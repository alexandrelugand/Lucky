#pragma once

namespace Lucky
{
	struct WindowProps 
    {
        WindowProps(const char* title = "Lucky engine",
                    uint32_t width = 1600, 
                    uint32_t height = 900,
                    bool vSync = true)
            : Title(title), Width(width), Height(height), VSync(vSync)
        {
        }

        std::string Title;
        uint32_t Width, Height;
        bool VSync;
    };
}

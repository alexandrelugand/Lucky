#pragma once

#include <string>

namespace Lucky
{
	struct WindowProps 
    {
        WindowProps(const char* title = "Lucky engine",
                    unsigned int width = 1280, 
                    unsigned int height = 720,
                    bool vSync = true)
            : Title(title), Width(width), Height(height), VSync(vSync)
        {
        }

        std::string Title;
        unsigned int Width, Height;
        bool VSync;
    };
} // namespace Lucky

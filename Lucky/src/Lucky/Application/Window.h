#pragma once

#include <string>
#include <functional>

#include "Lucky/Core/Events/Event.h"

namespace Lucky
{
    struct WindowProps 
    {
        WindowProps(const char* title = "Lucky engine",
                    unsigned int width = 1280, 
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }

        std::string Title;
        unsigned int Width, Height;
    };

    class Window 
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {};

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual bool IsVSync() const = 0;
        virtual void* GetHandle() const = 0;

        //Window attributes
        virtual void SetEventCallback(const EventCallbackFn& fn) = 0;
        virtual void SetVSync(bool enabled) = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
} // namespace Lucky

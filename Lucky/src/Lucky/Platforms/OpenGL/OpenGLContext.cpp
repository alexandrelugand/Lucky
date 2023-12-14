#include "LuckyPch.h"
#include "OpenGLContext.h"

namespace Lucky
{
	OpenGLContext::OpenGLContext(GLFWwindow *window)
		: m_Window(window)
	{
		LK_CORE_ASSERT(window != nullptr, "Windowh handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
#ifndef __EMSCRIPTEN__
        bool status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0;
        LK_CORE_ASSERT(status, "Failed to initialize GLAD");
#else
		bool status = true;
#endif
		if(status)
		{
			LK_CORE_INFO("OpenGL info:");
			LK_CORE_INFO("  Vendor: {0}", (const char*) glGetString(GL_VENDOR));
			LK_CORE_INFO("  Renderer: {0}", (const char*) glGetString(GL_RENDERER));
			LK_CORE_INFO("  Version: {0}", (const char*) glGetString(GL_VERSION));
		}
	}

	void OpenGLContext::SwapBuffers()
	{
        glfwSwapBuffers(m_Window);
	}
} // namespace Lucky

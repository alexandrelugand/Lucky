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

			#ifdef ENABLE_ASSERTS

			int versionMajor, versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

#ifndef __EMSCRIPTEN__
			LK_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 1), "Lucky requires at least OpenGL 4.1!");
#else
			LK_CORE_ASSERT(versionMajor > 3 || (versionMajor == 3 && versionMinor >= 0), "Lucky requires at least OpenGL 3.0 ES!");
#endif

			#endif
		}
	}

	void OpenGLContext::SwapBuffers()
	{
        glfwSwapBuffers(m_Window);
	}
} // namespace Lucky

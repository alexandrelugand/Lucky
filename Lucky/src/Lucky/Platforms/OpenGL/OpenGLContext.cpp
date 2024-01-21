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
		LK_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_Window);
        bool status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0;
        LK_CORE_ASSERT(status, "Failed to initialize GLAD");
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

			LK_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 1), "Lucky requires at least OpenGL 4.1!");

			#endif
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		LK_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Window);
	}
}

#include "LuckyPch.h"
#include "OpenGLES3Context.h"

namespace Lucky
{
	OpenGLES3Context::OpenGLES3Context(GLFWwindow *window)
		: m_Window(window)
	{
		LK_CORE_ASSERT(window != nullptr, "Windowh handle is null");
	}

	void OpenGLES3Context::Init()
	{
		LK_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_Window);
		
		LK_CORE_INFO("OpenGL info:");
		LK_CORE_INFO("  Vendor: {0}", (const char*) glGetString(GL_VENDOR));
		LK_CORE_INFO("  Renderer: {0}", (const char*) glGetString(GL_RENDERER));
		LK_CORE_INFO("  Version: {0}", (const char*) glGetString(GL_VERSION));

		#ifdef ENABLE_ASSERTS

		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		LK_CORE_ASSERT(versionMajor > 3 || (versionMajor == 3 && versionMinor >= 0), "Lucky requires at least OpenGL 3.0 ES!");

		#endif
	}

	void OpenGLES3Context::SwapBuffers()
	{
		LK_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Window);
	}
}

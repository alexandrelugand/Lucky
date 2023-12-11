#pragma once

#include <GLFW/glfw3.h>

#include "Lucky/Core/Renderer/Context.h"

namespace Lucky
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
} // namespace Lucky
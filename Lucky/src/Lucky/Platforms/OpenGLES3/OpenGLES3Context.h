#pragma once

#include "Lucky/Renderer/Context.h"

namespace Lucky
{
	class OpenGLES3Context : public Context
	{
	public:
		OpenGLES3Context(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
}
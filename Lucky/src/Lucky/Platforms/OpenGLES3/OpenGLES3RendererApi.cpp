#include "LuckyPch.h"
#include "OpenGLES3RendererApi.h"

namespace Lucky
{
	void OpenGLES3RendererApi::Init()
	{
		LK_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLES3RendererApi::SetClearColor(const glm::vec4 &color)
	{
		LK_PROFILE_FUNCTION();
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLES3RendererApi::Clear()
	{
		LK_PROFILE_FUNCTION();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLES3RendererApi::ClearDepth()
	{
		LK_PROFILE_FUNCTION();
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLES3RendererApi::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount)
	{
		LK_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0); //Unbind texture 2D slot
	}

	void OpenGLES3RendererApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		LK_PROFILE_FUNCTION();
		glViewport(x, y, width, height);
	}
}

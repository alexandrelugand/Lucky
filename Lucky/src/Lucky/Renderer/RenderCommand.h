#pragma once

#include "RendererApi.h"

namespace Lucky
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererApi->Init();
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererApi->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererApi->Clear();
		}

		static void ClearDepth()
		{
			s_RendererApi->ClearDepth();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererApi->DrawIndexed(vertexArray, indexCount);
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererApi->SetViewport(x, y ,width, height);
		}

	private:
		static RendererApi* s_RendererApi;
	};
}

#pragma once

#include <glm/glm.hpp>

#include "RendererApi.h"

namespace Lucky
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererApi->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererApi->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererApi->DrawIndexed(vertexArray);
		}

	private:
		static RendererApi* s_RendererApi;
	};
} // namespace Lucky

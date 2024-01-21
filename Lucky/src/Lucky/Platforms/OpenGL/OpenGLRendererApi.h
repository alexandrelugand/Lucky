#pragma once

#include <glm/glm.hpp>
#include "Lucky/Renderer/RendererApi.h"
#include "Lucky/Core/Base.h"

namespace Lucky
{
	class OpenGLRendererApi : public RendererApi
	{
	public:
		void Init() override;
		
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void ClearDepth() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}

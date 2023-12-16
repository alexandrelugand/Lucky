#pragma once

#include <glm/glm.hpp>
#include "Lucky/Core/Renderer/RendererApi.h"
#include "Lucky/Core/Core.h"

namespace Lucky
{
	class OpenGLRendererApi : public RendererApi
	{
	public:
		void Init() override;
		
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
} // namespace Lucky

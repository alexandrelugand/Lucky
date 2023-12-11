#pragma once

#include "Lucky/Core/Renderer/RendererApi.h"

namespace Lucky
{
	class OpenGLRendererApi : public RendererApi
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
} // namespace Lucky

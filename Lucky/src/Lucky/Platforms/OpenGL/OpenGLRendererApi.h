#pragma once

#include "Lucky/Core/Renderer/RendererApi.h"

namespace Lucky
{
	class OpenGLRendererApi : public RendererApi
	{
	public:
		void Init() override;
		
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
} // namespace Lucky
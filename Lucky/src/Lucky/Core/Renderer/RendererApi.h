#pragma once

#include "VertexArray.h"

namespace Lucky
{
	class RendererApi
	{
	public:
		enum class Api
		{
			None = 0,
			OpenGL
		};
		
		virtual void Init() = 0;
		
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		inline static Api GetApi() { return s_Api; }

	private:
		static Api s_Api;
	};

} // namespace Lucky

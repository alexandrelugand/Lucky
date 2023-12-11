#pragma once

namespace Lucky
{
	enum class RendererApi : uint8_t
	{
		None = 0,
		OpenGL
	};

	class Renderer 
	{
	public:
		inline static RendererApi GetApi() { return s_RendererApi; }

	private:
		static RendererApi s_RendererApi;
	};

} // namespace Lucky

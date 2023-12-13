#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"

namespace Lucky
{
	class Renderer 
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


		inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

} // namespace Lucky

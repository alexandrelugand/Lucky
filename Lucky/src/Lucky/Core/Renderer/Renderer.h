#pragma once

#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RendererApi.h"
#include "Lucky/Core/CameraController.h"

namespace Lucky
{
	class Renderer 
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Scope<CameraController>& cameraController);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererApi::Api GetApi() { return RendererApi::GetApi(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

} // namespace Lucky

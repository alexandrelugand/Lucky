#pragma once

#include "Lucky/Core/CameraController.h"
#include "Lucky/Core/Renderer/Texture.h"

namespace Lucky
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Scope<CameraController>& cameraController);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture);
	};
} // namespace Lucky

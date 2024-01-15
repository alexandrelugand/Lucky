#pragma once

#include "Lucky/Core/CameraController.h"
#include "Lucky/Renderer/Camera.h"
#include "Lucky/Renderer/EditorCamera.h"
#include "Lucky/Renderer/Texture.h"
#include "Lucky/Renderer/SubTexture2D.h"
#include "Lucky/Scene/Components.h"

namespace Lucky
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		// Runtime
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const Camera& camera, const glm::mat4& transform, const RenderPass& pass);

		// Editor
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const EditorCamera& camera, const RenderPass& pass);

		//TODO: to remove
		static void BeginScene(const BaseCamera& baseCamera);
		static void BeginScene(const Scope<CameraController>& cameraController);

		static void EndScene();
		static void EndScene(const RenderPass& pass);
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityId = -1);

		static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderComponent, int entityId = -1);

		struct Statictics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statictics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
} // namespace Lucky

#pragma once

#include <entt.hpp>

#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Renderer/EditorCamera.h"
#include "Lucky/Renderer/Framebuffer.h"
#include "Lucky/Renderer/Shader.h"

namespace Lucky
{
	class Entity;
	struct RenderPass;

	using RenderPassCallbackFn = std::function<void(const RenderPass&)>;

	struct RenderPass
	{
		RenderPass() = default;
		RenderPass(const RenderPass& pass) = default;

		std::string Name;
		Ref<Framebuffer> Framebuffer;
		Ref<Shader> Shader;
		RenderPassCallbackFn BeforeRenderCallback = nullptr;
		RenderPassCallbackFn AfterRenderCallback = nullptr;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void Clean();

		const std::vector<RenderPass>& GetPasses() const { return m_RenderPasses; }
		void AddPass(const RenderPass& pass);
		void ClearPass();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);
		void OnEvent(Event& e);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera() const;

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;
		std::vector<RenderPass> m_RenderPasses;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};	
}

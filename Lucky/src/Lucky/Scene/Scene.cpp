#include "LuckyPch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Lucky/Renderer/Renderer2D.h"

namespace Lucky
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto tag = name.empty() ? "Entity" : name;
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Clean()
	{
#ifdef COMPILER_MSVC
#pragma warning( push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
		m_Registry.each([&](auto entity)
		{
			m_Registry.destroy(entity);
		});
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update Native scripts
		{
#ifdef COMPILER_MSVC
#pragma warning( push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
			m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.InitScript(Entity{ entity, this });
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
#ifdef COMPILER_MSVC
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for(auto entity : view)
		{
			auto [cameraCmp, transformCmp] = view.get<CameraComponent, TransformComponent>(entity);
			if(cameraCmp.Primary)
			{
				mainCamera = &cameraCmp.Camera;
				cameraTransform = transformCmp.GetTransform();
				break;
			}
		}

		if(mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group)
			{
				const auto& [tc, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(tc.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnEvent(Event& e)
	{
		m_Registry.view<NativeScriptComponent>().each([&](NativeScriptComponent& nsc)
		{
			nsc.Instance->OnEvent(e);
		});
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize out non-Fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraCmp = view.get<CameraComponent>(entity);
			if (!cameraCmp.FixedAspectRatio)
				cameraCmp.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCamera() const
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& cameraCmp = view.get<CameraComponent>(entity);
			if (cameraCmp.Primary)
				return Entity{ entity, (Scene*)this };
		}
		return Entity{};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		LK_CORE_ASSERT(false, "Not supported");
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component)
	{
	}
}

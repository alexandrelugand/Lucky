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

	void Scene::OnUpdate(Timestep ts)
	{
		// Update Native scripts
		{
			m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.InitScript(Entity{ entity, this });
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		auto view = m_Registry.view<CameraComponent, TransformComponent>();
		for(auto entity : view)
		{
			auto [cameraCmp, transformCmp] = view.get<CameraComponent, TransformComponent>(entity);
			if(cameraCmp.Primary)
			{
				mainCamera = &cameraCmp.Camera;
				cameraTransform = &transformCmp.Transform;
				break;
			}
		}

		if(mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for(auto entity : group)
			{
				const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnEvent(Event& e)
	{
		m_Registry.view<CameraComponent>().each([&](CameraComponent& cc)
		{
			cc.Camera.OnEvent(e);
		});

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
}

#include "LuckyPch.h"
#include "Scene.h"

#include "Entity.h"
#include "ScriptableEntity.h"
#include "Components.h"
#include "Lucky/Renderer/Renderer2D.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Lucky
{
	static b2BodyType RigidBody2DTypeToBox2DType(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::Static:	return b2BodyType::b2_staticBody;
			case RigidBody2DComponent::BodyType::Dynamic:	return b2BodyType::b2_dynamicBody;
			case RigidBody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
			default:
				LK_CORE_ASSERT(false, "Unknown body type");
				return b2BodyType::b2_staticBody;
		}
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(Uuid(), name);
	}

	Entity Scene::CreateEntity(Uuid uuid, const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		entity.AddComponent<IdComponent>(uuid);
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

	void Scene::AddPass(const RenderPass& pass)
	{
		m_RenderPasses.push_back(pass);
	}

	void Scene::ClearPass()
	{
		m_RenderPasses.clear();
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for(auto e : view)
		{
			Entity entity{ e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = RigidBody2DTypeToBox2DType(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if(entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// S / P / R renderer pipeline
		Renderer2D::ResetStats();

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
			const auto& [cameraCmp, transformCmp] = view.get<CameraComponent, TransformComponent>(entity);
			if(cameraCmp.Primary)
			{
				mainCamera = &cameraCmp.Camera;
				cameraTransform = transformCmp.GetTransform();
				break;
			}
		}

		// Update Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<RigidBody2DComponent>();
			for(auto e : view)
			{
				Entity entity{ e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		// Update Renderer
		if(mainCamera)
		{
			for (auto& pass : m_RenderPasses)
			{
				Renderer2D::BeginScene(*mainCamera, cameraTransform, pass);

				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					const auto& [tc, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::DrawSprite(tc.GetTransform(), sprite, (int)entity);
				}

				Renderer2D::EndScene(pass);
			}
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera)
	{
		Renderer2D::ResetStats();

		for(auto& pass : m_RenderPasses)
		{
			Renderer2D::BeginScene(editorCamera, pass);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				const auto& [tc, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(tc.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene(pass);
		}
	}

	void Scene::OnEvent(Event& e)
	{
		m_Registry.view<NativeScriptComponent>().each([&](NativeScriptComponent& nsc)
		{
			if (nsc.Instance)
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
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, IdComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component)
	{
		if(m_ViewportWidth > 0 && m_ViewportHeight > 0)
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

	template<>
	void Scene::OnComponentAdded(Entity entity, RigidBody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, BoxCollider2DComponent& component)
	{
	}
}

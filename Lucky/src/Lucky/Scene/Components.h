#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "Lucky/Core/Uuid.h"
#include "Lucky/Renderer/Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Lucky
{
	struct IdComponent
	{
		Uuid Id;

		IdComponent() = default;
		IdComponent(uint64_t id)
			: Id(id) {}
		IdComponent(const IdComponent& other) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag = std::string())
			: Tag(tag) { }

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) { }

		glm::mat4 GetTransform() const
		{
			auto rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct CameraComponent
	{
		CameraComponent()
			: Camera({}) {}
		CameraComponent(const Camera::Settings& settings)
			: Camera(settings) {}
		CameraComponent(const CameraComponent& other) = default;

		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;
	};

	//Forward declarations
	class Entity;
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		std::function<ScriptableEntity* (Entity&&)> InitScript;
		std::function<void()> DestroyScript;

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent& other) = default;

		template<typename T>
		void Bind()
		{
			InitScript = [&](const Entity& entity) { return Instance = static_cast<ScriptableEntity*>(new T(entity)); };
			DestroyScript = [&]() { delete Instance; Instance = nullptr; };
		}
	};

	struct RigidBody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}

#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Lucky
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag = std::string())
			: Tag(tag) { }

		std::string Tag;

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) { }

		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const
		{
			auto rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3{ 0.0f, 0.0f, 1.0f });

			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }

		glm::vec4 Color{ 1.0f };

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
		bool Primary = false;
		bool FixedAspectRatio = false;
	};

	struct NativeScriptComponent
	{
		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent& other) = default;

		ScriptableEntity* Instance = nullptr;

		std::function<ScriptableEntity*(Entity&&)> InitScript;
		std::function<void()> DestroyScript;

		template<typename T>
		void Bind()
		{
			InitScript = [&](const Entity& entity) { return Instance = static_cast<ScriptableEntity*>(new T(entity)); };
			DestroyScript = [&]() { delete Instance; Instance = nullptr; };
		}
	};
}

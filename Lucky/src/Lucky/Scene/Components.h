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
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) { }

		glm::mat4 Transform{ 1.0f };

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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
		CameraComponent(const CameraSettings& settings)
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

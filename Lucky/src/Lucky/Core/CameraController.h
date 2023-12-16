#pragma once

#include <glm/glm.hpp>
#include "Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Renderer/Camera.h"

namespace Lucky
{
	struct CameraSettings
	{
		float AspectRatio;
		bool EnableRotation;
		glm::vec3 Position = {0.0f, 0.0f, 0.0f};
		float Rotation = 0.0f;
		float TranslationSpeed;
		float RotationSpeed;
		float ZoomSpeed;

		// Perspective properties
		float Fov;
		float ZNear;
		float ZFar;
	};

	class CameraController
	{
	public:
		virtual ~CameraController() = default;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void OnImGuiRender() = 0;

		virtual Camera& GetCamera() = 0;
		virtual const Camera& GetCamera() const = 0;

		static Scope<CameraController> Create(CameraType type, const CameraSettings& settings);
	};
} // namespace Lucky

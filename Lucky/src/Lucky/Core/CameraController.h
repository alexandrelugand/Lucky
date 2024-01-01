#pragma once

#include <glm/glm.hpp>
#include "Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Renderer/BaseCamera.h"

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
		float ZoomLevel;

		// Orthographic properties
		float Size;

		// Perspective properties
		float Fov;
		float NearClip;
		float FarClip;
	};

	struct CameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class CameraController
	{
	public:
		virtual ~CameraController() = default;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void OnImGuiRender() = 0;

		virtual BaseCamera& GetCamera() = 0;
		virtual const BaseCamera& GetCamera() const = 0;
		virtual const CameraBounds& GetBounds() const = 0;

		virtual float GetZoomLevel() const = 0;
		virtual void SetZoomLevel(float value) = 0;
		virtual void OnResize(float width, float height) = 0;

		static Scope<CameraController> Create(CameraType type, const CameraSettings& settings);
	};
} // namespace Lucky

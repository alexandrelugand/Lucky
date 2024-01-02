#pragma once

#include "Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Renderer/BaseCamera.h"
#include "Lucky/Renderer/Camera.h"

namespace Lucky
{
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

		static Scope<CameraController> Create(CameraType type, const Camera::Settings& settings);
	};
} // namespace Lucky

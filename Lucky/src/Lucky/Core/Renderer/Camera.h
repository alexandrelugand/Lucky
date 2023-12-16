#pragma once

#include "Lucky/Core/Core.h"

namespace Lucky
{
	struct CameraSettings;

	enum class CameraType
	{
		Orthographic = 0,
		Perspective
	};

	class Camera
	{
	public:
		virtual ~Camera() = default;

		const glm::vec3& GetPosition() const { return m_Position; }
		void 	SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void RecalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};
} // namespace Lucky

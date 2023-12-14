#include "LuckyPch.h"
#include "Camera.h"

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

namespace Lucky
{
	Camera::Camera()
		: m_ProjectionMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Scope<Camera> Camera::Create(CameraType type)
	{
		switch (type)
		{
		case CameraType::Orthographic:
			return std::make_unique<OrthographicCamera>(-1.6f, 1.6f, 0.9f, -0.9f);

		case CameraType::Perspective:
			return std::make_unique<PerspectiveCamera>(glm::radians(60.0f), 1.6f, 0.9f, 0.1f, 10.0f);
		}

		LK_CORE_ASSERT(false, "Unknown camera type!");
		return nullptr;
	}
} // namespace Lucky

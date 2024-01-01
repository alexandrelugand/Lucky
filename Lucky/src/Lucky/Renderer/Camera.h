#pragma once

namespace Lucky
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}

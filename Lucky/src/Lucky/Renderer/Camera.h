#pragma once

namespace Lucky
{
	class Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic
		};

		struct Settings
		{
			Settings() = default;
			Settings(const Settings& other) = default;

			ProjectionType ProjectionType = ProjectionType::Perspective;
			float AspectRatio = 0.0f;
			bool EnableRotation = true;

			glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
			float Rotation = 0.0f;
			float TranslationSpeed = 10.0f;
			float RotationSpeed = glm::radians(90.0f);
			float ZoomSpeed = 0.5f;
			float ZoomLevel = 1.0f;

			// Orthographic settings
			float OrthographicSize = 10.0f;
			float OrthographicNearClip = -1.0;
			float OrthographicFarClip = 1.0f;

			// Perspective settings
			float PerspectiveFov = 45.0f;
			float PerspectiveNearClip = 0.01f;
			float PerspectiveFarClip = 1000.0f;
		};

		Camera(const Settings& settings);
		Camera(const Settings& settings, const glm::mat4& projection);
		virtual ~Camera() = default;
			
		const Settings& GetSettings() const { return m_Settings; }
		const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		Settings m_Settings;
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}

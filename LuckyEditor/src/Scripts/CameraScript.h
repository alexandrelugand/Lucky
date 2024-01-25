#pragma once 
#include "Lucky/Application/Input.h"
#include "Lucky/Scene/Components.h"
#include "Lucky/Scene/ScriptableEntity.h"

namespace Lucky
{
	class CameraScript : public ScriptableEntity
	{
		Camera::Settings c_Settings;

	public:
		CameraScript(const Entity& entity)
			: ScriptableEntity(entity)
		{
			c_Settings.TranslationSpeed = 2.5f;
			c_Settings.RotationSpeed = glm::radians(90.0f);
			c_Settings.ZoomSpeed = 0.5f;
		}

		void OnCreate() override
		{
		}

		void OnDestroy() override
		{
		}

		void OnUpdate(Timestep ts) override
		{
			auto& tc = GetComponent<TransformComponent>();

#ifndef __EMSCRIPTEN__
			if (Input::IsKeyPressed(KeyCode::A))
#else
			if (Input::IsKeyPressed(KeyCode::Q))
#endif
			{
				tc.Translation.x -= cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				tc.Translation.y -= sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
			}
			else
				if (Input::IsKeyPressed(KeyCode::D))
				{
					tc.Translation.x += cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					tc.Translation.y += sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				}

			if (Input::IsKeyPressed(KeyCode::S))
			{
				tc.Translation.x -= -sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				tc.Translation.y -= cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
			}
			else
#ifndef __EMSCRIPTEN__
				if (Input::IsKeyPressed(KeyCode::W))
#else
				if (Input::IsKeyPressed(KeyCode::Z))
#endif
				{
					tc.Translation.x += -sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					tc.Translation.y += cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				}


			if (Input::IsKeyPressed(KeyCode::E))
				tc.Rotation.z -= c_Settings.RotationSpeed * ts;
			else
#ifndef __EMSCRIPTEN__
				if (Input::IsKeyPressed(KeyCode::Q))
#else
				if (Input::IsKeyPressed(KeyCode::A))
#endif
					tc.Rotation.z += c_Settings.RotationSpeed * ts;

			if (Input::IsKeyPressed(KeyCode::Space))
			{
				tc.Translation = glm::vec3(0.0f);
				tc.Rotation.z = 0.0f;
			}
		}

		void OnEvent(Event& e) override
		{
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraScript::OnMouseScrolled));
		}

		bool OnMouseScrolled(MouseScrolledEvent& e)
		{
			auto [tc, cc] = GetComponent<TransformComponent, CameraComponent>();
			auto& settings = cc.Camera.GetSettings();

			if (settings.ProjectionType == Camera::ProjectionType::Perspective)
			{
				tc.Translation.z -= e.GetYOffset() * settings.ZoomSpeed;
			}
			else
			{
				cc.Camera.SetOrthographicSize(cc.Camera.GetOrthographicSize() - e.GetYOffset() * settings.ZoomSpeed);
			}
			return false;
		}
	};	
}

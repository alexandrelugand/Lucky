#pragma once
#include <Lucky.h>

namespace Lucky
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event &event) override;

	private:
		Scope<CameraController> m_CameraController;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
} // namespace Lucky

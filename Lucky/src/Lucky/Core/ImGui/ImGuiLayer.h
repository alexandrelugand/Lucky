#pragma once

#include "Lucky/Core/Layer.h"

namespace Lucky
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;
		void OnEvent(Event& event) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }
        void Load();
        void Save();
        
        void Begin();
        void End();

    private:
		void SetDartThemeColors();

		bool m_BlockEvents = false;
    };
}

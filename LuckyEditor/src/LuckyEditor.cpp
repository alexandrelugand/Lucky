#include "LuckyEditorPch.h"
#include "LuckyEditor.h"

#include "Layers/EditorLayer.h"

namespace Lucky
{
	LuckyEditor::LuckyEditor(const WindowProps& props) :
		Application(props)
	{
        PushLayer(new EditorLayer());
		LK_INFO("Application initialized");
	}

	LuckyEditor::~LuckyEditor()
	{
	}
}

Lucky::Application* CreateApplication()
{
	return new Lucky::LuckyEditor({ "Lucky Editor", 1600, 900, false });
}

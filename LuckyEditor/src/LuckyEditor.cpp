#include "LuckyEditorPch.h"
#include "LuckyEditor.h"

#include "Layers/EditorLayer.h"

namespace Lucky
{
	LuckyEditor::LuckyEditor(const WindowProps& props) :
		Application(props)
	{
		auto& shaderLibrary = ShaderLibrary::GetInstance();
		if(!shaderLibrary.Exists("2DTexture"))
			shaderLibrary.Load("assets/shaders/2DTexture.glsl");
		if (!shaderLibrary.Exists("MousePicking"))
			shaderLibrary.Load("assets/shaders/MousePicking.glsl");

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

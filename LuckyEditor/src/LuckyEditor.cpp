#include "LuckyEditorPch.h"
#include "LuckyEditor.h"

#include "Layers/EditorLayer.h"

namespace Lucky
{
	LuckyEditor::LuckyEditor(const WindowProps& props, const ApplicationCommandLineArgs& args) :
		Application(props, args)
	{
		auto& shaderLibrary = ShaderLibrary::GetInstance();
		if(!shaderLibrary.Exists("2DTexture"))
			shaderLibrary.LoadByApi("2DTexture.glsl");

		if (RendererApi::GetApi() == RendererApi::Api::OpenGLES3)
		{
			if (!shaderLibrary.Exists("MousePicking"))
				shaderLibrary.LoadByApi("MousePicking.glsl");
		}

        PushLayer(new EditorLayer());
		LK_INFO("Application initialized");
	}

	LuckyEditor::~LuckyEditor()
	{
	}
}

Lucky::Application* CreateApplication(const Lucky::ApplicationCommandLineArgs& args)
{
	return new Lucky::LuckyEditor({ "Lucky Editor", 1600, 900, false }, args);
}

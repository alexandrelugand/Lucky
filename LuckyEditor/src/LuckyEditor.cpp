#include "LuckyEditorPch.h"
#include "LuckyEditor.h"

#include "Layers/EditorLayer.h"

namespace Lucky
{
	LuckyEditor::LuckyEditor(const WindowProps& props, const ApplicationSpecification& specification) :
		Application(props, specification)
	{
		auto& shaderLibrary = ShaderLibrary::GetInstance();
		if (RendererApi::GetApi() == RendererApi::Api::OpenGLES3)
		{
			if (!shaderLibrary.Exists("MousePicking"))
				shaderLibrary.LoadByApi("MousePicking.glsl");
		}

        PushLayer(new EditorLayer());
		LK_INFO("Application initialized");
	}
}

Lucky::Application* CreateApplication(const Lucky::ApplicationCommandLineArgs& args)
{
	Lucky::ApplicationSpecification spec;
	spec.WorkingDirectory = "../LuckyEditor";
	spec.CommandLineArgs = args;

	return new Lucky::LuckyEditor({ "Lucky Editor", 1600, 900, false }, spec);
}

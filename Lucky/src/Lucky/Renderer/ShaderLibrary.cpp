#include "LuckyPch.h"
#include "ShaderLibrary.h"

#include "RendererApi.h"

namespace Lucky
{
	ShaderLibrary ShaderLibrary::s_Instance;

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		LK_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	bool ShaderLibrary::Exists(const std::string &name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	Ref<Shader> ShaderLibrary::LoadByApi(const std::string& filename)
	{
		auto apiName = NAMEOF_ENUM(RendererApi::GetApi());
		auto filePath = std::filesystem::path("assets/shaders") / apiName / filename;
		auto shader = Shader::Create(filePath.string());
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	const Ref<Shader>& ShaderLibrary::Get(const std::string& name)
	{
		LK_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

}

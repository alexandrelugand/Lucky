#pragma once

#include "Shader.h"
#include "Lucky/Core/Core.h"

namespace Lucky
{
	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		bool Exists(const std::string& name);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		const Ref<Shader>& Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};
} // namespace Lucky

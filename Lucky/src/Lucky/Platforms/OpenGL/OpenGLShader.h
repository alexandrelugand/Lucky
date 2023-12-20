#pragma once

#include "Lucky/Core/Renderer/Shader.h"

namespace Lucky
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		inline const std::string& GetName() const override { return m_Name; }

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) override;

		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		std::string GetHeader(uint32_t shaderType);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

		uint32_t m_ProgramId;
		std::string m_Name;
	};
} // namespace Lucky

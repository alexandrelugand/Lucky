#pragma once

#include "Lucky/Renderer/Shader.h"

namespace Lucky
{
	class OpenGLES3Shader : public Shader
	{
	public:
		OpenGLES3Shader(const std::string& filepath);
		OpenGLES3Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLES3Shader();

		const std::string& GetName() const override { return m_Name; }

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

	protected:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		std::string GetHeader(uint32_t shaderType);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

		uint32_t m_ProgramId;
		std::string m_Name;

		friend class OpenGLES3UniformBuffer;
	};
}

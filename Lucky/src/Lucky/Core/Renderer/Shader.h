#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Lucky
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_ProgramId;
	};
} // namespace Lucky

#pragma once

#include <string>

namespace Lucky
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ProgramId;
	};
} // namespace Lucky

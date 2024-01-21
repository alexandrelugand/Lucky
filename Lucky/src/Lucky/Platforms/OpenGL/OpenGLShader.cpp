#include "LuckyPch.h"
#include "OpenGLShader.h"

namespace Lucky
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if(type == "vertex")
			return GL_VERTEX_SHADER;
		if(type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		LK_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		LK_PROFILE_FUNCTION();
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto firstDot = filepath.find('.');
		auto count = firstDot == std::string::npos ? filepath.size() - lastSlash : firstDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string &vertexSrc, const std::string &fragmentSrc)
		: m_Name(name)
	{
		LK_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		LK_PROFILE_FUNCTION();
		glDeleteProgram(m_ProgramId);
	}

	void OpenGLShader::Bind() const
	{
		LK_PROFILE_FUNCTION();
		glUseProgram(m_ProgramId);
	}

	void OpenGLShader::Unbind() const
	{
		LK_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string &name, int value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string &name, float value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2& value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3& value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4& value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string &name, const glm::mat3& value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string &name, const glm::mat4& value)
	{
		LK_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::ReadFile(const std::string &filepath)
	{
		LK_PROFILE_FUNCTION();
		std::string content;
		std::ifstream in(filepath.c_str(), std::ios::in | std::ios::binary);
		if(in)
		{
			in.seekg(0, std::ios::end);
			content.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&content[0], content.size());
			in.close();
		}
		else
		{
			LK_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return content;
	}
	
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		LK_PROFILE_FUNCTION();
		std::unordered_map<uint32_t, std::string> shaderSources;
		
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while(pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			LK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = Trim(source.substr(begin, eol - begin));
			GLenum shaderType = ShaderTypeFromString(type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderType] = GetHeader(shaderType) + source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	std::string OpenGLShader::GetHeader(uint32_t shaderType)
	{
		return "#version 410\r\n";
	}

	void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string> &shaderSources)
	{
		LK_PROFILE_FUNCTION();
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint programId = glCreateProgram();
		LK_CORE_ASSERT(shaderSources.size() <= 2, "2 Shaders only supported (vertex and fragment)");
		std::array<GLuint, 2> glShaderIds;
		uint32_t shaderIndex = 0;

		for(auto& kvp : shaderSources)
		{
			GLenum shaderType = kvp.first;
			const std::string& source = kvp.second;

			// Create an empty vertex shader handle
			GLuint shaderId = glCreateShader(shaderType);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar *src = source.c_str();
			glShaderSource(shaderId, 1, &src, 0);

			// Compile the vertex shader
			glCompileShader(shaderId);

			GLint isCompiled = 0;
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);
				
				// We don't need the shader anymore.
				glDeleteShader(shaderId);

				LK_CORE_ERROR("{0}", infoLog.data());
				LK_CORE_ASSERT(false, "Shader compilation failed!");
				break;
			}

			glAttachShader(programId, shaderId);
			glShaderIds[shaderIndex++] = shaderId;
		}

		// Link our program
		glLinkProgram(programId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the program anymore.
			glDeleteProgram(programId);

			// Don't leak shaders either.
			for(auto shaderId : glShaderIds)
				glDeleteShader(shaderId);

			LK_CORE_ERROR("{0}", infoLog.data());
			LK_CORE_ASSERT(false, "Shader link failed!");
			return;
		}

		// Always detach shaders after a successful link.
		for(auto shaderId : glShaderIds)
			glDetachShader(programId, shaderId);

		m_ProgramId = programId;
	}
}

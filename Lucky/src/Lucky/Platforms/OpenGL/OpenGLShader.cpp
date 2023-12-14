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
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string &name, int value)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string &name, float value)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string &filepath)
	{
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
		std::unordered_map<uint32_t, std::string> shaderSources;
		
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while(pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			LK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = trim(source.substr(begin, eol - begin));
			GLenum shaderType = ShaderTypeFromString(type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string> &shaderSources)
	{
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
} // namespace Lucky

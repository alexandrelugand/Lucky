#include "LuckyPch.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace Lucky
{
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
	{
		LK_CORE_TRACE("Vertex src:");
		LK_CORE_TRACE("\n{0}", vertexSrc);
		LK_CORE_TRACE("Fragment src:");
		LK_CORE_TRACE("\n{0}", fragmentSrc);

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			LK_CORE_ERROR("{0}", infoLog.data());
			LK_CORE_ASSERT(false, "Vertex shader compilation failed!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			LK_CORE_ERROR("{0}", infoLog.data());
			LK_CORE_ASSERT(false, "Fragment shader compilation failed!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ProgramId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ProgramId, vertexShader);
		glAttachShader(m_ProgramId, fragmentShader);

		// Link our program
		glLinkProgram(m_ProgramId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the program anymore.
			glDeleteProgram(m_ProgramId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LK_CORE_ERROR("{0}", infoLog.data());
			LK_CORE_ASSERT(false, "Shader link failed!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ProgramId, vertexShader);
		glDetachShader(m_ProgramId, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramId);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
} // namespace Lucky

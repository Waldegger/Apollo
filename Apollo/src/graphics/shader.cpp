#include "shader.h"

#include <stdexcept>

namespace agl
{
	shader::shader(type shader_type)
		: m_type{ shader_type }
		, m_handle{ glCreateShader(static_cast<GLuint>(shader_type)) }
	{
		if (!m_handle)
		{
			throw std::runtime_error{ "Error creating shader" };
		}
	}

	void shader::compile(const std::string_view& shader_source)
	{
		const GLchar* data = shader_source.data();

		glShaderSource(m_handle, 1, &data, NULL);
		glCompileShader(m_handle);

		GLint success;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			static constexpr size_t logSize = 512;
			char infoLog[logSize];

			glGetShaderInfoLog(m_handle, logSize, NULL, infoLog);
			throw std::runtime_error{ std::string{ "ERROR::SHADER::COMPILATION_FAILED\n" } + infoLog };
		};
	}

	void shader::delete_handle(GLuint handle)
	{
		glDeleteShader(handle);
	}
}
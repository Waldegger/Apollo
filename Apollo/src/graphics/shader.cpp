#include "shader.h"

#include <GL/glew.h>

#include <stdexcept>

namespace age
{
	shader::shader(shader_type type)
		: m_type{ type }
		, m_handle{ glCreateShader(convert_type(type)) }
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

	uint32_t shader::convert_type(shader_type type_to_convert)
	{
		switch (type_to_convert)
		{
		case shader_type::fragment:
			return GL_FRAGMENT_SHADER;

		case shader_type::geometry:
			return GL_GEOMETRY_SHADER;

		case shader_type::vertex:
			return GL_VERTEX_SHADER;

		default:
			throw std::runtime_error{ "SHADER::CONVERT_TYPE INVALID SHADER_TYPE!" };
		}
	}

	void shader::delete_handle(uint32_t handle)
	{
		glDeleteShader(handle);
	}
}
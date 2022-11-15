#include "shader_program.h"

#include <stdexcept>
#include <string>

namespace apollo
{
	shader_program::shader_program()
		: m_handle{ glCreateProgram() }
	{
		if (!m_handle)
		{
			throw std::runtime_error{ "ERROR::SHADER_PROGRAM::CREATION_FAILED\n" };
		}

		m_attached_shaders.reserve(8);
	}

	void shader_program::attach_shader(const shader& shader)
	{
		glAttachShader(get_handle(), shader.get_handle());

		if(std::find(m_attached_shaders.begin(), m_attached_shaders.end(), shader.get_handle()) == m_attached_shaders.end())
			m_attached_shaders.push_back(shader.get_handle());
	}

	void shader_program::detach_shader(const shader& shader)
	{
		glDetachShader(get_handle(), shader.get_handle());

		auto it = std::find(m_attached_shaders.begin(), m_attached_shaders.end(), shader.get_handle());
		if (it != m_attached_shaders.end())
			m_attached_shaders.erase(it);
	}

	void shader_program::link()
	{
		glLinkProgram(get_handle());

		GLint success;
		glGetProgramiv(get_handle(), GL_LINK_STATUS, &success);

		//Before potential throw, detach the shaders as it according to Khronos 
		//it is recommended to detach them after succesful or failed linking
		for(auto handle : m_attached_shaders)
			glDetachShader(get_handle(), handle);

		if (!success)
		{
			static constexpr size_t info_size = 512;
			char info_log[info_size];

			glGetProgramInfoLog(get_handle(), info_size, NULL, info_log);

			throw std::runtime_error{ std::string{ "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" } + info_log };
		}
	}

	void shader_program::use()
	{
		glUseProgram(get_handle());
	}

	void shader_program::delete_handle(GLuint handle)
	{
		glDeleteProgram(handle);
	}
}
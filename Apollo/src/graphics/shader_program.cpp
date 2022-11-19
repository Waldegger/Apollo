#include "shader_program.h"

#include <stdexcept>
#include <string>

namespace agl
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

		if (auto it = std::find(m_attached_shaders.begin(), m_attached_shaders.end(), shader.get_handle()); it != m_attached_shaders.end())
			m_attached_shaders.erase(it);
	}

	void shader_program::bind_attrib_location(uint32_t index, const std::string_view& name)
	{
		glBindAttribLocation(get_handle(), index, name.data());
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

		m_attached_shaders.clear();

		if (!success)
		{
			static constexpr size_t info_size = 512;
			char info_log[info_size];

			glGetProgramInfoLog(get_handle(), info_size, NULL, info_log);

			throw std::runtime_error{ std::string{ "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" } + info_log };
		}
	}

	void shader_program::bind()
	{
		glUseProgram(get_handle());
	}

	void shader_program::release()
	{
		glUseProgram(0);
	}

	int32_t shader_program::get_uniform_location(const std::string_view& name)
	{
		return glGetUniformLocation(get_handle(), name.data());
	}

	void shader_program::set_uniform(int32_t location, float v0)
	{
		glUniform1f(location, v0);
	}

	void shader_program::set_uniform(int32_t location, float v0, float v1)
	{
		glUniform2f(location, v0, v1);
	}

	void shader_program::set_uniform(int32_t location, float v0, float v1, float v2)
	{
		glUniform3f(location, v0, v1, v2);
	}

	void shader_program::set_uniform(int32_t location, float v0, float v1, float v2, float v3)
	{
		glUniform4f(location, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(int32_t location, int32_t v0)
	{
		glUniform1i(location, v0);
	}

	void shader_program::set_uniform(int32_t location, int32_t v0, int32_t v1)
	{
		glUniform2i(location, v0, v1);
	}

	void shader_program::set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2)
	{
		glUniform3i(location, v0, v1, v2);
	}

	void shader_program::set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2, int32_t v3)
	{
		glUniform4i(location, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(int32_t location, uint32_t v0)
	{
		glUniform1ui(location, v0);
	}

	void shader_program::set_uniform(int32_t location, uint32_t v0, uint32_t v1)
	{
		glUniform2ui(location, v0, v1);
	}

	void shader_program::set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2)
	{
		glUniform3ui(location, v0, v1, v2);
	}

	void shader_program::set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
	{
		glUniform4ui(location, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<float, 1>& v)
	{
		glUniform1fv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<float, 2>& v)
	{
		glUniform2fv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<float, 3>& v)
	{
		glUniform3fv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<float, 4>& v)
	{
		glUniform4fv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 1>& v)
	{
		glUniform1iv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 2>& v)
	{
		glUniform2iv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 3>& v)
	{
		glUniform3iv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 4>& v)
	{
		glUniform4iv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 1>& v)
	{
		glUniform1uiv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 2>& v)
	{
		glUniform2uiv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 3>& v)
	{
		glUniform3uiv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 4>& v)
	{
		glUniform4uiv(location, count, v.data());
	}

	void shader_program::set_uniform(int32_t location, const matrix4f& v, bool transpose)
	{
		glUniformMatrix4fv(location, 1, transpose, v.get_data().data());
	}

	void shader_program::set_uniform(int32_t location, const matrix4f* v[], size_t size, bool transpose)
	{
		glUniformMatrix4fv(location, size, transpose, reinterpret_cast<float*>(v));
	}

	void shader_program::set_uniform(const std::string_view& name, float v0)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0);
	}

	void shader_program::set_uniform(const std::string_view& name, float v0, float v1)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1);
	}

	void shader_program::set_uniform(const std::string_view& name, float v0, float v1, float v2)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2);
	}

	void shader_program::set_uniform(const std::string_view& name, float v0, float v1, float v2, float v3)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(const std::string_view& name, int32_t v0)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0);
	}

	void shader_program::set_uniform(const std::string_view& name, int32_t v0, int32_t v1)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1);
	}

	void shader_program::set_uniform(const std::string_view& name, int32_t v0, int32_t v1, int32_t v2)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2);
	}

	void shader_program::set_uniform(const std::string_view& name, int32_t v0, int32_t v1, int32_t v2, int32_t v3)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t v0)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1, uint32_t v2)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v0, v1, v2, v3);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 1>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 2>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 3>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 4>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 1>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 2>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 3>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 4>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 1>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 2>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 3>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 4>& v)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, count, v);
	}

	void shader_program::set_uniform(const std::string_view& name, const matrix4f& v, bool transpose)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v, transpose);
	}

	void shader_program::set_uniform(const std::string_view& name, const matrix4f* v[], size_t size, bool transpose)
	{
		auto loc = get_uniform_location(name);

		set_uniform(loc, v, size, transpose);
	}

	void shader_program::delete_handle(GLuint handle)
	{
		glDeleteProgram(handle);
	}
}
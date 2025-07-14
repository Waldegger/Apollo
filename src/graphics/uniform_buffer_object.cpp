#include "graphics/uniform_buffer_object.h"

#include <glad/glad.h>

#include "utility/gl_check.h"

namespace age
{
	uniform_buffer_object::uniform_buffer_object()
		: m_handle{ gen_handle() }
	{}

	void uniform_buffer_object::bind() const
	{
		auto handle = get_handle();

		if (handle != m_current_bound_uniform_buffer_object)
		{
			GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, handle));

			m_current_bound_uniform_buffer_object = handle;
		}
	}

	void uniform_buffer_object::buffer_data(size_t size, const void* data)
	{
		bind();

		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void uniform_buffer_object::buffer_sub_data(size_t offset, size_t size, const void* data)
	{
		bind();

		GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
	}

	void uniform_buffer_object::bind_buffer_base(uint32_t index)
	{
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, get_handle()));
	}

	void uniform_buffer_object::bind_buffer_range(uint32_t index, size_t offset, size_t size)
	{
		GL_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, index, get_handle(), offset, size));
	}

	uint32_t uniform_buffer_object::gen_handle()
	{
		GLuint handle;

		GL_CALL(glGenBuffers(1, &handle));

		return handle;
	}

	void uniform_buffer_object::delete_handle(uint32_t handle)
	{
		GL_CALL(glDeleteBuffers(1, &handle));
	}
}
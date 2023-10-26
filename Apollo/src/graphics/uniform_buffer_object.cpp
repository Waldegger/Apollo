#include "uniform_buffer_object.h"

#include <GL/glew.h>

namespace age
{
	uniform_buffer_object::uniform_buffer_object()
		: m_handle{ gen_handle() }
	{}

	void uniform_buffer_object::bind() const
	{
		auto handle = get_handle();

		if (handle != m_current_bound_unfiform_buffer_object)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, handle);

			m_current_bound_unfiform_buffer_object = handle;
		}
	}

	void uniform_buffer_object::buffer_data(size_t size, const void* data)
	{
		bind();

		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void uniform_buffer_object::buffer_sub_data(size_t offset, size_t size, const void* data)
	{
		bind();

		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	void uniform_buffer_object::bind_buffer_base(uint32_t index)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, index, get_handle());
	}

	void uniform_buffer_object::bind_buffer_range(uint32_t index, size_t offset, size_t size)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, index, get_handle(), offset, size);
	}

	uint32_t uniform_buffer_object::gen_handle()
	{
		GLuint handle;

		glGenBuffers(1, &handle);

		return handle;
	}

	void uniform_buffer_object::delete_handle(uint32_t handle)
	{
		glDeleteBuffers(1, &handle);
	}
}
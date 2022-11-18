#include "vertex_buffer_object.h"

namespace apollo
{
	vertex_buffer_object::vertex_buffer_object(target target)
		: m_handle{ create_handle() }
		, m_target{ target }
	{

	}

	void vertex_buffer_object::bind()
	{
		glBindBuffer(static_cast<GLenum>(m_target), get_handle());
	}

	void vertex_buffer_object::buffer_data(void* data, size_t size_in_bytes, usage usage)
	{
		bind();

		glBufferData(static_cast<GLenum>(m_target), size_in_bytes, data, static_cast<GLenum>(usage));

		m_last_buffer_size = size_in_bytes;
		m_last_buffer_usage = usage;
	}

	void vertex_buffer_object::update_data(void* data, size_t size_in_bytes, usage usage)
	{
		bind();

		glBufferData(static_cast<GLenum>(m_target), m_last_buffer_size, nullptr, static_cast<GLenum>(m_last_buffer_usage));
		glBufferData(static_cast<GLenum>(m_target), size_in_bytes, data, static_cast<GLenum>(usage));
	}

	void vertex_buffer_object::buffer_sub_data(void* data, size_t offset, size_t size_in_bytes)
	{
		bind();

		glBufferSubData(static_cast<GLenum>(m_target), offset, size_in_bytes, data);
	}

	GLuint vertex_buffer_object::create_handle()
	{
		GLuint handle{};
		glGenBuffers(1, &handle);

		return handle;
	}

	void vertex_buffer_object::delete_handle(GLuint handle)
	{
		glDeleteBuffers(1, &handle);
	}
}
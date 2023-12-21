#include "vertex_array_object.h"

#include <GL/glew.h>

namespace age
{
	vertex_array_object::vertex_array_object()
		: m_handle(create_handle())
	{}

	void vertex_array_object::bind() const
	{
		auto handle = get_handle();

		if (m_current_bound_array != handle)
		{
			glBindVertexArray(get_handle());

			m_current_bound_array = handle;
		}
	}

	void vertex_array_object::release() const
	{
		auto handle = get_handle();

		if (m_current_bound_array == handle)
		{
			glBindVertexArray(0);

			m_current_bound_array = 0;
		}
	}

	uint32_t vertex_array_object::create_handle()
	{
		GLuint handle;

		glGenVertexArrays(1, &handle);

		return handle;
	}

	void vertex_array_object::delete_handle(uint32_t handle)
	{
		glDeleteVertexArrays(1, &handle);
	}
}
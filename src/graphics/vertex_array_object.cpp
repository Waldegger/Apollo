#include "graphics/vertex_array_object.h"

#include <glad/glad.h>

#include "utility/gl_check.h"

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
			GL_CALL(glBindVertexArray(get_handle()));

			m_current_bound_array = handle;
		}
	}

	void vertex_array_object::release() const
	{
		auto handle = get_handle();

		if (m_current_bound_array == handle)
		{
			GL_CALL(glBindVertexArray(0));

			m_current_bound_array = 0;
		}
	}

	uint32_t vertex_array_object::create_handle()
	{
		GLuint handle;

		GL_CALL(glGenVertexArrays(1, &handle));

		return handle;
	}

	void vertex_array_object::delete_handle(uint32_t handle)
	{
		GL_CALL(glDeleteVertexArrays(1, &handle));
	}
}
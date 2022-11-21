#pragma once

#include <GL/glew.h>

#include "../utility/utility.h"

namespace agl
{
	class vertex_buffer_object
	{
	public:
		enum class target : GLuint
		{
			array = GL_ARRAY_BUFFER,
			element_array = GL_ELEMENT_ARRAY_BUFFER
		};

		enum class usage : GLenum
		{
			stream_draw = GL_STREAM_DRAW,
			static_draw = GL_STATIC_DRAW,
			dynamic_draw = GL_DYNAMIC_DRAW
		};

		vertex_buffer_object(target target);

	public:
		inline void set_target(target value) noexcept { m_target = value; }
		inline target get_target() const noexcept{ return m_target; }

		void bind();
		
		void buffer_data(void* data, size_t size_in_bytes, usage usage);
		void update_data(void* data, size_t size_in_bytes, usage usage);
		void buffer_sub_data(void* data, size_t offset, size_t size_in_bytes);

	protected:

	private:
		inline static uint32_t m_current_bound_buffer;

		GLuint get_handle() const { return m_handle; }

		static GLuint create_handle();
		static void delete_handle(GLuint handle);
		unique_handle <GLuint, delete_handle> m_handle;

		target m_target;

		size_t m_last_buffer_size{};
		usage m_last_buffer_usage = usage::static_draw;
	};
}
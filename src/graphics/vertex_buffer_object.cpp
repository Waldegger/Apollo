#include "graphics/vertex_buffer_object.h"

#include <GL/glew.h>

#include <stdexcept>

namespace age
{
	vertex_buffer_object::vertex_buffer_object(target target)
		: m_handle{ create_handle() }
		, m_target{ target }
	{}

	void vertex_buffer_object::bind() const
	{
		auto handle = get_handle();
		auto& current_bound_buffer = m_current_bound_buffer[static_cast<uint32_t>(m_target)];

		if (current_bound_buffer != handle)
		{
			glBindBuffer(convert_target(m_target), get_handle());

			current_bound_buffer = handle;
		}
	}

	void vertex_buffer_object::buffer_data(const void* data, size_t size_in_bytes, usage usage)
	{
		bind();

		glBufferData(convert_target(m_target), size_in_bytes, data, convert_usage(usage));

		m_last_buffer_size[static_cast<uint32_t>(m_target)] = size_in_bytes;
		m_last_buffer_usage[static_cast<uint32_t>(m_target)] = usage;
	}

	void vertex_buffer_object::update_data(const void* data, size_t size_in_bytes, usage usage)
	{
		bind();

		auto target = convert_target(m_target);

		glBufferData(target, m_last_buffer_size[static_cast<uint32_t>(m_target)], nullptr, convert_usage(m_last_buffer_usage[static_cast<uint32_t>(m_target)]));
		glBufferData(target, size_in_bytes, data, convert_usage(usage));

		m_last_buffer_size[static_cast<uint32_t>(m_target)] = size_in_bytes;
		m_last_buffer_usage[static_cast<uint32_t>(m_target)] = usage;
	}

	void vertex_buffer_object::buffer_sub_data(const void* data, size_t offset, size_t size_in_bytes)
	{
		bind();

		glBufferSubData(convert_target(m_target), offset, size_in_bytes, data);
	}

	uint32_t vertex_buffer_object::convert_target(target target_to_convert)
	{
		switch (target_to_convert)
		{
			case target::array:
				return GL_ARRAY_BUFFER;
			case target::element_array:
				return GL_ELEMENT_ARRAY_BUFFER;
			default:
				throw std::runtime_error{ "VERTEX_BUFFER_OBJECT::CONVERT_TARGET INVALID TARGET!" };
		}
	}

	uint32_t vertex_buffer_object::convert_usage(usage usage_to_convert)
	{
		switch (usage_to_convert)
		{
			case usage::stream_draw:
				return GL_STREAM_DRAW;
			case usage::static_draw:
				return GL_STATIC_DRAW;
			case usage::dynamic_draw:
				return GL_DYNAMIC_DRAW;
			default:
				throw std::runtime_error{ "VERTEX_BUFFER_OBJECT::CONVERT_USAGE INVALID USAGE!" };
		}
	}

	uint32_t vertex_buffer_object::create_handle()
	{
		GLuint handle{};
		glGenBuffers(1, &handle);

		return handle;
	}

	void vertex_buffer_object::delete_handle(uint32_t handle)
	{
		glDeleteBuffers(1, &handle);
	}
}
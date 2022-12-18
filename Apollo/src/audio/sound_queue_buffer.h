#pragma once

#include <stdint.h>

#include "sound_buffer.h"

namespace age
{
	class sound_source;

	class sound_queue_buffer
	{
	public:
		friend class sound_source;

		sound_queue_buffer(uint32_t handle);
		sound_queue_buffer(const sound_buffer& buffer);

	public:
		void buffer_data(sound_buffer::format the_format, const std::byte data[], size_t size_in_bytes, uint32_t frequency);

	protected:

	private:
		uint32_t get_handle() const { return m_handle; }

		uint32_t m_handle{};
	};
}
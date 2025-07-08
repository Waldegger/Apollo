#include "audio/sound_queue_buffer.h"

#include <AL/al.h>

namespace age
{
	sound_queue_buffer::sound_queue_buffer(uint32_t handle)
		: m_handle{ handle }
	{}

	sound_queue_buffer::sound_queue_buffer(const sound_buffer& buffer)
		: m_handle{ buffer.get_handle() }
	{}

	void sound_queue_buffer::buffer_data(sound_buffer::format the_format, const std::byte data[], size_t size_in_bytes, uint32_t frequency)
	{
		alBufferData(m_handle, sound_buffer::format_to_AL_enum(the_format), data, static_cast<ALsizei>(size_in_bytes), frequency);
	}
}
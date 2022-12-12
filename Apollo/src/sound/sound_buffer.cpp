#include "sound_buffer.h"

#include <AL/al.h>

namespace age
{
	sound_buffer::sound_buffer()
		: m_handle{ gen_handle() }
	{}

	uint32_t sound_buffer::gen_handle()
	{
		ALuint name = 0;
		alGenBuffers(1, &name);

		return name;
	}

	void sound_buffer::delete_handle(uint32_t handle)
	{
		ALuint name = handle;

		alDeleteBuffers(1, &name);
	}
}
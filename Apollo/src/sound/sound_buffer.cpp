#include "sound_buffer.h"

#include <AL/al.h>

#include <array>
#include <type_traits>

#include "../system/assetstream.h"

namespace age
{
	sound_buffer::sound_buffer()
		: m_handle{ gen_handle() }
	{}

	void sound_buffer::load(std::string_view fn)
	{
		assetistream is{ fn.data(), std::ios::binary };
		load(is);
	}

	void sound_buffer::load(std::istream& is)
	{
		
	}

	void sound_buffer::load(std::byte data[], size_t size_in_bytes)
	{

	}

	void sound_buffer::buffer_data(format the_format, const uint8_t data[], size_t size_in_bytes, uint32_t frequency)
	{
		std::array<ALenum, 4> format_names{ AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16 };
		ALenum format = format_names[static_cast<std::underlying_type_t<decltype(the_format)>>(the_format)];

		alBufferData(m_handle, format, data, static_cast<ALsizei>(size_in_bytes), frequency);
	}

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
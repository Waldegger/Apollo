#include "sound_buffer.h"

#include <AL/al.h>

#include <array>
#include <type_traits>
#include <stdexcept>

#include "audio_device.h"
#include "audio_format.h"
#include "sound_file_wave.h"
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
		switch (audio_format::get_format(is))
		{
			case audio_format::format::wave:
			{
				sound_file_wave wave_file;
				wave_file.load(is);

				format the_format = format::mono_8;

				if (wave_file.get_header().num_of_chan == 1 && wave_file.get_header().bits_per_sample == 8)
					the_format = format::mono_8;
				else if (wave_file.get_header().num_of_chan == 1 && wave_file.get_header().bits_per_sample == 16)
					the_format = format::mono_16;
				else if (wave_file.get_header().num_of_chan == 2 && wave_file.get_header().bits_per_sample == 8)
					the_format = format::stereo_8;
				else if (wave_file.get_header().num_of_chan == 2 && wave_file.get_header().bits_per_sample == 16)
					the_format = format::stereo_16;
				else
					throw std::runtime_error{ "Unrecognised wave format" };

				buffer_data(the_format, wave_file.get_data().data(), wave_file.get_data().size(), wave_file.get_header().samples_per_sec);
			}
			break;

			default:
			{
				throw std::runtime_error{ "Not supported format" };
			}
			break;
		}
	}

	void sound_buffer::load(std::byte data[], size_t size_in_bytes)
	{

	}

	void sound_buffer::buffer_data(format the_format, const std::byte data[], size_t size_in_bytes, uint32_t frequency)
	{
		alBufferData(m_handle, format_to_AL_enum(the_format), data, static_cast<ALsizei>(size_in_bytes), frequency);
	}

	int32_t sound_buffer::format_to_AL_enum(format the_format)
	{
		std::array<ALenum, 4> format_names{ AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16 };
		return format_names[static_cast<std::underlying_type_t<decltype(the_format)>>(the_format)];
	}

	uint32_t sound_buffer::gen_handle()
	{
		m_num_buffers++;

		if (m_num_buffers == 1)
		{
			if (!audio_device::get().is_initialised())
				audio_device::get().init();
		}

		ALuint name = 0;
		alGenBuffers(1, &name);

		return name;
	}

	void sound_buffer::delete_handle(uint32_t handle)
	{
		m_num_buffers--;

		if (!m_num_buffers)
		{
			if (audio_device::get().is_initialised())
				audio_device::get().destroy();

		}

		ALuint name = handle;

		alDeleteBuffers(1, &name);
	}
}
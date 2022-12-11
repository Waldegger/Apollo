#include "sound.h"

#include "sound_source.h"
#include "audio_device.h"

namespace age
{
	sound::sound()
		: m_position{}
		, m_owned_source{ nullptr }
		, m_pitch{ 1.0f }
		, m_volume{ 1.0f }
		, m_min_distance{ 1.0f }
		, m_attenuation{ 1.0f }
		, m_relative_to_listener{ false }
	{}

	void sound::play()
	{
		auto source = audio_device::get().get_free_source();

		if(m_owned_source)
			m_owned_source->set_owning_sound(nullptr);

		m_owned_source = source;

		if (m_owned_source)
		{
			m_owned_source->set_owning_sound(this);

			//start playing the sound here
		}	
	}

	void sound::set_owned_source(sound_source* value)
	{
		m_owned_source = value;
	}

	sound_source* sound::get_owned_source() const
	{
		return m_owned_source;
	}
}
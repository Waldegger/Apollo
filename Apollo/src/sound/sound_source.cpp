#include "sound_source.h"

#include <AL/al.h>

#include "sound.h"

namespace age
{
	sound_source::sound_source()
		: m_owning_sound{ nullptr }
		, m_handle{ gen_handle() }
	{}

	sound_source::state sound_source::get_state() const
	{
		ALint state;

		alGetSourcei(m_handle, AL_SOURCE_STATE, &state);

		switch (state)
		{
		case AL_PLAYING:
			return state::playing;
		case AL_PAUSED:
			return state::paused;
		case AL_STOPPED:
		case AL_INITIAL:
			return state::stopped;
		default:
			return state::stopped;
		}
	}

	void sound_source::set_owning_sound(sound* value)
	{
		m_owning_sound = value;
	}

	sound* sound_source::get_owning_sound() const
	{
		return m_owning_sound;
	}

	uint32_t sound_source::gen_handle()
	{
		ALuint name = 0;
		alGenSources(1, &name);

		return name;
	}

	void sound_source::delete_handle(uint32_t handle)
	{
		alDeleteSources(1, &handle);
	}
}
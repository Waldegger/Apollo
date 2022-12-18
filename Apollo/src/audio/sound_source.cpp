#include "sound_source.h"

#include <AL/al.h>

#include "sound_interface.h"
#include "sound_buffer.h"

namespace age
{
	sound_source::sound_source()
		: m_attached_sound{ nullptr }
		, m_handle{ gen_handle() }
	{}

	sound_source::sound_source(uint32_t handle)
		: m_attached_sound{ nullptr }
		, m_handle{ handle }
	{}

	void sound_source::play()
	{
		alSourcePlay(m_handle);
	}

	void sound_source::stop()
	{
		alSourceStop(m_handle);
	}

	void sound_source::pause()
	{
		alSourcePause(m_handle);
	}

	void sound_source::set_position(const vector3f& value)
	{
		alSource3f(m_handle, AL_POSITION, value.x, value.y, value.z);
	}

	vector3f sound_source::get_position() const
	{
		ALfloat value[3] = { 0.0f, 0.0f, 0.0f };
		alGetSource3f(m_handle, AL_POSITION, &value[0], &value[1], &value[2]);

		return vector3f{ value[0], value[1], value[2] };
	}

	void sound_source::set_pitch(float value)
	{
		alSourcef(m_handle, AL_PITCH, value);
	}

	float sound_source::get_pitch() const
	{
		ALfloat value{};
		alGetSourcef(m_handle, AL_PITCH, &value);

		return value;
	}

	void sound_source::set_volume(float value)
	{
		alSourcef(m_handle, AL_GAIN, value);
	}

	float sound_source::get_volume() const
	{
		ALfloat value{};
		alGetSourcef(m_handle, AL_GAIN, &value);

		return value;
	}

	void sound_source::set_min_distance(float value)
	{
		alSourcef(m_handle, AL_REFERENCE_DISTANCE, value);
	}

	float sound_source::get_min_distance() const
	{
		ALfloat value{};
		alGetSourcef(m_handle, AL_REFERENCE_DISTANCE, &value);

		return value;
	}

	void sound_source::set_attenuation(float value)
	{
		alSourcef(m_handle, AL_ROLLOFF_FACTOR, value);
	}

	float sound_source::get_attenuation() const
	{
		ALfloat value{};
		alGetSourcef(m_handle, AL_ROLLOFF_FACTOR, &value);

		return value;
	}

	void sound_source::set_relative_to_listener(bool value)
	{
		alSourcei(m_handle, AL_SOURCE_RELATIVE, value ? 1 : 0);
	}

	bool sound_source::get_relative_to_listener() const
	{
		ALint value{};
		alGetSourcei(m_handle, AL_SOURCE_RELATIVE, &value);

		return value == AL_TRUE;
	}

	void sound_source::set_looping(bool value)
	{
		alSourcei(m_handle, AL_LOOPING, value ? 1 : 0);
	}

	bool sound_source::get_looping() const
	{
		ALint value{};
		alGetSourcei(m_handle, AL_LOOPING, &value);

		return value == AL_TRUE;
	}

	void sound_source::set_buffer(const sound_buffer& value)
	{
		alSourcei(m_handle, AL_BUFFER, value.get_handle());
	}

	void sound_source::queue_buffer(sound_queue_buffer value)
	{
		ALuint handle = value.get_handle();

		alSourceQueueBuffers(m_handle, 1, &handle);
	}

	uint32_t sound_source::get_num_queued_buffers() const
	{
		ALint result = 0;

		alGetSourcei(m_handle, AL_BUFFERS_QUEUED, &result);

		return static_cast<uint32_t>(result);
	}

	uint32_t sound_source::get_num_processed_buffers() const
	{
		ALint result = 0;

		alGetSourcei(m_handle, AL_BUFFERS_PROCESSED, &result);

		return static_cast<uint32_t>(result);
	}

	sound_queue_buffer sound_source::unqueue_buffer()
	{
		ALuint handle = 0;

		alSourceUnqueueBuffers(m_handle, 1, &handle);

		return sound_queue_buffer{ handle };
	}

	void sound_source::clear_buffers()
	{
		alSourcei(m_handle, AL_BUFFER, AL_NONE);
	}

	sound_source::state sound_source::get_state() const
	{
		ALint state = AL_INITIAL;

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

	void sound_source::set_attached_sound(const sound_interface* value)
	{
		m_attached_sound = value;
	}

	const sound_interface* sound_source::get_attached_sound() const
	{
		return m_attached_sound;
	}

	void sound_source::detach_sound()
	{
		if (m_attached_sound)
		{
			m_attached_sound->attach_source(nullptr);
			m_attached_sound = nullptr;
		}
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
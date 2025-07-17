#include "audio/sound_source.h"

#include <AL/al.h>

#include "audio/sound_interface.h"
#include "audio/sound_buffer.h"

#include "utility/al_check.h"

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
		AL_CALL(alSourcePlay(m_handle));
	}

	void sound_source::stop()
	{
		AL_CALL(alSourceStop(m_handle));
	}

	void sound_source::pause()
	{
		AL_CALL(alSourcePause(m_handle));
	}

	void sound_source::rewind()
	{
		AL_CALL(alSourceRewind(m_handle));
	}

	void sound_source::set_position(const glm::vec3& value)
	{
		AL_CALL(alSource3f(m_handle, AL_POSITION, value.x, value.y, value.z));
	}

	glm::vec3 sound_source::get_position() const
	{
		ALfloat value[3] = { 0.0f, 0.0f, 0.0f };
		AL_CALL(alGetSource3f(m_handle, AL_POSITION, &value[0], &value[1], &value[2]));

		return glm::vec3{ value[0], value[1], value[2] };
	}

	void sound_source::set_pitch(float value)
	{
		AL_CALL(alSourcef(m_handle, AL_PITCH, value));
	}

	float sound_source::get_pitch() const
	{
		ALfloat value{};
		AL_CALL(alGetSourcef(m_handle, AL_PITCH, &value));

		return value;
	}

	void sound_source::set_volume(float value)
	{
		AL_CALL(alSourcef(m_handle, AL_GAIN, value));
	}

	float sound_source::get_volume() const
	{
		ALfloat value{};
		AL_CALL(alGetSourcef(m_handle, AL_GAIN, &value));

		return value;
	}

	void sound_source::set_min_distance(float value)
	{
		AL_CALL(alSourcef(m_handle, AL_REFERENCE_DISTANCE, value));
	}

	float sound_source::get_min_distance() const
	{
		ALfloat value{};
		AL_CALL(alGetSourcef(m_handle, AL_REFERENCE_DISTANCE, &value));

		return value;
	}

	void sound_source::set_attenuation(float value)
	{
		AL_CALL(alSourcef(m_handle, AL_ROLLOFF_FACTOR, value));
	}

	float sound_source::get_attenuation() const
	{
		ALfloat value{};
		AL_CALL(alGetSourcef(m_handle, AL_ROLLOFF_FACTOR, &value));

		return value;
	}

	void sound_source::set_relative_to_listener(bool value)
	{
		AL_CALL(alSourcei(m_handle, AL_SOURCE_RELATIVE, value ? 1 : 0));
	}

	bool sound_source::get_relative_to_listener() const
	{
		ALint value{};
		AL_CALL(alGetSourcei(m_handle, AL_SOURCE_RELATIVE, &value));

		return value == AL_TRUE;
	}

	void sound_source::set_looping(bool value)
	{
		AL_CALL(alSourcei(m_handle, AL_LOOPING, value ? 1 : 0));
	}

	bool sound_source::get_looping() const
	{
		ALint value{};
		AL_CALL(alGetSourcei(m_handle, AL_LOOPING, &value));

		return value == AL_TRUE;
	}

	void sound_source::set_buffer(const sound_buffer& value)
	{
		AL_CALL(alSourcei(m_handle, AL_BUFFER, value.get_handle()));
	}

	bool sound_source::has_buffer_attached(const sound_buffer& value) const
	{
		ALint bound = 0;

		AL_CALL(alGetSourcei(m_handle, AL_BUFFER, &bound));
		if (bound == value.get_handle())
			return true;

		return false;
	}

	void sound_source::detach_buffer(const sound_buffer& value)
	{
		if (has_buffer_attached(value))
		{
			if (auto state = get_state(); state == sound_state::playing || state == sound_state::paused)
				stop();

			AL_CALL(alSourcei(m_handle, AL_BUFFER, AL_NONE));
		}
	}

	void sound_source::queue_buffer(sound_queue_buffer value)
	{
		ALuint handle = value.get_handle();

		AL_CALL(alSourceQueueBuffers(m_handle, 1, &handle));
	}

	uint32_t sound_source::get_num_queued_buffers() const
	{
		ALint result = 0;

		AL_CALL(alGetSourcei(m_handle, AL_BUFFERS_QUEUED, &result));

		return static_cast<uint32_t>(result);
	}

	uint32_t sound_source::get_num_processed_buffers() const
	{
		ALint result = 0;

		AL_CALL(alGetSourcei(m_handle, AL_BUFFERS_PROCESSED, &result));

		return static_cast<uint32_t>(result);
	}

	sound_queue_buffer sound_source::unqueue_buffer()
	{
		ALuint handle = 0;

		AL_CALL(alSourceUnqueueBuffers(m_handle, 1, &handle));

		return sound_queue_buffer{ handle };
	}

	void sound_source::clear_buffers()
	{
		//stop if still playing
		if (auto state = get_state(); state == sound_state::playing || state == sound_state::paused)
			stop();

		//Get rid off all the queued buffers
		ALint queued = 0;
		AL_CALL(alGetSourcei(m_handle, AL_BUFFERS_QUEUED, &queued));
		while (queued--)
		{
			ALuint handle = 0;
			AL_CALL(alSourceUnqueueBuffers(m_handle, 1, &handle));
		}

		//also get rid of an eventual bound buffer
		AL_CALL(alSourcei(m_handle, AL_BUFFER, AL_NONE));
	}

	sound_state sound_source::get_state() const
	{
		ALint state = AL_INITIAL;

		AL_CALL(alGetSourcei(m_handle, AL_SOURCE_STATE, &state));

		switch (state)
		{
		case AL_PLAYING:
			return sound_state::playing;
		case AL_PAUSED:
			return sound_state::paused;
		case AL_STOPPED:
		case AL_INITIAL:
			return sound_state::stopped;
		default:
			return sound_state::stopped;
		}
	}

	void sound_source::set_attached_sound(sound_interface* value)
	{
		m_attached_sound = value;
	}

	sound_interface* sound_source::get_attached_sound() const
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
		AL_CALL(alGenSources(1, &name));

		return name;
	}

	void sound_source::delete_handle(uint32_t handle)
	{
		AL_CALL(alDeleteSources(1, &handle));
	}
}
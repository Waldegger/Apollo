#include "sound.h"

#include "sound_source.h"
#include "audio_device.h"

namespace age
{
	sound::sound()
		: m_position{}
		, m_owned_source{ nullptr }
		, m_buffer{ nullptr }
		, m_pitch{ 1.0f }
		, m_volume{ 1.0f }
		, m_min_distance{ 1.0f }
		, m_attenuation{ 1.0f }
		, m_relative_to_listener{ false }
	{}

	sound::sound(const sound& other)
		: m_position{ other.m_position }
		, m_owned_source{ nullptr }
		, m_buffer{ other.m_buffer }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
	{}

	sound::sound(sound&& other) noexcept
		: m_position{ other.m_position }
		, m_owned_source{ other.m_owned_source }
		, m_buffer{ other.m_buffer }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
	{
		other.m_owned_source = nullptr;

		if (m_owned_source)
			m_owned_source->set_owning_sound(this);
	}

	sound& sound::operator = (const sound& other)
	{
		m_position = other.m_position;
		m_buffer = other.m_buffer;
		m_pitch = other.m_pitch;
		m_volume = other.m_volume;
		m_min_distance = other.m_min_distance;
		m_attenuation = other.m_attenuation;
		m_relative_to_listener = other.m_relative_to_listener;

		return *this;
	}

	sound& sound::operator = (sound&& other) noexcept
	{
		m_position = other.m_position;
		m_owned_source = other.m_owned_source;
		m_buffer = other.m_buffer;
		m_pitch = other.m_pitch;
		m_volume = other.m_volume;
		m_min_distance = other.m_min_distance;
		m_attenuation = other.m_attenuation;
		m_relative_to_listener = other.m_relative_to_listener;

		other.m_owned_source = nullptr;
		if (m_owned_source)
			m_owned_source->set_owning_sound(this);

		return *this;
	}

	sound::~sound()
	{
		if (m_owned_source)
		{
			m_owned_source->set_owning_sound(nullptr);

			auto source_looping = m_owned_source->get_looping();

			//Our source is in the unavailable container. Make it available again
			if (source_looping || m_owned_source->get_state() == sound_source::state::paused)
			{
				m_owned_source->stop();

				if(source_looping)
					audio_device::get().make_source_available(m_owned_source);
			}
		}
	}

	void sound::play(bool looped) const
	{
		if (!m_buffer) return;

		if (m_owned_source)
		{
			if (m_owned_source->get_state() == sound_source::state::paused)
			{
				m_owned_source->play();
				return;
			}

			if (m_owned_source->get_looping())
			{
				m_owned_source->stop();
				audio_device::get().make_source_available(m_owned_source);
			}
		}

		if (!aquire_source(looped))
			return;

		m_owned_source->set_buffer(*m_buffer);
		m_owned_source->set_position(m_position);
		m_owned_source->set_pitch(m_pitch);
		m_owned_source->set_volume(m_volume);
		m_owned_source->set_min_distance(m_min_distance);
		m_owned_source->set_attenuation(m_attenuation);
		m_owned_source->set_relative_to_listener(m_relative_to_listener);
		m_owned_source->set_looping(looped);
		m_owned_source->play();
	}

	void sound::stop()
	{
		if (m_owned_source)
		{
			if (m_owned_source->get_looping())
			{
				audio_device::get().make_source_available(m_owned_source);
			}

			m_owned_source->stop();
		}
	}

	void sound::pause()
	{
		if (m_owned_source)
			m_owned_source->pause();
	}

	bool sound::aquire_source(bool permanent) const
	{
		reset_owned_source();

		m_owned_source = audio_device::get().get_free_source(permanent);

		if (m_owned_source)
		{
			m_owned_source->set_owning_sound(this);
			return true;
		}

		return false;
	}

	void sound::set_position(const vector3f& value)
	{
		m_position = value;
	}

	void sound::update_position(const vector3f& value)
	{
		if (!m_position.is_equal_to(value))
		{
			set_position(value);
			if (m_owned_source) m_owned_source->set_position(value);
		}
	}

	const vector3f& sound::get_position() const
	{
		return m_position;
	}

	void sound::set_buffer(const sound_buffer* value)
	{
		m_buffer = value;
	}

	const sound_buffer* sound::get_buffer() const
	{
		return m_buffer;
	}

	void sound::set_pitch(float value)
	{
		m_pitch = value;
	}

	void sound::update_pitch(float value)
	{
		if (m_pitch != value)
		{
			set_pitch(value);
			if (m_owned_source) m_owned_source->set_pitch(value);
		}
	}

	float sound::get_pitch() const
	{
		return m_pitch;
	}

	void sound::set_volume(float value)
	{
		m_volume = value;
	}

	void sound::update_volume(float value)
	{
		if (m_volume != value)
		{
			set_volume(value);
			if (m_owned_source) m_owned_source->set_pitch(value);
		}
	}

	float sound::get_volume() const
	{
		return m_volume;
	}

	void sound::set_min_distance(float value)
	{
		m_min_distance = value;
	}

	void sound::update_min_distance(float value)
	{
		if (m_min_distance != value)
		{
			set_min_distance(value);
			if (m_owned_source) m_owned_source->set_min_distance(value);
		}
	}

	float sound::get_min_distance() const
	{
		return m_min_distance;
	}

	void sound::set_attenuation(float value)
	{
		m_attenuation = value;
	}

	void sound::update_attenuation(float value)
	{
		if (m_attenuation != value)
		{
			set_attenuation(value);
			if (m_owned_source) m_owned_source->set_attenuation(value);
		}
	}

	float sound::get_attenuation() const
	{
		return m_attenuation;
	}

	void sound::set_relative_to_listener(bool value)
	{
		m_relative_to_listener = value;
	}

	void sound::update_relative_to_listener(bool value)
	{
		if (m_relative_to_listener != value)
		{
			set_relative_to_listener(value);
			if (m_owned_source) m_owned_source->set_relative_to_listener(value);
		}
	}

	bool sound::get_relative_to_listener() const
	{
		return m_relative_to_listener;
	}

	bool sound::get_looping() const
	{
		if(m_owned_source)
			return m_owned_source->get_looping();

		return false;
	}

	void sound::set_owned_source(sound_source* value) const
	{
		m_owned_source = value;
	}

	sound_source* sound::get_owned_source() const
	{
		return m_owned_source;
	}

	void sound::reset_owned_source() const
	{
		if (m_owned_source)
		{
			m_owned_source->set_owning_sound(nullptr);
		}
	}
}
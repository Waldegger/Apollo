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
		, m_looping{ false }
	{}

	sound::sound(const sound& other)
		: m_position{ other.m_position }
		, m_owned_source{ nullptr }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
		, m_looping{ false }
	{}

	sound::sound(sound&& other) noexcept
		: m_position{ other.m_position }
		, m_owned_source{ other.m_owned_source }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
		, m_looping{ other.m_looping }
	{
		other.m_owned_source = nullptr;

		if (m_owned_source)
			m_owned_source->set_owning_sound(this);
	}

	sound& sound::operator = (const sound& other)
	{
		m_position = other.m_position;
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
		m_pitch = other.m_pitch;
		m_volume = other.m_volume;
		m_min_distance = other.m_min_distance;
		m_attenuation = other.m_attenuation;
		m_relative_to_listener = other.m_relative_to_listener;
		m_looping = other.m_looping;

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
		}
	}

	void sound::play() const
	{
		if (!aquire_source())
			return;

		m_owned_source->set_pitch(m_pitch);
		m_owned_source->set_volume(m_volume);
		m_owned_source->set_min_distance(m_min_distance);
		m_owned_source->set_attenuation(m_attenuation);
		m_owned_source->set_relative_to_listener(m_relative_to_listener);
		m_owned_source->play();
	}

	bool sound::aquire_source() const
	{
		if (m_owned_source)
		{
			return true;
		}

		m_owned_source = audio_device::get().get_free_source();

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
		return m_looping;
	}

	void sound::set_owned_source(sound_source* value) const
	{
		m_owned_source = value;
	}

	sound_source* sound::get_owned_source() const
	{
		return m_owned_source;
	}
}
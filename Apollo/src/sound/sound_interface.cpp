#include "sound_interface.h"

#include "sound_source.h"
#include "audio_device.h"

namespace age
{
	sound_interface::sound_interface()
		: m_position{}
		, m_attached_source{ nullptr }
		, m_pitch{ 1.0f }
		, m_volume{ 1.0f }
		, m_min_distance{ 1.0f }
		, m_attenuation{ 1.0f }
		, m_relative_to_listener{ false }
	{}

	sound_interface::sound_interface(const sound_interface& other)
		: m_position{ other.m_position }
		, m_attached_source{ nullptr }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
	{}

	sound_interface::sound_interface(sound_interface&& other) noexcept
		: m_position{ other.m_position }
		, m_attached_source{ other.m_attached_source }
		, m_pitch{ other.m_pitch }
		, m_volume{ other.m_volume }
		, m_min_distance{ other.m_min_distance }
		, m_attenuation{ other.m_attenuation }
		, m_relative_to_listener{ other.m_relative_to_listener }
	{
		other.m_attached_source = nullptr;

		if (m_attached_source)
			m_attached_source->set_attached_sound(this);
	}

	sound_interface& sound_interface::operator = (const sound_interface& other)
	{
		m_position = other.m_position;
		m_pitch = other.m_pitch;
		m_volume = other.m_volume;
		m_min_distance = other.m_min_distance;
		m_attenuation = other.m_attenuation;
		m_relative_to_listener = other.m_relative_to_listener;

		return *this;
	}

	sound_interface& sound_interface::operator = (sound_interface&& other) noexcept
	{
		m_position = other.m_position;
		m_attached_source = other.m_attached_source;
		m_pitch = other.m_pitch;
		m_volume = other.m_volume;
		m_min_distance = other.m_min_distance;
		m_attenuation = other.m_attenuation;
		m_relative_to_listener = other.m_relative_to_listener;

		other.m_attached_source = nullptr;
		if (m_attached_source)
			m_attached_source->set_attached_sound(this);

		return *this;
	}

	void sound_interface::stop()
	{
		if (m_attached_source)
		{
			if (m_attached_source->get_looping())
			{
				audio_device::get().make_source_available(m_attached_source);
			}

			m_attached_source->stop();
		}
	}

	void sound_interface::update_source(sound_source& source, bool looped) const
	{
		source.set_position(m_position);
		source.set_pitch(m_pitch);
		source.set_volume(m_volume);
		source.set_min_distance(m_min_distance);
		source.set_attenuation(m_attenuation);
		source.set_relative_to_listener(m_relative_to_listener);
		source.set_looping(looped);
	}

	void sound_interface::set_position(const vector3f& value)
	{
		m_position = value;
	}

	void sound_interface::update_position(const vector3f& value)
	{
		if (!m_position.is_equal_to(value))
		{
			set_position(value);
			if (m_attached_source) m_attached_source->set_position(value);
		}
	}

	const vector3f& sound_interface::get_position() const
	{
		return m_position;
	}

	void sound_interface::set_pitch(float value)
	{
		m_pitch = value;
	}

	void sound_interface::update_pitch(float value)
	{
		if (m_pitch != value)
		{
			set_pitch(value);
			if (m_attached_source) m_attached_source->set_pitch(value);
		}
	}

	float sound_interface::get_pitch() const
	{
		return m_pitch;
	}

	void sound_interface::set_volume(float value)
	{
		m_volume = value;
	}

	void sound_interface::update_volume(float value)
	{
		if (m_volume != value)
		{
			set_volume(value);
			if (m_attached_source) m_attached_source->set_pitch(value);
		}
	}

	float sound_interface::get_volume() const
	{
		return m_volume;
	}

	void sound_interface::set_min_distance(float value)
	{
		m_min_distance = value;
	}

	void sound_interface::update_min_distance(float value)
	{
		if (m_min_distance != value)
		{
			set_min_distance(value);
			if (m_attached_source) m_attached_source->set_min_distance(value);
		}
	}

	float sound_interface::get_min_distance() const
	{
		return m_min_distance;
	}

	void sound_interface::set_attenuation(float value)
	{
		m_attenuation = value;
	}

	void sound_interface::update_attenuation(float value)
	{
		if (m_attenuation != value)
		{
			set_attenuation(value);
			if (m_attached_source) m_attached_source->set_attenuation(value);
		}
	}

	float sound_interface::get_attenuation() const
	{
		return m_attenuation;
	}

	void sound_interface::set_relative_to_listener(bool value)
	{
		m_relative_to_listener = value;
	}

	void sound_interface::update_relative_to_listener(bool value)
	{
		if (m_relative_to_listener != value)
		{
			set_relative_to_listener(value);
			if (m_attached_source) m_attached_source->set_relative_to_listener(value);
		}
	}

	bool sound_interface::get_relative_to_listener() const
	{
		return m_relative_to_listener;
	}

	bool sound_interface::get_looping() const
	{
		if (m_attached_source)
			return m_attached_source->get_looping();

		return false;
	}

	void sound_interface::attach_source(sound_source* value) const
	{
		if (m_attached_source == value) return;

		detach_source();
		m_attached_source = value;
		if (m_attached_source)
			m_attached_source->set_attached_sound(this);
	}

	sound_source* sound_interface::get_attached_source() const
	{
		return m_attached_source;
	}

	void sound_interface::detach_source() const
	{
		if (m_attached_source)
		{
			m_attached_source->set_attached_sound(nullptr);
			m_attached_source = nullptr;
		}
	}
}
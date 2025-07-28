#include "audio/audio_device.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdexcept>
#include <sstream>
#include <cstring>

#include "audio/sound.h"

#include "utility/al_check.h"

namespace age
{
	audio_device::audio_device()
		: m_device{ nullptr }
		, m_context{ nullptr }
		, m_is_initialised{ false }
	{

	}

	audio_device::~audio_device()
	{
		destroy_context_and_close_device();
	}

	sound_source* audio_device::get_free_source(bool for_permanent_use) const
	{
		sound_source* result = nullptr;

		std::lock_guard container_lock{ m_source_queue_mutex };

		for (auto it = m_available_sources.begin(); it != m_available_sources.end(); ++it)
		{
			auto& sound_source = *it;

			if (sound_source.get_state() == sound_state::stopped)
			{
				sound_source.detach_sound();

				if (for_permanent_use)
					m_unavailable_sources.splice(m_unavailable_sources.end(), m_available_sources, it);
				else
					m_available_sources.splice(m_available_sources.end(), m_available_sources, it);

				result = &sound_source;
				break;
			}
		}

		return result;
	}

	void audio_device::make_source_available(const sound_source* value)
	{
		std::lock_guard container_lock{ m_source_queue_mutex };

		for (auto it = m_unavailable_sources.begin(); it != m_unavailable_sources.end(); ++it)
		{
			if (&(*it) == value)
			{
				m_available_sources.splice(m_available_sources.end(), m_unavailable_sources, it);
				break;
			}
		}
	}

	void audio_device::stop_all_sounds()
	{
		for (auto& source : m_available_sources)
		{
			if (auto sound = source.get_attached_sound())
				sound->stop();
		}
	}

	void audio_device::remove_buffer_from_active_sources(const sound_buffer& buffer)
	{
		std::lock_guard container_lock{ m_source_queue_mutex };

		for (auto& source: m_available_sources)
		{
			source.detach_buffer(buffer);
		}
	}

	bool audio_device::is_initialised() const
	{
		return m_is_initialised;
	}

	audio_device& audio_device::get()
	{
		static audio_device audio_device_instance;
		return audio_device_instance;
	}

	std::vector<std::string_view> audio_device::get_device_names()
	{
		auto device_specifier_string = AL_CALL(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));
		auto result = std::vector<std::string_view>{};
		result.reserve(8);

		{
			size_t index = 0;
			do
			{
				result.push_back(&device_specifier_string[index]);
				index = strlen(&device_specifier_string[index]) + 1;

			} while (device_specifier_string[index] != '\0');
		}

		return result;
	}

	void audio_device::init()
	{
		std::lock_guard lock{ s_device_mutex };
		get().init(nullptr);
	}

	void audio_device::init(std::string_view device_name)
	{
		std::lock_guard lock{ s_device_mutex };
		get().init(device_name.data());
	}

	void audio_device::destroy()
	{
		std::lock_guard lock{ s_device_mutex };
		get().destroy_context_and_close_device();
	}

	void audio_device::set_listener_volume(float value)
	{
		m_listener_volume = value;
		AL_CALL(alListenerf(AL_GAIN, m_listener_volume));
	}

	float audio_device::get_listener_volume()
	{
		return m_listener_volume;
	}

	void audio_device::set_listener_position(const glm::vec3& value)
	{
		m_listener_position = value;
		AL_CALL(alListener3f(AL_POSITION, m_listener_position.x, m_listener_position.y, m_listener_position.z));
	}

	const glm::vec3& audio_device::get_listener_position()
	{
		return m_listener_position;
	}

	void audio_device::set_listener_direction(const glm::vec3& value)
	{
		m_listener_direction = value;
		
		float orientation[] = { m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z };

		AL_CALL(alListenerfv(AL_ORIENTATION, orientation));
	}

	const glm::vec3& audio_device::get_listener_direction()
	{
		return m_listener_direction;
	}

	void audio_device::set_listener_up_vector(const glm::vec3& value)
	{
		m_listener_up_vector = value;
		float orientation[] = { m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z };

		AL_CALL(alListenerfv(AL_ORIENTATION, orientation));
	}

	const glm::vec3& audio_device::get_listener_up_vector()
	{
		return m_listener_up_vector;
	}

	sound_source * audio_device::play_buffer(const sound_buffer &buffer, const sound_properties &properties) const
	{
		auto source = get_free_source(properties.looping);

		if (nullptr == source) return nullptr;

		source->set_buffer(buffer);
		source->set_position(properties.position);
		source->set_relative_to_listener(properties.relative_to_listener);
		source->set_volume(properties.volume);
		source->set_pitch(properties.pitch);
		source->set_attenuation(properties.attenuation);
		source->set_min_distance(properties.min_distance);
		source->set_looping(properties.looping);

		source->play();

		return source;
	}

	void audio_device::init(const char* device_name)
	{
		destroy_context_and_close_device();
		open_device_and_create_context(device_name);
		setup_sources();

		m_is_initialised = true;
	}

	void audio_device::open_device_and_create_context(const char* device_name)
	{
		const ALCchar* default_device = device_name ? device_name : alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		m_device = alcOpenDevice(default_device);

		if (!m_device)
		{
			ALCenum error = alcGetError(nullptr);
			std::stringstream ss;
			ss << "Failed to open audio device, with error: " << error;

			throw std::runtime_error{ ss.str() };
		}

		std::array<ALCint, 5> attributes = { ALC_MONO_SOURCES, MAX_SOURCES, ALC_STEREO_SOURCES, MAX_SOURCES, 0 };
		m_context = alcCreateContext(static_cast<ALCdevice*>(m_device), attributes.data());

		if (!m_context)
		{
			std::stringstream ss;
			ss << "Failed to create the audio context";

			throw std::runtime_error{ ss.str() };
		}

		AL_CALL(alcMakeContextCurrent(static_cast<ALCcontext*>(m_context)));

		// Apply the listener properties the user might have set
		float orientation[] = {m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z};
		AL_CALL(alListenerf(AL_GAIN, m_listener_volume));
		AL_CALL(alListener3f(AL_POSITION, m_listener_position.x, m_listener_position.y, m_listener_position.z));
		AL_CALL(alListenerfv(AL_ORIENTATION, orientation));
	}

	void audio_device::destroy_context_and_close_device()
	{
		if (m_context)
		{
			for (auto& source : m_available_sources)
				source.detach_sound();

			{
				std::lock_guard container_lock{ m_source_queue_mutex };

				m_available_sources.clear();
				m_unavailable_sources.clear();
			}

			alcMakeContextCurrent(nullptr);
			alcDestroyContext(static_cast<ALCcontext*>(m_context));
		}

		if (m_device)
			alcCloseDevice(static_cast<ALCdevice*>(m_device));

		m_context = nullptr;
		m_device = nullptr;
	}

	void audio_device::setup_sources()
	{
		std::lock_guard container_lock{ m_source_queue_mutex };
		for (uint32_t i = 0; i < MAX_SOURCES; ++i)
			m_available_sources.emplace_back();
	}
}
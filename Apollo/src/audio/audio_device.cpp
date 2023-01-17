#include "audio_device.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdexcept>
#include <sstream>

#include "sound.h"

namespace age
{
	audio_device::audio_device()
		: m_device{ nullptr }
		, m_context{ nullptr }
		, m_is_initialised{ false }
	{
		m_sound_sources.reserve(MAX_SOURCES);
		m_unvailable_sources.reserve(MAX_SOURCES);
	}

	audio_device::~audio_device()
	{
		destroy_context_and_close_device();
	}

	sound_source* audio_device::get_free_source(bool for_permanent_use) const
	{
		sound_source* result = nullptr;

		auto queue_pop_front = [](std::queue<sound_source*>& deque) -> sound_source*
		{
			auto result = deque.front();
			deque.pop();
			return result;
		};

		std::scoped_lock<std::mutex> container_lock{ m_source_queue_mutex };

		for (size_t i = 0; i < m_available_sources.size(); ++i)
		{
			auto source = queue_pop_front(m_available_sources);
			
			if (source->get_state() == sound_state::stopped)
			{
				source->detach_sound();

				if (for_permanent_use)
					m_unvailable_sources.push_back(source);
				else
					m_available_sources.push(source);

				result = source;
				break;
			}

			m_available_sources.push(source);
		}

		return result;
	}

	void audio_device::make_source_available(const sound_source* value)
	{
		std::scoped_lock<std::mutex> container_lock{ m_source_queue_mutex };

		for (auto it = m_unvailable_sources.begin(); it != m_unvailable_sources.end(); ++it)
		{
			if (*it == value)
			{
				m_available_sources.push(*it);
				m_unvailable_sources.erase(it);
				break;
			}
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
		auto device_specifier_string = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
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
		get().init(nullptr);
	}

	void audio_device::init(std::string_view device_name)
	{
		get().init(device_name.data());
	}

	void audio_device::destroy()
	{
		get().destroy_context_and_close_device();
	}

	void audio_device::set_listener_volume(float value)
	{
		m_listener_volume = value;
		alListenerf(AL_GAIN, m_listener_volume);
	}

	float audio_device::get_listener_volume()
	{
		return m_listener_volume;
	}

	void audio_device::set_listener_position(const vector3f& value)
	{
		m_listener_position = value;
		alListener3f(AL_POSITION, m_listener_position.x, m_listener_position.y, m_listener_position.z);
	}

	const vector3f& audio_device::get_listener_position()
	{
		return m_listener_position;
	}

	void audio_device::set_listener_direction(const vector3f& value)
	{
		m_listener_direction = value;
		
		float orientation[] = { m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z };

		alListenerfv(AL_ORIENTATION, orientation);
	}

	const vector3f& audio_device::get_listener_direction()
	{
		return m_listener_direction;
	}

	void audio_device::set_listener_up_vector(const vector3f& value)
	{
		m_listener_up_vector = value;
		float orientation[] = { m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z };

		alListenerfv(AL_ORIENTATION, orientation);
	}

	const vector3f& audio_device::get_listener_up_vector()
	{
		return m_listener_up_vector;
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
		m_device = alcOpenDevice(device_name);

		if (!m_device)
		{
			std::stringstream ss;
			ss << "Failed to open audio device";

			throw std::runtime_error{ ss.str() };
		}

		std::array<ALCint, 4> attributes = { ALC_MONO_SOURCES, MAX_SOURCES, ALC_STEREO_SOURCES, MAX_SOURCES };
		m_context = alcCreateContext(static_cast<ALCdevice*>(m_device), attributes.data());

		if (!m_context)
		{
			std::stringstream ss;
			ss << "Failed to create the audio context";

			throw std::runtime_error{ ss.str() };
		}

		alcMakeContextCurrent(static_cast<ALCcontext*>(m_context));

		// Apply the listener properties the user might have set
		float orientation[] = {m_listener_direction.x,
								m_listener_direction.y,
								m_listener_direction.z,
								m_listener_up_vector.x,
								m_listener_up_vector.y,
								m_listener_up_vector.z};
		alListenerf(AL_GAIN, m_listener_volume);
		alListener3f(AL_POSITION, m_listener_position.x, m_listener_position.y, m_listener_position.z);
		alListenerfv(AL_ORIENTATION, orientation);
	}

	void audio_device::destroy_context_and_close_device()
	{
		if (m_context)
		{
			for (auto& source : m_sound_sources)
				source.detach_sound();

			{
				std::scoped_lock<std::mutex> container_lock{ m_source_queue_mutex };

				while (!m_available_sources.empty()) m_available_sources.pop();
				m_unvailable_sources.clear();
			}
			
			m_sound_sources.clear();

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
		for (uint32_t i = 0; i < MAX_SOURCES; ++i)
			m_sound_sources.push_back(sound_source{});

		std::scoped_lock<std::mutex> container_lock{ m_source_queue_mutex };
		for (auto& source : m_sound_sources)
			m_available_sources.push(&source);
	}
}
#include "audio_device.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdexcept>
#include <sstream>

#include "sound.h"

namespace age
{
	static float listener_volume = 1.0f;
	static vector3f listener_position	{ 0.0f, 0.0f, 0.0f };
	static vector3f listener_direction	{ 0.0f, 0.0f, -1.0f };
	static vector3f listener_up_vector	{ 0.0f, 1.0f, 0.0f };

	audio_device::audio_device()
		: m_device{ nullptr }
		, m_context{ nullptr }
	{
		m_sound_sources.reserve(MAX_SOURCES);
		m_looping_sources.reserve(16);
	}

	audio_device::~audio_device()
	{
		destroy_context_and_close_device();
	}

	sound_source* audio_device::get_free_source() const
	{
		auto remove_owner_from_source = [](sound_source& source) -> void
		{
			if (source.get_owning_sound())
				source.get_owning_sound()->set_owned_source(nullptr);
		};

		auto deque_pop_front = [](std::deque<sound_source*>& deque) -> sound_source*
		{
			auto result = deque.front();
			deque.pop_front();
			return result;
		};

		for (size_t i = 0; i < m_available_sources.size(); ++i)
		{
			auto source = deque_pop_front(m_available_sources);
				
			if (source->get_state() == sound_source::state::stopped)
			{
				remove_owner_from_source(*source);
				return source;
			}	
		}

		for (size_t i = 0; i < m_music_sources.size(); ++i)
		{
			auto source = deque_pop_front(m_music_sources);
		
			if (source->get_state() == sound_source::state::stopped)
			{
				remove_owner_from_source(*source);
				return source;
			}
		}

		return nullptr;
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

		size_t index = 0;

		result.push_back(device_specifier_string);
		while (true)
		{
			if (device_specifier_string[index] == '\0')
			{
				++index;
				if (device_specifier_string[index] == '\0')
					break;

				result.push_back(&device_specifier_string[index]);
			}

			++index;
		}

		return result;
	}

	void audio_device::init()
	{
		get().init(nullptr);
	}

	void audio_device::init(const std::string_view& device_name)
	{
		get().init(device_name.data());
	}

	void audio_device::destroy()
	{
		get().destroy_context_and_close_device();
	}

	void audio_device::set_listener_volume(float value)
	{
		//ToDo: Implement me
	}

	float audio_device::get_listener_volume()
	{
		//ToDo: Implement me
		return 0.0f;
	}

	void audio_device::set_listener_position(const vector3f& value)
	{
		//ToDo: Implement me
	}

	const vector3f& audio_device::get_listener_position()
	{
		//ToDo: Implement me
		return vector3f{};
	}

	void audio_device::set_listener_direction(const vector3f& value)
	{
		//ToDo: Implement me
	}

	const vector3f& audio_device::get_listener_direction()
	{
		//ToDo: Implement me
		return vector3f{};
	}

	void audio_device::set_listener_up_vector(const vector3f& value)
	{
		//ToDo: Implement me
	}

	const vector3f& audio_device::get_listener_up_vector()
	{
		//ToDo: Implement me
		return vector3f{};
	}

	void audio_device::init(const char* device_name)
	{
		destroy_context_and_close_device();
		open_device_and_create_context(device_name);
		setup_sources();
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
		float orientation[] = {listener_direction.x,
								listener_direction.y,
								listener_direction.z,
								listener_up_vector.x,
								listener_up_vector.y,
								listener_up_vector.z};
		alListenerf(AL_GAIN, listener_volume);
		alListener3f(AL_POSITION, listener_position.x, listener_position.y, listener_position.z);
		alListenerfv(AL_ORIENTATION, orientation);
	}

	void audio_device::destroy_context_and_close_device()
	{
		if (m_context)
		{
			m_available_sources.clear();
			m_sound_sources.clear();
			m_music_sources.clear();
			m_looping_sources.clear();

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

		for (auto& source : m_sound_sources)
			m_available_sources.push_back(&source);
	}
}
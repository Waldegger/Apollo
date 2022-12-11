#pragma once

#include <array>
#include <vector>
#include <deque>
#include <string_view>

#include "sound_source.h"
#include "../algebra/vector3.h"

namespace age
{
	class sound;
	class sound_source;

	class audio_device
	{
	public:
		~audio_device();
	public:
		static audio_device& get();
		static std::vector<std::string_view> get_device_names();

		static void init();
		static void init(const std::string_view& device_name);
		static void destroy();

		static void set_listener_volume(float value);
		static float get_listener_volume();

		static void set_listener_position(const vector3f& value);
		static const vector3f& get_listener_position();
		
		static void set_listener_direction(const vector3f& value);
		static const vector3f& get_listener_direction();

		static void set_listener_up_vector(const vector3f& value);
		static const vector3f& get_listener_up_vector();

		sound_source* get_free_source() const;
	protected:

	private:
		inline static constexpr uint32_t MAX_SOURCES = 255;

		audio_device();
		audio_device(const audio_device& other) = delete;
		audio_device(audio_device&& other) = delete;

		void init(const char* device_name);
		void open_device_and_create_context(const char* device_name);
		void destroy_context_and_close_device();

		void setup_sources();

		void* m_device;
		void* m_context;

		std::vector<sound_source> m_sound_sources;
		mutable std::deque<sound_source*> m_available_sources;
		mutable std::deque<sound_source*> m_music_sources;
		mutable std::vector<sound_source*> m_looping_sources;
	};
}
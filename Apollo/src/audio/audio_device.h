#pragma once

#include <array>
#include <vector>
#include <queue>
#include <string_view>
#include <mutex>

#include "sound_source.h"
#include <glm/vec3.hpp>

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
		static void init(std::string_view device_name);
		static void destroy();

		static void set_listener_volume(float value);
		static float get_listener_volume();

		static void set_listener_position(const glm::vec3& value);
		static const glm::vec3& get_listener_position();
		
		static void set_listener_direction(const glm::vec3& value);
		static const glm::vec3& get_listener_direction();

		static void set_listener_up_vector(const glm::vec3& value);
		static const glm::vec3& get_listener_up_vector();

		sound_source* get_free_source(bool for_permanent_use = false) const;
		void make_source_available(const sound_source* value);

		void stop_all_sounds();

		bool is_initialised() const;
	protected:

	private:
		inline static constexpr uint32_t MAX_SOURCES = 255;

		inline static float m_listener_volume = 1.0f;
		inline static glm::vec3 m_listener_position{ 0.0f, 0.0f, 0.0f };
		inline static glm::vec3 m_listener_direction{ 0.0f, 0.0f, -1.0f };
		inline static glm::vec3 m_listener_up_vector{ 0.0f, 1.0f, 0.0f };

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
		mutable std::queue<sound_source*> m_available_sources;
		mutable std::vector<sound_source*> m_unvailable_sources;
		mutable std::mutex m_source_queue_mutex;

		bool m_is_initialised;
	};
}
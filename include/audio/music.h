#pragma once

#include "sound_interface.h"

#include <string_view>
#include <istream>
#include <vector>
#include <array>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "sound_buffer.h"
#include "sound_source.h"
#include "sound_stream.h"
#include "../system/background_worker.h"

namespace age
{
	class music
		: public sound_interface
	{
	public:
		music();
		music(const music& other) = delete;
		music(music&& other) noexcept = default;

		music& operator = (const music& other) = delete;
		music& operator = (music&& other) noexcept = delete;

		~music() override;
	public:
		void play(bool looped = false) override;
		void stop() override;
		void pause() override;

		void open(std::string_view fn);
		void open(std::istream& is);
		void open(std::unique_ptr<std::istream> is);
		void open(std::byte data[], size_t size);

		sound_state get_state() const;

		void update_position(const glm::vec3& value) override;
		void update_pitch(float value) override;
		void update_volume(float value) override;
		void update_min_distance(float value) override;
		void update_attenuation(float value) override;
		void update_relative_to_listener(bool value) override;

	protected:

	private:
		inline static constexpr size_t NUM_BUFFERS = 4;
		//inline static constexpr size_t BUFFER_SAMPLES = 65536;
		inline static constexpr size_t BUFFER_SAMPLES = 8192;

		void open_from_stream(std::istream& is);

		void buffer_play_and_stream(bool looped = false);

		mutable std::mutex m_source_mutex;
		mutable std::mutex m_stream_mutex;

		std::condition_variable m_buffer_cv;

		std::array<sound_buffer, NUM_BUFFERS> m_buffers;
		std::vector<std::byte> m_samples_buffer;

		sound_stream::info m_sound_stream_info;

		background_worker m_background_worker;
		std::unique_ptr<std::istream> m_istream;
		std::unique_ptr<sound_stream> m_sound_stream;

		std::atomic<sound_state> m_requested_state;
	};
}
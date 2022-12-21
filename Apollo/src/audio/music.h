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
		music& operator = (music&& other) noexcept = default;

		virtual ~music() override;
	public:
		virtual void play(bool looped = false) override;
		virtual void stop() override;
		virtual void pause() override;

		void open(std::string_view fn);
		void open(std::istream& is);
		void open(std::unique_ptr<std::istream> is);
		void open(std::byte data[], size_t size);
	protected:

	private:
		inline static constexpr size_t NUM_BUFFERS = 4;
		inline static constexpr size_t BUFFER_SAMPLES = 8192;

		void open_from_stream(std::istream& is);

		void buffer_play_and_stream(bool looped = false);

		mutable std::mutex m_source_state_mutex;

		std::mutex m_buffer_mutex;
		std::condition_variable m_buffer_cv;

		std::array<sound_buffer, NUM_BUFFERS> m_buffers;
		std::vector<std::byte> m_samples_buffer;

		sound_stream::info m_sound_stream_info;

		background_worker m_background_worker;
		std::unique_ptr<std::istream> m_istream;
		std::unique_ptr<sound_stream> m_sound_stream;
	};
}
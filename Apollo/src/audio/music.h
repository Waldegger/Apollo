#pragma once

#include "sound_interface.h"

#include <string_view>
#include <istream>
#include <array>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "sound_buffer.h"
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

		void buffer_play_and_stream(bool looped = false);

		mutable std::mutex m_source_state_mutex;

		std::array<sound_buffer, NUM_BUFFERS> m_buffers;

		background_worker m_background_worker;
		std::unique_ptr<std::istream> m_istream;
	};
}
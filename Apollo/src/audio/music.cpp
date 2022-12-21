#include "music.h"

#include <stdexcept>
#include <chrono>

#include "audio_device.h"
#include "sound_stream_factory.h"
#include "../system/assetstream.h"
#include "../system/memstream.h"

namespace age
{
	music::music()
	{
		m_samples_buffer.reserve(BUFFER_SAMPLES);
		set_relative_to_listener(true);
	}

	music::~music()
	{
		stop();
	}

	void music::play(bool looped)
	{
		if (!m_sound_stream_info.sample_count) return;

		auto source = get_attached_source();
		if (!source)
		{
			source = audio_device::get().get_free_source(true);
			attach_source(source);
		}

		if (source)
		{
			update_source(*source, false);

			switch (source->get_state())
			{
				case sound_source::state::stopped:
				{
					m_background_worker.add_job([this, looped]() -> void { buffer_play_and_stream(looped); });
				}
				break;

				case sound_source::state::paused:
				{
			
				}
				break;

				default:
				{
					//playing when the source is currently playing. What shall happen? Start new?
				}
				break;
			}
		}
	}

	void music::stop()
	{
		if (auto current_attached_source = get_attached_source())
		{
			std::scoped_lock<std::mutex> lock{ m_source_state_mutex };

			if(current_attached_source->get_state() != sound_source::state::stopped)
				current_attached_source->stop();
			
			current_attached_source->clear_buffers();
			audio_device::get().make_source_available(current_attached_source);
			detach_source();
		}
	}

	void music::pause()
	{
		auto current_attached_source = get_attached_source();

		if (current_attached_source)
		{
			std::scoped_lock<std::mutex> lock{ m_source_state_mutex };

			current_attached_source->pause();
		}
	}

	void music::open(std::string_view fn)
	{
		m_istream = std::make_unique<assetistream>(fn.data(), std::ios::binary);

		open_from_stream(*m_istream);
	}

	void music::open(std::istream& is)
	{
		m_istream.reset();

		open_from_stream(is);
	}

	void music::open(std::unique_ptr<std::istream> is)
	{
		m_istream = std::move(is);

		open_from_stream(*m_istream);
	}

	void music::open(std::byte data[], size_t size)
	{
		m_istream = std::make_unique<memistream>(data, size);

		open_from_stream(*m_istream);
	}

	void music::open_from_stream(std::istream& is)
	{
		m_sound_stream_info = sound_stream::info{};
		m_sound_stream = sound_stream_factory::create_from_stream(is);

		if (!m_sound_stream)
		{
			throw std::runtime_error{ "Unable to determin audio type from stream" };
		}
			
		m_sound_stream_info = m_sound_stream->open(is);
	}

	void music::buffer_play_and_stream(bool looped)
	{
		sound_buffer::format format = m_sound_stream_info.channel_count == 1 ? sound_buffer::format::mono_16 : sound_buffer::format::stereo_16;

		auto current_attached_source = get_attached_source();

		//first fill the buffers
		size_t filled_buffers = 0;
		for (auto& buffer : m_buffers)
		{
			size_t bytes_read = m_sound_stream->read(&m_samples_buffer[0], m_samples_buffer.size());

			if (!bytes_read) break;
			buffer.buffer_data(format, &m_samples_buffer[0], bytes_read, m_sound_stream_info.sample_rate);

			current_attached_source->queue_buffer(buffer);

			++filled_buffers;
		}

		//no buffers filled, no music to play
		if (!filled_buffers) return;

		current_attached_source->play();

		while (true)
		{
			//For the waiting, mulitply the pitch into the amount of miliseconds to wait
			std::unique_lock<std::mutex> wait_lock{ m_buffer_mutex };
			m_buffer_cv.wait_for(wait_lock, std::chrono::milliseconds{ 250 }, []() -> bool { return true; });

			uint32_t processed_buffers = current_attached_source->get_num_processed_buffers();

			while (processed_buffers)
			{
				--processed_buffers;
				size_t bytes_read = m_sound_stream->read(&m_samples_buffer[0], m_samples_buffer.size());

				//When there are no bytes read the stream has finished. For now we just exit the function.
				//When looped the file actually needs to be read again from the beginning and the buffer can be filled a bit more
				if (!bytes_read) return;

				auto processed_buffer = current_attached_source->unqueue_buffer();

				processed_buffer.buffer_data(format, &m_samples_buffer[0], bytes_read, m_sound_stream_info.sample_rate);

				current_attached_source->queue_buffer(processed_buffer);
			}
		}
	}
}
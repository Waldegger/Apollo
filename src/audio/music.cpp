#include "audio/music.h"

#include <stdexcept>
#include <chrono>

#include "audio/audio_device.h"
#include "audio/sound_stream_factory.h"
#include "system/assetstream.h"
#include "system/memstream.h"

namespace age
{
	music::music()
		: m_state{ sound_state::stopped }
	{
		m_samples_buffer.resize(BUFFER_SAMPLES);
		set_relative_to_listener(true);
	}

	music::~music()
	{
		stop();
	}

	void music::play(bool looped)
	{
		if (!m_sound_stream_info.sample_count) return;

		std::scoped_lock<std::mutex> source_lock{ m_source_mutex };
		auto current_attached_source = get_attached_source();
		if (!current_attached_source)
		{
			current_attached_source = audio_device::get().get_free_source(true);
			attach_source(current_attached_source);
		}

		if (current_attached_source)
		{
			update_source(*current_attached_source, false);

			switch (m_state)
			{
				case sound_state::stopped:
				{
					m_state = sound_state::playing;
					m_background_worker.add_job([this, looped]() -> void { buffer_play_and_stream(looped); });
				}
				break;

				case sound_state::paused:
				{
					m_state = sound_state::playing;
					m_buffer_cv.notify_one();
					current_attached_source->play();
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
		std::scoped_lock<std::mutex> lock{ m_source_mutex };

		if (auto current_attached_source = get_attached_source())
		{
			m_state = sound_state::stopped;
			m_buffer_cv.notify_one();

			if(current_attached_source->get_state() != sound_state::stopped)
				current_attached_source->stop();
			
			current_attached_source->clear_buffers();
			audio_device::get().make_source_available(current_attached_source);
			detach_source();
		}
	}

	void music::pause()
	{
		std::scoped_lock<std::mutex> lock{ m_source_mutex };

		auto current_attached_source = get_attached_source();

		if (current_attached_source)
		{
			m_state = sound_state::paused;
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

	sound_state music::get_state() const
	{
		return m_state;
	}

	void music::open_from_stream(std::istream& is)
	{
		stop();

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

		{
			std::scoped_lock<std::mutex> source_lock{ m_source_mutex };

			auto current_attached_source = get_attached_source();
			if (!current_attached_source) return;

			m_sound_stream->reset();

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
		}

		std::mutex wait_mutex;
		while (true)
		{
			//For the waiting, mulitply the pitch into the amount of miliseconds to wait
			std::unique_lock<std::mutex> wait_lock{ wait_mutex };
			m_buffer_cv.wait_for(wait_lock, std::chrono::milliseconds{ 50 }, [this]() -> bool { return m_state == sound_state::stopped; });
			m_buffer_cv.wait(wait_lock, [this]() -> bool { return m_state != sound_state::paused; });

			std::scoped_lock<std::mutex> source_lock{ m_source_mutex };
			auto current_attached_source = get_attached_source();

			if (m_state == sound_state::stopped || !current_attached_source) return;

			uint32_t processed_buffers = current_attached_source->get_num_processed_buffers();
			
			while (processed_buffers)
			{
				--processed_buffers;
				size_t bytes_read = m_sound_stream->read(&m_samples_buffer[0], m_samples_buffer.size());

				//When there are less bytes read than requested, the stream has finished.
				//When looped the file actually needs to be read again from the beginning and the buffer can be filled a bit more
				if (bytes_read < m_samples_buffer.size())
				{
					if (!looped)
					{
						goto finalize;
					}

					m_sound_stream->reset();
					size_t difference = m_samples_buffer.size() - bytes_read;
					bytes_read += m_sound_stream->read(&m_samples_buffer[bytes_read], difference);
				}

				auto processed_buffer = current_attached_source->unqueue_buffer();

				processed_buffer.buffer_data(format, &m_samples_buffer[0], bytes_read, m_sound_stream_info.sample_rate);

				current_attached_source->queue_buffer(processed_buffer);
			}

			//If there should have been a buffer underrun, just resume playing the source
			if (current_attached_source->get_state() == sound_state::stopped)
				current_attached_source->play();
		}

		finalize:
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
			std::scoped_lock<std::mutex> source_lock{ m_source_mutex };

			auto current_attached_source = get_attached_source();
			if (!current_attached_source) return;

			if (current_attached_source->get_state() == sound_state::stopped)
			{
				m_state = sound_state::stopped;

				audio_device::get().make_source_available(current_attached_source);
				detach_source();

				return;
			}
		}
	}
}
#include "audio/music.h"

#include <stdexcept>
#include <chrono>
#include <iostream>

#include "audio/audio_device.h"
#include "audio/sound_stream_factory.h"
#include "system/assetstream.h"
#include "system/memstream.h"

namespace age
{
	music::music()
		: m_requested_state{ sound_state::stopped }
	{
		m_samples_buffer.resize(BUFFER_SAMPLES);
		sound_interface::set_relative_to_listener(true);
		attach_source(&m_sound_source);
	}

	music::~music()
	{
		music::stop();
	}

	void music::play(bool looped)
	{
		switch (m_requested_state)
		{
			case sound_state::stopped:
			{
				m_requested_state = sound_state::playing;
				m_background_worker.add_job([this, looped]() -> void { buffer_play_and_stream(looped); });
			}
			break;

			case sound_state::paused:
			{
				m_requested_state = sound_state::playing;
				m_buffer_cv.notify_one();

				std::lock_guard source_lock{ m_source_mutex };
				m_sound_source.play();
			}
			break;

			default:
			{
				//playing when the source is currently playing. What shall happen? Start new?
			}
			break;
		}
	}

	void music::stop()
	{
		m_requested_state = sound_state::stopped;
		m_buffer_cv.notify_one();
	}

	void music::pause()
	{
		m_requested_state = sound_state::paused;
		m_buffer_cv.notify_one();

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.pause();
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
		return m_requested_state;
	}

	void music::update_position(const glm::vec3& value)
	{
		sound_interface::set_position(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_position(value);
	}

	void music::update_pitch(float value)
	{
		sound_interface::set_pitch(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_pitch(value);
	}

	void music::update_volume(float value)
	{
		sound_interface::set_volume(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_volume(value);
	}

	void music::update_min_distance(float value)
	{
		sound_interface::set_min_distance(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_min_distance(value);
	}

	void music::update_attenuation(float value)
	{
		sound_interface::set_attenuation(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_attenuation(value);
	}

	void music::update_relative_to_listener(bool value)
	{
		sound_interface::set_relative_to_listener(value);

		std::lock_guard source_lock{ m_source_mutex };
		m_sound_source.set_relative_to_listener(value);
	}

	void music::open_from_stream(std::istream& is)
	{
		stop();

		std::lock_guard stream_lock{ m_stream_mutex };

		m_sound_stream_info = sound_stream::info{};
		m_sound_stream = sound_stream_factory::create_from_stream(is);

		if (!m_sound_stream)
		{
			throw std::runtime_error{ "Unable to determine audio type from stream" };
		}
			
		m_sound_stream_info = m_sound_stream->open(is);
	}

	void music::buffer_play_and_stream(bool looped)
	{
		if (!m_sound_stream_info.sample_count) return;

		auto stop_source = [this]() -> void
		{
			m_sound_source.stop();
			m_sound_source.clear_buffers();
		};

		{
			std::lock_guard source_lock{ m_source_mutex };

			update_source(m_sound_source, false);


			//Buffer some data and start playing the music
			//first fill the buffers
			size_t filled_buffers = 0;
			for (auto& buffer : m_buffers)
			{
				//Lock the stream
				std::lock_guard stream_lock{ m_stream_mutex };

				switch (m_requested_state)
				{
				case sound_state::stopped:
					{
						stop_source();

						return;
					}
					break;
				case sound_state::paused:
					{
						if (m_sound_source.get_state() != sound_state::paused)
							m_sound_source.pause();
					}
					break;
				default:
					{

					}
					break;
				}

				sound_buffer::format format = m_sound_stream_info.channel_count == 1 ? sound_buffer::format::mono_16 : sound_buffer::format::stereo_16;
				size_t bytes_read = m_sound_stream->read(&m_samples_buffer[0], m_samples_buffer.size());

				if (!bytes_read) break;
				buffer.buffer_data(format, &m_samples_buffer[0], bytes_read, m_sound_stream_info.sample_rate);

				m_sound_source.queue_buffer(buffer);

				++filled_buffers;
			}

			//no buffers filled, no music to play
			if (!filled_buffers) return;
		}

		{
			{
				std::lock_guard source_lock{ m_source_mutex };
				m_sound_source.play();
			}

			std::mutex wait_mutex;

			while (true)
			{
				//ToDo: For the waiting duration, multiply the pitch into the amount of milliseconds to wait
				std::unique_lock wait_lock{ wait_mutex };
				m_buffer_cv.wait_for(wait_lock, std::chrono::milliseconds{ 50 }, [this]() -> bool { return m_requested_state == sound_state::stopped; });
				m_buffer_cv.wait(wait_lock, [this]() -> bool { return m_requested_state != sound_state::paused; });
				switch (m_requested_state)
				{
					case sound_state::stopped:
					{
						stop_source();
						return;
					}
					break;

					default:
					{}
				}

				uint32_t processed_buffers = 0;
				{
					std::lock_guard source_lock{ m_source_mutex };
					processed_buffers = m_sound_source.get_num_processed_buffers();
				}

				while (processed_buffers--)
				{
					if (m_requested_state == sound_state::stopped)
					{
						stop_source();
						return;
					}

					std::lock_guard stream_lock{ m_stream_mutex };

					size_t bytes_read = m_sound_stream->read(&m_samples_buffer[0], m_samples_buffer.size());

					//When there are fewer bytes read than requested, the stream has finished.
					//When looped the file actually needs to be read again from the beginning and the buffer can be filled a bit more
					if (bytes_read < m_samples_buffer.size())
					{
						if (!looped)
						{
							//nice loop until the last buffer stops playing
							while (true)
							{
								std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });

								std::lock_guard source_lock{ m_source_mutex };
								if (m_sound_source.get_state() == sound_state::stopped)
								{
									stop_source();
									return;
								}
							}
						}

						//Here we are when we want to loop. We just reset the strean and start a new
						m_sound_stream->reset();
						size_t difference = m_samples_buffer.size() - bytes_read;
						bytes_read += m_sound_stream->read(&m_samples_buffer[bytes_read], difference);
					}

					{
						std::lock_guard source_lock{ m_source_mutex };
						auto processed_buffer = m_sound_source.unqueue_buffer();

						sound_buffer::format format = m_sound_stream_info.channel_count == 1 ? sound_buffer::format::mono_16 : sound_buffer::format::stereo_16;
						processed_buffer.buffer_data(format, &m_samples_buffer[0], bytes_read, m_sound_stream_info.sample_rate);

						m_sound_source.queue_buffer(processed_buffer);

						//If there should have been a buffer underrun, just resume playing the source
						if (m_sound_source.get_state() == sound_state::stopped)
						{
							m_sound_source.play();
						}
					}
				}
			}
		}
	}
}
#include "music.h"

#include "audio_device.h"
#include "../system/assetstream.h"
#include "../system/memstream.h"

namespace age
{
	music::music()
	{
		set_relative_to_listener(true);
	}

	music::~music()
	{
		stop();
	}

	void music::play(bool looped)
	{
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
		m_istream = std::make_unique<assetistream>(fn.data());

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

	}

	void music::buffer_play_and_stream(bool looped)
	{

	}
}
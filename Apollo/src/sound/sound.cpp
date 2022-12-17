#include "sound.h"

#include "sound_source.h"
#include "audio_device.h"

namespace age
{
	sound::~sound()
	{
		auto current_attached_source = get_attached_source();

		if (current_attached_source)
		{
			current_attached_source->set_attached_sound(nullptr);

			auto source_looping = current_attached_source->get_looping();

			//Our source is in the unavailable container. Make it available again
			if (source_looping || current_attached_source->get_state() == sound_source::state::paused)
			{
				current_attached_source->stop();

				if (source_looping)
					audio_device::get().make_source_available(current_attached_source);
			}
		}
	}

	void sound::play(bool looped)
	{
		if (!m_buffer) return;

		auto current_attached_source = get_attached_source();
		if (current_attached_source)
		{
			if (current_attached_source->get_state() == sound_source::state::paused)
			{
				current_attached_source->play();
				return;
			}

			if (current_attached_source->get_looping())
			{
				current_attached_source->stop();
				audio_device::get().make_source_available(current_attached_source);
			}
		}

		sound_source* new_source = audio_device::get().get_free_source(looped);
		if (new_source)
		{
			update_source(*new_source, looped);
			new_source->set_buffer(*m_buffer);
			new_source->play();

			attach_source(new_source);
		}
	}

	void sound::stop()
	{
		auto current_attached_source = get_attached_source();

		if (current_attached_source)
		{
			if (current_attached_source->get_looping())
			{
				audio_device::get().make_source_available(current_attached_source);
			}

			current_attached_source->stop();
		}
	}

	void sound::pause()
	{
		auto current_attached_source = get_attached_source();

		if (current_attached_source)
			current_attached_source->pause();
	}

	void sound::set_buffer(const sound_buffer* value)
	{
		m_buffer = value;
	}

	const sound_buffer* sound::get_buffer() const
	{
		return m_buffer;
	}
}
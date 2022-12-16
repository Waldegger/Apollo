#pragma once
#include "sound_interface.h"

#include "../algebra/vector3.h"

namespace age
{
	class sound_source;
	class sound_buffer;
	
	class sound
		: public sound_interface
	{
	public:
		friend class audio_device;
		friend class sound_source;

		sound() = default;
		sound(const sound& other) = default;
		sound(sound&& other) noexcept = default;

		sound& operator = (const sound& other) = default;
		sound& operator = (sound&& other) noexcept = default;

		virtual ~sound() override;

	public:
		virtual void play(bool looped = false);
		virtual void stop();

		void set_buffer(const sound_buffer* value);
		const sound_buffer* get_buffer() const;

	protected:

	private:
		const sound_buffer* m_buffer;
	};
}
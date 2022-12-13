#pragma once

#include "../algebra/vector3.h"
#include "../utility/utility.h"

namespace age
{
	class sound;
	class sound_buffer;

	class sound_source
	{
	public:
		friend class audio_device;
		friend class sound;

		enum class state
		{
			playing,
			paused,
			stopped
		};

		sound_source();
		sound_source(uint32_t handle);

		sound_source(const sound_source& other) = delete;
		sound_source(sound_source&& other) = default;

		sound_source& operator = (const sound_source& other) = delete;
		sound_source& operator = (sound_source&& other) = default;

		~sound_source() = default;

	public:
		void play();
		void stop();
		void pause();
		
		void set_position(const vector3f& value);
		vector3f get_position() const;

		void set_pitch(float value);
		float get_pitch() const;

		void set_volume(float value);
		float get_volume() const;

		void set_min_distance(float value);
		float get_min_distance() const;

		void set_attenuation(float value);
		float get_attenuation() const;

		void set_relative_to_listener(bool value);
		bool get_relative_to_listener() const;

		void set_looping(bool value);
		bool get_looping() const;

		void set_buffer(const sound_buffer& value);
		const sound_buffer* get_buffer() const;

		state get_state() const;

	protected:

	private:
		void set_owning_sound(const sound* value);
		const sound* get_owning_sound() const;

		void reset_owning_sound();
		
		const sound* m_owning_sound;
		const sound_buffer* m_buffer;

		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);
		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
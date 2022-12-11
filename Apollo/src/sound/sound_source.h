#pragma once

#include "../algebra/vector3.h"
#include "../utility/utility.h"

namespace age
{
	class sound;

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

	public:
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

		state get_state() const;

	protected:

	private:

		void set_owning_sound(sound* value);
		sound* get_owning_sound() const;
		
		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);

		sound* m_owning_sound;
		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
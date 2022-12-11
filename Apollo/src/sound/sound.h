#pragma once

#include "../algebra/vector3.h"

namespace age
{
	class sound_source;
	
	class sound
	{
	public:
		friend class audio_device;

		sound();

	public:
		void play();

		void set_position(const vector3f& value);
		void update_position(const vector3f& value);
		const vector3f& get_position() const;

		void set_pitch(float value);
		void update_pitch(float value);
		float get_pitch() const;

		void set_volume(float value);
		void update_volume(float value);
		float get_volume() const;

		void set_min_distance(float value);
		void update_min_distance(float value);
		float get_min_distance() const;

		void set_attenuation(float value);
		void update_attenuation(float value);
		float get_attenuation() const;

		void set_relative_to_listener(bool value);
		void update_relative_to_listener(bool value);
		bool get_relative_to_listener() const;

	protected:

	private:

		void set_owned_source(sound_source* value);
		sound_source* get_owned_source() const;

		vector3f m_position;
		sound_source* m_owned_source;
		float m_pitch;
		float m_volume;
		float m_min_distance;
		float m_attenuation;
		bool m_relative_to_listener;

		
	};
}
#pragma once

#include "../algebra/vector3.h"

namespace age
{
	class sound_source;
	class sound_buffer;

	class sound_interface
	{
	public:
		friend class audio_device;
		friend class sound_source;

		sound_interface();
		sound_interface(const sound_interface& other);
		sound_interface(sound_interface&& other) noexcept;

		sound_interface& operator = (const sound_interface& other);
		sound_interface& operator = (sound_interface&& other) noexcept;

		virtual ~sound_interface() = default;

	public:
		virtual void play(bool looped = false) = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

		void update_source(sound_source& source, bool looped) const;

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

		bool get_looping() const;

	protected:
		void attach_source(sound_source* value) const;
		sound_source* get_attached_source() const;
		void detach_source() const;

	private:

		vector3f m_position;
		mutable sound_source* m_attached_source;

		float m_pitch;
		float m_volume;
		float m_min_distance;
		float m_attenuation;
		bool m_relative_to_listener;
	};
}
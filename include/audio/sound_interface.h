#pragma once

#include <glm/vec3.hpp>
#include "sound_properties.h"

namespace age
{
	class sound_source;
	class sound_buffer;

	class sound_interface
	{
	public:
		friend class audio_device;
		friend class sound_source;

		sound_interface() = default;
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

		//ToDo: Maybe add here set_position and update_position with a glm::vec3 too
		void set_position(const glm::vec3& value);
		void update_position(const glm::vec3& value);
		const glm::vec3& get_position() const;

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
		void attach_source(sound_source* value);
		sound_source* get_attached_source() const;
		void detach_source() const;

		const sound_properties& get_properties() const;

	private:
		sound_properties m_properties;

		mutable sound_source* m_attached_source{nullptr};
	};
}
#include "audio/listener.h"

#include "audio/audio_device.h"

namespace age
{
	namespace listener
	{
		void set_volume(float value)
		{
			audio_device::set_listener_volume(value);
		}

		float get_volume()
		{
			return audio_device::get_listener_volume();
		}

		void set_position(const glm::vec3& value)
		{
			audio_device::set_listener_position(value);
		}

		const glm::vec3& get_position()
		{
			return audio_device::get_listener_position();
		}

		void set_direction(const glm::vec3& value)
		{
			audio_device::set_listener_direction(value);
		}

		const glm::vec3& get_direction()
		{
			return audio_device::get_listener_direction();
		}

		void set_up_vector(const glm::vec3& value)
		{
			audio_device::set_listener_up_vector(value);
		}

		const glm::vec3& get_up_vector()
		{
			return audio_device::get_listener_up_vector();
		}
	}
}
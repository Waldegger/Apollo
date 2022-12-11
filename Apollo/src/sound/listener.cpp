#include "listener.h"

#include "audio_device.h"

namespace age
{
	void set_volume(float value)
	{
		audio_device::set_listener_volume(value);
	}

	float get_volume()
	{
		return audio_device::get_listener_volume();
	}

	void set_position(const vector3f& value)
	{
		audio_device::set_listener_position(value);
	}

	const vector3f& get_position()
	{
		return audio_device::get_listener_position();
	}

	void set_direction(const vector3f& value)
	{
		audio_device::set_listener_direction(value);
	}

	const vector3f& get_direction()
	{
		return audio_device::get_listener_direction();
	}

	void set_up_vector(const vector3f& value)
	{
		audio_device::set_listener_up_vector(value);
	}

	const vector3f& get_up_vector()
	{
		return audio_device::get_listener_up_vector();
	}
}
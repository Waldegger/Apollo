#pragma once

#include "../algebra/vector3.h"

namespace age
{
	namespace listener
	{
		void set_volume(float value);
		float get_volume();

		void set_position(const vector3f& value);
		const vector3f& get_position();

		void set_direction(const vector3f& value);
		const vector3f& get_direction();

		void set_up_vector(const vector3f& value);
		const vector3f& get_up_vector();
	}
}
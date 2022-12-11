#pragma once

#include "../algebra/vector2.h"
#include "color.h"

namespace age
{
	struct vertex_2d
	{
		vertex_2d()
			: position{}
			, color{ 255, 255, 255 }
			, tex_coords{}
		{}

		vertex_2d(vector2f p_position)
			: position{ p_position }
			, color{ 255, 255, 255 }
			, tex_coords{}
		{}

		vertex_2d(vector2f p_position, vector2f p_tex_coords)
			: position{ p_position }
			, color{ 255, 255, 255 }
			, tex_coords{ p_tex_coords }
		{}

		vertex_2d(vector2f p_position, color p_color, vector2f p_tex_coords)
			: position{ p_position }
			, color{ p_color }
			, tex_coords{ p_tex_coords }
		{}

		vector2f position;
		color color;
		vector2f tex_coords;
	};
}
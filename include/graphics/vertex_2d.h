#pragma once

#include <glm/vec2.hpp>
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

		vertex_2d(glm::vec2 p_position)
			: position{ p_position }
			, color{ 255, 255, 255 }
			, tex_coords{}
		{}

		vertex_2d(glm::vec2 p_position, glm::vec2 p_tex_coords)
			: position{ p_position }
			, color{ 255, 255, 255 }
			, tex_coords{ p_tex_coords }
		{}

		vertex_2d(glm::vec2 p_position, color p_color, glm::vec2 p_tex_coords)
			: position{ p_position }
			, color{ p_color }
			, tex_coords{ p_tex_coords }
		{}

		glm::vec2 position;
		color color;
		glm::vec2 tex_coords;
	};
}
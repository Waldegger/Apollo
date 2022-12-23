#pragma once

#include "shape_2d.h"

#include <vector>

#include "vertex_2d.h"

namespace age
{
	class circle_shape
		: public shape_2d
	{
	public:

	public:

	protected:

	private:
		std::vector<vertex_2d> m_vertices;
		std::vector<uint32_t> m_indices;
		
		std::vector<vertex_2d> m_outline_vertices;
		std::vector<uint32_t> m_outline_indices;
	};
}
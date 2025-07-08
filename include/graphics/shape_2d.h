#pragma once

#include "transformable_2d.h"
#include "drawable.h"

namespace age
{
	struct vertex_2d;

	class shape_2d
		: public transformable_2d
		, public drawable
	{
	public:
		struct vertex_data
		{
			vertex_data(const vertex_2d the_vertices[], size_t the_num_vertices, const uint32_t the_indices[], size_t the_num_indices)
				: vertices{ the_vertices }
				, num_vertices{ the_num_vertices }
				, indices{ the_indices }
				, num_indices{ the_num_indices }
			{}

			const vertex_2d* vertices{};
			size_t num_vertices{};
			const uint32_t* indices{};
			size_t num_indices{};
		};

	public:
		inline vertex_data get_vertex_data() const { return on_get_vertex_data(); }

	protected:

	private:
		virtual vertex_data on_get_vertex_data() const = 0;
	};
}
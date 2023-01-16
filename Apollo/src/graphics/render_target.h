#pragma once

#include "blend_mode.h"
#include "vertex_2d.h"
#include "view_2d.h"

namespace age
{
	class drawable;
	class render_states;
	class texture;

	enum class primitive_type : uint32_t
	{
		points = 0,
		lines = 1,
		line_strip = 2,
		triangles = 3,
		triangle_strip = 4,
		triangle_fan = 5
	};

	class render_target
	{
	public:
		render_target();
		virtual ~render_target() = default;

		render_target(const render_target& other) = default;
		render_target(render_target&& other) = default;
		
		render_target& operator = (const render_target& other) = default;
		render_target& operator = (render_target&& other) = default;

	public:
		int_rect get_viewport(const view_2d& view) const;

		vector2f map_pixel_to_coords(const vector2i& point) const;
		vector2i map_coords_to_pixel(const vector2f& point) const;
		
		virtual vector2u get_size() const = 0;

		void apply_view(const view_2d& view);
		const vector2f& get_view_size() const;

		void draw(const drawable& drawbable_object, const render_states& states);
		void draw(const vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const render_states& states);
		void draw(const vertex_2d vertices[], size_t num_vertices, primitive_type type, const render_states& states);

	protected:
		static constexpr uint32_t A_POSITION_INDEX = 0;
		static constexpr uint32_t A_COLOR_INDEX = 1;
		static constexpr uint32_t A_TEX_COORDS_INDEX = 2;

		void init();

	private:
		struct states_cache
		{
			blend_mode last_blend_mode = blend_mode::blend_none;
			const texture* last_texture = nullptr;
		};

		void prepare_draw(const vertex_2d vertices[], const render_states& states);
		void apply_blend_mode(const blend_mode& mode);

		const matrix4f& get_inverse_projection() const;

		vector2f m_view_size;
		int_rect m_viewport;
		matrix4f m_projection_matrix;
		mutable matrix4f m_projection_matrix_inverse;
		states_cache m_states_cache;

		mutable bool m_projection_needs_update;
	};
}
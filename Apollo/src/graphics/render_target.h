#pragma once

#include "blend_mode.h"
#include "vertex_2d.h"
#include "view_2d.h"

namespace agl
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

		int_rect get_viewport(const view_2d& view) const;

		virtual vector2u get_size() const = 0;

	public:
		void apply_view(const view_2d& view);

		void draw(const drawable& drawbable_object, const render_states& states);
		void draw(const agl::vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const render_states& states);
		void draw(const agl::vertex_2d vertices[], size_t num_vertices, primitive_type type, const render_states& states);

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

		void prepare_draw(const agl::vertex_2d vertices[], const render_states& states);
		void apply_blend_mode(const blend_mode& mode);

		matrix4f m_projection_matrix;
		states_cache m_states_cache;
	};
}
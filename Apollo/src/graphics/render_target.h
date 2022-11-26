#pragma once

#include "render_states.h"
#include "vertex_2d.h"

namespace agl
{
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
		void draw(const agl::vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const agl::render_states& states);

	protected:
		static constexpr uint32_t A_POSITION_INDEX = 0;
		static constexpr uint32_t A_COLOR_INDEX = 1;
		static constexpr uint32_t A_TEX_COORDS_INDEX = 2;

	private:
		struct states_cache
		{
			blend_mode last_blend_mode;
			const texture* last_texture = nullptr;
		};

		states_cache m_states_cache;
	};
}
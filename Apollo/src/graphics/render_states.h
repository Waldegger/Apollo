#pragma once

#include "matrix4.h"
#include "blend_mode.h"
#include "shader_program.h"

namespace agl
{
	class texture;
	class shader_program;

	struct render_states
	{
		render_states(const shader_program& the_program)
			: texture{ nullptr }
			, shader_program{ &the_program }
		{}

		render_states(const shader_program& the_program, const matrix4f& the_transform)
			: texture{ nullptr }
			, shader_program{ &the_program }
			, transform{ the_transform }
		{}

		render_states(const shader_program& the_program, const texture& the_texture, const matrix4f& the_transform)
			: texture{ &the_texture }
			, shader_program{ &the_program }
			, transform{ the_transform }
		{}

		render_states(const shader_program& the_program, const blend_mode& the_blend_mode, const matrix4f& the_transform)
			: texture{ nullptr }
			, shader_program{ &the_program }
			, blend_mode{ the_blend_mode }
			, transform{ the_transform }
		{}

		render_states(const shader_program& the_program, const texture& the_texture, const blend_mode& the_blend_mode, const matrix4f& the_transform)
			: texture{ &the_texture }
			, shader_program{ &the_program }
			, blend_mode{ the_blend_mode }
			, transform{ the_transform }
		{}
		
		const texture* texture = nullptr;
		const shader_program* shader_program = nullptr;

		blend_mode blend_mode = blend_mode::blend_alpha;
		matrix4f transform;
	};
}
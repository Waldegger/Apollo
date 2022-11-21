#pragma once

#include "matrix4.h"
#include "blend_mode.h"


namespace agl
{
	class texture_2d;
	class shader_program;

	struct render_states
	{

		matrix4f transform;
		blend_mode blend_mode;
		const texture_2d* texture = nullptr;
		const shader_program* shader_program = nullptr;

		static const render_states default_states;
	};
}
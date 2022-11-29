#include "render_target.h"

#include <GL/glew.h>

#include "render_states.h"
#include "texture.h"

namespace agl
{
	constexpr inline uint32_t factor_to_GL_constant(blend_mode::factor factor)
	{
		switch (factor)
		{
		case blend_mode::factor::zero:					return GL_ZERO;
		case blend_mode::factor::one:					return GL_ONE;
		case blend_mode::factor::src_color:				return GL_SRC_COLOR;
		case blend_mode::factor::one_minus_src_color:	return GL_ONE_MINUS_SRC_COLOR;
		case blend_mode::factor::dst_color:				return GL_DST_COLOR;
		case blend_mode::factor::one_minus_dst_color:	return GL_ONE_MINUS_DST_COLOR;
		case blend_mode::factor::src_alpha:				return GL_SRC_ALPHA;
		case blend_mode::factor::one_minus_src_alpha:	return GL_ONE_MINUS_SRC_ALPHA;
		case blend_mode::factor::dst_alpha:				return GL_DST_ALPHA;
		case blend_mode::factor::one_minus_dst_alpha:	return GL_ONE_MINUS_DST_ALPHA;
		}

		return GL_ZERO;
	}

	constexpr inline uint32_t equation_to_GL_constant(blend_mode::equation equation)
	{
		switch (equation)
		{
		case blend_mode::equation::add:					return GL_FUNC_ADD;
		case blend_mode::equation::subtract:			return GL_FUNC_SUBTRACT;
		case blend_mode::equation::reverse_subtract:	return GL_FUNC_REVERSE_SUBTRACT;
		/*
		* OpenGL ES 3
		case blend_mode::equation::min:					return GL_MIN;
		case blend_mode::equation::max:					return GL_MAX;
		*/
		}

		return GL_FUNC_ADD;
	}

	render_target::render_target()
	{
		
	}

	int_rect render_target::get_viewport(const view_2d& view) const
	{
		auto size = get_size();

		float            width = static_cast<float>(size.x);
		float            height = static_cast<float>(size.y);
		const float_rect& viewport = view.get_viewport();

		return int_rect({ static_cast<int>(0.5f + width * viewport.left), static_cast<int>(0.5f + height * viewport.top) },
			{ static_cast<int>(0.5f + width * viewport.width), static_cast<int>(0.5f + height * viewport.height) });
	}

	void render_target::apply_view(const view_2d& value)
	{
		// Set the viewport
		int_rect viewport = get_viewport(value);
		int     top = static_cast<int>(get_size().y) - (viewport.top + viewport.height);
		glViewport(viewport.left, top, viewport.width, viewport.height);

		// Set the projection matrix
		m_projection_matrix = value.get_transform();
	}

	void render_target::draw(const agl::vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const agl::render_states& states)
	{
		if (!vertices || !indices || !num_indices)
			return;

		auto& layout = states.get_program_layout();
		auto& program = layout.get_program();
	
		program.bind();

		if(layout.get_mvp_matrix_location() >= 0)
			program.set_uniform(layout.get_mvp_matrix_location(), m_projection_matrix * states.get_transform());

		if(layout.get_texture_location() >= 0)
			program.set_uniform(layout.get_texture_location(), 0);

		states.get_texture().bind();

		glVertexAttribPointer(A_POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), &vertices[0].position);
		glVertexAttribPointer(A_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(agl::vertex_2d), &vertices[0].color);
		glVertexAttribPointer(A_TEX_COORDS_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), &vertices[0].tex_coords);

		apply_blend_mode(states.get_blend_mode());
		
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_indices), GL_UNSIGNED_INT, indices);
	}

	void render_target::init()
	{
		glEnableVertexAttribArray(A_POSITION_INDEX);
		glEnableVertexAttribArray(A_COLOR_INDEX);
		glEnableVertexAttribArray(A_TEX_COORDS_INDEX);
		glEnable(GL_BLEND);

		apply_blend_mode(blend_mode::blend_alpha);
	}

	void render_target::apply_blend_mode(const blend_mode& mode)
	{
		if (mode != m_states_cache.last_blend_mode)
		{
			glBlendFuncSeparate(factor_to_GL_constant(mode.color_src_factor),
				factor_to_GL_constant(mode.color_dst_factor),
				factor_to_GL_constant(mode.alpha_src_factor),
				factor_to_GL_constant(mode.alpha_dst_factor));

			glBlendEquationSeparate(equation_to_GL_constant(mode.color_equation),
				equation_to_GL_constant(mode.alpha_equation));

			m_states_cache.last_blend_mode = mode;
		}
	}
}
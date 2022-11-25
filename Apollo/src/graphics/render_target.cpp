#include "render_target.h"

#include <GL/glew.h>

#include "render_states.h"

namespace agl
{
	render_target::render_target()
	{
		
	}

	void render_target::draw(const agl::vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const agl::render_states& states)
	{
		if (!vertices || !indices || !num_indices)
			return;

		states.shader_program.bind();
		states.shader_program.set_uniform("u_mvp_matrix", states.transform);
	
		glVertexAttribPointer(A_POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), &vertices[0].position);
		glVertexAttribPointer(A_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(agl::vertex_2d), &vertices[0].color);

		if (states.texture)
		{
			if (!m_states_cache.last_texture)
			{
				/*
				Maybe this is needed
				glActiveTexture(GL_TEXTURE0);
				*/
				glEnableVertexAttribArray(A_TEX_COORDS_INDEX);
			}

			//ToDo: Compare the handle of the texture, as the texture could be moved and the address is the same, but the handle might have been changed
			if (states.texture != m_states_cache.last_texture)
			{
				m_states_cache.last_texture = states.texture;
			}

			glVertexAttribPointer(A_TEX_COORDS_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), &vertices[0].tex_coords);
		}
		else
		{
			if(m_states_cache.last_texture)
				glDisableVertexAttribArray(A_TEX_COORDS_INDEX);
		}

		//Implement this
		if (states.blend_mode != m_states_cache.last_blend_mode)
		{

			m_states_cache.last_blend_mode = states.blend_mode;
		}
		
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_indices), GL_UNSIGNED_INT, indices);
	}
}
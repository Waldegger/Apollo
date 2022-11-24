#include "render_target.h"

#include <GL/glew.h>

#include "render_states.h"

namespace agl
{
	render_target::render_target()
	{
		
	}

	void render_target::draw(const agl::vertex_2d vertices[], const uint32_t indices[], size_t num_indices, const agl::render_states& states) const
	{
		if (!vertices || !indices || !num_indices)
			return;

		states.shader_program.bind();
		states.shader_program.set_uniform("u_mvp_matrix", states.transform);
	
		glVertexAttribPointer(A_POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), &vertices[0].position);
		glVertexAttribPointer(A_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(agl::vertex_2d), &vertices[0].color);

		//For later use
		//glVertexAttribPointer(A_TEX_COORDS_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(agl::vertex_2d), m_vertices.data() + sizeof(agl::vector2f) + sizeof(agl::color));

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(num_indices), GL_UNSIGNED_INT, indices);
	}
}
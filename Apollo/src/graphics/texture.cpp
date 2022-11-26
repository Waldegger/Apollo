#include "texture.h"

#include <GL/glew.h>

namespace agl
{
	texture::texture()
		: m_handle{ gen_handle() }
	{}

	void texture::bind()
	{
		auto handle = get_handle();

		if (handle != m_current_bound_texture)
		{
			glBindTexture(GL_TEXTURE_2D, get_handle());

			m_current_bound_texture = handle;
		}
	}

	uint32_t texture::gen_handle()
	{
		GLuint handle;
		glGenTextures(1, &handle);

		return handle;
	}

	void texture::delete_handle(uint32_t handle)
	{
		glDeleteTextures(1, &handle);
	}
}
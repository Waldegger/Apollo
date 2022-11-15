#pragma once

#include <GL/glew.h>

#include <vector>

#include "shader.h"
#include "../utility/utility.h"

namespace apollo
{
	class shader_program
	{
	public:
		shader_program();
		
	public:
		void attach_shader(const shader& shader);
		void detach_shader(const shader& shader);

		void link();

		void use();

	protected:

	private:

		GLuint get_handle() const { return m_handle; }

		static void delete_handle(GLuint handle);
		unique_handle <GLuint, delete_handle> m_handle;

		std::vector<GLuint> m_attached_shaders;
	};
}
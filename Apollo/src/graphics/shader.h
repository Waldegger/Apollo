#pragma once

#include <GL/glew.h>

#include <string_view>

#include "../utility/utility.h"

namespace apollo
{
	class shader
	{
	public:
		friend class shader_program;

		enum class type : GLuint
		{
			vertex = GL_VERTEX_SHADER,
			geometry = GL_GEOMETRY_SHADER,
			fragment = GL_FRAGMENT_SHADER
		};

		shader(type shader_type);
		
	public:
		type get_type() const { return m_type; }
		void compile(const std::string_view& shader_source);

	protected:

	private:

		static void delete_handle(GLuint handle);

		GLuint get_handle() const { return m_handle; }

		unique_handle<GLuint, delete_handle> m_handle;

		type m_type;
	};
}
#pragma once

#include <GL/glew.h>

#include <vector>
#include <string_view>

#include "shader.h"
#include "matrix4.h"
#include "../utility/utility.h"

namespace agl
{
	class shader_program
	{
	public:
		shader_program();
		
	public:
		void attach_shader(const shader& shader);
		void detach_shader(const shader& shader);

		void bind_attrib_location(uint32_t index, const std::string_view& name);

		void link();
		void bind();
		void release();

		int32_t get_uniform_location(const std::string_view& name);

		void set_uniform(int32_t location, float v0);
		void set_uniform(int32_t location, float v0, float v1);
		void set_uniform(int32_t location, float v0, float v1, float v2);
		void set_uniform(int32_t location, float v0, float v1, float v2, float v3);
		void set_uniform(int32_t location, int32_t v0);
		void set_uniform(int32_t location, int32_t v0, int32_t v1);
		void set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2);
		void set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2, int32_t v3);
		void set_uniform(int32_t location, uint32_t v0);
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1);
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v3);
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v3, uint32_t v4);
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 1>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 2>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 3>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 4>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 1>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 2>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 3>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 4>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 1>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 2>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 3>& v);
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 4>& v);
		void set_uniform(int32_t location, const matrix4f& v, bool transpose);
		void set_uniform(int32_t location, const matrix4f* v[], size_t size, bool transpose);
		//If necessary add more uniforms for matrices: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml

		void set_uniform(const std::string_view& name, float v0);
		void set_uniform(const std::string_view& name, float v0, float v1);
		void set_uniform(const std::string_view& name, float v0, float v1, float v2);
		void set_uniform(const std::string_view& name, float v0, float v1, float v2, float v3);
		void set_uniform(const std::string_view& name, int32_t v0);
		void set_uniform(const std::string_view& name, int32_t v0, int32_t v1);
		void set_uniform(const std::string_view& name, int32_t v0, int32_t v1, int32_t v2);
		void set_uniform(const std::string_view& name, int32_t v0, int32_t v1, int32_t v2, int32_t v3);
		void set_uniform(const std::string_view& name, uint32_t v0);
		void set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1);
		void set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1, uint32_t v3);
		void set_uniform(const std::string_view& name, uint32_t v0, uint32_t v1, uint32_t v3, uint32_t v4);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 1>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 2>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 3>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<float, 4>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 1>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 2>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 3>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<int32_t, 4>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 1>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 2>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 3>& v);
		void set_uniform(const std::string_view& name, uint32_t count, const std::array<uint32_t, 4>& v);
		void set_uniform(const std::string_view& name, const matrix4f& v, bool transpose);
		void set_uniform(const std::string_view& name, const matrix4f* v[], size_t size, bool transpose);

	protected:

	private:
		inline static GLuint m_current_bound_program;

		GLuint get_handle() const { return m_handle; }

		static void delete_handle(GLuint handle);
		unique_handle <GLuint, delete_handle> m_handle;

		std::vector<GLuint> m_attached_shaders;
	};
}
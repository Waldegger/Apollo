#pragma once

#include <vector>
#include <string_view>

#include "shader.h"
#include <glm/mat4x4.hpp>
#include "../utility/utility.h"

namespace age
{
	class shader_program
	{
	public:
		shader_program();

	public:
		void attach_shader(const shader& shader);
		void detach_shader(const shader& shader);

		void bind_attrib_location(uint32_t index, std::string_view name);

		void link();
		void bind() const;
		void release();

		int32_t get_uniform_location(std::string_view name) const;
		uint32_t get_uniform_block_index(std::string_view name) const;

		void set_uniform_block_binding(uint32_t index, uint32_t binding);
		void set_uniform_block_binding(std::string_view name, uint32_t binding);

		void set_uniform(int32_t location, float v0) const;
		void set_uniform(int32_t location, float v0, float v1) const;
		void set_uniform(int32_t location, float v0, float v1, float v2) const;
		void set_uniform(int32_t location, float v0, float v1, float v2, float v3) const;
		void set_uniform(int32_t location, int32_t v0) const;
		void set_uniform(int32_t location, int32_t v0, int32_t v1) const;
		void set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2) const;
		void set_uniform(int32_t location, int32_t v0, int32_t v1, int32_t v2, int32_t v3) const;
		void set_uniform(int32_t location, uint32_t v0) const;
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1) const;
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v3) const;
		void set_uniform(int32_t location, uint32_t v0, uint32_t v1, uint32_t v3, uint32_t v4) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 1>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 2>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 3>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<float, 4>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 1>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 2>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 3>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<int32_t, 4>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 1>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 2>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 3>& v) const;
		void set_uniform(int32_t location, uint32_t count, const std::array<uint32_t, 4>& v) const;
		void set_uniform(int32_t location, const glm::mat4& v, bool transpose = false) const;
		void set_uniform(int32_t location, const glm::mat4* v[], size_t size, bool transpose = false) const;
		//If necessary add more uniforms for matrices: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml

		void set_uniform(std::string_view name, float v0) const;
		void set_uniform(std::string_view name, float v0, float v1) const;
		void set_uniform(std::string_view name, float v0, float v1, float v2) const;
		void set_uniform(std::string_view name, float v0, float v1, float v2, float v3) const;
		void set_uniform(std::string_view name, int32_t v0) const;
		void set_uniform(std::string_view name, int32_t v0, int32_t v1) const;
		void set_uniform(std::string_view name, int32_t v0, int32_t v1, int32_t v2) const;
		void set_uniform(std::string_view name, int32_t v0, int32_t v1, int32_t v2, int32_t v3) const;
		void set_uniform(std::string_view name, uint32_t v0) const;
		void set_uniform(std::string_view name, uint32_t v0, uint32_t v1) const;
		void set_uniform(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v3) const;
		void set_uniform(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v3, uint32_t v4) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<float, 1>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<float, 2>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<float, 3>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<float, 4>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<int32_t, 1>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<int32_t, 2>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<int32_t, 3>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<int32_t, 4>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<uint32_t, 1>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<uint32_t, 2>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<uint32_t, 3>& v) const;
		void set_uniform(std::string_view name, uint32_t count, const std::array<uint32_t, 4>& v) const;
		void set_uniform(std::string_view name, const glm::mat4x4& v, bool transpose = false) const;
		void set_uniform(std::string_view name, const glm::mat4x4* v[], size_t size, bool transpose = false) const;

		uint32_t get_handle() const { return m_handle; }

	protected:

	private:
		inline static thread_local uint32_t m_current_bound_program{};
		static void delete_handle(uint32_t handle);

		std::vector<uint32_t> m_attached_shaders;
		unique_handle <uint32_t, delete_handle> m_handle;
	};
}
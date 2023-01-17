#pragma once

#include <functional>
#include <string_view>

#include "shader_program.h"

namespace age
{
	class program_layout
	{
	public:
		program_layout(const shader_program& program)
			: m_program{ program }
		{}

		program_layout(const shader_program& program, int32_t mvp_matrix_location, int32_t texture_location)
			: m_program { program }
			, m_mvp_matrix_location{ mvp_matrix_location }
			, m_texture_location{ texture_location }
		{}

		program_layout(const shader_program& program, std::string_view mvp_matrix_name, std::string_view texture_name)
			: m_program{ program }
			, m_mvp_matrix_location{ m_program.get().get_uniform_location(mvp_matrix_name) }
			, m_texture_location{ m_program.get().get_uniform_location(texture_name) }
		{}
		
	public:
		inline const shader_program& get_program() const { return m_program; }

		inline void set_mvp_matrix_location(int32_t value) { m_mvp_matrix_location = value; }
		inline int32_t get_mvp_matrix_location() const { return m_mvp_matrix_location; }

		inline void set_texture_location(int32_t value) { m_texture_location = value; }
		inline int32_t get_texture_location() const { return m_texture_location; }

		inline void mvp_matrix_name(const std::string_view name) { m_mvp_matrix_location = m_program.get().get_uniform_location(name); }
		inline void texture_name(const std::string_view name) { m_texture_location = m_program.get().get_uniform_location(name); }
	protected:

	private:
		std::reference_wrapper<const shader_program> m_program;

		int32_t m_mvp_matrix_location = -1;
		int32_t m_texture_location = -1;
	};
}
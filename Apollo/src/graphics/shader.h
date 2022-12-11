#pragma once

#include <string_view>

#include "../utility/utility.h"

namespace age
{
	class shader
	{
	public:
		friend class shader_program;

		enum class shader_type
		{
			vertex,
			geometry,
			fragment
		};

		shader(shader_type shader_type);
		
	public:
		shader_type get_type() const { return m_type; }
		void compile(const std::string_view& shader_source);

	protected:

	private:

		static uint32_t convert_type(shader_type type_to_convert);
		static void delete_handle(uint32_t handle);

		uint32_t get_handle() const { return m_handle; }

		unique_handle<uint32_t, delete_handle> m_handle;

		shader_type m_type;
	};
}
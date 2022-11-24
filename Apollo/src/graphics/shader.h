#pragma once

#include <string_view>

#include "../utility/utility.h"

namespace agl
{
	class shader
	{
	public:
		friend class shader_program;

		enum class type
		{
			vertex,
			geometry,
			fragment
		};

		shader(type shader_type);
		
	public:
		type get_type() const { return m_type; }
		void compile(const std::string_view& shader_source);

	protected:

	private:

		static uint32_t convert_type(type type_to_convert);
		static void delete_handle(uint32_t handle);

		uint32_t get_handle() const { return m_handle; }

		unique_handle<uint32_t, delete_handle> m_handle;

		type m_type;
	};
}
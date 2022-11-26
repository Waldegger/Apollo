#pragma once

#include <string_view>

#include "vector2.h"
#include "../utility/utility.h"

namespace agl
{
	class texture
	{
	public:
		texture();
	public:
		void bind();

		void create(const vector2u& size);
		void load_from_file(const std::string_view& v);

	protected:

	private:
		inline static uint32_t m_current_bound_texture;

		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);

		uint32_t get_handle() { return m_handle; }

		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
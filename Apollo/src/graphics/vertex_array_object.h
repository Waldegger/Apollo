#pragma once

#include "../utility/utility.h"

namespace age
{
	class vertex_array_object
	{
	public:
		vertex_array_object();
	public:
		void bind();
		void release();

	protected:

	private:
		inline static uint32_t m_current_bound_array;

		uint32_t get_handle() const { return m_handle; }

		static uint32_t create_handle();
		static void delete_handle(uint32_t handle);
		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
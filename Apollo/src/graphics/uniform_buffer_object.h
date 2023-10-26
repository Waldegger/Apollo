#pragma once

#include "../utility/utility.h"

namespace age
{
	class uniform_buffer_object
	{
	public:
		uniform_buffer_object();
	public:
		void bind() const;

		void buffer_data(size_t size, const void* data);
		void buffer_sub_data(size_t offset, size_t size, const void* data);

		void bind_buffer_base(uint32_t index);
		void bind_buffer_range(uint32_t index, size_t offset, size_t size);

		static void bind(const uniform_buffer_object* ubo);
	protected:

	private:
		inline static uint32_t m_current_bound_unfiform_buffer_object;

		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);

		uint32_t get_handle() const { return m_handle; };

		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
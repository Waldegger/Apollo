#pragma once

#include "../utility/utility.h"

namespace age
{
	class sound_buffer
	{
	public:
		sound_buffer();
	public:

	protected:

	private:
		friend class sound_source;

		inline uint32_t get_handle() const { return m_handle; }

		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);
		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
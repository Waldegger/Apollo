#pragma once

#include <string_view>
#include <istream>

#include "../utility/utility.h"

namespace age
{
	class sound_queue_buffer;

	class sound_buffer
	{
	public:
		friend class sound_queue_buffer;

		enum class format : uint32_t
		{
			mono_8,
			mono_16,
			stereo_8,
			stereo_16
		};

		sound_buffer();
	public:
		void load(std::string_view fn);
		void load(std::istream& is);
		void load(std::byte data[], size_t size_in_bytes);

		void buffer_data(format the_format, const std::byte data[], size_t size_in_bytes, uint32_t frequency);

	protected:

	private:
		friend class sound_source;

		inline uint32_t get_handle() const { return m_handle; }

		static int32_t format_to_AL_enum(format the_format);

		static uint32_t gen_handle();
		static void delete_handle(uint32_t handle);
		unique_handle<uint32_t, delete_handle> m_handle;
	};
}
#pragma once

#include <stdint.h>
#include <cstring>

namespace age
{
	namespace endian
	{
		constexpr inline bool is_big_endian()
		{
			constexpr int32_t a = 1;
			return !((int8_t*)&a)[0];
		}

		inline int32_t convert_to_int(const std::byte buffer[], std::size_t size)
		{
			int32_t a = 0;
			if (!is_big_endian())
				std::memcpy(&a, buffer, size);
			else
				for (std::size_t i = 0; i < size; ++i)
					reinterpret_cast<int8_t*>(&a)[3 - i] = std::to_integer<int8_t>(buffer[i]);

			return a;
		}
	}
}
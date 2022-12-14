#pragma once

#include <stdint.h>
#include <cstring>

namespace age
{
	constexpr inline bool is_big_endian()
	{
		constexpr int32_t a = 1;
		return !((int8_t*)&a)[0];
	}

	int32_t convert_to_int(const int8_t* buffer, std::size_t len)
	{
		int32_t a = 0;
		if (!is_big_endian())
			std::memcpy(&a, buffer, len);
		else
			for (std::size_t i = 0; i < len; ++i)
				reinterpret_cast<int8_t*>(&a)[3 - i] = buffer[i];

		return a;
	}
}
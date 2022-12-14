#pragma once

#include <istream>

namespace age
{
	namespace audio_format
	{
		enum class format
		{
			unknown,
			wave,
			ogg,
			mp3
		};

		format get_format(std::istream& is);
		format get_format(const std::byte data[], size_t size_in_bytes);
	}
}
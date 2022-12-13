#include "audio_format.h"

namespace age
{
	namespace audio_format
	{
		struct wave_header
		{
			uint8_t RIFF[4] = {0, 0, 0, 0};
			uint32_t chunk_size{};
			uint8_t WAVE[4] = {0, 0, 0, 0};
		};

		format get_format(std::istream& is)
		{
			auto position = is.tellg();
			wave_header wave_h;

			//peek a bit into the stream to determin with what filetype we are dealing
			is.read(reinterpret_cast<char*>(&wave_h), sizeof(decltype(wave_h)));
			is.seekg(position);

			if (wave_h.RIFF[0] == 'R' && wave_h.RIFF[1] == 'I' && wave_h.RIFF[2] == 'F' && wave_h.RIFF[3] == 'F'
				&& wave_h.WAVE[0] == 'W' && wave_h.WAVE[1] == 'A' && wave_h.WAVE[2] == 'V' && wave_h.WAVE[3] == 'E')
			{
				return format::wave;
			}

			return format::unknown;
		}

		format get_format(const std::uint8_t data[], size_t size_in_bytes)
		{
			if (!data || !size_in_bytes)
				return format::unknown;

			if (size_in_bytes >= sizeof(wave_header))
			{
				wave_header header{*reinterpret_cast<const wave_header*>(data)};

				if (header.RIFF[0] == 'R' && header.RIFF[1] == 'I' && header.RIFF[2] == 'F' && header.RIFF[3] == 'F'
					&& header.WAVE[0] == 'W' && header.WAVE[1] == 'A' && header.WAVE[2] == 'V' && header.WAVE[3] == 'E')
				{
					return format::wave;
				}
			}


			return format::unknown;
		}
	}
}
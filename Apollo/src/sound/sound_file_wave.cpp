#include "sound_file_wave.h"

#include "../system/assetstream.h"

namespace age
{
	void sound_file_wave::load(std::string_view fn)
	{
		assetistream is{ fn.data() };
		load(is);
	}

	void sound_file_wave::load(std::istream& is)
	{

	}

	void sound_file_wave::load(const std::byte data[], size_t size_in_bytes)
	{

	}

	const sound_file_wave::header& sound_file_wave::get_header() const
	{
		return m_header;
	}

	const std::vector<uint8_t>& sound_file_wave::get_data() const
	{
		return m_data;
	}
}
#pragma once

#include <memory>
#include <istream>

#include "sound_stream.h"

namespace age
{
	namespace sound_stream_factory
	{
		std::unique_ptr<sound_stream> create_from_stream(std::istream& is);
	}
}
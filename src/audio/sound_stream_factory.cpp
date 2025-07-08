#include "audio/sound_stream_factory.h"

#include "audio/audio_format.h"
#include "audio/priv/ogg_stream.h"

namespace age::sound_stream_factory
{
	std::unique_ptr<sound_stream> create_from_stream(std::istream& is)
	{
		std::unique_ptr<sound_stream> result;

		if (audio_format::get_format(is) == audio_format::format::ogg)
		{
			//ToDo add propert Test if stream is for an ogg file
		}

		result = std::make_unique<ogg_stream>();
		return result;
	}
}
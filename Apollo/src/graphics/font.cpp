#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

//Freetype callbacks
unsigned long read(FT_Stream rec, unsigned long offset, unsigned char* buffer, unsigned long count)
{
	auto converted_offset = static_cast<int64_t>(offset);
	auto stream = static_cast<std::istream*>(rec->descriptor.pointer);

	if (stream->seekg(converted_offset).tellg() == converted_offset)
	{
		if (count > 0)
		{
			return static_cast<unsigned long>(stream->read(reinterpret_cast<char*>(buffer), count).tellg());
		}

		return 0;
	}

	return count > 0 ? 0 : 1;
}

void close(FT_Stream)
{

}

namespace agl
{
	
}
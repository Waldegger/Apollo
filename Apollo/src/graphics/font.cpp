#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

#include <exception>
#include <sstream>
#include <limits>

#include "../system/assetstream.h"

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
	class font::font_handles
	{
	private:
		struct deleter
		{
			void operator()(FT_Library theLibrary)
			{
				FT_Done_FreeType(theLibrary);
			}
			void operator()(FT_Face theFace)
			{
				FT_Done_Face(theFace);
			}
			void operator()(FT_Stroker theStroker)
			{
				FT_Stroker_Done(theStroker);
			}
		};

	public:
		std::unique_ptr<std::remove_pointer_t<FT_Library>, deleter> library;	//< Pointer to the internal library interface
		std::unique_ptr<FT_StreamRec>                               stream_rec;	//< Pointer to the stream rec instance
		std::unique_ptr<std::remove_pointer_t<FT_Face>, deleter>    face;		//< Pointer to the internal font face
		std::unique_ptr<std::remove_pointer_t<FT_Stroker>, deleter> stroker;	//< Pointer to the stroker
	};

	font::font()
		: m_font_handles{}
	{}

	font::font(font&& other) noexcept = default;
	font::~font() = default;

	font& font::operator = (font&& other) noexcept = default;

	void font::load(const std::string_view& fn)
	{
		load(std::make_unique<assetistream>(fn));
	}

	void font::load(const void* data, size_t size_in_bytes)
	{
		m_stream.reset();
		cleanup();

		auto handles = std::make_unique<font_handles>();

		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font from memory(failed to initialize FreeType)";

			throw std::runtime_error{ ss.str() };
		}
		handles->library.reset(library);

		FT_Face face;
		if (FT_New_Memory_Face(library, reinterpret_cast<const FT_Byte*>(data), static_cast<FT_Long>(size_in_bytes), 0, &face) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font from memory (failed to create the font face)";

			throw std::runtime_error{ ss.str() };
		}
		handles->face.reset(face);

		FT_Stroker stroker;
		if (FT_Stroker_New(library, &stroker) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font from memory (failed to create the stroker)";

			throw std::runtime_error{ ss.str() };
		}
		handles->stroker.reset(stroker);

		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font from memory (failed to set the Unicode character set)";

			throw std::runtime_error{ ss.str() };
		}

		m_font_handles = std::move(handles);
		m_info.family = face->family_name ? face->family_name : std::string{};
	}

	void font::load(std::unique_ptr<std::istream> in_stream)
	{
		m_stream = std::move(in_stream);
		load_from_stream(*m_stream);
	}

	void font::load(std::istream& in_stream)
	{
		m_stream.reset();
		load_from_stream(in_stream);
	}

	void font::load_from_stream(std::istream& in_stream)
	{
		cleanup();

		auto handles = std::make_unique<font_handles>();

		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font (failed to initialize FreeType)";

			throw std::runtime_error{ ss.str() };
		}
		handles->library.reset(library);

		auto cur_stream_pos = in_stream.tellg();

		//Lets try if an undefined 0x7FFFFFFF streamsize will do. If not give it the real size
		/*
		in_stream.ignore(std::numeric_limits<std::streamsize>::max());
		auto size = in_stream.gcount() - cur_stream_pos;
		in_stream.clear();
		in_stream.seekg(cur_stream_pos);
		*/

		handles->stream_rec = std::make_unique<FT_StreamRec>();
		handles->stream_rec->base = nullptr;
		//handles->stream_rec->size = size;
		handles->stream_rec->size = 0x7FFFFFFF;
		handles->stream_rec->pos = static_cast<decltype(handles->stream_rec->pos)>(cur_stream_pos);
		handles->stream_rec->descriptor.pointer = &in_stream;
		handles->stream_rec->read = &read;
		handles->stream_rec->close = &close;

		FT_Open_Args args;
		args.flags = FT_OPEN_STREAM;
		args.stream = handles->stream_rec.get();
		args.driver = nullptr;

		FT_Face face;
		if (FT_Open_Face(library, &args, 0, &face) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font (failed to create the font face)";

			throw std::runtime_error{ ss.str() };
		}
		handles->face.reset(face);

		FT_Stroker stroker;
		if (FT_Stroker_New(library, &stroker) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font (failed to create the stroker)";

			throw std::runtime_error{ ss.str() };
		}
		handles->stroker.reset(stroker);

		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			std::stringstream ss;
			ss << "Failed to load font (failed to set the Unicode character set)";

			throw std::runtime_error{ ss.str() };
		}

		m_font_handles = std::move(handles);

		m_info.family = face->family_name ? face->family_name : std::string();
	}

	const font::info& font::get_info() const
	{
		return m_info;
	}

	const font::glyph& font::get_glyph(uint32_t code_point, uint32_t character_size, bool bold, float outline_thickness) const
	{
		
	}

	void font::cleanup()
	{
		m_font_handles.reset();
		m_pages.clear();
		m_pixel_buffer.clear();
		m_pixel_buffer.shrink_to_fit();
	}
}
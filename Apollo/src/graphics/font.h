#pragma once

#include <string>
#include <string_view>
#include <istream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "rect.h"
#include "texture.h"

namespace agl
{
	class font
	{
	public:
		font();
		font(const font& other);
		font(const font&& other) noexcept;

		font& operator = (const font& other);
		font& operator = (font&& other) noexcept;

		~font();

	public:
		struct info
		{
			std::string family;
		};

		struct glyph
		{
			float advance = 0.0f;
			uint32_t ls_delta = 0;
			uint32_t rs_delta = 0;
			float_rect bounds;
			float_rect texture_rect;
		};

		void load(const std::string_view& fn);
		void load(const void* data, size_t size_in_bytes);
		void load(std::unique_ptr<std::istream> in_stream);

		const info& get_info() const;
		const glyph& get_glyph(uint32_t code_point, uint32_t character_size, bool bold, float outline_thickness = 0.0f) const;
		bool has_glyph(uint32_t code_point) const;

		float get_kerning(uint32_t first, uint32_t second, uint32_t character_size, bool bold = false) const;
		float get_line_spacing(uint32_t character_size) const;

		float get_underline_position(uint32_t character_size) const;
		float get_underline_thickness(uint32_t character_size) const;

		const texture& get_texture() const;

		void set_smooth(bool value);
		bool get_smooth() const;

		void pre_buffer(const std::string_view& letters, uint32_t character_size);

	protected:

	private:

		struct row
		{
			float width = 0.0f;
			float height = 0.0f;
			float top = 0.0f;
		};

		struct page
		{
			explicit page(bool smooth);

			std::unordered_map<uint32_t, glyph> glyphs;
			texture m_texture;
			float next_row;
			std::vector<row> rows;
		};

		class font_handles;

		info m_info;

		mutable std::unordered_map<uint32_t, page> m_pages;
		mutable std::vector<uint8_t> m_pixel_buffer;

		std::unique_ptr<font_handles> m_font_handles;
		std::unique_ptr<std::istream> m_stream;

		bool m_smooth = false;
	};
}
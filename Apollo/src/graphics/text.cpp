#include "text.h"

#include "render_states.h"

void add_line(std::vector<age::vertex_2d>& vertices,
	float line_length,
	float line_top,
	const age::color& color,
	float offset,
	float thickness,
	float pixel_size,
	float outline_thickness = 0.0f)
{
	float top = std::floor(line_top + offset - (thickness / 2) + 0.5f);
	float bottom = top + std::floor(thickness + 0.5f);

	auto tex_coord = age::vector2f{ pixel_size, pixel_size };

	vertices.emplace_back(age::vertex_2d{ age::vector2f{-outline_thickness, top - outline_thickness}, color, tex_coord });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{line_length + outline_thickness, top - outline_thickness}, color, tex_coord });
	vertices.emplace_back(age::vertex_2d{ age::vector2f{-outline_thickness, bottom + outline_thickness}, color, tex_coord });
	vertices.emplace_back(age::vertex_2d{ age::vector2f{-outline_thickness, bottom + outline_thickness}, color, tex_coord });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{line_length + outline_thickness, top - outline_thickness}, color, tex_coord });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{line_length + outline_thickness, bottom + outline_thickness}, color, tex_coord });
}

void add_glyph_quad(std::vector<age::vertex_2d>& vertices,
	age::vector2f position,
	const age::color& color,
	const age::font::glyph& glyph,
	float italic_shear,
	float pixel_size)
{
	float padding = 1.0;

	float left = glyph.bounds.left - padding;
	float top = glyph.bounds.top - padding;
	float right = glyph.bounds.left + glyph.bounds.width + padding;
	float bottom = glyph.bounds.top + glyph.bounds.height + padding;

	float u1 = glyph.render_texture_rect.left - pixel_size;
	float v1 = glyph.render_texture_rect.top - pixel_size;
	float u2 = glyph.render_texture_rect.left + glyph.render_texture_rect.width + pixel_size;
	float v2 = glyph.render_texture_rect.top + glyph.render_texture_rect.height + pixel_size;

	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + left - italic_shear * top, position.y + top}, color, age::vector2f{u1, v1} });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + right - italic_shear * top, position.y + top}, color, age::vector2f{u2, v1} });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + left - italic_shear * bottom, position.y + bottom}, color, age::vector2f{u1, v2} });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + left - italic_shear * bottom, position.y + bottom}, color, age::vector2f{u1, v2} });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + right - italic_shear * top, position.y + top}, color, age::vector2f{u2, v1} });
	vertices.emplace_back(
		age::vertex_2d{ age::vector2f{position.x + right - italic_shear * bottom, position.y + bottom}, color, age::vector2f{u2, v2} });
}

namespace age
{
	text::text()
		: m_string{}
		, m_font{ nullptr }
		, m_character_size{ 30 }
		, m_letter_spacing_factor{ 1.0f }
		, m_line_spacing_factor{ 1.0f }
		, m_style{ text_styles::regular }
		, m_fill_color{ 255, 255, 255, 255 }
		, m_outline_color{ 0, 0, 0, 0 }
		, m_outline_thickness{ 0.0f }
		, m_last_texture_size{}
		, m_last_texture_id{ 0 }
		, m_vertices{}
		, m_outline_vertices{}
		, m_geometry_needs_update{ false }
	{}

	text::text(std::string_view the_text, const font& the_font, uint32_t the_character_size)
		: m_string{}
		, m_font{ &the_font }
		, m_character_size{ the_character_size }
		, m_letter_spacing_factor{ 1.0f }
		, m_line_spacing_factor{ 1.0f }
		, m_style{ text_styles::regular }
		, m_fill_color{ 255, 255, 255, 255 }
		, m_outline_color{ 0, 0, 0, 0 }
		, m_outline_thickness{ 0.0f }
		, m_last_texture_size{}
		, m_last_texture_id{ 0 }
		, m_vertices{}
		, m_outline_vertices{}
		, m_geometry_needs_update{ true }
	{
		set_string(the_text);
	}

	text::text(std::u32string_view the_text, const font& the_font, uint32_t the_character_size)
		: m_string{ the_text }
		, m_font{ &the_font }
		, m_character_size{ the_character_size }
		, m_letter_spacing_factor{ 1.0f }
		, m_line_spacing_factor{ 1.0f }
		, m_style{ text_styles::regular }
		, m_fill_color{ 255, 255, 255, 255 }
		, m_outline_color{ 0, 0, 0, 0 }
		, m_outline_thickness{ 0.0f }
		, m_last_texture_size{}
		, m_last_texture_id{ 0 }
		, m_vertices{}
		, m_outline_vertices{}
		, m_geometry_needs_update{ true }
	{}

	void text::set_string(std::string_view value)
	{
		/*
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		m_tmp_convert_string = converter.from_bytes(value.data());
		
		set_string(converter.from_bytes(value.data()));
		*/
	}

	void text::set_string(std::u32string_view value)
	{
		if (m_string != value)
		{
			m_string = value;
			m_geometry_needs_update = true;
		}
	}

	const std::u32string& text::get_string() const
	{
		return m_string;
	}

	void text::set_font(const font& value)
	{
		if (m_font != &value)
		{
			m_font = &value;
			m_geometry_needs_update = true;
		}
	}

	const font* text::get_font() const
	{
		return m_font;
	}

	void text::set_character_size(uint32_t value)
	{
		if (m_character_size != value)
		{
			m_character_size = value;
			m_geometry_needs_update = true;
		}
	}

	uint32_t text::get_character_size() const
	{
		return m_character_size;
	}

	void text::set_line_spacing(float factor)
	{
		if (m_line_spacing_factor != factor)
		{
			m_line_spacing_factor = factor;
			m_geometry_needs_update = true;
		}
	}

	float text::get_line_spacing() const
	{
		return m_line_spacing_factor;
	}

	void text::set_letter_spacing(float factor)
	{
		if (m_letter_spacing_factor != factor)
		{
			m_letter_spacing_factor = factor;
			m_geometry_needs_update = true;
		}
	}

	float text::get_letter_spacing() const
	{
		return m_letter_spacing_factor;
	}

	void text::set_style(uint32_t flags)
	{
		if (m_style != flags)
		{
			m_style = flags;
			m_geometry_needs_update = true;
		}
	}

	uint32_t text::get_style() const
	{
		return m_style;
	}

	void text::set_fill_color(const color& value)
	{
		if (m_fill_color != value)
		{
			m_fill_color = value;

			if (!m_geometry_needs_update)
			{
				for (auto& v : m_vertices)
					v.color = m_fill_color;
			}
		}
	}

	const color& text::get_fill_color() const
	{
		return m_fill_color;
	}

	void text::set_outline_color(const color& value)
	{
		if (m_outline_color != value)
		{
			m_outline_color = value;

			if (!m_geometry_needs_update)
			{
				for (auto& v : m_outline_vertices)
					v.color = m_outline_color;
			}
		}
	}

	void text::set_outline_thickness(float value)
	{
		if (m_outline_thickness != value)
		{
			m_outline_thickness = value;
			m_geometry_needs_update = true;
		}
	}

	float text::get_outline_thickness() const
	{
		return m_outline_thickness;
	}

	vector2f text::find_character_pos(size_t index) const
	{
		vector2f result;

		if (!m_font) return result;

		if (index > m_string.size()) index = m_string.size();

		bool is_bold = m_style & text_styles::bold;
		float whitespace_width = m_font->get_glyph(U' ', m_character_size, is_bold).advance;
		float letter_spacing = (whitespace_width / 3.0f) * (m_letter_spacing_factor - 1.0f);
		whitespace_width += letter_spacing;
		float line_spacing = m_font->get_line_spacing(m_character_size) * m_line_spacing_factor;

		uint32_t prev_char = 0;
		for (size_t i = 0; i < index; ++i)
		{
			uint32_t cur_char = m_string[i];

			result.x += m_font->get_kerning(prev_char, cur_char, m_character_size, is_bold);
			prev_char = cur_char;

			switch (cur_char)
			{
			case U' ':
				result.x += whitespace_width;
				continue;

			case U'\t':
				result.x += whitespace_width * 4.0f;
				continue;

			case U'\n':
				result.x = 0.0f;
				result.y += line_spacing;
				continue;
			}

			result.x += m_font->get_glyph(cur_char, m_character_size, is_bold, m_outline_thickness).advance + letter_spacing;
		}

		result = get_transform().transform_point(result);

		return result;
	}

	float_rect text::get_local_bounds() const
	{
		ensure_geometry_is_updated();

		return m_bounds;
	}

	float_rect text::get_global_bounds() const
	{
		return get_transform().transform_rect(get_local_bounds());
	}

	void text::draw(render_target& target, const render_states& states) const
	{
		if (m_font)
		{
			ensure_geometry_is_updated();

			render_states states_copy = states;
			states_copy.set_transform(states_copy.get_transform() * get_transform());
			states_copy.set_texture(m_font->get_texture(m_character_size));

			if (m_outline_thickness != 0.0f)
				target.draw(m_outline_vertices.data(), m_outline_vertices.size(), primitive_type::triangles, states_copy);

			target.draw(m_vertices.data(), m_vertices.size(), primitive_type::triangles, states_copy);
		}
	}

	void text::ensure_geometry_is_updated() const
	{
		if (!m_font)
			return;

		const auto& font_texture = m_font->get_texture(m_character_size);
		const auto& font_texture_size = font_texture.get_size();

		if (!m_geometry_needs_update && font_texture.get_id() == m_last_texture_id && font_texture_size.is_equal_to(m_last_texture_size))
			return;

		m_last_texture_id = font_texture.get_id();
		m_last_texture_size = font_texture_size;

		m_vertices.clear();
		m_outline_vertices.clear();
		m_bounds = float_rect{};

		if (m_string.empty())
			return;

		float pixel_size = 1.0f / static_cast<float>(font_texture_size.x);

		bool is_bold = m_style & text_styles::bold;
		bool is_underlined = m_style & text_styles::underlined;
		bool is_strike_through = m_style & text_styles::strike_through;
		float italic_shear = (m_style & text_styles::italic) ? degrees(12).as_radians() : 0.0f;
		float underline_offset = m_font->get_underline_position(m_character_size);
		float underline_thickness = m_font->get_underline_thickness(m_character_size);

		float_rect x_bounds = m_font->get_glyph(U'x', m_character_size, is_bold).bounds;
		float strike_through_offset = x_bounds.top + x_bounds.height * 0.5f;

		float whitespace_width = m_font->get_glyph(U' ', m_character_size, is_bold).advance;
		float letter_spacing = (whitespace_width / 3.0f) * (m_letter_spacing_factor - 1.0f);
		whitespace_width += letter_spacing;
		float line_spacing = m_font->get_line_spacing(m_character_size) * m_line_spacing_factor;

		float x = 0.0f;
		float y = static_cast<float>(m_character_size);

		float min_x = static_cast<float>(m_character_size);
		float min_y = static_cast<float>(m_character_size);
		float max_x = 0.f;
		float max_y = 0.f;

		uint32_t prev_char = 0;

		for (auto cur_char : m_string)
		{
			if (cur_char == U'\r') continue;

			x += m_font->get_kerning(prev_char, cur_char, m_character_size, is_bold);

			if (is_underlined && (cur_char == U'\n' && prev_char != U'\n'))
			{
				add_line(m_vertices, x, y, m_fill_color, underline_offset, underline_thickness, pixel_size);

				if (m_outline_thickness != 0)
					add_line(m_outline_vertices, x, y, m_outline_color, underline_offset, underline_thickness, pixel_size, m_outline_thickness);
			}

			if (is_strike_through && (cur_char == U'\n' && prev_char != U'\n'))
			{
				add_line(m_vertices, x, y, m_fill_color, strike_through_offset, underline_thickness, pixel_size);

				if (m_outline_thickness != 0)
					add_line(m_outline_vertices, x, y, m_outline_color, strike_through_offset, underline_thickness, pixel_size, m_outline_thickness);
			}

			prev_char = cur_char;

			// Handle special characters
			if ((cur_char == U' ') || (cur_char == U'\n') || (cur_char == U'\t'))
			{
				// Update the current bounds (min coordinates)
				min_x = std::min(min_x, x);
				min_y = std::min(min_y, y);

				switch (cur_char)
				{
				case U' ':
					x += whitespace_width;
					break;
				case U'\t':
					x += whitespace_width * 4.0f;
					break;
				case U'\n':
					y += line_spacing;
					x = 0.0f;
					break;
				}

				// Update the current bounds (max coordinates)
				max_x = std::max(max_x, x);
				max_y = std::max(max_y, y);

				// Next glyph, no need to create a quad for whitespace
				continue;
			}

			// Apply the outline
			if (m_outline_thickness != 0)
			{
				const font::glyph& glyph = m_font->get_glyph(cur_char, m_character_size, is_bold, m_outline_thickness);

				// Add the outline glyph to the vertices
				add_glyph_quad(m_outline_vertices, vector2f(x, y), m_outline_color, glyph, italic_shear, pixel_size);
			}

			// Extract the current glyph's description
			const font::glyph& glyph = m_font->get_glyph(cur_char, m_character_size, is_bold);

			// Add the glyph to the vertices
			add_glyph_quad(m_vertices, vector2f(x, y), m_fill_color, glyph, italic_shear, pixel_size);

			// Update the current bounds
			float left = glyph.bounds.left;
			float top = glyph.bounds.top;
			float right = glyph.bounds.left + glyph.bounds.width;
			float bottom = glyph.bounds.top + glyph.bounds.height;

			min_x = std::min(min_x, x + left - italic_shear * bottom);
			max_x = std::max(max_x, x + right - italic_shear * top);
			min_y = std::min(min_y, y + top);
			max_y = std::max(max_y, y + bottom);

			// Advance to the next character
			x += glyph.advance + letter_spacing;
		}

		// If we're using outline, update the current bounds
		if (m_outline_thickness != 0.0f)
		{
			float outline = std::abs(std::ceil(m_outline_thickness));
			min_x -= outline;
			max_x += outline;
			min_y -= outline;
			max_y += outline;
		}

		// If we're using the underlined style, add the last line
		if (is_underlined && (x > 0))
		{
			add_line(m_vertices, x, y, m_fill_color, underline_offset, underline_thickness, pixel_size);

			if (m_outline_thickness != 0)
				add_line(m_outline_vertices, x, y, m_outline_color, underline_offset, underline_thickness, m_outline_thickness);
		}

		// If we're using the strike through style, add the last line across all characters
		if (is_strike_through && (x > 0))
		{
			add_line(m_vertices, x, y, m_fill_color, strike_through_offset, underline_thickness, pixel_size);

			if (m_outline_thickness != 0)
				add_line(m_outline_vertices, x, y, m_outline_color, strike_through_offset, underline_thickness, m_outline_thickness);
		}

		// Update the bounding rectangle
		m_bounds.left = min_x;
		m_bounds.top = min_y;
		m_bounds.width = max_x - min_x;
		m_bounds.height = max_y - min_y;

		m_geometry_needs_update = false;
	}
}
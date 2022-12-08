#include "text.h"

namespace agl
{
	text::text()
		: m_string{}
		, m_font{ nullptr }
		, m_character_size{ 0 }
		, m_letter_spacing_factor{ 1.0f }
		, m_line_spacing_factor{ 1.0f }
		, m_style{ static_cast<std::underlying_type<text::style>::type>(text::style::regular) }
		, m_fill_color{ 255, 255, 255, 255 }
		, m_outline_color{ 0, 0, 0, 0 }
		, m_outline_thickness{ 0.0f }
		, m_vertices{}
		, m_outline_vertices{}
		, m_geometry_needs_update{ false }
	{}

	std::underlying_type<text::style>::type operator | (const text::style& lhs, const text::style& rhs)
	{
		return static_cast<std::underlying_type<text::style>::type>(lhs) | static_cast<std::underlying_type<text::style>::type>(rhs);
	}

	bool operator & (const std::underlying_type<text::style>::type& lhs, const text::style& rhs)
	{
		return lhs & static_cast<std::underlying_type<text::style>::type>(rhs);
	}
}
#pragma once

#include <vector>
#include <string_view>
#include <istream>

#include "../algebra/vector2.h"
#include "color.h"

namespace age
{
	class image
	{
	public:

	public:
		void create(const vector2u& size, const color& the_color = color::black);
		void create(const vector2u& size, const uint8_t* pixels);

		void load(std::string_view fn);
		void load(const void* data, size_t size);
		void load(std::istream& is);

		void save(const std::string_view& fn);
		void save(std::vector<uint8_t>& data, const std::string_view& format);

		void create_mask_from_color(const color& color_key, uint8_t alpha);

		void set_pixel(const vector2u& coords, const color& pixel_color);
		color get_pixel(const vector2u& coords) const;

		const vector2u& get_size() const;
		const uint8_t* get_pixel_ptr() const;

		void flip_horizontal();
		void flip_vertical();
	protected:

	private:
		vector2u m_size;
		std::vector<uint8_t> m_pixels;
	};

	std::istream& operator >> (std::istream& in, image& im);
}
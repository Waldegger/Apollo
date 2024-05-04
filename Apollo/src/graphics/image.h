#pragma once

#include <vector>
#include <string_view>
#include <istream>

#include <glm/vec2.hpp>
#include "color.h"

namespace age
{
	class image
	{
	public:

	public:
		void create(const glm::u32vec2& size, const color& the_color = color::black);
		void create(const glm::u32vec2& size, const uint8_t* pixels);

		void load(std::string_view fn);
		void load(const std::byte data[], size_t size);
		void load(std::istream& is);

		void save(const std::string_view& fn);
		void save(std::vector<uint8_t>& data, const std::string_view& format);

		void create_mask_from_color(const color& color_key, uint8_t alpha);

		void set_pixel(const glm::u32vec2& coords, const color& pixel_color);
		color get_pixel(const glm::u32vec2& coords) const;

		const glm::u32vec2& get_size() const;
		const uint8_t* get_pixel_ptr() const;

		void flip_horizontal();
		void flip_vertical();
	protected:

	private:
		glm::u32vec2 m_size{ 0.0f, 0.0f };
		std::vector<uint8_t> m_pixels;
	};

	std::istream& operator >> (std::istream& in, image& im);
}
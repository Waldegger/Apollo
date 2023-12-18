#include "texture.h"

#include <GL/glew.h>

#include <exception>
#include <sstream>
#include <cassert>

#include "../engine.h"

namespace age
{
	texture::texture()
		: m_handle{ gen_handle() }
	{}

	texture::texture(const texture& other)
		: m_handle{ gen_handle() }
	{

	}

	texture& texture::operator = (const texture& other)
	{
		return *this;
	}

	void texture::bind() const
	{
		auto handle = get_handle();

		if (handle != m_current_bound_texture)
		{
			glBindTexture(GL_TEXTURE_2D, handle);

			const age::vector2u& size = get_size();
			matrix4f tex_matrix = matrix4f::get_identity();
			tex_matrix.scale({ 1.0f / static_cast<float>(size.x), 1.0f / static_cast<float>(size.y) });
			age::engine::get_instance()->get_texture_matrix_ubo().buffer_data(sizeof(age::matrix4f), &tex_matrix.get_data());

			m_current_bound_texture = handle;
		}
	}

	void texture::create(const vector2u& size)
	{
		if (size.x == 0 || size.y == 0)
		{
			std::stringstream message;
			message << "Invalid texture size (" << size.x << ", " << size.y << ")";

			throw std::runtime_error{ message.str() };
		}

		uint32_t max_size = get_maximum_size();

		if (size.x > max_size || size.y > max_size)
		{
			std::stringstream message;
			message << "Internal size of texture is too high (" << size.x << ", " << size.y << ") maximum is: " << max_size;

			throw std::runtime_error{ message.str() };
		}

		m_size = size;
	
		bind();

		glTexImage2D(
			GL_TEXTURE_2D, 
			0,
			GL_RGBA,
			static_cast<GLsizei>(m_size.x),
			static_cast<GLsizei>(m_size.y),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr);

		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);

		m_has_mipmap = false;
	}

	void texture::load(std::string_view filename, const int_rect& area)
	{
		image img;
		img.load(filename);
		load(img, area);
	}

	void texture::load(const std::byte data[], std::size_t size, const int_rect& area)
	{
		image img;
		img.load(data, size);
		load(img, area);
	}

	void texture::load(std::istream& is, const int_rect& area)
	{
		image img;
		img.load(is);
		load(img, area);
	}

	void texture::load(const image& img, const int_rect& area)
	{
		int32_t width = static_cast<int32_t>(img.get_size().x);
		int32_t height = static_cast<int32_t>(img.get_size().y);

		if (area.width == 0 || area.height == 0 ||
			(area.left <= 0 && area.top <= 0 && area.width >= width && area.height >= height))
		{
			create(img.get_size());
			update(img);

			return;
		}

		int_rect rectangle = area;
		if(rectangle.left < 0)
			rectangle.left = 0;
		if (rectangle.top < 0)
			rectangle.top = 0;
		if (rectangle.left + rectangle.width > width)
			rectangle.width = width - rectangle.left;
		if (rectangle.top + rectangle.height > height)
			rectangle.height = height - rectangle.top;

		create(vector2u{ static_cast<uint32_t>(rectangle.width), static_cast<uint32_t>(rectangle.height) });

		auto pixels = img.get_pixel_ptr() + 4 * (rectangle.left + (width * rectangle.top));
		bind();

		for (int32_t i = 0; i < rectangle.height; ++i)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			pixels += 4 * width;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
			m_has_mipmap = false;

			//If there should be multithreading one day a glFlush will be necessary
		}
	}

	void texture::update(const uint8_t* pixels)
	{
		update(pixels, uint_rect{ vector2u{}, get_size() });
	}

	void texture::update(const uint8_t* pixels, const uint_rect& area)
	{
		assert(area.left + area.width <= m_size.x);
		assert(area.top + area.height <= m_size.y);

		if (pixels)
		{
			bind();

			glTexSubImage2D(GL_TEXTURE_2D,
				0,
				static_cast<GLint>(area.left),
				static_cast<GLint>(area.top),
				static_cast<GLsizei>(area.width),
				static_cast<GLsizei>(area.height),
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
			m_has_mipmap = false;

			//If there should be multithreading one day a glFlush will be necessary
		}
	}

	void texture::update(const texture& other_texture)
	{
		update(other_texture, vector2u{});
	}

	void texture::update(const texture& other_texture, const vector2u& dest)
	{
		assert(dest.x + other_texture.m_size.x <= m_size.x);
		assert(dest.y + other_texture.m_size.y <= m_size.y);

		update(other_texture.copy_to_image(), dest);
	}

	void texture::update(const image& img)
	{
		// Update the whole texture
		update(img.get_pixel_ptr(), uint_rect{ vector2u{}, img.get_size() });
	}

	void texture::update(const image& img, const vector2u& dest)
	{
		update(img.get_pixel_ptr(), uint_rect{ dest, img.get_size() });
	}

	void texture::update(const render_window& window)
	{
		update(window, { 0, 0 });
	}

	void texture::update(const render_window& window, const vector2u& dest)
	{
		bind();
		//ToDo: Finish me
	}

	const vector2u& texture::get_size() const
	{
		return m_size;
	}

	image texture::copy_to_image() const
	{
		image result{};

		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);

		if (framebuffer)
		{
			std::vector<std::uint8_t> pixels(static_cast<std::size_t>(m_size.x) * static_cast<std::size_t>(m_size.y) * 4);

			GLint previousFrameBuffer;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, get_handle(), 0);
			glReadPixels(0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

			glDeleteFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER_BINDING, previousFrameBuffer);

			result.create(m_size, pixels.data());
		}

		return result;
	}

	void texture::set_smooth(bool value)
	{
		if (value != m_smooth)
		{
			m_smooth = value;

			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
			if (m_has_mipmap)
			{
				glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER,
					m_smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
			}
		}
	}

	bool texture::get_smooth() const
	{
		return m_smooth;
	}

	void texture::set_srgb(bool value)
	{
		m_srgb = value;
	}

	bool texture::get_srgb() const
	{
		return m_srgb;
	}

	void texture::set_repeat(bool value)
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	}

	bool texture::get_repeat() const
	{
		return m_repeat;
	}

	void texture::generate_mipmap()
	{
		bind();

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			m_smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);

		m_has_mipmap = true;
	}

	void texture::invalidate_mipmap()
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_smooth ? GL_LINEAR : GL_NEAREST);
		m_has_mipmap = false;
	}

	uint32_t texture::get_id() const
	{
		return get_handle();
	}

	void texture::bind(const texture* tex)
	{
		if (tex)
		{
			tex->bind();
			return;
		}

		if (m_current_bound_texture != 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			m_current_bound_texture = 0;
		}
	}

	uint32_t texture::get_maximum_size()
	{
		static bool checked = false;
		static uint32_t result = 0;

		if (!checked)
		{
			checked = true;

			GLint size;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

			result = size;
		}

		return result;
	}

	uint32_t texture::gen_handle()
	{
		GLuint handle;
		glGenTextures(1, &handle);

		return handle;
	}

	void texture::delete_handle(uint32_t handle)
	{
		glDeleteTextures(1, &handle);
	}
}
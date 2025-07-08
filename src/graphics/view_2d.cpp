#include "graphics/view_2d.h"

#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

namespace age
{
	view_2d::view_2d()
	{
		reset(float_rect({ 0.0f, 0.0f }, { 1.0f, 1.0f }));
	}

	view_2d::view_2d(const float_rect& rect)
	{
		reset(rect);
	}

	view_2d::view_2d(const glm::vec2& center, const glm::vec2& size)
		: m_center{ center }
		, m_size{ size }
	{}

	void view_2d::set_center(const glm::vec2& value)
	{
		m_center = value;
		m_transform_updated = false;
		m_inv_transform_updated = false;
	}

	const glm::vec2& view_2d::get_center() const
	{
		return m_center;
	}

	void view_2d::set_size(const glm::vec2& value)
	{
		m_size = value;
		m_transform_updated = false;
		m_inv_transform_updated = false;
	}

	const glm::vec2& view_2d::get_size() const
	{
		return m_size;
	}

	void view_2d::set_rotation(float value)
	{
		m_rotation = value;
		m_rotation = fmod(m_rotation, 360.0f);
		if (m_rotation < 0.0f) m_rotation += 360.0f;

		m_transform_updated = false;
		m_inv_transform_updated = false;
	}

	float view_2d::get_rotation() const
	{
		return m_rotation;
	}

	void view_2d::set_viewport(const float_rect& value)
	{
		m_viewport = value;
	}

	const float_rect& view_2d::get_viewport() const
	{
		return m_viewport;
	}

	void view_2d::reset(const float_rect& rectangle)
	{
		m_center.x = rectangle.left + rectangle.width * 0.5f;
		m_center.y = rectangle.top + rectangle.height * 0.5f;
		m_size.x = rectangle.width;
		m_size.y = rectangle.height;
		m_rotation = 0.0f;

		m_transform_updated = false;
		m_inv_transform_updated = false;
	}

	void view_2d::move(const glm::vec2& offset)
	{
		set_center(m_center + offset);
	}

	void view_2d::rotate(float value)
	{
		set_rotation(m_rotation + value);
	}

	void view_2d::zoom(float factor)
	{
		set_size(m_size * factor);
	}

	const glm::mat4& view_2d::get_transform() const
	{
		// Recompute the matrix if needed
		if (!m_transform_updated)
		{
			//Remove following comment when everything works
			/*
			// Rotation components
			float angle = m_rotation * (3.14159265358979323846f / 180.0f);
			float cosine = std::cos(angle);
			float sine = std::sin(angle);
			float tx = -m_center.x * cosine - m_center.y * sine + m_center.x;
			float ty = m_center.x * sine - m_center.y * cosine + m_center.y;

			// Projection components
			float a = 2.0f / m_size.x;
			float b = -2.0f / m_size.y;
			float c = -a * m_center.x;
			float d = -b * m_center.y;
		
			// Rebuild the projection matrix
			m_transform = glm::mat3(a * cosine, a * sine, a * tx + c,
				-b * sine, b * cosine, b * ty + d,
				0.0f, 0.0f, 1.0f);
			*/
		
			m_transform = glm::ortho(0.0f, m_size.x, m_size.y, 0.0f);
			m_transform = glm::translate(m_transform, glm::vec3{ m_center.x, m_center.y, 0.0f });
			m_transform = glm::rotate(m_transform, glm::radians(m_rotation), glm::vec3{ 0.0f,0.0f,1.0f });
			m_transform = glm::translate(m_transform, glm::vec3{ -m_center.x, -m_center.y, 0.0f });

			m_transform_updated = true;
		}

		return m_transform;
	}

	const glm::mat4& view_2d::get_inverse_transform() const
	{
		// Recompute the matrix if needed
		if (!m_inv_transform_updated)
		{
			m_inverse_transform = glm::inverse(get_transform());
			m_inv_transform_updated = true;
		}

		return m_inverse_transform;
	}
}
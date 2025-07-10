#include "graphics/transformable_2d.h"

#include <glm/trigonometric.hpp>

namespace age
{
	transformable_2d::transformable_2d()
		: m_origin{ 0.0f, 0.0f }
		, m_position{ 0.0f, 0.0f }
		, m_rotation{ 0.0f }
		, m_scale{ 1.0f, 1.0f }
		, m_transform{ 1.0f }
		, m_inverse_transform{ 1.0f }
		, m_transform_needs_update{ false }
		, m_inverse_transform_needs_update{ false }
	{}

	transformable_2d::transformable_2d(const transformable_2d& other) = default;
	transformable_2d::transformable_2d(transformable_2d&& other) noexcept = default;

	transformable_2d& transformable_2d::operator = (const transformable_2d& other) = default;
	transformable_2d& transformable_2d::operator = (transformable_2d&& other) noexcept = default;

	transformable_2d::~transformable_2d() = default;

	void transformable_2d::set_position(const glm::vec2& value)
	{
		m_position = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::move(const glm::vec2& offset)
	{
		set_position(m_position + offset);
	}

	const glm::vec2& transformable_2d::get_position() const
	{
		return m_position;
	}

	void transformable_2d::set_rotation(float value)
	{
		m_rotation = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::rotate(float value)
	{
		set_rotation(m_rotation + value);
	}

	float transformable_2d::get_rotation() const
	{
		return m_rotation;
	}

	void transformable_2d::set_scale(const glm::vec2& value)
	{
		m_scale = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::scale(const glm::vec2& factor)
	{
		set_scale(glm::vec2{ m_scale.x * factor.x, m_scale.y * factor.y });
	}

	const glm::vec2& transformable_2d::get_scale() const
	{
		return m_scale;
	}

	void transformable_2d::set_origin(const glm::vec2& value)
	{
		m_origin = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	const glm::vec2& transformable_2d::get_origin() const
	{
		return m_origin;
	}

	const glm::mat4& transformable_2d::get_transform() const
	{
		if (m_transform_needs_update)
		{
			float angle = glm::radians(m_rotation); // No minus; OpenGL rotates counterclockwise by default
			float cosA = std::cos(angle);
			float sinA = std::sin(angle);

			// Rotation and scaling
			float m00 =  cosA * m_scale.x;
			float m01 =  -sinA * m_scale.y;
			float m10 =  sinA * m_scale.x;
			float m11 =  cosA * m_scale.y;

			// Origin compensation (pivot)
			float tx = -m_origin.x * m00 - m_origin.y * m01 + m_position.x;
			float ty = -m_origin.x * m10 - m_origin.y * m11 + m_position.y;

			// Column-major matrix
			m_transform = glm::mat4{
				m00, m10, 0.0f, 0.0f,	// column 0
				m01, m11, 0.0f, 0.0f,	// column 1
				0.0f,0.0f,1.0f, 0.0f,	// column 2 (Z)
				tx,  ty,  0.0f, 1.0f	// column 3 (translation)
			};
			
			m_transform_needs_update = false;
		}

		return m_transform;
	}

	const glm::mat4& transformable_2d::get_inverse_transform() const
	{
		if (m_inverse_transform_needs_update)
		{
			m_inverse_transform = glm::inverse(get_transform());
			m_inverse_transform_needs_update = false;
		}

		return m_inverse_transform;
	}
}
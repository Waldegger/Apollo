#include "transformable_2d.h"

namespace age
{
	transformable_2d::transformable_2d()
		: m_origin{ 0.0f, 0.0f }
		, m_position{ 0.0f, 0.0f }
		, m_rotation{}
		, m_scale{ 1.0f, 1.0f }
		, m_transform{}
		, m_inverse_transform{}
		, m_transform_needs_update{ false }
		, m_inverse_transform_needs_update{ false }
	{}

	transformable_2d::transformable_2d(const transformable_2d& other) = default;
	transformable_2d::transformable_2d(transformable_2d&& other) noexcept = default;

	transformable_2d& transformable_2d::operator = (const transformable_2d& other) = default;
	transformable_2d& transformable_2d::operator = (transformable_2d&& other) noexcept = default;

	transformable_2d::~transformable_2d() = default;

	void transformable_2d::set_position(const vector2f& value)
	{
		m_position = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::move(const vector2f& offset)
	{
		set_position(m_position + offset);
	}

	const vector2f& transformable_2d::get_position() const
	{
		return m_position;
	}

	void transformable_2d::set_rotation(angle value)
	{
		m_rotation = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::rotate(angle value)
	{
		set_rotation(m_rotation + value);
	}

	angle transformable_2d::get_rotation() const
	{
		return m_rotation;
	}

	void transformable_2d::set_scale(const vector2f& value)
	{
		m_scale = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	void transformable_2d::scale(const vector2f& factor)
	{
		set_scale(vector2f{ m_scale.x * factor.x, m_scale.y * factor.y });
	}

	const vector2f& transformable_2d::get_scale() const
	{
		return m_scale;
	}

	void transformable_2d::set_origin(const vector2f& value)
	{
		m_origin = value;
		m_transform_needs_update = true;
		m_inverse_transform_needs_update = true;
	}

	const vector2f& transformable_2d::get_origin() const
	{
		return m_origin;
	}

	const matrix4f& transformable_2d::get_transform() const
	{
		if (m_transform_needs_update)
		{
			float angle = -m_rotation.as_radians();
			float cosine = std::cos(angle);
			float sine = std::sin(angle);
			float sxc = m_scale.x * cosine;
			float syc = m_scale.y * cosine;
			float sxs = m_scale.x * sine;
			float sys = m_scale.y * sine;
			float tx = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
			float ty = m_origin.x * sxs - m_origin.y * syc + m_position.y;

			m_transform = matrix4f(sxc, sys, tx,
				-sxs, syc, ty,
				0.f, 0.f, 1.f);
			
			m_transform_needs_update = false;
		}

		return m_transform;
	}

	const matrix4f& transformable_2d::get_inverse_transform() const
	{
		if (m_inverse_transform_needs_update)
		{
			m_inverse_transform = get_transform().get_inverse();
			m_inverse_transform_needs_update = false;
		}

		return m_inverse_transform;
	}
}
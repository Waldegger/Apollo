#pragma once

#include "vector2.h"
#include "matrix4.h"
#include "angle.h"

namespace agl
{
	class transformable_2d
	{
	public:
		transformable_2d();
		transformable_2d(const transformable_2d& other);
		transformable_2d(transformable_2d&& other) noexcept;

		transformable_2d& operator = (const transformable_2d& other);
		transformable_2d& operator = (transformable_2d&& other) noexcept;

		virtual ~transformable_2d();

	public:
		void set_position(const vector2f& value);
		void move(const vector2f& offset);
		const vector2f& get_position() const;

		void set_rotation(angle value);
		void rotate(angle value);
		angle get_rotation() const;

		void set_scale(const vector2f& value);
		void scale(const vector2f& factor);
		const vector2f& get_scale() const;

		void set_origin(const vector2f& value);
		const vector2f& get_origin() const;

		const matrix4f& get_transform() const;
		const matrix4f& get_inverse_transform() const;

	protected:

	private:

		vector2f m_origin;
		vector2f m_position;
		angle m_rotation;
		vector2f m_scale;

		mutable matrix4f m_transform;
		mutable matrix4f m_inverse_transform;
		mutable bool m_transform_needs_update;
		mutable bool m_inverse_transform_needs_update;
	};
}
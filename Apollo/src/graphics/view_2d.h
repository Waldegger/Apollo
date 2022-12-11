#pragma once

#include "../algebra/matrix4.h"
#include "../algebra/vector2.h"
#include "rect.h"

namespace age
{
	class view_2d
	{
	public:
		view_2d();
		explicit view_2d(const float_rect& rect);
		view_2d(const vector2f& center, const vector2f& size);

	public:
		void set_center(const vector2f& value);
		const vector2f& get_center() const;

		void set_size(const vector2f& value);
		const vector2f& get_size() const;

		void set_rotation(float value);
		float get_rotation() const;

		void set_viewport(const float_rect& value);
		const float_rect& get_viewport() const;

		void reset(const float_rect& rectangle);

		void move(const vector2f& offset);
		void rotate(float value);
		void zoom(float factor);

		const matrix4f& get_transform() const;
		const matrix4f& get_inverse_transform() const;

	protected:

	private:
		vector2f m_center;
		vector2f m_size;
		float m_rotation = 0.0f;
		float_rect m_viewport{{0.0f, 0.0f}, {1.0f, 1.0f}};
		mutable matrix4f m_transform;
		mutable matrix4f m_inverse_transform;
		mutable bool m_transform_updated = false;
		mutable bool m_inv_transform_updated = false;
	};
}
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "rect.h"

namespace age
{
	class view_2d
	{
	public:
		view_2d();
		explicit view_2d(const float_rect& rect);
		view_2d(const glm::vec2& center, const glm::vec2& size);

	public:
		void set_center(const glm::vec2& value);
		const glm::vec2& get_center() const;

		void set_size(const glm::vec2& value);
		const glm::vec2& get_size() const;

		void set_rotation(float value);
		float get_rotation() const;

		void set_viewport(const float_rect& value);
		const float_rect& get_viewport() const;

		void reset(const float_rect& rectangle);

		void move(const glm::vec2& offset);
		void rotate(float value);
		void zoom(float factor);

		const glm::mat4x4& get_transform() const;
		const glm::mat4x4& get_inverse_transform() const;

	protected:

	private:
		glm::vec2 m_center;
		glm::vec2 m_size;
		float m_rotation = 0.0f;
		float_rect m_viewport{{0.0f, 0.0f}, {1.0f, 1.0f}};
		mutable glm::mat4x4 m_transform{ 1.0f };
		mutable glm::mat4x4 m_inverse_transform{ 1.0f };
		mutable bool m_transform_updated = false;
		mutable bool m_inv_transform_updated = false;
	};
}
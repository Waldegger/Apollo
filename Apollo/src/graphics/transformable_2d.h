#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace age
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
		void set_position(const glm::vec2& value);
		void move(const glm::vec2& offset);
		const glm::vec2& get_position() const;

		void set_rotation(float value);
		void rotate(float value);
		float get_rotation() const;

		void set_scale(const glm::vec2& value);
		void scale(const glm::vec2& factor);
		const glm::vec2& get_scale() const;

		void set_origin(const glm::vec2& value);
		const glm::vec2& get_origin() const;

		const glm::mat4x4& get_transform() const;
		const glm::mat4x4& get_inverse_transform() const;

	protected:

	private:

		glm::vec2 m_origin;
		glm::vec2 m_position;
		float m_rotation;
		glm::vec2 m_scale;

		mutable glm::mat4x4 m_transform;
		mutable glm::mat4x4 m_inverse_transform;
		mutable bool m_transform_needs_update;
		mutable bool m_inverse_transform_needs_update;
	};
}
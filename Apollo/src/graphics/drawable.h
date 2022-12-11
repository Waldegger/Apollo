#pragma once

namespace age
{
	class render_target;
	class render_states;

	class drawable
	{
	public:
		drawable() = default;
		drawable(const drawable& other) = default;
		drawable(drawable&& other) = default;

		drawable& operator = (const drawable& other) = default;
		drawable& operator = (drawable&& other) = default;

		virtual ~drawable() = default;

	public:

	protected:

	private:
		friend class render_target;

		virtual void draw(render_target& target, const render_states& states) const = 0;
	};
}
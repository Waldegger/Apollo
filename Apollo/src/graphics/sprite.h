#pragma once

#include "transformable_2d.h"
#include "drawable.h"

#include <array>

#include "texture.h"
#include "color.h"
#include "vertex_2d.h"

namespace age
{
	class sprite
		: public transformable_2d
		, public drawable
	{
	public:
		sprite();
		sprite(const texture& texture);

	public:
		void set_color(const color& value);
		const color& get_color() const;

		void set_texture(const texture& value);
		const texture& get_texture() const;

	protected:

	private:
		virtual void draw(render_target& target, const render_states& states) const override;
		
		void update_vertices();
		
		const texture* m_texture;
		std::array<vertex_2d, 4> m_vertices;
	};
}
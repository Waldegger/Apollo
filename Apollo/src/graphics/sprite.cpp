#include "sprite.h"

#include "render_states.h"
#include "../engine.h"

namespace age
{
	sprite::sprite()
		: m_texture{ engine::get_default_texture() }
	{
		update_vertices();
	}

	sprite::sprite(const texture& texture)
		: m_texture{ &texture }
	{
		update_vertices();
	}

	void sprite::set_color(const color& value)
	{
		if (value != m_vertices[0].color)
		{
			for (auto& v : m_vertices) v.color = value;
		}
	}

	const color& sprite::get_color() const
	{
		return m_vertices[0].color;
	}

	void sprite::set_texture(const texture& value)
	{
		m_texture = &value;

		update_vertices();
	}

	const texture& sprite::get_texture() const
	{
		return *m_texture;
	}

	void sprite::draw(render_target& target, const render_states& states) const
	{
		render_states states_copy = states;

		states_copy.set_texture(*m_texture);
		states_copy.get_transform() *= get_transform();

		target.draw(m_vertices.data(), m_vertices.size(), age::primitive_type::triangle_fan, states_copy);
	}

	void sprite::update_vertices()
	{
		auto& tex_size = m_texture->get_size();
		vector2f size{ static_cast<float>(tex_size.x), static_cast<float>(tex_size.y) };

		m_vertices[1].position.x = size.x;
		m_vertices[2].position = size;
		m_vertices[3].position.y = size.y;
	}
}
#pragma once

#include "shape_2d.h"

#include <vector>

#include "vertex_2d.h"

namespace age
{
	class texture;

	class circle_shape
		: public shape_2d
	{
	public:
		circle_shape(float radius, uint32_t resolution = 30);

	public:
		void set_resolution(uint32_t value);
		uint32_t get_resolution() const;

		void set_radius(float value);
		float get_radius() const;

		void set_outline_thickness(float value);
		float get_outline_thickness() const;

		void set_fill_color(const color& value);
		const color& get_fill_color() const;

		void set_outline_color(const color& value);
		const color& get_outline_color() const;

	protected:

	private:
		virtual void draw(render_target& target, const render_states& states) const override;
		virtual vertex_data on_get_vertex_data() const override;

		void gen_vertices();
		void gen_outline_vertices();
		void update_fill_color();
		void update_outline_color();

		std::vector<vertex_2d> m_vertices;
		std::vector<uint32_t> m_indices;
		
		std::vector<vertex_2d> m_outline_vertices;
		std::vector<uint32_t> m_outline_indices;

		const texture* m_texture;

		uint32_t m_resolution;
		float m_radius;
		float m_outline_thickness;

		color m_fill_color;
		color m_outline_color;
	};
}
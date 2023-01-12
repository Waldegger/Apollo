#pragma once

#include "../algebra/matrix4.h"
#include "blend_mode.h"
#include "program_layout.h"
#include "../engine.h"

namespace age
{
	class texture;
	class shader_program;

	class render_states
	{
	public:
		render_states() = default;

		render_states(const program_layout& program_layout)
			: m_program_layout{ &program_layout }
		{}

		render_states(const matrix4f& the_transform)
			: m_transform{ the_transform }
		{}

		render_states(const program_layout& program_layout, const matrix4f& the_transform)
			: m_program_layout{ &program_layout }
			, m_transform{ the_transform }
		{}

		render_states(const program_layout& program_layout, const texture& the_texture, const matrix4f& the_transform)
			: m_texture{ &the_texture }
			, m_program_layout{ &program_layout }
			, m_transform{ the_transform }
		{}

		render_states(const program_layout& program_layout, const blend_mode& the_blend_mode, const matrix4f& the_transform)
			: m_program_layout{ &program_layout }
			, m_blend_mode{ the_blend_mode }
			, m_transform{ the_transform }
		{}

		render_states(const program_layout& program_layout, const texture& the_texture, const blend_mode& the_blend_mode, const matrix4f& the_transform)
			: m_texture{ &the_texture }
			, m_program_layout{ &program_layout }
			, m_blend_mode{ the_blend_mode }
			, m_transform{ the_transform }
		{}

	public:
		inline void set_texture(const texture& value) { m_texture = &value; }
		inline const texture& get_texture() const { return *m_texture; }

		inline void set_program_layout(const program_layout& value) { m_program_layout = &value; }
		inline const program_layout& get_program_layout() const{ return *m_program_layout; }

		inline void set_blend_mode(const blend_mode& value) { m_blend_mode = value; }
		inline const blend_mode& get_blend_mode() const { return m_blend_mode; }
		inline blend_mode& get_blend_mode() { return m_blend_mode; }

		inline void set_transform(const matrix4f& value) { m_transform = value; }
		inline const matrix4f& get_transform() const { return m_transform; }
		inline matrix4f& get_transform() { return m_transform; }

		inline static const render_states& get_default();

	protected:

	private:
		const texture* m_texture = engine::get_default_texture();
		const program_layout* m_program_layout = engine::get_default_program_layout();

		blend_mode m_blend_mode = blend_mode::blend_alpha;
		matrix4f m_transform;
	};

	const render_states& render_states::get_default()
	{
		render_states result;
		return result;
	}
}
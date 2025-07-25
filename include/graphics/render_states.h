#pragma once

#include <glm/mat4x4.hpp>
#include "blend_mode.h"
#include "../engine.h"

namespace age
{
	class texture;
	class shader_program;

	class render_states
	{
	public:
		render_states() = default;

		render_states(const shader_program& shader_program)
			: m_shader_program{ &shader_program }
		{}

		render_states(const glm::mat4x4& the_transform)
			: m_transform{ the_transform }
		{}

		render_states(const shader_program& shader_program, const glm::mat4x4& the_transform)
			: m_shader_program{ &shader_program }
			, m_transform{ the_transform }
		{}

		render_states(const shader_program& shader_program, const texture& the_texture, const glm::mat4x4& the_transform)
			: m_texture{ &the_texture }
			, m_shader_program{ &shader_program }
			, m_transform{ the_transform }
		{}

		render_states(const shader_program& shader_program, const blend_mode& the_blend_mode, const glm::mat4x4& the_transform)
			: m_shader_program{ &shader_program }
			, m_blend_mode{ the_blend_mode }
			, m_transform{ the_transform }
		{}

		render_states(const shader_program& shader_program, const texture& the_texture, const blend_mode& the_blend_mode, const glm::mat4x4& the_transform)
			: m_texture{ &the_texture }
			, m_shader_program{ &shader_program }
			, m_blend_mode{ the_blend_mode }
			, m_transform{ the_transform }
		{}

	public:
		inline void set_texture(const texture& value) { m_texture = &value; }
		inline const texture& get_texture() const { return *m_texture; }

		inline void set_shader_program(const shader_program& value) { m_shader_program = &value; }
		inline const shader_program& get_shader_program() const{ return *m_shader_program; }

		inline void set_blend_mode(const blend_mode& value) { m_blend_mode = value; }
		inline const blend_mode& get_blend_mode() const { return m_blend_mode; }
		inline blend_mode& get_blend_mode() { return m_blend_mode; }

		inline void set_transform(const glm::mat4x4& value) { m_transform = value; }
		inline const glm::mat4& get_transform() const { return m_transform; }
		inline glm::mat4& get_transform() { return m_transform; }

		inline static const render_states& get_default();

	protected:

	private:
		const texture* m_texture = &engine::get_instance()->get_default_texture();
		const shader_program* m_shader_program = &engine::get_instance()->get_default_shader_program();

		blend_mode m_blend_mode = blend_mode::blend_alpha;
		glm::mat4 m_transform{ 1.0f };
	};

	const render_states& render_states::get_default()
	{
		static render_states result;
		return result;
	}
}
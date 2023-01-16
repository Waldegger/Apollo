#pragma once

#include <string_view>

#include "graphics/render_window.h"
#include "graphics/texture.h"
#include "graphics/shader_program.h"
#include "graphics/program_layout.h"
#include "utility/utility.h"

union SDL_Event;

namespace age
{
	namespace window_flags
	{
		enum flag
		{
			fullscreen =			1 << 0,
			fullscreen_desktop =	1 << 1,
			hidden =				1 << 2,
			borderless =			1 << 3,
			resizable =				1 << 4,
			minimized =				1 << 5,
			maximized =				1 << 6,
			input_grabbed =			1 << 7
		};
	}

	class engine
	{
	public:
		engine();
		virtual ~engine();

		engine(const engine& other) = delete;
		engine(engine&& other) = delete;

		engine& operator = (const engine& other) = delete;
		engine& operator = (engine&& other) = delete;

	public:
	
		int32_t start(std::string_view title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags);
		void stop(int32_t exit_code);

		inline const render_window& get_render_window() const { return m_render_window; }
		inline render_window& get_render_window() { return m_render_window; }

		inline static const shader* get_default_vertex_shader() { return m_default_vertex_shader_ptr; }
		inline static const shader* get_default_fragment_shader() { return m_default_fragment_shader_ptr; }
		inline static const program_layout* get_default_program_layout() { return m_default_program_layout_ptr; }
		inline static const texture* get_default_texture() { return m_default_texture_ptr; }

	protected:

	private:
		virtual void on_create() = 0;
		virtual void on_update() = 0;
		virtual void on_destroy() = 0;
		virtual void on_process_event(SDL_Event& e);

		static int32_t init_lib(uint32_t flags);
		static void quit_lib();

		inline static const shader* m_default_vertex_shader_ptr;
		inline static const shader* m_default_fragment_shader_ptr;
		inline static const program_layout* m_default_program_layout_ptr;
		inline static const texture* m_default_texture_ptr;

		void init_defaults();

		void create();
		void update();
		void destroy();

		initializer<init_lib, quit_lib, uint32_t> m_initializer;
		render_window m_render_window;

		shader m_default_vertex_shader{ shader::shader_type::vertex };
		shader m_default_fragment_shader{ shader::shader_type::fragment };
		shader_program m_default_shader_program;
		program_layout m_default_program_layout;
		texture m_default_texture;
		
		int32_t m_exit_code;
		bool m_running;
		bool m_started;
	};
}
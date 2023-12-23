#pragma once

#include <string_view>

#include "graphics/render_window.h"
#include "graphics/vertex_array_object.h"
#include "graphics/vertex_buffer_object.h"
#include "graphics/texture.h"
#include "graphics/shader_program.h"
#include "graphics/program_layout.h"
#include "graphics/uniform_buffer_object.h"
#include "graphics/vertex_array_object.h"
#include "graphics/vertex_buffer_object.h"
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
		void stop(int32_t exit_code = EXIT_SUCCESS);

		inline const render_window& get_render_window() const { return m_render_window; }
		inline render_window& get_render_window() { return m_render_window; }

		inline const vertex_array_object& get_default_vertex_array_object() const { return m_default_vertex_array_object; }
		inline const vertex_buffer_object& get_default_vertex_buffer_object() const { return m_default_vertex_buffer_object; }
		inline const vertex_buffer_object& get_default_element_buffer_object() const { return m_default_element_buffer_object; }

		inline vertex_array_object& get_default_vertex_array_object() { return m_default_vertex_array_object; }
		inline vertex_buffer_object& get_default_vertex_buffer_object() { return m_default_vertex_buffer_object; }
		inline vertex_buffer_object& get_default_element_buffer_object() { return m_default_element_buffer_object; }

		inline const uniform_buffer_object& get_vp_matrix_ubo() const{ return m_vp_matrix_ubo; }
		inline const uniform_buffer_object& get_model_matrix_ubo() const { return m_model_matrix_ubo; }
		inline const uniform_buffer_object& get_texture_matrix_ubo() const { return m_texture_matrix_ubo; }
		inline const uniform_buffer_object& get_viewport_uniform_buffer_object() const { return m_viewport_ubo; }
		inline uniform_buffer_object& get_vp_matrix_ubo() { return m_vp_matrix_ubo; }
		inline uniform_buffer_object& get_model_matrix_ubo() { return m_model_matrix_ubo; }
		inline uniform_buffer_object& get_texture_matrix_ubo() { return m_texture_matrix_ubo; }
		inline uniform_buffer_object& get_viewport_uniform_buffer_object() { return m_viewport_ubo; }

		inline const shader& get_default_vertex_shader() const { return m_default_vertex_shader; }
		inline const shader& get_default_fragment_shader() const { return m_default_fragment_shader; }
		inline const program_layout& get_default_program_layout() const { return m_default_program_layout; }
		inline const texture& get_default_texture() const { return m_default_texture; }

		inline static engine* get_instance() { return m_instance; }

		inline static constexpr uint32_t get_a_position_index() { return 0; }
		inline static constexpr uint32_t get_a_color_index() { return 1; }
		inline static constexpr uint32_t get_a_tex_coords_index() { return 2; }

	protected:

	private:
		virtual void on_create() = 0;
		virtual void on_update() = 0;
		virtual void on_destroy() = 0;
		virtual void on_process_event(SDL_Event& e);

		static int32_t init_lib(uint32_t flags);
		static void quit_lib();

		void init_defaults();

		void create();
		void update();
		void destroy();

		inline static engine* m_instance = nullptr;

		initializer<init_lib, quit_lib, uint32_t> m_initializer;
		render_window m_render_window;

		vertex_array_object m_default_vertex_array_object;
		vertex_buffer_object m_default_vertex_buffer_object{ vertex_buffer_object::target::array };
		vertex_buffer_object m_default_element_buffer_object{ vertex_buffer_object::target::element_array };

		uniform_buffer_object m_vp_matrix_ubo;
		uniform_buffer_object m_model_matrix_ubo;
		uniform_buffer_object m_texture_matrix_ubo;
		uniform_buffer_object m_viewport_ubo;
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
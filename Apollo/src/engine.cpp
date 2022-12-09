#include "engine.h"

#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL2/SDL.h>

#include <stdexcept>
#include <array>

#include "graphics/render_states.h"

namespace agl
{
	engine::engine()
		: m_initializer{ SDL_INIT_VIDEO }
		, m_default_program_layout{ m_default_shader_program }
	{
		static bool engine_instanced;

		if (engine_instanced)
		{
			throw std::runtime_error{ "Only one instance of engine is allowed!" };
		}

		init_defaults();

		engine_instanced = true;
	}

	engine::~engine()
	{
		m_default_vertex_shader_ptr = nullptr;
		m_default_fragment_shader_ptr = nullptr;
		m_default_program_layout_ptr = nullptr;
		m_default_texture_ptr = nullptr;
	}

	int32_t engine::start(const std::string_view& title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags)
	{
		/*
		SDL_WINDOW_FULLSCREEN: fullscreen window
		SDL_WINDOW_FULLSCREEN_DESKTOP: fullscreen window at desktop resolution
		SDL_WINDOW_OPENGL: window usable with an OpenGL context
		SDL_WINDOW_VULKAN: window usable with a Vulkan instance
		SDL_WINDOW_METAL: window usable with a Metal instance
		SDL_WINDOW_HIDDEN: window is not visible
		SDL_WINDOW_BORDERLESS: no window decoration
		SDL_WINDOW_RESIZABLE: window can be resized
		SDL_WINDOW_MINIMIZED: window is minimized
		SDL_WINDOW_MAXIMIZED: window is maximized
		SDL_WINDOW_INPUT_GRABBED: window has grabbed input focus
		SDL_WINDOW_ALLOW_HIGHDPI: window should be created in high-DPI mode if supported (>= SDL 2.0.1)
		*/

		uint32_t sdl_flags = SDL_WINDOW_OPENGL;
		std::array<uint32_t, 8> own_flags{
			static_cast<uint32_t>(window_flags::fullscreen),
			static_cast<uint32_t>(window_flags::fullscreen_desktop),
			static_cast<uint32_t>(window_flags::hidden),
			static_cast<uint32_t>(window_flags::borderless),
			static_cast<uint32_t>(window_flags::resizable),
			static_cast<uint32_t>(window_flags::minimized),
			static_cast<uint32_t>(window_flags::maximized),
			static_cast<uint32_t>(window_flags::input_grabbed),
		};

		std::array<uint32_t, 8> lib_flags{
			SDL_WINDOW_FULLSCREEN,
			SDL_WINDOW_FULLSCREEN_DESKTOP,
			SDL_WINDOW_HIDDEN,
			SDL_WINDOW_BORDERLESS,
			SDL_WINDOW_RESIZABLE,
			SDL_WINDOW_MINIMIZED,
			SDL_WINDOW_MAXIMIZED,
			SDL_WINDOW_INPUT_GRABBED
		};

		for (size_t i = 0; i < own_flags.size(); ++i)
		{
			if (flags & own_flags[i])
				sdl_flags |= lib_flags[i];
		}

		m_render_window.open(title, display_index, width, height, sdl_flags);

		create();

		//Put main loop here
		//----
		while (m_running)
		{
			update();
		}
		//----

		destroy();

		return m_exit_code;
	}

	void engine::stop(int32_t exit_code)
	{
		m_running = false;

		m_exit_code = exit_code;
	}

	int32_t engine::init_lib(uint32_t flags)
	{
		return SDL_Init(flags);
	}

	void engine::quit_lib()
	{
		SDL_Quit();
	}

	void engine::init_defaults()
	{
		const char* vertex_shader_source =
			"uniform mat4 u_mvp_matrix;\n"
			"attribute vec2 a_position;\n"
			"attribute vec4 a_color;\n"
			"attribute vec2 a_tex_coords;\n"
			"varying vec4 v_color;\n"
			"varying vec2 v_tex_coords;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = u_mvp_matrix * vec4(a_position, 0.0, 1.0);\n"
			"   v_color = a_color;\n"
			"   v_tex_coords = a_tex_coords;\n"
			"}";

		const char* fragment_shader_source =
			"precision mediump float;\n"
			"uniform sampler2D u_texture;\n"
			"varying vec4 v_color;\n"
			"varying vec2 v_tex_coords;\n"
			"void main()\n"
			"{\n"
			"    vec4 texel = texture2D(u_texture, v_tex_coords);\n"
			"    if(texel.a == 0.0) discard;"
			"    gl_FragColor = v_color * texel;\n"
			"}";

		m_default_vertex_shader.compile(vertex_shader_source);
		m_default_fragment_shader.compile(fragment_shader_source);

		m_default_shader_program.attach_shader(m_default_vertex_shader);
		m_default_shader_program.attach_shader(m_default_fragment_shader);
		m_default_shader_program.bind_attrib_location(render_target::A_POSITION_INDEX, "a_position");
		m_default_shader_program.bind_attrib_location(render_target::A_COLOR_INDEX, "a_color");
		m_default_shader_program.bind_attrib_location(render_target::A_TEX_COORDS_INDEX, "a_tex_coords");
		m_default_shader_program.link();

		m_default_program_layout.mvp_matrix_name("u_mvp_matrix");
		m_default_program_layout.texture_name("u_texture");

		m_default_texture.create(vector2u{ 1, 1 });
		m_default_texture.update(std::array<uint8_t, 4>{255, 255, 255, 255}.data());

		m_default_vertex_shader_ptr = &m_default_vertex_shader;
		m_default_fragment_shader_ptr = &m_default_fragment_shader;
		m_default_program_layout_ptr = &m_default_program_layout;
		m_default_texture_ptr = &m_default_texture;
	}

	void engine::create()
	{
		on_create();
	}

	void engine::update()
	{
		on_update();
	}

	void engine::destroy()
	{
		on_destroy();
	}
}
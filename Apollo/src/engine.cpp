#include "engine.h"

#include <stdexcept>
#include <array>

#include "graphics/render_states.h"

namespace agl
{
	engine::engine()
		: m_initializer{ SDL_INIT_VIDEO }
	{
		static bool engine_instanced;

		if (engine_instanced)
		{
			throw std::runtime_error{ "Only one instance of engine is allowed!" };
		}
		
		engine_instanced = true;
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
			static_cast<uint32_t>(window_flag::fullscreen),
			static_cast<uint32_t>(window_flag::fullscreen_desktop),
			static_cast<uint32_t>(window_flag::hidden),
			static_cast<uint32_t>(window_flag::borderless),
			static_cast<uint32_t>(window_flag::resizable),
			static_cast<uint32_t>(window_flag::minimized),
			static_cast<uint32_t>(window_flag::maximized),
			static_cast<uint32_t>(window_flag::input_grabbed),
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
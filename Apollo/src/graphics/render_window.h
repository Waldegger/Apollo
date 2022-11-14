#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <functional>
#include <type_traits>

#include "../utility/utility.h"

namespace apollo
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
	enum class window_flag
	{
		FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN_DESKTOP,
		HIDDEN = SDL_WINDOW_HIDDEN,
		BORDERLESS = SDL_WINDOW_BORDERLESS,
		RESIZABLE = SDL_WINDOW_RESIZABLE,
		MINIMIZED = SDL_WINDOW_MINIMIZED,
		MAXIMIZED = SDL_WINDOW_MAXIMIZED,
		INPUT_GRABBED = SDL_WINDOW_INPUT_GRABBED,
		ALLOW_HIGHDPI = SDL_WINDOW_ALLOW_HIGHDPI
	};

	inline uint32_t operator | (window_flag lhs, window_flag rhs)
	{
		return static_cast<std::underlying_type<window_flag>::type>(lhs) | static_cast<std::underlying_type<window_flag>::type>(rhs);
	}

	class render_window
	{
	public:
		render_window(const std::string_view& title, uint16_t width, uint16_t height, uint32_t flags);
		~render_window();
	public:

		void clear();
		void display();

		void close();
		bool is_open() const;
	protected:

	private:
		using window_handle_deleter = deleter<SDL_Window, SDL_DestroyWindow>;
		std::unique_ptr<SDL_Window, window_handle_deleter> m_windowhandle;

		using gl_context_type = std::remove_pointer<SDL_GLContext>::type;
		using gl_context_deleter = deleter<gl_context_type, SDL_GL_DeleteContext>;
		std::unique_ptr<gl_context_type, gl_context_deleter> m_GL_context;

		bool m_is_open = true;
	};
}

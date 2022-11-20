#pragma once

#include <SDL2/SDL.h>

#include <string_view>

namespace agl
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
	enum class window_flag : uint32_t
	{
		fullscreen = SDL_WINDOW_FULLSCREEN,
		fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
		hidden = SDL_WINDOW_HIDDEN,
		borderless = SDL_WINDOW_BORDERLESS,
		resizable = SDL_WINDOW_RESIZABLE,
		minimized = SDL_WINDOW_MINIMIZED,
		maximized = SDL_WINDOW_MAXIMIZED,
		input_grabbed = SDL_WINDOW_INPUT_GRABBED,
		allow_highdpi = SDL_WINDOW_ALLOW_HIGHDPI
	};

	inline uint32_t operator | (window_flag lhs, window_flag rhs)
	{
		return static_cast<std::underlying_type<window_flag>::type>(lhs) | static_cast<std::underlying_type<window_flag>::type>(rhs);
	}

	class engine
	{
	public:
		engine();
		virtual ~engine() = default;

		engine(const engine& other) = delete;
		engine(engine&& other) = delete;

		engine& operator = (const engine& other) = delete;
		engine& operator = (engine&& other) = delete;

	public:
	
		void start(const std::string_view& title, uint32_t display_index, int32_t width, int32_t height, uint32_t flags);

		void stop(int32_t exit_code);

	protected:
		virtual void on_create() = 0;
		virtual void on_update() = 0;
		virtual void on_destroy() = 0;

	private:
		void create();
		void update();
		void destroy();

		bool m_running = true;
	};
}
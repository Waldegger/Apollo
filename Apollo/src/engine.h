#pragma once

#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL2/SDL.h>
#include <string_view>

#include "graphics/render_window.h"
#include "graphics/texture.h"
#include "graphics/shader_program.h"
#include "utility/utility.h"

namespace agl
{
	enum class window_flag : uint32_t
	{
		fullscreen = 1 << 0,
		fullscreen_desktop = 1 << 1,
		hidden = 1 << 2,
		borderless = 1 << 3,
		resizable = 1 << 4,
		minimized = 1 << 5,
		maximized = 1 << 6,
		input_grabbed = 1 << 7
	};

	inline std::underlying_type<window_flag>::type operator | (window_flag lhs, window_flag rhs)
	{
		return static_cast<std::underlying_type<window_flag>::type>(lhs) | static_cast<std::underlying_type<window_flag>::type>(rhs);
	}

	inline std::underlying_type<window_flag>::type operator & (std::underlying_type<window_flag>::type lhs, window_flag rhs)
	{
		return static_cast<std::underlying_type<window_flag>::type>(lhs) & static_cast<std::underlying_type<window_flag>::type>(rhs);
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
	
		int32_t start(const std::string_view& title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags);
		void stop(int32_t exit_code);

		inline const render_window& get_render_window() const { return m_render_window; }
		inline render_window& get_render_window() { return m_render_window; }

	protected:
		virtual void on_create() = 0;
		virtual void on_update() = 0;
		virtual void on_destroy() = 0;

	private:
		static int32_t init_lib(uint32_t flags);
		static void quit_lib();

		void create();
		void update();
		void destroy();

		initializer<SDL_Init, SDL_Quit, Uint32> m_initializer;
		render_window m_render_window;

		int32_t m_exit_code = 0;
		bool m_running = true;
	};
}
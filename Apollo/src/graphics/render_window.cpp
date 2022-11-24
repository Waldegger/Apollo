#include "render_window.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>
#include <string_view>
#include <stdexcept>

namespace agl
{
	render_window::render_window()
		: m_clear_flags{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT }
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//OpenGL ES profile - only a subset of the base OpenGL functionality is available
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

		if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Warning: Unable to set Doublebuffer! SDL Error: %s", SDL_GetError());
		}

		m_windowhandle.reset(SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 64, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL));

		if (!m_windowhandle)
		{
			throw std::runtime_error{ std::string{ "Failed to create window\nSDL2 Error: " } + SDL_GetError() };
		}

		m_GL_context.reset(SDL_GL_CreateContext(static_cast<SDL_Window*>(m_windowhandle.get())));

		if (!m_GL_context)
		{
			throw std::runtime_error{ std::string{ "Failed to create context\nSDL2 Error: " } + SDL_GetError() };
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(-1) < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
		}

		glewInit();

		glEnableVertexAttribArray(A_POSITION_INDEX);
		glEnableVertexAttribArray(A_COLOR_INDEX);
		glEnableVertexAttribArray(A_TEX_COORDS_INDEX);
	}

	void render_window::open(const std::string_view& title, uint32_t display_index, uint32_t width, uint32_t height, uint32_t flags)
	{
		auto handle = static_cast<SDL_Window*>(m_windowhandle.get());

		SDL_SetWindowTitle(handle, title.data());
		SDL_SetWindowSize(handle , width, height);
		SDL_SetWindowPosition(handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		if (flags & SDL_WINDOW_FULLSCREEN)
			SDL_SetWindowFullscreen(handle, SDL_WINDOW_FULLSCREEN);
		if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
			SDL_SetWindowFullscreen(handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (flags & SDL_WINDOW_BORDERLESS)
			SDL_SetWindowBordered(handle, SDL_FALSE);
		if (flags & SDL_WINDOW_RESIZABLE)
			SDL_SetWindowResizable(handle, SDL_TRUE);
		if (flags & SDL_WINDOW_MINIMIZED)
			SDL_MinimizeWindow(handle);
		if (flags & SDL_WINDOW_MAXIMIZED)
			SDL_MaximizeWindow(handle);
		if (flags & SDL_WINDOW_INPUT_GRABBED)
		{
			SDL_SetWindowKeyboardGrab(handle, SDL_TRUE);
			SDL_SetWindowInputFocus(handle);
		}
		if (!(flags & SDL_WINDOW_HIDDEN))
			SDL_ShowWindow(handle);
		
		glViewport(0, 0, width, height);                                                                                                                                            
	}

	void render_window::clear()
	{
		glClear(m_clear_flags);
	}

	void render_window::display()
	{
		SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_windowhandle.get()));
	}

	vector2u render_window::get_size() const
	{
		int w;
		int h;

		SDL_GetWindowSize(static_cast<SDL_Window*>(m_windowhandle.get()), &w, &h);

		return vector2u{ static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
	}

	void render_window::destroy_window_lib(void* window)
	{
		SDL_DestroyWindow(static_cast<SDL_Window*>(window));
	}

	void render_window::delete_context_lib(void* context)
	{
		SDL_GL_DeleteContext(context);
	}
}
#include "graphics/render_window.h"

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>
#include <sstream>
#include <string_view>
#include <stdexcept>

#include <utility/gl_check.h>

namespace age
{
	render_window::render_window()
		: m_clear_flags{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT }
	{
		//Use OpenGL 4.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//OpenGL ES profile - only a subset of the base OpenGL functionality is available
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

		if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		{
			std::stringstream warning;
			warning << "Warning: Unable to set Doublebuffer! SDL Error: " << SDL_GetError();

			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, warning.str().c_str());
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
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
		}

		//Create the shared_context which we will use to share resources in threads
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
		SDL_GLContext shared_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(m_windowhandle.get()));
		if (!shared_context)
		{
			throw std::runtime_error{ std::string{ "Failed to create shared context\nSDL2 Error: " } + SDL_GetError() };
		}

		m_GL_shared_context.reset(shared_context);

		//SDL_GL_CreateContext makes the new context current. So lets reset it to the old one
		if (SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_windowhandle.get()), m_GL_context.get()) != 0)
		{
			throw std::runtime_error{ std::string{ "Failed to make context current\nSDL2 Error: " } + SDL_GetError() };
		}

		glewInit();
		init();
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
		
		glm::vec2 view_size{ 1.0f, static_cast<float>(height) / static_cast<float>(width) };
		view_2d view{ view_size * 0.5f, view_size };

		apply_view(view);
	}

	void * render_window::get_shared_context() const
	{
		return m_GL_shared_context.get();
	}

	void render_window::clear()
	{
		GL_CALL(glClear(m_clear_flags));
	}

	void render_window::display()
	{
		SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_windowhandle.get()));
	}

	glm::u32vec2 render_window::get_size() const
	{
		int w;
		int h;

		SDL_GetWindowSize(static_cast<SDL_Window*>(m_windowhandle.get()), &w, &h);

		return glm::u32vec2{ static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
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
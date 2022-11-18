#include "render_window.h"

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>
#include <string_view>
#include <stdexcept>

namespace apollo
{
	render_window::render_window(const std::string_view& title, uint16_t width, uint16_t height, uint32_t flags)
	{
		static initializer<SDL_Init, SDL_Quit, Uint32> init_sdl{ SDL_INIT_VIDEO };

		//Use OpenGL 2.1 -- or better 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//OpenGL ES profile - only a subset of the base OpenGL functionality is available
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

		if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Warning: Unable to set Doublebuffer! SDL Error: %s", SDL_GetError());
		}

		m_windowhandle.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags | SDL_WINDOW_OPENGL));

		if (!m_windowhandle)
		{
			throw std::runtime_error{ std::string{ "Failed to create window\nSDL2 Error: " } + SDL_GetError() };
		}

		m_GL_context.reset(SDL_GL_CreateContext(m_windowhandle.get()));

		if (!m_GL_context)
		{
			throw std::runtime_error{ std::string{ "Failed to create context\nSDL2 Error: " } + SDL_GetError() };
		}

		glewInit();

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
		}

		glViewport(0, 0, width, height);
		glFrustum(0.0f, width, height, 0.0f, -1.0f, 1.0f);
	}

	render_window::~render_window()
	{

	}

	void render_window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void render_window::display()
	{
		SDL_GL_SwapWindow(m_windowhandle.get());
	}

	void render_window::close()
	{
		m_is_open = false;
	}

	bool render_window::is_open() const
	{
		return m_is_open;
	}
}
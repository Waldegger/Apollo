#include "clock.h"

#include <SDL2/SDL.h>

namespace age
{
	clock::clock()
	{}

	void clock::start()
	{
		m_now = SDL_GetPerformanceCounter();
	}

	double clock::restart()
	{
		m_last_time = m_now;
		m_now = SDL_GetPerformanceCounter();

		return (double)((m_now - m_last_time) / (double)SDL_GetPerformanceFrequency());
	}
}
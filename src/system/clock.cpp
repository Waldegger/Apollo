#include "system/clock.h"

#include <SDL3/SDL.h>

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

		return static_cast<double>(m_now - m_last_time) / static_cast<double>(SDL_GetPerformanceFrequency());
	}
}
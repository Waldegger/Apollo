#pragma once

#include <cstdint>

namespace agl
{
	class clock
	{
	public:
		clock();
	public:
		void start();
		double restart();
		
	protected:

	private:
		uint64_t m_last_time = 0;
		uint64_t m_now = 0;
	};
}
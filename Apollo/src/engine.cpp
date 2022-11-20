#include "engine.h"

#include <stdexcept>

namespace agl
{
	engine::engine()
	{
		static bool engine_instanced;

		if (engine_instanced)
		{
			throw std::runtime_error{ "Only one instane of engine is allowed!" };
		}



		engine_instanced = true;
	}

	void engine::start(const std::string_view& title, uint32_t display_index, int32_t width, int32_t height, uint32_t flags)
	{
		create();

		//Put main loop here
		//----
		while (m_running)
		{
			update();
		}
		//----

		destroy();
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
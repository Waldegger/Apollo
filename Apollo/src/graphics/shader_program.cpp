#include "shader_program.h"

//For testing
#include <iostream>

namespace apollo
{
	shader_program::shader_program()
	{

	}

	shader_program::shader_program(const std::initializer_list<std::reference_wrapper<const shader>>& shaders)
	{
		link(shaders);
	}

	void shader_program::link(const std::initializer_list<std::reference_wrapper<const shader>>& shaders)
	{
		std::cout << "linker called\n";

		for (auto& s : shaders)
		{
			std::cout << "shader in list\n";
		}
	}
}
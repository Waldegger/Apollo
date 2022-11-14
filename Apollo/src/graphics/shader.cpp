#include "shader.h"

#include <iostream>

namespace apollo
{
	shader::shader()
	{
		std::cout << "shader constructed\n";
	}

	shader::shader(const shader& other)
	{
		std::cout << "shader copy\n";
	}

	shader::shader(shader&& other)
	{
		std::cout << "shader moved\n";
	}
}
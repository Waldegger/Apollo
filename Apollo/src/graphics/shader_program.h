#pragma once

#include <initializer_list>
#include <functional>

#include "shader.h"

namespace apollo
{
	class shader_program
	{
	public:
		shader_program();
		shader_program(const std::initializer_list<std::reference_wrapper<const shader>>& shaders);

		void link(const std::initializer_list< std::reference_wrapper<const shader>>& shaders);

	public:

	protected:

	private:
	};
}
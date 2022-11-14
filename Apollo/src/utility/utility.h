#pragma once

#include <utility>

namespace apollo
{
	template <typename T, auto fn>
	struct deleter
	{
		void operator()(T* ptr)
		{
			fn(ptr);
		}
	};

	template <auto fn_init, auto fn_uninit, typename... init_params>
	struct initializer
	{
		initializer(init_params&&... params)
		{
			fn_init(std::forward<init_params>(params)...);
		}

		~initializer()
		{
			fn_uninit();
		}
	
		initializer(const initializer& other) = delete;
		initializer(initializer&& other) = delete;

		initializer& operator = (const initializer& other) = delete;
		initializer& operator = (initializer&& other) = delete;
	};
}
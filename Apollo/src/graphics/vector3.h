#pragma once

#include <type_traits>

namespace agl
{
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	struct vector3
	{
		vector3(T _x, T _y, T _z)
			: x{ _x }
			, y{ _y }
			, z{ _z }
		{}

		T x{};
		T y{};
		T z{};
	};
}
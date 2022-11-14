#pragma once

#include <type_traits>

namespace apollo
{
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	struct quaternion
	{
		T w{};
		T x{};
		T y{};
		T z{};
	};
}
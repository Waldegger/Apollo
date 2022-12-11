#pragma once

#include <type_traits>

namespace age
{
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	struct vector3
	{
		vector3() = default;

		vector3(T _x, T _y, T _z)
			: x{ _x }
			, y{ _y }
			, z{ _z }
		{}

		T x{};
		T y{};
		T z{};
	};

	using vector3u = vector3<uint32_t>;
	using vector3i = vector3<int32_t>;
	using vector3f = vector3<float>;
	using vector3d = vector3<double>;
}
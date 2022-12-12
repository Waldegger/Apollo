#pragma once

#include <type_traits>

namespace age
{
	template<typename T>
	struct vector3
	{
		vector3();
		vector3(T _x, T _y, T _z);

		inline constexpr bool is_equal_to(const vector3<T>& other);

		T x{};
		T y{};
		T z{};
	};

	template<typename T>
	vector3<T>::vector3()
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	vector3<T>::vector3(T _x, T _y, T _z)
		: x{ _x }
		, y{ _y }
		, z{ _z }
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	inline constexpr bool vector3<T>::is_equal_to(const vector3<T>& other)
	{
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	using vector3u = vector3<uint32_t>;
	using vector3i = vector3<int32_t>;
	using vector3f = vector3<float>;
	using vector3d = vector3<double>;
}
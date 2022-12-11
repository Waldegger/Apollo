#pragma once

#include <type_traits>

namespace age
{
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	struct vector2
	{
		constexpr vector2()
		{}

		constexpr vector2(T x, T y)
			: x{ x }
			, y{ y }
		{}

		inline constexpr T length2() { return x * x + y * y; }
		inline constexpr T length()
		{
			constexpr T l2 = length2();
			if constexpr (l2 == static_cast<T>(0) || l2 == static_cast<T>(1))
				return l2;

			return sqrt(l2);
		}

		inline constexpr bool is_equal_to(const vector2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		T x{};
		T y{};
	};

	template<typename T>
	constexpr vector2<T> operator + (const vector2<T>& lhs, const vector2<T>& rhs)
	{
		return vector2<T>{ lhs.x + rhs.x, lhs.y + rhs.y };
	}

	template<typename T>
	constexpr vector2<T>& operator += (vector2<T>& lhs, const vector2<T>& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;

		return lhs;
	}

	template<typename T>
	constexpr vector2<T> operator - (const vector2<T>& lhs, const vector2<T>& rhs)
	{
		return vector2<T>{ lhs.x - rhs.x, lhs.y - rhs.y };
	}

	template<typename T>
	constexpr vector2<T>& operator -= (vector2<T>& lhs, const vector2<T>& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;

		return lhs;
	}

	template<typename T>
	constexpr vector2<T> operator * (const vector2<T>& lhs, T factor)
	{
		return vector2<T>{ lhs.x * factor, lhs.y * factor };
	}

	template<typename T>
	constexpr vector2<T>& operator *= (vector2<T>& lhs, T factor)
	{
		lhs.x *= factor;
		lhs.y *= factor;

		return lhs;
	}

	template<typename T>
	constexpr vector2<T> operator / (const vector2<T>& lhs, T factor)
	{
		return vector2<T>{ lhs.x / factor, lhs.y / factor };
	}

	template<typename T>
	constexpr vector2<T>& operator /= (vector2<T>& lhs, T factor)
	{
		lhs.x /= factor;
		lhs.y /= factor;

		return lhs;
	}

	using vector2u = vector2<uint32_t>;
	using vector2i = vector2<int32_t>;
	using vector2f = vector2<float>;
	using vector2d = vector2<double>;
}
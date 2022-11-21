#pragma once

#include <type_traits>

namespace agl
{
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	struct vector2
	{
		vector2(T x, T y)
			: x{ x }
			, y{ y }
		{}

		constexpr T length2() { return x * x + y * y; }
		constexpr T length()
		{
			T l2 = length2();
			if (l2 == static_cast<T>(0.0) || l2 == static_cast<T>(1.0))
				return l2;

			if constexpr (sizeof(T) <= 4)
				return sqrtf(l2);
			else if constexpr (sizeof(T) <= 8)
				return sqtd(l2);
			else
				return sqtl(l2);
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
#pragma once

#include <type_traits>

namespace age
{
	template<typename T>
	struct vector2
	{
		constexpr vector2();
		constexpr vector2(T x, T y);
		
		template<typename U>
		constexpr vector2(const vector2<U>& other);

		inline constexpr T length2() const;
		inline constexpr T length() const;
		
		inline constexpr bool is_equal_to(const vector2<T>& other) const;
	
		T x{};
		T y{};
	};

	template<typename T>
	constexpr vector2<T>::vector2()
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	constexpr vector2<T>::vector2(T x, T y)
		: x{ x }
		, y{ y }
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	template<typename U>
	constexpr vector2<T>::vector2(const vector2<U>& other)
		: x{ static_cast<T>(other.x) }
		, y{ static_cast<T>(other.y) }
	{}

	template<typename T>
	constexpr T vector2<T>::length2() const
	{
		return x * x + y * y;
	}

	template<typename T>
	constexpr T vector2<T>::length() const
	{
		constexpr T l2 = length2();
		if constexpr (l2 == static_cast<T>(0) || l2 == static_cast<T>(1))
			return l2;

		return sqrt(l2);
	}

	template<typename T>
	constexpr bool vector2<T>::is_equal_to(const vector2<T>& other) const
	{
		return x == other.x && y == other.y;
	}

	template<typename T>
	constexpr bool operator == (const vector2<T>& lhs, const vector2<T>& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	template<typename T>
	constexpr bool operator != (const vector2<T>& lhs, const vector2<T>& rhs)
	{
		return lhs.x != rhs.x || lhs.y != rhs.y;
	}

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
	constexpr vector2<T> operator - (const vector2<T>& v)
	{
		return vector2<T>{ -v.x, -v.y };
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
	constexpr vector2<T> operator * (T factor, const vector2<T>& lhs)
	{
		return lhs * factor;
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
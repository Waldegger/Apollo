#pragma once

#include <stdint.h>
#include <cassert>

namespace age
{
	class angle
	{
	public:
		constexpr angle();
		inline constexpr float as_degrees() const;
		inline constexpr float as_radians() const;
		inline constexpr angle wrap_signed() const;
		inline constexpr angle wrap_unsigned() const;

	public:

	protected:

	private:
		friend constexpr angle degrees(float value);
		friend constexpr angle radians(float value);

		explicit constexpr angle(float degrees);

		float m_degrees{};
	};

	inline constexpr angle degrees(float value);
	inline constexpr angle radians(float value);

	inline constexpr bool operator == (angle lhs, angle rhs);
	inline constexpr bool operator != (angle lhs, angle rhs);
	inline constexpr bool operator < (angle lhs, angle rhs);
	inline constexpr bool operator > (angle lhs, angle rhs);
	inline constexpr bool operator <= (angle lhs, angle rhs);
	inline constexpr bool operator >= (angle lhs, angle rhs);
	inline constexpr angle operator - (angle rhs);
	inline constexpr angle operator + (angle lhs, angle rhs);
	inline constexpr angle& operator += (angle& lhs, angle rhs);
	inline constexpr angle operator - (angle lhs, angle rhs);
	inline constexpr angle& operator -= (angle& lhs, angle rhs);
	inline constexpr angle operator * (angle lhs, float rhs);
	inline constexpr angle operator * (float lhs, angle rhs);
	inline constexpr angle& operator *= (angle& lhs, float rhs);
	inline constexpr angle operator / (angle lhs, float rhs);
	inline constexpr angle& operator /= (angle& lhs, float rhs);
	inline constexpr float operator / (angle lhs, angle rhs);
	inline constexpr angle operator % (angle lhs, angle rhs);
	inline constexpr angle& operator %= (angle& lhs, angle rhs);

	inline constexpr angle operator "" _deg(long double angle);
	inline constexpr angle operator "" _deg(uint64_t angle);
	inline constexpr angle operator "" _rad(long double angle);
	inline constexpr angle operator "" _rad(uint64_t angle);

	namespace priv
	{
		constexpr float PI = 3.141592654f;

		constexpr float positive_remainder(float a, float b)
		{
			assert(b > 0.0f);

			const float val = a - static_cast<float>(static_cast<int32_t>(a / b)) * b;
			if (val >= 0.f)
				return val;
			
			return val + b;
		}
	}

	constexpr angle::angle() = default;

	constexpr float angle::as_degrees() const
	{
		return m_degrees;
	}

	constexpr float angle::as_radians() const
	{
		return m_degrees * (priv::PI / 180.0f);
	}

	constexpr angle angle::wrap_signed() const
	{
		return degrees(priv::positive_remainder(m_degrees + 180.0f, 360.0f) - 180.0f);
	}

	constexpr angle angle::wrap_unsigned() const
	{
		return degrees(priv::positive_remainder(m_degrees, 360.0f));
	}

	constexpr angle::angle(float degrees)
		: m_degrees(degrees)
	{}

	constexpr angle degrees(float value)
	{
		return angle{ value };
	}

	constexpr angle radians(float value)
	{
		return angle{ value * (180.0f / priv::PI) };
	}

	constexpr bool operator == (angle lhs, angle rhs)
	{
		return lhs.as_degrees() == rhs.as_degrees();
	}

	constexpr bool operator != (angle lhs, angle rhs)
	{
		return lhs.as_degrees() != rhs.as_degrees();
	}

	constexpr bool operator < (angle lhs, angle rhs)
	{
		return lhs.as_degrees() < rhs.as_degrees();
	}

	constexpr bool operator > (angle lhs, angle rhs)
	{
		return lhs.as_degrees() > rhs.as_degrees();
	}

	constexpr bool operator <= (angle lhs, angle rhs)
	{
		return lhs.as_degrees() <= rhs.as_degrees();
	}

	constexpr bool operator >= (angle lhs, angle rhs)
	{
		return lhs.as_degrees() >= rhs.as_degrees();
	}

	constexpr angle operator - (angle rhs)
	{
		return degrees(-rhs.as_degrees());
	}

	constexpr angle operator + (angle lhs, angle rhs)
	{
		return degrees(lhs.as_degrees() + rhs.as_degrees());
	}

	constexpr angle& operator += (angle& lhs, angle rhs)
	{
		lhs = lhs + rhs;
		return lhs;
	}

	constexpr angle operator - (angle lhs, angle rhs)
	{
		return degrees(lhs.as_degrees() - rhs.as_degrees());
	}

	constexpr angle& operator -= (angle& lhs, angle rhs)
	{
		lhs = lhs - rhs;
		return lhs;
	}

	constexpr angle operator * (angle lhs, float rhs)
	{
		return degrees(lhs.as_degrees() * rhs);
	}

	constexpr angle operator * (float lhs, angle rhs)
	{
		return degrees(lhs * rhs.as_degrees());
	}

	constexpr angle& operator *= (angle& lhs, float rhs)
	{
		lhs = lhs * rhs;
		return lhs;
	}

	constexpr angle operator / (angle lhs, float rhs)
	{
		return degrees(lhs.as_degrees() / rhs);
	}

	constexpr float operator / (angle lhs, angle rhs)
	{
		return lhs.as_degrees() / rhs.as_degrees();
	}

	constexpr angle& operator /= (angle& lhs, float rhs)
	{
		lhs = lhs / rhs;
		return lhs;
	}

	constexpr angle operator % (angle lhs, angle rhs)
	{
		return degrees(priv::positive_remainder(lhs.as_degrees(), rhs.as_degrees()));
	}

	constexpr angle& operator %= (angle& lhs, angle rhs)
	{
		lhs = lhs % rhs;
		return lhs;
	}

	constexpr angle operator"" _deg(long double angle)
	{
		return degrees(static_cast<float>(angle));
	}

	constexpr angle operator"" _deg(uint64_t angle)
	{
		return degrees(static_cast<float>(angle));
	}

	constexpr angle operator"" _rad(long double angle)
	{
		return radians(static_cast<float>(angle));
	}

	constexpr angle operator"" _rad(uint64_t angle)
	{
		return radians(static_cast<float>(angle));
	}
}
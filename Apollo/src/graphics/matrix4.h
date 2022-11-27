#pragma once

#include <cmath>
#include <type_traits>
#include <array>

#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"

namespace agl
{
	template<typename T>
	class matrix4
	{
	public:
		constexpr matrix4();
		constexpr matrix4(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22);
		constexpr matrix4(const std::array<T, 4 * 4>& m);

	public:
		constexpr T operator[](size_t i) const { return m_data[i]; }
		constexpr operator const T* () const { return m_data.data(); }

		inline static constexpr const matrix4<T>& get_identity() { static constexpr matrix4<T> result; return result; }

		constexpr const std::array<T, 4 * 4>& get_data() const { return m_data; }

		constexpr matrix4<T> get_inverse() const
		{
			// Compute the determinant
			T det =	m_data[0] * (m_data[15] * m_data[5] - m_data[7] * m_data[13]) -
					m_data[1] * (m_data[15] * m_data[4] - m_data[7] * m_data[12]) +
					m_data[3] * (m_data[13] * m_data[4] - m_data[5] * m_data[12]);
			
			// Compute the inverse if the determinant is not zero
			// (don't use an epsilon because the determinant may *really* be tiny)
			if (det != 0.0)
			{
				return matrix4<T>
				{	
					(m_data[15] * m_data[5] - m_data[7] * m_data[13]) / det,
					-(m_data[15] * m_data[4] - m_data[7] * m_data[12]) / det,
					(m_data[13] * m_data[4] - m_data[5] * m_data[12]) / det,
					-(m_data[15] * m_data[1] - m_data[3] * m_data[13]) / det,
					(m_data[15] * m_data[0] - m_data[3] * m_data[12]) / det,
					-(m_data[13] * m_data[0] - m_data[1] * m_data[12]) / det,
					(m_data[7] * m_data[1] - m_data[3] * m_data[5]) / det,
					-(m_data[7] * m_data[0] - m_data[3] * m_data[4]) / det,
					(m_data[5] * m_data[0] - m_data[1] * m_data[4]) / det 
				};
			}
			else
			{
				return get_identity();
			}
		}

		[[nodiscard]] constexpr vector2<T> transform_point(const vector2<T>& point)
		{
			return vector2<T>
			{	
				m_data[0] * point.x + m_data[4] * point.y + m_data[12],
				m_data[1] * point.x + m_data[5] * point.y + m_data[13]
			};
		}

		constexpr matrix4<T>& combine(const matrix4<T>& other)
		{
			auto& a = *this;
			auto& b = other;
			
			*this = matrix4
			{
				{
					a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
					a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
					a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
					a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
					a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
					a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
					a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
					a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
					a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
					a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
					a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
					a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
					a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
					a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
					a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
					a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],
				}
			};
			
			return *this;
		}

		constexpr matrix4<T>& translate(const vector2<T>& offset)
		{
			matrix4<T> translation
			{ 
				1.0, 0.0, offset.x,
				0.0, 1.0, offset.y,
				0.0, 0.0, 1.0 
			};

			return combine(translation);
		}

		constexpr matrix4<T>& translate(const vector3<T>& offset)
		{
			matrix4<T> translation
			{ 
				{
					1.0, 0.0, 0.0, offset.x,
					0.0, 1.0, 0.0, offset.y,
					0.0, 0.0, 1.0, offset.z,
					0.0, 0.0, 0.0, 1.0
				}
			};

			return combine(translation);
		}

		constexpr matrix4<T>& rotate(T angle, const vector3<T>& axis)
		{
			T rad = angle * 3.141592654 / 180.0;
			T radover2 = angle * 0.5f;

			T cos = std::cos(radover2);
			T sin = std::sin(radover2);

			quaternion<T> q
			{
				cos,
				axis.x * sin,
				axis.y * sin,
				axis.z * sin
			};

			T pw = q.w * q.w;
			T px = q.x * q.x;
			T py = q.y * q.y;
			T pz = q.z * q.z;

			matrix4<T> rotation
			{
				{
					pw + px - py - pz, 2 * q.x * q.y - 2 * q.w * q.z, 2 * q.x * q.z + 2 * q.w * q.y,	0.0,
					2 * q.x * q.y, + 2 * q.w * q.z, pw - px + py - pz, 2 * q.y * q.z + 2 * q.w * q.x,	0.0,
					2 * q.x * q.z - 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, pw - px - py + pz,	0.0,
					0.0, 0.0, 0.0,																		1.0
				}
			};

			return combine(rotation);
		}

		constexpr matrix4<T>& rotate_x(T angle)
		{
			T rad = angle * 3.141592654 / 180.0;
			T cos = std::cos(rad);
			T sin = std::sin(rad);

			matrix4<T> rotation
			{ 
				{
					0.0, 0.0, 1.0, 0.0,
					cos, sin, 0.0, 0.0,
					-sin, cos, 0.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				}
			};

			return combine(rotation);
		}

		constexpr matrix4<T>& rotate_y(T angle)
		{
			T rad = angle * 3.141592654 / 180.0;
			T cos = std::cos(rad);
			T sin = std::sin(rad);

			matrix4<T> rotation
			{ 
				{
					-sin, cos, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					cos, sin, 0.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				}
			};

			return combine(rotation);
		}

		constexpr matrix4<T>& rotate_z(T angle)
		{
			T rad = angle * 3.141592654 / 180.0;
			T cos = std::cos(rad);
			T sin = std::sin(rad);

			matrix4<T> rotation
			{	
				{
					cos, -sin, 0.0, 0.0,
					sin, cos, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				}
			};

			return combine(rotation);
		}

		constexpr matrix4<T>& rotate_z(T angle, const vector2<T>& center)
		{
			T rad = angle * 3.141592654 / 180.0;
			T cos = std::cos(rad);
			T sin = std::sin(rad);

			matrix4<T> rotation
			{	cos, -sin, center.x * (1.0 - cos) + center.y * sin,
				sin, cos, center.y * (1.0 - cos) - center.x * sin,
				0.0, 0.0, 1.0 
			};
			
			return combine(rotation);
		}

		constexpr matrix4<T>& scale(const vector2<T>& factors)
		{
			
			matrix4<T> scaling
			{ 
				factors.x, 0.0, 0.0,
				0.0, factors.y, 0.0,
				0.0, 0.0, 1.0 
			};
			
			return combine(scaling);
		}

		constexpr matrix4<T>& scale(const vector2<T>& factors, const vector2<T>& center)
		{
			matrix4<T> scaling
			{ 
				factors.x, 0.0, center.x * (1.0 - factors.x),
				0.0, factors.y, center.y * (1.0 - factors.y),
				0.0, 0.0, 1.0 
			};

			return combine(scaling);
		}

		constexpr matrix4<T>& scale(const vector3<T>& factors)
		{

			matrix4<T> scaling
			{ 
				{	
					factors.x, 0.0, 0.0, 0.0,
					0.0, factors.y, 0.0, 0.0,
					0.0, 0.0, factors.y, 0.0,
					0.0, 0.0, 0.0,		 1.0
				} 
			};

			return combine(scaling);
		}

	protected:

	private:
		std::array<T, 4 * 4> m_data =
		{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	};

	template<typename T>
	constexpr matrix4<T> operator * (const matrix4<T>& lhs, const matrix4<T>& rhs)
	{
		auto& a = lhs;
		auto& b = rhs;

		return matrix4
		{ 
			{
				a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
				a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
				a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
				a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
				a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
				a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
				a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
				a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
				a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
				a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
				a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
				a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
				a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
				a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
				a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
				a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],
			}
		};
	}

	using matrix4u = matrix4<uint32_t>;
	using matrix4i = matrix4<int32_t>;
	using matrix4f = matrix4<float>;
	using matrix4d = matrix4<double>;

	template<typename T>
	constexpr matrix4<T>::matrix4()
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	constexpr matrix4<T>::matrix4(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22)
		: m_data{
			a00, a10, 0.0, a20,
			a01, a11, 0.0, a21,
			0.0, 0.0, 1.0, 0.0,
			a02, a12, 0.0, a22 }
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	constexpr matrix4<T>::matrix4(const std::array<T, 4 * 4>& m)
		: m_data{ m }
	{
		static_assert(std::is_arithmetic_v<T>, "Type needs to be arithmetic");
	}

	template<typename T>
	constexpr matrix4<T>& operator *= (const matrix4<T>& lhs, const matrix4<T>& rhs)
	{
		return lhs.combine(rhs);
	}
}
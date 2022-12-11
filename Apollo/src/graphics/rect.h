#pragma once

#include <optional>
#include <algorithm>

#include "../algebra/vector2.h"

namespace age
{
	template<typename T>
	struct rect
	{
		constexpr rect() = default;
		constexpr rect(const vector2<T>& positon, const vector2<T>& size);

		template<typename U>
		constexpr explicit rect(const rect<U>& rectangle);

		constexpr bool contains(vector2<T>& point) const;

		constexpr std::optional<rect<T>> find_intersection(const rect<T>& rectangle) const;

		constexpr vector2<T> get_position() const;
		constexpr vector2<T> get_size() const;

		T left{};
		T top{};
		T width{};
		T height{};
	};

	using int_rect = rect<int32_t>;
	using uint_rect = rect<uint32_t>;
	using float_rect = rect<float>;

	template<typename T>
	constexpr rect<T>::rect(const vector2<T>& position, const vector2<T>& size)
		: left{ position.x }
		, top{ position.y }
		, width{ size.x }
		, height{ size.y }
	{}

	template<typename T>
	template<typename U>
	constexpr rect<T>::rect(const rect<U>& rectangle)
		: left{ static_cast<T>(rectangle.left) }
		, top{ static_cast<T>(rectangle.top) }
		, width{ static_cast<T>(rectangle.width) }
		, height{ static_cast<T>(rectangle.height) }
	{}

	template<typename T>
	constexpr bool rect<T>::contains(vector2<T>& point) const
	{
		const T min_x = std::min(left, static_cast<T>(left + width));
		const T max_x = std::max(left, static_cast<T>(left + width));
		const T min_y = std::min(top, static_cast<T>(top + height));
		const T max_y = std::max(top, static_cast<T>(top + height));

		return (point.x >= min_x) && (point.x < max_x) && (point.y >= min_y) && (point.y < max_y);
	}

	template <typename T>
	constexpr std::optional<rect<T>> rect<T>::find_intersection(const rect<T>& rectangle) const
	{
		// Not using 'std::min' and 'std::max' to avoid depending on '<algorithm>'
		const auto min = [](T a, T b) { return (a < b) ? a : b; };
		const auto max = [](T a, T b) { return (a < b) ? b : a; };

		// Rectangles with negative dimensions are allowed, so we must handle them correctly

		// Compute the min and max of the first rectangle on both axes
		const T r1_min_x = min(left, static_cast<T>(left + width));
		const T r1_max_x = max(left, static_cast<T>(left + width));
		const T r1_min_y = min(top, static_cast<T>(top + height));
		const T r1_max_y = max(top, static_cast<T>(top + height));

		// Compute the min and max of the second rectangle on both axes
		const T r2_min_x = min(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
		const T r2_max_x = max(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
		const T r2_min_y = min(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
		const T r2_max_y = max(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));

		// Compute the intersection boundaries
		const T inter_left = max(r1_min_x, r2_min_x);
		const T inter_top = max(r1_min_y, r2_min_y);
		const T inter_right = min(r1_max_x, r2_max_x);
		const T inter_bottom = min(r1_max_y, r2_max_y);

		// If the intersection is valid (positive non zero area), then there is an intersection
		if ((inter_left < inter_right) && (inter_top < inter_bottom))
			return rect<T>({ inter_left, inter_top }, { inter_right - inter_left, inter_bottom - inter_top });
		
		return std::nullopt;
	}

	template<typename T>
	constexpr vector2<T> rect<T>::get_position() const
	{
		return vector2<T>{ left, top };
	}

	template<typename T>
	constexpr vector2<T> rect<T>::get_size() const
	{
		return vector2<T>{ width, height };
	}

	template <typename T>
	constexpr bool operator == (const rect<T>& lhs, const rect<T>& rhs)
	{
		return (lhs.left == rhs.left) && (lhs.width == rhs.width) && (lhs.top == rhs.top) && (lhs.height == rhs.height);
	}

	template <typename T>
	constexpr bool operator != (const rect<T>& lhs, const rect<T>& rhs)
	{
		return !(lhs == rhs);
	}
}
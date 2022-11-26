#pragma once

namespace agl
{
	struct blend_mode
	{
		enum class factor
		{
			zero,
			one,
			src_color,
			one_minus_src_color,
			dst_color,
			one_minus_dst_color,
			src_alpha,
			one_minus_src_alpha,
			dst_alpha,
			one_minus_dst_alpha
		};

		enum class equation
		{
			add,
			subtract,
			reverse_subtract,
			min,
			max
		};

		factor color_src_factor = factor::src_alpha;			//!< Source blending factor for the color channels
		factor color_dst_factor = factor::one_minus_src_alpha;	//!< Destination blending factor for the color channels
		equation color_equation = equation::add;				//!< Blending equation for the color channels
		factor alpha_src_factor = factor::one;					//!< Source blending factor for the alpha channel
		factor alpha_dst_factor = factor::one_minus_src_alpha;	//!< Destination blending factor for the alpha channel
		equation alpha_equation = equation::add;				//!< Blending equation for the alpha channel
	};

	constexpr inline bool operator == (const blend_mode& lhs, const blend_mode& rhs)
	{
		return lhs.color_src_factor == rhs.color_src_factor
			&& lhs.color_dst_factor == rhs.color_dst_factor
			&& lhs.color_equation == rhs.color_equation
			&& lhs.alpha_src_factor == rhs.alpha_src_factor
			&& lhs.alpha_dst_factor == rhs.alpha_dst_factor
			&& lhs.alpha_equation == rhs.alpha_equation;
	}

	constexpr inline bool operator != (const blend_mode& lhs, const blend_mode& rhs)
	{
		return !(lhs == rhs);
	}
}
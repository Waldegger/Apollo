#include "graphics/blend_mode.h"

namespace age
{
	blend_mode::blend_mode(factor source_factor, factor destination_factor, equation blend_equation)
		: color_src_factor{ source_factor }
		, color_dst_factor{ destination_factor}
		, color_equation{ blend_equation }
		, alpha_src_factor{ source_factor}
		, alpha_dst_factor{ destination_factor }
		, alpha_equation{ blend_equation }
	{}

	blend_mode::blend_mode(factor color_source_factor, factor color_destination_factor, equation color_blend_equation, factor alpha_source_factor, factor alpha_destination_factor, equation alpha_blend_equation)
		: color_src_factor{ color_source_factor }
		, color_dst_factor{ color_destination_factor }
		, color_equation{ color_blend_equation }
		, alpha_src_factor{ alpha_source_factor }
		, alpha_dst_factor{ alpha_destination_factor }
		, alpha_equation{ alpha_blend_equation }
	{}

	const blend_mode blend_mode::blend_alpha{ blend_mode::factor::src_alpha, blend_mode::factor::one_minus_src_alpha, blend_mode::equation::add, blend_mode::factor::one, blend_mode::factor::one_minus_src_alpha, blend_mode::equation::add };
	const blend_mode blend_mode::blend_add{ blend_mode::factor::src_alpha, blend_mode::factor::one, blend_mode::equation::add, blend_mode::factor::one, blend_mode::factor::one, blend_mode::equation::add };
	const blend_mode blend_mode::blend_multiply{ blend_mode::factor::dst_color, blend_mode::factor::zero, blend_mode::equation::add };
	/*OpenGL ES 3.0
	const blend_mode blend_mode::blend_min{ blend_mode::factor::one, blend_mode::factor::one, blend_mode::equation::min };
	const blend_mode blend_mode::blend_max{ blend_mode::factor::one, blend_mode::factor::one, blend_mode::equation::max };
	*/
	const blend_mode blend_mode::blend_none(blend_mode::factor::one, blend_mode::factor::zero, blend_mode::equation::add);
}
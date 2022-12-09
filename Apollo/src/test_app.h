#pragma once

#include "engine.h"
#include "graphics/shader_program.h"
#include "graphics/vertex_2d.h"
#include "graphics/matrix4.h"
#include "graphics/texture.h"
#include "graphics/view_2d.h"
#include "graphics/font.h"
#include "graphics/text.h"
#include "system/clock.h"

class test_app
	: public agl::engine
{
public:

public:

protected:
	virtual void on_create() override;
	virtual void on_update() override;
	virtual void on_destroy() override;

private:
    static constexpr uint32_t A_POSITION_INDEX = 0;
    static constexpr uint32_t A_COLOR_INDEX = 1;
    static constexpr uint32_t A_TEX_COORDS_INDEX = 2;

    std::array<agl::vertex_2d, 4> m_vertices = 
    {
        agl::vertex_2d{ agl::vector2f{ 0.0f, 0.0f } },
        agl::vertex_2d{ agl::vector2f{ 1.0f, 0.0f } },
        agl::vertex_2d{ agl::vector2f{ 1.0f, 1.777777f } },
        agl::vertex_2d{ agl::vector2f{ 0.0f, 1.777777f } }
    };
    
    std::array<agl::vertex_2d, 4> m_rect_vertices =
    {
        agl::vertex_2d{ agl::vector2f{ 0.0f, 0.0f }, agl::color{255, 255, 255, 255 }, agl::vector2f{0.0f, 109.0f / 2048.0f} },
        agl::vertex_2d{ agl::vector2f{ 0.18f, 0.0f }, agl::color{255, 255, 255, 255 }, agl::vector2f{110.0f / 1024.0f, 109.0f / 2048.0f} },
        agl::vertex_2d{ agl::vector2f{ 0.18f, 0.045f }, agl::color{255, 255, 255, 255 }, agl::vector2f{110.0f / 1024.0f, 136.0f / 2048.0f} },
        agl::vertex_2d{ agl::vector2f{ 0.0f, 0.045f }, agl::color{255, 255, 255, 255 }, agl::vector2f{0.0f, 136.0f / 2048.0f} }
    };

    std::array<uint32_t, 6> m_indizes = {
        0, 1, 2,
        2, 3, 0
    };

    agl::matrix4f m_rect_matrix;

    agl::shader_program m_background_program;
    agl::texture m_test_texture;

    agl::program_layout m_background_program_layout{ m_background_program };
    int32_t m_background_program_time_location = -1;

    agl::clock m_clock;

    agl::font m_test_font;
    agl::text m_test_text;

    float m_delta_time = 0.0;
    float m_elapsed_time = 0.0f;

    bool m_key_left = false;
    bool m_key_right = false;
};
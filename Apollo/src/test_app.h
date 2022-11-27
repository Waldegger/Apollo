#pragma once

#include "engine.h"
#include "graphics/shader_program.h"
#include "graphics/vertex_2d.h"
#include "graphics/matrix4.h"
#include "graphics/texture.h"

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
        agl::vertex_2d{ agl::vector2f{ -1.0f, -1.0f } },
        agl::vertex_2d{ agl::vector2f{ 1.0f, -1.0f } },
        agl::vertex_2d{ agl::vector2f{ 1.0f, 1.0f } },
        agl::vertex_2d{ agl::vector2f{ -1.0f, 1.0f } }
    };

    std::array<agl::vertex_2d, 4> m_rect_vertices =
    {
        agl::vertex_2d{ agl::vector2f{ -0.1f, -0.1f }, agl::color{255, 255, 255, 255 }, agl::vector2f{0.0f, 0.0f} },
        agl::vertex_2d{ agl::vector2f{ 0.1f, -0.1f }, agl::color{255, 255, 255, 255 }, agl::vector2f{1.0f, 0.0f} },
        agl::vertex_2d{ agl::vector2f{ 0.1f, 0.1f }, agl::color{255, 255, 255, 255 }, agl::vector2f{1.0f, 1.0f} },
        agl::vertex_2d{ agl::vector2f{ -0.1f, 0.1f }, agl::color{255, 255, 255, 255 }, agl::vector2f{0.0f, 1.0f} }
    };

    std::array<uint32_t, 6> m_indizes = {
        0, 1, 2,
        2, 3, 0
    };

    agl::matrix4f m_rect_matrix;

    agl::shader_program m_background_program;
    agl::shader_program m_default_program;

    agl::texture m_test_texture;

    uint64_t m_timer_now = SDL_GetPerformanceCounter();
    uint64_t m_timer_last = 0;
    float m_delta_time = 0.0;

    bool m_key_left = false;
    bool m_key_right = false;
};
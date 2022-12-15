#pragma once

#include "engine.h"
#include "graphics/shader_program.h"
#include "graphics/vertex_2d.h"
#include "algebra/matrix4.h"
#include "graphics/texture.h"
#include "graphics/view_2d.h"
#include "graphics/font.h"
#include "graphics/text.h"
#include "system/clock.h"
#include "sound/sound_buffer.h"
#include "sound/sound.h"

class test_app
	: public age::engine
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

    std::array<age::vertex_2d, 4> m_vertices =
    {
        age::vertex_2d{ age::vector2f{ 0.0f, 0.0f } },
        age::vertex_2d{ age::vector2f{ 1.0f, 0.0f } },
        age::vertex_2d{ age::vector2f{ 1.0f, 1.777777f } },
        age::vertex_2d{ age::vector2f{ 0.0f, 1.777777f } }
    };
    
    std::array<age::vertex_2d, 4> m_rect_vertices =
    {
        age::vertex_2d{ age::vector2f{ 0.0f, 0.0f }, age::color{255, 255, 255, 255 }, age::vector2f{0.0f, 109.0f / 2048.0f} },
        age::vertex_2d{ age::vector2f{ 0.18f, 0.0f }, age::color{255, 255, 255, 255 }, age::vector2f{110.0f / 1024.0f, 109.0f / 2048.0f} },
        age::vertex_2d{ age::vector2f{ 0.18f, 0.045f }, age::color{255, 255, 255, 255 }, age::vector2f{110.0f / 1024.0f, 136.0f / 2048.0f} },
        age::vertex_2d{ age::vector2f{ 0.0f, 0.045f }, age::color{255, 255, 255, 255 }, age::vector2f{0.0f, 136.0f / 2048.0f} }
    };

    std::array<uint32_t, 6> m_indizes = {
        0, 1, 2,
        2, 3, 0
    };

    age::matrix4f m_rect_matrix;

    age::shader_program m_background_program;
    age::texture m_test_texture;

    age::program_layout m_background_program_layout{ m_background_program };
    int32_t m_background_program_time_location = -1;

    age::clock m_clock;

    age::font m_test_font;
    age::text m_test_text;

    age::sound_buffer m_test_buffer;
    age::sound m_test_sound;

    float m_delta_time = 0.0;
    float m_elapsed_time = 0.0f;

    bool m_key_left = false;
    bool m_key_right = false;
};
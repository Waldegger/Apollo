#include "test_app.h"

#include <SDL2/SDL.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include "graphics/render_states.h"
#include "graphics/image.h"
#include "graphics/texture.h"
#include "system/assetstream.h"
#include "system/memstream.h"

#include "audio/audio_format.h"
#include "audio/sound_buffer.h"

void test_app::on_create()
{
    age::image test_image;
    test_image.load("./test_data/test.png");

    std::ifstream fs{ "./test_data/test.frag" };
    std::stringstream buffer;

    if (fs)
        buffer << fs.rdbuf();
   
    auto shader1 = age::shader{ age::shader::shader_type::fragment };
    shader1.compile(buffer.str());

    m_background_program.attach_shader(*engine::get_default_vertex_shader());
    m_background_program.attach_shader(shader1);

    m_background_program.bind_attrib_location(A_POSITION_INDEX, "a_position");
    m_background_program.bind_attrib_location(A_COLOR_INDEX, "a_color");
    m_background_program.bind_attrib_location(A_TEX_COORDS_INDEX, "a_tex_coords");

    m_background_program.link();

    m_background_program_layout.mvp_matrix_name("u_mvp_matrix");

    auto window_size = get_render_window().get_size();

    m_background_program.set_uniform("iResolution", static_cast<float>(window_size.x), static_cast<float>(window_size.y));
    m_background_program_time_location = m_background_program.get_uniform_location("iTime");
    
    m_test_texture.load("./test_data/test.png");

    m_rect_matrix.translate(age::vector2f{ 0.5f, -1.5f });

    uint32_t text_size = 64;
    m_test_font.load("./test_data/comic.ttf");
    m_test_font.pre_cache_glyphs(U"I am a wildfire and I can save this thing into a texure also with number 0123456789 some special chars !\"§$%/()=? and don't forget about Zonk or zuzu! Yolo ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", text_size, false, 0.0f);
    auto font_image = m_test_font.get_texture(text_size).copy_to_image();
    font_image.save("./test_data/font_texture.png");

    m_test_text.set_character_size(64);
    m_test_text.set_font(m_test_font);
    m_test_text.set_string(U"Apollo flies way\nhigher\nthan the moon!");
    m_test_text.set_scale(age::vector2f{ (1.0f / static_cast<float>(window_size.x)), (1.0f / static_cast<float>(window_size.x)) });
    m_test_text.set_position(age::vector2f{ -0.9f, 0.0f });


    //Testing memstream
 
    age::assetistream is{ "./test_data/laser.wav", std::ios::binary | std::ios::ate };
    size_t file_size = is.tellg();
    is.seekg(0);
    std::vector<std::byte> file_data{ file_size };
    is.read(reinterpret_cast<char*>(&file_data[0]), file_size);

    age::memistream ms{ &file_data[0], file_data.size() };

    m_test_buffer.load(ms);
    

    //Testing assetstream
    /*
    age::assetistream is{ "./test_data/laser.wav", std::ios::binary };
    m_test_buffer.load(is);
    */


    m_test_sound.set_buffer(&m_test_buffer);

    m_test_music.open("./test_data/track1.ogg");

    m_clock.start();
}

void test_app::on_update()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            stop(0);
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_LEFT:
                m_key_left = true;
                break;
            case SDLK_RIGHT:
                m_key_right = true;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_LEFT:
                    m_key_left = false;
                break;
                case SDLK_RIGHT:
                    m_key_right = false;
                break;
                case SDLK_s:
                    m_test_sound.play();
                break;
                case SDLK_a:
                {
                    age::vector3f sound_pos = m_test_sound.get_position();
                    sound_pos.x -= 1.0f;
                    m_test_sound.update_position(sound_pos);
                }
                break;
                case SDLK_d:
                {
                    age::vector3f sound_pos = m_test_sound.get_position();
                    sound_pos.x += 1.0f;
                    m_test_sound.update_position(sound_pos);
                }
                break;
            }
            break;
        }
    }

    m_delta_time = static_cast<float>(m_clock.restart());
    m_elapsed_time += m_delta_time;
    m_background_program.set_uniform(m_background_program_time_location, m_elapsed_time);

    if (m_key_left)
    {
        age::vector2f dir{ -1.0f, 0.0f };
        dir *= m_delta_time;
        m_rect_matrix.translate(dir);
    }

    if (m_key_right)
    {
        age::vector2f dir{ 1.0f, 0.0f };
        dir *= m_delta_time;
        m_rect_matrix.translate(dir);
    }

    //Clear screen
    get_render_window().clear();

    //Draw Stuff
    get_render_window().draw(m_vertices.data(), m_indizes.data(), m_indizes.size(), age::render_states{ m_background_program_layout, m_test_texture, age::matrix4f::get_identity() });
    get_render_window().draw(m_rect_vertices.data(), m_indizes.data(), m_indizes.size(), age::render_states{ *engine::get_default_program_layout(), m_test_texture, m_rect_matrix});
    get_render_window().draw(m_test_text, age::render_states{});

    //Update screen
    get_render_window().display();
}

void test_app::on_destroy()
{

}
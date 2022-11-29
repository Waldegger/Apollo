#include "test_app.h"

#include <SDL2/SDL.h>

#include <fstream>
#include <sstream>

#include "graphics/render_states.h"
#include "graphics/image.h"
#include "graphics/texture.h"
#include "system/assetstream.h"

void test_app::on_create()
{
    agl::image test_image;
    test_image.load("D:/temp/test.png");

    std::ifstream fs{ "D:\\temp\\test.frag" };
    std::stringstream buffer;

    if (fs)
        buffer << fs.rdbuf();
   
    auto shader1 = agl::shader{ agl::shader::shader_type::fragment };
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
    
    m_test_texture.load("D:/temp/test.png");

    m_rect_matrix.translate(agl::vector2f{ 0.5f, -1.5f });

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
            }
            break;
        }
    }

    m_delta_time = static_cast<float>(m_clock.restart());
    m_elapsed_time += m_delta_time;
    m_background_program.set_uniform(m_background_program_time_location, m_elapsed_time);

    if (m_key_left)
    {
        agl::vector2f dir{ -1.0f, 0.0f };
        dir *= m_delta_time;
        m_rect_matrix.translate(dir);
    }

    if (m_key_right)
    {
        agl::vector2f dir{ 1.0f, 0.0f };
        dir *= m_delta_time;
        m_rect_matrix.translate(dir);
    }

    //Clear screen
    get_render_window().clear();

    //Draw Stuff
    get_render_window().draw(m_vertices.data(), m_indizes.data(), m_indizes.size(), agl::render_states{ m_background_program_layout, m_test_texture, agl::matrix4f::get_identity() });
    get_render_window().draw(m_rect_vertices.data(), m_indizes.data(), m_indizes.size(), agl::render_states{ *engine::get_default_program_layout(), m_test_texture, m_rect_matrix});

    //Update screen
    get_render_window().display();
}

void test_app::on_destroy()
{

}
#include "test_app.h"

#include <fstream>
#include <sstream>

#include "graphics/image.h"
#include "graphics/texture.h"
#include "system/assetstream.h"

void test_app::on_create()
{
    auto shader1 = agl::shader{ agl::shader::type::vertex };
    auto shader2 = agl::shader{ agl::shader::type::fragment };

    agl::image test_image;
    test_image.load("D:/temp/test.png");

    std::ifstream fs{ "D:\\temp\\test.frag" };
    std::stringstream buffer;

    if (fs)
        buffer << fs.rdbuf();
   
    auto shader3 = agl::shader{ agl::shader::type::fragment };
    shader3.compile(buffer.str());

    const char* vertex_shader_source = 
        "uniform mat4 u_mvp_matrix;\n"
        "attribute vec2 a_position;\n"
        "attribute vec4 a_color;\n"
        "attribute vec2 a_tex_coords;\n"
        "varying vec4 v_color;\n"
        "varying vec2 v_tex_coords;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = u_mvp_matrix * vec4(a_position, 0.0, 1.0);\n"
        "   v_color = a_color;\n"
        "   v_tex_coords = a_tex_coords;\n"
        "}";

    const char* fragment_shader_source = 
        "precision mediump float;\n"
        "uniform sampler2D u_texture;\n"
        "varying vec4 v_color;\n"
        "varying vec2 v_tex_coords;\n"
        "void main()\n"
        "{\n"
        "    //gl_FragColor = v_color;\n"
        "    gl_FragColor = v_color * texture2D(u_texture, v_tex_coords);\n"
        "}";

    shader1.compile(vertex_shader_source);
    shader2.compile(fragment_shader_source);

    m_background_program.attach_shader(shader1);
    m_background_program.attach_shader(shader3);

    m_background_program.bind_attrib_location(A_POSITION_INDEX, "a_position");
    m_background_program.bind_attrib_location(A_COLOR_INDEX, "a_color");
    m_background_program.bind_attrib_location(A_TEX_COORDS_INDEX, "a_tex_coords");

    m_background_program.link();

    auto window_size = get_render_window().get_size();

    m_background_program.set_uniform("iResolution", static_cast<float>(window_size.x), static_cast<float>(window_size.y));
    
    m_default_program.attach_shader(shader1);
    m_default_program.attach_shader(shader2);

    m_default_program.bind_attrib_location(A_POSITION_INDEX, "a_position");
    m_default_program.bind_attrib_location(A_COLOR_INDEX, "a_color");
    m_default_program.bind_attrib_location(A_TEX_COORDS_INDEX, "a_tex_coords");

    m_default_program.link();

    m_test_texture.load("D:/temp/test.png");

    m_rect_matrix.translate(agl::vector2f{ 0.5f, -1.5f });
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

    m_timer_last = m_timer_now;
    m_timer_now = SDL_GetPerformanceCounter();

    m_delta_time = (float)((m_timer_now - m_timer_last) / (float)SDL_GetPerformanceFrequency());
    m_elapsed_time += m_delta_time;
    m_background_program.set_uniform("iTime", m_elapsed_time);

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
    get_render_window().draw(m_vertices.data(), m_indizes.data(), m_indizes.size(), agl::render_states{ m_background_program, m_test_texture, agl::matrix4f::get_identity() });
    get_render_window().draw(m_rect_vertices.data(), m_indizes.data(), m_indizes.size(), agl::render_states{ m_default_program, m_test_texture, m_rect_matrix });

    //Update screen
    get_render_window().display();
}

void test_app::on_destroy()
{

}
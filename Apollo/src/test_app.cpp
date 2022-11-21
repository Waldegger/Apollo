#include "test_app.h"

#include <fstream>
#include <sstream>

void test_app::on_create()
{
    static constexpr uint32_t VPOSITION_INDEX = 0;

    auto shader1 = agl::shader{ agl::shader::type::vertex };
    auto shader2 = agl::shader{ agl::shader::type::fragment };

    std::ifstream fs{ "D:/temp/test.frag" };
    std::stringstream buffer;
    buffer << fs.rdbuf();

    auto shader3 = agl::shader{ agl::shader::type::fragment };
    shader3.compile(buffer.str());

    const char* vertexShaderSource = "attribute vec4 vPosition; \n"
        "void main() \n"
        "{ \n"
        "   gl_Position = vPosition; \n"
        "} \n";

    const char* fragmentShaderSource = "precision mediump float; \n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
        "}";

    shader1.compile(vertexShaderSource);
    shader2.compile(fragmentShaderSource);

    m_test_program.attach_shader(shader1);
    //program.attach_shader(shader2);
    m_test_program.attach_shader(shader3);

    m_test_program.bind_attrib_location(VPOSITION_INDEX, "vPosition");

    m_test_program.link();
    m_test_program.bind();

    auto window_size = get_render_window().get_size();

    m_test_program.set_uniform("iResolution", static_cast<float>(window_size.x), static_cast<float>(window_size.y));
    //m_test_program.set_uniform("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
    m_test_program.set_uniform("iTime", 0.0f);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices.data());
    glEnableVertexAttribArray(VPOSITION_INDEX);
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
        }
    }

    //Here comes rendercode
    /* Clear the color and depth buffers. */
    get_render_window().clear();

    //Lets draw our test triangle
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indizes.size()), GL_UNSIGNED_INT, m_indizes.data());

    //Update screen
    get_render_window().display();
}

void test_app::on_destroy()
{

}
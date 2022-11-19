#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include "graphics/render_window.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <sstream>

#include "graphics/vector2.h"
#include "graphics/matrix4.h"
#include "graphics/shader_program.h"
#include "system/assetstream.h"

int main(int argc, char* argv[])
{
	static constexpr int SCREEN_WIDTH = 540;
	static constexpr int SCREEN_HEIGHT = 960;

    static constexpr uint32_t VPOSITION_INDEX = 0;

    try
    {
        agl::render_window rwindow{ "Apollo flies to the moon!", SCREEN_WIDTH, SCREEN_HEIGHT, 0 };
      
        agl::vector2f vec;
        agl::matrix4f mat1, mat2;

        mat1.translate(agl::vector2f{ 4.0f, 3.0f }).scale(agl::vector2f{ 2.0f, 2.0f });
        mat2.scale(agl::vector2f{ 0.4f, 0.1f }).combine(mat1);

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

        agl::shader_program program;
        program.attach_shader(shader1);
        //program.attach_shader(shader2);
        program.attach_shader(shader3);

        program.bind_attrib_location(VPOSITION_INDEX, "vPosition");

        program.link();
        program.bind();

        program.set_uniform("iResolution", static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
        //program.set_uniform("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
        program.set_uniform("iTime", 0.0f);

        std::array<float, 12> vertices = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f
        };

        std::array<uint32_t, 6> indizes = {
            0, 1, 2,
            2, 3, 0
        };

        // Load the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
        glEnableVertexAttribArray(VPOSITION_INDEX);

        bool keep_window_open = true;
        while (rwindow.is_open())
        {
            SDL_Event e;

            while (SDL_PollEvent(&e))
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    rwindow.close();
                    break;
                }
            }

            //Here comes rendercode
            /* Clear the color and depth buffers. */
            rwindow.clear();
            
            //Lets draw our test triangle
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, indizes.size(), GL_UNSIGNED_INT, indizes.data());
           

            //Update screen
            rwindow.display();
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }

	return EXIT_SUCCESS;
}
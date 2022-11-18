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
        apollo::render_window rwindow{ "Apollo flies to the moon!", SCREEN_WIDTH, SCREEN_HEIGHT, 0 };
      
        apollo::vector2f vec;
        apollo::matrix4f mat1, mat2;

        mat1.translate(apollo::vector2f{ 4.0f, 3.0f }).scale(apollo::vector2f{ 2.0f, 2.0f });
        mat2.scale(apollo::vector2f{ 0.4f, 0.1f }).combine(mat1);

        auto shader1 = apollo::shader{ apollo::shader::type::vertex };
        auto shader2 = apollo::shader{ apollo::shader::type::fragment };

        std::ifstream fs{ "D:/temp/test.frag" };
        std::stringstream buffer;
        buffer << fs.rdbuf();

        auto shader3 = apollo::shader{ apollo::shader::type::fragment };
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

        apollo::shader_program program;
        program.attach_shader(shader1);
        program.attach_shader(shader2);
        //program.attach_shader(shader3);

        program.bind_attrib_location(VPOSITION_INDEX, "vPosition");

        program.link();
        program.use();

        program.set_uniform("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
        program.set_uniform("iTime", 8.0f);

        float vertices[] = {
            -0.5f, -0.5f, 0.9f,
             0.5f, -0.5f, 0.9f,
             0.5f,  0.5f, 0.9f,
            -0.5f,  0.5f, -0.9f
        };

        unsigned int indizes[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Load the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        
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
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indizes);
           

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
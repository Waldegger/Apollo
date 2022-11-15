#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include "graphics/render_window.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "graphics/vector2.h"
#include "graphics/matrix4.h"
#include "graphics/shader_program.h"
#include "system/assetstream.h"

int main(int argc, char* argv[])
{
	static constexpr int SCREEN_WIDTH = 540;
	static constexpr int SCREEN_HEIGHT = 960;

    try
    {
        apollo::render_window rwindow{ "Apollo flies to the moon!", SCREEN_WIDTH, SCREEN_HEIGHT, 0 };
      
        GLuint vbo;
       
        glGenBuffers(1, &vbo);

        apollo::vector2f vec;
        apollo::matrix4f mat1, mat2;

        mat1.translate(apollo::vector2f{ 4.0f, 3.0f }).scale(apollo::vector2f{ 2.0f, 2.0f });
        mat2.scale(apollo::vector2f{ 0.4f, 0.1f }).combine(mat1);

        auto shader1 = apollo::shader{ apollo::shader::type::vertex };
        auto shader2 = apollo::shader{ apollo::shader::type::fragment };

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n";

        shader1.compile(vertexShaderSource);
        shader2.compile(fragmentShaderSource);

        apollo::shader_program program;
        program.attach_shader(shader1);
        program.attach_shader(shader2);

        program.link();
        program.use();

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

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
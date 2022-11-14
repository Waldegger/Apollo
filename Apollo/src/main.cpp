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
	static constexpr int SCREEN_WIDTH = 640;
	static constexpr int SCREEN_HEIGHT = 480;

    try
    {
        apollo::render_window rwindow{ "Apollo flies to the moon!", SCREEN_WIDTH, SCREEN_HEIGHT, 0 };
      
        GLuint vbo;
       
        glGenBuffers(1, &vbo);

        apollo::vector2f vec;
        apollo::matrix4f mat1, mat2;

        mat1.translate(apollo::vector2f{ 4.0f, 3.0f }).scale(apollo::vector2f{ 2.0f, 2.0f });
        mat2.scale(apollo::vector2f{ 0.4f, 0.1f }).combine(mat1);

        auto shader1 = apollo::shader{};
        auto shader2 = apollo::shader{};
        apollo::shader_program program;
        program.link({ shader1, shader2 });

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
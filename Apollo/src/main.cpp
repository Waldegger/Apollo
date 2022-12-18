#include <iostream>

#include "test_app.h"

int main(int argc, char* argv[])
{
	static constexpr uint32_t SCREEN_WIDTH = 540;
	static constexpr uint32_t SCREEN_HEIGHT = 960;
    
    try
    {
        test_app app;

        return app.start("Apollo goes outer space", 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;

        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Unknown exception happened" << std::endl;

        return EXIT_FAILURE;
    }
}
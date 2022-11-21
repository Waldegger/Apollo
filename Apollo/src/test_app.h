#pragma once

#include "engine.h"
#include "graphics/shader_program.h"

class test_app
	: public agl::engine
{
public:

public:

protected:
	virtual void on_create() override;
	virtual void on_update() override;
	virtual void on_destroy() override;

private:
    std::array<float, 12> m_vertices = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f
    };

    std::array<uint32_t, 6> m_indizes = {
        0, 1, 2,
        2, 3, 0
    };

	agl::shader_program m_test_program;
};
#include "system/transient_context_lock.h"

#include <thread>
#include <mutex>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <glad/glad.h>

#include "engine.h"
#include "graphics/render_window.h"

namespace age
{
    transient_context_lock::transient_context_lock()
    {
        if (s_thread_context)
        {
            s_thread_context->set_active(true);
            return;
        }

        auto& render_window = engine::get_instance()->get_render_window();
        s_thread_context = render_window.get_context().acquire_shared_context();

        s_thread_context->set_active(true);
    }

    transient_context_lock::~transient_context_lock()
    {
        if (s_thread_context)
        {
            s_thread_context->flush();
            s_thread_context->set_active(false);
        }
    }

    void transient_context_lock::flush()
    {
        if (s_thread_context)
            s_thread_context->flush();
    }

    transient_context_lock::thread_storage_cleaner::~thread_storage_cleaner()
    {
        SDL_TLSCleanup();
    }
}


#include "system/transient_context_lock.h"

#include <thread>
#include <mutex>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <gl/glew.h>

#include "engine.h"
#include "graphics/render_window.h"

age::transient_context_lock::transient_context_lock()
{
    if (nullptr != SDL_GL_GetCurrentContext()) {
        // Context already current in this thread; no action needed
        return;
    }

    m_window = age::engine::get_instance()->get_render_window().m_windowhandle.get();
    m_main_context = age::engine::get_instance()->get_render_window().get_shared_context();

    if (m_window == nullptr || m_main_context == nullptr)
    {
        throw std::runtime_error(std::string("No window or main_context. Is Engine Initialized?"));
    }

    static std::mutex s_mutex;
    std::lock_guard lock(s_mutex);

    if (!s_thread_context)
    {
        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window), m_main_context);

        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        s_thread_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(m_window));
        if (!s_thread_context)
        {
            throw std::runtime_error(std::string("Failed to create SDL GL context") + SDL_GetError());
        }

        return;
    }

    if (SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window), static_cast<SDL_GLContext>(s_thread_context)) != 0)
    {
        throw std::runtime_error(std::string("SDL_GL_MakeCurrent failed") + SDL_GetError());
    }

    m_owns_context = true;
}

age::transient_context_lock::~transient_context_lock()
{
    glFlush();

    if (m_owns_context)
    {
        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window), nullptr);
    }
}

age::transient_context_lock& age::transient_context_lock::operator=(transient_context_lock &&right) noexcept
{
    if (this == &right)
        return *this;

    this->m_window = right.m_window;
    this->m_owns_context = right.m_owns_context;

    right.m_window = nullptr;
    right.m_owns_context = false;

    return *this;
}

age::transient_context_lock::transient_context_lock(transient_context_lock &&other) noexcept
    : m_window(std::exchange(other.m_window, nullptr))
    , m_owns_context(std::exchange(other.m_owns_context, false))
{
}


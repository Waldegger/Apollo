#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <thread>

#include "../utility/utility.h"

namespace age
{
    class render_window;

    class context
    {
        friend class render_window;
        friend class transient_context_lock;

    public:

    private:
        context() = default;

    public:
        void set_active(bool value);
        bool is_active() const;

        void flush() const;

    protected:

    private:
        static void delete_context_lib(void* context);
        void reset_GL_state() const;

        void create(const render_window& window, bool shared = false);
        std::shared_ptr<context> acquire_shared_context();

        std::mutex m_acquire_context_mutex;
        std::vector<std::shared_ptr<context>> m_shared_context_list;

        const render_window* m_render_window{};

        using gl_context_type = void;
        using gl_context_deleter = deleter<gl_context_type, delete_context_lib>;
        std::unique_ptr<gl_context_type, gl_context_deleter> m_GL_context;
        std::unique_ptr<gl_context_type, gl_context_deleter> m_GL_shared_context;
    };
}
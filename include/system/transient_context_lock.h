#pragma once

#include <memory>

#include "graphics/context.h"

namespace age
{
    class render_window;

    class transient_context_lock
    {
    public:
        transient_context_lock();
        ~transient_context_lock();

        transient_context_lock(const transient_context_lock&) = delete;
        transient_context_lock& operator=(const transient_context_lock&) = delete;

        static void flush();

    public:

    protected:

    private:
        class thread_storage_cleaner
        {
            public:
            ~thread_storage_cleaner();
        };

        static inline thread_local std::shared_ptr<context> s_thread_context{nullptr};
        static inline thread_local thread_storage_cleaner s_thread_storage_cleaner{};
    };
}
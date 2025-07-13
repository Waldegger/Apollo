#pragma once

#include <thread>
#include <mutex>
#include <unordered_map>

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

        transient_context_lock& operator = (transient_context_lock&& right) noexcept;
        transient_context_lock(transient_context_lock&& other) noexcept;

    public:

    protected:

    private:
        void* m_window{nullptr};
        void* m_main_context{nullptr};

        void* m_context{nullptr};

        bool m_owns_context{false};

        static inline std::unordered_map<std::thread::id, void*> s_thread_contexts;
    };
}
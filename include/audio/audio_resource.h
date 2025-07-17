#pragma once

#include <atomic>
#include <cstddef>

namespace age
{
    class audio_resource
    {
    public:
        audio_resource();
        virtual ~audio_resource();
    public:

    protected:

    private:
        inline static std::atomic<size_t> m_num_resources{0};
    };
}
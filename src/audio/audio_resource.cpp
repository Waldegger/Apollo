#include "audio/audio_resource.h"
#include "audio/audio_device.h"

namespace age
{
    audio_resource::audio_resource()
    {
        ++m_num_resources;

        if (m_num_resources == 1)
        {
            if (!audio_device::get().is_initialised())
                audio_device::get().init();
        }
    }

    audio_resource::~audio_resource()
    {
        --m_num_resources;

        if (!m_num_resources)
        {
            if (audio_device::get().is_initialised())
                audio_device::get().destroy();
        }
    }
}

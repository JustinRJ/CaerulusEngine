#pragma once

#include <thread>

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class DeltaTimer : public Timer
        {
        public:
            float Delta(float frameLimit)
            {
                using namespace std::chrono;

                std::chrono::high_resolution_clock::time_point now;

                if (frameLimit > 0.0f)
                {
                    // Target next frame time
                    m_nextFrame += duration_cast<high_resolution_clock::duration>(duration<float>(frameLimit));

                    // Sleep until that time
                    std::this_thread::sleep_until(m_nextFrame);
                }

                now = high_resolution_clock::now();
                m_frameTime = duration_cast<duration<float>>(now - m_last).count();
                m_last = now;

                m_counter++;
                auto dif = duration_cast<duration<float>>(now - m_fpsTimer).count();
                if (dif > 1.0f)
                {
                    m_fps = static_cast<uint32_t>(m_counter / dif);
                    m_counter = 0;
                    m_fpsTimer = now;
                }

                return m_frameTime;
            }

            uint32_t GetFPS() const
            {
                return m_fps;
            }

        private:
            uint32_t m_fps = 0;
            uint32_t m_counter = 0;
            float m_frameTime = 0.0f;
            std::chrono::high_resolution_clock::time_point m_last = std::chrono::high_resolution_clock::now();
            std::chrono::high_resolution_clock::time_point m_nextFrame = m_last;
            std::chrono::high_resolution_clock::time_point m_fpsTimer = m_last;
        };

    }
}

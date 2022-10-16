#pragma once

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class FPSLimiter : public Timer
        {
        public:
            float Delta(float frameLimit)
            {
                using namespace std::chrono;

                m_counter++;
                m_frameTime = Timer::Delta();

                auto now = high_resolution_clock::now();
                auto dif = duration_cast<duration<float>>(now - m_fpsTimer).count();
                if (dif > 1)
                {
                    m_fps = m_counter / dif;
                    m_counter = 0;
                    m_fpsTimer = now;
                }

                if (frameLimit > 0.0f &&
                    m_frameTime > 0.0f &&
                    m_frameTime < frameLimit)
                {
                    // Sleep for time left for frame
                    Sleep(static_cast<time_t>(static_cast<double>(frameLimit) - static_cast<double>(m_frameTime)));
                    // Get time taken when sleeping
                    m_frameTime += Timer::Delta();
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
            std::chrono::high_resolution_clock::time_point m_fpsTimer = std::chrono::high_resolution_clock::now();
        };
    }
}

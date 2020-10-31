#pragma once

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class FPSLimiter : public Timer
        {
        public:
            FPSLimiter() = default;
            virtual ~FPSLimiter() = default;

            float Delta(float frameLimit)
            {
                m_frameTime = Timer::Delta();
                if (frameLimit > 0.0f &&
                    m_frameTime > 0.0f &&
                    m_frameTime < frameLimit)
                {
                    // Sleep for time left for frame
                    Sleep(static_cast<time_t>(frameLimit - m_frameTime));
                    // Get time taken when sleeping
                    m_frameTime += Timer::Delta();
                }
                return m_frameTime;
            }

        private:
            float m_frameTime = 0.0f;
        };
    }
}

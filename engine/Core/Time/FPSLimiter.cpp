#include "stdafx.h"

#include "FPSLimiter.h"

namespace Core
{
    namespace Time
    {
        float FPSLimiter::Delta(float frameLimit)
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
    }
}

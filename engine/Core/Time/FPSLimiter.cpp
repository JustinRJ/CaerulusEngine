#include "stdafx.h"

#include "FPSLimiter.h"

namespace Core
{
    namespace Time
    {
        FPSLimiter::FPSLimiter() :
            m_FrameTime(0.0f)
        {
        }

        FPSLimiter::~FPSLimiter() 
        { 
        }

        float FPSLimiter::Delta(float frameLimit)
        {
            m_FrameTime = Timer::Delta();

            if (frameLimit > 0.0f &&
                m_FrameTime > 0.0f &&
                m_FrameTime < frameLimit)
            {
                // Sleep for time left for frame
                Sleep(static_cast<time_t>(frameLimit - m_FrameTime));
                // Get time taken when sleeping
                m_FrameTime += Timer::Delta();
            }
            return m_FrameTime;
        }
    }
}

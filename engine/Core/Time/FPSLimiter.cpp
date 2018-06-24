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

        float FPSLimiter::Delta()
        {
            return Timer::Delta();
        }

        float FPSLimiter::Delta(const float frameLimit)
        {
            m_FrameTime = Delta();

            if (frameLimit > 0.0f &&
                m_FrameTime > 0.0f &&
                m_FrameTime < frameLimit)
            {
                // Sleep for time left for frame
                Sleep(static_cast<time_t>(frameLimit - m_FrameTime));
                // Get time taken when sleeping
                m_FrameTime += Delta();
            }
            return m_FrameTime;
        }
    }
}

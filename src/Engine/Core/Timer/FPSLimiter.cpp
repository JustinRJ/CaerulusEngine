#include "stdafx.h"

#include "FPSLimiter.h"


namespace Core
{
    namespace Timer
    {
        FPSLimiter::FPSLimiter() :
            m_FrameTime(0.0f)
        {
        }

        FPSLimiter::~FPSLimiter() 
        { 
        }

        Time FPSLimiter::Delta()
        {
            return Timer::Delta();
        }

        Time FPSLimiter::Delta(const Time frameLimit)
        {
            m_FrameTime = Delta();

            if (frameLimit > 0.0f && m_FrameTime > 0.0f && m_FrameTime < frameLimit)
            {
                Sleep(frameLimit - m_FrameTime); // Sleep for time left for frame
                m_FrameTime += Delta(); // Get time taken when sleeping
            }
            return m_FrameTime;
        }
    }
}

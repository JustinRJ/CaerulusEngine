#include "stdafx.h"

#include "FixedLimiter.h"

namespace Core
{
    namespace Time
    {
        FixedLimiter::FixedLimiter() :
            m_AccumulatedTime(0.0f)
        {
        }

        FixedLimiter::~FixedLimiter()
        {
        }

        float FixedLimiter::Fixed(float fixed)
        {
            m_AccumulatedTime += Timer::Delta();
            unsigned int count = 0;
            while (m_AccumulatedTime >= fixed)
            {
                m_AccumulatedTime -= fixed;
                count++;
            }

            return fixed * count;
        }
    }
}

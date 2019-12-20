#include "stdafx.h"

#include "FixedTimer.h"

namespace Core
{
    namespace Time
    {
        FixedTimer::FixedTimer() :
            m_AccumulatedTime(0.0f)
        {
        }

        float FixedTimer::Fixed(float fixed)
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

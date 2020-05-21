#include "stdafx.h"

#include "FixedTimer.h"

namespace Core
{
    namespace Time
    {
        float FixedTimer::Fixed(float fixed)
        {
            m_accumulatedTime += Timer::Delta();
            unsigned int count = 0;
            while (m_accumulatedTime >= fixed)
            {
                m_accumulatedTime -= fixed;
                count++;
            }

            return fixed * count;
        }
    }
}

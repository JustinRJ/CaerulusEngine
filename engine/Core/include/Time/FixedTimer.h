#pragma once

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class FixedTimer : public Timer
        {
        public:
            FixedTimer() = default;

            float Fixed(float fixed)
            {
                m_accumulatedTime += Timer::Delta();
                uint32_t count = 0;
                while (m_accumulatedTime >= fixed)
                {
                    m_accumulatedTime -= fixed;
                    count++;
                }
                return fixed * count;
            }

            float GetInterpolant(float fixed) const
            {
                return m_accumulatedTime / fixed;
            }

        private:
            float m_accumulatedTime = 0.0f;
        };
    }
}

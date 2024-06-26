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

        private:
            float m_accumulatedTime = 0.0f;
        };
    }
}

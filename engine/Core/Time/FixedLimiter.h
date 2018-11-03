#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class FixedLimiter : public Timer
        {
        public:

            CAERULUS_CORE FixedLimiter();
            CAERULUS_CORE virtual ~FixedLimiter();

            CAERULUS_CORE virtual float Fixed(float fixed);

        private:

            float m_AccumulatedTime;
        };
    }
}

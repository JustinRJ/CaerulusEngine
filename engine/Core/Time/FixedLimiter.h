#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class CAERULUS_CORE FixedLimiter : public Timer
        {
        public:

            FixedLimiter();
            virtual ~FixedLimiter();

            virtual float Fixed(float fixed);

        private:

            float m_AccumulatedTime;
        };
    }
}

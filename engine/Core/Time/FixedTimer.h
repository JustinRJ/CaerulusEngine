#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class CAERULUS_CORE FixedTimer : public Timer
        {
        public:

            FixedTimer() = default;
            virtual ~FixedTimer() = default;

            float Fixed(float fixed);

        private:

            float m_accumulatedTime = 0.0f;
        };
    }
}

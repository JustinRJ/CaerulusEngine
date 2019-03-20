#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class CAERULUS_CORE FPSLimiter : public Timer
        {
        public:

            FPSLimiter();
            virtual ~FPSLimiter();

            virtual float Delta(float frameLimit);

        private:

            float m_FrameTime;
        };
    }
}

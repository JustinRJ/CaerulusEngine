#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Timer
    {
        class FPSLimiter : public Timer
        {
        public:

            CAERULUS_CORE FPSLimiter();
            CAERULUS_CORE ~FPSLimiter();

            CAERULUS_CORE virtual float Delta();
            CAERULUS_CORE virtual float Delta(const float frameLimit);

        private:

            float m_FrameTime;
            float m_PreviousFrameTime;
        };
    }
}

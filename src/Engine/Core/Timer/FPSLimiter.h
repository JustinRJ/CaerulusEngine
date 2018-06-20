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

            CAERULUS_CORE virtual ~FPSLimiter();

            CAERULUS_CORE virtual Time Delta();

            CAERULUS_CORE virtual Time Delta(const Time frameLimit);

        private:

            Time m_FrameTime;
            Time m_PreviousFrameTime;
        };
    }
}

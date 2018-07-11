#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Timer.h"

namespace Core
{
    namespace Time
    {
        class FPSLimiter : public Timer
        {
        public:

            CAERULUS_CORE FPSLimiter();
            CAERULUS_CORE virtual ~FPSLimiter();

            CAERULUS_CORE virtual float Delta() override;
            CAERULUS_CORE virtual float Delta(const float frameLimit);

        private:

            float m_FrameTime;
        };
    }
}

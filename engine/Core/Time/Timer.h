#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Time.h"

namespace Core
{
    namespace Time
    {
        CAERULUS_CORE void Sleep(Time sec);
        CAERULUS_CORE void Sleep(time_t sec);
        CAERULUS_CORE void SleepMilli(time_t milli);

        class CAERULUS_CORE Timer
        {
        public:

            Timer(bool start = false);
            virtual ~Timer() = default;

            virtual void Start() final;
            virtual void Stop() final;

            virtual float Total() final;
            virtual float Delta() final;

            Time StartTime;
            Time EndTime;

        private:

            time_t NanoTime();
            time_t Frequency();

            bool m_running;
            float m_deltaTime;
            float m_multiplier;
            time_t m_frequency;
            time_t m_timeLastFrame;
        };
    }
}

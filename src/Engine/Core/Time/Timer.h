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

        class Timer
        {
        public:

            CAERULUS_CORE Timer(bool start = false);
            CAERULUS_CORE virtual ~Timer();

            CAERULUS_CORE virtual void Start();
            CAERULUS_CORE virtual void Stop();

            CAERULUS_CORE virtual float Total();
            CAERULUS_CORE virtual float Delta();

            Time StartTime;
            Time EndTime;

        protected:

            CAERULUS_CORE time_t NanoTime();
            CAERULUS_CORE time_t Frequency();

            bool m_Running;
            float m_DeltaTime;
            float m_Multiplier;
            time_t m_Frequency;
            time_t m_TimeLastFrame;
        };
    }
}

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
            virtual ~Timer();

            virtual void Start();
            virtual void Stop();

            virtual float Total();
            virtual float Delta();

            Time StartTime;
            Time EndTime;

        private:

            time_t NanoTime();
            time_t Frequency();

            bool m_Running;
            float m_DeltaTime;
            float m_Multiplier;
            time_t m_Frequency;
            time_t m_TimeLastFrame;
        };
    }
}

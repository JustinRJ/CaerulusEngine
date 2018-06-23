#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <string>
#include <ctime>

namespace Core
{
    namespace Timer
    {
        typedef float Time;
        typedef time_t TimeUnits;

        const float MILLISECOND = 1.0f / 1000000.0f;

        void Sleep(const Time sec);
        void Sleep(const TimeUnits sec);
        void SleepMilli(const TimeUnits milli);

        class TimeValue
        {
        public:

            CAERULUS_CORE TimeValue(bool update = true);

            CAERULUS_CORE virtual ~TimeValue();

            CAERULUS_CORE virtual void Update();

            CAERULUS_CORE virtual std::string Format(const char* format = "%H:%M:%S", size_t extralength = 50) const;

        protected:

            TimeUnits m_Time;
            struct tm m_TimeLocal;
        };

        class Timer
        {
        public:

            CAERULUS_CORE Timer(bool start = false);
            CAERULUS_CORE virtual ~Timer();
            CAERULUS_CORE virtual Time Total();
            CAERULUS_CORE virtual Time Delta();
            CAERULUS_CORE virtual void Start();
            CAERULUS_CORE virtual void Stop();

            TimeValue StartTime;
            TimeValue EndTime;

        protected:

            CAERULUS_CORE TimeUnits NanoTime();
            CAERULUS_CORE TimeUnits Frequency();

            bool m_Running;
            Time m_DeltaTime;
            Time m_Multiplier;
            TimeUnits m_Frequency;
            TimeUnits m_TimeLastFrame;
        };
    }
}

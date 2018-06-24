#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <ctime>
#include <string>

namespace Core
{
    namespace Timer
    {
        const double MILLISECOND = 1.0 / 1000000.0;

        CAERULUS_CORE std::string GetLocalTime(const std::string& format);

        class Time
        {
        public:

            CAERULUS_CORE Time(bool update = true);
            CAERULUS_CORE ~Time();

            CAERULUS_CORE virtual void Update();
            CAERULUS_CORE virtual void Increment(time_t time);
            CAERULUS_CORE virtual time_t GetTime() const;
            CAERULUS_CORE std::string GetFormat(const std::string&, size_t extralength = 0) const;

        protected:
            time_t m_Time;
        };
    }
}
#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <ctime>
#include <string>

namespace Core
{
    namespace Time
    {
        const double MILLISECOND = 1.0 / 1000000.0;

        CAERULUS_CORE std::string GetLocalTime(const std::string& format);
        CAERULUS_CORE std::string GetTime(const std::string&, size_t extralength = 20);

        class Time
        {
        public:

            CAERULUS_CORE Time(bool update = true);
            CAERULUS_CORE virtual ~Time();

            CAERULUS_CORE virtual void Update();
            CAERULUS_CORE virtual void Increment(time_t time);
            CAERULUS_CORE virtual time_t GetTime() const;

        private:
            time_t m_Time;
        };
    }
}
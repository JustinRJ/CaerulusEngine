#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <ctime>
#include <string>

namespace Core
{
    namespace Time
    {
        CAERULUS_CORE const double MILLISECOND = 1.0 / 1000000.0;

        std::string GetLocalTime(const std::string& format);
        std::string GetTime(const std::string&, size_t extralength = 20);

        class CAERULUS_CORE Time
        {
        public:

            Time(bool update = true);
            virtual ~Time();

            virtual void Update();
            virtual void Increment(time_t time);
            virtual time_t GetTime() const;

        private:
            time_t m_Time;
        };
    }
}
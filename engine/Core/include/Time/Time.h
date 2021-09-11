#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <ctime>
#include <string>

namespace Core
{
    namespace Time
    {
        class CAERULUS_CORE Time
        {
        public:
            Time(bool update = true);

            void Update();
            void Increment(time_t time);
            time_t GetTime() const;

            static std::string GetTime(const std::string& format);
            static std::string GetLocalTime(const std::string& format);

        private:
            time_t m_time;
        };
    }
}
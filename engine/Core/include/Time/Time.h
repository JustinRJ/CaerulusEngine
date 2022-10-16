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

            static std::string GetTime(std::string_view format);
            static std::string GetLocalTime(std::string_view format);

        private:
            time_t m_time;
        };
    }
}